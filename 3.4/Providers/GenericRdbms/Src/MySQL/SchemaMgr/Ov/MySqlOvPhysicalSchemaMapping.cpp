/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "stdafx.h"
#include <Rdbms/Override/MySQL/MySqlOvPhysicalSchemaMapping.h>
//TODO: need to get provider name from provider-specific header.
#include "../../Fdo/FdoRdbmsMySqlProviderInfo.h" // contains the provider name

FdoMySQLOvPhysicalSchemaMapping* FdoMySQLOvPhysicalSchemaMapping::Create()
{
    return new FdoMySQLOvPhysicalSchemaMapping();
}

FdoMySQLOvPhysicalSchemaMapping* FdoMySQLOvPhysicalSchemaMapping::Create(FdoString* name)
{
    return new FdoMySQLOvPhysicalSchemaMapping(name);
}

FdoMySQLOvPhysicalSchemaMapping::FdoMySQLOvPhysicalSchemaMapping()
{
    Init();
}

FdoMySQLOvPhysicalSchemaMapping::FdoMySQLOvPhysicalSchemaMapping(FdoString* name) : FdoRdbmsOvPhysicalSchemaMapping(name)
{
    Init();
}

FdoMySQLOvPhysicalSchemaMapping::~FdoMySQLOvPhysicalSchemaMapping()
{
}

FdoMySQLOvClassCollection* FdoMySQLOvPhysicalSchemaMapping::GetClasses()
{
    return FDO_SAFE_ADDREF( (FdoMySQLOvClassCollection*) mClasses );
}


FdoString *FdoMySQLOvPhysicalSchemaMapping::GetDataDirectory()
{
    return mDataDirectory;
}

void FdoMySQLOvPhysicalSchemaMapping::SetDataDirectory(FdoString *dataDirectory)
{
    mDataDirectory = dataDirectory;
}

FdoString *FdoMySQLOvPhysicalSchemaMapping::GetIndexDirectory()
{
    return mIndexDirectory;
}

void FdoMySQLOvPhysicalSchemaMapping::SetIndexDirectory(FdoString *indexDirectory)
{
    mIndexDirectory = indexDirectory;
}

MySQLOvStorageEngineType FdoMySQLOvPhysicalSchemaMapping::GetStorageEngine()
{
    return mStorageEngine;
}

void FdoMySQLOvPhysicalSchemaMapping::SetStorageEngine(MySQLOvStorageEngineType storageEngine)
{
    mStorageEngine = storageEngine;
}

FdoString *FdoMySQLOvPhysicalSchemaMapping::GetDatabase()
{
    return mDatabase;
}

void FdoMySQLOvPhysicalSchemaMapping::SetDatabase(FdoString *database)
{
    mDatabase = database;
}

FdoString* FdoMySQLOvPhysicalSchemaMapping::GetProvider(void)
{
    return RDBMS_MYSQL_PROVIDER_NAME;
}

void FdoMySQLOvPhysicalSchemaMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvPhysicalSchemaMapping::InitFromXml( pContext, attrs );

    FdoXmlAttributeP att = attrs->FindItem( L"database" );
    if ( att ) {
        mDatabase = att->GetValue();
    }

    att = attrs->FindItem( L"dataDirectory" );
    if ( att ) {
        mDataDirectory = att->GetValue();
    }

    att = attrs->FindItem( L"indexDirectory" );
    if ( att ) {
        mIndexDirectory = att->GetValue();
    }

    att = attrs->FindItem( L"storageEngine" );
    if ( att ) {
        mStorageEngine = StorageEngine_StringToEnum(att->GetValue(), pContext);
    }
}

void FdoMySQLOvPhysicalSchemaMapping::_writeXmlAttributes(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    xmlWriter->WriteAttribute( L"xmlns", L"http://fdomysql.osgeo.org/schemas" );

    if ( mDatabase.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"database", mDatabase );

    if ( mDataDirectory.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"dataDirectory", mDataDirectory );

    if ( mIndexDirectory.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"indexDirectory", mIndexDirectory );

    if ( mStorageEngine != MySQLOvStorageEngineType_Default)
        xmlWriter->WriteAttribute( L"storageEngine", StorageEngine_EnumToString(mStorageEngine) );
}

FdoRdbmsOvClassDefinition* FdoMySQLOvPhysicalSchemaMapping::CreateClass( 
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* atts
)
{
    FdoMySQLOvClassDefinition* pClass = FdoMySQLOvClassDefinition::Create();
    pClass->InitFromXml( context, atts );

    return pClass;
}
                
void FdoMySQLOvPhysicalSchemaMapping::AddClass( 
    FdoRdbmsOvClassDefinition* pClass
)
{
    FdoPtr<FdoMySQLOvClassCollection> classes = GetClasses(); 
    classes->Add( (FdoMySQLOvClassDefinition*) pClass );
}

void FdoMySQLOvPhysicalSchemaMapping::Dispose()
{
    delete this;
}

void FdoMySQLOvPhysicalSchemaMapping::Init()
{
    mStorageEngine = MySQLOvStorageEngineType_Default;
    mClasses = FdoMySQLOvClassCollection::Create(
        FdoRdbmsOvClassesP(
            FdoRdbmsOvPhysicalSchemaMapping::GetRdbmsClasses()
        )
    );
}

/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPhysicalSchemaMapping.h>
//TODO: need to get provider name from provider-specific header.
#include "../../Fdo/FdoRdbmsSqlServerProviderInfo.h" // contains the provider name

FdoSqlServerOvPhysicalSchemaMapping* FdoSqlServerOvPhysicalSchemaMapping::Create()
{
    return new FdoSqlServerOvPhysicalSchemaMapping();
}

FdoSqlServerOvPhysicalSchemaMapping* FdoSqlServerOvPhysicalSchemaMapping::Create(FdoString* name)
{
    return new FdoSqlServerOvPhysicalSchemaMapping(name);
}

FdoSqlServerOvPhysicalSchemaMapping::FdoSqlServerOvPhysicalSchemaMapping()
{
    Init();
}

FdoSqlServerOvPhysicalSchemaMapping::FdoSqlServerOvPhysicalSchemaMapping(FdoString* name) : FdoRdbmsOvPhysicalSchemaMapping(name)
{
    Init();
}

FdoSqlServerOvPhysicalSchemaMapping::~FdoSqlServerOvPhysicalSchemaMapping()
{
}

FdoSqlServerOvClassCollection* FdoSqlServerOvPhysicalSchemaMapping::GetClasses()
{
    return FDO_SAFE_ADDREF( (FdoSqlServerOvClassCollection*) mClasses );
}

FdoString *FdoSqlServerOvPhysicalSchemaMapping::GetTableFilegroup()
{
    return mTableFilegroup;
}

void FdoSqlServerOvPhysicalSchemaMapping::SetTableFilegroup(FdoString *tableFilegroup)
{
    mTableFilegroup = tableFilegroup;
}

FdoString *FdoSqlServerOvPhysicalSchemaMapping::GetTextFilegroup()
{
    return mTextFilegroup;
}

void FdoSqlServerOvPhysicalSchemaMapping::SetTextFilegroup(FdoString *textFilegroup)
{
    mTextFilegroup = textFilegroup;
}

FdoString *FdoSqlServerOvPhysicalSchemaMapping::GetIndexFilegroup()
{
    return mIndexFilegroup;
}

void FdoSqlServerOvPhysicalSchemaMapping::SetIndexFilegroup(FdoString *indexFilegroup)
{
    mIndexFilegroup = indexFilegroup;
}


SqlServerOvTextInRowOption FdoSqlServerOvPhysicalSchemaMapping::GetTextInRow()
{
    return mTextInRow;
}

void FdoSqlServerOvPhysicalSchemaMapping::SetTextInRow(SqlServerOvTextInRowOption textInRow)
{
    mTextInRow = textInRow;
}

FdoString *FdoSqlServerOvPhysicalSchemaMapping::GetDatabase()
{
    return mDatabase;
}

void FdoSqlServerOvPhysicalSchemaMapping::SetDatabase(FdoString *database)
{
    mDatabase = database;
}

FdoString *FdoSqlServerOvPhysicalSchemaMapping::GetOwner()
{
    return mOwner;
}

void FdoSqlServerOvPhysicalSchemaMapping::SetOwner(FdoString *owner)
{
    mOwner = owner;
}

FdoString* FdoSqlServerOvPhysicalSchemaMapping::GetProvider(void)
{
    return RDBMS_SQLSERVER_PROVIDER_NAME;
}

void FdoSqlServerOvPhysicalSchemaMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvPhysicalSchemaMapping::InitFromXml( pContext, attrs );

    FdoXmlAttributeP att = attrs->FindItem( L"database" );
    if ( att ) {
        mDatabase = att->GetValue();
    }

    att = attrs->FindItem( L"owner" );
    if ( att ) {
        mOwner = att->GetValue();
    }

    att = attrs->FindItem( L"tableFilegroup" );
    if ( att ) {
        mTableFilegroup = att->GetValue();
    }

    att = attrs->FindItem( L"textFilegroup" );
    if ( att ) {
        mTextFilegroup = att->GetValue();
    }

    att = attrs->FindItem( L"indexFilegroup" );
    if ( att ) {
        mIndexFilegroup = att->GetValue();
    }

    att = attrs->FindItem( L"textInRow" );
    if ( att ) {
        mTextInRow = TextInRow_StringToEnum(att->GetValue(), pContext);
    }
}

void FdoSqlServerOvPhysicalSchemaMapping::_writeXmlAttributes(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    xmlWriter->WriteAttribute( L"xmlns", L"http://www.autodesk.com/isd/fdo/SqlServerProvider" );

    if ( mDatabase.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"database", mDatabase );

    if ( mOwner.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"owner", mOwner );

    if ( mTableFilegroup.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"tableFilegroup", mTableFilegroup );

    if ( mTextFilegroup.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"textFilegroup", mTextFilegroup );

    if ( mIndexFilegroup.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"indexFilegroup", mIndexFilegroup );

    if ( mTextInRow != SqlServerOvTextInRowOption_Default)
        xmlWriter->WriteAttribute( L"textInRow", TextInRow_EnumToString(mTextInRow) );
}

FdoRdbmsOvClassDefinition* FdoSqlServerOvPhysicalSchemaMapping::CreateClass( 
    FdoXmlSaxContext* context,
    FdoXmlAttributeCollection* atts
)
{
    FdoSqlServerOvClassDefinition* pClass = FdoSqlServerOvClassDefinition::Create();
    pClass->InitFromXml( context, atts );

    return pClass;
}
                
void FdoSqlServerOvPhysicalSchemaMapping::AddClass( 
    FdoRdbmsOvClassDefinition* pClass
)
{
    FdoSqlServerOvClassesP(GetClasses())->Add( (FdoSqlServerOvClassDefinition*) pClass );
}

void FdoSqlServerOvPhysicalSchemaMapping::Dispose()
{
    delete this;
}


void FdoSqlServerOvPhysicalSchemaMapping::Init()
{
    mTextInRow = SqlServerOvTextInRowOption_NotInRow;
    mClasses = FdoSqlServerOvClassCollection::Create(
        FdoRdbmsOvClassesP(
            FdoRdbmsOvPhysicalSchemaMapping::GetRdbmsClasses()
        )
    );
}

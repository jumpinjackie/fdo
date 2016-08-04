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
#include <Rdbms/Override/MySQL/MySqlOvTable.h>

FdoMySQLOvTable* FdoMySQLOvTable::Create()
{
    return new FdoMySQLOvTable();
}

FdoMySQLOvTable* FdoMySQLOvTable::Create(FdoString *name)
{
    return new FdoMySQLOvTable(name);
}

FdoMySQLOvTable::FdoMySQLOvTable()
{
    Init();
}

FdoMySQLOvTable::FdoMySQLOvTable(FdoString *name) : FdoRdbmsOvTable(name)
{
    Init();
}

FdoMySQLOvTable::~FdoMySQLOvTable()
{
}



FdoString *FdoMySQLOvTable::GetDataDirectory()
{
    return mDataDirectory;
}

void FdoMySQLOvTable::SetDataDirectory(FdoString *dataDirectory)
{
    mDataDirectory = dataDirectory;
}

FdoString *FdoMySQLOvTable::GetIndexDirectory()
{
    return mIndexDirectory;
}

void FdoMySQLOvTable::SetIndexDirectory(FdoString *indexDirectory)
{
    mIndexDirectory = indexDirectory;
}

MySQLOvStorageEngineType FdoMySQLOvTable::GetStorageEngine()
{
    return mStorageEngine;
}

void FdoMySQLOvTable::SetStorageEngine(MySQLOvStorageEngineType storageEngine)
{
    mStorageEngine = storageEngine;
}

FdoString* FdoMySQLOvTable::GetDatabase()
{
    return mDatabase;
}

void FdoMySQLOvTable::SetDatabase(FdoString *database)
{
    mDatabase = database;
}

void FdoMySQLOvTable::SetParent(FdoPhysicalElementMapping* value)
{
    FdoPhysicalElementMapping::SetParent(value);
}

void FdoMySQLOvTable::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvTable::InitFromXml( pContext, attrs );

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

void FdoMySQLOvTable::_writeXmlContents(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    if ( mDatabase.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"database", mDatabase );

    if ( mDataDirectory.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"dataDirectory", mDataDirectory );

    if ( mIndexDirectory.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"indexDirectory", mIndexDirectory );

    if ( mStorageEngine != MySQLOvStorageEngineType_Default)
        xmlWriter->WriteAttribute( L"storageEngine", StorageEngine_EnumToString(mStorageEngine) );
}

void FdoMySQLOvTable::Dispose()
{
    delete this;
}

void FdoMySQLOvTable::Init()
{
    mStorageEngine = MySQLOvStorageEngineType_Default;
}

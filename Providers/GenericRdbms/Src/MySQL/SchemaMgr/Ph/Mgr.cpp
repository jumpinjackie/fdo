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
#include "Mgr.h"
#include "Database.h"
#include "TempObject.h"
#include <Rdbms/Override/MySQL/MySqlOvPhysicalSchemaMapping.h>
#include <Rdbms/Override/MySQL/MySqlOvStorageEngineType.h>


FdoSmPhMySqlMgr::MySqlStringMap FdoSmPhMySqlMgr::mMySqlReservedDbObjectNames;

FdoSmPhMySqlMgr::FdoSmPhMySqlMgr(GdbiConnection* connection, FdoStringP mqlSchemaName) :
    FdoSmPhGrdMgr(connection)
{
    SetDefaultOwnerName(mqlSchemaName);
}

FdoSmPhMySqlMgr::~FdoSmPhMySqlMgr(void)
{
}

FdoSmPhDatabaseP FdoSmPhMySqlMgr::CreateDatabase(FdoStringP database)
{
    //TODO: when database not L"", verify that it exists.
    return new FdoSmPhMySqlDatabase(database, FDO_SAFE_ADDREF(this) );
}

FdoSmPhDbObjectP FdoSmPhMySqlMgr::CreateTempObject()
{
    return new FdoSmPhMySqlTempObject( FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhCfgSchemaReader> FdoSmPhMySqlMgr::CreateCfgSchemaReader( FdoSmPhRowsP rows )
{
    return new FdoSmPhCfgGrdSchemaReader( rows, FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhCfgClassReader> FdoSmPhMySqlMgr::CreateCfgClassReader( FdoSmPhRowsP rows, FdoStringP schemaName )
{
    return new FdoSmPhCfgGrdClassReader( rows, schemaName, FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhCfgPropertyReader> FdoSmPhMySqlMgr::CreateCfgPropertyReader( FdoStringP schemaName, FdoStringP className, FdoSmPhDbObjectP dbObject )
{
    return new FdoSmPhCfgGrdPropertyReader( schemaName, className, dbObject, FDO_SAFE_ADDREF(this) );
}

FdoStringP FdoSmPhMySqlMgr::GetOverrideOwner(FdoRdbmsOvSchemaMappingP mapping)
{
    FdoMySQLOvPhysicalSchemaMapping* mqlMapping = (FdoMySQLOvPhysicalSchemaMapping*) mapping.p;

    return mqlMapping ? mqlMapping->GetDatabase() : L"";
}

FdoStringP FdoSmPhMySqlMgr::GetOverrideTableStorage(FdoRdbmsOvSchemaMappingP mapping)
{
    FdoMySQLOvPhysicalSchemaMapping* mqlMapping = (FdoMySQLOvPhysicalSchemaMapping*) mapping.p;

    return mqlMapping ? mqlMapping->GetDataDirectory() : L"";
}

FdoStringP FdoSmPhMySqlMgr::GetOverrideIndexStorage(FdoRdbmsOvSchemaMappingP mapping)
{
    FdoMySQLOvPhysicalSchemaMapping* mqlMapping = (FdoMySQLOvPhysicalSchemaMapping*) mapping.p;

    return mqlMapping ? mqlMapping->GetIndexDirectory() : L"";
}

FdoStringP FdoSmPhMySqlMgr::GetOverrideTableStorageEngine(FdoRdbmsOvSchemaMappingP mapping)
{
    FdoMySQLOvPhysicalSchemaMapping* mqlMapping = (FdoMySQLOvPhysicalSchemaMapping*) mapping.p;

    return mqlMapping ? StorageEngine_EnumToString(mqlMapping->GetStorageEngine()) : L"";
}


/* TODO: This might move down to the generic levle
FdoSmPhReaderP FdoSmPhMySqlMgr::GetDbObjectsReader( bool databaseSet )
{
    if ( databaseSet )
        return mDbObjectsReaderD;
    else
        return mDbObjectsReader;
}

void FdoSmPhMySqlMgr::SetDbObjectsReader( FdoSmPhReaderP reader, bool databaseSet )
{
    if ( databaseSet )
        mDbObjectsReaderD = reader;
    else
        mDbObjectsReader = reader;
}

FdoSmPhReaderP FdoSmPhMySqlMgr::GetDbObjectReader( bool databaseSet )
{
    if ( databaseSet )
        return mDbObjectReaderD;
    else
        return mDbObjectReader;
}

void FdoSmPhMySqlMgr::SetDbObjectReader( FdoSmPhReaderP reader, bool databaseSet )
{
    if ( databaseSet )
        mDbObjectReaderD = reader;
    else
        mDbObjectReader = reader;
}
*/
FdoStringP FdoSmPhMySqlMgr::FormatDefaultedField( FdoStringP fieldName, FdoStringP colName, FdoStringP defaultValue, FdoSmPhColType fieldType )
{
    return FdoStringP::Format(
        L"ifnull(%ls,%ls) %ls",
        (FdoString*) colName,
        (FdoString*) FormatSQLVal( defaultValue, fieldType ),
        (FdoString*) fieldName
    );
}

FdoStringP FdoSmPhMySqlMgr::FormatSQLVal( FdoStringP value, FdoSmPhColType valueType )
{
	FdoStringP sqlString;
    
    if ( value.GetLength() > 0 ) {
        if ( valueType == FdoSmPhColType_String || valueType == FdoSmPhColType_Date )
            sqlString = FdoStringP(L"'") + FdoStringP(value).Replace( L"'", L"\\'" ) + FdoStringP(L"'");
        else
            sqlString = value;
    }
    else {
        sqlString = L"null";
    }

	return sqlString;
}

FdoStringP FdoSmPhMySqlMgr::FormatCurrentDateField()
{
    return L"curdate()";
}

FdoStringP FdoSmPhMySqlMgr::FormatBindField( int pos)
{
    return L"?";
}

FdoBoolean FdoSmPhMySqlMgr::IsDbObjectNameReserved( FdoStringP objName )
{
    return mMySqlReservedDbObjectNames.IsReserved( objName );
}

FdoSize FdoSmPhMySqlMgr::DbObjectNameMaxLen()
{
    return 64;
}

FdoSize FdoSmPhMySqlMgr::ColNameMaxLen()
{
    return 64;
}

FdoSmPhMySqlMgr::MySqlStringMap::MySqlStringMap()
{
    // The following are reserved words specific to MySql.
    // Base constructor adds general reserved words to this list.
    Insert( L"bigint" ); 
    Insert( L"binary" ); 
    Insert( L"blob" ); 
    Insert( L"datetime" ); 
    Insert( L"double" ); 
    Insert( L"enum" ); 
    Insert( L"int" ); 
    Insert( L"longblob" ); 
    Insert( L"longtext" ); 
    Insert( L"mediumint" ); 
    Insert( L"mediumtext" ); 
    Insert( L"single" ); 
    Insert( L"smallint" ); 
    Insert( L"text" ); 
    Insert( L"time" ); 
    Insert( L"timestamp" ); 
    Insert( L"tinyint" ); 
    Insert( L"tinytext" ); 
    Insert( L"varbinary" ); 
    Insert( L"year" ); 
};



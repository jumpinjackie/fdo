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
#include <Rdbms/Override/ODBC/OdbcOvPhysicalSchemaMapping.h>
#include <Sm/Ph/Rd/ClassReader.h>

FdoSmPhOdbcMgr::OdbcStringMap FdoSmPhOdbcMgr::mOdbcReservedDbObjectNames;

FdoSmPhOdbcMgr::FdoSmPhOdbcMgr(GdbiConnection* connection, FdoStringP OdbcSchemaName) :
    FdoSmPhGrdMgr(connection),
    mIsGeometryFromOrdinatesWanted(false)
{
    SetDefaultOwnerName(OdbcSchemaName);
    if (connection->IsGeometryFromOrdinatesWanted() != 'f' && 
        connection->IsGeometryFromOrdinatesWanted() != 'F')
    {
        mIsGeometryFromOrdinatesWanted = true;
    }
}
FdoSmPhOdbcMgr::~FdoSmPhOdbcMgr(void)
{
}

FdoSmPhDatabaseP FdoSmPhOdbcMgr::CreateDatabase(FdoStringP database)
{
    //TODO: when database not L"", verify that it exists.
    return new FdoSmPhOdbcDatabase(database, FDO_SAFE_ADDREF(this) );
}

FdoSmPhDbObjectP FdoSmPhOdbcMgr::CreateTempObject()
{
    return new FdoSmPhOdbcTempObject( FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhCfgSchemaReader> FdoSmPhOdbcMgr::CreateCfgSchemaReader( FdoSmPhRowsP rows )
{
    return new FdoSmPhCfgGrdSchemaReader( rows, FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhCfgClassReader> FdoSmPhOdbcMgr::CreateCfgClassReader( FdoSmPhRowsP rows, FdoStringP schemaName )
{
    return new FdoSmPhCfgGrdClassReader( rows, schemaName, FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhCfgPropertyReader> FdoSmPhOdbcMgr::CreateCfgPropertyReader( FdoStringP schemaName, FdoStringP className, FdoSmPhDbObjectP dbObject )
{
    return new FdoSmPhCfgGrdPropertyReader( schemaName, className, dbObject, FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhRdSchemaReader> FdoSmPhOdbcMgr::CreateRdSchemaReader( FdoSmPhRowsP rows, FdoSmPhOwnerP owner, bool dsInfo )
{
    return new FdoSmPhRdOdbcSchemaReader( rows, owner, dsInfo );
}

FdoPtr<FdoSmPhRdClassReader> FdoSmPhOdbcMgr::CreateRdClassReader( 
    FdoPtr<FdoSmPhRowCollection> rows, 
    FdoStringP schemaName, 
    FdoStringP className,
    FdoBoolean keyedOnly,
    FdoStringP database,
    FdoStringP owner
)
{
#pragma message ("TODO: look up schema object to get owner name")
    // Ideally, we would not exclude based on RdSchemaPrefix, but rather
    // look up an approprate owner name, based on schemaName, or adjust the calling
    // code to pass in the right owner name -- which may be an empty string
    // on data sources that do not support named physical schemas.
    if (schemaName != NULL && schemaName.GetLength() > 0 &&
        schemaName != this->RdSchemaPrefix &&
        (owner == NULL || owner.GetLength() <= 0) )
    {
        owner = schemaName;
    }

    return FdoSmPhMgr::CreateRdClassReader(rows, schemaName, className, keyedOnly, database, owner);
}

void FdoSmPhOdbcMgr::SetConfiguration( 
    FdoStringP providerName,
    FdoIoStreamP configDoc,
    FdoFeatureSchemasP configSchemas,
    FdoSchemaMappingsP configMappings 
)
{
    // This does what base class' FdoSmPhMgr::SetConfiguration() does, except
    // that we do not check for metaschema here.

    mProviderName = providerName;
    mConfigDoc = configDoc;
    mConfigSchemas = configSchemas;
    mConfigMappings = configMappings;
}

FdoStringP FdoSmPhOdbcMgr::GetDcRdbmsObjectName( FdoStringP objectName )
{
    return objectName;
}

FdoStringP FdoSmPhOdbcMgr::GetOverrideDatabase(FdoRdbmsOvSchemaMappingP mapping)
{
    // ODBC Provider does not currently support this override.

    return L"";
}

FdoStringP FdoSmPhOdbcMgr::GetOverrideOwner(FdoRdbmsOvSchemaMappingP mapping)
{
    // ODBC Provider does not currently support this override.

    return L"";
}

FdoStringP FdoSmPhOdbcMgr::FormatDefaultedField( FdoStringP fieldName, FdoStringP colName, FdoStringP defaultValue, FdoSmPhColType fieldType )
{
    return FdoStringP::Format(
        L"isnull(%ls,%ls) %ls",
        (FdoString*) colName,
        (FdoString*) FormatSQLVal( defaultValue, fieldType ),
        (FdoString*) fieldName
    );
}

FdoStringP FdoSmPhOdbcMgr::FormatSQLVal( FdoStringP value, FdoSmPhColType valueType )
{
	FdoStringP sqlString;
    
    if ( value.GetLength() > 0 ) {
        if ( valueType == FdoSmPhColType_String || valueType == FdoSmPhColType_Date )
            sqlString = FdoStringP(L"'") + FdoStringP(value).Replace( L"'", L"''" ) + FdoStringP(L"'");
        else
            sqlString = value;
    }
    else {
        sqlString = L"null";
    }

	return sqlString;
}

FdoStringP FdoSmPhOdbcMgr::FormatCurrentDateField()
{
    return L"getdate()";
}

FdoStringP FdoSmPhOdbcMgr::FormatBindField( int pos)
{
    return L"?";
}

FdoBoolean FdoSmPhOdbcMgr::IsDbObjectNameReserved( FdoStringP objName )
{
    return mOdbcReservedDbObjectNames.IsReserved( objName );
}

FdoSize FdoSmPhOdbcMgr::DbObjectNameMaxLen()
{
    return 64;
}

FdoSize FdoSmPhOdbcMgr::ColNameMaxLen()
{
    return 64;
}

FdoBoolean FdoSmPhOdbcMgr::IsGeometryFromOrdinatesWanted()
{
    return mIsGeometryFromOrdinatesWanted;
}

FdoSmPhOdbcMgr::OdbcStringMap::OdbcStringMap()
{
    // The following are reserved words specific to Odbc.
    // Base constructor adds general reserved words to this list.
    Insert( L"bigint" ); 
    Insert( L"binary" ); 
    Insert( L"image" ); 
    Insert( L"datetime" ); 
    Insert( L"double" ); 
    Insert( L"numeric" ); 
    Insert( L"decimal" ); 
    Insert( L"int" ); 
    Insert( L"bit" ); 
    Insert( L"real" ); 
    Insert( L"float" ); 
    Insert( L"smallint" ); 
    Insert( L"text" ); 
    Insert( L"time" ); 
    Insert( L"timestamp" ); 
    Insert( L"tinyint" ); 
    Insert( L"varbinary" ); 
}



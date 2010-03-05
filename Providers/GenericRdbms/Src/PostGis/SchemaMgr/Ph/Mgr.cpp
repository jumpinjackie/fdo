//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "Mgr.h"
#include "Database.h"
#include "TempObject.h"
#include "SpatialContextGroupWriter.h"
#include "Rd/ClassReader.h"
#include "Rd/SchemaReader.h"
#include <Rdbms/Override/PostGis/PostGisOvPhysicalSchemaMapping.h>

class FdoSmPhRdPostGisClassReader;

//
// Static definitions
//
FdoSmPhPostGisMgr::ReservedWordsMap FdoSmPhPostGisMgr::mReservedWords;

//
// FdoSmPhPostGisOwner implementation.
//

FdoSmPhPostGisMgr::FdoSmPhPostGisMgr() :
    mDbVersion(0)
{
    // idle
}

FdoSmPhPostGisMgr::FdoSmPhPostGisMgr(GdbiConnection* conn,
    FdoStringP schemaName)
    : FdoSmPhGrdMgr(conn),
      mDbVersion(0)
{
    SetDefaultOwnerName(schemaName);
}

FdoSmPhPostGisMgr::~FdoSmPhPostGisMgr()
{
    // idle
}

FdoStringP FdoSmPhPostGisMgr::GetDbVersion()
{

    if ( mDbVersion == 0 ) {
        GdbiConnection* gdbiConn = GetGdbiConnection();
        GdbiCommands* gdbiCommands = gdbiConn->GetCommands();

        rdbi_vndr_info_def info;
        gdbiCommands->vndr_info( &info );

        mDbVersion = info.dbversion;    
    }
 
    return FdoStringP::Format( L"%d", mDbVersion );
}


bool FdoSmPhPostGisMgr::SupportsNumericCoordinateSystemName()
{
    return true;
}

bool FdoSmPhPostGisMgr::SupportsMixedCase()
{
    return true;
}

FdoSmPhDatabaseP FdoSmPhPostGisMgr::CreateDatabase(FdoStringP database)
{
    //TODO: When database not L"", verify that it exists.

    FdoSmPhPostGisDatabase* pgDb = NULL;
    pgDb = new FdoSmPhPostGisDatabase(database, FDO_SAFE_ADDREF(this));

    return pgDb;
}

FdoSmPhDbObjectP FdoSmPhPostGisMgr::CreateTempObject()
{
    FdoSmPhPostGisTempObject* pgTmp = NULL;
    pgTmp = new FdoSmPhPostGisTempObject(FDO_SAFE_ADDREF(this));

    return pgTmp;
}

FdoPtr<FdoSmPhRdClassReader> FdoSmPhPostGisMgr::CreateRdClassReader( 
    FdoPtr<FdoSmPhRowCollection> rows, 
    FdoStringP schemaName, 
    FdoStringP className,
    FdoBoolean keyedOnly,
    FdoStringP database,
    FdoStringP owner
)
{
    return new FdoSmPhRdPostGisClassReader( rows, schemaName, className, FDO_SAFE_ADDREF(this), keyedOnly, database, owner );
}

FdoPtr<FdoSmPhCfgSchemaReader> FdoSmPhPostGisMgr::CreateCfgSchemaReader(
    FdoSmPhRowsP rows)
{
    FdoSmPhCfgGrdSchemaReader* smReader = NULL;
    smReader = new FdoSmPhCfgGrdSchemaReader(rows, FDO_SAFE_ADDREF(this));
    
    return smReader;
}

FdoPtr<FdoSmPhCfgClassReader> FdoSmPhPostGisMgr::CreateCfgClassReader(
    FdoSmPhRowsP rows,
    FdoStringP schemaName)
{
    FdoSmPhCfgGrdClassReader* classReader = NULL;
    classReader = new FdoSmPhCfgGrdClassReader(rows,
        schemaName, FDO_SAFE_ADDREF(this));

    return classReader;
}

FdoPtr<FdoSmPhCfgPropertyReader> FdoSmPhPostGisMgr::CreateCfgPropertyReader(
    FdoStringP schemaName,
    FdoStringP className,
    FdoSmPhDbObjectP dbObject)
{
    FdoSmPhCfgGrdPropertyReader* propReader = NULL;
    propReader = new FdoSmPhCfgGrdPropertyReader(schemaName, className,
        dbObject, FDO_SAFE_ADDREF(this));

    return propReader;
}

FdoPtr<FdoSmPhRdSchemaReader> FdoSmPhPostGisMgr::CreateRdSchemaReader( FdoSmPhRowsP rows, FdoSmPhOwnerP owner, bool dsInfo )
{
    return new FdoSmPhRdPostGisSchemaReader( rows, owner, dsInfo );
}

FdoSmPhSpatialContextGroupWriterP FdoSmPhPostGisMgr::NewSpatialContextGroupWriter()
{
    return (FdoSmPhSpatialContextGroupWriter*)(new FdoSmPhPostGisSpatialContextGroupWriter(FDO_SAFE_ADDREF(this)) );
}

FdoStringP FdoSmPhPostGisMgr::GetOverrideOwner(
    FdoRdbmsOvSchemaMappingP mapping)
{
    // 'mapping' is allowed to be NULL pointer

    FdoPostGISOvPhysicalSchemaMapping* pgMapping = NULL;
    pgMapping = static_cast<FdoPostGISOvPhysicalSchemaMapping*>(mapping.p);

    FdoStringP owner(pgMapping ? pgMapping->GetOwner() : L"");
    
    return owner;
}

FdoStringP FdoSmPhPostGisMgr::GetDcDbObjectName( FdoStringP objectName )
{
    FdoStringP fullObjectName = GetRealDbObjectName( objectName );

    return GetDcRdbmsObjectName( fullObjectName );
}

FdoStringP FdoSmPhPostGisMgr::GetRealDbObjectName( FdoStringP objectName )
{
    FdoStringP fullObjectName = ((objectName == L"") || (objectName.Contains(L"."))) ?
        objectName :
        // user defaults to public
        FdoStringP(L"public.") + objectName;

    return fullObjectName;
}

FdoStringP FdoSmPhPostGisMgr::DbObject2MetaSchemaName( FdoStringP objectName )
{
    if ( objectName.Mid(0, 7) == L"public." )
        return objectName.Mid(7,99999);

    return objectName;
}

FdoStringP FdoSmPhPostGisMgr::GetDefaultPhysicalSchemaName()
{
    return L"public";
}

FdoStringP FdoSmPhPostGisMgr::FormatDefaultedField(FdoStringP fieldName,
    FdoStringP colName,
    FdoStringP defaultValue,
    FdoSmPhColType fieldType)
{
    FdoStringP sql = FdoStringP::Format(
        L" COALESCE(%ls,%ls) AS %ls",
        static_cast<FdoString*>(colName),
        static_cast<FdoString*>(FormatSQLVal(defaultValue, fieldType)),
        static_cast<FdoString*>(fieldName));

    return sql;
}

FdoStringP FdoSmPhPostGisMgr::FormatSQLVal(FdoStringP value,
    FdoSmPhColType type)
{
    // Default return if 'value' is empty
	FdoStringP sqlString;
    
    if (value.GetLength() > 0)
    {
        if (FdoSmPhColType_String == type)
        {
            sqlString = FdoStringP(L"'") 
                        + FdoStringP(value).Replace(L"'", L"''")
                        + FdoStringP(L"'");
        }
        else if (FdoSmPhColType_Bool == type)
        {
            sqlString = FdoStringP(value).Replace(L"1", L"true");
            sqlString = sqlString.Replace(L"0", L"false");
        }
        else
        {
            sqlString = value;
        }
    }
    else {
        if ( type == FdoSmPhColType_String ) 
            sqlString = L"cast(null as varchar)";
        else
            sqlString = L"null";
    }

	return sqlString;
}

FdoStringP FdoSmPhPostGisMgr::FormatCurrentDateField()
{
    return (L"CURRENT_DATE");
}

FdoStringP FdoSmPhPostGisMgr::FormatBindField(int pos)
{
    return FdoStringP::Format(L"$%d", pos + 1);
}

FdoStringP FdoSmPhPostGisMgr::FormatCollateColumnSql( FdoStringP columnName)
{
    GetDbVersion();

    FdoStringP functionName = L"convert_to";

    if ( mDbVersion < 80300 ) 
        functionName = L"convert";

    return FdoStringP::Format( L"%ls(%ls,'UTF8')", (FdoString*) functionName, (FdoString*) columnName );
}


FdoBoolean FdoSmPhPostGisMgr::IsDbObjectNameReserved(FdoStringP objName)
{
    return mReservedWords.IsReserved(objName);
}

FdoSize FdoSmPhPostGisMgr::DbObjectNameMaxLen()
{
    return 63;
}

FdoSize FdoSmPhPostGisMgr::ColNameMaxLen()
{
    return 63;
}

FdoSmPhPostGisMgr::ReservedWordsMap::ReservedWordsMap()
{
    // The following are reserved words specific to PostgreSQL/PostGIS.
    // Base constructor adds general reserved words to this list.

    Insert(L"bigint"); 
    Insert(L"binary"); 
    Insert(L"blob"); 
    Insert(L"datetime"); 
    Insert(L"double"); 
    Insert(L"enum"); 
    Insert(L"int"); 
    Insert(L"longblob"); 
    Insert(L"longtext"); 
    Insert(L"mediumint"); 
    Insert(L"mediumtext"); 
    Insert(L"single"); 
    Insert(L"smallint"); 
    Insert(L"text"); 
    Insert(L"time"); 
    Insert(L"timestamp"); 
    Insert(L"tinyint"); 
    Insert(L"tinytext"); 
    Insert(L"varbinary"); 
    Insert(L"year"); 
    Insert(L"number"); 
    Insert(L"xmin"); 
    Insert(L"xmax"); 
}

FdoSmPhPostGisMgr::ReservedWordsMap::~ReservedWordsMap()
{
    // idle
}

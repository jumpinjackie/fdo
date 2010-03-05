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
#include "DbObjectReader.h"
#include <Sm/Ph/Rd/SchemaDbObjectBinds.h>
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"
#include "../Owner.h"

FdoSmPhRdPostGisDbObjectReader::FdoSmPhRdPostGisDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoStringP objectName)
    : FdoSmPhRdDbObjectReader(NULL, owner, objectName)
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    if ( objectName != L"" ) 
        objectNames->Add(objectName);

    SetSubReader(MakeQueryReader(owner, objectNames));
}

FdoSmPhRdPostGisDbObjectReader::FdoSmPhRdPostGisDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames)
    : FdoSmPhRdDbObjectReader(NULL, owner, L"")
{
    SetSubReader(MakeQueryReader(owner, objectNames));
}

FdoSmPhRdPostGisDbObjectReader::FdoSmPhRdPostGisDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join)
    : FdoSmPhRdDbObjectReader(NULL, owner, L"")
{
    FdoStringsP objectNames = FdoStringCollection::Create();

    SetSubReader(MakeQueryReader(owner, objectNames, join));
}

FdoSmPhRdPostGisDbObjectReader::~FdoSmPhRdPostGisDbObjectReader()
{
    // idle
}

FdoSmPhDbObjType FdoSmPhRdPostGisDbObjectReader::GetType()
{
    // Default value returned if type not recognized.
    FdoSmPhDbObjType retType = FdoSmPhDbObjType_Unknown;

    FdoStringP type;
    type = GetString(L"", L"type");

    if (type == L"base table")
        retType = FdoSmPhDbObjType_Table;
    else if (type == L"view")
        retType = FdoSmPhDbObjType_View;

    return retType;
}

FdoSmPhReaderP FdoSmPhRdPostGisDbObjectReader::MakeQueryReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames,
    FdoSmPhRdTableJoinP join)
{
    FdoStringP sqlString;

    FdoSmPhPostGisOwner* pgOwner = NULL;
    pgOwner = static_cast<FdoSmPhPostGisOwner*>(owner.p);

    FdoStringP tablesTableName(pgOwner->GetTablesTable());

    FdoSmPhMgrP mgr(owner->GetManager());
    
    FdoSmPhPostGisMgr* pgMgr = NULL;
    pgMgr = static_cast<FdoSmPhPostGisMgr*>(mgr.p);

    // template_postgis=# \d information_schema.tables
    //                        View "information_schema.tables"
    //        Column        |      Type      |
    // ---------------------+----------------+
    //  table_catalog       | sql_identifier |
    //  table_schema        | sql_identifier |
    //  table_name          | sql_identifier |
    //  table_type          | character_data |
    //  is_insertable_into  | character_data | <- Always YES for BASE TABLE
    //  commit_action       | character_data | <- NULL, for temp tables is PRESERVE 


    // Names of PostgreSQL objects: database, table, field and columns
    // are case-independent, unless you created them with double-quotes
    // around their name, in which case they are case-sensitive.
    // NOTE: In MySQL, table names can be case-sensitive or not,
    //       depending on which operating system you are using.
    //       On Linux, names are case-sensitive.

    // TODO: mloskot - Remove unused fields.

    sqlString = FdoStringP::Format(
        L" SELECT %ls t.table_schema  || '.' || t.table_name AS name, "
        //L" SELECT %ls t.table_name AS name, t.table_schema AS schema, "
        L" lower(t.table_type) AS type, "
        L" %ls as collate_schema_name, "
        L" %ls as collate_table_name "
        L" FROM %ls AS t $(JOIN_FROM) "
        L" WHERE t.table_schema not in ( 'information_schema' ,'pg_catalog') "           
        L" $(AND) $(QUALIFICATION)"
        L" AND t.table_type IN ('BASE TABLE','VIEW') "
        L" AND t.table_name not in ( 'geometry_columns' ,'spatial_ref_sys') "
        L" ORDER BY collate_schema_name, collate_table_name ASC ",
        (join ? L"distinct" : L""),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"t.table_schema"),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"t.table_name"),
        static_cast<FdoString*>(tablesTableName)
    );

    FdoSmPhReaderP reader = FdoSmPhRdDbObjectReader::MakeQueryReader(
        L"",
        owner,
        sqlString,
        L"t.table_schema",
        L"t.table_name",
        objectNames,
        join
    );

    return reader;
}



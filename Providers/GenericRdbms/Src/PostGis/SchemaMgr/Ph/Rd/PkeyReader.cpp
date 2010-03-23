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
#include "PkeyReader.h"
#include "../Mgr.h"
#include "../Owner.h"
#include <Sm/Ph/Rd/SchemaDbObjectBinds.h>
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdPostGisPkeyReader::FdoSmPhRdPostGisPkeyReader(FdoSmPhOwnerP owner,
    FdoSmPhDbObjectP dbObject)
    : FdoSmPhRdPkeyReader(),
    mDbObject(dbObject)
{
    SetSubReader(
        MakeReader(
            owner,
            DbObject2Objects(dbObject)
        )
    );
}

FdoSmPhRdPostGisPkeyReader::FdoSmPhRdPostGisPkeyReader(FdoSmPhOwnerP owner,
    FdoStringsP objectNames)
    : FdoSmPhRdPkeyReader()
{
    SetSubReader(
        MakeReader(
            owner,
            objectNames
        )
    );
}

FdoSmPhRdPostGisPkeyReader::~FdoSmPhRdPostGisPkeyReader()
{
    // idle
}

FdoSmPhReaderP FdoSmPhRdPostGisPkeyReader::MakeReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames,
    FdoSmPhRdTableJoinP join)
{
    // NOTE: mloskot - We need to remove const qualifier first,
    // then cast to specialized type.
    FdoSmPhMgrP mgr = owner->GetManager();
    FdoSmPhPostGisMgrP   pgMgr = mgr->SmartCast<FdoSmPhPostGisMgr>();
    FdoSmPhPostGisOwnerP pgOwner = owner->SmartCast<FdoSmPhPostGisOwner>();

    //
    // Generate SQL statement to get foreign keys
    //
    // template_postgis=# \d information_schema.table_constraints
    //     View "information_schema.table_constraints"
    //        Column       |       Type        |
    // --------------------+-------------------+
    //  constraint_catalog | character varying |
    //  constraint_schema  | character varying |
    //  constraint_name    | character varying |
    //  table_catalog      | character varying |
    //  table_schema       | character varying |
    //  table_name         | character varying |
    //  constraint_type    | character varying | <- Type: CHECK,FOREIGN KEY,
    //  is_deferrable      | character varying |          PRIMARY KEY,UNIQUE
    //  initially_deferred | character varying |
    //
    //
    // template_postgis=# \d information_schema.key_column_usage
    //    View "information_schema.key_column_usage"
    //             Column             |     Type        |
    // -------------------------------+-----------------+
    //  constraint_catalog            | sql_identifier  |
    //  constraint_schema             | sql_identifier  |
    //  constraint_name               | sql_identifier  |
    //  table_catalog                 | sql_identifier  |
    //  table_schema                  | sql_identifier  |
    //  table_name                    | sql_identifier  |
    //  column_name                   | sql_identifier  |
    //  ordinal_position              | cardinal_number | <- Starts at 1

    // Names of PostgreSQL objects: database, table, field and columns
    // are case-independent, unless you created them with double-quotes
    // around their name, in which case they are case-sensitive.
    // NOTE: In MySQL, table names can be case-sensitive or not,
    //       depending on which operating system you are using.
    //       On Linux, names are case-sensitive.

    FdoStringP sqlString = FdoStringP::Format(
        L" SELECT %ls tc.conname AS constraint_name,"
        L" ns.nspname ||'.'|| c.relname AS table_name,"
        L" cast(tc.conkey as text) AS column_name, "
        L" ns.nspname AS table_schema,"
        L" %ls as collate_schema_name, "
        L" %ls as collate_table_name, "
        L" %ls as collate_constraint_name "
        L" FROM pg_constraint tc,  pg_class c, pg_namespace ns $(JOIN_FROM) "
        L" WHERE tc.contype = 'p' "
        L" and c.oid = tc.conrelid and ns.oid = tc.connamespace "
        L" $(AND) $(QUALIFICATION)\n"
        L" ORDER BY collate_schema_name, collate_table_name, collate_constraint_name",
        (join ? L"distinct" : L""),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"ns.nspname"),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"c.relname"),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"tc.conname")
    );

    FdoSmPhReaderP reader = MakeQueryReader(
        L"",
        owner,
        sqlString,
        L"ns.nspname",
        L"c.relname",
        objectNames,
        join
    );

    return reader;
}

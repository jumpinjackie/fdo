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
#include "FkeyReader.h"
#include "../Mgr.h"
#include "../Owner.h"
#include <Sm/Ph/Rd/SchemaDbObjectBinds.h>
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdPostGisFkeyReader::FdoSmPhRdPostGisFkeyReader(FdoSmPhOwnerP owner,
    FdoSmPhDbObjectP dbObject)
    : FdoSmPhRdFkeyReader(),
    mDbObject(dbObject)
{
    SetSubReader(
        MakeReader(
            owner,
            DbObject2Objects(dbObject)
        )
    );
}

FdoSmPhRdPostGisFkeyReader::FdoSmPhRdPostGisFkeyReader(FdoSmPhOwnerP owner,
    FdoStringsP objectNames)
    : FdoSmPhRdFkeyReader()
{
    SetSubReader(
        MakeReader(
            owner,
            objectNames
        )
    );
}

FdoSmPhRdPostGisFkeyReader::~FdoSmPhRdPostGisFkeyReader()
{
    // idle
}

FdoSmPhReaderP FdoSmPhRdPostGisFkeyReader::MakeReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames,
    FdoSmPhRdTableJoinP join)
{
    FdoSmPhMgrP mgr = owner->GetManager();
    FdoSmPhPostGisMgrP   pgMgr = mgr->SmartCast<FdoSmPhPostGisMgr>();
    FdoSmPhPostGisOwnerP pgOwner = owner->SmartCast<FdoSmPhPostGisOwner>();

    //
    // Generate SQL statement to get foreign keys
    //

    FdoStringP sqlString = FdoStringP::Format(
        L" SELECT %ls tc.conname AS constraint_name,"
        L" ns.nspname ||'.'|| c.relname AS table_name,"
        L" cast(tc.conkey as text) AS column_name, "
        L" ns.nspname AS table_schema,"
        L" cast('%ls' as varchar) as r_owner_name, "
        L" nr.nspname ||'.'|| r.relname AS r_table_name,"
        L" cast(tc.confkey as text) AS r_column_name, "
        L" %ls as collate_schema_name, "
        L" %ls as collate_table_name, "
        L" %ls as collate_constraint_name "
        L" FROM pg_constraint tc,  pg_class c, pg_namespace ns, "
        L"    pg_class r, pg_namespace nr $(JOIN_FROM) "
        L" WHERE tc.contype = 'f' "
        L" and c.oid = tc.conrelid and ns.oid = tc.connamespace "
        L" and r.oid = tc.confrelid and nr.oid = r.relnamespace "
        L" $(AND) $(QUALIFICATION)\n"
        L" ORDER BY collate_schema_name, collate_table_name, collate_constraint_name",
        (join ? L"distinct" : L""),
        (FdoString*) owner->GetName(),
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

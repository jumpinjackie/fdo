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
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdPostGisPkeyReader::FdoSmPhRdPostGisPkeyReader(FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP dbObject)
    : FdoSmPhRdPkeyReader((FdoSmPhReader*) NULL),
    mDbObject(dbObject)
{
    SetSubReader(
        MakeReader(mgr,
            static_cast<const FdoSmPhOwner*>(dbObject->GetParent()), dbObject)
    );
}

FdoSmPhRdPostGisPkeyReader::FdoSmPhRdPostGisPkeyReader(FdoSmPhMgrP mgr,
    FdoSmPhOwnerP owner)
    : FdoSmPhRdPkeyReader((FdoSmPhReader*) NULL)
{
    SetSubReader(
        MakeReader(mgr,
            static_cast<FdoSmPhOwner*>(owner), NULL)
    );
}

FdoSmPhRdPostGisPkeyReader::~FdoSmPhRdPostGisPkeyReader()
{
    // idle
}

FdoSmPhReaderP FdoSmPhRdPostGisPkeyReader::MakeReader(FdoSmPhMgrP mgr,
    const FdoSmPhOwner* owner,
    FdoSmPhDbObjectP dbObject)
{
    // NOTE: mloskot - We need to remove const qualifier first,
    // then cast to specialized type.
    FdoSmPhPostGisMgrP   pgMgr = mgr->SmartCast<FdoSmPhPostGisMgr>();
    FdoSmPhPostGisOwner* pgOwner = 
        static_cast<FdoSmPhPostGisOwner*>(
            const_cast<FdoSmPhOwner*>(owner));

    FdoStringP objectName = (dbObject ? dbObject->GetName() : L"");
    if( objectName.Contains(L".") )
        objectName = objectName.Right(L".");

    FdoStringP ownerName = owner->GetName();

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
      L"select tc.constraint_name as constraint_name,\n"
      L" tc.table_schema||'.'||tc.table_name as table_name, kcu.column_name as column_name\n"
      L" from %ls tc, %ls kcu\n"
      L" where (tc.constraint_schema = kcu.constraint_schema\n"
      L"     and tc.constraint_name = kcu.constraint_name\n"
      L"     and tc.table_schema = kcu.table_schema\n"
      L"     and tc.table_name = kcu.table_name\n"
      //L"     and tc.table_schema = $1\n"
      L"     %ls\n"
      L"     and tc.constraint_type = 'PRIMARY KEY')\n"
      L" order by %ls, %ls, kcu.ordinal_position",
      (FdoString*) pgOwner->GetTableConstraintsTable(),
      (FdoString*) pgOwner->GetKeyColumnUsageTable(),
      dbObject ? L"and tc.table_name = $1" : L"",
      (FdoString*) pgMgr->FormatCollateColumnSql(L"tc.table_schema"),
      (FdoString*) pgMgr->FormatCollateColumnSql(L"tc.table_name")
    );

    // Create a field object for each field in the select list.
    FdoSmPhRowsP rows = MakeRows(mgr);

    // Create and set the bind variables
    FdoSmPhRowP binds(new FdoSmPhRow(mgr, L"Binds"));
    FdoSmPhDbObjectP rowObj(binds->GetDbObject());

    //FdoSmPhFieldP field(new FdoSmPhField(binds,
    //    L"owner_name",
   //     rowObj->CreateColumnDbObject(L"owner_name", false)));

    //field->SetFieldValue(ownerName);

    if (dbObject)
    {
        FdoSmPhFieldP field = new FdoSmPhField(binds,
            L"object_name",
            rowObj->CreateColumnDbObject(L"object_name", false));

        field->SetFieldValue(objectName);
    }

    //TODO: cache this query to make full use of the binds.
    FdoSmPhRdGrdQueryReader* reader = NULL;
    reader = new FdoSmPhRdGrdQueryReader(
        FdoSmPhRowP(rows->GetItem(0)), sqlString, mgr, binds);

    return reader;
}

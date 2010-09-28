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
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdPostGisFkeyReader::FdoSmPhRdPostGisFkeyReader(FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP dbObject)
    : FdoSmPhRdFkeyReader(MakeReader(mgr,
        static_cast<const FdoSmPhOwner*>(dbObject->GetParent()), dbObject)),
      mDbObject(dbObject)
{
    // idle
}

FdoSmPhRdPostGisFkeyReader::FdoSmPhRdPostGisFkeyReader(FdoSmPhMgrP mgr,
    FdoSmPhOwnerP owner)
    : FdoSmPhRdFkeyReader(MakeReader(mgr,
        static_cast<FdoSmPhOwner*>(owner), NULL))
{
    // idle
}

FdoSmPhRdPostGisFkeyReader::~FdoSmPhRdPostGisFkeyReader()
{
    // idle
}

FdoSmPhReaderP FdoSmPhRdPostGisFkeyReader::MakeReader(FdoSmPhMgrP mgr,
    const FdoSmPhOwner* owner,
    FdoSmPhDbObjectP dbObject)
{
    return (FdoSmPhReader*) NULL;

    FdoSmPhPostGisMgrP   pgMgr = mgr->SmartCast<FdoSmPhPostGisMgr>();
    // NOTE: mloskot - We need to remove const qualifier first,
    // then cast to specialized type.
    FdoSmPhPostGisOwner* pgOwner = 
        static_cast<FdoSmPhPostGisOwner*>(
            const_cast<FdoSmPhOwner*>(owner));

    FdoStringP objectName = (dbObject ? dbObject->GetName() : L"");
    FdoStringP ownerName = owner->GetName();

    //
    // Generate SQL statement to get foreign keys
    //
    FdoStringP sql = FdoStringP::Format(
	    L"select R.constraint_name, K1.table_name, K1.column_name,\n"
	    L" R.unique_constraint_schema as r_owner_name,\n"
	    L" K2.table_name as r_table_name,\n"
	    L" K2.column_name as r_column_name\n"
	    L" from INFORMATION_SCHEMA.referential_constraints R,\n"
	    L" INFORMATION_SCHEMA.key_column_usage K1,\n"
	    L" INFORMATION_SCHEMA.key_column_usage K2\n"
	    L" where R.constraint_catalog = K1.constraint_catalog\n"
	    L"   and R.constraint_schema = K1.constraint_schema\n"
	    L"   and R.constraint_name = K1.constraint_name\n"
	    L"   and R.unique_constraint_catalog = K2.constraint_catalog\n"
	    L"   and R.unique_constraint_schema = K2.constraint_schema\n"
	    L"   and R.unique_constraint_name = K2.constraint_name\n"
	    L"   and K1.ordinal_position = K2.ordinal_position\n"
	    L"   and R.constraint_schema = $1\n"
	    L"%ls"
	    L" order by %ls, %ls, %ls, K1.ordinal_position",
        dbObject ? L"   and K1.table_name = $2\n" : L"",
        (FdoString*) pgMgr->FormatCollateColumnSql(L"K1.table_schema"),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"K1.table_name"),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"R.constraint_name")
    );


    // Create a field object for each field in the select list
    FdoSmPhRowsP rows = MakeRows(mgr);

    // Create and set the bind variables
    FdoSmPhRowP binds(new FdoSmPhRow(mgr, L"Binds"));
    FdoSmPhDbObjectP rowObj(binds->GetDbObject());

    FdoSmPhFieldP field(new FdoSmPhField(binds,
        L"constraint_schema",
        rowObj->CreateColumnDbObject(L"constraint_schema", false)));

    field->SetFieldValue(ownerName);

    if (dbObject)
    {
        field = new FdoSmPhField(binds, L"table_name",
            rowObj->CreateColumnDbObject(L"table_name", false));

        field->SetFieldValue(objectName);
    }

    //TODO: cache this query to make full use of the binds.
    FdoSmPhRdGrdQueryReader* reader = NULL;
    reader = new FdoSmPhRdGrdQueryReader(
        FdoSmPhRowP(rows->GetItem(0)), sql, mgr, binds);

    return reader;
}

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
#include "IndexReader.h"
#include "../Mgr.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdPostGisIndexReader::FdoSmPhRdPostGisIndexReader(FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP dbObject)
    : FdoSmPhRdIndexReader(),
      mDbObject(dbObject)
{
    const FdoSmPhOwner* owner = NULL;
    owner = static_cast<const FdoSmPhOwner*>(dbObject->GetParent());

    SetSubReader(MakeReader(mgr, owner, dbObject));
}

FdoSmPhRdPostGisIndexReader::FdoSmPhRdPostGisIndexReader(FdoSmPhMgrP mgr,
    FdoSmPhOwnerP owner)
    : FdoSmPhRdIndexReader(NULL)
{
    FdoSmPhOwner* smPhOwner = owner.p;

    SetSubReader(MakeReader(mgr, smPhOwner, NULL));
}

FdoSmPhRdPostGisIndexReader::~FdoSmPhRdPostGisIndexReader()
{
    // idle
}

FdoSmPhReaderP FdoSmPhRdPostGisIndexReader::MakeReader(FdoSmPhMgrP mgr,
    const FdoSmPhOwner* owner,
    FdoSmPhDbObjectP dbObject)
{

    //BR TODO:
    return (FdoSmPhReader*)NULL;


    FdoStringP objectName = (dbObject ? dbObject->GetName() : L"");
    FdoStringP ownerName = owner->GetName();

    FdoSmPhPostGisMgrP   pgMgr = mgr->SmartCast<FdoSmPhPostGisMgr>();

    //
    // TODO: mloskot - Find out equivalent to non-standard
    // table information_schema.statistics available in MySQL
    //

    // Generate SQL statement for selecting the indexes and their columns

    // TODO: mloskot - MySQL SQL left as a placeholder

    FdoStringP sql = FdoStringP::Format(
        L"select index_name, table_name, column_name, if(non_unique>0,'NONUNIQUE','UNIQUE') as uniqueness, index_type \n"
        L"  from INFORMATION_SCHEMA.statistics\n"
        L"  where\n"
        L"    1 = 1\n"
        L"    %ls\n"
        L"  order by %ls, %ls, seq_in_index",
        (FdoString*) pgMgr->FormatCollateColumnSql(L"table_name"),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"index_name"),
        dbObject ? L"and table_name = $1" : L""
    );

    // Create a field object for each field in the select list
    FdoSmPhRowsP rows(MakeRows(mgr));

    // Create and set the bind variables
    FdoSmPhRowP binds(new FdoSmPhRow(mgr, L"Binds"));

    //FdoSmPhFieldP field(new FdoSmPhField(binds,
    //    L"table_schema",
    //    binds->CreateColumnDbObject(L"table_schema", false)));

    //field->SetFieldValue(ownerName);

    if (dbObject)
    {
       FdoSmPhFieldP field = new FdoSmPhField(binds,
            L"table_name",
            binds->CreateColumnDbObject(L"table_name", false));

        field->SetFieldValue(objectName);
    }

//TODO: cache this query to make full use of the binds.
    FdoSmPhRdGrdQueryReader* reader = NULL;
    reader = new FdoSmPhRdGrdQueryReader(
        FdoSmPhRowP(rows->GetItem(0)), sql, mgr, binds );

    return reader;
}

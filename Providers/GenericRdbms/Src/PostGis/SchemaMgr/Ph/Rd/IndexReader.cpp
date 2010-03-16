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

FdoSmPhRdPostGisIndexReader::FdoSmPhRdPostGisIndexReader(FdoSmPhOwnerP owner,
    FdoSmPhDbObjectP dbObject)
    : FdoSmPhRdIndexReader()
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    if ( dbObject ) 
        objectNames->Add(dbObject->GetName());
    
    SetSubReader(MakeReader(owner,objectNames));
}

FdoSmPhRdPostGisIndexReader::FdoSmPhRdPostGisIndexReader(FdoSmPhOwnerP owner,
    FdoStringsP objectNames)
    : FdoSmPhRdIndexReader(NULL)
{
    FdoSmPhOwner* smPhOwner = owner.p;

    SetSubReader(MakeReader(owner, objectNames));
}

FdoSmPhRdPostGisIndexReader::~FdoSmPhRdPostGisIndexReader()
{
    // idle
}

FdoSmPhReaderP FdoSmPhRdPostGisIndexReader::MakeReader(FdoSmPhOwnerP owner,
    FdoStringsP objectNames)
{

    //BR TODO:
    //return (FdoSmPhReader*)NULL;

    FdoSmPhMgrP mgr(owner->GetManager());
    
    FdoSmPhPostGisMgr* pgMgr = NULL;
    pgMgr = static_cast<FdoSmPhPostGisMgr*>(mgr.p);

    // Generate SQL statement for selecting the indexes and their columns

    FdoStringP sqlString = FdoStringP::Format(
        L"select (N.nspname || '.' || CI.relname) as index_name, \n"
        L" (N.nspname || '.' || C.relname ) as table_name, \n"
        L" I.indkey as column_name, \n"
        L" CASE \n"
        L"   WHEN indisunique THEN 'UNIQUE'::text \n" 
        L"   ELSE 'NONUNIQUE'::text \n" 
        L" END as uniqueness, "
        L" NULL::text as index_type, \n"
        L" %ls as collate_schema_name, "
        L" %ls as collate_table_name, "
        L" %ls as collate_index_name "
        L" from pg_catalog.pg_index I, pg_class CI, pg_class C, pg_namespace N  $(JOIN_FROM)\n"
        L" where\n"
        L"    I.indexrelid = CI.oid and I.indrelid = C.oid and c.relnamespace = N.oid\n"
        L"    $(AND) $(QUALIFICATION)\n"
        L" ORDER BY collate_schema_name, collate_table_name, collate_index_name ASC ",
        (FdoString*) pgMgr->FormatCollateColumnSql(L"N.nspname"),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"C.relname"),
        (FdoString*) pgMgr->FormatCollateColumnSql(L"CI.relname")
    );

    FdoSmPhReaderP reader = FdoSmPhRdIndexReader::MakeQueryReader(
        L"",
        owner,
        sqlString,
        L"N.nspname",
        L"C.relname",
        objectNames,
        NULL
    );

    return reader;
}

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
#include "OwnerReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"

FdoSmPhRdPostGisOwnerReader::FdoSmPhRdPostGisOwnerReader(
    FdoSmPhDatabaseP database,
    FdoStringP ownerName)
    : FdoSmPhRdOwnerReader((FdoSmPhReader*) NULL,
        database, ownerName)
{
    SetSubReader(
        MakeQueryReader(database, ownerName)
    );
}

FdoSmPhRdPostGisOwnerReader::~FdoSmPhRdPostGisOwnerReader()
{
    // idle
}

FdoSmPhReaderP FdoSmPhRdPostGisOwnerReader::MakeQueryReader(
    FdoSmPhDatabaseP database,
    FdoStringP ownerName)
{
    bool isOwnerSet = true;
    
    FdoSmPhMgrP mgr(database->GetManager());

    FdoSmPhPostGisMgr* pgMgr = NULL;
    pgMgr = static_cast<FdoSmPhPostGisMgr*>(mgr.p);

    if (0 == ownerName.GetLength())
        isOwnerSet = false;

    FdoSmPhReaderP reader;

    if (!reader)
    {
        // Generate SQL statement to query datastores (owners).
        // If no owner is given, query all datastores (schemas)
        // available in connected PostgreSQL server.

        // $1 - name of particular datastore (schema in PostgreSQL)

        FdoStringP sqlString;

//        L"WHERE s.schema_name NOT IN ('pg_catalog', 'public', 'information_schema') "
        /*
        sqlString = FdoStringP::Format(
            L"SELECT s.schema_name AS name "
            L"FROM information_schema.schemata AS s "
            L"WHERE s.schema_name NOT IN ('pg_catalog','information_schema') "
            L" %ls "
            L"ORDER BY Cast(s.schema_name as text) ASC",
            (isOwnerSet ? L"AND s.schema_name = $1" : L"")
        );
        */
        sqlString = FdoStringP::Format(
            L"SELECT d.datname AS name "
            L"FROM pg_catalog.pg_database d "
            L"JOIN pg_catalog.pg_roles r ON d.datdba = r.oid "
            L"WHERE d.datistemplate = 'f' "
            L" %ls "
            L"ORDER BY 1 ASC",
            (isOwnerSet ? L"AND d.datname = $1" : L"")
        );
        FdoSmPhRowsP rows = MakeRows(mgr);
        FdoSmPhRowP row = rows->GetItem(0);

        reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr,
            MakeBinds(mgr, ownerName));

    }
    else
    {
        // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* queryReader = NULL;
        queryReader = static_cast<FdoSmPhRdGrdQueryReader*>(reader.p);

        FdoSmPhRowP binds(queryReader->GetBinds());
		FdoSmPhFieldsP fields(binds->GetFields());

        if (isOwnerSet)
        {
            FdoSmPhFieldP field(fields->GetItem(L"owner_name"));
            field->SetFieldValue(ownerName);
        }

        queryReader->Execute();
    }

    return reader;
}

FdoSmPhReaderP FdoSmPhRdPostGisOwnerReader::MakeHasMetaSchemaReader(
    FdoSmPhDatabaseP database,
    FdoStringP ownerName 
)
{
    bool                 owner_set = true;
    FdoStringP           sqlString;
    FdoSmPhMgrP          mgr = database->GetManager();
    FdoSmPhPostGisMgr*   pMgr = (FdoSmPhPostGisMgr*)(FdoSmPhMgr*)mgr;

    if (ownerName.GetLength() == 0 )
        owner_set = false;

    FdoSmPhReaderP reader;
//TODO: cache the queries for performance
/*
    if ( object_set ) 
        reader = pMgr->GetOwnerReader(dblink_set);
    else
        reader = pMgr->GetOwnersReader(dblink_set);
*/
    // TODO: create constant for F_SCHEMAINFO
    if ( !reader ) {
        // Generate sql statement if not already done

        // information_schema tables use the utf8 character set with a 
        // case-insensitive collation. This causes problems with MySQL instances
        // on Linux, where database and table names are case-sensitive.
        // The following query overrides the collations to utf8_bin, which
        // is case-sensitive. 

        sqlString = FdoStringP::Format(
              L"select distinct table_schema as name \n"
              L" from information_schema.tables T\n"
              L" where T.table_name = 'f_schemainfo' \n"
              L"and T.table_schema = 'public'"
        );

        FdoSmPhRowsP rows = MakeRows( mgr );
        FdoSmPhRowP row = rows->GetItem(0);

        reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, NULL );
/*
        if ( object_set ) 
            pMgr->SetOwnerReader(reader, dblink_set);
        else
            pMgr->SetOwnersReader(reader, dblink_set);
*/
    }
    else {
        // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* pReader = (FdoSmPhRdGrdQueryReader*)(FdoSmPhReader*) reader;
        FdoSmPhRowP binds = pReader->GetBinds();
		FdoSmPhFieldsP	fields = binds->GetFields();

        if ( owner_set ) 
            FdoSmPhFieldP(fields->GetItem(L"owner_name"))->SetFieldValue(ownerName);

        pReader->Execute();
    }

    return reader;
}

FdoSmPhRowP FdoSmPhRdPostGisOwnerReader::MakeBinds(FdoSmPhMgrP mgr,
    FdoStringP ownerName)
{
    FdoSmPhRowP binds(new FdoSmPhRow(mgr, L"Binds"));
    FdoSmPhDbObjectP rowDbObj(binds->GetDbObject());

    if (ownerName.GetLength() > 0)
    {
        FdoSmPhFieldP field(new FdoSmPhField(binds, L"owner_name",
            rowDbObj->CreateColumnDbObject(L"owner_name", false)));

        field->SetFieldValue(ownerName); 
    }

    return binds;
}

FdoStringP FdoSmPhRdPostGisOwnerReader::GetDescription()
{
    FdoStringP ownerName(GetName());

    FdoSmPhMgrP mgr(mDatabase->GetManager());
    FdoSmPhRowP row(new FdoSmPhRow(mgr, L"fields")); 
    FdoSmPhDbObjectP rowDbObj(row->GetDbObject());

    FdoStringP sqlString = FdoStringP::Format(
        L"SELECT description "
        L"FROM %ls.f_schemainfo "
        L"WHERE schemaname = '%ls'",
        static_cast<FdoString*>(ownerName),
        static_cast<FdoString*>(ownerName));

    // Each field adds itself to the row.
    FdoSmPhFieldP field(new FdoSmPhField(row, L"description",
        row->CreateColumnDbObject(L"description", false)));
 
	FdoSmPhRowP binds(new FdoSmPhRow(mgr, L"Binds"));

    FdoSmPhRdGrdQueryReaderP queryReader(
        new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, binds));

	FdoStringP	description;
    
    if (queryReader->ReadNext())
    {
		description = queryReader->GetString(L"", "description");
    }

    return description;
}

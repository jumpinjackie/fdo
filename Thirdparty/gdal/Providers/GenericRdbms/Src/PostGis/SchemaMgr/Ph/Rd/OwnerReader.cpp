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
    : FdoSmPhRdOwnerReader(MakeQueryReader(database, ownerName),
        database, ownerName)
{
    // idle
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
            L"SELECT d.datname AS name, s.description as description "
            L"FROM pg_catalog.pg_database d "
            L"JOIN pg_catalog.pg_roles r ON d.datdba = r.oid "
            L"LEFT OUTER JOIN pg_catalog.pg_shdescription s ON d.oid = s.objoid "
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
    return new FdoSmPhReader(GetManager(), (FdoSmPhRowCollection*) NULL);
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

/// Returns true if current owner has MetaSchema tables.
FdoBoolean FdoSmPhRdPostGisOwnerReader::GetHasMetaSchema()
{
    bool hasMetaSchema = false;

    FdoStringP description = GetString(L"", L"description");

    if ( description.Mid(0, 13) == L"FDO Enabled: ")
        hasMetaSchema = true;

    return hasMetaSchema;
}


FdoStringP FdoSmPhRdPostGisOwnerReader::GetDescription()
{
    FdoStringP description = GetString(L"", L"description");

    if ( description.Mid(0, 13) == L"FDO Enabled: ")
        description = description.Mid(13, 9999);

    return description;
}

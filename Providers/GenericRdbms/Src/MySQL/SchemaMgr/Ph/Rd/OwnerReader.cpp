/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "stdafx.h"
#include "OwnerReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"

FdoSmPhRdMySqlOwnerReader::FdoSmPhRdMySqlOwnerReader(
    FdoSmPhDatabaseP database,
    FdoStringP       ownerName
) :
	FdoSmPhRdOwnerReader(MakeQueryReader(database,ownerName), database, ownerName)
{
}

FdoSmPhRdMySqlOwnerReader::~FdoSmPhRdMySqlOwnerReader(void)
{
}


FdoSmPhReaderP FdoSmPhRdMySqlOwnerReader::MakeQueryReader(
    FdoSmPhDatabaseP database,
    FdoStringP ownerName 
)
{
    bool                 owner_set = true;
    FdoStringP           sqlString;
    FdoSmPhMgrP          mgr = database->GetManager();
    FdoSmPhMySqlMgr*       pMgr = (FdoSmPhMySqlMgr*)(FdoSmPhMgr*)mgr;

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
              L"select schema_name as name, \n"
              L" (select table_name from information_schema.tables T where S.schema_name = T.table_schema and T.table_name = 'F_SCHEMAINFO') as schemas_table\n"
              L" from information_schema.schemata S\n"
              L" %ls\n"
              L" order by schema_name asc",
               owner_set ? L"where S.schema_name collate utf8_bin = ?" : L""
        );

        FdoSmPhRowsP rows = MakeRows( mgr );
        FdoSmPhRowP row = rows->GetItem(0);

        reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, MakeBinds(mgr,ownerName) );
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

        if ( owner_set ) 
            binds->RefFields()->RefItem(L"owner_name")->SetFieldValue(ownerName);

        pReader->Execute();
    }

    return reader;
}


FdoSmPhRowP FdoSmPhRdMySqlOwnerReader::MakeBinds( FdoSmPhMgrP mgr, FdoStringP ownerName )
{
    FdoSmPhRowP row = new FdoSmPhRow( mgr, L"Binds" );
    FdoSmPhDbObjectP rowObj = row->GetDbObject();

    if ( ownerName.GetLength() > 0 ) {
        FdoSmPhFieldP field = new FdoSmPhField(
            row, 
            L"owner_name",
            rowObj->CreateColumnDbObject(L"owner_name",false)
        );

        field->SetFieldValue(ownerName); 
    }

    return( row );
}

FdoStringP FdoSmPhRdMySqlOwnerReader::GetDescription()
{
    FdoStringP           sqlString;
    FdoSmPhMgrP          mgr = mDatabase->GetManager();

    FdoSmPhRowP			row = new FdoSmPhRow(mgr, L"fields"); 
    FdoSmPhDbObjectP	 rowObj = row->GetDbObject();

    FdoStringP ownerName = this->GetName();
//	FdoStringP qownerName = rowObj->GetDbQName(); CRASH!

    sqlString = FdoStringP::Format(
            L"select T.description from %ls.f_schemainfo T \n\
            where T.schemaname = '%ls'",
			(FdoString*)ownerName,
            (FdoString*)ownerName            
    );

    // Each field adds itself to the row.
    FdoSmPhFieldP pField = new FdoSmPhField(
        row, 
        L"description",
        row->CreateColumnDbObject(L"description",false)
    );
 
	FdoSmPhRowP binds = new FdoSmPhRow( mgr, L"Binds" );

    FdoSmPhRdGrdQueryReaderP pReader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, binds );

	FdoStringP	description;
    if ( pReader->ReadNext() )
		description = pReader->GetString(L"", "description");

    return ( description );
}

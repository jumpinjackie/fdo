/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

FdoSmPhRdSqsOwnerReader::FdoSmPhRdSqsOwnerReader(
    FdoSmPhDatabaseP database,
    FdoStringP       ownerName
) :
	FdoSmPhRdOwnerReader(MakeQueryReader(database,ownerName), database, ownerName)
{
}

FdoSmPhRdSqsOwnerReader::~FdoSmPhRdSqsOwnerReader(void)
{
	EndSelect();
}


FdoSmPhReaderP FdoSmPhRdSqsOwnerReader::MakeQueryReader(
    FdoSmPhDatabaseP database,
    FdoStringP ownerName 
)
{
    bool                 owner_set = true;
    FdoStringP           sqlString;
    FdoSmPhMgrP          mgr = database->GetManager();
    FdoSmPhSqsMgr*       pMgr = (FdoSmPhSqsMgr*)(FdoSmPhMgr*)mgr;

    if (ownerName.GetLength() == 0 )
        owner_set = false;

	FdoStringP readerName;
	FdoSmPhReaderP reader;

	if ( !owner_set )	{
		readerName = FdoStringP::Format(L"OwnerSqsReader");
		reader = mgr->GetStaticReader( readerName );
	}

    // TODO: create constant for F_SCHEMAINFO
    if ( !reader ) {
        // Generate sql statement if not already done

        sqlString = FdoStringP::Format(
              L"select name \n"
              L" from master.dbo.sysdatabases S \n"
              L" where databasepropertyex(name, 'Status')='ONLINE' %ls \n"
              L" order by name collate latin1_general_bin asc",
               owner_set ? L"and S.name = ?" : L""
        );

        FdoSmPhRowsP rows = MakeRows( mgr );
        FdoSmPhRowP row = rows->GetItem(0);

		reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, MakeBinds(mgr,ownerName) );
		/*if ( reader && !owner_set )
			mgr->SetStaticReader( readerName, reader );*/
	}
    else {
        // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* pReader = (FdoSmPhRdGrdQueryReader*)(FdoSmPhReader*) reader;

		FdoSmPhRowP binds = pReader->GetBinds();

		if ( owner_set ) {
			FdoSmPhFieldsP	fields = binds->GetFields();
			FdoSmPhFieldP(fields->GetItem(L"owner_name"))->SetFieldValue(ownerName);
		}

		pReader->Execute();
    }

    return reader;
}

FdoSmPhReaderP FdoSmPhRdSqsOwnerReader::MakeHasMetaSchemaReader(
    FdoSmPhDatabaseP database,
    FdoStringP ownerName 
)
{
    //This function is not used because this class overrides
    //GetHasMetaSchema.

    return (FdoSmPhReader*) NULL;
}

FdoBoolean FdoSmPhRdSqsOwnerReader::GetHasMetaSchema()
{
    FdoStringP           sqlString;
    FdoSmPhMgrP          mgr = mDatabase->GetManager();
    FdoSmPhSqsMgr*       pMgr = (FdoSmPhSqsMgr*)(FdoSmPhMgr*)mgr;
    GdbiConnection*      gdbiConn = pMgr->GetGdbiConnection();
	FdoBoolean found	 = false;

    FdoSmPhRdQueryReaderP rdr;
    FdoSmPhRowP row = new FdoSmPhRow( mgr, L"schemas_table" );

    // Get current owner
    FdoStringP owner = this->GetName();
    sqlString = FdoStringP::Format(
            L"select TABLE_SCHEMA as schemas_table from \"%ls\".INFORMATION_SCHEMA.TABLES T \n"
            L" where T.TABLE_NAME = 'f_schemainfo'",
            (const wchar_t*)owner            
    );
	try
	{
        rdr = mgr->CreateQueryReader( row, sqlString );
        found = rdr->ReadNext();
	}
	catch (FdoException *ex)	
	{
		int rc = gdbiConn->GetLastError();
		if (rc != RDBI_NOT_VALID_USER_IN_DATABASE)
			throw ex;
		else
			return false;
	}
		
    return ( found );
}

FdoStringP FdoSmPhRdSqsOwnerReader::GetDescription()
{
    FdoStringP	description;

    if ( GetHasMetaSchema() ) {
        FdoStringP           sqlString;
        FdoSmPhMgrP          mgr = mDatabase->GetManager();

        FdoSmPhRowP			row = new FdoSmPhRow(mgr, L"fields"); 
        FdoSmPhDbObjectP	 rowObj = row->GetDbObject();

        FdoStringP ownerName = this->GetName();
	    //FdoStringP qownerName = rowObj->GetDbQName(); CRASHES!

        sqlString = FdoStringP::Format(
                L"select T.description from %ls.dbo.f_schemainfo T \n"
                L" where T.schemaname = '%ls'",
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

        if ( pReader->ReadNext() )
		    description = pReader->GetString(L"", "description");
    }

    return ( description );
}

FdoSmPhRowP FdoSmPhRdSqsOwnerReader::MakeBinds( FdoSmPhMgrP mgr, FdoStringP ownerName )
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



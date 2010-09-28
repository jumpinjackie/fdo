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
#include "OraPkeyReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

FdoSmPhRdOraOdbcPkeyReader::FdoSmPhRdOraOdbcPkeyReader(
    FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdPkeyReader(MakeReader(mgr,(const FdoSmPhOwner*)(dbObject->GetParent()), dbObject)),
    mDbObject(dbObject)
{
}

FdoSmPhRdOraOdbcPkeyReader::FdoSmPhRdOraOdbcPkeyReader(
    FdoSmPhMgrP mgr,
    FdoSmPhOwnerP    owner
) :
    FdoSmPhRdPkeyReader(MakeReader(mgr,(FdoSmPhOwner*) owner, (FdoSmPhDbObject*) NULL))
{
}

FdoSmPhRdOraOdbcPkeyReader::~FdoSmPhRdOraOdbcPkeyReader(void)
{
	EndSelect();
}

FdoSmPhReaderP FdoSmPhRdOraOdbcPkeyReader::MakeReader(
    FdoSmPhMgrP mgr,
    const FdoSmPhOwner* owner,
    FdoSmPhDbObjectP    dbObject
)
{
    FdoStringP objectName = dbObject ? dbObject->GetName() : L"";
    FdoStringP ownerName = owner->GetName();
    FdoStringP databaseName = owner->GetParent()->GetName();

    bool dbSet = databaseName.GetLength() > 0;

	FdoStringP readerName = L"PkeyOraReader_NOWM";
	if ( databaseName.GetLength() > 0 )
		readerName += FdoStringP::Format(L"_%ls", (FdoString*)databaseName );

	FdoSmPhReaderP reader = mgr->GetStaticReader( readerName );

	if ( !reader )	{
		// Select from the right constraints view depending if the DB Object is 
		// OWM-enabled.
		FdoStringP allConstraintsName = L"all_constraints";

		// Generate SQL statement to get the primary key columns.\
		// If DbObject OWM-enabled skip VERSION column since it is OWM-generated.
		// TODO: change back to bind variables.
		FdoStringP sqlString = FdoStringP::Format(
			L"select PK.constraint_name as constraint_name, PK.table_name as table_name, CK.column_name as column_name\n"
			L"    from %ls%ls%ls  PK, all_cons_columns%ls%ls CK\n"
			L"    where (%ls = CK.table_name \n"
			L"      and PK.owner = CK.owner\n"
			L"      and PK.constraint_name  = CK.constraint_name)\n"
			L"      and PK.owner    = :1\n"
			L"      %ls\n"
			L"      and (PK.constraint_type = 'P' or PK.constraint_type = 'PU')\n"
			L"    order by PK.table_name, CK.position",
			(FdoString*) allConstraintsName,
			dbSet ? L"@" : L"",
			dbSet ? (FdoString*) databaseName : L"",
			dbSet ? L"@" : L"",
			dbSet ? (FdoString*) databaseName : L"",
			L"PK.table_name",
			dbObject ? L"and PK.table_name  = :2" : L""
		);

		// Create a field object for each field in the select list.
		FdoSmPhRowsP rows = MakeRows(mgr);

		// Create and set the bind variables
		FdoSmPhRowP binds = new FdoSmPhRow( mgr, L"Binds" );
		FdoSmPhDbObjectP rowObj = binds->GetDbObject();

		FdoSmPhFieldP field = new FdoSmPhField(
			binds,
			L"owner_name",
			rowObj->CreateColumnDbObject(L"owner_name",false)
		);

		field->SetFieldValue(ownerName);

		if ( dbObject ) {
			field = new FdoSmPhField(
				binds,
				L"object_name",
				rowObj->CreateColumnDbObject(L"object_name",false)
			);

			field->SetFieldValue(objectName);
		}
		reader =
			new FdoSmPhRdGrdQueryReader( rows->GetItem(0), sqlString, mgr, binds );

		if ( reader )
			mgr->SetStaticReader( readerName, reader );
	}
	else	{

		FdoSmPhRdGrdQueryReader* pReader = (FdoSmPhRdGrdQueryReader*)(FdoSmPhReader*) reader;
		pReader->EndSelect();
        FdoSmPhRowP binds = pReader->GetBinds();
		FdoSmPhFieldsP	pFields = binds->GetFields();
		FdoSmPhFieldP	pField = pFields->GetItem(L"owner_name");
		pField->SetFieldValue(ownerName);

        if ( dbObject ) 
		{
            FdoSmPhFieldP	pField = pFields->GetItem(L"object_name");
			pField->SetFieldValue(objectName);
		}

		pReader->Execute();
	}

    return( reader );
}

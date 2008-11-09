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
#include "OraDbObjectReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
//#include "../Mgr.h"
//#include "../Database.h"
//#include "../Owner.h"
#include <Sm/Ph/Rd/DbObjectBinds.h>

FdoSmPhRdOraOdbcDbObjectReader::FdoSmPhRdOraOdbcDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoStringP objectName
) :
    FdoSmPhRdDbObjectReader((FdoSmPhReader*) NULL, owner, objectName)
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    if ( objectName != L"" ) 
        objectNames->Add(objectName);
    SetSubReader(MakeQueryReader(owner,objectNames));

}

FdoSmPhRdOraOdbcDbObjectReader::FdoSmPhRdOraOdbcDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames
) :
    FdoSmPhRdDbObjectReader((FdoSmPhReader*) NULL, owner, L"")
{
    SetSubReader(MakeQueryReader(owner,objectNames));

}

FdoSmPhRdOraOdbcDbObjectReader::FdoSmPhRdOraOdbcDbObjectReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join
) :
    FdoSmPhRdDbObjectReader((FdoSmPhReader*) NULL, owner, L"")
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    SetSubReader(MakeQueryReader(owner,objectNames,join));

}

FdoSmPhRdOraOdbcDbObjectReader::~FdoSmPhRdOraOdbcDbObjectReader(void)
{
	EndSelect();
}

bool FdoSmPhRdOraOdbcDbObjectReader::ReadNext()
{
    bool found = false;

    while ( !found ) {
        if ( !FdoSmPhRdDbObjectReader::ReadNext() )
            return false;

        FdoStringP objectName = GetString( L"", L"name" );
        FdoStringP objectType = GetString( L"", L"type" );
        
        
        // Check if the object is an OWM-managed object. These are hidden from
        // the caller.

        // The following checks if the object name has an OWM-specific suffix.
        // If it does then the suffix is stripped off and we check if the result
        // is the name of an OWM view on a table. If it is then the current
        // object is skipped.

        FdoStringP owmViewName;
        FdoStringP suffix = objectName.Mid( objectName.GetLength() - 3, 9999 );

        if ( (suffix == L"_LT") || (suffix == L"_MW") ) {
            owmViewName = objectName.Mid( 0, objectName.GetLength() - 3 );
        }
        else {
            suffix = objectName.Mid( objectName.GetLength() - 4, 9999 );

            if ( (suffix == L"_AUX") || (suffix == L"_LTS") || (suffix == L"_PKC") || (suffix == L"_PKD") ) {
                owmViewName = objectName.Mid( 0, objectName.GetLength() - 4 );
            }
            else {
                suffix = objectName.Mid( objectName.GetLength() - 5, 9999 );
                if ( (suffix == L"_BASE") || (suffix == L"_BPKC") || (suffix == L"_CONF") || (suffix == L"_CONS") ||
                     (suffix == L"_DIFF") || (suffix == L"_PKDB") || (suffix == L"_PKDC")
                ) 
                    owmViewName = objectName.Mid( 0, objectName.GetLength() - 5 );
            }
        }
        
        if ( objectType == L"table" )
            mObjectType = FdoSmPhDbObjType_Table;
        else if ( objectType == L"view" )
            mObjectType =  FdoSmPhDbObjType_View;
        else if ( objectType == L"index" )
            mObjectType =  FdoSmPhDbObjType_Index;
        else if ( objectType == L"synonym" ) {
            mObjectType =  FdoSmPhDbObjType_Provider;
        }
        else if ( objectType == L"sequence" ) {
            mObjectType =  FdoSmPhDbObjType_Provider;
        }
        else 
            mObjectType = FdoSmPhDbObjType_Unknown;

        found = true;
    }

    return true;
}

FdoSmPhDbObjType FdoSmPhRdOraOdbcDbObjectReader::GetType()
{
    return mObjectType;
}

FdoSmPhReaderP FdoSmPhRdOraOdbcDbObjectReader::MakeQueryReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames,
    FdoSmPhRdTableJoinP join
)
{
    bool                 dblink_set = true;
    FdoStringP           sqlString;
    FdoStringP           ownerName = owner->GetName();
    FdoStringP           dblinkName = owner->GetParent()->GetName();
    FdoSmPhMgrP          mgr = owner->GetManager();
	bool				 object_set = objectNames->GetCount() > 0;

    if (dblinkName.GetLength() == 0 )
        dblink_set = false;

	FdoStringP readerName = L"DbObjectOraReader";
	if ( object_set )
		readerName += FdoStringP::Format(L"_%ls", (objectNames->GetCount() > 1) ? L"MO" : L"SO");

	if ( !join )
		readerName += L"_NOWM";

	if ( dblinkName.GetLength() > 0 )
		readerName += FdoStringP::Format(L"_%ls", (FdoString*)dblinkName );

#ifdef ODBCORA_USE_STATIC_CURSORS
    // Using static cursors is currently turned off due to a defect
    // in ODBC with re-use of bound values.
    FdoSmPhReaderP reader = mgr->GetStaticReader ( readerName );
#else
    FdoSmPhReaderP reader;
#endif
    if ( !reader ) {
        // Create binds for owner and optional object names
        FdoSmPhRdDbObjectBindsP binds = new FdoSmPhRdDbObjectBinds(
            mgr,
            L"O.owner",
            L"owner_name",
            L"O.object_name",
            L"object_name",
            ownerName,
            objectNames
        );


        // Generate sql statement if not already done

        // If joining to another table, generated from sub-clause for table.
        FdoStringP joinFrom;
        if ( join != NULL ) 
            joinFrom = FdoStringP::Format( L"  , %ls\n", (FdoString*) join->GetFrom() );

        // Get where clause for owner and object name binds.
        FdoStringP qualification = binds->GetSQL();
		
        if ( join != NULL ) {
            // If joining to another table, add generated join clause.
            qualification += FdoStringP::Format( L"  and (%ls)\n", (FdoString*) join->GetWhere(L"O.object_name") );
        }
        else {
            if ( objectNames->GetCount() == 0 ) 
                // Not selecting specific tables so need to filter out some that FDO doesn't need to handle.
                // Skip MDRT tables created by spatial indexes and BIN$ tables created by Oracle recycle bin facility.
                qualification += L"  and upper(secondary) = 'N' and O.object_name not like 'BIN$%'\n";
        }

        sqlString = FdoStringP::Format(
              L"select %ls O.object_name as name, lower(O.object_type) as type\n"
              L" from all_objects%ls%ls O\n"
              L"%ls"
              L" where\n"
              L" %ls"
              L" and O.object_type in('TABLE', 'VIEW', 'SYNONYM', 'SEQUENCE')\n"
              L" order by O.object_name asc",
               join ? L"distinct" : L"",
               dblink_set ? L"@" : L"",
               dblink_set ? (FdoString*) dblinkName : L"",
               (FdoString*) joinFrom,
               (FdoString*) qualification
        );

        FdoSmPhRowsP rows = MakeRows( mgr );
        FdoSmPhRowP row = rows->GetItem(0);
        reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, binds->GetBinds() );

#ifdef ODBCORA_USE_STATIC_CURSORS
		if (reader && !join)
			mgr->SetStaticReader ( readerName, reader );
#endif
    }
#ifdef ODBCORA_USE_STATIC_CURSORS
    else {
        // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* pReader = (FdoSmPhRdGrdQueryReader*)(FdoSmPhReader*) reader;
		pReader->EndSelect();

		FdoSmPhRowP bindRows = pReader->GetBinds();
		if (	bindRows )
			FdoSmPhRdDbObjectBindsP binds = new FdoSmPhRdDbObjectBinds(
				mgr,
				L"O.owner",
				L"owner_name",
				L"O.object_name",
				L"object_name",
				ownerName,
				objectNames,
				bindRows,
				true
			);
		/*FdoInt32 bindOffset = binds->GetFields()->IndexOf(L"owner_name");
		binds->GetFields()->GetItem(bindOffset)->SetFieldValue(ownerName);
		FdoInt32 i = 0;
		if (objectNames->GetCount() > 0)	{
			for (i=0; i < objectNames->GetCount(); i++)
				binds->GetFields()->GetItem(i + 1 + bindOffset)->SetFieldValue(objectNames->GetString(i));
		}*/

        pReader->Execute();
    }
#endif

    return reader;
}

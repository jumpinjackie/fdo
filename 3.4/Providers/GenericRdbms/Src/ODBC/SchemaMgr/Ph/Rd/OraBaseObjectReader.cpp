/*
 * (C) Copyright 2007 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 *
 */

#include "stdafx.h"
#include "OraBaseObjectReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"
#include "../Database.h"
#include "../Owner.h"
#include <Sm/Ph/Rd/DbObjectBinds.h>

FdoSmPhRdOdbcOraBaseObjectReader::FdoSmPhRdOdbcOraBaseObjectReader(
    FdoSmPhOwnerP owner
) :
    FdoSmPhRdBaseObjectReader((FdoSmPhReader*) NULL, owner)
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdOdbcOraBaseObjectReader::FdoSmPhRdOdbcOraBaseObjectReader(
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdBaseObjectReader((FdoSmPhReader*) NULL, dbObject)
{
    FdoSmPhOwnerP owner = FDO_SAFE_ADDREF((FdoSmPhOwner*)(FdoSmPhDbElement*)dbObject->GetParent());
    FdoStringsP objectNames = FdoStringCollection::Create();
    objectNames->Add(dbObject->GetName());
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdOdbcOraBaseObjectReader::FdoSmPhRdOdbcOraBaseObjectReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames
) :
    FdoSmPhRdBaseObjectReader((FdoSmPhReader*) NULL, owner)
{
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdOdbcOraBaseObjectReader::FdoSmPhRdOdbcOraBaseObjectReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join
) :
    FdoSmPhRdBaseObjectReader((FdoSmPhReader*) NULL, owner)
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    SetSubReader(MakeQueryReader(owner,objectNames,join));
}

FdoSmPhRdOdbcOraBaseObjectReader::~FdoSmPhRdOdbcOraBaseObjectReader(void)
{
	EndSelect();
}

FdoSmPhReaderP FdoSmPhRdOdbcOraBaseObjectReader::MakeQueryReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames,
    FdoSmPhRdTableJoinP join
)
{
    bool                 dblink_set = true;
    FdoStringP           sqlString;
    FdoStringP           owmViewString;
    FdoStringP           ownerName = owner->GetName();
    FdoStringP           dblinkName = owner->GetParent()->GetName();
    FdoSmPhMgrP          mgr = owner->GetManager();
	bool				 object_set = objectNames->GetCount() > 0;

    if (dblinkName.GetLength() == 0 )
        dblink_set = false;

	FdoStringP readerName = L"OdbcOraBaseObjectReader";
    // Differentiate reader name based on Multiple Objects (MO) versus Single Objects (SO).
    // Different names will be used for reader pooling when calling SetStaticReader().
	if ( object_set )
		readerName += FdoStringP::Format(L"_%ls", (objectNames->GetCount() > 1) ? L"MO" : L"SO");

	if ( dblinkName.GetLength() > 0 )
		readerName += FdoStringP::Format(L"_%ls", (FdoString*)dblinkName );

    FdoSmPhReaderP reader = mgr->GetStaticReader ( readerName );

    if ( !reader ) {
        // Create binds for owner and optional object names

        FdoSmPhRdDbObjectBindsP binds = new FdoSmPhRdDbObjectBinds(
            mgr,
            L"D.owner",
            L"owner_name",
            L"D.name",
            L"name",
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

        sqlString = FdoStringP::Format(
              L"select %ls D.name, D.referenced_name as base_name,\n"
              L" D.referenced_owner as base_owner, D.referenced_link_name as base_database\n"
              L" from all_dependencies%ls%ls D\n"
              L"%ls"
              L" where\n"
              L" %ls"
              L" and D.type in('TABLE', 'VIEW', 'SYNONYM')\n"
              L" and D.referenced_type in('TABLE', 'VIEW', 'SYNONYM')\n"
              L" order by D.name asc",
               join ? L"distinct" : L"",
               dblink_set ? L"@" : L"",
               dblink_set ? (FdoString*) dblinkName : L"",
               (FdoString*) joinFrom,
               (FdoString*) qualification
        );

        FdoSmPhRowsP rows = MakeRows( mgr );
        FdoSmPhRowP row = rows->GetItem(0);

        reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, binds->GetBinds() );

		if (reader && !join)
			mgr->SetStaticReader ( readerName, reader );
    }
    else {
        // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* pReader = (FdoSmPhRdGrdQueryReader*)(FdoSmPhReader*) reader;
		pReader->EndSelect();

		FdoSmPhRowP bindRows = pReader->GetBinds();
		if ( bindRows )
			FdoSmPhRdDbObjectBindsP binds = new FdoSmPhRdDbObjectBinds(
				mgr,
                L"D.owner",
                L"owner_name",
                L"D.name",
                L"name",
                ownerName,
                objectNames,
				bindRows,
				true
			);

        pReader->Execute();
    }

    return reader;
}


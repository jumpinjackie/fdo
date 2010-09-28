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
#include "BaseObjectReader.h"
#include "DbObjectBinds.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"

FdoSmPhRdSqsBaseObjectReader::FdoSmPhRdSqsBaseObjectReader(
    FdoSmPhOwnerP owner
) :
    FdoSmPhRdBaseObjectReader((FdoSmPhReader*) NULL, owner)
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdSqsBaseObjectReader::FdoSmPhRdSqsBaseObjectReader(
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdBaseObjectReader((FdoSmPhReader*) NULL, dbObject)
{
    FdoSmPhOwnerP owner = FDO_SAFE_ADDREF((FdoSmPhOwner*)(FdoSmPhDbElement*)dbObject->GetParent());
    FdoStringsP objectNames = FdoStringCollection::Create();
    objectNames->Add(dbObject->GetName());
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdSqsBaseObjectReader::FdoSmPhRdSqsBaseObjectReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames
) :
    FdoSmPhRdBaseObjectReader((FdoSmPhReader*) NULL, owner)
{
    SetSubReader(MakeQueryReader(owner,objectNames));
}

FdoSmPhRdSqsBaseObjectReader::FdoSmPhRdSqsBaseObjectReader(
    FdoSmPhOwnerP owner,
    FdoSmPhRdTableJoinP join
) :
    FdoSmPhRdBaseObjectReader((FdoSmPhReader*) NULL, owner)
{
    FdoStringsP objectNames = FdoStringCollection::Create();
    SetSubReader(MakeQueryReader(owner,objectNames,join));
}

FdoSmPhRdSqsBaseObjectReader::~FdoSmPhRdSqsBaseObjectReader(void)
{
}

FdoStringP FdoSmPhRdSqsBaseObjectReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    FdoStringP fieldValue;

    if ( fieldName == L"name" ) {
        FdoStringP dbObjectName = FdoSmPhRdBaseObjectReader::GetString( tableName, fieldName ); 
        FdoStringP userName = FdoSmPhRdBaseObjectReader::GetString( tableName, L"view_schema" ); 

        fieldValue = userName + L"." + dbObjectName;        
    }
    else if ( fieldName == L"base_name" ) {
        FdoStringP dbObjectName = FdoSmPhRdBaseObjectReader::GetString( tableName, fieldName ); 
        FdoStringP userName = FdoSmPhRdBaseObjectReader::GetString( tableName, L"table_schema" ); 

        fieldValue = userName + L"." + dbObjectName;        
    }
    else {
        fieldValue = FdoSmPhRdBaseObjectReader::GetString( tableName, fieldName );        
    }

    return fieldValue;
}

FdoSmPhRowsP FdoSmPhRdSqsBaseObjectReader::MakeRows( FdoSmPhMgrP mgr )
{
    // Call superclass to populate generic row/columns:
    FdoSmPhRowsP rows = FdoSmPhRdBaseObjectReader::MakeRows(mgr);
        
    FdoSmPhRowP row = rows->GetItem(L"BaseObjectFields");

    // Each field adds itself to the row.
    FdoSmPhFieldP pField;
    pField = new FdoSmPhField(row, L"view_schema", row->CreateColumnDbObject(L"view_schema", false));
    pField = new FdoSmPhField(row, L"table_schema", row->CreateColumnDbObject(L"table_schema", false));

    return( rows);
}

FdoSmPhReaderP FdoSmPhRdSqsBaseObjectReader::MakeQueryReader(
    FdoSmPhOwnerP owner,
    FdoStringsP objectNames,
    FdoSmPhRdTableJoinP join
)
{
    FdoStringP           sqlString;
    FdoStringP           ownerName = owner->GetName();
    FdoSmPhMgrP          mgr = owner->GetManager();
    FdoSmPhSqsMgr*       pMgr = (FdoSmPhSqsMgr*)(FdoSmPhMgr*)mgr;

    FdoSmPhReaderP reader;

//TODO: cache the queries for performance
/*
    if ( object_set )
        reader = pMgr->GetBaseObjectReader(dblink_set);
    else
        reader = pMgr->GetDbObjectsReader(dblink_set);
*/
    if ( !reader ) {
        FdoSmPhRowsP rows = MakeRows( mgr );
        FdoSmPhRowP row = rows->GetItem(0);

        if ( owner->GetElementState() == FdoSchemaElementState_Added ) {
            // The BaseObject query selects from owner.INFORMATION_SCHEMA tables so
            // it will fail when the owner does not yet exist. In this case create
            // an empty reader that returns no rows.
            reader = new FdoSmPhReader( mgr, rows );
        }
        else {
            // Generate sql statement if not already done

            // Create binds for owner and optional object names
            FdoSmPhRdSqsDbObjectBindsP binds = new FdoSmPhRdSqsDbObjectBinds(
                mgr,
                L"D.VIEW_SCHEMA",
                L"user_name",
                L"D.VIEW_NAME",
                L"name",
                objectNames
            );

            // If joining to another table, generated from sub-clause for table.
            FdoStringP joinFrom;
            if ( join != NULL ) 
                joinFrom = FdoStringP::Format( L"  , %ls\n", (FdoString*) join->GetFrom() );

            // Get where clause for owner and object name binds.
            FdoStringP qualification = binds->GetSQL();

            if ( join != NULL ) {
                // If joining to another table, add join clause.
                qualification = FdoStringP::Format( 
                    L"  %ls and ((D.VIEW_SCHEMA = 'dbo' and D.VIEW_NAME = %ls) or ((D.VIEW_SCHEMA + '.' + D.VIEW_NAME) = %ls))\n", 
                    (FdoString*) ((FdoSmPhRdJoin*)(join.p))->GetWhere(),
                    (FdoString*) join->GetJoinColumn(),
                    (FdoString*) join->GetJoinColumn()
                );
            }


            sqlString = FdoStringP::Format(
                L"select %ls D.VIEW_NAME collate latin1_general_bin as name, D.TABLE_NAME as base_name,\n"
                L" D.TABLE_CATALOG as base_owner, null as base_database,\n"
                L" D.VIEW_SCHEMA collate latin1_general_bin as view_schema, D.TABLE_SCHEMA as table_schema\n"
                L" from %ls.INFORMATION_SCHEMA.VIEW_TABLE_USAGE D\n"
                L" %ls\n"
                L" %ls\n"
                L" %ls \n"
                L" order by D.VIEW_SCHEMA collate latin1_general_bin asc, D.VIEW_NAME collate latin1_general_bin asc",
                join ? L"distinct" : L"",
                (FdoString*)ownerName,
                (FdoString*)joinFrom,
                (qualification == L"") ? L"" : L"where",
                (FdoString*) qualification
            );

            reader = new FdoSmPhRdGrdQueryReader(row, sqlString, mgr, binds->GetBinds() );
    /*
            if ( object_set )
                pMgr->SetBaseObjectReader(reader, dblink_set);
            else
                pMgr->SetDbObjectsReader(reader, dblink_set);
    */
        }
    }
    else {
        // Re-executing so update bind variables first.
        FdoSmPhRdGrdQueryReader* pReader = dynamic_cast<FdoSmPhRdGrdQueryReader*>((FdoSmPhReader*) reader);

        // pReader is NULL when it is an empty reader. No need to re-execute in this case.
        if ( pReader ) {
            FdoSmPhRowP bindRow = pReader->GetBinds();

            FdoSmPhRdSqsDbObjectBindsP binds = new FdoSmPhRdSqsDbObjectBinds(
                mgr,
                L"D.VIEW_SCHEMA",
                L"user_name",
                L"D.VIEW_NAME",
                L"name",
                objectNames,
                bindRow,
                true
            );

            pReader->Execute();
        }
    }

    return reader;
}

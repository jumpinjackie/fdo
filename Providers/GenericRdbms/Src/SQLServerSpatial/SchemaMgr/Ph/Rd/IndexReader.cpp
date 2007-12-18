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
#include "IndexReader.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "../Mgr.h"

FdoSmPhRdSqsIndexReader::FdoSmPhRdSqsIndexReader(
    FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP    dbObject
) :
    FdoSmPhRdIndexReader(NULL),
    mDbObject(dbObject)
{
    SetSubReader(MakeReader(mgr, (const FdoSmPhOwner*)(dbObject->GetParent()), dbObject));
}

FdoSmPhRdSqsIndexReader::FdoSmPhRdSqsIndexReader(
    FdoSmPhMgrP mgr,
    FdoSmPhOwnerP    owner
) :
    FdoSmPhRdIndexReader((FdoSmPhReader*) NULL)
{
    SetSubReader(MakeReader(mgr, (FdoSmPhOwner*)owner, (FdoSmPhDbObject*)NULL));
}
   
FdoSmPhRdSqsIndexReader::~FdoSmPhRdSqsIndexReader(void)
{
}

FdoStringP FdoSmPhRdSqsIndexReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    FdoStringP fieldValue;

    if ( fieldName == L"table_name" ) {
        FdoStringP dbObjectName = FdoSmPhRdIndexReader::GetString( tableName, fieldName ); 
        FdoStringP userName = FdoSmPhRdIndexReader::GetString( tableName, L"table_schema" ); 

        fieldValue = userName + L"." + dbObjectName;        
    }
    else {
        fieldValue = FdoSmPhRdIndexReader::GetString( tableName, fieldName );        
    }

    return fieldValue;
}

FdoSmPhReaderP FdoSmPhRdSqsIndexReader::MakeReader(
    FdoSmPhMgrP mgr,
    const FdoSmPhOwner* owner,
    FdoSmPhDbObjectP    dbObject
)
{
    FdoStringP objectName = dbObject ? dbObject->GetName() : L"";
    FdoStringP ownerName = owner->GetName();
    FdoSmPhSqsMgrP sqsMgr = mgr->SmartCast<FdoSmPhSqsMgr>();

    // Generate SQL statement for selecting the indexes and their columns
    // "indid between 1 and 250" filters out heap table and text column entries.
    FdoStringP sql = FdoStringP::Format(
        L"select b.name as index_name, a.name as table_name, d.name as column_name,\n"
        L"   CASE INDEXPROPERTY(b.id, b.name, 'IsUnique') \n"
        L"     WHEN 1 THEN 'UNIQUE' \n"
        L"     ELSE 'NONUNIQUE' \n"
        L"   END as uniqueness, \n"
        L"   e.name as table_schema \n"
        L"   from %ls.dbo.sysobjects a, \n"
        L"        %ls.dbo.sysindexes b, \n"
        L"        %ls.dbo.sysindexkeys c, \n"
        L"        %ls.dbo.syscolumns d, \n"
        L"        %ls.dbo.sysusers e \n"
        L"    where b.id = a.id \n"
        L"%ls"
        L"      and (b.indid between 1 and 250) \n"
        L"      and b.id = c.id and b.indid = c.indid\n"
        L"      and c.id = d.id and c.colid = d.colid\n"
        L"      and a.uid = e.uid\n"
        L"        order by e.name collate latin1_general_bin asc, a.name collate latin1_general_bin asc, b.name collate latin1_general_bin asc, c.keyno asc",
        (FdoString*)ownerName,
        (FdoString*)ownerName,
        (FdoString*)ownerName,
        (FdoString*)ownerName,
        (FdoString*)ownerName,
        dbObject ? L"      and e.name = ? and a.name = ?\n" : L""
    );

    // Create a field object for each field in the select list
    FdoSmPhRowsP rows = MakeRows(mgr);
    FdoSmPhRowP binds;

    FdoStringP userName;
    FdoStringP localObjectName;

    if ( objectName != L"" ) {
        if ( objectName.Contains(L".") ) {
            userName = objectName.Left(L".");
            localObjectName = objectName.Right(L".");
        }
        else {
            userName = L"dbo";
            localObjectName = objectName;
        }

        // Create and set the bind variables
        binds = new FdoSmPhRow( mgr, L"Binds" );

        FdoSmPhFieldP field = new FdoSmPhField(
            binds,
            L"user_name",
            binds->CreateColumnDbObject(L"table_name",false)
        );

        field->SetFieldValue (userName);

        field = new FdoSmPhField(
            binds,
            L"table_name",
            binds->CreateColumnDbObject(L"table_name",false)
        );

        field->SetFieldValue (localObjectName);
    }

    // INDEXPROPERTY function always returns null for indexes not in the active schema.
    // Therefore, owner (schema) for the indexes to retrieved must be the current one
    // in order to retrieve the uniqueness setting of each index.

    FdoSmPhOwnerP           activeOwner     = mgr->FindOwner();
    bool                    ownerSwitched   = false;

    if ( !activeOwner || (owner->GetQName().ICompare(activeOwner->GetQName()) != 0) ) {
        ((FdoSmPhOwner*)owner)->SetCurrent();
        ownerSwitched = true;
    }

    FdoSmPhRdGrdQueryReader* reader = NULL;

    try {
        //TODO: cache this query to make full use of the binds.
        reader = new FdoSmPhRdGrdQueryReader( rows->GetItem(0), sql, mgr, binds );
    }
    catch ( ... ) {
        // Switch back to original current schema, if any
        if ( ownerSwitched && activeOwner && (wcslen(activeOwner->GetName()) > 0) ) 
            activeOwner->SetCurrent();
        else
            mgr->GetDatabase()->UnsetCurrentOwner();

        throw;
    }

    // Switch back to original current schema, if any
    if ( ownerSwitched ) {
        // TODO: shouldn't need to check owner name, however have to fix 
        // FdoSmLpDatabase::FindOwner to not create dummy owner when there
        // is no active owner.
        if ( activeOwner && (wcslen(activeOwner->GetName()) > 0) ) 
            activeOwner->SetCurrent();
        else
            mgr->GetDatabase()->UnsetCurrentOwner();
    }

    return( reader );
}

FdoSmPhRowsP FdoSmPhRdSqsIndexReader::MakeRows( FdoSmPhMgrP mgr )
{
    FdoSmPhRowsP rows = FdoSmPhRdIndexReader::MakeRows( mgr );

    // Single row, no joins
    FdoSmPhRowP row = rows->GetItem(0);
    
    // Each field adds itself to the row.
    FdoSmPhFieldP field = new FdoSmPhField(
        row, 
        L"table_schema",
        row->CreateColumnDbObject(L"table_schema",false)
    );

    return( rows );
}


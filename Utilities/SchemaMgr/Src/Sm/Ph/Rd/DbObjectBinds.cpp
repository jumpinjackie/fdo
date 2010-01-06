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
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Rd/DbObjectBinds.h>

FdoSmPhRdDbObjectBinds::FdoSmPhRdDbObjectBinds(
    FdoSmPhMgrP mgr,
    FdoStringP ownerFieldName,
    FdoStringP ownerAlias,
    FdoStringP objectFieldName,
    FdoStringP objectAlias,
    FdoStringP ownerName,
    FdoStringsP objectNames,
    FdoSmPhRowP binds,
    bool rebind
) :
	FdoSmPhSchemaElement()
{
    FdoInt32 ix = 0;
    FdoInt32 objectIx;
    FdoInt32 bindOffset = 0;
    
    if ( binds ) {
        mBinds = binds;
    }
    else {
        // No bind fields passed in, create new collection.
        mBinds = new FdoSmPhRow( mgr, L"binds" );
    }

	FdoSmPhFieldsP pFields = mBinds->GetFields();

    // figure out offset of owner name bind field.
    if ( rebind ) {
        // If rebinding, look it up in bind fields collection
        bindOffset = pFields->IndexOf( ownerAlias );
    }
    else {
        // Not rebinding so owner bind will be added to the end of bind collection.
        bindOffset = pFields->GetCount();

        FdoSmPhDbObjectP rowObj = mBinds->GetDbObject();

        // Add owner name bind field.
        FdoSmPhFieldP field = new FdoSmPhField(
            mBinds,
            ownerAlias,
            rowObj->CreateColumnDbObject(ownerAlias,false)
        );

        // Add a bind field for each object name.
        for ( objectIx = 0; objectIx < objectNames->GetCount(); objectIx++ ) {
            FdoStringP bindFieldName = FdoStringP::Format( 
                L"%ls%d",
                (FdoString*) objectAlias,
                objectIx + 1
            );

            field = new FdoSmPhField(
                mBinds,
                bindFieldName,
                rowObj->CreateColumnDbObject(bindFieldName,false)
            );
        }
    }

    // Set bind value for owner
	FdoSmPhFieldP	pField = pFields->GetItem(bindOffset);
	pField->SetFieldValue( ownerName );

    // Set bind values for objects.
    for ( objectIx = 0; objectIx < objectNames->GetCount(); objectIx++ ) {
		FdoSmPhFieldP	pField = pFields->GetItem(objectIx + bindOffset + 1);
        pField->SetFieldValue( objectNames->GetString(objectIx) );
    }

    // Generate SQL

    // Owner bind first
    FdoStringP sqlOwnerBind = mgr->FormatBindField(ix + bindOffset);
    ix++;

    // Bind variable for each object name.
    FdoStringsP sqlObjectBinds = FdoStringCollection::Create();

    for ( objectIx = 0; objectIx < objectNames->GetCount(); objectIx++ ) {
        sqlObjectBinds->Add( mgr->FormatBindField(ix + bindOffset) );
        ix++;
    }

    // Format the assignment for owner name bind variable
    mSQL = FdoStringP::Format(
            L"%ls = %ls",
            (FdoString*) ownerFieldName,
            (FdoString*) sqlOwnerBind
    );

    // Add the object name bind variables to SQL.
    if ( objectNames->GetCount() > 0 ) {
        mSQL += FdoStringP::Format(
                L" and %ls in ( %ls )",
                (FdoString*) objectFieldName,
                (FdoString*) sqlObjectBinds->ToString()
        );
    }
}

FdoSmPhRdDbObjectBinds::~FdoSmPhRdDbObjectBinds(void)
{
}

FdoStringP FdoSmPhRdDbObjectBinds::GetSQL()
{
    return mSQL;
}

FdoSmPhRowP FdoSmPhRdDbObjectBinds::GetBinds()
{
    return mBinds;
}

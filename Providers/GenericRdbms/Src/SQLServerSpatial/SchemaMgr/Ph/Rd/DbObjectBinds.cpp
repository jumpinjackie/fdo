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
#include "DbObjectBinds.h"

FdoSmPhRdSqsDbObjectBinds::FdoSmPhRdSqsDbObjectBinds(
    FdoSmPhMgrP mgr,
    FdoStringP userFieldName,
    FdoStringP userAlias,
    FdoStringP objectFieldName,
    FdoStringP objectAlias,
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

    // figure out offset of owner name bind field.
    if ( rebind ) {
        // If rebinding, look it up in bind fields collection
        bindOffset = mBinds->RefFields()->IndexOf( userAlias );
    }
    else {
        // Not rebinding so owner bind will be added to the end of bind collection.
        bindOffset = mBinds->RefFields()->GetCount();

        FdoSmPhDbObjectP rowObj = mBinds->GetDbObject();

        // Add a user name and object name bind field for each object.
        for ( objectIx = 0; objectIx < objectNames->GetCount(); objectIx++ ) {
            FdoStringP bindFieldName = FdoStringP::Format( 
                L"%ls%d",
                (FdoString*) userAlias,
                objectIx + 1
            );

            FdoSmPhFieldP field = new FdoSmPhField(
                mBinds,
                bindFieldName,
                rowObj->CreateColumnDbObject(bindFieldName,false)
            );

            bindFieldName = FdoStringP::Format( 
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
	FdoSmPhFieldsP	fields = mBinds->GetFields();

    // Set bind values for objects.
    for ( objectIx = 0, ix = bindOffset; objectIx < objectNames->GetCount(); objectIx++ ) {
        FdoStringP fullObjectName = objectNames->GetString( objectIx );
        FdoStringP userName;
        FdoStringP objectName;

        // Parse out user name and object name parts from each full object name.
        if ( fullObjectName.Contains(L".") ) {
            // Full Object name contains user name, set object_schema to user name.
            userName = fullObjectName.Left(L".");
            objectName = fullObjectName.Right(L".");
        }
        else {
            // Object name not qualified by user name, default the user name to dbo.
            userName = L"dbo";
            objectName = fullObjectName;
        }

        FdoSmPhFieldP(fields->GetItem(ix++))->SetFieldValue( userName );
        FdoSmPhFieldP(fields->GetItem(ix++))->SetFieldValue( objectName );
    }

    // Generate SQL

    FdoStringsP sqlObjectBinds = FdoStringCollection::Create();

    // Object name binds next.
    for ( objectIx = 0, ix = bindOffset; objectIx < objectNames->GetCount(); objectIx++ ) {
        FdoStringP userBind =  mgr->FormatBindField(ix++);
        FdoStringP objectBind =  mgr->FormatBindField(ix++);

        sqlObjectBinds->Add( 
            FdoStringP::Format(
                L"(%ls = %ls and %ls = %ls)",
                (FdoString*) userFieldName,
                (FdoString*) userBind,
                (FdoString*) objectFieldName,
                (FdoString*) objectBind
            )
        );
    }

    if ( objectNames->GetCount() > 0 ) {
        mSQL += FdoStringP::Format(
                L" ( %ls )",
                (FdoString*) sqlObjectBinds->ToString( L" or " )
        );
    }
}

FdoSmPhRdSqsDbObjectBinds::~FdoSmPhRdSqsDbObjectBinds(void)
{
}

FdoStringP FdoSmPhRdSqsDbObjectBinds::GetSQL()
{
    return mSQL;
}

FdoSmPhRowP FdoSmPhRdSqsDbObjectBinds::GetBinds()
{
    return mBinds;
}

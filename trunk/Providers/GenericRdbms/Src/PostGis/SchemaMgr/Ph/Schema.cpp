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
#include "Schema.h"
#include "Owner.h"
#include "Rd/DbSchemaReader.h"
#include <Sm/Error.h>

FdoSmPhPostGisSchema::FdoSmPhPostGisSchema(
    FdoStringP name,
    const FdoSmPhOwner* pOwner,
    FdoSmPhRdPostGisDbSchemaReader* reader
) :
    FdoSmPhDbElement(
        name,
        (FdoSmPhMgr*) NULL, 
        pOwner,
        reader ? FdoSchemaElementState_Unchanged : FdoSchemaElementState_Added
    ),
    mIsUserDefault(false),
    mIsFixed(false),
    mIsSystem(false)
{
    // Read info from Reader that is specific to sql server tables/views/indexes/etc:
    if (reader != NULL)
    {
        mUserName    = reader->GetString(L"", L"user_name");
        mIsUserDefault = ( name == reader->GetString(L"", L"default_schema") );
        
        // IsFixed is set for special schemas owned by certain roles
        // (e.g. db_datareader)
        mIsFixed = ( reader->GetInteger(L"", L"is_fixed") == 1);

        // All special schemas, except public, are marked as 
        // system schemas. Feature Schemas are not generated from
        // system schemas but are for dbo and guest.
        if ( mIsFixed || 
             (name == L"INFORMATION_SCHEMA") ||
             (name == L"sys")
         )
            mIsSystem = true;
    }
}

FdoSmPhPostGisSchema::FdoSmPhPostGisSchema(void)
{
}

FdoSmPhPostGisSchema::~FdoSmPhPostGisSchema(void)
{
}

FdoStringP FdoSmPhPostGisSchema::GetUsrName() const
{
    return mUserName;
}

void FdoSmPhPostGisSchema::SetElementState(FdoSchemaElementState elementState)
{
    if ( elementState == FdoSchemaElementState_Deleted ) {
        // Feature Schemas are generate for public but 
        // we disallow deleting it since it's  created when the
        // datastore is created. 
        if ( IsSystem() || 
             (wcscmp(GetName(), L"public") == 0)
        ) {
            AddDeleteSystemSchemaError();
        }
    }

    FdoSmPhDbElement::SetElementState( elementState );
}

void FdoSmPhPostGisSchema::Discard()
{
    FdoSmPhPostGisOwner* owner = (FdoSmPhPostGisOwner*)(GetParent());

    owner->DiscardSchema( this );
}

bool FdoSmPhPostGisSchema::Add()
{
    FdoSmPhGrdOwner* owner = (FdoSmPhGrdOwner*)(GetParent());

    FdoStringP sqlStmt = FdoStringP::Format(
        L"create schema %ls",
        (FdoString*) GetDbName()
    );

    owner->ActivateAndExecute( (FdoString*) sqlStmt );

    return true;
}

bool FdoSmPhPostGisSchema::Modify()
{
    //Nothing to modify yet
    return true;
}

bool FdoSmPhPostGisSchema::Delete()
{
    FdoSmPhGrdOwner* owner = (FdoSmPhGrdOwner*)(GetParent());

    FdoStringP sqlStmt = FdoStringP::Format(
        L"drop schema %ls",
        (FdoString*) GetDbName()
    );

    owner->ActivateAndExecute( (FdoString*) sqlStmt );

    return true;
}

void FdoSmPhPostGisSchema::AddDeleteSystemSchemaError()
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            NlsMsgGet1(
                FDORDBMS_16,
                "Cannot delete system schema '%1$ls' from datastore.",
                GetName()
            )
		)
	);
}


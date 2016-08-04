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
#include "SchemaReader.h"
#include "../Owner.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"

#include <Sm/Error.h>
#include "DbSchemaReader.h"
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
        // we disallow deleting it since it was created when the
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


FdoSmPhRdPostGisSchemaReader::FdoSmPhRdPostGisSchemaReader(FdoSmPhRowsP rows, FdoSmPhOwnerP owner, bool dsInfo) :
	FdoSmPhRdSchemaReader(rows, owner, dsInfo),
    mOwner(owner),
    mCurrSchema(-1)
{
}


FdoSmPhRdPostGisSchemaReader::~FdoSmPhRdPostGisSchemaReader(void)
{
}

bool FdoSmPhRdPostGisSchemaReader::ReadNext()
{
    SetBOF(false);

    // Keep reading until the next non-system schema is found or
    // all rows have been read.
    for ( ; ; ) {
        mCurrSchema++;

        FdoSmPhPostGisOwnerP owner = mOwner->SmartCast<FdoSmPhPostGisOwner>();

        // Get all physical schemas.
        FdoSmPhPostGisSchemasP schemas = owner->GetSchemas();

        if ( mCurrSchema >= schemas->GetCount() ) {
            // All physical schemas have been looked at.
            SetEOF(true);
            return false;
        }

        FdoSmPhPostGisSchemaP schema = schemas->GetItem( mCurrSchema );
    
        // Don't make a logical schema out of special schemas such
        // as INFORMATION_SCHEMA.
        if ( schema->IsSystem() )
            continue;

        // We have a non-system physical schema so set the field values
        // from it.
        FdoSmPhRowsP pFroms = GetRows();
        FdoSmPhRowP pFrom;

        if (pFroms != NULL)
            pFrom = pFroms->GetItem(0);

        if ( pFrom ) {
		    FdoSmPhFieldsP pFields = pFrom->GetFields();
            FdoSmPhFieldP pField = pFields->GetItem(L"schemaname");

            if ( pField ) 
                pField->SetFieldValue( schema->GetName() );

            pField = pFields->GetItem(L"owner");

            if ( pField ) 
                pField->SetFieldValue( schema->GetUsrName() );
        }

        break;
    }

    return true;
}

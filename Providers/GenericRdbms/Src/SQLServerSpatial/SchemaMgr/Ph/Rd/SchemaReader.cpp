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

FdoSmPhRdSqsSchemaReader::FdoSmPhRdSqsSchemaReader(FdoSmPhRowsP rows, FdoSmPhOwnerP owner, bool dsInfo) :
	FdoSmPhRdSchemaReader(rows, owner, dsInfo),
    mOwner(owner),
    mCurrSchema(-1)
{
}


FdoSmPhRdSqsSchemaReader::~FdoSmPhRdSqsSchemaReader(void)
{
}

bool FdoSmPhRdSqsSchemaReader::ReadNext()
{
    SetBOF(false);

    // Keep reading until the next non-system schema is found or
    // all rows have been read.
    for ( ; ; ) {
        mCurrSchema++;

        FdoSmPhSqsOwnerP owner = mOwner->SmartCast<FdoSmPhSqsOwner>();

        // Get all physical schemas.
        FdoSmPhSqsSchemasP schemas = owner->GetSchemas();

        if ( mCurrSchema >= schemas->GetCount() ) {
            // All physical schemas have been looked at.
            SetEOF(true);
            return false;
        }

        FdoSmPhSqsSchemaP schema = schemas->GetItem( mCurrSchema );
    
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

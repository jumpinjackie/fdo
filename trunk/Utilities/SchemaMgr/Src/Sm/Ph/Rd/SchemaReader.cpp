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
#include <Sm/Ph/Rd/SchemaReader.h>
#include <Sm/Ph/Mgr.h>

FdoSmPhRdSchemaReader::FdoSmPhRdSchemaReader(FdoSmPhRowsP rows, FdoSmPhOwnerP owner, bool dsInfo) :
	FdoSmPhReader(owner->GetManager(), rows),
    mOwner(owner),
    mDsInfo(dsInfo)
{
}


FdoSmPhRdSchemaReader::~FdoSmPhRdSchemaReader(void)
{
}

bool FdoSmPhRdSchemaReader::ReadNext()
{
    if ( IsBOF() ) {
        // First read, autogenerate feature schema for 
        // current physical schema
        FdoSmPhRowsP pFroms = GetRows();
        FdoSmPhRowP pFrom;
        if (pFroms != NULL)
            pFrom = pFroms->GetItem(0);

        if ( pFrom ) {
			FdoSmPhFieldsP pFields = pFrom->GetFields();
            FdoSmPhFieldP pField = pFields->GetItem(L"schemaname");

            if ( mDsInfo ) {
                // When retrieving datastore information, just set 
                // schema name to datastore name.
                pField->SetFieldValue( mOwner->GetName() );
            }
            else {
                // FDO MetaSchema has a special  schema named the same as the default 
                // physical schema. Therefore, this feature schema needs to be prefixed to
                // prevent it from being named the same as this special schema.
                pField->SetFieldValue( mOwner->GetBestSchemaName() );
            }
        }

        SetBOF(false);
    }
    else {
        // Only one feature schema is read.
        SetEOF(true);
    }

	return(!IsEOF());
}


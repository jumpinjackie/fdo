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
#include <Sm/Ph/Rd/AssociationReader.h>

FdoSmPhRdAssociationReader::FdoSmPhRdAssociationReader(FdoSmPhRowsP froms, FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd, FdoSmPhMgrP physicalSchema) :
	FdoSmPhReader(physicalSchema, froms),
    mFkeyIdx(-1),
// LogicalPhysical schema reverses primary and foreign tables
    mPkTableName(fkTableName)
{
    mpTable = physicalSchema->FindDbObject(pkTableName)->SmartCast<FdoSmPhTable>();
    if ( !mpTable ) 
        SetEOF(true);
}


FdoSmPhRdAssociationReader::~FdoSmPhRdAssociationReader(void)
{
}

bool FdoSmPhRdAssociationReader::ReadNext()
{
    bool found = false;

    while ( !IsEOF() && (!found) ) {
        mFkeyIdx++;
        if ( mFkeyIdx >= mpTable->GetFkeysUp()->GetCount() ) {
            SetEOF(true);
            break;
        }

        FdoSmPhFkeyP pFkey = mpTable->GetFkeysUp()->GetItem(mFkeyIdx);
        FdoSmPhTableP pPkeyTable = pFkey->GetPkeyTable();

        FdoSmPhRowsP pFroms = this->GetRows();
        FdoSmPhRowP pFrom = pFroms ? pFroms->GetItem(0) : NULL;

        if ( pFrom && pPkeyTable ) {

            if ( mPkTableName.ICompare(pPkeyTable->GetName()) == 0 ) {
                // LogicalPhysical schema reverses primary and foreign tables
                // so that is why the pk is set from the fk and vice versa.
                FdoSmPhFieldP pField = pFrom->GetFields()->GetItem(L"fktablename");
                pField->SetFieldValue( pPkeyTable->GetName() );

                pField = pFrom->GetFields()->GetItem(L"fkcolumnnames");
                pField->SetFieldValue( pFkey->GetPkeyColumns()->ToString() );

                pField = pFrom->GetFields()->GetItem(L"pktablename");
                pField->SetFieldValue( mpTable->GetName() );

                pField = pFrom->GetFields()->GetItem(L"pkcolumnnames");
                pField->SetFieldValue( pFkey->GetFkeyColumns()->ToString() );

                found = true;
            }
        }

        SetBOF(false);
    }

	return(!IsEOF());
}


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
#include <Sm/Ph/Index.h>
#include <Sm/Error.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Table.h>
#include <Sm/Ph/Rd/ColumnReader.h>

FdoSmPhIndex::FdoSmPhIndex(
        FdoStringP name, 
        const FdoSmPhTable* pTable,
        bool isUnique,
		FdoSchemaElementState elementState
) : 
    FdoSmPhDbObject( name, (const FdoSmPhOwner*) pTable->GetParent(), elementState ),
    mpTable(pTable),
	mIsUnique(isUnique)
{
}

FdoSmPhIndex::~FdoSmPhIndex(void)
{
}


void FdoSmPhIndex::AddColumn( FdoSmPhColumnP pColumn )
{
	FdoSmPhColumnsP	pColumns = GetColumns();
    pColumns->Add( pColumn );
}

long FdoSmPhIndex::GetWeight()
{
    long ixSize = 0;
    int idx;
    FdoSmPhColumnsP idxCols = GetColumns();

    for ( idx = 0; idx < idxCols->GetCount(); idx++ ) {
        FdoSmPhColumnP col = idxCols->GetItem(idx);

        switch ( col->GetType() ) {
        case FdoSmPhColType_Date:
            ixSize += mDateWeight;
            break;
        
        // Use size in bytes for most scalar columns.
        case FdoSmPhColType_Bool:
            ixSize += 1;
            break;

        case FdoSmPhColType_Byte:
            ixSize += 1;
            break;

        case FdoSmPhColType_Int16:
            ixSize += 2;
            break;

        case FdoSmPhColType_Int32:
            ixSize += 4;
            break;

        case FdoSmPhColType_Int64:
        case FdoSmPhColType_Decimal:
        case FdoSmPhColType_Single:
        case FdoSmPhColType_Double:
            ixSize += col->GetBindSize();
            break;

        case FdoSmPhColType_String:
            // Subtract 1 to get string length without null terminator.
            ixSize += (col->GetBindSize() - 1);
            break;

        default:
            // If we're here this type of column is not an appropriate identity column
            // (e.g. blob). Set a high score so that its index will not be chosen as the
            // object identity.
            ixSize += (mMaxWeight * 2);
        }
    }

    return ixSize;
}

bool FdoSmPhIndex::CheckCommitDependencies( bool fromParent, bool isBeforeParent )
{
    // Grab the state before committing.
    FdoSchemaElementState elementState = GetElementState();

    // Get the table's element state
    FdoSchemaElementState tableElementState = FdoSchemaElementState_Detached;
    const FdoSmPhTable* pTable = RefTable();

    if ( pTable ) 
        tableElementState = pTable->GetElementState();

    if ( fromParent && (tableElementState == FdoSchemaElementState_Added) )
        // Table is new. Commit must wait until table is added.
        return false;

    if ( isBeforeParent &&
         (elementState == FdoSchemaElementState_Added)
    )
        // new indexes must be added after mods to table are committed.
        // modified and deleted indexes are processed before table is committed.
        return false;

    return true; 
}

void FdoSmPhIndex::XMLSerialize( FILE* xmlFp, int ref ) const
{
	fprintf( xmlFp, "<dbmsIndex name=\"%s\" unique=\"%ls\" table=\"%s\">\n",
			(const char*) FdoStringP(GetName()), 
            mIsUnique ? L"true" : L"false",
            mpTable ? (const char*) FdoStringP(mpTable->GetName()) : ""
	);

    FdoSmPhDbObject::XMLSerialize(xmlFp, ref);

    fprintf( xmlFp, "</dbmsIndex>\n" );
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhIndex::CreateColumnReader()
{
    // No column reader, parent table sets the column list.
    return (FdoSmPhRdColumnReader*) NULL;
}

void FdoSmPhIndex::Discard()
{
    ((FdoSmPhTable*) RefTable())->DiscardIndex( this );
}

void FdoSmPhIndex::AddColumnNoexistError( FdoString* pColName )
{
    GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_335), 
				(FdoString*) mpTable->GetQName(),
                pColName,
                GetName() 
            )
        )
	);
}

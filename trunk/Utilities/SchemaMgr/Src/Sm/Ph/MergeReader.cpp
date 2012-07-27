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
#include <Sm/Ph/MergeReader.h>

FdoSmPhMergeReader::FdoSmPhMergeReader( FdoSmPhReaderP reader1, FdoSmPhReaderP reader2, bool unionAll ) :
    FdoSmPhReader(reader1->GetManager(),(FdoSmPhRowCollection*) NULL),	
	mReader1(reader1),
	mReader2(reader2),
    mUnionAll(unionAll)
{
}

FdoSmPhMergeReader::~FdoSmPhMergeReader(void)
{
}

bool FdoSmPhMergeReader::ReadNext()
{
	// Give up if at EOF
	if ( IsEOF() ) 
		return(false);

	if ( IsBOF() ) {

		// If before first row, prime the sub-readers.

        mReader1->ReadNext();
        mReader2->ReadNext();
        SetBOF(false);
    }
    else {
        // Otherwise read next row from current reader.

        FdoSmPhReaderP currReader = GetCurrentReader();
        currReader->ReadNext();
    }

    // Quit when no more rows in either reader.
    if ( mReader1->IsEOF() && mReader2->IsEOF() ) 
        SetEOF(true);

    return !IsEOF();
}

FdoSmPhFieldP FdoSmPhMergeReader::GetField( FdoStringP tableName, FdoStringP fieldName )
{
    // Read field from current reader
    return GetCurrentReader()->GetField( tableName, fieldName );
}

FdoStringP FdoSmPhMergeReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    // Read row from current reader
    return GetCurrentReader()->GetString( tableName, fieldName );
}

FdoSmPhReaderP FdoSmPhMergeReader::GetCurrentReader()
{
    if ( mReader1->IsEOF() ) 
        // First reader done so can only read from second reader
        return mReader2;

    if ( mReader2->IsEOF() ) 
        // Second reader done so can only read from first reader
        return mReader1;

    // Both readers still have rows. Set the current reader to the 
    // one whose current row has the lowest key value. 
    // This ensures that the merged result set is ordered by the key.
    FdoStringP key1 = GetKeyValue( mReader1, true );
    FdoStringP key2 = GetKeyValue( mReader2, false );

    if ( key1 < key2 ) 
        return mReader1;

    if ( key1 > key2 ) 
        return mReader2;

    // Current keys are equal. 
    if ( !mUnionAll ) {
        // Not doing union all, so discard any reader2 rows whose key
        // matches the current reader1 row (Reader1 row takes precedence.
        while ( mReader2->ReadNext() ) {
            FdoStringP nextKey = GetKeyValue( mReader2, false );
            if ( nextKey != key2 ) 
                break;
        }
    }

    // When keys equal, read from reader1 first.
    return mReader1;
}

void FdoSmPhMergeReader::EndSelect()
{
	mReader1->EndSelect();
	mReader2->EndSelect();
}


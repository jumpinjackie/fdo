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
#include <Sm/Ph/GroupReader.h>

FdoSmPhGroupReader::FdoSmPhGroupReader( FdoStringP groupName, FdoSmPhReaderP reader) :
    FdoSmPhReader((FdoSmPhReader*) NULL),	
    mGroupName(groupName),
	mReader(reader)
{
}

FdoSmPhGroupReader::~FdoSmPhGroupReader(void)
{
}

bool FdoSmPhGroupReader::ReadNext()
{
	// Give up if at EOF
	if ( IsEOF() ) 
		return(false);

	if ( IsBOF() ) {

		// If before first property, need to do first read from the 
		// property reader.

		if ( mReader->IsBOF() ) 
			SetEOF( !mReader->ReadNext());
		if ( mReader->IsEOF() )
			SetEOF(true);

		// If just starting, we need to read until the 
		// group is found or we've passed by the group.
		// Note that we might already be positioned at the group.

		while ( (!IsEOF()) && (wcscmp(mGroupName, GetGroupName()) > 0) ) 
			SetEOF( !mReader->ReadNext() );
	}
	else {
		// Otherwise just read the next property
		SetEOF( !mReader->ReadNext() );
	}

	if ( !IsEOF() ) 
		// We're at EOF if not at a property for the class.
		SetEOF( wcscmp(mGroupName, GetGroupName()) != 0 );

	if ( !IsEOF() )
		// We're at a row so no longer BOF.
		SetBOF(false);

	return(!IsEOF());
}

FdoStringP FdoSmPhGroupReader::GetString( FdoStringP tableName, FdoStringP fieldName )
{
    return mReader->GetString( tableName, fieldName );
}


#ifndef FDOSMPHLOCKTYPESCOLLECTION_H
#define FDOSMPHLOCKTYPESCOLLECTION_H		1
//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Collection.h>
#include <Sm/Ph/LockTypes.h>

// Represents a collection of Lock Type Array objects. 

class FdoSmPhLockTypesCollection : public FdoSmCollection<FdoSmPhLockTypes>
{
public:
	FdoSmPhLockTypesCollection(void) :
		FdoSmCollection<FdoSmPhLockTypes>()
	{}
	~FdoSmPhLockTypesCollection(void) {}

    /// Overload of RefItem causes RefItem on base class
    /// to disappear, so added wrapper around it.
	const FdoSmPhLockTypes* RefItem(FdoInt32 index) const
	{
		return FdoSmCollection<FdoSmPhLockTypes>::RefItem(index);
	}

    /// Get the lock types for the given locking mode.
	const FdoSmPhLockTypes* RefItem( FdoLtLockModeType lockingMode ) const
	{
		for ( int i = 0; i < GetCount(); i++ ) {
			const FdoSmPhLockTypes* pLockTypes = RefItem(i);

			if ( lockingMode ==  pLockTypes->GetLockingMode() )
				return(pLockTypes);
		}

		return NULL;
	}

    /// Get the position of the lock types for the given locking mode.
    FdoInt32 IndexOf( FdoLtLockModeType lockingMode ) const
    {
		for ( int i = 0; i < GetCount(); i++ ) {
			const FdoSmPhLockTypes* pLockTypes = RefItem(i);

			if ( lockingMode ==  pLockTypes->GetLockingMode() )
				return(i);
		}

		return -1;
    }

};

typedef FdoPtr<FdoSmPhLockTypesCollection> FdoSmPhLockTypesCollectionP;

#endif



#ifndef FDOSMPHLOCKTYPES_H
#define FDOSMPHLOCKTYPES_H		1
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

#include <Sm/Disposable.h>

// This class contains a list of valid FDO lock types for a 
// particular locking mode
class FdoSmPhLockTypes : public FdoSmDisposable
{
public:
    /// Constructs an instance of a LockTypes object.
    /// 
    /// Parameters:
    /// 	lockingMode: the locking mode
    ///      lockTypeCount: number of lock types
    /// 	lockTypes: the valid lock types for the given locking mode. This object
    ///      takes control of these lockTypes and destroys them in its destructor.
    FdoSmPhLockTypes(
        FdoLtLockModeType lockingMode, 
		FdoInt32 lockTypeCount,
        FdoLockType* lockTypes
    );

	~FdoSmPhLockTypes(void);

    /// Gets the locking mode.
    FdoLtLockModeType GetLockingMode() const;

    /// Gets the lock types.
	const FdoLockType* GetLockTypes(FdoInt32& size) const;

private:
    FdoLtLockModeType   mLockingMode;

    /// Supported Lock types.
	FdoInt32 mLockTypeCount;
	FdoLockType* mLockTypes;
};

typedef FdoPtr<FdoSmPhLockTypes> FdoSmPhLockTypesP;

#endif



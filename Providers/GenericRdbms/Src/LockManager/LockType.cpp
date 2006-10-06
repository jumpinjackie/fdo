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
//

// =============================================================================
// | The file contains the implementation of the class FdoRdbmsLockType.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "LockType.h"

// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoRdbmsLockType::FdoRdbmsLockType ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsLockType ()

FdoRdbmsLockType::~FdoRdbmsLockType ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

}  //  ~FdoRdbmsLockType ()


FdoRdbmsLockType *FdoRdbmsLockType::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new object of this class and returns the reference
// | to it back to the calling routine.
// +---------------------------------------------------------------------------

{

    return new FdoRdbmsLockType();

}  //  Create ()

FdoRdbmsLockTypeEnum FdoRdbmsLockType::GetLockType ()

// +---------------------------------------------------------------------------
// | The function returns the value of the lock type property.
// +---------------------------------------------------------------------------

{

    return lock_type;

}  //  GetLockType ()

void FdoRdbmsLockType::SetLockType (FdoRdbmsLockTypeEnum value)

// +---------------------------------------------------------------------------
// | The function sets the value of the lock type property.
// +---------------------------------------------------------------------------

{

    lock_type = value;

}  //  SetLockType ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsLockType::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lock_type = FdoRdbmsLockTypeEnum_Exclusive;

}  //  SetToZero ()

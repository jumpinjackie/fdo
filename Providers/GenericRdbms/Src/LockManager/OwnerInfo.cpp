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
// | The file contains the implementation of the class FdoRdbmsLockOwnerInfo.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "OwnerInfo.h"

// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoRdbmsLockOwnerInfo::FdoRdbmsLockOwnerInfo ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsLockOwnerInfo ()

FdoRdbmsLockOwnerInfo::~FdoRdbmsLockOwnerInfo ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release the memory occupied by the class variables.

    ClearMemory();

}  //  ~FdoRdbmsLockOwnerInfo ()

FdoRdbmsLockOwnerInfo *FdoRdbmsLockOwnerInfo::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new object of this class and returns the reference
// | to it back to the calling routine.
// +---------------------------------------------------------------------------

{

    return new FdoRdbmsLockOwnerInfo();

}  //  Create ()

FdoString *FdoRdbmsLockOwnerInfo::GetLockOwner ()

// +---------------------------------------------------------------------------
// | The function returns the value of the lock owner property.
// +---------------------------------------------------------------------------

{

    return lock_owner;

}  //  GetLockOwner ()

void FdoRdbmsLockOwnerInfo::SetLockOwner (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the lock owner property.
// +---------------------------------------------------------------------------

{

    if (lock_owner != NULL) {

        delete[] lock_owner;
        lock_owner = NULL;

    }  //  if (lock_owner != NULL) ...

    if ((lock_owner = SetValue(value)) == NULL)
        throw FdoCommandException::Create(NlsMsgGet(
                                    FDORDBMS_91, "Failed to allocate memory"));

}  //  SetLockOwner ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsLockOwnerInfo::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    if (lock_owner != NULL) delete[] lock_owner;

    SetToZero();

}  //  ClearMemory ()

void FdoRdbmsLockOwnerInfo::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lock_owner = NULL;

}  //  SetToZero ()

wchar_t *FdoRdbmsLockOwnerInfo::SetValue (FdoString *value_buffer)

// +---------------------------------------------------------------------------
// | The function allocates memory for a string and initializes it with the
// | given value.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    wchar_t *value = NULL;

    // If the given value is NULL return a NULL pointer.

    if (value_buffer == NULL) return value;

    // Allocate memory for the given buffer. If this fails terminate the
    // routine.

    if ((value = new wchar_t[wcslen(value_buffer) + 1]) == NULL) return value;
    wcscpy(value, value_buffer);
    return value;

}  //  SetValue ()

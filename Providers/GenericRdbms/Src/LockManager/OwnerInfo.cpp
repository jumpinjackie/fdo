/***************************************************************************
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 *
 ***************************************************************************/

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

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
// | The file contains the implementation of the class FdoRdbmsLockInfo.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "LockInfo.h"

// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoRdbmsLockInfo::FdoRdbmsLockInfo ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsLockInfo ()

FdoRdbmsLockInfo::~FdoRdbmsLockInfo ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release the memory occupied by the class variables.

    ClearMemory ();

}  //  ~FdoRdbmsLockInfo ()

FdoRdbmsLockInfo *FdoRdbmsLockInfo::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new object of this class and returns the reference
// | to it back to the calling routine.
// +---------------------------------------------------------------------------

{

    return new FdoRdbmsLockInfo();

}  //  Create ()

FdoInt64 FdoRdbmsLockInfo::GetClassId ()

// +---------------------------------------------------------------------------
// | The function returns the value of the class id property.
// +---------------------------------------------------------------------------

{

    return class_id;

}  //  GetClassId ()

void FdoRdbmsLockInfo::SetClassId (FdoInt64 value)

// +---------------------------------------------------------------------------
// | The function sets the value of the class id property.
// +---------------------------------------------------------------------------

{

    class_id = value;

}  //  SetClassId ()

FdoString *FdoRdbmsLockInfo::GetLockOwner ()

// +---------------------------------------------------------------------------
// | The function returns the value of the lock owner property.
// +---------------------------------------------------------------------------

{

    return lock_owner;

}  //  GetLockOwner ()

void FdoRdbmsLockInfo::SetLockOwner (FdoString *value)

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

FdoString *FdoRdbmsLockInfo::GetLtName ()

// +---------------------------------------------------------------------------
// | The function returns the value of the long transaction name property.
// +---------------------------------------------------------------------------

{

    return lt_name;

}  //  GetLtName ()

void FdoRdbmsLockInfo::SetLtName (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the long transaction name property.
// +---------------------------------------------------------------------------

{

    if (lt_name != NULL) {

        delete[] lt_name;
        lt_name = NULL;

    }  //  if (lt_name != NULL) ...

    if ((lt_name = SetValue(value)) == NULL)
        throw FdoCommandException::Create(NlsMsgGet(
                                    FDORDBMS_91, "Failed to allocate memory"));

}  //  SetLtName ()

FdoString *FdoRdbmsLockInfo::GetTableName ()

// +---------------------------------------------------------------------------
// | The function returns the value of the table name property.
// +---------------------------------------------------------------------------

{

    return table_name;

}  //  GetTableName ()

void FdoRdbmsLockInfo::SetTableName (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the table_name property.
// +---------------------------------------------------------------------------

{

    if (table_name != NULL) {

        delete[] table_name;
        table_name = NULL;

    }  //  if (table_name != NULL) ...

    if ((table_name = SetValue(value)) == NULL)
        throw FdoCommandException::Create(NlsMsgGet(
                                    FDORDBMS_91, "Failed to allocate memory"));

}  //  SetTableName ()

FdoString *FdoRdbmsLockInfo::GetLockType ()

// +---------------------------------------------------------------------------
// | The function returns the value of the lock type property.
// +---------------------------------------------------------------------------

{

    return lock_type;

}  //  GetLockType ()

void FdoRdbmsLockInfo::SetLockType (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the lock type property.
// +---------------------------------------------------------------------------

{

    lock_type = SetValue(value);

}  //  SetLockType ()

FdoRdbmsPrimaryKey *FdoRdbmsLockInfo::GetPrimaryKey ()

// +---------------------------------------------------------------------------
// | The function returns the reference to the primary key property.
// +---------------------------------------------------------------------------

{

    return object_pk;

}  //  GetPrimaryKey ()

void FdoRdbmsLockInfo::SetPrimaryKey (FdoRdbmsPrimaryKey *value)

// +---------------------------------------------------------------------------
// | The function sets the reference to the primary key property.
// +---------------------------------------------------------------------------

{

    if (object_pk != NULL) {

        object_pk->Clear();
        FDO_SAFE_RELEASE(object_pk);
        object_pk = NULL;

    }  //  if (object_pk ...

    object_pk = value;

}  //  SetPrimaryKey ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsLockInfo::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    if (lt_name    != NULL) delete[] lt_name;
    if (table_name != NULL) delete[] table_name;
    if (lock_owner != NULL) delete[] lock_owner;
	if (lock_type != NULL) delete[] lock_type;

    if (object_pk  != NULL) {

        object_pk->Clear();
        FDO_SAFE_RELEASE(object_pk);

    }  //  if (object_pk ...

    SetToZero();

}  //  ClearMemory ()

void FdoRdbmsLockInfo::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    class_id   = -1;

    lt_name    = NULL;
    table_name = NULL;
    lock_owner = NULL;
    object_pk  = NULL;

    lock_type  = NULL;

}  //  SetToZero ()

wchar_t *FdoRdbmsLockInfo::SetValue (FdoString *value_buffer)

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

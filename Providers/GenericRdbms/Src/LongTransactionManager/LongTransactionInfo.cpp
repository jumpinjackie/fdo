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
// | The file contains the implementation of the class FdoRdbmsLongTransaction-
// | Info.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "LongTransactionInfo.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoRdbmsLongTransactionInfo::FdoRdbmsLongTransactionInfo ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsLongTransactionInfo ()

FdoRdbmsLongTransactionInfo::~FdoRdbmsLongTransactionInfo ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release the memory occupied by the class variables.

    ClearMemory ();

}  //  ~FdoRdbmsLongTransactionInfo ()


// ----------------------------------------------------------------------------
// --                            Public Interfaces                           --
// ----------------------------------------------------------------------------

FdoRdbmsLongTransactionInfo *FdoRdbmsLongTransactionInfo::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new object of this class and returns the reference
// | to it back to the calling routine.
// +---------------------------------------------------------------------------

{

    return new FdoRdbmsLongTransactionInfo();

}  //  Create ()

void FdoRdbmsLongTransactionInfo::Initialize ()

// +---------------------------------------------------------------------------
// | The function resets the contents.
// +---------------------------------------------------------------------------

{

    ClearMemory();

}  //  Initialize ()

FdoInt64 FdoRdbmsLongTransactionInfo::GetLtId ()

// +---------------------------------------------------------------------------
// | The function returns the value of the long transaction id property.
// +---------------------------------------------------------------------------

{

    return lt_id;

}  //  GetLtId ()

void FdoRdbmsLongTransactionInfo::SetLtId (FdoInt64 value)

// +---------------------------------------------------------------------------
// | The function sets the value of the long transaction id property.
// +---------------------------------------------------------------------------

{

    lt_id = value;

}  //  SetLtId ()

FdoDateTime FdoRdbmsLongTransactionInfo::GetCreationDate ()

// +---------------------------------------------------------------------------
// | The function returns the value of the creation date property.
// +---------------------------------------------------------------------------

{

    return creation_date;

}  //  GetCreationDate ()

void FdoRdbmsLongTransactionInfo::SetCreationDate (FdoDateTime value)

// +---------------------------------------------------------------------------
// | The function sets the value of the creation date property.
// +---------------------------------------------------------------------------

{

    creation_date = value;

}  //  SetCreationDate ()

FdoString *FdoRdbmsLongTransactionInfo::GetDescription ()

// +---------------------------------------------------------------------------
// | The function returns the value of the long transaction description prop-
// | erty.
// +---------------------------------------------------------------------------

{

    return description;

}  //  GetDescription ()

void FdoRdbmsLongTransactionInfo::SetDescription (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the long transaction description property.
// +---------------------------------------------------------------------------

{

    // Clear the current property value.

    if (description != NULL) { delete[] description; description = NULL; }

    // Save the given value.

    description = SetValue(value);

}  //  SetDescription ()

FdoString *FdoRdbmsLongTransactionInfo::GetFreezeMode ()

// +---------------------------------------------------------------------------
// | The function returns the value of the long transaction freeze mode prop-
// | erty.
// +---------------------------------------------------------------------------

{

    return freeze_mode;

}  //  GetFreezeMode ()

void FdoRdbmsLongTransactionInfo::SetFreezeMode (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the long transaction freeze mode property.
// +---------------------------------------------------------------------------

{

    // Clear the current property value.

    if (freeze_mode != NULL) { delete[] freeze_mode; freeze_mode = NULL; }

    // Save the given value.

    freeze_mode = SetValue(value);

}  //  SetFreezeMode ()

FdoString *FdoRdbmsLongTransactionInfo::GetFreezeOwner ()

// +---------------------------------------------------------------------------
// | The function returns the value of the long transaction freeze owner prop-
// | erty.
// +---------------------------------------------------------------------------

{

    return freeze_owner;

}  //  GetFreezeOwner ()

void FdoRdbmsLongTransactionInfo::SetFreezeOwner (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the long transaction freeze owner property.
// +---------------------------------------------------------------------------

{

    // Clear the current property value.

    if (freeze_owner != NULL) { delete[] freeze_owner; freeze_owner = NULL; }

    // Save the given value.

    freeze_owner = SetValue(value);

}  //  SetFreezeOwner ()

FdoString *FdoRdbmsLongTransactionInfo::GetFreezeWriter ()

// +---------------------------------------------------------------------------
// | The function returns the value of the long transaction freeze writer prop-
// | erty.
// +---------------------------------------------------------------------------

{

    return freeze_writer;

}  //  GetFreezeWriter ()

void FdoRdbmsLongTransactionInfo::SetFreezeWriter (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the long transaction freeze writer prop-
// | erty.
// +---------------------------------------------------------------------------

{

    // Clear the current property value.

    if (freeze_writer != NULL) { delete[] freeze_writer; freeze_writer = NULL; }

    // Save the given value.

    freeze_writer = SetValue(value);

}  //  SetFreezeWriter ()

bool FdoRdbmsLongTransactionInfo::GetIsActiveFlag ()

// +---------------------------------------------------------------------------
// | The function returns value of the flag indicating whether or not the long
// | transaction is active.
// +---------------------------------------------------------------------------

{

    return is_active;

}  //  GetIsActiveFlag ()

void FdoRdbmsLongTransactionInfo::SetIsActiveFlag (bool value)

// +---------------------------------------------------------------------------
// | The function sets the value of the flag indicating whether or not the long
// | transaction is active.
// +---------------------------------------------------------------------------

{

    is_active = value;

}  //  SetIsActiveFlag ()

bool FdoRdbmsLongTransactionInfo::GetIsRefreshedFlag ()

// +---------------------------------------------------------------------------
// | The function returns the value of the flag indicating whether or not the
// | long transaction is continously refreshed.
// +---------------------------------------------------------------------------

{

    return is_refreshed;

}  //  GetIsRefreshedFlag ()

void FdoRdbmsLongTransactionInfo::SetIsRefreshedFlag (bool value)

// +---------------------------------------------------------------------------
// | The function sets the value of the flag indicating whether or not the long
// | transaction is continously refreshed.
// +---------------------------------------------------------------------------

{

    is_refreshed = value;

}  //  SetIsRefreshedFlag ()

FdoString *FdoRdbmsLongTransactionInfo::GetName ()

// +---------------------------------------------------------------------------
// | The function returns the value of the long transaction name property.
// +---------------------------------------------------------------------------

{

    return name;

}  //  GetName ()

void FdoRdbmsLongTransactionInfo::SetName (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the long transaction name property.
// +---------------------------------------------------------------------------

{

    // Clear the current property value.

    if (name != NULL) { delete[] name; name = NULL; }

    // Save the given value.

    name = SetValue(value);

}  //  SetName ()

FdoString *FdoRdbmsLongTransactionInfo::GetOwner ()

// +---------------------------------------------------------------------------
// | The function returns the value of the long transaction owner property.
// +---------------------------------------------------------------------------

{

    return owner;

}  //  GetOwner ()

void FdoRdbmsLongTransactionInfo::SetOwner (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the long transaction owner property.
// +---------------------------------------------------------------------------

{

    // Clear the current property value.

    if (owner != NULL) {delete[] owner; owner = NULL; }

    // Save the given value.

    owner = SetValue(value);

}  //  SetOwner ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsLongTransactionInfo::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    if (name          != NULL) delete[] name;
    if (description   != NULL) delete[] description;
    if (owner         != NULL) delete[] owner;
    if (freeze_owner  != NULL) delete[] freeze_owner;
    if (freeze_mode   != NULL) delete[] freeze_mode;
    if (freeze_writer != NULL) delete[] freeze_writer;

    SetToZero();

}  //  ClearMemory ()

void FdoRdbmsLongTransactionInfo::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lt_id           = -1;

    name            = NULL;
    description     = NULL;
    owner           = NULL;
    freeze_owner    = NULL;
    freeze_mode     = NULL;
    freeze_writer   = NULL;

    is_active       = false;
    is_refreshed    = false;

}  //  SetToZero ()

wchar_t *FdoRdbmsLongTransactionInfo::SetValue (FdoString *value_buffer)

// +---------------------------------------------------------------------------
// | The function allocates memory for a string and initializes it with the
// | given value.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    wchar_t *value = NULL;

    // If the given value is NULL, just return the NULL pointer.

    if (value_buffer == NULL)
        return value;

    // Allocate memory for the given buffer. If this fails terminate issue
    // an exception. Otherwise copy the given value into the allocated memory
    // and return the handle to the string back to the caller.

    if ((value = new wchar_t[wcslen(value_buffer) + 1]) == NULL)
        throw FdoCommandException::Create(
                        NlsMsgGet(FDORDBMS_91, "Failed to allocate memory"));
    wcscpy(value, value_buffer);
    return value;

}  //  SetValue ()

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
// | Conflict.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "LongTransactionConflict.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoRdbmsLongTransactionConflict::FdoRdbmsLongTransactionConflict ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsLongTransactionConflict ()

FdoRdbmsLongTransactionConflict::~FdoRdbmsLongTransactionConflict ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release the memory occupied by the class variables.

    ClearMemory ();

}  //  ~FdoRdbmsLongTransactionConflict ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoRdbmsLongTransactionConflictInfos 
                            *FdoRdbmsLongTransactionConflict::GetConflicts ()

// +---------------------------------------------------------------------------
// | The function returns the reference to the conflict list.
// +---------------------------------------------------------------------------

{

    return conflicts;

}  //  GetConflicts ()

void FdoRdbmsLongTransactionConflict::SetConflicts (
                                FdoRdbmsLongTransactionConflictInfos *root)

// +---------------------------------------------------------------------------
// | The function sets the reference to the conflict list.
// +---------------------------------------------------------------------------

{

    if (conflicts != NULL) {

        conflicts->Clear();
        conflicts = NULL;

    }  //  if (conflicts != NULL) ...

    conflicts = root;

}  //  SetConflicts ()

FdoString *FdoRdbmsLongTransactionConflict::GetDataStore ()

// +---------------------------------------------------------------------------
// | The function returns the value of the data store property.
// +---------------------------------------------------------------------------

{

    return data_store;

}  //  GetDataStore ()

void FdoRdbmsLongTransactionConflict::SetDataStore (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the data store property.
// +---------------------------------------------------------------------------

{

    if (data_store != NULL) {

        delete[] data_store;
        data_store = NULL;

    }  //  if (data_store != NULL) ...

    data_store = SetValue(value);

}  //  SetDataStore ()

FdoString *FdoRdbmsLongTransactionConflict::GetTableName ()

// +---------------------------------------------------------------------------
// | The function returns the value of the table name property.
// +---------------------------------------------------------------------------

{

    return table_name;

}  //  GetTableName ()

void FdoRdbmsLongTransactionConflict::SetTableName (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the table name property.
// +---------------------------------------------------------------------------

{

    if (table_name != NULL) {

        delete[] table_name;
        table_name = NULL;

    }  //  if (table_name != NULL) ...

    table_name = SetValue(value);

}  //  SetTableName ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsLongTransactionConflict::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function releases memory allocated for the class variables.
// +---------------------------------------------------------------------------

{

    if (data_store != NULL) delete[] data_store;
    if (table_name != NULL) delete[] table_name;
    if (conflicts  != NULL) conflicts->Clear();

    SetToZero();

}  //  ClearMemory ()

void FdoRdbmsLongTransactionConflict::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    conflicts  = NULL;
    data_store = NULL;
    table_name = NULL;

}  //  SetToZero ()

wchar_t *FdoRdbmsLongTransactionConflict::SetValue (FdoString *value_buffer)

// +---------------------------------------------------------------------------
// | The function allocates memory for a copy of the given string.
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

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
// | The file contains the implementation of the class FdoRdbmsTableInfo.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "TableInfo.h"

// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoRdbmsTableInfo::FdoRdbmsTableInfo ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsTableInfo ()

FdoRdbmsTableInfo::~FdoRdbmsTableInfo ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release the memory occupied by the class variables.

    ClearMemory();

}  //  ~FdoRdbmsTableInfo ()

FdoRdbmsTableInfo *FdoRdbmsTableInfo::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new object of this class and returns the reference
// | to it back to the calling routine.
// +---------------------------------------------------------------------------

{

    return new FdoRdbmsTableInfo();

}  //  Create ()

FdoString *FdoRdbmsTableInfo::GetDataStoreName ()

// +---------------------------------------------------------------------------
// | The function returns the value of the data store property.
// +---------------------------------------------------------------------------

{

    return data_store_name;

}  //  GetDataStoreName ()

void FdoRdbmsTableInfo::SetDataStoreName (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the data store property.
// +---------------------------------------------------------------------------

{

    if (data_store_name != NULL) {

        delete[] data_store_name;
        data_store_name = NULL;

    }  //  if (data_store_name != NULL) ...

    if ((data_store_name = SetValue(value)) == NULL)
        throw FdoCommandException::Create(NlsMsgGet(
                                    FDORDBMS_91, "Failed to allocate memory"));

}  //  SetDataStoreName ()

FdoString *FdoRdbmsTableInfo::GetTableName ()

// +---------------------------------------------------------------------------
// | The function returns the value of the table name property.
// +---------------------------------------------------------------------------

{

    return table_name;

}  //  GetTableName ()

void FdoRdbmsTableInfo::SetTableName (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the table name property.
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


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsTableInfo::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    if (data_store_name != NULL) delete[] data_store_name;
    if (table_name      != NULL) delete[] table_name;

    SetToZero();

}  //  ClearMemory ()

void FdoRdbmsTableInfo::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    data_store_name = NULL;
    table_name      = NULL;

}  //  SetToZero ()

wchar_t *FdoRdbmsTableInfo::SetValue (FdoString *value_buffer)

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

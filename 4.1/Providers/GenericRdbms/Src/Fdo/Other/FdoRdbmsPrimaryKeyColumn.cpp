/***************************************************************************
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
 ***************************************************************************/

// =============================================================================
// | The file contains the implementation of the class FdoRdbmsPrimaryKeyColumn.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "FdoRdbmsPrimaryKeyColumn.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoRdbmsPrimaryKeyColumn::FdoRdbmsPrimaryKeyColumn ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsPrimaryKeyColumn ()

FdoRdbmsPrimaryKeyColumn::~FdoRdbmsPrimaryKeyColumn ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release the memory occupied by the class variables.

    ClearMemory ();

}  //  ~FdoRdbmsPrimaryKeyColumn ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoRdbmsPrimaryKeyColumn *FdoRdbmsPrimaryKeyColumn::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new primary key column object and returns a handle
// | to it back to the calling routine.
// +---------------------------------------------------------------------------

{

    return new FdoRdbmsPrimaryKeyColumn();

}  //  Create ()

FdoString *FdoRdbmsPrimaryKeyColumn::GetColumnName ()

// +---------------------------------------------------------------------------
// | The function returns the value of the primary key column name property.
// +---------------------------------------------------------------------------

{

    return column_name;

}  //  GetColumnName ()

void FdoRdbmsPrimaryKeyColumn::SetColumnName (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets value of the primary key column name property.
// +---------------------------------------------------------------------------

{

    // If the object already stores a column name release the allocated memory.

    if (column_name != NULL) {

        delete[] column_name;
        column_name = NULL;

    }  //  if (column_name != NULL) ...

    // Set the new column value.

    column_name = SetValue(value);

}  //  SetColumnName ()

FdoString *FdoRdbmsPrimaryKeyColumn::GetColumnValue ()

// +---------------------------------------------------------------------------
// | The function returns the value for the primary key column value property.
// +---------------------------------------------------------------------------

{

    return column_value;

}  //  GetColumnValue ()

void FdoRdbmsPrimaryKeyColumn::SetColumnValue (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value for the primary key column value property.
// +---------------------------------------------------------------------------

{

    // If the object already stores a column value release the allocated memory.

    if (column_value != NULL) {

        delete[] column_value;
        column_value = NULL;

    }  //  if (column_value != NULL) ...

    // Set the new column value.

    column_value = SetValue(value);

}  //  SetColumnValue ()

FdoInt32 FdoRdbmsPrimaryKeyColumn::GetColumnDataType ()

// +---------------------------------------------------------------------------
// | The function returns the value for the primary key column data type
// | property.
// +---------------------------------------------------------------------------

{

    return column_data_type;

}  //  GetColumnValue ()

void FdoRdbmsPrimaryKeyColumn::SetColumnDataType (FdoInt32 rdbi_data_type)

// +---------------------------------------------------------------------------
// | The function sets the value for the primary key column data type property.
// +---------------------------------------------------------------------------

{

    column_data_type = rdbi_data_type;

}  //  SetColumnValue ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsPrimaryKeyColumn::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    if (column_name  != NULL) delete[] column_name;
    if (column_value != NULL) delete[] column_value;

    SetToZero();

}  //  ClearMemory ()

void FdoRdbmsPrimaryKeyColumn::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    column_data_type = RDBI_STRING;
    column_name      = NULL;
    column_value     = NULL;

}  //  SetToZero ()

wchar_t *FdoRdbmsPrimaryKeyColumn::SetValue (FdoString *value_buffer)

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

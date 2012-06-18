/******************************************************************************
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
  *
 ******************************************************************************/

// =============================================================================
// | The file contains the definition of the class FdoRdbmsPrimaryKeyColumn. The
// | class defines the structure used to store information associated with a
// | single primary key column.
// =============================================================================


#ifndef FdoRdbmsPrimaryKeyColumn_h
#define FdoRdbmsPrimaryKeyColumn_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include <Inc/Nls/fdordbms_msg.h>
#include "FdoRdbmsUtil.h"
#include "FdoRdbmsException.h"
#include "rdbi.h"

class FdoRdbmsPrimaryKeyColumn: public FdoIDisposable
{

    public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  GetColumnDataType:
    //      The function gets the column data type property value set for the
    //      current object.

    FdoInt32 GetColumnDataType ();

    //  GetColumnName:
    //      The function gets the column name property value set for the cur-
    //      rent object.

    FdoString *GetColumnName  ();

    //  GetColumnValue:
    //      The function gets the column value property value set for the cur-
    //      rent object.

    FdoString *GetColumnValue ();

    //  SetColumnDataType:
    //      The function sets the column data type property for the current
    //      object.

    void SetColumnDataType (FdoInt32 rdbi_data_type);

    //  SetColumnName:
    //      The function sets the column name property for the current object.

    void SetColumnName (FdoString *value);

    //  SetColumnValue:
    //      The function sets the column value property for the current object.

    void SetColumnValue (FdoString *value);

    //  Create:
    //      The function creates a new object of this class and returns a
    //      handle to it back to the caller.

    static FdoRdbmsPrimaryKeyColumn *Create ();


    protected:

    // ************************************************************************
    // *                         Protected Interfaces                         *
    // ************************************************************************

    //  Dispose:
    //      The function deletes an object of this class.

    virtual void Dispose () { delete this; }

    //  FdoRdbmsPrimaryKeyColumn:
    //      The class constructor.

    FdoRdbmsPrimaryKeyColumn ();

    //  ~FdoRdbmsPrimaryKeyColumn:
    //      The class destructor.

    virtual ~FdoRdbmsPrimaryKeyColumn ();


    private:

    // ************************************************************************
    // *                          Private Interfaces                          *
    // ************************************************************************

    //  ClearMemory:
    //      The function releases all allocated memory for an object of this
    //      class.

    void ClearMemory ();

    //  SetToZero:
    //      The function initializes all class member variables.

    void SetToZero ();

    //  SetValue:
    //      The function makes a copy of the given string.

    wchar_t *SetValue (FdoString *value_buffer);


    // ************************************************************************
    // *                        Private Member Variables                      *
    // ************************************************************************

    //  column_data_type:
    //      The variable stores the column data type set for an object of this
    //      class.

    FdoInt32 column_data_type;

    //  column_name:
    //      The variable stores the column name set for an object of this
    //      class.

    FdoString *column_name;

    //  column_value:
    //      The variable stores the column value set for an object of this
    //      class.

    FdoString *column_value;


};  //  class FdoRdbmsPrimaryKeyColumn

#endif


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
// | The file contains the definition of the class FdoRdbmsTableInfo.
// =============================================================================


#ifndef FdoRdbmsTableInfo_h
#define FdoRdbmsTableInfo_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include <Inc/Nls/fdordbms_msg.h>
#include "FdoRdbmsUtil.h"
#include "FdoRdbmsException.h"
#include "Disposable.h"

class FdoRdbmsTableInfo : public FdoLkDisposable
{

    public:

    // Class Functions:
    //  Get*:
    //      Functions to get the values of the class variables.
    //  Set*:
    //      Functions to set the class variable values.


    void                     SetDataStoreName  (FdoString *value);
    void                     SetTableName      (FdoString *value);

    FdoString                *GetDataStoreName ();
    FdoString                *GetTableName     ();

    static FdoRdbmsTableInfo *Create           ();

    protected:

    // Class Functions:
    //  FdoRdbmsTableInfo:
    //      Class constructor.
    //  ~FdoRdbmsTableInfo:
    //      Class destructor.

        FdoRdbmsTableInfo          ();
        virtual ~FdoRdbmsTableInfo ();


    private:

    // Class Variables:
    //  data_store_name:
    //      The variable stores the name of the table host.
    //  table_name:
    //      The variable stores the name of the table.


    FdoString *data_store_name,
              *table_name;


    // Class Functions:
    //  ClearMemory:
    //      Function to release occupied memory.
    //  SetToZero:
    //      Function to initialize class variables.
    //  SetValue:
    //      Function to save a given value buffer.

    void    ClearMemory ();
    void    SetToZero   ();

    wchar_t *SetValue   (FdoString *value_buffer);


};  //  class FdoRdbmsTableInfo

#endif


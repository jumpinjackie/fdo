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
// | The file contains the definition of the class FdoRdbmsLockInfo. The class
// | is used to store and provide access to lock information for a single ob-
// | ject.
// =============================================================================


#ifndef FdoRdbmsLockInfo_h
#define FdoRdbmsLockInfo_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include <Inc/Nls/fdordbms_msg.h>
#include "FdoRdbmsUtil.h"
#include "FdoRdbmsException.h"
#include "Disposable.h"
#include "FdoRdbmsPrimaryKey.h"
#include "LockTypeEnum.h"

class FdoRdbmsLockInfo : public FdoLkDisposable
{

    public:

    // Class Functions:
    //  Get*:
    //      Functions to get the values of the class variables.
    //  Set*:
    //      Functions to set the class variable values.

    void                    SetClassId       (FdoInt64             value);
    void                    SetLockOwner     (FdoString            *value);
    void                    SetLockType      (FdoString			   *value);
    void                    SetLtName        (FdoString            *value);
    void                    SetPrimaryKey    (FdoRdbmsPrimaryKey   *value);
    void                    SetTableName     (FdoString            *value);

    FdoInt64                GetClassId       ();

    FdoString               *GetLockOwner    ();
    FdoString               *GetLtName       ();
    FdoString               *GetTableName    ();

    FdoString				*GetLockType      ();

    FdoRdbmsPrimaryKey      *GetPrimaryKey   ();

    static FdoRdbmsLockInfo *Create          ();

	void    ClearMemory ();

    protected:

    // Class Functions:
    //  FdoRdbmsLockInfo:
    //      Class constructor.
    //  ~FdoRdbmsLockInfo:
    //      Class destructor.

        FdoRdbmsLockInfo          ();
        virtual ~FdoRdbmsLockInfo ();


    private:

    // Class Variables:
    //  class_id:
    //      The unique id associated with the object for which this informatiom
    //      is provided identifying its class it belongs to.
    //  lt_name:
    //      The name of the long transaction the object for which this informa-
    //      tion is provided belongs to.
    //  table_name:
    //      The name of the table from which the object information is obtained.
    //  lock_owner:
    //      The name of the FDO user who has applied a persistent lock on the
    //      object for which this information is provided. If no persistent 
    //      lock has been applied the value of the member variable is NULL.
    //  lock_type:
    //      The type of lock that was applied on the object for which this in-
    //      formation is provided.
    //  object_pk:
    //      A reference to the primary key information for this object.

    FdoInt64             class_id;

    FdoString            *lt_name,
                         *table_name,
                         *lock_owner;

    FdoString			 *lock_type;

    FdoRdbmsPrimaryKey   *object_pk;


    // Class Functions:
    //  ClearMemory:
    //      Function to release occupied memory.
    //  SetToZero:
    //      Function to initialize class variables.
    //  SetValue:
    //      Function to save a given value buffer.

    void    SetToZero   ();

    wchar_t *SetValue   (FdoString *value_buffer);


};  //  class FdoRdbmsLockInfo

#endif


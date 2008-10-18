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
// | The file contains the definition of the class FdoRdbmsLockType. The class 
// | is used to store and provide access to a supported lock type.
// =============================================================================


#ifndef FdoRdbmsLockType_h
#define FdoRdbmsLockType_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include "Disposable.h"
#include "LockTypeEnum.h"

class FdoRdbmsLockType : public FdoLkDisposable
{

    public:

    // Class Functions:
    //  Get*:
    //      Functions to get the values of the class variables.
    //  Set*:
    //      Functions to set the class variable values.

    void                    SetLockType (FdoRdbmsLockTypeEnum value);

    FdoRdbmsLockTypeEnum    GetLockType ();

    static FdoRdbmsLockType *Create     ();

    protected:

    // Class Functions:
    //  FdoRdbmsLockType:
    //      Class constructor.
    //  ~FdoRdbmsLockType:
    //      Class destructor.

        FdoRdbmsLockType          ();
        virtual ~FdoRdbmsLockType ();


    private:

    // Class Variables:
    //  lock_type:
    //      The variable holds the lock type information.

    FdoRdbmsLockTypeEnum lock_type;


    // Class Functions:
    //  SetToZero:
    //      Function to initialize class variables.

    void    SetToZero   ();


};  //  class FdoRdbmsLockType

#endif


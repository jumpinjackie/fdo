/******************************************************************************
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
  *
 ******************************************************************************/

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


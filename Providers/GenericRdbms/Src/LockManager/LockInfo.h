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


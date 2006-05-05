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
// | The file contains the definition of the class FdoRdbmsLockConflict. The
// | class implements properties and functions used to store a lock conflict
// | data set.
// =============================================================================

#ifndef FdoRdbmsLockConflict_h
#define FdoRdbmsLockConflict_h 1

// The links to the header files to be included.

#include "FdoRdbmsConnection.h"
#include "stdafx.h"
#include "Disposable.h"

class FdoRdbmsLockConflict: public FdoLkDisposable
{

  private:

    // Class Enumerations:

  public:

    // Class Functions:
    //   AddIdentity       : Adds another identity property set to the identity
    //                       collection for the current lock conflict.
    //   GetClassName      : Returns the class name property for the current
    //                       lock conflict.
    //   GetIdentity       : Returns a handle to the identity collection for
    //                       the current lock conflict.
    //   GetLockOwner      : Returns the lock owner property for the current
    //                       lock conflict.
    //   GetConflictType   : Returns the conflict type property for the current
    //                       lock conflict.
    //   GetLongTransaction: Returns the long transaction information for the
    //                       current lock conflict.
    //   SetClassName      : Sets the class name property for the current lock
    //                       conflict.
    //   SetOwner          : Sets the owner property for the current lock con-
    //                       flict.
    //   SetLockType       : Sets the lock type property for the current lock
    //                       conflict.
    //   SetLongTransaction: Sets the long transaction information for the cur-
    //                       rent lock conflict.

                               FdoRdbmsLockConflict (
                                            FdoRdbmsConnection   *a_dbi_connection);

    void                       AddIdentity          (
                                            FdoString            *property_name,
                                            FdoString            *value_buffer);
    void                       SetOwner             (
                                            FdoString            *lk_owner);
    void                       SetLockType          (
                                            FdoString            *lk_type);
    void                       SetConflictType      (
                                            FdoConflictType cfl_type);
    void                       SetLongTransaction   (
                                            FdoString            *lk_lt_name);
    void                       SetClassName         (
                                            FdoString   *lk_class_name);
    FdoString                  *GetFeatureClassName        ();
    FdoString                  *GetLockOwner        ();
    FdoString                  *GetLongTransaction  ();
    FdoLockType                GetLockType          ();
    FdoConflictType            GetConflictType      ();
    FdoPropertyValueCollection *GetIdentity         ();
	void						FreeMemory            ();

  protected:

    // Class Functions:
    //   SetToZero         : Initializes all class member variables.
    //   FreeMemory        : Frees all memory allocated by the member variables.

    void    SetToZero             ();

    virtual void Dispose          () { delete this; }

    virtual ~FdoRdbmsLockConflict ();

    // Class Variables:

    FdoRdbmsConnection *dbi_connection;


  private:

    // Class Variables:
    //   class_name      : Holds the class name associated with the current
    //                     lock conflict.
    //   lock_owner      : Holds the owner of the object causing the lock
    //                     conflict.
    //   long_transaction: Holds the name of the long transaction where the
    //                     object causing the lock conflict is locked by a
    //                     different user.
    //   lock_type       : Holds the type of lock applied to an object.
    //   identities      : Holds the identity information for the object
    //                     causing a lock conflict.

    FdoString                  *class_name;
    FdoString                  *lock_owner;
    FdoString                  *long_transaction;

    FdoLockType                lock_type;

    FdoPropertyValueCollection *identities;


    // Class Functions:


};  //  class FdoRdbmsLockConflict

#endif

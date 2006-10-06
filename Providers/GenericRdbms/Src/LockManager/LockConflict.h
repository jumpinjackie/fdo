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

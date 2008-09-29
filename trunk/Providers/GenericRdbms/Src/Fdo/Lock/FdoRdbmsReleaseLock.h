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
// | The file contains the definition of the class FdoRdbmsReleaseLock. The
// | class releases all locks on instance data identified by the data.
// =============================================================================

#ifndef FdoRdbmsReleaseLock_h
#define FdoRdbmsReleaseLock_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsFeatureCommand.h"
#include "FdoRdbmsLockConflictReader.h"


class FdoRdbmsReleaseLock : public FdoRdbmsFeatureCommand<FdoIReleaseLock>
{

  friend class FdoRdbmsConnection;

  public:

    // Class Functions:
    //   Execute     : Executes an unlock request.
    //   GetLockOwner: Gets the class lock owner property value.
    //   SetLockOwner: Sets the lock owner. This is required if an administrator
    //                 needs to unlock objects locked by a different user.

    void                    SetLockOwner  (FdoString *lockOwner);
    FdoString               *GetLockOwner ();
    FdoILockConflictReader* Execute       ();

  protected:

    // Class Functions:
    //   ExecuteUnlockRequest: Performs the unlock operation.
    //   LockOwnerIsUser     : Returns TRUE if the lock owner class property is
    //                         the same as the current user, FALSE otherwise.
    //   UserIsAdministrator : Returns TRUE if the current user has administrator
    //                         priviledges, FALSE otherwise.
    //   SetToZero           : Initializes all class variables.
    //   FreeMemory          : Releases occupied memory.

            FdoRdbmsReleaseLock  ();
            FdoRdbmsReleaseLock  (FdoIConnection *connection);
    virtual ~FdoRdbmsReleaseLock ();

    FdoRdbmsLockConflictReader    *ExecuteUnlockRequest (bool is_administrator_op);
    bool    LockOwnerIsUser      (const wchar_t        *current_user);
    bool    UserIsAdministrator  (const wchar_t        *current_user);
    void    SetToZero            ();
    void    FreeMemory           ();

  protected:

    // Class Variables:
    //   lock_owner          : Stores the lock owner set by the user.
    //   *_connection        : References the connection objects
    //   conflict_class_name : The name of the class for which the command is
    //                         actually executed. This name might be different
    //                         from the specified class name for the command.

    DbiConnection      *dbi_connection;
    FdoIdentifier      *conflict_class_name;
    FdoIConnection     *fdo_i_connection;

    FdoString          *lock_owner;
    FdoRdbmsConnection *fdo_rdbms_connection;

  private:

    // Class Enumerations:

    // Class Functions:

};  //  class FdoRdbmsReleaseLock

#endif

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
// | The file contains the definition of the class FdoRdbmsAcquireLock. The
// | class locks instances of a given class that match the specified criteria.
// | Input to the acquire lock command includes the name of the class, the lock
// | type and strategy and filter criteria by which to identify the instances to
// | be locked. The filter may be specified either as text or as an expression
// | tree (most likely produced be a query builder).
// =============================================================================

#ifndef FdoRdbmsAcquireLock_h
#define FdoRdbmsAcquireLock_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsFeatureCommand.h"
#include "FdoRdbmsLockConflictReader.h"


class FdoRdbmsAcquireLock : public FdoRdbmsFeatureCommand<FdoIAcquireLock>
{

  friend class FdoRdbmsConnection;

  public:

    // Class Functions:
    //   Execute        : Executes a lock request.
    //   SetLockType    : The function sets the value of the lock type
    //                    property.
    //   GetLockType    : The function returns the current value of the lock
    //                    type property.
    //   SetLockStrategy: The function sets the value of the lock strategy
    //                    property.
    //   GetLockStrategy: The function returns the current value of the lock
    //                    strategy property.


    void                       SetLockType     (FdoLockType     lockType);
    void                       SetLockStrategy (FdoLockStrategy lockStrategy);
    FdoLockType                GetLockType     ();
    FdoLockStrategy            GetLockStrategy ();
    FdoILockConflictReader     *Execute        ();

  protected:

    // Class Functions:
    //   ExecuteLockRequest : Performs the lock request.
    //   SetToZero          : Initializes all class member variables.
    //   FreeMemory         : Frees all memory allocated by the member
    //                        variables.

            FdoRdbmsAcquireLock   ();
            FdoRdbmsAcquireLock   (
                            FdoIConnection             *connection);
    virtual ~FdoRdbmsAcquireLock  ();

    FdoRdbmsLockConflictReader    *ExecuteLockRequest    ();
    void    FreeMemory            ();
    void    SetToZero             ();

  protected:

    // Class Variables:
    //   *_connection        : References the connection objects
    //   lock_type           : Identifies the lock type.
    //   lock_strategy       : Identifies the lock strategy.
    //   conflict_class_name : The name of the class for which the command is
    //                         actually executed. This name might be different
    //                         from the specified class name for the command.

    DbiConnection      *dbi_connection;
    FdoIConnection     *fdo_i_connection;
    FdoRdbmsConnection *fdo_rdbms_connection;

    FdoLockType        lock_type;
    FdoIdentifier      *req_class_name;
    FdoLockStrategy    lock_strategy;

  private:

    // Class Enumerations:

    // Class Functions:

};  //  class FdoRdbmsAcquireLock

#endif

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
// | The file contains the implementation of the class FdoRdbmsGetLockedObjects.
// | The initiates the retrieval of the objects locked by a named owner.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "FdoRdbmsGetLockedObjects.h"
#include "LockUtility.h"
#include "FdoRdbmsException.h"


FdoRdbmsGetLockedObjects::FdoRdbmsGetLockedObjects ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    fdo_rdbms_connection = NULL;
    SetToZero();

}  //  FdoRdbmsGetLockedObjects ()

FdoRdbmsGetLockedObjects::FdoRdbmsGetLockedObjects (FdoIConnection *connection)
    : FdoRdbmsCommand<FdoIGetLockedObjects>(connection)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Configure the RDBMS and DBI connection.

    fdo_rdbms_connection = static_cast<FdoRdbmsConnection*>(connection);

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsGetLockedObjects ()

FdoRdbmsGetLockedObjects::~FdoRdbmsGetLockedObjects ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    FreeMemory();

}  //  ~FdoRdbmsGetLockedObjects ()


void FdoRdbmsGetLockedObjects::SetLockOwner (FdoString* value)

// +---------------------------------------------------------------------------
// | The function sets the lock owner for which the command to get locked ob-
// | jects should be executed.
// +---------------------------------------------------------------------------

{

    if (requested_owner != NULL) {

        delete[] requested_owner;
        requested_owner = NULL;

    }  //  if (requested_owner != NULL) ...

    if ((requested_owner = LockUtility::SetValue(value)) == NULL)
         throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                        LockUtility::MEMORY_ALLOCATION_ERROR));

}  //

FdoString *FdoRdbmsGetLockedObjects::GetLockOwner ()

// +---------------------------------------------------------------------------
// | The function retrieves the lock owner for which the command to get locked
// | objects should be executed.
// +---------------------------------------------------------------------------

{

    return (requested_owner);

}

FdoILockedObjectReader *FdoRdbmsGetLockedObjects::Execute ()

// +---------------------------------------------------------------------------
// | The function executes the request.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoCommandException         *fdo_cmd_ex            = NULL;

    FdoRdbmsLockedObjectsReader *locked_objects_reader = NULL;

    try {

      // Ensure that the connection is established. Otherwise raise an
      // exception.


      if (fdo_rdbms_connection == NULL)
          throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                                         LockUtility::INVALID_DBI_CONNECTION));

      // Check if the lock name is set. If this is not the case terminate the
      // routine.

      if (requested_owner == NULL)
          throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                                         LockUtility::INVALID_LOCK_OWNER));

      // Create the reader and return it back to the calling routine.

     if ((locked_objects_reader =
                     new FdoRdbmsLockedObjectsReader(fdo_rdbms_connection,
												     requested_owner)) == NULL)
          throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                                         LockUtility::FAILED_TO_CREATE_LKOR));

      return locked_objects_reader;

    }  //  try ...

    catch (FdoException *ex) {

      FreeMemory();
      fdo_cmd_ex = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
      ex->Release();
      throw fdo_cmd_ex;

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      FreeMemory();
      throw;

    }  //  catch ( ... ) ...

}  //  Execute ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsGetLockedObjects::FreeMemory ()

// +---------------------------------------------------------------------------
// | The function frees memory.
// +---------------------------------------------------------------------------

{

    if (requested_owner != NULL) delete[] requested_owner;
    SetToZero();

}  //  FreeMemory ()

void FdoRdbmsGetLockedObjects::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    requested_owner = NULL;

}  //  SetToZero ()

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
// | The file contains the implementation of the class FdoRdbmsGetLockOwners.
// | The class retrieves a list of available locks in the current schema. The
// | information is provided to the user via the ILockOwnersReader interface.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "FdoRdbmsGetLockOwners.h"
#include "LockUtility.h"


FdoRdbmsGetLockOwners::FdoRdbmsGetLockOwners ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    dbi_connection = NULL;

}  //  FdoRdbmsGetLockOwners ()

FdoRdbmsGetLockOwners::FdoRdbmsGetLockOwners (FdoIConnection *connection)
    : FdoRdbmsCommand<FdoIGetLockOwners>(connection),
      dbi_connection(NULL)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Configure the RDBMS and DBI connection.

    fdo_rdbms_connection = static_cast<FdoRdbmsConnection*>(connection);
    if (fdo_rdbms_connection)
        dbi_connection = fdo_rdbms_connection->GetDbiConnection();

}  //  FdoRdbmsGetLockOwners ()

FdoRdbmsGetLockOwners::~FdoRdbmsGetLockOwners ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    dbi_connection       = NULL;
    fdo_rdbms_connection = NULL;

}  //  ~FdoRdbmsGetLockOwners ()

FdoILockOwnersReader *FdoRdbmsGetLockOwners::Execute ()

// +---------------------------------------------------------------------------
// | The function executes a request to retrieve the list of available locks
// | in the current schema.
// | NOTE: All the information that is to be returned by this request is stored
// |       in one table independent of environment (Workspace Manager or DBI).
// |       Therefore, the reader returned by this routine does all the work.
// |       The execute operation just returns a handle to the necessary reader.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoRdbmsLockOwnersReader *lock_owners_reader = NULL;

    // The request is executed only, if the connection and the class support
    // locking. If this is not the case issue an error.

    if (!LockUtility::IsLockSupported(fdo_rdbms_connection,
                                      NULL                 ))
        throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                      LockUtility::LOCK_SUPPORT_ERROR,
                                      NULL,
                                      L"FdoIGetLockOwners"));

    // Create the return object and passit back to the caller.

    if ((lock_owners_reader =
                 new FdoRdbmsLockOwnersReader(
                            FdoPtr<FdoIConnection>(GetConnection()))) == NULL)
         throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                              LockUtility::LOCK_OWNER_READER_GENERATION_ERROR));

    return lock_owners_reader;

}  //  Execute ()


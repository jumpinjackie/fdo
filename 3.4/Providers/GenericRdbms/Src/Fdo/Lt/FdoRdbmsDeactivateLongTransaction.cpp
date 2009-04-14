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
// | The file contains the implementation of the class FdoRdbmsDeactivateLong-
// | Transaction. The class deactivates the currently active long transaction
// | and activates the root long transaction. Any attempt to deactivate the
// | root long transaction is ignored.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "FdoRdbmsDeactivateLongTransaction.h"
#include "LockUtility.h"
#include "FdoRdbmsException.h"


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoRdbmsDeactivateLongTransaction::FdoRdbmsDeactivateLongTransaction ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the connection references.

    fdo_rdbms_connection = NULL;

}  //  FdoRdbmsDeactivateLongTransaction ()

FdoRdbmsDeactivateLongTransaction::FdoRdbmsDeactivateLongTransaction (
                                                    FdoIConnection *connection)
    : FdoRdbmsCommand<FdoIDeactivateLongTransaction>(connection)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the connection references.

    fdo_rdbms_connection = static_cast<FdoRdbmsConnection*>(connection);

}  //  FdoRdbmsDeactivateLongTransaction ()

FdoRdbmsDeactivateLongTransaction::~FdoRdbmsDeactivateLongTransaction ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Reset the connection references.

    fdo_rdbms_connection = NULL;

}  //  ~FdoRdbmsDeactivateLongTransaction ()

void FdoRdbmsDeactivateLongTransaction::Execute ()

// +---------------------------------------------------------------------------
// | The function executes the command.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoCommandException *fdo_cmd_ex = NULL;

    try {

      // Ensure that the connection is established. Otherwise raise an
      // exception.

      if (fdo_rdbms_connection == NULL)
          throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                                         LockUtility::INVALID_DBI_CONNECTION));

      // Deactivating the currently active long transaction. This automatically
      // activates the root long transaction.

      FdoPtr<FdoRdbmsLongTransactionManager>(fdo_rdbms_connection->GetLongTransactionManager())->Deactivate();

    }  //  try ...

    catch (FdoException *ex) {

      fdo_cmd_ex = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
      ex->Release();
      throw fdo_cmd_ex;

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      throw;

    }  //  catch ( ... ) ...

}  //  Execute ()

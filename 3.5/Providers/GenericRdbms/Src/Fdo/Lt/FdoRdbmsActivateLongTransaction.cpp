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
// | The file contains the implementation of the class FdoRdbmsActivateLong-
// | Transaction. The class activates the provided long transaction. Input to
// | the command is the name of the long transaction to be activated.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "FdoRdbmsActivateLongTransaction.h"
#include "LockUtility.h"
#include "FdoRdbmsException.h"
#include "FdoRdbmsLongTransactionReader.h"

// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoRdbmsActivateLongTransaction::FdoRdbmsActivateLongTransaction ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the connection references.

    fdo_rdbms_connection = NULL;

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsActivateLongTransaction ()

FdoRdbmsActivateLongTransaction::FdoRdbmsActivateLongTransaction (
                                                    FdoIConnection *connection)
    : FdoRdbmsCommand<FdoIActivateLongTransaction>(connection)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the connection references.

    fdo_rdbms_connection = static_cast<FdoRdbmsConnection*>(connection);

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsActivateLongTransaction ()

FdoRdbmsActivateLongTransaction::~FdoRdbmsActivateLongTransaction ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release the memory occupied by the class variables.

    ClearMemory ();

    // Reset the connection references.

    fdo_rdbms_connection = NULL;

}  //  ~FdoRdbmsActivateLongTransaction ()

FdoString *FdoRdbmsActivateLongTransaction::GetName ()

// +---------------------------------------------------------------------------
// | The function returns the value of the name property set for this command.
// +---------------------------------------------------------------------------

{

    return(lt_name);

}  //  GetName ()

void FdoRdbmsActivateLongTransaction::SetName (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the name property for this command.
// +---------------------------------------------------------------------------

{

    // Check if the given long transaction name is valid.

    if (value == NULL)
        throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                        LockUtility::LT_NAME_NULL_ERROR));

    if ((wcslen(value) >  DBI_VERSION_NAME_LENGTH) ||
        (wcslen(value) == 0                      )    )
         throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                        LockUtility::LT_NAME_LENGTH_ERROR));

    // The basic tests to ensure the given long transaction name is valid have
    // been completed and the given name can be stored with the command.

    if (lt_name != NULL) { delete[] lt_name; lt_name = NULL; }
    if ((lt_name = LockUtility::SetValue(value)) == NULL)
        throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                        LockUtility::MEMORY_ALLOCATION_ERROR));

}  //  SetName ()

void FdoRdbmsActivateLongTransaction::Execute ()

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

      // Check if the long transaction name is set. If this is not the case
      // terminate the routine.

      if (lt_name == NULL)
          throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                                   LockUtility::INVALID_LT_NAME,
                                   lt_name,
                                   L"FdoIActivateLongTransaction"));

      // Check if an attempt is made to activate the active long transaction via
      // the corresponding code word. If this is the case, nothing needs to be
      // done and the function can be terminated.

      if (wcscmp(lt_name, FDO_ACTIVELONGTRANSACTION) == 0) return;

      // Activate the named long transaction.

      FdoPtr<FdoRdbmsLongTransactionManager>(fdo_rdbms_connection->GetLongTransactionManager())->Activate(lt_name);

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


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsActivateLongTransaction::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    if (lt_name != NULL) delete[] lt_name;
    SetToZero();

}  //  ClearMemory ()

void FdoRdbmsActivateLongTransaction::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lt_name = NULL;

}  //  SetToZero ()

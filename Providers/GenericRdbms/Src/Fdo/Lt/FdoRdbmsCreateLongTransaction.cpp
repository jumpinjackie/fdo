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
// | The file contains the implementation of the class FdoRdbmsCreateLongTrans-
// | action. The class creates a new long transaction utilizing the given para-
// | meters. Input to the command is the name of the long transaction to be
// | created and an optional description of it.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "FdoRdbmsCreateLongTransaction.h"
#include "LockUtility.h"
#include "FdoRdbmsException.h"
#include "../Lt/version_info.h"
#include "FdoRdbmsLongTransactionReader.h"

// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoRdbmsCreateLongTransaction::FdoRdbmsCreateLongTransaction ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the connection references.

    dbi_connection       = NULL;
    fdo_rdbms_connection = NULL;

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsCreateLongTransaction ()

FdoRdbmsCreateLongTransaction::FdoRdbmsCreateLongTransaction (
                                                    FdoIConnection *connection)
    : FdoRdbmsCommand<FdoICreateLongTransaction>(connection),
      dbi_connection(NULL)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the connection references.

    fdo_rdbms_connection = static_cast<FdoRdbmsConnection*>(connection);
    if (fdo_rdbms_connection)
        dbi_connection = fdo_rdbms_connection->GetDbiConnection();

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsCreateLongTransaction ()

FdoRdbmsCreateLongTransaction::~FdoRdbmsCreateLongTransaction ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release the memory occupied by the class variables.

    ClearMemory ();

    // Reset the connection references.

    dbi_connection       = NULL;
    fdo_rdbms_connection = NULL;

}  //  ~FdoRdbmsCreateLongTransaction ()

FdoString *FdoRdbmsCreateLongTransaction::GetName ()

// +---------------------------------------------------------------------------
// | The function returns the value of the name property set for this command.
// +---------------------------------------------------------------------------

{

    return(lt_name);

}  //  GetName ()

void FdoRdbmsCreateLongTransaction::SetName (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the name property for this command.
// +---------------------------------------------------------------------------

{

    // Check if the given long transaction name is valid. This includes:
    //  -> Checking if the given name is not NULL.
    //  -> Checking if the given name is within the valid length limitations.
    //  -> Checking if the name represents a valid name according to the FDO
    //     rules (only alphanumeric values and underscores).
    //  -> Checking whether or not the given name is identical with predefined
    //     names.

    if (value == NULL)
        throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                        LockUtility::LT_NAME_NULL_ERROR));

    if ((wcslen(value) >  DBI_VERSION_NAME_LENGTH) ||
        (wcslen(value) == 0                      )    )
         throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                        LockUtility::LT_NAME_LENGTH_ERROR));

    if (!LockUtility::IsValidName((wchar_t *)value))
        throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                                LockUtility::INVALID_LT_NAME,
                                                value,
                                                L"FdoICreateLongTransaction"));

    if ((wcscmp(value, FDO_ACTIVELONGTRANSACTION) == 0) ||
        (wcscmp(value, FDO_ROOTLONGTRANSACTION  ) == 0)    )
         throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                                LockUtility::INVALID_LT_NAME,
                                                value,
                                                L"FdoICreateLongTransaction"));

    // The basic tests to ensure the given long transaction name is valid have
    // been completed and the given name can be stored with the command.

    if (lt_name != NULL) { delete[] lt_name; lt_name = NULL; }
    if ((lt_name = LockUtility::SetValue(value)) == NULL)
         throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                        LockUtility::MEMORY_ALLOCATION_ERROR));

}  //  SetName ()

FdoString *FdoRdbmsCreateLongTransaction::GetDescription ()

// +---------------------------------------------------------------------------
// | The function returns the value of the description property set for this
// | command.
// +---------------------------------------------------------------------------

{

    return(lt_description);

}  //  GetDescription ()

void FdoRdbmsCreateLongTransaction::SetDescription (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the description property for this command.
// +---------------------------------------------------------------------------

{

    // Process the given long transaction description.

    if (value != NULL)
        if (wcslen(value) > DBI_VERSION_DESC_RET_SIZE)
            throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                    LockUtility::LT_DESCRIPTION_LENGTH_ERROR));

    // If there is already a value stored, clear it. Then store a copy of the
    // given value.

    if (lt_description != NULL) {

        delete[] lt_description;
        lt_description = NULL;

    }  //  if (lt_description != ...

    if (value != NULL)
        if ((lt_description = LockUtility::SetValue(value)) == NULL)
             throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                        LockUtility::MEMORY_ALLOCATION_ERROR));

}  //  SetDescription ()

void FdoRdbmsCreateLongTransaction::Execute ()

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
                                   L"FdoICreateLongTransaction"));

      // Create the new long transaction.

      FdoPtr<FdoRdbmsLongTransactionManager>(fdo_rdbms_connection->GetLongTransactionManager())->Create(lt_name,
                                                                lt_description);

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

void FdoRdbmsCreateLongTransaction::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    if (lt_name        != NULL) delete[] lt_name;
    if (lt_description != NULL) delete[] lt_description;
    SetToZero();

}  //  ClearMemory ()

void FdoRdbmsCreateLongTransaction::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lt_name        = NULL;
    lt_description = NULL;

}  //  SetToZero ()

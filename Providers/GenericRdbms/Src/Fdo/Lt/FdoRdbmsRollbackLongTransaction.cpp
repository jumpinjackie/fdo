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
// | The file contains the implementation of the class FdoRdbmsRollbackLong-
// |  Transaction. The class executes either a full or partial rollback opera-
// | tion on the identified data. A full rollback operation includes the roll-
// | ing back of the named long transaction and all its descendents and remov-
// | ing of all the processed long transactions. A partial rollback operation
// | rolls back specified data in the named long transaction and all its de-
// | scendents, but does not remove the processed long transactions.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include "FdoRdbmsSchemaUtil.h"
#include <malloc.h>

#include "FdoRdbmsRollbackLongTransaction.h"
#include "FdoRdbmsException.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoRdbmsRollbackLongTransaction::FdoRdbmsRollbackLongTransaction ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the connection references.

    fdo_i_connection     = NULL;
    fdo_rdbms_connection = NULL;

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsRollbackLongTransaction ()

FdoRdbmsRollbackLongTransaction::FdoRdbmsRollbackLongTransaction (
                                                    FdoIConnection *connection)
    : FdoRdbmsFeatureCommand<FdoIRollbackLongTransaction>(connection)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the connection references.

    fdo_i_connection     = connection;
    fdo_rdbms_connection = static_cast<FdoRdbmsConnection*>(connection);

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsRollbackLongTransaction ()

FdoRdbmsRollbackLongTransaction::~FdoRdbmsRollbackLongTransaction ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release the memory occupied by the class variables.

    ClearMemory ();

    // Reset the connection references.

    fdo_rdbms_connection = NULL;

}  //  ~FdoRdbmsRollbackLongTransaction ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoString *FdoRdbmsRollbackLongTransaction::GetName ()

// +---------------------------------------------------------------------------
// | The function returns the value of the name property set for this command.
// +---------------------------------------------------------------------------

{

    return(lt_name);

}  //  GetName ()

void FdoRdbmsRollbackLongTransaction::SetName (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the name property for this command.
// +---------------------------------------------------------------------------

{

    // The given long transaction name cannot be NULL. Issue an error if this
    // is the case.

    if (value == NULL)
        throw FdoCommandException::Create(
                          NlsMsgGet(FDORDBMS_359,
                                    "Long transaction name cannot be null"));

    // If the given long transaction name is the same as the currently stored
    // one, exit the function.

    if ((lt_name != NULL) && (wcscmp(lt_name, value) == 0))
         return;

    // A new long transaction name has been identified. The following steps
    // ensure that the name is valid.
    // The supplied long transaction name may identify the root long trans-
    // action as the one to be rolled back. However, it is impossible to roll-
    // back the root long transaction. Only if the caller has used the constant
    // identifying the root long transaction can such an error be identified
    // and handled. If the caller has used the real root long transaction name
    // the error cannot be spotted here as the name depends on the underlying
    // long transaction concept. This will be caugth by the implementation of
    // the request within the Long Transaction Manager.
    // It is valid to rollback the active long transaction. If such a request
    // is issued the process will deactivate that long transaction first be-
    // fore proceeding the request.

    if ((wcslen(value) > 
                FDORDBMSLONGTRANSACTIONCONSTANTS_LT_NAME_MAX_LENGTH) ||
        (wcslen(value) == 0                                        )    )
        throw FdoCommandException::Create(
                      NlsMsgGet1(FDORDBMS_358, 
                                 "Invalid long transaction name '%1$ls'",
                                 value));

    if (wcscmp(value, FDO_ROOTLONGTRANSACTION) == 0)
        throw FdoCommandException::Create(
                     NlsMsgGet1(FDORDBMS_367, 
                                "%1$ls: Cannot process root long transaction",
                                L"FdoIRollbackLongTransaction"));

    // The basic tests to ensure the given long transaction name is valid have
    // been completed and the given name can be stored with the command.

    if (lt_name != NULL) { delete[] lt_name; lt_name = NULL; }
    if ((lt_name = SetValue(value)) == NULL)
        throw FdoCommandException::Create(
                        NlsMsgGet(FDORDBMS_91, "Failed to allocate memory"));

}  //  SetName ()

FdoILockConflictReader 
                    *FdoRdbmsRollbackLongTransaction::GetLockConflictReader ()

// +---------------------------------------------------------------------------
// | The function returns a handle to the lock conflict reader listing lock
// | conflicts that were detected during the execution of the command.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoILockConflictReader *tmp_lt_lk_cfl_reader = NULL;

    // The function returns the handle to the current lock conflict reader
    // back to the caller. The routine does not keep any copy to that reader.

    tmp_lt_lk_cfl_reader = lt_lk_cfl_reader;
    lt_lk_cfl_reader     = NULL;

    return(tmp_lt_lk_cfl_reader);

}  //  GetLockConflictReader ()

void FdoRdbmsRollbackLongTransaction::Execute ()

// +---------------------------------------------------------------------------
// | The function executes the command.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                           active_lt_reset         = false,
                                   lock_conflicts_found    = false,
                                   uses_active_lt_constant = false;

    FdoCommandException            *fdo_cmd_ex             = NULL;

    FdoRdbmsLongTransactionInfo    *active_lt_info         = NULL;

    FdoPtr<FdoRdbmsLongTransactionManager>lt_manager;

    try {

      // Issue an exception if the long transaction name is not set.

      if (lt_name == NULL)
          throw FdoCommandException::Create(
              NlsMsgGet2(
                FDORDBMS_363, 
                "Invalid long transaction name ('%1$ls') for command '%2$ls'",
                L"NULL",
                L"FdoIRollbackLongTransaction"));

      // Set a shortcut reference to the Long Transaction Manager. 

      lt_manager = fdo_rdbms_connection->GetLongTransactionManager();

      // Get the name of the currently active long transaction (there will al-
      // ways be an active long transaction).

      lt_manager->GetActive(&active_lt_info);

      // If the long transaction to be rolled back identifies the active long
      // transaction either by name or via the corresponding constant, it needs
      // to be deactivated first before the processing can start.

      if ((wcscmp(lt_name, FDO_ACTIVELONGTRANSACTION) == 0) ||
          (wcscmp(lt_name, active_lt_info->GetName()) == 0)    ) {

           // Remember whether or not the user identified the active long trans-
           // action with the corresponding constant.

           uses_active_lt_constant =
                             (wcscmp(lt_name, FDO_ACTIVELONGTRANSACTION) == 0);

           // Activate the root long transaction and remember that this was
           // done.

           lt_manager->Activate(FDO_ROOTLONGTRANSACTION);
           active_lt_reset = true;

      }  //  if ((wcscmp(lt_name, ...

      // Attempt to rollback the named long transaction.

      lt_manager->Rollback(((uses_active_lt_constant) 
                                ? active_lt_info->GetName()
                                : lt_name),
                           &lt_lk_cfl_reader,
                           &lock_conflicts_found);

      // If lock conflicts were identified, the long transaction was not rolled
      // back. In this case, if the named long transaction was the active long
      // transaction when the request was issued, activate the long transaction
      // again.

      if ((lock_conflicts_found) && (active_lt_reset)) {

           lt_manager->Activate(active_lt_info->GetName());
           active_lt_reset = false;

      }  //  if ((lock_conflicts_found) && ...

      // Clean up.

      active_lt_info->Release();
      active_lt_info = NULL;

      lt_manager     = NULL;

    }  //  try ...

    catch (FdoException *ex) {

      // If the user had attempted to rollback the active long transaction, re-
      // set the long transaction to be rolled back as the active long transac-
      // tion.

      if (active_lt_reset) {

          lt_manager->Activate(active_lt_info->GetName());
          active_lt_reset = false;

      }  //  if (active_lt_reset) ...

      if (active_lt_info != NULL) {

          active_lt_info->Release();
          active_lt_info = NULL;

      }  //  if (active_lt_info != NULL) ...

      if (lt_manager != NULL) lt_manager = NULL;

      fdo_cmd_ex = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
      ex->Release();
      throw fdo_cmd_ex;

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      // If the user had attempted to rollback the active long transaction, re-
      // set the long transaction to be rolled back as the active long transac-
      // tion.

      if (active_lt_reset) {

          lt_manager->Activate(active_lt_info->GetName());
          active_lt_reset = false;

      }  //  if (active_lt_reset) ...

      if (active_lt_info != NULL) {

          active_lt_info->Release();
          active_lt_info = NULL;

      }  //  if (active_lt_info != NULL) ...

      if (lt_manager != NULL) lt_manager = NULL;

      throw;

    }  //  catch ( ... ) ...

}  //  Execute ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsRollbackLongTransaction::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    if (lt_lk_cfl_reader != NULL) {

        lt_lk_cfl_reader->Release();
        lt_lk_cfl_reader = NULL;

    }  //  if (lt_lk_cfl_reader != NULL) ...

    if (lt_name != NULL) delete[] lt_name;
    SetToZero();

}  //  ClearMemory ()

void FdoRdbmsRollbackLongTransaction::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lt_name          = NULL;
    lt_lk_cfl_reader = NULL;

}  //  SetToZero ()

wchar_t *FdoRdbmsRollbackLongTransaction::SetValue (FdoString *value_buffer)

// +---------------------------------------------------------------------------
// | The function allocates memory for a string and initializes it with the
// | given value.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    wchar_t *value = NULL;

    // Allocate memory for the given buffer. If this fails terminate the
    // routine.

    if ((value = new wchar_t[wcslen(value_buffer) + 1]) == NULL) return value;
    wcscpy(value, value_buffer);
    return value;

}  //  SetValue ()

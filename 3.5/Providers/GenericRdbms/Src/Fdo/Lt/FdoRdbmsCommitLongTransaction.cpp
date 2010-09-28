/***************************************************************************
 * Copyright (C) 2004-2007  Autodesk, Inc.
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
// | The file contains the implementation of the class FdoRdbmsCommitLongTrans-
// | action. The class executes either a full or partial commit operation on
// | the identified data. A full commit operation includes the committing of
// | the named long transaction to its ancestor one and its removal. A partial
// | commit operation commits specified data in the named long transaction to
// | its parent long transaction, but does not remove the processed long trans-
// | actions.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include "FdoRdbmsSchemaUtil.h"
#include <malloc.h>

#include "FdoRdbmsCommitLongTransaction.h"
#include "FdoRdbmsException.h"
#include "FdoRdbmsLongTransactionReader.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoRdbmsCommitLongTransaction::FdoRdbmsCommitLongTransaction ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the connection references.

    fdo_i_connection     = NULL;
    fdo_rdbms_connection = NULL;

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsCommitLongTransaction ()

FdoRdbmsCommitLongTransaction::FdoRdbmsCommitLongTransaction (
                                                    FdoIConnection *connection)
    : FdoRdbmsFeatureCommand<FdoICommitLongTransaction>(connection)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the connection references.

    fdo_i_connection     = connection;
    fdo_rdbms_connection = static_cast<FdoRdbmsConnection*>(connection);

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsCommitLongTransaction ()

FdoRdbmsCommitLongTransaction::~FdoRdbmsCommitLongTransaction ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release the memory occupied by the class variables.

    ClearMemory ();

    // Reset the connection references.

    fdo_rdbms_connection = NULL;

}  //  ~FdoRdbmsCommitLongTransaction ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoString *FdoRdbmsCommitLongTransaction::GetName ()

// +---------------------------------------------------------------------------
// | The function returns the value of the name property set for this command.
// +---------------------------------------------------------------------------

{

    return(lt_name);

}  //  GetName ()

void FdoRdbmsCommitLongTransaction::SetName (FdoString *value)

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
    // The supplied long transaction name may identify the root long transac-
    // tion as the one to be committed. However, it is impossible to commit
    // the root long transaction. Only if the caller has used the constant
    // identifying the root long transaction can such an error be identified
    // and handled. If the caller has used the real root long transaction name
    // the error cannot be spotted here as the name depends on the underlying
    // long transaction concept. This will be caugth by the implementation of
    // the request within the Long Transaction Manager.
    // It is valid to commit the active long transaction. If such a request
    // is issued the process will deactivate that long transaction first be-
    // fore proceeding the request.

    if ((wcslen(value) >
                FDORDBMSLONGTRANSACTIONCONSTANTS_LT_NAME_MAX_LENGTH) ||
        (wcslen(value) == 0                                        )    )
        throw FdoCommandException::Create(
                NlsMsgGet2(
                    FDORDBMS_358, 
                    "%1$ls: Long transaction name ('%2$ls') too long (0 < name <= 30)",
                    L"FdoICommitLongTransaction",
                    value));

    if (wcscmp(value, FDO_ROOTLONGTRANSACTION) == 0)
        throw FdoCommandException::Create(
                     NlsMsgGet1(FDORDBMS_367, 
                                "%1$ls: Cannot process root long transaction",
                                L"FdoICommitLongTransaction"));

    // The basic tests to ensure the given long transaction name is valid have
    // been completed and the given name can be stored with the command.

    if (lt_name != NULL) { delete[] lt_name; lt_name = NULL; }
    if ((lt_name = SetValue(value)) == NULL)
        throw FdoCommandException::Create(
                        NlsMsgGet(FDORDBMS_91, "Failed to allocate memory"));

    // A new long tranaction name has been set. If the command still holds on
    // to a long transaction conflict enumerator, invalidate it.

    if (lt_cfl_enum != NULL) {

        lt_cfl_enum->Invalidate();
        lt_cfl_enum = NULL;

    }  //  if (lt_cfl_enum != NULL) ...

}  //  SetName ()

FdoILockConflictReader *FdoRdbmsCommitLongTransaction::GetLockConflictReader ()

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

FdoILongTransactionConflictDirectiveEnumerator
                                    *FdoRdbmsCommitLongTransaction::Execute ()

// +---------------------------------------------------------------------------
// | The function executes the command.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                           active_lt_reset         = FALSE,
                                   lk_conflicts_detected   = false,
                                   lt_conflicts_detected   = FALSE,
                                   uses_active_lt_constant = FALSE;

    FdoCommandException            *fdo_cmd_ex             = NULL;

    FdoRdbmsLongTransactionInfo    *active_lt_info         = NULL;

    FdoPtr<FdoRdbmsLongTransactionManager>lt_manager;

    try {

      // Check if the long transaction name is set. If this is not the case
      // terminate the routine.

      if (lt_name == NULL)
          throw FdoCommandException::Create(
              NlsMsgGet2(
                FDORDBMS_363, 
                "%1$ls: Invalid long transaction name ('%2$ls')",
                L"NULL",
                L"FdoICommitLongTransaction"));

      // Set a shortcut reference to the Long Transaction Manager. 

      lt_manager = fdo_rdbms_connection->GetLongTransactionManager();

      // Get the name of the currently active long transaction (there will al-
      // ways be an active long transaction).

      lt_manager->GetActive(&active_lt_info);

      // If the long transaction to be committed identifies the active long
      // transaction either by name or via the corresponding constant, it needs
      // to be deactivated first before the processing can start.

      if ((wcscmp(lt_name, FDO_ACTIVELONGTRANSACTION) == 0) ||
          (wcscmp(lt_name, active_lt_info->GetName()) == 0)     ) {

           // Remember whether or not the user identified the active long trans-
           // action with the corresponding constant.

           uses_active_lt_constant =
                             (wcscmp(lt_name, FDO_ACTIVELONGTRANSACTION) == 0);

           // Activate the root long transaction and remember that this was
           // done.

           lt_manager->Activate(FDO_ROOTLONGTRANSACTION);
           active_lt_reset = TRUE;

      }  //  if ((wcscmp(lt_name, ...

      // If the command still owns a lock conflict reader at this point release
      // it.

      if (lt_lk_cfl_reader != NULL) {

          lt_lk_cfl_reader->Close();
          lt_lk_cfl_reader->Release();
          lt_lk_cfl_reader = NULL;

      }  //  if (lt_lk_cfl_reader != NULL) ...

      // If there is an active long transaction conflict enumerator, process the
      // long transaction conflicts first before attempting to commit the named
      // long tranaction.

      if (lt_cfl_enum != NULL) {

          ProcessLTConflicts();

          // All conflicts were successfully processed. Clear the long trans-
          // action conflict structures.

          lt_cfl_enum->Invalidate();
          lt_cfl_enum = NULL;

          if (lt_conflicts != NULL) {

              lt_conflicts->Clear();
              lt_conflicts = NULL;

          }  //  if (lt_conflicts != NULL);

      }  //  if (lt_cfl_enum != NULL) ...

      // Attempt to commit the named long transaction.
      // NOTE: The last parameter must be a pointer to the structure that keeps
      //       the long transaction conflicts. This structure needs still to be
      //       defined. For now, the Long Transaction Manager interface expects
      //       a reference to a FdoRdbmsLongTransactionConflictDirectiveEnumer-
      //       ator which may or may not be kept.

      lt_manager->Commit(((uses_active_lt_constant) 
                                ? active_lt_info->GetName()
                                : lt_name),
                           &lt_lk_cfl_reader,
                           &lk_conflicts_detected,
                           &lt_conflicts);

      // Create the long transaction conflict enumerator to be returned back to
      // the caller.
      // NOTE: The NULL pointer has to be replaced with the pointer to the root
      //       of the structure that holds the long transaction conflict infor-
      //       mation.

      if ((lt_cfl_enum =
            new FdoRdbmsLongTransactionConflictDirectiveEnumerator(
                                            fdo_rdbms_connection,
                                            lt_conflicts         )) == NULL)
           throw FdoCommandException::Create(
                    NlsMsgGet(
                      FDORDBMS_375,
                      "Failed to create long transaction conflict enumerator"));

      lt_cfl_enum->AddRef();

      // If the user had attempted to commit the active long transaction and
      // long transaction or lock conflicts are detected, the long transaction
      // was not committed. In this case re-activate the long transaction.

      if ((active_lt_reset)                                    &&
          ((lt_conflicts_detected) || (lt_conflicts_detected))    ) {

           lt_manager->Activate(active_lt_info->GetName());
           active_lt_reset = false;

      }  //  if ((active_lt_reset) && ...

      // Clean up.

      active_lt_info->Release();
      active_lt_info = NULL;

      lt_manager     = NULL;

      // Return the enumerator.

      return lt_cfl_enum;

    }  //  try ...

    catch (FdoException *ex) {

      // If the user had attempted to commit the active long transaction, reset
      // the long transaction to be committed as the active long transaction.

      if (active_lt_reset) {

          lt_manager->Activate(active_lt_info->GetName());
          active_lt_reset = FALSE;

      }  //  if (active_lt_reset) ...

      // Clean up.

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

      // If the user had attempted to commit the active long transaction, reset
      // the long transaction to be committed as the active long transaction.

      if (active_lt_reset) {

          lt_manager->Activate(active_lt_info->GetName());
          active_lt_reset = FALSE;

      }  //  if (active_lt_reset) ...

      // Clean up.

      if (active_lt_info != NULL) {

          active_lt_info->Release();
          active_lt_info = NULL;

      }  //  if (active_lt_info != NULL) ...

      if (lt_manager != NULL) lt_manager = NULL;

      // Rethrow the exception.

      throw;

    }  //  catch ( ... ) ...

}  //  Execute ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

bool FdoRdbmsCommitLongTransaction::CheckConflictResolutionFlags ()

// +---------------------------------------------------------------------------
// | The function processes long transaction conflicts.
// +---------------------------------------------------------------------------

{

    // -----------------------------------------------------------------------
    // NOTE:
    // This function is not required yet because the Generic Long Transaction
    // Manager does not support cases yet that may result in long transaction
    // conflicts. Therefore, for now, just return TRUE and implement the pro-
    // cedure once required.
    // -----------------------------------------------------------------------

    return true;

}  //  CheckConflictResolutionFlags ()

void FdoRdbmsCommitLongTransaction::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function releases memory occupied by the various member variables.
// +---------------------------------------------------------------------------

{

    // If the command still holds on to a long transaction conflict enumerator
    // remove it.

    if (lt_cfl_enum != NULL) {

        lt_cfl_enum->Invalidate();
        lt_cfl_enum = NULL;

    }  //  if (lt_cfl_enum != NULL) ...

    if (lt_lk_cfl_reader != NULL) {

        lt_lk_cfl_reader->Release();
        lt_lk_cfl_reader = NULL;

    }  //  if (lt_lk_cfl_reader != NULL) ...

    if (lt_conflicts != NULL)
        lt_conflicts->Clear();

    // Handle other member variables.

    if (lt_name != NULL) delete[] lt_name;
    SetToZero();

}  //  ClearMemory ()

void FdoRdbmsCommitLongTransaction::ProcessLTConflicts ()

// +---------------------------------------------------------------------------
// | The function processes long transaction conflicts.
// +---------------------------------------------------------------------------

{

    // -----------------------------------------------------------------------
    // NOTE:
    // In a Generic Long Transaction Manager environment, no long transaction
    // conflicts exist because ...
    //  ... a user is not allowed to update an object in a version other than
    //      the most recent one.
    //  ... multi-branching is not supported and hence, a parent version can-
    //      not be modified by committing one branch while a different one is
    //      still active.
    // So, for now, just return and implement the solution once required. This
    // also needs additional data structures the Long Transaction Conflict
    // Directive Enumerator can work on.
    // -----------------------------------------------------------------------



}  //  ProcessLTConflicts ()

void FdoRdbmsCommitLongTransaction::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lt_name          = NULL;
    lt_cfl_enum      = NULL;
    lt_conflicts     = NULL;
	lt_lk_cfl_reader = NULL;

}  //  SetToZero ()

wchar_t *FdoRdbmsCommitLongTransaction::SetValue (FdoString *value_buffer)

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

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
// | The file contains the implementation of the class FdoRdbmsReleaseLock. The
// | class releases all locks on instance data identified by the data.
// =============================================================================

#include "stdafx.h"
#include "FdoRdbmsSchemaUtil.h"
#include <malloc.h>
#include "FdoCommonOSUtil.h"

// The links to the header(s) to be included.

#include "FdoRdbmsReleaseLock.h"
#include "FdoRdbmsLockConflictReader.h"
#include "LockUtility.h"
#include "FdoRdbmsFilterUtil.h"
#include "FdoRdbmsFilterProcessor.h"
#ifndef _WIN32
#ifdef stricmp
#undef stricmp
#endif
#endif


FdoRdbmsReleaseLock::FdoRdbmsReleaseLock ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();
    dbi_connection = NULL;

}  //  FdoRdbmsReleaseLock ()

FdoRdbmsReleaseLock::FdoRdbmsReleaseLock (FdoIConnection *connection)
    : FdoRdbmsFeatureCommand<FdoIReleaseLock>(connection),
      dbi_connection (NULL)

// +---------------------------------------------------------------------------
// | The function represents another class constructor.
// +---------------------------------------------------------------------------

{

    // Configure the RDBMS and DBI connection.

    fdo_i_connection     = connection;
    fdo_rdbms_connection = static_cast<FdoRdbmsConnection*>(connection);
    if (fdo_rdbms_connection)
        dbi_connection = fdo_rdbms_connection->GetDbiConnection();

    // Initialize the member variables.

    SetToZero();

}  //  FdoRdbmsReleaseLock ()

FdoRdbmsReleaseLock::~FdoRdbmsReleaseLock()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release memory allocated by the class variables and reset the connection
    // objects.

    FreeMemory();
    dbi_connection       = NULL;
    fdo_rdbms_connection = NULL;

}  //  ~FdoRdbmsReleaseLock ()

FdoString *FdoRdbmsReleaseLock::GetLockOwner ()

// +---------------------------------------------------------------------------
// | The function returns the value of the member variable storing the lock
// | owner currently set by the user.
// +---------------------------------------------------------------------------

{

    return(lock_owner);

}  //  GetLockOwner ()

void FdoRdbmsReleaseLock::SetLockOwner (FdoString *lockOwner)

// +---------------------------------------------------------------------------
// | The function sets the value of the member variable storing the lock owner
// | to the given value.
// +---------------------------------------------------------------------------

{

    // The function can be used to reset the assigned lock owner name to the
    // command. Therefore, a NULL pointer is a valid entry. This case is
    // handled first.

    if (lockOwner == NULL) {

        // If the class variable holding the associated lock owner name for
        // the command is set, free the occupied memory an set the class
        // proprty to NULL.

        if (lock_owner != NULL) delete[] lock_owner;
        lock_owner = NULL;
        return;

    }  //  if (lockOwner == NULL) ...

    // A lock owner name has been given. Check if the given name is valid.
    // This includes:
    //    -> checking the length of the name.
    //    -> checking if the name consists of spaces only or if it has leading
    //       and/or trailing blanks.
    // If any of these rules are broken throw an exception.

    if ((wcslen(lockOwner) >  DBI_LKNM_LENGTH) ||
        (wcslen(lockOwner) == 0              ) ||
        (!LockUtility::IsValidName(lockOwner))    )
         throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                        LockUtility::INVALID_LOCK_OWNER));

    // The basic tests to ensure the given lock owner name is valid have been
    // completed and the given name can be stored with the command.

    if (lock_owner != NULL) delete[] lock_owner;
    if ((lock_owner = LockUtility::SetValue(lockOwner)) == NULL)
         throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                        LockUtility::MEMORY_ALLOCATION_ERROR));

}  //  SetLockOwner ()

FdoILockConflictReader* FdoRdbmsReleaseLock::Execute ()

// +---------------------------------------------------------------------------
// | The function executes an unlock request.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                       is_administrator_op   = FALSE;

    FdoStringP                 current_user;

    FdoCommandException        *fdo_cmd_ex           = NULL;

    FdoRdbmsLockConflictReader *lock_conflict_reader = NULL;

    try {

      // Ensure that the connection is established. Otherwise raise an exception.

      if (dbi_connection == NULL)
          throw FdoCommandException::Create(
                            LockUtility::GetExceptionMessage(
                                         LockUtility::INVALID_DBI_CONNECTION));

      // Check if the class name is set. If this is not the case terminate the
      // routine.

      if (this->GetClassNameRef() == NULL)
          throw FdoCommandException::Create(
                            LockUtility::GetExceptionMessage(
                                             LockUtility::INVALID_CLASS_NAME));

      // Get the current user.

      current_user = dbi_connection->GetUser();

      // If the lock owner is set and is different from the current user, an
      // attempt is made by the current user to unlock objects locked by a
      // different user. Such an attempt is valid only if the current user
      // has administrative privileges. If this is not the case the request
      // has to be denied.

      if ((lock_owner != NULL) && (!LockOwnerIsUser(current_user)))

          if (!UserIsAdministrator(current_user))
              throw FdoCommandException::Create(
                            LockUtility::GetExceptionMessage(
                                    LockUtility::ADMINISTRATOR_ROLE_REQUIRED));
          else {

            is_administrator_op = TRUE;

          }  //  else ...

      // Execute the request

	  lock_conflict_reader = ExecuteUnlockRequest(is_administrator_op);

      // Clean up.

      // Return the lock conflict reader back to the caller.

      return lock_conflict_reader;

    }  // try ...

    catch (FdoException *ex) {

      // Clean up.

      fdo_cmd_ex = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
      ex->Release();
      throw fdo_cmd_ex;

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      // Clean up.

      throw;

    }  //  catch ( ... ) ...

}  //  Execute ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

bool FdoRdbmsReleaseLock::UserIsAdministrator (const wchar_t *current_user)

// +---------------------------------------------------------------------------
// | The function returns TRUE if the user identified by the given name has
// | administration priviledges, FALSE otherwise.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    return TRUE;

}  //  UserIsAdministrator ()

bool FdoRdbmsReleaseLock::LockOwnerIsUser (const wchar_t *current_user)

// +---------------------------------------------------------------------------
// | The function returns TRUE if the lock owner is the current user, FALSE
// | otherwise.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool lock_owner_is_user;

    // Determine whether or not the current user is identical with the one
    // identified by the lock owner property.

    lock_owner_is_user =
                    (FdoCommonOSUtil::wcsicmp(current_user, lock_owner) == 0);

    return lock_owner_is_user;

}  //  LockOwnerIsUser ()

FdoRdbmsLockConflictReader *FdoRdbmsReleaseLock::ExecuteUnlockRequest (bool is_administrator_op)
                            
// +---------------------------------------------------------------------------
// | The function executes the requested unlock operation.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                    filter_modified                     = FALSE,
                            transaction_started                 = FALSE,
                            class_name_modified                 = FALSE;

    FdoStringP              current_lock,
                            current_user;
    wchar_t                 lock_user[PROCESS_NAME_LENGTH+1];

    FdoFilter               *fdo_filter                         = NULL;

    FdoIdentifier           *class_name                         = NULL,
                            *temp_class_name                    = NULL,
                            *modified_class_name                = NULL;

    FdoITransaction         *fdo_i_transaction                  = NULL;
	char			   *tableName;
	char			   *sqlFilter;
	int					execution_status;

    try {

      // Get the current class name. The class name can come in any of the
      // following structures:
      //
      //   just a class name (like "testClass")
      //   <class_name>.<property>[.<property>]
      //   <schema>:<class_name> (like "testDb:testClass")
      //
      // At the end of the process a usable class name is available.

      class_name = GetFeatureClassName();
      modified_class_name = LockUtility::GetClassName(class_name,
                                                      &class_name_modified);

      conflict_class_name = modified_class_name;
      modified_class_name->AddRef();

      // The request is executed only, if the connection and the class support
      // locking. If this is not the case issue an error.

      if (!LockUtility::IsLockSupported(fdo_rdbms_connection,
                                        modified_class_name))
          throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                      LockUtility::LOCK_SUPPORT_ERROR,
                                      modified_class_name->GetName(),
                                      L"FdoIReleaseLock"));

      // Keep a reference to the current lock and user. The current lock is
      // either the current user or the user being set as the lock owner for
      // the command.

      current_lock = (lock_owner != NULL)
                   ? FdoStringP(lock_owner)
                   : dbi_connection->GetUser();

      current_user = dbi_connection->GetUser();

      // Ensure that a lock for the current user is defined in the current
      // schema.

      //if (!LockUtility::MaintainLockData(fdo_rdbms_connection,current_lock)) {

      //    return NULL;

      //}  //  if (!LockUtility::MaintainLockData( ...

      // If this is an administrative unlock request, the specified lock must
      // be set active. Otherwise, the lock is already activated. The activa-
      // tion of the lock has to be undone before the procedure finishes.
      // NOTE: In a Workspace Manager enabled environment, the request to acti-
      //       vate a lock is ignored.

      if (is_administrator_op)
          if (!LockUtility::ActivateLock(fdo_rdbms_connection,
                                         current_lock,
                                         lock_user      )) {

              return FALSE;

          }  //  if (!LockUtility::ActivateLock( ...

      // Modify the filter if the class name has been changed.

      if (class_name_modified) {

          temp_class_name =
              FdoRdbmsFilterUtil::ConvertFilterToMainClass(class_name,
                                                         this->GetFilterRef());
          temp_class_name->Release();
          filter_modified = TRUE;

      }  //  if (class_name_modified) ...

      // Get hold of the filter.

      fdo_filter = GetFilter();

      // Start a transaction to cover the following modifications. They may
      // have to be rolled back depending on the execution status of the pro-
      // cedure removing the lock. Don't start a transaction if there is one
      // already active.

      if (!fdo_rdbms_connection->GetIsTransactionStarted()) {

          if ((fdo_i_transaction =
                        fdo_rdbms_connection->BeginTransaction()) == NULL) {

               // The transaction could not be started. Do some clean up and
               // terminate the routine.
               // - If the class name was modified and the filter was changed
               //   reset the filter to its original value.

               if ((class_name_modified         ) &&
                   (filter_modified             ) &&
                   (this->GetFilterRef() != NULL)    )
                    FdoRdbmsFilterUtil::ConvertFilterToObjectClass(
                                                        class_name,
                                                        this->GetFilterRef());

               // - Free some memory.

               if ((class_name_modified) && (modified_class_name != NULL))
                    modified_class_name->Release();
               modified_class_name = NULL;
               class_name->Release();
               class_name = NULL;
               if (fdo_filter != NULL) {

                   fdo_filter->Release();
                   fdo_filter = NULL;

               }  //  if (fdo_filter != ...

               // - If this is an administrator unlock request, reset the active
               //   lock to the one of the current user and handle the corres-
               //   ponding references.

               if (is_administrator_op)
                   LockUtility::ActivateLock(fdo_rdbms_connection,
                                             current_user,
                                             lock_user      );

               return FALSE;

          }  //  if ((fdo_i_transaction = ...

          transaction_started = TRUE;

      }  //  if (!fdo_rdbms_connection->GetIsTransactionStarted()) ...

      // Process the unlock request.

	  execution_status =
                LockUtility::ProcessLockRequest(fdo_rdbms_connection,
                                                FdoPtr<FdoRdbmsFilterProcessor>(FdoPtr<FdoRdbmsConnection>((FdoRdbmsConnection*)GetConnection())->GetFilterProcessor()),
                                                class_name,
                                                modified_class_name,
												class_name_modified,
                                                fdo_filter,
												&tableName,
												&sqlFilter);
	  // transaction lock
	  execution_status = 
		      FdoPtr<FdoRdbmsLockManager>(fdo_rdbms_connection->GetLockManager())->AcquireLock(LockUtility::ConvertString(tableName), 
			                                                      LockUtility::ConvertString(sqlFilter), 
																  (FdoString *)NULL, FdoLockType_Transaction);

	  FdoRdbmsLockConflictQueryHandler *lockConflictQuery = 
				FdoPtr<FdoRdbmsLockManager>(fdo_rdbms_connection->GetLockManager())->LockConflictQuery(LockUtility::ConvertString(tableName),
																		  LockUtility::ConvertString(sqlFilter));

	  FdoRdbmsLockConflictReader *conflictReader = new FdoRdbmsLockConflictReader(fdo_rdbms_connection, lockConflictQuery, modified_class_name);

	  //unlock
	  execution_status = FdoPtr<FdoRdbmsLockManager>(fdo_rdbms_connection->GetLockManager())->UnlockData(LockUtility::ConvertString(tableName),
																		    LockUtility::ConvertString(sqlFilter), 
																		    lockConflictQuery->GetConflictDbObject());

      // If a transaction was started, it has to be handled: it is rolled back
      // if the execution of the unlock request failed, committed otherwise.

      if (transaction_started) {

          if (!execution_status)
              fdo_i_transaction->Rollback();
          else
            fdo_i_transaction->Commit();

          fdo_i_transaction->Release();

      }  //  if (transaction_started) ...

      // Perform some cleaning up:
      // - If the filter was modified, set it back.

      if ((class_name_modified         ) &&
          (filter_modified             ) &&
          (this->GetFilterRef() != NULL)    )
          FdoRdbmsFilterUtil::ConvertFilterToObjectClass(class_name,
                                                     this->GetFilterRef());

      // - Free some memory.

      if ((class_name_modified) && (modified_class_name != NULL))
           modified_class_name->Release();
      modified_class_name = NULL;

      if (class_name   != NULL) { class_name->Release(); class_name   = NULL; }
      if (fdo_filter   != NULL) { fdo_filter->Release(); fdo_filter   = NULL; }

      // - If this is an administrator unlock request, reset the active lock to
      //   the one of the current user and handle the corresponding references.

      if (is_administrator_op)
          LockUtility::ActivateLock(fdo_rdbms_connection,
                                    current_user,
                                    lock_user      );

      return conflictReader;

    }  //  try ...

    catch ( ... ) {

      // Reset the filter if it was changed.

      if ((class_name_modified         ) &&
          (filter_modified             ) &&
          (this->GetFilterRef() != NULL)    )
          FdoRdbmsFilterUtil::ConvertFilterToObjectClass(class_name,
                                                     this->GetFilterRef());

      // Free some memory.

      if ((class_name_modified) && (modified_class_name != NULL))
           modified_class_name->Release();
      modified_class_name = NULL;

      if (class_name   != NULL) { class_name->Release(); class_name   = NULL; }
      if (fdo_filter   != NULL) { fdo_filter->Release(); fdo_filter   = NULL; }

      // If this is an administrator unlock request, reset the active lock to
      // the one of the current user and handle the corresponding references.

      if (is_administrator_op)
          LockUtility::ActivateLock(fdo_rdbms_connection,
                                    current_user,
                                    lock_user      );

      throw;

    }  //  catch ( ... ) ...

}  //  ExecuteUnlockRequest ()

void FdoRdbmsReleaseLock::FreeMemory ()

// +---------------------------------------------------------------------------
// | The function releases memory occupied by the member variables.
// +---------------------------------------------------------------------------

{

    if (lock_owner          != NULL) delete[] lock_owner;
    if (conflict_class_name != NULL) conflict_class_name->Release();
    SetToZero();

}  //  FreeMemory ()

void FdoRdbmsReleaseLock::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lock_owner          = NULL;
    conflict_class_name = NULL;

}  //  SetToZero ()

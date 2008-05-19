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
// | The file contains the implementation of the class FdoRdbmsAcquireLock. The
// | class locks instances of a given class that match the specified criteria.
// | Input to the acquire lock command includes the name of the class, the lock
// | type and strategy and filter criteria by which to identify the instances to
// | be locked. The filter may be specified either as text or as an expression
// | tree (most likely produced be a query builder).
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include "FdoRdbmsSchemaUtil.h"
#include <malloc.h>

#include "FdoRdbmsAcquireLock.h"
#include "LockUtility.h"
#include "FdoRdbmsFilterUtil.h"
#include "FdoRdbmsFilterProcessor.h"
#include "FdoRdbmsException.h"
#include "LockConflictQueryHandler.h"


FdoRdbmsAcquireLock::FdoRdbmsAcquireLock ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    SetToZero();
    dbi_connection       = NULL;
    fdo_rdbms_connection = NULL;

}  //  FdoRdbmsAcquireLock ()

FdoRdbmsAcquireLock::FdoRdbmsAcquireLock (FdoIConnection *connection)
    : FdoRdbmsFeatureCommand<FdoIAcquireLock>(connection)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Configure the RDBMS and DBI connection.

    fdo_i_connection = connection;
    fdo_rdbms_connection = static_cast<FdoRdbmsConnection*>(connection);
    if (fdo_rdbms_connection)
        dbi_connection = fdo_rdbms_connection->GetDbiConnection();

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsAcquireLock ()

FdoRdbmsAcquireLock::~FdoRdbmsAcquireLock ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    dbi_connection       = NULL;
    fdo_rdbms_connection = NULL;

    FreeMemory();

}  //  ~FdoRdbmsAcquireLock ()

FdoLockType FdoRdbmsAcquireLock::GetLockType ()

// +---------------------------------------------------------------------------
// | The function returns the current value of the lock type property.
// +---------------------------------------------------------------------------

{

    return lock_type;

}  //  GetLockType ()

void FdoRdbmsAcquireLock::SetLockType (FdoLockType lockType)

// +---------------------------------------------------------------------------
// | The function sets the value of the lock type property.
// +---------------------------------------------------------------------------

{

    FdoInt32 lockTypeCount;
	bool valid = false;
	FdoLockType *lockTypes = 
		         fdo_rdbms_connection->GetConnectionCapabilities()->GetLockTypes(lockTypeCount);
	for (int i=0; i<lockTypeCount && !valid; i++)
		valid = lockTypes[i] == lockType;

	if (!valid)
		throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                                     LockUtility::LOCK_TYPE_INPUT_ERROR,
                                            L"FdoLockType_Unsupported",
											L"FdoIAquireLock"));

	lock_type = lockType;
} 

FdoLockStrategy FdoRdbmsAcquireLock::GetLockStrategy ()

// +---------------------------------------------------------------------------
// | The function returns the current value of the lock strategy property.
// +---------------------------------------------------------------------------

{

    return lock_strategy;

}  //  GetLockStrategy ()

void FdoRdbmsAcquireLock::SetLockStrategy (FdoLockStrategy lockStrategy)

// +---------------------------------------------------------------------------
// | The function sets the value of the lock strategy property.
// +---------------------------------------------------------------------------

{

    lock_strategy = lockStrategy;

}  //  SetLockStrategy ()

FdoILockConflictReader *FdoRdbmsAcquireLock::Execute ()

// +---------------------------------------------------------------------------
// | The function executes the invoked lock request.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                       throw_exception       = FALSE;

    FdoCommandException        *fdo_cmd_ex           = NULL;

    FdoRdbmsLockConflictReader *lock_conflict_reader = NULL;

    try {

      // Ensure that the connection is established. Otherwise raise an
      // exception.

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

      // Execute the request.

      lock_conflict_reader = ExecuteLockRequest();
          /*throw FdoCommandException::Create(
                     LockUtility::GetExceptionMessage(
                                            LockUtility::DBI_LOCK_FAILURE));*/

      // Return the lock conflict reader back to the caller.

      return lock_conflict_reader;

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

FdoRdbmsLockConflictReader *FdoRdbmsAcquireLock::ExecuteLockRequest ()

// +---------------------------------------------------------------------------
// | The function executes the requested lock operation.
// +---------------------------------------------------------------------------

{
	FdoRdbmsLockConflictReader *conflictReader = NULL;
	// get class name, filter
	// get table name sql filter
	// start transaction
	// get lock conflicts
	// acquire lock


    // Declare and initialize all necessary local variables.

    bool               filter_modified         = FALSE,
                       transaction_started     = FALSE,
                       class_name_modified     = FALSE;

    FdoFilter          *fdo_filter             = NULL;

    FdoIdentifier      *class_name             = NULL,
                       *temp_class_name        = NULL,
                       *modified_class_name    = NULL;

    FdoITransaction    *fdo_i_transaction      = NULL;
	char			   *tableName;
	char			   *sqlFilter;
	int					execution_status;

    try {

      // Get the current class name. The class name can come in any of the
      // following structures:
      //
      //   just a class name (like "testClass")
      //   <class_name>.<property>[.<property>]
      //   <schema>:<class_name> (like "LockTests:testClass")
      //
      // At the end of the process a usable class name is available.

      class_name = GetFeatureClassName();
      req_class_name = LockUtility::GetClassName(class_name,
                                                      &class_name_modified);
      // The request is executed only, if the connection and the class support
      // locking. If this is not the case issue an error.

      if (!LockUtility::IsLockSupported(fdo_rdbms_connection,
                                        req_class_name))
          throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                      LockUtility::LOCK_SUPPORT_ERROR,
                                      req_class_name->GetName(),
                                      L"FdoIAcquireLock"));

	  

      if ((class_name_modified) && (this->GetFilterRef() != NULL)) {

          temp_class_name =
              FdoRdbmsFilterUtil::ConvertFilterToMainClass(class_name,
                                                         this->GetFilterRef());
          temp_class_name->Release();
          filter_modified = TRUE;

      }  //  if (class_name_modified) ...

      execution_status =
                LockUtility::ProcessLockRequest(fdo_rdbms_connection,
                                                FdoPtr<FdoRdbmsFilterProcessor>(FdoPtr<FdoRdbmsConnection>((FdoRdbmsConnection*)GetConnection())->GetFilterProcessor()),
                                                class_name,
                                                req_class_name,
												class_name_modified,
                                                GetFilter(),
												&tableName,
												&sqlFilter);
	  if (!execution_status)
		throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                      LockUtility::LOCK_REQUEST_PROCESSING_ERROR));

	  // Start a transaction to cover the following modifications. They may
      // have to be rolled back depending on the execution status of the pro-
      // cedure applying the lock. Don't start a transaction if there is one
      // already active.
	  FdoLockType lockType = GetLockType();

	  if (!fdo_rdbms_connection->GetIsTransactionStarted()) {

		  if (lockType == FdoLockType_Transaction)
			  throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                      LockUtility::LOCK_ACTIVE_TRANSACTION_ERROR));

          if ((fdo_i_transaction =
                        fdo_rdbms_connection->BeginTransaction()) == NULL) {

               if ((class_name_modified         ) &&
                   (filter_modified             ) &&
                   (this->GetFilterRef() != NULL)    )
                    FdoRdbmsFilterUtil::ConvertFilterToObjectClass(
                                                        class_name,
                                                        this->GetFilterRef());
               if ((class_name_modified) && (req_class_name != NULL))
                    req_class_name->Release();
               req_class_name = NULL;
               class_name->Release();
               class_name = NULL;
               if (fdo_filter != NULL) {

                   fdo_filter->Release();
                   fdo_filter = NULL;

               }  //  if (fdo_filter != NULL) ...

               return conflictReader;

          }  //  if ((fdo_i_transaction = ...

          transaction_started = TRUE;

      }  //  if (!fdo_rdbms_connection->GetIsTransactionStarted()) ...

	  execution_status = 
		      FdoPtr<FdoRdbmsLockManager>(fdo_rdbms_connection->GetLockManager())->AcquireLock(LockUtility::ConvertString(tableName), 
			                                                      LockUtility::ConvertString(sqlFilter), 
																  (FdoString *)NULL, FdoLockType_Transaction);

	  FdoRdbmsLockConflictQueryHandler *lockConflictQuery = 
				FdoPtr<FdoRdbmsLockManager>(fdo_rdbms_connection->GetLockManager())->LockConflictQuery(LockUtility::ConvertString(tableName),
																		  LockUtility::ConvertString(sqlFilter));

	  if (lockType != FdoLockType_Transaction)
	  {
		execution_status = 
			FdoPtr<FdoRdbmsLockManager>(fdo_rdbms_connection->GetLockManager())->AcquireLock(LockUtility::ConvertString(tableName), 
									   LockUtility::ConvertString(sqlFilter), 
									   lockConflictQuery->GetConflictDbObject(), lockType);

		// If a transaction was started, it has to be handled: it is rolled back
		// if the execution of the lock request fails, committed otherwise.
		if (!execution_status)
		{
			if (transaction_started)
			{
				fdo_i_transaction->Rollback();
				fdo_i_transaction->Release();
			}
		}
		else
		{	
			if (transaction_started)
			{	
				fdo_i_transaction->Commit();
				fdo_i_transaction->Release();
			}
		}
	  }

	  // Finally, create a lock conflict reader.
	  if (execution_status)
		conflictReader = new FdoRdbmsLockConflictReader(fdo_rdbms_connection, 
		                                                lockConflictQuery, req_class_name);
		
      // If the filter was modified, set it back.

      if ((class_name_modified         ) &&
          (filter_modified             ) &&
          (this->GetFilterRef() != NULL)    )
           FdoRdbmsFilterUtil::ConvertFilterToObjectClass(class_name,
                                                      this->GetFilterRef());

      // Do some cleaning up and return the execution status back to the calling
      // routine.

      if ((class_name_modified) && (modified_class_name != NULL))
           modified_class_name->Release();
      modified_class_name = NULL;
      class_name->Release();
      class_name = NULL;
      if (fdo_filter != NULL) {fdo_filter->Release(); fdo_filter = NULL;}

      return conflictReader;

    }  //  try ...

    catch ( ... ) {

      if (fdo_i_transaction != NULL) {

          fdo_i_transaction->Rollback();
          fdo_i_transaction->Release();

      }  //  if (fdo_i_transaction != NULL)

      if ((class_name_modified         ) &&
          (filter_modified             ) &&
          (this->GetFilterRef() != NULL)    )
           FdoRdbmsFilterUtil::ConvertFilterToObjectClass(class_name,
                                                      this->GetFilterRef());

      if ((class_name_modified) && (modified_class_name != NULL))
           modified_class_name->Release();
      modified_class_name = NULL;

      if (class_name != NULL) { class_name->Release(); class_name = NULL; }
      if (fdo_filter != NULL) { fdo_filter->Release(); fdo_filter = NULL; }

      throw;

    } //  catch ( ... ) ...
}  //  ExecuteLockRequest ()

void FdoRdbmsAcquireLock::FreeMemory ()

// +---------------------------------------------------------------------------
// | The function frees memory.
// +---------------------------------------------------------------------------

{

    if (req_class_name != NULL) {

        req_class_name->Release();
        req_class_name = NULL;

    }  //  if (req_class_name != NULL) ...

}  //  FreeMemory ()

void FdoRdbmsAcquireLock::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lock_type           = FdoLockType_None;
    lock_strategy       = FdoLockStrategy_Partial;
    req_class_name = NULL;

}  //  SetToZero ()

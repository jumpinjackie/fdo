//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

// =============================================================================
// | The file contains the definition of the class FdoRdbmsLongTransactionMan-
// | ager. The class defines the interfaces a provider has to implement to sup-
// | port long transactions.
// =============================================================================


#ifndef FdoRdbmsLongTransactionManager_h
#define FdoRdbmsLongTransactionManager_h

#ifdef _WIN32
#pragma once
#endif

#include "Fdo/Commands/Locking/ILockConflictReader.h"

#include <FdoCommonThreadMutex.h>
#include <Inc/Nls/fdordbms_msg.h>
#include "GdbiException.h"
#include "FdoRdbmsException.h"
#include "GdbiConnection.h"
#include "Disposable.h"
#include "LongTransactionDataRequestType.h"
#include "LongTransactionFreezeOperation.h"
#include "LongTransactionInfo.h"
#include "LongTransactionConflicts.h"
#include "LongTransactionEnvInfo.h"
#include "LongTransactionLTQueryHandler.h"
#include "LongTransactionManagerService.h"
#include "LongTransactionConstants.h"
#include "../LockManager/LockManager.h"
#include "../Fdo/Pvc/FdoRdbmsPvcProcessor.h"


class FdoRdbmsLongTransactionManager : public FdoLtmDisposable
{

    public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  Activate:
    //      The interface allows the activation of a long transaction.

    virtual void Activate (FdoString *lt_name) = 0;

    //  AddLtInTableInfo:
    //      The function adds a entry for the given table name and currently
    //      active long transaction to the system table F_LTID_IN_TABLE. The
    //      table is used to document all tables where the currently active
    //      long transaction is used. It is up to the caller to ensure that
    //      the information stored in the table is up-to-date. If not then
    //      there are consequences for a commit- or rollback operation.
 
    virtual void AddLtInTableInfo (FdoString *data_store_name,
                                   FdoString *table_name       ) = 0;

    //  Commit:
    //      The interface allows the committing of a long transaction.

    virtual void Commit (
                FdoString                        *lt_name,
                FdoILockConflictReader           **lock_conflicts,
                bool                             *lock_conflicts_found,
                FdoRdbmsLongTransactionConflicts **lt_conflicts        )  = 0;

    //  Create:
    //      The interface allows the creation of a long transaction.

    virtual void Create (FdoString *lt_name,
                         FdoString *lt_description) = 0;

    //  Deactivate:
    //      The interface allows the deactivation of the actve long transac-
    //      tion. The result of this operation is the activation of the
    //      root long transaction.

    virtual void Deactivate () = 0;

    //  DropDbCleanUp:
    //      The interface controls the removal of any entries from the table
    //      F_LTID_IN_TABLE that reference the named database.

    virtual void DropDbCleanUp (FdoString *data_store_name) = 0;

    //  FreezeOperation:
    //      The interface allows the freezing or unfreezing of a long trans-
    //      action.

    virtual void FreezeOperation (
                    FdoString                              *lt_name,
                    FdoRdbmsLongTransactionFreezeOperation operation_type) = 0;

    //  GetActive:
    //      The interface allows the retrieval of information for the active
    //      long transaction.

    virtual void GetActive (FdoRdbmsLongTransactionInfo **lt_info) = 0;

    //  GetEnvironmentInfo:
    //      The interface allows the retrieval of the current long transaction
    //      environment.

    virtual void GetEnvironmentInfo (
                            FdoRdbmsLongTransactionEnvInfo **lt_env_info) = 0;

    //  GetPvcProcessor:
    //      The function provides a handle to the Property Value Collection
    //      Processor used to handle conditional data processing.

	virtual const FdoRdbmsPvcProcessor *GetPvcProcessor() = 0;

    //  GetQueryQualification:
    //      The interface allows the retrieval of the query qualification
    //      clause. This clause is a SQL statement qualifying rows to be
    //      addressed in a query with respect to the current long transaction
    //      environment.

    virtual FdoStringP GetQueryQualification (
                                FdoString      *table_name,
                                bool           include_added_rows,
                                bool           include_deleted_rows,
                                FdoCommandType command_type          ) = 0;

    //  IsParentOfActiveLongTransaction:
    //      The interface returns true if the long transaction identified by
    //      either its name or its id represents a parent long transaction of
    //      the currently active long transaction, false otherwise.

    virtual bool IsParentOfActiveLongTransaction (FdoString *lt_name) = 0;
    virtual bool IsParentOfActiveLongTransaction (FdoInt64  lt_id   ) = 0;

    //  IsValidOperation:
    //      The interface allows feature command functions to check whether or
    //      not an identified object can be manipulated in the current long
    //      transaction environment. The interface returns TRUE if the mani-
    //      pulation is valid, FALSE otherwise.

    virtual bool IsValidOperation (
                                FdoString          *table_name,
                                FdoRdbmsPrimaryKey *object_primary_key) = 0;

    //  LTQuery:
    //      The interface allows the querying of long transaction information.

    virtual FdoRdbmsLongTransactionLtQueryHandler *LTQuery (
                    FdoString                              *lt_name,
                    FdoRdbmsLongTransactionDataRequestType request_type) = 0;

    //  MapIdToName:
    //      The interface allows the mapping of a long transaction id to its
    //      associated name.

    virtual void MapIdToName (FdoInt64  lt_id,
                              FdoString **lt_name) = 0;

    //  MapNameToId:
    //      The interface allows the mapping of a long transaction name to its
    //      associated id.

    virtual void MapNameToId (FdoString *lt_name,
                              FdoInt64  *lt_id) = 0;

    //  Rollback:
    //      The interface allows the rolling back of a long transaction.

    virtual void Rollback (FdoString              *lt_name,
                           FdoILockConflictReader **lock_conflicts,
                           bool                   *lock_conflicts_found) = 0;

    //  SetLockManager:
    //      The function sets the Long Transaction Manager's reference to the
    //      Lock Manager. The Lock Manager is required to - for example - gain
    //      exclusive access to long transactions.

    virtual void SetLockManager (FdoRdbmsLockManagerP the_lock_manager) = 0;

    //  SetLongTransactionManagerService:
    //      Sets the reference to the current Long Transaction Manager Service.

    virtual void SetLongTransactionManagerService (
                        FdoRdbmsLongTransactionManagerService *ltm_service) = 0;


    protected:

    // ************************************************************************
    // *                       Protected Member Variables                     *
    // ************************************************************************

    //  active_lt_data_set:
    //      The variable references the data set for the currently active long
    //      transaction.

    FdoRdbmsLongTransactionInfo *active_lt_data_set;

    //  active_lt_data_set_loaded:
    //      The variable indicates whether or not the data set for the active
    //      long transaction is loaded.

    bool active_lt_data_set_loaded;

    //  gdbi_connection:
    //      The current connection object.

    GdbiConnection *gdbi_connection;

    //  lock_manager:
    //      A reference to the Lock Manager required to handle lock related
    //      operations like applying a persistent lock on a long transaction.

    FdoRdbmsLockManagerP lock_manager;

    //  long_transaction_manager_service:
    //      A reference to the Long Transaction Manager Service required to
    //      provide special provider specific services.

    FdoRdbmsLongTransactionManagerService *long_transaction_manager_service;


    private:

    // ************************************************************************
    // *                          Private Interfaces                          *
    // ************************************************************************

    //  CheckLockManagerAvailability:
    //      The Long Transaction Manager requires the presence of a Lock Man-
    //      ager. Although some of the Long Transaction Manager interfaces do
    //      not invoke services provided by the Lock Manager and would still
    //      work if the Lock Manager is not present, other interfaces like
    //      "Commit" and "Rollback" would risk encountering concurrency issues
    //      if the Lock Manager with the services it provides would not be
    //      present. Because of this, this function checks the availability of
    //      a valid reference to a Lock Manager and issues an exception if this
    //      is not the case. Each implementation of this Long Transaction Man-
    //      ager interface must implement such a routine.

    virtual void CheckLockManagerAvailability () = 0;


};  //  class FdoRdbmsLongTransactionManager

#endif

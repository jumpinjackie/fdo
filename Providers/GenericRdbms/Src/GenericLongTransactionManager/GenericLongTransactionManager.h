/******************************************************************************
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 * 
  *
 ******************************************************************************/

// =============================================================================
// | The file contains the definition of the class FdoRdbmsGenericLongTransac-
// | tionManager. The class implements the generic long transaction support.
// =============================================================================


#ifndef FdoRdbmsGenericLongTransactionManager_h
#define FdoRdbmsGenericLongTransactionManager_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include <Sm/Ph/Mgr.h>
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsLockConflictReader.h"
#include "../LongTransactionManager/LongTransactionManager.h"
#include "GenericLongTransactionManagerService.h"
#include "GenericLongTransactionVersManager/proto.h"
#include "GenericLongTransactionVersManager/vers.h"
#include "LongTransactionPVCProcessor.h"
#include "stdio.h"


// ----------------------------------------------------------------------------
// --                          Constant Definitions                          --
// ----------------------------------------------------------------------------

//  GENERICLONGTRANSACTIONMANAGER_COMMIT_OP:
//      Identifies the Long Transaction Manager interface "Commit".

#define GENERICLONGTRANSACTIONMANAGER_COMMIT_OP     1

//  GENERICLONGTRANSACTIONMANAGER_ROLLBACK_OP:
//      Identifies the Long Transaction Manager interface "Rollback".

#define GENERICLONGTRANSACTIONMANAGER_ROLLBACK_OP   2


//  GENERICLONGTRANSACTIONMANAGER_ROOT_LT_ID:
//      Identifies the long transaction id associated with the root long trans-
//      action in a generic long transaction environment.

static const FdoInt64 GENERICLONGTRANSACTIONMANAGER_ROOT_LT_ID = 0;

//  GENERICLONGTRANSACTIONMANAGER_ROOT_LT_NAME:
//      Identifies the name of the root long transaction for a generic
//      long transaction environment.

static const FdoStringP GENERICLONGTRANSACTIONMANAGER_ROOT_LT_NAME = 
                                                                  L"Default_0";


// ----------------------------------------------------------------------------
// --                            Class Definition                            --
// ----------------------------------------------------------------------------

class FdoRdbmsGenericLongTransactionManager : 
                                        public FdoRdbmsLongTransactionManager
{

    private:

    // ************************************************************************
    // *                   Private Helper Class Definitions                   *
    // ************************************************************************

    //  LtInTableDataSet:
    //      The following data set is used to store a record retrieved from the
    //      table F_LTID_IN_TABLE.

    class LtInTableDataSet {

        public:

            wchar_t *data_store_name;
            wchar_t *table_name;
            bool    data_store_inaccessible;

            LtInTableDataSet (FdoString *datastorename,
                              FdoString *tablename);
            virtual ~LtInTableDataSet ();

    };  //  class LtInTableDataSet

    public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  Activate:
    //      The interface allows the activation of a long transaction.

    void Activate (FdoString *lt_name);

    //  AddLtUsedInfo:
    //      The function adds a entry for the given table name and currently
    //      active long transaction to the system table F_LTID_IN_TABLE. The
    //      table is used to document all tables where the currently active
    //      long transaction is used. It is up to the caller to ensure that
    //      the information stored in the table is up-to-date. If not then
    //      there are consequences for a commit- or rollback operation.
 
    void AddLtInTableInfo (FdoString *data_store_name,
                           FdoString *table_name       );

    //  Commit:
    //      The interface allows the committing of a long transaction.

    void Commit (FdoString                        *lt_name,
                 FdoILockConflictReader           **lock_conflicts,
                 bool                             *lock_conflicts_found,
                 FdoRdbmsLongTransactionConflicts **lt_conflicts        );

    //  Create:
    //      The interface allows the creation of a long transaction.

    void Create (FdoString *lt_name,
                 FdoString *lt_description);

    //  Deactivate:
    //      The interface allows the deactivation of the actve long transac-
    //      tion. The result of this operation is the activation of the
    //      root long transaction.

    void Deactivate ();

    //  DropDbCleanUp:
    //      The interface controls the removal of any entries from the table
    //      F_LTID_IN_TABLE that reference the named database.

    void DropDbCleanUp (FdoString *data_store_name);

    //  FreezeOperation:
    //      The interface allows the freezing or unfreezing of a long trans-
    //      action.

    void FreezeOperation (
                    FdoString                              *lt_name,
                    FdoRdbmsLongTransactionFreezeOperation operation_type);

    //  GetActive:
    //      The interface allows the retrieval of information for the active
    //      long transaction.

    void GetActive (FdoRdbmsLongTransactionInfo **lt_info);

    //  GetEnvironmentInfo:
    //      The interface allows the retrieval of the current long transaction
    //      environment.

    void GetEnvironmentInfo (FdoRdbmsLongTransactionEnvInfo **lt_env_info);

    //  GetPvcProcessor:
    //      The function provides a handle to the Property Value Collection
    //      processor used to handle conditional data processing.

    const FdoRdbmsPvcProcessor *GetPvcProcessor () { return pvc_processor; }

    //  GetQueryQualification:
    //      The interface allows the retrieval of the query qualification
    //      clause. This clause is a SQL statement qualifying rows to be
    //      addressed in a query with respect to the current long transaction
    //      environment.

    FdoStringP GetQueryQualification (FdoString      *table_name,
                                      bool           include_added_rows,
                                      bool           include_deleted_rows,
                                      FdoCommandType command_type          );

    //  IsParentOfActiveLongTransaction:
    //      The interface returns true if the long transaction identified by
    //      either its name or its id represents a parent long transaction of
    //      the currently active long transaction, false otherwise.

    bool IsParentOfActiveLongTransaction (FdoString *lt_name);
    bool IsParentOfActiveLongTransaction (FdoInt64  lt_id);

    //  IsValidOperation:
    //      The interface allows feature command functions to check whether or
    //      not an identified object can be manipulated in the current long
    //      transaction environment. The interface returns TRUE if the mani-
    //      pulation is valid, FALSE otherwise.

    bool IsValidOperation (FdoString          *table_name,
                           FdoRdbmsPrimaryKey *object_primary_key);

    //  LTQuery:
    //      The interface allows the querying of long transaction information.
    //      The caller can request data for the named long transaction or the
    //      direct ascendent or descendants of it. The type of information re-
    //      quested by the caller is identified by the parameter "request_type".

    FdoRdbmsLongTransactionLtQueryHandler *LTQuery (
                        FdoString                              *lt_name,
                        FdoRdbmsLongTransactionDataRequestType request_type);

    //  MapIdToName:
    //      The interface allows the mapping of a long transaction id to its
    //      associated name.

    void MapIdToName (FdoInt64  lt_id,
                      FdoString **lt_name);

    //  MapNameToId:
    //      The interface allows the mapping of a long transaction name to its
    //      associated id.

    void MapNameToId (FdoString *lt_name,
                      FdoInt64  *lt_id);

    //  Rollback:
    //      The interface allows the rolling back of a long transaction.

    void Rollback (FdoString              *lt_name,
                   FdoILockConflictReader **lock_conflicts,
                   bool                   *lock_conflicts_found);

    //  SetLockManager:
    //      The function sets the Long Transaction Manager's reference to the
    //      Lock Manager. The Lock Manager is required to - for example - gain
    //      exclusive access to long transactions.

    void SetLockManager (FdoRdbmsLockManagerP the_lock_manager);

    //  SetLongTransactionManagerService:
    //      Sets the reference to the current Long Transaction Manager Service.

    void SetLongTransactionManagerService (
                        FdoRdbmsLongTransactionManagerService *ltm_service);


    //  FdoRdbmsGenericLongTransactionManager:
    //      The Generic Long Transaction Manager constructor.

    FdoRdbmsGenericLongTransactionManager (
                FdoRdbmsConnection                    *a_fdo_rdbms_connection,
                GdbiConnection                        *connection,
                FdoRdbmsLongTransactionManagerService *ltm_service);


    protected:

    // ************************************************************************
    // *                         Protected Interfaces                         *
    // ************************************************************************

    //  FdoRdbmsGenericLongTransactionManager:
    //      A Generic Long Transaction Manager constructor.

    FdoRdbmsGenericLongTransactionManager ();

    //  ~FdoRdbmsGenericLongTransactionManager:
    //      The Generic Long Transaction Manager destructor.

    virtual ~FdoRdbmsGenericLongTransactionManager ();


    private:

    // ************************************************************************
    // *                          Private Interfaces                          *
    // ************************************************************************

    //  BuildLtidSQLFilter:
    //      Based on the provided long transaction information, the function
    //      generates a long transaction id based SQL filter with which objects
    //      in tables can be identified for further processing. Such a filter
    //      is required by the Lock Manager to lock objects affected by either
    //      a commit or rollback operation.

    FdoStringP BuildLtidSQLFilter (
                                FdoInt64                     lt_id,
                                FdoRdbmsLongTransactionInfo  *ancestor_lt,
                                FdoRdbmsLongTransactionInfos *descendant_lts,
                                int                          interface_id);

    //  BuildRollbackDeleteFilter:
    //      The function creates a SQL where-clause to be used when deleting
    //      object versions during a rollback operation.

    FdoStringP BuildRollbackDeleteFilter (
                                FdoInt64                     lt_id,
                                FdoRdbmsLongTransactionInfo  *ancestor_lt,
                                FdoRdbmsLongTransactionInfos *descendant_lts);

    //  BuildRollbackUpdateFilter:
    //      The function creates a SQL where-clause to be used when updating
    //      object versions during a rollback operation.

    FdoStringP BuildRollbackUpdateFilter (
                                FdoInt64                     lt_id,
                                FdoRdbmsLongTransactionInfo  *ancestor_lt,
                                FdoRdbmsLongTransactionInfos *descendant_lts);

    //  CheckDataStoreAvailability:
    //      The function checks whether or not all data stores stored with the
    //      given vector are accessible and issues an exception if at least one
    //      of them is not.

    void CheckDataStoreAvailability (
                            std::vector<LtInTableDataSet*> table_data_vector,
                            FdoString                      *operation_name);

    //  CheckIfLtIsParentOfActiveLongTransaction:
    //      The function executes the request to check whether or not a long
    //      transaction identified by its long transaction id represents a
    //      parent long transaction of the currently active long transaction.
    //      If this is the case the routine returns true, false otherwise.

    bool CheckIfLtIsParentOfActiveLongTransaction (FdoInt64 lt_id);

    //  CheckLockManagerAvailability:
    //      The Long Transaction Manager requires the presence of a Lock Man-
    //      ager. Although some of the Long Transaction Manager interfaces do
    //      not invoke services provided by the Lock Manager and would still
    //      work if the Lock Manager is not present, other interfaces like
    //      "Commit" and "Rollback" would risk encountering concurrency issues
    //      if the Lock Manager with the services it provides would not be
    //      present. Because of this, this function checks the availability of
    //      a valid reference to a Lock Manager and issues an exception if this
    //      is not the case.

    void CheckLockManagerAvailability ();

    //  ClearInvalidSessionEntries:
    //      The function deletes any entry from the table F_ACTIVELT that is
    //      associated with an invalid session.
 
    void ClearInvalidSessionEntries ();

    //  ConstructConvertFunctionString:
    //      The function generates a string that represents the call to the
    //      currently valid convert function for the given column to convert
    //      strings into numbers.

    FdoStringP ConstructConvertFunctionString (FdoString *column_name);

    //  DeleteFromActiveLtTable:
    //      The function removes the entry identified by the given user and
    //      session identifiers from the table F_ACTIVELT.
 
    void DeleteFromActiveLtTable (FdoString *user_name,
                                  FdoInt64  session_id);

    //  DocumentProcess:
    //      The function writes the provided string to the Long Transaction
    //      Manager log file.

    void DocumentProcess (FdoString *msg);

    //  EstablishLtDependency:
    //      The function adds a new long transaction dependency from the cur-
    //      rently active long transaction to the given long transaction.

    void EstablishLtDependency (FdoInt64 lt_id);

    //  GetAbsOfLTID:
    //      The function returns the absolut value of the given long trans-
    //      action id. This function is used instead of the build-in procedure
    //      "abs()" because the build-in function only handles INT types.

    FdoInt64 GetAbsOfLTID (FdoInt64 lt_id);

    //  GetAffectedLtList:
    //      The function determines all long transactions used within the
    //      named data store.

    void GetAffectedLtList (FdoString               *data_store_name,
                            std::vector<FdoString*> *affected_lt_vector);

    //  GetAffectedTableList:
    //      The function determines the affected tables for a commit or roll-
    //      back operation.

    void GetAffectedTableList (
                            FdoInt64                       lt_id,
                            FdoRdbmsLongTransactionInfos   *descendant_lts,
                            std::vector<LtInTableDataSet*> *table_data_vector);

    //  GetDeleteQueryQualification:
    //      The function generates the query qualification clause required for
    //      a delete operation.

    FdoStringP GetDeleteQueryQualification (FdoString *table_name);

    //  GetLtAncestor:
    //      The function determines the ancestor long transaction for the long
    //      transaction identified by the given name.

    FdoRdbmsLongTransactionInfo *GetLtAncestor (FdoString *lt_name);

    //  GetLtDescendants:
    //      The function determines the descendant long transactions for the
    //      long transaction identified by the given name.

    FdoRdbmsLongTransactionInfos *GetLtDescendants (FdoString *lt_name);

    //  GetLtIdString:
    //      The function returns a string with the comma-separated list of
    //      all long transaction ids for all long transactions referenced by
    //      the given parameters.

    wchar_t *GetLtIdString (FdoInt64                     lt_id,
                            FdoRdbmsLongTransactionInfos *descendant_lts,
                            bool                         inc_plus_rep,
                            bool                         inc_minus_rep);

    //  InitActiveRootLtDataSet:
    //      The function initializes the data set holding the information for
    //      the active long transaction with the data for the root long trans-
    //      action.

    void InitActiveRootLtDataSet (bool initial_load);

    //  InsertIntoActiveLtTable:
    //      The function adds an entry to the table F_ACTIVELT using the pro-
    //      vided parameters.
 
    void InsertIntoActiveLtTable (FdoString *user_name,
                                  FdoInt64  session_id,
                                  FdoInt64  lt_id);

    //  IsLeafLt:
    //      The function returns TRUE if the named long transaction represents
    //      a leaf long transaction, FALSE otherwise.

    bool IsLeafLt (FdoInt64 lt_id);

    //  LoadDependencyInformation:
    //      The function controls the process of loading the dependency infor-
    //      mation for the given long transaction into the long transaction
    //      context.

    void LoadDependencyInformation ();

    //  ProcessCommit:
    //      The function processes a commit request.

    void ProcessCommit (FdoInt64                       lt_id,
                        FdoRdbmsLongTransactionInfo    *ancestor_lt,
                        std::vector<LtInTableDataSet*> table_data_vector);

    //  ProcessLtPersistentLockSettings:
    //      The function either applies or releases persistent locks on resp.
    //      from the identified long transactions.

    void ProcessLtPersistentLockSettings (
                            FdoString                    *lt_name,
                            FdoRdbmsLongTransactionInfo  *ancestor_lt,
                            FdoRdbmsLongTransactionInfos *descendant_lts,
                            bool                         is_apply_operation);

    //  ProcessRollback:
    //      The function processes a rollback request.

    void ProcessRollback (FdoInt64                       lt_id,
                          FdoRdbmsLongTransactionInfo    *ancestor_lt,
                          FdoRdbmsLongTransactionInfos   *descendant_lts,
                          std::vector<LtInTableDataSet*> table_data_vector);

    //  QueryDependencyInformation:
    //      The function reads long transaction dependency information from the
    //      corresponding system table and stores the data with the long trans-
    //      action context.

    void QueryDependencyInformation (vers_context lt_context,
                                     FdoStringP   sql_statement,
                                     bool         process_ancestors,
                                     bool         prepare_statement,
                                     int          flags,
                                     FdoInt64     *lt_ids,
                                     int          lt_ids_count,
                                     bool         terminate_query);

    //  RemoveLtDefinitions:
    //      The function removes all long transaction definitions from the long
    //      transaction system table F_LT that are identified by the given long
    //      transaction id string.

    void RemoveLtDefinitions (FdoString *ltid_str);

    //  RemoveLtDependencies:
    //      The function removes all long transaction dependencies from the
    //      long transaction system table F_LTDEPENDENCY that are identified
    //      by the given long transaction id string.

    void RemoveLtDependencies (FdoInt64  parent_ltid,
                               FdoString *ltid_str);

    //  RemoveLtUsedData:
    //      The function removes all data from the long transaction system
    //      table F_LTID_IN_TABLE that are identified by the given query
    //      identifier.

    void RemoveLtUsedData (FdoString *query_identifier,
                           bool      is_data_store_name);

    //  RollbackReferenceCrosscheck:
    //      The function crosschecks a list of descendant long transactions
    //      versus a newly retrieved list of descendant long transactions for
    //      the named long transaction and returns the list of long transac-
    //      tions that are addition in the newly retrieved list back to the
    //      caller for further processing.

    FdoRdbmsLongTransactionInfos *RollbackReferenceCrosscheck (
                                FdoString                    *lt_name,
                                FdoRdbmsLongTransactionInfos *lt_descendants);
 
    //  SetValue:
    //      Function to save a given value buffer.

    wchar_t *SetValue (FdoString *value_buffer);

    //  SortAffectedTableList:
    //      The function sorts the given table list to ensure it lists feature
    //      tables before all other tables.

    void SortAffectedTableList (
                            std::vector<LtInTableDataSet*> *table_data_vector);

    //  UpdateActiveLtTableEntry:
    //      The function updates an entry in the table F_ACTIVELT using the
    //      provided parameters.
 
    void UpdateActiveLtTableEntry (FdoString *user_name,
                                   FdoInt64  session_id,
                                   FdoInt64  lt_id,
                                   bool      *row_updated);

    //  UpdateLtInTableCache:
    //      The function updates the long transaction context cache holding the
    //      list of tables for which an entry in the table F_LTID_IN_TABLE has
    //      already been made for the active long transaction.

    void UpdateLtInTableCache ();


    // ************************************************************************
    // *                        Private Member Variables                      *
    // ************************************************************************
    // * NOTES:                                                               *
    // *    The Long Transaction Manager caches information for the root long *
    // *    transaction as this cannot be modified. The variables that hold   *
    // *    root long transaction information are prefixed with "root_".      *
    // ************************************************************************

    //  fdo_rdbms_connection:
    //      A reference to the FDORDBMS connection object provided when the
    //      Long Transaction Manager is created.

    FdoRdbmsConnection *fdo_rdbms_connection;

    //  long_transaction_context:
    //      References the long transaction context for this Generic Long
    //      Transaction Manager.

    vers_context long_transaction_context;

    //  pvc_processor:
    //      References the Property Value Collection Processor.

    FdoRdbmsLongTransactionPVCProcessor *pvc_processor;

    //  root_lt_creator:
    //      The variable references the creator of the root long transaction.
    //      This information is retrieved once only and referenced by this
    //      variable for further usage.

    FdoString *root_lt_creator;

    //  root_lt_description:
    //      The variable references the description of the root long transac-
    //      tion. This information is retrieved once only and referenced by
    //      this variable for further usage.

    FdoString *root_lt_description;

    //  root_lt_creation_date:
    //      The variable references the time-stamp of the root long transac-
    //      tion. This information is retrieved once only and referenced by
    //      this variable for further usage.

    FdoDateTime root_lt_creation_date;

};  //  class FdoRdbmsGenericLongTransactionManager

#endif


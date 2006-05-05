/******************************************************************************
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
  *
 ******************************************************************************/

// =============================================================================
// | The file contains the definition of the class FdoMySqlLongTransactionMan-
// | agerService. The class implements Long Transaction Manager services for the
// | MySQL environment.
// =============================================================================

#ifndef FdoMySqlLongTransactionManagerService_h
#define FdoMySqlLongTransactionManagerService_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include <Sm/Ph/Mgr.h>
#include <Inc/Nls/fdordbms_msg.h>

#include "FdoRdbmsUtil.h"
#include "FdoRdbmsException.h"
#include "../../GenericLongTransactionManager/GenericLongTransactionManagerService.h"


class FdoMySqlLongTransactionManagerService : 
                            public FdoRdbmsGenericLongTransactionManagerService
{

    public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  BuildActivateLtDeleteStatement:
    //      The interface creates a SQL statement with which an entry can be
    //      removed from the table F_ACTIVELT in a MySQL environment.

    FdoStringP BuildActivateLtDeleteStatement (FdoString *user_name,
                                               FdoInt64  session_id);

    //  BuildActivateLtInsertStatement:
    //      The interface creates a SQL statement with which a new entry can be
    //      added to the table F_ACTIVELT can be updated in a MySQL environment.

    FdoStringP BuildActivateLtInsertStatement (FdoString *user_name,
                                               FdoInt64  session_id,
                                               FdoInt64  lt_id);

    //  BuildActivateLtQueryStatement:
    //      The interface creates a SQL statement with which it is possible to
    //      check whether or not any given long transaction is active in a
    //      MySQL environment.

    FdoStringP BuildActivateLtQueryStatement (FdoString *ltid_string);

    //  BuildActivateLtUpdateStatement:
    //      The interface creates a SQL statement with which an existing entry
    //      in the table F_ACTIVELT can be updated in a MySQL environment.

    FdoStringP BuildActivateLtUpdateStatement (FdoString *user_name,
                                               FdoInt64  session_id,
                                               FdoInt64  lt_id);

    //  BuildActiveLtSessionCleanupStatement:
    //      The interface creates a SQL statement with which data in the table
    //      F_ACTIVELT can be removed that references runaway sessions in a
    //      MySQL environment.

    FdoStringP BuildActiveLtSessionCleanupStatement ();

    //  BuildCommitDeleteStatement:
    //      The interface creates a SQL statement with which data in the named
    //      table can be deleted during a commit operation in a MySQL environ-
    //      ment.

    FdoStringP BuildCommitDeleteStatement (FdoString *data_store_name,
                                           FdoString *table_name,
                                           bool      is_commit_to_root);

    //  BuildCommitUpdateStatement:
    //      The interface creates a SQL statement with which data in the named
    //      table can be updated during a commit operation in a MySQL environ-
    //      ment.

    FdoStringP BuildCommitUpdateStatement (FdoString *data_store_name,
                                           FdoString *table_name,
                                           bool      is_ltid_update_request);

    //  BuildDataStoreAccessibilityQueryStatement:
    //      The interface creates a SQL statement with which it can be verified
    //      whether or not a data store is accessible in a MySQL environment.

    FdoStringP BuildDataStoreAccessibilityQueryStatement ();

    //  BuildGetAffectedLtQueryStatement:
    //      The interface creates a SQL statement returning all long transac-
    //      tions that are used within the named data store in a MySQL envir-
    //      onment.

    FdoStringP BuildGetAffectedLtQueryStatement (FdoString *data_store_name);

    //  BuildLtDeleteStatement:
    //      The interface creates a SQL statement with which entries in the
    //      table F_LT identified by the given parameter can be deleted in a
    //      MySQL environment.

    FdoStringP BuildLtDeleteStatement (FdoString *ltid_string);

    // BuildLtDependencyDeleteStatement:
    //      The interface creates a SQL statement with which entries in the
    //      table F_LTDEPENDENCY as identified by the given parameter can be
    //      deleted in a MySQL environment.

    FdoStringP BuildLtDependencyDeleteStatement (FdoInt64  parent_ltid,
                                                 FdoString *ltid_string);

    //  BuildLtDependencyInsertStatement:
    //      The interface creates a SQL statement with which an entry can be
    //      added to the table F_LTDEPENDENCY in a MySQL environment.

    FdoStringP BuildLtDependencyInsertStatement (FdoInt64 parent_lt_id,
                                                 FdoInt64 child_lt_id);

    //  BuildLtDependencyQueryStatement:
    //      The interface creates a SQL statement with which long transaction
    //      dependencies for the given long transaction can be obtained from
    //      the table F_LTDEPENDENCY in a MySQL environment.

    FdoStringP BuildLtDependencyQueryStatement (bool get_descendents);

    //  BuildLtInTableDeleteStatement:
    //      The interface creates a SQL statement with which entries in the
    //      table F_LTID_IN_TABLE as identified by the given parameter can
    //      be deleted in a MySQL environment.

    FdoStringP BuildLtInTableDeleteStatement (FdoString *query_identifier,
                                              bool      is_data_store_name);

    //  BuildLtInTableInsertStatement:
    //      The interface creates a SQL statement with which an entry can be
    //      added to the table F_LTID_IN_TABLE in a MySQL environment.

    FdoStringP BuildLtInTableInsertStatement (FdoInt64  lt_id,
                                              FdoString *data_store_name,
                                              FdoString *table_name      );

    //  BuildLtInTableQueryStatement:
    //      The interface creates a SQL statement with which data from the
    //      table F_LTID_IN_TABLE can be loaded in a MySQL environment.

    FdoStringP BuildLtInTableQueryStatement (FdoInt64  lt_id);
    FdoStringP BuildLtInTableQueryStatement (FdoString *ltid_list);

    //  BuildLtQueryStatement:
    //      The interface creates a SQL statement with which long transaction
    //      information can be retrieved from the table F_LT in a MySQL
    //      environment.

    FdoStringP BuildLtQueryStatement (
                        FdoString                              *lt_name,
                        FdoRdbmsLongTransactionDataRequestType request_type);

    //  BuildMapIdToNameStatement:
    //      The interface creates a SQL statement with which the name of the
    //      long transaction identified by the given long transaction id can
    //      be retrieved in a MySQL environment.

    FdoStringP BuildMapIdToNameStatement (FdoInt64 lt_id);

    //  BuildMapNameToIdStatement:
    //      The interface creates a SQL statement with which the long trans-
    //      action id of the long transaction identified by the given name
    //      can be retrieved in a MySQL environment.

    FdoStringP BuildMapNameToIdStatement (FdoString *lt_name);

    //  BuildNewLtInsertStatement:
    //      The interface creates a SQL statement with which a new long trans-
    //      action can be inserted into the long transaction table F_LT in a 
    //      MySQL environment.

    FdoStringP BuildNewLtInsertStatement (FdoString *lt_name,
                                          FdoString *lt_description,
                                          FdoString *lt_owner);

    //  BuildRollbackDeleteStatement:
    //      The interface creates a SQL statement with which identified rows
    //      in the named table can be deleted in a MySQL environment.

    FdoStringP BuildRollbackDeleteStatement (FdoString *data_store_name,
                                             FdoString *table_name,
                                             FdoString *where_clause);

    //  BuildRollbackUpdateStatement:
    //      The interface creates a SQL statement with which identified rows
    //      in the named table can be updated in a MySQL environment.

    FdoStringP BuildRollbackUpdateStatement (FdoString *data_store_name,
                                             FdoString *table_name,
                                             FdoString *where_clause);

    //  GetCharToNumberConvertionFunctionName:
    //      The interface provides the name of the routine used to convert a
    //      string to a number in a MySQL environment.

    char *GetCharToNumberConvertionFunctionName ();

    //  GetEmbeddedQuoteEsqSeq:
    //      The function returns the escape sequence for embedded quotes in a
    //      MySQL environment.

    FdoStringP GetEmbeddedQuoteEsqSeq ();

    //  LtDependencyQueryLoopRequired:
    //      Depending on the underlying environment, retrieving dependency
    //      information from the long transaction table F_LtDependency may
    //      require some looping. This function indicates whether or not a
    //      loop is required in a MySQL environment.

    bool LtDependencyQueryLoopRequired ();

    //  FdoMySqlLongTransactionManagerService:
    //      The MySQL Long Transaction Manager Service constructor.

    FdoMySqlLongTransactionManagerService (GdbiConnection *connection);


    protected:

    // ************************************************************************
    // *                         Protected Interfaces                         *
    // ************************************************************************

    //  ~FdoMySqlLongTransactionManagerService:
    //      The MySQL Long Transaction Manager Service destructor.

    virtual ~FdoMySqlLongTransactionManagerService ();


};  //  class FdoMySqlLongTransactionManagerService

#endif

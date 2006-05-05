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
// | The file contains the definition of the class FdoRdbmsLongTransactionMan-
// | agerService. The class defines the interfaces for specific services for the
// | Long Transaction Manager.
// =============================================================================


#ifndef FdoRdbmsLongTransactionManagerService_h
#define FdoRdbmsLongTransactionManagerService_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include "Disposable.h"
#include "LongTransactionDataRequestType.h"
#include "GdbiConnection.h"

class FdoRdbmsLongTransactionManagerService : public FdoLtmDisposable
{

    public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  BuildActivateLtDeleteStatement:
    //      The interface creates a SQL statement with which an entry can be
    //      removed from the table F_ACTIVELT. Because the structure of the
    //      statement depends on the current environment, the procedure is
    //      implemented by the corresponding Long Transaction Manager Service.

    virtual FdoStringP BuildActivateLtDeleteStatement (
                                            FdoString *user_name,
                                            FdoInt64  session_id) = 0;

    //  BuildActivateLtInsertStatement:
    //      The interface creates a SQL statement with which a new entry can be
    //      added to the table F_ACTIVELT. Because the structure of the state-
    //      ment depends on the current environment, the procedure is implemen-
    //      ted by the corresponding Long Transaction Manager Service.

    virtual FdoStringP BuildActivateLtInsertStatement (
                                            FdoString *user_name,
                                            FdoInt64  session_id,
                                            FdoInt64  lt_id      ) = 0;

    //  BuildActivateLtQueryStatement:
    //      The interface creates a SQL statement with which it is possible to
    //      check whether or not any given long transaction is active. Because
    //      the structure of the statement depends on the current environment,
    //      the procedure is implemented by the corresponding Long Transaction
    //      Manager Service.

    virtual FdoStringP BuildActivateLtQueryStatement (
                                            FdoString *ltid_string) = 0;

    //  BuildActivateLtUpdateStatement:
    //      The interface creates a SQL statement with which an existing entry
    //      in the table F_ACTIVELT can be updated. Because the structure of
    //      the statement depends on the current environment, the procedure is
    //      implemented by the corresponding Long Transaction Manager Service.

    virtual FdoStringP BuildActivateLtUpdateStatement (
                                            FdoString *user_name,
                                            FdoInt64  session_id,
                                            FdoInt64  lt_id      ) = 0;

    //  BuildActiveLtSessionCleanupStatement:
    //      The interface creates a SQL statement with which data in the table
    //      F_ACTIVELT can be removed that references runaway sessions. Because
    //      the structure of the statement depends on the current environment,
    //      the procedure is implemented by the corresponding Long Transaction
    //      Manager Service.

    virtual FdoStringP BuildActiveLtSessionCleanupStatement () = 0;

    //  BuildCommitDeleteStatement:
    //      The interface creates a SQL statement with which data in the named
    //      table can be deleted during a commit operation. Because the struc-
    //      ture of the statement depends on the current environment, the pro-
    //      cedure is implemented by the corresponding Long Transaction Manager
    //      Service.

    virtual FdoStringP BuildCommitDeleteStatement (
                                            FdoString *data_store_name,
                                            FdoString *table_name,
                                            bool      is_commit_to_root) = 0;

    //  BuildCommitUpdateStatement:
    //      The interface creates a SQL statement with which data in the named
    //      table can be updated during a commit operation. Because the struc-
    //      ture of the statement depends on the current environment, the pro-
    //      cedure is implemented by the corresponding Long Transaction Manager
    //      Service.

    virtual FdoStringP BuildCommitUpdateStatement (
                                        FdoString *data_store_name,
                                        FdoString *table_name,
                                        bool      is_ltid_update_request) = 0;

    //  BuildDataStoreAccessibilityQueryStatement:
    //      The interface creates a SQL statement with which it can be verified
    //      whether a data store is accessible. Because the structure of the
    //      statement depends on the current environment, the procedure is
    //      implemented by the corresponding Long Transaction Manager Service.

    virtual FdoStringP BuildDataStoreAccessibilityQueryStatement () = 0;

    //  BuildGetAffectedLtQueryStatement:
    //      The interface creates a SQL statement returning all long transac-
    //      tions that are used within the named data store. Because the SQQL
    //      statement depends on the current environment, the procedure is
    //      implemented by the corresponding Long Transaction Manager Service.

    virtual FdoStringP BuildGetAffectedLtQueryStatement (
                                            FdoString *data_store_name) = 0;

    //  BuildLtDeleteStatement:
    //      The interface creates a SQL statement with which entries in the
    //      table F_LT identified by the given parameter can be deleted. Be-
    //      cause the structure of the statement depends on the current en-
    //      vironment, the procedure is implemented by the corresponding Long
    //      Transaction Manager Service.

    virtual FdoStringP BuildLtDeleteStatement (FdoString *ltid_string) = 0;

    // BuildLtDependencyDeleteStatement:
    //      The interface creates a SQL statement with which entries in the
    //      table F_LTDEPENDENCY as identified by the given parameter can be
    //      deleted. Because the structure of the statement depends on the
    //      current environment, the procedure is implemented by the corres-
    //      ponding Long Transaction Manager Service.

    virtual FdoStringP BuildLtDependencyDeleteStatement (
                                                FdoInt64  parent_ltid,
                                                FdoString *ltid_string ) = 0;

    //  BuildLtDependencyInsertStatement:
    //      The interface creates a SQL statement with which an entry can be
    //      added to the table F_LTDEPENDENCY. Because the structure of the
    //      statement depends on the current environment, the procedure is
    //      implemented by the corresponding Long Transaction Manager Service.

    virtual FdoStringP BuildLtDependencyInsertStatement (
                                            FdoInt64 parent_lt_id,
                                            FdoInt64 child_lt_id) = 0;

    //  BuildLtDependencyQueryStatement:
    //      The interface creates a SQL statement with which long transaction
    //      dependencies for the given long transaction can be obtained from
    //      the table F_LTDEPENDENCY. Because the structure of the statement
    //      depends on the current environment, the procedure is implemented
    //      by the corresponding Long Transaction Manager Service.

    virtual FdoStringP BuildLtDependencyQueryStatement (
                                                    bool get_descendents) = 0;

    //  BuildLtInTableDeleteStatement:
    //      The interface creates a SQL statement with which entries in the
    //      table F_LTID_IN_TABLE as identified by the given parameter can
    //      be deleted. Because the structure of the statement depends on the
    //      current environment, the procedure is implemented by the corres-
    //      ponding Long Transaction Manager Service.

    virtual FdoStringP BuildLtInTableDeleteStatement (
                                            FdoString *query_identifier,
                                            bool      is_data_store_name) = 0;

    //  BuildLtInTableInsertStatement:
    //      The interface creates a SQL statement with which an entry can be
    //      added to the table F_LTID_IN_TABLE. Because the structure of the
    //      statement depends on the current environment, the procedure is
    //      implemented by the corresponding Long Transaction Manager Service.

    virtual FdoStringP BuildLtInTableInsertStatement (
                                            FdoInt64  lt_id,
                                            FdoString *data_store_name,
                                            FdoString *table_name      ) = 0;

    //  BuildLtInTableQueryStatement:
    //      The interface creates a SQL statement with which data from the
    //      table F_LTID_IN_TABLE can be loaded. Because the structure of the
    //      statement depends on the current environment, the procedure is
    //      implemented by the corresponding Long Transaction Manager Service.

    virtual FdoStringP BuildLtInTableQueryStatement (FdoInt64  lt_id)      = 0;
    virtual FdoStringP BuildLtInTableQueryStatement (FdoString *ltid_list) = 0;

    //  BuildLtQueryStatement:
    //      The interface creates a SQL statement with which long transaction
    //      information can be retrieved. Because the structure of the state-
    //      ment depends on the current environment, the procedure is implemen-
    //      ted by the corresponding Long Transaction Manager Service.

    virtual FdoStringP BuildLtQueryStatement (
                    FdoString                              *lt_name,
                    FdoRdbmsLongTransactionDataRequestType request_type) = 0;

    //  BuildMapIdToNameStatement:
    //      The interface creates a SQL statement with which the name of the
    //      long transaction identified by the given long transaction id can
    //      be retrieved. Because the structure of the statement depends on
    //      the current environment, the procedure is implemented by the cor-
    //      responding Long Transaction Manager Service.

    virtual FdoStringP BuildMapIdToNameStatement (FdoInt64 lt_id) = 0;

    //  BuildMapNameToIdStatement:
    //      The interface creates a SQL statement with which the long trans-
    //      action id of the long transaction identified by the given name
    //      can be retrieved. Because the structure of the statement depends
    //      on the current environment, the procedure is implemented by the
    //      corresponding Long Transaction Manager Service.

    virtual FdoStringP BuildMapNameToIdStatement (FdoString *lt_name) = 0;

    //  BuildNewLtInsertStatement:
    //      The interface creates a SQL statement with which a new long trans-
    //      action can be inserted into the long transaction table F_LT. Be-
    //      cause this statement depends on the current environment, the pro-
    //      cedure is implemented by the corresponding Long Transaction Manager
    //      Service.

    virtual FdoStringP BuildNewLtInsertStatement (
                                            FdoString *lt_name,
                                            FdoString *lt_description,
                                            FdoString *lt_owner) = 0;

    //  BuildRollbackDeleteStatement:
    //      The interface creates a SQL statement with which identified rows
    //      in the named table can be deleted. Because this statement depends
    //      on the current environment, the procedure is implemented by the
    //      corresponding Long Transaction Manager Service.

    virtual FdoStringP BuildRollbackDeleteStatement (
                                            FdoString *data_store_name,
                                            FdoString *table_name,
                                            FdoString *where_clause     ) = 0;

    //  BuildRollbackUpdateStatement:
    //      The interface creates a SQL statement with which identified rows
    //      in the named table can be updated. Because this statement depends
    //      on the current environment, the procedure is implemented by the
    //      corresponding Long Transaction Manager Service.

    virtual FdoStringP BuildRollbackUpdateStatement (
                                            FdoString *data_store_name,
                                            FdoString *table_name,
                                            FdoString *where_clause     ) = 0;

    //  GetCharToNumberConvertionFunctionName:
    //      The interface provides the name of the routine used to convert a
    //      string to a number. Because the function name depends on the cur-
    //      rent environment, the procedure is implemented by the correspond-
    //      ing Long Transaction Manager Service.

    virtual char *GetCharToNumberConvertionFunctionName () = 0;

    //  GetEmbeddedQuoteEsqSeq:
    //      The function returns the escape sequence for embedded quotes. Be-
    //      cause the value depends on the current environment, the procedure
    //      is implemented by the corresponding Long Transaction Manager Ser-
    //      vice.

    virtual FdoStringP GetEmbeddedQuoteEsqSeq () = 0;

    //  LtDependencyQueryLoopRequired:
    //      Depending on the underlying environment, retrieving dependency
    //      information from the long transaction table F_LtDependency may
    //      require some looping. This function indicates whether or not a
    //      loop is required.

    virtual bool LtDependencyQueryLoopRequired () = 0;


    protected:

    // ************************************************************************
    // *                      Protected Member Variables                      *
    // ************************************************************************

    //  gdbi_connection:
    //      The current connection object.

    GdbiConnection *gdbi_connection;


};  //  class FdoRdbmsLongTransactionManagerService

#endif


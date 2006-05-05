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
// | The file contains the implementation of the class FdoMySqlLongTransaction-
// | ManagerService.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "GdbiCommands.h"
#include "MySqlLongTransactionManagerService.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoMySqlLongTransactionManagerService::
                        FdoMySqlLongTransactionManagerService (
                                                    GdbiConnection *connection)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    gdbi_connection = connection;

}  //  FdoMySqlLongTransactionManagerService ()

FdoMySqlLongTransactionManagerService::
                                    ~FdoMySqlLongTransactionManagerService ()

// +---------------------------------------------------------------------------
// | The function represents a class destructor.
// +---------------------------------------------------------------------------

{
}  //  ~FdoMySqlLongTransactionManagerService ()


// ----------------------------------------------------------------------------
// --                            Public Interfaces                           --
// ----------------------------------------------------------------------------

FdoStringP FdoMySqlLongTransactionManagerService::
                                BuildActivateLtDeleteStatement (
                                                        FdoString *user_name,
                                                        FdoInt64  session_id)

// +---------------------------------------------------------------------------
// | The interface creates a SQL statement with which an entry can be removed
// | from the table F_ACTIVELT in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement to delete any entry from the long transaction
    // system table F_ACTIVELT identified by the given name. The SQL statement
    // is structured as shown next:
    //
    //    delete from FDOSYS.LT_ACTIVE_LT_TABLE
    //      where LT_ACTIVE_LT_USERNAME  = 'user_name' and
    //            LT_ACTIVE_LT_SESSIONID = session_id;

#ifdef _WIN32

    sql_statement = 
        FdoStringP::Format(
                L"delete from FDOSYS.%ls where %ls = '%ls' and %ls = %I64d",
                (FdoString *) LT_ACTIVE_LT_TABLE,
                (FdoString *) LT_ACTIVE_LT_USERNAME,
                user_name,
                (FdoString *) LT_ACTIVE_LT_SESSIONID,
                session_id);

#else

    sql_statement = 
        FdoStringP::Format(
                L"delete from FDOSYS.%ls where %ls = '%ls' and %ls = %lld",
                (FdoString *) LT_ACTIVE_LT_TABLE,
                (FdoString *) LT_ACTIVE_LT_USERNAME,
                user_name,
                (FdoString *) LT_ACTIVE_LT_SESSIONID,
                session_id);

#endif

    return sql_statement;

}  //  BuildActivateLtDeleteStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                                BuildActivateLtInsertStatement (
                                                        FdoString *user_name,
                                                        FdoInt64  session_id,
                                                        FdoInt64  lt_id)

// +---------------------------------------------------------------------------
// | The interface creates a SQL statement with which a new entry can be added
// | to the table F_ACTIVELT can be updated in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement to insert an entry into the long transaction
    // system table F_ACTIVELT using the given parameters. The SQL statement is
    // structured as shown next:
    //
    //    insert into FDOSYS.LT_ACTIVE_LT_TABLE 
    //             (LT_ACTIVE_LT_USERNAME,
    //              LT_ACTIVE_LT_SESSIONID,
    //              LT_ACTIVE_LT_ACTIVELTID,
    //              LT_ACTIVE_LT_ACTIVATEDATE)
    //      values ('user_name',
    //              session_id,
    //              lt_id,
    //              current_date());

#ifdef _WIN32

    sql_statement = 
        FdoStringP::Format(
            L"insert into FDOSYS.%ls (%ls, %ls, %ls, %ls) "
            L"                            values ('%ls', %I64d, %I64d, %ls)",
            (FdoString *) LT_ACTIVE_LT_TABLE,
            (FdoString *) LT_ACTIVE_LT_USERNAME,
            (FdoString *) LT_ACTIVE_LT_SESSIONID,
            (FdoString *) LT_ACTIVE_LT_ACTIVELTID,
            (FdoString *) LT_ACTIVE_LT_ACTIVATEDATE,
            user_name,
            session_id,
            lt_id,
            L"current_date()");

#else

    sql_statement = 
        FdoStringP::Format(
            L"insert into FDOSYS.%ls (%ls, %ls, %ls, %ls) "
            L"                           values ('%ls', %lld, %lld, %ls)",
            (FdoString *) LT_ACTIVE_LT_TABLE,
            (FdoString *) LT_ACTIVE_LT_USERNAME,
            (FdoString *) LT_ACTIVE_LT_SESSIONID,
            (FdoString *) LT_ACTIVE_LT_ACTIVELTID,
            (FdoString *) LT_ACTIVE_LT_ACTIVATEDATE,
            user_name,
            session_id,
            lt_id,
            L"current_date()");

#endif

    return sql_statement;

}  //  BuildActivateLtInsertStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                        BuildActivateLtQueryStatement (FdoString *ltid_string)

// +---------------------------------------------------------------------------
// | The interface creates a SQL statement with which it is possible to check
// | whether or not any given long transaction is active in a MySQL environ-
// | ment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement to check whether or not long transactions
    // are active. The SQL statement is structured as shown next:
    //
    //    select LT_ACTIVE_LT_ACTIVELTID
    //      from  FDOSYS.LT_ACTIVE_LT_TABLE
    //      where LT_ACTIVE_LT_ACTIVELTID in (ltid_string);

    sql_statement = 
        FdoStringP::Format(L"select %ls from FDOSYS.%ls where %ls in (%ls)",
                           (FdoString *) LT_ACTIVE_LT_ACTIVELTID,
                           (FdoString *) LT_ACTIVE_LT_TABLE,
                           (FdoString *) LT_ACTIVE_LT_ACTIVELTID,
                           ltid_string);

    return sql_statement;

}  //  BuildActivateLtQueryStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                                BuildActivateLtUpdateStatement (
                                                        FdoString *user_name,
                                                        FdoInt64  session_id,
                                                        FdoInt64  lt_id)

// +---------------------------------------------------------------------------
// | The interface creates a SQL statement with which an existing entry in the
// | table F_ACTIVELT can be updated in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement to delete any entry from the long transaction
    // system table F_ACTIVELT identified by the given name. The SQL statement
    // is structured as shown next:
    //
    //    update  FDOSYS.LT_ACTIVE_LT_TABLE
    //      set   LT_ACTIVE_LT_ACTIVELTID   = lt_id,
    //            LT_ACTIVE_LT_ACTIVATEDATE = current_date()
    //      where LT_ACTIVE_LT_USERNAME     = 'user_name' and
    //            LT_ACTIVE_LT_SESSIONID    = session_id;

#ifdef _WIN32

    sql_statement = 
        FdoStringP::Format(L"update FDOSYS.%ls "
                           L"    set   %ls = %I64d, %ls = %ls "
                           L"    where %ls = '%ls' and %ls = %I64d",
                           (FdoString *) LT_ACTIVE_LT_TABLE,
                           (FdoString *) LT_ACTIVE_LT_ACTIVELTID,
                            lt_id,
                           (FdoString *) LT_ACTIVE_LT_ACTIVATEDATE,
                           L"current_date()",
                           (FdoString *) LT_ACTIVE_LT_USERNAME,
                           user_name,
                           (FdoString *) LT_ACTIVE_LT_SESSIONID,
                           session_id);

#else

    sql_statement = 
        FdoStringP::Format(L"update FDOSYS.%ls "
                           L"    set   %ls = %lld, %ls = %ls "
                           L"    where %ls = '%ls' and %ls = %lld",
                           (FdoString *) LT_ACTIVE_LT_TABLE,
                           (FdoString *) LT_ACTIVE_LT_ACTIVELTID,
                           lt_id,
                           (FdoString *) LT_ACTIVE_LT_ACTIVATEDATE,
                           L"current_date()",
                           (FdoString *) LT_ACTIVE_LT_USERNAME,
                           user_name,
                           (FdoString *) LT_ACTIVE_LT_SESSIONID,
                           session_id);

#endif

    return sql_statement;

}  //  BuildActivateLtUpdateStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                                    BuildActiveLtSessionCleanupStatement ()

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which data in the system table
// | F_ACTIVELT can be removed that references runaway sessions in a MySQL
// | environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // NOTE: Needs to be implemented when known where to cross check session
    //       ids to determine whether or not such an id is still valid. This
    //       is not a priority as there is no long transaction support in a
    //       MySQL environment at this point.
    // ----------------------------------------------------------------------
    // TODO: Identify how to determine whether or not a session is still
    //       valid. This is different depending on the underlying database.
    // 
    // I know how to do it. However, it cannot be done in a single query and
    // that's a problem. MySQL must be keeping an inventory for all sessions
    // in some internal structures rather than in a table like Oracle.
    // MySQL provides function mysql_list_processes() for retrieving all
    // sessions and that function can be used for finding active sessions.
    // If the session from f_dbopen/f_active_lt is not active, the row can
    // be removed. So, there is a little bit more work with this than in
    // Oracle.
    // ----------------------------------------------------------------------

    return sql_statement;

}  //  BuildActiveLtSessionCleanupStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                    BuildCommitDeleteStatement (FdoString *data_store_name,
                                                FdoString *table_name,
                                                bool      is_commit_to_root)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which data in the named table
// | can be deleted during a commit operation in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Create the generic SQL statement. The generated statement's structure
    // depends on the value of the parameter "is_commit_to_root" and can have
    // any of the following two structures:
    //
    //  is_commit_to_root == true:
    //
    //      delete
    //        from  <data_store_name>.<table_name>
    //        where cast(NEXTLTID as signed) in (?, ?) or LTID = ?;
    //
    //  is_commit_to_root == false:
    //
    //      delete
    //        from <data_store_name>.<table_name>
    //        where LTID = ? and cast(NEXTLTID as signed) = ?;

    if (is_commit_to_root)
        sql_statement =
            FdoStringP::Format(
                    L"delete from %ls.%ls where %ls in (?, ?) or LTID = ?",
                    data_store_name,
                    table_name,
                    L"cast(NEXTLTID as signed)");
    else
      sql_statement =
            FdoStringP::Format(
                            L"delete from %ls.%ls where LTID = ? and %ls = ?",
                            data_store_name,
                            table_name,
                            L"cast(NEXTLTID as signed)");

    return sql_statement;

}  //  BuildCommitDeleteStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                BuildCommitUpdateStatement (FdoString *data_store_name,
                                            FdoString *table_name,
                                            bool      is_ltid_update_request)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which data in the named table
// | can be updated during a commit operation in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Create the generic SQL statement. The generated statement's structure
    // depends on the value of the parameter "is_ltid_update_request" and can
    // have any of the following two structures:
    //
    //  is_ltid_update_request == true:
    //
    //      update <data_store_name>.<table_name> set LTID = ? where LTID = ?;
    //
    //  is_ltid_update_request == false:
    //
    //      update  <data_store_name>.<table_name>
    //        set   NEXTLTID = ?
    //        where cast(NEXTLTID as signed) = ?;

    if (is_ltid_update_request)
        sql_statement =
                FdoStringP::Format(
                                L"update %ls.%ls set LTID = ? where LTID = ?",
                                data_store_name,
                                table_name);
    else
      sql_statement =
            FdoStringP::Format(
                        L"update %ls.%ls set NEXTLTID = ? where %ls = ?",
                        data_store_name,
                        table_name,
                        L"cast(NEXTLTID as signed)");

    return sql_statement;

}  //  BuildCommitUpdateStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                                BuildDataStoreAccessibilityQueryStatement ()

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which it can be verified whether
// | or not a data store is accessible in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement used to verify whether or not a data store
    // is accessible in the current environment. The SQL statement is struc-
    // tured as shown next:
    //
    //   select  TableOwner
    //     from  F_CLASSDEFINITION
    //     where TableOwner = '?';

    sql_statement = 
        FdoStringP::Format(
                        L"select %ls from F_CLASSDEFINITION where %ls = '?'",
                        L"TableOwner",
                        L"TableOwner");

    return sql_statement;

}  //  BuildDataStoreAccessibilityQueryStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                BuildGetAffectedLtQueryStatement (FdoString *data_store_name)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement returning all long transactions that
// | are used within the named data store in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Create the SQL statement with which the requested data can be retrieved.
    // The SQL statement is structured as shown next:
    //
    //   select  LT_CORE_LTNAME
    //     from  FDOSYS.LT_CORE_TABLE
    //     where LT_CORE_LTID in (
    //                  select  LT_USED_LTID
    //                    from  FDOSYS.LT_USED_TABLE
    //                    where LT_USED_DATASTORENAME = <data_store_name>);

    sql_statement =
                FdoStringP::Format(
                    L"select %ls from FDOSYS.%ls where %ls in ("
                    L"select %ls from FDOSYS.%ls where %ls = upper('%ls'))",
                    (FdoString *) LT_CORE_LTNAME,
                    (FdoString *) LT_CORE_TABLE,
                    (FdoString *) LT_CORE_LTID,
                    (FdoString *) LT_USED_LTID,
                    (FdoString *) LT_USED_TABLE,
                    (FdoString *) LT_USED_DATASTORENAME,
                    data_store_name);

    return sql_statement;

}  //  BuildGetAffectedLtQueryStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                            BuildLtDeleteStatement (FdoString *ltid_string)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which entries in the table F_LT
// | identified by the given parameter can be deleted in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement with which the identified rows can be deleted
    // from the table F_LT. The SQL statement is structured as shown next:
    //
    //    delete from FDOSYS.LT_CORE_TABLE
    //      where LT_CORE_LTID in (<ltid_string>);

    sql_statement =
            FdoStringP::Format(L"delete from FDOSYS.%ls where %ls in (%ls)",
                               (FdoString *) LT_CORE_TABLE,
                               (FdoString *) LT_CORE_LTID,
                               ltid_string);

    return sql_statement;

}  //  BuildLtDeleteStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                    BuildLtDependencyDeleteStatement (FdoInt64  parent_ltid,
                                                      FdoString *ltid_string)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which entries in the table
// | F_LTDEPENDENCY as identified by the given parameter can be deleted in
// | a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement with which to delete the identified entries
    // from the table F_LTDEPENDENCY. The SQL statement is structured as shown
    // next:
    //
    //    delete from FDOSYS.LT_DEPENDENCY_TABLE
    //      where (LT_DEPENDENCY_PARENTLTID in (<ltid_string>)     ) or
    //            (LT_DEPENDENCY_PARENTLTID = parent_ltid      and
    //            (LT_DEPENDENCY_CHILDLTID  in (<ltid_string>)     );

#ifdef _WIN32

    sql_statement = 
        FdoStringP::Format(
            L"delete from  FDOSYS.%ls "
            L"       where (%ls in (%ls)) or (%ls = %I64d and %ls in (%ls))",
            (FdoString *) LT_DEPENDENCY_TABLE,
            (FdoString *) LT_DEPENDENCY_PARENTLTID,
            ltid_string,
            (FdoString *) LT_DEPENDENCY_PARENTLTID,
            parent_ltid,
            (FdoString *) LT_DEPENDENCY_CHILDLTID,
            ltid_string);

#else

    sql_statement = 
        FdoStringP::Format(
            L"delete from  FDOSYS.%ls "
            L"       where (%ls in (%ls)) or (%ls = %lld and %ls in (%ls))",
            (FdoString *) LT_DEPENDENCY_TABLE,
            (FdoString *) LT_DEPENDENCY_PARENTLTID,
            ltid_string,
            (FdoString *) LT_DEPENDENCY_PARENTLTID,
            parent_ltid,
            (FdoString *) LT_DEPENDENCY_CHILDLTID,
            ltid_string);

#endif

    return sql_statement;

}  //  BuildLtDependencyDeleteStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                    BuildLtDependencyInsertStatement (FdoInt64 parent_lt_id,
                                                      FdoInt64 child_lt_id)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which an entry can be added to
// | the table F_LTDEPENDENCY in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement to insert an entry into the long transaction
    // system table F_LTDEPENDENCY using the given parameters. The SQL state-
    // ment is structured as shown next:
    //
    //    insert into FDOSYS.LT_DEPENDENCY_TABLE
    //             (LT_DEPENDENCY_PARENTLTID, LT_DEPENDENCY_CHILDLTID)
    //      values (<active_lt_id>,           lt_id);

#ifdef _WIN32

    sql_statement = 
        FdoStringP::Format(
            L"insert into FDOSYS.%ls (%ls, %ls) values (%I64d, %I64d)",
            (FdoString *) LT_DEPENDENCY_TABLE,
            (FdoString *) LT_DEPENDENCY_PARENTLTID,
            (FdoString *) LT_DEPENDENCY_CHILDLTID,
            parent_lt_id,
            child_lt_id);

#else

    sql_statement = 
        FdoStringP::Format(
            L"insert into FDOSYS.%ls (%ls, %ls) values (%lld, %lld)",
            (FdoString *) LT_DEPENDENCY_TABLE,
            (FdoString *) LT_DEPENDENCY_PARENTLTID,
            (FdoString *) LT_DEPENDENCY_CHILDLTID,
            parent_lt_id,
            child_lt_id);

#endif

    return sql_statement;

}  //  BuildLtDependencyInsertStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                        BuildLtDependencyQueryStatement (bool get_descendents)

// +---------------------------------------------------------------------------
// | The interface creates a SQL statement with which long transaction depen-
// | dencies for the given long transaction can be obtained from the table
// | F_LTDEPENDENCY in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement to query long transaction dependencies from
    // the table F_LTDEPENDENCY for the identified long transaction. The struc-
    // ture of the SQL statement depends on whether or not the ancestors or
    // descendents are to be retrieved:
    //
    //  Request for the ancestors:
    //
    //      select  LT_DEPENDENCY_CHILDLTID, LT_DEPENDENCY_PARENTLTID
    //        from  LT_DEPENDENCY_TABLE
    //        where LT_DEPENDENCY_CHILDLTID in (?, ?, ?, ?, ?, ?, ?, ?, ?, ?,
    //                                          ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,
    //                                          ?, ?, ?, ?, ?)
    //
    //  Request for the descendents:
    //
    //      select  LT_DEPENDENCY_CHILDLTID, LT_DEPENDENCY_PARENTLTID
    //        from  LT_DEPENDENCY_TABLE
    //        where LT_DEPENDENCY_PARENTLTID in (?, ?, ?, ?, ?, ?, ?, ?, ?, ?,
    //                                           ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,
    //                                           ?, ?, ?, ?, ?)
    //

    sql_statement = 
        FdoStringP::Format(
            L"select %ls, %ls from FDOSYS.%ls "
            L"    where %ls in (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
            L"                  ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
            L"                  ?, ?, ?, ?, ?)",
            (FdoString *) LT_DEPENDENCY_CHILDLTID,
            (FdoString *) LT_DEPENDENCY_PARENTLTID,
            (FdoString *) LT_DEPENDENCY_TABLE,
            ((get_descendents) ? (FdoString *) LT_DEPENDENCY_PARENTLTID
                               : (FdoString *) LT_DEPENDENCY_CHILDLTID));

    return sql_statement;

}  //  BuildLtDependencyQueryStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                BuildLtInTableDeleteStatement (FdoString *query_identifier,
                                               bool      is_data_store_name)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which entries in the table
// | F_LTID_IN_TABLE as identified by the given parameter can be deleted in
// | a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement with which the identified entries can be re-
    // moved from the table F_LTID_IN_TABLE. The SQL statement is structured as
    // shown next:
    //
    //  If data_store_name is false:
    //
    //    delete from FDOSYS.LT_USED_TABLE
    //      where LT_USED_LTID in (<query_identifier>);
    //
    //  If data_store_name is true:
    //
    //    delete from FDOSYS.LT_USED_TABLE
    //      where LT_USED_DATASTORENAME = '<query_identifier>';

    sql_statement = (is_data_store_name)
        ? FdoStringP::Format(L"delete from FDOSYS.%ls where %ls = '%ls'",
                             (FdoString *) LT_USED_TABLE,
                             (FdoString *) LT_USED_DATASTORENAME,
                             query_identifier)
        : FdoStringP::Format(L"delete from FDOSYS.%ls where %ls in (%ls)",
                             (FdoString *) LT_USED_TABLE,
                             (FdoString *) LT_USED_LTID,
                             query_identifier);

    return sql_statement;

}  //  BuildLtInTableDeleteStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                         BuildLtInTableInsertStatement (
                                                    FdoInt64  lt_id,
                                                    FdoString *data_store_name,
                                                    FdoString *table_name)

// +---------------------------------------------------------------------------
// | The interface creates a SQL statement with which an entry can be added to
// | the table F_LTID_IN_TABLE in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement to insert an entry into the long transaction
    // system table F_LTID_IN_TABLE using the given parameters. The SQL state-
    // ment is structured as shown next:
    //
    //    insert into FDOSYS.LT_USED_TABLE
    //             (LT_USED_LTID, LT_USED_DATASTORENAME,   LT_USED_TABLENAME)
    //      values (lt_id,        upper('datastore_name'), upper('table_name'));

#ifdef _WIN32

    sql_statement = 
        FdoStringP::Format(
            L"insert into FDOSYS.%ls (%ls, %ls, %ls) "
            L"                  values (%I64d, upper('%ls'), upper('%ls'))",
            (FdoString *) LT_USED_TABLE,
            (FdoString *) LT_USED_LTID,
            (FdoString *) LT_USED_DATASTORENAME,
            (FdoString *) LT_USED_TABLENAME,
            lt_id,
            data_store_name,
            table_name);

#else

    sql_statement = 
        FdoStringP::Format(
            L"insert into FDOSYS.%ls (%ls, %ls, %ls) "
            L"                  values (%lld, upper('%ls'), upper('%ls'))",
            (FdoString *) LT_USED_TABLE,
            (FdoString *) LT_USED_LTID,
            (FdoString *) LT_USED_DATASTORENAME,
            (FdoString *) LT_USED_TABLENAME,
            lt_id,
            data_store_name,
            table_name);

#endif

    return sql_statement;

}  //  BuildLtInTableInsertStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                                BuildLtInTableQueryStatement (FdoInt64 lt_id)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which data from the table
// | F_LTID_IN_TABLE can be loaded in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement to select data from the long transaction
    // system table F_LTID_IN_TABLE using the given parameters. The SQL state-
    // ment is structured as shown next:
    //
    //    select LT_USED_DATASTORENAME, LT_USED_TABLENAME
    //      from FDOSYS.LT_USED_TABLE
    //      where LT_USED_LTID = lt_id;

#ifdef _WIN32

    sql_statement = 
        FdoStringP::Format(
                        L"select %ls, %ls from FDOSYS.%ls where %ls = %I64d",
                        (FdoString *) LT_USED_DATASTORENAME,
                        (FdoString *) LT_USED_TABLENAME,
                        (FdoString *) LT_USED_TABLE,
                        (FdoString *) LT_USED_LTID,
                        lt_id);

#else

    sql_statement = 
        FdoStringP::Format(
                        L"select %ls, %ls from FDOSYS.%ls where %ls = %lld",
                        (FdoString *) LT_USED_DATASTORENAME,
                        (FdoString *) LT_USED_TABLENAME,
                        (FdoString *) LT_USED_TABLE,
                        (FdoString *) LT_USED_LTID,
                        lt_id);

#endif


    return sql_statement;

}  //  BuildLtInTableQueryStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                            BuildLtInTableQueryStatement (FdoString *ltid_list)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which data from the table
// | F_LTID_IN_TABLE can be loaded in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement to select data from the long transaction
    // system table F_LTID_IN_TABLE using the given parameters. The SQL state-
    // ment is structured as shown next:
    //
    //    select LT_USED_DATASTORENAME, LT_USED_TABLENAME
    //      from FDOSYS.LT_USED_TABLE
    //      where LT_USED_LTID in (ltid_list);

    sql_statement = 
        FdoStringP::Format(
                        L"select %ls, %ls from FDOSYS.%ls where %ls in (%ls)",
                        (FdoString *) LT_USED_DATASTORENAME,
                        (FdoString *) LT_USED_TABLENAME,
                        (FdoString *) LT_USED_TABLE,
                        (FdoString *) LT_USED_LTID,
                        ltid_list);

    return sql_statement;

}  //  BuildLtInTableQueryStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                  BuildLtQueryStatement (
                        FdoString                              *lt_name,
                        FdoRdbmsLongTransactionDataRequestType request_type)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which long transaction informa-
// | tion can be retrieved from the table F_LT in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // The SQL statement depends on the provided request type.

    switch (request_type) {

      case FdoRdbmsLongTransactionDataRequestType_Data:

        // The user requests long transaction information for either a given
        // long transaction or all available long transactions. The generated
        // SQL statement depends on whether or not a long transaction name is
        // supplied. The possible queries are:
        //
        //  No long transaction name is supplied:
        //
        //      select  LT_CORE_LTNAME      LT_INFO_LTNAME,
        //              LT_CORE_DESCRIPTION LT_INFO_DESCRIPTION,
        //              LT_CORE_USERNAME    LT_INFO_USERNAME,
        //              LT_CORE_CREATEDATE  LT_INFO_CREATEDATE,
        //              LT_CORE_LOCKTYPE    LT_INFO_FREEZE_MODE,
        //              LT_CORE_LTID        LT_INFO_LTID,
        //              LT_CORE_LOCKID      LT_INFO_LOCK_ID
        //        from  FDOSYS.LT_CORE_TABLE;
        //
        //  Long transaction name is supplied:
        //
        //      select  LT_CORE_LTNAME      LT_INFO_LTNAME,
        //              LT_CORE_DESCRIPTION LT_INFO_DESCRIPTION,
        //              LT_CORE_USERNAME    LT_INFO_USERNAME,
        //              LT_CORE_CREATEDATE  LT_INFO_CREATEDATE,
        //              LT_CORE_LOCKTYPE    LT_INFO_FREEZE_MODE,
        //              LT_CORE_LTID        LT_INFO_LTID,
        //              LT_CORE_LOCKID      LT_INFO_LOCK_ID
        //        from  FDOSYS.LT_CORE_TABLE
        //        where LT_CORE_LTNAME = '<lt_name>';

        sql_statement = (lt_name == NULL)
                  ? FdoStringP::Format(
                            L"select %ls %ls, %ls %ls, %ls %ls, %ls %ls, "
                            L"       %ls %ls, %ls %ls, %ls %ls "
                            L"  from FDOSYS.%ls",
                            (FdoString *) LT_CORE_LTNAME,
                            (FdoString *) LT_INFO_LTNAME,
                            (FdoString *) LT_CORE_DESCRIPTION,
                            (FdoString *) LT_INFO_DESCRIPTION,
                            (FdoString *) LT_CORE_USERNAME,
                            (FdoString *) LT_INFO_USERNAME,
                            (FdoString *) LT_CORE_CREATEDATE,
                            (FdoString *) LT_INFO_CREATEDATE,
                            (FdoString *) LT_CORE_LOCKTYPE,
                            (FdoString *) LT_INFO_FREEZE_MODE,
                            (FdoString *) LT_CORE_LTID,
                            (FdoString *) LT_INFO_LTID,
                            (FdoString *) LT_CORE_LOCKID,
                            (FdoString *) LT_INFO_LOCK_ID,
                            (FdoString *) LT_CORE_TABLE)
                  : FdoStringP::Format(
                            L"select  %ls %ls, %ls %ls, %ls %ls, %ls %ls, "
                            L"        %ls %ls, %ls %ls, %ls %ls "
                            L"  from  FDOSYS.%ls "
                            L"  where %ls = '%ls'",
                            (FdoString *) LT_CORE_LTNAME,
                            (FdoString *) LT_INFO_LTNAME,
                            (FdoString *) LT_CORE_DESCRIPTION,
                            (FdoString *) LT_INFO_DESCRIPTION,
                            (FdoString *) LT_CORE_USERNAME,
                            (FdoString *) LT_INFO_USERNAME,
                            (FdoString *) LT_CORE_CREATEDATE,
                            (FdoString *) LT_INFO_CREATEDATE,
                            (FdoString *) LT_CORE_LOCKTYPE,
                            (FdoString *) LT_INFO_FREEZE_MODE,
                            (FdoString *) LT_CORE_LTID,
                            (FdoString *) LT_INFO_LTID,
                            (FdoString *) LT_CORE_LOCKID,
                            (FdoString *) LT_INFO_LOCK_ID,
                            (FdoString *) LT_CORE_TABLE,
                            (FdoString *) LT_CORE_LTNAME,
                            lt_name);

        break;  //  case FdoRdbmsLongTransactionDataRequestType_Data

      case FdoRdbmsLongTransactionDataRequestType_Parents:

        // The user requests long transaction information for all the parents
        // of the named long transaction. The query for this request has the
        // following structure:
        //
        //      select  LT_CORE_LTNAME      LT_INFO_LTNAME,
        //              LT_CORE_DESCRIPTION LT_INFO_DESCRIPTION,
        //              LT_CORE_USERNAME    LT_INFO_USERNAME,
        //              LT_CORE_CREATEDATE  LT_INFO_CREATEDATE,
        //              LT_CORE_LOCKTYPE    LT_INFO_FREEZE_MODE,
        //              LT_CORE_LTID        LT_INFO_LTID,
        //              LT_CORE_LOCKID      LT_INFO_LOCK_ID
        //        from  FDOSYS.LT_CORE_TABLE
        //        where LT_CORE_LTID in (
        //                  select  LT_DEPENDENCY_PARENTLTID
        //                    from  FDOSYS.LT_DEPENDENCY_TABLE
        //                    where LT_DEPENDENCY_CHILDLTID in (
        //                              select  LT_CORE_LTID
        //                                from  FDOSYS.LT_CORE_TABLE
        //                                where LT_CORE_LTNAME = '<lt_name>';

        sql_statement = 
            FdoStringP::Format(
                L"select  %ls %ls, %ls %ls, %ls %ls, %ls %ls, %ls %ls, "
                L"        %ls %ls, %ls %ls "
                L"  from  FDOSYS.%ls "
                L"  where %ls in ( "
                L"    select %ls from FDOSYS.%ls where %ls in ( "
                L"           select %ls from FDOSYS.%ls where %ls = '%ls'))",
                (FdoString *) LT_CORE_LTNAME,
                (FdoString *) LT_INFO_LTNAME,
                (FdoString *) LT_CORE_DESCRIPTION,
                (FdoString *) LT_INFO_DESCRIPTION,
                (FdoString *) LT_CORE_USERNAME,
                (FdoString *) LT_INFO_USERNAME,
                (FdoString *) LT_CORE_CREATEDATE,
                (FdoString *) LT_INFO_CREATEDATE,
                (FdoString *) LT_CORE_LOCKTYPE,
                (FdoString *) LT_INFO_FREEZE_MODE,
                (FdoString *) LT_CORE_LTID,
                (FdoString *) LT_INFO_LTID,
                (FdoString *) LT_CORE_LOCKID,
                (FdoString *) LT_INFO_LOCK_ID,
                (FdoString *) LT_CORE_TABLE,
                (FdoString *) LT_CORE_LTID,
                (FdoString *) LT_DEPENDENCY_PARENTLTID,
                (FdoString *) LT_DEPENDENCY_TABLE,
                (FdoString *) LT_DEPENDENCY_CHILDLTID,
                (FdoString *) LT_CORE_LTID,
                (FdoString *) LT_CORE_TABLE,
                (FdoString *) LT_CORE_LTNAME,
                lt_name);

        break;  //  case FdoRdbmsLongTransactionDataRequestType_Parents

      case FdoRdbmsLongTransactionDataRequestType_Children:

        // The user requests long transaction information for all the parents
        // of the named long transaction. The query for this request has the
        // following structure:
        //
        //      select  LT_CORE_LTNAME      LT_INFO_LTNAME,
        //              LT_CORE_DESCRIPTION LT_INFO_DESCRIPTION,
        //              LT_CORE_USERNAME    LT_INFO_USERNAME,
        //              LT_CORE_CREATEDATE  LT_INFO_CREATEDATE,
        //              LT_CORE_LOCKTYPE    LT_INFO_FREEZE_MODE,
        //              LT_CORE_LTID        LT_INFO_LTID,
        //              LT_CORE_LOCKID      LT_INFO_LOCK_ID
        //        from  FDOSYS.LT_CORE_TABLE
        //        where LT_CORE_LTID in (
        //                  select  LT_DEPENDENCY_CHILDLTID
        //                    from  FDOSYS.LT_DEPENDENCY_TABLE
        //                    where LT_DEPENDENCY_PARENTLTID in (
        //                              select  LT_CORE_LTID
        //                                from  FDOSYS.LT_CORE_TABLE
        //                                where LT_CORE_LTNAME = '<lt_name>';

        sql_statement = 
            FdoStringP::Format(
                L"select  %ls %ls, %ls %ls, %ls %ls, %ls %ls, %ls %ls, "
                L"        %ls %ls, %ls %ls"
                L"  from  FDOSYS.%ls "
                L"  where %ls in ( "
                L"    select %ls from FDOSYS.%ls where %ls in ( "
                L"           select %ls from FDOSYS.%ls where %ls = '%ls'))",
                (FdoString *) LT_CORE_LTNAME,
                (FdoString *) LT_INFO_LTNAME,
                (FdoString *) LT_CORE_DESCRIPTION,
                (FdoString *) LT_INFO_DESCRIPTION,
                (FdoString *) LT_CORE_USERNAME,
                (FdoString *) LT_INFO_USERNAME,
                (FdoString *) LT_CORE_CREATEDATE,
                (FdoString *) LT_INFO_CREATEDATE,
                (FdoString *) LT_CORE_LOCKTYPE,
                (FdoString *) LT_INFO_FREEZE_MODE,
                (FdoString *) LT_CORE_LTID,
                (FdoString *) LT_INFO_LTID,
                (FdoString *) LT_CORE_LOCKID,
                (FdoString *) LT_INFO_LOCK_ID,
                (FdoString *) LT_CORE_TABLE,
                (FdoString *) LT_CORE_LTID,
                (FdoString *) LT_DEPENDENCY_CHILDLTID,
                (FdoString *) LT_DEPENDENCY_TABLE,
                (FdoString *) LT_DEPENDENCY_PARENTLTID,
                (FdoString *) LT_CORE_LTID,
                (FdoString *) LT_CORE_TABLE,
                (FdoString *) LT_CORE_LTNAME,
                lt_name);

        break;  //  case FdoRdbmsLongTransactionDataRequestType_Children

    }  //  switch ...

    return sql_statement;

}  //  BuildLtQueryStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                                BuildMapIdToNameStatement (FdoInt64 lt_id)

// +---------------------------------------------------------------------------
// | The interface creates a SQL statement with which the name of the long
// | transaction identified by the given long transaction id can  be retrieved
// | in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;
    
    // Generate the SQL statement to find the long transaction name for the
    // given long transaction id. The SQL statement is structured as shown
    // next:
    //
    //    select  LT_CORE_LTNAME
    //      from  FDOSYS.LT_CORE_TABLE
    //      where LT_CORE_LTID = lt_id;

#ifdef _WIN32

    sql_statement = 
            FdoStringP::Format(
                            L"select %ls from FDOSYS.%ls where %ls = %I64d",
                            (FdoString *) LT_CORE_LTNAME,
                            (FdoString *) LT_CORE_TABLE,
                            (FdoString *) LT_CORE_LTID,
                            lt_id);

#else

    sql_statement = 
            FdoStringP::Format(
                            L"select %ls from FDOSYS.%ls where %ls = %lld",
                            (FdoString *) LT_CORE_LTNAME,
                            (FdoString *) LT_CORE_TABLE,
                            (FdoString *) LT_CORE_LTID,
                            lt_id);

#endif

    return sql_statement;

}  //  BuildMapNameToIdStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                                BuildMapNameToIdStatement (FdoString *lt_name)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which the long transaction id of
// | the long transaction identified by the given name can be retrieved in a
// | MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Generate the SQL statement to find the long transaction id for the
    // given long transaction name. The SQL statement is structured as shown
    // next:
    //
    //    select  LT_CORE_LTID
    //      from  FDOSYS.LT_CORE_TABLE
    //      where LT_CORE_LTNAME = lt_name;

    sql_statement = 
            FdoStringP::Format(L"select %ls from FDOSYS.%ls where %ls = '%ls'",
                               (FdoString *) LT_CORE_LTID,
                               (FdoString *) LT_CORE_TABLE,
                               (FdoString *) LT_CORE_LTNAME,
                               (FdoString *) lt_name);

    return sql_statement;

}  //  BuildMapNameToIdStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                                BuildNewLtInsertStatement (
                                                    FdoString *lt_name,
                                                    FdoString *lt_description,
                                                    FdoString *lt_owner)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which a new long transaction can
// | be inserted into the long transaction table F_LT in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare all necessary local variables.

    FdoStringP sql_statement;

    // Create the SQL statement that adds a new long transaction into the table
    // F_LT in a MySQL environment. The structure of the statement depends on
    // whether or not a description is provided:
    //
    //  With a description:
    //    insert into FDOSYS.LT_CORE_TABLE
    //              (LT_CORE_LTNAME,
    //               LT_CORE_DESCRIPTION,
    //               LT_CORE_CREATEDATE,
    //               LT_CORE_USERNAME)
    //      values  ('lt_name',
    //               'lt_description',
    //               current_date(),
    //               'lt_owner');
    //
    //  Without a description:
    //    insert into FDOSYS.LT_CORE_TABLE
    //              (LT_CORE_LTNAME,
    //               LT_CORE_CREATEDATE,
    //               LT_CORE_USERNAME)
    //      values  ('lt_name',
    //               current_date(),
    //               'lt_owner');

    sql_statement = (lt_description == NULL)
        ? FdoStringP::Format(L"insert into FDOSYS.%ls (%ls, %ls, %ls) "
                             L"  values ('%ls', %ls, '%ls')",
                             (FdoString *) LT_CORE_TABLE,
                             (FdoString *) LT_CORE_LTNAME,
                             (FdoString *) LT_CORE_CREATEDATE,
                             (FdoString *) LT_CORE_USERNAME,
                             lt_name,
                             L"current_date()",
                             lt_owner)
        : FdoStringP::Format(L"insert into FDOSYS.%ls (%ls, %ls, %ls, %ls) "
                             L"  values ('%ls', '%ls', %ls, '%ls')",
                             (FdoString *) LT_CORE_TABLE,
                             (FdoString *) LT_CORE_LTNAME,
                             (FdoString *) LT_CORE_DESCRIPTION,
                             (FdoString *) LT_CORE_CREATEDATE,
                             (FdoString *) LT_CORE_USERNAME,
                             lt_name,
                             lt_description,
                             L"current_date()",
                             lt_owner);

    return sql_statement;

}  //  BuildNewLtInsertStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                        BuildRollbackDeleteStatement (FdoString *data_store_name,
                                                      FdoString *table_name,
                                                      FdoString *where_clause)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which identified rows in the
// | named table can be deleted in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP sql_statement;

    // Create the generic SQL statement. The statement's structure is as shown
    // next:
    //
    //  delete from <data_store_name>.<table_name> where <where_clause>;

    sql_statement = FdoStringP::Format(L"delete from %ls.%ls where %ls",
                                       data_store_name,
                                       table_name,
                                       where_clause);

    return sql_statement;

}  //  BuildRollbackDeleteStatement ()

FdoStringP FdoMySqlLongTransactionManagerService::
                        BuildRollbackUpdateStatement (
                                                FdoString *data_store_name,
                                                FdoString *table_name,
                                                FdoString *where_clause)

// +---------------------------------------------------------------------------
// | The function creates a SQL statement with which identified rows in the
// | named table can be updated in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP   sql_statement;

    // Create the generic SQL statement. The statement's structure is like
    // shown next:
    //
    //  update  <data_store_name>.<table_name>
    //    set   NEXTLTID = NULL
    //    where <where_clause>;
    //
    // NOTE: This statement will work only in a non-multi-branch environment.

    sql_statement =
            FdoStringP::Format(
                            L"update %ls.%ls set NEXTLTID = NULL where %ls",
                            data_store_name,
                            table_name,
                            where_clause);

    return sql_statement;

}  // BuildRollbackUpdateStatement ()

char *FdoMySqlLongTransactionManagerService::
                                    GetCharToNumberConvertionFunctionName ()

// +---------------------------------------------------------------------------
// | The function provides the name of the routine used to convert a string to
// | a number in a MySQL environment.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    char *function_name = NULL;

    // Generate the return value.

    if ((function_name = new char[5]) != NULL)
        strcpy(function_name, "cast");

    return function_name;

}  //  GetCharToNumberConvertionFunctionName ()

FdoStringP FdoMySqlLongTransactionManagerService::GetEmbeddedQuoteEsqSeq ()

// +---------------------------------------------------------------------------
// | The function returns the escape sequence for embedded quotes in a MySQL
// | environment.
// +---------------------------------------------------------------------------

{
    return (L"\\'");

}  //  GetEmbeddedQuoteEsqSeq ()

bool FdoMySqlLongTransactionManagerService::LtDependencyQueryLoopRequired ()

// +---------------------------------------------------------------------------
// | Depending on the underlying environment, retrieving dependency information
// | from the long transaction table F_LtDependency may require some looping.
// | This function indicates whether or not a loop is required in a MySQL en-
// | vironment.
// +---------------------------------------------------------------------------

{

    return true;

}  //  LtDependencyQueryLoopRequired ()


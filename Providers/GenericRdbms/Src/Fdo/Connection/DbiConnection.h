#ifndef DBICONNECTION_H
#define DBICONNECTION_H     1
/*
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
 */
#include "FdoRdbms.h"
#include "FdoRdbmsUtil.h"
#include <Sm/SchemaManager.h>
#include <../../Gdbi/GdbiConnection.h>
#include <../../Gdbi/GdbiQueryResult.h>

#include "../Lt/version_info.h"
#include "../Lt/tab_cfl_list_obj.h"
#include "../Lock/lknm_spec.h"
#include "../Lock/lk_data_obj.h"
#include "Inc/rdbi.h"
#include "Inc/Rdbi/proto.h"
#include "Inc/Rdbi/context.h"
#include "Inc/Util/thread.h"

#include "FdoCommon.h"
typedef FdoByteArray ByteArray; // This helps defer some changes to existing code.

typedef long    dbi_pn_id_t;

// Need to export few things for white box testing
#ifdef _DEBUG
#define FDORDBMS_TEST FDORDBMS_API
#else
#define FDORDBMS_TEST
#endif

class   FdoSmLpSchema;
class   FdoRdbmsFilterProcessor;
class   FdoRdbmsSchemaUtil;
class   FdoRdbmsLockConflicts;

class DbiConnection
{
public:

    static const wchar_t DELIMITER[];
    static const wchar_t DELIMITER2[];
    static const wchar_t SEPARATOR;
    static const wchar_t DATASOURCE[];
    static const wchar_t DATASOURCE2[];
    static const wchar_t USERID[];
    static const wchar_t USERID2[];
    static const wchar_t PASSWORD[];
    static const wchar_t SCHEMA[];
    static const wchar_t CONNECTIONSTRING[];
    static const wchar_t GENERATEDEFAULTGEOMETRYPROPERTY[];

private:

    class ParseInfo
    {
        public:
            FdoStringP mDataSource;
            FdoStringP mUser;
            FdoStringP mPassword;
            FdoStringP mSchema;
            FdoStringP mConnectionStringProperty;    // A connection property actually named "ConnectionString"
            FdoStringP mIsGeometryFromOrdinatesWanted;   // Unspecified if empty.  Can be "true" or "false".
            FdoStringP mRemainder;

            ParseInfo (wchar_t *datasource, wchar_t *user, wchar_t *password, wchar_t *schema, wchar_t *connectionString, wchar_t *defaultGeometryWanted, wchar_t *remainder);
            virtual ~ParseInfo ();
    };

    int     mDbiContextId; // Dbi database id

    wchar_t *mConnectionString; // connection string for the database

    ParseInfo* mParsedConnection;

    FdoStringP mDbSchemaName;

    FdoConnectionState     mOpen;     // if the database is open

    bool    mIsWorkspaceManagerEnvironment;

    FdoRdbmsFilterProcessor     *mFilterProcessor;

    FdoRdbmsSchemaUtil      *mSchemaUtil;

    FdoRdbmsUtil            mUtility;

    rdbi_context_def        *mContext;

    int mIndex;

    /// <summary>
    /// Extract dbuser and password from the connection string, leaving the remainder.
    /// </summary>
    void parseConnectString ();
    ParseInfo* parseConnectString ( const wchar_t *connStr );

public:
    FDORDBMS_TEST DbiConnection( );
    FDORDBMS_TEST ~DbiConnection(void);

    FDORDBMS_TEST FdoConnectionState Open (bool skipPending);

    void SetConnectionState (FdoConnectionState state);

    FDORDBMS_TEST void SetConnectionString( const wchar_t* connString );

    void InitRdbi( initializer* driver )
    {
        rdbi_init( &mContext, driver);
        mGdbiConnection = new GdbiConnection( mContext );
    }

    bool IsWorkspaceManagerEnvironment ();
    FdoStringP GetUser ();
    FdoStringP GetPassword();
    FdoStringP GetSchema ();
    FdoStringP GetDataSource ();
    FDORDBMS_TEST  void Close ();

    //dbi_context_def *GetCtxt() { return mContext; }

    int DoSql (char *sql_buf, int *rows_processed, bool bThrowException = false);

    //
    // Get a schema utility object
    FDORDBMS_TEST FdoRdbmsSchemaUtil* GetSchemaUtil( );

    //
    // Get the utility object
    FdoRdbmsUtil* GetUtility();

    //
    // Given a select statement of the form select featid from ..., this helper method attempts to create a RDBMS transaaction lock
    // on the F_Feature table for the rows defined by the inner select
    bool SetTransactionLock( const char *sqlStatement );

    //
    // Start of DBI wrapper methods
    // Refer to the DBI function of the same name for a detailed description of the method and parameters


    // See function dbi_ver_dbase
    int     dbi_ver_dbase( char *db_name, int  *exists);

    // See function dbi_conflict
    //dbi_ifp_def   dbi_conflict( dbi_ifp_def report_func );

    // See function dbi_version_activate_query
    int dbi_version_activate_query (dbi_version_info_def lt_info,
                                    int                  query_request,
                                    int                  *qid);

    // See function dbi_version_get_query_data
    int dbi_version_get_query_data (dbi_version_info_def *lt_info,
                                    int                  *is_eof,
                                    int                  qid);

    // See function dbi_version_terminate_query
    int dbi_version_terminate_query (int qid);

    // See function dbi_version_active_get
    FDORDBMS_TEST long  dbi_version_active_get( void );

    // See function dbi_version_get_active_lt
    FDORDBMS_TEST int dbi_version_get_active_lt (char **active_lt);

    // See function dbi_version_active_set()
    FDORDBMS_TEST int dbi_version_active_set( dbi_version_info_def *version_info,
                                              int                  *err_code );

    // See function dbi_version_add
    FDORDBMS_TEST int dbi_version_add( dbi_version_info_def *version_info,
                                       int                  ignore_dup,
                                       int                  *err_code );

    // See function dbi_version_commit.
    int dbi_version_commit( char *lt_names,
                            int  len,
                            int  conflict_flag,
                            char *tran_id,
                            int  *err_code );

    // See function dbi_version_load_conflicts.
    int dbi_version_load_conflicts (char                 *lt_name,
                                    dbi_tab_cfl_list_obj **cfl_root,
                                    int                  *err_code);

    // See function dbi_version_clear_conflicts.
    int dbi_version_clear_conflicts (dbi_tab_cfl_list_obj **cfl_root);

    // See function dbi_version_clear_conflicts.
    int dbi_version_process_conflicts (char                 *lt_name,
                                       dbi_tab_cfl_list_obj **cfl_root,
                                       int                  *err_code);

    // See function dbi_version_rollback.
    int dbi_version_rollback (char *lt_names,
                              int  len,
                              int  conflict_flag,
                              char *tran_id,
                              int  *err_code);

    // See function dbi_version_add_depend
    FDORDBMS_TEST int dbi_version_add_depend( dbi_vn_id_t parent_version );

    // See function dbi_version_sel_set_add
    FDORDBMS_TEST int dbi_version_sel_set_add( long *vers_list, int list_len );

    // See function dbi_version_ws_disable
    FDORDBMS_TEST int dbi_version_ws_disable (char *table_name,
                                              int  force_discard,
                                              int  ignore_last_error,
                                              int  is_topology);

    // See function dbi_version_ws_disable_all
    FDORDBMS_TEST int dbi_version_ws_disable_all ();

    // See function dbi_version_ws_enable
    FDORDBMS_TEST int dbi_version_ws_enable (char *table_name,
                                             char *history_option,
                                             int  is_topology);

    // See function dbi_version_ws_is_enabled
    int dbi_version_ws_is_enabled (char *owner,
                                   char *table_name,
                                   int  *is_version_enabled);

    // See function dbi_bind
    FDORDBMS_TEST int dbi_bind(
                int gid,
                char *name,
                int datatype,
                int size,
                char *address,
                char *null_ind,
                int *ccode
            );

    // See function dbi_col_in_tab
    int dbi_col_in_tab(
                char *col_name,
                char *table_name,
                int *exists_flag
            );

    // See function dbi_db_empty
    int dbi_db_empty( int *db_empty );

    // See function dbi_deac_table
    int dbi_deac_table( void );

    // See function dbi_define
    FDORDBMS_TEST int dbi_define(
                int gid,
                char *name,
                int datatype,
                int size,
                char *address,
                char *null_ind,
                int *ccode
            );

    // See function dbi_desc_nslct
    int dbi_desc_nslct( int gid, int *n );

    // See function dbi_execute
    FDORDBMS_TEST int dbi_execute( int gid,  int *rows_processed, int *ccode );


    // See function dbi_fetch
    FDORDBMS_TEST int dbi_fetch( int gid, int *ccode );

    // See function dbi_flush
    int dbi_flush(
            int gid,
            int *rows_processed,
            int *ccode
            );

    // See function dbi_flush_all
    int dbi_flush_all( int *ccode );

    // See function dbi_free
    FDORDBMS_TEST int dbi_free( int gid, int *ccode );

    // See function dbi_get_val_a
    int dbi_get_val_a(
            int gid,
            char *name,
            int ml,
            char *ascii,
            int  *isnull,
            int *ccode
            );

    // See function dbi_get_val_b
    FDORDBMS_TEST int dbi_get_val_b(
            int gid,
            char *name,
            int ml,
            char *address,
            char *null_ind,
            int *ccode
            );

    // See function dbi_gql
    FDORDBMS_TEST int dbi_gql(
            char *gql,
            int *gid
            );

    // -----------------------------------------------------------------------
    // Lock related DBI functions
    // --------------------------
    //    dbi_lock_act:
    //      Activates the given lock
    //    dbi_lock_activate_lkd_query:
    //       Activates a query to retrieve locked objects for a given lock
    //      owner.
    //    dbi_lock_chk_lk_conf:
    //      Checks identified objects for persistent lock conflicts.
    //    dbi_lock_create:
    //      Creates a new lock definition.
    //    dbi_lock_deac:
    //      Deactivates the active lock.
    //    dbi_lock_def_conflict_init:
    //      Initializes a lock conflict handler (NOT USED).
    //    dbi_lock_del:
    //      Deletes the given lock.
    //    dbi_lock_find_by_id:
    //      Returns the lock definition for the lock identified by its id
    //    dbi_lock_get_lk_types:
    //      Returns supported lock types for the Oracle Provider.
    //    dbi_lock_get_lkd_query_data:
    //      Returns a record for an object that is locked by a specified
    //      lock owner.
    //    dbi_lock_get_lock_info:
    //       Retrieves lock information for identified objects
    //    dbi_lock_lock:
    //      Executes a lock request
    //    dbi_lock_terminate_lkd_query:
    //       Terminates the query to return locked objects for a specified
    //      lock owner.
    //    dbi_lock_transaction:
    //      Applies a transaction lock on selected objects
    //    dbi_lock_unlock:
    //      Executes an unlock request
    // -----------------------------------------------------------------------

    FDORDBMS_TEST int dbi_lock_act (
                          char              *lock_name,
                          char              *opened_by_conflict);
                  int dbi_lock_activate_lkd_query (
                          char              *lock_owner,
                          char              *table_name,
                          dbi_lk_data_obj   **lk_info);
                  int dbi_lock_chk_lk_conf (
                          char              *table_name,
                          char              *sql_filter,
                          char              *current_user,
                          int               operation_id,
                          int               *conflict_counter);
    FDORDBMS_TEST int dbi_lock_create (
                          char              *lock_name,
                          int               autogen,
                          char              *description,
                          char              *gen_lock_name,
                          int               *exists);
    FDORDBMS_TEST int dbi_lock_deac ();
                  int dbi_lock_def_conflict_init ();
    FDORDBMS_TEST int dbi_lock_del (
                          char              *lock_name,
                          int               vers_act);
                  int dbi_lock_find_by_id (
                          dbi_li_id_t       lock_id,
                          dbi_lknm_spec_def *lock_spec,
                          int               *found);
    FDORDBMS_TEST int dbi_lock_get_lk_types  (
                          int               lock_types[],
                          int               array_size);
                  int dbi_lock_get_lkd_query_data (
                          dbi_lk_data_obj   *lk_info,
                          int               *is_eof);
                  int dbi_lock_get_lock_info (
                          char              *table_name,
                          char              *sql_filter,
                          char              **sql_statement);
                  int dbi_lock_lock (
                          char              *table_name,
                          char              *sql_filter,
                          int               lock_type,
                          char              *lock_conflicts_view_name,
                          int               *err_code);
                  int dbi_lock_terminate_lkd_query (
                          dbi_lk_data_obj   **lk_info);
                  int dbi_lock_transaction (
                          char              *table_name,
                          char              *sql_filter,
                          int               do_sync_cm);
                  int dbi_lock_unlock (
                          char              *table_name,
                          char              *sql_filter,
                          int               is_admin_op,
                          char              *lock_conflicts_view_name);

    // -----------------------------------------------------------------------

    // See function dbi_get_user_num
    int dbi_get_user_num();

    // See function dbi_is_sys_tbl
    int dbi_is_sys_tbl(char *table_name,
                       int  *system_tbl);

    // See function dbi_set_nnull
    void dbi_set_nnull(
            char *null_ind,
            int start,
            int end
            );

    // See function dbi_object_find
    int dbi_object_find(
            char *db_name,
            char *object_name,
            char  *dblink,
            int *found,
            char *object_type
            );

    // See function dbi_option
    FDORDBMS_TEST int dbi_option(
            int gid,
            char *option,
            int *ccode
            );

    // See function dbi_tran_begin
    FDORDBMS_TEST int dbi_tran_begin(
            char *tran_id
            );

    // See function dbi_tran_end
    FDORDBMS_TEST int dbi_tran_end(
            char *tran_id
            );

    // See function dbi_tran_rollback
    int dbi_tran_rollback(
            void
            );

    int dbi_index_add(
        char *index_name_in,
        char *table_name,
        char * *column_list,
        int ncol,
        int type,
        char *table_space,
        char *index_name_out
    );

    int dbi_index_act(
        char            *lookup_key,
        int             ltype
    );

    int dbi_index_get(
        char            *index_name,
        char            *table_name,
        char            *column_name,
        int             *itype,
        int             *start,
        int             *eof
    );

    int dbi_index_deac();

        // Activate database retrieval
    int dbi_act_dbase(
        void
        );

    // Gets the current database name
    int dbi_get_dbase(
        char *dbname,
        int  *eof
        );

    // Deactivate database retrieval
    int dbi_deac_dbase(
        void
        );

    // Create Spatial Index on a geometry table
    FDORDBMS_TEST int dbi_index_add_sptl(
        char *geom_tbl_name,
        char *geom_col_name,
        char *index_name,
        int is_rtree,
        int num_dims
        );

    // Drop Spatial Index on a geometry table
    FDORDBMS_TEST int dbi_index_drop_sptl(
        char *geom_tbl_name,
        char *geom_col_name,
        char *index_name
        );

    // Computes statistics
    int dbi_stats_gather(
        char *geom_tbl_name
        );

    // LOB related DBI functions
    FDORDBMS_TEST int dbi_lob_create_ref(
            int sqlid,
            void **lob_ref
            );

    FDORDBMS_TEST int dbi_lob_destroy_ref(
            int sqlid,
            void *lob_ref
            );

    FDORDBMS_TEST int dbi_lob_get_size(
            int sqlid,
            void *lob_ref,
            unsigned int *size);

    FDORDBMS_TEST int dbi_lob_write_next(
            int sqlid,
            void *lob_ref,
            int  lob_type,
            unsigned int chunk_size,
            char* chunk
            );

    FDORDBMS_TEST int dbi_lob_read_next(
            int sqlid,
            void *lob_ref,
            int  lob_type,
            unsigned int chunk_size,
            char *chunk,
            unsigned int *chunk_size_out,
            int *eol
            );

    FDORDBMS_TEST int dbi_lob_open(
            int sqlid,
            void *lob_ref,
            int for_read
            );

    FDORDBMS_TEST int dbi_lob_close(
            int sqlid,
            void *lob_ref
            );

    FDORDBMS_TEST int dbi_csys_get_wkt(
            long    srid_I,
            char    *wkt_O
            );

    // lt_method can be one of:
    //  DBI_WORKSPACE_MANAGER_LT_METHOD - OWM enabled datastore
    //  DBI_NO_LT_METHOD - datastore without versioning
    //  -1 - defaults to DBI_WORKSPACE_MANAGER_LT_METHOD
    FDORDBMS_TEST int dbi_set_lt_method(int lt_method);

    int dbi_col_has_values(char *table, char *column, int *has_values);

    // Get the schema (database) name for this connection.
    FdoStringP GetDbSchemaName();

    void SetDbSchemaName(const wchar_t * schemaName);

    void SetActiveSchema(const wchar_t * schemaName);

    const wchar_t *Property2ColName(const wchar_t *className, const wchar_t *attribute);
    const wchar_t *ColName2Property(const wchar_t *className, const wchar_t *column);

    const wchar_t *GetTable(const wchar_t *className);

    const FdoSmLpSchema* DbiConnection::GetSchema(const wchar_t *className);

    int GetNextIndex(void);

    private:
        void LoadClassMapping();

            GdbiConnection      *mGdbiConnection;

    public:
           GdbiCommands*      GetGdbiCommands() { return mGdbiConnection->GetCommands();}

    GdbiConnection*      GetGdbiConnection() { return mGdbiConnection;}

    // Temporary functions until we get rid of the old schema manager.
    void SetSchemaManager( FdoSchemaManagerP schemaManager );
    FdoSchemaManagerP GetSchemaManager();

};

#endif // DBICONNECTION_H


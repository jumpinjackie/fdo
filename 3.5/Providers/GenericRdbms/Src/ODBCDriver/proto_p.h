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

#ifndef ODBCDR_PROTO_P_H
#define ODBCDR_PROTO_P_H    1

#include <Inc/Rdbi/vndr_info.h>	/* rdbi_vndr_info_def	*/
#include <Inc/Rdbi/methods.h>   /* rdbi methods table   */

#include "context.h"

int odbcdr_rdbi_init (odbcdr_context_def **context, rdbi_methods methods);

int odbcdr_bind( odbcdr_context_def *context, char *cursor, char *name, int datatype, int size, char *address, SQLLEN *null_ind );
int odbcdr_close_cursor( odbcdr_context_def *context, char *cursor );

int odbcdr_col_act( odbcdr_context_def *context, const char *owner, const char *object_name, const char *dbaselink );
int odbcdr_col_actW( odbcdr_context_def *context, const wchar_t *owner, const wchar_t *object_name, const wchar_t *dbaselink );

int odbcdr_col_get( odbcdr_context_def *context, char *column_name, char *type, int *length, int *scale, int *nullable, int *is_autoincrement, int *position, int *eof );
int odbcdr_col_getW( odbcdr_context_def *context, wchar_t *column_name, wchar_t *type, int *length, int *scale, int *nullable, int *is_autoincrement, int *position, int *eof );

int odbcdr_col_deac( odbcdr_context_def *context );

int odbcdr_pkeys_act( odbcdr_context_def *context, const char *owner, const char *target );
int odbcdr_pkeys_actW( odbcdr_context_def *context, const wchar_t *owner, const wchar_t *target );

int odbcdr_pkeys_get( odbcdr_context_def *context, char *name, int *eof );
int odbcdr_pkeys_getW( odbcdr_context_def *context, wchar_t *name, int *eof );

int odbcdr_pkeys_deac( odbcdr_context_def *context );

int odbcdr_users_act( odbcdr_context_def *context, const char *target );
int odbcdr_users_actW( odbcdr_context_def *context, const wchar_t *target );

int odbcdr_users_get( odbcdr_context_def *context, char *name, int *eof );
int odbcdr_users_getW( odbcdr_context_def *context, wchar_t *name, int *eof );

int odbcdr_users_deac( odbcdr_context_def *context );

int odbcdr_stores_act( odbcdr_context_def *context );

int odbcdr_stores_get( odbcdr_context_def *context, char *name, int *eof );
int odbcdr_stores_getW( odbcdr_context_def *context, wchar_t *name, int *eof );

int odbcdr_stores_deac( odbcdr_context_def *context );

int odbcdr_commit( odbcdr_context_def *context, int commit_action );

int odbcdr_connect( odbcdr_context_def *context, const char *connect_string, const char *user, const char *pswd, char **vendor_data, int *connect_id );
int odbcdr_connectW( odbcdr_context_def *context, const wchar_t *connect_string, const wchar_t *user, const wchar_t *pswd, char **vendor_data, int *connect_id );

odbcdr_cursor_def *odbcdr_crt_cursor( odbcdr_context_def *context, odbcdr_cursor_def *old_cursor);

int odbcdr_define( odbcdr_context_def *context, char *cursor, char *name, int datatype, int size, char *address, SQLLEN *null_ind );

int odbcdr_desc_bind( odbcdr_context_def *context, char *cursor, int position, int name_size, char *name );

int odbcdr_desc_slct( odbcdr_context_def *context, char *cursor, int position, int name_size, char *name, int *rdbi_type, int *binary_size, int *null_ok );
int odbcdr_desc_slctW( odbcdr_context_def *context, char *cursor, int position, int name_size, wchar_t *name, int *rdbi_type, int *binary_size, int *null_ok );

int odbcdr_disconnect( odbcdr_context_def *context, char **vendor_data );

int odbcdr_est_cursor( odbcdr_context_def *context, char **cursor ); // CMB: was char **cursor

int odbcdr_exec_coc( odbcdr_context_def *context, char *cursor, char *sql, void *ptree, int *rows );
int odbcdr_execute( odbcdr_context_def *context, char *cursor, int count, int offset, int *rows_processed );

int odbcdr_fetch( odbcdr_context_def *context, char *cursor, int count, int *rows_processed );

int odbcdr_fetch2( odbcdr_context_def *context, char *cursor, int count, int do_exec, int do_cancel, int *rows_processed );

int odbcdr_fre_cursor( odbcdr_context_def *context, char **cursor );

int odbcdr_get_con_var( odbcdr_context_def *context, char *value );
int odbcdr_get_con_varW( odbcdr_context_def *context, wchar_t *value );

int odbcdr_get_cursor( odbcdr_context_def *context, char *cursorOpaque_I, odbcdr_cursor_def **cursorStruct_O );

int odbcdr_get_curr_conn( odbcdr_context_def *context, odbcdr_connData_def **connData_O );

void odbcdr_get_msg( odbcdr_context_def *context, char *buffer );
void odbcdr_get_msgW( odbcdr_context_def *context, wchar_t *buffer );

void odbcdr_get_o_t_s( odbcdr_context_def *context, int datatype, int size, int *odbcdr_datatype, int *odbcdr_size );

int odbcdr_index_act( odbcdr_context_def *context, char *owner, char *object_name, int object_type );
int odbcdr_index_get( odbcdr_context_def *context, char *index_name, char *table_name, char *column_name, int *type, int *eof );
int odbcdr_index_deac( odbcdr_context_def *context );
int odbcdr_object( odbcdr_context_def *context, char *owner, char *object_name, char *type, int *exists );

int odbcdr_objects_act( odbcdr_context_def *context, const char *owner, const char *target );
int odbcdr_objects_actW( odbcdr_context_def *context, const wchar_t *owner, const wchar_t *target );

int odbcdr_objects_get( odbcdr_context_def *context, char *object_name, char *object_type, int *eof );
int odbcdr_objects_getW( odbcdr_context_def *context, wchar_t *object_name, wchar_t *object_type, int *eof );

int odbcdr_objects_deac( odbcdr_context_def *context );

int odbcdr_run_sql( odbcdr_context_def *context, const char *sql, int isddl, int *rows_processed);
int odbcdr_run_sqlW( odbcdr_context_def *context, const wchar_t *sql, int isddl, int *rows_processed);

void odbcdr_show_context( odbcdr_context_def *context, odbcdr_cursor_def *c );

void odbcdr_show_cursor_list( odbcdr_context_def *context, odbcdr_cursor_def *list_head );

int odbcdr_sql( odbcdr_context_def *context, char *cursor, const char *sql, int defer, char *verb, void *ptree, char *cursor_coc );
int odbcdr_sqlW( odbcdr_context_def *context, char *cursor, const wchar_t *sql, int defer, char *verb, void *ptree, char *cursor_coc );

int odbcdr_switch( odbcdr_context_def *context, int connect_id );

int odbcdr_usr_exists( odbcdr_context_def *context, char *user_name, int *exists );
int odbcdr_usr_existsW( odbcdr_context_def *context, wchar_t *user_name, int *exists );

char *odbcdr_vndr_name( odbcdr_context_def *context );
wchar_t *odbcdr_vndr_nameW( odbcdr_context_def *context );

int odbcdr_vndr_info( odbcdr_context_def *context, rdbi_vndr_info_def *vndr_info);

int odbcdr_get_gen_id( odbcdr_context_def  *context, const char *table_name, int  *id );
int odbcdr_get_gen_idW( odbcdr_context_def  *context, const wchar_t *table_name, int  *id );

int odbcdr_alcnullind( odbcdr_context_def *context, int n, char  **null_ind );

void odbcdr_set_null( odbcdr_context_def *context, SQLLEN *null_ind, int start, int end );

void odbcdr_set_nnull( odbcdr_context_def *context, SQLLEN *null_ind, int start, int end );

int odbcdr_is_null( odbcdr_context_def  *context, SQLLEN *null_ind, int offset );

int odbcdr_xlt_status( odbcdr_context_def *context, SQLRETURN odbcdr_status, SQLSMALLINT handle_type, SQLHANDLE handle);

int odbcdr_set_schema( odbcdr_context_def *context, const char *schema_name);
int odbcdr_set_schemaW( odbcdr_context_def *context, const wchar_t *schema_name);

int odbcdr_set_sess_env( odbcdr_context_def *context );

int odbcdr_term( void **contextp );

int odbcdr_geom_bindColumn( odbcdr_context_def *context, odbcdr_cursor_def *cursor, int position, char *address );

int odbcdr_geom_defineColumn( odbcdr_context_def *context, odbcdr_cursor_def *cursor, int position, char *address );

int odbcdr_geom_setNumRows( odbcdr_context_def *context, odbcdr_cursor_def *cursor, int num_rows );

int odbcdr_geom_getSqlServerBuffAddr( odbcdr_context_def *context, odbcdr_geom_col_list_def *columnList_I, int position_I, odbcdr_geom_def ***buffer_O, odbcdr_geomNI_def ***ni_O );

int odbcdr_geom_convertBoundToSqlServer(odbcdr_context_def *context,  odbcdr_cursor_def *cursor, int num_rows );

int odbcdr_geom_convertDefinedFromSqlServer( odbcdr_context_def *context, odbcdr_cursor_def *cursor, int num_rows );

int odbcdr_geom_freeSqlServerGeometries( odbcdr_context_def *context, odbcdr_cursor_def *cursor );

int odbcdr_geom_freeAllColumns( odbcdr_context_def *context, odbcdr_cursor_def *cursor );

int odbcdr_autocommit_on( odbcdr_context_def *context );

int odbcdr_autocommit_off( odbcdr_context_def *context );

int odbcdr_autocommit_mode( odbcdr_context_def *context );

#ifdef _WIN32
    #define odbcdr_swprintf swprintf_s
#else
    #define odbcdr_swprintf swprintf
#endif

#endif /* ODBCDR_PROTO_P_H */

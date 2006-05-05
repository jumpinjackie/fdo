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

/************************************************************************
*
* Name
*   inc/rdbi/proto.h - Defines function prototypes for rdbi package
*
* Synopsis
*   #include <Inc/Rdbi/proto.h>
*
* Description
*   Defines function prototypes
*
* Remarks
*
*
************************************************************************/

#ifndef INC_RDBI_PROTO_H
#define INC_RDBI_PROTO_H

#include <Inc/Rdbi/vndr_info.h>
#include <Inc/Rdbi/context.h>
#include <Inc/Rdbi/methods.h>

/* GENERATED */

typedef int initializer (void **contextp, rdbi_methods methods);

int rdbi_bind(
        rdbi_context_def *context,
        int sqlid,
        char *name,
        int datatype,
        int size,
        char *address,
        void *null_ind
        );
int rdbi_break(
        rdbi_context_def *context
        );
int rdbi_col_act(
        rdbi_context_def *context,
        char *owner,
        char *object_name,
        char  *dbaselink
        );
int rdbi_col_get(
        rdbi_context_def *context,
        char *column_name,
        char *type,
        int *length,
        int *scale,
        int *nullable,
        int *is_autoincrement,
        int *position,
        int *eof
        );
int rdbi_col_deac(
        rdbi_context_def *context
        );
int rdbi_commit(
        rdbi_context_def *context
        );
int rdbi_connect(
        rdbi_context_def *context,
        char *connect_string,
        char *dataset,
        char *passwd,
        int *connect_id
        );
int rdbi_crsr_nrows(
        rdbi_context_def *context,
        int sqlid
        );
int rdbi_define(
        rdbi_context_def *context,
        int sqlid,
        char *name,
        int datatype,
        int size,
        char *address,
        void *null_ind
        );
int rdbi_desc_bind(
        rdbi_context_def *context,
        int sqlid,
        int position,
        int name_len,
        char *name
        );
int rdbi_desc_slct(
        rdbi_context_def *context,
        int sqlid,
        int pos,
        int name_len,
        char *name,
        int *rdbi_type,
        int *binary_size,
        int *null_ok
        );
int rdbi_desc_slctW(
        rdbi_context_def *context,
        int sqlid,
        int pos,
        int name_len,
        wchar_t *name,
        int *rdbi_type,
        int *binary_size,
        int *null_ok
        );
int rdbi_disconnect(
        rdbi_context_def *context
        );
int rdbi_end_select(
        rdbi_context_def *context,
        int sqlid
        );
int rdbi_est_cursor(
        rdbi_context_def *context,
        int *sqlid
        );
int rdbi_exec_coc(
        rdbi_context_def *context,
        int sqlid,
        char *sql,
        int *rows
        );
int rdbi_execute(
        rdbi_context_def *context,
        int sqlid,
        int count,
        int offset
        );
int rdbi_fetch(
        rdbi_context_def *context,
        int sqlid,
        int count,
        int *rows_processed
        );
int rdbi_exec_fetch(
        rdbi_context_def *context,
        int sqlid,
        int count,
        int do_cancel,
        int *rows_processed
        );
int rdbi_fre_cursor(
        rdbi_context_def *context,
        int sqlid
        );
int rdbi_get_info(
        rdbi_context_def *context,
        char *keyword,
        int type,
        void *value,
        int *exists
        );
void rdbi_get_msg(
        rdbi_context_def *context
        );
int rdbi_get_size(
        rdbi_context_def *context,
        int datatype
        );
int rdbi_id_len(
        rdbi_context_def *context
        );
int rdbi_init (
        rdbi_context_def **context,
        initializer *driver
        );
int rdbi_initialize (
        rdbi_context_def **context,
        char *driver
        );
int rdbi_alcnullind(
        rdbi_context_def *context,
        int n,
        char * *null_ind
        );
int rdbi_objects_act(
        rdbi_context_def *context,
        const char *owner,
        const char *target
        );
int rdbi_objects_get(
        rdbi_context_def *context,
        char *object_name,
        char *object_type,
        int *eof
        );
int rdbi_objects_deac(
        rdbi_context_def *context 
        );
void rdbi_set_null(
        rdbi_context_def *context,
        void *null_ind,
        int start,
        int end
        );
void rdbi_set_nnull(
        rdbi_context_def *context,
        void *null_ind,
        int start,
        int end
        );
int rdbi_is_null(
        rdbi_context_def *context,
        void *null_ind,
        int offset
        );
int rdbi_sql(
        rdbi_context_def *context,
        int sqlid,
        char *sql
        );
int rdbi_sqlW(
        rdbi_context_def *context,
        int sqlid,
        wchar_t *sql
        );
int rdbi_sql_d(
        rdbi_context_def *context,
        int sqlid,
        char *sql
        );
int rdbi_sql_va(
        rdbi_context_def *context,
        int opts,
        int sqlid,
        char *sql,
        ... /* long va_alist */
        );
int rdbi_switch(
        rdbi_context_def *context,
        int connect_id
        );
int rdbi_term(
        rdbi_context_def **context
        );
int rdbi_tran_begin(
        rdbi_context_def *context,
        char *tran_id
        );
int rdbi_tran_end(
        rdbi_context_def *context,
        char *tran_id
        );
int rdbi_tran_getid(
        rdbi_context_def *context,
        char * *tran_id,
        int *last_trans
        );
int rdbi_tran_rolbk(
        rdbi_context_def *context
        );
int rdbi_pkeys_act(
    	rdbi_context_def *context,
        const char *object
        );
int rdbi_pkeys_deac(
        rdbi_context_def *context
        );
int rdbi_pkeys_get(
	    rdbi_context_def *context,
        char *name,
        int  *eof
        );
int rdbi_users_act(
    	rdbi_context_def *context,
        const char *target
        );
int rdbi_users_deac(
        rdbi_context_def *context
        );
int rdbi_users_get(
	    rdbi_context_def *context,
        char *name,
        int  *eof
        );
int rdbi_stores_act(
    	rdbi_context_def *context
        );
int rdbi_stores_deac(
        rdbi_context_def *context
        );
int rdbi_stores_get(
	    rdbi_context_def *context,
        char *name,
        int  *eof
        );
int rdbi_usr_exists(
        rdbi_context_def *context,
        char *user_name,
        int *exists
        );
int rdbi_version(
        rdbi_context_def *context
        );
/* function prototypes from rdbi.h */

void rdbi_msg_set_0( rdbi_context_def *context, int msg_num, char* default_msg );

void rdbi_msg_set_S( rdbi_context_def *context, int msg_num, char* default_msg, char* arg1 );

char * rdbi_vis_owner(
        rdbi_context_def *context
        );
char * rdbi_vndr_name(
        rdbi_context_def *context
        );

int rdbi_vndr_info (
        rdbi_context_def *context,
        rdbi_vndr_info_def *vndr_info
        );

int rdbi_geom_srid_set (
        rdbi_context_def *context,
        int sqlid,
        long srid );

int rdbi_geom_dimens_set (
        rdbi_context_def *context,
        int sqlid,
        int wantedDim );

int rdbi_get_geoms_ext (
        rdbi_context_def *context,
        char *table_name_I,
        char *geom_col_name_I,
        void **pGeom_O
        );

int rdbi_lob_create_ref(
        rdbi_context_def *context,
        int sqlid,
        void **lob_ref
        );

int rdbi_lob_destroy_ref(
        rdbi_context_def *context,
        int sqlid,
        void *lob_ref
        );

int rdbi_lob_get_size(
        rdbi_context_def *context,
        int sqlid,
        void *lob_ref,
        unsigned int *size);

int rdbi_lob_write_next(
        rdbi_context_def *context,
        int sqlid,
        void *lob_ref,
        int lob_type,
        unsigned int block_size,
        char* block
        );

int rdbi_lob_read_next(
        rdbi_context_def *context,
        int sqlid,
        void *lob_ref,
        int lob_type,
        unsigned int block_size,
        char *block,
        unsigned int *block_size_out,
        int *eol
        );

int rdbi_lob_open(
        rdbi_context_def *context,
        int sqlid,
        void *lob_ref,
        int for_read
        );

int rdbi_lob_close(
        rdbi_context_def *context,
        int sqlid,
        void *lob_ref
        );
int rdbi_get_gen_id(
        rdbi_context_def  *context,
	    char *table_name,
	    int  *id
	    );

int rdbi_set_schema (rdbi_context_def *context, char *schema_name);
int rdbi_est_cursor_obj(rdbi_context_def *context, int *sqlid);
int rdbi_init_context(rdbi_context_def *context);

int rdbi_run_sql (rdbi_context_def *context, char *sql, int isDDL, int *rows_processed);
int rdbi_run_sqlW (rdbi_context_def *context, wchar_t *sql, int isDDL, int *rows_processed);
int rdbi_autocommit_on (rdbi_context_def *context);
int rdbi_autocommit_off (rdbi_context_def *context);
int rdbi_autocommit_mode (rdbi_context_def *context);

#endif /* INC_RDBI_PROTO_H */


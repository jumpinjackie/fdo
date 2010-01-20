/*
 * Copyright (C) 2006 Refractions Research, Inc. 
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
 */

#include "stdafx.h"

#include "rdbi_init.h"
#include "bind.h"
#include "commit.h"
#include "connect.h"
#include "define.h"
#include "desc_bind.h"
#include "desc_slct.h"
#include "disconnect.h"
#include "est_cursor.h"
#include "execute.h"
#include "fetch.h"
#include "fre_cursor.h"
#include "geom_srid.h"
#include "get_gen_id.h"
#include "get_msg.h"
#include "null.h"
#include "run_sql.h"
#include "sql.h"
#include "term.h"
#include "vndr_info.h"
#include "vndr_name.h"

#include <string.h> // memset()
#include <assert.h>


int postgis_rdbi_init (void **contextp, rdbi_methods methods)
{
    postgis_context_def *context = NULL;
    int initialized = 0;
    int ret = RDBI_GENERIC_ERROR;

    context = (postgis_context_def *) malloc(sizeof(postgis_context_def));
    if (NULL == context)
    {
        ret = RDBI_MALLOC_FAILED;
    }
    else
    {
        memset (context, 0, sizeof(postgis_context_def));
        context->postgis_current_connect = -1;
		context->postgis_current_connect2 = -1;
        
        methods->close_cursor = NULL;
        methods->connect    = (int (*)(void*, const char*, const char*, const char*, char**, int*))postgis_connect;
        methods->disconnect = (int (*)(void*, char**))postgis_disconnect;
        methods->est_cursor = (int (*)(void*, char**))postgis_est_cursor;
        methods->est_cursor_obj = (int (*)(void*, char**))postgis_est_cursor_obj;
        methods->fre_cursor = (int (*)(void*, char**))postgis_fre_cursor;
        methods->sql        = (int (*)(void*, char*, const char*, int, char*, void*, char*))postgis_sql;
        methods->execute    = (int (*)(void*, char*, int, int, int*))postgis_execute;
        methods->exec_coc   = NULL;
        methods->define     = (int (*)(void*, char*, char*, int, int, char*, void*))postgis_define;
        methods->bind       = (int (*)(void*, char*, char*, int, int, char*, void*))postgis_bind;
        methods->fetch      = (int (*)(void*, char*, int, int, int, int*))postgis_fetch;
        methods->get_msg    = (void (*)(void*, char*))postgis_get_msg;
        methods->get_msgW   = (void (*)(void*, wchar_t*))postgis_get_msgW;
        methods->alcnullind = (int (*)(void*, int, char**))postgis_alloc_null_ind; 
        methods->set_null   = (void (*)(void*, void*, int, int))postgis_set_null;
        methods->set_nnull  = (void (*)(void*, void*, int, int))postgis_set_not_null;
        methods->is_null    = (int (*)(void*, void*, int))postgis_is_null;
        methods->desc_slct  = (int (*)(void*, char*, int, int, char*, int*, int*, int*))postgis_desc_slct;
        methods->desc_bind  = (int (*)(void*, char*, int, int, char*))postgis_desc_bind;
        methods->dbswitch   = NULL;
        methods->vndr_name  = (char*(*)(void*))postgis_vndr_name;
        methods->commit     = (int (*)(void*, int))postgis_commit;
        methods->col_act    = NULL;
        methods->col_get    = NULL;
        methods->col_deac   = NULL;
        methods->objects_act  = NULL;
        methods->objects_get  = NULL;
        methods->objects_deac = NULL;
        methods->users_act    = NULL;
        methods->users_get    = NULL;
        methods->users_deac   = NULL;
        methods->usr_exists   = NULL;
        methods->get_con_var  = NULL;
        methods->do_break     = NULL;
        methods->set_schema   = NULL;
        methods->set_schemaW  = NULL;
        methods->vndr_info    = (int (*)(void*, rdbi_vndr_info_def*))postgis_vndr_info;
        methods->geom_srid_set   = (int (*)(void*, char*, char*, long))postgis_geom_srid_set;
        methods->geom_dimens_set = NULL;
        methods->get_geoms_ext   = NULL;
        methods->lob_create_ref  = NULL;
        methods->lob_destroy_ref = NULL;
        methods->lob_get_size    = NULL;
        methods->lob_write_next  = NULL;
        methods->lob_read_next   = NULL;
        methods->lob_open        = NULL;
        methods->lob_close       = NULL;
        methods->term            = (int (*)(void*))postgis_term;
        methods->run_sql         = (int (*)(void*, const char*, int, int*))postgis_run_sql;
        methods->autocommit_on      = NULL;
		methods->autocommit_off     = NULL;
		methods->autocommit_mode    = NULL;
        methods->tran_begin         = (int (*)(void*))postgis_trans_begin;
        methods->capabilities.supports_sequence      = 1;
        methods->capabilities.supports_autoincrement = 0;
        methods->capabilities.supports_unicode       = 0;
        methods->capabilities.supports_int64_binding = 1;

        methods->get_gen_id = (int (*)(void*, const char*, /*const char*, */int*))postgis_get_gen_id;

        *contextp = context;
        ret = RDBI_SUCCESS;
    }

    return (ret);
}

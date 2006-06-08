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
 *
 */

#include "stdafx.h"

#include "rdbi_init.h"
#include "term.h"
#include "get_msg.h"
#include "vndr_name.h"
#include "vndr_info.h"
#include "connect.h"
#include "disconnect.h"
#include "est_cursor.h"
#include "fre_cursor.h"
#include "sql.h"
#include "execute.h"
#include "commit.h"
#include "fetch.h"
#include "define.h"
#include "bind.h"
#include "desc_slct.h"
#include "desc_bind.h"
#include "set_schema.h"
#include "run_sql.h"
#include "get_gen_id.h"
#include "null.h"

int mysql_rdbi_init (void **contextp, rdbi_methods methods)
{
    mysql_context_def *context;
    int initialized;
    int ret;

    context = (mysql_context_def *)malloc (sizeof (mysql_context_def));
    if ((mysql_context_def *)NULL == context)
        ret = RDBI_MALLOC_FAILED;
    else
    {
        memset (context, 0, sizeof (mysql_context_def));
        context->mysql_current_connect = -1;
		context->mysql_current_connect2 = -1;
        //initialized = mysql_library_init (-1, NULL, NULL); /* not embedded server */
        initialized = 1;
        if (0 == initialized)
        {
            free (context);
            ret = RDBI_GENERIC_ERROR;
        }
        else
        {
            methods->connect    = (int (*)(void*, char*, char*, char*, char**, int*))mysql_connect;
            methods->disconnect = (int (*)(void*, char**))mysql_disconnect;
            methods->est_cursor = (int (*)(void*, char**))mysql_est_cursor;
            methods->est_cursor_obj = (int (*)(void*, char**))mysql_est_cursor_obj;
            methods->fre_cursor = (int (*)(void*, char**))mysql_fre_cursor;
            methods->sql        = (int (*)(void*, char*, char*, int, char*, void*, char*))mysql_sql;
            methods->execute    = (int (*)(void*, char*, int, int, int*))mysql_execute;
            methods->exec_coc   = NULL;
            methods->define     = (int (*)(void*, char*, char*, int, int, char*, void*))mysql_define;
            methods->bind       = (int (*)(void*, char*, char*, int, int, char*, void*))mysql_bind;
            methods->fetch      = (int (*)(void*, char*, int, int, int, int*))mysql_fetch;
            methods->get_msg    = (void (*)(void*, char*))mysql_get_msg;
            methods->alcnullind = (int (*)(void*, int, char**))mysql_alcnullind; 
            methods->set_null   = (void (*)(void*, void*, int, int))mysql_set_null;
            methods->set_nnull  = (void (*)(void*, void*, int, int))mysql_set_nnull;
            methods->is_null    = (int (*)(void*, void*, int))mysql_is_null;
            methods->desc_slct  = (int (*)(void*, char*, int, int, char*, int*, int*, int*))mysql_desc_slct;
            methods->desc_bind  = (int (*)(void*, char*, int, int, char*))mysql_desc_bind;
            methods->dbswitch   = NULL;
            methods->vndr_name  = (char*(*)(void*))mysql_vndr_name;
            methods->commit     = (int (*)(void*, int))mysql_commitx;
            methods->col_act    = NULL;
            methods->col_get    = NULL;
            methods->col_deac   = NULL;
            methods->objects_act    = NULL;
            methods->objects_get    = NULL;
            methods->objects_deac   = NULL;
            methods->users_act    = NULL;
            methods->users_get    = NULL;
            methods->users_deac   = NULL;
            methods->usr_exists = NULL;
            methods->get_con_var = NULL;
            methods->do_break   = NULL;
            methods->set_schema = (int (*)(void*, char*))mysql_set_schema;
            methods->set_schemaW = NULL;
            methods->vndr_info  = (int (*)(void*, rdbi_vndr_info_def*))mysql_vndr_info;
            methods->geom_srid_set = NULL;
            methods->geom_dimens_set = NULL;
            methods->get_geoms_ext = NULL;
            methods->lob_create_ref = NULL;
            methods->lob_destroy_ref = NULL;
            methods->lob_get_size   = NULL;
            methods->lob_write_next = NULL;
            methods->lob_read_next  = NULL;
            methods->lob_open       = NULL;
            methods->lob_close      = NULL;
            methods->term           = (int (*)(void*))mysql_term;
            methods->run_sql        = (int (*)(void*, char*, int, int*))mysql_run_sql;
			methods->autocommit_on = NULL;
			methods->autocommit_off = NULL;
			methods->autocommit_mode = NULL;
            methods->capabilities.supports_sequence = 0;
            methods->capabilities.supports_autoincrement = 1;
			methods->capabilities.supports_unicode = 0;
            methods->capabilities.supports_int64_binding = 1;

            methods->get_gen_id     = (int (*)(void*, char*, int*))mysql_get_gen_id;

            *contextp = context;
            ret = RDBI_SUCCESS;
        }
    }

    return (ret);
}

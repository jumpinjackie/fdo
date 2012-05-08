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
#include "commit.h"
#include "fre_cursor.h"
#include "sql.h"
#include "conn_status.h"
#include "pgresult_clear.h"
#include "pgresult_error.h"
#include "pgresult_status.h"
#include "set_err_msg.h"
#include <stdlib.h> /* malloc(), free() */

// TODO: mloskot - To be removed for release.
#include <assert.h>

int postgis_sql (postgis_context_def *context,
                 char *cursor,
                 char *sql,
                 int defer,         /* not used */
                 char *verb,        /* not used */
                 void *ptree,       /* not used */
                 char *cursor_coc)  /* not used */
{
    postgis_cursor_def* curs = NULL;
    PGconn* postgis = NULL;
    PGresult* pgresult = NULL;
    int ret = RDBI_GENERIC_ERROR;
    const char* format = "DECLARE %s CURSOR FOR %s";
    assert(NULL != context);

    
    ret = postgis_conn_status(context);
    if (RDBI_SUCCESS == ret)
    {
        postgis = context->postgis_connections[context->postgis_current_connect];
        assert(NULL != postgis);

        if( strcmp(sql,"rollback") == 0 )
            context->postgis_in_transaction[context->postgis_current_connect] = -1;

        curs = (postgis_cursor_def*)cursor;
        if (NULL == curs)
            ret = RDBI_GENERIC_ERROR;
        else
        {
            assert(NULL != curs->stmt_name);

            postgis_fre_binds (curs);

            pgresult = PQprepare(postgis, curs->stmt_name,
                sql, curs->bind_count, NULL);
            if (NULL != pgresult)
            {
                ret = postgis_pgresult_status(pgresult);
                if (RDBI_SUCCESS != ret)
                {
                    postgis_pgresult_error(context, pgresult);
                    ret = RDBI_GENERIC_ERROR;
                }
                else if( strcmp(verb,"select") == 0 )
                {
                    postgis_trans_begin(context); // Postgres cursor requires a transaction
                    curs->sql = (char*)malloc(strlen(format)+strlen(sql)+strlen(curs->cursor_name)+1);
                    sprintf(curs->sql,format, curs->cursor_name, sql);
                    curs->isSelect = 1;/* true */
                }
            }
            else
            {
                /*
                * NULL indicates a fatal error like out-of-memory or
                * inability to send the command at all.
                */
                postgis_set_err_msg(context, PQerrorMessage(postgis));
                ret = RDBI_MALLOC_FAILED;
            }

            /*
            * Release PostgreSQL resources for query result.
            */
            postgis_pgresult_clear(pgresult);

        }
    }

    return ret;
}

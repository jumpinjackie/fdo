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

#include "define.h"
#include "type.h"
#include "pgconn_status.h"
#include "pgresult_clear.h"
#include "pgresult_status.h"
#include <limits.h> /* LONG_MIN, LONG_MAX */
#include <stdlib.h>
#include <string.h> /* memset() */
#include <assert.h>


int postgis_define (
    postgis_context_def *context,
    char *cursor,
    char *name,
    int datatype,
    int size,
    char *address,
    short *null_ind)
{
    postgis_cursor_def *curs;
    PGconn* postgis = NULL;
    PGresult* pgres = NULL;
    PGSQL_BIND* defines = NULL;
    int nfields = 0;
    Oid typeoid = 0;
    int index = 0;
    int ret = RDBI_GENERIC_ERROR;

    assert(NULL != context);

    if (-1 == context->postgis_current_connect)
        ret = RDBI_NOT_CONNECTED;
    else
    {
        postgis = context->postgis_connections[context->postgis_current_connect];

        ret = postgis_pgconn_status(postgis);
        if (RDBI_SUCCESS == ret)
        {
            curs = (postgis_cursor_def*)cursor;
            if (NULL == curs)
                ret = RDBI_GENERIC_ERROR;
            else
            {
                if (NULL != curs->stmt_name)
                {
                    pgres = PQdescribePrepared(postgis, curs->stmt_name);
                    if (RDBI_SUCCESS != postgis_pgresult_status(pgres))
                        ret = RDBI_GENERIC_ERROR; // TODO: mloskot - Find better error code
                    else
                    {
                        nfields = PQnfields(pgres);

                        /* Allocate array of defines if it hasn't been already allocated */
                        if (NULL == curs->defines)
                        {
                            defines = (PGSQL_BIND*)malloc(sizeof(PGSQL_BIND) * nfields);
                            if (NULL != defines)
                            {
                                memset(defines, 0, sizeof(PGSQL_BIND) * nfields);
                                curs->defines = defines;
                                curs->define_count = nfields;
                            }
                        }

                        /* Initialize defines */
                        if (NULL == curs->defines)
                            ret = RDBI_INVLD_DESCR_OBJTYPE;
                        else
                        {
                            assert(nfields == curs->define_count);

                            /* Check if numeric position */
                            index = strtol(name, NULL, 10);
                            if (0 == index || LONG_MIN == index || LONG_MAX == index)
                            {
                                /* If non-index passed, try to find column by-name */
                                index = PQfnumber(pgres, name);
                            }

                            if (0 >= index)
                            {
                                /* Invalid column index passed or PQfname can't find it by name */
                                ret = RDBI_NO_SUCH_COLUMN;
                            }
                            else
                            {
                                index--; /* Make column index zero-based */

                                typeoid = rdbi_to_postgis_type(postgis, datatype, size, 0);
                                if (0 >= typeoid)
                                    ret = RDBI_INVLD_DESCR_OBJTYPE;
                                else
                                {
                                    curs->defines[index].buffer_type = typeoid;
                                    curs->defines[index].buffer_length = size;
                                    curs->defines[index].buffer = address;
                                    if ((short*)NULL != null_ind)
                                    {
                                        curs->defines[index].is_null = (char*)null_ind;
                                    }
                                    ret = RDBI_SUCCESS;
                                }
                            }
                        }
                    }

                    postgis_pgresult_clear(pgres);
                } /* if (NULL != curs->stmt_name) */
            }
        }
    }

    return (ret);
}

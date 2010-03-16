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

#include "digits.h"
#include "errno.h"
#include "execute.h"
#include "pgconn_status.h"
#include "pgresult_clear.h"
#include "pgresult_error.h"
#include "pgresult_status.h"
#include "null.h"

#include "Geometry.h"

#include <limits.h>
#include <stdlib.h> /* malloc(), free() */
#include <string.h> /* memset(), strcpy() */

#include <assert.h>


int postgis_execute (
    postgis_context_def *context,
    char *cursor,
    int count,
    int offset,
    int *rows_processed)
{
    PGconn* postgis = NULL;
    postgis_cursor_def* curs = NULL;

    char** param_values = NULL;
    int params_count = 0;

    char* buf = NULL;
    size_t buf_size = 0;

    int rows = 0;
    int ret = RDBI_GENERIC_ERROR;


    if (NULL != rows_processed)
        *rows_processed = 0;

    if (-1 == context->postgis_current_connect)
        ret = RDBI_NOT_CONNECTED;
    else
    {
        postgis = context->postgis_connections[context->postgis_current_connect];
        if (RDBI_SUCCESS != postgis_pgconn_status(postgis))
            ret = RDBI_NOT_CONNECTED;
        else
        {
            curs = (postgis_cursor_def*)cursor;
            if (NULL != curs)
            {
                assert (NULL != curs->stmt_name);

                params_count = 0; /* Important, if binds is NULL. */

                /*
                * Prepare input parameters for statement.
                */
                if (NULL != curs->binds)
                {
                    params_count = curs->bind_count;
                    param_values = (char**)malloc(sizeof(char*) * params_count);
                    if (NULL == param_values)
                        ret = RDBI_MALLOC_FAILED;
                    else
                    {
                        memset(param_values, 0, sizeof(*param_values) * params_count);

                        for (int i = 0; i < params_count; ++i)
                        {
                            if ( curs->binds[i].is_null && postgis_is_null(context, curs->binds[i].is_null, 0) ) {
                                buf = NULL;
                                buf_size = 0;
                            }
                            else {
                                switch (curs->binds[i].buffer_type)
                                {
                                case PGSQL_TYPE_CHAR:  /* TODO: mloskot - check if not move to separate case */
                                case PGSQL_TYPE_BPCHAR:
                                case PGSQL_TYPE_VARCHAR:
                                case PGSQL_TYPE_TEXT:
                                    {
                                        buf = NULL;
                                        buf_size = curs->binds[i].buffer_length * sizeof(char);
                                        if( buf_size > 0 )
                                        {
                                            buf = (char*)malloc(buf_size);
                                            memset(buf, 0, buf_size);
                                            strncpy(buf, (const char*)curs->binds[i].buffer, buf_size);
                                        }

                                    }
                                    break;
                                case PGSQL_TYPE_BOOL:
                                    {
                                        buf_size = 6;
                                        buf = (char*)malloc(buf_size);
                                        memset(buf, 0, buf_size);
                                        snprintf(buf, buf_size, "%s", ((int)(*((short*)curs->binds[i].buffer)))?"TRUE":"FALSE");
                                    }
                                    break;
                                case PGSQL_TYPE_INT2:
                                    {
                                        buf_size = NUMBER_OF_DIGITS10(short);
                                        buf = (char*)malloc(buf_size);
                                        memset(buf, 0, buf_size);
                                        snprintf(buf, buf_size, "%d", (int)(*((short*)curs->binds[i].buffer)));
                                    }
                                    break;
                                case PGSQL_TYPE_INT4:
                                    {
                                        buf_size = NUMBER_OF_DIGITS10(int);
                                        buf = (char*)malloc(buf_size);
                                        memset(buf, 0, buf_size);
                                        snprintf(buf, buf_size, "%d", (*((int*)curs->binds[i].buffer)));
                                    }
                                    break;
                                case PGSQL_TYPE_INT8:
                                    {
                                        buf_size = NUMBER_OF_DIGITS10(rdbi_int64_t);
                                        buf = (char*)malloc(buf_size);
                                        memset(buf, 0, buf_size);
                                        snprintf(buf, buf_size, "%I64d", (*((rdbi_int64_t*)curs->binds[i].buffer)));
                                    }
                                    break;
                                case PGSQL_TYPE_FLOAT:
                                    {
                                        buf_size = NUMBER_OF_FLOAT_DIGITS;
                                        buf = (char*)malloc(buf_size);
                                        memset(buf, 0, buf_size);
                                        snprintf(buf, buf_size, "%.20g", (*((float*)curs->binds[i].buffer)));
                                    }
                                    break;
                                case PGSQL_TYPE_DOUBLE:
                                    {
                                        buf_size = NUMBER_OF_DOUBLE_DIGITS;
                                        buf = (char*)malloc(buf_size);
                                        memset(buf, 0, buf_size);
                                        snprintf(buf, buf_size, "%.20g", (*((double*)curs->binds[i].buffer)));
                                    }
                                    break;
                                default:
                                    if (postgis_get_geometry_oid(postgis) == curs->binds[i].buffer_type) {
                                        int count = 0;
                                        //TODO - handle 3D geometries
                                        char* wkb = WkbFromGeometry( (*(void**)(curs->binds[i].buffer)), &count);
                                        if ( curs->srids[i] > 0 ) 
                                            *((int*)&wkb[5]) |= 0x20000000; // Add hasSRID flag.
                                        buf = (char*)malloc((count * 2) + 10);
                                        int j;
                                        char* p = buf;
                                        for ( j = 4; j < count; j++ ) {
                                            if ( (j == 9) && (curs->srids[i] > 0) ) {
                                                unsigned char * sc = (unsigned char*) &(curs->srids[i]);
                                                for ( int k = 0; k < 4; k++ ) {
                                                    sprintf( p, "%02X", sc[k] );
                                                    p += 2;
                                                }
                                            }
                                            sprintf( p, "%02X", (unsigned char)(wkb[j]) );
                                            p += 2;
                                        }
                                    }
                                    else {
                                        assert(!"execute: NOT IMPLEMENTED TYPE");
                                    }
                                }
                            }

                            /* Assign value to input parameter. */
                            param_values[i] = buf;

                            buf = NULL;
                            buf_size = 0;
                        }
                    }
                }

                /*
                * Execute prepared statement.
                */
                if( curs->isSelect && curs->sql != NULL )
                    curs->stmt_result = PQexecParams( postgis, curs->sql, params_count,NULL, param_values,NULL,NULL,0);
                else
                    curs->stmt_result = PQexecPrepared(postgis, curs->stmt_name,
                                                   params_count, param_values,
                                                   NULL, NULL, 0);

                ret = postgis_pgresult_status(curs->stmt_result);
                if (RDBI_SUCCESS != ret)
                {
                    postgis_pgresult_error(context, curs->stmt_result);
                    postgis_pgresult_clear(curs->stmt_result);
                    curs->stmt_result = NULL;
                }
                else
                {
                    if (NULL != rows_processed)
                    {
                        if (PGRES_TUPLES_OK == PQresultStatus(curs->stmt_result))
                            // # of rows retrieved by executing select command.
                            *rows_processed = PQntuples(curs->stmt_result);
                        else if (PGRES_COMMAND_OK == PQresultStatus(curs->stmt_result))
                            // # of rows affected by update or delete command.
                            *rows_processed = strtol(PQcmdTuples(curs->stmt_result),NULL,10);
                    }
                }

                /*
                * Clean input parameters.
                */
                if (NULL != param_values)
                {
                    for (int i = 0; i < params_count; ++i)
                    {
                        free(param_values[i]);
                        param_values[i] = NULL;
                    }

                    free(param_values);
                    param_values = NULL;
                }
            }
        }
    }

    return (ret);
}

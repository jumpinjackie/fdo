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

#include "fetch.h"
#include "local.h"
#include "conn_status.h"
#include "pgresult_status.h"
#include "pgresult_error.h"
#include "pgresult_clear.h"
#include "Geometry.h"
#include <stdlib.h> /* strtol() */
#include <string.h> /* memcpy(), strncpy() */
#include <assert.h>
#include "set_err_msg.h"
#include "commit.h"

#ifdef _WIN32
#include <malloc.h> /* alloca() */
#else
#include<alloca.h>
#endif

int postgis_fetch (postgis_context_def *context,
                   char *cursor,
                   int count,
                   int do_exec,
                   int do_cancel,
                   int *rows_processed)
{
    PGconn* postgis = NULL;
    postgis_cursor_def *curs = NULL;
    char* buffer = NULL;
    char* fvalue = NULL;
    size_t fvalue_length = 0;
    int nrows = 0;
    int nfields = 0;
    int ret = RDBI_GENERIC_ERROR;

    if (NULL != rows_processed)
        *rows_processed = 0;

    ret = postgis_conn_status(context);
    if (RDBI_SUCCESS == ret)
    {
        postgis = context->postgis_connections[context->postgis_current_connect];
        assert(NULL != postgis);

        curs = (postgis_cursor_def*)cursor;
        if (NULL == curs)
            ret = RDBI_GENERIC_ERROR;
        else
        {
            if( curs->isSelect && curs->sql != NULL )
            {
                const char* sqlFormat = "FETCH %d FROM %s";
                if( curs->current_row == -1 )
                {
                    int length = sizeof(sqlFormat)+strlen(curs->cursor_name)+POSTGIS_STMT_NAME_SIZE+1;
                    char* sql = (char*)alloca(length);
                    sprintf(sql,sqlFormat,POSTGIS_FETCH_SIZE,curs->cursor_name);
                    postgis_pgresult_clear(curs->stmt_result);
                    curs->stmt_result = NULL;
                    curs->stmt_result = PQexec(postgis, sql);
                }
                else 
                {
                    nrows = PQntuples(curs->stmt_result);
                    if ((curs->current_row+1) >= nrows && nrows >= POSTGIS_FETCH_SIZE )
                    {
                        int length = sizeof(sqlFormat)+strlen(curs->cursor_name)+POSTGIS_STMT_NAME_SIZE+1;
                        char* sql = (char*)alloca(length);
                        sprintf(sql,sqlFormat,POSTGIS_FETCH_SIZE,curs->cursor_name);
                        postgis_pgresult_clear(curs->stmt_result);
                        curs->stmt_result = NULL;
                        curs->stmt_result = PQexec(postgis, sql);
                        curs->current_row = -1;
                    }
                }
            }
            if (NULL != curs->stmt_name
                && NULL != curs->defines
                && PGRES_TUPLES_OK == PQresultStatus(curs->stmt_result))
            {
                /* Move cursor to next row. */
                curs->current_row++;

                nrows = PQntuples(curs->stmt_result);
                if (curs->current_row >= nrows)
                {
                    /* End of query result. */
                    curs->current_row = -1;
                    ret = RDBI_END_OF_FETCH;
                }
                else
                {
                    /* Fetch data into buffers in defines. */
                    const int nfields = PQnfields(curs->stmt_result);

                    /* Pre-condition assures the state of RDBI machinery. */
                    assert(nfields == curs->define_count);

                    for (int i = 0; i < nfields; ++i)
                    {
                         
                        curs->defines[i].is_null[0] = PQgetisnull(curs->stmt_result, curs->current_row, i);
                        if(1 == curs->defines[i].is_null[0] ) 
                            continue;

                        /* Read field value */
                        fvalue = PQgetvalue(curs->stmt_result, curs->current_row, i);
                        assert(NULL != curs->defines[i].buffer);

                        /* Convert value type and safe to define buffer. */
                        switch(curs->defines[i].buffer_type)
                        {
                        case PGSQL_TYPE_CHAR:
                        case PGSQL_TYPE_BPCHAR:
                        case PGSQL_TYPE_VARCHAR:
                        case PGSQL_TYPE_TEXT:
                        case PGSQL_TYPE_NAME:
                            {
                                assert((strlen(fvalue) + 1) <= curs->defines[i].buffer_length);

                                fvalue_length = strlen(fvalue);
                                buffer = (char*)curs->defines[i].buffer;
                                strncpy(buffer, fvalue, fvalue_length);
                                buffer[fvalue_length] = '\0';
                                break;
                            }
                        case PGSQL_TYPE_BOOL:
                            {
                                assert(sizeof(short) == curs->defines[i].buffer_length);

                                short val = (fvalue[0] == 't') ? 1 : 0;
                                memcpy(curs->defines[i].buffer, &val, sizeof(short));
                            }
                            break;
                        case PGSQL_TYPE_INT2:
                            {
                                assert(sizeof(short) == curs->defines[i].buffer_length);

                                short val = (short)strtol(fvalue, NULL, 10);
                                memcpy(curs->defines[i].buffer, &val, sizeof(short));
                            }
                            break;
                        case PGSQL_TYPE_INT4:
                            {
                                assert(sizeof(int) == curs->defines[i].buffer_length);

                                int val = (int)strtol(fvalue, NULL, 10);
                                memcpy(curs->defines[i].buffer, &val, sizeof(int));
                            }
                            break;
                        case PGSQL_TYPE_INT8:
                            {
                                assert(sizeof(rdbi_int64_t) == curs->defines[i].buffer_length);
#ifdef _WIN32
                                rdbi_int64_t val = (rdbi_int64_t)_strtoi64(fvalue, NULL, 10);
#else
                                rdbi_int64_t val = (rdbi_int64_t)strtoll(fvalue, NULL, 10);
#endif
                                size_t type_size = sizeof(rdbi_int64_t);
                                memcpy(curs->defines[i].buffer, &val, type_size);
                            }
                            break;
                        case PGSQL_TYPE_FLOAT:
                            {
                                assert(sizeof(float) == curs->defines[i].buffer_length);

                                float val = (float)strtod(fvalue, NULL);
                                memcpy(curs->defines[i].buffer, &val, sizeof(float));
                            }
                            break;
                        case PGSQL_TYPE_DOUBLE:
                            {
                                assert(sizeof(double) == curs->defines[i].buffer_length);

                                double val = (double)strtod(fvalue, NULL);
                                memcpy(curs->defines[i].buffer, &val, sizeof(double));
                            }
                            break;
                        case PGSQL_TYPE_BLOB:
                            break;
                        case PGSQL_TYPE_DECIMAL:
                            {
                                assert(sizeof(double) == curs->defines[i].buffer_length);

                                double val = (double)strtod(fvalue, NULL);
                                memcpy(curs->defines[i].buffer, &val, sizeof(double));
                            }
                            break;

                        default:
                            if ( -1 == curs->geometry_oid )
                                curs->geometry_oid = postgis_get_geometry_oid(postgis);

                            if ( (curs->geometry_oid != -1) && (curs->geometry_oid == curs->defines[i].buffer_type) ) {
                                FreeGeometry( curs->defines[i].geometry );
                                int defcount = strlen(fvalue);
                                char* buffer = new char[(size_t)(defcount/2)];
                                int j;
                                int k = 0;
                                for ( j = 0; j < defcount; j+=2 ) {
                                    buffer[k++] = ((fvalue[j] - (fvalue[j] >= 'A' ? ('A' - 10) : '0')) * 16) + (fvalue[j+1] - (fvalue[j+1] >= 'A' ? ('A' - 10) : '0'));
                                }

                                curs->defines[i].geometry = GeometryFromExtendedWkb ( buffer, k );
                                memcpy(curs->defines[i].buffer, &(curs->defines[i].geometry), sizeof(void*));

                                delete[] buffer;
                            }
                            else {
                                assert(!"fetch: NOT IMPLEMENTED TYPE");
                            }
                        }

                        buffer = NULL;
                        fvalue_length = 0;
                    }

                    if (NULL != rows_processed)
                        *rows_processed = 1;

                    ret = RDBI_SUCCESS;
                }
            }
            else if (NULL != curs->stmt_name
                     && PGRES_COMMAND_OK == PQresultStatus(curs->stmt_result))
            {
                curs->current_row = -1;
                ret = RDBI_END_OF_FETCH;
            }
            else
            {
                if( curs->isSelect && curs->sql != NULL )
                {
                    // Make sure to rollback the transaction that we started for this cursor. Otherwise the server
                    // will go in an out of synch state.
                    PGresult* pgresult = PQexec(postgis, "rollback");
                    postgis_pgresult_clear(pgresult);
                    context->postgis_in_transaction[context->postgis_current_connect] = -1;
                    curs->isSelect = 0; // prevents a possible commit.
                }
                postgis_set_err_msg(context, PQresultErrorMessage(curs->stmt_result));
                ret = RDBI_GENERIC_ERROR;
            }
        }
    }

    return ret;
}

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
#include "Fdo.h"

#include "est_cursor.h"
#include "pgconn_status.h"

#include <stdlib.h> // malloc()
#include <string.h> // memset()
#include <assert.h>

static FdoInt64 MaxCursorId = 9223372036854775807LL;

#ifdef _WIN32
#include <windows.h>
static CRITICAL_SECTION _mutex;
static int mutexInitialized = 0;
static FdoInt64 getNextCursorId();
static FdoInt64  _nextCursorId = getNextCursorId();
#else
#include <pthread.h>
pthread_mutex_t _mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
static int64_t _nextCursorId = 0;
static int mutexInitialized = 1;
#endif

static FdoInt64 getNextCursorId()
{
    FdoInt64    ret = 0;
    if( ! mutexInitialized )
    {
#ifdef _WIN32
        InitializeCriticalSection(&_mutex);
        mutexInitialized = 1;
#endif
    }
#ifdef _WIN32
    EnterCriticalSection(&_mutex);
#else
    pthread_mutex_lock(&_mutex);
#endif
    if( _nextCursorId >= MaxCursorId )
        _nextCursorId = 0;
    ret = (_nextCursorId++);
#ifdef _WIN32
    LeaveCriticalSection(&_mutex);
#else
    pthread_mutex_unlock(&_mutex);
#endif
    return ret;
}

int postgis_est_cursor (postgis_context_def *context, char **cursor)
{
    return (postgis_est_cursor_obj (context, cursor));
}

int postgis_est_cursor_obj (postgis_context_def *context, char **cursor)
{
    PGconn* postgis = NULL;
    postgis_cursor_def* curs = NULL;
    int ret = RDBI_GENERIC_ERROR;

    assert(NULL != context);

    if (-1 == context->postgis_current_connect)
    {
        ret = RDBI_NOT_CONNECTED;
    }
    else
    {
        postgis = context->postgis_connections[context->postgis_current_connect];
        if (RDBI_SUCCESS != postgis_pgconn_status(postgis))
            ret = RDBI_NOT_CONNECTED;
        else
        {
            // PostgreSQL prepared statements
            // does not need any pre-initialization steps,
            // statement allocation, etc.
            // So, we only allocates cursor area.

            curs = (postgis_cursor_def*)malloc (sizeof (postgis_cursor_def));
            if (NULL == curs)
            {
                return RDBI_MALLOC_FAILED;
            }

            memset (curs, 0, sizeof (postgis_cursor_def));

            // The stmt_name may be "" to create an unnamed statement,
            // in which case any pre-existing unnamed statement is
            // automatically replaced.It is an error if the statement name is
            // already defined in the current session.

            sprintf (curs->stmt_name, "fdo%x",(unsigned long)&(curs->stmt_name));
#ifdef _WIN32
            sprintf (curs->cursor_name, "fdo%I64d",getNextCursorId());
#else
            sprintf (curs->cursor_name, "fdo%lld",getNextCursorId());
#endif

            /* Initialize cursor members. */
            curs->sql = NULL;
            curs->stmt_result = NULL;
            curs->current_row = POSTGIS_STMT_NOROWS; /* = -1 */
            curs->define_count = 0;
            curs->defines = NULL;
            curs->bind_count = 0;
            curs->binds = NULL;
            curs->redefines = NULL;
            curs->rebinds = NULL;
            curs->geometry_oid = -1;
            curs->isSelect = 0;
            *cursor = (char*)curs;
            ret = RDBI_SUCCESS;
        }
    }

    return (ret);
}

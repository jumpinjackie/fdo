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

#include "get_msg.h"
#include <string.h> /* memset() */
#include <wchar.h>
#include <assert.h>

int postgis_get_msg (postgis_context_def *context, char *buffer)
{
    PGconn* postgis = NULL;
    const char* msg = NULL;
    int ret = RDBI_GENERIC_ERROR;

    assert(NULL != buffer);
    memset(buffer, '\0', RDBI_MSG_SIZE);

    if (-1 == context->postgis_current_connect)
    {
        strncpy (buffer, context->postgis_last_err_msg, RDBI_MSG_SIZE);
        buffer[RDBI_MSG_SIZE - 1] = '\0'; 
        ret = RDBI_SUCCESS;
    }
    else if (0 != context->postgis_last_err_msg[0])
    {
        strncpy (buffer, context->postgis_last_err_msg, RDBI_MSG_SIZE);
        buffer[RDBI_MSG_SIZE - 1] = '\0';
        ret = RDBI_SUCCESS;
    }
    else
    {
        postgis = context->postgis_connections[context->postgis_current_connect];
        msg = PQerrorMessage(postgis);

        assert(NULL != msg);

        // TODO: Is it reasonable to check strlen(err) <= 0
        // and return error code?

        strncpy (buffer, msg, RDBI_MSG_SIZE);
        buffer[RDBI_MSG_SIZE - 1] = '\0';
        ret = RDBI_SUCCESS;
    }

    return ret;
}

int postgis_get_msgW (postgis_context_def *context, wchar_t *buffer)
{
    PGconn* postgis = NULL;
    const char* msg = NULL;
    int ret = RDBI_GENERIC_ERROR;

    assert(NULL != buffer);
    memset(buffer, '\0', RDBI_MSG_SIZE);

    /*
     * TODO: mloskot - last_err_msg store Unicode
     */

    assert(!"postgis_get_msgW() - Not Implemented");

    if (-1 == context->postgis_current_connect)
    {
        //wcscpy (buffer, context->postgis_last_err_msg);
        ret = RDBI_SUCCESS;
    }
    else if ( 0 != context->postgis_last_err_msg[0] ) 
    {
        //wcscpy (buffer, context->postgis_last_err_msg);
        ret = RDBI_SUCCESS;
    }
    else
    {
        postgis = context->postgis_connections[context->postgis_current_connect];
        msg = PQerrorMessage(postgis);

        swprintf (buffer, RDBI_MSG_SIZE, L"%hs", msg);
        buffer[RDBI_MSG_SIZE - 1] = '\0'; 
        ret = RDBI_SUCCESS;
    }

    return ret;
}

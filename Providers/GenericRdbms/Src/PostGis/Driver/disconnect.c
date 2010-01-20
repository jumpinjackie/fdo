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

#include "disconnect.h"
#include <assert.h>

int postgis_disconnect(postgis_context_def *context)
{
    PGconn* conn = NULL;
    int index = 0;
    int conn_ids[2];
    int ret = RDBI_GENERIC_ERROR;

    assert(NULL != context);

    if (-1 == context->postgis_current_connect)
    {
        ret = RDBI_NOT_CONNECTED;
    }
    else
    {
        /*
         * It seems connection IS open.
         */

        conn_ids[0] = context->postgis_current_connect;
        conn_ids[1] = context->postgis_current_connect2;
        for(int i = 0; i<2; i++ )
        {
            index = conn_ids[i];
            conn  = context->postgis_connections[index];

            if (NULL != conn)
            {
                /*
                 * If connection attempts occurred, conn is ALWAYS
                 * returned as NOT NULL from libpq connecting functions.
                 */

                if (CONNECTION_OK == PQstatus(conn))
                {
                    /* Close 1st connection. */
                    PQfinish(conn);
                    conn = NULL;

                    /* Remove 1st connection from sockets table. */
                    context->postgis_connections[index] = NULL;
                    context->postgis_connect_count--;

                    ret = RDBI_SUCCESS;
                }
                else
                {
                    PQfinish(conn); /* In case of connection attempt failed. */
                    conn = NULL;
                    
                    ret = RDBI_NOT_CONNECTED;
                }
            }
            else
            {
                /*
                 * NEVER SHOULD GET HERE
                 *
                 * If current connection is present in current context
                 * but the connection pointer is NULL, this situation likely
                 * means there is a serious BUG.
                 */
                assert(false);

                return RDBI_GENERIC_ERROR;
            }
        }
    }

    context->postgis_current_connect = -1;
    context->postgis_current_connect2 = -1;
    return (ret);
}

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

#include "conn_status.h"
#include "pgconn_status.h"
#include "local.h"
#include <assert.h>

int postgis_conn_status(postgis_context_def *context)
{
    PGconn* conn = NULL;
    int index = -1;
    int ret = RDBI_GENERIC_ERROR;
    
    if (NULL != context)
    {
        /*
         * TODO: mloskot - Is secondary connection mandatory?
         */

        if (-1 == context->postgis_current_connect
            || -1 == context->postgis_current_connect2)
        {
            ret = RDBI_NOT_CONNECTED;
        }
        else
        {
            /*
             * Check primary connection.
             */
            index = context->postgis_current_connect;
            conn  = context->postgis_connections[index];
 
            ret = postgis_pgconn_status(conn);
            if (RDBI_SUCCESS == ret)
            {
                /*
                * Check secondary connection.
                */
                index = context->postgis_current_connect2;
                conn  = context->postgis_connections[index];

                ret = postgis_pgconn_status(conn);
            }
        }
    }
    
    assert(RDBI_SUCCESS == ret
           || RDBI_NOT_CONNECTED == ret
           || RDBI_GENERIC_ERROR == ret);

    return ret;
}

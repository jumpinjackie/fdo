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

#include "pgconn_status.h"
#include <Inc/rdbi.h>
#include <libpq-fe.h>
#include <assert.h>

int postgis_pgconn_status (PGconn* conn)
{
    int ret = RDBI_GENERIC_ERROR;

    if (NULL != conn)
    {
        if (CONNECTION_OK != PQstatus(conn))
        {
            /* Try error recovery if a working connection is lost. */
            ret = RDBI_SUCCESS;
            PQreset(conn);

            if (CONNECTION_OK != PQstatus(conn))
            {
                ret = RDBI_NOT_CONNECTED;
            }
        }
        else
        {
            ret = RDBI_SUCCESS;
        }
    }

    return ret;
}

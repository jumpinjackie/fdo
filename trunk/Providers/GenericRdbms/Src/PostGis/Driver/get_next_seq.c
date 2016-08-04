/*
 * Copyright (C) 2010 Refractions Research, Inc. 
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

#include "get_next_seq.h"
#include "conn_status.h"
#include "pgresult_clear.h"
#include <libpq-fe.h>

#include <assert.h>
#include <limits.h> /* LONG_MIN, LONG_MAX */
#include <stdlib.h> /* strtol */

int postgis_get_next_seq(
    postgis_context_def* context,
	const char* sequence_name,
	long* id)
{
    int ret = RDBI_GENERIC_ERROR;
    PGconn* conn;
    PGresult* pgresult = NULL; 
    const char* fmt = "SELECT nextval('%s')";
    char stmt[512];

    assert(NULL != context);
    assert(NULL != sequence_name);
    assert(NULL != id);

    if (RDBI_SUCCESS == postgis_conn_status(context))
    {
        conn = context->postgis_connections[context->postgis_current_connect];
        assert(NULL != conn);

        sprintf( stmt, fmt, sequence_name );
        pgresult = PQexec(conn, stmt);
        if (NULL != pgresult)
        {
            if (PGRES_TUPLES_OK == PQresultStatus(pgresult) && PQntuples(pgresult) > 0)
            {
                (*id) = strtol(PQgetvalue(pgresult, 0, 0), NULL, 10);

                if (LONG_MIN == (*id) || LONG_MAX == (*id) || 0 == (*id))
                {
                    /* Report if value overflow occured or no conversion was performed */

                    /*
                     * TODO - print error
                     */
                    assert(!"strtol value overflow");
                }
            }
        }

        postgis_pgresult_clear(pgresult);

        ret = RDBI_SUCCESS;
    }

    return ret;
}

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

#include "run_sql.h"
#include "commit.h"
#include "conn_status.h"
#include "pgconn_status.h"
#include "pgresult_status.h"
#include "pgresult_clear.h"
#include "set_err_msg.h"

#include <stdlib.h> /* strtol */
#include <errno.h>  /* ERANGE, EINVAL */
#include <assert.h>

int postgis_run_sql(
    postgis_context_def *context,
    char* sql,
	int isddl,
    int* rows_processed
)
{
    PGconn* postgis = NULL;
    PGresult* pgresult = NULL;
    char* buf = NULL;
    long conn_index = 0;
    long rows = 0;
    int ret = RDBI_GENERIC_ERROR;

    assert(NULL != context);
    assert(NULL != sql);

    ret = postgis_conn_status(context);
    if (RDBI_SUCCESS == ret)
    {
		if (isddl > 0)
        {
            // Commit all transactions before issuing a DDL statement; Otherwise the server may block if a transaction is open
            // and accessing a table that may be modified by the DDL statement.
            if( context->postgis_in_transaction[context->postgis_current_connect] >= 0 )
            {
                (void)PQexec(context->postgis_connections[context->postgis_current_connect], "COMMIT");  
                context->postgis_in_transaction[context->postgis_current_connect] = -1;
            }
            if( context->postgis_in_transaction[context->postgis_current_connect2] >= 0 )
            {
                (void)PQexec(context->postgis_connections[context->postgis_current_connect2], "COMMIT");  
                context->postgis_in_transaction[context->postgis_current_connect2] = -1;
            }
            conn_index = context->postgis_current_connect2;
        }
		else
			conn_index = context->postgis_current_connect;

        postgis = context->postgis_connections[conn_index];
        assert(NULL != postgis);

        ret = postgis_pgconn_status(postgis);
        if (RDBI_SUCCESS == ret)
        {
            /* SQL statement execution */
            pgresult = PQexec(postgis, sql);

            ret = postgis_pgresult_status(pgresult);
            if (RDBI_SUCCESS == ret)
            {
                if (NULL != rows_processed)
                {
                    rows = 0;
                    if (PGRES_TUPLES_OK == PQresultStatus(pgresult))
                    {
                        buf = PQcmdTuples(pgresult);
                        rows = strtol(buf, NULL, 10);
// BR - TODO - recheck if needed.
//                        assert(ERANGE != errno && EINVAL != errno);
                    }

                    (*rows_processed) = (int)rows;
                }
            }
            else
            {
                /* Retrieve error message associated with last command. */
                buf = PQresultErrorMessage(pgresult);

                postgis_set_err_msg(context, buf);

                ret = RDBI_RESOURCE_LOCK;
            }
            /* Clear PostgreSQL resources. */
            postgis_pgresult_clear(pgresult);
        }
    }

    return (ret);
}

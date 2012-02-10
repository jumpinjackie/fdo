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

#include "commit.h"
#include "pgconn_status.h"
#include "pgresult_clear.h"
#include "pgresult_status.h"
#include <assert.h>

int postgis_commit (
    postgis_context_def *context,
    int commit_action)
{
    PGconn* postgis = NULL;
    PGresult* pgresult = NULL;
    int ret = RDBI_GENERIC_ERROR;

    assert(NULL != context);

    if (-1 == context->postgis_current_connect)
    {
        ret = RDBI_NOT_CONNECTED;
    }
    else if ( context->postgis_in_transaction[context->postgis_current_connect] == -1 )
    {
        // no transaction to commit. This can happen if a rollback or a DDL statement was excuted in which case the transaction was ended.
        return RDBI_SUCCESS;
    }
    else
    {
        postgis = context->postgis_connections[context->postgis_current_connect];       
        if (RDBI_SUCCESS == postgis_pgconn_status(postgis))
        {
            context->postgis_in_transaction[context->postgis_current_connect]--;
            if( context->postgis_in_transaction[context->postgis_current_connect] >= 0 )
                ret = RDBI_SUCCESS;
            else 
            {
                pgresult = PQexec(postgis, "COMMIT");                   
                if (RDBI_SUCCESS != postgis_pgresult_status(pgresult))
                {
                    ret = RDBI_GENERIC_ERROR;
                }
                else
                {
                    context->postgis_in_transaction[context->postgis_current_connect] = -1;
                    ret = RDBI_SUCCESS;
                }

                postgis_pgresult_clear(pgresult);
            }
        }
    }
    return (ret);
}

int postgis_trans_begin(
    postgis_context_def *context
	)
{
    PGconn* postgis = NULL;
    PGresult* pgresult = NULL;
    int ret = RDBI_GENERIC_ERROR;

    assert(NULL != context);

    if (-1 == context->postgis_current_connect)
    {
        ret = RDBI_NOT_CONNECTED;
    }
    else
    {
        postgis = context->postgis_connections[context->postgis_current_connect];       
        if (RDBI_SUCCESS == postgis_pgconn_status(postgis) )
        {
            if( context->postgis_in_transaction[context->postgis_current_connect] != -1 )
            {
                ret = RDBI_SUCCESS; // Already in transaction
                context->postgis_in_transaction[context->postgis_current_connect]++;
            }
            else
            {
                // start a transaction.
                pgresult = PQexec(postgis, "BEGIN");
                if (RDBI_SUCCESS != postgis_pgresult_status(pgresult))
                {
                    ret = RDBI_GENERIC_ERROR;
                }
                else
                {
                    context->postgis_in_transaction[context->postgis_current_connect] = 0;
                    ret = RDBI_SUCCESS;
                }

                postgis_pgresult_clear(pgresult);
            }
        }
    }

    return (ret);
}

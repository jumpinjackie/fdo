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

#include "set_database.h"
#include "connect.h"
#include "disconnect.h"
#include "conn_status.h"
#include "set_err_msg.h"
#include "pgconn_status.h"
#include "pgresult_clear.h"
#include "pgresult_error.h"
#include "pgresult_status.h"
#include <string.h> /* memset(), strlen(), strncpy() */
#include <malloc.h>
#include <assert.h>

static void
localNoticeProcessor(void *arg, const char *message)
{
    //fprintf(stderr, "%s", message);
}

PGconn* get_pgconn(postgis_context_def *context, const char* database_name, int conn_index );

void set_database(postgis_context_def *context, int conn_index, PGconn* newConn);

int postgis_set_database(postgis_context_def *context, const char* database_name)
{
    PGconn* newConn = NULL;
    PGconn* newConn2 = NULL;
    long conn_index = 0;
    long conn_index2 = 0;
    int ret = RDBI_GENERIC_ERROR;

    assert(NULL != context);
    assert(NULL != database_name);

    /*
     * PostgreSQL does not appear to have a command to set current database.
     * To simulate this, we create new connections. These have same connection
     * parms as current connections, except for database.
     */

    newConn = get_pgconn( context, database_name, context->postgis_current_connect );
    newConn2 = get_pgconn( context, database_name, context->postgis_current_connect2 );

    if ( (newConn || (context->postgis_current_connect == -1)) &&
         (newConn2 || (context->postgis_current_connect2 == -1)) )
    {
        /*
         * Succeeded creating required new connections, so replace old connections
         * with new ones.
         */
        set_database(context, context->postgis_current_connect, newConn);  
        set_database(context, context->postgis_current_connect2, newConn2);  
        ret = RDBI_SUCCESS;
    }

    return(ret);
}

PGconn* get_pgconn(postgis_context_def *context, const char* database_name, int conn_index )
{
    int ret = RDBI_GENERIC_ERROR;
    PGconn* conn = NULL;
    PGconn* newConn = NULL;

    char pghost[500];
    char pguser[500];
    char pgpwd[500];
    char pgport[50];
    char pgoldDb[500];
    char pgoptions[500];
    char* pgtty = NULL;

    char* pch = NULL;

    if ( conn_index != -1 )
    {
        conn  = context->postgis_connections[conn_index];
        assert(NULL != conn);

        ret = postgis_pgconn_status(conn);
        if (RDBI_SUCCESS == ret)
        {
            /* 
             * Get current connection parms 
             */

            strncpy(pghost, PQhost(conn), 499);
            strncpy(pgport, PQport(conn), 49);
            strncpy(pguser, PQuser(conn), 499);
            strncpy(pgpwd, PQpass(conn), 499);
            strncpy(pgoldDb, PQdb(conn), 499);
            strncpy(pgoptions, PQoptions(conn), 499);

            /*
             * Establish connection to new database.
             */
            newConn = PQsetdbLogin(pghost, pgport, pgoptions, pgtty, database_name, pguser, pgpwd);
                    
            ret = postgis_pgconn_status(newConn);
            if (RDBI_SUCCESS == ret)
            {

                ret = PQsetClientEncoding( newConn, "UTF8");
                if (RDBI_SUCCESS == ret)
                {
                    PQsetNoticeProcessor( newConn, localNoticeProcessor, (void*)context);
                }
            }
                
            if (RDBI_SUCCESS != ret)
            {
                /* Retrieve connection error and
                 * set as last error message for context.
                 */
                pch = PQerrorMessage(newConn);
                postgis_set_err_msg(context, pch);
                
                PQfinish(newConn);
                newConn = NULL;
            }
        }
    }

    return (newConn);
}

void set_database(postgis_context_def *context, int conn_index, PGconn* newConn)
{
    PGconn* conn = NULL;

    if ( conn_index != -1 )
    {
        conn  = context->postgis_connections[conn_index];
        context->postgis_connections[conn_index] = newConn;
        PQfinish(conn);
    }
}


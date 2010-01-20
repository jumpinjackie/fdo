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

#include "connect.h"
#include "set_err_msg.h"
#include "pgconn_status.h"
#include "Inc/ut.h"
#include <stdio.h>
#include <assert.h>

#ifdef _WIN32
#include <malloc.h> /* alloca() */
#else
#include<alloca.h>
#endif

static void
localNoticeProcessor(void *arg, const char *message)
{
    //fprintf(stderr, "%s", message);
}

int postgis_connect(postgis_context_def* context,
    char*  connect_string,
    char*  user,
    char*  pswd,
    char** vendor_data,
    int*   connect_id)
{
    PGconn* conn = NULL;    /* Database connection handler. */

    /* Properties retrieved from connection string. */
    char* dbname = NULL;
    char* pghost = NULL;
    char* pgport = DEF_PGPORT_STR;  /* default port, defined in stdafx.h */

    /* Properties passed explicitly. */
    const char* pglogin = user;
    const char* pgpwd   = pswd;
    
    /* Properties not used in current version. */
    const char* pgoptions = NULL;
    const char* pgtty  = NULL;

    /* Used in strings manipulation. */
    char* pch = NULL;
    size_t length = 0;

    int conn_index = 0;
    int max_connects = 0;
    int ret = RDBI_SUCCESS;

    assert(NULL != context);

    /*
     * Retrieve connection properties from the RDBI connection string.
     * Connection string format is:
     * - short: database
     *          where default host is localhost and default port is 5432.
     * - long: database@host:port
     */
    pghost = "localhost";

    if (NULL != connect_string && '\0' != connect_string[0])
    {
        pch = strchr(connect_string, '@');
        if (NULL != pch)
        {
            length = pch - connect_string;
            dbname = (char*)alloca(length + 1);
            strncpy(dbname, connect_string, length);
            dbname[length] = '\0';
            pch++;
            connect_string = pch;

            /*
            * Hostname and port.
            */
            pch = strchr(connect_string, ':');
            if (NULL != pch)
            {
                length = pch - connect_string;
                pghost = (char*)alloca(length + 1);
                strncpy(pghost, connect_string, length);
                pghost[length] = '\0';
                pch++;

                pgport = pch;
            }
            else
            {
                pghost = connect_string;
            }
        }
        else
        {
            pch = strchr(connect_string, ':');
            if (NULL != pch)
            {
                length = pch - connect_string;
                pghost = (char*)alloca(length + 1);
                strncpy(pghost, connect_string, length);
                pghost[length] = '\0';
                pch++;
                pgport = pch;       
            }
            else
            {
                pghost = connect_string;
            }
        }
    }

    /*
    * Find an open slot and try to connect.
    */
    max_connects = ELEMENTS(context->postgis_connections);

   if (max_connects <= context->postgis_connect_count)
    {
        ret = RDBI_TOO_MANY_CONNECTS;
    }
    else
    {
        assert(NULL != pghost);
        assert(NULL != pgport);
        
        for (conn_index = 0;
            ((conn_index < max_connects) && (RDBI_SUCCESS == ret) && context->postgis_current_connect2 == -1 );
            conn_index++)
        {
            if (NULL == context->postgis_connections[conn_index])
            {
                /*
                 * Establish connection with PostgreSQL database.
                 */
                conn = PQsetdbLogin(pghost, pgport, pgoptions, pgtty, dbname, pglogin, pgpwd);
                
                ret = postgis_pgconn_status(conn);
                if (RDBI_SUCCESS != ret)
                {
                    /* Retrieve connection error and
                     * set as last error message for context.
                     */
                    pch = PQerrorMessage(conn);
                    postgis_set_err_msg(context, pch);
                    
                    PQfinish(conn);
                    conn = NULL;
                }
                else
                {
                    
                    if (-1 == context->postgis_current_connect)
                    {
                        context->postgis_connect_count++;
                        context->postgis_current_connect = conn_index;
                        context->postgis_connections[conn_index] = conn;
                        context->postgis_in_transaction[conn_index] = -1;
                        /* Index is used as a connection ID. */
                        *connect_id = conn_index;
                        
                        ret = RDBI_SUCCESS;
                    }
                    else if (-1 == context->postgis_current_connect2)
                    {
                        context->postgis_connect_count++;
                        context->postgis_current_connect2 = conn_index;
                        context->postgis_connections[conn_index] = conn;
                        context->postgis_in_transaction[conn_index] = -1;
                        ret = RDBI_SUCCESS;
                    }

                    if( PQsetClientEncoding( conn, "UTF8") != RDBI_SUCCESS )
                    {
                        // If we cannot set the client character set, then alot of operation may go wrong
                        pch = PQerrorMessage(conn);
                        postgis_set_err_msg(context, pch);
                        
                        PQfinish(conn);
                        conn = NULL;
                    }

                    if( conn != NULL )
                    {
                        PQsetNoticeProcessor( conn, localNoticeProcessor, (void*)context);
                    }
                }
            }
        } // for
    }

    return ret;
}

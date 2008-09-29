/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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
#include <errmsg.h>

#include "nls.h"
#include <Inc/Nls/fdordbms_msg.h>
#include <Common/Std.h>
#include <Common/StringP.h>

#include "connect.h"
#include "Inc/ut.h"
#include "run_sql.h"
#include "errno.h"
#include "xlt_status.h"
#include <malloc.h>
#include <wchar.h>
#include <locale.h>

static int set_characterset (
    mysql_context_def *context,
    bool isDDL
);

int mysql_connect (
    mysql_context_def *context,
    char  *connect_string,
    char  *user,
    char  *pswd,
    char **vendor_data,
    int   *connect_id)
{
    char *p;
    size_t length;
    char *host;
    int port;
    char *db;
    int max_connects;
    int index;
    MYSQL *mysql;
    MYSQL *handle;
    int rows_processed;
    int ret;
    unsigned long server_client_version = 0;
    const wchar_t* server_client_msg = NULL;

    ret = mysql_xlt_status(context, MYSQL_SUCCESS, (MYSQL*) NULL, (MYSQL_STMT*) NULL);

    /* connect string format db@host:port, default host to localhost, default port to MYSQL_PORT */
    host = "localhost";
    port = MYSQL_PORT;
    db = NULL;
    if (NULL != connect_string)
    {
        p = strchr (connect_string, '@');
        if ((char *)NULL != p)
        {
            length = p - connect_string;
            db = (char*)alloca (length + 1);
            strncpy (db, connect_string, length);
            db[length] = '\0';
            p++;
            connect_string = p;
        }

        p = strchr (connect_string, ':');
        if ((char *)NULL != p)
        {
            length = p - connect_string;
            host = (char*)alloca (length + 1);
            strncpy (host, connect_string, length);
            host[length] = '\0';
            p++;
            port = atoi (p);
        }
        else
            host = connect_string;
    }

    /* find an open slot */
    max_connects = ELEMENTS (context->mysql_connections);
    if (max_connects <= context->mysql_connect_count) {
        ret = RDBI_TOO_MANY_CONNECTS;
    } else {
        for (index = 0; (index < max_connects) && (RDBI_SUCCESS == ret); index++) {
            if ((MYSQL*)NULL == context->mysql_connections[index])
            {   /* initialize a connection */
                mysql = mysql_init ((MYSQL *)NULL);
                if ((MYSQL *)NULL == mysql)
                    ret = RDBI_MALLOC_FAILED;
                else
                {
                    handle = mysql_real_connect (mysql, host, user, pswd, db, port, NULL, 0);
                    if ((MYSQL *)NULL == handle) {
                        // mysql_get_msg() will get this
                        // Handle the case when the host is not found.
                        // The language setting is only on the server. If the host is invalid, the message
                        // returned by mysql_error is in English.
                        if (mysql_errno(mysql) == CR_UNKNOWN_HOST)
                        {
                            swprintf(context->mysql_last_err_msg, RDBI_MSG_SIZE, mysql_nls_msg_get(FDORDBMS_505, "Unknown MySQL server host '%1$ls'", (FdoString *) FdoStringP(host)));
                        }
                        else
                        {
                            swprintf(context->mysql_last_err_msg, RDBI_MSG_SIZE, L"%hs", mysql_error(mysql));
                        }
                        context->mysql_last_err_msg[RDBI_MSG_SIZE - 1] = 0;
                        ret = RDBI_GENERIC_ERROR;
                    }
                    else
                    {
                        server_client_version = mysql_get_client_version();
                        if ( server_client_version < MIN_CLIENT_VER )
                            server_client_msg = mysql_nls_client_version( server_client_version );
                        
                        server_client_version = mysql_get_server_version( handle );
                        if ( server_client_version < MIN_SERVER_VER )
                            server_client_msg = mysql_nls_server_version( server_client_version );

                        if ( server_client_msg != NULL)
                        {
                            swprintf(context->mysql_last_err_msg, RDBI_MSG_SIZE, L"%ls", server_client_msg);
                            mysql_close (handle);
                            context->mysql_last_err_msg[RDBI_MSG_SIZE - 1] = 0;
                            ret = RDBI_GENERIC_ERROR;
                        }
                        else
                        {
						    if (context->mysql_current_connect == -1)
						    {
							    context->mysql_connect_count++;
							    context->mysql_current_connect = index;
							    context->mysql_connections[index] = handle;

							    *connect_id = index;
                                // The following allows RDBMS object names to be double quote
                                // delimited in SQL statements, thus allowing queries on tables
                                // and columns with special characters in their names.
                                ret = mysql_run_sql( context, "set sql_mode='ANSI_QUOTES'", false, &rows_processed );
                                if ( ret == 0 ) 
                                    ret = set_characterset( context, false );
						    }
						    else if (context->mysql_current_connect2 == -1)
						    {
							    context->mysql_connect_count++;
							    context->mysql_current_connect2 = index;
							    context->mysql_connections[index] = handle;
                                // See note 10 lines above.
                                ret = mysql_run_sql( context, "set sql_mode='ANSI_QUOTES'", true, &rows_processed );
                                if ( ret == 0 ) 
                                    ret = set_characterset( context, true );
							    break;
						    }
                        }
                    }
                }
            }
        } // for
    }

    return (ret);
}

int set_characterset (
    mysql_context_def *context,
    bool isDDL
)
{
    int ret = 0;
    int rows_processed;

    ret = mysql_run_sql( context, "set character_set_connection='utf8'", isDDL, &rows_processed );
    if ( ret == 0 ) 
        ret = mysql_run_sql( context, "set character_set_client='utf8'", isDDL, &rows_processed );
    if ( ret == 0 ) 
        ret = mysql_run_sql( context, "set character_set_results='utf8'", isDDL, &rows_processed );
    if ( ret == 0 ) 
        ret = mysql_run_sql( context, "set collation_connection='utf8_bin'", isDDL, &rows_processed );
    if ( ret == 0 ) 
        ret = mysql_run_sql( context, "set collation_database='utf8_bin'", isDDL, &rows_processed );

    return ret;
}

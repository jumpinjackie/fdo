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

#include "connect.h"
#include "Inc/ut.h"
#include "run_sql.h"
#include "errno.h"
#include "xlt_status.h"
#include <malloc.h>

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
                        strncpy(context->mysql_last_err_msg, mysql_error(mysql), RDBI_MSG_SIZE);
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
						}
						else if (context->mysql_current_connect2 == -1)
						{
							context->mysql_connect_count++;
							context->mysql_current_connect2 = index;
							context->mysql_connections[index] = handle;
                            // See note 10 lines above.
                            ret = mysql_run_sql( context, "set sql_mode='ANSI_QUOTES'", true, &rows_processed );
							break;
						}

                    }
                }
            }
        } // for
    }

    return (ret);
}

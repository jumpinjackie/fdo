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

#include "est_cursor.h"

int mysql_est_cursor (mysql_context_def *context, char **cursor)
{
    return (mysql_est_cursor_obj (context, cursor));
}

int mysql_est_cursor_obj (mysql_context_def *context, char **cursor)
{
    MYSQL *mysql;
    MYSQL_STMT *statement;
    mysql_cursor_def* curs;
    int ret;

    if (-1 == context->mysql_current_connect)
        ret = RDBI_NOT_CONNECTED;
    else
    {
        mysql = context->mysql_connections[context->mysql_current_connect];
        if ((MYSQL*)NULL == mysql)
            ret = RDBI_NOT_CONNECTED;
        else
        {
            statement = mysql_stmt_init (mysql);
            if ((MYSQL_STMT *)NULL == statement)
                ret = RDBI_MALLOC_FAILED;
            else
            {
                curs = (mysql_cursor_def*)malloc (sizeof (mysql_cursor_def));
                memset (curs, 0, sizeof (mysql_cursor_def));
                curs->statement = statement;
                *cursor = (char*)curs;
                ret = RDBI_SUCCESS;
            }
        }
    }

    return (ret);
}

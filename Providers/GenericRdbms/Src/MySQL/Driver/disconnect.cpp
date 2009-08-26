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

#include "disconnect.h"

int mysql_disconnect (mysql_context_def *context)
{
    int index;
    MYSQL *mysql;
    int ret;

    if (-1 == context->mysql_current_connect)
        ret = RDBI_NOT_CONNECTED;
    else
    {
        index = context->mysql_current_connect;
        mysql = context->mysql_connections[index];
        if ((MYSQL*)NULL == mysql)
            ret = RDBI_NOT_CONNECTED;
        else
        {
            mysql_close (mysql);
            context->mysql_connections[index] = (MYSQL*)NULL;
            context->mysql_current_connect = -1;
            context->mysql_connect_count--;

            // Close the 2nd connection opened by connect()
            if ( -1 != context->mysql_current_connect2 ) {
                index = context->mysql_current_connect2;
                mysql = context->mysql_connections[index];
                
                mysql_close (mysql);
                context->mysql_connections[index] = (MYSQL*)NULL;
                context->mysql_current_connect2 = -1;
                context->mysql_connect_count--;
            }
            ret = RDBI_SUCCESS;
        }
    }

    return (ret);
}

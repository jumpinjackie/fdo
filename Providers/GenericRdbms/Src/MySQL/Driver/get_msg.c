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

#include "get_msg.h"

int mysql_get_msg (mysql_context_def *context, char *buffer)
{
    MYSQL *mysql;
    const char *message;
    int ret;

    if (-1 == context->mysql_current_connect)
    {
        strcpy (buffer, context->mysql_last_err_msg);
        ret = RDBI_NOT_CONNECTED;
    }
    else
    {
        mysql = context->mysql_connections[context->mysql_current_connect];
        message = mysql_error (mysql);
        if ('\0' == message[0])
        {
            /* should be an error for no last error */
            ret = RDBI_GENERIC_ERROR;
            buffer[0] = '\0';
        }
        else
        {
            /* should be an error for message buffer not big enough */
            strncpy (buffer, message, RDBI_MSG_SIZE);
            buffer[RDBI_MSG_SIZE - 1] = '\0'; 
            ret = RDBI_SUCCESS;
        }
    }


    return (ret);
}

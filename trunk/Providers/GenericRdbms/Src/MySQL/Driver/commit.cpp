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

#include "commit.h"
#include "errno.h"
#include "xlt_status.h"

int mysql_commitx (
    mysql_context_def *context,
    int commit_action)
{
    MYSQL *mysql;
    int ret;
    my_bool mysql_status;

    if (-1 == context->mysql_current_connect)
        ret = RDBI_NOT_CONNECTED;
    else
    {
        mysql = context->mysql_connections[context->mysql_current_connect];
        if ((MYSQL*)NULL == mysql)
            ret = RDBI_NOT_CONNECTED;
        else
        {
            mysql_status = mysql_commit (mysql);
            ret = mysql_xlt_status( context, 0 == mysql_status ? MYSQL_SUCCESS : MYSQL_GENERIC_ERROR, mysql, (MYSQL_STMT*) NULL );
        }
    }

    return (ret);
}

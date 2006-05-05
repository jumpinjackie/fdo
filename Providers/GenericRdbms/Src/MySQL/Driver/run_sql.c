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

#include "xlt_status.h"
#include "run_sql.h"

/* Runs a SQL statement in one shot without the prepare, describe, bind and 
 * execute steps. Needed for certain statements such as "create database" and
 * "use". MySql does not support running these as prepared statements.
 */
int mysql_run_sql (
    mysql_context_def *context,
    char *sql,
	int isddl,
    int *rows_processed
)
{
    MYSQL *mysql;
    int result;
    int ret;

    if (-1 == context->mysql_current_connect)
        ret = RDBI_NOT_CONNECTED;
    else
    {
		if (isddl)
			mysql = context->mysql_connections[context->mysql_current_connect2];
		else
			mysql = context->mysql_connections[context->mysql_current_connect];
        if ((MYSQL*)NULL == mysql)
            ret = RDBI_NOT_CONNECTED;
        else
        {
            result = mysql_query (mysql, sql);
            if (0 != result)
            {   
				ret = mysql_xlt_status(context, result);
            }
            else {
                ret = RDBI_SUCCESS;
                if ( rows_processed ) 
                    (*rows_processed) = (int) mysql_affected_rows(mysql);
            }
        }
    }

    return (ret);
}

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

#include "local.h"
#include "define.h"
#include "type.h"
#include "Inc/ut.h"

int mysql_desc_bind (
    mysql_context_def *context,
    char *cursor,
    int   position,     /* position within select clause    */
    int   name_size,    /* The maximum size of name         */
    char *name)        /* Name of this bind variable       */
{
    MYSQL *mysql;
    mysql_cursor_def *curs;
    MYSQL_STMT *statement;
//    MYSQL_RES *param_meta_result;
//    MYSQL_FIELD *fields;
    char buffer[20];
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
            curs = (mysql_cursor_def*)cursor;
            if ((mysql_cursor_def*)NULL == curs)
                ret = RDBI_GENERIC_ERROR;
            else
            {
                statement = curs->statement;
                if ((MYSQL_STMT*)NULL != statement)
                {   // this is bogus until 
                    //    param_meta_result = mysql_stmt_param_metadata (statement);
                    // works.
#pragma message ("ToDo: fix mysql_stmt_param_metadata ()")
                    position--; /* zero based */
                    if ((0 > position) || (curs->bind_count - 1 < position))
                        ret = RDBI_NOT_IN_DESC_LIST;
                    else
                    {
                        ut_itoa (position + 1, buffer );
                        strncpy (name, buffer, name_size);
                        ret = RDBI_SUCCESS;
                    }
                }
            }
        }
    }

    return (ret);
}

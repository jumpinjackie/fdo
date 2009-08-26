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

#include "Geometry.h"
#include "fre_cursor.h"

void mysql_fre_binds (mysql_cursor_def* cursor)
{
    int i;

    if ((mysql_define_def *)NULL != cursor->redefines)
    {
        for (i = 0; i < cursor->define_count; i++)
            if ((void*)NULL != cursor->redefines[i].geometry)
            {
                FreeGeometry (cursor->redefines[i].geometry);
                cursor->redefines[i].geometry = (void*)NULL;
                *((char**)(cursor->redefines[i].original)) = NULL;
            }
        free (cursor->redefines);
        cursor->redefines = (mysql_define_def *)NULL;
    }
    if ((mysql_define_def *)NULL != cursor->rebinds)
    {
        for (i = 0; i < cursor->bind_count; i++)
            if ((void*)NULL != cursor->rebinds[i].substitution)
            {
                free (cursor->rebinds[i].substitution);
                cursor->rebinds[i].substitution = (void*)NULL;
            }
        free (cursor->rebinds);
        cursor->rebinds = (mysql_define_def *)NULL;
    }
    if ((MYSQL_BIND *)NULL != cursor->defines)
    {
        free (cursor->defines);
        cursor->define_count = 0;
        cursor->defines = (MYSQL_BIND *)NULL;
    }
    if ((MYSQL_BIND *)NULL != cursor->binds)
    {
        free (cursor->binds);
        cursor->bind_count = 0;
        cursor->binds = (MYSQL_BIND *)NULL;
    }

    if ((int *)NULL != cursor->srids)
    {
        free (cursor->srids);
        cursor->srids = (int *)NULL;
    }
}

int mysql_fre_cursor (mysql_context_def *context, char **cursor)
{
    MYSQL *mysql;
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
            curs = (mysql_cursor_def*)*cursor;
            if ((mysql_cursor_def*)NULL != curs)
            {
                if ((MYSQL_STMT *)NULL != curs->statement)
                {
                    mysql_stmt_close (curs->statement);
                    curs->statement = (MYSQL_STMT *)NULL;
                }
                mysql_fre_binds (curs);
                free (curs);
                *cursor = NULL;
                ret = RDBI_SUCCESS;
            }
        }
    }

    return (ret);
}

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

#include "bind.h"
#include "type.h"

int mysql_bind (
    mysql_context_def *context,
    char *cursor,
    char *name,
    int datatype,
    int size,
    char *address,
    my_bool *null_ind)
{
    MYSQL *mysql;
    mysql_cursor_def *curs;
    MYSQL_STMT *statement;
    int index;
    int type;
    MYSQL_BIND *binds;
	int		*srids;
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
                {   
                    index = atoi (name); /* numeric position */
                    /* need an error for bind names not supported */
                    if (0 >= index)
                        ret = RDBI_GENERIC_ERROR; /* need an error for unknown name */
                    else
                    {
                        index--; /* make it zero based */
                        type = rdbi_to_mysql_type (datatype, size);
                        if (0 > type)
                            ret = RDBI_INVLD_DESCR_OBJTYPE; 
                        else
                        {
                            if (index >= curs->bind_count)
                            {
                                binds = (MYSQL_BIND *)malloc ((index + 1) * sizeof (MYSQL_BIND));
								srids = (int*)malloc((index + 1) * sizeof(int));
                                if ((MYSQL_BIND*)NULL == binds || (int*)NULL == srids)
                                {
                                    if ((MYSQL_BIND*)NULL != curs->binds)
                                        free (curs->binds);
									if ((int*)NULL != curs->srids)
										free (curs->srids);
                                    curs->bind_count = 0;
                                }
                                else
                                {
                                    memset (binds, 0, (index + 1) * sizeof (MYSQL_BIND));
									memset (srids, 0, (index + 1) * sizeof (int));
                                    if ((MYSQL_BIND*)NULL != curs->binds)
                                    {
                                        memcpy (binds, curs->binds, curs->bind_count * sizeof (MYSQL_BIND));
										free (curs->binds);
										memcpy (srids, curs->srids, curs->bind_count * sizeof (int));
										free (curs->srids);
                                    }
                                    curs->bind_count = index + 1;
                                    curs->binds = binds;
									curs->srids = srids;
                                }
                            }
                            if (index >= curs->bind_count)
                                ret = RDBI_MALLOC_FAILED;
                            else
                            {
                                curs->binds[index].buffer_type = (enum enum_field_types)type;
                                curs->binds[index].buffer_length = size;
                                curs->binds[index].buffer = address;
                                if ((my_bool*)NULL != null_ind)
                                {
                                    curs->binds[index].is_null = null_ind;
                                }
                                ret = RDBI_SUCCESS;
                            }
                        }
                    }
                }
            }
        }
    }

    return (ret);
}

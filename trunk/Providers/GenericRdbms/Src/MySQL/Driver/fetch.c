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

#include "fetch.h"
#include "Geometry.h"
#include "xlt_status.h"

/// <summary>Run through the defines and fix the geometries.</summary>
static void fix_geometries (mysql_cursor_def* cursor)
{
    int i;

    for (i = 0; i < cursor->define_count; i++)
    {
        if ((void*)NULL != cursor->redefines[i].substitution)
        {
            /* free any existing geometry */
            if ((void*)NULL != cursor->redefines[i].geometry)
            {
                FreeGeometry (cursor->redefines[i].geometry);
                cursor->redefines[i].geometry = (void*)NULL;
                *((char**)(cursor->redefines[i].original)) = NULL;
            }
            if (0 != *(cursor->defines[i].is_null))
                *((char**)(cursor->redefines[i].original)) = NULL;
            else
            {
				// MySQL returns a WKB prefixed with SRID_SIZE field (4 bytes).
                cursor->redefines[i].geometry = GeometryFromWkb (
                    (char*)(cursor->redefines[i].substitution) + 4, 
                    *(cursor->defines[i].length) - 4);
                *((char**)(cursor->redefines[i].original)) = (char*)cursor->redefines[i].geometry;
            }
        }
    }
}

int mysql_fetch (
    mysql_context_def *context,
    char *cursor,                               /* cursor context area      */
    int count,                                  /* # rows requested         */
    int do_exec,
    int do_cancel,
    int *rows_processed)                        /* # rows actually returned */
{
    MYSQL *mysql;
    mysql_cursor_def *curs;
    int result;
    my_ulonglong rows;
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
            if ((mysql_cursor_def*)NULL != curs)
            {
				*rows_processed = 0;
                result = mysql_stmt_fetch (curs->statement);
                ret = mysql_xlt_status(context, result, mysql, (MYSQL_STMT*) NULL);

				// Post-process BLOB columns
				for (int i=0; i<curs->define_count; i++)
				{
					if (curs->defines[i].buffer_type != MYSQL_TYPE_BLOB)
						continue;

					// avoid datatruncation error on BLOBs
					if (ret == RDBI_DATA_TRUNCATED && curs->defines[i].error)
						ret = RDBI_SUCCESS;
					else
					{
						// MySql doesn't clean up the internal buffer which is copied into the user's buffer.
						// Therefore consider the returned length of the BLOB. By inserting a null terminator 
						// helps the caller to handle the TEXT columns.
						unsigned long	*size = curs->defines[i].length;
						char			*address = (char *)curs->defines[i].buffer;
						if (curs->redefines == NULL || curs->redefines[i].orig_type != MYSQL_TYPE_GEOMETRY)
							address[*size] = '\0';
					}
				}

				if (ret == RDBI_SUCCESS)
                {
					// mysql_stmt_affected_rows returns inconsistent values for select
                    //rows  = mysql_stmt_affected_rows (curs->statement);
                    //if ((my_ulonglong)-1 != rows)
					rows = (my_ulonglong)1;
                    *rows_processed = (int)rows; /* truncation possible */
                    if ((0L != rows) && ((mysql_define_def *)NULL != curs->redefines))
                        fix_geometries (curs);
                    ret = RDBI_SUCCESS;
                }
            }
            else
                ret = RDBI_GENERIC_ERROR;
        }
    }

    return (ret);
}

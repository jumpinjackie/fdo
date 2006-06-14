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

#include "execute.h"
#include "Geometry.h"
#include "xlt_status.h"

static void print_st_error (MYSQL_STMT *stmt, const char *msg)
{
    int err;
    const char *error;

    if (stmt && mysql_stmt_errno (stmt))
    {
        err = mysql_stmt_errno (stmt);
        error = mysql_stmt_error (stmt);
        printf ("\n[%d] %s\n", err, error);
    }
    else if (msg)
        fprintf(stderr, " [MySQL] %s\n", msg);
}

// Expect geometries up this size. 
// A better idea is to check in advance the max size of the actual geometries by using
// a "select max (length(<geom_column>)) from ..." and allocate the redefines accordingly.
// The same apply for blobs.

#define GEOMETRY_MAXIMUM_SIZE  1 * 1024 * 1024 /* 1MB */

/// <summary>Run through the defines and change geometries to MYSQL_TYPE_BLOB.</summary>
/// <remark>The actual MYSQL_TYPE_XXX type used doesn't appear to matter.</remark>
/// <remark>At this point the MYSQL_FIELD for the geometry has a length of 16777215,
/// and a max_length of 0, so we use an arbitrary maximum size for each geometry.</remark>
static void adjust_defines (mysql_cursor_def* cursor)
{
    int i;
    int geometries;
    int allocation;
    char* buffer;

    geometries = 0;
    for (i = 0; i < cursor->define_count; i++)
        if (MYSQL_TYPE_GEOMETRY == cursor->defines[i].buffer_type)
            geometries++;
        else if (246 /* MYSQL_TYPE_NEWDECIMAL */ == cursor->defines[i].buffer_type)
            cursor->defines[i].buffer_type = MYSQL_TYPE_DECIMAL;
    if (0 != geometries)
    {   /* redefinition of some field(s) needed */
        allocation = sizeof (mysql_define_def) * cursor->define_count;
        allocation += GEOMETRY_MAXIMUM_SIZE * geometries;
        cursor->redefines = (mysql_define_def *)malloc (allocation);
        memset (cursor->redefines, 0, allocation);
        buffer = (char*)&(cursor->redefines[cursor->define_count]);
        for (i = 0; i < cursor->define_count; i++)
            if (MYSQL_TYPE_GEOMETRY == cursor->defines[i].buffer_type)
            {
                cursor->redefines[i].original = cursor->defines[i].buffer;
                cursor->redefines[i].substitution = buffer;
                cursor->redefines[i].orig_type = cursor->defines[i].buffer_type;
                buffer += GEOMETRY_MAXIMUM_SIZE;
                cursor->defines[i].buffer_type = MYSQL_TYPE_BLOB;
                cursor->defines[i].buffer_length = GEOMETRY_MAXIMUM_SIZE;
                cursor->defines[i].buffer = cursor->redefines[i].substitution;
            }
    }
}

/// <summary>Run through the binds and change geometries to MYSQL_TYPE_BLOB.</summary>
/// <remark>At this point the MYSQL_FIELD for the geometry has a length of 16777215,
/// and a max_length of 0, so we use an arbitrary maximum size for each geometry.</remark>
static void adjust_binds (mysql_cursor_def* cursor)
{
    int i;
    int geometries;
    int type;
    int valid;
    int size;
    int allocation;
    char* buffer;

    geometries = 0;
    for (i = 0; i < cursor->bind_count; i++)
    {
        type = cursor->binds[i].buffer_type;
        if (MYSQL_TYPE_GEOMETRY == type)
            geometries++;
        else if (246 /* MYSQL_TYPE_NEWDECIMAL */ == type)
            cursor->binds[i].buffer_type = MYSQL_TYPE_DECIMAL;
        else
        {
            valid = (NULL == cursor->binds[i].is_null) || (0 == *(cursor->binds[i].is_null));
            if (valid && (MYSQL_TYPE_STRING == type || MYSQL_TYPE_VAR_STRING == type))
                /* set the length of each bound string. Even for variable strings,
                * MySql needs the length, it does not seem to check for the null
                * terminator.
                */
                cursor->binds[i].buffer_length = (unsigned long) strlen((const char*)cursor->binds[i].buffer);
        }
    }
    if ((0 != geometries) || cursor->rebinds )
    {   /* redefinition of some field(s) needed or fields were redefined by 
         * previous execute.
         */
        if ( !cursor->rebinds ) {
            allocation = sizeof (mysql_define_def) * cursor->bind_count;
            cursor->rebinds = (mysql_define_def *)malloc (allocation);
            memset (cursor->rebinds, 0, allocation);
        }
        for (i = 0; i < cursor->bind_count; i++)
            if ( (MYSQL_TYPE_GEOMETRY == cursor->binds[i].buffer_type) ||
                 (MYSQL_TYPE_GEOMETRY == cursor->rebinds[i].orig_type) )
            {
                // Set up original buffer (containing value set by caller)
                // if not already done.
                if ( !(cursor->rebinds[i].original) )
                    cursor->rebinds[i].original = cursor->binds[i].buffer;

                // get bound geometry value set by caller 
				buffer = WkbFromGeometry (*((void**)(cursor->rebinds[i].original)), &size);

                // Set up substitute bind buffer, containing geometry in WKB blob form.
                if (NULL != cursor->rebinds[i].substitution)
                    free (cursor->rebinds[i].substitution);
                cursor->rebinds[i].substitution = buffer;

                // Remember original bind variable type
                cursor->rebinds[i].orig_type = MYSQL_TYPE_GEOMETRY;

                // Substitute a blob bind variable for the geometry variable.
                cursor->binds[i].buffer_type = MYSQL_TYPE_BLOB;
                cursor->binds[i].buffer_length = size;
                cursor->binds[i].buffer = cursor->rebinds[i].substitution;
				if ( NULL != cursor->binds[i].is_null && size == 0) {
					*(cursor->binds[i].is_null) = 1;
				}
            }
    }
}

int mysql_execute (
    mysql_context_def *context,
    char *cursor,                     /* cursor associated with SQL stmnt */
    int count,                        /* # rows to process                */
    int offset,                       /* offset in row buffer to start    */
    int *rows_processed)              /* # rows processed by this exec    */
{
    MYSQL *mysql;
    mysql_cursor_def* curs;
    int result;
    my_ulonglong rows;
    int ret = RDBI_GENERIC_ERROR;

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
                // apply binds and defines if necessary
                result = 0;
                if ((MYSQL_BIND*)NULL != curs->defines)
                {
                    adjust_defines (curs);
                    result = mysql_stmt_bind_result (curs->statement, curs->defines);
                }
                if (0 != result)
                {
                    print_st_error (curs->statement, "bind result failed");
                }
                else
                {
                    if ((MYSQL_BIND*)NULL != curs->binds)
                    {
                        adjust_binds (curs);
                        result = mysql_stmt_bind_param (curs->statement, curs->binds);
                    }
                    if (0 != result)
                    {
						ret = mysql_xlt_status(context, result);
                    }
                    else
                    {
                        result = mysql_stmt_execute (curs->statement);
                        if (0 != result)
                        {
                            result = mysql_stmt_errno(curs->statement);
							ret = mysql_xlt_status(context, result);
                        }
                        else
                        {
							MYSQL_RES * result_metadata = mysql_stmt_result_metadata(curs->statement);

							// KLUDGE: Store results to avoid "Commands out of sync" in later queries.
							(void) mysql_stmt_store_result (curs->statement);

                            if (NULL == result_metadata)
                            {
								// The statement returns no data, and thus could
								// have affected something.
                                rows  = mysql_stmt_affected_rows (curs->statement);
                                if ((my_ulonglong)-1 != rows) {
                                    *rows_processed = (int)rows; /* truncation possible */
                                    ret = RDBI_SUCCESS;
                                }                       
                            }
							else
							{
								// No reliable row count if there is a result set.  That's fine,
								// as we'll get the count by fetching data later.
								mysql_free_result(result_metadata);
                                *rows_processed = 0;
								result_metadata = NULL;
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

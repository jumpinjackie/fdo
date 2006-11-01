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
#include "errno.h"
#include "xlt_status.h"

#ifdef _WIN32
#define case_insensitive_compare _stricmp
#else
#define case_insensitive_compare strcasecmp
#endif

int mysql_desc_slct (
    mysql_context_def *context,
    char *cursor,
    int   position,     /* position within select clause    */
    int   name_size,    /* The maximum size of name         */
    char *name,         /* Name of this field               */
    int  *rdbi_type,    /* rdbi constant                    */
    int  *binary_size,  /* bytes of memory to hold it       */
    int  *null_ok)     /* TRUE iff NULLs ok in this column */
{
    MYSQL *mysql;
    mysql_cursor_def *curs;
    MYSQL_STMT *statement;
    MYSQL_RES *prepare_meta_result;
    MYSQL_FIELD *fields;
    int type;
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
                {   /* fetch result set meta information and check prepare was done */
                    prepare_meta_result = mysql_stmt_result_metadata (statement);
                    if ((MYSQL_RES*)NULL == prepare_meta_result)
                    {
                        mysql_xlt_status( context, MYSQL_GENERIC_ERROR, mysql, statement );
                        ret = RDBI_INVLD_DESCR_OBJTYPE;
                    }
                    else
                    {
                        position--; /* zero based */
                        if ((0 > position)
                            || (((unsigned int)position) >= mysql_num_fields (prepare_meta_result)))
                            ret = RDBI_NOT_IN_DESC_LIST;
                        else
                        {
                            fields = mysql_fetch_fields (prepare_meta_result);
                            type = mysql_to_rdbi_type (fields[position].type, fields[position].length, fields[position].decimals);
                            if (-1 == type)
                                ret = RDBI_INVLD_DESCR_OBJTYPE;
                            else
                            {
                                strncpy (name, fields[position].name, name_size);
                                name[name_size - 1] = '\0';
                                *rdbi_type = type;
								switch (type)
								{
									case RDBI_DOUBLE:
										*binary_size = sizeof(double);
										break;
									case RDBI_SHORT:
										*binary_size = sizeof(short);
										break;
									case RDBI_LONG:
										*binary_size = sizeof(long);
										break;
									case RDBI_GEOMETRY:
										*binary_size = sizeof(long); // geometry address
										break;
                                    case RDBI_LONGLONG:
#ifdef _WIN32
					                    *binary_size = sizeof(__int64);
#else
					                    *binary_size = sizeof(int64_t);
#endif
                                        break;

									default:
										*binary_size = fields[position].length;
										break;
								}
                                *null_ok = (0 == IS_NOT_NULL(fields[position].flags));
                                ret = RDBI_SUCCESS;
                            }
                        }
                        mysql_free_result(prepare_meta_result);
                    }
                }
            }
        }
    }

    return (ret);
}

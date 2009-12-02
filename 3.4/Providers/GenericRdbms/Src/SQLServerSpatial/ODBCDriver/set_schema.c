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
 */

#include <Inc/rdbi.h>					/* rdbi status values		*/
#include    <Inc/ut.h>
#include	<Inc/debugext.h>
#include "proto_p.h"
#include <stdio.h>
#include <wchar.h>

int local_odbcdr_sql( odbcdr_context_def  *context,	char *cursor, rdbi_string_def* sql,
	int defer, char *verb, void *ptree, char *cursor_coc);

/************************************************************************
* Name                                                                  *
*   odbcdr_set_schema - Alter the session schema.                       *
*                                                                       *
* Synopsis                                                              *
*   odbcdr_set_schema(const char *schema_name)                          *
*                                                                       *
* Description                                                           *
*   This  module uses 'alter session' to change the current schema.     *
*                                                                       *
* Parameters                                                            *
*   schema_name: input                                                  *
*       The schema name (SqlServer user) to switch to.                  *
*                                                                       *
* Function value                                                        *
*   Returns an rdbi status code from inc/rdbi.h                         *
*                                                                       *
************************************************************************/

/* Makes the given schema (SqlServer database) the current schema */
int local_odbcdr_set_schema (
    odbcdr_context_def *context,
    rdbi_string_def *schema_name
    )
{
    wchar_t              sql_buf[200];
    rdbi_string_def      sqlval;
    int                  rows;
    odbcdr_cursor_def    *c;
    odbcdr_connData_def  *connData;
    int                  rdbi_status;
    bool                 schema_valid = false;
    sqlval.wString = sql_buf;
#ifdef _DEBUG
    if (context->odbcdr_UseUnicode){
        debug_on1("odbcdr_set_schema", "schema_name '%ls'", ISNULL(schema_name->cwString));
    }else{
        debug_on1("odbcdr_set_schema", "schema_name '%s'", ISNULL(schema_name->ccString));
    }
#endif
    schema_valid = !ODBCDRV_STRING_EMPTY(schema_name);

    ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

    rdbi_status = RDBI_SUCCESS;

    if (ODBCDriverType_SQLServer == connData->driver_type && schema_valid)
    {
        if (context->odbcdr_UseUnicode){
            (void) odbcdr_swprintf(sqlval.wString, 200, L"USE \"%ls\"", schema_name->cwString);
            debug1("%.120ls", sqlval.cwString);
        }else{
            (void) sprintf(sqlval.cString, "USE \"%s\"", schema_name->ccString);
            debug1("%.120s", sqlval.ccString);
        }

        /* establish cursor */
        if (RDBI_SUCCESS == (rdbi_status = odbcdr_est_cursor (context, (char **)&c)))
        {
            /* parse command */
            if (RDBI_SUCCESS == (rdbi_status = local_odbcdr_sql (context, (char *)c, &sqlval,
                FALSE, "", (void *)NULL, (char *) NULL)))
            {
                /* execute the SQL statement */
                rdbi_status = odbcdr_execute( context, (char *)c, 1, 0, &rows );
            }
            odbcdr_fre_cursor (context, (char **)&c);
        }
    }
    else if (ODBCDriverType_OracleNative == connData->driver_type && schema_valid)
    {
        if (context->odbcdr_UseUnicode){
            (void) odbcdr_swprintf(sqlval.wString, 200, L"alter session set current_schema = %ls", schema_name->cwString);
            debug1("%.120ls", sqlval.cwString);
        }else{
            (void) sprintf(sqlval.cString, "alter session set current_schema = %s", schema_name->ccString);
            debug1("%.120s", sqlval.ccString);
        }
        /* establish cursor */
        if (RDBI_SUCCESS == (rdbi_status = odbcdr_est_cursor (context, (char **)&c)))
        {
            /* parse command */
            if (RDBI_SUCCESS == (rdbi_status = local_odbcdr_sql (context, (char *)c, &sqlval,
                FALSE, "", (void *)NULL, (char *) NULL)))
            {
                /* execute the SQL statement */
                rdbi_status = odbcdr_execute( context, (char *)c, 1, 0, &rows );
            }
            odbcdr_fre_cursor (context, (char **)&c);
        }
    }

the_exit:
    debug_return (NULL, rdbi_status);
}
int odbcdr_set_schema (
    odbcdr_context_def *context,
    const char *schema_name
)
{
    rdbi_string_def str;
    str.ccString = schema_name;
    return local_odbcdr_set_schema(context, &str);
}

int odbcdr_set_schemaW (
    odbcdr_context_def *context,
    const wchar_t *schema_name
)
{
    rdbi_string_def str;
    str.cwString = schema_name;
    return local_odbcdr_set_schema(context, &str);
}


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

/************************************************************************
* Name                                                                  *
*   odbcdr_set_schema - Alter the session schema.                       *
*                                                                       *
* Synopsis                                                              *
*   odbcdr_set_schema(char *schema_name)                                *
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

#include <Inc/rdbi.h>					/* rdbi status values		*/
#include    <Inc/ut.h>
#include	<Inc/debugext.h>
#include "proto_p.h"
#include <stdio.h>

/* Makes the given schema (SqlServer database) the current schema */
int odbcdr_set_schemaW (
    odbcdr_context_def *context,
    wchar_t *schema_name
)
{
    wchar_t              sql_buf[200];
    int                  rows;
    odbcdr_cursor_def    *c;
    odbcdr_connData_def  *connData;
    int                  rdbi_status;
    
    debug_on1("odbcdr_set_schema", "schema_name '%s'", ISNULL(schema_name));

    ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

    rdbi_status = RDBI_SUCCESS;

    if (ODBCDriverType_SQLServer == connData->driver_type && NULL != schema_name && (wcslen(schema_name) > 0))
    {
	    (void) swprintf(sql_buf, 199, L"USE \"%ls\"", schema_name);

        debug1("%.120ls", sql_buf);

        /* establish cursor */
        if (RDBI_SUCCESS == (rdbi_status = odbcdr_est_cursor (context, (char **)&c)))
        {
            /* parse command */
            if (RDBI_SUCCESS == (rdbi_status = odbcdr_sqlW (context, (char *)c, sql_buf,
                FALSE, "", (void *)NULL, (char *) NULL)))
            {
                /* execute the SQL statement */
                rdbi_status = odbcdr_execute( context, (char *)c, 1, 0, &rows );
            }
            odbcdr_fre_cursor (context, (char **)&c);
        }
    }
    else if (ODBCDriverType_OracleNative == connData->driver_type &&
             NULL != schema_name && schema_name[0] != '\0')
    {
	    (void) swprintf(sql_buf, 199, L"alter session set current_schema = %ls", schema_name);

        debug1("%.120ls", sql_buf);

        /* establish cursor */
        if (RDBI_SUCCESS == (rdbi_status = odbcdr_est_cursor (context, (char **)&c)))
        {
            /* parse command */
            if (RDBI_SUCCESS == (rdbi_status = odbcdr_sqlW (context, (char *)c, sql_buf,
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
    char *schema_name
)
{
    char                 sql_buf[200];
    int                  rows;
    odbcdr_cursor_def    *c;
    odbcdr_connData_def  *connData;
    int                  rdbi_status;
    
    debug_on1("odbcdr_set_schema", "schema_name '%s'", ISNULL(schema_name));

    ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

    rdbi_status = RDBI_SUCCESS;

    if (ODBCDriverType_SQLServer == connData->driver_type && NULL != schema_name && (strlen(schema_name) > 0))
    {
	    (void) sprintf(sql_buf, "USE %s", schema_name);

        debug1("%.120s", sql_buf);

        /* establish cursor */
        if (RDBI_SUCCESS == (rdbi_status = odbcdr_est_cursor (context, (char **)&c)))
        {
            /* parse command */
            if (RDBI_SUCCESS == (rdbi_status = odbcdr_sql (context, (char *)c, sql_buf,
                FALSE, "", (void *)NULL, (char *) NULL)))
            {
                /* execute the SQL statement */
                rdbi_status = odbcdr_execute( context, (char *)c, 1, 0, &rows );
            }
            odbcdr_fre_cursor (context, (char **)&c);
        }
    }
    else if (ODBCDriverType_OracleNative == connData->driver_type &&
             NULL != schema_name && schema_name[0] != '\0')
    {
	    (void) sprintf(sql_buf, "alter session set current_schema = %s", schema_name);

        debug1("%.120s", sql_buf);

        /* establish cursor */
        if (RDBI_SUCCESS == (rdbi_status = odbcdr_est_cursor (context, (char **)&c)))
        {
            /* parse command */
            if (RDBI_SUCCESS == (rdbi_status = odbcdr_sql (context, (char *)c, sql_buf,
                FALSE, "", (void *)NULL, (char *) NULL)))
            {
                /* execute the SQL statement */
                rdbi_status = odbcdr_execute( context, (char *)c, 1, 0, &rows );
            }
            odbcdr_fre_cursor (context, (char **)&c);
        }
    }

    if (RDBI_SUCCESS == rdbi_status)
    {
        connData->db_name[0] = '\0';
        if (NULL != schema_name)
            (void) strcpy (connData->db_name, schema_name);
    }

the_exit:
    debug_return (NULL, rdbi_status);
}

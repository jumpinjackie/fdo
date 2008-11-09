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
*   odbcdr_set_sess_env - Alter the session NLS environment.            *
*                                                                       *
* Synopsis                                                              *
*   odbcdr_set_sess_env()												*
*                                                                       *
* Description                                                           *
*   This  module changes Oracle session NLS parameters.					*
*                                                                       *
* Function value                                                        *
*   Returns an rdbi status code from inc/rdbi.h                         *
*                                                                       *
************************************************************************/

#include <Inc/rdbi.h>					/* rdbi status values		*/
#include <Inc/ut.h>
#include <Inc/debugext.h>
#include "proto_p.h"
#include <stdio.h>
#include <wchar.h>

int local_odbcdr_execute_direct(
    odbcdr_context_def  *context,
	rdbi_string_def     *sql,
	int    *rows_processed
	);

int odbcdr_set_sess_env(odbcdr_context_def *context)
{
    int                  rows;
    odbcdr_connData_def  *connData;
    int                  rdbi_status;
    rdbi_string_def sql1;
    rdbi_string_def sql2;
    
    debug_on("odbcdr_set_sess_env");

    ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

    rdbi_status = RDBI_SUCCESS;

    if (ODBCDriverType_OracleNative == connData->driver_type ||
		ODBCDriverType_OracleNonNative == connData->driver_type )
    {
        if (context->odbcdr_UseUnicode)
        {
            sql1.cwString = L"alter session set NLS_DATE_FORMAT = 'YYYY-MM-DD-HH24-MI-SS'";
            sql2.cwString = L"alter session set NLS_NUMERIC_CHARACTERS=\".,\"";
        }
        else
        {
            sql1.ccString = "alter session set NLS_DATE_FORMAT = 'YYYY-MM-DD-HH24-MI-SS'";
            sql2.ccString = "alter session set NLS_NUMERIC_CHARACTERS=\".,\"";
        }
        rdbi_status = local_odbcdr_execute_direct(context, &sql1, &rows);
        if (rdbi_status == RDBI_SUCCESS)
            rdbi_status = local_odbcdr_execute_direct(context, &sql2, &rows);
    }

the_exit:
    debug_return (NULL, rdbi_status);
}

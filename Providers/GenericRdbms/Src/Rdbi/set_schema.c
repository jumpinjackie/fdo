// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

/************************************************************************
*                                                                       *
* Name                                                                  *
*   rdbi_set_schema - Alter the session schema.                         *
*                                                                       *
* Synopsis                                                              *
*   rdbi_set_schema(char *schema_name)                                  *
*                                                                       *
* Description                                                           *
*   This  module alters the current schema.                             *
*                                                                       *
* Parameters                                                            *
*   schema_name: input                                                  *
*       The schema name to set.                                         *
*                                                                       *
* Function value                                                        *
*   Returns an rdbi status code from Inc/rdbi.h                         *
*                                                                       *
* Remarks                                                               *
*       Global rdbi_last_status is updated here.                        *
*                                                                       *
************************************************************************/

#include <Inc/debugext.h>

#include <stdio.h>
#include <Inc/Nls/rdbi_msg.h>
#include "global.h"
#include	<Inc/Rdbi/context.h>
#include <Inc/ut.h>

int rdbi_set_schema (rdbi_context_def *context, char *schema_name)
{
    int   status;
	char			sql[200];
	int             trace_line_num;

    debug_on("rdbi_usr_exists");
	
	sprintf(sql, "set current_schema = %s", schema_name);
	debug_trace(sql, (wchar_t *)NULL, &trace_line_num);

    status = (*(context->dispatch.set_schema))(context->drvr, schema_name);

	sprintf(sql, "Status: %d", status);
	debug_trace(sql, (wchar_t *)NULL, NULL);

    context->rdbi_last_status = status;

    debug_return(NULL, status);

}

int rdbi_set_schemaW (rdbi_context_def *context, wchar_t *schema_name)
{
    int   status;
	wchar_t			sql[200];
	int             trace_line_num;

    debug_on("rdbi_usr_exists");
	
	swprintf(sql, 199, L"set current_schema = %ls", schema_name);
	debug_trace((char*) NULL, sql, &trace_line_num);

    status = (*(context->dispatch.set_schemaW))(context->drvr, schema_name);

	swprintf(sql, 199, L"Status: %d", status);
	debug_trace((char*) NULL, sql, NULL);

    context->rdbi_last_status = status;

    debug_return(NULL, status);

}

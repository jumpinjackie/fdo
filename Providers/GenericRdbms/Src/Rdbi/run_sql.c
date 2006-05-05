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
*   rdbi_run_sql - Parses and executes a SQl statement in one call.     *
*                                                                       *
*                                                                       *
* Synopsis                                                              *
*   rdbi_run_sql(char *sql, int *rows_processed)                                             *
*                                                                       *
* Description                                                           *
*   This  module runs the given SQL statement,                          *
*                                                                       *
* Parameters                                                            *
*   sql: input                                                          *
*       The sql statement. 
*
*   rows_processed: output
*       The number of rows processed by the statement.
*       Can be NULL, in which case this parameter is ignored.
*                                                                       *
* Function value                                                        *
*   Returns an rdbi status code from Inc/rdbi.h                         *
*                                                                       *
* Remarks                                                               *
*       Although available for general use, this function was written   *
*       to satisfy a MySql requirement where MySql "create database",   *
*       "drop database" and "use" statements cannot be prepared.        *
*                                                                       *
************************************************************************/

#include <Inc/debugext.h>
#include <stdio.h>
#include <Inc/Nls/rdbi_msg.h>
#include "global.h"
#include	<Inc/Rdbi/context.h>
#include <Inc/Rdbi/proto.h>
#include "Inc/ut.h"


static int local_rdbi_run_sql (rdbi_context_def *context, char *sql, wchar_t *sqlW, int isddl, int *rows_processed)
{
    int   status;
	int             trace_line_num = 0;
	char            stats[128];
	char			tran_id[60];
	int				tran_begin = FALSE;

    debug_on("rdbi_run_sql");

	debug_trace(sql, sqlW, &trace_line_num);

	*rows_processed = 0;

	if (!isddl)	{
		if (context->rdbi_cnct->autocommit_on) {
			sprintf(tran_id, "auto-exec-run_sql %d", trace_line_num );
			rdbi_tran_begin(context, tran_id);
			tran_begin = TRUE;
		}
	}
	if( sql != NULL )
		status = (*(context->dispatch.run_sql))(context->drvr, sql, isddl, rows_processed);
	else if ( context->dispatch.run_sqlW != NULL )
		status = (*(context->dispatch.run_sqlW))(context->drvr, sqlW, isddl, rows_processed);
	else
		status = RDBI_GENERIC_ERROR;


	(void) sprintf(stats, "Rows processed: %ld", *rows_processed);
    
    debug_trace(stats, (wchar_t *) NULL, NULL);

    context->rdbi_last_status = status;

	if (tran_begin)
		/* Transaction will be committed if it is the last one.*/
		rdbi_tran_end(context, tran_id);

    debug_return(NULL, status);

}

int rdbi_run_sql (rdbi_context_def *context, char *sql, int isddl, int *rows_processed)
{
	return local_rdbi_run_sql( context, sql, NULL, isddl, rows_processed );
}

int rdbi_run_sqlW (rdbi_context_def *context, wchar_t *sql, int isddl, int *rows_processed)
{
	return local_rdbi_run_sql( context, NULL, sql, isddl, rows_processed );
}


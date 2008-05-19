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


#include <Inc/ut.h>
#include <Inc/debugext.h>
#include <Inc/ut.h>
#include <Inc/rdbi.h>                   /* rdbi status values       */

#include "constants.h"
#include "structs.h"
#include "proto_p.h"

int local_odbcdr_execute_direct( odbcdr_context_def  *context, rdbi_string_def *sql, int *rows_processed );
int local_odbcdr_sql( odbcdr_context_def  *context,	char *cursor, rdbi_string_def* sql,
	int defer, char *verb, void *ptree, char *cursor_coc);

int local_odbcdr_run_sql (
    odbcdr_context_def *context,
    rdbi_string_def    *sql,
	int isddl,
    int *rows_processed
)
{
    odbcdr_cursor_def    *c;
    int                 rdbi_status = RDBI_GENERIC_ERROR;
    int                 rows;
    ODBCDR_ERRORINFO_VARS;

#ifdef _DEBUG
    if (context->odbcdr_UseUnicode){
        debug_on1("odbcdr_run_sql", "sql: %.250ls ", sql->cwString );
    }else{
        debug_on1("odbcdr_run_sql", "sql: %.250s ", sql->ccString );
    }
#endif

	if (isddl)
	{
		if (RDBI_SUCCESS == (rdbi_status = local_odbcdr_execute_direct(context, sql, &rows)))
		{
			if ( rows_processed ) 
				(*rows_processed) = rows;
		}
	}
	else
	{
		/* establish cursor */
		if (RDBI_SUCCESS == (rdbi_status = odbcdr_est_cursor (context, (char **)&c)))
		{
			rdbi_status = local_odbcdr_sql (context, (char *)c, sql, FALSE, NULL, (void *)NULL, (char *) NULL);
			/* parse command */
			if ( RDBI_SUCCESS == rdbi_status )
			{
				if (RDBI_SUCCESS == (rdbi_status = odbcdr_execute( context, (char *)c, 1, 0, &rows )))
				{
					if ( rows_processed ) 
						(*rows_processed) = rows;
				}
			}

			ODBCDR_ERRORINFO_GET;
			odbcdr_fre_cursor (context, (char **)&c);
			ODBCDR_ERRORINFO_SET;
		}
	}

    debug_return(NULL, rdbi_status );
}

int odbcdr_run_sql (
    odbcdr_context_def *context,
    const char *sql,
	int isddl,
    int *rows_processed
)
{
    rdbi_string_def str;
    str.ccString = sql;
    return local_odbcdr_run_sql(context, &str, isddl, rows_processed);
}

int odbcdr_run_sqlW (
    odbcdr_context_def *context,
    const wchar_t *sql,
	int isddl,
    int *rows_processed
)
{
    rdbi_string_def str;
    str.cwString = sql;
    return local_odbcdr_run_sql(context, &str, isddl, rows_processed);
}

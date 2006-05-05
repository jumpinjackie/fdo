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

int odbcdr_execute_direct(						/* execute an SQL statement		  */
    odbcdr_context_def  *context,
	char	  *sql,
	wchar_t   *sqlW, 					/* cursor associated with SQL stmnt */
	int    *rows_processed	 			/* # rows processed by this exec	*/
	);

static int local_odbcdr_run_sql (
    odbcdr_context_def *context,
    char *sql,
	wchar_t *sqlW,
	int isddl, 
    int *rows_processed
)
{
    odbcdr_cursor_def    *c;
    int                 rdbi_status = RDBI_GENERIC_ERROR;
    int                 rows;
    ODBCDR_ERRORINFO_VARS;
    debug_on1("odbcdr_run_sql", "sql: %.250s ", sql );

	if (isddl)
	{
		if (RDBI_SUCCESS == (rdbi_status = odbcdr_execute_direct(context, sql, sqlW, &rows)))
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
			if( sql != NULL )
				rdbi_status = odbcdr_sql (context, (char *)c, sql, FALSE, NULL, (void *)NULL, (char *) NULL);
			else
				rdbi_status = odbcdr_sqlW (context, (char *)c, sqlW, FALSE, NULL, (void *)NULL, (char *) NULL);
			/* parse command */
			if ( RDBI_SUCCESS == rdbi_status )
			{
				if (RDBI_SUCCESS == (rdbi_status = odbcdr_execute( context, (char *)c, 1, 0, &rows )))
				{
					if ( rows_processed ) 
						(*rows_processed) = rows;
				}
			}

			ODBCDR_ERRORINFO_GET(context);
			odbcdr_fre_cursor (context, (char **)&c);
			ODBCDR_ERRORINFO_SET(context);
		}
	}

    debug_return(NULL, rdbi_status );
}

int odbcdr_run_sql (
    odbcdr_context_def *context,
    char *sql,
	int isddl, /* not used */
    int *rows_processed
)
{
	return local_odbcdr_run_sql( context, sql, NULL, isddl, rows_processed );
}

int odbcdr_run_sqlW (
    odbcdr_context_def *context,
    wchar_t *sql,
	int isddl, /* not used */
    int *rows_processed
)
{
	return local_odbcdr_run_sql( context, NULL, sql, isddl, rows_processed );
}

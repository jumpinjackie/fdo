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
* Name																	*
*	odbcdr_fetch - Fetch the next row[s] from the specified cursor		*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	int odbcdr_fetch(cursor, count, rows_processed)						*
*	char		*cursor;												*
*	int 		 count; 												*
*	int 		*rows_processed;										*
*																		*
* Description															*
*		Fetch  the	next  'count'  row[s] from the specified open		*
*		cursor.  The retrieved data will  be  put  into  the data		*
*		locations specified  for  each	selected  column by prior		*
*		calls  to  odbcdr_define().	 If this is  an  array	fetch		*
*		(count	>  1),	the  define'd  addresses  must	point  to		*
*		sufficient space for arrays of column data. 					*
*																		*
*		The cursor must have  an  associated  SQL  statement (via		*
*		odbcdr_sql()).  It must have *all* of the selected columns		*
*		define'd	to	  user	   specified	addresses	 (via		*
*		odbcdr_define()'s).	 If there are  bind  variables,  they		*
*		must  be   bound   to	user   specified  addresses  (via		*
*		odbcdr_bind()).	 And the cursor must have  been  executed		*
*		via odbcdr_execute().											*
*																		*
* Parameters															*
*	cursor: 	input													*
*		Pointer to the relevant open cursor context area.				*
*																		*
*	count:		input													*
*		An integer	count  of  the	number	of rows to be fetched		*
*		before	returning.	Oracle array processing is supported.		*
*		A count  >	1  makes  sense  only  if the define'd select		*
*		column	addresses  point  to  room	for arrays of  column		*
*		values.  Usually 1. 											*
*																		*
*	offset: 	input													*
*		An integer (base 0) offset into  the array[s] of define'd		*
*		variables  at which to start placing the  fetched  column		*
*		values.  Usually 0. 											*
*																		*
*	rows_processed: output												*
*		A pointer  to  an  integer where we can put the number of		*
*		rows  fetched  so  far,  ie.	cumulatively,  from  this		*
*		cursor. 														*
*																		*
* Function Value														*
*		An RDBI status integer.  Good is RDBI_SUCCESS (ie 0).  At		*
*		end-of-fetch,  RDBI_END_OF_FETCH  will	be returned.  See		*
*		inc/rdbi.h														*
*																		*
************************************************************************/

#include <Inc/ut.h> 					/* ut_vm_malloc()			*/
#include <Inc/rdbi.h>					/* rdbi status values		*/

#include "proto_p.h"
#include	<Inc/debugext.h>
#include <time.h>

#include <assert.h>

#ifdef _WIN32
#include "odbcss.h"
#endif


int odbcdr_fetch(								/* fetch rows from oracle	 */
    odbcdr_context_def  *context,
	char                *cursor,				/* cursor context area		*/
	int                 count,					/* # rows requested 		*/
	int                 *rows_processed			/* # rows actually returned */
	)
{
	return odbcdr_fetch2( context, cursor, count, FALSE, FALSE, rows_processed );
}

int odbcdr_fetch2(
    odbcdr_context_def  *context,
	char                *cursor,		/* cursor context area		*/
	int                 count,			/* # rows requested 		*/
	int		            do_exec,
	int		            do_cancel,
	int                 *rows_processed	/* # rows actually returned */
	)
{
	odbcdr_cursor_def	*c;
	SQLRETURN			rc;			/* ODBC return value */
	int 				rdbi_status = RDBI_SUCCESS;
	int					rows_in_last_fetch;

	debug_on4("odbcdr_fetch2", "c:%#x count: %d %sexec %scancel",
			cursor, count, do_exec?"do ":"no ", do_cancel?"do ":"no ");

	ODBCDR_RDBI_ERR( odbcdr_get_cursor(context, cursor, &c ) );
	assert(count <= ODBCDR_MAX_ARRAY_SIZE);

    if ( c->defined_geometries != NULL )
    {
        /* Free any Oracle objects from previous executions/fetches. */
        ODBCDR_RDBI_ERR( odbcdr_geom_freeSqlServerGeometries( context, c ) );

        /* Set array size to exactly that which is requested. */
        ODBCDR_RDBI_ERR( odbcdr_geom_setNumRows( context, c, count ) );

    }   /* end if geometries */

	//Set the fetch size
	SQLSetStmtAttr( c->hStmt,
					SQL_ATTR_ROW_ARRAY_SIZE,
					(PTR)count,
					SQL_IS_INTEGER);

	//Enable retreival of rows_processed
	SQLSetStmtAttr( c->hStmt,
					SQL_ATTR_ROWS_FETCHED_PTR,
					&rows_in_last_fetch,
					SQL_IS_POINTER);

	// if using server cursors and if end of fetch, the value of rows
	// in last fetch will be garbage. Set it to 0 to get rid of the garbage
	rows_in_last_fetch = 0;

	if ( do_exec )
	{
		debug0("Running execfetch");
#ifdef _WIN32
		// Turn on autofetch
		SQLSetStmtAttr( c->hStmt,
					SQL_SOPT_SS_CURSOR_OPTIONS,
					(PTR)SQL_CO_AF,
					SQL_IS_INTEGER);
#endif

		rdbi_status = odbcdr_execute( context, cursor, 1, 0, rows_processed ); 
 
#ifdef _WIN32
		// Turn off autofetch
		SQLSetStmtAttr( c->hStmt,
					SQL_SOPT_SS_CURSOR_OPTIONS,
					(PTR)SQL_CO_OFF,
					SQL_IS_INTEGER);

		//Make sure we are still using fast forward cursors
		SQLSetStmtAttr( c->hStmt,
					SQL_SOPT_SS_CURSOR_OPTIONS,
					(PTR)SQL_CO_FFO,
					SQL_IS_INTEGER);
#endif

		if ( rdbi_status != RDBI_SUCCESS &&
			rdbi_status != RDBI_END_OF_FETCH)
		{
			goto the_exit;
		}

		if (rdbi_status == RDBI_END_OF_FETCH)
		{
			SQLCloseCursor(c->hStmt);
		}

	}

	if (!do_exec){ // if do_exec was true, the fetch was already done in the call to odbcdr_execute()
		rc = SQLFetch( c->hStmt);

		if ( rc != SQL_SUCCESS &&
			 rc != SQL_SUCCESS_WITH_INFO &&
			 rc != SQL_NO_DATA)
		{
			debug0( "SQLFetch FAILED!" );
			rdbi_status = odbcdr_xlt_status( context, rc, SQL_HANDLE_STMT, c->hStmt);
			context->odbcdr_last_rc = rc;
			goto the_exit;
		}
		else
		{
			rdbi_status = odbcdr_xlt_status( context, rc, SQL_HANDLE_STMT, c->hStmt );
			debug0( "ODBCStmtFetch succeeded." );
			if (rc == SQL_NO_DATA)
			{
				SQLCloseCursor(c->hStmt);
			}
		}
	}

	/* 
	** In ODBC rows_processed is only for the LAST fetch 
	** (In OCI, rows_processed is for ALL previous fetches)
	** We need a cumulative total in c->cumul_rows_fetched
	*/
	*rows_processed = c->cumul_rows_fetched + rows_in_last_fetch;
	c->cumul_rows_fetched = *rows_processed;

	debug2( "%d rows fetch (cumul %ld)", rows_in_last_fetch, c->cumul_rows_fetched );

    /* Translate geometry columns from SqlServer . */
    if ( c->defined_geometries != NULL && rows_in_last_fetch > 0 )
    {
        if ( rows_in_last_fetch > ODBCDR_MAX_GEOM_ROWS )
        {
	//		char buffer[256];
	//		sprintf(buffer, "TOO MANY GEOMETRY ROWS: %d (max is %d)", rows_in_last_fetch, ODBCDR_MAX_GEOM_ROWS);
	//		msg_issue(buffer, 'e');
            goto the_exit;
        }

        /* Reduce array size to exactly that which is populated. */
        ODBCDR_RDBI_ERR( odbcdr_geom_setNumRows( context, c, rows_in_last_fetch ) );

        ODBCDR_RDBI_ERR( odbcdr_geom_convertDefinedFromSqlServer(context, c, rows_in_last_fetch));

    }   /* end if geometries */


the_exit:
debug_area() odbcdr_show_context( context, c );
	debug_return(NULL, rdbi_status );
}


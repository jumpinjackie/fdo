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
*	ocidr_est_cursor - Allocate and initialize a cursor context area	*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	int ocidr_est_cursor(cursor)										*
*	char		**cursor;												*
*																		*
* Description															*
*		Allocate  and initialize the contents of a cursor context		*
*		area to be used to process SQL statements.						*
*																		*
* Parameters															*
*	cursor: 	output													*
*		This is  the address of a place to put the address of the		*
*		allocated and initialized cursor context area.					*
*																		*
* Function Value														*
*		An RDBI status integer.  Good is RDBI_SUCCESS (ie 0).			*
*																		*
* Remarks																*
*																		*
*************************************************************************/

#include <Inc/ut.h>						/* ut_vm_malloc()			*/
#include <Inc/rdbi.h>					/* rdbi status values		*/
#include	<Inc/debugext.h>

#include "proto_p.h"

static odbcdr_cursor_def *extract_free_cursor( odbcdr_connData_def *connData,
											  unsigned long apiversion );


/*
 * Malloc an ODBC statement handle structure and open it.
 */
int odbcdr_est_cursor(
    odbcdr_context_def *context,
	char	**cursor
	)
{
	odbcdr_cursor_def	*c;
	odbcdr_connData_def	*connData;
	int 				rdbi_status = RDBI_GENERIC_ERROR;

	debug_on( "ocidr_est_cursor" );

	c = (odbcdr_cursor_def *)NULL;

	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

	/* 
	** allocate and initialize the cursor
	*/
	*cursor = (char *) odbcdr_crt_cursor(context, (odbcdr_cursor_def *)NULL);

	c = (odbcdr_cursor_def *)(*cursor);

	if( *cursor == (char *)NULL ) {
		rdbi_status = RDBI_MALLOC_FAILED;
		goto the_exit;
	}

	// initialize the cursor values
	ODBCDR_ODBC_ERR( SQLAllocHandle(SQL_HANDLE_STMT,
						connData->hDbc,
						&(c->hStmt)),
					SQL_HANDLE_DBC, connData->hDbc,
				   "SQLAllocHandle", "statement" );

	c->next = connData->cursors; // linked list management
	connData->cursors = c;
	c->is_rollback = FALSE;	

	rdbi_status = RDBI_SUCCESS;

the_exit:
	debug_area() odbcdr_show_context( context, c );
	debug1("Returning cursor c:%#p", *cursor);
	debug_return(NULL, rdbi_status );
}


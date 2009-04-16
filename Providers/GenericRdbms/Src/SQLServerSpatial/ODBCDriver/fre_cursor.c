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
*	odbcdr_fre_cursor - Free up a cursor (deassign it)					*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	int odbcdr_fre_cursor(cursor)										*
*	char		**cursor;												*
*																		*
* Description															*
*		Close  a  specified cursor context area and then make it		*
*		available for reassignment. 									*
*																		*
* Parameters															*
*	cursor: 	input/output											*
*		This is the address  of the address of the cursor context		*
*		area to be closed and  made  available	for reassignment.		*
*		If the cursor is successfully closed, the address  of the		*
*		cursor will be set to NULL. 									*
*																		*
* Function Value														*
*		An	RDBI  status  integer.	 Good is RDBI_SUCCESS (ie 0).		*
*		See inc/rdbi.h													*
*																		*
* Remarks																*
*		This also frees up any	memory	allocated  to support the		*
*		operations done within the given cursor context.				*
*																		*
************************************************************************/

#include <Inc/ut.h>
#include <Inc/rdbi.h>					/* rdbi status values		*/
#include	<Inc/debugext.h>

#include "proto_p.h"

int odbcdr_fre_cursor(
    odbcdr_context_def  *context,
	char   **cursor
	)
{
	odbcdr_cursor_def	*c;
	odbcdr_connData_def	*connData;
	odbcdr_cursor_def	*parent_cursor; 	/* For traversing cursor list	*/
	SQLRETURN			rc;
	int					rdbi_status = RDBI_GENERIC_ERROR;
	//int					i;

	debug_on1("odbcdr_fre_cursor", "s:0x%p", *cursor);

	ODBCDR_RDBI_ERR( odbcdr_get_cursor( context, *cursor, &c ) );

	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData) );

	/* Remove this cursor from the connection's linked list of cursors */
	parent_cursor = connData->cursors;
	if( parent_cursor == c ) {
		connData->cursors = c->next;
	} else {
		while( parent_cursor != (odbcdr_cursor_def *)NULL &&
			   parent_cursor->next != c ) {

			parent_cursor = parent_cursor->next;
		}
		if( parent_cursor != (odbcdr_cursor_def *)NULL ) {
			parent_cursor->next = c->next;
		}
		else {
            // ERROR: Cursor not found
            // Needs a message catalog...
			//nls_msg_issue('s', MF_OCIDR, S_OCIDR_1, OCIDR_1, "Cursor not found.");
			goto the_exit;
		}
	}

	/*
	** Free the statement handle
	*/
	rc = SQLFreeHandle(SQL_HANDLE_STMT, c->hStmt);
	if ( rc != ODBCDR_SUCCESS )
	{
		debug0( "SQLFreeHandle for cursor FAILED!" );
		/* Don't exit, even if error. */
		rdbi_status = odbcdr_xlt_status( context, rc, SQL_HANDLE_STMT, c->hStmt );
	}
	c->hStmt = NULL;

	c->cumul_rows_fetched = 0;

	/*
	** Deallocate the string memory
	*/
	if(c->sqlstring != NULL) {
		ut_vm_free( _db_function, c->sqlstring );
		c->sqlstring = NULL;
	}

    // Free the working buffer
    if ( c->odbcdr_geom != NULL )
        ut_vm_free( _db_function, c->odbcdr_geom );

    if ( c->odbcdr_geomNI != NULL )
        ut_vm_free( _db_function, c->odbcdr_geomNI );

    // Free the working buffer
    if ( c->odbcdr_blob_tmp != NULL )
        ut_vm_free( _db_function, c->odbcdr_blob_tmp );

	// Free the geometry buffers
    ODBCDR_RDBI_ERR( odbcdr_geom_freeAllColumns( context, c ) );   

	ut_vm_free( _db_function, (char *)c );
	c = NULL;

	*cursor = (char *)NULL;
	rdbi_status = RDBI_SUCCESS;

    // Sometimes the last error message is retrieved after the cursor is closed.
    // Clearing the error code means to inhibit reading the error message.
	//context->odbcdr_last_rc = ODBCDR_SUCCESS;

the_exit:
	debug_return(NULL, rdbi_status );
}

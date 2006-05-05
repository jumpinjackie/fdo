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
*	odbcdr_disconnect - Disconnect from the specified ODBC connection	*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	int odbcdr_disconnect(vendor_data)									*
*	char		**vendor_data;											*
*																		*
* Description															*
*		Disconnect from the specififed connection to a user space		*
*		in an SQL Server DB. 											*
*																		*
* Parameters															*
*	vendor_data:	update												*
*		This  is  the  address	of	a  vendor specific connection		*
*		context area as returned to the caller at odbcdr_connect()		*
*		time.	 This  connection  context	area  is  marked   as		*
*		available, once the disconnection is done.						*
*																		*
* Function Value														*
*		An RDBI  status  integer.	Good  is RDBI_SUCCESS (ie 0).		*
*		See inc/rdbi.h													*
*																		*
* Remarks																*
*		After disconnect we are connected to nothing.  The caller		*
*		must odbcdr_switch() to some other connection before doing		*
*		anything else.													*
*																		*
************************************************************************/

#include <Inc/ut.h> 					/* ut_vm_malloc()			*/
#include <Inc/rdbi.h>					/* rdbi status values		*/
#include	<Inc/debugext.h>
#include "proto_p.h"


#define _check_status2() 	if( 		rdbi_status != RDBI_SUCCESS 	\
									 && first_error == RDBI_SUCCESS )	\
								first_error = rdbi_status;

int odbcdr_disconnect(					/* terminate Oracle connection	*/
    odbcdr_context_def *context,
	char **vendor_data	 				/* Oracle-specific login data	*/
	)
{
	odbcdr_connData_def	*connData;
	odbcdr_cursor_def	*next_cursor;
	int 				first_error = RDBI_SUCCESS;
	int 				rdbi_status = RDBI_GENERIC_ERROR;
	SQLRETURN			rc;

	debug_on2( "odbcdr_disconnect", "conn# %d, lda 0x%p",
		context->odbcdr_current_connect, context->odbcdr_conns[context->odbcdr_current_connect]  );

	if( context->odbcdr_current_connect == -1
			|| context->odbcdr_conns[context->odbcdr_current_connect] == (odbcdr_connData_def *)NULL ) {
		rdbi_status = RDBI_NOT_CONNECTED;
		goto the_exit;
	}

	rdbi_status = odbcdr_commit( context, RDBI_COMMIT_ONCE );
	if( rdbi_status != RDBI_SUCCESS )
		goto the_exit;

	/* Free any associated cursors that are still hanging around on this
	 * database connection.  Routines that call this function are expected
	 * to have little error recovery (like, what are they supposed to do?),
	 * so a odbcdr_fre_cursor() failure or five will not stop us from
	 * getting as much done as possible.
	 */

	connData = context->odbcdr_conns[context->odbcdr_current_connect];

	if( connData->identity != (odbcdr_cursor_def *)NULL ) {
		rdbi_status = odbcdr_fre_cursor( context, (char **)&connData->identity );
		_check_status2();
		connData->identity = (odbcdr_cursor_def *)NULL;
	}

	if( connData->cols != (odbcdr_cursor_def *)NULL ) {
		rdbi_status = odbcdr_col_deac(context);
		_check_status2();
		rdbi_status = odbcdr_fre_cursor( context, (char **)&connData->cols );
		_check_status2();
		connData->cols = (odbcdr_cursor_def *)NULL;
	}
	if( connData->users != (odbcdr_cursor_def *)NULL ) {
		rdbi_status = odbcdr_users_deac(context);
		_check_status2();
		rdbi_status = odbcdr_fre_cursor( context, (char **)&connData->users );
		_check_status2();
		connData->users = (odbcdr_cursor_def *)NULL;
	}
	if( connData->usr_exists != (odbcdr_cursor_def *)NULL ) {
		rdbi_status = odbcdr_fre_cursor( context, (char **)&connData->usr_exists );
		_check_status2();
		connData->usr_exists = (odbcdr_cursor_def *)NULL;
	}

	do {
		next_cursor = connData->cursors;
		while( next_cursor != (odbcdr_cursor_def *)NULL ) {
			next_cursor = connData->cursors->next;
			rdbi_status = odbcdr_fre_cursor( context, (char **)&connData->cursors );
			connData->cursors = next_cursor;
			_check_status2();
		}
		//CMB: don't implement the free list yet
		connData->cursors = NULL;
	} while(connData->cursors);

	debug_area() odbcdr_show_context( context, (odbcdr_cursor_def *)NULL );

	/* Now get off of the current database	*/
	rc = SQLDisconnect(connData->hDbc);
	/* Release the conenction handle */
	rc = SQLFreeHandle(SQL_HANDLE_DBC, connData->hDbc);

	ut_vm_free( _db_function, (char *)context->odbcdr_conns[context->odbcdr_current_connect] );

	context->odbcdr_conns[context->odbcdr_current_connect] = (odbcdr_connData_def *)NULL;

	context->odbcdr_current_connect = -1;
	context->odbcdr_connect_count -- ;
	rdbi_status = first_error;
	context->odbcdr_last_rc = ODBCDR_SUCCESS;

the_exit:
	debug_return(NULL, rdbi_status);
}

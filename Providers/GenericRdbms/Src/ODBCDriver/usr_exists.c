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
*	odbcdr_usr_exists - Check if user exists.							*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	odbcdr_usr_exists(user_name, exists) 								*
*	char *user_name;													*
*	int  *exists;														*
*																		*
* Description															*
*		This  module  checks  to  see  if  a SQL Server database name	*
*		is already in use. 												*
*																		*
* Parameters															*
*	user_name: input													*
*		The user name to check on.										*
*																		*
*	exists: output														*
*		TRUE if the user exists.										*
*																		*
* Function value														*
*	Returns an rdbi status code from inc/rdbi.h 						*
*																		*
************************************************************************/

#include <Inc/rdbi.h>
#include <Inc/ut.h>
#include	<Inc/debugext.h>
#include "proto_p.h"

int odbcdr_usr_exists(
    odbcdr_context_def  *context,
	char *user_name,
	int  *exists
	)
{
	char				 sql_buf[1054];
	int 				 rows;
	odbcdr_cursor_def	*c;
	odbcdr_connData_def	*connData;
    char 	            user_name2[ODBCDR_NAME_SIZE];
    long 	            returned_count2;

	int 				rdbi_status = RDBI_GENERIC_ERROR;

	debug_on1("odbcdr_usr_exists", "user_name '%s'", ISNULL(user_name));


// CMB: THIS NEEDS TO BE CONVERTED TO SQL SERVER
// For now, just go right to the exit
	// odbcdr_usr_exists is not implemented in the SQL Server Driver
	*exists = TRUE;
	goto the_exit;

// CMB: DO NOT EXECUTE ANY OF THIS ON SQL SERVER. It has not been implemented yet

	/* assume user does not exist */
	*exists = FALSE;

	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

	c = connData->usr_exists;

	strcpy(user_name2, user_name);

	ut_to_upper(user_name2);

	if (c == (odbcdr_cursor_def *)NULL) {
		/* establish cursor */
		rdbi_status = odbcdr_est_cursor(context, (char **)&c);
		_check_status;

		connData->usr_exists = c;

		/*
		*************************************************************
		** This has to be converted to a metadata stored procedure call
		** in SQL Server. Also, instead of searching for a username,
		** in SQL Server, we have to search for a database name instead.
		*************************************************************
		*/
		strcpy(sql_buf,
				"select count(*) from sys.user$ where name = :1");

		debug1("%.150s", sql_buf);
		debug_trace( sql_buf, (wchar_t *)NULL, NULL );

		/* parse command */

		rdbi_status = odbcdr_sql( context, (char *)c, sql_buf, FALSE, (char *)NULL,
									(void *)NULL, (char *) NULL);
		_check_status;

		/* bind the input user name    */
		rdbi_status = odbcdr_bind( context, (char *)c, "1", RDBI_STRING, ODBCDR_NAME_SIZE,
											user_name2,(SQLLEN *) NULL);
		_check_status;

		/* define output locations */

		rdbi_status = odbcdr_define( context, (char *)c, "1", RDBI_LONG, sizeof(long),
										(char *) &returned_count2, (SQLLEN *)NULL);
		_check_status;

	}

	/* execute the SQL statement & fetch row */
	rdbi_status = odbcdr_fetch2( context, (char *)c, 1, TRUE, TRUE, &rows);
	_check_status;

	if( returned_count2 != 0 ) {
		*exists = TRUE;
	}

the_exit:
	/*
	*****************************************************************
	** While we are waiting for the metadata query to be converted to a
	** SQL Server version, we will assume the user/database exists, and
	** return RDBI_SUCCESS
	*****************************************************************
	*/
	*exists=TRUE;
	rdbi_status = RDBI_SUCCESS;
	debug_return(NULL, rdbi_status);

}

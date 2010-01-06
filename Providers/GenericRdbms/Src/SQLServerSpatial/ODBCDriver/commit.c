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
*   odbcdr_commit - Commit a transaction or turn autocommit on or off   *
*                                                                       *
* Synopsis                                                              *
*   #include <inc/rdbi.h>                                               *
*   int obdcdr_commit(commit_action)                                    *
*   int         commit_action;                                          *
*                                                                       *
* Description                                                           *
*       Depending on the commit action, either commit the current       *
*       transaction  (which  will free all locks), or either turn       *
*       on or turn off the 'autocommit' mode.                           *
*                                                                       *
* Parameters                                                            *
*   commit_action:  input                                               *
*       Indicate whether the desired action is to:                      *
*           RDBI_COMMIT_ONCE:       Commit the current transaction      *
*           RDBI_COMMIT_AUTO_ON:    Turn autocommit mode on             *
*           RDBI_COMMIT_AUTO_OFF:   Turn autocommit mode off            *
*       See inc/rdbi.h                                                  *
*                                                                       *
* Function Value                                                        *
*       An RDBI status  integer.   Good  is  RDBI_SUCCESS (ie 0).       *
*       See inc/rdbi.h                                                  *
*                                                                       *
* Remarks                                                               *
*                                                                       *
************************************************************************/

#include <Inc/ut.h> 				
#include <Inc/debugext.h>
#include <Inc/rdbi.h>			/* rdbi status values		*/
#include "proto_p.h"


int odbcdr_commit(
    odbcdr_context_def *context,
	int 	commit_action
	)
{
	odbcdr_connData_def	*connData;
	int					rdbi_status = RDBI_GENERIC_ERROR;

	debug_on1("odbcdr_commit", "Commit_action: %d", commit_action);

	if (commit_action != RDBI_COMMIT_ONCE)
		goto the_exit;
 
	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

	ODBCDR_ODBC_ERR( SQLEndTran(SQL_HANDLE_DBC,
								connData->hDbc,
								SQL_COMMIT),
								SQL_HANDLE_DBC, connData->hDbc,
								"SQLEndTran", "commit");
	rdbi_status = RDBI_SUCCESS;
	
the_exit:

	debug_return(NULL, rdbi_status);
}

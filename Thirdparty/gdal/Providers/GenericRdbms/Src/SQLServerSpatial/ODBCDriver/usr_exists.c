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
	int 				rdbi_status = RDBI_GENERIC_ERROR;

    // This is an obsolete function.

    *exists=FALSE;
	debug_return(NULL, rdbi_status);

}

int odbcdr_usr_existsW(
    odbcdr_context_def  *context,
	wchar_t *user_name,
	int  *exists
	)
{
	int rdbi_status = RDBI_GENERIC_ERROR;

    // This is an obsolete function.

    *exists=FALSE;
	debug_return(NULL, rdbi_status);
}

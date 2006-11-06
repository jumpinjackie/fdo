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
*																		*
* Name																	*
* 	odbcdr_get_con_var - get the name of the connect environment 		*
*						variable for ODBC.								*
*																		*
* Synopsis																*
*	odbcdr_get_con_var(value)											*
*	char	*value;														*
*																		*
* Description															*
*		This functions returns the name of the connect environment      *
*		variable for SqlServer.  This variable can be set to run in 	*
*		client/server mode.												*
*																		*
* Parameters															*
*	value:	output														*
*		name of the environment variable.								*
*																		*
* Function value														*
*		Always TRUE.													*
*																		*
************************************************************************/

#include    "proto_p.h"
#include	<Inc/debugext.h>
#include    <Inc/ut.h>

int
odbcdr_get_con_var(
    odbcdr_context_def  *context,
	char	*value
	)
{
	debug_on("odbcdr_get_con_var");

	strcpy(value, "TWO_TASK");

	debug_return(NULL, TRUE);
}

int
odbcdr_get_con_varW(
    odbcdr_context_def  *context,
	wchar_t	*value
	)
{
	debug_on("odbcdr_get_con_var");

	wcscpy(value, L"TWO_TASK");

	debug_return(NULL, TRUE);
}


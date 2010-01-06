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
*	odbcdr_vndr_name - Return RDBMS vendor name "sql_server" 			*
*																		*
* Synopsis																*
*	char *odbcdr_vndr_name() 											*
*																		*
* Description															*
*		Return	the  RDBMS	vendor	name.  In this case "sql_server".	*
*																		*
* Parameters															*
*		N/A 															*
*																		*
* Function Value														*
*		Void.															*
*																		*
************************************************************************/

#include "proto_p.h"


char *odbcdr_vndr_name(odbcdr_context_def  *context)
{
	return ("ODBC");
}

wchar_t *odbcdr_vndr_nameW(odbcdr_context_def  *context)
{
	return (L"ODBC");
}

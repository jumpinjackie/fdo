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
*	odbcdr_desc_bind - Return the name of the n'th bound variable.		*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	int odbcdr_desc_bind(cursor, position, name_size, name)				*
*	char		*cursor;												*
*	int 		 position;												*
*	int 		 name_size;												*
*	char		*name;													*
*																		*
* Description															*
*		Return	 the  name	of	n'th  the  bound  variable	of	a		*
*		previously prepared SQL statement.								*
*																		*
* Parameters															*
*	cursor: 	input													*
*		Pointer to the relevant cursor context area.  This cursor		*
*		must have  had	an	SQL statement assodbcated with it by a		*
*		prior call to odbcdr_sql().	Also, that SQL statement must		*
*		have  some	references	to bind  variables	of	the  form		*
*		':<n>'. 														*
*																		*
*	position:	input													*
*		A 1-based specification of	which bind variable's name to		*
*		return.  Note that this is integer, not ascii as it is in		*
*		odbcdr_bind() and odbcdr_define().								*
*																		*
*	name_size:	input													*
*		The maximum length of the name to be returned plus the NUL		*
*		terminator.  The size of the available area pointed to  by		*
*		the next parameter.												*
*																		*
*	name:		output													*
*		A  pointer to a place to put the  name	of	the  selected		*
*		bound variable.  Today, the typical result will be "1" or		*
*		"5".   Someday,  the  result  may  be  bound  names  like		*
*		"feat_num". 													*
*																		*
* Function Value														*
*		An RDBI  status  integer.	Good  is RDBI_SUCCESS (ie 0).		*
*		See  inc/rdbi.h.   If the position specified  is  greater		*
*		than  the number  of  bind	variable  references  in  the		*
*		assodbcated SQL	statement,	RDBI_NOT_IN_DESC_LIST will be		*
*		returned.														*
*																		*
* Remarks																*
*		If the name is	too  long  to  fit,  it  will not be null		*
*		terminated.  Good luck. 										*
*																		*
*		Note  that	if the same variable is bound multiple times,		*
*		it is possible	to get back the same name multiple times.		*
*																		*
************************************************************************/

#include <Inc/rdbi.h>					/* rdbi status values		*/
#include	<Inc/debugext.h>
#include <stdio.h>
#include "proto_p.h"

int odbcdr_desc_bind(
    odbcdr_context_def *context,
	char *cursor,		/* RDBI work area 				*/
	int   position, 	/* position of bind variable	*/
	int   name_size, 	/* The maximum size of name 	*/
	char *name	 		/* Name of this bind variable	*/
	)
{
	odbcdr_cursor_def	*c;
	//int 				sql_success = FALSE;
	int 				rdbi_status = RDBI_GENERIC_ERROR;
	SQLSMALLINT			num_params;
	short				precision = -1;
 
	debug_on3("odbcdr_desc_bind", "c:%#x Position: %d, max name length %d",
												cursor, position, name_size);

	ODBCDR_RDBI_ERR( odbcdr_get_cursor( context, cursor, &c ) );

	SQLNumParams(c->hStmt, &num_params);

	if (position > num_params) 
		debug_return(NULL, RDBI_NOT_IN_DESC_LIST);

    sprintf( name, "%d", position );

	rdbi_status = RDBI_SUCCESS;

the_exit:
	debug_return(NULL, rdbi_status);
}

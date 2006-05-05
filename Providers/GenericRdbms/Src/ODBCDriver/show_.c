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
*	odbcdr_show_context	 - show context of odbcdr library				*
*																		*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	void	odbcdr_show_alignment()										*
*	void	odbcdr_show_context( c ) 									*
*		odbcdr_cursor_def *c;											*
*	void	odbcdr_show_context( c ) 									*
*		odbcdr_cursor_def *list_head;									*
*																		*
* Description															*
*		These  internal  functions	are  for  debugging the odbcdr		*
*		library.														*
*																		*
*		odbcdr_show_alignment()	shows	how  some  ORACLE-related		*
*		structures got compiled  by  showing  the  offsets of the		*
*		structure members.												*
*																		*
*		odbcdr_show_context()  prints  out  the	current  database		*
*		name, and,	if	specified,	the  current  cursor  number.		*
*																		*
* Parameters															*
*	c:			input													*
*		A pointer to the caller's cursor.  If the caller does not		*
*		deal with a cursor, this pointer may be NULL.					*
*	list_head:	input													*
*		A pointer to the cursor which is the head of a linked list.		*
*																		*
* Function Value														*
*		void															*
*																		*
************************************************************************/

#include <Inc/rdbi.h>					/* rdbi status values		*/
#include	<Inc/debugext.h>
#include "proto_p.h"


void odbcdr_show_context(
    odbcdr_context_def  *context,
	odbcdr_cursor_def	*c
	)
{
	debug_on1("odbcdr_show_context", "cursor 0x%lx", (long) c );

	if( context->odbcdr_current_connect == -1
			|| context->odbcdr_conns[context->odbcdr_current_connect] == (odbcdr_connData_def *)NULL )
	{
		debug1("  conn# %d, no current db", context->odbcdr_current_connect);
	}
	else
	{
		if( c == (odbcdr_cursor_def *)NULL )
		{
			debug2("  conn# %d, db '%s'", context->odbcdr_current_connect,
				context->odbcdr_conns[context->odbcdr_current_connect]->db_name);
		}
		else
		{
			debug3("  conn# %d, db '%s', curs v8+ 0x%lx",
					context->odbcdr_current_connect,
					context->odbcdr_conns[context->odbcdr_current_connect]->db_name,
					c->hStmt );
		}
	}

	debug_return_void(NULL);
}

void odbcdr_show_cursor_list(
    odbcdr_context_def  *context,
	odbcdr_cursor_def	*list_head
	)
{
	odbcdr_cursor_def	*c = list_head;
	debug_on1("odbcdr_show_cursor_list", "head 0x%lx", (long) c );

	while ( c != NULL )
	{
		debug2( "cursor 0x%lx, next 0x%lx", (long) c, (long) c->next );
		c = c->next;
	}

	debug_return_void(NULL);
}

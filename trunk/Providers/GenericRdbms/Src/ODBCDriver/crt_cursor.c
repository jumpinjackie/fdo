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
*	odbcdr_crt_cursor - Create but do not establish a statement.		*
*																		*
* Synopsis																*
*	odbcdr_crt_cursor()													*
*																		*
* Description															*
*		Allocate  and initialize the contents of a cursor context		*
*		area to be used to process SQL statements.  Does NOT establish	*
*		a cursor (mallocs and linked list management).					*
*																		*
* Function value														*
*		Returns a pointer to a odbcdr_cursor_def structure.				*
*																		*
************************************************************************/

#include <Inc/ut.h>
#include <Inc/rdbi.h>
#include <Inc/debugext.h>
#include "proto_p.h"

odbcdr_cursor_def *odbcdr_crt_cursor(
    odbcdr_context_def *context,
	odbcdr_cursor_def	*old_cursor
	)
{
	odbcdr_cursor_def	*c;

	debug_on1("odbcdr_crt_cursor", "c: 0x%p", old_cursor);

	if(old_cursor)
		c = old_cursor;
	else {
		c = (odbcdr_cursor_def *)
			ut_vm_malloc( "odbcdr_crt_cursor", sizeof( odbcdr_cursor_def ) );
		memset((char *)c, '\0', sizeof(odbcdr_cursor_def)); // cmb
	}

	if( c == (odbcdr_cursor_def *)NULL ) goto the_exit;

	// Initialize all struct elements
	c->sqlstring		    = (char *)NULL;
	c->hStmt                = (odbcdr_cursor_handle_def) NULL;
	c->next				    = (odbcdr_cursor_def *)NULL;
	c->cumul_rows_fetched   = 0;
	c->is_rollback		    = FALSE;
    c->odbcdr_blob_tmp      = NULL;
    c->odbcdr_blob_tmp_size = 0;

the_exit:
	debug_return (NULL,c);
}

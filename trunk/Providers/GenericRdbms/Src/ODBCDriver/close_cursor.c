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

#include <Inc/rdbi.h>					/* rdbi status values		*/
#include <Inc/ut.h>							/* ut_vm_malloc()			*/
#include	<Inc/debugext.h>
#include "proto_p.h"


int odbcdr_close_cursor(						
    odbcdr_context_def  *context,
	char   *cursor					
	)
{
	odbcdr_cursor_def		*c;
	odbcdr_connData_def		*connData;
	int 					rdbi_status = RDBI_GENERIC_ERROR;

	debug_on1("odbcdr_close_cursor", "c:%#p", cursor);

	ODBCDR_RDBI_ERR( odbcdr_get_cursor( context, cursor, &c ) );
	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

	SQLCloseCursor(c->hStmt);

    rdbi_status = RDBI_SUCCESS;

the_exit:
	debug_area() odbcdr_show_context( context, c );
	debug1("Returning cursor c:%#p", cursor);
	debug_return(NULL, rdbi_status );
}

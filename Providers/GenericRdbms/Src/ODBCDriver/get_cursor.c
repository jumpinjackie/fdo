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
*
* Name
*	odbcdr_get_cursor - get cursor
*
* Synopsis
*
*	#include "constants.h"
*	#include "proto_p.h"
*	#include <Inc/rdbi.h>
*
*	int odbcdr_get_cursor( char *cursorOpaque_I,
*						  ocidr_cursor_def **cursorStruct_O );
*
* Description
*
*		This function converts a character pointer (the result of
*		an earlier call to ocidr_est_cursor() or ocidr_est_cursor_obj())
*		to a cursor structure.  Calling this function will also have
*		the side effect of makeing sure that the current database'
*		connection context is for the correct version (7 or 8) of OCI.
*
* Parameters
*
*		cursorOpaque_I: An opaque pointer, really to a cursor.
*
*		cursorStruct_O: Pointer to the non-opaque cursor structure.
*
* Function value
*
*	On success, RDBI_SUCCESS.
*	On failure, it will return one of several error RDBI_* error codes.
*	See inc/rdbi.h.
*
************************************************************************/

#include <Inc/rdbi.h>					/* rdbi status values		*/
#include	<Inc/debugext.h>
#include "context.h"
#include "proto_p.h"


int odbcdr_get_cursor(
    odbcdr_context_def  *context,
	char				*cursorOpaque_I,
	odbcdr_cursor_def   **cursorStruct_O
	)
{
	odbcdr_cursor_def		*c;
	int					rdbi_status = RDBI_GENERIC_ERROR;

	debug_on1( "odbcdr_get_cursor", "opaque=0x%lx", (long)cursorOpaque_I );

	if ( cursorOpaque_I == NULL ) goto the_exit;

	c = (odbcdr_cursor_def *) cursorOpaque_I;

	*cursorStruct_O = c;

	rdbi_status = RDBI_SUCCESS;

  the_exit:
	debug_return( NULL, rdbi_status );
}

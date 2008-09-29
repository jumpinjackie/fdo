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
*	odbcdr_get_curr_conn - get current connection data area
*
* Synopsis
*
*	#include "proto_p.h"
*
*	int odbcdr_get_curr_conn( char *cursorOpaque_I,
*						  ocidr_cursor_def **cursorStruct_O );
*
* Description
*   Retrieves the ODBC connection data area for the current 
*   database connection.
*
* Parameters
*
*		connData_O: Pointer to the current connection structure.
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


int odbcdr_get_curr_conn( 
    odbcdr_context_def *context, 
    odbcdr_connData_def **connData_O )
{
	int					    rdbi_status = RDBI_GENERIC_ERROR;

	debug_on( "odbcdr_get_curr_conn" );

    if ( context->odbcdr_current_connect == -1 ||
         context->odbcdr_conns[context->odbcdr_current_connect] == (odbcdr_connData_def *)NULL )
    {
        rdbi_status = RDBI_NOT_CONNECTED;
        goto the_exit;
    }

    *connData_O = context->odbcdr_conns[context->odbcdr_current_connect];
    rdbi_status = RDBI_SUCCESS;

the_exit:
	debug_return( NULL, rdbi_status );
}

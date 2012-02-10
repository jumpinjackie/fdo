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
*	odbcdr_switch - Switch to a previously established connection		*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	int odbcdr_switch(connect_id)										*
*			int connect_id; 											*
*																		*
* Description															*
*		Switches to a previously established connection.				*
*																		*
* Parameters															*
*	connect_id: input													*
*		A	number	 that  uniquely  identifies  the  established		*
*		database  connection.	This   number  should  have  been		*
*		obtained from an earlier call to odbcdr_connect().				*
*																		*
* Function Value														*
*		An RDBI status integer.  Good is RDBI_SUCCESS (ie 0).  If		*
*		the connection does not exist, RDBI_NOT_CONNECTED will be		*
*		returned.  See inc/rdbi.h										*
*																		*
************************************************************************/

#include <Inc/ut.h>		 				/* ut_vm_malloc()			*/
#include <Inc/rdbi.h>					/* rdbi status values		*/
#include	<Inc/debugext.h>
#include "proto_p.h"

int odbcdr_switch(
    odbcdr_context_def  *context,
	int connect_id
	)
{
	int rdbi_status = RDBI_GENERIC_ERROR;

	debug_on1( "odbcdr_switch", "connection id = '%d'", connect_id );

	if( connect_id < 0 || connect_id >= RDBI_MAX_CONNECTS ) {
		rdbi_status = RDBI_NOT_CONNECTED;
		goto the_exit;
	}

	if( context->odbcdr_conns[connect_id] != (odbcdr_connData_def *)NULL ) {
		context->odbcdr_current_connect = connect_id;
		rdbi_status = RDBI_SUCCESS;
		context->odbcdr_last_rc = ODBCDR_SUCCESS;
	} else {
		rdbi_status = RDBI_NOT_CONNECTED;
	}

the_exit:
	debug_return(NULL, rdbi_status );
}

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
*	odbcdr_get_msg - Get the message text for last ORACLE RDBMS status	*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	void odbcdr_get_msg(buffer)											*
*	char	*buffer;													*
*																		*
* Description															*
*		Get  the message text associated with the last internally		*
*		kept ORACLE RDBMS status.										*
*																		*
* Parameters															*
*	buffer: 	output													*
*		This is  the  address  of  the	area to place the message		*
*		text.  It must be at least of  RDBI_MSG_SIZE  size.   See		*
*		inc/rdbi.h														*
*																		*
* Function Value														*
*		Void.															*
*																		*
************************************************************************/

#include <Inc/ut.h> 					/* ut_vm_malloc()			*/
#include <Inc/rdbi.h>					/* rdbi status values		*/
#include <Inc/debugext.h>
#include "proto_p.h"

#define MSG_NODBOPEN  "No current open database."
#define MSG_NODBOPENW L"No current open database."
#define MSG_EMPTY  ""
#define MSG_EMPTYW L""

void local_odbcdr_get_msg(odbcdr_context_def  *context, rdbi_string_def *buffer)
{
	int				rs;

	odbcdr_connData_def	*connData;

	debug_on( "odbcdr_get_msg" );

	rs = odbcdr_get_curr_conn(context, &connData);

	if ( rs == RDBI_NOT_CONNECTED )
	{
		ODBCDRV_STRING_COPY_LST( buffer, MSG_NODBOPEN );
	}
	else if ( rs == RDBI_SUCCESS )
	{
        if ( context->odbcdr_last_rc != ODBCDR_SUCCESS ){
			ODBCDRV_STRING_COPY_LST( buffer, context->odbcdr_last_err_msg );
        }else{
            ODBCDRV_STRING_COPY_LST( buffer, MSG_EMPTY );
        }
	}
	debug_return_void(NULL);
}

void odbcdr_get_msg(		/* Get a message describing the last operation	*/
    odbcdr_context_def  *context,
	char *buffer			/* Output buffer								*/
	)
{
    rdbi_string_def str;
    str.cString = buffer;
    return local_odbcdr_get_msg(context, &str);
}

void odbcdr_get_msgW(		/* Get a message describing the last operation	*/
    odbcdr_context_def  *context,
	wchar_t *buffer			/* Output buffer								*/
	)
{
    rdbi_string_def str;
    str.wString = buffer;
    return local_odbcdr_get_msg(context, &str);
}

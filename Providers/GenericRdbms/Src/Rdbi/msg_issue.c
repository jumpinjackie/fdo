// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

/************************************************************************
* Name                                                                  *
*   rdbi_msg_issue - issue a message for the last rdbms operation       *
*                                                                       *
* Synopsis                                                              *
*   void rdbi_msg_issue(severity)                                       *
*   char severity;                                                      *
*                                                                       *
* Description                                                           *
*       Issue a message when an rdbms error has occurred.               *
*                                                                       *
* Parameters                                                            *
*   severity: input                                                     *
*       Argument to pass on to msg_issue().                             *
*                                                                       *
* Function value                                                        *
*       void                                                            *
*                                                                       *
* Remarks                                                               *
*       We  use  the  status of the global: rdbi_last_status.  It       *
*       behaves quite like the UNIX nerror global.                      *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.            *
*                                                                       *
*   FILE_DEPENDENCY(LTCART): WARNING! This code is duplicated in PL/SQL.
************************************************************************/


#ifndef _WIN32
char *rdbi_cat = "fdordbms.cat";
#else
char *rdbi_cat = "RdbmsMsg.dll";
#endif

#include <Inc/Nls/rdbi_msg.h>
#include <Inc/debugext.h>
#include <Inc/rdbi.h>               /* rdbi status values               */
#include <Inc/Rdbi/proto.h>
#include <Inc/Rdbi/context.h>
#include <Inc/ut.h>

extern wchar_t*  Gdbi_nls_msg_get_0( int msg_num, char* default_msg, char* catalog );

extern wchar_t*  Gdbi_nls_msg_get_S( int msg_num, char* default_msg, char* catalog, char* arg1 );

#include "global.h"                 /* rdbi global area                 */


static void msg_set( rdbi_context_def *context, wchar_t *msg )
{
	if( context->last_error_msg != NULL )
	{
		free( context->last_error_msg );
		context->last_error_msg = NULL;
	}
	if( msg != NULL )
		context->last_error_msg = (wchar_t*)malloc( (wcslen( msg )+1) * sizeof(wchar_t) );

	wcscpy( context->last_error_msg, msg );
}

void rdbi_msg_set_0( rdbi_context_def *context, int msg_num, char* default_msg )
{
	msg_set( context, Gdbi_nls_msg_get_0( msg_num, default_msg, MF_RDBI ) );
}

void rdbi_msg_set_S( rdbi_context_def *context, int msg_num, char* default_msg, char* arg1 )
{
	msg_set(  context, Gdbi_nls_msg_get_S( msg_num, default_msg, MF_RDBI, arg1 ) );
}

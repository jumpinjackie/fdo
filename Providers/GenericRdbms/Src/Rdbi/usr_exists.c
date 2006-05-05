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
*                                                                       *
* Name                                                                  *
*   rdbi_usr_exists - Check if user exists.                             *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_usr_exists(user_name, exists)                                  *
*   char *user_name;                                                    *
*   int  *exists;                                                       *
*                                                                       *
* Description                                                           *
*       This  module  checks  to see if a user name is already in       *
*       use.                                                            *
*                                                                       *
* Parameters                                                            *
*   user_name: input                                                    *
*       The user name to check on.                                      *
*                                                                       *
*   exists: output                                                      *
*       TRUE if the user exists.                                        *
*                                                                       *
* Function value                                                        *
*       Returns an rdbi status code from Inc/rdbi.h                     *
*                                                                       *
* Remarks                                                               *
*       Global rdbi_last_status is updated here.                        *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.        *
*                                                                       *
************************************************************************/

#include <Inc/debugext.h>

#include    <Inc/Nls/rdbi_msg.h>
#include "global.h"
#include	<Inc/Rdbi/context.h>

int rdbi_usr_exists(
	rdbi_context_def *context,
    char *user_name,
    int  *exists)
{
    int   status;


    debug_on("rdbi_usr_exists");

    status = (*(context->dispatch.usr_exists))(context->drvr, user_name, exists);

    context->rdbi_last_status = status;

    debug_return(NULL, status);

}


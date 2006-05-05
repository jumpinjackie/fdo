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
*   rdbi_users_act - Activate a fetch of all users for a connection     *
*                                                                       *
* Synopsis                                                              *
*   #include "<Inc/rdbi.h>                                              *
*   rdbi_users_act(rdbi_context_def *context, target);                  *
*                                                                       *
* Description                                                           *
*       This  module  activates  a  fetch  of  all users accessible     *
*       the current connection.                                         *
*                                                                       *
* Parameters															*
*	target: input 		        										*
*		The name of a specific user.  Results will be limited to this   *
*       user if given.  Pass NULL to get all users.                     *
*																		*
* Function value                                                        *
*       Returns an rdbi status code from Inc/rdbi.h                     *
*                                                                       *
* Remarks                                                               *
*       Global rdbi_last_status is updated here.                        *
*                                                                       *
************************************************************************/

#include <Inc/debugext.h>

#include    <Inc/Nls/rdbi_msg.h>
#include "global.h"
#include <Inc/Rdbi/proto.h>
#include <Inc/Rdbi/context.h>

static char     *tran_id = "auto-users";

int rdbi_users_act(
	rdbi_context_def *context,
    const char *target)
{
    int     status;
    int     tran_begun = FALSE;

    debug_on("rdbi_users_act");

    if (context->rdbi_cnct->autocommit_on) {
        rdbi_tran_begin(context, tran_id);
        tran_begun = TRUE;
    }

    status = (*(context->dispatch.users_act))(context->drvr, target);

    context->rdbi_last_status = status;

    /* This code is based on rdbi_col_act, which assumes that prefetches 
    * occur, making it safe to end the transaction
    * without getting fetch across commit problems.  We'll do the same.
    */

    if ( tran_begun ) {
        rdbi_tran_end(context, tran_id);
    }

    debug_return(NULL, status);

}


/************************************************************************
*                                                                       *
* Name                                                                  *
*   rdbi_user_get - Get the next user.                                  *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_user_get(context, name, eof)                                   *
*   rdbi_context_def *context;                                          *
*   char *name;                                                         *
*   int  *eof;                                                          *
*                                                                       *
* Description                                                           *
*       Get the next user in the fetch activated by rdbi_users_act.     *
*                                                                       *
* Parameters                                                            *
*   name: output                                                        *
*       The name of the user.                                           *
*                                                                       *
*   eof: output                                                         *
*       End of fetch flag.                                              *
*                                                                       *
* Function value                                                        *
*       Returns an rdbi status code from Inc/rdbi.h                     *
*                                                                       *
* Remarks                                                               *
*       Global rdbi_last_status is updated here.                        *
*                                                                       *
************************************************************************/

int rdbi_users_get(
	rdbi_context_def *context,
    char *name,
    int  *eof)
{
    int   status;


    debug_on("rdbi_users_get");

    status = (*(context->dispatch.users_get))(context->drvr, name, eof);

    context->rdbi_last_status = status;

    debug_return(NULL, status);

}
/************************************************************************
*                                                                       *
* Name                                                                  *
*   rdbi_users_deac - Dectivate a fetch of all users for an connection  *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_users_deac(context)                                            *
*   rdbi_context_def *context;                                          *
*                                                                       *
* Description                                                           *
*       This  module deactivates  a fetch  of  all users for an         *
*       connection.                                                     *
*                                                                       *
* Parameters                                                            *
*                                                                       *
* Function value                                                        *
*       Returns an rdbi status code from Inc/rdbi.h                     *
*                                                                       *
* Remarks                                                               *
*       Global rdbi_last_status is updated here.                        *
*                                                                       *
************************************************************************/

int rdbi_users_deac(rdbi_context_def *context)
{
    int   status;

    debug_on("rdbi_users_deac");

    status = (*(context->dispatch.users_deac))(context->drvr);

    context->rdbi_last_status = status;

    debug_return(NULL, status);

}





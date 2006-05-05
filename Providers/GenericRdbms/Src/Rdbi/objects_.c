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
*   rdbi_objects_act - Activate a fetch of all users for an owner       *
*                                                                       *
* Synopsis                                                              *
*   #include "<Inc/rdbi.h>                                              *
*   rdbi_objects_act(rdbi_context_def *context, owner, target);         *
*                                                                       *
* Description                                                           *
*       This  module  activates  a  fetch  of  all users accessible     *
*       the current connection.                                         *
*                                                                       *
* Parameters															*
*   owner: input                                                        *
*       The name of the owner.  This is typically a user name.          *
*                                                                       *
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

int rdbi_objects_act(
	rdbi_context_def *context,
    const char *owner,
    const char *target)
{
    int     status;
    int     tran_begun = FALSE;

    debug_on("rdbi_objects_act");

    if (context->rdbi_cnct->autocommit_on) {
        rdbi_tran_begin(context, tran_id);
        tran_begun = TRUE;
    }

    status = (*(context->dispatch.objects_act))(context->drvr, owner, target);

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
*   rdbi_objects_get - Get the next object.                             *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_objects_get(context, name, eof)                                *
*   rdbi_context_def *context;                                          *
*   char *name;                                                         *
*   char *type;                                                         *
*   int  *eof;                                                          *
*                                                                       *
* Description                                                           *
*       Get the next object in the fetch activated by rdbi_objects_act. *
*                                                                       *
* Parameters                                                            *
*   name: output                                                        *
*       The name of the object.                                         *
*                                                                       *
*   type: output                                                        *
*       The type of the object.  One of:                                *
*           "T" (Table)                                                 *
*           "V" (View)                                                  *
*           "I" (Index)                                                 *
*           "S" (Synonym)                                               *
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

int rdbi_objects_get(
	rdbi_context_def *context,
    char *name,
    char *type,
    int  *eof)
{
    int   status;


    debug_on("rdbi_objects_get");

    status = (*(context->dispatch.objects_get))(context->drvr, name, type, eof);

    context->rdbi_last_status = status;

    debug_return(NULL, status);

}
/************************************************************************
*                                                                       *
* Name                                                                  *
*   rdbi_users_deac - Dectivate a fetch of all objects for a user       *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_objects_deac(context)                                          *
*   rdbi_context_def *context;                                          *
*                                                                       *
* Description                                                           *
*       This  module deactivates  a fetch  of  all objects for a user   *
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

int rdbi_objects_deac(rdbi_context_def *context)
{
    int   status;

    debug_on("rdbi_objects_deac");

    status = (*(context->dispatch.objects_deac))(context->drvr);

    context->rdbi_last_status = status;

    debug_return(NULL, status);

}





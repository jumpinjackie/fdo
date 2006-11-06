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
*   rdbi_pkeys_act - Activate a fetch of all primary keys for an object *
*                                                                       *
* Synopsis                                                              *
*   #include "<Inc/rdbi.h>                                              *
*   rdbi_pkeys_act(rdbi_context_def *context, object);                  *
*                                                                       *
* Description                                                           *
*		This module activates a fetch of all primary key column names   *
*       for an object (table or view) in a data source.         	    *
*                                                                       *
* Parameters															*
*	object: input 		        										*
*		The name of a specific object.                                  *
*																		*
* Function value                                                        *
*       Returns an rdbi status code from Inc/rdbi.h                     *
*                                                                       *
* Remarks                                                               *
*       Global rdbi_last_status is updated here.                        *
*                                                                       *
************************************************************************/

#include <Inc/debugext.h>

#include <Inc/Nls/rdbi_msg.h>
#include "global.h"
#include <Inc/Rdbi/proto.h>
#include <Inc/Rdbi/context.h>

static char     *tran_id = "auto-pkeys";

int rdbi_pkeys_act(
	rdbi_context_def *context,
    const char *owner,
    const char *object)
{
    int     status;
    int     tran_begun = FALSE;

    debug_on("rdbi_pkeys_act");

    if (context->rdbi_cnct->autocommit_on) {
        rdbi_tran_begin(context, tran_id);
        tran_begun = TRUE;
    }

    status = (*(context->dispatch.pkeys_act))(context->drvr, owner, object);

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

int rdbi_pkeys_actW(
	rdbi_context_def *context,
    const wchar_t *owner,
    const wchar_t *object)
{
    int     status;
    int     tran_begun = FALSE;

    debug_on("rdbi_pkeys_actW");

    if (context->rdbi_cnct->autocommit_on) {
        rdbi_tran_begin(context, tran_id);
        tran_begun = TRUE;
    }

    status = (*(context->dispatch.pkeys_actW))(context->drvr, owner, object);

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
*   rdbi_pkey_get - Get the next pkey.                                  *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_pkey_get(context, name, eof)                                   *
*   rdbi_context_def *context;                                          *
*   char *name;                                                         *
*   int  *eof;                                                          *
*                                                                       *
* Description                                                           *
*       Get the next key in the fetch activated by rdbi_pkeys_act.      *
*                                                                       *
* Parameters                                                            *
*   name: output                                                        *
*       The name of the primary key column.                             *
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

int rdbi_pkeys_get(
	rdbi_context_def *context,
    char *name,
    int  *eof)
{
    int   status;

    debug_on("rdbi_pkeys_get");

    status = (*(context->dispatch.pkeys_get))(context->drvr, name, eof);

    context->rdbi_last_status = status;

    debug_return(NULL, status);
}

int rdbi_pkeys_getW(
	rdbi_context_def *context,
    wchar_t *name,
    int  *eof)
{
    int   status;

    debug_on("rdbi_pkeys_getW");

    status = (*(context->dispatch.pkeys_getW))(context->drvr, name, eof);

    context->rdbi_last_status = status;

    debug_return(NULL, status);
}
/************************************************************************
*                                                                       *
* Name                                                                  *
*   rdbi_pkeys_deac - Dectivate a fetch of all primary keys             *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_pkeys_deac(context)                                            *
*   rdbi_context_def *context;                                          *
*                                                                       *
* Description                                                           *
*       This  module deactivates  a fetch  of  all primary keys for an  *
*       object.                                                         *
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

int rdbi_pkeys_deac(rdbi_context_def *context)
{
    int   status;

    debug_on("rdbi_pkeys_deac");

    status = (*(context->dispatch.pkeys_deac))(context->drvr);

    context->rdbi_last_status = status;

    debug_return(NULL, status);

}





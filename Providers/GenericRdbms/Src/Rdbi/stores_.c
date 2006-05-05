// Copyright (C) 2006  Autodesk, Inc.
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
*   rdbi_stores_act - Activate a fetch of all datastores (without a connection) *
*                                                                       *
* Synopsis                                                              *
*   #include "<Inc/rdbi.h>                                              *
*   rdbi_users_act(rdbi_context_def *context);                          *
*                                                                       *
* Description                                                           *
*       This  module  activates  a  fetch  of  all datastores without   *
*       making any connection.                                          *
*                                                                       *
* Parameters															*
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

int rdbi_stores_act(
	rdbi_context_def *context)
{
    int     status;

    debug_on("rdbi_stores_act");

    status = (*(context->dispatch.stores_act))(context->drvr);

    context->rdbi_last_status = status;

    debug_return(NULL, status);

}


/************************************************************************
*                                                                       *
* Name                                                                  *
*   rdbi_stores_get - Get the next datastore.                           *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_stores_get(context, name, eof)                                 *
*   rdbi_context_def *context;                                          *
*   char *name;                                                         *
*   int  *eof;                                                          *
*                                                                       *
* Description                                                           *
*       Get the next datastore in the fetch activated by rdbi_stores_act.*
*                                                                       *
* Parameters                                                            *
*   name: output                                                        *
*       The name of the datastore.                                      *
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

int rdbi_stores_get(
	rdbi_context_def *context,
    char *name,
    int  *eof)
{
    int   status;


    debug_on("rdbi_stores_get");

    status = (*(context->dispatch.stores_get))(context->drvr, name, eof);

    context->rdbi_last_status = status;

    debug_return(NULL, status);

}
/************************************************************************
*                                                                       *
* Name                                                                  *
*   rdbi_stores_deac - Dectivate a fetch of all datastores              *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_stores_deac(context)                                           *
*   rdbi_context_def *context;                                          *
*                                                                       *
* Description                                                           *
*       This  module deactivates  a fetch  of  all datstores            *
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

int rdbi_stores_deac(rdbi_context_def *context)
{
    int   status;

    debug_on("rdbi_stores_deac");

    status = (*(context->dispatch.stores_deac))(context->drvr);

    context->rdbi_last_status = status;

    debug_return(NULL, status);

}





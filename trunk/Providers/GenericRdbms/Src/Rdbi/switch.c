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
*   rdbi_switch - Switch between rdbms connection                       *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_switch(connect_id)                                             *
*       int connect_id;                                                 *
*                                                                       *
* Description                                                           *
*       Establish a previously-connected database as the current,       *
*       active   connection.    The   user   must   have   called       *
*       rdbi_connect()  before   switching  to  this  connection.       *
*                                                                       *
* Parameters                                                            *
*   connect_id: input                                                   *
*       A  number  that   uniquely   identifies  the  established       *
*       database  connection.   This  number  should   have  been       *
*       obtained from an earlier call to rdbi_connect().                *
*                                                                       *
* Function value                                                        *
*       Returns an rdbi status code.  Status  definitions  can be       *
*       found in <Inc/rdbi.h>.  No messages issued.                     *
*                                                                       *
* Remarks                                                               *
*       This routine updates the rdbi_last_status global                *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.            *
*                                                                       *
************************************************************************/

extern char *rdbi_cat;

#include <Inc/Nls/rdbi_msg.h>
#include <Inc/debugext.h>
#include "global.h"             /* rdbi global area */
#include	<Inc/Rdbi/context.h>

int
rdbi_switch(
	rdbi_context_def *context,
    int connect_id)
{
    int i;                          /* Index into rdbi_connections array    */
    int new_conn;
    int status;

    debug_on1("rdbi_switch", "connect_id = %d", connect_id);

    status = RDBI_SUCCESS;
    /*
     * Check if they've exceeded the maximum number of connections
     */
    new_conn = -1;
    for (i = 0; i < RDBI_MAX_CONNECTS; i++) {
        if (context->rdbi_connection[i].connect_id == connect_id) {
            new_conn = i;
            break;
        }
    }

    if (new_conn == -1) {
        status = RDBI_NOT_CONNECTED;
        goto the_exit;
    }
    context->rdbi_cnct = &context->rdbi_connection[new_conn];

    status = (*(context->dispatch.dbswitch))(context->drvr, connect_id);

the_exit:
    context->rdbi_last_status = status;
    debug_return(NULL, status);
}


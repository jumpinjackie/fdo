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
*   rdbi_connect -(Re)connect to the rdbms                              *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_connect(connect_string, dataset, passwd, connect_id)           *
*   char    *connect_string;                                            *
*   char    *dataset;                                                   *
*   char    *passwd;                                                    *
*   int     *connect_id;                                                *
*                                                                       *
* Description                                                           *
*       If  called  for the first time, connect to the RDBMS.  If       *
*       we've been called  once  already,  connect again.  If the       *
*       connection is successful, we'll establish this one as the       *
*       active connection.                                              *
*                                                                       *
* Parameters                                                            *
*   connect_string: input                                               *
*       Any  connect-specific  information  (NULL  or ""  if  not       *
*       applicable)                                                     *
*                                                                       *
*   data_set: input                                                     *
*       A name to identify the connect-id.                              *
*                                                                       *
*   passwd: input                                                       *
*       A password to go along with "name".                             *
*                                                                       *
*   connect_id: output                                                  *
*       A number that uniquely identifies the established database      *
*       connection.  This can later be passed to rdbi_switch().         *
*       The value is only meaningful if this function's return value    *
*       is RDBI_SUCCESS.                                                *
*                                                                       *
* Function value                                                        *
*       Returns an rdbi status code.   Status  definitions can be       *
*       found in <Inc/rdbi.h>.  No messages issued, except in the       *
*       case of a malloc failure.                                       *
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



#include <Inc/Nls/rdbi_msg.h>
#include <Inc/debugext.h>
#include "global.h"             /* rdbi global area */
#include <Inc/Rdbi/proto.h>
#include <Inc/Rdbi/context.h>
#include <Inc/Rdbi/proto.h>
#include <Inc/ut.h>

int
rdbi_connect(
    rdbi_context_def *context,
    char    *connect_string,        /* rdbms-specific connect information   */
    char    *dataset,               /* Data set name                        */
    char    *passwd,                /* password                             */
    int     *connect_id            /* output connection identifier         */
)
{
    int               i;            /* Index into rdbi_connections array    */
    int               new_id;
    int               status;
    rdbi_connect_def *old_cnct;

    debug_on2("rdbi_connect", "connect_string '%s', dataset '%s'",
        ISNULL(connect_string), ISNULL(dataset));

    old_cnct  = context->rdbi_cnct;

    /*
     * Check if they've exceeded the maximum number of connections
     */
    for (i = 0, new_id = -1; i < RDBI_MAX_CONNECTS; i++) {
        if (!context->rdbi_connection[i].in_use) {
            context->rdbi_connection[i].in_use = TRUE;
            new_id = i;
            break;
        }
    }

    debug1("new_id = %d", new_id);

    if (new_id == -1) {
        status = RDBI_TOO_MANY_CONNECTS;
        goto the_exit;
    }
    context->rdbi_cnct = &context->rdbi_connection[new_id];


    status = (*(context->dispatch.connect))(context->drvr, connect_string, dataset, passwd,
                                            &(context->rdbi_cnct->vendor_data),
                                            connect_id);
    /*
     * Initialize variables for this connection
     */
    if (status == RDBI_SUCCESS) {
        strcpy(context->rdbi_cnct->dataset,    (NULL==dataset) ? "" : dataset);
        strcpy(context->rdbi_cnct->db_name,    (NULL==dataset) ? "" : dataset);
        strcpy(context->rdbi_cnct->prev_name,  "");
        context->rdbi_cnct->in_use           = TRUE;
        context->rdbi_cnct->connect_id       = *connect_id;
        context->rdbi_cnct->uncommitted_data = FALSE;
        context->rdbi_cnct->read_only        = FALSE;
        context->rdbi_cnct->autocommit_on    = TRUE;
        context->rdbi_cnct->tran_head        = NULL;
        debug1("connect_id = %d", *connect_id);
		/* This is to set the autocommit mode. It does not do anything for some drivers. */
		status = rdbi_autocommit_on(context);

    } else {
        context->rdbi_cnct->in_use           = FALSE;
        context->rdbi_cnct                   = old_cnct;
    }
the_exit:
    context->rdbi_last_status = status;
    debug_return(NULL, status);
}


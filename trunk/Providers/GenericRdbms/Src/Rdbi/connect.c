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
*   const char    *connect_string;                                      *
*   const char    *dataset;                                             *
*   const char    *passwd;                                              *
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

static int local_rdbi_connect( rdbi_context_def *context,
    rdbi_string_def    *connect_string,  /* rdbms-specific connect information   */
    rdbi_string_def    *dataset,         /* Data set name                        */
    rdbi_string_def    *passwd,          /* password                             */
    int     *connect_id             /* output connection identifier         */
    );
int
rdbi_connect(
    rdbi_context_def *context,
    const char    *connect_string,        /* rdbms-specific connect information   */
    const char    *dataset,               /* Data set name                        */
    const char    *passwd,                /* password                             */
    int     *connect_id             /* output connection identifier         */
)
{
    rdbi_string_def strConnect_string;
    rdbi_string_def strDataset;
    rdbi_string_def strPswd;
    strConnect_string.ccString = connect_string;
    strDataset.ccString = dataset;
    strPswd.ccString = passwd;
    return local_rdbi_connect(context, &strConnect_string, &strDataset, &strPswd, connect_id);
}

int
rdbi_connectW(
    rdbi_context_def *context,
    const wchar_t *connect_string, /* rdbms-specific connect information   */
    const wchar_t *dataset,        /* Data set name                        */
    const wchar_t *passwd,         /* password                             */
    int     *connect_id            /* output connection identifier         */
)
{
    rdbi_string_def strConnect_string;
    rdbi_string_def strDataset;
    rdbi_string_def strPswd;
    strConnect_string.cwString = connect_string;
    strDataset.cwString = dataset;
    strPswd.cwString = passwd;
    return local_rdbi_connect(context, &strConnect_string, &strDataset, &strPswd, connect_id);
}

int
local_rdbi_connect(
    rdbi_context_def *context,
    rdbi_string_def    *connect_string,  /* rdbms-specific connect information   */
    rdbi_string_def    *dataset,         /* Data set name                        */
    rdbi_string_def    *passwd,          /* password                             */
    int     *connect_id             /* output connection identifier         */
    )
{
    int               i;            /* Index into rdbi_connections array    */
    int               new_id;
    int               status;
    rdbi_connect_def *old_cnct;

    if (context->dispatch.capabilities.supports_unicode == 1)
    {
        debug_on2("rdbi_connect", "connect_string '%ls', dataset '%ls'",
            ISNULL(connect_string->cwString), ISNULL(dataset->cwString));
    }
    else
    {
        debug_on2("rdbi_connect", "connect_string '%s', dataset '%s'",
            ISNULL(connect_string->ccString), ISNULL(dataset->ccString));
    }
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

    if (context->dispatch.capabilities.supports_unicode == 1)
        status = (*(context->dispatch.connectW))(context->drvr, connect_string->cwString, dataset->cwString, 
                    passwd->cwString, &(context->rdbi_cnct->vendor_data), connect_id);
    else
        status = (*(context->dispatch.connect))(context->drvr, connect_string->ccString, dataset->ccString, 
                    passwd->ccString, &(context->rdbi_cnct->vendor_data), connect_id);
    /*
     * Initialize variables for this connection
     */
    if (status == RDBI_SUCCESS) {
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

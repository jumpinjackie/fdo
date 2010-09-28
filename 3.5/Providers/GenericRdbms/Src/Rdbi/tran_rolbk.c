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
*   rdbi_tran_rolbk - rollback transactions                             *
*                                                                       *
* Synopsis                                                              *
*   rdbi_tran_rolbk()                                                   *
*                                                                       *
* Description                                                           *
*   Cancel all open transactions and perform a rollback.                *
*                                                                       *
* Parameters                                                            *
*                                                                       *
* Function value                                                        *
*       TRUE/FALSE                                                      *
*                                                                       *
* Remarks                                                               *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.        *
*                                                                       *
*   FILE_DEPENDENCY(LTCART): WARNING! This code is duplicated in PL/SQL.
************************************************************************/


#include    <Inc/Nls/rdbi_msg.h>
#include <Inc/debugext.h>
#include "global.h"
#include <Inc/Rdbi/proto.h>
#include <Inc/Rdbi/context.h>
#include <Inc/ut.h>

int
rdbi_tran_rolbk(rdbi_context_def *context)
{
    tran_entry_def  *tran_entry;
    int             rc;
    int             status = FALSE;
    int             sqlid = -1;

    debug_on("rdbi_tran_rolbk");

    /* execute the rollback */

    rc = rdbi_est_cursor(context, &sqlid);
    if (rc != RDBI_SUCCESS) goto the_exit;
    
    if (context->dispatch.capabilities.supports_unicode == 1)
        rc = rdbi_sqlW(context, sqlid, L"rollback");
    else
        rc = rdbi_sql(context, sqlid, "rollback");
    if (rc != RDBI_SUCCESS) goto the_exit;

    rc = rdbi_execute(context, sqlid, 1, 0);
    if (rc != RDBI_SUCCESS) goto the_exit;

    /* free up transaction entries */

    for (tran_entry = context->rdbi_cnct->tran_head; tran_entry != NULL;
                        tran_entry = context->rdbi_cnct->tran_head) {
        context->rdbi_cnct->tran_head = tran_entry->next;
        (void) ut_vm_free("rdbi_tran_rolbk", tran_entry);
    }

the_exit:
    if (sqlid != -1)
        rdbi_fre_cursor(context, sqlid);

    status = (rc == RDBI_SUCCESS);

    debug_return(NULL, status);
}


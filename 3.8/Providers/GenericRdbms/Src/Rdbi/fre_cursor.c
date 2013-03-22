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
*   rdbi_fre_cursor - Free this SQL control area.                       *
*                                                                       *
* Synopsis                                                              *
*   int rdbi_fre_cursor(sqlid)                                          *
*   int sqlid;                                                          *
*                                                                       *
* Description                                                           *
*       Free  the  SQL  control  area  specified  by  the "sqlid"       *
*       parameter.                                                      *
*                                                                       *
*                                                                       *
* Parameters                                                            *
*   sqlid: input                                                        *
*       Index into internal global array of cursors.                    *
*                                                                       *
* Function value                                                        *
*       An  RDBI  function  value  (include  <Inc/rdbi.h).  If we       *
*       fail, this cursor area will not be re-usable.                   *
*                                                                       *
* Remarks                                                               *
*       This routine updates the rdbi_last_status global                *
*                                                                       *
*       If  the cursor has not been  opened,  this  routine  will       *
*       simply return a successful status.                              *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.            *
*                                                                       *
*   FILE_DEPENDENCY(LTCART): WARNING! This code is duplicated in PL/SQL.
************************************************************************/

#include <Inc/ut.h>
#include <Inc/debugext.h>

#include <Inc/Nls/rdbi_msg.h>
#include "global.h"                 /* rdbi global area                     */
#include <Inc/Rdbi/proto.h>
#include <Inc/Rdbi/context.h>

int rdbi_fre_cursor(
	rdbi_context_def *context,
    int sqlid)                      /* index to internal cursor globals     */
{
    rdbi_cursor_def *cursor;        /* For ease of reference                */
	char			stats[128];


    debug_on1("rdbi_fre_cursor", "sqlid %d", sqlid);

    context->rdbi_last_status = RDBI_SUCCESS;

    if (context->rdbi_cnct == NULL) {
		char temp[64];
		sprintf(temp, "%d", sqlid);
        rdbi_msg_set_S(context, RDBI_30, "No current database is open; cannot free cursor %1$ls", temp);
        goto the_exit;
    }

    if (sqlid > context->rdbi_num_cursor_ptrs)
        goto the_exit;
    cursor = context->rdbi_cursor_ptrs[sqlid];

    if (cursor == (rdbi_cursor_def *)NULL)
        goto the_exit;

	sprintf(stats, "Closed cursor: %d, executions: %d, rows processed:%ld",
					sqlid, cursor->n_executions, cursor->trows_processed);
	(void) debug_trace(stats, (wchar_t *)NULL, NULL);

    if (context->rdbi_cnct->autocommit_on && cursor->tran_begun) {
        char    tran_id[50];
        sprintf(tran_id, "auto-exec-%s %d", cursor->verb, cursor->trace_line);
        rdbi_tran_end(context, tran_id);
        cursor->tran_begun = FALSE;
    }


    context->rdbi_last_status = (*(context->dispatch.fre_cursor))(context->drvr, &cursor->vendor_data);

    /* ut_vm_free can handle NULL ptrs  */
#ifdef _DEBUG
    ut_vm_free("rdbi_fre_cursor", cursor->sql);
#endif
    ut_vm_free("rdbi_fre_cursor", (char *)cursor);
    context->rdbi_cursor_ptrs[sqlid] = (rdbi_cursor_def *)NULL;

the_exit:
    debug_return(NULL, context->rdbi_last_status);
}


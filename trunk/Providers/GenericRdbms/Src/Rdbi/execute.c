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
*   rdbi_execute - [Array] execute a previously parsed SQL statement    *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_execute(sqlid, count, offset)                                  *
*   int     sqlid;                                                      *
*   int     count;                                                      *
*   int     offset;                                                     *
*                                                                       *
* Description                                                           *
*                                                                       *
*       If  rdbi_est_cursor()  and rdbi_sql() have been called to       *
*       prepare an SQL statement, execute it here.                      *
*                                                                       *
* Parameters                                                            *
*   sqlid: input                                                        *
*       Index into internal global array of cursors.                    *
*                                                                       *
*   count: input                                                        *
*       Number of rows to process                                       *
*                                                                       *
*   offset: input                                                       *
*       Offset within array to begin at.                                *
*                                                                       *
* Function value                                                        *
*       A status value from Inc/rdbi.h                                  *
*                                                                       *
* Remarks                                                               *
*       To issue a  message  for an error, call rdbi_msg_issue().       *
*                                                                       *
*       This routine updates the rdbi_last_status global                *
*                                                                       *
*
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

#include    <Inc/Nls/rdbi_msg.h>
#include "global.h"                 /* rdbi global area         */
#include <Inc/Rdbi/proto.h>
#include	<Inc/Rdbi/context.h>

#ifndef _WIN32
#define _stricmp  strcasecmp
#endif

int
rdbi_execute(
	rdbi_context_def *context,
    int     sqlid,
    int     count,
    int     offset)
{
    rdbi_cursor_def *cursor;        /* for convenience          */
    char    tran_id[60];
    int     is_select = FALSE;


    debug_on3("rdbi_execute", "context: %d count: %d, SQL: '%.120s'", sqlid, count, context->rdbi_cursor_ptrs[sqlid]->sql);

    cursor = context->rdbi_cursor_ptrs[sqlid];
    cursor->rows_processed = 0;

    is_select = (_stricmp(cursor->verb, "select") == 0);
    /*
        * "count" is polymophic:
        *   - for SQL "select", it means # of rows (we want zero on execute)
        *   - for other verbs, it means # of executions (# rows for array insert/update, otherwise 1 is expected)
        */
    count  = is_select ? 0 : count;

    if (context->rdbi_cnct->autocommit_on) {
        sprintf(tran_id, "auto-exec-%s %d", cursor->verb, cursor->trace_line);
        if (cursor->tran_begun) {           /* a re-bind before eof??   */
            rdbi_tran_end(context, tran_id);
            cursor->tran_begun = FALSE;
        }
        rdbi_tran_begin(context, tran_id);
        cursor->tran_begun = TRUE;
    }
    cursor->status = (*(context->dispatch.execute))(context->drvr, cursor->vendor_data,
                                    count, offset, &cursor->rows_processed);

    cursor->trows_processed += cursor->rows_processed;

    debug2("%d rows (%ld total)", cursor->rows_processed, cursor->trows_processed);

    context->rdbi_last_status = cursor->status;

    if (context->rdbi_cnct->autocommit_on) {
        if (_stricmp(cursor->verb, "select") != 0  ||
            cursor->status == RDBI_END_OF_FETCH)    {
            rdbi_tran_end(context, tran_id);
            cursor->tran_begun = FALSE;
        }
    }
    if (cursor->status == RDBI_END_OF_FETCH)
        cursor->status = RDBI_SUCCESS;

    if (cursor->status == RDBI_SUCCESS) {
        cursor->n_executions++;
    }

    /*
     * If the execute failed, we don't want to keep the transaction active
     */
    if ((cursor->status != RDBI_SUCCESS) && cursor->tran_begun) {
        rdbi_tran_end(context, tran_id);
        cursor->tran_begun = FALSE;
    }

    debug_return(NULL, cursor->status);
}


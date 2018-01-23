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
*   rdbi_fetch - Fetch row(s) from the rdbms                            *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_fetch(sqlid, count, rows_processed)                            *
*   int  sqlid;                                                         *
*   int  count;                                                         *
*   int *rows_processed;                                                *
*                                                                       *
* Description                                                           *
*       Fetch column values into the defined locations.                 *
*                                                                       *
*       'rows_processed'  will return the number of rows actually       *
*       returned.                                                       *
*                                                                       *
* Parameters                                                            *
*   sqlid: input                                                        *
*       Index into  internal  global  array  of  cursor pointers.       *
*                                                                       *
*   count: input                                                        *
*       If array processing, 'count' will indicate how  many rows       *
*       to attempt to fetch.                                            *
*                                                                       *
*   rows_processed: output                                              *
*       The number of rows actually returned.                           *
*                                                                       *
* Function value                                                        *
*       An rdbi status code from Inc/rdbi.h                             *
*                                                                       *
* Remarks                                                               *
*       No messages issued here.                                        *
*                                                                       *
*       This routine updates the rdbi_last_status global.               *
*                                                                       *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages  in this source file have been externalized.       *
*       Do   not   add    any   more   msg_issue()   calls,   use       *
*       nls_msg_issue()  instead.   If  you   modify   or  delete       *
*       messages, be sure to modify or  delete  the corresponding       *
*       message in $root/nls/msf/rdbi.msf.                              *
*                                                                       *
*   FILE_DEPENDENCY(LTCART): WARNING! This code is duplicated in PL/SQL.
************************************************************************/

#include <Inc/ut.h>
#include <Inc/debugext.h>

#include    <Inc/Nls/rdbi_msg.h>
#include <Inc/rdbi.h>               /* rdbi status codes            */
#include <Inc/Rdbi/cursor.h>        /* rdbi_cursor_def              */

#include "global.h"                 /* rdbi global area             */
#include <Inc/Rdbi/proto.h>
#include	<Inc/Rdbi/context.h>

static int rdbi_fetch2(rdbi_context_def *context, int      sqlid,
           int      count,
           int      do_exec,
           int      do_cancel,
           int      *rows_processed);
        

int
rdbi_fetch(
	rdbi_context_def *context,
    int  sqlid,                     /* index into cursor globals    */
    int  count,                     /* # rows to fetch              */
    int *rows_processed)            /* # rows actually fetched      */
{
    return rdbi_fetch2(context, sqlid, count, FALSE, FALSE, rows_processed);
}

int
rdbi_exec_fetch(
	rdbi_context_def *context,
    int  sqlid,                     /* index into cursor globals    */
    int  count,                     /* # rows to fetch              */
    int  do_cancel,
    int *rows_processed)            /* # rows actually fetched      */
{
    return rdbi_fetch2(context, sqlid, count, TRUE, do_cancel, rows_processed);
}

static int rdbi_fetch2(
	rdbi_context_def *context,
    int  sqlid,                     /* index into cursor globals    */
    int  count,                     /* # rows to fetch              */
    int  do_exec,
    int  do_cancel,
    int *rows_processed)            /* # rows actually fetched      */
{
    int     local_rows_processed;
    int     rows_fetched;
    rdbi_cursor_def *cursor;
    char    tran_id[60];

    debug_on3("rdbi_fetch2","\tContext: %d, count %d, SQL: '%.120s'", sqlid, count, context->rdbi_cursor_ptrs[sqlid]->sql);

    if (rows_processed == (int *)NULL)
        rows_processed = &local_rows_processed;

    cursor = context->rdbi_cursor_ptrs[sqlid];

    if(do_exec) {
        cursor->rows_processed = 0;

        if (context->rdbi_cnct->autocommit_on) {
            sprintf(tran_id, "auto-exec-%s %d", cursor->verb, cursor->trace_line);
            if (cursor->tran_begun) {           /* a re-bind before eof??   */
                rdbi_tran_end(context, tran_id);
                cursor->tran_begun = FALSE;
            }
            rdbi_tran_begin(context, tran_id);
            cursor->tran_begun = TRUE;
        }
    }

    debug2("initial rows_processed %d, trows_processed %ld",
        cursor->rows_processed, cursor->trows_processed);

    if(!do_exec && context->rdbi_cnct->autocommit_on && cursor->tran_begun == RDBI_TRAN_EOF) {
        cursor->tran_begun = TRUE;
        cursor->status     = RDBI_END_OF_FETCH;
        *rows_processed    = cursor->rows_processed;
        debug0("Returning delayed END_OF_FETCH status");
    } else {
        cursor->status = (*(context->dispatch.fetch))(context->drvr, cursor->vendor_data, count,
                                        do_exec, do_cancel, rows_processed);
        if (do_exec && (cursor->status == RDBI_SUCCESS ||
                        cursor->status == RDBI_END_OF_FETCH)) {
            cursor->n_executions++;
        }
    }


    cursor->exec_coc_legal =
                ((cursor->status == RDBI_SUCCESS) && cursor->sel_for_update
                 && (!do_exec || !do_cancel));

    rows_fetched             = *rows_processed - cursor->rows_processed;
    if(cursor->status == RDBI_END_OF_FETCH &&
            rows_fetched > 0 && cursor->tran_begun) {
        cursor->tran_begun = RDBI_TRAN_EOF;
        cursor->status     = RDBI_SUCCESS;
        debug2("Delaying premature END_OF_FETCH after %d rows (%d rows fetched)",
            *rows_processed, rows_fetched);
    }
    cursor->trows_processed += rows_fetched;
    cursor->rows_processed   = *rows_processed;

    if (context->rdbi_cnct->autocommit_on && cursor->status == RDBI_END_OF_FETCH) {
        if(!do_exec)
            sprintf(tran_id, "auto-exec-%s %d",cursor->verb,cursor->trace_line);
        rdbi_tran_end(context, tran_id);
        cursor->tran_begun = FALSE;
    }

    /*
     * If the execute failed, we don't want to keep the transaction active
     */
    if (do_exec && cursor->tran_begun
            && cursor->status != RDBI_SUCCESS
            && cursor->status != RDBI_END_OF_FETCH) {
        rdbi_tran_end(context, tran_id);
        cursor->tran_begun = FALSE;
    }

    debug2("rows_processed %d (%d this call)", *rows_processed, rows_fetched);

    context->rdbi_last_status = cursor->status;

    debug_return(NULL, context->rdbi_last_status);
}


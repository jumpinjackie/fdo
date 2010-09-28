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
*   rdbi_tran_getid - get the id of the last transaction                *
*                                                                       *
* Synopsis                                                              *
*   rdbi_tran_getid(tran_id, last_trans)                                *
*   char    **tran_id;                                                  *
*   int     *last_trans;
*                                                                       *
* Description                                                           *
*       Get the id of the currently open transaction.                   *
*                                                                       *
* Parameters                                                            *
*   tran_id: output                                                     *
*       A pointer to the last transaction id, NULL if none open.        *
*                                                                       *
*   last_trans: output
*       TRUE if the returned transaction id is the last one open or
*       if there is no open transaction; FALSE otherwise
*
* Function value                                                        *
*       TRUE/FALSE                                                      *
*                                                                       *
* Remarks                                                               *
*   Note: this function just returns a pointer.  Do not update the      *
*   id received.                                                        *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.        *
*                                                                       *
*   FILE_DEPENDENCY(LTCART): WARNING! This code is duplicated in PL/SQL.
************************************************************************/

extern char *rdbi_cat;

#include    <Inc/Nls/rdbi_msg.h>
#include <Inc/debugext.h>
#include "global.h"             /* for Gtran_head */
#include	<Inc/Rdbi/context.h>
#include <Inc/Rdbi/proto.h>

int
rdbi_tran_getid(
rdbi_context_def *context,
char    **tran_id,
int     *last_trans )
{
    int      status = FALSE;

    debug_on("rdbi_tran_getid");

    if (tran_id == NULL) {
        rdbi_msg_set_0( context, RDBI_24, "Illegal (null) transaction id pointer.");
        goto the_exit;
    }

    status = TRUE;

    /* Just follow the tran_head pointer to get the necessary info. */

    if (context->rdbi_cnct->tran_head == NULL) {
        *tran_id = NULL;
        *last_trans = TRUE;
    }
    else {
        *tran_id = context->rdbi_cnct->tran_head->tran_id;
        *last_trans = (context->rdbi_cnct->tran_head->next == NULL);
    }

    debug2("tran_id = '%s', last_trans = '%s'",
        ISNULL(*tran_id), ISTRUE(*last_trans));

the_exit:
    debug_return(NULL, status);
}


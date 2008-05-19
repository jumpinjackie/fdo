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
*   rdbi_tran_begin - Begin a new transaction                           *
*                                                                       *
* Synopsis                                                              *
*   rdbi_tran_begin(tran_id)                                            *
*   char    *tran_id;                                                   *
*                                                                       *
* Description                                                           *
*       Record   the   beginning  of  a  new  transaction  to  be       *
*       identified by the  string <id>.  This will also increment       *
*       the transaction level count.                                    *
*                                                                       *
* Parameters                                                            *
*   tran_id:    input                                                   *
*       An  arbitrary  string  to   be   associated   with   this       *
*       transaction.   This same string must be used to  end  the       *
*       transaction.                                                    *
*                                                                       *
* Function value                                                        *
*       TRUE, except  for illegal parameters or memory allocation       *
*       failure                                                         *
*                                                                       *
* Remarks                                                               *
*       The transaction record is just a LIFO stack.                    *
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
#include "global.h"             /* for rdbi_cnct->tran_head     */
#include <Inc/Rdbi/context.h>
#include <Inc/Rdbi/proto.h>
#include <Inc/ut.h>


int
rdbi_tran_begin(
rdbi_context_def *context,
char    *tran_id )
{

    tran_entry_def  *tran_entry;
    int              status;


    debug_on3("rdbi_tran_begin", "New tran id [%s.R%d:'%s']",
                    context->rdbi_cnct->db_name, context->rdbi_cnct->connect_id, ISNULL(tran_id));

    status = FALSE;
    if (tran_id == NULL || strcmp(tran_id, "") == 0)    {
        rdbi_msg_set_0(context, RDBI_20, "Illegal (null or empty) transaction id.");
        goto the_exit;
    }

    if (context->rdbi_cnct == NULL) {
        rdbi_msg_set_S(context, RDBI_21, "No current database is open; cannot begin transaction '%1$ls' .", tran_id);
        goto the_exit;
    }

    tran_entry = (tran_entry_def *)ut_vm_malloc("rdbi_tran_begin",
                                            sizeof(struct tran_entry_def));
    if (tran_entry == NULL) goto the_exit;

    strncpy(tran_entry->tran_id, tran_id, sizeof(tran_entry->tran_id));
    tran_entry->tran_id[sizeof(tran_entry->tran_id)-1] = '\0';
    tran_entry->tran_ended  = FALSE;
    tran_entry->next        = context->rdbi_cnct->tran_head;
    context->rdbi_cnct->tran_head    = tran_entry;

    status = TRUE;

the_exit:
    debug_return(NULL, status);
}


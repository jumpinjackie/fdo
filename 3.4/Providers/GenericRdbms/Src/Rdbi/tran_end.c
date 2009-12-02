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
*   rdbi_tran_end - end a transaction                                   *
*                                                                       *
* Synopsis                                                              *
*   rdbi_tran_end(tran_id)                                              *
*   char    *tran_id;                                                   *
*                                                                       *
* Description                                                           *
*       Record  the ending of a transaction that is identified by       *
*       the  string   <id>.    This   will   also  decrement  the       *
*       transaction level count.  If the count  goes  to zero, an       *
*       RDBMS commit will be issued.                                    *
*                                                                       *
* Parameters                                                            *
*   id: input                                                           *
*       An   arbitrary  string  that  was  associated  with  this       *
*       transaction on the rdbi_tran_begin() call.                      *
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
#include "global.h"             /* for Gtran_head       */
#include <Inc/Rdbi/proto.h>
#include <Inc/Rdbi/context.h>
#include <Inc/ut.h>


#ifndef _WIN32
#define  strnicmp strncasecmp
#define  _strnicmp strncasecmp
#endif

int
rdbi_tran_end(
rdbi_context_def *context,
char    *tran_id )
{

    tran_entry_def  *tran_entry;
    int              status;
    int              first = FALSE;
    int              second = FALSE;

    debug_on3("rdbi_tran_end","  Old tran id [%s.R%d:'%s']",
                    context->rdbi_cnct->db_name, context->rdbi_cnct->connect_id, ISNULL(tran_id));

    status = FALSE;
    if (tran_id == NULL || *tran_id == '\0')    {
        rdbi_msg_set_0( context, RDBI_22, "Illegal (null or empty) transaction id.");
        goto the_exit;
    }

#define Gtran_head context->rdbi_cnct->tran_head

    if (Gtran_head == NULL) {
        rdbi_msg_set_S( context, RDBI_23, "There are no transactions to end. ['%1$ls']", tran_id);
        goto the_exit;
    }

    tran_entry = Gtran_head;

	if (_strnicmp("auto-exec-select", tran_id, 16 /*sizeof( "auto-exec-select" )*/ ) == 0) {
        /*
         *  For auto-exec-select transactions we allow
         *  the tran_end to happen in any order (i.e. it
         *  does not have to follow the strict transaction
         *  nesting order).  So search the entire stack for
         *  the auto-exec-select transaction.  The tran_ended
         *  check ensures that the transaction is only ended once.
         */
        for( ; tran_entry != NULL; tran_entry=tran_entry->next) {
            if (!tran_entry->tran_ended && strncmp(tran_id, tran_entry->tran_id,
                                            sizeof(tran_entry->tran_id)) == 0) {
                if(tran_entry != Gtran_head) {
                    debug1("Ending nested ['%s'] transaction",
                                tran_entry->tran_id);
                }
                break;
            }
        }
        /*
         *  If we didn't find it then let the transaction nesting
         *  error report the transaction on the stack top.
         */
        if(tran_entry == NULL)
            tran_entry = Gtran_head;
    }
    if (strncmp(tran_id, tran_entry->tran_id,
                                        sizeof(tran_entry->tran_id)) != 0) {
		char	transmsg[128];
        /*
         *  Transaction nesting error.
         */
		if ((_strnicmp("auto-exec", tran_id, 9 ) != 0))
            first = TRUE;

		if ((_strnicmp("auto-exec", tran_entry->tran_id, 9 ) != 0))
            second = TRUE;

		sprintf(transmsg,"%c%s%s%s%c",
					(first || second) ? '[' : ' ',
                    first             ? tran_id : "",
                    (first && second) ? " vs " : "",
                    second            ? tran_entry->tran_id : "",
                    (first || second) ? ']' : ' ');

        rdbi_msg_set_S( context, RDBI_29, "Transaction nesting error. '%1$ls'", transmsg );

        goto the_exit;
    } else
        tran_entry->tran_ended = TRUE;

    /*
     *  Free up all transactions on the top of the
     *  stack that have been previously `ended'.
     */
    while(Gtran_head && Gtran_head->tran_ended) {
        tran_entry = Gtran_head;
        Gtran_head = Gtran_head->next;
        (void)ut_vm_free("rdbi_tran_end", tran_entry);
    }

    status = TRUE;
    if (Gtran_head == NULL) {                   /* last one ==> commit  */
        if (context->rdbi_last_status == RDBI_SUCCESS ||
            context->rdbi_last_status == RDBI_END_OF_FETCH)
            status = (rdbi_commit(context) == 0);

    } else {                                /* Track down problem areas */
        debug_area() {
        /* See if there are any explicit (ie. non-auto-) transactions   */
        for (tran_entry = Gtran_head; tran_entry != NULL;
                                            tran_entry = tran_entry->next) {
			if (_strnicmp("auto-exec-", tran_entry->tran_id, 9 ) != 0)
                break;
        }

        /*
         * For automatic, but non-select, transactions (eg. update, delete,
         * insert,...) report when they are held off by any non-explicit
         * transaction
         */
		if (_strnicmp("auto-exec-", tran_id, 9 ) == 0 &&
            _strnicmp("auto-exec-select", tran_id, 16) != 0) {

            if (tran_entry == NULL) {
                debug2("No commit for ['%s'], ['%s'] is outstanding",
                                        tran_id, Gtran_head->tran_id);
            } else {
                debug2("Commit for ['%s'] held off by ['%s']",
                                        tran_id, tran_entry->tran_id);
            }
        }
        } /* end debug area */
    }

the_exit:
    debug_return(NULL, status);
}


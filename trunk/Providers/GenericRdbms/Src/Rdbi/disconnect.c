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
*   rdbi_disconnect - disconnect from an RDBMS                          *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   int rdbi_disconnect()                                               *
*                                                                       *
* Description                                                           *
*       Disconnect  from  an RDBMS.  If the application wishes to       *
*       continue operations, it  must  call  rdbi_connect  again.       *
*                                                                       *
* Parameters                                                            *
*                                                                       *
* Function value                                                        *
*       TRUE  for success; FALSE for failure.  Message(s)  issued       *
*       on failure.   Our  connection will be in an indeterminant       *
*       state should this call fail.                                    *
*                                                                       *
* Remarks                                                               *
*       Free  all  resources  and   memory  associated  with  the       *
*       connection and perform the RDBMS disconnect.                    *
*                                                                       *
*       This routine updates the rdbi_last_status  global  If not       *
*       connected,  this  routine  will  simply  return  success.       *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.        *
*                                                                       *
************************************************************************/


#include    <Inc/Nls/rdbi_msg.h>
#include <Inc/debugext.h>
#include <Inc/ut.h>         /* ut_vm_free()             */
#include <Inc/Rdbi/proto.h>
#include <Inc/Rdbi/context.h>

#include "global.h"         /* rdbi global area         */

int rdbi_disconnect(rdbi_context_def *context)
{

    debug_on("rdbi_disconnect");

    context->rdbi_last_status = RDBI_SUCCESS;

    if (context->rdbi_cnct == (rdbi_connect_def *)NULL)
        debug_return(NULL, RDBI_SUCCESS);

    if (context->rdbi_cnct->tran_head != NULL) {
        tran_entry_def  *tran_entry;
        for (tran_entry=context->rdbi_cnct->tran_head; tran_entry != NULL;
                                            tran_entry=context->rdbi_cnct->tran_head) {
            context->rdbi_cnct->tran_head = tran_entry->next;
            (void)ut_vm_free("rdbi_disconnect", tran_entry);
        }
    }

    rdbi_free_all(context);
    context->rdbi_last_status = (*(context->dispatch.disconnect))(context->drvr, &context->rdbi_cnct->vendor_data);

    context->rdbi_cnct->in_use = FALSE;              /* mark as available    */

    context->rdbi_cnct = NULL;

    debug_return(NULL, context->rdbi_last_status);
}


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
*   rdbi_free_all - free all cursor areas for a connection              *
*                                                                       *
* Synopsis                                                              *
*   int rdbi_free_all()                                                 *
*                                                                       *
* Description                                                           *
*       Free  all cursor areas associated with a session.  We can       *
*       free them  to  varying degress via the "level" parameter.       *
*                                                                       *
* Parameters                                                            *
*                                                                       *
* Function value                                                        *
*       A  value  from Inc/rdbi.h  No  messages  issued  locally.       *
*                                                                       *
* Remarks                                                               *
*       This routine updates the rdbi_last_status global                *
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

#include "global.h"                 /* rdbi global area         */
#include <Inc/Rdbi/proto.h>
#include	<Inc/Rdbi/context.h>

int rdbi_free_all(rdbi_context_def *context)                 /* free all cursor areas    */
{
    int i;
    int status;


    debug_on("rdbi_free_all");

    context->rdbi_last_status = RDBI_SUCCESS;
    /*
     * Loop through all cursor areas. If they're allocated, pass them
     * on to rdbi_fre_cursor(). Note that we send a 1-based offset
     */
    for (i = 0; i < context->rdbi_num_cursor_ptrs; i++) {
        if (context->rdbi_cursor_ptrs[i] != (rdbi_cursor_def *)NULL) {
            status = rdbi_fre_cursor(context, i);
            if (status != RDBI_SUCCESS)
                context->rdbi_last_status = status;
        }
    }

    debug_return(NULL, context->rdbi_last_status);
}


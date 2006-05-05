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
*                                                                       *
* Name                                                                  *
*   rdbi_term - deinitialize the rdbms.                                 *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_term()                                                         *
*                                                                       *
* Description                                                           *
*       Close down the rdbms by freeing the cursor areas.               *
*                                                                       *
* Parameters                                                            *
*                                                                       *
* Function value                                                        *
*   RDBI_SUCCESS (0) iff successful, RDBI_GENERIC_ERROR otherwise.      *
*                                                                       *
* Remarks                                                               *
*       Returns an rdbi status code.   Status  definitions can be       *
*       found in <Inc/rdbi.h>.                                          *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.            *
*                                                                       *
************************************************************************/

#include <Inc/ut.h>
#include <Inc/debugext.h>
#include <Inc/rdbi.h>
#include "global.h"
#include <Inc/Rdbi/context.h>

int rdbi_term (rdbi_context_def **contextp)
{
    rdbi_context_def *context;
    int     status;
    int     i;

    debug_on("rdbi_term");

    context = *contextp;
    status = RDBI_GENERIC_ERROR;
    if (context->rdbi_initialized)
    {
        for (i=0; i<context->rdbi_num_cursor_ptrs; i++)
        {
#ifdef _DEBUG
            if (context->rdbi_cursor_ptrs[i])
                ut_vm_free ("rdbi_term", context->rdbi_cursor_ptrs[i]->sql);
#endif
            ut_vm_free ("rdbi_term", context->rdbi_cursor_ptrs[i]);
        }
        ut_vm_free ("rdbi_term", context->rdbi_cursor_ptrs);
        context->rdbi_cursor_ptrs = NULL;
        context->rdbi_num_cursor_ptrs = 0;
        context->rdbi_initialized = FALSE;
        status = (*(context->dispatch.term)) (&(context->drvr));
        ut_vm_free ("rdbi_term", context);
        *contextp = NULL;
    }

    debug_return(NULL, status);
}


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
*   rdbi_init - initialize the rdbms.                                   *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_init()                                                         *
*                                                                       *
* Description                                                           *
*       Initialize the rdbms by calling the routine to initialize the   *
*       vendor-specific routine addresses and mallocing the first few   *
*       cursor areas                                                    *
*                                                                       *
* Parameters                                                            *
*                                                                       *
* Function value                                                        *
*   TRUE iff successful, FALSE otherwise.                               *
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
#include <Inc/Nls/rdbi_msg.h>
#include <Inc/Rdbi/context.h>
#include <Inc/Rdbi/proto.h>

#define RDBI_GLOBAL
#include "global.h"

int rdbi_init (rdbi_context_def **contextp, initializer* driver)
{
    rdbi_context_def *context;
    int i;
    int ret;


    debug_on("rdbi_init");

    context = (rdbi_context_def *) ut_vm_malloc ("rdbi_init", sizeof (rdbi_context_def));
    if ((rdbi_context_def *)NULL == context)
        ret = RDBI_MALLOC_FAILED;
    else
    {
        if (!rdbi_init_context (context))
            ret = RDBI_GENERIC_ERROR;
        else
        {
            memset (&(context->dispatch), 0, sizeof (context->dispatch));
            ret = driver (&(context->drvr), &(context->dispatch));
            if (RDBI_SUCCESS == ret)
            {
                context->rdbi_initialized = TRUE;
                for (i = 0; i < RDBI_MAX_CONNECTS; i++)
                    context->rdbi_connection[i].in_use = FALSE;

                context->rdbi_cursor_ptrs = (rdbi_cursor_def **)NULL;
                /*
                * Malloc the first few cursor areas
                */
                context->rdbi_cursor_ptrs = (rdbi_cursor_def **)ut_vm_malloc(
                "rdbi_connect", RDBI_CONTEXT_MALLOC_COUNT * sizeof(rdbi_cursor_def*));

                if ((rdbi_cursor_def **)NULL == context->rdbi_cursor_ptrs)
                {
                    (context->dispatch.term) (&(context->drvr));
                    ret = RDBI_MALLOC_FAILED;
                }
                else
                {
                    context->rdbi_num_cursor_ptrs = RDBI_CONTEXT_MALLOC_COUNT;
                    for (i=0; i < context->rdbi_num_cursor_ptrs; i++)
                        context->rdbi_cursor_ptrs[i] = (rdbi_cursor_def *)NULL;
                    *contextp = context;
                }
            }
        }
    }

    if (RDBI_SUCCESS != ret)
        if ((rdbi_context_def *)NULL != context)
        {
            if ((rdbi_cursor_def **)NULL != context->rdbi_cursor_ptrs)
                ut_vm_free ("rdbi_init", context->rdbi_cursor_ptrs);
            ut_vm_free ("rdbi_init", context);
        }
    debug_return(NULL, ret);
}


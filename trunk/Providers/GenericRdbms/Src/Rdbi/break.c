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
*   rdbi_break - Cancel any currently executing operations.             *
*                                                                       *
* Synopsis                                                              *
*   rdbi_break()                                                        *
*                                                                       *
* Description                                                           *
*       Cancels any operations that are currently executing.            * 
*                                                                       *
* Parameters                                                            *
*   None                                                                *
*                                                                       *
* Returns                                                               *
*   An rdbi return code - see Inc/rdbi.h                                *
*                                                                       *
* Remarks                                                               *
*       This is not for general purpose use but is intended to          *
*       terminate any operations that would interfere with closing down *
*       the database in response to a signal.                           *
*                                                                       *
************************************************************************/

#include <Inc/Nls/rdbi_msg.h>
#include <Inc/debugext.h>
#include <Inc/rdbi.h>
#include <Inc/Rdbi/proto.h>
#include "global.h"
#include <Inc/Rdbi/context.h>

int
rdbi_break(rdbi_context_def *context)
{
    debug_on("rdbi_commit");

    if ( context->dispatch.do_break != NULL )
        context->rdbi_last_status = (*(context->dispatch.do_break))(context->drvr);
    else
        context->rdbi_last_status = RDBI_SUCCESS;

    debug_return(NULL,context->rdbi_last_status);
}


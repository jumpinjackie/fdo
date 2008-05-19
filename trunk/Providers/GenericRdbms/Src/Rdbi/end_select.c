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
*   rdbi_end_select - End a selection prior to end-of-fetch             *
*                                                                       *
* Synopsis                                                              *
*   rdbi_end_select(sqlid)                                              *
*   int     sqlid;                                                      *
*                                                                       *
* Description                                                           *
*       This routine will end any outstanding transaction on this       *
*       cursor.   It  is  expected  to  be  used in cases where a       *
*       select   loop   is  logically  complete  before  reaching       *
*       end-of-fetch                                                    *
*                                                                       *
* Parameters                                                            *
*   sqlid: input                                                        *
*       The integer id of the associated cursor                         *
*                                                                       *
* Function Value                                                        *
*   Always RDBI_SUCCESS.                                                *
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


#include <stdio.h>
#include    <Inc/Nls/rdbi_msg.h>
#include <Inc/debugext.h>
#include "global.h"
#include <Inc/Rdbi/proto.h>
#include <Inc/Rdbi/context.h>

int
rdbi_end_select(
rdbi_context_def *context,
int     sqlid)
{
    rdbi_cursor_def *cursor;


    debug_on1("rdbi_end_select", "\tContext: %d", sqlid);

    cursor = context->rdbi_cursor_ptrs[sqlid];
	if ( cursor )	{
		if ( context->dispatch.close_cursor != NULL )
			cursor->status = (*(context->dispatch.close_cursor))(context->drvr, cursor->vendor_data);

		if ( context->rdbi_cnct->autocommit_on && cursor->tran_begun) {
			char    tran_id[60];
			sprintf(tran_id, "auto-exec-%s %d", cursor->verb, cursor->trace_line);
			rdbi_tran_end(context, tran_id);
			cursor->tran_begun = FALSE;
		}
	}

    debug_return(NULL, RDBI_SUCCESS);
}


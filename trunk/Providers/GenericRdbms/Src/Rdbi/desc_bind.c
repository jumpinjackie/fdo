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
*   rdbi_desc_bind - Describe a given bound variable identifier         *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   int rdbi_desc_bind(sqlid, position, name_len, name)                 *
*   int   sqlid;                                                        *
*   int   position;                                                     *
*   int   name_len;                                                     *
*   char *name;                                                         *
*                                                                       *
* Description                                                           *
*       Return information on bind variables.                           *
*                                                                       *
* Parameters                                                            *
*   sqlid: input                                                        *
*       Index into global array of cursor pointers                      *
*                                                                       *
*   position: input                                                     *
*       Position within the select expression.                          *
*                                                                       *
*   name_len: input                                                     *
*       The maximum size of name                                        *
*                                                                       *
*   name: output                                                        *
*       The column/expression name                                      *
*                                                                       *
* Function value                                                        *
*       An rdbi status code from Inc/rdbi.h                             *
*                                                                       *
* Remarks                                                               *
*       No messages issued here.                                        *
*                                                                       *
*       This routine updates the rdbi_last_status global.               *
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
#include <Inc/rdbi.h>                   /* rdbi status codes    */
#include <Inc/Rdbi/context.h>

#include "global.h"                     /* rdbi global area     */

int rdbi_desc_bind(
	rdbi_context_def *context,
    int   sqlid,            /* index into ptrs to Oracle cursor */
    int   position,         /* position within select clause    */
    int   name_len,         /* The maximum size of name         */
    char *name             /* Name of this field               */
)
{
    rdbi_cursor_def *cursor;


    debug_on("rdbi_desc_bind");
    debug1("Position: %d", position);

    cursor = context->rdbi_cursor_ptrs[sqlid];
    context->rdbi_last_status = (*(context->dispatch.desc_bind))(context->drvr, cursor->vendor_data, position,
                    name_len, name);

    debug_return(NULL, context->rdbi_last_status);
}


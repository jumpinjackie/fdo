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
*   rdbi_desc_slct - describe an element of the select clause           *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   int rdbi_desc_slct(sqlid, pos, name_len, name, rdbi_type,           *
*                           binary_size, null_ok)                       *
*   int   sqlid;                                                        *
*   int   pos;                                                          *
*   int   name_len;                                                     *
*   char *name;                                                         *
*   int  *rdbi_type;                                                    *
*   int  *binary_size;                                                  *
*   int  *null_ok;                                                      *
*                                                                       *
* Description                                                           *
*       Return information on elements of a select clause               *
*                                                                       *
* Parameters                                                            *
*   sqlid: input                                                        *
*       Index into global array of cursor pointers                      *
*                                                                       *
*   pos: input                                                          *
*       Position within the select expression.                          *
*                                                                       *
*   name_len: input                                                     *
*       The maximum size of name                                        *
*                                                                       *
*   name: output                                                        *
*       The column/expression name                                      *
*                                                                       *
*   rdbi_type: output                                                   *
*       RDBI   constant   indicating   the  type  of  data  we're       *
*       expecting.                                                      *
*                                                                       *
*   binary_size: output                                                 *
*       The number of bytes  of  memory  required  to  hold  this       *
*       field.  This includes room for a null-terminating byte if       *
*       the type is string, date, rowid etc.                            *
*                                                                       *
*   null_ok: output                                                     *
*       TRUE if the field may return a NULL value                       *
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

int rdbi_desc_slct(
	rdbi_context_def *context,
    int   sqlid,        /* Index into ptrs to Oracle work areas */
    int   pos,          /* position within select clause        */
    int   name_len,     /* The maximum size of name             */
    char *name,         /* Name of this field                   */
    int  *rdbi_type,    /* rdbi constant                        */
    int  *binary_size,  /* bytes of memory to store the field   */
    int  *null_ok      /* TRUE iff NULLs allowed               */
)
{
    rdbi_cursor_def *cursor;



    debug_on1("rdbi_desc_slct", "Pos: %d", pos);

    cursor = context->rdbi_cursor_ptrs[sqlid];

    context->rdbi_last_status = (*(context->dispatch.desc_slct))(context->drvr, cursor->vendor_data, pos,
        name_len, name, rdbi_type, binary_size, null_ok);

    debug_return(NULL, context->rdbi_last_status);
}

int rdbi_desc_slctW(
	rdbi_context_def *context,
    int   sqlid,        /* Index into ptrs to Oracle work areas */
    int   pos,          /* position within select clause        */
    int   name_len,     /* The maximum size of name             */
    wchar_t *name,         /* Name of this field                   */
    int  *rdbi_type,    /* rdbi constant                        */
    int  *binary_size,  /* bytes of memory to store the field   */
    int  *null_ok      /* TRUE iff NULLs allowed               */
)
{
    rdbi_cursor_def *cursor;



    debug_on1("rdbi_desc_slct", "Pos: %d", pos);

	if( context->dispatch.desc_slctW == NULL )
	{
		context->rdbi_last_status = RDBI_GENERIC_ERROR;
		debug_return(NULL, context->rdbi_last_status);
	}

    cursor = context->rdbi_cursor_ptrs[sqlid];

	
    context->rdbi_last_status = (*(context->dispatch.desc_slctW))(context->drvr, cursor->vendor_data, pos,
        name_len, name, rdbi_type, binary_size, null_ok);

    debug_return(NULL, context->rdbi_last_status);
}

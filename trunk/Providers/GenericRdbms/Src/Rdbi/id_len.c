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
*   rdbi_id_len - Get size for rdbi types found in Inc/rdbi.h       *
*                                                                       *
* Synopsis                                                              *
*   rdbi_id_len()                                           *
*                                                                       *
* Description                                                           *
        This function returns the maximum size of an identifier for
        each rdbms. An identifier is an object name in the rdbms eg
        a table name, column name, view name, index name etc.

* Parameters                                                            *
*                                                                       *
* Function Value                                                        *
        Maximum identifier size.
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
#include <Inc/rdbi.h>
#include <Inc/Rdbi/proto.h>
#include	<Inc/Rdbi/context.h>
#include <Inc/ut.h>

int
rdbi_id_len(rdbi_context_def *context)
{
    int size = 30;

    debug_on("rdbi_get_len");

    if (strcmp(rdbi_vndr_name(context), "ingres") == 0)
        size = 25;

    debug_return(NULL,size);
}


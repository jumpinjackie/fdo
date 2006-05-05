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
*   rdbi_vndr_name - return the name of the rdbms vendor                *
*                                                                       *
* Synopsis                                                              *
*   char *rdbi_vndr_name()                                              *
*                                                                       *
* Description                                                           *
*       Return  the  address  of  a lower case vendor name.  This       *
*       address  should be treated as  "read  only".   It  should       *
*       never be modified or free'd by the applicaiton.                 *
*                                                                       *
* Parameters                                                            *
*   none                                                                *
*                                                                       *
* Function value                                                        *
*       A valid pointer.                                                *
*                                                                       *
* Remarks                                                               *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.        *
*                                                                       *
************************************************************************/

extern char *rdbi_cat;

#include <Inc/Nls/rdbi_msg.h>
#include <Inc/debugext.h>
#include <Inc/Rdbi/proto.h>
#include <Inc/Rdbi/context.h> 

#include "global.h"                 /* rdbi global area                     */

char *rdbi_vndr_name(rdbi_context_def *context)              /* Return name string of RDBM vendor    */
{
    char *vendor;

    debug_on( "rdbi_vndr_name" );
    vendor = (*(context->dispatch.vndr_name))(context->drvr);
    debug_return(NULL, vendor);
}


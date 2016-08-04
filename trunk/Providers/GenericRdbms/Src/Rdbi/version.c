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
*   rdbi_version - return the version of the interface                  *
*                                                                       *
* Synopsis                                                              *
*   char *rdbi_version()                                                *
*                                                                       *
* Description                                                           *
*       Return 100 in the AR version of the RDBI interface.             *
*                                                                       *
* Parameters                                                            *
*   none                                                                *
*                                                                       *
* Function value                                                        *
*       An integer version number.                                      *
*                                                                       *
* Remarks                                                               *
*       NOTE:  this is a temporary routine to allow for the fact that   *
*       we have two versions of the RDBI interface (AR and non-AR).     *
*       This routine will disappear when the AR version of RDBI replaces*
*       the non-AR version--so don't use this routine!                  *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.        *
*                                                                       *
************************************************************************/

extern char *rdbi_cat;

#include    <Inc/Nls/rdbi_msg.h>
#include <Inc/debugext.h>
#include <Inc/Rdbi/methods.h>
#include	<Inc/Rdbi/context.h>

#include "global.h"                 /* rdbi global area                     */

int
rdbi_version(rdbi_context_def *context)
{
    debug_on("rdbi_version");
    debug_return(NULL,100);
}


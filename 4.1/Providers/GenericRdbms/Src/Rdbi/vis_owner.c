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
*   rdbi_vis_owner - Return the name of the RDBMS vision owner.         *
*                                                                       *
* Synopsis                                                              *
*   char *rdbi_vis_owner()                                              *
*                                                                       *
* Description                                                           *
*       Return the name of the vision owner account.                    *
*                                                                       *
* Remarks                                                               *
*       The  RDBMS  vision owner is different the OS vision owner       *
*       in the case  of  oracle.   The  rdbms name is preceded by       *
*       ops$.                                                           *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.        *
*                                                                       *
************************************************************************/

#include <stdlib.h>             /* getenv()                         */
#include <stdio.h>
#include <Inc/debugext.h>
extern char *rdbi_cat;

#include    <Inc/Nls/rdbi_msg.h>
#include <string.h>
#include <Inc/rdbi.h>
#include <Inc/Rdbi/proto.h>
#include	<Inc/Rdbi/context.h>

char *rdbi_vis_owner(rdbi_context_def *context)
{
    char        *p;
    int          is_oracle;

    debug_on("rdbi_vis_owner");

    if (strcmp(context->vision_owner, "") == 0) {
        p = getenv("VISION_OWNER");

        is_oracle = (strcmp(rdbi_vndr_name(context), "oracle") == 0);

        if (p == NULL) {
            if (is_oracle)
                (void) sprintf(context->vision_owner, "ops$%s", "vision");
            else
                (void) strcpy(context->vision_owner, "vision");
        } else {
            if (is_oracle)
                (void) sprintf(context->vision_owner, "ops$%s", p);
            else
                (void) strcpy(context->vision_owner, p);
        }
    }

    debug_return(NULL,context->vision_owner);
}


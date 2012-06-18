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
*   rdbi_get_info  -  retrieve requested piece of info from the RDBMS   *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_get_info(keyword, type, value, exists)                         *
*   char    *keyword;                                                   *
*   int     type;                                                       *
*   void    *value;                                                     *
*   int     *exists;                                                    *
*                                                                       *
* Description                                                           *
*       This  module  retrieves  a requested piece of information       *
*       from the active RDBMS based on the contents of the string       *
*       it receives as input.                                           *
*                                                                       *
* Parameters                                                            *
*   keyword: input                                                      *
*       name of the information to retrieve                             *
*                                                                       *
*   type:   input                                                       *
*       type   of   the   information   (integer   constant   ie:       *
*       RDBI_STRING)                                                    *
*                                                                       *
*   value:  output                                                      *
*       value of the information retrieved                              *
*                                                                       *
*   exists: output                                                      *
*       TRUE if value exists                                            *
*                                                                       *
* Function value                                                        *
*       TRUE if no errors.                                              *
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


#include <Inc/ut.h>
#include <Inc/debugext.h>

#include    <Inc/Nls/rdbi_msg.h>
#include <Inc/rdbi.h>
#include "global.h"
#include	<Inc/Rdbi/context.h>

#ifndef _WIN32
#define _stricmp  strcasecmp
#endif
int
rdbi_get_info(
rdbi_context_def *context,
char    *keyword,
int     type,
void    *value,
int     *exists)
{
    int     status = FALSE;

    debug_on("rdbi_get_info");

    *exists = FALSE;
	if (_stricmp(keyword, RDBI_CONNECT_VARIABLE) == 0) {
        status = (*(context->dispatch.get_con_var))(context->drvr, (char*)value);
        *exists = TRUE;
    }

    debug3("keyword %s, value %s, exists %d", keyword, value, *exists);

    debug_return(NULL, status);

}


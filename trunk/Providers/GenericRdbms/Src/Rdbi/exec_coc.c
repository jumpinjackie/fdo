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
*   rdbi_exec_coc - execute a "current of cursor" statement             *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_exec_coc(sqlid, sql, rows)                                     *
*   int   sqlid;                                                        *
*   char *sql;                                                          *
*   int  *rows;                                                         *
*                                                                       *
* Description                                                           *
*       Execute  a  delete  or  update where "current of cursor".       *
*       The operation active on the  cursor must be a "select ...       *
*       for update...".  The sql statement must  be  an update or       *
*       delete command.  It may not contain a where clause; since       *
*       it  will  be  qualified  to operate on the  current  row.       *
*                                                                       *
*       RULES: an exec_coc is legal  only after a fetch which did       *
*       not reach "end-of-fetch".  exec_coc may not be used twice       *
*       consecutively on the same row.                                  *
*                                                                       *
* Parameters                                                            *
*   sqlid: input                                                        *
*       Index  into  internal  global  array of  sqlca  pointers.       *
*                                                                       *
*   sql: input                                                          *
*       The update or delete SQL statement                              *
*                                                                       *
*   rows: output                                                        *
*       The number of rows processed.   On  success, should be 1.       *
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
#include <Inc/rdbi.h>               /* rdbi status codes        */
#include <Inc/Rdbi/context.h>

#include "global.h"                 /* rdbi global area         */

int
rdbi_exec_coc(
	rdbi_context_def *context,
    int     sqlid,                  /* cursor id                */
    char   *sql,                    /* SQL statement to execute */
    int    *rows                   /* # of rows modified       */
)
{
	return RDBI_GENERIC_ERROR;
}


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
*   rdbi_crsr - variety of modules to return cursor information         *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*                                                                       *
* Description                                                           *
*       Return  components  of  the active cursor structure.  For       *
*       now, only rdbi_crsr_nrows() is functional; it will return       *
*       the  number of rows  processed  for  the  active  cursor.       *
*                                                                       *
* Parameters                                                            *
*   sqlid: input                                                        *
*       Index into  internal  global  array  of  sqlca  pointers.       *
*                                                                       *
* Function value                                                        *
*                                                                       *
* Remarks                                                               *
*       No messages issued here.                                        *
*                                                                       *
*       This  routine   will   not  update  the  rdbi_last_status       *
*       global.                                                         *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.        *
*                                                                       *
*   FILE_DEPENDENCY(LTCART): WARNING! This code is duplicated in PL/SQL.
************************************************************************/

#include <Inc/ut.h>
#include <Inc/debugext.h>

#include    <Inc/Nls/rdbi_msg.h>
#include <Inc/rdbi.h>           /* rdbi status codes    */
#include "global.h"
#include <Inc/Rdbi/cursor.h>
#include <Inc/Rdbi/context.h>

int rdbi_crsr_nrows(
    rdbi_context_def  *context,
    int sqlid )
{
    rdbi_cursor_def *cursor;    /* convenience pointer                  */

    debug_on("rdbi_crsr_nrows");

    cursor = context->rdbi_cursor_ptrs[sqlid];
    debug_return(NULL, cursor->rows_processed);
}

#if 0
rdbi_crsr_tnrows(sqlid)         /* total # rows processed for all       */
    int sqlid;                  /* executes.                            */

rdbi_crsr_est(sqlid)            /* returns TRUE if sqlid established    */
    int sqlid;

rdbi_crsr_parsd(sqlid)          /* TRUE if sql is parsed                */
    int sqlid;

rdbi_crsr_bound(sqlid)          /* TRUE if input variables are bound    */
    int sqlid;

rdbi_crsr_defnd(sqlid)          /* TRUE if output variables are defined */
    int sqlid;

rdbi_crsr_rc(sqlid)             /* gvc status of last rdbms operation   */
    int sqlid;

rdbi_crsr_rdbrc(sqlid)          /* status of last rdbms operation       */
    int sqlid;

char *rdbi_crsr_sql(sqlid)      /* returns pointer to SQL statement     */
    int sqlid;

rdbi_crsr_nexec(sqlid)          /* # times this statement executed      */
    int sqlid;

rdbi_crsr_index(sqlid)          /* Index into external SQL file         */
    int sqlid;

rdbi_crsr_dbg(sqlid)            /* display debug info for this stmt     */
    int sqlid;

#endif


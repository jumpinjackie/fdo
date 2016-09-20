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
*   rdbi_commit - Close off a transaction. Commit updates. Free locks.  *
*                                                                       *
* Synopsis                                                              *
*   rdbi_commit()                                                       *
*                                                                       *
* Description                                                           *
*       This  routine  will  commit all updates to the RDBMS.  It       *
*       will  also free all  locks  held  in  the  current  RDBMS       *
*       connection (except  maybe,  internal  Oracle cursor parse       *
*       locks).                                                         *
*                                                                       *
* Parameters                                                            *
*   None                                                                *
*                                                                       *
* Returns                                                               *
*   An rdbi return code - see Inc/rdbi.h                                *
*                                                                       *
* Remarks                                                               *
*       This routine is preferred over executing an SQL 'commit;'       *
*       statement in that it is  more efficient and offers better       *
*       opportunity  to  control/monitor  the current transaction       *
*       state.                                                          *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.        *
*                                                                       *
*   FILE_DEPENDENCY(LTCART): WARNING! This code is duplicated in PL/SQL.
************************************************************************/


#include    <Inc/Nls/rdbi_msg.h>
#include <Inc/debugext.h>
#include <Inc/rdbi.h>       /* for RDBI_COMMIT_ONCE     */
#include "global.h"         /* for rdbi_last_status     */
#include <Inc/Rdbi/context.h>

int
rdbi_commit(rdbi_context_def *context)
{
    debug_on2("rdbi_commit", "Commit database %s.R%d",
                        context->rdbi_cnct == NULL ? " None" : context->rdbi_cnct->db_name,
                        context->rdbi_cnct == NULL ? -1      : context->rdbi_cnct->connect_id);

    context->rdbi_last_status = (*(context->dispatch.commit))(context->drvr, RDBI_COMMIT_ONCE);

    debug_return(NULL, context->rdbi_last_status);
}


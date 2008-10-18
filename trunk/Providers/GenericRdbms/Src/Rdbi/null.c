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
*   rdbi_alcnullind - allocate a null indicator array                   *
*   rdbi_set_null - set a member of a null indicator array NULL         *
*   rdbi_set_nnull - set a member of a null indicator array NOT NULL    *
*   rdbi_is_null - check the NULL status of a null indicator array member
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_alcnullind(n, null_ind)                                        *
*   int   n;                                                            *
*   short *null_ind;                                                    *
*                                                                       *
*   rdbi_set_null(null_ind, start, end)                                 *
*   short *null_ind;                                                    *
*   int   start;                                                        *
*   int   end;                                                          *
*                                                                       *
*   rdbi_set_nnull(null_ind, start, end)                                *
*   short *null_ind;                                                    *
*   int   start;                                                        *
*   int   end;                                                          *
*                                                                       *
*   rdbi_is_null(null_ind, offset)                                      *
*   short *null_ind;                                                    *
*   int   offset;                                                       *
*                                                                       *
* Description                                                           *
*       Interface  to  allocate,  set  and  check  NULL indicator       *
*       variables.                                                      *
*                                                                       *
* Parameters                                                            *
*   null_ind: input/output                                              *
*       The RDBMS-maintained array of indicator variables               *
*                                                                       *
*   start: input                                                        *
*       When  dealing  with a range of indicator  variables,  the       *
*       start index                                                     *
*                                                                       *
*   end: input                                                          *
*       When dealing with a range of indicator variables, the end       *
*       index                                                           *
*                                                                       *
*   offset: input                                                       *
*       When dealing with  an  individual indicator variable, the       *
*       index.                                                          *
*                                                                       *
* Function value                                                        *
*       An rdbi status code from Inc/rdbi.h                             *
*                                                                       *
* Remarks                                                               *
*       No messages issued here.                                        *
*                                                                       *
*       rdbi_alcnullind() will update rdbi_last_status.                 *
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
#include <Inc/rdbi.h>   /* rdbi status codes                    */

#include "global.h"     /* rdbi global area                     */
#include <Inc/Rdbi/context.h>
#include <Inc/ut.h>

int rdbi_alcnullind(
	rdbi_context_def *context,
    int    n,           /* The number of elements to allocate   */
    char  **null_ind)  /* address of pointer to allocate       */
{
    int     rc;

    debug_on1("rdbi_alcnullind", "Number of elements %d", n);

    rc = (*(context->dispatch.alcnullind)) (context->drvr, n, null_ind );

    debug_return(NULL, rc);
}

void rdbi_set_null(
	rdbi_context_def *context,
    void *null_ind,    /* NULL indicator array                     */
    int   start,        /* The start index to begin setting NULL    */
    int   end)          /* The end index to set NULL                */
{
    debug_on("rdbi_set_null");

    (void) (*(context->dispatch.set_null)) (context->drvr, null_ind, start, end );

    debug_return_void(NULL);
}

void rdbi_set_nnull(
	rdbi_context_def *context,
    void *null_ind,    /* NULL indicator array                     */
    int   start,        /* The start index to begin setting NULL    */
    int   end)          /* The end index to set NULL                */
{
    debug_on("rdbi_set_nnull");

    (void) (*(context->dispatch.set_nnull)) (context->drvr, null_ind, start, end );

    debug_return_void(NULL);
}

int rdbi_is_null(
	rdbi_context_def *context,
    void *null_ind,    
    int  offset)
{
    int     ret;

    debug_on("rdbi_is_null");

    ret = (*(context->dispatch.is_null)) (context->drvr, null_ind, offset );

    debug_return(NULL, ret);
}


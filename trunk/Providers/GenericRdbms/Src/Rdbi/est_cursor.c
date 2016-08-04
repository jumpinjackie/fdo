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
*   rdbi_est_cursor - establish an SQL control area.                    *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_est_cursor(sqlid)                                              *
*   rdbi_est_cursor_obj(sqlid)                                          *
*   int *sqlid;                                                         *
*                                                                       *
* Description                                                           *
*       Open a cursor area for the application.  This will:             *
*        - malloc the cursor area within rdbi                           *
*        - do the rdb specific cursor open                              *
*        - return an index identifying this cursor area                 *
*                                                                       *
* Parameters                                                            *
*   sqlid: output                                                       *
*       Index into internal global array of cursors.                    *
*                                                                       *
* Remarks                                                               *
*       There's  a  field  in  the active rdbi_connect_def called       *
*       "cntxt_ptrs".   which  is  an  array   of   pointers   to       *
*       rdbi_cursor_def's for  this  connection.   There's also a       *
*       field  called  "num_cursor_ptrs"  to  indicate  how  many       *
*       pointers are in the array.   New pointers are set to NULL       *
*       as  the  array  of  pointers is  (re)alloc'd.   When  the       *
*       application needs a new cursor  area,  we  use  the first       *
*       available (NULL) cursor area.                                   *
*                                                                       *
*       The  "sqlid" indice returned by this routine is an  input       *
*       parameter to most other rdbi routines.                          *
*                                                                       *
*       No  messages  issued  here.   This  routine  updates  the       *
*       rdbi_last_status global                                         *
*                                                                       *
* Function value                                                        *
*       An rdbi status code from Inc/rdbi.h                             *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.            *
*                                                                       *
*   FILE_DEPENDENCY(LTCART): WARNING! This code is duplicated in PL/SQL.
************************************************************************/

#include <Inc/ut.h>
#include <Inc/debugext.h>

#include    <Inc/Nls/rdbi_msg.h>
#include <Inc/rdbi.h>               /* rdbi status codes    */
#include <Inc/Rdbi/context.h>
#include	<Inc/Rdbi/context.h>

#include "global.h"                 /* rdbi global area     */

#define TYPE_RELATIONAL         (0)
#define TYPE_OBJECT_RELATIONAL  (1)

static int realloc_ptrs(rdbi_context_def *context, int *index);
static int rdbi_est_cursor_S(rdbi_context_def *context, int type, int *sqlid);


int rdbi_est_cursor(
	rdbi_context_def *context,
    int *sqlid)
{
    return rdbi_est_cursor_S( context, TYPE_RELATIONAL, sqlid );
}


int rdbi_est_cursor_obj(
	rdbi_context_def *context,
    int *sqlid)
{
    return rdbi_est_cursor_S( context, TYPE_OBJECT_RELATIONAL, sqlid );
}


static int rdbi_est_cursor_S(
	rdbi_context_def *context,
    int type,
    int *sqlid)
{
    int              status;
    int              found;
    int              index;     /* index to return      */
    rdbi_cursor_def *cursor;

    debug_on1("rdbi_est_cursor_obj:rdbi_est_cursor_S", "type %s", 
            (type == TYPE_RELATIONAL) ? "RELATIONAL" :
            (type == TYPE_OBJECT_RELATIONAL ) ? "OBJECT_RELATIONAL" : "unknown");

    if (! context->rdbi_initialized) {
        status = RDBI_NOT_CONNECTED;
        goto the_exit;
    }

    /*
     * Find the first available (NULL) pointer
     */
    found = FALSE;
    for (index=0; index < context->rdbi_num_cursor_ptrs; index++) {
        if (context->rdbi_cursor_ptrs[index] == (rdbi_cursor_def *)NULL) {
            found = TRUE;
            break;
        }
    }

    /*
     * No available cursor area was found, realloc the array of pointers so
     * we'll have more.
     */
    if (!found) {
        if (!realloc_ptrs(context, &index)) {
            status = RDBI_MALLOC_FAILED;
            goto the_exit;
        }
    }

    cursor = context->rdbi_cursor_ptrs[index] = (rdbi_cursor_def *)
            ut_vm_malloc("rdbi_est_cursor_S", sizeof(rdbi_cursor_def));
    if (cursor == (rdbi_cursor_def *)NULL) {
        status = RDBI_MALLOC_FAILED;
        goto the_exit;
    }

    /*
     * Initialize cursor area components
     */
#ifdef _DEBUG
    cursor->sql             = NULL;
#endif
    cursor->status          = RDBI_SUCCESS;
    cursor->sql_parsed      = FALSE;
    cursor->sel_for_update  = FALSE;
    cursor->exec_coc_legal  = FALSE;
    cursor->bound_vars      = FALSE;
    cursor->defined_vars    = FALSE;
    cursor->n_executions    = 0;
    cursor->rows_processed  = 0;
    cursor->trows_processed = 0;
    cursor->statement_type  = 0;
    cursor->sql_index       = 0;
    cursor->tran_begun      = FALSE;
    strcpy(cursor->verb,"");

    /*
     * Do the vendor-specific cursor open
     */
    switch ( type ) {
      case TYPE_RELATIONAL:
        status = (*(context->dispatch.est_cursor))(context->drvr, &(cursor->vendor_data));
        break;
      case TYPE_OBJECT_RELATIONAL:
        status = (*(context->dispatch.est_cursor_obj))(context->drvr, &(cursor->vendor_data));
        break;
      default:
        goto the_exit;
    }

    if (status == RDBI_SUCCESS)
        *sqlid = index;

    debug2( "Established cursor #%d with driver data at 0x%lx",
            index, (long) cursor->vendor_data );

the_exit:
    context->rdbi_last_status = status;
    debug_return(NULL, status);
}

/************************************************************************
*  Extend the array of pointers so we can have more cursor areas.       *
*  Return the next available index.                                     *
************************************************************************/
static int realloc_ptrs(
    rdbi_context_def *context,
    int *index)
{
    int               i;
    rdbi_cursor_def **old_cursor_ptr;   /* save in case realloc fails   */
    int               old_max_index;    /* # index ptrs before realloc  */

    debug_on("est_cursor: realloc_ptrs");

    old_max_index         = context->rdbi_num_cursor_ptrs;
    old_cursor_ptr        = context->rdbi_cursor_ptrs;

    context->rdbi_num_cursor_ptrs += RDBI_CONTEXT_MALLOC_COUNT;
    context->rdbi_cursor_ptrs      = (rdbi_cursor_def **)realloc(
                    context->rdbi_cursor_ptrs,
                    context->rdbi_num_cursor_ptrs * sizeof(rdbi_cursor_def *));

    if (context->rdbi_cursor_ptrs == (rdbi_cursor_def **)NULL) {/* uh -oh ...    */
        context->rdbi_num_cursor_ptrs = old_max_index;
        context->rdbi_cursor_ptrs     = old_cursor_ptr;
        debug_return(NULL,FALSE);
    }

    /*
     * Set newly alloc'd pointers to NULL,
     * and return our next available index.
     */
    for (i = old_max_index; i < context->rdbi_num_cursor_ptrs; i++)
        context->rdbi_cursor_ptrs[i] = (rdbi_cursor_def *)NULL;

    *index = old_max_index;

    debug_return(NULL,TRUE);
}


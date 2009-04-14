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
*   rdbi_bind - Define an output location for value retrieval           *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_bind(sqlid, name, datatype, size, address, null_ind)           *
*   int   sqlid;                                                        *
*   char *name;                                                         *
*   int   datatype;                                                     *
*   int   size;                                                         *
*   char *address;                                                      *
*   char *null_ind;                                                     *
*                                                                       *
* Description                                                           *
*       When  we  have  bound  variables in an SQL statement, use       *
*       this routine to specify the address for the rdbms to look       *
*       for input values.  NULL values  are  handled  by the null       *
*       indicator array.                                                *
*                                                                       *
* Parameters                                                            *
*   sqlid: input                                                        *
*       Index into internal global array of cursors                     *
*                                                                       *
*   name: input                                                         *
*       The column identifier.  Either a name or a number.              *
*                                                                       *
*   datatype: input                                                     *
*       The rdbi data type - found in Inc/rdbi.h                        *
*                                                                       *
*   size: input                                                         *
*       Binary size (# bytes).  Will be used only  if size is not       *
*       apparent from 'datatype'.                                       *
*                                                                       *
*   address: input                                                      *
*       Address where we will place the input value.                    *
*                                                                       *
*   null_ind: input                                                     *
*       Address  at   which   we   will   place   NULL  indicator       *
*       variable(s).   Use  rdbi_alcnullind()  to  allocate  this       *
*       array, and rdbi_set_null() rdbi_set_nnull() to initialize       *
*       NULL states.                                                    *
*                                                                       *
* Function value                                                        *
*       An rdbi status code from Inc/rdbi.h                             *
*                                                                       *
* Remarks                                                               *
*       When  performing  array  processing,  the  'address'  and       *
*       'null_ind' variables become arrays of values.                   *
*                                                                       *
*       No  messages   issued  here.   This  module  updates  the       *
*       rdbi_last_status global.                                        *
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

#include <Inc/rdbi.h>   /* rdbi status codes                        */

#include "global.h"     /* rdbi global area                         */
#include <Inc/Rdbi/context.h>

int
rdbi_bind(
    rdbi_context_def *context,
    int   sqlid,        /* index into cursor array                  */
    char *name,         /* column/expression position               */
    int   datatype,     /* A data type from Inc/rdbi.h              */
    int   size,         /* binary size                              */
    char *address,      /* data address                             */
    void *null_ind     /* pointer to null indicator variables      */
)
{
    rdbi_cursor_def *cursor;            /* convenience pointer      */


    debug_on4("rdbi_bind", "sqlid: %d pos: %s, type: %d, size: %d",
                sqlid, name, datatype, size);

    /*
     * If it's type string, it may not have a zero length.
     * Note that it may have a length of '-1' for null-terminated strings.
     */
    if ((datatype == RDBI_STRING) && (size == 1)) {
        context->rdbi_last_status = RDBI_ZERO_LEN_STRING;
        goto the_exit;
    }


    cursor = context->rdbi_cursor_ptrs[sqlid];

    /*
     * Make the rdb call.
     */
    context->rdbi_last_status = (*(context->dispatch.bind))(context->drvr, cursor->vendor_data, name, datatype,
                                            size, address, null_ind);

    /*
     * If we've successfully done at least one bind, set the flag
     * to indicate this condition. (It doesn't necessarily mean that
     * ALL variables have been bound).
     */
    if (context->rdbi_last_status == RDBI_SUCCESS)
        cursor->bound_vars = TRUE;

the_exit:
    debug_return(NULL, context->rdbi_last_status);
}


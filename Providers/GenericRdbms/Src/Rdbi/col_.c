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
*   rdbi_col_act - Activate a fetch of all columns for an object        *
*                                                                       *
* Synopsis                                                              *
*   #include "<Inc/rdbi.h>                                              *
*   rdbi_col_act(owner, object_name)                                    *
*   char *owner;                                                        *
*   char *object_name;                                                  *
*                                                                       *
* Description                                                           *
*       This  module  activates  a  fetch  of  all columns for an       *
*       object in a database.                                           *
*                                                                       *
* Parameters                                                            *
*   owner: input                                                        *
*       The owner of the object.                                        *
*                                                                       *
*   object_name: input                                                  *
*       The name of the object.                                         *
*                                                                       *
* Function value                                                        *
*       Returns an rdbi status code from Inc/rdbi.h                     *
*                                                                       *
* Remarks                                                               *
*       Global rdbi_last_status is updated here.                        *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.        *
*                                                                       *
************************************************************************/

#include <Inc/debugext.h>

#include    <Inc/Nls/rdbi_msg.h>
#include "global.h"
#include <Inc/Rdbi/proto.h>
#include <Inc/Rdbi/context.h>

static char     *tran_id = "auto-col";

int rdbi_col_act(
    rdbi_context_def *context,
    char *owner,
    char *object_name,
    char *dbaselink
)
{
    int     status;
    int     tran_begun = FALSE;


    debug_on2("rdbi_col_act", "owner='%s', object='%s'", ISNULL(owner), ISNULL(object_name));

    if (context->rdbi_cnct->autocommit_on) {
        rdbi_tran_begin(context, tran_id);
        tran_begun = TRUE;
    }

    status = (*(context->dispatch.col_act))(context->drvr, owner, object_name, dbaselink);

    context->rdbi_last_status = status;

    /* ingdr_col_act prefetches all columns so its safe to end the transaction
    *   here without getting fetch across commit problems.
    */

    if ( tran_begun ) {
        rdbi_tran_end(context, tran_id);
    }

    debug_return(NULL, status);

}


/************************************************************************
*                                                                       *
* Name                                                                  *
*   rdbi_col_get - Get the next column.                                 *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_col_get(column_name, type, length, scale,                      *
*                nullable, is_autoincrement, position,                  *
*                eof)                                                   *
*   char *column_name;                                                  *
*   char *type;                                                         *
*   int  *length;                                                       *
*   int  *scale;                                                        *
*   int  *nullable;                                                     *
*   int  *is_autoincrement;                                             *
*   int  *position;                                                     *
*   int  *eof;                                                          *
*                                                                       *
* Description                                                           *
*       This  module  gets the next column in the fetch activated       *
*       by rdbi_col_act.                                                *
*                                                                       *
* Parameters                                                            *
*   column_name: output                                                 *
*       The name of the column.                                         *
*                                                                       *
*   type: output                                                        *
*       The data type of the column.                                    *
*                                                                       *
*   length: output                                                      *
*       Column length.                                                  *
*                                                                       *
*   scale: output                                                       *
*       Number of digits  after  the  decimal point for data type       *
*       NUMBER.                                                         *
*                                                                       *
*   nullable: output                                                    *
*       TRUE if the column can be NULL.                                 *
*                                                                       *
*   is_autoincrement: output                                            *
*       TRUE if the column values are autoincremented                   *
*       (i.e. a DB-supported sequence)                                  *
*                                                                       *
*   position: output                                                    *
*       Column position.                                                *
*                                                                       *
*   eof: output                                                         *
*       End of fetch flag.                                              *
*                                                                       *
* Function value                                                        *
*       Returns an rdbi status code from Inc/rdbi.h                     *
*                                                                       *
* Remarks                                                               *
*       Global rdbi_last_status is updated here.                        *
*                                                                       *
************************************************************************/

int rdbi_col_get(
	rdbi_context_def *context,
    char *column_name,
    char *type,
    int  *length,
    int  *scale,
    int  *nullable,
    int  *is_autoincrement,
    int  *position,
    int  *eof)
{
    int   status;


    debug_on("rdbi_col_get");

    status = (*(context->dispatch.col_get))(context->drvr, column_name, type, length, scale, nullable, is_autoincrement,
                            position, eof);

    context->rdbi_last_status = status;

    debug_area()
    {
        if (*eof)
        {
            debug0("eof=TRUE");
        }
        else
        {
            debug6("column='%s', type='%s', length=%d, scale=%d, nullable=%s, position=%d",
                ISNULL(column_name), ISNULL(type), *length, *scale, ISTRUE(*nullable), *position);
        }
    }
    debug_return(NULL, status);

}
/************************************************************************
*                                                                       *
* Name                                                                  *
*   rdbi_col_deac - Dectivate a fetch of all columns for an object      *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_col_deac()                                                     *
*                                                                       *
* Description                                                           *
*       This  module deactivates  a fetch  of  all columns for an       *
*       object in a database.                                           *
*                                                                       *
* Parameters                                                            *
*                                                                       *
* Function value                                                        *
*       Returns an rdbi status code from Inc/rdbi.h                     *
*                                                                       *
* Remarks                                                               *
*       Global rdbi_last_status is updated here.                        *
*                                                                       *
************************************************************************/

int rdbi_col_deac(rdbi_context_def *context)
{
    int   status;


    debug_on("rdbi_col_deac");

    status = (*(context->dispatch.col_deac))(context->drvr);

    context->rdbi_last_status = status;

    debug_return(NULL, status);

}





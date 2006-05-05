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
*   rdbi_get_msg - Get the message text for the last operation.         *
*                                                                       *
* Synopsis                                                              *
*   void rdbi_get_msg(msg_buf)                                          *
*   char *msg_buf;                                                      *
*                                                                       *
* Description                                                           *
*       Get  the  message  text  for  the  last operation on this       *
*       rdbms.                                                          *
*                                                                       *
* Parameters                                                            *
*   msg_buf: output                                                     *
*       Application  buffer  in which to put the  message.   This       *
*       buffer should be  at least RDBI_MSG_SIZE long.  (#include       *
*       <Inc/rdbi.h>).                                                  *
*                                                                       *
* Function value                                                        *
*       void                                                            *
*                                                                       *
* Remarks                                                               *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.            *
*                                                                       *
************************************************************************/


#include <Inc/ut.h>                 /* for ut_isprint()                     */
#include <Inc/Nls/rdbi_msg.h>
#include <Inc/ut.h>
#include <Inc/debugext.h>
#include <Inc/rdbi.h>               /* rdbi status values                   */
#include "global.h"                 /* rdbi global area - rdbi_last_status  */
#include <Inc/Rdbi/proto.h>
#include	<Inc/Rdbi/context.h>

void rdbi_get_msg(rdbi_context_def *context)               /* Get msg text for last operation      */
{
    debug_on("rdbi_get_msg");

    /*
     * Take care of the messages which have no RDBMS equivalent here,
     * and call a vendor routine for the rest.
     */

    switch (context->rdbi_last_status) {
        case RDBI_SUCCESS :
            rdbi_msg_set_0( context, RDBI_3, "RDBMS: Normal, successful completion.");
            break;

        case RDBI_MALLOC_FAILED :
            rdbi_msg_set_0( context, RDBI_4, "RDBMS: Memory allocation failure.");
            break;

        case RDBI_OBJECT_EXISTS :
            rdbi_msg_set_0( context, RDBI_5, "RDBMS: Table or view already exists.");
            break;

        case RDBI_TOO_MANY_CONNECTS :
            rdbi_msg_set_0( context, RDBI_6, "RDBMS: Too many connections active - can't establish another.");
            break;

        case RDBI_TOO_FEW_CURSORS :
            rdbi_msg_set_0( context, RDBI_7, "RDBMS: Cannot allocate another cursor. Must free one first.");
            break;

        case RDBI_NOT_IN_DESC_LIST :
            rdbi_msg_set_0( context, RDBI_8, "RDBMS: Select expression or bind variable does not exist.");
            break;

        case RDBI_INVALID_UPDATE :
            rdbi_msg_set_0( context, RDBI_9, "RDBMS: Either no row has been fetched, or this is not a 'select for update'.");
            break;

        case RDBI_NOT_CONNECTED :
            rdbi_msg_set_0( context, RDBI_10, "RDBMS: Not connected to the specified database.");
            break;

        case RDBI_ZERO_LEN_STRING :
            rdbi_msg_set_0( context, RDBI_12, "RDBMS: Failed to bind/define zero length string");
            break;

        case RDBI_INCOMPATIBLE_COLUMN_TYPES :
            rdbi_msg_set_0( context, RDBI_25, "RDBMS: Incompatible column type");
            break;

        case RDBI_END_OF_FETCH :
            rdbi_msg_set_0( context, RDBI_26, "RDBMS: End-of-fetch reached");
            break;

        case RDBI_RESOURCE_BUSY :
            rdbi_msg_set_0( context, RDBI_13, "RDBMS: Lock conflict with another user");
            break;

        case RDBI_NO_SUCH_CURRENT_OF_CURSOR :
            rdbi_msg_set_0(context, RDBI_16, "RDBMS: Failed to locate cursor referenced by current of cursor clause");
            break;

        case RDBI_NO_SUCH_CURRENT_OF_WHERE :
            rdbi_msg_set_0( context, RDBI_27, "RDBMS: Failed to locate current of <cursor> clause in SQL statement");
            break;

        case RDBI_NOT_SELECT_FOR_UPDATE :
            rdbi_msg_set_0( context, RDBI_17, "RDBMS: Cursor referenced by current of cursor clause must be a for update cursor");
            break;

        case RDBI_INVLD_DESCR_OBJTYPE:
            rdbi_msg_set_0( context, RDBI_32, "RDBMS: Invalid object type to describe.");
            break;

		case RDBI_GEOMETRY_CONVERION_ERROR:
			rdbi_msg_set_S( context, RDBI_33, "RDBMS(%1$ls): Geometry conversion error.",(*(context->dispatch.vndr_name))(context->drvr));
			break;

		case RDBI_DATA_TRUNCATED:
			rdbi_msg_set_0( context, RDBI_35, "RDBMS: Data truncation error.");
			break;

        default:
			{
				char	msg_buf[RDBI_MSG_SIZE];
            (*(context->dispatch.get_msg))(context->drvr, msg_buf);
			rdbi_msg_set_S( context, RDBI_34,"RDBMS: %1$ls",msg_buf );
			}
    }
    debug_return_void(NULL);
}


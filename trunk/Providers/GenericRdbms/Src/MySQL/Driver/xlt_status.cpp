/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ************************************************************************/

#include "stdafx.h"
#include <wchar.h>
#include "errno.h"
#include "xlt_status.h"
#include "nls.h"

static void reset_last_msg( 
	mysql_context_def	*context
    );

static void set_last_msg( 
	mysql_context_def	*context,
    MYSQL* mysql,
    MYSQL_STMT* stmt
    );

static void cat_msg( 
	wchar_t*& msg,
	const wchar_t* msg2,
    size_t& len_left
    );

int
mysql_xlt_status( 
	mysql_context_def	*context,
    int mysql_status,
    MYSQL* mysql,
    MYSQL_STMT* stmt
    )
{
    int  rdbi_status = RDBI_GENERIC_ERROR;

    switch( mysql_status ) {

        case MYSQL_SUCCESS :
            rdbi_status = RDBI_SUCCESS;
            break;

        case MYSQL_END_OF_FETCH :
            rdbi_status = RDBI_END_OF_FETCH;
            break;

        case MYSQL_DUPLICATE_INDEX :
            rdbi_status = RDBI_DUPLICATE_INDEX;
            break;

		case MYSQL_DATA_TRUNCATED :
			rdbi_status = RDBI_DATA_TRUNCATED;
			break;

// these errors may need handling:

        /* case mysql_NOT_IN_DESC_LIST :
            rdbi_status = RDBI_NOT_IN_DESC_LIST;
            break;

        case mysql_INVALID_UPDATE :
            rdbi_status = RDBI_INVALID_UPDATE;
            break;

        case mysql_OBJECT_EXISTS :
            rdbi_status = RDBI_OBJECT_EXISTS;
            break;

        case mysql_TABLE_NONEXISTENT :
            rdbi_status = RDBI_NO_SUCH_TABLE;
            break;

        case mysql_INVALID_COLUMN_NAME :
            rdbi_status = RDBI_NO_SUCH_COLUMN;
            break;

        case mysql_TABLE_CONSTRAINT_NONEXISTENT :
            rdbi_status = RDBI_NO_SUCH_TABLE_CONSTRAINT;
            break;

        case mysql_INCOMPATIBLE_COLUMN_TYPES :
            rdbi_status = RDBI_INCOMPATIBLE_COLUMN_TYPES;
            break;

        case mysql_RESOURCE_BUSY :
            rdbi_status = RDBI_RESOURCE_BUSY;
            break;

        case mysql_INVALID_DROP_OPTION :
            rdbi_status = RDBI_INVALID_DROP_OPTION;
            break;

        */

        default:
            rdbi_status = RDBI_GENERIC_ERROR;
    }

    if ( mysql_status == MYSQL_SUCCESS ) 
        reset_last_msg( context );
    else
        set_last_msg( context, mysql, stmt );

    return rdbi_status;
}

void reset_last_msg( 
	mysql_context_def	*context
    )
{
    context->mysql_last_err_msg[0] = '\0';
}

void set_last_msg( 
	mysql_context_def	*context,
    MYSQL* mysql,
    MYSQL_STMT* stmt
    )
{
    const char *error;
    const char *dflt_error = "";
    wchar_t* err_msg = context->mysql_last_err_msg;
    size_t err_msg_left = RDBI_MSG_SIZE;

    unsigned long client_version = mysql_get_client_version();

    // The messages issued, when MySQL client or server is too old, are rather 
    // cryptic. Therefore, prepend more explanatory info when one of the other
    // is too old to work with the MySQL Provider.

    if ( client_version < MIN_CLIENT_VER ) {
        // Pre-version 5 clients can't handle some version 5 data types used in the
        // information_schema, so MySQL provider can't work with them.
        const wchar_t* client_msg = mysql_nls_client_version( client_version );
        cat_msg( err_msg, client_msg, err_msg_left );
    }

    if ( mysql ) {
        unsigned long server_version = mysql_get_server_version( mysql );

        if ( server_version < MIN_SERVER_VER ) {
            // Also, various errors can occur when database has too old a version.
            const wchar_t* server_msg = mysql_nls_server_version( server_version );
            cat_msg( err_msg, server_msg, err_msg_left );
        }
    }

    if (stmt && mysql_stmt_errno (stmt))
    {
        // Get error message from supplied statement
        error = mysql_stmt_error (stmt);
    }
    else if (mysql)
    {
        // No statement so get error message directly from connection.
        error = mysql_error (mysql);
    }
    else 
        // Should never get here, but assume no error if no connection or 
        // statement was supplied.
        error = dflt_error;

    swprintf( err_msg, err_msg_left, L"%hs", error );
    err_msg[err_msg_left - 1] = 0;
}

void cat_msg( 
	wchar_t*& msg,
	const wchar_t* msg2,
    size_t& len_left
    )
{
    if ( len_left > 0 ) {
        size_t msg2_len = wcslen( msg2 );
        if ( msg2_len > len_left ) 
            msg2_len = len_left;

        wcsncpy( msg, msg2, len_left );
        msg[len_left - 1] = 0;
        msg += msg2_len;
        len_left = len_left - msg2_len;
    }
 }

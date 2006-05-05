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
#include "errno.h"
#include "xlt_status.h"

int
mysql_xlt_status( 
	mysql_context_def	*context,
    int mysql_status
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

    return rdbi_status;
}

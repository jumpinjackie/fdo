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
 */

#include "stdafx.h"

#include "type.h"

/// <summary>Convert rdbi type into MYSQL type.</summary>
int rdbi_to_mysql_type (int datatype, int size)
{
    int ret;

    switch (datatype)
    {
        case RDBI_STRING:
            ret = MYSQL_TYPE_STRING;
            break;
        case RDBI_CHAR:
            ret = MYSQL_TYPE_VAR_STRING;
            break;
        case RDBI_SHORT:
            ret = MYSQL_TYPE_SHORT;
            break;
        case RDBI_FIXED_CHAR:
            ret = MYSQL_TYPE_VAR_STRING;
            break;
        case RDBI_INT:
            ret = MYSQL_TYPE_LONG;
            break;
        case RDBI_FLOAT:
            ret = MYSQL_TYPE_FLOAT;
            break;
        case RDBI_BLOB:
            ret = MYSQL_TYPE_BLOB;
            break;
        case RDBI_RAW:
            ret = -1;
            break;
        case RDBI_DOUBLE:
            ret = MYSQL_TYPE_DOUBLE;
            break;
        case RDBI_ROWID:
            ret = -1;
            break;
        case RDBI_DATE:
            ret = MYSQL_TYPE_VAR_STRING;
            break;
        case RDBI_LONG:
            ret = MYSQL_TYPE_LONG;
            break;
        case RDBI_GEOMETRY:
            ret = MYSQL_TYPE_GEOMETRY;
            break;
        case RDBI_BLOB_REF:
            ret = -1;
            break;
        case RDBI_LONGLONG:
            ret =  MYSQL_TYPE_LONGLONG;
            break;
        default:
            ret = -1;
    }

    return (ret);
}

/// <summary>Convert MYSQL type into rdbi type.</summary>
int mysql_to_rdbi_type (enum enum_field_types type, int size, int precision)
{
    int ret;

    switch (type)
    {
        case MYSQL_TYPE_TINY:
            ret = RDBI_SHORT;
            break;
        case MYSQL_TYPE_SHORT:
            ret = RDBI_DOUBLE; /* this should be fixed */
            break;
        case MYSQL_TYPE_LONG:
            ret = RDBI_DOUBLE; /* this should be fixed */
            break;
        case MYSQL_TYPE_INT24:
            ret = RDBI_DOUBLE; /* this should be fixed */
            break;
        case MYSQL_TYPE_LONGLONG:
            ret = RDBI_LONGLONG;
            break;
        case MYSQL_TYPE_FLOAT:
            ret = RDBI_DOUBLE; /* this should be fixed */
            break;
        case MYSQL_TYPE_DOUBLE:
            ret = RDBI_DOUBLE;
            break;
        case MYSQL_TYPE_DECIMAL:
        case 246: // MYSQL_TYPE_NEWDECIMAL
            if (0 == precision)
            {
                size--; /* mysql always adds one (for the minus sign?) */
                if (4 >= size)
                    ret = RDBI_SHORT;
                else if (9 >= size)
                    ret = RDBI_LONG; /* should be RDBI_INT and let RDBI_LONG be a long long */
                else
                    ret = RDBI_DOUBLE;
            }
            else
                ret = RDBI_DOUBLE;
            break;
        case MYSQL_TYPE_TIME:
            ret = RDBI_DATE;
            break;
        case MYSQL_TYPE_DATE:
            ret = RDBI_DATE;
            break;
        case MYSQL_TYPE_DATETIME:
            ret = RDBI_DATE;
            break;
        case MYSQL_TYPE_TIMESTAMP:
            ret = RDBI_DATE;
            break;
        case MYSQL_TYPE_STRING:
            if (1 == size)
                ret = RDBI_CHAR;
            else
                ret = RDBI_FIXED_CHAR;
            break;
        case MYSQL_TYPE_VAR_STRING:
            ret = RDBI_STRING;
            break;
        case MYSQL_TYPE_TINY_BLOB:
            ret = -1;
            break;
        case MYSQL_TYPE_BLOB:
            ret = RDBI_BLOB;
            break;
        case MYSQL_TYPE_NULL:
            ret = RDBI_CHAR; /* this should be fixed */
            break;
		case MYSQL_TYPE_YEAR:
			ret = RDBI_INT;
			break;
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_NEWDATE:
        case MYSQL_TYPE_ENUM:
        case MYSQL_TYPE_SET:
            ret = -1;
            break;
        case MYSQL_TYPE_GEOMETRY:
            ret = RDBI_GEOMETRY;
            break;
        default:
            ret = -1;
            break;
    }

    return (ret);
}

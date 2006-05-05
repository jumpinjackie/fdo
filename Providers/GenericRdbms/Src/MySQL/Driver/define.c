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

#include "define.h"
#include "type.h"

#ifdef _WIN32
#define case_insensitive_compare _stricmp
#else
#define case_insensitive_compare strcasecmp
#endif

static int indexof_field (MYSQL_RES *prepare_meta_result, char *name)
{
    unsigned int num_fields;
    MYSQL_FIELD *fields;
    int index;
    unsigned int i;

    num_fields = mysql_num_fields (prepare_meta_result);
    fields = mysql_fetch_fields (prepare_meta_result);
    for (i = 0, index = -1; (i < num_fields) && (-1 == index); i++)
        if (0 == case_insensitive_compare (name, fields[i].name))
            index = i;

    return (index);
}

/// <summary>Return the bind or define size for a given type and length.</summary>
/// <remark>Length is ignored for numeric and temporal data types because
/// the length of the data value is determined by the buffer_type value.</remark>
/// <pre>
/// buffer_type            SQL Type         C Type  
/// MYSQL_TYPE_TINY        TINYINT          char  
/// MYSQL_TYPE_SHORT       SMALLINT         short int  
/// MYSQL_TYPE_LONG        INT              int  
/// MYSQL_TYPE_LONGLONG    BIGINT           long long int  
/// MYSQL_TYPE_FLOAT       FLOAT            float  
/// MYSQL_TYPE_DOUBLE      DOUBLE           double  
/// MYSQL_TYPE_TIME        TIME             MYSQL_TIME  
/// MYSQL_TYPE_DATE        DATE             MYSQL_TIME  
/// MYSQL_TYPE_DATETIME    DATETIME         MYSQL_TIME  
/// MYSQL_TYPE_TIMESTAMP   TIMESTAMP        MYSQL_TIME  
/// MYSQL_TYPE_STRING      CHAR  
/// MYSQL_TYPE_VAR_STRING  VARCHAR  
/// MYSQL_TYPE_TINY_BLOB   TINYBLOB/TINYTEXT  
/// MYSQL_TYPE_BLOB        BLOB/TEXT  
/// MYSQL_TYPE_MEDIUM_BLOB MEDIUMBLOB/MEDIUMTEXT  
/// MYSQL_TYPE_LONG_BLOB   LONGBLOB/LONGTEXT  
/// </pre>
static int field_size (enum enum_field_types type, int length)
{
    int ret;

    switch (type)
    {
        case MYSQL_TYPE_TINY:
            ret = sizeof (char);
            break;
        case MYSQL_TYPE_SHORT:
            ret = sizeof (short int);
            break;
        case MYSQL_TYPE_LONG:
        case MYSQL_TYPE_INT24:
            ret = sizeof (int);
            break;
        case MYSQL_TYPE_LONGLONG:
            ret = sizeof (longlong);
            break;
        case MYSQL_TYPE_FLOAT:
            ret = sizeof (float);
            break;
        case MYSQL_TYPE_DOUBLE:
        case MYSQL_TYPE_DECIMAL:
        case 246: // MYSQL_TYPE_NEWDECIMAL
            ret = sizeof (double);
            break;
        case MYSQL_TYPE_TIME:
            ret = sizeof (MYSQL_TIME);
            break;
        case MYSQL_TYPE_DATE:
            ret = sizeof (MYSQL_TIME);
            break;
        case MYSQL_TYPE_DATETIME:
            ret = sizeof (MYSQL_TIME);
            break;
        case MYSQL_TYPE_TIMESTAMP:
            ret = sizeof (MYSQL_TIME);
            break;
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
            ret = length;
            break;
        case MYSQL_TYPE_NULL:
            ret = 0;
            break;

        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_NEWDATE:
        case MYSQL_TYPE_ENUM:
        case MYSQL_TYPE_SET:
            ret = -1;
            break;
        case MYSQL_TYPE_GEOMETRY:
            ret = sizeof (void*);
            break;
        default:
            ret = -1;
            break;
    }

    return (ret);
}

/// <summary>Make the defines array for a statement.</summary>
/// <remark>All pointers point to the same allocation, so only
///  the main array (i.e. &(binds[0]) needs to be freed.</remark>
static MYSQL_BIND *make_defines (MYSQL_RES *prepare_meta_result)
{
    unsigned int num_fields;
    MYSQL_FIELD *fields;
    int buffer_size;
    unsigned int i;
    enum enum_field_types type;
    unsigned long length;
    int size;
    int allocation;
    MYSQL_BIND *p;
    char* buffer;
    unsigned long* lengths;
    my_bool* nulls;
    MYSQL_BIND *ret;

    ret = (MYSQL_BIND*)NULL;

    /* get total columns in the query */
    num_fields = mysql_num_fields (prepare_meta_result);
    fields = mysql_fetch_fields (prepare_meta_result);
    buffer_size = 0;
    for (i = 0; i < num_fields; i++)
    {
        type = fields[i].type;
        length = fields[i].length;
        size = field_size (type, length);
        if (0 > size)
            return (ret);
        buffer_size += size;
    }
    /* reserve space for bind array */
    allocation = sizeof (MYSQL_BIND) * num_fields;
    /* reserve space for a buffer for each field */
    allocation += buffer_size;
    /* reserve space for a length indicator for each field */
    allocation += num_fields * sizeof (unsigned long);
    /* reserve space for a null indicator for each field */
    allocation += num_fields * sizeof (my_bool);
    ret = (MYSQL_BIND*)malloc (allocation);
    /* initialize the fields */
    memset (ret, 0, allocation);
    buffer = (char*)&(ret[num_fields]);
    lengths = (unsigned long*)(buffer + buffer_size);
    nulls = (my_bool*)(lengths + num_fields);
    for (i = 0, p = ret; i< num_fields; i++,p++)
    {
        type = fields[i].type;
        length = fields[i].length;
        size = field_size (type, length);
        p->buffer_type = fields[i].type; /* The type of the buffer. The allowable buffer_type values are listed later in this section. For input, buffer_type indicates what type of value you are binding to a statement parameter. For output, it indicates what type of value you expect to receive in a result buffer. */
        p->buffer = buffer; /* For input, this is a pointer to the buffer in which a statement parameter's data value is stored. For output, it is a pointer to the buffer in which to return a result set column value. For numeric column types, buffer should point to a variable of the proper C type. (If you are associating the variable with a column that has the UNSIGNED attribute, the variable should be an unsigned C type. Indicate whether the variable is signed or unsigned by using the is_unsigned member, described later in this list.) For date and time column types, buffer should point to a MYSQL_TIME structure. For character and binary string column types, buffer should point to a character buffer. */
        buffer += size;
        p->buffer_length = size; /* The actual size of *buffer in bytes. This indicates the maximum amount of data that can be stored in the buffer. For character and binary C data, the buffer_length value specifies the length of *buffer when used with mysql_stmt_bind_param(), or the maximum number of data bytes that can be fetched into the buffer when used with mysql_stmt_bind_result(). */
        p->length = lengths; /* A pointer to an unsigned long variable that indicates the actual number of bytes of data stored in *buffer. length is used for character or binary C data. For input parameter data binding, length points to an unsigned long variable that indicates the length of the parameter value stored in *buffer; this is used by mysql_stmt_execute(). For output value binding, mysql_stmt_fetch() places the length of the column value that is returned into the variable that length points to. */
        lengths++;
        p->is_null = nulls; /* This member points to a my_bool variable that is true if a value is NULL, false if it is not NULL. For input, set *is_null to true to indicate that you are passing a NULL value as a statement parameter. For output, this value is set to true after you fetch a row if the result set column value returned from the statement is NULL. */
        nulls++;
        p->is_unsigned = 0; /* This member is used for integer types. (These correspond to the MYSQL_TYPE_TINY, MYSQL_TYPE_SHORT, MYSQL_TYPE_LONG, and MYSQL_TYPE_LONGLONG type codes.) is_unsigned should be set to true for unsigned types and false for signed types. */
        // p->error = 0; /* For output, this member is used output to report data truncation errors. Truncation reporting must be enabled by calling mysql_options() with the MYSQL_REPORT_DATA_TRUNCATION option. When enabled, mysql_stmt_fetch() returns MYSQL_DATA_TRUNCATED and error is true in the MYSQL_BIND structures for parameters in which truncation occurred. Truncation indicates loss of sign or significant digits, or that a string was too long to fit in a column. The error member was added in MySQL 5.0.3. */
    }

    return (ret);
}

int mysql_define (
    mysql_context_def *context,
    char *cursor,
    char *name,
    int datatype,
    int size,
    char *address,
    short *null_ind)
{
    MYSQL *mysql;
    mysql_cursor_def *curs;
    MYSQL_STMT *statement;
    MYSQL_RES *prepare_meta_result;
    int index;
    int type;
    int ret;

    if (-1 == context->mysql_current_connect)
        ret = RDBI_NOT_CONNECTED;
    else
    {
        mysql = context->mysql_connections[context->mysql_current_connect];
        if ((MYSQL*)NULL == mysql)
            ret = RDBI_NOT_CONNECTED;
        else
        {
            curs = (mysql_cursor_def*)cursor;
            if ((mysql_cursor_def*)NULL == curs)
                ret = RDBI_GENERIC_ERROR;
            else
            {
                statement = curs->statement;
                if ((MYSQL_STMT*)NULL != statement)
                {   /* fetch result set meta information and check prepare was done */
                    prepare_meta_result = mysql_stmt_result_metadata (statement);
                    if ((MYSQL_RES*)NULL == prepare_meta_result)
                        ret = RDBI_GENERIC_ERROR;
                    else
                    {
                        /* allocate the defines array if it hasn't been already allocated */
                        if ((MYSQL_BIND*)NULL == curs->defines)
                            curs->defines = make_defines (prepare_meta_result);
                        if ((MYSQL_BIND*)NULL == curs->defines)
                            ret = RDBI_GENERIC_ERROR; /* need an error for illegal type */
                        else
                        {
                            curs->define_count = mysql_num_fields (prepare_meta_result);
                            index = atoi (name); /* numeric position */
                            if (0 == index)
                                index = indexof_field (prepare_meta_result, name);
                            if (0 >= index)
                                ret = RDBI_GENERIC_ERROR; /* need an error for unknown name */
                            else
                            {
                                index--; /* make it zero based */
                                type = rdbi_to_mysql_type (datatype, size);
                                if (0 > type)
                                    ret = RDBI_GENERIC_ERROR; /* need an error for unknown name */
                                else
                                {
                                    curs->defines[index].buffer_type = (enum enum_field_types)type;
                                    curs->defines[index].buffer_length = size;
                                    curs->defines[index].buffer = address;
                                    if ((short*)NULL != null_ind)
                                    {
                                        curs->defines[index].is_null = (my_bool*)null_ind;
                                    }
                                    ret = RDBI_SUCCESS;
                                }
                            }
                        }
                        mysql_free_result(prepare_meta_result);
                    }
                }
            }
        }
    }

    return (ret);
}

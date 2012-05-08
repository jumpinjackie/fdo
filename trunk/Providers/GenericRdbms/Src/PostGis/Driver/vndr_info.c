/*
 * Copyright (C) 2006 Refractions Research, Inc. 
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

#include "vndr_info.h"
#include "vndr_name.h"
#include <assert.h>

int postgis_vndr_info (postgis_context_def *context, rdbi_vndr_info_def *vndr_info)
{
    PGconn *postgis;
    int version_number;
    int ret;

    assert(NULL != context);
    assert(NULL != vndr_info);

	/* no array fetching */
	vndr_info->maxFetchSize = 1;

    if (-1 == context->postgis_current_connect)
    {
        ret = RDBI_NOT_CONNECTED;
    }
    else
    {
        postgis = context->postgis_connections[context->postgis_current_connect];
        if (NULL == postgis)
        {
            ret = RDBI_NOT_CONNECTED;
        }
        else
        {
            /*
             * Fetch PostgreSQL server version of current connection
             * For example:
             * 7.4.2 is returned as 70402
             * 8.2.0 is returned as 80200
             */
            
            version_number = PQserverVersion(postgis);
            if (0 == version_number)
            {
                /* Zero is returned if the connection is bad. */
                ret = RDBI_NOT_CONNECTED;
            }
            else
            {
                vndr_info->dbversion = version_number;

                /*
                 * Source: 8.1. Numeric Types
                 * http://www.postgresql.org/docs/8.1/interactive/datatype.html
                 *
                 * The type NUMERIC can store numbers with up to 1000 digits of
                 * precision (user-specified precision, with range of no limit) and 
                 * perform calculations exactly.
                 * If the scale of a value to be stored is greater than the declared
                 * scale of the column, the system will round the value to the specified
                 * number of fractional digits. Then, if the number of digits to the left
                 * of the decimal point exceeds the declared precision minus the declared
                 * scale, an error is raised.
                 * Both the maximum precision and the maximum scale of a numeric column
                 * can be configured. The types decimal and numeric are equivalent.
                 * Both types are part of the SQL standard.
                 *
                 * A double-precision floating-point number is accurate to approximately
                 * 15 decimal places.
                 */

                /*
                 * test=# create table boffo (test decimal(1045,2));
                 * ERROR:  DECIMAL precision 1045 must be between 1 and 1000
                 *
                 * test=# create table boffo (test decimal(1001));
                 * ERROR:  DECIMAL precision 1001 must be between 1 and 1000
                 *
                 * test=# create table boffo (test decimal(1000));
                 * CREATE TABLE
                 * test=# \d boffo
                 *      Table "public.boffo"
                 * Column |      Type       | Modifiers
                 * --------+-----------------+-----------
                 * test   | numeric(1000,0) |
                 *
                 * test=# create table boffo (test decimal(0));
                 * ERROR:  DECIMAL precision 0 must be between 1 and 1000
                 *
                 * Documentation: The precision must be positive, the scale zero or positive.
                 *
                 * test=# create table boffo (test decimal(1));
                 * CREATE TABLE
                 * test=# \d boffo
                 *      Table "public.boffo"
                 * Column |      Type       | Modifiers
                 * --------+-----------------+-----------
                 * test   | numeric(1,0) |
                 *
                 * test=# create table boffo (test decimal(20,-1));
                 * ERROR:  syntax error at or near "-" at character 37
                 * LINE 1: create table boffo (test decimal(20,-1));
                 *
                 * test=# create table boffo (test decimal(20,200));
                 * ERROR:  DECIMAL scale 200 must be between 0 and precision 20
                 *
                 */

                /*
                 * TODO - mloskot - Verify these values for PostgreSQL.
                 */

                vndr_info->minPrecision = 1;
                vndr_info->maxPrecision = 1000;
                vndr_info->minScale = 0;
                vndr_info->maxScale = 20;


                /*
                 * Source: 8.3. Character Types
                 * http://www.postgresql.org/docs/8.1/interactive/datatype-character.html
                 *
                 * varchar(M) - A variable-length with limit string, where M is a positive
                 * integer and type can store strings up to M characters in length. 
                 * In any case, the longest possible character string that can be stored
                 * is about 1 GB. The maximum value that will be allowed for M in the data
                 * type declaration is less than that.
                 */
   
                /*
                 * TODO - mloskot - Verify following values for PostgreSQL.
                 * PostgreSQL is able to store much longer strings. How about TEXT type?
                 */
                vndr_info->minStrLen = 0;
                vndr_info->maxStrLen = 32766; /* 65,532 / 2 for wide char, compatible with MySQL */

                /* Name should always be within 30 char, just truncate if too long. */
                memset (vndr_info->name, 0, sizeof (vndr_info->name));
                strncpy (vndr_info->name, postgis_vndr_name (context), RDBI_TABLE_NAME_SIZE);

                /*
                 * No array fetching.
                 * TODO - mloskot: Change when batch fetching is used.
                 */
                vndr_info->maxFetchSize = 1;

                ret = RDBI_SUCCESS;

            }
        }
    }

    return (ret);
}

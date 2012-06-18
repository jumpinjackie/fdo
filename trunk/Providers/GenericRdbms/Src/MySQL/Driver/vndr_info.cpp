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

#include "vndr_info.h"
#include "vndr_name.h"

int mysql_vndr_info (mysql_context_def *context, rdbi_vndr_info_def *vndr_info)
{
    MYSQL *mysql;
    const char* version;
    char* major_version;
    char* p;
    char* minor_version;
    char* sub_version;

    int ret;

	/* no array fetching */
	vndr_info->maxFetchSize = 1;

    if (-1 == context->mysql_current_connect)
        ret = RDBI_NOT_CONNECTED;
    else
    {
        mysql = context->mysql_connections[context->mysql_current_connect];
        if ((MYSQL*)NULL == mysql)
            ret = RDBI_NOT_CONNECTED;
        else
        {
            // vndr_info->dbversion = mysql_get_server_version (mysql);
            version = mysql_get_server_info (mysql);
            major_version = (char*)alloca (strlen (version) + 1);
            strcpy (major_version, version);
            p = major_version;
            p = strchr (p, '.');
            *p = '\0';
            p++;
            minor_version = p;
            p = strchr (p, '.');
            *p = '\0';
            p++;
            sub_version = p;
            p = strchr (p, '-');
            if ( p ) 
                *p = '\0';
            vndr_info->dbversion = atoi (major_version) * 10000 + atoi (minor_version) *100 + atoi (sub_version);

            /* The maximum range of DECIMAL and NUMERIC values is the same as for DOUBLE,
            * but the actual range for a given DECIMAL or NUMERIC column can be
            * constrained by the precision or scale for a given column. When such a
            * column is assigned a value with more digits following the decimal point
            * than are allowed by the specified scale, the value is converted to that
            * scale. (The precise behavior is operating system-specific, but
            * generally the effect is truncation to the allowable number of digits.)
            * A double-precision floating-point number is accurate to approximately
            * 15 decimal places.
            */

            /*
            * mysql> create table boffo (test decimal(1045,2));
            * ERROR 1074 (42000): Column length too big for column 'test' (max = 255); use BLOB or TEXT instead
            *
            * mysql> create table boffo (test decimal(255));
            * ERROR 1074 (42000): Column length too big for column 'test' (max = 255); use BLOB or TEXT instead
            *
            * mysql> create table boffo (test decimal(254));
            * Query OK, 0 rows affected (0.06 sec)
            * mysql> desc boffo;
            * +-------+----------------+------+-----+---------+-------+
            * | Field | Type           | Null | Key | Default | Extra |
            * +-------+----------------+------+-----+---------+-------+
            * | test  | decimal(254,0) | YES  |     | NULL    |       |
            * +-------+----------------+------+-----+---------+-------+
            * 1 row in set (0.02 sec)
            *
            * mysql> create table boffo (test decimal(0));
            * Query OK, 0 rows affected (0.05 sec)
            * mysql> desc boffo;
            * +-------+---------------+------+-----+---------+-------+
            * | Field | Type          | Null | Key | Default | Extra |
            * +-------+---------------+------+-----+---------+-------+
            * | test  | decimal(10,0) | YES  |     | NULL    |       |
            * +-------+---------------+------+-----+---------+-------+
            * 1 row in set (0.00 sec)
            *
            * mysql> create table boffo (test decimal(1));
            * Query OK, 0 rows affected (0.06 sec)
            * mysql> desc boffo;
            * +-------+--------------+------+-----+---------+-------+
            * | Field | Type         | Null | Key | Default | Extra |
            * +-------+--------------+------+-----+---------+-------+
            * | test  | decimal(1,0) | YES  |     | NULL    |       |
            * +-------+--------------+------+-----+---------+-------+
            * 1 row in set (0.00 sec)
            *
            * mysql> create table boffo (test decimal(20,-1));
            * ERROR 1064 (42000): You have an error in your SQL syntax; check the manual that
            * corresponds to your MySQL server version for the right syntax to use near '-1))'
            * at line 1
            *
            * mysql> create table boffo (test decimal(20,200));
            * Query OK, 0 rows affected (0.06 sec)
            * mysql> desc boffo;
            * +-------+----------------+------+-----+---------+-------+
            * | Field | Type           | Null | Key | Default | Extra |
            * +-------+----------------+------+-----+---------+-------+
            * | test  | decimal(31,30) | YES  |     | NULL    |       |
            * +-------+----------------+------+-----+---------+-------+
            * 1 row in set (0.00 sec)
            *
            * mysql> create table boffo (test decimal(36,255));
            * Query OK, 0 rows affected (0.06 sec)
            * mysql> desc boffo;
            * +-------+----------------+------+-----+---------+-------+
            * | Field | Type           | Null | Key | Default | Extra |
            * +-------+----------------+------+-----+---------+-------+
            * | test  | decimal(36,30) | YES  |     | NULL    |       |
            * +-------+----------------+------+-----+---------+-------+
            * 1 row in set (0.00 sec)
            */
            vndr_info->minPrecision = 1;
            vndr_info->maxPrecision = 254;
            vndr_info->minScale = 0;
            vndr_info->maxScale = 30;

            /* VARCHAR(M) A variable-length string. M represents the maximum column
            * length. The range of M is 1 to 255 before MySQL 4.0.2, 0 to 255 as of
            * MySQL 4.0.2, and 0 to 65,535 as of MySQL 5.0.3. (The maximum actual
            * length of a VARCHAR in MySQL 5.0 is determined by the maximum row size
            * and the character set you use. The maximum effective length is
            * 65,532 bytes.)
            * From MySQL 4.1.0 to 5.0.2, a VARCHAR column with a length specification
            * greater than 255 is converted to the smallest TEXT type that can hold
            * values of the given length. For example, VARCHAR(500) is converted to
            * TEXT, and VARCHAR(200000) is converted to MEDIUMTEXT.
            * This is a compatibility feature. However, this conversion affects
            * trailing-space removal.
            */
            vndr_info->minStrLen = 0;
            vndr_info->maxStrLen = 32766; /* 65,532 / 2 for wide char, compatible with 5.0.3++ */

            /* name should always be within 30 char, just truncate if too long */
            memset (vndr_info->name, 0, sizeof (vndr_info->name));
            strncpy (vndr_info->name, mysql_vndr_name (context), RDBI_TABLE_NAME_SIZE);

			/* no array fetching */
			vndr_info->maxFetchSize = 1;

            ret = RDBI_SUCCESS;
        }
    }

    return (ret);
}

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

#ifndef LOCAL_H
#define LOCAL_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

/* this limit is temporary and should be removed when BLOB, TEXT, LONGTEXT etc.
   are truly supported.
*/
#define BLOB_MAX_LENGTH 65535

typedef struct _mysql_context_def
{
    long  mysql_connect_count; /* number of open connections */
    long  mysql_current_connect; /* index into mysql_connections */
	long  mysql_current_connect2; /* 2nd connection for DDLs */
    MYSQL *mysql_connections[RDBI_MAX_CONNECTS]; /* connection objects */
    wchar_t  mysql_last_err_msg[RDBI_MSG_SIZE];
} mysql_context_def;

typedef struct _mysql_redefine_def
{
    void *original;
    void *substitution;
    void *geometry; /* geometry to be deleted, since the client won't */
    int  orig_type;
} mysql_define_def;

typedef struct _mysql_cursor_def
{
    MYSQL_STMT *statement;
    int define_count;
    MYSQL_BIND *defines;
    int bind_count;
    MYSQL_BIND *binds;
    mysql_define_def *redefines; /* array of size define_count, NULL entries for non-geometries */
    mysql_define_def *rebinds; /* array of size bind_count, NULL entries for non-geometries */
} mysql_cursor_def;

#endif // LOCAL_H

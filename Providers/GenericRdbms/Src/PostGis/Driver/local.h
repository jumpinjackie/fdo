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

#ifndef POSTGIS_LOCAL_H
#define POSTGIS_LOCAL_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "type.h"

#define POSTGIS_STMT_NAME_SIZE  32
#define POSTGIS_FETCH_SIZE      512
#define POSTGIS_STMT_NOROWS     -1

/*
 * Structures used in RDBI layer for PostgreSQL.
 */

typedef struct tag_PGSQL_BIND
{
    Oid   buffer_type;
    int   buffer_length;    /**< Data format: text (put a zero in the array) or binary (put a one in the array). */
    int   buffer_format;    /**< Data length, ignored for null parameters and text-format parameters. */
    void* buffer;           /**< Pointer to buffer where bind or statement data value is stored. */
    void* geometry;         /**< Geometry to be deleted, since the client won't */
    char* is_null;
} PGSQL_BIND;

/**
 * PostGIS session context structure.
 */
typedef struct tag_postgis_context_def
{
    long    postgis_connect_count;                  /**< Number of open connections */
    long    postgis_current_connect;                /**< Index into postgis_connections */
	long    postgis_current_connect2;               /**< 2nd connection for DDLs */
    PGconn* postgis_connections[RDBI_MAX_CONNECTS]; /**< Connection objects */
    int     postgis_in_transaction[RDBI_MAX_CONNECTS];     /* 0(false), 1(true) */
    /*
     * TODO: This new wchar version requires changes in a few other RDBI functions.
    wchar_t postgis_last_err_msg[RDBI_MSG_SIZE];
     */

    char postgis_last_err_msg[RDBI_MSG_SIZE];       /**< Error message guffer. */
} postgis_context_def;

/**
 * Cursor to control statement operations.
 */
typedef struct tag_postgis_cursor_def
{
    /**< Name of prepared statement, may be empty or connection index as text. */
    char        stmt_name[POSTGIS_STMT_NAME_SIZE];
    char        cursor_name[POSTGIS_STMT_NAME_SIZE];
    int         isSelect; /* 1=true, 0=false */
    char*       sql;
    PGresult*   stmt_result;       /**< Statement execution result. */
    int         current_row;
    int         define_count;
    PGSQL_BIND* defines;
    int         bind_count;
    PGSQL_BIND* binds;
	int	  *srids; /* array of SRIDs, 0 if not explicitly set and for non-geometries */
    Oid         geometry_oid;
} postgis_cursor_def;

#endif /* POSTGIS_LOCAL_H */

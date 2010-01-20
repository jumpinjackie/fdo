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

#ifndef POSTGIS_TYPE_H
#define POSTGIS_TYPE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <libpq-fe.h>

/**
 * This enum defines constants used with variable-length types.
 */
enum enum_varlena_details
{
    PGSQL_VAR_NOINFO = -1, /**< No information is available */
    PGSQL_VAR_HDRSZ  = 4,   /**< Represented as sizeof(int) */

    /* Max size of variable unlimited length text is
     * arbitrarily assumed as 64 KB
     */
    PGSQL_VARCHAR_MAX_SIZE = 65536  
};

/*
 * Definition of characteristics of boolean type.
 */
enum enum_boolean_details
{
    PGSQL_BOOLEAN_SIZE = 1,
    PGSQL_BOOLEAN_RDBI_SIZE = 2
};

/**
 * This enum maps PostgreSQL OIDs to types enumeration.
 * Enumerators are assigned with values from PostgreSQL 'pg_type' table.
 */
enum enum_field_types
{
    PGSQL_TYPE_UNKNOWN = 705, // UNKNOWN

    PGSQL_TYPE_BOOL = 16, // bool

    PGSQL_TYPE_INT2 = 21, // int2, smallint
    PGSQL_TYPE_INT4 = 23, // int4, integer
    PGSQL_TYPE_INT8 = 20, // int8, bigint

    PGSQL_TYPE_FLOAT    = 700, // float4 
    PGSQL_TYPE_DOUBLE   = 701, // float8
    PGSQL_TYPE_DECIMAL  = 1700, // numeric

    PGSQL_TYPE_ABSTIME  = 702, // abstime
    PGSQL_TYPE_RELTIME  = 703, // reltime
    PGSQL_TYPE_DATE     = 1082, // date
    PGSQL_TYPE_TIME     = 1083, // time
    PGSQL_TYPE_TIMETZ      = 1266, // timetz
    PGSQL_TYPE_TIMESTAMP   = 1114, // timestamp
    PGSQL_TYPE_TIMESTAMPTZ = 1184, // timestamptz

    PGSQL_TYPE_CHAR        = 18, // char
    PGSQL_TYPE_BPCHAR      = 1042, // bpchar, the same as char
    PGSQL_TYPE_VARCHAR     = 1043, // varchar
    PGSQL_TYPE_TEXT        = 25, // text 
    PGSQL_TYPE_NAME        = 19, // name, length 64 characters

    PGSQL_TYPE_BIT         = 1560,
    PGSQL_TYPE_BLOB        = 17, // bytea

    /*
     * Oid for PostGIS geometry is queried in run-time.
     * See postgis_get_geometry_oid() function below.
     *
     * PGSQL_TYPE_GEOMETRY = ???
     */
};

/**
 * Translates RDBI data type to PostGIS data type.
 * @param conn - connection to PostgreSQL database
 * @param type [in] - constant value of RDBI type.
 * @param size [in] - size in bytes of given type.
 * @param typemod [in] - typemod, value returned from PQfmod() function. NOT USED.
 * @return Oid representing PostGIS data type or zero on failure or
 * if type is unrecognized.
 */
Oid rdbi_to_postgis_type(PGconn* conn, int type, int size, int typemod);

/**
 * Translates PostGIS data type to RDBI data type.
 * @param conn - connection to PostgreSQL database
 * @param typeoid [in] - PostgreSQL type Oid, most types defined in
 *  enum_field_types enumeration.
 * @param size [in] - size in bytes of given type, usually returned by PQfsize().
 * @param typemod [in] - typemod, value returned from PQfmod() function.
 * @return Code representing RDBI data type or -1 on failure or if type is unrecognized.
 */
int postgis_to_rdbi_type(PGconn* conn, Oid typeoid, int size, int typemod);

/**
 * Query for PostGIS geometry Oid using given connection.
 * @param conn - connection to PostgreSQL database.
 * @return Oid identifier for PostGIS geometry type or zero on failure.
 */
Oid postgis_get_geometry_oid(PGconn* conn);

/**
 * Retrieves default maximum length of NAME type.
 * NAME type is used internally by PostgreSQL, in pg_catalog tables and views.
 * Default length is set in compile time using NAMEDATALEN constant.
 *
 * @param conn - connection to PostgreSQL database.
 * @return Value of maximum length for NAME type.
 */
int postgis_get_name_length(PGconn* conn);

/**
 * Retrieves precision component for type of given column.
 * The precision is decoded form value returned by PQfmod() function.
 *
 * Value of typemod of NUMERIC type is calculated as:
 * ((precision << 16) | scale) + VARHDRSZ.
 *
 * @param pqres [in] - pointer to PGresult; should be a valid result of query.
 * @param column_number - numerical identifier of column from given result,
 * starts with 0.
 * @return Value of precision property for given column if of NUMERIC type.
 * Value of PGSQL_VAR_NOINFO indicates "No information available".
 */
int postgis_get_numeric_precision(const PGresult* pqres, int column_number);

/**
 * Retrieves scale component for type of given column.
 * The scale is decoded form value returned by PQfmod() function.
 *
 * Value of typemod of NUMERIC type is calculated as:
 * ((precision << 16) | scale) + VARHDRSZ.
 *
 * @param pqres [in] - pointer to PGresult; should be a valid result of query.
 * @param column_number - numerical identifier of column from given result,
 * starts with 0.
 * @return Value of scale property for given column if of NUMERIC type.
 * Value of PGSQL_VAR_NOINFO indicates "No information available".
 */
int postgis_get_numeric_scale(const PGresult* pqres, int column_number);

/**
 * Retrieves length of CHARACTER and BPCHAR type - fixed-length and blank padded.
 * The length is encoded in value returned by PQfmod() function.
 *
 * @param pqres [in] - pointer to PGresult; should be a valid result of query.
 * @param column_number - numerical identifier of column from given result,
 * starts with 0.
 * @return Value of length for given column if of CHARACTER type.
 * Value of PGSQL_VAR_NOINFO indicates "No information available".
 */
int postgis_get_char_length(const PGresult* pqres, int column_number);

/**
 * Retrieves maximum length of VARCHAR types - variable-length with limit.
 * The length is encoded in value returned by PQfmod() function.
 * For varchar, typmod returned by PQfmod() is the maximum length + VARHDRSZ.
 *
 * @param pqres [in] - pointer to PGresult; should be a valid result of query.
 * @param column_number - numerical identifier of column from given result,
 * starts with 0.
 * @return Value of length for given column if of VARCHAR type.
 * Value of PGSQL_VAR_NOINFO indicates "No information available".
 */
int postgis_get_varchar_length(const PGresult* pqres, int column_number);


#endif /* POSTGIS_TYPE_H */
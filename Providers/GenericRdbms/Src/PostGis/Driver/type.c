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

#include "type.h"
#include <assert.h>
#include <limits.h> /* LONG_MIN, LONG_MAX */
#include <stdlib.h> /* strtol */


Oid rdbi_to_postgis_type(PGconn* conn, int type, int size, int typemod)
{
    Oid ret = 0;
    
    assert(NULL != conn);

    switch (type)
    {
    case RDBI_BOOLEAN:
        ret = PGSQL_TYPE_BOOL;
        break;
    case RDBI_CHAR:
        ret = PGSQL_TYPE_CHAR;
        break;
    case RDBI_FIXED_CHAR:
        ret = PGSQL_TYPE_VARCHAR;
        break;
    case RDBI_STRING:
        ret = PGSQL_TYPE_TEXT;
        break;
    case RDBI_SHORT:
        ret = PGSQL_TYPE_INT2;
        break;
    case RDBI_INT:
        ret = PGSQL_TYPE_INT4;
        break;
    case RDBI_LONG:
        ret = PGSQL_TYPE_INT4; /* TODO: mloskot - Check this mapping. */
        break;
    case RDBI_LONGLONG:
        ret =  PGSQL_TYPE_INT8;
        break;
    case RDBI_FLOAT:
        ret = PGSQL_TYPE_FLOAT;
        break;
    case RDBI_DOUBLE:
        ret = PGSQL_TYPE_DOUBLE;
        break;
    case RDBI_DATE:
        ret = PGSQL_TYPE_VARCHAR; /* TODO: mloskot - Check this conversion. */
        break;
    case RDBI_BLOB:
        ret = PGSQL_TYPE_BLOB;
        break;
    case RDBI_BLOB_REF:
        ret = 0; /* TODO: check it */
        break;
    case RDBI_GEOMETRY:
        /*
        * Fetch PostGIS geometry Oid from database.
        */
        ret = postgis_get_geometry_oid(conn);

        /* TODO: remove it for release */
        assert(0 != ret);

        break;
    default:
        /*
        * Type not recognized.
        */

        // TODO: mloskot - Should we use OID of UNKNOWN type - 705 - or return Zero?
        ret = PGSQL_TYPE_UNKNOWN;

        ret = 0;
    }

    return (ret);
}

int postgis_to_rdbi_type(PGconn* conn, Oid typeoid, int size, int typemod)
{
    int ret = -1;
    int scale = 0;      /* Used for NUMERIC */
    int precision = 0;
    int length = 0;     /* Used for CHAR and VARCHAR */

    switch (typeoid)
    {
        case PGSQL_TYPE_BOOL:
            ret = RDBI_BOOLEAN;
            break;
        case PGSQL_TYPE_INT2:
            ret = RDBI_SHORT;
            break;
        case PGSQL_TYPE_INT4:
            ret = RDBI_INT;
            break;
        case PGSQL_TYPE_INT8:
            ret = RDBI_LONGLONG;
            break;
        case PGSQL_TYPE_FLOAT:
            ret = RDBI_FLOAT;
            break;
        case PGSQL_TYPE_DOUBLE:
            ret = RDBI_DOUBLE;
            break;
        case PGSQL_TYPE_DECIMAL:
            ret = RDBI_DOUBLE;  /* Default type for NUMERIC aka DECIMAL */
            scale = ((typemod - PGSQL_VAR_HDRSZ) & 0xffff);
            if (0 == scale)
            {
                if (2 >= size)
                    ret = RDBI_SHORT;
                if (4 >= size)
                    ret = RDBI_INT;
                else if (9 >= size)
                    ret = RDBI_LONGLONG;
                else
                    ret = RDBI_DOUBLE;
            }
            break;

        case PGSQL_TYPE_TIME:
        case PGSQL_TYPE_TIMETZ:
        case PGSQL_TYPE_DATE:
        case PGSQL_TYPE_TIMESTAMP:
        case PGSQL_TYPE_TIMESTAMPTZ:
        case PGSQL_TYPE_ABSTIME:
        case PGSQL_TYPE_RELTIME:
            ret = RDBI_DATE;
            break;

        case PGSQL_TYPE_CHAR:
        case PGSQL_TYPE_BPCHAR:
            length = (typemod - PGSQL_VAR_HDRSZ);
            assert(length > 0);

            if (1 == length)
                ret = RDBI_CHAR;
            else
                ret = RDBI_FIXED_CHAR; /* character WITH a length specifier. */
            break;

        case PGSQL_TYPE_VARCHAR:
            length = (typemod - PGSQL_VAR_HDRSZ);
            if (length > 0)
                // TODO: mloskot - Find a better type for VARCHAR(N)
                ret = RDBI_STRING; /* character varying WITH a length specifier. */
            else
                ret = RDBI_STRING; /* character varying WITHOUT a length specifier. */
            break;

        case PGSQL_TYPE_NAME:       /* default maximum length is 64 characters. */
            ret = RDBI_FIXED_CHAR; 
            break;
        case PGSQL_TYPE_TEXT:
            ret = RDBI_STRING;
            break;
        case PGSQL_TYPE_BLOB:
            ret = RDBI_BLOB;
            break;

        case PGSQL_TYPE_BIT:
            ret = RDBI_CHAR;
            break;

        case PGSQL_TYPE_UNKNOWN:
            // TODO: mloskot - Should we cast to STRING?
            ret = -1;
            break;
        default:
            /* Type not recognized or identifies PostGIS geometry, see below */
            ret = -1;
            break;
    }

    /* Check if type represents PostGIS geometry */
    if (-1 == ret)
    {
        if (postgis_get_geometry_oid(conn) == typeoid)
            ret = RDBI_GEOMETRY;
        else
            ret = -1; /* Type not recognized */
    }

    return (ret);
}

Oid postgis_get_geometry_oid(PGconn* conn)
{
    PGresult* pgresult = NULL; 
    Oid oid = 0;
    const char* query = "SELECT oid FROM pg_type WHERE typname = 'geometry'";

    assert(NULL != conn);

    pgresult = PQexec(conn, query);
    if (NULL != pgresult)
    {
        if (PGRES_TUPLES_OK == PQresultStatus(pgresult) && PQntuples(pgresult) > 0)
        {
            oid = strtol(PQgetvalue(pgresult, 0, 0), NULL, 10);

            if (LONG_MIN == oid || LONG_MAX == oid || 0 == oid)
            {
                /* Report if value overflow occured or no conversion was performed */

                /*
                 * TODO - print error
                 */
                assert(!"strtol value overflow");
            }
        }
    }

    PQclear(pgresult);

    /*
     * 0 - failure
     */
    return oid;
}

int postgis_get_name_length(PGconn* conn)
{
    PGresult* pgresult = NULL; 
    int length = 0;
    const char* query = "SELECT typlen FROM pg_type WHERE typname = 'name'";

    assert(NULL != conn);

    pgresult = PQexec(conn, query);
    if (NULL != pgresult)
    {
        if (PGRES_TUPLES_OK == PQresultStatus(pgresult) && PQntuples(pgresult) > 0)
        {
            length = strtol(PQgetvalue(pgresult, 0, 0), NULL, 10);

            if (LONG_MIN == length || LONG_MAX == length || 0 == length)
            {
                /* Report if value overflow occured or no conversion was performed */
                length = -1;

                /*
                 * TODO - print error
                 */
                assert(!"strtol value overflow");
            }
        }
    }

    PQclear(pgresult);

    /*
     * 0 - failure
     */
    return length;
}

int postgis_get_numeric_precision(const PGresult* pqres, int column_number)
{
    int precision = PGSQL_VAR_NOINFO; /* No information available */
    int fmod = 0;

    if (NULL != pqres)
    {
        /* Check if given column is of type of NUMERIC */
        if (PGSQL_TYPE_DECIMAL == PQftype(pqres, column_number))
        {
            fmod = PQfmod(pqres, column_number);
            precision = ((fmod - PGSQL_VAR_HDRSZ) >> 16);
        }
    }

    return precision;
}

int postgis_get_numeric_scale(const PGresult* pqres, int column_number)
{
    int scale = PGSQL_VAR_NOINFO; /* No information available */
    int fmod = 0;

    if (NULL != pqres)
    {
        /* Check if given column is of type of NUMERIC */
        if (PGSQL_TYPE_DECIMAL == PQftype(pqres, column_number))
        {
            fmod = PQfmod(pqres, column_number);
            scale = ((fmod - PGSQL_VAR_HDRSZ) & 0xffff);
        }
    }

    return scale;
}

int postgis_get_char_length(const PGresult* pqres, int column_number)
{
    int length = PGSQL_VAR_NOINFO; /* No information available */
    int fmod = 0;

    if (NULL != pqres)
    {
        /* Check if given column is of type of fixed-length CHARACTER */
        if (PGSQL_TYPE_CHAR == PQftype(pqres, column_number)
            || PGSQL_TYPE_BPCHAR == PQftype(pqres, column_number)
            || PGSQL_TYPE_NAME == PQftype(pqres, column_number))
        {
            fmod = PQfmod(pqres, column_number);
            if (PGSQL_VAR_NOINFO != fmod)
                length = (fmod - PGSQL_VAR_HDRSZ);
        }
        else if( PGSQL_TYPE_BIT == PQftype(pqres, column_number) )
            length = 1; // Byte type
    }

    return length;
}

int postgis_get_varchar_length(const PGresult* pqres, int column_number)
{
    int length = PGSQL_VAR_NOINFO; /* No information available */
    int fmod = 0;

    if (NULL != pqres)
    {
        /* Check if given column is of character varying type */
        if (PGSQL_TYPE_VARCHAR == PQftype(pqres, column_number)
            || PGSQL_TYPE_NAME == PQftype(pqres, column_number))
        {
            fmod = PQfmod(pqres, column_number);
            if (PGSQL_VAR_NOINFO != fmod)
                length = (fmod - PGSQL_VAR_HDRSZ);
        }
    }

    return length;
}

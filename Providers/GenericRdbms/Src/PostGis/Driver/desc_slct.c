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

#include "define.h"
#include "local.h"
#include "type.h"
#include "pgconn_status.h"
#include "pgresult_status.h"
#include "pgresult_clear.h"

/*
 * TODO: mloskot - To be removed before release.
 */
#include "assert.h"

#ifdef _WIN32
#define case_insensitive_compare _stricmp
#else
#define case_insensitive_compare strcasecmp
#endif


int postgis_desc_slct (
    postgis_context_def *context,
    char *cursor,
    int   position,     /* position within select clause    */
    int   name_size,    /* The maximum size of name         */
    char *name,         /* Name of this field               */
    int  *rdbi_type,    /* rdbi constant                    */
    int  *binary_size,  /* bytes of memory to hold it       */
    int  *null_ok)     /* TRUE iff NULLs ok in this column */
{
    PGconn* postgis = NULL;
    PGresult* pgres = NULL;
    postgis_cursor_def* curs = NULL;
    PGSQL_BIND *binds = NULL;
    
    char* fname = NULL; /* name of the column */
    Oid ftype = 0;      /* data type of the column */
    int fsize = 0;      /* size in bytes of the column */
    int fmod = -1;      /* typemod, z`-1 indicates "no information available" */

    int type = -1; /* RDBI type */

    int ret = RDBI_GENERIC_ERROR;

    if (-1 == context->postgis_current_connect)
        ret = RDBI_NOT_CONNECTED;
    else
    {
        postgis = context->postgis_connections[context->postgis_current_connect];
        if (RDBI_SUCCESS != postgis_pgconn_status(postgis))
            ret = RDBI_NOT_CONNECTED;
        else
        {
            curs = (postgis_cursor_def*)cursor;
            if (NULL == curs)
                ret = RDBI_GENERIC_ERROR;
            else
            {
                if (NULL != curs->stmt_name)
                {   
                    //
                    // TODO: mloskot - Test with non-existing statement
                    //
                    
                    /* Obtain information about prepared statement. */
                    pgres = PQdescribePrepared(postgis, curs->stmt_name);
                    if (RDBI_SUCCESS != postgis_pgresult_status(pgres))
                        ret = RDBI_INVLD_DESCR_OBJTYPE;
                    else
                    {
                        assert(PGRES_COMMAND_OK == PQresultStatus(pgres));

                        /* Zero based. */
                        position--;

                        if ((0 > position) || (position >= PQnfields(pgres)))
                            ret = RDBI_NOT_IN_DESC_LIST;
                        else
                        {
                            ftype = PQftype(pgres, position);
                            fsize = PQfsize(pgres, position);
                            fmod = PQfmod(pgres, position);

                            type = postgis_to_rdbi_type(postgis, ftype, fsize, fmod);
                            if (-1 == type)
                                ret = RDBI_INVLD_DESCR_OBJTYPE;
                            else
                            {
                                /* NULL is returned if the column number is out of range. */
                                fname = PQfname(pgres, position);
                                assert(NULL != fname);

                                strncpy (name, fname, name_size);
                                name[name_size - 1] = '\0';
                                *rdbi_type = type;

                                switch (type)
                                {
                                case RDBI_CHAR:       /* Fixed-length strings. */
                                case RDBI_FIXED_CHAR:
                                    if (-1 == fsize)
                                    {
                                        /* Fetch size from typemod. */
                                        fsize = postgis_get_char_length(pgres, position);
                                    }
                                    assert(PGSQL_VAR_NOINFO != fsize);

                                    if (1 == fsize) /* CHAR(1) */
                                        *binary_size = fsize + 1;
                                    else            /* CHAR(N) */
                                        *binary_size = fsize; 

                                    if ( (*binary_size) > PGSQL_VARCHAR_MAX_SIZE )
                                        *binary_size = PGSQL_VARCHAR_MAX_SIZE;
                                    break;

                                case RDBI_STRING:   /* Variable-length strings. */
                                    if (-1 == fsize)
                                    {
                                        /* Fetch size from typemod. */
                                        fsize = postgis_get_varchar_length(pgres, position);
                                    }

                                    if ( (fsize > 0) && (fsize < PGSQL_VARCHAR_MAX_SIZE) ) /* VARCHAR(N) */
                                        *binary_size = fsize;
                                    else            /* TEXT */
                                        *binary_size = PGSQL_VARCHAR_MAX_SIZE;
                                    break;

                                case RDBI_FLOAT:
                                    *binary_size = sizeof(float);
                                    break;
                                case RDBI_DOUBLE:
                                    *binary_size = sizeof(double);
                                    break;
                                case RDBI_SHORT:
                                    *binary_size = sizeof(short);
                                    break;
                                case RDBI_INT:
                                    *binary_size = sizeof(int);
                                    break;
                                case RDBI_LONG:
                                    *binary_size = sizeof(long);
                                    break;
                                case RDBI_GEOMETRY:
                                    *binary_size = sizeof(void*); /* Geometry address */
                                    break;
                                case RDBI_LONGLONG:
#ifdef _WIN32
                                    *binary_size = sizeof(__int64);
#else
                                    *binary_size = sizeof(int64_t);
#endif
                                    break;
                                case RDBI_BOOLEAN:
                                    *binary_size = PGSQL_BOOLEAN_RDBI_SIZE;
                                    break;
                                case RDBI_DATE:
                                    /*
                                     * TODO: mloskot - check size of DATE type
                                     */
                                    *binary_size = PGSQL_VARCHAR_MAX_SIZE;
                                    break;
                                case RDBI_BLOB:
                                    /*
                                     * TODO: mloskot - check size of DATE type
                                     */
                                    *binary_size = 4;
                                    break;
                                case RDBI_ROWID:
                                    assert(!"postgis_desc_slct: ROWID type is unsupported");
                                    break;
                                default:
                                    /*
                                     * TODO: mloskot - Unknown size?
                                     */
                                    assert(!"postgis_desc_slct: Unknown field size");

                                    *binary_size = 0;
                                }

                                /*
                                 * TODO: mloskot - How to get nullability on this stage?
                                 */
                                *null_ok = 1; /* 1 is a temporary value. */
                                
                                ret = RDBI_SUCCESS;
                            }
                        }
                    }

                    /* Free PostgreSQL result. */
                    postgis_pgresult_clear(pgres);
                }

            } /* if (NULL == curs) */
        
        }

    } /* if (-1 == context->postgis_current_connect) */

    return (ret);
}

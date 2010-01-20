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

#include "bind.h"
#include "local.h"
#include "type.h"
#include "pgconn_status.h"
#include "pgresult_status.h"
#include <limits.h> /* LONG_MIN, LONG_MAX */
#include <stdlib.h> /* malloc(), free() */
#include <string.h> /* memset() */

int postgis_bind (
    postgis_context_def *context,
    char *cursor,
    char *name,
    int   datatype,
    int   size,
    char *address,
    char *null_ind)
{
    PGconn* postgis = NULL;
    postgis_cursor_def* curs = NULL;
    PGSQL_BIND *binds = NULL;
	int		*srids;
    Oid typeoid = 0;
    int index = 0;
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
                /* TODO: need an error for bind names not supported */

                /* Numeric position */
                index = strtol(name, NULL, 10);
                if (0 >= index || LONG_MIN == index || LONG_MAX == index)
                {
                    ret = RDBI_GENERIC_ERROR; /* need an error for unknown name */
                }
                else
                {
                    index--; /* Make parameter index zero -based */

                    typeoid = rdbi_to_postgis_type(postgis, datatype, size, 0);
                    if (0 >= typeoid)
                    {
                        ret = RDBI_INVLD_DESCR_OBJTYPE; 
                    }
                    else
                    {
                        if (index >= curs->bind_count)
                        {
                            binds = (PGSQL_BIND *)malloc((index + 1) * sizeof(PGSQL_BIND));
                            srids = (int*)malloc((index + 1) * sizeof(int));
                            if ((PGSQL_BIND*)NULL == binds || (int*)NULL == srids)
                            {
                                if ((PGSQL_BIND*)NULL != curs->binds)
                                    free (curs->binds);
								if ((int*)NULL != curs->srids)
									free (curs->srids);
                                curs->bind_count = 0;
                            }
                            else
                            {
                                memset (binds, 0, (index + 1) * sizeof(PGSQL_BIND));
    							memset (srids, 0, (index + 1) * sizeof (int));
                                if ((PGSQL_BIND*)NULL != curs->binds)
                                {
                                    memcpy (binds, curs->binds, curs->bind_count * sizeof(PGSQL_BIND));
                                    free (curs->binds);
									memcpy (srids, curs->srids, curs->bind_count * sizeof (int));
									free (curs->srids);
                                }
                                curs->bind_count = index + 1;
                                curs->binds = binds;
                                curs->srids = srids;
                            }
                        }
                        if (index >= curs->bind_count)
                            ret = RDBI_MALLOC_FAILED;
                        else
                        {
                            /*
                             * typeoid - native PostgreSQL type is detected.
                             * size - value passed from GDBI layer.
                             * address - comes from GDBI too
                             */
                            curs->binds[index].buffer_type = typeoid;
                            curs->binds[index].buffer_length = size;
                            curs->binds[index].buffer = address;
                            if ((char*)NULL != null_ind)
                            {
                                curs->binds[index].is_null = null_ind;
                            }
                            ret = RDBI_SUCCESS;
                        }
                    }
                }
            }
        }
    }

    return (ret);
}

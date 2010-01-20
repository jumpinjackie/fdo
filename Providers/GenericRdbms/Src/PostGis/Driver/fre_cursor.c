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

#include "Geometry.h"
#include "fre_cursor.h"
#include "run_sql.h"
#include "pgresult_clear.h"
#include "commit.h"
#include <stdlib.h> // free()
#include <assert.h>

void postgis_fre_binds (postgis_cursor_def* cursor)
{
    int i = 0;

    assert(NULL != cursor);

    if (NULL != cursor->redefines)
    {
        for (i = 0; i < cursor->define_count; i++)
        {
            if (NULL != cursor->redefines[i].geometry)
            {
                FreeGeometry (cursor->redefines[i].geometry);
                cursor->redefines[i].geometry = NULL;
            }
        }
        free (cursor->redefines);
        cursor->redefines = NULL;
    }

    if (NULL != cursor->rebinds)
    {
        for (i = 0; i < cursor->bind_count; i++)
        {
            if (NULL != cursor->rebinds[i].substitution)
            {
                free (cursor->rebinds[i].substitution);
                cursor->rebinds[i].substitution = NULL;
            }
        }
        free (cursor->rebinds);
        cursor->rebinds = NULL;
    }

    if (NULL != cursor->defines)
    {
        free (cursor->defines);
        cursor->define_count = 0;
        cursor->defines = NULL;
    }

    if (NULL != cursor->binds)
    {
        free (cursor->binds);
        cursor->bind_count = 0;
        cursor->binds = NULL;
    }

    if ((int *)NULL != cursor->srids)
    {
        free (cursor->srids);
        cursor->srids = (int *)NULL;
    }

    if (NULL != cursor->sql)
    {
        free (cursor->sql);
        cursor->sql = NULL;
    }
}

int postgis_fre_cursor (postgis_context_def *context, char **cursor)
{
    PGconn* postgis = NULL;
    postgis_cursor_def* curs = NULL;
    int ret = RDBI_GENERIC_ERROR;

    assert(NULL != context);

    if (-1 == context->postgis_current_connect)
    {
        ret = RDBI_NOT_CONNECTED;
    }
    else
    {
        postgis = context->postgis_connections[context->postgis_current_connect];
        if (NULL != postgis)
        {
            curs = (postgis_cursor_def*)*cursor;
            if (NULL != curs)
            {
                /* Reset statement name. */
                if (NULL != curs->stmt_result)
                {
                    postgis_pgresult_clear(curs->stmt_result);
                    curs->stmt_result = NULL;
                }
                if( curs->isSelect && curs->sql != NULL )
                    postgis_commit(context, 0 ); // end the transaction that we started for the cursor

                /* Free binds collection. */
                postgis_fre_binds (curs);

                char buffer[50];
                int rows_processed;
                sprintf( buffer, "DEALLOCATE %s", curs->stmt_name );
                postgis_run_sql( context, buffer, false, &rows_processed );
                
                /* Reset statement name. */
                memset(curs->stmt_name, 0, POSTGIS_STMT_NAME_SIZE);

                /* Free the whole cursor instance. */
                free (curs);
                curs = NULL;

                *cursor = NULL;
                ret = RDBI_SUCCESS;
            }
        }
        else
        {
            ret = RDBI_NOT_CONNECTED;
        }
    }

    return (ret);
}

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

#include "local.h"
#include "define.h"
#include "type.h"
#include "Inc/ut.h"

int postgis_desc_bind (
    postgis_context_def *context,
    char *cursor,
    int   position,     /* position within select clause    */
    int   name_size,    /* The maximum size of name         */
    char *name)        /* Name of this bind variable       */
{
    PGconn* postgis = NULL;
    postgis_cursor_def* curs = NULL;
    PGSQL_BIND *binds = NULL;

    char buffer[20];
    int ret;

    if (-1 == context->postgis_current_connect)
    {
        ret = RDBI_NOT_CONNECTED;
    }
    else
    {
        postgis = context->postgis_connections[context->postgis_current_connect];
        if (NULL != postgis)
        {
            curs = (postgis_cursor_def*)cursor;
            if (NULL != curs)
            {
                /*
                 * Check if statement exists.
                 *
                 * TODO: mloskot - Check if curs->stmt_result is
                 * available on this stage. Now, it seems it isn.t
                 */
                if (NULL != curs->stmt_name)
                {
                    position--; /* zero based */
                    if ((0 > position) || (curs->bind_count - 1 < position))
                    {
                        ret = RDBI_NOT_IN_DESC_LIST;
                    }
                    else
                    {
                        ut_itoa (position + 1, buffer);
                        strncpy (name, buffer, name_size);
                        ret = RDBI_SUCCESS;
                    }
                }
            }
            else
            {
                ret = RDBI_GENERIC_ERROR;
            }
        }
        else
        {
            ret = RDBI_NOT_CONNECTED;
        }
    }

    return (ret);
}

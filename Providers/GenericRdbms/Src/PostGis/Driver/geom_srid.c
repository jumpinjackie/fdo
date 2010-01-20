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
#include "local.h"
#include <stdlib.h>

int postgis_geom_srid_set ( 
	postgis_context_def	*context,
    char   *cursor,                     /* cursor associated with SQL stmnt */
	char	*geom_col_name,
    long srid ) 
{
    postgis_cursor_def    *curs;
	int index;
    int                 rc = RDBI_SUCCESS;

	if (context->postgis_current_connect == -1)
        rc = RDBI_NOT_CONNECTED;
    else
    {
        curs = (postgis_cursor_def*)cursor;
        if (curs != (postgis_cursor_def*)NULL)
		{
			index = atoi (geom_col_name); /* numeric position */
            /* need an error if columns have not been bound yet */
            if (index <= 0 || curs->bind_count < index)
                rc = RDBI_GENERIC_ERROR; /* need an error for unknown name */
            else
            {
                index--; /* make it zero based */
				if (curs->srids != (int*)NULL)
					curs->srids[index] = (int)srid;
			}
		}
	}

    return rc;
}

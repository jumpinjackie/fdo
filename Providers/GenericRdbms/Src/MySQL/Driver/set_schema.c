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

#include "set_schema.h"
#include "run_sql.h"

/* Makes the given schema (MySql database) the current schema */
int mysql_set_schema (
    mysql_context_def *context,
    char *schema_name
)
{
    char* stmt = NULL;
    int ret;

    stmt = (char*)alloca( strlen(schema_name) + 10 );

    sprintf( stmt, "use %s", schema_name );
	
    ret = mysql_run_sql( context, stmt, 0, NULL );
	if (ret == RDBI_SUCCESS)
		ret = mysql_run_sql( context, stmt, 1 /* set schema for 2nd connection */, NULL );

    return (ret);
}

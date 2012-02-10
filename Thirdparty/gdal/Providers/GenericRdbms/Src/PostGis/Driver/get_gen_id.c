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

#include "get_gen_id.h"
#include "est_cursor.h"
#include "sql.h"
#include "define.h"
#include "execute.h"
#include "fetch.h"
#include "fre_cursor.h"

#include <string.h> /*stricmp() */
#include <assert.h>

int postgis_get_gen_id(
    postgis_context_def* context,
	const char* table_name,
//    const char* column_name,
	int* id)
{
    postgis_cursor_def* curs = NULL;
    //const char* column_name = NULL;
    int ret = RDBI_GENERIC_ERROR;

    assert(NULL != context);
    assert(NULL != table_name);
    assert(NULL != id);
#if 0    
    if (0 == stricmp(table_name, "f_classdefinition"))
        column_name = "classid";
    else if (0 == stricmp(table_name, "f_spatialcontext"))
        column_name = "scid";
    else if (0 == stricmp(table_name, "f_spatialcontextgroup"))
        column_name = "scid";
    else if (0 == stricmp(table_name, "f_spatialcontextgeom"))
        column_name = "scgid";
    else 
        assert(!"postgis_get_gen_id() - UNKNOWN TABLE");
#endif    

	return ret;
}

/*
	// establish cursor
	if ( RDBI_SUCCESS != mysql_est_cursor(context, (char **)&c) )
        goto the_exit;

	sprintf(sql_buf, "select LAST_INSERT_ID()");

	if ( RDBI_SUCCESS != mysql_sql( context, (char *)c, sql_buf, FALSE, (char *)NULL,
								NULL, (char *) NULL) )
        goto the_exit;

	// define output locations
	if ( RDBI_SUCCESS != mysql_define( context, (char *)c, "1", RDBI_LONG, sizeof(long),
									(char *) id, (short *)NULL) )
        goto the_exit;

    // execute the SQL statement
    if ( RDBI_SUCCESS != mysql_execute( context, (char *)c, 1, 0, &rows) )
        goto the_exit;

	// execute the SQL statement & fetch row
	if ( RDBI_SUCCESS != mysql_fetch( context, (char *)c, 1, FALSE, FALSE, &rows) )
        goto the_exit;

    if (c != (mysql_cursor_def *)NULL)
        mysql_fre_cursor (context, (char **)&c);

    rdbi_status = RDBI_SUCCESS;
*/

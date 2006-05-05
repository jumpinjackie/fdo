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
 */

/************************************************************************
* Name																	*
*	odbcdr_get_gen_id - Gets the last value generated for an            *
*                     IDENTITY column                                   *
*                                                                       *
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	rdbi_get_gen_id(table_name, id) 								    *
*	char *table_name;													*
*	int  *id;														    *
*																		*
* Description															*
*       This function returns the last generated value. 	 		    *
*																		*
* Parameters															*
*	table_name: input													*
*		The table containing the IDENTITY column of interest            *
*       (unique per table).	                                            *
*																		*
*	id: output														    *
*		The value.										                *
*																		*
* Function value														*
*	Returns an rdbi status code from inc/rdbi.h 						*
*																		*
************************************************************************/
#include <Inc/rdbi.h>
#include <Inc/ut.h>
#include	<Inc/debugext.h>
#include "proto_p.h"

int odbcdr_get_gen_id(
    odbcdr_context_def  *context,
	char *table_name_I,
	int  *id_O
	)
{
	char				    sql_buf[100];
	int 				    rows;
	odbcdr_cursor_def	    *c = NULL;
	odbcdr_connData_def	    *connData;
    SQLLEN                  null_ind;
	int 				    rdbi_status = RDBI_GENERIC_ERROR;
	int						global_identity = (table_name_I[0] == '\0'); 
    ODBCDR_ERRORINFO_VARS;

	debug_on1("odbcdr_get_gen_id", "table_name '%s'", ISNULL(table_name_I));

    *id_O = 0;
	
	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

	/* establish cursor. Reuse the existing one */
	if ( global_identity ) {
		
		c = connData->identity;

		if ( c == (odbcdr_cursor_def *)NULL ) {

			ODBCDR_RDBI_ERR( odbcdr_est_cursor(context, (char **)&c) );

			sprintf(sql_buf, "select @@IDENTITY" );  

			ODBCDR_RDBI_ERR( odbcdr_sql( context, (char *)c, sql_buf, FALSE, (char *)NULL,
									(void *)NULL, (char *) NULL) );

			/* define output locations */
			ODBCDR_RDBI_ERR( odbcdr_define( context, (char *)c, "1", RDBI_LONG, sizeof(long),
										(char *) &connData->identity_id, (SQLLEN *)&connData->identity_ni) );
		}

		/* execute the SQL statement */
		ODBCDR_RDBI_ERR( odbcdr_execute( context, (char *)c, 1, 0, &rows) );

		/* execute the SQL statement & fetch row */
		ODBCDR_RDBI_ERR( odbcdr_fetch( context, (char *)c, 1, &rows) );

		/* set output value */
		*id_O = connData->identity_id;

	} else {

		ODBCDR_RDBI_ERR( odbcdr_est_cursor(context, (char **)&c) );

		sprintf(sql_buf, "select IDENT_CURRENT('%s')", table_name_I);

		ODBCDR_RDBI_ERR( odbcdr_sql( context, (char *)c, sql_buf, FALSE, (char *)NULL,
									(void *)NULL, (char *) NULL) );

		/* define output locations */
		ODBCDR_RDBI_ERR( odbcdr_define( context, (char *)c, "1", RDBI_LONG, sizeof(long),
										(char *) id_O, (SQLLEN *)&null_ind) );

		/* execute the SQL statement */
		ODBCDR_RDBI_ERR( odbcdr_execute( context, (char *)c, 1, 0, &rows) );

		/* execute the SQL statement & fetch row */
		ODBCDR_RDBI_ERR( odbcdr_fetch( context, (char *)c, 1, &rows) );

		/* Note: it returns null_ind set if no value was generated during this session */
	}

    rdbi_status = RDBI_SUCCESS;

the_exit:
	
/*	if ( !global_identity && c != (odbcdr_cursor_def *)NULL) {   */
	if ( c != (odbcdr_cursor_def *)NULL) {   
	    ODBCDR_ERRORINFO_GET(context);
	    odbcdr_fre_cursor (context, (char **)&c);
	    ODBCDR_ERRORINFO_SET(context);
	}

	debug_return(NULL, rdbi_status);
}

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

#include <Inc/rdbi.h>
#include <Inc/ut.h>
#include <Inc/debugext.h>
#include "proto_p.h"

int local_odbcdr_sql(odbcdr_context_def  *context, char *cursor, rdbi_string_def* sql, int defer,
	char *verb, void *ptree, char *cursor_coc);

/************************************************************************
* Name																	*
*	odbcdr_get_gen_id - Gets the last value generated for an            *
*                     IDENTITY column                                   *
*                                                                       *
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	odbcdr_get_gen_id(table_name, id) 								    *
*	rdbi_string_def *table_name;										*
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

int local_odbcdr_get_gen_id(
    odbcdr_context_def  *context,
	rdbi_string_def     *table_name_I,
	int  *id_O
	)
{
	wchar_t				    sql_buf[100];
    rdbi_string_def         sqlval;
	int 				    rows;
	odbcdr_cursor_def	    *c = NULL;
	odbcdr_connData_def	    *connData;
    SQLLEN                  null_ind;
	int 				    rdbi_status = RDBI_GENERIC_ERROR;
	int						global_identity = ODBCDRV_STRING_EMPTY(table_name_I);
    ODBCDR_ERRORINFO_VARS;
    sqlval.wString = sql_buf;

#ifdef _DEBUG
    if (context->odbcdr_UseUnicode){
        debug_on1("odbcdr_get_gen_id", "table_name '%ls'", ISNULL(table_name_I->cwString));
    }else{
        debug_on1("odbcdr_get_gen_id", "table_name '%s'", ISNULL(table_name_I->ccString));
    }
#endif

    *id_O = 0;
	
	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

	/* establish cursor. Reuse the existing one */
	if ( global_identity ) {
		
		c = connData->identity;

		if ( c == (odbcdr_cursor_def *)NULL ) {

			ODBCDR_RDBI_ERR( odbcdr_est_cursor(context, (char **)&c) );

            if (context->odbcdr_UseUnicode)
                odbcdr_swprintf(sqlval.wString, 100, L"select @@IDENTITY" );
            else
                sprintf(sqlval.cString, "select @@IDENTITY" );

			ODBCDR_RDBI_ERR( local_odbcdr_sql( context, (char *)c, &sqlval, FALSE, (char *)NULL,
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

        if (context->odbcdr_UseUnicode)
            odbcdr_swprintf(sqlval.wString, 100, L"select IDENT_CURRENT('%ls')", table_name_I->cwString);
        else
            sprintf(sqlval.cString, "select IDENT_CURRENT('%s')", table_name_I->ccString);

		ODBCDR_RDBI_ERR( local_odbcdr_sql( context, (char *)c, &sqlval, FALSE, (char *)NULL,
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
	    ODBCDR_ERRORINFO_GET;
	    odbcdr_fre_cursor (context, (char **)&c);
	    ODBCDR_ERRORINFO_SET;
	}

	debug_return(NULL, rdbi_status);
}

int odbcdr_get_gen_id(
    odbcdr_context_def  *context,
	const char *table_name_I,
	int  *id_O
	)
{
    rdbi_string_def str;
    str.ccString = table_name_I;
    return local_odbcdr_get_gen_id(context,	&str, id_O);
}

int odbcdr_get_gen_idW(
    odbcdr_context_def  *context,
	const wchar_t *table_name_I,
	int  *id_O
	)
{
    rdbi_string_def str;
    str.cwString = table_name_I;
    return local_odbcdr_get_gen_id(context,	&str, id_O);
}

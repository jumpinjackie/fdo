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
*																		*
* Name																	*
*	odbcdr_stores_act - Activate a fetch of all datastores
*																		*
* Synopsis																*
*	odbcdr_stores_act(context)    				    			        *
*	odbcdr_context_def *context;										*
*																		*
* Description															*
*		This  module  activates  a	fetch  of  all datastores           *
*       (DataSourceNames in ODBC terms)                                 *
*																		*
* Parameters															*
*																		*
* Function value														*
*		Returns an rdbi status code from inc/rdbi.h 					*
*																		*
* Remarks																*
*       This code is NOT re-entrant.  There is no handle to return      *
*       to identify a specific query of users.  It operates the same    *
*       way as the older odbcdr_col_*() functions.                      *
*                                                                       *
************************************************************************/

#include    <Inc/rdbi.h>
#include    <Inc/ut.h>
#include	<Inc/debugext.h>
#include    "proto_p.h"

#define _check_status  if (rdbi_status != RDBI_SUCCESS) goto the_exit;

int odbcdr_stores_act(
    odbcdr_context_def *context
	)
{
    SQLCHAR outDataSourceName[SQL_MAX_DSN_LENGTH+1];
    SQLSMALLINT outDataSourceNameLength = 0;
    SQLCHAR outDataSourceDesc[ODBCDR_MAX_BUFF_SIZE];
    SQLSMALLINT outDataSourceDescLength = 0;
    SQLUSMALLINT direction = SQL_FETCH_FIRST;

	int 				rdbi_status = RDBI_GENERIC_ERROR;
    SQLRETURN   ret = SQL_SUCCESS;

	debug_on("odbcdr_stores_act");

	/*
	** Set up ODBC environment (only once for all connections).
	*/
	if ( context->odbcdr_env == SQL_NULL_HENV ) {
		// Allocate the environment handle
		context->odbcdr_env = SQL_NULL_HENV;

    	ODBCDR_ODBC_ERR( SQLAllocHandle(SQL_HANDLE_ENV,
			     SQL_NULL_HENV,
			     &context->odbcdr_env),
		    SQL_HANDLE_ENV, context->odbcdr_env,
	       "SQLAllocHandle", "environment handle" );
		// Set ODBC version 
        ODBCDR_ODBC_ERR( SQLSetEnvAttr(context->odbcdr_env,
                SQL_ATTR_ODBC_VERSION,
                (SQLPOINTER)SQL_OV_ODBC3,
                SQL_IS_INTEGER),
            SQL_HANDLE_ENV, context->odbcdr_env,
	       "SQLSetEnvAttr", "environment attribute" );
    }
	
	/* Deactivate any outstanding queries */
	rdbi_status = odbcdr_stores_deac( context );
	_check_status;

    // Retrieve the list of DSN names, both SYSTEM and USER. 
    do
    {
        ret = SQLDataSources(context->odbcdr_env,
                             direction,
                             outDataSourceName,
                             SQL_MAX_DSN_LENGTH+1,
                             &outDataSourceNameLength,
                             outDataSourceDesc,
                             ODBCDR_MAX_BUFF_SIZE,
                             &outDataSourceDescLength);

        if (ret == SQL_SUCCESS) {
            /* Add name to the list. */
            odbcdr_NameListEntry_store_def newNle;
            (void) strcpy(newNle.name, (char*)outDataSourceName);
            if (NULL == ut_da_append( &context->odbcdr_nameList_stores, 1L, (void *) &newNle ))
            {
                rdbi_status = RDBI_MALLOC_FAILED;
                goto the_exit;
            }
        }

        direction = SQL_FETCH_NEXT;
    }
    while (ret == SQL_SUCCESS);

	context->odbcdr_nameListNextPosition_stores = 0;

the_exit:
	debug_return(NULL, rdbi_status);
}


/************************************************************************
*																		*
* Name																	*
*	odbcdr_stores_get - Get the next datastore.							*
*																		*
* Synopsis																*
*	odbcdr_stores_get(context, name, eof)								*
*	odbcdr_context_def *context;										*
*	char *name;		        											*
*	int  *eof;															*
*																		*
* Description															*
*		This module gets the next datatore in the fetch activated		*
*		by odbcdr_stores_act.											*
*																		*
* Parameters															*
*	name: output 		        										*
*		The name of the datstore. 										*
*																		*
*	eof: output 														*
*		End of fetch flag.												*
*																		*
* Function value														*
*		Returns an rdbi status code from inc/rdbi.h 					*
*																		*
* Remarks																*
*																		*
************************************************************************/

int odbcdr_stores_get(
    odbcdr_context_def *context,
	char *name,
	int  *eof
	)
{
  	int					rdbi_status = RDBI_GENERIC_ERROR;


	debug_on("odbcdr_stores_get");

	/* assume not end of fetch */
	*eof = FALSE;

    if (context->odbcdr_nameListNextPosition_stores < 0 || context->odbcdr_nameListNextPosition_stores > context->odbcdr_nameList_stores.size)
        goto the_exit;

    if (context->odbcdr_nameListNextPosition_stores == context->odbcdr_nameList_stores.size)
    {
        *eof = TRUE;
        context->odbcdr_nameListNextPosition_stores = -1;
        (void) ut_da_free(&context->odbcdr_nameList_stores);
    }
    else
    {
        odbcdr_NameListEntry_store_def * nle = (odbcdr_NameListEntry_store_def *) ut_da_get(&context->odbcdr_nameList_stores, context->odbcdr_nameListNextPosition_stores++);
        (void) strcpy(name, nle->name);
    }

	rdbi_status = RDBI_SUCCESS;
the_exit:
	debug_return(NULL, rdbi_status);

}


/************************************************************************
*																		*
* Name																	*
*	odbcdr_stores_deac - Deactivate the fetch of all datastore.			*
*																		*
* Synopsis																*
*	odbcdr_stores_deac(context)											*
*	odbcdr_context_def *context;										*
*																		*
* Description															*
*		This  module  deactivates	the   fetch  of all datastores 		*
*		activated by odbcdr_stores_act.									*
*																		*
* Parameters															*
*																		*
* Function value														*
*		Returns an rdbi status code from inc/rdbi.h 					*
*																		*
* Remarks																*
*																		*
************************************************************************/

int odbcdr_stores_deac( odbcdr_context_def *context )
{
	int 				rdbi_status = RDBI_GENERIC_ERROR;

	debug_on("odbcdr_stores_deac");

    context->odbcdr_nameListNextPosition_stores = -1;
    (void) ut_da_free(&context->odbcdr_nameList_stores);

	rdbi_status = RDBI_SUCCESS;
	debug_return(NULL, rdbi_status);
}

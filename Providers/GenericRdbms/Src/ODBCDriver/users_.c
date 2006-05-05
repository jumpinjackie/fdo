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
*	odbcdr_users_act - Activate a fetch of all users for an connection  *
*																		*
* Synopsis																*
*	odbcdr_users_act(context, target)    				    			*
*	odbcdr_context_def *context;										*
*   const char *target                                                  *
*																		*
* Description															*
*		This  module  activates  a	fetch  of  all users for a  		*
*		connection in an ODBC data source.   							*
*																		*
* Parameters															*
*	target: input 		        										*
*		The name of a specific user.  Results will be limited to this   *
*       user if given.  Pass NULL to get all users.                     *
*																		*
* Function value														*
*		Returns an rdbi status code from inc/rdbi.h 					*
*																		*
* Remarks																*
*       This code is NOT re-entrant.  There is no handle to return      *
*       to identify a specific query of users.  It operates the same    *
*       way as the older odbcdr_col_*() functions.                      *
*                                                                       *
*       This actually gets all SCHEMAS, not users.                      *
*
************************************************************************/

#include    <Inc/rdbi.h>
#include    <Inc/ut.h>
#include	<Inc/debugext.h>
#include    "proto_p.h"

#define _check_status  if (rdbi_status != RDBI_SUCCESS) goto the_exit;

int odbcdr_users_act(
    odbcdr_context_def *context,
    const char *target
	)
{
	odbcdr_cursor_def	*c;
	odbcdr_connData_def	*connData = NULL;
	int 				rdbi_status = RDBI_GENERIC_ERROR;
	int 				target_set = (NULL != target && target[0] != '\0');
    SQLUINTEGER schemaUsages = 0;
    SQLCHAR*    schemaToGet = (SQLCHAR*)SQL_ALL_SCHEMAS;
    const char* cTableTypes = "TABLE,VIEW";
    SQLCHAR     szSchema[ODBCDR_MAX_BUFF_SIZE];
    SQLINTEGER  cbSchema = 0;
    SQLRETURN   ret = SQL_SUCCESS;
    long        i;
    int         found;
    odbcdr_NameListEntry_user_def * nle;


	debug_on("odbcdr_users_act");

	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

	/* Deactivate any outstanding queries */
	rdbi_status = odbcdr_users_deac( context );
	_check_status;

    ODBCDR_ODBC_ERR( SQLGetInfo(
        connData->hDbc, 
        SQL_SCHEMA_USAGE,
        (SQLPOINTER)&schemaUsages,
        0,
        NULL),
        SQL_HANDLE_DESC, connData->hDbc, L"SQLGetInfo", "Fetching schemas" );

    if (schemaUsages & SQL_SU_DML_STATEMENTS)
    {
	    c = connData->users;

	    if (c == (odbcdr_cursor_def *)NULL) 
        {
		    rdbi_status = odbcdr_est_cursor( context, (char **)&c);
		    _check_status;
		    connData->users = c;
	    }

        /* Get the users, using the "all schemas" mode of SQLTables().
         * NOTE: it seems that SQLTables() returns 1 row per table/view as
         * opposed to 1 row per schema, which means that we may get the same
         * schema multiple times.  We filter out repeats by immediately
         * fetching all results and storing the unique ones in a local list.
         */

        ODBCDR_ODBC_ERR( SQLTables(
            c->hStmt,
            NULL,
            0,
            schemaToGet, 
            SQL_NTS,
            NULL, 
            0,
            (SQLCHAR*)cTableTypes, 
            SQL_NTS ),
            SQL_HANDLE_STMT, c->hStmt, L"SQLTables", L"Fetching schemas");


        ODBCDR_ODBC_ERR( SQLBindCol(
            c->hStmt,
            2,
            SQL_CHAR,
            szSchema,
            sizeof(szSchema),
            &cbSchema),
            SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching schemas");

        while (ret != SQL_NO_DATA)
        {
            szSchema[0] = '\0';

            ret = SQLFetch(c->hStmt);

            if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
                goto the_exit;

            if (ret == SQL_NO_DATA)
                break;

            if (target_set && strcmp(target, (char*)szSchema) != 0)
                continue;

            /* Look for the name already in the list. */
            for (i=0, found=false;  !found && i < context->odbcdr_nameList_users.size;  i++)
            {
                nle = (odbcdr_NameListEntry_user_def *) ut_da_get(&context->odbcdr_nameList_users, i);
                if (strcmp(nle->name, (char*)szSchema) == 0)
                    found = true;
            }

            if (!found)
            {
                /* Add name to the list. */
                odbcdr_NameListEntry_user_def newNle;
                (void) strcpy(newNle.name, (char*)szSchema);
                if (NULL == ut_da_append( &context->odbcdr_nameList_users, 1L, (void *) &newNle ))
                {
                    rdbi_status = RDBI_MALLOC_FAILED;
                    goto the_exit;
                }
            }
        } /* end while (ret != SQL_NO_DATA) */
    }
    else /* The data store did not support the right DML statement. */
    {
        /* Make a single, default name. */
        if (!target_set || strcmp(target, ODBCDR_DEFAULT_SCHEMA_NAME) == 0)
        {
            odbcdr_NameListEntry_user_def newNle;
            newNle.name[0] = '\0';
            if (NULL == ut_da_append( &context->odbcdr_nameList_users, 1L, (void *) &newNle ))
            {
                rdbi_status = RDBI_MALLOC_FAILED;
                goto the_exit;
            }
        }
    }

	context->odbcdr_nameListNextPosition_users = 0;

the_exit:
	if (NULL != connData && NULL != connData->users)
    {
        /* Ideally, we would keep the cursor around, and just re-execute later.
         * However, many ODBC drivers do not support concurrent cursors, and
         * calls to the module may be interleaved with others.  To avoid an
         * "Invalid cursor state" error from SQLTables later,
         * we'll have to free the cursor here. 
         */
		ODBCDR_RDBI_ERR( odbcdr_fre_cursor(context, (char **)(&connData->users)) );
        connData->users = NULL;
    }
	debug_return(NULL, rdbi_status);

}


/************************************************************************
*																		*
* Name																	*
*	odbcdr_users_get - Get the next user.								*
*																		*
* Synopsis																*
*	odbcdr_users_get(context, name, eof)								*
*	odbcdr_context_def *context;										*
*	char *name;		        											*
*	int  *eof;															*
*																		*
* Description															*
*		This module gets the next user in  the  fetch activated		    *
*		by odbcdr_users_act.											*
*																		*
* Parameters															*
*	name: output 		        										*
*		The name of the column. 										*
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

int odbcdr_users_get(
    odbcdr_context_def *context,
	char *name,
	int  *eof
	)
{
  	int					rdbi_status = RDBI_GENERIC_ERROR;


	debug_on("odbcdr_users_get");

	/* assume not end of fetch */
	*eof = FALSE;

    if (context->odbcdr_nameListNextPosition_users < 0 || context->odbcdr_nameListNextPosition_users > context->odbcdr_nameList_users.size)
        goto the_exit;

    if (context->odbcdr_nameListNextPosition_users == context->odbcdr_nameList_users.size)
    {
        *eof = TRUE;
        context->odbcdr_nameListNextPosition_users = -1;
        (void) ut_da_free(&context->odbcdr_nameList_users);
    }
    else
    {
        odbcdr_NameListEntry_user_def * nle = (odbcdr_NameListEntry_user_def *) ut_da_get(&context->odbcdr_nameList_users, context->odbcdr_nameListNextPosition_users++);
        (void) strcpy(name, nle->name);
    }

	rdbi_status = RDBI_SUCCESS;
the_exit:
	debug_return(NULL, rdbi_status);

}


/************************************************************************
*																		*
* Name																	*
*	odbcdr_users_deac - Deactivate the fetch of all users.				*
*																		*
* Synopsis																*
*	odbcdr_users_deac(context)											*
*	odbcdr_context_def *context;										*
*																		*
* Description															*
*		This  module  deactivates	the   fetch  of  all  users 		*
*		activated by odbcdr_users_act. 									*
*																		*
* Parameters															*
*																		*
* Function value														*
*		Returns an rdbi status code from inc/rdbi.h 					*
*																		*
* Remarks																*
*																		*
************************************************************************/

int odbcdr_users_deac( odbcdr_context_def *context )
{
	int 				rdbi_status = RDBI_GENERIC_ERROR;

	debug_on("odbcdr_users_deac");

    context->odbcdr_nameListNextPosition_users = -1;
    (void) ut_da_free(&context->odbcdr_nameList_users);

	rdbi_status = RDBI_SUCCESS;
	debug_return(NULL, rdbi_status);
}

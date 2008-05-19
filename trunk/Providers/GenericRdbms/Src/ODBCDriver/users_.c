/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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

#include    <Inc/rdbi.h>
#include    <Inc/ut.h>
#include	<Inc/debugext.h>
#include    "proto_p.h"

#define _check_status  if (rdbi_status != RDBI_SUCCESS) goto the_exit;
#define TABLESTYPES  "TABLE,VIEW"
#define TABLESTYPESW L"TABLE,VIEW"

#define SQLSERVER_SYS_SCHEMA_NAME "sys"
#define SQLSERVER_SYS_SCHEMA_NAMEW L"sys"
#define SQLSERVER_INFO_SCHEMA_NAME "INFORMATION_SCHEMA"
#define SQLSERVER_INFO_SCHEMA_NAMEW L"INFORMATION_SCHEMA"

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

int local_odbcdr_users_act(
    odbcdr_context_def *context,
    rdbi_string_def *target)
{
	odbcdr_cursor_def	*c;
	odbcdr_connData_def	*connData = NULL;
	int rdbi_status = RDBI_GENERIC_ERROR;
	int target_set;
    SQLUINTEGER schemaUsages = 0;
    int schemaUsagesSupported = TRUE;
    short	_odbcValue = SQL_ERROR;
    // vectors can be used as SQLCHAR
    wchar_t    schemaToGet[10];
    SQLWCHAR    szSchemaBuf[ODBCDR_MAX_BUFF_SIZE];
    rdbi_string_def szSchema;
    SQLLEN      cbSchema = 0;
    SQLRETURN   ret = SQL_SUCCESS;
    long        i;
    int         found;
    SQLSMALLINT charType;
    odbcdr_NameListEntry_user_def * nle;
    szSchema.wString = (wchar_t *)szSchemaBuf;

    if (context->odbcdr_UseUnicode)
    {
        odbcdr_swprintf(schemaToGet, 10, L"%s", SQL_ALL_SCHEMAS);
        charType = SQL_C_WCHAR;
    }
    else
    {
        sprintf((char*)schemaToGet, "%s", SQL_ALL_SCHEMAS);
        charType = SQL_C_CHAR;
    }
    target_set = !ODBCDRV_STRING_EMPTY(target);
	debug_on("odbcdr_users_act");

	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

	/* Deactivate any outstanding queries */
	rdbi_status = odbcdr_users_deac( context );
	_check_status;

    if (context->odbcdr_UseUnicode){
	    _odbcValue = SQLGetInfoW(connData->hDbc, SQL_SCHEMA_USAGE, (SQLPOINTER)&schemaUsages, 0, NULL);
    }else{
        _odbcValue = SQLGetInfo(connData->hDbc, SQL_SCHEMA_USAGE, (SQLPOINTER)&schemaUsages, 0, NULL);
    }

    if (_odbcValue != ODBCDR_SUCCESS)
        schemaUsagesSupported = FALSE;

    /* Sybase (observed with version 15.0) reports that it supports schemas, but does not
     * actually return any from SQLTables().  We'll bypass the schema query here, and hard-code
     * a schema name of the default creator name, "dbo", in the following block.
     */
    if (ODBCDriverType_Sybase == connData->driver_type)
        schemaUsagesSupported = FALSE;

    if (schemaUsagesSupported && (schemaUsages & SQL_SU_DML_STATEMENTS))
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
        if (context->odbcdr_UseUnicode){
            ODBCDR_ODBC_ERR( SQLTablesW(c->hStmt, NULL, 0, (SQLWCHAR*)schemaToGet,  SQL_NTS,
                NULL, 0, (SQLWCHAR*)TABLESTYPESW,  SQL_NTS ),
                SQL_HANDLE_STMT, c->hStmt, "SQLTables", "Fetching schemas");
        }else{
            ODBCDR_ODBC_ERR( SQLTables(c->hStmt, NULL, 0, (SQLCHAR*)schemaToGet,  SQL_NTS,
                NULL, 0, (SQLCHAR*)TABLESTYPES,  SQL_NTS ),
                SQL_HANDLE_STMT, c->hStmt, "SQLTables", "Fetching schemas");
        }

        ODBCDR_ODBC_ERR( SQLBindCol(c->hStmt, 2, charType, szSchema.wString,
            ODBCDR_MAX_BUFF_SIZE, &cbSchema),
            SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching schemas");

        while (ret != SQL_NO_DATA)
        {
            *szSchema.wString = L'\0';

            ret = SQLFetch(c->hStmt);

            if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
                goto the_exit;

            if (ret == SQL_NO_DATA)
                break;

            if (ODBCDriverType_SQLServer == connData->driver_type &&
                (ODBCDRV_STRING_COMPARE_LST(&szSchema, SQLSERVER_SYS_SCHEMA_NAME) == 0 ||
                 ODBCDRV_STRING_COMPARE_LST(&szSchema, SQLSERVER_INFO_SCHEMA_NAME) == 0))
                continue;

            if (target_set && ODBCDRV_STRING_COMPARE(target, &szSchema) != 0)
                continue;

            /* Look for the name already in the list. */
            for (i=0, found=false;  !found && i < context->odbcdr_nameList_users.size;  i++)
            {
                nle = (odbcdr_NameListEntry_user_def *) ut_da_get(&context->odbcdr_nameList_users, i);
                if (ODBCDRV_STRING_COMPARE_LST(&szSchema, nle->name) == 0)
                    found = true;
            }

            if (!found)
            {
                /* Add name to the list. */
                odbcdr_NameListEntry_user_def newNle;
                ODBCDRV_STRING_COPY_RST(newNle.name, &szSchema);
                if (NULL == ut_da_append( &context->odbcdr_nameList_users, 1L, (void *) &newNle ))
                {
                    rdbi_status = RDBI_MALLOC_FAILED;
                    goto the_exit;
                }
            }
        } /* end while (ret != SQL_NO_DATA) */
    }
    else if (ODBCDriverType_Sybase == connData->driver_type)
    {
        odbcdr_NameListEntry_user_def newNle;

        /* Make a single, default name of "dbo", which is the default "creator" in Sybase. */
        ODBCDRV_STRING_COPY_CST(newNle.name, ODBCDR_DRIVER_SYBASE_DEFAULT_SCHEMA_NAME);
        if (NULL == ut_da_append( &context->odbcdr_nameList_users, 1L, (void *) &newNle ))
        {
            rdbi_status = RDBI_MALLOC_FAILED;
            goto the_exit;
        }
    }
    else /* The data store did not support the right DML statement. */
    {
        /* Make a single, default name. */
        if (!target_set || ODBCDRV_STRING_COMPARE_LST(target, ODBCDR_DEFAULT_SCHEMA_NAME) == 0)
        {
            odbcdr_NameListEntry_user_def newNle;
            newNle.nameW[0] = L'\0';
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

int odbcdr_users_act(
    odbcdr_context_def *context,
    const char *target
	)
{
    rdbi_string_def str;
    str.ccString = target;
    return local_odbcdr_users_act(context, &str);
}

int odbcdr_users_actW(
    odbcdr_context_def *context,
    const wchar_t *target
	)
{
    rdbi_string_def str;
    str.cwString = target;
    return local_odbcdr_users_act(context, &str);
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
int local_odbcdr_users_get(
    odbcdr_context_def *context,
	rdbi_string_def *name,
	int  *eof
	)
{
  	int rdbi_status = RDBI_GENERIC_ERROR;

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
        ODBCDRV_STRING_COPY_LST(name, nle->name)
    }

	rdbi_status = RDBI_SUCCESS;
the_exit:
	debug_return(NULL, rdbi_status);
}

int odbcdr_users_get(
    odbcdr_context_def *context,
	char *name,
	int  *eof
	)
{
    rdbi_string_def str;
    str.cString = name;
    return local_odbcdr_users_get(context, &str, eof);
}

int odbcdr_users_getW(
    odbcdr_context_def *context,
	wchar_t *name,
	int  *eof
	)
{
    rdbi_string_def str;
    str.wString = name;
    return local_odbcdr_users_get(context, &str, eof);
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

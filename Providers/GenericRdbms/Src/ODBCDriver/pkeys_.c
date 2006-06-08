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
*	odbcdr_pkeys_act - Activate a fetch of all primary keys for an object *
*																		*
* Synopsis																*
*	odbcdr_pkeys_act(context, object)    				    			*
*	odbcdr_context_def *context;										*
*   const char *object;                                                 *
*																		*
* Description															*
*		This module activates a fetch of all primary key column names   *
*       for an object (table or view) in an ODBC data source.      	    *
*																		*
* Parameters															*
*                                                                       *
*	object: input 		        										*
*		The name of a specific object.                                  *
*																		*
* Function value														*
*		Returns an rdbi status code from inc/rdbi.h 					*
*																		*
* Remarks																*
*       This code is NOT re-entrant.  There is no handle to return      *
*       to identify a specific query of keys.  It operates the same     *
*       way as the older odbcdr_col_*() functions.                      *
*                                                                       *
*       The strategy for this method is to try several approaches to
*       obtaining the primary keys, in the following order, until one
*       of them succeeds.  If none succeed, no primary keys will be
*       detected.
*
*       1) Attempt to use SQLPrimaryKeys().
*       2) If using Oracle driver, and table is enabled for 
*          Workspace Manager, SQLPrimaryKeys() to find the primary keys
*          on the *_LT auxiliary OWM table.
*       3) Attempt to use SQLSpecialColumns().
*       4) Find all non-nullable columns and use them as the primary
*          keys.
*
************************************************************************/

#include    <Inc/rdbi.h>
#include    <Inc/ut.h>
#include	<Inc/debugext.h>
#include    "proto_p.h"

#define _check_status  if (rdbi_status != RDBI_SUCCESS) goto the_exit;

int odbcdr_pkeys_act(
    odbcdr_context_def *context,
    char *owner,
    const char *object
	)
{
	odbcdr_cursor_def	*c;
	odbcdr_connData_def	*connData = NULL;
	int 				rdbi_status = RDBI_GENERIC_ERROR;
    SQLINTEGER  cbColumnName = 0;
    SQLCHAR     szColumnName[ODBCDR_MAX_BUFF_SIZE];
    SQLRETURN   ret = SQL_SUCCESS;
    long        i;
    int         found;
    int         bFoundIdentityProperties = FALSE;
    int         bDriverIsNativeOracle = FALSE;
    int         bDriverIsOracle = FALSE;
    odbcdr_NameListEntry_pkey_def * nle;


	debug_on("odbcdr_pkeys_act");

    if (NULL == object || '\0' == object[0])
        goto the_exit;

	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

	/* Deactivate any outstanding queries */
	rdbi_status = odbcdr_pkeys_deac( context );
	_check_status;

    /* Check driver type, so we can make driver-specific decisions where required. */
    bDriverIsNativeOracle = (connData->driver_type == ODBCDriverType_OracleNative);
    bDriverIsOracle = 
        (connData->driver_type == ODBCDriverType_OracleNative ||
         connData->driver_type == ODBCDriverType_OracleNonNative);

    c = connData->keys;

    if (c == (odbcdr_cursor_def *)NULL) 
    {
	    rdbi_status = odbcdr_est_cursor( context, (char **)&c);
	    _check_status;
	    connData->keys = c;
    }

    /*********************************************************************************
     * Approach #1: Attempt to use SQLPrimaryKeys() to find primary keys on the table
     *********************************************************************************/

    /* We do not exit with error if ret != SQL_SUCCESS because some drivers do
     * not support SQLPrimaryKeys.
     */
    ret = SQLPrimaryKeys(
        c->hStmt,
        NULL, 
        0,
        NULL,
        0,
        (SQLCHAR*)object, 
        SQL_NTS);

    if (ret == SQL_SUCCESS)
    {
        ODBCDR_ODBC_ERR( SQLBindCol(
            c->hStmt, 4,  SQL_CHAR, szColumnName, sizeof(szColumnName), &cbColumnName),
            SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching primary keys" );

        while (ret != SQL_NO_DATA)
        {
            odbcdr_NameListEntry_pkey_def newNle;
            szColumnName[0] = '\0';

            ret = SQLFetch(c->hStmt);

            if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
                goto the_exit;

            if (ret == SQL_NO_DATA)
                break;

            (void) strcpy(newNle.name, (char*)szColumnName);
            if (NULL == ut_da_append( &context->odbcdr_nameList_pkeys, 1L, (void *) &newNle ))
            {
                rdbi_status = RDBI_MALLOC_FAILED;
                goto the_exit;
            }
            bFoundIdentityProperties = true;
        }
    }


    /*********************************************************************************
     * Approach #2: If using Oracle driver, and table is workspace-manager-enabled,
     *    use SQLPrimaryKeys() to find the primary keys on the *_LT auxiliary OWM table:
     *********************************************************************************/

    if (!bFoundIdentityProperties && bDriverIsOracle)
    {
        // Build the Oracle Workspace Manager (OWM) auxiliary table name:
        char objectNameOWM[ODBCDR_MAX_BUFF_SIZE];
        sprintf(objectNameOWM, "%s%s", object, ODBCDR_DRIVER_ORACLE_OWM_TABLE_SUFFIX);

#ifndef _WIN32
        /* Easysoft's ODBC driver for Oracle doesn't like cursor re-use. */
        ODBCDR_RDBI_ERR( odbcdr_fre_cursor(context, (char **)(&connData->keys)) );
        rdbi_status = odbcdr_est_cursor( context, (char **)&c);
        _check_status;
        connData->keys = c;
#endif

        ret = SQLPrimaryKeys(
            c->hStmt,
            NULL, 
            0,
            NULL, 
            0,
            (SQLCHAR*)objectNameOWM, 
            SQL_NTS);

        if (ret == SQL_SUCCESS)
        {
            ODBCDR_ODBC_ERR( SQLBindCol(
                c->hStmt, 4,  SQL_C_CHAR, szColumnName, sizeof(szColumnName), &cbColumnName),
                SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching primary keys" );

            while (ret != SQL_NO_DATA) {
                szColumnName[0] = '\0';

                ret = SQLFetch(c->hStmt);
                if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
                goto the_exit;

                if (ret == SQL_NO_DATA)
                    break;

                // Filter out workspace-manager-specific primary key columns:
                if (0!=_strnicmp((char*)szColumnName, ODBCDR_DRIVER_ORACLE_OWM_COLUMN_VERSION, sizeof(szColumnName)) &&
                    0!=_strnicmp((char*)szColumnName, ODBCDR_DRIVER_ORACLE_OWM_COLUMN_NEXTVER, sizeof(szColumnName)) &&
                    0!=_strnicmp((char*)szColumnName, ODBCDR_DRIVER_ORACLE_OWM_COLUMN_DELSTATUS, sizeof(szColumnName)) &&
                    0!=_strnicmp((char*)szColumnName, ODBCDR_DRIVER_ORACLE_OWM_COLUMN_LTLOCK, sizeof(szColumnName))
                   )
                {
                    odbcdr_NameListEntry_pkey_def newNle;
                    (void) strcpy(newNle.name, (char*)szColumnName);
                    if (NULL == ut_da_append( &context->odbcdr_nameList_pkeys, 1L, (void *) &newNle ))
                    {
                        rdbi_status = RDBI_MALLOC_FAILED;
                        goto the_exit;
                    }
                    bFoundIdentityProperties = true;
                }
            }
        }
    }


    /*********************************************************************************
     * Approach #3: Attempt to use SQLSpecialColumns() to find a set of columns that
     * uniquely identify rows
     *********************************************************************************/

    if (!bFoundIdentityProperties)
    {
#pragma message ("ToDo: Try returning just ROWID for primary key on Oracle")
#if 0
        if (bDriverIsNativeOracle) 
        {
            /* On Oracle, don't call SQLSpecialColumns() because it is incredibly slow.
             * This block should be entered only when the table is on Oracle and has
             * no primary keys. Typically, user-defined tables have primary keys and
             * thus we don't end up here.
             */
            /* The "ToDo" above is because it's not clear how much work is required in 
             * the schema manager to handle a primary key that isn't already in its
             * list of columns.  So, this code block is not compiled for now.
             */
            odbcdr_NameListEntry_pkey_def newNle;
            (void) strcpy(newNle.name, ODBCDR_DRIVER_ORACLE_ROWID_NAME);
            if (NULL == ut_da_append( &context->odbcdr_nameList_pkeys, 1L, (void *) &newNle ))
            {
                rdbi_status = RDBI_MALLOC_FAILED;
                goto the_exit;
            }
            bFoundIdentityProperties = true;
        }
        else
#endif
        {
#ifndef _WIN32
            /* Easysoft's ODBC driver for Oracle doesn't like cursor re-use. */
            ODBCDR_RDBI_ERR( odbcdr_fre_cursor(context, (char **)(&connData->keys)) );
            rdbi_status = odbcdr_est_cursor( context, (char **)&c);
            _check_status;
            connData->keys = c;
#endif

            ret = SQLSpecialColumns(
                c->hStmt,
                SQL_BEST_ROWID,
                NULL,
                0,
                NULL, 
                0,
                (SQLCHAR*)object,
                SQL_NTS,
                SQL_SCOPE_CURROW, /* Ideally we would use SQL_SCOPE_SESSION, but is not always
                                   * a guarantee that the rowid will be immutable. */
                SQL_NULLABLE    /* From Microsoft ODBC docs: "Some drivers cannot support SQL_NO_NULLS,
                                 * and these drivers will return an empty result set if SQL_NO_NULLS was
                                 * specified. Applications should be prepared for this case and request
                                 * SQL_NO_NULLS only if it is absolutely required. */
                );

            /* We do not exit with error if ret != SQL_SUCCESS because some drivers do
             * not support SQLSpecialColumns.
             */
            if (ret == SQL_SUCCESS)
            {
                SQLINTEGER  cbDataType = 0;
                SQLSMALLINT ssDataType = 0;
                SQLINTEGER  cbTypeName = 0;
                SQLCHAR    szTypeName[ODBCDR_MAX_BUFF_SIZE];
                SQLINTEGER  cbColumnSize = 0;
                SQLINTEGER  iColumnSize = 0;
                SQLINTEGER  cbBufferLen = 0;
                SQLINTEGER  iBufferLen = 0;
                SQLINTEGER  cbDecimalDigits = 0;
                SQLSMALLINT iDecimalDigits = 0;
                SQLINTEGER  cbPseudoColumn = 0;
                SQLSMALLINT iPseudoColumn = 0;

                szTypeName[0] = '\0';

                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 2,  SQL_C_CHAR, szColumnName, sizeof(szColumnName), &cbColumnName ),
                SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching primary keys" );

#if 0
                /* If it becomes possible (and necessary) to return completely described
                 * columns from this module, do the following extra binds.  This may become
                 * an issue for pseudo-columns, which may not be in the caller's column list.
                 * They can be detected by checking for "SQL_PC_PSEUDO == iPseudoColumn".
                 */
                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 3,  SQL_C_SSHORT, &ssDataType, 0, &cbDataType),
                SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching primary keys" );

                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 4,  SQL_C_CHAR, szTypeName, sizeof(szTypeName), &cbTypeName),
                SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching primary keys" ); 

                 //NOTE: parameter 5 is in *bytes* not *characters*!
                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 5,  SQL_C_SLONG, &iColumnSize, 0, &cbColumnSize),
                SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching primary keys" );

                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 6,  SQL_C_SLONG, &iBufferLen, 0, &cbBufferLen),
                SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching primary keys" );

                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 7,  SQL_C_SSHORT, &iDecimalDigits, 0, &cbDecimalDigits),
                SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching primary keys" );

                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 8,  SQL_C_SSHORT, &iPseudoColumn, 0, &cbPseudoColumn),
                SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching primary keys" );
#endif
                while (ret != SQL_NO_DATA)
                {
                    szColumnName[0] = '\0';

                    ret = SQLFetch(c->hStmt);
                    if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
                        goto the_exit;

                    if (ret == SQL_NO_DATA)
                        break;

                    /* Do not use Oracle's ROWID.  We currently do not return it as a
                     * column name for a table, and Schema Manager requires that a primary
                     * key appear in the column list.
                     */
                    if (strcmp((char *)szColumnName, ODBCDR_DRIVER_ORACLE_ROWID_NAME)==0)
                        continue;
                                                                                                     

                    /* The Microsoft Access driver sometimes returns the same column more 
                     * than once, so be careful to avoid duplicates in our collection.
                     */
                    for (i=0, found=false;  !found && i < context->odbcdr_nameList_pkeys.size;  i++)
                    {
                        nle = (odbcdr_NameListEntry_pkey_def *) ut_da_get(&context->odbcdr_nameList_pkeys, i);
                        if (strcmp(nle->name, (char*)szColumnName) == 0)
                            found = true;
                    }

                    if (!found)
                    {
                        /* Add name to the list. */
                        odbcdr_NameListEntry_pkey_def newNle;
                        (void) strcpy(newNle.name, (char*)szColumnName);
                        if (NULL == ut_da_append( &context->odbcdr_nameList_pkeys, 1L, (void *) &newNle ))
                        {
                            rdbi_status = RDBI_MALLOC_FAILED;
                            goto the_exit;
                        }
                        bFoundIdentityProperties = true;
                    }
                }
            }
        }
    }

    /*********************************************************************************
     * Approach #4: Find all auto-incremented columns and use them as the identity property.
     *********************************************************************************/

    if (!bFoundIdentityProperties)
    {
	    char column_name[ODBCDR_MAX_BUFF_SIZE];
	    char type[ODBCDR_MAX_BUFF_SIZE];
	    int  length;
	    int  scale;
	    int  nullable;
        int  is_autoincrement;
	    int  position;
	    int  eof = FALSE;

        ODBCDR_RDBI_ERR( odbcdr_col_act( context, owner, (char *)object, NULL ) );

        while (!eof && rdbi_status == RDBI_SUCCESS)
        {
            rdbi_status = odbcdr_col_get(
                context,
	            column_name,
	            type,
	            &length,
	            &scale,
	            &nullable,
                &is_autoincrement,
	            &position,
	            &eof
	            );

            if (!eof && rdbi_status == RDBI_SUCCESS && is_autoincrement)
            {
                odbcdr_NameListEntry_pkey_def newNle;
                (void) strcpy(newNle.name, column_name);
                if (NULL == ut_da_append( &context->odbcdr_nameList_pkeys, 1L, (void *) &newNle ))
                {
                    rdbi_status = RDBI_MALLOC_FAILED;
                    goto the_exit;
                }
                bFoundIdentityProperties = true;
            }
        }

        ODBCDR_RDBI_ERR( odbcdr_col_deac(context) );
    }

    /*********************************************************************************
     * Approach #5: Find all non-nullable columns and use them as the identity property.
     *********************************************************************************/

    if (!bFoundIdentityProperties)
    {
	    char column_name[ODBCDR_MAX_BUFF_SIZE];
	    char type[ODBCDR_MAX_BUFF_SIZE];
	    int  length;
	    int  scale;
	    int  nullable;
        int  is_autoincrement;
	    int  position;
	    int  eof = FALSE;

        ODBCDR_RDBI_ERR( odbcdr_col_act( context, owner, (char *)object, NULL ) );

        while (!eof && rdbi_status == RDBI_SUCCESS)
        {
            rdbi_status = odbcdr_col_get(
                context,
	            column_name,
	            type,
	            &length,
	            &scale,
	            &nullable,
                &is_autoincrement,
	            &position,
	            &eof
	            );

            if (!eof && rdbi_status == RDBI_SUCCESS && !nullable)
            {
                odbcdr_NameListEntry_pkey_def newNle;
                (void) strcpy(newNle.name, column_name);
                if (NULL == ut_da_append( &context->odbcdr_nameList_pkeys, 1L, (void *) &newNle ))
                {
                    rdbi_status = RDBI_MALLOC_FAILED;
                    goto the_exit;
                }
                bFoundIdentityProperties = true;
            }
        }

        ODBCDR_RDBI_ERR( odbcdr_col_deac(context) );
    }

    /*********************************************************************************/

	context->odbcdr_nameListNextPosition_pkeys = 0;

the_exit:
	if (NULL != connData && NULL != connData->keys)
    {
        /* Ideally, we would keep the cursor around, and just re-execute later.
         * However, many ODBC drivers do not support concurrent cursors, and
         * calls to the module may be interleaved with others.  To avoid an
         * "Invalid cursor state" error from SQLTables later,
         * we'll have to free the cursor here. 
         */
		ODBCDR_RDBI_ERR( odbcdr_fre_cursor(context, (char **)(&connData->keys)) );
        connData->keys = NULL;
    }
	debug_return(NULL, rdbi_status);

}


/************************************************************************
*																		*
* Name																	*
*	odbcdr_pkeys_get - Get the next key.								*
*																		*
* Synopsis																*
*	odbcdr_pkeys_get(context, name, eof)								*
*	odbcdr_context_def *context;										*
*	char *name;		        											*
*	int  *eof;															*
*																		*
* Description															*
*		This module gets the next key in  the  fetch activated		    *
*		by odbcdr_pkeys_act.											*
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

int odbcdr_pkeys_get(
    odbcdr_context_def *context,
	char *name,
	int  *eof
	)
{
  	int					rdbi_status = RDBI_GENERIC_ERROR;


	debug_on("odbcdr_pkeys_get");

	/* assume not end of fetch */
	*eof = FALSE;

    if (context->odbcdr_nameListNextPosition_pkeys < 0 || context->odbcdr_nameListNextPosition_pkeys > context->odbcdr_nameList_pkeys.size)
        goto the_exit;

    if (context->odbcdr_nameListNextPosition_pkeys == context->odbcdr_nameList_pkeys.size)
    {
        *eof = TRUE;
        context->odbcdr_nameListNextPosition_pkeys = -1;
        (void) ut_da_free(&context->odbcdr_nameList_pkeys);
    }
    else
    {
        odbcdr_NameListEntry_pkey_def * nle = (odbcdr_NameListEntry_pkey_def *) ut_da_get(&context->odbcdr_nameList_pkeys, context->odbcdr_nameListNextPosition_pkeys++);
        (void) strcpy(name, nle->name);
    }

	rdbi_status = RDBI_SUCCESS;
the_exit:
	debug_return(NULL, rdbi_status);
}


/************************************************************************
*																		*
* Name																	*
*	odbcdr_pkeys_deac - Deactivate the fetch of all keys.				*
*																		*
* Synopsis																*
*	odbcdr_pkeys_deac(context)											*
*	odbcdr_context_def *context;										*
*																		*
* Description															*
*		This  module  deactivates	the   fetch  of  all  keys 		*
*		activated by odbcdr_pkeys_act. 									*
*																		*
* Parameters															*
*																		*
* Function value														*
*		Returns an rdbi status code from inc/rdbi.h 					*
*																		*
* Remarks																*
*																		*
************************************************************************/

int odbcdr_pkeys_deac( odbcdr_context_def *context )
{
	int 				rdbi_status = RDBI_GENERIC_ERROR;

	debug_on("odbcdr_pkeys_deac");

    context->odbcdr_nameListNextPosition_pkeys = -1;
    (void) ut_da_free(&context->odbcdr_nameList_pkeys);

	rdbi_status = RDBI_SUCCESS;
	debug_return(NULL, rdbi_status);
}

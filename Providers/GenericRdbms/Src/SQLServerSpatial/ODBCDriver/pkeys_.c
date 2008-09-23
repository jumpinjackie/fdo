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

#include    <Inc/rdbi.h>
#include    <Inc/ut.h>
#include	<Inc/debugext.h>
#include    "proto_p.h"

#define _check_status  if (rdbi_status != RDBI_SUCCESS) goto the_exit;
int local_odbcdr_col_act(odbcdr_context_def *context, rdbi_string_def *owner, rdbi_string_def *object_name, rdbi_string_def *dbaselink);
int local_odbcdr_col_get(odbcdr_context_def *context, rdbi_string_def *column_name, rdbi_string_def *type, int *length,
	        int *scale,int *nullable, int *is_autoincrement, int *position, int  *eof);


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

int local_odbcdr_pkeys_act(
    odbcdr_context_def *context,
    rdbi_string_def *owner,
    rdbi_string_def *object
	)
{
    rdbi_string_def dbaselink;
    SQLWCHAR    szColumnNameBuf[ODBCDR_MAX_BUFF_SIZE];
    rdbi_string_def szColumnName;
    SQLWCHAR    szTypeNameBuf[ODBCDR_MAX_BUFF_SIZE];
    rdbi_string_def szTypeName;
	odbcdr_cursor_def	*c;
	odbcdr_connData_def	*connData = NULL;
	int 		rdbi_status = RDBI_GENERIC_ERROR;
    SQLINTEGER  cbColumnName = 0;
    SQLRETURN   ret = SQL_SUCCESS;
    long        i;
    int         found;
    int         bFoundIdentityProperties = FALSE;
    int         bDriverIsNativeOracle = FALSE;
    int         bDriverIsOracle = FALSE;
    odbcdr_NameListEntry_pkey_def * nle;
    SQLSMALLINT charType;
    szColumnName.wString = (wchar_t *)szColumnNameBuf;
    szTypeName.wString = (wchar_t *)szTypeNameBuf;
    dbaselink.wString = NULL;

	debug_on("odbcdr_pkeys_act");

    if ( ODBCDRV_STRING_EMPTY(object) )
        goto the_exit;

	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn ( context, &connData ) );

	/* Deactivate any outstanding queries */
	rdbi_status = odbcdr_pkeys_deac ( context );
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
    if (context->odbcdr_UseUnicode)
    {
        charType = SQL_C_WCHAR;
        ret = SQLPrimaryKeysW( c->hStmt, NULL, 0, NULL, 0, (SQLWCHAR*)object->cwString,  SQL_NTS);
    }
    else
    {
        charType = SQL_C_CHAR;
        ret = SQLPrimaryKeys( c->hStmt, NULL, 0, NULL, 0, (SQLCHAR*)object->ccString,  SQL_NTS);
    }

    if (ret == SQL_SUCCESS)
    {
        ODBCDR_ODBC_ERR( SQLBindCol(
            c->hStmt, 4,  charType, szColumnName.wString, ODBCDR_MAX_BUFF_SIZE, &cbColumnName),
            SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching primary keys" );

        while (ret != SQL_NO_DATA)
        {
            odbcdr_NameListEntry_pkey_def newNle;
            *szColumnName.wString = L'\0';

            ret = SQLFetch(c->hStmt);

            if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
                goto the_exit;

            if (ret == SQL_NO_DATA)
                break;

            ODBCDRV_STRING_COPY_RST(newNle.name, &szColumnName);
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
        // use szTypeName as temporary for OWM
        if (context->odbcdr_UseUnicode)
            odbcdr_swprintf(szTypeName.wString, ODBCDR_MAX_BUFF_SIZE, L"%ls%ls", object->cwString, ODBCDR_DRIVER_ORACLE_OWM_TABLE_SUFFIXW);
        else
            sprintf(szTypeName.cString, "%s%s", object->ccString, ODBCDR_DRIVER_ORACLE_OWM_TABLE_SUFFIX);

#ifndef _WIN32
        /* Easysoft's ODBC driver for Oracle doesn't like cursor re-use. */
        ODBCDR_RDBI_ERR( odbcdr_fre_cursor(context, (char **)(&connData->keys)) );
        rdbi_status = odbcdr_est_cursor( context, (char **)&c);
        _check_status;
        connData->keys = c;
#endif

        if (context->odbcdr_UseUnicode)
            ret = SQLPrimaryKeysW(c->hStmt, NULL, 0, NULL, 0, (SQLWCHAR*)szTypeName.cwString, SQL_NTS);
        else
            ret = SQLPrimaryKeys(c->hStmt, NULL, 0, NULL, 0, (SQLCHAR*)szTypeName.ccString, SQL_NTS);

        if (ret == SQL_SUCCESS)
        {
            ODBCDR_ODBC_ERR( SQLBindCol(
                c->hStmt, 4,  charType, szColumnName.wString, ODBCDR_MAX_BUFF_SIZE, &cbColumnName),
                SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching primary keys" );

            while (ret != SQL_NO_DATA) {
                *szColumnName.wString = L'\0';

                ret = SQLFetch(c->hStmt);
                if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
                goto the_exit;

                if (ret == SQL_NO_DATA)
                    break;

                // Filter out workspace-manager-specific primary key columns:
                if (0!=ODBCDRV_STRING_COMPARE_NOCASE_CST(&szColumnName, ODBCDR_DRIVER_ORACLE_OWM_COLUMN_VERSION, ODBCDR_MAX_BUFF_SIZE) &&
                    0!=ODBCDRV_STRING_COMPARE_NOCASE_CST(&szColumnName, ODBCDR_DRIVER_ORACLE_OWM_COLUMN_NEXTVER, ODBCDR_MAX_BUFF_SIZE) &&
                    0!=ODBCDRV_STRING_COMPARE_NOCASE_CST(&szColumnName, ODBCDR_DRIVER_ORACLE_OWM_COLUMN_DELSTATUS, ODBCDR_MAX_BUFF_SIZE) &&
                    0!=ODBCDRV_STRING_COMPARE_NOCASE_CST(&szColumnName, ODBCDR_DRIVER_ORACLE_OWM_COLUMN_LTLOCK, ODBCDR_MAX_BUFF_SIZE))
                {
                    odbcdr_NameListEntry_pkey_def newNle;
                    ODBCDRV_STRING_COPY_RST(newNle.name, &szColumnName);
                    if (NULL == ut_da_append( &context->odbcdr_nameList_pkeys, 1L, (void *) &newNle ))
                    {
                        rdbi_status = RDBI_MALLOC_FAILED;
                        goto the_exit;
                    }
                    bFoundIdentityProperties = true;
                }
            }
        }
        *szTypeName.wString = L'\0';
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
            ODBCDRV_STRING_COPY_CST(newNle.name, ODBCDR_DRIVER_ORACLE_ROWID_NAME);
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
            /* SQL_SCOPE_CURROW -> Ideally we would use SQL_SCOPE_SESSION, but is not always
             * a guarantee that the rowid will be immutable. */
            /* SQL_NULLABLE -> From Microsoft ODBC docs: "Some drivers cannot support SQL_NO_NULLS,
             * and these drivers will return an empty result set if SQL_NO_NULLS was
             * specified. Applications should be prepared for this case and request
             * SQL_NO_NULLS only if it is absolutely required. */
            if (context->odbcdr_UseUnicode)
                ret = SQLSpecialColumnsW(c->hStmt, SQL_BEST_ROWID, NULL, 0, NULL, 
                    0, (SQLWCHAR*)object->cwString, SQL_NTS, SQL_SCOPE_CURROW, SQL_NULLABLE);
            else
                ret = SQLSpecialColumns(c->hStmt, SQL_BEST_ROWID, NULL, 0, NULL, 
                    0, (SQLCHAR*)object->ccString, SQL_NTS, SQL_SCOPE_CURROW, SQL_NULLABLE);

            /* We do not exit with error if ret != SQL_SUCCESS because some drivers do
             * not support SQLSpecialColumns.
             */
            if (ret == SQL_SUCCESS)
            {
                SQLINTEGER  cbDataType = 0;
                SQLSMALLINT ssDataType = 0;
                SQLINTEGER  cbTypeName = 0;
                SQLINTEGER  cbColumnSize = 0;
                SQLINTEGER  iColumnSize = 0;
                SQLINTEGER  cbBufferLen = 0;
                SQLINTEGER  iBufferLen = 0;
                SQLINTEGER  cbDecimalDigits = 0;
                SQLSMALLINT iDecimalDigits = 0;
                SQLINTEGER  cbPseudoColumn = 0;
                SQLSMALLINT iPseudoColumn = 0;

                *szTypeName.wString = L'\0';

                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 2,  charType, szColumnName.wString, ODBCDR_MAX_BUFF_SIZE, &cbColumnName ),
                SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching primary keys" );

#if 0
                /* If it becomes possible (and necessary) to return completely described
                 * columns from this module, do the following extra binds.  This may become
                 * an issue for pseudo-columns, which may not be in the caller's column list.
                 * They can be detected by checking for "SQL_PC_PSEUDO == iPseudoColumn".
                 */
                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 3,  SQL_C_SSHORT, &ssDataType, 0, &cbDataType),
                SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching primary keys" );

                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 4,  charType, szTypeName.wString, ODBCDR_MAX_BUFF_SIZE, &cbTypeName),
                SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching primary keys" ); 

                 //NOTE: parameter 5 is in *bytes* not *characters*!
                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 5,  SQL_C_SLONG, &iColumnSize, 0, &cbColumnSize),
                SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching primary keys" );

                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 6,  SQL_C_SLONG, &iBufferLen, 0, &cbBufferLen),
                SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching primary keys" );

                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 7,  SQL_C_SSHORT, &iDecimalDigits, 0, &cbDecimalDigits),
                SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching primary keys" );

                ODBCDR_ODBC_ERR( SQLBindCol(
                    c->hStmt, 8,  SQL_C_SSHORT, &iPseudoColumn, 0, &cbPseudoColumn),
                SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching primary keys" );
#endif
                while (ret != SQL_NO_DATA)
                {
                    *szColumnName.wString = L'\0';

                    ret = SQLFetch(c->hStmt);
                    if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
                        goto the_exit;

                    if (ret == SQL_NO_DATA)
                        break;

                    /* Do not use Oracle's ROWID.  We currently do not return it as a
                     * column name for a table, and Schema Manager requires that a primary
                     * key appear in the column list.
                     */
                    if (ODBCDRV_STRING_COMPARE_LST(&szColumnName, ODBCDR_DRIVER_ORACLE_ROWID_NAME)==0)
                        continue;
                                                                                                     

                    /* The Microsoft Access driver sometimes returns the same column more 
                     * than once, so be careful to avoid duplicates in our collection.
                     */
                    for (i=0, found=false;  !found && i < context->odbcdr_nameList_pkeys.size;  i++)
                    {
                        nle = (odbcdr_NameListEntry_pkey_def *) ut_da_get(&context->odbcdr_nameList_pkeys, i);
                        if (ODBCDRV_STRING_COMPARE_LST(&szColumnName, nle->name) == 0)
                            found = true;
                    }

                    if (!found)
                    {
                        /* Add name to the list. */
                        odbcdr_NameListEntry_pkey_def newNle;
                        ODBCDRV_STRING_COPY_RST(newNle.name, &szColumnName);
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
	    int  length;
	    int  scale;
	    int  nullable;
        int  is_autoincrement;
	    int  position;
	    int  eof = FALSE;

        ODBCDR_RDBI_ERR( local_odbcdr_col_act( context, owner, object, &dbaselink ) );

        while (!eof && rdbi_status == RDBI_SUCCESS)
        {
            rdbi_status = local_odbcdr_col_get(
                context,
	            &szColumnName,
	            &szTypeName,
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
                ODBCDRV_STRING_COPY_RST(newNle.name, &szColumnName);
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
	    int  length;
	    int  scale;
	    int  nullable;
        int  is_autoincrement;
	    int  position;
	    int  eof = FALSE;

        ODBCDR_RDBI_ERR( local_odbcdr_col_act( context, owner, object, &dbaselink ) );

        while (!eof && rdbi_status == RDBI_SUCCESS)
        {
            rdbi_status = local_odbcdr_col_get(
                context,
	            &szColumnName,
	            &szTypeName,
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
                ODBCDRV_STRING_COPY_RST(newNle.name, &szColumnName);
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

int odbcdr_pkeys_act(
    odbcdr_context_def *context,
    const char *owner,
    const char *object
	)
{
    rdbi_string_def strOwner;
    rdbi_string_def strObject;
    strOwner.ccString = owner;
    strObject.ccString = object;
    return local_odbcdr_pkeys_act(context, &strOwner, &strObject);
}

int odbcdr_pkeys_actW(
    odbcdr_context_def *context,
    const wchar_t *owner,
    const wchar_t *object
	)
{
    rdbi_string_def strOwner;
    rdbi_string_def strObject;
    strOwner.cwString = owner;
    strObject.cwString = object;
    return local_odbcdr_pkeys_act(context, &strOwner, &strObject);
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

int local_odbcdr_pkeys_get(
    odbcdr_context_def *context,
	rdbi_string_def *name,
	int  *eof
	)
{
  	int	rdbi_status = RDBI_GENERIC_ERROR;

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
        ODBCDRV_STRING_COPY_LST(name, nle->name);
    }

	rdbi_status = RDBI_SUCCESS;
the_exit:
	debug_return(NULL, rdbi_status);
}
int odbcdr_pkeys_get(
    odbcdr_context_def *context,
	char *name,
	int  *eof
	)
{
    rdbi_string_def str;
    str.cString = name;
    return local_odbcdr_pkeys_get(context, &str, eof);
}

int odbcdr_pkeys_getW(
    odbcdr_context_def *context,
	wchar_t *name,
	int  *eof
	)
{
    rdbi_string_def str;
    str.wString = name;
    return local_odbcdr_pkeys_get(context, &str, eof);
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

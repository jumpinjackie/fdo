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
*	odbcdr_col_act - Activate a fetch of all columns for an object		*
*																		*
* Synopsis																*
*	odbcdr_col_act(owner, object_name)									*
*	char *owner;														*
*	char *object_name;													*
*																		*
* Description															*
*		This  module  activates  a	fetch  of  all columns for an		*
*		object in an SQL Server database.								*
*																		*
* Parameters															*
*	owner: input														*
*		The owner of the object.										*
*																		*
*	object_name: input													*
*		The name of the object. 										*
*																		*
* Function value														*
*		Returns an rdbi status code from inc/rdbi.h 					*
*																		*
* Remarks																*
*       This code is NOT re-entrant.  There is no handle to return      *
*       to identify a specific query of objects.                        *
*																		*
************************************************************************/

#include    <Inc/rdbi.h>
#include    <Inc/ut.h>
#include	<Inc/debugext.h>
#include    "proto_p.h"
#ifndef _WIN32
#include <sqlucode.h>
#endif


#define _check_status  if (rdbi_status != RDBI_SUCCESS) goto the_exit;

static int odbcdr_col_act_SQLColumns( 
    odbcdr_context_def *context, 
    char *owner, 
    char *object_name);

typedef struct
{
    int             odbcDataType;
    int             odbcDataTypeDateTimeSubcode;  // only used when odbcDataType is SQL_DATETIME or SQL_INTERVAL
    const char *    name;
} TypeNameMapEntry;

/* This name map is just to map SQL numbers to strings that the caller
 * can understand without including SQL header files.  The caller must
 * do the actual type mapping.  The only type mapping done here is for
 * unsupported types.
 */
TypeNameMapEntry typeNameMap_S[] =
{
    { SQL_CHAR,             -1,                  "char" },
    { SQL_VARCHAR,          -1,                  "varchar" },
    { SQL_LONGVARCHAR,      -1,                  "longvarchar" },
    { SQL_WCHAR,            -1,                  "wchar" },
    { SQL_WVARCHAR,         -1,                  "wvarchar" },
    { SQL_WLONGVARCHAR,     -1,                  "wlongvarchar" },
    { SQL_TINYINT,          -1,                  "tinyint" },
    { SQL_SMALLINT,         -1,                  "smallint" },
    { SQL_INTEGER,          -1,                  "integer" },
    { SQL_BIGINT,           -1,                  "bigint" },
    { SQL_DECIMAL,          -1,                  "decimal" },
    { SQL_NUMERIC,          -1,                  "numeric" },
    { SQL_REAL,             -1,                  "real" },
    { SQL_FLOAT,            -1,                  "float" },
    { SQL_DOUBLE,           -1,                  "double" },
    { SQL_DATETIME,         SQL_CODE_DATE,       "date"},
    { SQL_DATETIME,         SQL_CODE_TIME,       "time"},
    { SQL_DATETIME,         SQL_CODE_TIMESTAMP,  "timestamp"},
    { SQL_TYPE_DATE,        -1,                  "date" },
    { SQL_TYPE_TIME,        -1,                  "time" },
    { SQL_TYPE_TIMESTAMP,   -1,                  "timestamp" },
    { SQL_BINARY,           -1,                  "binary" },
    { SQL_VARBINARY,        -1,                  "varbinary" },
    { SQL_LONGVARBINARY,    -1,                  "longvarbinary" },
    { SQL_UNKNOWN_TYPE,     -1,                  "unknown" },
    { SQL_BIT,              -1,                  "bit" },
    { SQL_INTERVAL_MONTH,               -1,      "unsupported" },
    { SQL_INTERVAL_YEAR,                -1,      "unsupported" },
    { SQL_INTERVAL_YEAR_TO_MONTH,       -1,      "unsupported" },
    { SQL_INTERVAL_DAY,                 -1,      "unsupported" },
    { SQL_INTERVAL_HOUR,                -1,      "unsupported" },
    { SQL_INTERVAL_MINUTE,              -1,      "unsupported" },
    { SQL_INTERVAL_SECOND,              -1,      "unsupported" },
    { SQL_INTERVAL_DAY_TO_HOUR,         -1,      "unsupported" },
    { SQL_INTERVAL_DAY_TO_MINUTE,       -1,      "unsupported" },
    { SQL_INTERVAL_DAY_TO_SECOND,       -1,      "unsupported" },
    { SQL_INTERVAL_HOUR_TO_MINUTE,      -1,      "unsupported" },
    { SQL_INTERVAL_HOUR_TO_SECOND,      -1,      "unsupported" },
    { SQL_INTERVAL_MINUTE_TO_SECOND,    -1,      "unsupported" },
    { SQL_GUID,                         -1,      "unsupported" },
    { SQL_INTERVAL_MONTH,               -1,      "unsupported" },
    { SQL_INTERVAL_MONTH,               -1,      "unsupported" },
    { SQL_INTERVAL_MONTH,               -1,      "unsupported" }
};

#define ODBCDR_LONGVARCHAR_LENGTH       (1073741824)

static const char * typeNumberToName(int odbcType, int odbcTypeDateTimeSubcode)
{
    int i;
    int array_size = sizeof(typeNameMap_S) / sizeof(typeNameMap_S[0]);
    static const char * unsupported = "unsupported";
    const char * name = unsupported;

    for (i=0;  i < array_size && name != NULL;  i++)
    {
        if ( (typeNameMap_S[i].odbcDataType == odbcType)
             && (    (odbcTypeDateTimeSubcode == typeNameMap_S[i].odbcDataTypeDateTimeSubcode)
                  || ((odbcType != SQL_DATETIME) && (odbcType != SQL_INTERVAL)) ) )
            name = typeNameMap_S[i].name;
    }

    return name;
}


int odbcdr_col_act(
    odbcdr_context_def *context,
	char *owner,
	char *object_name,
    char *dbaselink         /* Not supported */
	)
{
	odbcdr_cursor_def	*c;
	odbcdr_connData_def	*connData;
	int 				rdbi_status = RDBI_GENERIC_ERROR;
	int 				owner_set = TRUE;
    char                szSql[ODBCDR_MAX_BUFF_SIZE];
	SQLSMALLINT			num_cols = 0;
    SQLSMALLINT         position = 0;
    int                 rows_processed = 0;
    SQLSMALLINT         cbColumnName = 0;
    SQLCHAR             szColumnName[ODBCDR_MAX_BUFF_SIZE];
    SQLSMALLINT         ssDataType = 0;
    SQLSMALLINT         ssDataType2 = -1;
    SQLSMALLINT         ssDataTypeDateTimeSubcode = 0;
    SQLSMALLINT         ssIsAutoIncrement = -1;
    SQLSMALLINT         ssLength = -1;
    SQLSMALLINT         ssIsNullable = -1;
    SQLSMALLINT         ssScale = -1;
    SQLSMALLINT         ssBinarySize = -1;
    SQLRETURN           ret = SQL_SUCCESS;
    long                i;
    int                 found;
    odbcdr_NameListEntry_col_def *     nle;


	debug_on2("odbcdr_col_act", "owner '%s', object_name '%s'", owner, object_name );

	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

	/* check if owner is null or empty */
	if (owner == NULL || owner[0] == '\0')
		owner_set = FALSE;

	/* Deactivate any outstanding queries */
	rdbi_status = odbcdr_col_deac( context );
	_check_status;

    if (object_name[0] == '?')
    {
        /* A '?' in a column name is illegal, but we could get this
         * if we have a name in a non-Latin character set when the OS' current
         * language is not a Latin one.  Mimic the behaviour of SQLColumns()
         * and just return zero rows.
         */
        rdbi_status = RDBI_SUCCESS;
        context->odbcdr_nameListNextPosition_cols = 0;
        goto the_exit;
    }

	c = connData->cols;

	if (c == (odbcdr_cursor_def *)NULL) {
		rdbi_status = odbcdr_est_cursor( context, (char **)&c);
		_check_status;

		connData->cols = c;
	}

    if (owner_set)
    {
        const char * formatStr = "select * from %s.\"%s\" where 1 = 2";
        if (strlen(owner) + strlen(object_name) + strlen(formatStr) >= sizeof(szSql))
            goto the_exit;
        (void) sprintf(szSql, formatStr, owner, object_name);
    }
    else
    {
        const char * formatStr = "select * from \"%s\" where 1 = 2";
        if (strlen(object_name) + strlen(formatStr) >= sizeof(szSql))
            goto the_exit;
        (void) sprintf(szSql, formatStr, object_name);
    }

    ODBCDR_RDBI_ERR( odbcdr_sql( context, (char *) c, szSql, TRUE, NULL, NULL, NULL ) );
    ODBCDR_RDBI_ERR( odbcdr_execute( context, (char *) c, 1, 0, &rows_processed ) );

    ODBCDR_ODBC_ERR( SQLNumResultCols(c->hStmt, &num_cols) , 
        SQL_HANDLE_STMT, c->hStmt,
		"SQLDescribeCol", "get columns" );

    for (position=1;  position <= num_cols;  position++)
    {
        szColumnName[0] = '\0';
        ssIsAutoIncrement = -1;
        ssDataType = -1;
        ssDataType2 = -1;
        ssDataTypeDateTimeSubcode = -1;
        ssLength = -1;
        ssIsNullable = -1;
        ssScale = -1;
        ssBinarySize = -1;

        /* We would create and populate newNle just before adding it to
         * the array, but the Easysoft driver for Oracle on Linux
         * has a habit of overwriting buffers passed to it earlier.  So,
         * we'll copy the information out immediately after each call.
         */
        odbcdr_NameListEntry_col_def newNle;
        (void) memset(&newNle, 0, sizeof(newNle));
        newNle.position = position;
        int isLongType = FALSE;  /* Some drivers won't tell size of long types (we'll hard-code it). */

        ODBCDR_ODBC_ERR( SQLColAttribute(
            c->hStmt,
            (SQLUSMALLINT) position,
            (SQLUSMALLINT) SQL_DESC_NAME, 
            szColumnName,
            sizeof(szColumnName),
            &cbColumnName, 
            NULL  ),
            SQL_HANDLE_STMT, c->hStmt, L"SQLColAttribute", L"Getting column name" );
        (void) strcpy(newNle.name, (char*)szColumnName);

        ODBCDR_ODBC_ERR( SQLColAttribute(
            c->hStmt,
            (SQLUSMALLINT) position,
            (SQLUSMALLINT) SQL_DESC_TYPE, 
            NULL,
            0,
            NULL, 
            &ssDataType  ),
            SQL_HANDLE_STMT, c->hStmt, L"SQLColAttribute", L"Getting type" );
        if (ssDataType == SQL_LONGVARCHAR)
            isLongType = TRUE;
        ssDataType2 = ssDataType;   /* Sve from overwriting by Easysoft's driver. */

        ret = SQLColAttribute(
            c->hStmt,
            (SQLUSMALLINT) position,
            (SQLUSMALLINT) SQL_DESC_DATETIME_INTERVAL_CODE, 
            NULL,
            0,
            NULL, 
            &ssDataTypeDateTimeSubcode );
        if (ret != ODBCDR_SUCCESS)
            ssDataTypeDateTimeSubcode = -1; /* Some drivers do not support this subcode. */
        (void) strcpy(newNle.type, typeNumberToName((int)ssDataType2, ssDataTypeDateTimeSubcode));

        ODBCDR_ODBC_ERR( SQLColAttribute(
            c->hStmt,
            (SQLUSMALLINT) position,
            (SQLUSMALLINT) SQL_DESC_LENGTH, 
            NULL,
            0,
            NULL, 
            &ssLength  ),
            SQL_HANDLE_STMT, c->hStmt, L"SQLColAttribute", L"Getting length" );
        newNle.length = ssLength;

        if (ssLength <= 0)
        {
            /* Numerics may return length of zero, regardless of ODBC spec.  Use binary size. */
            ODBCDR_ODBC_ERR( SQLColAttribute(
                c->hStmt,
                (SQLUSMALLINT) position,
                (SQLUSMALLINT) SQL_DESC_OCTET_LENGTH, 
                NULL,
                0,
                NULL, 
                &ssBinarySize  ),
                SQL_HANDLE_STMT, c->hStmt, L"SQLColAttribute", L"Getting binary size" );
            newNle.length = ssBinarySize;
        }
        if (newNle.length <=0 && isLongType)
        {
            // Hard-code with the answer that we would get from SQLColumns
            // (at least, from the Microsoft Excel driver).
            newNle.length = ODBCDR_LONGVARCHAR_LENGTH;
        }

        ODBCDR_ODBC_ERR( SQLColAttribute(
            c->hStmt,
            (SQLUSMALLINT) position,
            (SQLUSMALLINT) SQL_DESC_SCALE, 
            NULL,
            0,
            NULL, 
            &ssScale  ),
            SQL_HANDLE_STMT, c->hStmt, L"SQLColAttribute", L"Getting scale" );
        newNle.scale = ssScale;

        ODBCDR_ODBC_ERR( SQLColAttribute(
            c->hStmt,
            (SQLUSMALLINT) position,
            (SQLUSMALLINT) SQL_DESC_NULLABLE, 
            NULL,
            0,
            NULL, 
            &ssIsNullable  ),
            SQL_HANDLE_STMT, c->hStmt, L"SQLColAttribute", L"Getting isnullable" );
        newNle.isnullable = (ssIsNullable == SQL_NULLABLE);

        ODBCDR_ODBC_ERR( SQLColAttribute(
            c->hStmt,
            (SQLUSMALLINT) position,
            (SQLUSMALLINT) SQL_DESC_AUTO_UNIQUE_VALUE, 
            NULL,
            0,
            NULL, 
            &ssIsAutoIncrement  ),
            SQL_HANDLE_STMT, c->hStmt, L"SQLColAttribute", L"Getting isautoincrement" );
        newNle.isautogenerated = ( ssIsAutoIncrement == SQL_TRUE );

        if (strchr((const char*)newNle.name, '.') != NULL)
        {
            // Oracle allows a dot '.' character in a column name if 
            // it is double-quoted during an SQL "create table" or "alter table"
            // command.  E.g.:
            //     alter table table2 add ( "name.withdot" varchar2(40));
            // This is actually used in Oracle system tables in some
            // non-default installations (see defect 654283).  It conflicts
            // with FDO's identifier rules.  Rather than reject the entire
            // schema or table, we'll just skip the offending column.
            continue;
        }

        /* Look for the name already in the list.  Some drivers can return duplicates, especially with views. */
        for (i=0, found=false;  !found && i < context->odbcdr_nameList_cols.size;  i++)
        {
            nle = (odbcdr_NameListEntry_col_def *) ut_da_get(&context->odbcdr_nameList_cols, i);
            if (strcmp(nle->name, (char*)newNle.name) == 0)
                found = true;
        }

        if (!found)
        {
            /* Add name and other attributes to the list. */

            if (NULL == ut_da_append( &context->odbcdr_nameList_cols, 1L, (void *) &newNle ))
            {
                rdbi_status = RDBI_MALLOC_FAILED;
                goto the_exit;
            }
        }
    } /* end for each position */

    context->odbcdr_nameListNextPosition_cols = 0;

the_exit:
    if (NULL != connData && NULL != connData->cols)
    {
        /* Ideally, we would keep the cursor around, and just re-execute later.
         * However, many ODBC drivers do not support concurrent cursors, and
         * calls to the module may be interleaved with others.  To avoid an
         * "Invalid cursor state" error from SQLTables later,
         * we'll have to free the cursor here. 
         */
		ODBCDR_RDBI_ERR( odbcdr_fre_cursor(context, (char **)(&connData->cols)) );
        connData->cols = NULL;
    }
    if (rdbi_status != RDBI_SUCCESS)
    {
        /* Try the SQLColumns approach. */
        rdbi_status = odbcdr_col_act_SQLColumns(context, owner, object_name);
    }
    debug_return(NULL, rdbi_status);

}

/* This is an older version of odbcdr_col_act, which is based on
 * the ODBC function "SQLColumns".  Some drivers support this better 
 * than the new SQLColAttribute.  However, SQLColumns has the limitation
 * of not yielding the SQL_DESC_AUTO_UNIQUE_VALUE value (for
 * autoincrement/autogenerated columns).
*/
static int odbcdr_col_act_SQLColumns(
    odbcdr_context_def *context,
    char *owner,
    char *object_name
	)
{
    odbcdr_cursor_def	*c;
    odbcdr_connData_def	*connData;
    int 				rdbi_status = RDBI_GENERIC_ERROR;
    int 				owner_set = TRUE;
    SQLINTEGER          cbColumnName = 0;
    SQLCHAR             szColumnName[ODBCDR_MAX_BUFF_SIZE];
    SQLINTEGER          cbDataType = 0;
    SQLSMALLINT         ssDataType = 0;
    SQLINTEGER          cbTypeName = 0;
    SQLCHAR             szTypeName[ODBCDR_MAX_BUFF_SIZE];
    SQLINTEGER          cbColumnSize = 0;
    SQLINTEGER          iColumnSize = 0;
    SQLINTEGER          cbBufferLen = 0;
    SQLINTEGER          iBufferLen = 0;
    SQLINTEGER          cbDecimalDigits = 0;
    SQLINTEGER          iDecimalDigits = 0;
    SQLINTEGER          cbNumPrecRadix = 0;
    SQLINTEGER          iNumPrecRadix = 0;
    SQLINTEGER          cbNullable = 0;
    SQLINTEGER          iNullable = 0;
    SQLINTEGER          cbRemarks = 0;
    SQLCHAR             szRemarks[ODBCDR_MAX_BUFF_SIZE];
    SQLINTEGER          cbColumnDefault = 0;
    SQLCHAR             szColumnDefault[ODBCDR_MAX_BUFF_SIZE];
    SQLINTEGER          cbSQLDataType = 0;
    SQLSMALLINT         ssSQLDataType = 0;
    SQLINTEGER          cbOrdinalPosition = 0;
    SQLINTEGER          iOrdinalPosition = 0;
    SQLINTEGER          cbIsNullable = 0;
    SQLCHAR             szIsNullable[ODBCDR_MAX_BUFF_SIZE];
    SQLRETURN           ret = SQL_SUCCESS;
    long                i;
    int                 found;
    odbcdr_NameListEntry_col_def *     nle;


    debug_on2("odbcdr_col_act", "owner '%s', object_name '%s'", owner, object_name );

    ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

    /* check if owner is null or empty */
    if (owner == NULL || owner[0] == '\0')
    owner_set = FALSE;

    /* Deactivate any outstanding queries */
    rdbi_status = odbcdr_col_deac( context );
    _check_status;

    c = connData->cols;

    if (c == (odbcdr_cursor_def *)NULL) {
        rdbi_status = odbcdr_est_cursor( context, (char **)&c);
        _check_status;

        connData->cols = c;
    }

    ODBCDR_ODBC_ERR( SQLColumns(
        c->hStmt,
        NULL, 
        0,
        owner_set ? (SQLCHAR*)owner : NULL, 
        SQL_NTS,
        (SQLCHAR*)object_name, 
        SQL_NTS,
        NULL, 
        0  ),
        SQL_HANDLE_STMT, c->hStmt, L"SQLColumns", L"Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 4,  SQL_C_CHAR, szColumnName, sizeof(szColumnName), &cbColumnName),
        SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 5,  SQL_C_SSHORT, &ssDataType, 0, &cbDataType),
        SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 6,  SQL_C_CHAR, szTypeName, sizeof(szTypeName), &cbTypeName),
        SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 7,  SQL_C_SLONG, &iColumnSize, 0, &cbColumnSize),
        SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 8,  SQL_C_SLONG, &iBufferLen, 0, &cbBufferLen),
        SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 9,  SQL_C_SLONG, &iDecimalDigits, 0, &cbDecimalDigits),
        SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 10,  SQL_C_SLONG, &iNumPrecRadix, 0, &cbNumPrecRadix),
        SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 11,  SQL_C_SLONG, &iNullable, 0, &cbNullable),
        SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 12,  SQL_C_CHAR, szRemarks, sizeof(szRemarks), &cbRemarks),
        SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 13,  SQL_C_CHAR, szColumnDefault, sizeof(szColumnDefault), &cbColumnDefault),
        SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 14,  SQL_C_SSHORT, &ssSQLDataType, 0, &cbSQLDataType),
        SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 17,  SQL_C_SLONG, &iOrdinalPosition, 0, &cbOrdinalPosition),
        SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 18,  SQL_C_CHAR, szIsNullable, sizeof(szIsNullable), &cbIsNullable),
        SQL_HANDLE_STMT, c->hStmt, L"SQLBindCol", L"Fetching columns" );

    while (ret != SQL_NO_DATA) {
        szColumnName[0] = '\0';
        szTypeName[0] = '\0';
        szIsNullable[0] = '\0';
        szColumnDefault[0] = '\0';
        szRemarks[0] = '\0';

        ret = SQLFetch(c->hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
            goto the_exit;

        if (ret == SQL_NO_DATA)
            break;

        if (strchr((const char*)szColumnName, '.') != NULL)
        {
            // Oracle allows a dot '.' character in a column name if 
            // it is double-quoted during an SQL "create table" or "alter table"
            // command.  E.g.:
            //     alter table table2 add ( "name.withdot" varchar2(40));
            // This is actually used in Oracle system tables in some
            // non-default installations (see defect 654283).  It conflicts
            // with FDO's identifier rules.  Rather than reject the entire
            // schema or table, we'll just skip the offending column.
            continue;
        }

        /* Look for the name already in the list.  Some drivers can return duplicates, especially with views. */
        for (i=0, found=false;  !found && i < context->odbcdr_nameList_cols.size;  i++)
        {
            nle = (odbcdr_NameListEntry_col_def *) ut_da_get(&context->odbcdr_nameList_cols, i);
            if (strcmp(nle->name, (char*)szColumnName) == 0)
                found = true;
        }

        if (!found)
        {
            /* Add name and other attributes to the list. */

            odbcdr_NameListEntry_col_def newNle;

            /* Best effort to give valid result from ODBC's TWO (2) nullability flags. */
            int isReallyNullable = 
                (iNullable == SQL_NULLABLE) &&
                (_stricmp((const char*)szIsNullable, "YES")==0);

            (void) strcpy(newNle.name, (char*)szColumnName);
            (void) strcpy(newNle.type, typeNumberToName((int)ssDataType, -1));
#if 0
            /* Calling code may support "Description" attrbibute later.  Use Remarks when needed. */
            (void) strcpy(newNle.remarks, szRemarks);
            /* Calling code may support default proeprty value persistence later.  Use ColumnDefault when needed. */
            (void) strcpy(newNle.default, szColumnDefault);
#endif
            newNle.isnullable = isReallyNullable;
            newNle.length = iColumnSize;
            newNle.scale = iDecimalDigits;
            newNle.position = iOrdinalPosition;
            newNle.isautogenerated = FALSE;

            if (NULL == ut_da_append( &context->odbcdr_nameList_cols, 1L, (void *) &newNle ))
            {
                rdbi_status = RDBI_MALLOC_FAILED;
                goto the_exit;
            }
        }
    } /* end while (ret != SQL_NO_DATA) */

    context->odbcdr_nameListNextPosition_cols = 0;

the_exit:
	if (NULL != connData && NULL != connData->cols)
    {
        /* Ideally, we would keep the cursor around, and just re-execute later.
         * However, many ODBC drivers do not support concurrent cursors, and
         * calls to the module may be interleaved with others.  To avoid an
         * "Invalid cursor state" error from SQLTables later,
         * we'll have to free the cursor here. 
         */
		ODBCDR_RDBI_ERR( odbcdr_fre_cursor(context, (char **)(&connData->cols)) );
        connData->cols = NULL;
    }
    debug_return(NULL, rdbi_status);

}


/************************************************************************
*																		*
* Name																	*
*	odbcdr_col_get - Get the next column.								*
*																		*
* Synopsis																*
*   rdbi_col_get(column_name, type, length, scale,                      *
*                nullable, is_autoincrement, position,                  *
*                eof)                                                   *
*	char *column_name;													*
*	char *type; 														*
*	int  *length;														*
*	int  *scale;														*
*	int  *nullable; 													*
*   int  *is_autoincrement;                                             *
*	int  *position; 													*
*	int  *eof;															*
*																		*
* Description															*
*		This module gets the next column in  the  fetch activated		*
*		by odbcdr_col_act.												*
*																		*
* Parameters															*
*	column_name: output 												*
*		The name of the column. 										*
*																		*
*	type: output														*
*		The data type of the column.									*
*																		*
*	length: output														*
*		Column length.													*
*																		*
*	scale: output														*
*		Number	of	digits after the decimal point for data  type		*
*		NUMBER. 														*
*																		*
*	nullable: output													*
*		TRUE if the column can be NULL. 								*
*																		*
*   is_autoincrement: output                                            *
*       TRUE if the column values are autoincremented                   *
*       (i.e. a DB-supported sequence)                                  *
*                                                                       *
*	position: output													*
*		Column position.												*
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

int odbcdr_col_get(
    odbcdr_context_def *context,
	char *column_name,
	char *type,
	int  *length,
	int  *scale,
	int  *nullable,
    int  *is_autoincrement,
	int  *position,
	int  *eof
	)
{
  	int					rdbi_status = RDBI_GENERIC_ERROR;


	debug_on("odbcdr_col_get");

	/* assume not end of fetch */
	*eof = FALSE;

    if (context->odbcdr_nameListNextPosition_cols < 0 || context->odbcdr_nameListNextPosition_cols > context->odbcdr_nameList_cols.size)
        goto the_exit;

    if (context->odbcdr_nameListNextPosition_cols == context->odbcdr_nameList_cols.size)
    {
        *eof = TRUE;
        context->odbcdr_nameListNextPosition_cols = -1;
        (void) ut_da_free(&context->odbcdr_nameList_cols);
    }
    else
    {
        odbcdr_NameListEntry_col_def * nle = (odbcdr_NameListEntry_col_def *) ut_da_get(&context->odbcdr_nameList_cols, context->odbcdr_nameListNextPosition_cols++);
        (void) strcpy(column_name, nle->name);
        (void) strcpy(type, nle->type);
        *length = nle->length;
        *scale = nle->scale;
        *nullable = nle->isnullable;
        *position = nle->position;
        *is_autoincrement = nle->isautogenerated;
    }

	rdbi_status = RDBI_SUCCESS;
the_exit:
	debug_return(NULL, rdbi_status);
}


/************************************************************************
*																		*
* Name																	*
*	odbcdr_col_deac - Deactivate the fetch of all columns.				*
*																		*
* Synopsis																*
*	odbcdr_col_deac()													*
*																		*
* Description															*
*		This  module  deactivates	the   fetch  of  all  columns		*
*		activated by odbcdr_col_act. 									*
*																		*
* Parameters															*
*																		*
* Function value														*
*		Returns an rdbi status code from inc/rdbi.h 					*
*																		*
* Remarks																*
*																		*
************************************************************************/

int odbcdr_col_deac( odbcdr_context_def *context )
{
	int 				rdbi_status = RDBI_GENERIC_ERROR;

	debug_on("odbcdr_col_deac");

    context->odbcdr_nameListNextPosition_cols = -1;
    (void) ut_da_free(&context->odbcdr_nameList_cols);

	rdbi_status = RDBI_SUCCESS;
	debug_return(NULL, rdbi_status);
}

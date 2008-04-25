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
#ifndef _WIN32
#include <sqlucode.h>
#endif


#define _check_status  if (rdbi_status != RDBI_SUCCESS) goto the_exit;
#define YES_STRING  "YES"
#define YES_STRINGW L"YES"

#define AUTOGEN_STATUS_BITFLAG_SYBASE	(1<<7)


int local_odbcdr_sql(odbcdr_context_def  *context, char *cursor,
	rdbi_string_def* sql, int defer, char *verb, void *ptree, char *cursor_coc);

static int odbcdr_col_act_SQLColumns( 
    odbcdr_context_def *context, 
    rdbi_string_def *owner, 
    rdbi_string_def *object_name);

typedef struct
{
    int             odbcDataType;
    int             odbcDataTypeDateTimeSubcode;  // only used when odbcDataType is SQL_DATETIME or SQL_INTERVAL
    const char *    odbcDataTypeName;             // only for special cases
    const char *    name;
    const wchar_t*  odbcDataTypeNameW;             // only for special cases
    const wchar_t*  nameW;
} TypeNameMapEntry;

/* This name map is just to map SQL numbers to strings that the caller
 * can understand without including SQL header files.  The caller must
 * do the actual type mapping.  The only type mapping done here is for
 * unsupported types.
 */
TypeNameMapEntry typeNameMap_S[] =
{
    { SQL_CHAR,             -1,                  NULL, "char",          NULL, L"char" },
    { SQL_VARCHAR,          -1,                  NULL, "varchar",       NULL, L"varchar" },
    { SQL_LONGVARCHAR,      -1,                  NULL, "longvarchar",   NULL, L"longvarchar" },
    { SQL_WCHAR,            -1,                  NULL, "wchar",         NULL, L"wchar" },
    { SQL_WVARCHAR,         -1,                  NULL, "wvarchar",      NULL, L"wvarchar" },
    { SQL_WLONGVARCHAR,     -1,                  NULL, "wlongvarchar",  NULL, L"wlongvarchar" },
    { SQL_TINYINT,          -1,                  NULL, "tinyint",       NULL, L"tinyint" },
    { SQL_SMALLINT,         -1,                  NULL, "smallint",      NULL, L"smallint" },
    { SQL_INTEGER,          -1,                  NULL, "integer",       NULL, L"integer" },
    { SQL_BIGINT,           -1,                  NULL, "bigint",        NULL, L"bigint" },
    { SQL_DECIMAL,          -1,                  NULL, "decimal",       NULL, L"decimal" },
    { SQL_NUMERIC,          -1,                  NULL, "numeric",       NULL, L"numeric" },
    { SQL_REAL,             -1,                  NULL, "real",          NULL, L"real" },
    { SQL_FLOAT,            -1,                  NULL, "float",         NULL, L"float" },
    { SQL_DOUBLE,           -1,                  NULL, "double",        NULL, L"double" },
    { SQL_DATETIME,         SQL_CODE_DATE,       NULL, "date",          NULL, L"date"},
    { SQL_DATETIME,         SQL_CODE_TIME,       NULL, "time",          NULL, L"time"},
    { SQL_DATETIME,         SQL_CODE_TIMESTAMP,  NULL, "timestamp",     NULL, L"timestamp"},
    { SQL_TYPE_DATE,        -1,                  NULL, "date",          NULL, L"date" },
    { SQL_DATETIME,         -1,                  NULL, "datetime",      NULL, L"datetime"},
    { SQL_TYPE_TIME,        -1,                  NULL, "time",          NULL, L"time" },
    { SQL_TYPE_TIMESTAMP,   -1,                  NULL, "timestamp",     NULL, L"timestamp" },
    { SQL_BINARY,           -1,                  NULL, "binary",        NULL, L"binary" },
    { SQL_VARBINARY,        -1,                  NULL, "varbinary",     NULL, L"varbinary" },
    { SQL_LONGVARBINARY,    -1,                  "SDO_GEOMETRY", "unsupported", L"SDO_GEOMETRY", L"unsupported" },
    { SQL_LONGVARBINARY,    -1,                  NULL, "longvarbinary", NULL, L"longvarbinary" },
    { SQL_UNKNOWN_TYPE,     -1,                  NULL, "unknown",       NULL, L"unknown" },
    { SQL_BIT,              -1,                  NULL, "bit",           NULL, L"bit" },
    { SQL_INTERVAL_MONTH,               -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_YEAR,                -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_YEAR_TO_MONTH,       -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_DAY,                 -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_HOUR,                -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_MINUTE,              -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_SECOND,              -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_DAY_TO_HOUR,         -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_DAY_TO_MINUTE,       -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_DAY_TO_SECOND,       -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_HOUR_TO_MINUTE,      -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_HOUR_TO_SECOND,      -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_MINUTE_TO_SECOND,    -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_GUID,                         -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_MONTH,               -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_MONTH,               -1,      NULL, "unsupported",   NULL, L"unsupported" },
    { SQL_INTERVAL_MONTH,               -1,      NULL, "unsupported",   NULL, L"unsupported" }
};

#define ODBCDR_LONGVARCHAR_LENGTH       (1073741824)
#define ODBCDR_DECIMAL_LENGTH           (16)

static const char * typeNumberToName(odbcdr_DriverType driver_type, int odbcType, int odbcTypeDateTimeSubcode, const char * odbcTypeName)
{
    int i;
    int array_size = sizeof(typeNameMap_S) / sizeof(typeNameMap_S[0]);
    static const char * unsupported = "unsupported";
    static const char * ora_date = "date";
    const char * name = unsupported;
    int found = FALSE;

    for (i=0;  !found && i < array_size && name != NULL;  i++)
    {
        if ( (typeNameMap_S[i].odbcDataType == odbcType)
             && (    (odbcTypeDateTimeSubcode == typeNameMap_S[i].odbcDataTypeDateTimeSubcode)
                  || ((odbcType != SQL_DATETIME) && (odbcType != SQL_INTERVAL)) )
             && (    (typeNameMap_S[i].odbcDataTypeName == NULL || odbcTypeName == NULL)
                  || strcmp(typeNameMap_S[i].odbcDataTypeName, odbcTypeName) == 0) )
        {
                name = typeNameMap_S[i].name;
                if ((strcmp(name, "datetime") == 0) && 
                    (ODBCDriverType_OracleNative == driver_type ||
                    ODBCDriverType_OracleNonNative == driver_type))
                name = ora_date;
                found = TRUE;
        }
    }

    return name;
}

static const wchar_t * typeNumberToNameW(odbcdr_DriverType driver_type, int odbcType, int odbcTypeDateTimeSubcode, const wchar_t * odbcTypeName)
{
    int i;
    int array_size = sizeof(typeNameMap_S) / sizeof(typeNameMap_S[0]);
    static const wchar_t * unsupported = L"unsupported";
    static const wchar_t * ora_date = L"date";
    const wchar_t * name = unsupported;
    int found = FALSE;

    for (i=0;  !found && i < array_size && name != NULL;  i++)
    {
        if ( (typeNameMap_S[i].odbcDataType == odbcType)
             && (    (odbcTypeDateTimeSubcode == typeNameMap_S[i].odbcDataTypeDateTimeSubcode)
                  || ((odbcType != SQL_DATETIME) && (odbcType != SQL_INTERVAL)) )
             && (    (typeNameMap_S[i].odbcDataTypeNameW == NULL || odbcTypeName == NULL)
                  || wcscmp(typeNameMap_S[i].odbcDataTypeNameW, odbcTypeName) == 0) )
        {
                name = typeNameMap_S[i].nameW;
                if ((wcscmp(name, L"datetime") == 0) && 
                    (ODBCDriverType_OracleNative == driver_type ||
                    ODBCDriverType_OracleNonNative == driver_type))
                name = ora_date;
                found = TRUE;
        }
    }

    return name;
}

/************************************************************************
*																		*
* Name																	*
*	odbcdr_col_act - Activate a fetch of all columns for an object		*
*																		*
* Synopsis																*
*	odbcdr_col_act(owner, object_name)									*
*	const char *owner;													*
*	const char *object_name;											*
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

int local_odbcdr_col_act(
    odbcdr_context_def *context,
	rdbi_string_def *owner,
	rdbi_string_def *object_name,
    rdbi_string_def *dbaselink         /* Not supported */
	)
{
	odbcdr_cursor_def	*c;
	odbcdr_cursor_def	*sybase_columns_cursor = NULL;
	odbcdr_connData_def	*connData;
	int 				rdbi_status = RDBI_GENERIC_ERROR;
	int 				owner_set = TRUE;
    wchar_t             szSqlBuf[ODBCDR_MAX_BUFF_SIZE];
    rdbi_string_def     szSql;
	SQLSMALLINT			num_cols = 0;
    SQLSMALLINT         position = 0;
    int                 rows_processed = 0;
    SQLSMALLINT         cbColumnName = 0;
    SQLWCHAR            szColumnNameBuf[ODBCDR_MAX_BUFF_SIZE];
    rdbi_string_def     szColumnName;
    SQLLEN              ssDataType = 0;
    SQLLEN              ssDataType2 = -1;
    SQLLEN              ssDataTypeDateTimeSubcode = 0;
    SQLLEN              ssIsAutoIncrement = -1;
    SQLLEN              ssLength = -1;
    SQLLEN              ssIsNullable = -1;
    SQLLEN              ssScale = -1;
    SQLLEN              ssBinarySize = -1;
    SQLINTEGER          ssSybaseTableId = -1;
    SQLINTEGER          ssSybaseColumnStatus = -1;
    SQLRETURN           ret = SQL_SUCCESS;
    long                i;
    int                 found;
    odbcdr_NameListEntry_col_def *     nle;
    szSql.wString = (wchar_t *)szSqlBuf;
    szColumnName.wString = (wchar_t *)szColumnNameBuf;

#ifdef _DEBUG
    if (context->odbcdr_UseUnicode){
        debug_on2("odbcdr_col_act", "owner '%ls', object_name '%ls'", owner->cwString, object_name->cwString );
    }else{
        debug_on2("odbcdr_col_act", "owner '%s', object_name '%s'", owner->ccString, object_name->ccString );
    }
#endif

	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

	/* check if owner is null or empty */
	if (ODBCDRV_STRING_EMPTY(owner))
		owner_set = FALSE;

	/* Deactivate any outstanding queries */
	rdbi_status = odbcdr_col_deac( context );
	_check_status;

    if (object_name->cwString != NULL && (context->odbcdr_UseUnicode ? (*object_name->cwString == L'?') : (*object_name->ccString == '?')))
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

    if (context->odbcdr_UseUnicode)
    {
        if (owner_set)
        {
            const wchar_t * formatStr = (ODBCDriverType_Sybase == connData->driver_type) ?
                L"select * from %ls.%ls where 1 = 2" :
                L"select * from %ls.\"%ls\" where 1 = 2";
            if (wcslen(owner->cwString) + wcslen(object_name->cwString) + wcslen(formatStr) >= ODBCDR_MAX_BUFF_SIZE)
                goto the_exit;
            (void) odbcdr_swprintf(szSql.wString, ODBCDR_MAX_BUFF_SIZE, formatStr, owner->cwString, object_name->cwString);
        }
        else
        {
            const wchar_t * formatStr = (ODBCDriverType_Sybase == connData->driver_type) ?
                L"select * from %ls where 1 = 2" :
                L"select * from \"%ls\" where 1 = 2";
            if (wcslen(object_name->cwString) + wcslen(formatStr) >= ODBCDR_MAX_BUFF_SIZE)
                goto the_exit;
            (void) odbcdr_swprintf(szSql.wString, ODBCDR_MAX_BUFF_SIZE, formatStr, object_name->cwString);
        }
    }
    else
    {
        if (owner_set)
        {
            const char * formatStr = (ODBCDriverType_Sybase == connData->driver_type) ?
               "select * from %s.%s where 1 = 2" :
               "select * from %s.\"%s\" where 1 = 2";
            if (strlen(owner->ccString) + strlen(object_name->ccString) + strlen(formatStr) >= ODBCDR_MAX_BUFF_SIZE)
                goto the_exit;
            (void) sprintf(szSql.cString, formatStr, owner->ccString, object_name->ccString);
        }
        else
        {
            const char * formatStr = (ODBCDriverType_Sybase == connData->driver_type) ?
                "select * from %s where 1 = 2" :
                "select * from \"%s\" where 1 = 2";
            if (strlen(object_name->ccString) + strlen(formatStr) >= ODBCDR_MAX_BUFF_SIZE)
                goto the_exit;
            (void) sprintf(szSql.cString, formatStr, object_name->ccString);
        }
    }
    ODBCDR_RDBI_ERR( local_odbcdr_sql( context, (char *) c, &szSql, TRUE, NULL, NULL, NULL ) );
    ODBCDR_RDBI_ERR( odbcdr_execute( context, (char *) c, 1, 0, &rows_processed ) );

    ODBCDR_ODBC_ERR( SQLNumResultCols(c->hStmt, &num_cols) , 
        SQL_HANDLE_STMT, c->hStmt,
		"SQLDescribeCol", "get columns" );

    for (position=1;  position <= num_cols;  position++)
    {
        *szColumnName.wString = L'\0';
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

        if (context->odbcdr_UseUnicode)
        {
            ODBCDR_ODBC_ERR( SQLColAttributeW(
                c->hStmt,
                (SQLUSMALLINT) position,
                (SQLUSMALLINT) SQL_DESC_NAME, 
                szColumnName.wString,
                ODBCDR_MAX_BUFF_SIZE,
                &cbColumnName, 
                NULL  ),
                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting column name" );
            (void) wcscpy(newNle.nameW, szColumnName.cwString);

            ODBCDR_ODBC_ERR( SQLColAttributeW(
                c->hStmt,
                (SQLUSMALLINT) position,
                (SQLUSMALLINT) SQL_DESC_TYPE, 
                NULL,
                0,
                NULL, 
                &ssDataType  ),
                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting type" );
            if (ssDataType == SQL_LONGVARCHAR || ssDataType == SQL_WLONGVARCHAR)
                isLongType = TRUE;
            ssDataType2 = ssDataType;   /* Save from overwriting by Easysoft's driver. */

            ret = SQLColAttributeW(
                c->hStmt,
                (SQLUSMALLINT) position,
                (SQLUSMALLINT) SQL_DESC_DATETIME_INTERVAL_CODE, 
                NULL,
                0,
                NULL, 
                &ssDataTypeDateTimeSubcode );
            if (ret != ODBCDR_SUCCESS)
                ssDataTypeDateTimeSubcode = -1; /* Some drivers do not support this subcode. */
            (void) wcscpy(newNle.typeW, typeNumberToNameW(connData->driver_type, (int)ssDataType2, (int)ssDataTypeDateTimeSubcode, NULL));

            ODBCDR_ODBC_ERR( SQLColAttributeW(
                c->hStmt,
                (SQLUSMALLINT) position,
                (SQLUSMALLINT) SQL_DESC_LENGTH, 
                NULL,
                0,
                NULL, 
                &ssLength  ),
                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting length" );
            newNle.length = (int)ssLength;

            if (ssLength <= 0)
            {
                /* Numerics may return length of zero, regardless of ODBC spec.  Use binary size. */
                ODBCDR_ODBC_ERR( SQLColAttributeW(
                    c->hStmt,
                    (SQLUSMALLINT) position,
                    (SQLUSMALLINT) SQL_DESC_OCTET_LENGTH, 
                    NULL,
                    0,
                    NULL, 
                    &ssBinarySize  ),
                    SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting binary size" );
                newNle.length = (int)ssBinarySize;
            }
            if (newNle.length <=0 && isLongType)
            {
                // Hard-code with the answer that we would get from SQLColumns
                // (at least, from the Microsoft Excel driver).
                newNle.length = ODBCDR_LONGVARCHAR_LENGTH;
            }

            ODBCDR_ODBC_ERR( SQLColAttributeW(
                c->hStmt,
                (SQLUSMALLINT) position,
                (SQLUSMALLINT) SQL_DESC_SCALE, 
                NULL,
                0,
                NULL, 
                &ssScale  ),
                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting scale" );
            newNle.scale = (int)ssScale;

            if (newNle.length <=0 && ssDataType2 == SQL_DECIMAL)
            {
                // Some drivers' SQLColAttribute do not give size of numerics.
                // Force an error so that we'll fall back to the SQLColumns method.
                rdbi_status = RDBI_GENERIC_ERROR;
                goto the_exit;
            }

            ODBCDR_ODBC_ERR( SQLColAttributeW(
                c->hStmt,
                (SQLUSMALLINT) position,
                (SQLUSMALLINT) SQL_DESC_NULLABLE, 
                NULL,
                0,
                NULL, 
                &ssIsNullable  ),
                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting isnullable" );
            newNle.isnullable = (ssIsNullable == SQL_NULLABLE);

            ODBCDR_ODBC_ERR( SQLColAttributeW(
                c->hStmt,
                (SQLUSMALLINT) position,
                (SQLUSMALLINT) SQL_DESC_AUTO_UNIQUE_VALUE, 
                NULL,
                0,
                NULL, 
                &ssIsAutoIncrement  ),
                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting isautoincrement" );
            newNle.isautogenerated = ( ssIsAutoIncrement == SQL_TRUE );
            if (wcschr(newNle.nameW, L'.') != NULL)
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
        }
        else
        {
            ODBCDR_ODBC_ERR( SQLColAttribute(
                c->hStmt,
                (SQLUSMALLINT) position,
                (SQLUSMALLINT) SQL_DESC_NAME, 
                szColumnName.cString,
                ODBCDR_MAX_BUFF_SIZE,
                &cbColumnName, 
                NULL  ),
                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting column name" );
            (void) strcpy(newNle.name, szColumnName.ccString);

            ODBCDR_ODBC_ERR( SQLColAttribute(
                c->hStmt,
                (SQLUSMALLINT) position,
                (SQLUSMALLINT) SQL_DESC_TYPE, 
                NULL,
                0,
                NULL, 
                &ssDataType),
                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting type" );
            if (ssDataType == SQL_LONGVARCHAR || ssDataType == SQL_WLONGVARCHAR)
                isLongType = TRUE;
            ssDataType2 = ssDataType;   /* Save from overwriting by Easysoft's driver. */

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
            (void) strcpy(newNle.type, typeNumberToName(connData->driver_type,(int)ssDataType2, (int)ssDataTypeDateTimeSubcode, NULL));

            ODBCDR_ODBC_ERR( SQLColAttribute(
                c->hStmt,
                (SQLUSMALLINT) position,
                (SQLUSMALLINT) SQL_DESC_LENGTH, 
                NULL,
                0,
                NULL, 
                &ssLength  ),
                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting length" );
            newNle.length = (int)ssLength;

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
                    &ssBinarySize),
                    SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting binary size" );
                newNle.length = (int)ssBinarySize;
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
                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting scale" );
            newNle.scale = (int)ssScale;

            if (newNle.length <=0 && ssDataType2 == SQL_DECIMAL)
            {
                // Some drivers' SQLColAttribute do not give size of numerics.
                // Force an error so that we'll fall back to the SQLColumns method.
                rdbi_status = RDBI_GENERIC_ERROR;
                goto the_exit;
            }

            ODBCDR_ODBC_ERR( SQLColAttribute(
                c->hStmt,
                (SQLUSMALLINT) position,
                (SQLUSMALLINT) SQL_DESC_NULLABLE, 
                NULL,
                0,
                NULL, 
                &ssIsNullable  ),
                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting isnullable" );
            newNle.isnullable = (ssIsNullable == SQL_NULLABLE);

            ODBCDR_ODBC_ERR( SQLColAttribute(
                c->hStmt,
                (SQLUSMALLINT) position,
                (SQLUSMALLINT) SQL_DESC_AUTO_UNIQUE_VALUE, 
                NULL,
                0,
                NULL, 
                &ssIsAutoIncrement  ),
                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting isautoincrement" );
            newNle.isautogenerated = ( ssIsAutoIncrement == SQL_TRUE );
            if (strchr(newNle.name, '.') != NULL)
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
        }

        /* Look for the name already in the list.  Some drivers can return duplicates, especially with views. */
        for (i=0, found=false;  !found && i < context->odbcdr_nameList_cols.size;  i++)
        {
            nle = (odbcdr_NameListEntry_col_def *) ut_da_get(&context->odbcdr_nameList_cols, i);
            if (ODBCDRV_STRING_COMPARE_CST(nle->name, newNle.name) == 0)
                found = true;
        }

        if (!found)
        {
            /* Sybase 12.5.4 and 15.0 have defects where they never report auto-generated
             * column values.  So, if we ever get a column reported this way, it means
             * that the defect is fixed and our local workaround is not required.
             */
            if (ODBCDriverType_Sybase == connData->driver_type && newNle.isautogenerated)
                connData->skip_sybase_autogen_id_workaround = TRUE;

            /* Given the table name, find the numeric Sybase table ID.
             */
            if (ODBCDriverType_Sybase == connData->driver_type &&
                !connData->skip_sybase_autogen_id_workaround &&
                -1 == ssSybaseTableId)
            {
                SQLLEN          dummy = 0;
	            if (sybase_columns_cursor == (odbcdr_cursor_def *)NULL)
                {
            		rdbi_status = odbcdr_est_cursor( context, (char **)&sybase_columns_cursor);
            		_check_status;
                }
                if (context->odbcdr_UseUnicode)
                {
                    const wchar_t * formatStr = L"select id from sysobjects where name = '%ls'";
                    if (wcslen(object_name->cwString) + wcslen(formatStr) >= ODBCDR_MAX_BUFF_SIZE)
                        goto the_exit;
                    (void) odbcdr_swprintf(szSql.wString, ODBCDR_MAX_BUFF_SIZE, formatStr, object_name->cwString);
                }
                else
                {
                    const char * formatStr = "select id from sysobjects where name = '%s'";
                    if (strlen(object_name->ccString) + strlen(formatStr) >= ODBCDR_MAX_BUFF_SIZE)
                        goto the_exit;
                    (void) sprintf(szSql.cString, formatStr, object_name->ccString);
                }
                ODBCDR_RDBI_ERR( local_odbcdr_sql( context, (char *) sybase_columns_cursor, &szSql, TRUE, NULL, NULL, NULL ) );
                ODBCDR_RDBI_ERR( odbcdr_execute( context, (char *) sybase_columns_cursor, 1, 0, &rows_processed ) );
                ODBCDR_ODBC_ERR( SQLBindCol(
                    sybase_columns_cursor->hStmt, 1,  SQL_C_SLONG, &ssSybaseTableId, 0, &dummy),
                    SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );
                ret = SQLFetch(sybase_columns_cursor->hStmt);
                if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
                    goto the_exit;
                ODBCDR_RDBI_ERR( odbcdr_fre_cursor(context, (char **)(&sybase_columns_cursor)) );
            }

            /* Given the table ID, find the Sybase auto-generated flag.
             */
            if (ODBCDriverType_Sybase == connData->driver_type &&
                !connData->skip_sybase_autogen_id_workaround)
            {
                SQLLEN          dummy = 0;
	            if (sybase_columns_cursor == (odbcdr_cursor_def *)NULL)
                {
            		rdbi_status = odbcdr_est_cursor( context, (char **)&sybase_columns_cursor);
            		_check_status;
                }
                if (context->odbcdr_UseUnicode)
                {
                    const wchar_t * formatStr = L"select status from syscolumns where id = %d and name = '%ls'";
                    (void) odbcdr_swprintf(szSql.wString, ODBCDR_MAX_BUFF_SIZE, formatStr, ssSybaseTableId, newNle.name);
                }
                else
                {
                    const char * formatStr = "select status from syscolumns where id = %d and name = '%s'";
                    (void) sprintf(szSql.cString, formatStr, ssSybaseTableId, newNle.name);
                }
                ODBCDR_RDBI_ERR( local_odbcdr_sql( context, (char *) sybase_columns_cursor, &szSql, TRUE, NULL, NULL, NULL ) );
                ODBCDR_RDBI_ERR( odbcdr_execute( context, (char *) sybase_columns_cursor, 1, 0, &rows_processed ) );
                ODBCDR_ODBC_ERR( SQLBindCol(
                    sybase_columns_cursor->hStmt, 1,  SQL_C_SLONG, &ssSybaseColumnStatus, 0, &dummy),
                    SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );
                ssSybaseColumnStatus = 0;
                ret = SQLFetch(sybase_columns_cursor->hStmt);
                if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
                    goto the_exit;
                ODBCDR_RDBI_ERR( odbcdr_fre_cursor(context, (char **)(&sybase_columns_cursor)) );
                if ((ssSybaseColumnStatus & AUTOGEN_STATUS_BITFLAG_SYBASE) != 0)
                    newNle.isautogenerated = TRUE;
            }

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
    if (rdbi_status != RDBI_SUCCESS || context->odbcdr_last_rc != ODBCDR_SUCCESS)
    {
        /* Try the SQLColumns approach. */
        rdbi_status = odbcdr_col_act_SQLColumns(context, owner, object_name);
    }
    debug_return(NULL, rdbi_status);
}

int odbcdr_col_act(
    odbcdr_context_def *context,
	const char *owner,
	const char *object_name,
    const char *dbaselink         /* Not supported */
	)
{
    rdbi_string_def strOwner;
    rdbi_string_def strObject_name;
    rdbi_string_def strDbaselink;
    strOwner.ccString = owner;
    strObject_name.ccString = object_name;
    strDbaselink.ccString = dbaselink;
    return local_odbcdr_col_act(context, &strOwner, &strObject_name, &strDbaselink);
}

int odbcdr_col_actW(
    odbcdr_context_def *context,
	const wchar_t *owner,
	const wchar_t *object_name,
    const wchar_t *dbaselink         /* Not supported */
	)
{
    rdbi_string_def strOwner;
    rdbi_string_def strObject_name;
    rdbi_string_def strDbaselink;
    strOwner.cwString = owner;
    strObject_name.cwString = object_name;
    strDbaselink.cwString = dbaselink;
    return local_odbcdr_col_act(context, &strOwner, &strObject_name, &strDbaselink);
}


/* This is an older version of odbcdr_col_act, which is based on
 * the ODBC function "SQLColumns".  Some drivers support this better 
 * than the new SQLColAttribute.  However, SQLColumns has the limitation
 * of not yielding the SQL_DESC_AUTO_UNIQUE_VALUE value (for
 * autoincrement/autogenerated columns).
*/

static int odbcdr_col_act_SQLColumns(
    odbcdr_context_def *context,
    rdbi_string_def *owner,
    rdbi_string_def *object_name
	)
{
    odbcdr_cursor_def	*c;
    odbcdr_connData_def	*connData;
    int 				rdbi_status = RDBI_GENERIC_ERROR;
    int 				owner_set = TRUE;
    SQLLEN              cbColumnName = 0;
    SQLWCHAR            szColumnNameBuf[ODBCDR_MAX_BUFF_SIZE];
    rdbi_string_def     szColumnName;
    SQLLEN              cbDataType = 0;
    SQLSMALLINT         ssDataType = 0;
    SQLLEN              cbTypeName = 0;
    SQLWCHAR            szTypeNameBuf[ODBCDR_MAX_BUFF_SIZE];
    rdbi_string_def     szTypeName;
    SQLLEN              cbColumnSize = 0;
    SQLINTEGER          iColumnSize = 0;
    SQLLEN              cbBufferLen = 0;
    SQLINTEGER          iBufferLen = 0;
    SQLLEN              cbDecimalDigits = 0;
    SQLINTEGER          iDecimalDigits = 0;
    SQLLEN              cbNumPrecRadix = 0;
    SQLINTEGER          iNumPrecRadix = 0;
    SQLLEN              cbNullable = 0;
    SQLINTEGER          iNullable = 0;
    SQLLEN              cbRemarks = 0;
    SQLWCHAR            szRemarksBuf[ODBCDR_MAX_BUFF_SIZE];
    rdbi_string_def     szRemarks;
    SQLLEN              cbColumnDefault = 0;
    SQLWCHAR            szColumnDefaultBuf[ODBCDR_MAX_BUFF_SIZE];
    rdbi_string_def     szColumnDefault;
    SQLLEN              cbSQLDataType = 0;
    SQLSMALLINT         ssSQLDataType = 0;
    SQLLEN              cbOrdinalPosition = 0;
    SQLINTEGER          iOrdinalPosition = 0;
    SQLLEN              cbIsNullable = 0;
    SQLWCHAR            szIsNullableBuf[ODBCDR_MAX_BUFF_SIZE];
    rdbi_string_def     szIsNullable;
    SQLRETURN           ret = SQL_SUCCESS;
    long                i;
    int                 found;
    odbcdr_NameListEntry_col_def *     nle;
    SQLSMALLINT charType;
    szColumnName.wString = (wchar_t *)szColumnNameBuf;
    szTypeName.wString = (wchar_t *)szTypeNameBuf;
    szRemarks.wString = (wchar_t *)szRemarksBuf;
    szColumnDefault.wString = (wchar_t *)szColumnDefaultBuf;
    szIsNullable.wString = (wchar_t *)szIsNullableBuf;

#ifdef _DEBUG
    if (context->odbcdr_UseUnicode){
        debug_on2("odbcdr_col_act", "owner '%ls', object_name '%ls'", owner->cwString, object_name->cwString );
    }else{
        debug_on2("odbcdr_col_act", "owner '%s', object_name '%s'", owner->ccString, object_name->ccString );
    }
#endif

    ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

    /* check if owner is null or empty */
    if (ODBCDRV_STRING_EMPTY(owner))
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

    if (context->odbcdr_UseUnicode)
    {
        charType = SQL_C_WCHAR;
        ODBCDR_ODBC_ERR( SQLColumnsW( c->hStmt, NULL, 0, owner_set ? (SQLWCHAR*)owner->cwString : NULL, 
            SQL_NTS, (SQLWCHAR*)object_name->cwString, SQL_NTS, NULL, 0 ),
            SQL_HANDLE_STMT, c->hStmt, "SQLColumns", "Fetching columns" );
    }
    else
    {
        charType = SQL_C_CHAR;
        ODBCDR_ODBC_ERR( SQLColumns( c->hStmt, NULL, 0, owner_set ? (SQLCHAR*)owner->ccString : NULL, 
            SQL_NTS, (SQLCHAR*)object_name->ccString, SQL_NTS, NULL, 0 ),
            SQL_HANDLE_STMT, c->hStmt, "SQLColumns", "Fetching columns" );
    }
    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 4,  charType, szColumnName.wString, ODBCDR_MAX_BUFF_SIZE, &cbColumnName),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 5,  SQL_C_SSHORT, &ssDataType, 0, &cbDataType),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 6,  charType, szTypeName.wString, ODBCDR_MAX_BUFF_SIZE, &cbTypeName),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 7,  SQL_C_SLONG, &iColumnSize, 0, &cbColumnSize),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 8,  SQL_C_SLONG, &iBufferLen, 0, &cbBufferLen),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 9,  SQL_C_SLONG, &iDecimalDigits, 0, &cbDecimalDigits),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 10,  SQL_C_SLONG, &iNumPrecRadix, 0, &cbNumPrecRadix),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 11,  SQL_C_SLONG, &iNullable, 0, &cbNullable),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 12,  charType, szRemarks.wString, ODBCDR_MAX_BUFF_SIZE, &cbRemarks),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 13,  charType, szColumnDefault.wString, ODBCDR_MAX_BUFF_SIZE, &cbColumnDefault),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 14,  SQL_C_SSHORT, &ssSQLDataType, 0, &cbSQLDataType),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 17,  SQL_C_SLONG, &iOrdinalPosition, 0, &cbOrdinalPosition),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt, 18,  charType, szIsNullable.wString, ODBCDR_MAX_BUFF_SIZE, &cbIsNullable),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching columns" );

    while (ret != SQL_NO_DATA) {
        *szColumnName.wString = L'\0';
        *szTypeName.wString = L'\0';
        *szIsNullable.wString = L'\0';
        *szColumnDefault.wString = L'\0';
        *szRemarks.wString = L'\0';

        ret = SQLFetch(c->hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
            goto the_exit;

        if (ret == SQL_NO_DATA)
            break;

        // Oracle allows a dot '.' character in a column name if 
        // it is double-quoted during an SQL "create table" or "alter table"
        // command.  E.g.:
        //     alter table table2 add ( "name.withdot" varchar2(40));
        // This is actually used in Oracle system tables in some
        // non-default installations (see defect 654283).  It conflicts
        // with FDO's identifier rules.  Rather than reject the entire
        // schema or table, we'll just skip the offending column.
        if (context->odbcdr_UseUnicode){
            if (wcschr(szColumnName.cwString, L'.') != NULL)
                continue;
        }else{
            if (strchr(szColumnName.ccString, '.') != NULL)
                continue;
        }

        /* Look for the name already in the list.  Some drivers can return duplicates, especially with views. */
        for (i=0, found=false;  !found && i < context->odbcdr_nameList_cols.size;  i++)
        {
            nle = (odbcdr_NameListEntry_col_def *) ut_da_get(&context->odbcdr_nameList_cols, i);
            if (ODBCDRV_STRING_COMPARE_LST(&szColumnName, nle->name) == 0)
                found = true;
        }

        if (!found)
        {
            /* Add name and other attributes to the list. */

            odbcdr_NameListEntry_col_def newNle;

            /* Best effort to give valid result from ODBC's TWO (2) nullability flags. */
            int isReallyNullable = (iNullable == SQL_NULLABLE) && (ODBCDRV_STRING_COMPARE_NOCASE_CST(&szIsNullable, YES_STRING, 4)==0);
            
            if (context->odbcdr_UseUnicode)
            {
                wcscpy(newNle.nameW, szColumnName.cwString);
                wcscpy(newNle.typeW, typeNumberToNameW(connData->driver_type, (int)ssDataType, -1, szTypeName.cwString));
            }
            else
            {
                strcpy(newNle.name, szColumnName.ccString);
                strcpy(newNle.type, typeNumberToName(connData->driver_type, (int)ssDataType, -1, szTypeName.ccString));
            }

#if 0
            /* Calling code may support "Description" attrbibute later.  Use Remarks when needed. */
            ODBCDRV_STRING_COPY_RST(newNle.remarks, &szRemarks);
            /* Calling code may support default proeprty value persistence later.  Use ColumnDefault when needed. */
            ODBCDRV_STRING_COPY_RST(newNle.default, &szColumnDefault);
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
*	rdbi_string_def *column_name;										*
*	rdbi_string_def *type; 												*
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
int local_odbcdr_col_get(
    odbcdr_context_def *context,
	rdbi_string_def *column_name,
	rdbi_string_def *type,
	int  *length,
	int  *scale,
	int  *nullable,
    int  *is_autoincrement,
	int  *position,
	int  *eof
	)
{
  	int rdbi_status = RDBI_GENERIC_ERROR;

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
        ODBCDRV_STRING_COPY_LST(column_name, nle->name);
        ODBCDRV_STRING_COPY_LST(type, nle->type);
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
    rdbi_string_def strcColumn_name;
    rdbi_string_def strType;
    strcColumn_name.cString = column_name;
    strType.cString = type;
    return local_odbcdr_col_get(context, &strcColumn_name, &strType, length, scale, nullable, is_autoincrement, position, eof);
}

int odbcdr_col_getW(
    odbcdr_context_def *context,
	wchar_t *column_name,
	wchar_t *type,
	int  *length,
	int  *scale,
	int  *nullable,
    int  *is_autoincrement,
	int  *position,
	int  *eof
	)
{
    rdbi_string_def strcColumn_name;
    rdbi_string_def strType;
    strcColumn_name.wString = column_name;
    strType.wString = type;
    return local_odbcdr_col_get(context, &strcColumn_name, &strType, length, scale, nullable, is_autoincrement, position, eof);
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

// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef __FDOODBCDRIVERSQL
#define __FDOODBCDRIVERSQL

#include "ODBCDriver.h"

#ifdef __cplusplus
extern "C" { 
#endif  

//////////////////////////////////////////////////////////////////////
// ODBC Driver Setup DLL functions
//////////////////////////////////////////////////////////////////////

BOOL INSTAPI ConfigDriver(
    HWND            hwndParent, 
    WORD            fRequest, 
    LPCSTR          lpszDriver,
    LPCSTR          lpszArgs, 
    LPSTR           lpszMsg, 
    WORD            cbMsgMax, 
    WORD 	        *pcbMsgOut);

BOOL INSTAPI ConfigDriverW(
    HWND            hwndParent, 
    WORD            fRequest, 
    LPCWSTR         lpszDriver,
    LPCWSTR         lpszArgs, 
    LPWSTR          lpszMsg, 
    WORD            cbMsgMax, 
    WORD 	        *pcbMsgOut);

BOOL INSTAPI ConfigDSN(
    HWND	        hwndParent,
    WORD	        fRequest,
    LPCSTR	        lpszDriver,
    LPCSTR	        lpszAttributes);

BOOL INSTAPI ConfigDSNW(
    HWND	        hwndParent,
    WORD	        fRequest,
    LPCWSTR	        lpszDriver,
    LPCWSTR	        lpszAttributes);

    
//////////////////////////////////////////////////////////////////////
// ODBC Handles
//////////////////////////////////////////////////////////////////////

SQLRETURN SQL_API SQLAllocHandle(
    SQLSMALLINT     HandleType,
    SQLHANDLE       InputHandle, 
    SQLHANDLE       *OutputHandle);

SQLRETURN SQL_API SQLFreeHandle(
    SQLSMALLINT     HandleType, 
    SQLHANDLE       Handle);

SQLRETURN SQL_API SQLFreeStmt(
    SQLHSTMT        StatementHandle,
    SQLUSMALLINT    Option);


//////////////////////////////////////////////////////////////////////
// ODBC Connection
//////////////////////////////////////////////////////////////////////

SQLRETURN SQL_API SQLConnect(
    SQLHDBC         hdbc,
    SQLCHAR         *szDSN,
    SQLSMALLINT     cbDSN,
    SQLCHAR         *szUID,
    SQLSMALLINT     cbUID,
    SQLCHAR         *szAuthStr,
    SQLSMALLINT     cbAuthStr);

SQLRETURN SQL_API SQLConnectW(
    SQLHDBC         hdbc,
    SQLWCHAR        *szDSN,
    SQLSMALLINT     cbDSN,
    SQLWCHAR        *szUID,
    SQLSMALLINT     cbUID,
    SQLWCHAR        *szAuthStr,
    SQLSMALLINT     cbAuthStr);

SQLRETURN SQL_API SQLDriverConnect(
    SQLHDBC         hdbc,
    SQLHWND         hwnd,
    SQLCHAR         *szConnStrIn,
    SQLSMALLINT     cbConnStrIn,
    SQLCHAR         *szConnStrOut,
    SQLSMALLINT     cbConnStrOutMax,
    SQLSMALLINT     *pcbConnStrOut,
    SQLUSMALLINT    fDriverCompletion);

SQLRETURN SQL_API SQLDriverConnectW(
    SQLHDBC         hdbc,
    SQLHWND         hwnd,
    SQLWCHAR        *szConnStrIn,
    SQLSMALLINT     cbConnStrIn,
    SQLWCHAR        *szConnStrOut,
    SQLSMALLINT     cbConnStrOutMax,
    SQLSMALLINT     *pcbConnStrOut,
    SQLUSMALLINT    fDriverCompletion);

SQLRETURN SQL_API SQLGetConnectAttr(
    SQLHDBC         hdbc,
    SQLINTEGER      fAttribute,
    SQLPOINTER      rgbValue,
    SQLINTEGER      cbValueMax,
    SQLINTEGER      *pcbValue);

SQLRETURN SQL_API SQLSetConnectAttr(
    SQLHDBC         hdbc,
    SQLINTEGER      fAttribute,
    SQLPOINTER      rgbValue,
    SQLINTEGER      cbValue);

//////////////////////////////////////////////////////////////////////
// ODBC
//////////////////////////////////////////////////////////////////////


/*

ODBCDRIVER_API SQLRETURN SQL_API SQLBindCol(
    SQLHSTMT        StatementHandle, 
    SQLUSMALLINT    ColumnNumber, 
    SQLSMALLINT     TargetType, 
    SQLPOINTER      TargetValue, 
    SQLLEN          BufferLength, 
    SQLLEN          *StrLen_or_Ind);

ODBCDRIVER_API SQLRETURN SQL_API SQLBindParam(
    SQLHSTMT        StatementHandle,
    SQLUSMALLINT    ParameterNumber, 
    SQLSMALLINT     ValueType,
    SQLSMALLINT     ParameterType, 
    SQLULEN         LengthPrecision,
    SQLSMALLINT     ParameterScale, 
    SQLPOINTER      ParameterValue,
    SQLLEN          *StrLen_or_Ind);

ODBCDRIVER_API SQLRETURN SQL_API SQLCancel(
    SQLHSTMT        StatementHandle);

ODBCDRIVER_API SQLRETURN SQL_API SQLCloseCursor(
    SQLHSTMT        StatementHandle);

ODBCDRIVER_API SQLRETURN SQL_API SQLColAttribute (SQLHSTMT StatementHandle,
    SQLUSMALLINT    ColumnNumber, 
    SQLUSMALLINT    FieldIdentifier,
    SQLPOINTER      CharacterAttribute, 
    SQLSMALLINT     BufferLength,
    SQLSMALLINT     *StringLength, 
    SQLPOINTER      NumericAttribute);

//////////////////////////////////////////////////////////////////////

ODBCDRIVER_API SQLRETURN SQL_API SQLColAttributeW(
    SQLHSTMT        hstmt,
    SQLUSMALLINT    iCol,
    SQLUSMALLINT    iField,
    SQLPOINTER      pCharAttr,
    SQLSMALLINT     cbCharAttrMax,    
    SQLSMALLINT     *pcbCharAttr,
    SQLPOINTER      pNumAttr);    

ODBCDRIVER_API SQLRETURN SQL_API SQLColAttributesW(
    SQLHSTMT        hstmt,
    SQLUSMALLINT    icol,
    SQLUSMALLINT    fDescType,
    SQLPOINTER      rgbDesc,
    SQLSMALLINT     cbDescMax,
    SQLSMALLINT     *pcbDesc,
    SQLLEN          *pfDesc);

ODBCDRIVER_API SQLRETURN SQL_API SQLDescribeColW(
    SQLHSTMT        hstmt,
    SQLUSMALLINT    icol,
    SQLWCHAR        *szColName,
    SQLSMALLINT     cbColNameMax,
    SQLSMALLINT     *pcbColName,
    SQLSMALLINT     *pfSqlType,
    SQLULEN         *pcbColDef,
    SQLSMALLINT     *pibScale,
    SQLSMALLINT     *pfNullable);

ODBCDRIVER_API SQLRETURN SQL_API SQLErrorW(
    SQLHENV         henv,
    SQLHDBC         hdbc,
    SQLHSTMT        hstmt,
    SQLWCHAR        *szSqlState,
    SQLINTEGER      *pfNativeError,
    SQLWCHAR        *szErrorMsg,
    SQLSMALLINT     cbErrorMsgMax,
    SQLSMALLINT     *pcbErrorMsg);

ODBCDRIVER_API SQLRETURN SQL_API SQLExecDirectW(
    SQLHSTMT        hstmt,
    SQLWCHAR        *szSqlStr,
    SQLINTEGER      cbSqlStr);


ODBCDRIVER_API SQLRETURN SQL_API SQLGetCursorNameW(
    SQLHSTMT        hstmt,
    SQLWCHAR        *szCursor,
    SQLSMALLINT     cbCursorMax,
    SQLSMALLINT     *pcbCursor);

ODBCDRIVER_API SQLRETURN  SQL_API SQLSetDescFieldW(
    SQLHDESC        DescriptorHandle,
    SQLSMALLINT     RecNumber, 
    SQLSMALLINT     FieldIdentifier,
    SQLPOINTER      Value, 
    SQLINTEGER      BufferLength);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetDescFieldW(
    SQLHDESC        hdesc,
    SQLSMALLINT     iRecord,
    SQLSMALLINT     iField,
    SQLPOINTER      rgbValue,
    SQLINTEGER      cbValueMax,
    SQLINTEGER      *pcbValue);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetDescRecW(
    SQLHDESC        hdesc,
    SQLSMALLINT     iRecord,
    SQLWCHAR        *szName,
    SQLSMALLINT     cbNameMax,
    SQLSMALLINT     *pcbName,
    SQLSMALLINT     *pfType,
    SQLSMALLINT     *pfSubType,
    SQLLEN          *pLength,
    SQLSMALLINT     *pPrecision, 
    SQLSMALLINT     *pScale,
    SQLSMALLINT     *pNullable);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetDiagFieldW(
    SQLSMALLINT     fHandleType,
    SQLHANDLE       handle,
    SQLSMALLINT     iRecord,
    SQLSMALLINT     fDiagField,
    SQLPOINTER      rgbDiagInfo,
    SQLSMALLINT     cbDiagInfoMax,
    SQLSMALLINT     *pcbDiagInfo);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetDiagRecW(
    SQLSMALLINT     fHandleType,
    SQLHANDLE       handle,
    SQLSMALLINT     iRecord,
    SQLWCHAR        *szSqlState,
    SQLINTEGER      *pfNativeError,
    SQLWCHAR        *szErrorMsg,
    SQLSMALLINT     cbErrorMsgMax,
    SQLSMALLINT     *pcbErrorMsg);

ODBCDRIVER_API SQLRETURN SQL_API SQLPrepareW(
    SQLHSTMT        hstmt,
    SQLWCHAR        *szSqlStr,
    SQLINTEGER      cbSqlStr);

ODBCDRIVER_API SQLRETURN SQL_API SQLSetCursorNameW(
    SQLHSTMT        hstmt,
    SQLWCHAR        *szCursor,
    SQLSMALLINT     cbCursor);

ODBCDRIVER_API SQLRETURN SQL_API SQLColumnsW(
    SQLHSTMT        hstmt,
    SQLWCHAR        *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLWCHAR        *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLWCHAR        *szTableName,
    SQLSMALLINT     cbTableName,
    SQLWCHAR        *szColumnName,
    SQLSMALLINT     cbColumnName);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetInfoW(
    SQLHDBC         hdbc,
    SQLUSMALLINT    fInfoType,
    SQLPOINTER      rgbInfoValue,
    SQLSMALLINT     cbInfoValueMax,
    SQLSMALLINT     *pcbInfoValue);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetTypeInfoW(
    SQLHSTMT        StatementHandle,
    SQLSMALLINT     DataType);


ODBCDRIVER_API SQLRETURN SQL_API SQLSpecialColumnsW(
    SQLHSTMT        hstmt,
    SQLUSMALLINT    fColType,
    SQLWCHAR        *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLWCHAR        *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLWCHAR        *szTableName,
    SQLSMALLINT     cbTableName,
    SQLUSMALLINT    fScope,
    SQLUSMALLINT    fNullable);

ODBCDRIVER_API SQLRETURN SQL_API SQLStatisticsW(
    SQLHSTMT        hstmt,
    SQLWCHAR        *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLWCHAR        *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLWCHAR        *szTableName,
    SQLSMALLINT     cbTableName,
    SQLUSMALLINT    fUnique,
    SQLUSMALLINT    fAccuracy);

ODBCDRIVER_API SQLRETURN SQL_API SQLTablesW(
    SQLHSTMT        hstmt,
    SQLWCHAR        *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLWCHAR        *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLWCHAR        *szTableName,
    SQLSMALLINT     cbTableName,
    SQLWCHAR        *szTableType,
    SQLSMALLINT     cbTableType);

ODBCDRIVER_API SQLRETURN SQL_API SQLDataSourcesW(
    SQLHENV         henv,
    SQLUSMALLINT    fDirection,
    SQLWCHAR        *szDSN,
    SQLSMALLINT     cbDSNMax,
    SQLSMALLINT     *pcbDSN,
    SQLWCHAR        *szDescription,
    SQLSMALLINT     cbDescriptionMax,
    SQLSMALLINT     *pcbDescription);

ODBCDRIVER_API SQLRETURN SQL_API SQLColumnPrivilegesW(
    SQLHSTMT        hstmt,
    SQLWCHAR        *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLWCHAR        *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLWCHAR        *szTableName,
    SQLSMALLINT     cbTableName,
    SQLWCHAR        *szColumnName,
    SQLSMALLINT     cbColumnName);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetStmtAttrW(
    SQLHSTMT        hstmt,
    SQLINTEGER      fAttribute,
    SQLPOINTER      rgbValue,
    SQLINTEGER      cbValueMax,
    SQLINTEGER      *pcbValue);

ODBCDRIVER_API SQLRETURN SQL_API SQLSetStmtAttrW(
    SQLHSTMT        hstmt,
    SQLINTEGER      fAttribute,
    SQLPOINTER      rgbValue,
    SQLINTEGER      cbValueMax);

ODBCDRIVER_API SQLRETURN SQL_API SQLForeignKeysW(
    SQLHSTMT        hstmt,
    SQLWCHAR        *szPkCatalogName,
    SQLSMALLINT     cbPkCatalogName,
    SQLWCHAR        *szPkSchemaName,
    SQLSMALLINT     cbPkSchemaName,
    SQLWCHAR        *szPkTableName,
    SQLSMALLINT     cbPkTableName,
    SQLWCHAR        *szFkCatalogName,
    SQLSMALLINT     cbFkCatalogName,
    SQLWCHAR        *szFkSchemaName,
    SQLSMALLINT     cbFkSchemaName,
    SQLWCHAR        *szFkTableName,
    SQLSMALLINT     cbFkTableName);

ODBCDRIVER_API SQLRETURN SQL_API SQLNativeSqlW(
    SQLHDBC         hdbc,
    SQLWCHAR        *szSqlStrIn,
    SQLINTEGER      cbSqlStrIn,
    SQLWCHAR        *szSqlStr,
    SQLINTEGER      cbSqlStrMax,
    SQLINTEGER      *pcbSqlStr);

ODBCDRIVER_API SQLRETURN SQL_API SQLPrimaryKeysW(
    SQLHSTMT        hstmt,
    SQLWCHAR        *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLWCHAR        *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLWCHAR        *szTableName,
    SQLSMALLINT     cbTableName);

ODBCDRIVER_API SQLRETURN SQL_API SQLProcedureColumnsW(
    SQLHSTMT        hstmt,
    SQLWCHAR        *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLWCHAR        *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLWCHAR        *szProcName,
    SQLSMALLINT     cbProcName,
    SQLWCHAR        *szColumnName,
    SQLSMALLINT     cbColumnName);

ODBCDRIVER_API SQLRETURN SQL_API SQLProceduresW(
    SQLHSTMT        hstmt,
    SQLWCHAR        *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLWCHAR        *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLWCHAR        *szProcName,
    SQLSMALLINT     cbProcName);

ODBCDRIVER_API SQLRETURN SQL_API SQLTablePrivilegesW(
    SQLHSTMT        hstmt,
    SQLWCHAR        *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLWCHAR        *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLWCHAR        *szTableName,
    SQLSMALLINT     cbTableName);

ODBCDRIVER_API SQLRETURN SQL_API SQLDriversW(
    SQLHENV         henv,
    SQLUSMALLINT    fDirection,
    SQLWCHAR        *szDriverDesc,
    SQLSMALLINT     cbDriverDescMax,
    SQLSMALLINT     *pcbDriverDesc,
    SQLWCHAR        *szDriverAttributes,
    SQLSMALLINT     cbDrvrAttrMax,
    SQLSMALLINT     *pcbDrvrAttr);

//////////////////////////////////////////////////////////////////////

ODBCDRIVER_API SQLRETURN SQL_API SQLColAttributeA(
    SQLHSTMT        hstmt,
    SQLSMALLINT     iCol,
    SQLSMALLINT     iField,
    SQLPOINTER      pCharAttr,
    SQLSMALLINT     cbCharAttrMax,    
    SQLSMALLINT     *pcbCharAttr,
    SQLLEN          *pNumAttr);    

ODBCDRIVER_API SQLRETURN SQL_API SQLColAttributesA(
    SQLHSTMT        hstmt,
    SQLUSMALLINT    icol,
    SQLUSMALLINT    fDescType,
    SQLPOINTER      rgbDesc,
    SQLSMALLINT     cbDescMax,
    SQLSMALLINT     *pcbDesc,
    SQLLEN          *pfDesc);

ODBCDRIVER_API SQLRETURN SQL_API SQLConnectA(
    SQLHDBC         hdbc,
    SQLCHAR         *szDSN,
    SQLSMALLINT     cbDSN,
    SQLCHAR         *szUID,
    SQLSMALLINT     cbUID,
    SQLCHAR         *szAuthStr,
    SQLSMALLINT     cbAuthStr);

ODBCDRIVER_API SQLRETURN SQL_API SQLDescribeColA(
    SQLHSTMT        hstmt,
    SQLUSMALLINT    icol,
    SQLCHAR         *szColName,
    SQLSMALLINT     cbColNameMax,
    SQLSMALLINT     *pcbColName,
    SQLSMALLINT     *pfSqlType,
    SQLUINTEGER     *pcbColDef,
    SQLSMALLINT     *pibScale,
    SQLSMALLINT     *pfNullable);

ODBCDRIVER_API SQLRETURN SQL_API SQLErrorA(
    SQLHENV         henv,
    SQLHDBC         hdbc,
    SQLHSTMT        hstmt,
    SQLCHAR         *szSqlState,
    SQLINTEGER      *pfNativeError,
    SQLCHAR         *szErrorMsg,
    SQLSMALLINT     cbErrorMsgMax,
    SQLSMALLINT     *pcbErrorMsg);

ODBCDRIVER_API SQLRETURN SQL_API SQLExecDirectA(
    SQLHSTMT        hstmt,
    SQLCHAR         *szSqlStr,
    SQLINTEGER      cbSqlStr);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetCursorNameA(
    SQLHSTMT        hstmt,
    SQLCHAR         *szCursor,
    SQLSMALLINT     cbCursorMax,
    SQLSMALLINT     *pcbCursor);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetDescFieldA(
    SQLHDESC        hdesc,
    SQLSMALLINT     iRecord,
    SQLSMALLINT     iField,
    SQLPOINTER      rgbValue,
    SQLINTEGER      cbValueMax,
    SQLINTEGER      *pcbValue);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetDescRecA(
    SQLHDESC        hdesc,
    SQLSMALLINT     iRecord,
    SQLCHAR         *szName,
    SQLSMALLINT     cbNameMax,
    SQLSMALLINT     *pcbName,
    SQLSMALLINT     *pfType,
    SQLSMALLINT     *pfSubType,
    SQLINTEGER      *pLength,
    SQLSMALLINT     *pPrecision, 
    SQLSMALLINT     *pScale,
    SQLSMALLINT     *pNullable);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetDiagFieldA(
    SQLSMALLINT     fHandleType,
    SQLHANDLE       handle,
    SQLSMALLINT     iRecord,
    SQLSMALLINT     fDiagField,
    SQLPOINTER      rgbDiagInfo,
    SQLSMALLINT     cbDiagInfoMax,
    SQLSMALLINT     *pcbDiagInfo);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetDiagRecA(
    SQLSMALLINT     fHandleType,
    SQLHANDLE       handle,
    SQLSMALLINT     iRecord,
    SQLCHAR         *szSqlState,
    SQLINTEGER      *pfNativeError,
    SQLCHAR         *szErrorMsg,
    SQLSMALLINT     cbErrorMsgMax,
    SQLSMALLINT     *pcbErrorMsg);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetStmtAttrA(
    SQLHSTMT        hstmt,
    SQLINTEGER      fAttribute,
    SQLPOINTER      rgbValue,
    SQLINTEGER      cbValueMax,
    SQLINTEGER      *pcbValue);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetTypeInfoA(
    SQLHSTMT        StatementHandle,
    SQLSMALLINT     DataTyoe);

ODBCDRIVER_API SQLRETURN SQL_API SQLPrepareA(
    SQLHSTMT        hstmt,
    SQLCHAR         *szSqlStr,
    SQLINTEGER      cbSqlStr);

ODBCDRIVER_API SQLRETURN SQL_API SQLSetCursorNameA(
    SQLHSTMT        hstmt,
    SQLCHAR         *szCursor,
    SQLSMALLINT     cbCursor);

ODBCDRIVER_API SQLRETURN SQL_API SQLColumnsA(
    SQLHSTMT        hstmt,
    SQLCHAR         *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLCHAR         *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLCHAR         *szTableName,
    SQLSMALLINT     cbTableName,
    SQLCHAR         *szColumnName,
    SQLSMALLINT     cbColumnName);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetInfoA(
    SQLHDBC         hdbc,
    SQLUSMALLINT    fInfoType,
    SQLPOINTER      rgbInfoValue,
    SQLSMALLINT     cbInfoValueMax,
    SQLSMALLINT*    pcbInfoValue);

ODBCDRIVER_API SQLRETURN SQL_API SQLGetStmtOptionA(
    SQLHSTMT        hstmt,
    SQLUSMALLINT    fOption,
    SQLPOINTER      pvParam);

ODBCDRIVER_API SQLRETURN SQL_API SQLSetStmtOptionA(
    SQLHSTMT        hstmt,
    SQLUSMALLINT    fOption,
    SQLULEN         vParam);

ODBCDRIVER_API SQLRETURN SQL_API SQLSpecialColumnsA(
    SQLHSTMT        hstmt,
    SQLUSMALLINT    fColType,
    SQLCHAR         *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLCHAR         *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLCHAR         *szTableName,
    SQLSMALLINT     cbTableName,
    SQLUSMALLINT    fScope,
    SQLUSMALLINT    fNullable);

ODBCDRIVER_API SQLRETURN SQL_API SQLStatisticsA(
    SQLHSTMT        hstmt,
    SQLCHAR         *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLCHAR         *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLCHAR         *szTableName,
    SQLSMALLINT     cbTableName,
    SQLUSMALLINT    fUnique,
    SQLUSMALLINT    fAccuracy);

ODBCDRIVER_API SQLRETURN SQL_API SQLTablesA(
    SQLHSTMT        hstmt,
    SQLCHAR         *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLCHAR         *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLCHAR         *szTableName,
    SQLSMALLINT     cbTableName,
    SQLCHAR         *szTableType,
    SQLSMALLINT     cbTableType);

ODBCDRIVER_API SQLRETURN SQL_API SQLDataSourcesA(
    SQLHENV         henv,
    SQLUSMALLINT    fDirection,
    SQLCHAR         *szDSN,
    SQLSMALLINT     cbDSNMax,
    SQLSMALLINT     *pcbDSN,
    SQLCHAR         *szDescription,
    SQLSMALLINT     cbDescriptionMax,
    SQLSMALLINT     *pcbDescription);

ODBCDRIVER_API SQLRETURN SQL_API SQLDriverConnectA(
    SQLHDBC         hdbc,
    SQLHWND         hwnd,
    SQLCHAR         *szConnStrIn,
    SQLSMALLINT     cbConnStrIn,
    SQLCHAR         *szConnStrOut,
    SQLSMALLINT     cbConnStrOutMax,
    SQLSMALLINT     *pcbConnStrOut,
    SQLUSMALLINT    fDriverCompletion);

ODBCDRIVER_API SQLRETURN SQL_API SQLColumnPrivilegesA(
    SQLHSTMT        hstmt,
    SQLCHAR         *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLCHAR         *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLCHAR         *szTableName,
    SQLSMALLINT     cbTableName,
    SQLCHAR         *szColumnName,
    SQLSMALLINT     cbColumnName);

ODBCDRIVER_API SQLRETURN SQL_API SQLDescribeParamA(
    SQLHSTMT        hstmt,
    SQLUSMALLINT    ipar,
    SQLSMALLINT     *pfSqlType,
    SQLUINTEGER     *pcbParamDef,
    SQLSMALLINT     *pibScale,
    SQLSMALLINT     *pfNullable);

ODBCDRIVER_API SQLRETURN SQL_API SQLForeignKeysA(
    SQLHSTMT        hstmt,
    SQLCHAR         *szPkCatalogName,
    SQLSMALLINT     cbPkCatalogName,
    SQLCHAR         *szPkSchemaName,
    SQLSMALLINT     cbPkSchemaName,
    SQLCHAR         *szPkTableName,
    SQLSMALLINT     cbPkTableName,
    SQLCHAR         *szFkCatalogName,
    SQLSMALLINT     cbFkCatalogName,
    SQLCHAR         *szFkSchemaName,
    SQLSMALLINT     cbFkSchemaName,
    SQLCHAR         *szFkTableName,
    SQLSMALLINT     cbFkTableName);

ODBCDRIVER_API SQLRETURN SQL_API SQLNativeSqlA(
    SQLHDBC         hdbc,
    SQLCHAR         *szSqlStrIn,
    SQLINTEGER      cbSqlStrIn,
    SQLCHAR         *szSqlStr,
    SQLINTEGER      cbSqlStrMax,
    SQLINTEGER      *pcbSqlStr);

ODBCDRIVER_API SQLRETURN SQL_API SQLPrimaryKeysA(
    SQLHSTMT        hstmt,
    SQLCHAR         *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLCHAR         *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLCHAR         *szTableName,
    SQLSMALLINT     cbTableName);

ODBCDRIVER_API SQLRETURN SQL_API SQLProcedureColumnsA(
    SQLHSTMT        hstmt,
    SQLCHAR         *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLCHAR         *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLCHAR         *szProcName,
    SQLSMALLINT     cbProcName,
    SQLCHAR         *szColumnName,
    SQLSMALLINT     cbColumnName);

ODBCDRIVER_API SQLRETURN SQL_API SQLProceduresA(
    SQLHSTMT        hstmt,
    SQLCHAR         *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLCHAR         *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLCHAR         *szProcName,
    SQLSMALLINT     cbProcName);

ODBCDRIVER_API SQLRETURN SQL_API SQLTablePrivilegesA(
    SQLHSTMT        hstmt,
    SQLCHAR         *szCatalogName,
    SQLSMALLINT     cbCatalogName,
    SQLCHAR         *szSchemaName,
    SQLSMALLINT     cbSchemaName,
    SQLCHAR         *szTableName,
    SQLSMALLINT     cbTableName);

ODBCDRIVER_API SQLRETURN SQL_API SQLDriversA(
    SQLHENV         henv,
    SQLUSMALLINT    fDirection,
    SQLCHAR         *szDriverDesc,
    SQLSMALLINT     cbDriverDescMax,
    SQLSMALLINT     *pcbDriverDesc,
    SQLCHAR         *szDriverAttributes,
    SQLSMALLINT     cbDrvrAttrMax,
    SQLSMALLINT     *pcbDrvrAttr);

*/

#ifdef __cplusplus
}                                   
#endif 

#endif //__FDOODBCDRIVERSQL
#include "StdHdr.h"
#include "ODBC.h"

SQLRETURN SQL_API SQLAllocHandle(
    SQLSMALLINT     HandleType,
    SQLHANDLE       InputHandle, 
    SQLHANDLE       *OutputHandle)
{
    OutputHandle = NULL;
    return SQL_ERROR;
}

SQLRETURN SQL_API SQLFreeStmt(
    SQLHSTMT        StatementHandle,
    SQLUSMALLINT    Option)
{
    return SQL_ERROR;
}

SQLRETURN SQL_API SQLFreeHandle(
    SQLSMALLINT     HandleType, 
    SQLHANDLE       Handle)
{
    return SQL_ERROR;
}

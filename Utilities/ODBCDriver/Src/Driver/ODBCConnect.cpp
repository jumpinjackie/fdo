#include "StdHdr.h"
#include "ODBC.h"

SQLRETURN SQL_API SQLConnect(
    SQLHDBC         hdbc,
    SQLCHAR         *szDSN,
    SQLSMALLINT     cbDSN,
    SQLCHAR         *szUID,
    SQLSMALLINT     cbUID,
    SQLCHAR         *szAuthStr,
    SQLSMALLINT     cbAuthStr)
{
    return TRUE;
}

SQLRETURN SQL_API SQLConnectW(
    SQLHDBC         hdbc,
    SQLWCHAR        *szDSN,
    SQLSMALLINT     cbDSN,
    SQLWCHAR        *szUID,
    SQLSMALLINT     cbUID,
    SQLWCHAR        *szAuthStr,
    SQLSMALLINT     cbAuthStr)
{
    return TRUE;
}

SQLRETURN SQL_API SQLDriverConnect(
    SQLHDBC         hdbc,
    SQLHWND         hwnd,
    SQLCHAR         *szConnStrIn,
    SQLSMALLINT     cbConnStrIn,
    SQLCHAR         *szConnStrOut,
    SQLSMALLINT     cbConnStrOutMax,
    SQLSMALLINT     *pcbConnStrOut,
    SQLUSMALLINT    fDriverCompletion)
{
    return TRUE;
}

SQLRETURN SQL_API SQLDriverConnectW(
    SQLHDBC         hdbc,
    SQLHWND         hwnd,
    SQLWCHAR        *szConnStrIn,
    SQLSMALLINT     cbConnStrIn,
    SQLWCHAR        *szConnStrOut,
    SQLSMALLINT     cbConnStrOutMax,
    SQLSMALLINT     *pcbConnStrOut,
    SQLUSMALLINT    fDriverCompletion)
{
    return TRUE;
}

SQLRETURN SQL_API SQLGetConnectAttr(
    SQLHDBC         hdbc,
    SQLINTEGER      fAttribute,
    SQLPOINTER      rgbValue,
    SQLINTEGER      cbValueMax,
    SQLINTEGER      *pcbValue)
{
    return TRUE;
}

SQLRETURN SQL_API SQLSetConnectAttr(
    SQLHDBC         hdbc,
    SQLINTEGER      fAttribute,
    SQLPOINTER      rgbValue,
    SQLINTEGER      cbValue)
{
    return TRUE;
}

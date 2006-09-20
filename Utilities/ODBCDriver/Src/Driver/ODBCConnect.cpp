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

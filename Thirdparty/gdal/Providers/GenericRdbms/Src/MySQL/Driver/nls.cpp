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
 *
 ************************************************************************/

#include "stdafx.h"

//TODO: this is needed to work around a #define bool BOOL in older mysql
//headers, which messes up various STL inclusions from the headers below.
//Once we move to a higher version of mysql, we can remove this 
#undef bool

#include <Fdo.h>
#include <FdoCommonNlsUtil.h>
#include <Inc/Nls/fdordbms_msg.h>

extern char *fdordbms_cat;

const wchar_t* mysql_nls_msg_get(int msg_num, char* default_msg,  ...)
{
    FdoString *ret = NULL;
    va_list varargs;
    va_start(varargs, default_msg);
    ret = FdoException::NLSGetMessage(msg_num, default_msg, MF_FDORDBMS, varargs);
    va_end(varargs);
    return ret;
}

const wchar_t* mysql_nls_version( int msg_num, char* default_msg, unsigned long mysql_version, FdoString* req_version )
{
    // Parse out version components from integer version.
    FdoInt32 major = (FdoInt32) (mysql_version / 10000);
    FdoInt32 minor1 = (FdoInt32)((mysql_version - (major * 10000 )) / 100 );
    FdoInt32 minor2 = (FdoInt32)(mysql_version - (major * 10000 ) - (minor1 * 100));

    return mysql_nls_msg_get( msg_num, default_msg, major, minor1, minor2, req_version );
}

const wchar_t* mysql_nls_client_version( unsigned long client_version )
{
    return mysql_nls_version( 
        FDORDBMS_493, 
        "MySQL client (libmysql) version %1$d.%2$d.%3$d is too old, FDO requires at least version %4$ls; ",
        client_version,
        L"5"
    );
}

const wchar_t* mysql_nls_server_version( unsigned long server_version )
{
    return mysql_nls_version( 
        FDORDBMS_494, 
        "MySQL database version %1$d.%2$d.%3$d is too old, FDO requires at least version %4$ls; ",
        server_version,
        L"5.0.22"
    );
}


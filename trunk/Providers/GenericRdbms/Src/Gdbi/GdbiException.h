#ifndef _GDBIEXCEPTION_
#define _GDBIEXCEPTION_   1
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
 */
#ifdef _WIN32
#pragma once
#endif

#include "Fdo.h"
#include <Inc/Nls/fdordbms_msg.h>
#include <FdoCommonNlsUtil.h>

extern char *fdordbms_cat;

#ifdef _WIN32
#define         NlsMsgGet(msg_num,default_msg) (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, MF_FDORDBMS)
#define         NlsMsgGet1(msg_num,default_msg,arg1) (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, MF_FDORDBMS,arg1)
#define         NlsMsgGet2(msg_num,default_msg,arg1,arg2) (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, MF_FDORDBMS,arg1,arg2)
#define         NlsMsgGet3(msg_num,default_msg,arg1,arg2,arg3) (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, MF_FDORDBMS,arg1,arg2,arg3)
#define         NlsMsgGet4(msg_num,default_msg,arg1,arg2,arg3,arg4) (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, MF_FDORDBMS,arg1,arg2,arg3,arg4)
#define         NlsMsgGet5(msg_num,default_msg,arg1,arg2,arg3,arg4,arg5) (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, MF_FDORDBMS,arg1,arg2,arg3,arg4,arg5)
#else
#define         NlsMsgGet(msg_num,default_msg) (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, MF_FDORDBMS)
#define         NlsMsgGet1(msg_num,default_msg,arg1) (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, MF_FDORDBMS, arg1)
#define         NlsMsgGet2(msg_num,default_msg,arg1,arg2)  (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, MF_FDORDBMS, arg1, arg2)
#define         NlsMsgGet3(msg_num,default_msg,arg1,arg2,arg3) (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, MF_FDORDBMS, arg1, arg2, arg3)
#define         NlsMsgGet4(msg_num,default_msg,arg1,arg2,arg3,arg4) (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, MF_FDORDBMS, arg1, arg2, arg3, arg4)
#define         NlsMsgGet5(msg_num,default_msg,arg1,arg2,arg3,arg4,arg5) (wchar_t*)FdoCommonNlsUtil::NLSGetMessage( msg_num, default_msg, MF_FDORDBMS, arg1, arg2, arg3, arg4, arg5)
#endif

#ifdef __cplusplus
    #define EXTERN_C    extern "C"
#else
    #define EXTERN_C    extern
#endif


extern wchar_t*  Gdbi_nls_msg_get_0( int msg_num, char* default_msg, char* catalog );

extern wchar_t*  Gdbi_nls_msg_get_S( int msg_num, char* default_msg, char* catalog, char* arg1 );

extern wchar_t*  Gdbi_nls_msg_get_SW( int msg_num, char* default_msg, char* catalog, wchar_t* warg1 );

class GdbiException: public FdoException
{
public:
    GdbiException(void);
    virtual ~GdbiException(void);

    GdbiException(const wchar_t* message);

    GdbiException(const wchar_t* message, FdoException* cause);

    virtual void Dispose();

public:
    static GdbiException* Create();

    static GdbiException* Create(const wchar_t* message);

    static GdbiException* Create(const char* message);

    static GdbiException* Create(const wchar_t* message, FdoException* cause);
};

#endif // _GDBIEXCEPTION_

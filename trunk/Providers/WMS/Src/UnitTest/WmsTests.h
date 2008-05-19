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

#ifndef CPP_UNIT_WMTESTS_H
#define CPP_UNIT_WMTESTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#define VERBOSE false


#ifdef _WIN32
// macro to convert a multibyte string into a wide character string, allocating space on the stack
#define mb2w(w,mb)\
{\
    const char* p = mb;\
    size_t i = strlen (p);\
    i++;\
    w = (wchar_t*)alloca (i * sizeof (wchar_t));\
    i = MultiByteToWideChar (\
        CP_THREAD_ACP,\
        0,\
        p,\
        (int)i,\
        w,\
        (int)i);\
    if (0 == i)\
        w = NULL;\
	if (NULL==w) throw FdoException::Create(L"Failed to allocate memory.");\
}

// macro to convert a wide character string into a multibyte string, allocating space on the stack
#define w2mb(mb,w)\
{\
    const wchar_t* p = w;\
    size_t i = wcslen (p);\
    i++;\
    mb = (char*)alloca (i * 6);\
    i = WideCharToMultiByte (\
        CP_THREAD_ACP,\
        0,\
        p,\
        (int)i,\
        mb,\
        (int)i * 6,\
        NULL,\
        NULL);\
    if (0 == i)\
        mb = NULL;\
	if (NULL==mb) throw FdoException::Create(L"Failed to allocate memory.");\
}
#else
// macro to convert a multibyte string into a wide character string, allocating space on the stack
#define mb2w(w,mb)\
{\
    const char* p = mb;\
    size_t i = strlen (p);\
    i++;\
    w = (wchar_t*)alloca (i * sizeof (wchar_t));\
    i = mbstowcs (w, p, i);\
    if (0 > i)\
        w = NULL;\
	if (NULL==w) throw FdoException::Create(L"Failed to allocate memory.");\
}

// macro to convert a wide character string into a multibyte string, allocating space on the stack
#define w2mb(mb,w)\
{\
    const wchar_t* p = w;\
    size_t i = wcslen (p);\
    i++;\
    mb = (char*)alloca (i * 6);\
    i = wcstombs (mb, p, i);\
    if (0 > i)\
        mb = NULL;\
	if (NULL==mb) throw FdoException::Create(L"Failed to allocate memory.");\
}
#endif


class WmsTests : public CppUnit::TestCase
{
public:
    WmsTests (void);
    virtual ~WmsTests (void);

	virtual void setUp ();
	virtual void tearDown ();

    virtual FdoIConnection* GetConnection ();

    static void fail (FdoException* ge);
    static void fail (char* error);

private:
    FdoPtr<FdoIConnection> mConnection;
};

#endif // CPP_UNIT_WMTESTS_H

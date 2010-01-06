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

#ifndef SHPREAD_STDAFX_H
#define SHPREAD_STDAFX_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#ifdef _WIN32

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#ifndef _UNICODE
#define _UNICODE
#endif

// NOTE: Uncomment the following #define statement to enable tracing....
// #define FDORPT_TRACING
// 

#ifdef FDORPT_TRACING
#define _FDORPT0(rptno, msg) _RPT0(rptno, msg)
#define _FDORPT1(rptno, msg, arg1) _RPT1(rptno, msg, arg1)
#define _FDORPT2(rptno, msg, arg1, arg2) _RPT2(rptno, msg, arg1, arg2)
#define _FDORPT3(rptno, msg, arg1, arg2, arg3) _RPT3(rptno, msg, arg1, arg2, arg3)
#define _FDORPT4(rptno, msg, arg1, arg2, arg3, arg4) _RPT4(rptno, msg, arg1, arg2, arg3, arg4)
#else //FDORPT_TRACING
#define _FDORPT0(rptno, msg) __noop
#define _FDORPT1(rptno, msg, arg1) __noop
#define _FDORPT2(rptno, msg, arg1, arg2) __noop
#define _FDORPT3(rptno, msg, arg1, arg2, arg3) __noop
#define _FDORPT4(rptno, msg, arg1, arg2, arg3, arg4) __noop
#endif // FDORPT_TRACING

#else // _WIN32

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <alloca.h>

#define FALSE   0
#define TRUE    1
typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef char CHAR;
typedef BYTE                *PBYTE;
typedef wchar_t     TCHAR;
typedef unsigned short      WORD;

typedef long LONG;
typedef unsigned long DWORD;
typedef unsigned long ULONG;
typedef unsigned long ULONG_PTR;
typedef ULONG_PTR DWORD_PTR;
#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)((DWORD_PTR)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((DWORD_PTR)(l) >> 16))
#define LOBYTE(w)           ((BYTE)((DWORD_PTR)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((DWORD_PTR)(w) >> 8))

#define _ASSERT(expr) ((void)0)
#define _FDORPT0(rptno, msg) ((void)0)
#define _FDORPT1(rptno, msg, arg1) ((void)0)
#define _FDORPT2(rptno, msg, arg1, arg2) ((void)0)
#define _FDORPT3(rptno, msg, arg1, arg2, arg3) ((void)0)
#define _FDORPT4(rptno, msg, arg1, arg2, arg3, arg4) ((void)0)
#define _tcsicmp(s1,s2) wcscasecmp(s1,s2)

#endif // _WIN32

#include <cstring>
#include <cstdio>
#include <Fdo.h>
#include <FdoCommonStringUtil.h>
#include <FdoCommonMiscUtil.h>

#include <../Message/ShpMessageStatic.h>
#include <../Message/Inc/ShpMessage.h>

#endif // SHPREAD_STDAFX_H


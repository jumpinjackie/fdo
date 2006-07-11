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

#ifndef SHPINDEX_STDAFX_H
#define SHPINDEX_STDAFX_H

#ifdef _WIN32

#pragma once

//#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#ifndef _UNICODE
#define _UNICODE
#endif

// NOTE: Uncomment the following #define statement to enable tracing....
// #define FDORPT_TRACING
// 

#ifdef FDORPT_TRACING
#define _FDORPT0(rptno, msg) _RPT0(rptno, msg)
#else 
#define _FDORPT0(rptno, msg) __noop
#endif 

#else

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
typedef LONG HRESULT;
typedef unsigned long       DWORD;
typedef unsigned long       ULONG;
#define FILE_BEGIN           0
#define FILE_CURRENT         1
#define FILE_END             2

#define CONST               const
typedef CONST WCHAR *LPCWSTR, *PCWSTR;
typedef LPCWSTR PCTSTR, LPCTSTR;
typedef WCHAR *LPWSTR, *PWSTR;
typedef LPWSTR PTSTR, LPTSTR;

typedef time_t FILETIME;

#define _ASSERT(expr) ((void)0)
#define _RPT0(rptno, msg) ((void)0)
#define _tcsicmp(s1,s2) wcscasecmp(s1,s2)

#endif

#include <Fdo.h>
#include <FdoCommonStringUtil.h>
#include <../Message/Inc/ShpMessage.h>

#endif // SHPINDEX_STDAFX_H


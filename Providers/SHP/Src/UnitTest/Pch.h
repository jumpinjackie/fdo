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

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
// Undefine GetMessage
#ifdef GetMessage
#undef GetMessage
#endif
#endif

#include <malloc.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <Fdo.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonStringUtil.h>
#include <FdoCommonMiscUtil.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#define _CRTDBG_MAP_ALLOC

#ifdef _WIN32

#include <crtdbg.h>
#include <Windows.h>

#else

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
typedef LONG HRESULT;
#define CONST               const
typedef CONST WCHAR *LPCWSTR, *PCWSTR;
typedef LPCWSTR PCTSTR, LPCTSTR;
typedef WCHAR *LPWSTR, *PWSTR;
typedef LPWSTR PTSTR, LPTSTR;
typedef time_t FILETIME;

#define _ASSERT(expr) ((void)0)
#define _tcsicmp(s1,s2) wcscasecmp(s1,s2)

#endif // _WIN32



// 
//  
//  Copyright (C) 2011 Autodesk Inc.
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
//  

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <vector>
#include <string>

#include <Fdo.h>
#include <wchar.h>
#include <FdoCommonStringUtil.h>

#ifdef _WIN32
#define OGR_API __declspec(dllexport)
#else
#define OGR_API
#endif

#ifndef _WIN32
#define _wcsnicmp wcsncasecmp
#define _wcsicmp wcscasecmp
#endif

#include <ogrsf_frmts.h>


#define W2A_PROPNAME(x)  \
    size_t wlen = wcslen(x); \
    size_t clen = wlen*4 + 1; \
    char* mb##x = (char*)alloca(clen); \
    W2A_FAST(mb##x, clen, x, wlen);

static std::string W2A_SLOW(const wchar_t* input)
{
    size_t wlen = wcslen(input);
    int mbslen = (int) wlen * 4 + 1;
    char* mbs = (char*)alloca(mbslen);
    //WideCharToMultiByte(CP_UTF8, 0, input, -1, mbs, mbslen, 0, 0);
    _ut_utf8_from_unicode(input, wlen, mbs, mbslen);
    return std::string(mbs);
}

static std::wstring A2W_SLOW(const char* input)
{
    int wlen = (int)strlen(input) + 1;
    wchar_t* ws = (wchar_t*)alloca(sizeof(wchar_t)*wlen);
    //MultiByteToWideChar(CP_UTF8, 0, input, -1, ws, wlen);
    _ut_utf8_to_unicode(input, wlen, ws, wlen);
    return std::wstring(ws);
}

static int A2W_FAST(wchar_t* dst, int wlen, const char* src, int clen)
{
    return _ut_utf8_to_unicode(src, clen, dst, wlen);
}

static int W2A_FAST(char* dst, int clen, const wchar_t* src, int wlen)
{
    return _ut_utf8_from_unicode(src, wlen, dst, clen);
}

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

#ifndef OGR_STDAFX_H
#define OGR_STDAFX_H

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

#define PROP_GEOMETRY       "GEOMETRY"
#define PROP_FID            "FID"

#define WCHAR_ENCODING CPL_ENC_UCS2

#define W2A_PROPNAME(x) \
    std::string sConv##x = W2A_SLOW(x); \
    const char* mb##x = sConv##x.c_str();

#define W2A_PROPNAME_FROM_ENCODING(x, encoding) \
    std::string sConv##x = W2A_SLOW(x, encoding); \
    const char* mb##x = sConv##x.c_str();

#define W2A_PROPNAME_NAMEMAP(x)  \
    std::string sConv##x; \
    if (m_bUseNameMap) { \
        sConv##x = (char*)m_namemap[x].c_str(); \
    } else { \
        sConv##x = W2A_SLOW(x); \
    } \
    const char* mb##x = sConv##x.c_str();

static std::string W2A_SLOW(const wchar_t* input, const std::string& encoding = CPL_ENC_UTF8)
{
    char* conv = CPLRecodeFromWChar(input, WCHAR_ENCODING, encoding.c_str());
    std::string sConv(conv);
    CPLFree(conv);
    return sConv;
}

static std::wstring A2W_SLOW(const char* input, const std::string& encoding = CPL_ENC_UTF8)
{
    wchar_t* ws = CPLRecodeToWChar(input, encoding.c_str(), WCHAR_ENCODING);
    std::wstring wConv(ws);
    CPLFree(ws);
    return wConv;
}

static int A2W_FAST(wchar_t* dst, int wlen, const char* src, int clen)
{
    return _ut_utf8_to_unicode(src, clen, dst, wlen);
}

static int W2A_FAST(char* dst, int clen, const wchar_t* src, int wlen)
{
    return _ut_utf8_from_unicode(src, wlen, dst, clen);
}

#endif //OGR_STDAFX_H

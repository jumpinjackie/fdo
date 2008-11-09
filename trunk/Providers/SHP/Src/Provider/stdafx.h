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

#ifndef SHP_STDAFX_H
#define SHP_STDAFX_H

//defines that helps us use hash_map (and similar STL classes) in the same way 
//under Linux and Windows.
#ifdef WIN32
#include <hash_map>
#include <functional>
#else
#include <ext/hash_map>
#include <ext/functional>
namespace stdext = ::__gnu_cxx;
using namespace std;
#endif

#ifdef _WIN32
#pragma once

#define WIN32_LEAN_AND_MEAN        // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#else // _WIN32

#define FDOSHP_API
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
typedef LONG HRESULT;
typedef CHAR *LPSTR;
typedef LPSTR LPTSTR;
typedef const CHAR *LPCSTR;
typedef LPCSTR LPCTSTR;
typedef unsigned long DWORD;
typedef struct tagFILETIME {
DWORD dwLowDateTime;
DWORD dwHighDateTime;
} FILETIME;

#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)((DWORD_PTR)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((DWORD_PTR)(l) >> 16))
#define LOBYTE(w)           ((BYTE)((DWORD_PTR)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((DWORD_PTR)(w) >> 8))


#endif // _WIN32


// Base FDO headers
#include <Fdo.h>
#include <Fdo/Xml/SpatialContextReader.h>

// FdoCommon headers
#include <FdoCommonConnPropDictionary.h>
#include <FdoCommonMiscUtil.h>
#include <FdoCommonSchemaUtil.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonStringUtil.h>
#include <FdoCommonCommand.h>
#include <FdoCommonFeatureCommand.h>

// Shp Override headers
#include <SHP/FdoShpOverrides.h>

// Provider specific headers
#include <ShpProvider.h>
#include <../Message/Inc/ShpMessage.h>
#include <ShpConnectionCapabilities.h>
#include <ShpCommandCapabilities.h>
#include <ShpExpressionCapabilities.h>
#include <ShpFilterCapabilities.h>
#include <ShpSchemaCapabilities.h>
#include <ShpRasterCapabilities.h>
#include <ShpTopologyCapabilities.h>
#include <ShpGeometryCapabilities.h>
#include <ShpConnection.h>
#include <ShpConnectionInfo.h>
#include <ShpDescribeSchemaCommand.h>
#include <ShpDescribeSchemaMappingCommand.h>
#include <ShpFilterExecutor.h>
#include <ShpQueryOptimizer.h>
#include <ShpFeatIdQueryEvaluator.h>
#include <ShpFeatIdQueryTester.h>
#include <ShpReader.h>
#include <ShpFeatureReader.h>
#include <ShpSelectCommand.h>
#include <ShpInsertCommand.h>
#include <ShpUpdateCommand.h>
#include <ShpDeleteCommand.h>
#include <ShpGetSpatialContextsCommand.h>
#include <ShpCreateSpatialContextCommand.h>
#include <ShpApplySchemaCommand.h>
#include <ShpDestroySchemaCommand.h>
#include <ShpSchemaUtilities.h>
#include <../../Inc/SHP/ShpCommandType.h>
#include <ShpSelectAggregatesCommand.h>


#endif


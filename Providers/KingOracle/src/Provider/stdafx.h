/*
* Copyright (C) 2006  SL-King d.o.o
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

#ifndef _KGORA_STDAFX_H
#define _KGORA_STDAFX_H

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN        // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#else // _WIN32

#define FDOKGORA_API

#include <cstring>
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

// number of elements in an array
// #define ELEMENTS(x) (sizeof(x)/sizeof(x[0]))FdoTypes.hfdotypes

// Base FDO headers
#include <Fdo.h>
#include <FdoGeometry.h>
#include <Geometry/EnvelopeImpl.h>

// FdoCommon headers
#include <FdoCommonConnPropDictionary.h>
#include <FdoCommonMiscUtil.h>
#include <FdoCommonSchemaUtil.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonStringUtil.h>

// Provider specific headers
#include <KingOracle/FdoKgOraOverrides.h>
#include "KgOraProvider.h"
#include "../Message/inc/KgOraMessage.h"

#include <c_KgOraSridDesc.h>
#include <c_KgOraConnectionCapabilities.h>
#include <c_KgOraCommandCapabilities.h>
#include <c_KgOraExpressionCapabilities.h>
#include <c_KgOraFilterCapabilities.h>
#include <c_KgOraSchemaCapabilities.h>
#include <c_KgOraRasterCapabilities.h>
#include <c_KgOraTopologyCapabilities.h>
#include <c_KgOraGeometryCapabilities.h>
#include <c_KgOraConnection.h>
#include <c_KgOraConnectionInfo.h>
#include <c_KgOraFdoFeatureCommand.h>
#include <c_KgOraSchemaDesc.h>
#include <c_KgOraDescribeSchemaCommand.h>
#include <c_KgOraReader.h>
#include <c_KgOraFeatureReader.h>
#include <c_KgOraFeatureReaderInsert.h>
#include <c_KgOraSQLDataReader.h>
#include <c_KgOraSelectCommand.h>
#include <c_KgOraSelectAggregates.h>
#include <c_KgOraSQLCommand.h>
#include <c_KgOraInsert.h>
#include <c_KgOraDelete.h>
#include <c_KgOraUpdate.h>
#include <c_KgOraApplySchema.h>
#include <c_KgOraCreateDataStore.h>
#include <c_KgOraGetSpatialContextsCommand.h>
#include <c_KgOraCreateSpatialContext.h>
#include <c_KgOraExpressionProcessor.h>

#include "c_LogAPI.h"


#endif


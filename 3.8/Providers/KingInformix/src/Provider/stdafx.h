/*
* Copyright (C) 2006  Haris Kurtagic
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

#ifndef _KGINF_STDAFX_H
#define _KGINF_STDAFX_H

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN        // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#else // _WIN32

#define FDOKGINF_API
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
#define ELEMENTS(x) (sizeof(x)/sizeof(x[0]))


#include "FDO_31_Types.h"

// Base FDO headers
#include <Fdo.h>
#include <geometry/EnvelopeImpl.h>

#include <fdo/Xml/SpatialContextReader.h>

// FdoCommon headers
#include <FdoCommonConnPropDictionary.h>
#include <FdoCommonMiscUtil.h>
#include <FdoCommonSchemaUtil.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonStringUtil.h>

//#include <FdoCommonSelectAggregatesCommand.h>

#include "FdoKgInfProvider.h"

//#include <KingOracle/FdoKgOraProvider.h>
#include <c_KgInfOverColumnDefinition.h>
#include <c_KgInfOverPropertyDefinition.h>
#include <c_KgInfOverPropertyDefinitionCollection.h>
#include <c_KgInfOverClassDefinition.h>
#include <c_KgInfOverClassCollection.h>
#include <c_KgInfOverPhysicalSchemaMapping.h>
#include "c_KgInfOverXmlGlobals.h"

// Provider specific headers

//#include "../Message/inc/KgOraMessage.h"


#include <c_KgInfSridDesc.h>
#include <c_KgInfConnectionCapabilities.h>
#include <c_KgInfCommandCapabilities.h>
#include <c_KgInfExpressionCapabilities.h>
#include <c_KgInfFilterCapabilities.h>
#include <c_KgInfSchemaCapabilities.h>
#include <c_KgInfRasterCapabilities.h>
#include <c_KgInfTopologyCapabilities.h>
#include <c_KgInfGeometryCapabilities.h>
#include <c_KgInfConnection.h>
#include <c_KgInfConnectionInfo.h>
#include <c_KgInfFdoFeatureCommand.h>
#include <c_KgInfSchemaDesc.h>
#include <c_KgInfDescribeSchemaCommand.h>


#include <c_KgInfReader.h>

#include <c_KgInfFeatureReader.h>
#include <c_KgInfFeatureReaderInsert.h>
#include <c_KgInfSQLDataReader.h>
#include <c_KgInfSelectCommand.h>

#include <c_KgInfSQLCommand.h>
#include <c_KgInfInsert.h>
#include <c_KgInfDelete.h>
#include <c_KgInfUpdate.h>
#include <c_KgInfApplySchema.h>
#include <c_KgInfCreateDataStore.h>

#include <c_KgInfGetSpatialContextsCommand.h>
#include <c_KgInfCreateSpatialContext.h>

#include <c_KgInfExpressionProcessor.h>

#include "c_FdoInf_API.h"
#include "c_LogApi.h"


#endif


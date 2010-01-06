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

#ifndef _KGKML_STDAFX_H
#define _KGKML_STDAFX_H




#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN        // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#else // _WIN32

#define FDOKGKML_API
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
//#define ELEMENTS(x) (sizeof(x)/sizeof(x[0]))FdoTypes.hfdotypes


#include <KingKml/FdoKgKmlProvider.h>

//#include "FdoTypes.h"

// Base FDO headers
#include <Fdo.h>
#include <geometry/EnvelopeImpl.h>

#include <fdo/Xml/SpatialContextReader.h>

// FdoCommon headers
#include <FdoCommonConnPropDictionary.h>
#include <FdoCommonMiscUtil.h>
#include <FdoCommonSchemaUtil.h>
//#include <FdoCommonOSUtil.h>
#include <FdoCommonStringUtil.h>
#include <FdoCommonThreadMutex.h>




// Provider specific headers
#include "KgKmlProvider.h"
//#include "../Message/inc/KgOraMessage.h"

#include "c_KgKmlSpatialContext.h"
#include "FdoKgKmlXmlGlobals.h"
#include "FdoKgKmlPhysicalSchemaMapping.h"

#include "c_KgKmlConnectionCapabilities.h"
#include "c_KgKmlSchemaCapabilities.h"
#include "c_KgKmlCommandCapabilities.h"
#include "c_KgKmlGeometryCapabilities.h"
#include "c_KgKmlRasterCapabilities.h"
#include "c_KgKmlFilterCapabilities.h"
#include "c_KgKmlExpressionCapabilities.h"
#include "c_KgKmlTopologyCapabilities.h"
#include "c_KgKmlSpatialContext.h"
#include "c_KgKmlSchemaDesc.h"
#include "c_KgKmlConnection.h"
#include "c_KgKmlConnectionInfo.h"
#include "c_KgKmlFdoCommand.h"
#include "c_KgKmlFdoFeatureCommand.h"

#include "c_KgKmlGetSpatialContextsCommand.h"
#include "c_KgKmlCreateSpatialContext.h"
#include "c_KgKmlDescribeSchemaCommand.h"

#include "c_KgKmlApplySchema.h"
#include "c_KgKmlCreateDataStore.h"
#include "c_KgKmlSelectCommand.h"
#include "c_KgKmlInsert.h"
#include "c_KgKmlUpdate.h"

//#include "c_FdoKml_API.h"

#include "c_LogApi.h"


#endif


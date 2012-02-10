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

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifdef _WIN32
#pragma once
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <tchar.h>
#endif

#include <vector>
#include <string>
#include <cstring>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <limits>       // For quiet_NaN()
#include <float.h>      // For _isnan()
#include <stdlib.h>
#include <string.h>
#include <typeinfo>
#include <assert.h>

#ifndef ARCSDE_LOADER

#include <sdeerno.h>
#include <FdoSde.h>

#else

// Base FDO headers
#include <Fdo.h>
#include <FdoCommonSchemaUtil.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonStringUtil.h>
#include <FdoCommonMiscUtil.h>
#include <FdoCommonConnPropDictionary.h>

#endif

#include <FdoCommon.h>

#define _CRTDBG_MAP_ALLOC

#ifdef _WIN32
#include <windows.h>
#include <crtdbg.h>  // for _crtSetDbgFlag() and _CrtCheckMemory() and _ASSERTE
#endif

#ifdef WIN32
#include <hash_map>
#include <functional>
#else
#include <ext/hash_map>
#include <ext/functional>
namespace stdext = ::__gnu_cxx;
using namespace std;
#endif


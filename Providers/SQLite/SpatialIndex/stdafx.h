// 
//  
//  Copyright (C) 2008 Autodesk Inc.
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

#ifndef STDAFX_H
#define STDAFX_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#endif

#include <float.h>
#include <time.h>
#include <malloc.h>

//disable STL bounds checking
#define _SECURE_SCL 0

#include <string>
#include <wchar.h>

//FDO headers
#include "Fdo.h"

//define this if linking to FdoSpatial utility is OK
#define SLT_USE_FDO_SPATIAL

#ifdef SLT_USE_FDO_SPATIAL
    #include "FdoSpatial.h"
#endif

#ifndef _MSC_VER
  #define __restrict __restrict__
  #define __forceinline __inline__
  #define __fastcall 
#endif

#endif

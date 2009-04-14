 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
 // 
 // This library is free software; you can redistribute it and/or
 // modify it under the terms of version 2.1 of the GNU Lesser
 // General Public License as published by the Free Software Foundation.
 // 
 // This library is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 // Lesser General Public License for more details.
 // 
 // You should have received a copy of the GNU Lesser General Public
 // License along with this library; if not, write to the Free Software
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifdef _WIN32
#pragma once
#endif

#ifdef _WIN32
#pragma pack(push, 16) /* large alignment */
#include <config-win.h>
#else
#include <my_global.h>
#endif
#include <mysql.h>
#ifdef _WIN32
#pragma pack(pop) /* restore alignment */
#define WIN32_LEAN_AND_MEAN /* exclude rarely-used stuff from Windows headers */

#include <windows.h>
#endif

#include <cstring>
#include <Inc/rdbi.h>
#include <stdio.h>
#include <string.h>

// Helper macro that determines the number of elements in a statically-sized array
// (this doesn't work for dynamically-sized arrays)
#define ELEMENTS(x) (sizeof(x)/sizeof(x[0]))

// defines minimum client version accepted
#define MIN_CLIENT_VER 50000
// defines minimum server version accepted
#define MIN_SERVER_VER 50022

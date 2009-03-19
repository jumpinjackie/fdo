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

#ifndef _GENERICRDBMSFDOSTDAFX_H_
#define _GENERICRDBMSFDOSTDAFX_H_
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifdef _WIN32
#pragma once


// Windows Header Files:
#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#undef GetMessage
#endif

// System includes
#include <cstring>

// Base FDO headers
#include <Fdo.h>

// FdoRdbms common headers
#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "DbiConnection.h"
#include <Sm/SchemaManager.h>

#include <Rdbms/Override/RdbmsOv.h>
#endif

#ifdef _WIN32
#include <hash_map>
#include <functional>
#else
#include <ext/hash_map>
#include <ext/functional>
namespace stdext = ::__gnu_cxx;
using namespace std;
#endif


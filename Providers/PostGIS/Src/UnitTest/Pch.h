//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#ifdef GetMessage
#undef GetMessage
#endif
#include <tchar.h>
#endif // _WIN32

// std
// TODO: replace with C++ headers
//#include <malloc.h>
//#include <math.h>
//#include <time.h>
//#include <limits>       // For quiet_NaN()
//#include <float.h>      // For _isnan()
//#include <stdlib.h>

// FDO
#include <Fdo.h>
#include <FdoCommon.h>

// CppUnit
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

// PostGIS UnitTest
#include "Conv.h"


#define _CRTDBG_MAP_ALLOC

#ifdef _WIN32
#include <windows.h>
#include <crtdbg.h>  // _crtSetDbgFlag(), _CrtCheckMemory()
#endif // _WIN32

// NOTE: Define __CPPUNIT_MFC_APP in order to have the CPPUNIT UI appear... 
#ifdef __CPPUNIT_MFC_APP
# ifdef WIN32
#  ifdef _DEBUG
#   pragma comment(lib, "testrunnerd.lib")
#  else
#   pragma comment(lib, "testrunner.lib")
#  endif // _DEBUG
# endif
#endif // __CPPUNIT_MFC_APP

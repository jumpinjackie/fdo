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
 *
 */

#ifndef _GENERICRDBMSUNITTESTCOMMONPCH_H_
#define _GENERICRDBMSUNITTESTCOMMONPCH_H_

#ifdef _WIN32
#pragma once
#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
// Undefine GetMessage
#ifdef GetMessage
#undef GetMessage
#endif
#endif

// NOTE: Define __CPPUNIT_MFC_APP in order to have the CPPUNIT UI appear... 
#ifdef __CPPUNIT_MFC_APP
    #ifdef WIN32
	    #ifdef _DEBUG
		    #pragma comment(lib, "testrunnerd.lib")
	    #else
		    #pragma comment(lib, "testrunner.lib")
	    #endif
    #endif
#endif

#ifdef _WIN32
#include <crtdbg.h>
#include <Windows.h>
#else // _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifndef __STDC_LIMIT_MACROS
    #define __STDC_LIMIT_MACROS
#endif
#include <stdint.h>
#include <pwd.h>
#include <unistd.h>
#endif // _WIN32

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include <Fdo/Expression/DataValueCollection.h>
#include <Fdo.h>

#include <FdoCommonOSUtil.h>
#include <FdoCommonStringUtil.h>

#include <Rdbms.h>
#include <Rdbms/Override/RdbmsOv.h>
#include <TestCommon.h>

#endif //_GENERICRDBMSUNITTESTCOMMONPCH_H_

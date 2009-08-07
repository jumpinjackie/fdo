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

#ifdef _WIN32
#pragma once
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#ifdef GetMessage
#undef GetMessage
#endif
#include <tchar.h>
#endif

#include <malloc.h>
#include <math.h>
#include <time.h>
#include <limits>       // For quiet_NaN()
#include <float.h>      // For _isnan()
#include <stdlib.h>

#include <FdoSde.h>
#include <FdoCommon.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#define _CRTDBG_MAP_ALLOC

#ifdef _WIN32
#include <windows.h>
#include <crtdbg.h>  // for _crtSetDbgFlag() and _CrtCheckMemory() and _ASSERTE
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

// NOTE: Define SDEVER_ARCUNITTEST in order to use ArcSDE 9.1/9.2 client 
#ifdef WIN32
    #if _SDEVER_ARCUNITTEST == 91
	    #pragma comment(lib, "pe91.lib")
	    #pragma comment(lib, "sde91.lib")
    #else
        #if _SDEVER_ARCUNITTEST == 92
            #pragma comment(lib, "pe.lib")
            #pragma comment(lib, "sde.lib")
        #else
            #if _SDEVER_ARCUNITTEST == 93
                #pragma comment(lib, "pe.lib")
                #pragma comment(lib, "sde.lib")
            #else
                #pragma message("Please define the environment variable %SDEVER_ARCUNITTEST% having 91, 92 or 93 as value")
            #endif
        #endif
    #endif
#endif

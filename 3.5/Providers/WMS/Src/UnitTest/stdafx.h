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
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifdef _WIN32
#pragma once
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif // _WIN32

#include <cstring>
#include <stdio.h>
#include <tchar.h>

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

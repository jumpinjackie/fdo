// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifdef _WIN32
#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif		


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <float.h>
#include <crtdbg.h>

#endif

#ifndef _WIN32
#   define LLONG_MAX    9223372036854775807LL
#   define LLONG_MIN    (-LLONG_MAX - 1LL)
#endif


#ifdef _DEBUG
    #ifdef _WIN32
#ifndef ASSERT
        #include <crtdbg.h>
        #define ASSERT  _ASSERT
#endif
    #else
    /// linux doesn't seem to have a standard assert mechanism?
        #define _ASSERT(x)
        #define ASSERT(x)
    #endif
#else
    /// no assertions in release mode
#ifndef _ASSERT
    #define _ASSERT(x)
#endif
#ifndef ASSERT
    #define ASSERT(x)
#endif
#endif



//defines that helps us use hash_map (and similar STL classes) in the same way 
//under Linux and Windows.
#ifdef WIN32
#include <hash_map>
#include <functional>
#else
#include <ext/hash_map>
#include <ext/functional>
namespace stdext = ::__gnu_cxx;
using namespace std;
#endif



#include <FdoStd.h>
#include <Fdo.h>

#ifndef _WIN32
#define fdofdo_cat "FdoMessage.cat"
#else
#define fdofdo_cat "FDOMessage.dll"
#endif

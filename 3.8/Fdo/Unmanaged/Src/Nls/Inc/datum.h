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

/************************************************************************
*                                                                       *
* Name                                                                  *
*   datum.h - Basic data type definitions                               *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/datum.h>                                              *
*                                                                       *
* Description                                                           *
*                                                                       *
*                                                                       *
************************************************************************/
#ifndef DATUM_H_DEFINED
#define DATUM_H_DEFINED

#ifdef BOOL
#undef BOOL
#endif
                            
#include <Sys/types.h>
#include <Inc/os.h>

#ifdef SOLARIS
#include <Inc/limits.h>
#endif

#if defined(MS_WINDOWS) || defined(VISION_NUTC)

#if defined(VISION_NUTC)
typedef unsigned int    HANDLE;
typedef unsigned int    HINSTANCE;
#endif

typedef unsigned char       UChar; 
#ifndef Bool
typedef int                 Bool;
#endif
typedef short               Int16;
typedef unsigned short      UInt16;
typedef long                Int32;
/* typedef long long            Int64; */
typedef long int            Int64;      /* INCORRECT */
typedef unsigned long       UInt64;
typedef unsigned long       UInt32;
typedef float               Double32;
typedef double              Double64;   

typedef unsigned int    u_int;
typedef int             bool_t;
typedef unsigned long   u_long; 
typedef unsigned short  u_short;
typedef char*           caddr_t;
typedef int             enum_t;
typedef Int64           longlong_t; 
typedef UInt64          u_longlong_t;
typedef unsigned char   u_char;

#ifndef INT32_MIN
#define INT32_MIN   LONG_MIN
#endif
#ifndef INT32_MAX
#define INT32_MAX   LONG_MAX
#endif

#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif 

#elif defined(OSF1)

#ifndef Bool
typedef int             Bool;
#endif
typedef short           Int16;
typedef unsigned short  UInt16;
typedef int             Int32;
typedef unsigned int    UInt32;
/*typedef long          Int64;*/
typedef float           Double32;
typedef double          Double64;   

#ifndef INT32_MIN
#define INT32_MIN       INT_MIN
#endif 

#ifndef INT32_MAX
#define INT32_MAX       INT_MAX
#endif 

typedef unsigned int    HANDLE;
typedef unsigned int    HINSTANCE;

#else /* Everyone else */

#ifndef Bool
typedef int             Bool;
#endif
#if defined(_AIX) && !defined(bool_t)
typedef int             bool_t;
#endif
typedef short           Int16;
typedef unsigned short  UInt16;
typedef int             Int32;
typedef unsigned int    UInt32;
/*typedef long long         Int64;*/
typedef float           Double32;
typedef double          Double64;   


#ifdef LINUX
#include <stdint.h>
#else
#ifndef INT32_MIN
#define INT32_MIN       LONG_MIN
#endif 

#ifndef INT32_MAX
#define INT32_MAX       LONG_MAX
#endif 
#endif /* LINUX */

typedef unsigned int    HANDLE;
typedef unsigned int    HINSTANCE;

#endif


#endif /* DATUM_H_DEFINED */


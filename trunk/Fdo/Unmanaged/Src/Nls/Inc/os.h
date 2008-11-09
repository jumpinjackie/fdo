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
*   os.h - OS-specific definitions                                      *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/os.h>                                                 *
*                                                                       *
* Description                                                           *
*   This header file contains macro and type definitions that are       *
*   potentially operating system specific.                              *
*                                                                       *
************************************************************************/

#ifndef _OS_H_
#define _OS_H_



#if defined (__unix__) || defined (__unix)
#ifndef unix
#define unix
#endif
#endif


#if defined(_MSC_VER) && !defined(VISION_NUTC)

/*#include <windef.h>*/
#ifndef WINVER
#define WINVER 0x0400
#endif
    #define MS_WINDOWS      /* Used for Win 16 and Win32 */

    #if (_MSC_VER <= 800) 
        
        #define MS_WINDOWS_16   /* Used for Win 16 */

        #ifndef FAR
            #define FAR _far
        #endif

        #ifndef NEAR
            #define NEAR _near
        #endif

        #ifndef EXPORT
            #define EXPORT _export
        #endif


        #if !defined(PASCAL) && !defined(_WIN32)
            #define PASCAL _pascal
        #endif
    #endif /* _MSC_VER */


    #if defined(WIN32) && ! defined(VISION_NUTC)

        #if !defined(_PPC_) && !defined(_ALPHA_) && !defined(_MIPS_) && !defined(_X86_) && defined(_M_IX86)
            #define _X86_
        #endif

        #include <windef.h>

        #define MS_WINDOWS_32


        #if 0
            #include <windows.h>

            #ifndef FAR
            #define FAR
            #endif

            #ifndef NEAR
            #define NEAR
            #endif

            #ifndef PASCAL
            #define PASCAL
            #endif

            #ifndef EXPORT
            #define EXPORT
            #endif

            #ifndef _huge
            #define _huge
            #endif
        #endif
    #endif /*_WIN32 */

#else /* _MSC_VER */

        /*********** Unix Definitions ***********/

#define FAR 
#define NEAR 
#define PASCAL 
#define EXPORT

#endif /* _MSC_VER */

/*
 *  Conditionally define C_PLUSPLUS and set FULLPROTO if C_PLUSPLUS is defined
 */
#if defined(c_plusplus) || defined(__cplusplus) 
#if defined(MS_WINDOWS) || defined(HPUX) || defined(SOLARIS) || defined (OSF1) || defined(_AIX) || defined(VISION_NUTC) || defined(LINUX)
#define C_PLUSPLUS
#if !defined(FULLPROTO)
#define FULLPROTO
#endif
#endif
#endif

#if defined(LINUX)
#if defined(__ia64__)       /* Defeined by Itanium compiler */
#define LINUX64 1
#endif
#endif

#endif


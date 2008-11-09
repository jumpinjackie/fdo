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
*   time.h - header to decide which time header files to include.       *
*                                                                       *
* Synopsis                                                              *
*   #include    <Inc/time.h>                                            *
*                                                                       *
* Description                                                           *
*       This header file decides which `time' header file(s) are to be  *
*       to included for the system we are compiling on.                 *
*                                                                       *
* Parameters                                                            *
*   N/A                                                                 *
*                                                                       *
* Function value                                                        *
*   N/A                                                                 *
*                                                                       *
* Remarks                                                               *
*       New AMS code that uses any time functionality should            *
*       include this file not the system header files.                  *
*       This file should be verified to be correct for any new          *
*       Operating Systems (or versions).                                *
*                                                                       *
************************************************************************/

#ifndef TIME_H
#define TIME_H  1

#include <Inc/os.h>

#ifdef VMS
        /***    'timeval' structure for use of ut_tick on VMS   ***/
struct timeval {
    long    tv_sec;     /* seconds */
    long    tv_usec;    /* and microseconds */
};
#include    <time.h>

#else 

#ifdef MS_WINDOWS

#include    <time.h>

#else   /* UNIX */

#include    <sys/time.h>

#ifdef _AIX
#include    <time.h>
#endif  /* _AIX */

#endif  /* VMS */

#endif  /* TIME_H */
#endif               



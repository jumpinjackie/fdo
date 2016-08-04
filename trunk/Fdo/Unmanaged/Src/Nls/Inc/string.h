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
*   Name                                                                *
*       string.h - header file to include correct system include files. *
*                                                                       *
*   Synopsis                                                            *
*       #include <Inc/string.h>                                         *
*                                                                       *
*   Description                                                         *
*       This  header  defines  the macros and functions which are       *
*       required by many applications,  and  are  not application       *
*       specific.                                                       *
*                                                                       *
*   Parameters                                                          *
*       n/a                                                             *
*                                                                       *
*   Function value                                                      *
*       n/a                                                             *
*                                                                       *
*   Remarks                                                             *
*       This file should be included to define the `string' and         *
*       the `memory' families of system functions.  The ANSI C standard *
*       specifies its memory move functions be declared in <string.h>.  *
*       For non-ANSI C compilers the include file `memory.h' can be     *
*       included at compile time by defining `MEMORY'                   *
*                                                                       *
************************************************************************/

#ifndef STRING_H
#define STRING_H    1

#include    <string.h>

#ifdef  sun
    /* Include memory.h by default if we are
     * compiling on a SUN since it does not
     * have an ANSI C compiler at this time.
     * Once SUN comes out with an ANSI C compiler
     * remove the following include.
     */
#include    <memory.h>
#else
#ifdef  MEMORY
#include    <memory.h>
#endif  /* MEMORY */
#endif  /* sun */

#endif  /* STRING_H */


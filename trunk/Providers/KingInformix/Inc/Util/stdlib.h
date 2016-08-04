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
*   stdlib.h - ANSI C include files                                     *
*                                                                       *
* Synopsis                                                              *
*   #include    <inc/stdlib.h>                                          *
*                                                                       *
* Description                                                           *
*       This  header  decides  what  header  files should be used       *
*       depending on the ARCH i.e AIX, VMS, other.                      *
*                                                                       *
* Parameters                                                            *
*   N/A                                                                 *
*                                                                       *
* Function value                                                        *
*   N/A                                                                 *
*                                                                       *
* Remarks                                                               *
*                                                                       *
************************************************************************/

#ifndef STDLIB_H
#define STDLIB_H

#ifdef abs                                  /* stdlib.h declares `int abs()' */
#undef abs
#endif

#include <stdlib.h>

#ifdef _AIX
#include <malloc.h>                         /* for mallopt routines */
#elif   defined(sun)
#define labs(x) ((long) abs(((int) x)))     /* no labs() on SunOs 4.1.3 */
#endif

#endif  /* STDLIB_H */


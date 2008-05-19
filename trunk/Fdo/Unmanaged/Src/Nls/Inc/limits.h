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
*   Name                                                                *
*       limits.h  - header file to include correct system include       *
*       files.                                                          *
*                                                                       *
*   Synopsis                                                            *
*       #include <Inc/limits.h>                                         *
*                                                                       *
*   Description                                                         *
*       This  file   includes  declaration  of  memory  functions       *
*       in occordance with the underlying platform.                     *
*                                                                       *
*   Parameters                                                          *
*                                                                       *
*   Function value                                                      *
*                                                                       *
*   Remarks                                                             *
*                                                                       *
************************************************************************/

#ifndef INC_LIMITS_H
#define INC_LIMITS_H

#include <Inc/os.h>

#ifdef VISION_NUTC
#include <limits.h>

#define LOGNAME_MAX 8
#define MINUINT     UINT_MIN
#define MAXINT      INT_MAX
#define MAXUINT     UINT_MAX

#define MINULONG    ULONG_MIN
#define MAXLONG     LONG_MAX
#define MAXULONG    ULONG_MAX

#else

#ifdef unix
#ifdef HPUX
#ifdef MAXINT
#undef MAXINT
#endif
#endif
#include <limits.h>
#else
#ifdef MS_WINDOWS
#include <limits.h>

#include <winnt.h>
 
#define MININT      INT_MIN
#define MINUINT     UINT_MIN
#define MAXINT      INT_MAX
#define MAXUINT     UINT_MAX
 
#define MINULONG    ULONG_MIN
#define MAXULONG    ULONG_MAX


#endif
#endif

#endif

#endif /* INC_LIMITS_H */
    


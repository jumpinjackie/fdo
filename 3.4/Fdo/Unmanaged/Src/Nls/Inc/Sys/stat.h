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
*       Inc/Sys/stat.h  - System stat header file
*       files.                                                          *
*                                                                       *
*   Synopsis                                                            *
*       #include <Inc/Sys/stat.h>                                       *
*                                                                       *
*   Description                                                         *
*       This  file   includes  declaration  of file stat structures     *
*       abd the associated macros and prototypes.                       *
*                                                                       *
*   Parameters                                                          *
*                                                                       *
*   Function value                                                      *
*                                                                       *
*   Remarks                                                             *
*                                                                       *
************************************************************************/

#ifndef INC_SYS_STAT_H_DEFINED
#define INC_SYS_STAT_H_DEFINED

#include <Inc/os.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(MS_WINDOWS)

#define S_ISDIR(mode)   ((mode&_S_IFDIR) == _S_IFDIR)

/* #define stat _stat; */

#endif

#endif /* INC_MEMORY_H */
    


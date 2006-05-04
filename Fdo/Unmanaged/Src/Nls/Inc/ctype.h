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
*   ctype.h                                                             *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/ctype>                                                *
*                                                                       *
* Description                                                           *
*                                                                       *
*                                                                       *
************************************************************************/
#ifndef INC_CTYPE_H
#define INC_CTYPE_H
#include <ctype.h>

#ifdef FULLPROTO
#ifdef OSF1
#ifdef isdigit

/*
 * undefine these macros that end up calling function pointers with no 
 * prototypes. only on OSF1.
 */
#undef isalpha
#undef isupper
#undef islower
#undef isdigit
#undef isxdigit
#undef isspace
#undef ispunct
#undef isalnum
#undef isprint
#undef isgraph
#undef iscntrl
 
#undef toupper
#undef tolower

#endif /* isdigit */
#endif /* OSF1 */
#endif /* FULLPROTO */
#endif /*INC_CTYPE_H */


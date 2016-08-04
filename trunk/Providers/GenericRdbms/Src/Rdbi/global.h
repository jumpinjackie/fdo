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
* Name                                                                  *
*   rdbi global.h - global/external definitions for rdbi package        *
*                                                                       *
* Synopsis                                                              *
*   #include "global.h"                                                 *
*                                                                       *
* Description                                                           *
*       Contains definitions for all global variables used by the       *
*       rdbi  package.   To  define  these  variables  as global,       *
*       define  the  constant  RDBI_GLOBAL;  otherwise they'll be       *
*       defined as external.                                            *
*                                                                       *
* Remarks                                                               *
*                                                                       *
************************************************************************/

#ifndef RDBI_GLOBAL_INCLUDED
#define RDBI_GLOBAL_INCLUDED

#include <Inc/Nls/rdbi_msg.h>
#include <Inc/rdbi.h>               /* RDBI_MAX_CONNECTS    */
#include <Inc/Rdbi/connect.h>       /* rdbi_connect_def     */
#include <Inc/Rdbi/cursor.h>        /* rdbi_cursor_def      */

#ifdef RDBI_GLOBAL

#define SCOPE
#define INIT_ZERO = 0
#define INIT_NULL = NULL

#else

#define SCOPE extern
#define INIT_ZERO
#define INIT_NULL

#endif

#define RDBI_CONTEXT_MALLOC_COUNT 16

void rdbi_nom_db();

#endif


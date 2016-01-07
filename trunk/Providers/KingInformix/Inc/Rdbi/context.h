#ifndef RDBI_CONTEXT_H
#define RDBI_CONTEXT_H

/***************************************************************************
 * Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 ***************************************************************************/

#include <Inc/Rdbi/connect.h>
#include <Inc/Rdbi/methods.h>
#include <ctype.h>
#ifndef _WIN32
#include <wchar.h>
#endif

#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

#ifndef NULL
#define NULL    0
#endif

typedef struct
{
/* globals */
    int rdbi_initialized;

/* One definition per connection        */
    rdbi_connect_def  rdbi_connection[RDBI_MAX_CONNECTS];

 /* a pointer to the active connect      */
    rdbi_connect_def *rdbi_cnct;

/* available cursors (all connects)     */
    rdbi_cursor_def **rdbi_cursor_ptrs;
    int               rdbi_num_cursor_ptrs;
    int rdbi_last_status;

/* vis_owner.c */
    char  vision_owner[32];

    void *drvr;
    rdbi_methods_def dispatch;

	wchar_t*	last_error_msg;
} rdbi_context_def;

#endif


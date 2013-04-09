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
 *
 ***************************************************************************/

#include <Inc/Rdbi/context.h>
#include <memory.h>  
int rdbi_init_context(rdbi_context_def *context)
{
/* globals */
    context->rdbi_initialized = 0;

/* One definition per connection        */
    memset(context->rdbi_connection, 0, sizeof(context->rdbi_connection));

 /* a pointer to the active connect      */
    context->rdbi_cnct = NULL;

/* available cursors (all connects)     */
    context->rdbi_cursor_ptrs = NULL;
    context->rdbi_num_cursor_ptrs = 0;
    context->rdbi_last_status = 0;

/* vis_owner.c */
    context->vision_owner[0] = '\0';

	context->last_error_msg = NULL;

    return TRUE;
}


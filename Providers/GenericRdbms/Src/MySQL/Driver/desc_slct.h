/*
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
 */

#ifndef DESC_SLCT_H
#define DESC_SLCT_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

int mysql_desc_slct (
    mysql_context_def *context,
    char *cursor,
    int   position,     /* position within select clause    */
    int   name_size,    /* The maximum size of name         */
    char *name,         /* Name of this field               */
    int  *rdbi_type,    /* rdbi constant                    */
    int  *binary_size,  /* bytes of memory to hold it       */
    int  *null_ok);     /* TRUE iff NULLs ok in this column */

#endif // DESC_SLCT_H

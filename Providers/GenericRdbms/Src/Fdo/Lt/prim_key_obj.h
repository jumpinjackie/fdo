/************************************************************************
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
* Name                                                                  *
*   dbi_prim_key_obj - Structure to hold a primary key object           *
*                                                                       *
* Synopsis                                                              *
*   None.                                                               *
*                                                                       *
* Description                                                           *
*   This header file defines the structure "dbi_prim_key_obj" to store  *
*   primary key information. The structure consists of the column name  *
*   that represents or is part of the primary key, its value and a link *
*   to the next primary key object (in case a primary key consist of    *
*   multiple columns).                                                  *
*                                                                       *
************************************************************************/

#ifndef DBI_PRIM_KEY_OBJ
#define DBI_PRIM_KEY_OBJ

typedef struct dbi_prim_key_obj_struct {

    FdoString                           column_name[GDBI_COLUMN_NAME_SIZE+1];
    FdoString                           *column_value;
    int                            data_type;
    struct dbi_prim_key_obj_struct *next;

} dbi_prim_key_obj;

#endif

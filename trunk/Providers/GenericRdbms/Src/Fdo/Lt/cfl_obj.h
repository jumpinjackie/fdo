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
*   dbi_cfl_obj - Structure to hold one long transaction conflict       *
*                                                                       *
* Synopsis                                                              *
*   None.                                                               *
*                                                                       *
* Description                                                           *
*   The header file defines the structure "dbi_cfl_obj" to store a long *
*   transaction conflict. The structure consists of the primary key for *
*   the object causing the conflict, its class name, the row id, a flag *
*   to indicate the user's request on how to solve the issue and a link *
*   to the next long transaction conflict object.                       *
*                                                                       *
************************************************************************/

#ifndef DBI_CFL_OBJ
#define DBI_CFL_OBJ

#include "../Lt/prim_key_obj.h"

typedef struct dbi_cfl_obj_struct {

    char                      *class_name;
    char                      row_id[65];
    int                       solution_flag;
    dbi_prim_key_obj          *primary_key_info_root;
    struct dbi_cfl_obj_struct *next;

} dbi_cfl_obj;

#endif

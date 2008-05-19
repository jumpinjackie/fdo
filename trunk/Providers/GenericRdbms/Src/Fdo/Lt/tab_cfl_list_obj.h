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
*   dbi_tab_cfl_list_obj - Long transaction conflict list for a table   *
*                                                                       *
* Synopsis                                                              *
*   None                                                                *
*                                                                       *
* Description                                                           *
*   The header file defines the structure "dbi_tab_cfl_obj_list" to     *
*   long transaction conflicts for a table. The structure consists of   *
*   the table name, a link to the long transaction conflicts and a link *
*   to the next object in the list.                                     *
*                                                                       *
************************************************************************/

#ifndef DBI_TAB_CFL_OBJ_LIST
#define DBI_TAB_CFL_OBJ_LIST

#include "../Lt/cfl_obj.h"
#include "../../Gdbi/GdbiTypes.h"

typedef struct dbi_tab_cfl_list_obj_struct {

    char                               table_name[GDBI_TABLE_NAME_SIZE+1];
    char                               table_owner[GDBI_TABLE_NAME_SIZE+1];
    dbi_cfl_obj                        *lt_conflicts_root;
    struct dbi_tab_cfl_list_obj_struct *next;

} dbi_tab_cfl_list_obj;

#endif

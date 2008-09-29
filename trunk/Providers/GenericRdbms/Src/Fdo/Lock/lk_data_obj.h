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
*
* Name                                                                  *
*   dbi_lk_data_obj - Structure to hold lock related data               *
*                                                                       *
* Synopsis                                                              *
*   None.                                                               *
*                                                                       *
* Description                                                           *
*   The header file defines the structure "dbi_lk_data_obj". It allows  *
*   the storage of lock related data for or independent of an object.   *
*   The structure caontains a primary key list property, a class name   *
*   property and a lock type and -owner property. The primary key list  *
*   and class name property are used only if the data stored in the     *
*   structure is related to an object. The structure also has a pointer *
*   to another object of this type in case multiple information units   *
*   need to be connected.                                               *
*                                                                       *
************************************************************************/

#ifndef DBI_LK_DATA_OBJ
#define DBI_LK_DATA_OBJ

#include <../Lt/prim_key_obj.h>

typedef struct dbi_lk_data_obj_struct {

    // Properties to communicate lock data.

    int                           lock_type;
    char                          *lock_owner;
    char                          *class_name;
    char                          *schema_name;
    char                          *lt_name;
    dbi_prim_key_obj              *primary_key_info_root;
    struct dbi_lk_data_obj_struct *next;

    // Properties allowing DBI to handle cursors.
    // ------------------------------------------------------------------------
    // NOTE: If the structure is used to retrieve data, DBI initiates the
    //       request. This means that it also allocates cursors for the
    //       queries involved in the process. The process itself is splitted
    //       into an activate, get and terminate part. Each of them is imple-
    //       mented as a separate DBI call. To correctly address the query
    //       each of the three functions must have the correct cursor id.
    //       Storing the cursor information with the context is not an option
    //       as the user may generate multiple commands that would need the
    //       information stored in the context at the same time. Therefore,
    //       it has to be handed over from the command to the DBI function.
    //       As a result, it is stored within this structure which is unique
    //       to each command.
    // ------------------------------------------------------------------------

    int                           tbl_qry_qid;
    int                           tbl_lst_qry_qid;
    char                          *table_name;

} dbi_lk_data_obj;

#endif

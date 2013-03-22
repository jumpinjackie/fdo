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
*   dbi_version_info.h - Header for typedef of dbi_version_info_def.    *
*                                                                       *
* Synopsis                                                              *
*   #include <inc/dbi/version_info.h>                                   *
*                                                                       *
* Description                                                           *
*   This is the header file for the typedef of "dbi_version_info_def"   *
*   which is a structure containing version information.                *
*                                                                       *
s*   FILE_DEPENDENCY(LTCART): WARNING! This code is duplicated in PL/SQL.
************************************************************************/

#ifndef VERSION_INFO_DEFINED
#define VERSION_INFO_DEFINED

#include "../../Gdbi/GdbiTypes.h"

#define VERSION_NAME_LENGTH      30         /* length of a version name */
#define VERSION_NAME_SIZE        32
#define VERSION_DESC_SIZE       255         /* size of a version description */
#define VERSION_DESC_RET_SIZE  1000         /* size of a version description in WM */

#define VERSION_TYPE_NORMAL             ' '            /* scenario types */
#define VERSION_TYPE_SCENARIO_SET       'S'
#define VERSION_TYPE_BASE_SCENARIO      'B'
#define VERSION_TYPE_INTERNAL_SCENARIO  'I'

#define VERSION_ROOT_LT_ID              0
#define VERSION_ROOT_LT_NAME            "_ROOT_"

typedef long dbi_vn_id_t;

typedef struct dbi_version_info_def {

    /* ------------------------------------------------------------------- */
    /*                     Common structure properties                     */
    /* ------------------------------------------------------------------- */
    char name[VERSION_NAME_SIZE+1];
    char date[GDBI_DATE_TIME+1];
    char owner[VERSION_NAME_SIZE+1];
    char description[VERSION_DESC_RET_SIZE+1];

    /* ------------------------------------------------------------------- */
    /*                Workspace Manager related properties                 */
    /* ------------------------------------------------------------------- */
    int  is_active;
    int  is_frozen;
    int  is_refreshed;
    int  multi_parent;
    int  is_auto_commit;

    /* ------------------------------------------------------------------- */
    /*                Traditional Schema related properties                */
    /* ------------------------------------------------------------------- */
    char type;
    long version;
    long scenario;
    long row_lock;
    long row_lock_type;

} dbi_version_info_def;

#endif

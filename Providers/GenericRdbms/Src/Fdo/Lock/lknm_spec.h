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
*                                                                       *
* Name                                                                  *
*   dbi_lknm_spec.h - Header for typedef of dbi_lknm_spec_def.          *
*                                                                       *
* Synopsis                                                              *
*   dbi_lknm_spec.h ()                                                  *
*                                                                       *
* Description                                                           *
*       This   is   the   header   file   for   the   typedef  of       *
*       "dbi_lknm_spec_def"  which  is  a   structure  containing       *
*       lock name definition information.                               *
*                                                                       *
* Remarks                                                               *
*       None.                                                           *
*                                                                       *
*   FILE_DEPENDENCY(LTCART): WARNING! This code is duplicated in PL/SQL.
************************************************************************/
#ifndef DBI_LKNM_SPEC_DEFINED   /* skip if already defined */
#define DBI_LKNM_SPEC_DEFINED 1

#define DBI_LKNM_LENGTH         30      /* lock name length */
#define DBI_LKNM_SIZE           32      /* lock name size */
#define DBI_LKNM_DESC_LENGTH    80      /* lock name description length */
#define DBI_LKNM_DESC_SIZE      82      /* lock name description size */

typedef long dbi_li_id_t;

typedef struct dbi_lknm_spec_def {

    char        lock_name[DBI_LKNM_SIZE+1];
    dbi_li_id_t lock_id;
    char        owner[GDBI_DB_NAME_SIZE+1];
    char        creation_date[GDBI_DATE_TIME+1];
    char        description[DBI_LKNM_DESC_LENGTH+1];

} dbi_lknm_spec_def;

#endif

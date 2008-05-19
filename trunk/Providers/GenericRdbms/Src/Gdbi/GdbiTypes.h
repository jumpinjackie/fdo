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
#ifndef _GDBITYPES_H_
#define _GDBITYPES_H_

#ifdef USE_NONRDBMS_HEADER
#include <inc/rdbi.h>
#else
#include <Inc/rdbi.h>
#endif

#define         GDBI_NI_TYPE    long

#define         GDBI_DB_NAME_SIZE               32
#define         GDBI_COLUMN_NAME_SIZE           RDBI_COLUMN_NAME_SIZE
#define         GDBI_TABLE_NAME_SIZE            RDBI_TABLE_NAME_SIZE
#define         GDBI_SCHEMA_ELEMENT_NAME_SIZE   256         /* size of a schema, class or property name */
#define         GDBI_MAXIMUM_TEXT_SIZE          4000
#define         GDBI_DATE_TIME                  63
#define	        GDBI_MAXIMUM_STRING_SIZE		4000
/* define reserved identifiers */


/************************************************************************
*  The information we can determine from the selection syntax.          *
************************************************************************/
typedef struct
{
    char  owner[GDBI_DB_NAME_SIZE];      /* Table owner (if specified)       */

                                        /* col. name or expression syntax   */
    char  column[GDBI_COLUMN_NAME_SIZE]; /* if expression, may be truncated  */
    char  c_alias[GDBI_COLUMN_NAME_SIZE];/* col alias, if specified          */

    char  table[GDBI_TABLE_NAME_SIZE];   /* table name if !is_expression     */
    char  t_alias[GDBI_TABLE_NAME_SIZE]; /* table alias, if specified        */

    int   is_expression;                /* TRUE iff not a simple column     */


    /* The following elements are loaded using rdbi_slct(). This means
       they are obtained directly from the RDBMS.
    */

    int   datatype;                     /* Inc/rdbi.h - rdbi data types     */
    int   size;                         /* #bytes to store binary type      */
    int   null_ok;                      /* Null value indicator useful      */

} GdbiColumnDesc;


typedef struct gdbi_feat_info_def
{
    long        feat_num;           /* feature number */
    long        classid;            /* class id */
    long        changeseq;
    int         geom_type;          /* point, line, poly, node */
    int         coord_type;
    long        plan;               /* plan number */
    long        version;            /* version number */
    long        next_version;       /* next dependent version */
    long        row_lock;           /* row lock id, 0 if none */
    char        row_lock_type;      /* S,E,V=shared,exclusive,version exclusive lock */
} gdbi_feat_info_def;

#endif

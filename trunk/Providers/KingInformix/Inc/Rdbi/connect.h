// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef RDBI_CONNECT_H
#define RDBI_CONNECT_H

/************************************************************************
* Name                                                                  *
*   connect.h - contains typedef for rdbms connections                  *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/Rdbi/connect.h>                                       *
*                                                                       *
* Description                                                           *
*       Keep  track of all connection information - including the       *
*       vendor-specific data, the active transaction information,       *
*       and context areas for SQL transactions.                         *
*                                                                       *
* Remarks                                                               *
*                                                                       *
************************************************************************/

#include <Inc/rdbi.h>           /* RDBI_DATASET_NAME_SIZE   */
#include <Inc/Rdbi/cursor.h>    /* rdbi_cursor_def          */

typedef struct tran_entry_def {
    char                     tran_id[32];
    struct tran_entry_def   *next;
    int                      tran_ended;
} tran_entry_def;

typedef struct rdbi_connect_def {
    char   *vendor_data;                        /* rdbms-specific information               */
    int     in_use;                             /* TRUE if this connect structure allocated */
    int     connect_id;                         /* identifies db connection (vendor generated ) */
    char    cuserid[RDBI_DATASET_NAME_SIZE];    /* OS user name                             */
    int     uncommitted_data;                   /* TRUE when an update transaction active   */
    int     read_only;                          /* TRUE when transaction is read-only       */
    int     autocommit_on;                      /* TRUE if in autocommit mode               */
    tran_entry_def  *tran_head;                 /* head of transaction id stack             */
} rdbi_connect_def;

#endif



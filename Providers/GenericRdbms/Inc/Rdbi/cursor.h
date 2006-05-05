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

/************************************************************************
* Name                                                                  *
*   cursor.h - define typedef for the SQL transaction working area      *
*                                                                       *
* Synopsis                                                              *
*   #include <inc/rdbi/cursor.h>                                        *
*                                                                       *
* Description                                                           *
*       Defines  the  typedef  which contains all the information       *
*       necessary to maintain an active SQL statement.                  *
*                                                                       *
* Remarks                                                               *
*                                                                       *
************************************************************************/

#ifndef RDBI_CONTEXT_DEFINED
#define RDBI_CONTEXT_DEFINED

/************************************************************************
*       We define out own Control Area in which to administer the       *
*       processing of SQL statements.  The elements of the cursor       *
*       area which are frequently  checked  by  applications  are       *
*       explicitly  itemized,  so  they  can  be referenced in  a       *
*       generic manner.                                                 *
************************************************************************/

#define VERB_LEN    31

typedef struct rdbi_cursor_def {
    char               *vendor_data;        /* Vendor specific cursor area              */
#ifdef _DEBUG
    char               *sql;                /* sql string                               */
#endif
    char                verb[VERB_LEN+1];   /* SQL command verb (1st word in stmt)      */
    int                 status;             /* our own translated status                */
    int                 sql_parsed;         /* TRUE if rdbi_sql() has been called       */
    int                 sel_for_update;     /* TRUE if "select ... for update" stmt     */
    int                 exec_coc_legal;     /* TRUE if sel_for_update && we have a row  */
    int                 bound_vars;         /* TRUE if input variables are bound        */
    int                 defined_vars;       /* TRUE if output variables are defined     */
    int                 n_executions;       /* # times they've called rdbi_execute      */
    int                 rows_processed;     /* rows inserted/updated/fetched/deleted
                                               since last rdbi_execute()                */
    long                trows_processed;    /* total number of rows processed for all
                                               rdbi_execute()                           */
    int                 statement_type;     /* insert/update/delete/select/create/
                                                grant/alter/drop ...                    */
    int                 sql_index;          /* index to external SQL file               */
    int                 tran_begun;         /* transaction begun on this cursor         */
    int                 trace_line;         /* index into trace file                    */
} rdbi_cursor_def;

/*
 *  The valid values for tran_begun are FALSE, TRUE and
 *  RDBI_TRAN_EOF.  The latter indicates that a transaction
 *  is active and that the query has hit end of fetch but
 *  the caller hasn't been notified of that yet.
 */
#define RDBI_TRAN_EOF   (2)

#endif


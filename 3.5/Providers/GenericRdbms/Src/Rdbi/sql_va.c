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
*                                                                       *
* Name                                                                  *
*   rdbi_sql_va - parse/bind/define/execute in one varargs(3) call.     *
*                                                                       *
* Synopsis                                                              *
*   rdbi_sql_va(opts, sqlid, sql_str,                                   *
*     [[bind_name,] bind_type, bind_size, bind_var, [bind_nl_ind,]]...  *
*     RDBI_VA_EOL,                                                      *
*     [[defn_name,] defn_type, defn_size, defn_var, [defn_nl_ind,]]...  *
*     RDBI_VA_EOL                                                       *
*     [,exec_count, exec_offset],                                       *
*     [,fetch_count, rows_processed] )                                  *
*   int         opts;                                                   *
*   int         sqlid;                                                  *
*   char        *sql_str;                                               *
*                                                                       *
*   char        *bind_name;                                             *
*   int         bind_type;                                              *
*   int         bind_size;                                              *
*   void        *bind_var;                                              *
*   short       *bind_nl_ind;                                           *
*                                                                       *
*   char        *defn_name;                                             *
*   int         defn_type;                                              *
*   int         defn_size;                                              *
*   void        *defn_var;                                              *
*   short       *defn_nl_ind;                                           *
*                                                                       *
*   int         exec_count;                                             *
*   int         exec_offset;                                            *
*                                                                       *
*   int         fetch_count;                                            *
*   int         *rows_processed;                                        *
*                                                                       *
* Description                                                           *
*       Parse sql, define output variables, bind input variables,       *
*       and  optionally  execute  the  statement all in one call.       *
*       Can also fetch the first array of results if requested and      *
*       immediately cancel the query.                                   *
*                                                                       *
*       This    is    a    varargs(3)    interface     to     the       *
*       rdbi_sql/bind/define/execute/fetch  sequence of  calls.   The   *
*       optional arguments  are  controlled  by flags in the opts       *
*       argument.  If the corresponding option  bit  is  set then       *
*       the argument is required.                                       *
*                                                                       *
*       The possible values for the options are some  combination       *
*       of:                                                             *
*                                                                       *
*           RDBI_VA_BNDNAMES  -- bind variable names are specified      *
*                                                                       *
*           RDBI_VA_BNDNULLS  -- bind null indicator variables are      *
*                                specified                              *
*                                                                       *
*           RBDI_VA_BNDALL    -- all bind args are specified            *
*                                                                       *
*           RDBI_VA_DEFNAMES  -- define variable names are specified    *
*                                                                       *
*           RDBI_VA_DEFNULLS  -- define null indicator variables        *
*                                are specified                          *
*                                                                       *
*           RBDI_VA_DEFALL    -- all define args are specified          *
*                                                                       *
*           RDBI_VA_ALLNULLS  -- both define and bind null indicators   *
*                                are specified                          *
*                                                                       *
*           RDBI_VA_ALLNAMES  -- both define and bind names are         *
*                                specified                              *
*                                                                       *
*           RDBI_VA_NEXEC     -- execute the statement with specified   *
*                                count, offset                          *
*                                                                       *
*           RDBI_VA_EXEC      -- execute the statement with count 1,    *
*                                offset 0                               *
*                                                                       *
*           RDBI_VA_NFETCH    -- fetch the statement with specified     *
*                                count                                  *
*                                                                       *
*           RDBI_VA_FETCH     -- fetch the statement with count 1       *
*                                                                       *
*           RDBI_VA_ENDSEL    -- perform auto-end_select after fetch
*                                                                       *
*       The bind variable and define variable definition lists          *
*       are  both  terminated   by   specifying  the  RDBI_VA_EOL       *
*       argument.                                                       *
*                                                                       *
*       Note that the variable name  or null indicator parameters       *
*       should  not  be  specified in the  arg  list  unless  the       *
*       corresponding   *NAMES   or  *NULLS   option   has   been       *
*       specified.                                                      *
*                                                                       *
*       If *NAMES is  not specified, then the variables are given       *
*       numeric names starting at  1 and increasing by 1 for each       *
*       variable defined/bound.  The count is  reset  to 1 at the       *
*       start of both the define and bind lists.                        *
*                                                                       *
*       If  *NULLS  is  not  specified, then the  null  indicator       *
*       variables default to no null indicators.                        *
*                                                                       *
*       An example usage of this interface would be:                    *
*                                                                       *
*           rc = rdbi_sql_va(                                           *
*               RDBI_VA_DEFNAMES|RDBI_VA_BNDNULLS|RDBI_VA_NEXEC,        *
*               sqlid, sql,                                             *
*               RDBI_INT,    0, &bnd1, &nl_bnd1,                        *
*               RDBI_INT,    0, &bnd2, &nl_bnd2,                        *
*               RDBI_STRING, 0,  bnd3, &nl_bnd3,                        *
*               RDBI_VA_EOL,                                            *
*               "1", RDBI_INT, 0, &def1,                                *
*               "2", RDBI_INT, 0, &def2,                                *
*               RDBI_VA_EOL,                                            *
*               1, 0);                                                  *
*                                                                       *
* Parameters                                                            *
*   opts  - Input                                                       *
*       A bit mask of options.   This can affect which subsequent       *
*       arguments are expected.                                         *
*                                                                       *
*   sqlid - Input                                                       *
*       A valid RDBI cursor.                                            *
*                                                                       *
*   sql_str - Input                                                     *
*       The  RDBI  SQL  statement  to parse,  bind,  define,  and       *
*       optionally execute.                                             *
*                                                                       *
*   bind_name - Input                                                   *
*       Bind  variable  name  string.   Only   required   if  the       *
*       RDBI_VA_BNDNAMES option is set in opts.                         *
*                                                                       *
*   bind_type - Input                                                   *
*       The RDBI internal type of the bind variable.                    *
*                                                                       *
*   bind_size - Input                                                   *
*       The size in bytes of the bind  variable.   Ignored if the       *
*       bind_type  doesn't  require it, but it must be specified.       *
*                                                                       *
*   bind_var - Input                                                    *
*       The address of the bind variable.                               *
*                                                                       *
*   bind_nl_ind - Input                                                 *
*       The address of  the  bind  variable null indicator.  Only       *
*       required if the RDBI_VA_BNDNULLS option is  set  in opts.       *
*                                                                       *
*   defn_name - Input                                                   *
*       Define  variable  name  string.   Only  required  if  the       *
*       RDBI_VA_DEFNAMES option is set in opts.                         *
*                                                                       *
*   defn_type - Input                                                   *
*       The RDBI internal type of the define variable.                  *
*                                                                       *
*   defn_size - Input                                                   *
*       The size in bytes of the define variable.  Ignored if the       *
*       defn_type  doesn't require it, but it must be  specified.       *
*                                                                       *
*   defn_var - Input                                                    *
*       The address of the define variable.                             *
*                                                                       *
*   defn_nl_ind - Input                                                 *
*       The address  of the define variable null indicator.  Only       *
*       required if the  RDBI_VA_DEFNULLS  option is set in opts.       *
*                                                                       *
*   exec_count - Input                                                  *
*       The   array  execute  count.   Only   required   if   the       *
*       RDBI_VA_NEXEC option  is  specified in opts.  It defaults       *
*       to 1 if RDBI_VA_EXEC is instead specified in opts.              *
*                                                                       *
*   exec_offset - Input                                                 *
*       The  array  execute  offset.    Only   required   if  the       *
*       RDBI_VA_NEXEC  option is specified in opts.  It  defaults       *
*       to 0 if RDBI_VA_EXEC is instead specified in opts.              *
*                                                                       *
*   fetch_count - Input                                                 *
*       The   array  fetch  count.   Only   required     if   the       *
*       RDBI_VA_NFETCH option  is  specified in opts.                   *
*                                                                       *
* Function value                                                        *
*       RDBI_SUCCESS  if   successful;   an   RDBI   error   code       *
*       otherwise.                                                      *
*                                                                       *
* Remarks                                                               *
*       This     is     a    varargs(3)    interface    to    the       *
*       rdbi_sql/bind/define/execute/fetch sequence of RDBI calls.  It  *
*       just provides a  more compact interface to those routines       *
*       by reducing the need  for  repeated  error  checks and by       *
*       making less often used arguments optional.                      *
*                                                                       *
*       The  current interface doesn't report the exact phase  or       *
*       column that  produces  an error, but this could easily be       *
*       rectified with a new  RDBI_VA_RPTERR option that requires       *
*       phase  and  col_num  pointer  arguments  to  return  that       *
*       infomation.  If needed this can  easily  be  added  at  a       *
*       later time.                                                     *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.        *
*                                                                       *
*                                                                       *
************************************************************************/


#include <stdio.h>
extern char *rdbi_cat;

#include    <Inc/Nls/rdbi_msg.h>
#include <Inc/ut.h>
#include <Inc/debugext.h>
#include <Inc/rdbi.h>
#include <Inc/Rdbi/proto.h>
#include <stdarg.h>
#include "global.h"                     /* rdbi global area                 */
#include	<Inc/Rdbi/context.h>

#ifndef _WIN32
#define _stricmp  strcasecmp
#endif

/*VARARGS3*/
int local_rdbi_sql(rdbi_context_def *context, int sqlid, rdbi_string_def *sql, int defer);

int local_rdbi_sql_va(rdbi_context_def *context, int opts, int sqlid, rdbi_string_def* sql, va_list args)
{
    char    *name;
    int     type, size;
    char    *addr;
    void    *n_addr;

    int     count, offset, *rows_proc;

    int     exec_done = FALSE;
    int     idx, rc;
    char    tname[10];

    debug_on("rdbi_sql_va");

    /*
     *  First, parse the SQL statement (defered parse).
     */
    rc = local_rdbi_sql(context, sqlid, sql, TRUE);
    if(rc != RDBI_SUCCESS) goto the_exit;

    /*
     *  Now process the bind variable specifications.
     */
    for(idx=1; ; idx++) {
        if(opts & RDBI_VA_BNDNAMES) {
            name   = va_arg(args, char *);
            if(name == RDBI_VA_EOL)
                break;
        } else {
            (void)sprintf(tname, "%d", idx);
            name = tname;
        }
        type   = va_arg(args, int);
        if((opts & RDBI_VA_BNDNAMES) == 0 && type == RDBI_VA_EOL)
            break;
        size   = va_arg(args, int);
        addr   = va_arg(args, char *);
        if(opts & RDBI_VA_BNDNULLS)
            n_addr = va_arg(args, char *);
        else
            n_addr = NULL;

        rc = rdbi_bind(context, sqlid, name, type, size, addr, n_addr);
        if(rc != RDBI_SUCCESS) goto the_exit;
    }

    /*
     *  Now process the define variable specifications.
     */
    for(idx=1; ; idx++) {
        if(opts & RDBI_VA_DEFNAMES) {
            name   = va_arg(args, char *);
            if(name == RDBI_VA_EOL)
                break;
        } else {
            (void)sprintf(tname, "%d", idx);
            name = tname;
        }
        type   = va_arg(args, int);
        if((opts & RDBI_VA_DEFNAMES) == 0 && type == RDBI_VA_EOL)
            break;
        size   = va_arg(args, int);
        addr   = va_arg(args, char *);
        if(opts & RDBI_VA_DEFNULLS)
            n_addr = va_arg(args, char *);
        else
            n_addr = NULL;

        rc = rdbi_define(context, sqlid, name, type, size, addr, n_addr);
        if(rc != RDBI_SUCCESS) goto the_exit;
    }

    /*
     *  Now, execute the statement (if requested)
     */
    if(opts & RDBI_VA_NEXEC) {
        count  = va_arg(args, int);
        offset = va_arg(args, int);
    } else {
        int is_select = FALSE;
        rdbi_cursor_def *cursor = (rdbi_cursor_def *) NULL;
        cursor = context->rdbi_cursor_ptrs[sqlid];
		is_select = (_stricmp(cursor->verb, "select") == 0);
        /*
         * "count" is polymophic:
         *   - for SQL "select", it means # of rows (we want zero on execute)
         *   - for other verbs, it means # of executions (we want exactly one)
         */
        count  = is_select ? 0 : 1;
        offset = 0;
    }
    if(opts & (RDBI_VA_EXEC|RDBI_VA_NEXEC)) {
        rc = rdbi_execute(context, sqlid, count, offset);
        if(rc != RDBI_SUCCESS) goto the_exit;
        exec_done = TRUE;
    }

    /*
     *  Finally, fetch the first row of results (if requested)
     *  Note this will also execute query unless done above.
     */
    if(opts & RDBI_VA_NFETCH) {
        count     = va_arg(args, int);
        rows_proc = va_arg(args, int *);
    } else {
        count     = 1;
        rows_proc = NULL;;
    }
    if(opts & (RDBI_VA_FETCH|RDBI_VA_NFETCH)) {
        if(exec_done)
            rc = rdbi_fetch(context, sqlid, count, rows_proc);
        else {
            rc = rdbi_exec_fetch(context, sqlid, count,
                                    (opts&RDBI_VA_ENDSEL)!=0, rows_proc);
        }
        if(rc != RDBI_SUCCESS) goto the_exit;

        /*
         *  Do automatic end select, if requested.
         */
        if(opts & RDBI_VA_ENDSEL) {
            rc = rdbi_end_select(context, sqlid);
            if(rc != RDBI_SUCCESS) goto the_exit;
        }
    }

the_exit:

    debug_return(NULL, rc);
}

int rdbi_sql_va(rdbi_context_def *context, int opts, int sqlid, const char* sql, ...)
{
    rdbi_string_def str;
    int rc;

    va_list args;
    va_start(args, sql);
    
    str.ccString = sql;
    rc = local_rdbi_sql_va(context, opts, sqlid, &str, args);

    va_end(args);
    return rc;
}

int rdbi_sql_vaW(rdbi_context_def *context, int opts, int sqlid, const wchar_t* sql, ...)
{
    rdbi_string_def str;
    int rc;

    va_list args;
    va_start(args, sql);
    
    str.cwString = sql;
    rc = local_rdbi_sql_va(context, opts, sqlid, &str, args);

    va_end(args);
    return rc;
}


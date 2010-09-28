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
*   rdbi_sql - Parse an SQL statement                                   *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   int rdbi_sql(sqlid, sql)                                            *
*   int   sqlid;                                                        *
*   char *sql;                                                          *
*                                                                       *
* Description                                                           *
*       Call  the  RDBMS  routine to parse an SQL statement.  Any       *
*       parse errors will result in routine failure.                    *
*                                                                       *
*       We  do  some  string  substitutions  for  known  keywords       *
*       (RDBI_DB_NAME)                                                  *
*                                                                       *
* Parameters                                                            *
*   sqlid: input                                                        *
*       Index into internal global array of cursor ptrs.                *
*                                                                       *
*   sql: input                                                          *
*       String containing SQL statement.                                *
*                                                                       *
* Function value                                                        *
*       TRUE for success; FALSE  for  failure.  No Message issued       *
*       on  parse failure ! (A message  could  be  issued  for  a       *
*       malloc failure)                                                 *
*                                                                       *
* Remarks                                                               *
*       To issue  a  message for an error, call rdbi_msg_issue().       *
*                                                                       *
*       This routine updates the rdbi_last_status global                *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.            *
*                                                                       *
************************************************************************/


#include <Inc/ut.h>                     /* ut_isspace()                     */
#include <Inc/Nls/rdbi_msg.h>
#include <Inc/ut.h>
#include <Inc/debugext.h>
#include "global.h"                     /* rdbi global area                 */
#include <Inc/Rdbi/proto.h>

#include <Inc/Rdbi/context.h>

int local_rdbi_sql(rdbi_context_def *context, int sqlid, rdbi_string_def *sql, int defer);

int rdbi_sql(
	rdbi_context_def *context,
    int     sqlid,
    const char *sql )
{
    rdbi_string_def str;
    str.ccString = sql;
    return local_rdbi_sql(context, sqlid, &str, FALSE);
}

int rdbi_sqlW(
	rdbi_context_def *context,
    int     sqlid,
    const wchar_t *sql )
{
    rdbi_string_def str;
    str.cwString = sql;
    return local_rdbi_sql(context, sqlid, &str, FALSE);
}

int rdbi_sql_d(
	rdbi_context_def *context,
    int     sqlid,
    const char *sql )
{
    rdbi_string_def str;
    str.ccString = sql;
    return local_rdbi_sql(context, sqlid, &str, TRUE);
}

int rdbi_sql_dW(
	rdbi_context_def *context,
    int     sqlid,
    const wchar_t *sql )
{
    rdbi_string_def str;
    str.cwString = sql;
    return local_rdbi_sql(context, sqlid, &str, TRUE);
}

/*
 *  Extract first word (delimited by a space, tab, or newline)
 *  from the SQL statement.
 */
static void local_parse (const char *sql, char *verb)
{
#define WORD_LEN    31
    const char *p = sql;
    int i;

	while (*p == '\t' || *p == ' ')
      p++;
    for (i = 0; *p && i < WORD_LEN && !ut_isspace (*p); p++, i++)
    {
        if (ut_isupper (*p))
            verb[i] = ut_tolower (*p);
        else
            verb[i] = *p;
    }
    verb[i] = '\0';
}

static void local_parseW (const wchar_t *sql, char *verb)
{
#define WORD_LEN    31
    const wchar_t *p = sql;
    int i;

    // We are searching for English keywords; we can cast a whar_t to char.
	while (*p == '\t' || *p == ' ')
      p++;
    for (i = 0; *p && i < WORD_LEN && !isspace (*p); p++, i++)
    {
        if (isupper (*p))
            verb[i] = ut_tolower (*p);
        else
            verb[i] = (char)*p;  
    }
    verb[i] = '\0';
}

int local_rdbi_sql(
	rdbi_context_def *context,
    int     sqlid,
    rdbi_string_def* sql,
    int     defer )
{


    rdbi_cursor_def *cursor     = (rdbi_cursor_def *) NULL; /* for ease of reference            */
    rdbi_cursor_def *cursor_coc = (rdbi_cursor_def *) NULL; /* for ease of reference            */
    int             trace_line_num = 0;
    int             sqlid_coc   = -1;

	char			stats[128];

#ifdef _DEBUG
    if (context->dispatch.capabilities.supports_unicode == 1){
        debug_on4("rdbi_sql2", "\tContext: %d Db: %s#%d\n\tSQL: %.120ls",
            sqlid, context->rdbi_cnct->db_name, context->rdbi_cnct->connect_id, sql->cwString);
    }else{
        debug_on4("rdbi_sql2", "\tContext: %d Db: %s#%d\n\tSQL: %.120s",
            sqlid, context->rdbi_cnct->db_name, context->rdbi_cnct->connect_id, sql->ccString);
    }
#endif

    cursor = context->rdbi_cursor_ptrs[sqlid];

    if (context->rdbi_cnct->autocommit_on && cursor->tran_begun) {
        char    tran_id[50];
        sprintf(tran_id, "auto-exec-%s %d", cursor->verb, cursor->trace_line);
        rdbi_tran_end(context, tran_id);
        cursor->tran_begun = FALSE;
    }

	sprintf(stats, "Open Cursor: %d", sqlid);
	debug_trace(stats, (wchar_t *)NULL, NULL);
    if (context->dispatch.capabilities.supports_unicode == 1)
	    debug_trace(NULL, sql->wString, &trace_line_num);
    else
        debug_trace(sql->cString, NULL, &trace_line_num);

    cursor->sql_parsed      = FALSE;    /* set default cursor conditions */
    cursor->bound_vars      = FALSE;
    cursor->defined_vars    = FALSE;
    cursor->sel_for_update  = FALSE;
    cursor->n_executions    = 0;
    cursor->tran_begun      = FALSE;
    cursor->trace_line      = trace_line_num;

#ifdef _DEBUG
	/* free any previously-stored SQL statement             */
	ut_vm_free("rdbi_sql", cursor->sql);
    cursor->sql = (char *)NULL;
 
  	if( sql->cwString )
  	{
        if (context->dispatch.capabilities.supports_unicode == 1)
        {
            cursor->sqlW = (wchar_t*)ut_vm_malloc("rdbi_sql", (wcslen(sql->cwString) + 1)*sizeof(wchar_t));
  		    if (cursor->sqlW == (wchar_t *)NULL) {
  			    cursor->status = RDBI_MALLOC_FAILED;
  			    goto the_exit;
  		    }
  		    wcscpy(cursor->sqlW, sql->cwString);
        }
        else
        {
  		    cursor->sql = (char*)ut_vm_malloc("rdbi_sql", strlen(sql->ccString) + 1);
  		    if (cursor->sql == (char *)NULL) {
  			    cursor->status = RDBI_MALLOC_FAILED;
  			    goto the_exit;
  		    }
  		    strcpy(cursor->sql, sql->ccString);
        }
	}
#endif

    /* instead of parsing the statement for 'current of cursor' we just do this: */
    cursor->status = RDBI_SUCCESS;
    cursor->statement_type = 0;
    cursor->sel_for_update = 0;

    /* get the verb */
	if (context->dispatch.capabilities.supports_unicode == 1)
    {
        local_parseW (sql->cwString, cursor->verb);
		cursor->status = (*(context->dispatch.sqlW))(context->drvr, cursor->vendor_data, sql->cwString, defer,
                                cursor->verb, NULL, cursor_coc == (rdbi_cursor_def *) NULL ?
                                (char *) NULL : cursor_coc->vendor_data);
    }
    else
    {
		local_parse (sql->ccString, cursor->verb);
		cursor->status = (*(context->dispatch.sql))(context->drvr, cursor->vendor_data, sql->ccString, defer,
                                cursor->verb, NULL, cursor_coc == (rdbi_cursor_def *) NULL ?
                                (char *) NULL : cursor_coc->vendor_data);
    }

    if (cursor->status == RDBI_SUCCESS)
        cursor->sql_parsed =    TRUE;

#ifdef _DEBUG
the_exit:
#endif
    context->rdbi_last_status = cursor->status;

    debug_return(NULL, cursor->status);
}


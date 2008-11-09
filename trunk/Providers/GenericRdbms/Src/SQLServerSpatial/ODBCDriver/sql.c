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
 */

/************************************************************************
* Name																	*
*	odbcdr_sql - Associate an SQL statement with a cursor				*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	#include <Inc/dbi/prse.h>											*
*	int odbcdr_sql(cursor, sql, defer, verb, ptree, cursor_coc)			*
*	char				*cursor;										*
*	const char				*sql;									    *
*	int					defer;											*
*	char				*verb;											*
*	void	*ptree; 										            *
*	char				*cursor_coc;									*
*																		*
* Description															*
*		Associate  the	supplied SQL statement with the specified		*
*		cursor area.													*
*																		*
* Parameters															*
*	cursor: 	update													*
*		Pointer to the	relevant  pre-established  cursor context		*
*		area.  It will have various flags  updated	and will have		*
*		descriptor area(s) allocated and assigned.						*
*																		*
*	sql:		input													*
*		Pointer to a null terminated ascii SQL statement.				*
*																		*
*	defer:		input													*
*		If TRUE then defer parse to first exec/describe.				*
*																		*
*	verb:		input													*
*		Pointer to a null terminated ascii string identifying the		*
*		primary  verb  of  the	SQL  statement.   Eg.	"select",		*
*		"create", ...  This is currently unused.						*
*																		*
*	ptree:		input													*
*		A  pointer	to	the SQL statement's parse tree.   Unused.		*
*																		*
*	cursor_coc: 	input												*
*		Pointer to the	relevant  pre-established  cursor				*
*		referenced by a current of cursor clause in the sql.			*
*																		*
* Function Value														*
*		An RDBI status integer.   Good	is	RDBI_SUCCESS  (ie 0).		*
*		See inc/rdbi.h													*
*																		*
************************************************************************/

#include <Inc/ut.h>
#include <Inc/rdbi.h>					/* rdbi status values		*/
#include	<Inc/debugext.h>
#include "proto_p.h"
#ifdef _WIN32
#include "odbcss.h"
#else
#include <sqlucode.h>
#include <wchar.h>
#endif

char	*odbcdr_bindable_verbs[] = { "select", "insert", "update", "delete", NULL };
#define ROLLBACK_STRING  "rollback"
#define ROLLBACK_STRINGW L"rollback"

int local_odbcdr_sql(
    odbcdr_context_def  *context,
	char				*cursor,
	rdbi_string_def     *sql,
	int					defer,
	char				*verb,
	void				*ptree,
	char				*cursor_coc
	)
{
	odbcdr_cursor_def	*c;
	odbcdr_connData_def *connData;
	int 				rdbi_status = RDBI_GENERIC_ERROR;
	int					num_bind_vars = 0; 
	int					i = 0;
	SQLRETURN			rc;

#ifdef _DEBUG
    if (context->odbcdr_UseUnicode)
    {
	    debug_on5("odbcdr_sql", "c:%#x %sdeferred verb '%s' %.550ls %.550ls", cursor,
		    defer? "":"non-",
            ISNULL(verb), ISNULL(sql->cwString),
            sql->wString ? L"" : ISNULL(((odbcdr_cursor_def*)cursor)->sqlstringW));
    }
    else
    {
	    debug_on5("odbcdr_sql", "c:%#x %sdeferred verb '%s' %.550s %.550s", cursor,
		    defer?"":"non-",
            ISNULL(verb), ISNULL(sql->ccString),
            sql->ccString ? "" : ISNULL(((odbcdr_cursor_def*)cursor)->sqlstring));
    }
#endif

	ODBCDR_RDBI_ERR( odbcdr_get_cursor( context, cursor, &c ) );

	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn(context, &connData) );
	
	c->cumul_rows_fetched = 0;
	c->is_rollback = FALSE;

#ifdef _DEBUG
    if( sql->ccString ) {
		if( c->sqlstring != (char *)NULL ) {
			ut_vm_free( _db_function, c->sqlstring );
			c->sqlstring = NULL;
		}
        if (context->odbcdr_UseUnicode)
        {
            c->sqlstringW = (wchar_t*)ut_vm_malloc( "odbcdr_sql: sql string", (wcslen( sql->cwString ) + 1)*sizeof(wchar_t) ); 
            if( c->sqlstringW == (wchar_t *)NULL ) {
			    rdbi_status = RDBI_MALLOC_FAILED;
			    goto the_exit;
		    }
        }
        else
        {
            c->sqlstring = (char*)ut_vm_malloc( "odbcdr_sql: sql string", strlen( sql->ccString ) + 1 ); 
		    if( c->sqlstring == (char *)NULL ) {
			    rdbi_status = RDBI_MALLOC_FAILED;
			    goto the_exit;
		    }
        }
        ODBCDRV_STRING_COPY_RST(c->sqlstring, sql)
	} 
#endif
	/*
	** Intercept any attempt to execute a ROLLBACK statement.
	** In ODBC, transactions must be managed through the
	** transaction API calls.
	*/
	if ( sql != NULL && (ODBCDRV_STRING_COMPARE_LST(sql, ROLLBACK_STRING) == 0) ) {
		// We do not prepare and execute "rollback" statements.
		// in ODBC. Instead, an SQLEndTran() function call will
		// be run in odbcdr_execute()
		c->is_rollback = TRUE;
		rc = SQL_SUCCESS;
	} else {

		/*
		** clean up the statement before parsing another SQL statement
		*/
		rc = SQLFreeStmt(c->hStmt, SQL_RESET_PARAMS);
		rc = SQLFreeStmt(c->hStmt, SQL_UNBIND);
		rc = SQLFreeStmt(c->hStmt, SQL_CLOSE);

		/*
		** Use fast forward server side cursors
		*/
        // DS: Getting Invalid option error
		//ODBCDR_ODBC_ERR( SQLSetStmtAttr(c->hStmt, 
  //                          SQL_SOPT_SS_CURSOR_OPTIONS, 
  //                          (SQLPOINTER)SQL_CO_FFO, 
  //                          SQL_IS_INTEGER),
		//			    SQL_HANDLE_STMT, c->hStmt,
		//			    "SQLSetStmtAttr", "SQL statement" );

		/*
		** Now we can parse the SQL
		*/
        if (context->odbcdr_UseUnicode){
            ODBCDR_ODBC_ERR( SQLPrepareW( c->hStmt, (SQLWCHAR*)sql->cwString, 
                        SQL_NTS), SQL_HANDLE_STMT, c->hStmt,
				    "SQLPrepare", "SQL statement" );
        }else{
            ODBCDR_ODBC_ERR( SQLPrepare( c->hStmt, (SQLCHAR*)sql->ccString, 
                        SQL_NTS),SQL_HANDLE_STMT, c->hStmt,
				    "SQLPrepare", "SQL statement" );
        }
	}

	ODBCDR_ODBC_ERR( rc,
					SQL_HANDLE_STMT, c->hStmt,
					"SQLPrepare", "SQL statement" );

	rdbi_status = RDBI_SUCCESS;

the_exit:

	debug_area() odbcdr_show_context( context, c );
	debug1("Returning cursor c:%#x", cursor);
	debug_return(NULL, rdbi_status );

}

int odbcdr_sql(
    odbcdr_context_def  *context,
	char				*cursor,
	const char			*sql,
	int					defer,
	char				*verb,
	void				*ptree,
	char				*cursor_coc
	)
{
    rdbi_string_def str;
    str.ccString = sql;
    return local_odbcdr_sql(context, cursor, &str, defer, verb, ptree, cursor_coc);
}

int odbcdr_sqlW(
    odbcdr_context_def  *context,
	char				*cursor,
	const wchar_t	    *sql,
	int					defer,
	char				*verb,
	void                *ptree,
	char				*cursor_coc
	)
{
    rdbi_string_def str;
    str.cwString = sql;
    return local_odbcdr_sql(context, cursor, &str, defer, verb, ptree, cursor_coc);
}

/*
** CMB: I got this search and replace function from
**		http://groups.google.com/groups?hl=en&selm=8rdjvm%54gjk%541%40sunnews.cern.ch
** It assumes the new string will fit into the memory allocated for the original string
**
** It will only replace properly delimited instances of the find string
*/
void replace_substring(char *search_str, char *find_str, char *replace_str)
{
	char *p;
	char * unsearched_str;
	size_t find_len;
	size_t replace_len;
	int  string_found = TRUE;

	find_len = strlen(find_str), 
	replace_len = strlen(replace_str);
	unsearched_str = search_str; // start looking at the beginning of the search string
	
	// keep looking until no instances of the find string are found in the unsearched_str
	while (string_found) {
		p = strstr(unsearched_str, find_str);
		if (p != NULL) {
			string_found = TRUE; // there still could be more instances 
			// only replace properly delimited versions of the find string
			// ie. "a.find_me", "a+find_me" will be replaced, but "a_find_me", and "find_me_too" will not
			if(	(
					(p == search_str) ||	// if find_str occurs at the beginning of search_str 
					(*(p - 1) == ' ') ||	// if the character before the search string is a delimiter	
					(*(p - 1) == '.') ||
					(*(p - 1) == ',') ||
					(*(p - 1) == '+') ||
					(*(p - 1) == '-') ||
					(*(p - 1) == '=') ||
					(*(p - 1) == ')') ||
					(*(p - 1) == '(') ||
					(*(p - 1) == '\n') ||
					(*(p - 1) == '\t') ||
					(*(p) == ' ') ||			// if the first character of the search string is a delimiter		
					(*(p) == '.') ||
					(*(p) == ',') ||
					(*(p) == '+') ||
					(*(p) == '-') ||
					(*(p) == '=') ||
					(*(p) == ')') ||
					(*(p) == '(') ||
					(*(p) == '\n') ||
					(*(p) == '\t')
				) && (
					(*(p + find_len-1) == ' ') ||	// if the last character in the search string is a delimiter
					(*(p + find_len-1) == '.') ||
					(*(p + find_len-1) == ',') ||
					(*(p + find_len-1) == '+') ||
					(*(p + find_len-1) == '-') ||
					(*(p + find_len-1) == '=') ||
					(*(p + find_len-1) == ')') ||
					(*(p + find_len-1) == '(') ||
					(*(p + find_len-1) == '\n') ||
					(*(p + find_len-1) == '\t') ||
					(*(p + find_len) == ' ') ||		// if the character following the search string is a delimiter	
					(*(p + find_len) == '.') ||
					(*(p + find_len) == ',') ||
					(*(p + find_len) == '+') ||
					(*(p + find_len) == '-') ||
					(*(p + find_len) == '=') ||
					(*(p + find_len) == ')') ||
					(*(p + find_len) == '(') ||
					(*(p + find_len) == '\n') ||
					(*(p + find_len) == '\t') ||
					(*(p + find_len) == '\0')		// if find_str occurs at the end of search_str 
				)
			) {
				memmove(p + replace_len, p + find_len, strlen(p + find_len) + 1);
 				memcpy(p, replace_str, replace_len);
			} 
			unsearched_str = (p + replace_len);
		} else {
			string_found = FALSE; 
		} // if p!=null
	} // while string_found

}

/*
** Counts the number of define variables (select columns)
** in a sql statement. It will not work if the select contains 
** a function that calls another function 
** ( ie. select a, fn1(b, fn5(c)) col_alias, d from tab1)
*/
int num_define_vars(const char *sql) {
	int commas_found = 0;	// the number of commas between select..from, excluding those inside sql function calls 
	const char *from_pos;	// the position of the from string in the sql string
	const char *pos;

	// find the position of the "from" substring in the sql string
	from_pos = strstr(sql, "from");
	
	// count commas between "select" and "from"  
	pos = sql; //"select" should be a position 0 of the sql string
	while(pos < from_pos) { //is this right?
		// Do not count commas inside an sql function.
		// We are assuming that there will not be a function inside a function
		if (*pos == '(') {
			while ( *pos != ')') {
				pos++;
			}
		} else if (*pos == ',') {
			commas_found++;
		}
		pos++;
	}

	// the number of define vars is 1 + the number of commas found
	// ex: select a,b, fn1(c,d,e) col_alias, e from table tab1
	// There are 3 commas outside functions between the select and from
	// There are 4 define vars in the statement.
	return commas_found + 1;
}

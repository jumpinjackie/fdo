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

#include <Inc/rdbi.h>					/* rdbi status values		*/
#include <Inc/ut.h>							/* ut_vm_malloc()			*/
#include	<Inc/debugext.h>
#include "proto_p.h"
#include <time.h>
#ifdef _WIN32
#include "odbcss.h"
#endif

#include <assert.h>

/************************************************************************
* Name																	*
*	odbcdr_execute - Execute the SQL statement associated with 			*
*		this cursor														*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	int odbcdr_execute(cursor, count, offset, rows_processed)			*
*	char		*cursor;												*
*	int 		 count; 												*
*	int 		 offset;												*
*	int 		*rows_processed;										*
*																		*
* Description															*
*		Execute  the  SQL  statement  that	has  been  previously		*
*		associated with this cursor via odbcdr_sql().  Execute the		*
*		statement 'count'  times,  beginning  at  the  'offset'th		*
*		element of any bound variables.  Usually count	is	1 and		*
*		offset is 0.													*
*																		*
* Parameters															*
*	cursor: 	input													*
*		Pointer to the relevant cursor context area.  This cursor		*
*		must have an SQL statement associated with it by means of		*
*		a  prior call to odbcdr_sql().  Any bind'ing or define'ing		*
*		should	already   be   complete  prior	to	calling  this		*
*		function.														*
*																		*
*	count:		input													*
*		An integer count of the number of times this statement is		*
*		to be executed.  Oracle array processing is supported.	A		*
*		count > 1 makes  sense	only if there are arrays of bound		*
*		variables to process.  Usually 1.								*
*																		*
*	offset: 	input													*
*		An integer (base 0) offset into  the  array[s]	of	bound		*
*		variables at  which  to start the execute loop operation.		*
*		Usually 0.														*
*																		*
*	rows_processed: output												*
*		A pointer to an  integer  where  we can put the number of		*
*		rows affected by this operation.  In an  array	scenario,		*
*		this count is for only the last execute done.					*
*																		*
* Function Value														*
*		An	RDBI  status  integer.	 Good is RDBI_SUCCESS (ie 0).		*
*		See inc/rdbi.h													*
*																		*
************************************************************************/

int odbcdr_execute(						/* execute an SQL statement		  */
    odbcdr_context_def  *context,
	char   *cursor, 					/* cursor associated with SQL stmnt */
	int 	count,						/* # rows to process				*/
	int 	offset, 					/* offset in row buffer to start	*/
	int    *rows_processed	 			/* # rows processed by this exec	*/
	)
{
	odbcdr_cursor_def		*c;
	odbcdr_connData_def		*connData;
	SQLRETURN				rc;
	int 					rdbi_status = RDBI_GENERIC_ERROR;

//	SQLUSMALLINT			*status_array;
	SQLUSMALLINT			params_processed = 1234;
	SQLUSMALLINT			*operation_array; // used to emulate OCI's execute offset
	int						i;  
	short					num_cols; //used to determine if the statement was a SELECT

	debug_on3("odbcdr_execute", "c:%#p count %d from offset %d",
												cursor, count, offset);

	ODBCDR_RDBI_ERR( odbcdr_get_cursor( context, cursor, &c ) );
	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );
	assert(count <= ODBCDR_MAX_ARRAY_SIZE);

	/*
	** If rdbi is trying to execute a "rollback" statement,
	** issue an ODBC SQLEndTran() instead. This condition
	** was detected in odbcdr_sql()
	*/
	if (c->is_rollback) {
		rc = SQLEndTran(SQL_HANDLE_DBC,
					connData->hDbc,
					SQL_ROLLBACK);
		context->odbcdr_last_rc = rc;
		rdbi_status = odbcdr_xlt_status( context, rc, SQL_HANDLE_DBC, connData->hDbc);
		// That's all we have to do to execute a rollback.
		goto the_exit;
	}

	c->cumul_rows_fetched = 0;

	/*
	** Set the statement attributes
	*/
    if (context->odbcdr_UseUnicode)
    {
	     // set the size of the bind parameter arrays
        if (count > 0)
	        SQLSetStmtAttrW(c->hStmt, SQL_ATTR_PARAMSET_SIZE, (PTR)count, SQL_IS_INTEGER);
	    // retreive the number of params processed
	    // SQLSetStmtAttrW(c->hStmt, SQL_ATTR_PARAMS_PROCESSED_PTR, &params_processed, SQL_IS_POINTER);
    }
    else
    {
	     // set the size of the bind parameter arrays
        if (count > 0)
	        SQLSetStmtAttr(c->hStmt, SQL_ATTR_PARAMSET_SIZE, (PTR)count, SQL_IS_INTEGER);

	    // retreive the number of params processed
	    // SQLSetStmtAttr(c->hStmt, SQL_ATTR_PARAMS_PROCESSED_PTR, &params_processed, SQL_IS_POINTER);
    }
	//array insert/update/delete is not implemented yet
	//array to retreive the result codes
	//status_array = ut_vm_malloc("odbcdr_execute: result codes", (sizeof(SQLUSMALLINT) * count));
	//SQLSetStmtAttr(c->hStmt,
	//				SQL_ATTR_PARAM_STATUS_PTR,
	//				status_array,
	//				SQL_IS_POINTER);

	 if (offset > 0) {
		// emulate the setting of the offset into the array
		// by using the SQL_ATTR_PARAM_OPERATION_PTR array
		operation_array = (SQLUSMALLINT*)ut_vm_malloc("odbcdr_execute: offset array", sizeof(SQLUSMALLINT) * count);

		for (i=0; i<count; i++) {
			if (i<offset)
				operation_array[i] = SQL_PARAM_IGNORE;
			else
				operation_array[i] = SQL_PARAM_PROCEED;
		} //for

		if (context->odbcdr_UseUnicode)
            SQLSetStmtAttrW(c->hStmt, SQL_ATTR_PARAM_OPERATION_PTR, operation_array, SQL_IS_POINTER);
        else
        {
            SQLSetStmtAttr(c->hStmt, SQL_ATTR_PARAM_OPERATION_PTR, operation_array, SQL_IS_POINTER);
        }
	} // if offset


	// Emulate Oracle's NOWAIT hint on select for update statements
/* We are now using the undocumented NOWAIT hint now to cut down
   on network traffic
	if (c->is_nowait) {
		SQLAllocHandle(SQL_HANDLE_STMT,	connData->hDbc, &hStmt5);
		rc5 = SQLExecDirect(hStmt5, "SET LOCK_TIMEOUT 0", SQL_NTS);
	}
*/
	/*
	** now execute the SQL
	*/
	rc = SQLExecute(c->hStmt);


   ///* For data-at-execution parameters, call SQLParamData to */
   ///* get the parameter number set by SQLBindParameter. */
   ///* Call InitUserData. SQLPutData                 */
   ///* repeatedly to get and put all data for the parameter. */
   ///* Call SQLParamData to finish processing this parameter */

    if ( rc == SQL_NEED_DATA && c->bound_geometries != NULL )
    {
        assert(count <= ODBCDR_MAX_ARRAY_SIZE);

        /* Free any Oracle objects from previous executions/fetches. */
        ODBCDR_RDBI_ERR( odbcdr_geom_freeSqlServerGeometries( context, c ) );

        /* Reduce array size to exactly that which is populated. */
        ODBCDR_RDBI_ERR( odbcdr_geom_setNumRows( context, c, count ) );

        ODBCDR_RDBI_ERR( odbcdr_geom_convertBoundToSqlServer( context, c, count ) );

        rc = SQL_SUCCESS;

    }   /* end if geometries */

	// Reset the lock timeout so that we now waits on locks
/* We are now using the undocumented NOWAIT hint now to cut down
   on network traffic
	if (c->is_nowait) {
		rc5 = SQLFreeStmt(hStmt5, SQL_CLOSE);
		rc5 = SQLExecDirect(hStmt5, "SET LOCK_TIMEOUT -1", SQL_NTS);
		//Destroy the statement handles
		rc5 = SQLFreeHandle(SQL_HANDLE_STMT, hStmt5);
	}
*/
	/*
	** Clear the parameter state. This allows us to
	** use new parameters on a SQL statement that
	** we are re-executing.
	*/
	//	SQLFreeStmt(c->hStmt, SQL_RESET_PARAMS);

	/*
	** Get the number of rows processed
	*/
	*rows_processed = 0;
	if ((rc == SQL_SUCCESS) || (rc == SQL_SUCCESS_WITH_INFO) )
	{
		SQLNumResultCols(c->hStmt, (SQLSMALLINT*) &num_cols);
		SQLRowCount(c->hStmt, (SQLLEN*)rows_processed);
		if (num_cols > 0) { // if the statement was a select
			*rows_processed	= 0;
		}
        else {
            if ( c->is_sqlserver_insert ) 
            {
                // for inserts against SQL Server databases, 
                // odbcdr_sql tacks on a statement to select the
                // autoincremented value, which is generated if any column
                // is autoincremented. For SQL Server there is a maximum of 1
                // autoincremented column per table. 
                for ( int i = 0; i < 2; i++ ) 
                {
                    // Close the insert statement's result set and move on to
                    // the select's result set. TODO: find how why this sometimes
                    // has to be done twice. 
                    rc = SQLMoreResults( c->hStmt );

                    // SQL_NO_DATA means there are no more results. This happens
                    // when the insertion table has no autoincrement column.
                    if ( (rc == SQL_NO_DATA) ) 
                        break;

                    if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) ) {
		                rdbi_status = rc;
		                goto the_exit;
	                }

                    // Check if the current results set has the column holding
                    // the autoincremented value. If it does then move on to 
                    // fetching it.
                    SQLSMALLINT         cbColumnName = 0;

            		SQLNumResultCols(c->hStmt, (SQLSMALLINT*) &num_cols);
                    if ( num_cols == 1 ) 
                    {
                        if (context->odbcdr_UseUnicode)
                        {
                            wchar_t ident_col_name[ODBCDR_MAX_BUFF_SIZE + 1];
                            ident_col_name[0] = '\0';
                            ODBCDR_ODBC_ERR( SQLColAttributeW(
                                c->hStmt,
                                (SQLUSMALLINT) 1,
                                (SQLUSMALLINT) SQL_DESC_NAME, 
                                ident_col_name,
                                ODBCDR_MAX_BUFF_SIZE,
                                &cbColumnName, 
                                NULL  ),
                                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting column name" );
                       
                            if ( wcscmp(ident_col_name,L"fdo_ident24356") == 0 )
                                break;
                        }
                        else
                        {
                            char ident_col_name[ODBCDR_MAX_BUFF_SIZE + 1];
                            ident_col_name[0] = '\0';
                            ODBCDR_ODBC_ERR( SQLColAttribute(
                                c->hStmt,
                                (SQLUSMALLINT) 1,
                                (SQLUSMALLINT) SQL_DESC_NAME, 
                                ident_col_name,
                                ODBCDR_MAX_BUFF_SIZE,
                                &cbColumnName, 
                                NULL  ),
                                SQL_HANDLE_STMT, c->hStmt, "SQLColAttribute", "Getting column name" );

                            if ( strcmp(ident_col_name,"fdo_ident24356") == 0 )
                                break;
                        }
                    }
                }

                if ( rc == SQL_NO_DATA ) 
                {
                    // no autoincremented value
                    context->odbcdr_last_autoincrement = 0;
                }
                else
                {
                    // Fetch the autoincremented value.
                    ODBCDR_ODBC_ERR( SQLFetch(c->hStmt),
                               SQL_HANDLE_STMT, c->hStmt,
                                "SQLFetch", "fetch");

                    SQLLEN null_ind;
                    ODBCDR_ODBC_ERR( SQLGetData( c->hStmt, 1, SQL_C_LONG, (SQLPOINTER) &(context->odbcdr_last_autoincrement), 0, &null_ind),
                               SQL_HANDLE_STMT, c->hStmt,
                                "SQLGetData", "getData");

                    if ( null_ind == SQL_NULL_DATA ) 
                        // convert null value to 0.
                        context->odbcdr_last_autoincrement = 0;

                    // Finish this results set. Works around a problem where
                    // an infinite loop occurs when this statement is next executed.
                    rc = SQLMoreResults( c->hStmt );
                    if ( (rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO) && (rc != SQL_NO_DATA) ) {
	                    rdbi_status = rc;
	                    goto the_exit;
                    }
                }
            }
        }
	}

	if (offset > 0) {
		// free the memory for the offset emulation array
		ut_vm_free("odbcdr_execute: offset emulation", (char *)operation_array);
	}

	if ( rc != ODBCDR_SUCCESS &&
		rc != SQL_SUCCESS_WITH_INFO &&
		rc != SQL_NO_DATA )
	{
		debug0( "SQLExecute FAILED!" );
		rdbi_status = odbcdr_xlt_status( context, rc, SQL_HANDLE_STMT, c->hStmt);
		context->odbcdr_last_rc = rc;
		goto the_exit;
	}
	else
	{
        rdbi_status = RDBI_SUCCESS;
		debug0( "SQLExecute." );
	}

	c->cumul_rows_fetched = 0; // Rows fetched is calculated in odbcdr_fetch()

the_exit:
	debug_area() odbcdr_show_context( context, c );
	debug1("Returning cursor c:%#p", cursor);
	debug_return(NULL, rdbi_status );
}

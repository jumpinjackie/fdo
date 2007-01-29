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
* Name                                                                  *
*   odbcdr_bind - Bind a :n variable to a given address and data type   *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   int odbcdr_bind(cursor, name, datatype, size, address, null_ind)    *
*   char        *cursor;                                                *
*   char        *name;                                                  *
*   int          datatype;                                              *
*   int          size;                                                  *
*   char        *address;                                               *
*   short       *null_ind;                                              *
*                                                                       *
* Description                                                           *
*       Bind  a  ":n"  variable to a given address, data type and       *
*       binary size.                                                    *
*                                                                       *
* Parameters                                                            *
*   cursor:     input                                                   *
*       Pointer to the relevant cursor context area.  This cursor       *
*       must have had an  SQL  statement  associated with it by a       *
*       prior call to odbcdr_sql().  Also, that SQL statement must      *
*       have  some  references  to  bind  variables of  the  form       *
*       ':<n>'.                                                         *
*                                                                       *
*   name:       input                                                   *
*       Pointer  to  a null terminated ascii  string  identifying       *
*       which bound variable to bind.  Typcially strings like "1"       *
*       or  "2".   Eventually   names  like  "feat_num"  will  be       *
*       supported.                                                      *
*                                                                       *
*   datatype:   input                                                   *
*       The  variable's  data  type.   eg.   RDBI_STRING  --  see       *
*       inc/rdbi.h                                                      *
*                                                                       *
*   size:       input                                                   *
*       The actual binary size  of the variable's data area.  Eg.       *
*       4 for long integers.  In  the  case of character strings,       *
*       this length should be the maximum string length including       *
*       trailing null (or use the sizeof() construct).                  *
*                                                                       *
*       If  the  binary  size  of a variable  can  be  determined       *
*       directly  from  the data type,  this  parameter  will  be       *
*       ignored.                                                        *
*                                                                       *
*   address:    input                                                   *
*       The address of  the  variable's  data  area  -- cast as a       *
*       pointer  to  char,  regardless  of the actual data  type.       *
*                                                                       *
*   null_ind:   input                                                   *
*       The address of a short  integer  which  indicates whether       *
*       the variable is NULL or not.  (0 ==>  not  NULL;  -1  ==>       *
*       NULL).  If this pointer is itself NULL, the variable will       *
*       be presumed to be always not NULL.                              *
*                                                                       *
* Function Value                                                        *
*       An RDBI status integer.   Good  is  RDBI_SUCCESS  (ie 0).       *
*       See inc/rdbi.h.  If the bound variable cannot be found in       *
*       the  associated  SQL statement, the return value will  be       *
*       RDBI_NOT_IN_DESC_LIST.                                          *
*                                                                       *
* Remarks                                                               *
*       The same bound  variable may be referenced more than once       *
*       in a single SQL  statement.   This routine will take care       *
*       of binding all occurences of that  bind  variable  to the       *
*       supplied address.                                               *
*                                                                       *
************************************************************************/

#include <Inc/debugext.h>
#include <Inc/rdbi.h>			/* rdbi status values		*/
#include <Inc/ut.h> 			/* ut_vm_malloc()			*/
#include "proto_p.h"
#include "constants.h"
#ifndef _WIN32
#include <sqlucode.h>
#endif

#include <limits.h>

int odbcdr_bind(
    odbcdr_context_def *context,
	char	*cursor,
	char	*name,
	int 	 datatype,
	int 	 size,
	char	*address,
	SQLLEN	*null_ind
	)
{
	odbcdr_cursor_def	*c;
	int 				odbcdr_datatype;
	int 				odbcdr_size;
	int 				rdbi_status = RDBI_GENERIC_ERROR;
	int					bindnum;

	SQLSMALLINT			sql_type;
	SQLUINTEGER			col_size;
	SQLSMALLINT			decimal_digits;
	SQLSMALLINT			nullable; 

	SQLRETURN			rc;

	debug_on5("odbcdr_bind", "c:%#x name: %s type: %d address: 0x%lx size: %d",
							cursor, name, datatype, address, size);

	ODBCDR_RDBI_ERR( odbcdr_get_cursor( context, cursor, &c ) );

	bindnum = atoi(name);

	/*
	** Get the C and SQL data type information
	*/
	odbcdr_get_o_t_s( context, datatype, size, &odbcdr_datatype, &odbcdr_size );

	rc = SQLDescribeParam(c->hStmt,
							(SQLUSMALLINT)bindnum,
							&sql_type,
							&col_size,
							&decimal_digits,
							&nullable);
	/* 
	** SQLDescribeParam() will not work if there are bind variables in sub-queries,
	** UNIONS or select lists (ie. select feat_num, ? from g_master where...).
	** If the function did not work, we assume that the parameter is of type
	** SQL_CHAR, with a length of 100. The database should be able to implicitly
	** convert the parameter to the correct type.
	*/  
	if (
		(rc == SQL_ERROR) ||
		   ((sql_type != SQL_CHAR) &&	
            (sql_type != SQL_WVARCHAR ) &&
			(sql_type != SQL_WCHAR ) &&
			(sql_type != SQL_NUMERIC) &&
			(sql_type != SQL_DECIMAL) &&
			(sql_type != SQL_INTEGER) &&
			(sql_type != SQL_SMALLINT) &&
            (sql_type != SQL_BIGINT) &&
            (sql_type != SQL_TINYINT) &&
			(sql_type != SQL_FLOAT) &&
			(sql_type != SQL_REAL) &&
			(sql_type != SQL_DOUBLE) &&
			(sql_type != SQL_DATETIME) &&
			(sql_type != SQL_VARCHAR) &&
			(sql_type != SQL_DATE) &&		
			(sql_type != SQL_TIMESTAMP) &&
            (sql_type != SQL_LONGVARCHAR) &&
            (sql_type != SQL_WLONGVARCHAR) &&
            (sql_type != SQL_BINARY) &&
            (sql_type != SQL_VARBINARY) &&
            (sql_type != SQL_LONGVARBINARY))
		)
	{
		debug3 ("\nError=%d in SQLDescribeParam() for '%s', type=%d. Assuming type = SQL_CHAR, length = 100\n", rc, name, sql_type );
		// most SQL types can be converted to SQL_CHAR
		sql_type = SQL_CHAR;
		col_size = 100;
		decimal_digits = 0;
	}

	/*
	** Bind the variable
	*/
    if ( datatype != RDBI_GEOMETRY )
    {
		ODBCDR_ODBC_ERR( SQLBindParameter(
						c->hStmt,
						(SQLUSMALLINT) bindnum,
						SQL_PARAM_INPUT,
						(SQLSMALLINT) odbcdr_datatype,
						(SQLSMALLINT) sql_type,
						(SQLUINTEGER) col_size,
						(SQLSMALLINT) decimal_digits,
						(SQLPOINTER) address,
						(SQLINTEGER) size,      // buffer size
						(SQLLEN *) null_ind),   // length indicator 
					SQL_HANDLE_STMT,c->hStmt,
					"SQLBindParameter",	"bind" );
	
    } else {

        ODBCDR_RDBI_ERR( odbcdr_geom_bindColumn( context, c, bindnum, address ) );

        ODBCDR_RDBI_ERR( odbcdr_geom_setNumRows( context, c, ODBCDR_MAX_ARRAY_SIZE) );

        /* 
        ** For geometries, pass the parameter number in ParameterValuePtr 
        ** instead of a buffer address. 
        ** Pass the maximum length for the buffer. It doesn't work to set it
        ** to 0, in spite of what samples say.
        */
        // "For SQL_LONGVARBINARY type, ColumnSize must be set to the total length of the data to be sent, not the precision as defined in this table.

        c->lenDataParam = SQL_LEN_DATA_AT_EXEC(0);

        rc = SQLBindParameter(
						c->hStmt,
						(SQLUSMALLINT)bindnum,
						SQL_PARAM_INPUT,
						(SQLSMALLINT) SQL_C_BINARY, 
						SQL_LONGVARBINARY,
						(SQLUINTEGER) address,  
						(SQLSMALLINT) 0,
						(SQLPOINTER) bindnum,
						(SQLINTEGER) 0, 
						&c->lenDataParam);

        if ( rc != SQL_SUCCESS_WITH_INFO ) {
            ODBCDR_ODBC_ERR( rc,
                    			SQL_HANDLE_STMT,c->hStmt,
						    "SQLBindParameter",	"bind" );
        }
    }

	rdbi_status = RDBI_SUCCESS;

the_exit:

	debug_area() odbcdr_show_context( context, c );
	debug1("Returning cursor c:%#x", cursor);
	debug_return(NULL, rdbi_status );
}

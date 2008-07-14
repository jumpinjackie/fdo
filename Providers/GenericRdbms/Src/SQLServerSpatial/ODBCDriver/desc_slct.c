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

#include <Inc/ut.h> 					/* ut_vm_malloc()			*/
#include <Inc/rdbi.h>					/* rdbi status values		*/
#include	<Inc/debugext.h>
#include "proto_p.h"
#ifndef _WIN32
#include <sqlucode.h>
#endif

/************************************************************************
* Name																	*
*	odbcdr_desc_slct - Describe the n'th select'ed column				*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	int odbcdr_desc_slct(cursor, position, name_size, name, rdbi_type,	*
*												binary_size, null_ok)	*
*	char		*cursor;												*
*	int 		 position;												*
*	int 		 name_size;												*
*	char		*name;													*
*	int 		*rdbi_type; 											*
*	int 		*binary_size;											*
*	int 		*null_ok;												*
*																		*
* Description															*
*		Describe  the  n'th  select'ed	column.  Return its name,		*
*		type, size (binary) and whether it can be NULL. 				*
*																		*
* Parameters															*
*	cursor: 	input													*
*		Pointer to the relevant cursor context area.  This cursor		*
*		must have had an SQL  statement  assodbcated  with it by a		*
*		prior call to odbcdr_sql().										*
*																		*
*	position:	input													*
*		A 1-based specification of which select column's name  to		*
*		return.  Note that this is integer, not ascii as it is in		*
*		odbcdr_bind() and odbcdr_define().								*
*																		*
*	name_size:	input													*
*		The maximum length of the name to be returned plus NUL			*
*		terminator.	 The size of the  available area pointed to by		*
*		the next parameter.												*
*																		*
*	name:		output													*
*		A pointer to  a  place	to	put  the name of the selected		*
*		column	name  or  expression.	This  will	not  be  null		*
*		terminated if the name is too long to  fit.  Any trailing		*
*		blanks will be stripped.										*
*																		*
*	rdbi_type:	output													*
*		The  variable's  data  type.   eg.	 RDBI_STRING  --  see		*
*		inc/rdbi.h														*
*																		*
*	binary_size:	output												*
*		The actual	binary size of the variable's data area.  Eg.		*
*		4 for long	integers.	In	the  case  of multi-character		*
*		strings,  this	length will be the maximum string  length		*
*		including trailing null.										*
*																		*
*	null_ok:		output												*
*		TRUE or FALSE  indication  as  to whether this column, as		*
*		selected, ie.  as defined, can ever  be  NULL.	 (Was  it		*
*		odbcdr_define()'ed with a null_indicator or not?)				*
*																		*
* Function Value														*
*		An RDBI  status  integer.	Good  is RDBI_SUCCESS (ie 0).		*
*		See inc/rdbi.h.  If the supplied position is greater than		*
*		the  number  of  select'ed columns, RDBI_NOT_IN_DESC_LIST		*
*		will be returned.												*
*																		*
* Remarks																*
*		In order to return	all  the  information requested it is		*
*		necessary  to  do  an  ORACLE  'describe'  of	the   SQL		*
*		statement.	 This  in  turn has a pre-requisite to do  an		*
*		ORACLE 'prepare' of the  statement.   We  don't incur the		*
*		overhead   of	prepare  or  describe  unless  absolutely		*
*		necessary.	Hence this	request  will  force  us  to take		*
*		those steps if the cursor context area tells us that they		*
*		haven't been done yet.	See also odbcdr_desc_sql().				*
*																		*
************************************************************************/

int local_odbcdr_desc_slct(
    odbcdr_context_def *context,
	char *cursor,		/* RDBI work area 				*/
	int   position, 	/* position within select clause	*/
	int   name_size, 	/* The maximum size of name 		*/
	rdbi_string_def *name, 		/* Name of this field				*/
	int  *rdbi_type,	/* rdbi constant					*/
	int  *binary_size,	/* bytes of memory to hold it		*/
	int  *null_ok		/* TRUE iff NULLs ok in this column */
	)
{
	odbcdr_cursor_def	*c;
	SQLUINTEGER 		odbc_precision;
	SQLSMALLINT			odbc_scale, odbc_type, odbc_nullable;

	int 				rdbi_status = RDBI_GENERIC_ERROR;
	SQLSMALLINT			column_name_length;
	SQLSMALLINT			num_cols;

	debug_on3("odbcdr_desc_slct", "c:%#x Position: %d, max name length %d",
							cursor, position, name_size);

	ODBCDR_RDBI_ERR( odbcdr_get_cursor( context, cursor, &c ) );

    ODBCDR_ODBC_ERR( SQLNumResultCols(c->hStmt, &num_cols) , 
        SQL_HANDLE_STMT, c->hStmt,
		"SQLDescribeCol", "describe" );
	if (position > num_cols) 
		debug_return(NULL, RDBI_NOT_IN_DESC_LIST);

	/*
	** Get the variable information
	*/
    if (context->odbcdr_UseUnicode){
	    ODBCDR_ODBC_ERR( SQLDescribeColW(c->hStmt,
					        (SQLUSMALLINT)position, 
                            (SQLWCHAR*)name->wString, 
					        (SQLUSMALLINT)name_size,
					        &column_name_length, 
					        &odbc_type,
					        &odbc_precision,
					        &odbc_scale,
					        &odbc_nullable),
                        SQL_HANDLE_STMT, c->hStmt,
					    "SQLDescribeCol", "describe" );
    }else{
	    ODBCDR_ODBC_ERR( SQLDescribeCol(c->hStmt,
					        (SQLUSMALLINT)position, 
                            (SQLCHAR*)name->cString, 
					        (SQLUSMALLINT)name_size,
					        &column_name_length, 
					        &odbc_type,
					        &odbc_precision,
					        &odbc_scale,
					        &odbc_nullable),
                        SQL_HANDLE_STMT, c->hStmt,
					    "SQLDescribeCol", "describe" );
    }
	rdbi_status = RDBI_SUCCESS;
	*null_ok = odbc_nullable; 

	switch(odbc_type) {
		case SQL_CHAR :
            if (odbc_precision == 1 ) {
                *rdbi_type	= RDBI_CHAR;
                *binary_size = odbc_precision + 1;
            } else {
                *rdbi_type	= RDBI_FIXED_CHAR;
                *binary_size = odbc_precision;
            }

            break;
		case SQL_WCHAR :
            *rdbi_type	= RDBI_WSTRING;
            *binary_size = odbc_precision;
            break;
        case SQL_BIT:
            /* Handle much like a CHAR. */
            *rdbi_type = RDBI_BOOLEAN;
            *binary_size = 2;
            break;
		case SQL_WVARCHAR:  // ex: INFORMATION_SCHEAMA.SCHEMATA.SCHEMA_NAME
			*rdbi_type	= RDBI_WSTRING;
			*binary_size = odbc_precision;
			break;
		case SQL_VARCHAR :
			*rdbi_type	= RDBI_STRING;
			*binary_size = odbc_precision;
			break;
        case SQL_LONGVARCHAR:
			*rdbi_type	= RDBI_STRING;
			*binary_size = 16000;  // 'text': Arbitrary for now.
			break;
        case SQL_WLONGVARCHAR:
			*rdbi_type	= RDBI_WSTRING;
			*binary_size = 32000;  // 'text': Arbitrary for now.
			break;
		case SQL_TYPE_TIMESTAMP :
            *rdbi_type	 = RDBI_DATE;
			*binary_size = ODBCDR_DATE_SIZE + 1;
			break;
        case SQL_DOUBLE:
			*rdbi_type = RDBI_DOUBLE;
			*binary_size = sizeof(double);
            break;
		case SQL_FLOAT:
        case SQL_REAL:
            /* 
             * Fixed precision and scale numeric data from -10^38 +1 through 10^38 1. [prec = 24]
             * Double precision is a float [prec = 53]
             * Float is float [prec = 53]
             * Real is a float [prec = 24]
             */
            *rdbi_type =  (odbc_precision >= 53)? RDBI_DOUBLE : RDBI_FLOAT;
            *binary_size = (odbc_precision >= 53)? sizeof(double) : sizeof(float);
            break;
		case SQL_INTEGER:
			*rdbi_type	= RDBI_INT;
			*binary_size = sizeof(long);
            break;
		case SQL_SMALLINT:
        case SQL_TINYINT:              
			*rdbi_type	= RDBI_SHORT;
			*binary_size = sizeof(short);
            break;
		case SQL_NUMERIC:
		case SQL_DECIMAL:
			if ((odbc_scale == 0) && (odbc_precision != 0)) {    /* number(p[,0]) 		   */
				if (odbc_precision <= 4) {
					*rdbi_type	= RDBI_SHORT;
					*binary_size = sizeof(short);
				} else if (odbc_precision > 10) {
					*rdbi_type = RDBI_DOUBLE;
					*binary_size = sizeof(double);
				} else {
					*rdbi_type	= RDBI_LONG;
					*binary_size = sizeof(long);
				}
            } else {
					*rdbi_type = RDBI_DOUBLE;
					*binary_size = sizeof(double);
            }
			break;
        case SQL_BIGINT:
            // When attempting to bind a int64 value using "SQL_BIGINT" as the
            // ODBC type, the function "SQLBindParameter" will issue an error
            // ("Program Type out of range"). To bind values of such a type,
            // it is necessary to use a string.
            // On the other side, the functions "SQLDescribeCol" and
            // "SQLDescribeColW" both return the ODBC data type "SQL_BIGINT"
            // the column is of type "BIGINT". To have a consistent way of
            // treating the int64 cases, the ODBC type "SQL_BIGINT" is mapped
            // to "RDBI_STRING" rather than "RDBI_LONGLONG".
            // The question remains why the binding of an int64 value fails in
            // the first place. One reason could be that the development en-
            // vironment is not a 64bit one. There are also hints on the web
            // that indicate issues for cases like this and use strings as a
            // work-around. If a solution can be identified this code can be
            // modified 
			*rdbi_type	= RDBI_STRING;
			*binary_size = odbc_precision+1;
            break;
        case SQL_LONGVARBINARY:
			*rdbi_type = RDBI_GEOMETRY;
			*binary_size = sizeof(long);
            break;
		default:
            // ODBC doesn't return an error. This is better than a generic error.
            debug1("odbcdr_desc_slct:Unknown ODBC type %d\n", odbc_type);
			rdbi_status = RDBI_INCOMPATIBLE_COLUMN_TYPES;
	}

#ifdef _DEBUG
    if (context->odbcdr_UseUnicode){
        debug3("name '%ls', ora type %d, length %d", name->cwString, odbc_type, odbc_precision);
    }else{
        debug3("name '%s', ora type %d, length %d", name->ccString, odbc_type, odbc_precision);
    }
#endif

the_exit:
	debug_area() odbcdr_show_context( context, c );
	if( rdbi_status == RDBI_SUCCESS ) {
		debug2("Rdbms type %d, binary size %d", *rdbi_type, *binary_size);
    } 
    
	debug_return(NULL, rdbi_status);
}

int odbcdr_desc_slct(
    odbcdr_context_def *context,
	char *cursor,		/* RDBI work area 				*/
	int   position, 	/* position within select clause	*/
	int   name_size, 	/* The maximum size of name 		*/
	char *name, 		/* Name of this field				*/
	int  *rdbi_type,	/* rdbi constant					*/
	int  *binary_size,	/* bytes of memory to hold it		*/
	int  *null_ok		/* TRUE iff NULLs ok in this column */
	)
{
    rdbi_string_def str;
    str.cString = name;
    return local_odbcdr_desc_slct(context, cursor, position, name_size, 
        &str, rdbi_type, binary_size, null_ok);
}

int odbcdr_desc_slctW(
    odbcdr_context_def *context,
	char *cursor,		/* RDBI work area 				*/
	int   position, 	/* position within select clause	*/
	int   name_size, 	/* The maximum size of name 		*/
	wchar_t *name, 		/* Name of this field				*/
	int  *rdbi_type,	/* rdbi constant					*/
	int  *binary_size,	/* bytes of memory to hold it		*/
	int  *null_ok		/* TRUE iff NULLs ok in this column */
	)
{
    rdbi_string_def str;
    str.wString = name;
    return local_odbcdr_desc_slct(context, cursor, position, name_size, 
        &str, rdbi_type, binary_size, null_ok);
}

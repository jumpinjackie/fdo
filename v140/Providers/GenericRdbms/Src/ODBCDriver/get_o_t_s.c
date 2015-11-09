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
*	odbcdr_get_o_t_s - Determine ODBC datatype and datasize			    *
*																		*
*																		*
* Synopsis																*
*	odbcdr_get_o_t_s( datatype, size, odbcdr_datatype, odbcdr_size )	*
*		int   datatype;   int	size;  int	*odbcdr_datatype;  int		*
*		*odbcdr_size;													*
*																		*
* Description															*
*		Determine  the equivalent oracle  datatype	and  datasize		*
*		from the rdbi equivalents.										*
*																		*
* Parameters															*
*	datatype:			input											*
*		The RDBI data type. 											*
*																		*
*	size:				input											*
*		The RDBI data size. 											*
*																		*
*	odbcdr_datatype: 	output											*
*		The ODBC data type.												*
*																		*
*	odbcdr_size: 		output											*
*		The ODBC data size.												*
*																		*
* Function Value														*
*		void															*
*																		*
************************************************************************/

#include <Inc/rdbi.h>					/* rdbi status values		*/
#include	<Inc/debugext.h>
#include "proto_p.h"
#ifndef _WIN32
#include <sqlucode.h>
#endif


void odbcdr_get_o_t_s(
    odbcdr_context_def  *context,
	int datatype,				/* rdbi type	*/
	int size,					/* rdbi size	*/
	int *odbcdr_datatype,		/* odbc type	*/
	int *odbcdr_size			/* odbc size	*/
	)
{
	debug_on("odbcdr_odbcdr_get_o_type_size");

	*odbcdr_size = size; 		/* default to same size */

	switch(datatype) {
		case RDBI_STRING:
        case RDBI_FIXED_CHAR:
			*odbcdr_datatype = SQL_C_CHAR; 
			break;
		case RDBI_WSTRING:
			*odbcdr_datatype = SQL_C_WCHAR; 
			break;
		case RDBI_CHAR:
			*odbcdr_datatype = SQL_C_CHAR;  
			*odbcdr_size    = 1;
			break;
		case RDBI_SHORT:
			*odbcdr_datatype = SQL_C_SHORT;
			*odbcdr_size    = sizeof(short);
			break;
		case RDBI_INT:
			*odbcdr_datatype = SQL_C_LONG;
			*odbcdr_size    = sizeof(int);
			break;
		case RDBI_LONG:
			*odbcdr_datatype = SQL_C_LONG;
			*odbcdr_size    = sizeof(long);
			break;
		case RDBI_FLOAT:
			*odbcdr_datatype = SQL_C_FLOAT;
			*odbcdr_size    = sizeof(float);
			break;
		case RDBI_DOUBLE:
			*odbcdr_datatype = SQL_C_DOUBLE;
			*odbcdr_size    = sizeof(double);
			break;
        case RDBI_DATE:
            *odbcdr_datatype = SQL_C_CHAR; //SQL_C_TIMESTAMP;
            *odbcdr_size = ODBCDR_DATE_SIZE + 1; 
            break;
        case RDBI_GEOMETRY:
            *odbcdr_datatype = SQL_C_BINARY; 
            *odbcdr_size    = sizeof(long);
            break;
		case RDBI_LONGLONG:
			*odbcdr_datatype = SQL_BIGINT;
			break;
		default:
			*odbcdr_datatype = SQL_C_CHAR;
	}

	debug4( "Rdbi in type %d, size %d, odbcdr out type %d, size %d",
			datatype, size, *odbcdr_datatype, *odbcdr_size );

	debug_return_void(NULL);
}

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
*	odbcdr_is_null - Returns the interpretation of the null indicator   *
*                                                                       *
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	odbcdr_is_null(null_ind) 								            *
*	char *null_ind;													    *
*   int  offset;                                                        *
*																		*
* Description															*
*       This function returns a boolean representing the interpretation *
*       in terms of 'null value' of the memory location passed in.      * 
*																		*
* Parameters															*
*	null_ind: input														*
*		The address of the null indicator.				                *
*																		*
*   offset: input                                                       *
*       Offset in the null_ind[] array.                                 *
*                                                                       *
* Function value														*
*	Returns TRUE if the null indicator is SQL_NULL_DATA, FALSE otherwise*
*																		*
************************************************************************/

#include "proto_p.h"
#include <Inc/ut.h>
#include <Inc/debugext.h>

int odbcdr_alcnullind(
	odbcdr_context_def *context,
    int                 n,           /* The number of elements to allocate   */
    char                **null_ind   /* address of pointer to allocate       */
    )
{
    debug_on1("odbcdr_alcnullind", "Number of elements %d", n);

    *null_ind = (char *) calloc( sizeof(SQLLEN), n);

    context->odbcdr_last_rc = RDBI_SUCCESS;
    if (*null_ind == (char *)NULL)
        context->odbcdr_last_rc = RDBI_MALLOC_FAILED;

    debug_return(NULL, context->odbcdr_last_rc);
}

void odbcdr_set_null(
	odbcdr_context_def  *context,
    SQLLEN              *null_ind,     /* NULL indicator array                     */
    int                 start,        /* The start index to begin setting NULL    */
    int                 end           /* The end index to set NULL                */
    )
{
    SQLLEN      *null_ind_addr = (SQLLEN *) null_ind;
    register int i;

    debug_on("odbcdr_set_null");

    if (end < start)
        end = start;

    for (i = start; i <= end; i++)
        null_ind[i] = SQL_NULL_DATA;

    debug_return_void(NULL);
}

void odbcdr_set_nnull(
	odbcdr_context_def  *context,
    SQLLEN              *null_ind,    /* NULL indicator array                     */
    int                 start,        /* The start index to begin setting NULL    */
    int                 end           /* The end index to set NULL                */
    )
{
    register int i;

    debug_on("odbcdr_set_nnull");

    if (end < start)
        end = start;

    // If the C type of a data buffer is SQL_C_CHAR or SQL_C_BINARY, 
    // and the length/indicator value is SQL_NTS, the string length 
    // of the data is put into the length/indicator buffer.
    // 
    for (i = start; i <= end; i++)
        null_ind[i] = SQL_NTS; 

    debug_return_void(NULL);
}

int odbcdr_is_null(
    odbcdr_context_def  *context,
	SQLLEN              *null_ind,
    int                 offset
	)
{
    SQLLEN      null_ind_val = null_ind[offset];
    int         ret;

	debug_on2("odbcdr_is_null", "null_ind[%d] = %d", null_ind_val, offset);

    ret = ( null_ind_val == SQL_NULL_DATA );

	debug_return(NULL, ret);
}

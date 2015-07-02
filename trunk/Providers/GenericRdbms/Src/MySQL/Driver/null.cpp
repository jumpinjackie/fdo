/************************************************************************
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
* Name                                                                  *
*   mysql_alcnullind - allocate a null indicator array                  *
*   mysql_set_null - set a member of a null indicator array NULL        *
*   mysql_set_nnull - set a member of a null indicator array NOT NULL   *
*   mysql_is_null - check the NULL status of a null indicator array member
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_alcnullind(n, null_ind)                                        *
*   int   n;                                                            *
*   short *null_ind;                                                    *
*                                                                       *
*   rdbi_set_null(null_ind, start, end)                                 *
*   short *null_ind;                                                    *
*   int   start;                                                        *
*   int   end;                                                          *
*                                                                       *
*   rdbi_set_nnull(null_ind, start, end)                                *
*   short *null_ind;                                                    *
*   int   start;                                                        *
*   int   end;                                                          *
*                                                                       *
*   rdbi_is_null(null_ind, offset)                                      *
*   short *null_ind;                                                    *
*   int   offset;                                                       *
*                                                                       *
* Description                                                           *
*       Interface  to  allocate,  set  and  check  NULL indicator       *
*       variables.                                                      *
*                                                                       *
* Parameters                                                            *
*   null_ind: input/output                                              *
*       The RDBMS-maintained array of indicator variables               *
*                                                                       *
*   start: input                                                        *
*       When  dealing  with a range of indicator  variables,  the       *
*       start index                                                     *
*                                                                       *
*   end: input                                                          *
*       When dealing with a range of indicator variables, the end       *
*       index                                                           *
*                                                                       *
*   offset: input                                                       *
*       When dealing with  an  individual indicator variable, the       *
*       index.                                                          *
*                                                                       *
* Function value                                                        *
*                                                                       *
* Remarks                                                               *
*																		*
*
************************************************************************/
#include "stdafx.h"

#include "null.h"

int mysql_alcnullind(
	mysql_context_def *context,
    int                 n,           /* The number of elements to allocate   */
    char                **null_ind   /* address of pointer to allocate       */
    )
{
    int     rc = RDBI_SUCCESS;

    *null_ind = (char *) malloc(sizeof(my_bool) * n);

    if (*null_ind == (char *)NULL)
        rc = RDBI_MALLOC_FAILED;

    return (rc);
}

void mysql_set_null(
	mysql_context_def  *context,
    my_bool             *null_ind,     /* NULL indicator array                     */
    int                 start,        /* The start index to begin setting NULL    */
    int                 end           /* The end index to set NULL                */
    )
{
    int  i;

    if (end < start)
        end = start;

    for (i = start; i <= end; i++)
        null_ind[i] = TRUE;
}

void mysql_set_nnull(
	mysql_context_def  *context,
    my_bool             *null_ind,    /* NULL indicator array                     */
    int                 start,        /* The start index to begin setting NULL    */
    int                 end           /* The end index to set NULL                */
    )
{
    int i;

    if (end < start)
        end = start;

    for (i = start; i <= end; i++)
        null_ind[i] = FALSE; 
}

int mysql_is_null(
    mysql_context_def  *context,
	my_bool            *null_ind,
    int                 offset
	)
{
    return ( null_ind[offset] );
}

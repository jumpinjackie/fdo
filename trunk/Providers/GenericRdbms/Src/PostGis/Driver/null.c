/*
 * Copyright (C) 2006 Refractions Research, Inc. 
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
 *
 */

/************************************************************************
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
*
************************************************************************/
#include "stdafx.h"

#include "null.h"
#include <Inc/Rdbi/context.h> /* TRUE, FALSE */
#include <stdlib.h> /* malloc() */
#include <string.h> /* memset() */
#include <assert.h>

/*
 * TODO: Remove asserts before merging with trunk
 */

int postgis_alloc_null_ind(
        postgis_context_def *context,
        int n,
        char** null_ind)
{
    int rc = RDBI_SUCCESS;

    *null_ind = (char *) malloc(sizeof(char) * n);

    if (*null_ind == (char *)NULL)
    {
        rc = RDBI_MALLOC_FAILED;
    }
    else
    {
        memset(*null_ind, 0, sizeof(char) * n);
    }

    return (rc);
}

void postgis_set_null(
        postgis_context_def* context,
        char* null_ind,
        int start,
        int end)
{
    int  i = 0;

    assert(NULL != null_ind);

    if (end < start)
    {
        end = start;
    }

    for (i = start; i <= end; i++)
    {
        null_ind[i] = TRUE;
    }
}

void postgis_set_not_null(
        postgis_context_def* context,
        char* null_ind,
        int start,
        int end)
{
    int i = 0;

    assert(NULL != null_ind);

    if (end < start)
    {
        end = start;
    }

    for (i = start; i <= end; i++)
    {
        null_ind[i] = FALSE; 
    }
}

int postgis_is_null(
        postgis_context_def  *context,
        char *null_ind,
        int offset)
{
    assert(NULL != null_ind);

    return ( null_ind[offset] );
}

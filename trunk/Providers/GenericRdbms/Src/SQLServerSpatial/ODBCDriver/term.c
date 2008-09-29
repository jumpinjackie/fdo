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
*   ocidr_term - Close down the driver - opposite of ocidr_rdbi_init    *
*                                                                       *
* Synopsis                                                              *
*   #include <inc/rdbi.h>                                               *
*   int ocidr_term(context)                                             *
*           void **context;                                             *
*                                                                       *
* Description                                                           *
*       Free any memory allocated by the driver and close up shop.      *
*                                                                       *
* Parameters                                                            *
*   context: input                                                      *
*       The context pointer initially passed in to ocidr_rdbi_init.     *
*                                                                       *
* Function Value                                                        *
*       An RDBI status integer.  Good is RDBI_SUCCESS (ie 0).           *
*                                                                       *
************************************************************************/

#include <Inc/ut.h>                     /* ut_vm_malloc()           */
#include <Inc/rdbi.h>                   /* rdbi status values       */
#include	<Inc/debugext.h>
#include "proto_p.h"

int odbcdr_term(
	void **contextp )
{
    odbcdr_context_def* context;
    debug_on( "odbcdr_term" );

    context = (odbcdr_context_def *)(*contextp);

#pragma message ("ToDo: make sure all ODBC connections are closed")

	/* Release the environment handle */
    if (NULL != context->odbcdr_env)
    	(void) SQLFreeHandle(SQL_HANDLE_ENV, context->odbcdr_env);

    ut_vm_free ("ocidr_term", context);
    *contextp = NULL;

    debug_return (NULL, RDBI_SUCCESS);
}

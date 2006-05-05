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
*   odbcdr_vndr_info - Get information about the SqlServer vendor       *
*                                                                       *
* Synopsis                                                              *
*   int odbcdr_vndr_info( rdbi_vndr_info_def *vndr_info )               *
*                                                                       *
* Description                                                           *
*       Get information about the SqlServer vendor                      *
*                                                                       *
* Parameters                                                            *
*   vndr_info: output                                                   *
*       Information about the Oracle vendor                             *
*                                                                       *
* Function Value                                                        *
*       An RDBI status integer.  Good is RDBI_SUCCESS (ie 0).           *                                                        *
*                                                                       *
************************************************************************/

#include <Inc/ut.h>                     /* ut_vm_malloc()           */
#include <Inc/rdbi.h>                   /* rdbi status values       */
#include <Inc/Rdbi/vndr_info.h>			/* rdbi_vndr_info_def		*/
#include	<Inc/debugext.h>
#include "constants.h"
#include "structs.h"
#include "proto_p.h"
#include "context.h"


int odbcdr_vndr_info(
    odbcdr_context_def	*context,
	rdbi_vndr_info_def *vndr_info)
{
    int                  rdbi_status = RDBI_SUCCESS;

    debug_on("odbcdr_vndr_info");

	/* name should always be within 30 char, just truncate if too long */

	strncpy( vndr_info->name, odbcdr_vndr_name(context), RDBI_TABLE_NAME_SIZE );
	vndr_info->name[RDBI_TABLE_NAME_SIZE] = '\0';

	vndr_info->minPrecision = 0;
	vndr_info->maxPrecision = 38;
	vndr_info->minScale = -84;	// To check
	vndr_info->maxScale = 127;	// To check
	vndr_info->minStrLen = 1;
	vndr_info->maxStrLen = 8000;
    vndr_info->dbversion = ODBCVER; 
	/* cannot fetch more than an array of 10 rows with Geometry */
	vndr_info->maxFetchSize = 10;

    debug_return(NULL,rdbi_status);
}

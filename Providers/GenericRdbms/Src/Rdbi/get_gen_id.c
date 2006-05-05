// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

/************************************************************************
* Name																	*
*	rdbi_get_gen_id - Gets the last value generated for an              *
*                     autoincremeneted column                           *
*                                                                       *
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	rdbi_get_gen_id(table_name, id) 								    *
*	char *table_name;													*
*	int  *id;														    *
*																		*
* Description															*
*		Some RDBMS implement IDENTITY/AUTO_INCREMENT column types.      *
*       This function returns the last generated one. 	 				*
*																		*
* Parameters															*
*	table_name: input													*
*		The table containing the autoincremented column of interest     *
*       (unique per table).	                                            *
*																		*
*	id: output														    *
*		The value.										                *
*																		*
* Function value														*
*	Returns an rdbi status code from Inc/rdbi.h 						*
*																		*
* Remarks																*
*																		*
************************************************************************/
#include <Inc/rdbi.h>
#include <Inc/debugext.h>
#include <Inc/Rdbi/proto.h>


int rdbi_get_gen_id(
    rdbi_context_def  *context,
	char *table_name,
	int  *id
	)
{
	int rdbi_status = RDBI_GENERIC_ERROR;

	debug_on1("rdbi_get_gen_id", "table_name '%s'", ISNULL(table_name));

    context->rdbi_last_status = (*(context->dispatch.get_gen_id)) (context->drvr, table_name, id);

    debug_return(NULL, context->rdbi_last_status);

}


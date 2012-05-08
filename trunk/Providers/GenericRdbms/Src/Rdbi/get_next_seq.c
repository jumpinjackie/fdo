// Copyright (C) 2010  Autodesk, Inc.
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
*	rdbi_get_next_seq - Gets the last value generated for an            *
*                     autoincremeneted column                           *
*                                                                       *
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	rdbi_get_next_seq(sequence_name, id) 								*
*	const char *sequence_name;											*
*	int  *id;														    *
*																		*
* Description															*
*		Gets the next value from the given RDBMS sequence.				*
*																		*
* Parameters															*
*	sequence_name: input												*
*		The sequence.                                                   *
*																		*
*	id: output														    *
*		The next value.		    						                *
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


int rdbi_get_next_seq(
    rdbi_context_def  *context,
	const char *sequence_name,
	long  *id
	)
{
	int rdbi_status = RDBI_GENERIC_ERROR;

	debug_on1("rdbi_get_next_seq", "table_name '%s'", ISNULL(sequence_name));

    if( context->dispatch.get_next_seq != NULL )
        context->rdbi_last_status = (*(context->dispatch.get_next_seq)) (context->drvr, sequence_name, id);

    debug_return(NULL, context->rdbi_last_status);

}

int rdbi_get_next_seqW(
    rdbi_context_def  *context,
	const wchar_t *sequence_name,
	long  *id
	)
{
	int rdbi_status = RDBI_GENERIC_ERROR;

	debug_on1("rdbi_get_next_seq", "table_name '%ls'", ISNULL(sequence_name));

    if( context->dispatch.get_next_seqW != NULL )
        context->rdbi_last_status = (*(context->dispatch.get_next_seqW)) (context->drvr, sequence_name, id);

    debug_return(NULL, context->rdbi_last_status);

}


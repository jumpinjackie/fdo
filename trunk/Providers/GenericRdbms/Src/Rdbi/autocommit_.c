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
* Name                                                                  *
*   rdbi_commit_on - turns autocommit mode on							*
*                                                                       *
************************************************************************/


#include    <Inc/Nls/rdbi_msg.h>
#include <Inc/debugext.h>
#include <Inc/rdbi.h>       /* for RDBI_COMMIT_ONCE     */
#include "global.h"         /* for rdbi_last_status     */
#include <Inc/Rdbi/context.h>

int
rdbi_autocommit_on(rdbi_context_def *context)
{
    debug_on("rdbi_autocommit_on");

	if (context->dispatch.autocommit_off != NULL)
	{
		context->rdbi_last_status = (*(context->dispatch.autocommit_on))(context->drvr);
		if (context->rdbi_last_status == RDBI_SUCCESS)
			context->rdbi_cnct->autocommit_on = FALSE;
	}
	else
		context->rdbi_last_status = RDBI_SUCCESS;

    debug_return(NULL, context->rdbi_last_status);
}

int
rdbi_autocommit_off(rdbi_context_def *context)
{
    debug_on("rdbi_autocommit_off");

	if (context->dispatch.autocommit_off != NULL)	
	{
		context->rdbi_last_status = (*(context->dispatch.autocommit_off))(context->drvr);
		if (context->rdbi_last_status == RDBI_SUCCESS)
			context->rdbi_cnct->autocommit_on = TRUE;
	}
	else
		context->rdbi_last_status = RDBI_SUCCESS;

    debug_return(NULL, context->rdbi_last_status);
}

int
rdbi_autocommit_mode(rdbi_context_def *context)
{
	int mode; /* 0 or 1 */
	debug_on("rdbi_autocommit_mode");

	if (context->dispatch.autocommit_mode !=NULL)
		mode = (*(context->dispatch.autocommit_mode))(context->drvr);
	else
		mode = TRUE;

	debug_return(NULL, mode);
}




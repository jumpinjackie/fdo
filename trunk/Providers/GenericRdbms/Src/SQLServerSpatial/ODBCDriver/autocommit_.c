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
*   odbcdr_commit - Commit a transaction or turn autocommit on or off   *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   int obdcdr_commit(commit_action)                                    *
*   int         commit_action;                                          *
*                                                                       *
* Description                                                           *
*       Depending on the commit action, either commit the current       *
*       transaction  (which  will free all locks), or either turn       *
*       on or turn off the 'autocommit' mode.                           *
*                                                                       *
* Parameters                                                            *
*   commit_action:  input                                               *
*       Indicate whether the desired action is to:                      *
*           RDBI_COMMIT_ONCE:       Commit the current transaction      *
*           RDBI_COMMIT_AUTO_ON:    Turn autocommit mode on             *
*           RDBI_COMMIT_AUTO_OFF:   Turn autocommit mode off            *
*       See inc/rdbi.h                                                  *
*                                                                       *
* Function Value                                                        *
*       An RDBI status  integer.   Good  is  RDBI_SUCCESS (ie 0).       *
*       See inc/rdbi.h                                                  *
*                                                                       *
************************************************************************/

#include <Inc/ut.h> 				
#include <Inc/debugext.h>
#include <Inc/rdbi.h>			/* rdbi status values		*/
#include "proto_p.h"


int odbcdr_autocommit_on(
    odbcdr_context_def *context
	)
{
	int					rdbi_status = RDBI_GENERIC_ERROR;
	SQLRETURN   rc;
	odbcdr_connData_def	*connData;

	//odbcdr_service_ctx_def	hDbc = SQL_NULL_HDBC;

	debug_on("odbcdr_autocommit_on");

	//rc = SQLAllocHandle(SQL_HANDLE_DBC,	context->odbcdr_env, &hDbc);
	rc = odbcdr_get_curr_conn( context, &connData );
	if (rc == RDBI_SUCCESS)
	{
		rc = SQLSetConnectAttr(connData->hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER);

		if ( rc != SQL_ERROR )
			rdbi_status = RDBI_SUCCESS;
	}
	
	debug_return(NULL, rdbi_status);
}

int odbcdr_autocommit_off(
    odbcdr_context_def *context
	)
{
	int					rdbi_status = RDBI_GENERIC_ERROR;
	SQLRETURN   rc;
	odbcdr_connData_def	*connData;

	debug_on("odbcdr_autocommit_off");

	rc = odbcdr_get_curr_conn( context, &connData );

	if (rc == RDBI_SUCCESS)
    {
        if (ODBCDriverType_SQLServer == connData->driver_type)
	    {
		    rc = SQLSetConnectAttr(connData->hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);

		    if ( rc != SQL_ERROR )
			    rdbi_status = RDBI_SUCCESS;
	    }
        else
        {
            rdbi_status = RDBI_SUCCESS;
        }
    }

	debug_return(NULL, rdbi_status);
}

int odbcdr_autocommit_mode(
	   odbcdr_context_def *context
	   )
{	
	int					rdbi_status = RDBI_GENERIC_ERROR;
	SQLRETURN   rc;
	SQLUINTEGER  mode = SQL_AUTOCOMMIT_ON;
	SQLINTEGER	 len;
	odbcdr_connData_def	*connData;
	//odbcdr_service_ctx_def	hDbc = SQL_NULL_HDBC;

	debug_on("odbcdr_autocommit_mode");

	//rc = SQLAllocHandle(SQL_HANDLE_DBC,	context->odbcdr_env, &hDbc);
	rc = odbcdr_get_curr_conn( context, &connData );
	if (rc == RDBI_SUCCESS)
		rc = SQLGetConnectAttr(connData->hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)&mode, sizeof(mode), &len);

	debug_return(NULL, (int)mode);
}


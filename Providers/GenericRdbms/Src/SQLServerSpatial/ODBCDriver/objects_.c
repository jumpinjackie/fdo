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

#include    <Inc/rdbi.h>
#include    <Inc/ut.h>
#include	<Inc/debugext.h>
#include    "proto_p.h"

#define _check_status  if (rdbi_status != RDBI_SUCCESS) goto the_exit;
#ifndef _WIN32
#define  strnicmp strncasecmp
#define  _strnicmp strncasecmp
#define _stricmp  strcasecmp
#define _wcsnicmp wcsncasecmp
#endif
#define TABLE_TYPES   "TABLE,VIEW"
#define TABLE_TYPES2   "TABLE"
#define TABLE_TYPESW  L"TABLE,VIEW"
#define TABLE_TYPES2W  L"TABLE"
#define TABLE_STRING  "TABLE"
#define TABLE_STRINGW L"TABLE"
#define VIEW_STRING   "VIEW"
#define VIEW_STRINGW  L"VIEW"

/************************************************************************
*																		*
* Name																	*
*	odbcdr_objects_act - Activate a fetch of all objects for a user     *
*																		*
* Synopsis																*
*	odbcdr_objects_act(context, target)    				    			*
*	odbcdr_context_def *context;										*
*   rdbi_string_def *owner;                                                  *
*   rdbi_string_def *target;                                                 *
*																		*
* Description															*
*		This  module  activates  a	fetch  of  all objects for a  		*
*		connection in an ODBC data source.   							*
*																		*
* Parameters															*
*   owner: input                                                        *
*       The name of the owner.  This is typically a user name.          *
*       This may be NULL or empty, in which case the default schema     *
*       is assumed.                                                     *
*                                                                       *
*	target: input 		        										*
*		The name of a specific object.  Results will be limited to this *
*       object if given.  Pass NULL to get all objects.                 *
*																		*
* Function value														*
*		Returns an rdbi status code from inc/rdbi.h 					*
*																		*
* Remarks																*
*       This code is NOT re-entrant.  There is no handle to return      *
*       to identify a specific query of objects.  It operates the same  *
*       way as the older odbcdr_col_*() functions.                      *
*                                                                       *
*       This actually gets all objects in a SCHEMA.                     *
*                                                                       *
*       This only gets tables and views.  Indexes and synonyms are not  *
*       supported, mainly because the calling code isn't expecting it.  *
*
************************************************************************/

int local_odbcdr_objects_act(
    odbcdr_context_def *context,
    rdbi_string_def *owner,
    rdbi_string_def *target
	)
{
    SQLWCHAR     szObjectNameBuf[ODBCDR_MAX_BUFF_SIZE];
    rdbi_string_def szObjectName;
    SQLWCHAR     szObjectTypeBuf[ODBCDR_MAX_BUFF_SIZE];
    rdbi_string_def szObjectType;
	odbcdr_cursor_def	*c;
	odbcdr_connData_def	*connData = NULL;
	int 				rdbi_status = RDBI_GENERIC_ERROR;
    int                 owner_set = !ODBCDRV_STRING_EMPTY(owner);
	int 				target_set = !ODBCDRV_STRING_EMPTY(target);
    SQLLEN      cbTable = 0;
    SQLLEN      cbTable2 = 0;
    SQLRETURN   ret = SQL_SUCCESS;
    long        i;
    int         found;
	odbcdr_NameListEntry_obj_def * nle;
    SQLSMALLINT charType;
    szObjectName.wString = (wchar_t *)szObjectNameBuf;
    szObjectType.wString = (wchar_t *)szObjectTypeBuf;

	debug_on("odbcdr_objects_act");

	ODBCDR_RDBI_ERR( odbcdr_get_curr_conn( context, &connData ) );

	/* Deactivate any outstanding queries */
    if (target_set)
    {
        context->odbcdr_singletonNameInUse = false;
        context->odbcdr_singletonNameReturned = false;
    }
    else
    {
        context->odbcdr_nameListNextPosition_objs = -1;
        (void) ut_da_free(&context->odbcdr_nameList_objs);
    }

    c = connData->objects;

    if (c == (odbcdr_cursor_def *)NULL) 
    {
	    rdbi_status = odbcdr_est_cursor( context, (char **)&c);
	    _check_status;
	    connData->objects = c;
    }

    if (context->odbcdr_UseUnicode)
    {
        SQLWCHAR * table_typesw = (SQLWCHAR *)TABLE_TYPESW;
        if (ODBCDriverType_Sybase == connData->driver_type)
            table_typesw = (SQLWCHAR *)TABLE_TYPES2W;

        charType = SQL_C_WCHAR;

        ODBCDR_ODBC_ERR( SQLTablesW(c->hStmt, NULL, 0, (SQLWCHAR *) (owner_set ? owner->cwString : NULL), 
            SQL_NTS, NULL, 0, table_typesw,  SQL_NTS), 
            SQL_HANDLE_STMT, c->hStmt, "SQLTables", "Fetching tables and views");
    }else{
        SQLCHAR * table_types = (SQLCHAR *)TABLE_TYPES;
        if (ODBCDriverType_Sybase == connData->driver_type)
            table_types = (SQLCHAR *)TABLE_TYPES2;

        charType = SQL_C_CHAR;

        ODBCDR_ODBC_ERR( SQLTables(c->hStmt, NULL, 0, (SQLCHAR *) (owner_set ? owner->ccString : NULL), 
            SQL_NTS, NULL, 0, table_types,  SQL_NTS), 
            SQL_HANDLE_STMT, c->hStmt, "SQLTables", "Fetching tables and views");
    }

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt,
        3,
        charType,
        szObjectName.wString,
        ODBCDR_MAX_BUFF_SIZE,
        &cbTable),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching tables and views");

    ODBCDR_ODBC_ERR( SQLBindCol(
        c->hStmt,
        4,
        charType,
        szObjectType.wString,
        ODBCDR_MAX_BUFF_SIZE,
        &cbTable2),
        SQL_HANDLE_STMT, c->hStmt, "SQLBindCol", "Fetching tables and views");

    while (ret != SQL_NO_DATA)
    {
        *szObjectName.wString = L'\0';
        *szObjectType.wString = L'\0';

        ret = SQLFetch(c->hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
            goto the_exit;

        if (ret == SQL_NO_DATA)
            break;

        // Filter out other objects if asked for a specific object.
        if (target_set && ODBCDRV_STRING_COMPARE(target, &szObjectName) != 0)
            continue;

        /* We only support tables and views. */
        if (ODBCDRV_STRING_COMPARE_LST(&szObjectType, TABLE_STRING) != 0 && ODBCDRV_STRING_COMPARE_LST(&szObjectType, VIEW_STRING) != 0)
            continue;

        if (target_set)
        {
            ODBCDRV_STRING_COPY_RST(context->odbcdr_singletonName_objects.name, &szObjectName);
            context->odbcdr_singletonName_objects.type = (char)(context->odbcdr_UseUnicode ? *szObjectType.wString : *szObjectType.cString);
            context->odbcdr_singletonNameInUse = true;
        }
        else
        {
            /* Look for the name already in the list. */
            for (i=0, found=false;  !found && i < context->odbcdr_nameList_objs.size;  i++)
            {
                nle = (odbcdr_NameListEntry_obj_def *) ut_da_get(&context->odbcdr_nameList_objs, i);
                if (ODBCDRV_STRING_COMPARE_LST(&szObjectName, nle->name) == 0)
                    found = true;
            }

            if (!found)
            {
                /* Add name to the list. */
                odbcdr_NameListEntry_obj_def newNle;
                ODBCDRV_STRING_COPY_RST(newNle.name, &szObjectName);
                newNle.type = (char)(context->odbcdr_UseUnicode ? *szObjectType.wString : *szObjectType.cString);
                if (NULL == ut_da_append( &context->odbcdr_nameList_objs, 1L, (void *) &newNle ))
                {
                    rdbi_status = RDBI_MALLOC_FAILED;
                    goto the_exit;
                }
            }
        	context->odbcdr_nameListNextPosition_objs = 0;
        }
    } /* end while (ret != SQL_NO_DATA) */

the_exit:
	if (NULL != connData && NULL != connData->objects)
    {
        /* Ideally, we would keep the cursor around, and just re-execute later.
         * However, many ODBC drivers do not support concurrent cursors, and
         * calls to the module may be interleaved with others.  To avoid an
         * "Invalid cursor state" error from SQLTables later,
         * we'll have to free the cursor here. 
         */
		ODBCDR_RDBI_ERR( odbcdr_fre_cursor(context, (char **)(&connData->objects)) );
        connData->objects = NULL;
    }
	debug_return(NULL, rdbi_status);
}

int odbcdr_objects_act(
    odbcdr_context_def *context,
    const char *owner,
    const char *target
	)
{
    rdbi_string_def strOwner;
    rdbi_string_def strTarget;
    strOwner.ccString = owner;
    strTarget.ccString = target;
    return local_odbcdr_objects_act(context, &strOwner, &strTarget);
}

int odbcdr_objects_actW(
    odbcdr_context_def *context,
    const wchar_t *owner,
    const wchar_t *target
	)
{
    rdbi_string_def strOwner;
    rdbi_string_def strTarget;
    strOwner.cwString = owner;
    strTarget.cwString = target;
    return local_odbcdr_objects_act(context, &strOwner, &strTarget);
}

/************************************************************************
*																		*
* Name																	*
*	odbcdr_objects_get - Get the next object.							*
*																		*
* Synopsis																*
*	odbcdr_objects_get(context, name, eof)								*
*	odbcdr_context_def *context;										*
*	rdbi_string_def *name;		        								*
*   rdbi_string_def *type;                                              *
*	int  *eof;															*
*																		*
* Description															*
*		This module gets the next user in  the  fetch activated		    *
*		by odbcdr_objects_act.											*
*																		*
* Parameters															*
*	name: output 		        										*
*		The name of the column. 										*
*																		*
*   type: output                                                        *
*       The type of the object.  One of:                                *
*           "T" (Table)                                                 *
*           "V" (View)                                                  *
*           "I" (Index)                                                 *
*           "S" (Synonym)                                               *
*                                                                       *
*	eof: output 														*
*		End of fetch flag.												*
*																		*
* Function value														*
*		Returns an rdbi status code from inc/rdbi.h 					*
*																		*
* Remarks																*
*																		*
************************************************************************/
int local_odbcdr_objects_get(
    odbcdr_context_def *context,
	rdbi_string_def *name,
    rdbi_string_def *type,
	int  *eof
	)
{
  	int	rdbi_status = RDBI_GENERIC_ERROR;
    
	debug_on("odbcdr_objects_get");

	/* assume not end of fetch */
	*eof = FALSE;

    if (context->odbcdr_singletonNameInUse)
    {
        if (context->odbcdr_singletonNameReturned)
        {
            *eof = TRUE;
            context->odbcdr_singletonNameInUse = false;
            context->odbcdr_singletonNameReturned = false;
        }
        else
        {
            ODBCDRV_STRING_COPY_LST(name, context->odbcdr_singletonName_objects.name);
            if (context->odbcdr_UseUnicode)
            {
                *type->wString = (wchar_t)context->odbcdr_singletonName_objects.type;
                *(type->wString+1) = L'\0';
            }
            else
            {
                *type->cString = (char)context->odbcdr_singletonName_objects.type;
                *(type->cString+1) = '\0';
            }
            context->odbcdr_singletonNameReturned = true;

            // Callers for singletons only fetch one row, never looping to
            // trigger and end-of-fetch.  End the query now.
            context->odbcdr_singletonNameInUse = false;
        }
    }
    else /* Look for next name in list. */
    {
        if (context->odbcdr_nameListNextPosition_objs < 0 || context->odbcdr_nameListNextPosition_objs > context->odbcdr_nameList_objs.size)
        {
            // Simply "goto the_exit" (resulting in a run-time error) is the right thing to do here,
            // but returning EOF here works around some obscure defect
            // in the Schema Manager.
            *eof = TRUE;
            context->odbcdr_nameListNextPosition_objs = -1;
            (void) ut_da_free(&context->odbcdr_nameList_objs);
            rdbi_status = RDBI_SUCCESS;
            goto the_exit;
        }

        if (context->odbcdr_nameListNextPosition_objs == context->odbcdr_nameList_objs.size)
        {
            *eof = TRUE;
            context->odbcdr_nameListNextPosition_objs = -1;
            (void) ut_da_free(&context->odbcdr_nameList_objs);
        }
        else
        {
            odbcdr_NameListEntry_obj_def * nle = (odbcdr_NameListEntry_obj_def *) ut_da_get(&context->odbcdr_nameList_objs, context->odbcdr_nameListNextPosition_objs++);
            ODBCDRV_STRING_COPY_LST(name, nle->name);
            if (context->odbcdr_UseUnicode)
            {
                *type->wString = (wchar_t)nle->type;
                *(type->wString+1) = L'\0';
            }
            else
            {
                *type->cString = (char)nle->type;
                *(type->cString+1) = '\0';
            }
        }
    }

	rdbi_status = RDBI_SUCCESS;
the_exit:
	debug_return(NULL, rdbi_status);
}

int odbcdr_objects_get(
    odbcdr_context_def *context,
	char *name,
    char *type,
	int  *eof
	)
{
    rdbi_string_def strName;
    rdbi_string_def strType;
    strName.cString = name;
    strType.cString = type;
    return local_odbcdr_objects_get(context, &strName, &strType, eof);
}

int odbcdr_objects_getW(
    odbcdr_context_def *context,
	wchar_t *name,
    wchar_t *type,
	int  *eof
	)
{
    rdbi_string_def strName;
    rdbi_string_def strType;
    strName.wString = name;
    strType.wString = type;
    return local_odbcdr_objects_get(context, &strName, &strType, eof);
}

/************************************************************************
*																		*
* Name																	*
*	odbcdr_objects_deac - Deactivate the fetch of objects.				*
*																		*
* Synopsis																*
*	odbcdr_objects_deac(context)										*
*	odbcdr_context_def *context;										*
*																		*
* Description															*
*		This  module  deactivates	the   fetch  of  all  objects 		*
*		activated by odbcdr_objects_act.								*
*																		*
* Parameters															*
*																		*
* Function value														*
*		Returns an rdbi status code from inc/rdbi.h 					*
*																		*
* Remarks																*
*																		*
************************************************************************/

int odbcdr_objects_deac( odbcdr_context_def *context )
{
	int 				rdbi_status = RDBI_GENERIC_ERROR;

	debug_on("odbcdr_objects_deac");

    if (context->odbcdr_singletonNameInUse)
    {
        context->odbcdr_singletonNameInUse = false;
        context->odbcdr_singletonNameReturned = false;
    }
    else
    {
        context->odbcdr_nameListNextPosition_objs = -1;
        (void) ut_da_free(&context->odbcdr_nameList_objs);
    }

	rdbi_status = RDBI_SUCCESS;
	debug_return(NULL, rdbi_status);
}

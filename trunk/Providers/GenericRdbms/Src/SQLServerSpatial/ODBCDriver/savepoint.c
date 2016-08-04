// Copyright (C) 2004-2011  Autodesk, Inc.
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
*   rdbi_tran_sp - Execute a save point related operation               *
*                                                                       *
* Synopsis                                                              *
*   odbcdr_trans_sp/odbcdr_tran_spW(action, sp_name)                    *
*   int          action;                                                *
*   const char/wchar_t   *sp_name;                                      *
*                                                                       *
* Description                                                           *
*       Execute a save point related operation, the operation can       *
*       be adding a save point, rolling back to a save point or         *
*       releasing a save point according to the <action> type. All      *
*       these save points are within a single transaction and           *
*       identified by unique save point names.                          *
*                                                                       *
* Parameters                                                            *
*   action: input                                                       *
*       The type of action to be taken.                                 *
*       action value:                                                   *
*       1 - add a save point                                            *
*       2 - roll back to a save point                                   *
*       3 - release a save point                                        *
*                                                                       *
*   sp_name: input                                                      *
*       the name of the save point on which operation will perform on   *
*                                                                       *
* Function value                                                        *
*       Returns an rdbi status code.                                    *
*                                                                       *
* Remarks                                                               *
*       Release save point operation is not supported by all RDBMSs     *
*                                                                       *
************************************************************************/
#include <stdio.h>
#include <Inc/ut.h>
#include "proto_p.h"

static int local_tran_sp( 
    odbcdr_context_def    *context,
    int                  action,  /* action type */
    rdbi_string_def     *sp_name  /* save point name */
    );

static int local_tran_sp_add( 
    odbcdr_context_def    *context,
    rdbi_string_def     *sp_name  /* save point name */
    );

static int local_tran_sp_rolbk( 
    odbcdr_context_def    *context,
    rdbi_string_def     *sp_name  /* save point name */
    );

static int local_tran_sp_rel( 
    odbcdr_context_def    *context,
    rdbi_string_def     *sp_name  /* save point name */
    );

int odbcdr_tran_sp(odbcdr_context_def * context, int action, const char* sp_name)
{
    rdbi_string_def sp;
    sp.ccString = sp_name;
    return local_tran_sp(context, action, &sp);
}

int odbcdr_tran_spW(odbcdr_context_def * context, int action, const wchar_t* sp_name)
{
     rdbi_string_def sp;
     sp.cwString = sp_name;
     return local_tran_sp(context, action, &sp);
}

int local_tran_sp(odbcdr_context_def *context, int action, rdbi_string_def *sp_name)
{
    int rc = RDBI_SUCCESS;
    switch(action)
    {
        case RDBI_SP_ADD:
            rc = local_tran_sp_add(context, sp_name);
            break;
        case RDBI_SP_RB:
            rc = local_tran_sp_rolbk(context, sp_name);
            break;
        case RDBI_SP_RL:
            rc = local_tran_sp_rel(context, sp_name);
            break;
        default:
            rc = RDBI_SP_NOT_SUPPORTED;
    }
    return rc;
}

int local_tran_sp_add(odbcdr_context_def *context, rdbi_string_def *sp_name)
{
    int rc = RDBI_SUCCESS;
    if(context->odbcdr_UseUnicode)
    {
        wchar_t* sp = (wchar_t*) ut_vm_malloc("local_tran_sp_add", sizeof(wchar_t) *(wcslen(sp_name->cwString) + 18));
        wsprintfW(sp, L"SAVE TRANSACTION %ls", sp_name->cwString);
        rc = odbcdr_run_sqlW(context, sp, 0, NULL);
        ut_vm_free("local_tran_sp_add", sp);
    }
    else
    {
        char* sp = (char*) ut_vm_malloc("local_tran_sp_add", sizeof(char) *(strlen(sp_name->ccString) + 18));
        wsprintfA(sp, "SAVE TRANSACTION %s", sp_name->ccString);
        rc = odbcdr_run_sql(context, sp, 0, NULL);
        ut_vm_free("local_tran_sp_add", sp);
    }
    return rc;
}

int local_tran_sp_rolbk(odbcdr_context_def  *context, rdbi_string_def *sp_name)
{
    int rc = RDBI_SUCCESS;
    if(context->odbcdr_UseUnicode)
    {
         wchar_t* sp = (wchar_t*) ut_vm_malloc("local_tran_sp_rolbk", sizeof(wchar_t) *(wcslen(sp_name->cwString) + 22));
         wsprintfW(sp, L"ROLLBACK TRANSACTION %ls", sp_name->cwString);
         rc = odbcdr_run_sqlW(context, sp, 0, NULL);
         ut_vm_free("local_tran_sp_add", sp);
    }
    else
    {
        char* sp = (char*) ut_vm_malloc("local_tran_sp_rolbk", sizeof(char) *(strlen(sp_name->ccString) + 22));
        wsprintfA(sp, "ROLLBACK TRANSACTION %s", sp_name->ccString);
        rc = odbcdr_run_sql(context, sp, 0, NULL);
        ut_vm_free("local_tran_sp_add", sp);
    }
    return rc;
}

int local_tran_sp_rel(odbcdr_context_def *context, rdbi_string_def *sp_name)
{
    return RDBI_SP_NOT_SUPPORTED;
}


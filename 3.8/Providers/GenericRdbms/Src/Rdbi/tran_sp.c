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
*   rdbi_tran_sp/rdbi_tran_spW - Execute a save point related operation *
*                                                                       *
* Synopsis                                                              *
*   rdbi_tran_sp/rdbi_tran_spW(action, sp_name)                         *
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
*       3 - release a save point to the transaction                     *
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
#include <string.h>
#include <Inc/Rdbi/proto.h>
#include <Inc/debugext.h>
#include <Inc/Nls/rdbi_msg.h>
#include <Inc/ut.h>

static sp_entry_def* rdbi_find_sp_entry(rdbi_context_def *context, const char* sp_name);
static sp_entry_def* rdbi_find_sp_entryW(rdbi_context_def *context, const wchar_t* sp_name);

int rdbi_tran_sp(rdbi_context_def *context, int action, const char* sp_name)
{
    int status = RDBI_SUCCESS;

    debug_on("rdbi_tran_sp");
    char* name = (char*)ut_vm_malloc("rdbi_tran_spW", sizeof(char) * (strlen(sp_name) + 1));
    strcpy(name, sp_name);
     
    //Can only start save point operation when a transaction is active
    if(context->rdbi_cnct->tran_head == NULL)
    {
        status = RDBI_39;
        rdbi_msg_set_0(context, RDBI_39, "Invalid operation, no transaction is active.");
        goto the_exit;
    }
    if(context->dispatch.tran_sp != NULL)
    {
        status = (*(context->dispatch.tran_sp))(context->drvr, action, sp_name);
         if(status != RDBI_SUCCESS)
        {
            goto the_exit;
        }

        if(action == RDBI_SP_ADD)
        {
            if(rdbi_tran_sp_exists(context, sp_name))
            {
                status = RDBI_38;
                rdbi_msg_set_S(context, RDBI_38, "Save point already exists.", name);
                goto the_exit;
            }
                
            //Create the save point entry
            sp_entry_def* newsp_entry = (sp_entry_def*)ut_vm_malloc("rdbi_tran_sp", sizeof(sp_entry_def));
            newsp_entry->next = context->rdbi_cnct->sp_head;
            newsp_entry->cString = (char *)ut_vm_malloc("rdbi_tran_sp", (strlen(sp_name) + 1) * sizeof(char));
            strcpy(newsp_entry->cString, sp_name);
            context->rdbi_cnct->sp_head = newsp_entry;
        }
        else if(action == RDBI_SP_RB)
        {
            sp_entry_def* found = rdbi_find_sp_entry(context, sp_name);
            if(found == NULL)
            {
                status = RDBI_37;
                rdbi_msg_set_S(context, RDBI_37, "Save point does not exist.", name);
                goto the_exit;
            }
            sp_entry_def* entry = context->rdbi_cnct->sp_head;
            while(entry != found)
            {
                sp_entry_def* my_entry = entry;
                entry = entry->next;
                ut_vm_free("rdbi_tran_sp", my_entry->cString);
                ut_vm_free("rdbi_tran_sp", my_entry);
            }

            context->rdbi_cnct->sp_head = found;
        }
        else if(action == RDBI_SP_RL)
        {
            sp_entry_def* found = rdbi_find_sp_entry(context, sp_name);
            if(found == NULL)
            {
                status = RDBI_37;
                rdbi_msg_set_S(context, RDBI_37, "Save point does not exist.", name);
                goto the_exit;
            }
            sp_entry_def* entry = context->rdbi_cnct->sp_head;
            sp_entry_def* last_entry = NULL;
            
            while(entry && entry != found)
            {
                last_entry = entry;
                entry = entry->next;
            }
            
            if(last_entry != NULL)
            {
                last_entry->next = entry->next;
            }
            else
            {
                context->rdbi_cnct->sp_head = entry->next;
            }
            
            ut_vm_free("rdbi_tran_sp", found->cString);
            ut_vm_free("rdbi_tran_sp", found);
        }
    }

the_exit:
    context->rdbi_last_status = status;
    if(status != RDBI_SUCCESS)
        ut_vm_free("rdbi_tran_spW", name);
    debug_return(NULL, status);
}

int rdbi_tran_spW(rdbi_context_def *context, int action, const wchar_t* sp_name)
{
    int status = RDBI_SUCCESS;
    debug_on("rdbi_tran_spW");

    //Can only start save point operation when a transaction is active
    if(context->rdbi_cnct->tran_head == NULL)
    {
        status = RDBI_39;
        rdbi_msg_set_0(context, RDBI_39, "Invalid operation, no transaction is active.");
        goto the_exit;
    }
   
    if(context->dispatch.tran_spW != NULL)
    {
        status = (*(context->dispatch.tran_spW))(context->drvr, action, sp_name);
        
        if(status != RDBI_SUCCESS)
        {
            goto the_exit;
        }

        if(action == RDBI_SP_ADD)
        {
            if(rdbi_tran_sp_existsW(context, sp_name))
            {
                status = RDBI_38;
                rdbi_msg_set_SW(context, RDBI_38, "Save point '%1$ls' already exists.", (wchar_t*)sp_name);
                goto the_exit;
            }
            //Create the save point entry
            sp_entry_def* newsp_entry = (sp_entry_def*)ut_vm_malloc("rdbi_tran_spW", sizeof(sp_entry_def));
            newsp_entry->next = context->rdbi_cnct->sp_head;
            newsp_entry->wString = (wchar_t *)ut_vm_malloc("rdbi_tran_spW", (wcslen(sp_name) + 1) * sizeof(wchar_t));
            wcscpy(newsp_entry->wString, sp_name);
            context->rdbi_cnct->sp_head = newsp_entry;
        }
        else if(action == RDBI_SP_RB)
        {
            sp_entry_def* found = rdbi_find_sp_entryW(context, sp_name);
            if(found == NULL)
            {
                status = RDBI_37;
                rdbi_msg_set_SW(context, RDBI_37, "Save point '%1$ls' already exists.", (wchar_t*)sp_name);
                goto the_exit;
            }
            sp_entry_def* entry = context->rdbi_cnct->sp_head;
            while(entry != found)
            {
                sp_entry_def* my_entry = entry;
                entry = entry->next;
                ut_vm_free("rdbi_tran_spW", my_entry->wString);
                ut_vm_free("rdbi_tran_spW", my_entry);
            }

            context->rdbi_cnct->sp_head = found;         
        }
        else if(action == RDBI_SP_RL)
        {
            sp_entry_def* found = rdbi_find_sp_entryW(context, sp_name);
            if(found == NULL)
            {
                rdbi_msg_set_SW(context, RDBI_37, "Save point '%1$ls' already exists.", (wchar_t*)sp_name);
                goto the_exit;
            }
            sp_entry_def* entry = context->rdbi_cnct->sp_head;
            sp_entry_def* last_entry = NULL;
        
            while(entry && entry != found)
            {
                last_entry = entry;
                entry = entry->next;
            }
            
            if(last_entry != NULL)
            {
                last_entry->next = entry->next;
            }
            else
            {
                context->rdbi_cnct->sp_head = entry->next;
            }
            
            ut_vm_free("rdbi_tran_spW", found->wString);
            ut_vm_free("rdbi_tran_spW", found);
        }
    }

the_exit:
    context->rdbi_last_status = status;
    debug_return(NULL, context->rdbi_last_status);
}


int rdbi_tran_sp_exists(rdbi_context_def *context, const char* sp_name)
{
    return rdbi_find_sp_entry(context, sp_name) != NULL;
}

int rdbi_tran_sp_existsW(rdbi_context_def *context, const wchar_t* sp_name)
{
    return rdbi_find_sp_entryW(context, sp_name) != NULL;
}


sp_entry_def* rdbi_find_sp_entry(rdbi_context_def *context, const char* sp_name)
{
    sp_entry_def* entry = context->rdbi_cnct->sp_head;
    while(entry != NULL)
    {
        const char* name= entry->cString;
        if(strcmp(sp_name, name) == 0)
        {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

sp_entry_def* rdbi_find_sp_entryW(rdbi_context_def *context, const wchar_t* sp_name)
{
    sp_entry_def* entry = context->rdbi_cnct->sp_head;
    while(entry != NULL)
    {
        const wchar_t* name= entry->wString;
        if(wcscmp(sp_name, name) == 0)
        {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}



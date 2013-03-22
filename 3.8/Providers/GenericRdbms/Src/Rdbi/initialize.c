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
*                                                                       *
* Name                                                                  *
*   rdbi_initialize - initialize the rdbms.                             *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/rdbi.h>                                               *
*   rdbi_initialize()                                                   *
*                                                                       *
* Description                                                           *
*       Initialize the rdbms by calling loading the dynamic library     *
*       locating the exported initialize routine and then calling it.   *
*                                                                       *
* Parameters                                                            *
*                                                                       *
* Function value                                                        *
*   TRUE iff successful, FALSE otherwise.                               *
*                                                                       *
* Remarks                                                               *
*       Returns an rdbi status code.   Status  definitions can be       *
*       found in <Inc/rdbi.h>.                                          *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.            *
*                                                                       *
************************************************************************/


#include <windows.h>

#include <Inc/ut.h>
#include <Inc/debugext.h>
#include <Inc/Nls/rdbi_msg.h>
#include <Inc/Rdbi/context.h>
#include <Inc/Rdbi/proto.h>

#include "global.h"

int rdbi_initialize (rdbi_context_def **contextp, char* driver)
{
    DWORD nchars;
    char module[MAX_PATH];
    char* last;
    HMODULE handle;
    FARPROC procedure;
    int status;

    debug_on("rdbi_initialize");

    nchars = GetModuleFileName (NULL, module, MAX_PATH);
    if (0 == nchars)
        strcpy (module, ".\\");
    else
    {   
        // scan the string for the last occurrence of a slash
        last = strrchr (module, '\\');
        if (NULL != last)
        {
            last++; // move past the slash
            *last = '\0'; // null terminate it there
        }
    }
    strcat (module, driver);
#ifdef _WIN32
    strcat (module, ".dll");
#else
    strcat (module, ".so"); // doesn't really work here, just a placeholder
#endif
    handle = LoadLibrary (module);
    if (NULL != handle)
    {
        procedure = GetProcAddress (handle, "init");
        if (NULL != procedure)
            status = rdbi_init (contextp, (initializer*)procedure);
        else
            status = RDBI_GENERIC_ERROR;
    }
    else
        status = RDBI_GENERIC_ERROR;
    
    debug_return(NULL, status);
}


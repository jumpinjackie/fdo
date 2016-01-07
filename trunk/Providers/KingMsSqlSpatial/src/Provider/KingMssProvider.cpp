/*
* Copyright (C) 2007  Haris Kurtagic
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

#include "stdafx.h"

#include "c_KgOtl.h"


#ifdef _WIN32

static wchar_t g_AppFileName[MAX_PATH];
static wchar_t g_HomeDir[MAX_PATH];
wchar_t g_LogFileName[MAX_PATH];


//wchar_t g_WcharBuff1024[1024+1];

BOOL APIENTRY DllMain (HANDLE Module, DWORD Reason, LPVOID lpReserved)
{
    DWORD nchars;
    wchar_t* last;
    BOOL ret;


    // UNCOMMENT THE FOLLOWING CODE TO HELP DISCOVER MEMORY LEAKS OR PREMATURE MEMORY DEALLOCATIONS;
    // WARNING: THIS CAN MAKE THE CODE RUN EXTREMELY SLOWLY IN CERTAIN PLACES!
    //
    //int debugFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    //_CrtSetDbgFlag(debugFlags | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);


  c_KgOtl::Init();

    ret = TRUE;
    if ( Reason == DLL_PROCESS_ATTACH )
    {
        nchars = GetModuleFileNameW ((HINSTANCE)Module, g_AppFileName, MAX_PATH);
        if (0 == nchars)
            ret = FALSE;
        else
        {   
            // scan the string for the last occurrence of a slash
            wcscpy (g_HomeDir, g_AppFileName);
            last = wcsrchr (g_HomeDir, L'\\');
            if (NULL == last)
                ret = FALSE;
            else
            {
                last++; // move past the slash
                *last = L'\0'; // null terminate it there
            }
            
            wcscpy (g_LogFileName, g_HomeDir);
            wcscat (g_LogFileName, L"\\KingMsSqlSpatial.log");
        }
    }

    return (ret);
}
#endif // _WIN32
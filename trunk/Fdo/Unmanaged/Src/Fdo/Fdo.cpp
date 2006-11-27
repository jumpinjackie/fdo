/*
 * 
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
* 
 */

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include <Fdo.h>

wchar_t module[MAX_PATH];
wchar_t home_dir[MAX_PATH];

BOOL APIENTRY DllMain (HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    DWORD nchars;
    wchar_t* last;
    BOOL ret;

    // UNCOMMENT THE FOLLOWING CODE TO HELP DISCOVER MEMORY LEAKS OR PREMATURE MEMORY DEALLOCATIONS;
    // WARNING: THIS CAN MAKE THE CODE RUN EXTREMELY SLOWLY IN CERTAIN PLACES!
    //
    //    int debugFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    //    _CrtSetDbgFlag(debugFlags | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);

    ret = TRUE;
    if (DLL_PROCESS_ATTACH == ul_reason_for_call)
    {
        // hModule - The value is the base address of the DLL.
        // The HINSTANCE of a DLL is the same as the HMODULE of the DLL,
        // so hinstDLL can be used in calls to functions that require a module handle.
        nchars = GetModuleFileNameW ((HINSTANCE)hModule, module, MAX_PATH);
        if (0 == nchars)
            ret = FALSE;
        else
        {   
            // scan the string for the last occurrence of a slash
            wcscpy (home_dir, module);
            last = wcsrchr (home_dir, L'\\');
            if (NULL == last)
                ret = FALSE;
            else
            {
                last++; // move past the slash
                *last = L'\0'; // null terminate it there
            }
        }
    }
    else if (DLL_PROCESS_DETACH == ul_reason_for_call)
    {
        // delete the static FdoFeatureAccessManager members
        FdoFeatureAccessManager::Reset();
    }

    return (ret);
}
#else
void _load();
void _unload();

/*
   This is temporary code to call _load/_unload in FdoGeometryDll.cpp
   when the shared library is loaded/unloaded.
*/
class InitClass
{
public:
    InitClass()
    {
        _load();
    };
    ~InitClass()
    {
        _unload();
    };
};

static InitClass initClass;

#endif // _WIN32



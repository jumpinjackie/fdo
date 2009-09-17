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

//
// FdoGeometryDll.cpp : Defines the entry point for the DLL.
// Implementation code is adapted from FDO Oracle Provider.
//

#include "GeometryThreadData.h"

#ifdef _WIN32

#include <windows.h>

static wchar_t module[MAX_PATH];
static wchar_t home_dir[MAX_PATH];

/// <summary>
/// Main entry point for the dll.
/// </summary>
/// <param name="hModule">Handle to the DLL module.</param> 
/// <param name="ul_reason_for_call">Reason for calling function.</param> 
/// <param name="lpReserved">Reserved.</param> 
/// <returns>TRUE if it succeeds or FALSE if initialization fails.</returns> 
BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    DWORD nchars;
    wchar_t* last;
    BOOL ret;

    ret = TRUE;
    switch (ul_reason_for_call)
    {

        case DLL_PROCESS_ATTACH:
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
            FdoGeometryThreadDataKey tlsIndex = TlsAlloc();
            FdoGeometryThreadData::SetKey(tlsIndex);
            break;
        }


        case DLL_THREAD_ATTACH: 
 
            // Nothing essential here.  We to defer any pre-allocation
            // until someone calls FdoGeometryThreadData::GetValue().
 
            break; 
 
        case DLL_THREAD_DETACH: 

            FdoGeometryThreadData::ReleaseValue();
            break; 
 
        case DLL_PROCESS_DETACH: 
 
            FdoGeometryThreadData::ReleaseValue();
            TlsFree(FdoGeometryThreadData::GetKey());
            break; 

    }

    return (ret);
}

#else   // no _WIN32

// This destructor function is called automatically when the
// threading library destroys a thread key.
void _threadDataDestructor(void * voidData)
{
    FdoGeometryThreadData *data = (FdoGeometryThreadData *) voidData;
    if (NULL != data)
    {
        delete data;
    }
}

void _load()
{
    FdoGeometryThreadDataKey key=0;
    pthread_key_create(&key, _threadDataDestructor);
    FdoGeometryThreadData::SetKey(key);
}


void _unload()
{
    FdoGeometryThreadDataKey key = FdoGeometryThreadData::GetKey();
    FdoGeometryThreadData::ReleaseValue();
    pthread_key_delete(key);
}

#endif


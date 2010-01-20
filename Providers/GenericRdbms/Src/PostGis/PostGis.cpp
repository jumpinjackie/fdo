//
// Copyright (C) 2006 Refractions Research, Inc. 
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
//


//
// PostGis.cpp: Defines the entry point for the DLL application.
//
#include "stdafx.h"
#ifdef _WIN32

#include "FdoRdbmsUtil.h"
#include "Fdo/FdoRdbmsPostGisConnection.h"
#include "Fdo/FdoRdbmsPostGisProviderInfo.h"
#include <Fdo/ClientServices/FeatureAccessManager.h>
#include <Fdo/ClientServices/ProviderRegistry.h>
#include <Inc/Util/thread.h>
#include <Inc/debugext.h>
#include <olectl.h>

static wchar_t module[MAX_PATH];
static wchar_t home_dir[MAX_PATH];
static wchar_t com_dir[MAX_PATH];

// FdoFeatureAccessManager::GetProviderRegistry();
typedef IProviderRegistry* (* GetProviderRegistry) ();

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
                    wcscpy (com_dir, home_dir);
                    wcscat (com_dir, L"com\\");
                }
#ifdef _DEBUG
                // Look for a "debug.images" file in $HOME for debug message control.
				char* cmodule;
				wide_to_multibyte(cmodule, module);
                char * argv[] = { cmodule, 0 };
                int argc = 1;

                debug_init(&argc, argv);
#endif
            }
            tlsIndex = TlsAlloc();
            ut_thread_mutex_init();
            break;
        }


        case DLL_THREAD_ATTACH:

            break;

        case DLL_THREAD_DETACH:

            break;

        case DLL_PROCESS_DETACH:

            TlsFree(tlsIndex);
            ut_thread_mutex_destroy();
            break;


    }

    return (ret);
}

wchar_t* getComDir ()
{
    // if it starts with "\\?\", return the part after that
    return (com_dir);
}

/// <summary>
/// Calls on the FDO Client services to register the FdoRdbms provider.
/// Use dynamic proc address binding because there will not be a path set up yet.
/// </summary>
/// <returns>SELFREG_E_CLASS if there was an error, S_OK otherwise.</returns>
HRESULT APIENTRY DllRegisterServer (void)
{
    HRESULT ret;
    FdoRdbmsUtil  util;
    ret = SELFREG_E_CLASS; // The server was unable to complete the registration of all the object classes.
    IProviderRegistry *registry = FdoFeatureAccessManager::GetProviderRegistry();
    if (registry != NULL)
    {
        registry->RegisterProvider (RDBMS_POSTGIS_PROVIDER_NAME,
                                    NlsMsgGet(FDORDBMS_472, RDBMS_POSTGIS_PROVIDER_DEFAULT_DISPLAY_NAME),
                                    NlsMsgGet(FDORDBMS_473, RDBMS_POSTGIS_PROVIDER_DEFAULT_DESCRIPTION),
                                    RDBMS_POSTGIS_PROVIDER_VERSION,
                                    RDBMS_POSTGIS_FDO_VERSION,
                                    module,
                                    false);
        ret = S_OK;
    }
    return (ret);
}

/// <summary>
/// Calls on the FDO Client services to unregister the FdoRdbms provider.
/// Use dynamic proc address binding because there will not be a path set up yet.
/// </summary>
/// <returns>SELFREG_E_CLASS if there was an error, S_OK otherwise.</returns>
HRESULT APIENTRY DllUnregisterServer (void)
{
    HRESULT ret;

    ret = SELFREG_E_CLASS; // The server was unable to remove the entries of all the object classes.
    IProviderRegistry *registry = FdoFeatureAccessManager::GetProviderRegistry();
    if (registry != NULL)
    {
        registry->UnregisterProvider (RDBMS_POSTGIS_PROVIDER_NAME);
        ret = S_OK;
    }

    return (ret);
}
#else

const char *RDBMS_LIB_NAME = "libFdoPostgreSQL";
#include <link.h>
#include <wchar.h>

#include "FdoRdbmsUtil.h"
#include "FdoCommonStringUtil.h"
#include "Fdo/FdoRdbmsPostGisConnection.h"
#include "Fdo/FdoRdbmsPostGisProviderInfo.h"
#include <Fdo/ClientServices/FeatureAccessManager.h>
#include <Fdo/ClientServices/ProviderRegistry.h>
#include <Inc/Util/thread.h>
#include <Inc/debugext.h>

static wchar_t com_dirW[PATH_MAX];
static char home_dir[PATH_MAX];

wchar_t *getComDir ()
{
	char com_dir[PATH_MAX];
    char* last;
    struct link_map *current = _r_debug.r_map;
    while (current)
    {
        if (strlen(current->l_name) != 0)
        {
            strcpy (home_dir, current->l_name);
            last = strrchr (home_dir, '/');
            last++; // move past the slash
            if (strncmp(last, RDBMS_LIB_NAME, strlen(RDBMS_LIB_NAME)) == 0)
            {
                *last = '\0'; // null terminate it there
                sprintf(com_dir, "%scom/", home_dir);
                break;
            }
        }
        current= current->l_next;
    }
        wchar_t* com_dir_cv;
        multibyte_to_wide(com_dir_cv, com_dir);
        wcscpy(com_dirW, com_dir_cv);
    return com_dirW;
}


__attribute__((constructor)) void _load()
{
    pthread_key_create(&key, NULL);
    ut_thread_mutex_init();
}



__attribute__((destructor)) void _unload()
{
    pthread_key_delete(key);
    ut_thread_mutex_destroy();
}

#endif

extern "C" FDORDBMS_API FdoIConnection* CreateConnection()
{
   return FdoRdbmsPostGisConnection::Create();
}

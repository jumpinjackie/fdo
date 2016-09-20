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

#include "stdafx.h"
#include <ArcSDEProvider.h>

#ifdef _WIN32

// because of define and typedef conflicts we can't: #include <olectl.h>
// so...

typedef long SCODE;

#define MAKE_SCODE(sev,fac,code) \
    ((SCODE) (((unsigned long)(sev)<<31) | ((unsigned long)(fac)<<16) | ((unsigned long)(code))) )

#define SELFREG_E_FIRST    MAKE_SCODE(SEVERITY_ERROR,   FACILITY_ITF, 0x0200)
#define SELFREG_E_LAST     MAKE_SCODE(SEVERITY_ERROR,   FACILITY_ITF, 0x020F)

#define SELFREG_E_CLASS             (SELFREG_E_FIRST+1)
// failed to register/unregister class

// end of olectl.h shenanigans

#include <Fdo/ClientServices/FeatureAccessManager.h>
#include <Fdo/ClientServices/ProviderRegistry.h>

// FdoFeatureAccessManager::GetProviderRegistry();
typedef IProviderRegistry* (* GetProviderRegistry) ();

static wchar_t module[MAX_PATH];
static wchar_t home_dir[MAX_PATH];

/// <summary>
/// Calls on the FDO Client services to register the FdoRdbms provider.
/// Use dynamic proc address binding because there will not be a path set up yet.
/// </summary>
/// <returns>SELFREG_E_CLASS if there was an error, S_OK otherwise.</returns> 
HRESULT APIENTRY DllRegisterServer (void)
{
    wchar_t temp[MAX_PATH];
    HMODULE registry;
    FARPROC get_registry;
    HRESULT ret;

    ret = SELFREG_E_CLASS; // The server was unable to complete the registration of all the object classes. 
    wcscpy (temp, home_dir);
    wcscat (temp, L"FdoClientServices.dll");
    registry = LoadLibraryW (temp);
    if (NULL != registry)
    {
        get_registry = GetProcAddress (
            registry, // handle to DLL module
            "?GetProviderRegistry@FdoFeatureAccessManager@@SAPAVIProviderRegistry@@XZ"); // function name
        if (NULL != get_registry)
        {   // FdoFeatureAccessManager::GetProviderRegistry();
            IProviderRegistry* registry = ((GetProviderRegistry)get_registry) ();
            if (registry != NULL)
            {
                registry->RegisterProvider (
                    ARCSDE_PROVIDER_NAME,
                    NlsMsgGet(ARCSDE_PROVIDER_DISPLAY_NAME, ARCSDE_PROVIDER_DEFAULT_DISPLAY_NAME),
                    ARCSDE_PROVIDER_VERSION,
                    NlsMsgGet(ARCSDE_PROVIDER_DESCRIPTION, ARCSDE_PROVIDER_DEFAULT_DESCRIPTION),
                    ARCSDE_FDO_VERSION,
                    module,
                    false);
                ret = S_OK;
            }
        }
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
    wchar_t temp[MAX_PATH];
    HMODULE registry;
    FARPROC get_registry;
    HRESULT ret;

    ret = SELFREG_E_CLASS; // The server was unable to remove the entries of all the object classes. 
    wcscpy (temp, home_dir);
    wcscat (temp, L"FdoClientServices.dll");
    registry = LoadLibraryW (temp);
    if (NULL != registry)
    {
        get_registry = GetProcAddress (
            registry, // handle to DLL module
            "?GetProviderRegistry@FdoFeatureAccessManager@@SAPAVIProviderRegistry@@XZ"); // function name
        if (NULL != get_registry)
        {   // FdoFeatureAccessManager::GetProviderRegistry();
            IProviderRegistry* registry = ((GetProviderRegistry)get_registry) ();
            if (registry != NULL)
            {
                registry->UnregisterProvider (ARCSDE_PROVIDER_NAME);
                ret = S_OK;
            }
        }
    }

    return (ret);
}

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

    return (ret);
}

#endif // _WIN32



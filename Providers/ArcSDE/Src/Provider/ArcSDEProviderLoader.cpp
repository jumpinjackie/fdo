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

#ifndef _WIN32
#include <dlfcn.h>
#include <unistd.h>
#endif

typedef FdoIConnection* (*CreateConnectionProc)();

#ifdef _WIN32

static wchar_t module[MAX_PATH];
static wchar_t home_dir[MAX_PATH];

#ifdef ARCSDEPROVIDER_EXPORTS
#define FDOSDE_API __declspec(dllexport)
#else
#define FDOSDE_API __declspec(dllimport)
#endif

typedef HRESULT (*DllRegisterServerProc)();
typedef HRESULT (*DllUnregisterServerProc)();

class ArcSDELoaderLibrary
{
private:
    HMODULE m_hArcSDE;

public:
    ArcSDELoaderLibrary()
    {
        m_hArcSDE = NULL;
        try
        {
            HMODULE hmod = ::LoadLibraryW(L"sde.dll");
            if (hmod != NULL)
            {
                ::FreeLibrary(hmod);
                m_hArcSDE = ::LoadLibraryW(L"ArcSDEProvider92.dll");
                // in case load ArcSDEProvider92.dll fails try to load 91
                if (m_hArcSDE == NULL)
                    m_hArcSDE = ::LoadLibraryW(L"ArcSDEProvider91.dll");
            }
            else
                m_hArcSDE = ::LoadLibraryW(L"ArcSDEProvider91.dll");
        }
        catch(...){}
    }
    HMODULE GetLibraryHandle()
    {
        return m_hArcSDE;
    }
    ~ArcSDELoaderLibrary()
    {
        try
        {
            if (m_hArcSDE != NULL)
                ::FreeLibrary(m_hArcSDE);
        }
        catch(...){}
    }
};

static ArcSDELoaderLibrary arcSDEloader;

HRESULT APIENTRY DllRegisterServer (void)
{
    DllRegisterServerProc procReg;
    HMODULE hArc = arcSDEloader.GetLibraryHandle();

    if (hArc != NULL)
    {
        procReg = (DllRegisterServerProc)::GetProcAddress (hArc, "DllRegisterServer");
        if (procReg != NULL)
            return procReg();
    }
    return 0;
}

HRESULT APIENTRY DllUnregisterServer (void)
{
    DllUnregisterServerProc procUnreg;
    HMODULE hArc = arcSDEloader.GetLibraryHandle();

    if (hArc != NULL)
    {
        procUnreg = (DllUnregisterServerProc)::GetProcAddress (hArc, "DllUnregisterServer");
        if (procUnreg != NULL)
            return procUnreg();
    }
    return 0;
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

// external access to connection for client services
extern "C" FDOSDE_API FdoIConnection* CreateConnection ()
{
    CreateConnectionProc procCreateConn;
    HMODULE hArc = arcSDEloader.GetLibraryHandle();

    if (hArc != NULL)
    {
        procCreateConn = (CreateConnectionProc)::GetProcAddress (hArc, "CreateConnection");
        if (procCreateConn != NULL)
            return procCreateConn();
    }
    return NULL;
}
#else

#define FDOSDE_API

class ArcSDELoaderLibrary
{
private:
    void* m_hArcSDE;

public:
    ArcSDELoaderLibrary()
    {
        m_hArcSDE = NULL;
        try
        {
            void* hmod = dlopen("libsde.so", RTLD_NOW);
            if (hmod != NULL)
            {
                dlclose(hmod);
                m_hArcSDE = dlopen("libArcSDEProvider92.so", RTLD_NOW);
                // in case load libArcSDEProvider92.so fails try to load 91
                if (m_hArcSDE == NULL)
                    m_hArcSDE = dlopen("libArcSDEProvider91.so", RTLD_NOW);
            }
            else
                m_hArcSDE = dlopen("libArcSDEProvider91.so", RTLD_NOW);
        }
        catch(...){}
    }
    void* GetLibraryHandle()
    {
        return m_hArcSDE;
    }
    ~ArcSDELoaderLibrary()
    {
        try
        {
            if (m_hArcSDE != NULL)
                dlclose(m_hArcSDE);
        }
        catch(...){}
    }
};

static ArcSDELoaderLibrary arcSDEloader;

// external access to connection for client services
extern "C" FDOSDE_API FdoIConnection* CreateConnection ()
{
    CreateConnectionProc procCreateConn;
    void* hArc = arcSDEloader.GetLibraryHandle();

    if (hArc != NULL)
    {
        procCreateConn = (CreateConnectionProc)dlsym (hArc, "CreateConnection");
        if (procCreateConn != NULL)
            return procCreateConn();
    }
    return NULL;
}

#endif // _WIN32

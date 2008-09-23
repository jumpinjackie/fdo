/***************************************************************************
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
 ***************************************************************************/
#ifndef _WIN32
#include <dlfcn.h>
#include <unistd.h>
#else
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include "ProviderDef.h"
#include "RegistryUtility.h"
#include <Fdo/ClientServices/ConnectionManager.h>
#include <Fdo/ClientServices/ClientServiceException.h>

extern wchar_t module[];

#ifdef _WIN32

#include <malloc.h>
#define ENV_VAR L"PATH"

// macro to convert a multibyte string into a wide character string, allocating space on the stack
#define multibyte_to_wide(w,mb)\
{\
    const char* p = (mb);\
    size_t i = strlen (p);\
    i++;\
    w = (wchar_t*)alloca (i * sizeof (wchar_t));\
    i = MultiByteToWideChar (\
        CP_THREAD_ACP,\
        0,\
        p,\
        (int)i,\
        w,\
        (int)i);\
    if (0 == i)\
        w = NULL;\
    if (NULL==w) throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));\
}

// macro to convert a wide character string into a multibyte string, allocating space on the stack
#define wide_to_multibyte(mb,w)\
{\
    const wchar_t* p = (w);\
    size_t i = wcslen (p);\
    i++;\
    mb = (char*)alloca (i * 6);\
    i = WideCharToMultiByte (\
        CP_THREAD_ACP,\
        0,\
        p,\
        (int)i,\
        mb,\
        (int)i * 6,\
        NULL,\
        NULL);\
    if (0 == i)\
        mb = NULL;\
    if (NULL==mb) throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));\
}
#else
// macro to convert a multibyte string into a wide character string, allocating space on the stack
#define multibyte_to_wide(w,mb)\
{\
    const char* p = (mb);\
    size_t i = strlen (p);\
    i++;\
    w = (wchar_t*)alloca (i * sizeof (wchar_t));\
    i = mbstowcs (w, p, i);\
    if (0 > i)\
        w = NULL;\
    if (NULL==w) throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));\
}

// macro to convert a wide character string into a multibyte string, allocating space on the stack
#define wide_to_multibyte(mb,w)\
{\
    const wchar_t* p = (w);\
    size_t i = wcslen (p);\
    i++;\
    mb = (char*)alloca (i * 6);\
    i = wcstombs (mb, p, i);\
    if (0 > i)\
        mb = NULL;\
    if (NULL==mb) throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));\
}
#endif

// typedefs
typedef FdoIConnection* (*DLLCREATEOBJECT)();
typedef FdoModuleMap::iterator FdoModuleMapIterator;
#ifdef _WIN32
typedef std::pair <const std::wstring, HMODULE> FdoNamedModulePair;
#else
typedef std::pair <const std::wstring, void *> FdoNamedModulePair;
#endif

// Constructs a default instance of a FdoProviderRegistry object.
FdoConnectionManager::FdoConnectionManager()
{
}

// Default destructor for FdoProviderRegistry object.
FdoConnectionManager::~FdoConnectionManager()
{
    FdoModuleMapIterator moduleIterator = m_moduleMap.begin();
    while (moduleIterator != m_moduleMap.end()) {
#ifdef _WIN32
        HMODULE library = moduleIterator->second;
        if (library != NULL) {
            ::FreeLibrary(moduleIterator->second);
#else
        void * library = moduleIterator->second;
        if (library != NULL) {
			dlclose(moduleIterator->second);	
#endif
        moduleIterator++;
        }
    }

}

void FdoConnectionManager::Dispose()
{
    delete this;
}

// Add the path to the given executable to the environment PATH.
static int addPath (std::wstring exe)
{
    int ret;

#ifdef _WIN32
	//An environment variable has a maximum size limit of 32,767 characters, including the null-terminating character
    wchar_t env[0x7FFF];
    const wchar_t* DIR_SEP_CHAR = L"\\";
    const wchar_t* PATH_SEP_CHAR = L";";
    std::wstring::size_type pos;
    std::wstring path;
    std::wstring new_path;
	ret = 0;

	pos = exe.find_last_of (DIR_SEP_CHAR);
    if (std::wstring::npos != pos)
    {
        pos++; // move past the slash
        exe = exe.substr (0, pos);
        if (GetEnvironmentVariableW(ENV_VAR, env, 0x7FFF) != 0)
        {
            path = env;
			std::wstring::size_type last = 0;
            while (!ret && std::wstring::npos != (pos = path.find (exe, last)))
            {   // check it's just the directory and nothing else
                last = pos + exe.length ();
                if ((path.length () <= last) || (path.at (last) == PATH_SEP_CHAR[0]))
                    ret = 1;
            }
			if (!ret)
			{   // need to add the path
				path = exe + PATH_SEP_CHAR + path;
				ret = (SetEnvironmentVariableW(ENV_VAR, path.c_str()) != 0) ? 1 : 0;
			}
        }
    }
#else
    ret = 1;
#endif

    return (ret);
}

// Creates an unitialized connection object given the provider name
FdoIConnection* FdoConnectionManager::CreateConnection(const wchar_t* providerName)
{
#ifdef _WIN32
    FdoIConnection* connection = NULL;
    HMODULE providerLibrary = NULL;
    std::wstring libraryLocation;
    FdoModuleMapIterator moduleIterator;
    DLLCREATEOBJECT dco = NULL;

    if (providerName == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_1_NULLINPUTPOINTER)));
    }

    if (_wcsicmp(providerName, L"") == 0) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_2_EMPTYINPUTPARAMETER)));
    }

    moduleIterator = m_moduleMap.find(providerName);
    if (moduleIterator == m_moduleMap.end()) {
        FdoRegistryUtility::GetLibraryLocation(providerName, libraryLocation);

        // SPR 664919 Failure to load FDO.dll because of dependency with FDOSpatial
        // so we add ourselves first
        addPath (module);

        addPath (libraryLocation);

        providerLibrary = LoadLibraryW(libraryLocation.c_str());
        if (providerLibrary == NULL) {
			LPVOID lpMsgBuf;
			wchar_t szBuf[256];
			DWORD dw = GetLastError();
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &lpMsgBuf, 0, NULL);
            swprintf(szBuf, sizeof(szBuf), L"%hs", lpMsgBuf);
			FdoClientServiceException *ex = FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_8_UNABLE_TO_LOAD_LIBRARY), szBuf));
			LocalFree(lpMsgBuf);
			throw ex;
        }

        m_moduleMap.insert(FdoNamedModulePair(providerName, providerLibrary));
    }
    else {
        providerLibrary = moduleIterator->second;
        if (providerLibrary == NULL) {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_3_NULLPOINTER)));
        }
    }

    dco = (DLLCREATEOBJECT) GetProcAddress(providerLibrary, c_providerEntryPoint.c_str());
    if (dco == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_9_INVALID_PROVIDER_ENTRY_POINT)));
    }

    connection = dco();
    if (connection == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_10_NULL_CONNECTION_FROM_PROVIDER)));
    }

    return connection;
#else
    FdoIConnection* connection = NULL;
    void * providerLibrary = NULL;
    std::wstring libraryLocation;
    FdoModuleMapIterator moduleIterator;
    DLLCREATEOBJECT dco = NULL;

    if (providerName == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_1_NULLINPUTPOINTER)));
    }

    if (wcscasecmp(providerName, L"") == 0) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_2_EMPTYINPUTPARAMETER)));
    }

    moduleIterator = m_moduleMap.find(providerName);
    if (moduleIterator == m_moduleMap.end()) {
        FdoRegistryUtility::GetLibraryLocation(providerName, libraryLocation);

        addPath (libraryLocation);

        FdoStringP temp = libraryLocation.c_str();
        
        providerLibrary = dlopen(temp, RTLD_NOW);
        if (providerLibrary == NULL) {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_8_UNABLE_TO_LOAD_LIBRARY), dlerror()));
        }

        m_moduleMap.insert(FdoNamedModulePair(providerName, providerLibrary));
    }
    else {
        providerLibrary = moduleIterator->second;
        if (providerLibrary == NULL) {
            throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_3_NULLPOINTER)));
        }
    }

    dco = (DLLCREATEOBJECT) dlsym(providerLibrary, c_providerEntryPoint.c_str());
    if (dco == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_9_INVALID_PROVIDER_ENTRY_POINT)));
    }

    connection = dco();
    if (connection == NULL) {
        throw FdoClientServiceException::Create(FdoClientServiceException::NLSGetMessage(FDO_NLSID(CLNT_10_NULL_CONNECTION_FROM_PROVIDER)));
    }

    return connection;
#endif
}

// Frees a connection library reference given the provider name
void FdoConnectionManager::FreeLibrary(const wchar_t* providerName)
{
    FdoModuleMapIterator moduleIterator = m_moduleMap.find(providerName);

    if (moduleIterator != m_moduleMap.end()) {
#ifdef _WIN32
        HMODULE library = moduleIterator->second;
        if (library != NULL) {
            ::FreeLibrary(moduleIterator->second);
        }
#else
        void *library = moduleIterator->second;
        if (library != NULL) {
            dlclose(moduleIterator->second);
        }
		
#endif
        m_moduleMap.erase(moduleIterator);
    }

}




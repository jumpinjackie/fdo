/*
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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FDORFP.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
// common C++ headers:
#include <malloc.h>
#include "FdoRfpGlobals.h"

#ifdef _WIN32
// macro to convert a multibyte string into a wide character string, allocating space on the stack
#define multibyte_to_wide(w,mb)\
{\
    const char* p = mb;\
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
    if (NULL==w) throw FdoException::Create(NlsMsgGet(GRFP_7_MEMORY_FAILURE, "Failed to allocate memory."));\
}

// macro to convert a wide character string into a multibyte string, allocating space on the stack
#define wide_to_multibyte(mb,w)\
{\
    const wchar_t* p = w;\
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
    if (NULL==mb) throw FdoException::Create(NlsMsgGet(GRFP_7_MEMORY_FAILURE, "Failed to allocate memory."));\
}
#else
// macro to convert a multibyte string into a wide character string, allocating space on the stack
#define multibyte_to_wide(w,mb)\
{\
    const char* p = mb;\
    size_t i = strlen (p);\
    i++;\
    w = (wchar_t*)alloca (i * sizeof (wchar_t));\
    i = mbstowcs (w, p, i);\
    if (0 > i)\
        w = NULL;\
    if (NULL==w) throw FdoException::Create(NlsMsgGet(GRFP_7_MEMORY_FAILURE, "Failed to allocate memory."));\
}

// macro to convert a wide character string into a multibyte string, allocating space on the stack
#define wide_to_multibyte(mb,w)\
{\
    const wchar_t* p = w;\
    size_t i = wcslen (p);\
    i++;\
    mb = (char*)alloca (i * 6);\
    i = wcstombs (mb, p, i);\
    if (0 > i)\
        mb = NULL;\
    if (NULL==mb) throw FdoException::Create(NlsMsgGet(GRFP_7_MEMORY_FAILURE, "Failed to allocate memory."));\
}
#endif


typedef long SCODE;

#define MAKE_SCODE(sev,fac,code) \
    ((SCODE) (((unsigned long)(sev)<<31) | ((unsigned long)(fac)<<16) | ((unsigned long)(code))) )

#define SELFREG_E_FIRST    MAKE_SCODE(SEVERITY_ERROR,   FACILITY_ITF, 0x0200)
#define SELFREG_E_LAST     MAKE_SCODE(SEVERITY_ERROR,   FACILITY_ITF, 0x020F)
#define SELFREG_E_CLASS    (SELFREG_E_FIRST+1)

static char module[MAX_PATH];
static char home_dir[MAX_PATH];

/// <summary>
/// Calls on the FDO Client services to register the RFP provider.
/// Use dynamic proc address binding because there will not be a path set up yet.
/// </summary>
/// <returns>SELFREG_E_CLASS if there was an error, S_OK otherwise.</returns> 
FDOGRFP_API HRESULT APIENTRY DllRegisterServer (void)
{
    wchar_t* wmodule;
    HRESULT ret;

    ret = SELFREG_E_CLASS; // The server was unable to complete the registration of all the object classes. 
    multibyte_to_wide (wmodule, module);
    // Find a matching provider in the Provider Registry.
    IProviderRegistry* registry = FdoFeatureAccessManager::GetProviderRegistry();

    registry->RegisterProvider (
        FdoGrfpGlobals::GRFPProviderName,
        FdoGrfpGlobals::GRFPProviderDisplayName,
        FdoGrfpGlobals::GRFPProviderVersion,
        FdoGrfpGlobals::GRFPProviderDescription,
        FdoGrfpGlobals::GRFPFeatureDataObjectsVersion,
        wmodule,
        false);
    ret = S_OK;

    return (ret);
}

/// <summary>
/// Calls on the FDO Client services to unregister the RFP provider.
/// Use dynamic proc address binding because there will not be a path set up yet.
/// </summary>
/// <returns>SELFREG_E_CLASS if there was an error, S_OK otherwise.</returns> 
FDOGRFP_API HRESULT APIENTRY DllUnregisterServer (void)
{
    // Find a matching provider in the Provider Registry.
    IProviderRegistry* registry = FdoFeatureAccessManager::GetProviderRegistry();
	registry->UnregisterProvider (FdoGrfpGlobals::GRFPProviderName);
	return S_OK;
}


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
      DWORD nchars;
      char* last;
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
          nchars = GetModuleFileName ((HINSTANCE)hModule, module, MAX_PATH);
          if (0 == nchars)
              ret = FALSE;
          else
          {   
              // scan the string for the last occurrence of a slash
              strcpy (home_dir, module);
              last = strrchr (home_dir, '\\');
              if (NULL == last)
                  ret = FALSE;
              else
              {
                  last++; // move past the slash
                  *last = '\0'; // null terminate it there
              }
          }
      }

	  return ret;
}

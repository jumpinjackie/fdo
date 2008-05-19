// RFPOverrides.cpp : Defines the entry point for the DLL application.
//

#include <Overrides.h>
#include <stdafx.h>

// the default message catalog filename
#ifndef _WIN32
char *fdorfp_cat = "GRFPMessage.cat";
#else
char *fdorfp_cat = "GRFPMessage.dll";
#endif

#ifdef _WIN32
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
#endif // _WIN32

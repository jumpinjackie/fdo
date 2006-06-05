#ifndef ___ODBCDRIVER_H
#define ___ODBCDRIVER_H

#ifdef _WIN32
#pragma once
#endif

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ODBCDRIVER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ODBCDRIVER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef _WIN32
    #ifdef ODBCDRIVER_EXPORTS
        #define ODBCDRIVER_API __declspec(dllexport)
        #else
        #define ODBCDRIVER_API __declspec(dllimport)
    #endif
#else 
#define ODBCDRIVER_API
#endif

// Global Functions...
VOID ThrowDriverError();
BOOL UninstallDriver();
BOOL InstallDriver(CHAR* lpszDriverPath);

// Temprary Testing Functions...
ODBCDRIVER_API inline void CallTest() {return;};


#endif // ___ODBCDRIVER_H
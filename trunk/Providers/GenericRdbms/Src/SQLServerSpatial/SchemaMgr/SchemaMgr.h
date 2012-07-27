// SchemaMgr.h : main header file for the SchemaMgr DLL
//

#ifdef _WIN32
#pragma once
#endif

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CSchemaMgrApp
// See SchemaMgr.cpp for the implementation of this class
//

class CSchemaMgrApp : public CWinApp
{
public:
    CSchemaMgrApp();

// Overrides
public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};

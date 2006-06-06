// ODBCSetup.h : main header file for the ODBCSetup DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CODBCSetupApp
// See ODBCSetup.cpp for the implementation of this class
//

class CODBCSetupApp : public CWinApp
{
public:
	CODBCSetupApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

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

// SchemaMgr.h : main header file for the SchemaMgr DLL
//

#ifdef _WIN32
#pragma once

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif
#endif

#include "resource.h"       // main symbols


/// \brief
/// DLL hacks required for linking with MFC library.
/// See SchemaMgr.cpp for the implementation of this class
///
class CSchemaMgrApp : public CWinApp
{
public:

    /// Default constructor
    CSchemaMgrApp();

    /// Overriden CWinApp interface.
    /// Initializes each new instance of application.
    /// @return Nonzero if initialization is successful; otherwise 0.
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};


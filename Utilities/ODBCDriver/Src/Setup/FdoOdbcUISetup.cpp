// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

// FdoOdbcUISetup.cpp : implementation file
//

#include "stdafx.h"
#include "ODBCSetup.h"
#include "FdoOdbcUISetup.h"


// FdoOdbcUISetup dialog

IMPLEMENT_DYNAMIC(FdoOdbcUISetup, CDialog)

FdoOdbcUISetup::FdoOdbcUISetup(CWnd* pParent /*=NULL*/)
	: CDialog(FdoOdbcUISetup::IDD, pParent)
{

}

FdoOdbcUISetup::~FdoOdbcUISetup()
{
}

void FdoOdbcUISetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FdoOdbcUISetup, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, &FdoOdbcUISetup::OnBnClickedButton1)
END_MESSAGE_MAP()


// FdoOdbcUISetup message handlers

void FdoOdbcUISetup::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here 
}

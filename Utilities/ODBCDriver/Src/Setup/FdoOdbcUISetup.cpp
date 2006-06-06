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

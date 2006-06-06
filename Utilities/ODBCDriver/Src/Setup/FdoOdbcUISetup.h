#pragma once


// FdoOdbcUISetup dialog

class FdoOdbcUISetup : public CDialog
{
	DECLARE_DYNAMIC(FdoOdbcUISetup)

public:
	FdoOdbcUISetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~FdoOdbcUISetup();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
};

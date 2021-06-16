#pragma once


// CSetupDlg dialog

class CSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupDlg)

public:
	CSetupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	BOOL IcqAutoCon;
public:
	BOOL InternetAutoCon;
public:
	CString InternetCon;
public:
	CString ManagerPath;
public:
	afx_msg void OnBnClickedBrowseb();
public:
	BOOL ManagerAutoStart;
public:
	CString mPort;
public:
	BOOL SendModemSignal;
};

// SetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MP707.h"
#include "SetupDlg.h"


// CSetupDlg dialog

IMPLEMENT_DYNAMIC(CSetupDlg, CDialog)

CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
	, IcqAutoCon(FALSE)
	, InternetAutoCon(FALSE)
	, InternetCon(_T(""))
	, ManagerPath(_T(""))
	, ManagerAutoStart(FALSE)
	, mPort(_T(""))
	, SendModemSignal(FALSE)
{

}

CSetupDlg::~CSetupDlg()
{
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, IcqAutoCon);
	DDX_Check(pDX, IDC_CHECK2, InternetAutoCon);
	DDX_Text(pDX, IDC_EDIT1, InternetCon);
	DDX_Text(pDX, IDC_EDIT3, ManagerPath);
	DDX_Check(pDX, IDC_CHECK3, ManagerAutoStart);
	DDX_CBString(pDX, IDC_COMBO1, mPort);
	DDX_Check(pDX, IDC_CHECK4, SendModemSignal);
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetupDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDBrowseB, &CSetupDlg::OnBnClickedBrowseb)
END_MESSAGE_MAP()


BOOL CSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	if (GetPrivateProfileInt("Setup","ICQAutoConnect",0,"DWControl.ini")==1) IcqAutoCon=true;
	else IcqAutoCon=false;
	if (GetPrivateProfileInt("Setup","InternetAutoConnect",0,"DWControl.ini")==1) InternetAutoCon=true;
	else InternetAutoCon=false;
	if (GetPrivateProfileInt("Setup","ManagerAutoStart",0,"DWControl.ini")==1) ManagerAutoStart=true;
	else ManagerAutoStart=false;
	if (GetPrivateProfileInt("Setup","SendModemATZOPRT",0,"DWControl.ini")==1) SendModemSignal=true;
	else SendModemSignal=false;
	//Internet Connection Name
	char cn[50];
	char cp[10];
	char mgr[255];
	GetPrivateProfileString("Setup","InternetConnection","",cn,49,"DWControl.ini");
	GetPrivateProfileString("Setup","ConnectionManager","",mgr,254,"DWControl.ini");
	GetPrivateProfileString("Setup","ModemComPort","",cp,10,"DWControl.ini");
	InternetCon=cn;
	ManagerPath=mgr;
	mPort=cp;
	
	UpdateData(FALSE);


	return true;

}
// CSetupDlg message handlers

void CSetupDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	OnOK();
}

void CSetupDlg::OnBnClickedBrowseb()
{
	CFileDialog fileDialog(TRUE,NULL,"",NULL,"Приложения (EXE)|*.exe|Все файлы (*.*)|*.*|");
	if (fileDialog.DoModal()==IDOK)
	{
		ManagerPath=fileDialog.GetPathName();
		UpdateData(FALSE);
	}
}

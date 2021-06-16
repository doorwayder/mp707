// MP707Dlg.cpp : implementation file   2009
//

#include "stdafx.h"
#include "MP707.h"
#include "MP707Dlg.h"
#include "SetupDlg.h"
#include "icq.h"
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern "C" {
#include "hidsdi.h"	
#include "hidpi.h"
#include "setupapi.h"
} 

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMP707Dlg dialog


CMP707Dlg::CMP707Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMP707Dlg::IDD, pParent)
	, m_txt(_T(""))
	, tempr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMP707Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_txt);
	DDV_MaxChars(pDX, m_txt, 250);
	DDX_Control(pDX, IDC_LIST2, indR);
	DDX_Control(pDX, IDC_LIST3, indS1);
	DDX_Control(pDX, IDC_LIST4, indS2);
	DDX_Text(pDX, IDC_EDIT2, tempr);
	DDX_Control(pDX, IDC_LIST1, mLog);
}

BEGIN_MESSAGE_MAP(CMP707Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CMP707Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMP707Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CMP707Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMP707Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMP707Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMP707Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMP707Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMP707Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMP707Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CMP707Dlg::OnBnClickedButton8)
	ON_MESSAGE(WM_SICQ_EVENTWND_RECVDATA,RcvData)
	ON_MESSAGE(WM_SICQ_EVENTWND_SENDMESSAGE,SndData)
	ON_WM_DESTROY()
	ON_COMMAND(ID_ICQ_ON, &CMP707Dlg::OnIcqConnectM)
	ON_BN_CLICKED(IDOK2, &CMP707Dlg::OnBnClickedOk2)
	ON_COMMAND(ID_ICQ_SETUP, &CMP707Dlg::OnIcqDiscon)
	ON_WM_TIMER()
	ON_COMMAND(ID_EXIT, &CMP707Dlg::OnExit)
	ON_BN_CLICKED(IDC_BUTTON9, &CMP707Dlg::OnBnClickedButton9)
	ON_COMMAND(ID_INI_SETUP, &CMP707Dlg::OnSetupDlg)
	ON_COMMAND(ID_SETUP_TYPE1, &CMP707Dlg::OnDeviceRecon)
	ON_COMMAND(ID_INTERNET_CONNECT, &CMP707Dlg::OnInternetConnect)
	ON_COMMAND(ID_INTERNET_DISCONNECT, &CMP707Dlg::OnInternetDisconnect)
	ON_COMMAND(ID_INTERNET_RUNCMANAGER, &CMP707Dlg::OnInternetRunManager)
END_MESSAGE_MAP()


// CMP707Dlg message handlers

BOOL CMP707Dlg::OnInitDialog()
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	////Get Window Handler
	hEventWnd = this->GetSafeHwnd();
	//Load Winsock
	_LoadWS();
	// INIT HID Device
	HIDD_ATTRIBUTES deviceAttributes;
	DWORD ven = 5824;
	DWORD pro = 1503;
	DWORD ver = 256;
	tempr="--";
	ONEWIRE_COUNT=0;
	stateR=false;
	stateS1=false;
	stateS2=false;

	bool con = connectToUSBHIDDevice(&ven,&pro,&ver);

	CString info;
	if (devCnt==0) info = "No One Device Connected!";
	else info.Format("%d devices connected...",devCnt);
	m_txt = info;
	
	UpdateData(FALSE);

	if (GetPrivateProfileInt("Setup","ICQAutoConnect",0,"DWControl.ini")==1) 
					OnIcqConnectM();
	if (GetPrivateProfileInt("Setup","InternetAutoConnect",0,"DWControl.ini")==1) 
					OnInternetConnect();
	if (GetPrivateProfileInt("Setup","ManagerAutoStart",0,"DWControl.ini")==1) 
					OnInternetRunManager();
	if (GetPrivateProfileInt("Setup","SendModemATZOPRT",0,"DWControl.ini")==1) 
					SetTimer(4,120000,NULL);
	
	
	usb_get_portR();
	usb_get_portS();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMP707Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMP707Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMP707Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HANDLE CMP707Dlg::connectToIthUSBHIDDevice(DWORD deviceIndex)
{
	GUID hidGUID;
    HDEVINFO hardwareDeviceInfoSet;
    SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
    PSP_INTERFACE_DEVICE_DETAIL_DATA deviceDetail;
    ULONG requiredSize;
    HANDLE deviceHandle = INVALID_HANDLE_VALUE;
    DWORD result;

    //Get the HID GUID value - used as mask to get list of devices
    HidD_GetHidGuid (&hidGUID);

    //Get a list of devices matching the criteria (hid interface, present)
    hardwareDeviceInfoSet = SetupDiGetClassDevs (&hidGUID,
                                                 NULL, // Define no enumerator (global)
                                                 NULL, // Define no
                                                 (DIGCF_PRESENT | // Only Devices present
                                                 DIGCF_DEVICEINTERFACE)); // Function class devices.

    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    //Go through the list and get the interface data
    result = SetupDiEnumDeviceInterfaces (hardwareDeviceInfoSet,
                                          NULL, //infoData,
                                          &hidGUID, //interfaceClassGuid,
                                          deviceIndex, 
                                          &deviceInterfaceData);

    /* Failed to get a device - possibly the index is larger than the number of devices */
    if (result == FALSE)
    {
        SetupDiDestroyDeviceInfoList (hardwareDeviceInfoSet);
        return INVALID_HANDLE_VALUE;
    }

    //Get the details with null values to get the required size of the buffer
    SetupDiGetDeviceInterfaceDetail (hardwareDeviceInfoSet,
                                     &deviceInterfaceData,
                                     NULL, //interfaceDetail,
                                     0, //interfaceDetailSize,
                                     &requiredSize,
                                     0); //infoData))

    //Allocate the buffer
    deviceDetail = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(requiredSize);
    deviceDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

    //Fill the buffer with the device details
    if (!SetupDiGetDeviceInterfaceDetail (hardwareDeviceInfoSet,
                                          &deviceInterfaceData,
                                          deviceDetail,
                                          requiredSize,
                                          &requiredSize,
                                          NULL)) 
    {
        SetupDiDestroyDeviceInfoList (hardwareDeviceInfoSet);
        free (deviceDetail);
        return INVALID_HANDLE_VALUE;
    }

    //Open file on the device
    deviceHandle = CreateFile (deviceDetail->DevicePath,
                               GENERIC_READ,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL,        // no SECURITY_ATTRIBUTES structure
                               OPEN_EXISTING, // No special create flags
                               0, 
                               NULL);       // No template file

    SetupDiDestroyDeviceInfoList (hardwareDeviceInfoSet);
    free (deviceDetail);
    return deviceHandle;
}

bool CMP707Dlg::connectToUSBHIDDevice(DWORD* vendorID, DWORD* productID, DWORD* versionNumber)
{
	hdR = INVALID_HANDLE_VALUE;
	hdS = INVALID_HANDLE_VALUE;
	HANDLE deviceHandle = INVALID_HANDLE_VALUE;
    DWORD index = 0;
    HIDD_ATTRIBUTES deviceAttributes;
    BOOL matched = FALSE;
	devCnt=0;

    while (!matched)
    {
		if (index>30) return false;
		deviceHandle = connectToIthUSBHIDDevice (index);
		if (deviceHandle == INVALID_HANDLE_VALUE) {index++;continue;}
		if (!HidD_GetAttributes (deviceHandle, &deviceAttributes)) {index++;continue;}
           
        if ((vendorID == 0 || deviceAttributes.VendorID == *vendorID) &&
            (productID == 0 || deviceAttributes.ProductID == *productID) &&
            (versionNumber == 0 || deviceAttributes.VersionNumber == *versionNumber) &&
			(GetHIDId(deviceHandle)==479))
		{hdS = deviceHandle; devCnt++; index++; mLog.InsertString(0,"MP707 Connected");continue;}/* matched */

		if ((vendorID == 0 || deviceAttributes.VendorID == *vendorID) &&
            (productID == 0 || deviceAttributes.ProductID == *productID) &&
            (versionNumber == 0 || deviceAttributes.VersionNumber == *versionNumber) &&
			(GetHIDId(deviceHandle)==128))
		{hdR = deviceHandle; devCnt++; index++; mLog.InsertString(0,"MP709 Connected");continue;}/* matched */
        
        CloseHandle (deviceHandle); /* not a match - close and try again */

        index++;
    }

    if (devCnt>0) return true; 
	else return false;
}

int CMP707Dlg::GetHIDId(HANDLE hnd)
{
	buf_clear();
	int ID;
	buf0[1]=0x1d;
    HidD_SetFeature(hnd,buf0,9);
	HidD_GetFeature(hnd,buf1,9);
	ID=(buf1[5]<<24)+(buf1[6]<<16)+(buf1[7]<<8)+buf1[8];
	return ID;
}

void CMP707Dlg::SendComPortAT(char *port)
{
	DWORD bytes;
	char* pOut = new char[255];
	char* cСh="AT+ZOPRT=5";
	int wwWritten=0,wwRead=0;
	//\\\\.\\Com11

	HANDLE hCom = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , 0);

	DCB dcb;
	COMMTIMEOUTS rCommTimeouts;

      dcb.DCBlength = sizeof (dcb);

     // Omit the call to SetupComm to use the default queue sizes.
     // Get the current configuration.
      if (! GetCommState (hCom, &dcb))
        GetLastError ();
      else {
         // Fill in the DCB: baud=9600, 8 data bits, no parity, 1 stop bit.
          dcb.BaudRate = 9600;
          dcb.ByteSize = 8;
          dcb.Parity = NOPARITY;
          dcb.StopBits = ONESTOPBIT;
     
          if (! SetCommState (hCom, &dcb)) {
            GetLastError ();
		  }
	  }
      
             // Устанавливаем тайм-аут на ожидание чтения
             // очередного символа из COM-порта
              if (! GetCommTimeouts (hCom,    // handle to comm device
				  &rCommTimeouts))   // pointer to comm time-outs structure);
                GetLastError ();
              else {
                // Устанавливаемый тайм-аут - 4 сек. (с потолка)
                  rCommTimeouts.ReadTotalTimeoutConstant = 4000;
                  rCommTimeouts.ReadTotalTimeoutMultiplier = 0;
                  if (! SetCommTimeouts (hCom,   // handle to comm device
                           &rCommTimeouts)) { // pointer to comm time-outs structure);
                    GetLastError ();
				  }
			  }
 
			  if (hCom==INVALID_HANDLE_VALUE) {int e=GetLastError();CString er;er.Format("Error:%d",e);}//AfxMessageBox(er);}
	WriteFile(hCom, "AT+ZOPRT=5\r\n", 13, &bytes, 0);
	ReadFile(hCom, pOut, 255, &bytes, 0);
	CloseHandle(hCom);
}

void CMP707Dlg::OnBnClickedOk()
{
}

void CMP707Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

bool CMP707Dlg::usb_set_portS(unsigned char PS)
{
	buf_clear();
	buf0[1]=0xE7;
    buf0[2]=PS;
	return HidD_SetFeature(hdS,buf0,9);
}

bool CMP707Dlg::usb_set_portR(unsigned char PS)
{
	if (hdR!=INVALID_HANDLE_VALUE){
	buf_clear();
	buf0[1]=0xE7;
    buf0[2]=PS;
	return HidD_SetFeature(hdR,buf0,9);
	}
	return false;
}

bool CMP707Dlg::usb_get_portS()
{
	if (hdS!=INVALID_HANDLE_VALUE){
	buf_clear();
	buf0[1]=0x7e;
	if (!HidD_SetFeature(hdS,buf0,9)) {indS1.SetBkColor(RGB(255,0,0));stateS1=false;indS2.SetBkColor(RGB(255,0,0));stateS2=false;return false;}
	HidD_GetFeature(hdS,buf1,9);

	if (buf1[2]==0x00) {indS1.SetBkColor(RGB(255,0,0));indS2.SetBkColor(RGB(255,0,0));stateS1=false;stateS2=false;}
	if (buf1[2]==0x08) {indS1.SetBkColor(RGB(0,255,0));indS2.SetBkColor(RGB(255,0,0));stateS1=true;stateS2=false;}
	if (buf1[2]==0x10) {indS1.SetBkColor(RGB(255,0,0));indS2.SetBkColor(RGB(0,255,0));stateS1=false;stateS2=true;}
	if (buf1[2]==0x18) {indS1.SetBkColor(RGB(0,255,0));indS2.SetBkColor(RGB(0,255,0));stateS1=true;stateS2=true;}
	indS1.RedrawWindow();
	indS2.RedrawWindow();
	return true;
	}else {indS1.SetBkColor(RGB(255,0,0));indS2.SetBkColor(RGB(255,0,0));indS1.RedrawWindow();indS2.RedrawWindow();stateS1=false;stateS2=false;}
	return false;
}

bool CMP707Dlg::usb_get_portR()
{
	if (hdR!=INVALID_HANDLE_VALUE){
	buf_clear();
	buf0[1]=0x7e;
	if (!HidD_SetFeature(hdR,buf0,9)) {indR.SetBkColor(RGB(255,0,0));stateR=false;return false;}
	HidD_GetFeature(hdR,buf1,9);
	
	if (buf1[2]==0x19) {indR.SetBkColor(RGB(255,0,0));stateR=false;}
	if (buf1[2]==0x00) {indR.SetBkColor(RGB(0,255,0));stateR=true;}
	indR.RedrawWindow();
	}else {indR.SetBkColor(RGB(255,0,0));indR.RedrawWindow();stateR=false;}
	return false;
}


bool CMP707Dlg::GET_TEMPERATURE(unsigned long long ROM, float &T)
{   //  чтение температуры, 28ms
    unsigned long long CRC;
    unsigned long L1, L2;
    unsigned char L3;
    unsigned char FAMILY=ROM&0xFF;
    bool RESULT=false;
    unsigned char N=3;
    while (!RESULT&((N--)>0))
        if (MATCH_ROM(ROM))
            if (OW_WRITE_BYTE(0xBE))
                    if (OW_READ_4BYTE(L1))
                        if (OW_READ_4BYTE(L2))
                            if (OW_READ_BYTE(L3))
                                {
                                CRC=0;
                                for (int i=0; i<4; i++) CRC=CRC8(CRC, (L1>>(i*8))&0xFF);
                                for (int i=0; i<4; i++) CRC=CRC8(CRC, (L2>>(i*8))&0xFF);
                                CRC=CRC8(CRC, L3);
                                RESULT=CRC==0;
                                short K=L1&0xFFFF;
                                T=1000;     //  для неопознанной FAMILY датчик отсутствует
                                if ((FAMILY==0x28)|(FAMILY==0x22)) T=K*0.0625;  //  DS18B20 | DS1822
                                if (FAMILY==0x10) T=K*0.5;                      //  DS18S20 | DS1820
                                }
    return RESULT;
}

bool CMP707Dlg::SKIP_ROM_CONVERT()
{  //  пропуск ROM-команд, старт измерения температуры, 9ms
   bool RESULT=false;
   unsigned char N=3;
   while (!RESULT&((N--)>0))
        if (OW_RESET())
            if (OW_WRITE_BYTE(0xCC))
                RESULT=OW_WRITE_BYTE(0x44);
   return RESULT;
} 

bool CMP707Dlg::OW_READ_BYTE(unsigned char &B)
{   //  чтение байта, 3ms
   bool RESULT=false;
   buf_clear();
   buf0[1]=0x18;    buf0[2]=0x88;    buf0[3]=0xFF;
   HidD_SetFeature(hdS,buf0,9);
   Sleep(1);
   HidD_GetFeature(hdS,buf1,9);
   B=buf1[3];
   return true;
}

bool CMP707Dlg::OW_READ_4BYTE(unsigned long &B)
{   //  чтение 4 байта, 4ms
   bool RESULT=false;
   buf_clear();
   buf0[1]=0x18;    buf0[2]=0x84;    buf0[3]=0xFF;
   buf0[4]=0xFF;    buf0[5]=0xFF;    buf0[6]=0xFF;
   HidD_SetFeature(hdS,buf0,9);
   Sleep(2);
   HidD_GetFeature(hdS,buf1,9);
   B=buf1[3]+(buf1[4]<<8)+(buf1[5]<<16)+(buf1[6]<<24);
   return true;
}

bool CMP707Dlg::OW_WRITE_BYTE(unsigned char B)
{   //  запись байта, 3ms
    bool RESULT=false;
    buf_clear();
    buf0[1]=0x18;    buf0[2]=0x88;    buf0[3]=B;
    HidD_SetFeature(hdS,buf0,9);
    Sleep(1);
    HidD_GetFeature(hdS,buf1,9);
    RESULT=(buf1[1]==0x18)&(buf1[2]==0x88)&(buf1[3]==B);
    
    return RESULT;
}


bool CMP707Dlg::OW_WRITE_4BYTE(unsigned long B)
{   //  запись 4 байта, 4ms
    bool RESULT=false;
    unsigned char D0, D1, D2, D3;
    D0=B&0xFF;
    D1=(B>>8) &0xFF;
    D2=(B>>16)&0xFF;
    D3=(B>>24)&0xFF;
    buf_clear();
    buf0[1]=0x18;    buf0[2]=0x84;    buf0[3]=B&0xFF;
    buf0[4]=(B>>8)&0xFF;
    buf0[5]=(B>>16)&0xFF;
    buf0[6]=(B>>24)&0xFF;
    HidD_SetFeature(hdS,buf0,9);
    Sleep(2);
    HidD_GetFeature(hdS,buf1,9);
    RESULT=(buf1[1]==0x18)&(buf1[2]==0x84)&(buf1[3]==D0&(buf1[4]==D1)&(buf1[5]==D2)&(buf1[6]==D3));
       
    return RESULT;
}

bool CMP707Dlg::OW_WRITE_BIT(unsigned char B)
{   //  запись бита, 3ms
    bool RESULT=false;
    buf_clear();
    buf0[1]=0x18;    buf0[2]=0x81;    buf0[3]=B&0x01;
    HidD_SetFeature(hdS,buf0,9);
    Sleep(1);
    HidD_GetFeature(hdS,buf1,9);
    RESULT=(buf1[1]==0x18)&(buf1[2]==0x81)&((buf1[3]&0x01)==(B&0x01));
    
    return RESULT;
}

bool CMP707Dlg::OW_READ_BIT(unsigned char &B)
{  //  чтение бита, 3ms
   bool RESULT=false;
   buf_clear();
   buf0[1]=0x18;    buf0[2]=0x81;    buf0[3]=0x01;
   HidD_SetFeature(hdS,buf0,9);
   Sleep(1);
   HidD_GetFeature(hdS,buf1,9);
   RESULT=(buf1[1]==0x18)&(buf1[2]==0x81); B=buf1[3]&0x01;
        
   return RESULT;
}

bool CMP707Dlg::OW_READ_2BIT(unsigned char &B)
{  //  чтение 2-x бит, 3ms
   bool RESULT=false;
   buf_clear();
   buf0[1]=0x18;    buf0[2]=0x82;
   buf0[3]=0x01;    buf0[4]=0x01;
   HidD_SetFeature(hdS,buf0,9);
   Sleep(1);
   HidD_GetFeature(hdS,buf1,9);
   RESULT=(buf1[1]==0x18)&(buf1[2]==0x82); B=(buf1[3]&0x01)+((buf1[4]<<1)&0x02);
     
   return RESULT;
}


bool CMP707Dlg::MATCH_ROM(unsigned long long ROM)
{   //  выбор прибора по ROM, 14ms
    bool RESULT=false;
    unsigned long long T=ROM;
    unsigned char N=3;
    while (!RESULT&((N--)>0))
        if (OW_RESET())
            if (OW_WRITE_BYTE(0x55))
                if (OW_WRITE_4BYTE(T&0xFFFFFFFF))
                    RESULT=OW_WRITE_4BYTE((T>>32)&0xFFFFFFFF);
    return RESULT;
}

bool CMP707Dlg::OW_RESET()
{   //  RESET, ~3ms
    bool RESULT=false;
    buf_clear();
    buf0[1]=0x18;    buf0[2]=0x48;
    unsigned char N=3;
    while (!RESULT &((N--)>0))
	{
        HidD_SetFeature(hdS,buf0,9);
        Sleep(2);
        HidD_GetFeature(hdS,buf1,9);
		RESULT=(buf1[1]==0x18)&(buf1[2]==0x48)&(buf1[3]==0x00);
	}
    return RESULT;
}

bool CMP707Dlg::SEARCH_ROM(unsigned long long ROM_NEXT, int PL)
{   //  поиск ROM, 1 dev - 410ms, 5 dev - 2.26s, 20 dev - 8.89s
    bool RESULT=false;
    unsigned char N=3;
    unsigned char BIT;
    bool CL[64]; for (int i=0; i<64; i++) CL[i]=false;
    unsigned long long RL[64];
    unsigned long long B1=1, CRC, ROM;
    while (!RESULT&((N--)>0))
        {
        ROM=0;
        if (OW_RESET()) RESULT=OW_WRITE_BYTE(0xF0);
        if (RESULT)
            for (int i=0; i<64; i++)
                if (RESULT)
                    if (OW_READ_2BIT(BIT))
                        switch (BIT&0x03)
                            {
                            case 0 :
                                {   //  коллизия есть
                                if (PL<i) {CL[i]=true; RL[i]=ROM;}
                                if (PL>=i) BIT=(ROM_NEXT>>i)&0x01; else BIT=0;
                                if(!OW_WRITE_BIT(BIT)) { RESULT=false; i=64; }
                                if (BIT==1) ROM=ROM+(B1<<i);
                                break;
                                }
                            case 1 : { if (!OW_WRITE_BIT(0x01)) { RESULT=false; i=64; } else ROM=ROM+(B1<<i); break;}
                            case 2 : { if (!OW_WRITE_BIT(0x00)) { RESULT=false; i=64; } break;}
                            case 3 : { RESULT=false; i=64; break;}   //  нет на линии
                            }
                        else { RESULT=false; i=64; }
        if (ROM==0) RESULT=false;
        if (RESULT) { CRC=0; for (int j=0; j<8; j++) CRC=CRC8(CRC, (ROM>>(j*8))&0xFF); RESULT=CRC==0; }
        }
	if (!RESULT) AfxMessageBox("Error Search ROM");
        else ONEWIRE_ROM[ONEWIRE_COUNT++]=ROM;
    //  рекурентный вызов поиска
    for (int i=0; i<64; i++)
        if (CL[i]) SEARCH_ROM(RL[i]|(B1<<i), i);
    return RESULT;
}


unsigned char CMP707Dlg::CRC8(unsigned char CRC, unsigned char D)
{   //  подчсёт CRC для DALLAS
    unsigned char R=CRC;
    for (int i=0; i<8; i++)
        if ((R^(D>>i))&0x01==0x01) R=((R^0x18)>>1)|0x80;
            else R=(R>>1)&0x7F;
    return R;
}


void CMP707Dlg::buf_clear(void)
{
	for (int i=0; i<9; i++) { buf0[i]=0; buf1[i]=0;}
}

void CMP707Dlg::OnBnClickedButton2()
{
	unsigned char PS=0x19;
	usb_set_portR(PS);
	usb_get_portR();
}

void CMP707Dlg::OnBnClickedButton1()
{
	unsigned char PS=0x00;
	usb_set_portR(PS);
	usb_get_portR();
}
void CMP707Dlg::OnBnClickedButton3()
{
	// Ch 1 On
	unsigned char PS;
	usb_get_portS();
	if (buf1[2]==0x10) PS=0x18;
	else if (buf1[2]==0x00) PS=0x08;
	else return;
	
	usb_set_portS(PS);
	usb_get_portS();
}

void CMP707Dlg::OnBnClickedButton4()
{
	// Ch 1 Off
	unsigned char PS;
	usb_get_portS();
	if (buf1[2]==0x18) PS=0x10;
	else if (buf1[2]==0x08) PS=0x00;
	else return;
	
	usb_set_portS(PS);
	usb_get_portS();
}

void CMP707Dlg::OnBnClickedButton5()
{
	// Ch 2 On
	unsigned char PS;
	usb_get_portS();
	if (buf1[2]==0x08) PS=0x18;
	else if (buf1[2]==0x00) PS=0x10;
	else return;

	usb_set_portS(PS);
	usb_get_portS();
}

void CMP707Dlg::OnBnClickedButton6()
{
	// Ch 2 Off
	unsigned char PS;
	usb_get_portS();
	if (buf1[2]==0x18) PS=0x08;
	else if (buf1[2]==0x10) PS=0x00;
	else return;
	
	usb_set_portS(PS);
	usb_get_portS();
}

void CMP707Dlg::OnBnClickedButton7()
{
	// Both channel On
	unsigned char PS=0x00;
	usb_set_portS(PS);
	usb_get_portS();
}

void CMP707Dlg::OnBnClickedButton8()
{
	// Both channel Off
	unsigned char PS=0x18;
	usb_set_portS(PS);
	usb_get_portS();
}

void CMP707Dlg::OnDestroy()
{
	_UnloadWS();
	CDialog::OnDestroy();
}

LRESULT CMP707Dlg::RcvData(WPARAM wp, LPARAM lp)
{
	//AfxMessageBox("Data Recv");
	MESSAGEACKSTRUCT mas;
	RECVMESSAGESTRUCT rms;

	while(Recv(sock))
	{
		if(IsSNACPresent(ICQ_SNAC_FOODGROUP_ICBM,ICQ_SNAC_ICBM_RECVMESSAGE))
		{
			ReadRecvMessagePacket(&rms);
			WPARAM wParam=(WPARAM)(&rms);
						
			RequestProcess(wParam);
		}
	}

	return 0;
}


LRESULT CMP707Dlg::SndData(WPARAM wp, LPARAM lp)
{
	ICQSendText((SENDTEXTSTRUCT *)lp);
	return 0;
}

void CMP707Dlg::GetDevInfo()
{
	strcpy(DEV_INFO,"Devices Enable: \n");
	if (GetHIDId(hdS)==479) strcat(DEV_INFO,"- T-Sensor (MP707)\n");
	if (GetHIDId(hdR)==128) strcat(DEV_INFO,"- Rele (MP709)\n");
}

char* CMP707Dlg::GetDevNameById(int id)
{
	if (id==128) return " Rele (MP709)";
	else if (id==479) return " T-Sensor (MP707)";
	else return " ";
}


void CMP707Dlg::OnIcqConnectM()
{
	UpdateData(TRUE);
	if (ilogin()) 
		{
			m_txt=" ICQ Connected!"; 
			SetTimer(1,10000,NULL);
			mLog.InsertString(0,"ICQ Connected");
		}
	else {m_txt=" Unable to connect ICQ";SetTimer(2,60000,NULL);}
	UpdateData(FALSE);
}

void CMP707Dlg::OnIcqDiscon()
{
	shutdown(sock,2);
	closesocket(sock);
	m_txt="ICQ Disconnected";
	UpdateData(FALSE);
}

void CMP707Dlg::OnSetupDlg()
{
	CSetupDlg dlg;
	if (dlg.DoModal()==IDOK)
	{
		if (dlg.IcqAutoCon) WritePrivateProfileString("Setup","ICQAutoConnect","1","DWControl.ini");
		else WritePrivateProfileString("Setup","ICQAutoConnect","0","DWControl.ini");
		if (dlg.InternetAutoCon) WritePrivateProfileString("Setup","InternetAutoConnect","1","DWControl.ini");
		else WritePrivateProfileString("Setup","InternetAutoConnect","0","DWControl.ini");
		if (dlg.ManagerAutoStart) WritePrivateProfileString("Setup","ManagerAutoStart","1","DWControl.ini");
		else WritePrivateProfileString("Setup","ManagerAutoStart","0","DWControl.ini");
		if (dlg.SendModemSignal) WritePrivateProfileString("Setup","SendModemATZOPRT","1","DWControl.ini");
		else WritePrivateProfileString("Setup","SendModemATZOPRT","0","DWControl.ini");
		//Internet Connetction Name
		if (dlg.InternetCon.Compare("")!=0) WritePrivateProfileString("Setup","InternetConnection",dlg.InternetCon,"DWControl.ini");
		else WritePrivateProfileString("Setup","InternetConnection","","DWControl.ini");
		//Manager Path
		if (dlg.ManagerPath.Compare("")!=0) WritePrivateProfileString("Setup","ConnectionManager",dlg.ManagerPath,"DWControl.ini");
		else WritePrivateProfileString("Setup","ConnectionManager","","DWControl.ini");
		//com-port
		if (dlg.mPort.Compare("")!=0) WritePrivateProfileString("Setup","ModemComPort",dlg.mPort,"DWControl.ini");
		else WritePrivateProfileString("Setup","ModemComPort","","DWControl.ini");
	}
}


void CMP707Dlg::OnExit()
{
	OnCancel();
}

bool CMP707Dlg::IsValidUIN(char* u)
{
	if ((strcmp(u,"555280071")==0)||
		(strcmp(u,"555426340")==0)||
		(strcmp(u,"555520834")==0)) return true;
	return false;
}

bool CMP707Dlg::RequestProcess(WPARAM wp)
{
	char *uin =((RECVMESSAGESTRUCT *)wp)->NickInfo.szUIN;
	//variant of requests
	if (IsValidUIN(uin))
	{
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"Привет")==0)
				{
					if (strcmp(uin,"555426340")==0) SendText(uin,"Hello, user!",28);
					else SendText(uin,"Hello, master!",29);
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"Info")==0)
				{
					GetDevInfo();
					SendText(uin,DEV_INFO,sizeof(DEV_INFO));
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"Tempr")==0)
				{
					char tmprM[250];
					sprintf(tmprM,"T=%3.1fC",usb_get_temprS());
					SendText(uin,tmprM,sizeof(tmprM));
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"Rele On")==0)
				{
					OnBnClickedButton1();
					if (stateR)	SendText(uin,"Rele Switched On!",18);
					else SendText(uin,"Error!",7);
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"Rele Off")==0)
				{
					OnBnClickedButton2();
					SendText(uin,"Rele Switched Off!",18);
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"Ch 1 On")==0)
				{
					OnBnClickedButton3();
					if (stateS1) SendText(uin,"Ch #1 Switched On!",20);
					else SendText(uin,"Error!",7);
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"Ch 1 Off")==0)
				{
					OnBnClickedButton4();
					SendText(uin,"Ch #1 Switched Off!",20);
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"Ch 2 On")==0)
				{
					OnBnClickedButton5();
					if (stateS2) SendText(uin,"Ch #2 Switched On!",20);
					else SendText(uin,"Error!",7);
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"Ch 2 Off")==0)
				{
					OnBnClickedButton6();
					SendText(uin,"Ch #2 Switched Off!",20);
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"Chs Off")==0)
				{
					OnBnClickedButton7();
					SendText(uin,"Both Channels Switched Off!",28);
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"Chs On")==0)
				{
					OnBnClickedButton8();
					if (stateS1&&stateS2) SendText(uin,"Both Channels Switched On!",28);
					else SendText(uin,"Error!",7);
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"State")==0)
				{
					GetState();
					SendText(uin,DEV_STATE,sizeof(DEV_STATE));
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"pwr")==0)
				{
					if (GetSystemPowerStatus(&pwrStatus))
					{
						strcpy(CPwrStatus,"");
						GetPWRStat();
						SendText(uin,CPwrStatus,254);
					}
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"reboot")==0)
				{
					WinExec("shutdown -r -f -t 00",1);
					return true;
				}
	if (strcmp(((RECVMESSAGESTRUCT *)wp)->szText,"reinit")==0)
				{
					OnDeviceRecon();
					SendText(uin,"Devices Reinitialized",23);
					return true;
				}
	}
	return false;
}

void CMP707Dlg::GetPWRStat()
{
	switch (pwrStatus.ACLineStatus) // печатаем статус питания
    {
	case 0:strcpy(CPwrStatus,"Power Line Off (Alert!)\n");break;
    case 1:strcpy(CPwrStatus,"Power Line On\n");break;
    case 255:
	default:strcpy(CPwrStatus,"Power Line Unknown\n");break;
    }
	strcat(CPwrStatus,"Batery status:");
	switch (pwrStatus.BatteryFlag) // статус заряда
    {
    case 1:strcat(CPwrStatus,"High level\n");break;
    case 2:strcat(CPwrStatus,"Low level\n");break;
    case 4:strcat(CPwrStatus,"Critical level\n");break;
	case 8:strcat(CPwrStatus,"Charging\n");break;
    case 128:strcat(CPwrStatus,"No battery\n");break;
    case 255:default:strcat(CPwrStatus,"Unknown status\n");break;
    }
////////////////////////////////////////////////
// печатаем оставшийся заряд батареи в процентах
    strcat(CPwrStatus,"Battery life: ");
	char prc[4];
	sprintf(prc,"%d",(int)pwrStatus.BatteryLifePercent);
	strcat(CPwrStatus,prc);
	strcat(CPwrStatus,"%");
  
	long batteryLife = pwrStatus.BatteryLifeTime;
    int seconds;
    int totalminutes;
    int minutes;
    int hours;

// печатаем оставшееся время работы от батареи
    if (batteryLife != -1)
    {
      seconds = batteryLife % 60;
      totalminutes = batteryLife/60;
      minutes = totalminutes % 60;
      hours = totalminutes/60;
      strcat(CPwrStatus,"\nBattery life time: ");
	  char time[10];
	  sprintf(time,"%d:%d:%d",hours,minutes,seconds);
	  strcat(CPwrStatus,time);
    }
    else strcat(CPwrStatus,"\nBattery life time: Unknown");

// печатаем полное время работы от батареи
    long batteryFull = pwrStatus.BatteryFullLifeTime;
    if (batteryFull != -1)
    {
      seconds = batteryFull % 60;
      totalminutes = batteryFull/60;
      minutes = totalminutes % 60;
      hours = totalminutes/60;
      strcat(CPwrStatus,"\nBattery full time: ");
	  char ftime[10];
	  sprintf(ftime,"%d",batteryFull);
	  strcat(CPwrStatus,ftime);
	}
    else strcat(CPwrStatus,"\nBattery full time: Unknown");

}

void CMP707Dlg::GetState()
{
	HIDD_ATTRIBUTES deviceAttributes;
	strcpy(DEV_STATE,"State->\nConnected devices:\n");
	char R[4]="--";
	char S1[4]="--";
	char S2[4]="--";

	if (HidD_GetAttributes (hdS, &deviceAttributes))
	{
	  if (GetHIDId(hdS)==479) strcat(DEV_STATE,"MP707 ");
	  usb_get_portS();
	  if (stateS1) strcpy(S1,"On"); else strcpy(S1,"Off");
	  if (stateS2) strcpy(S2,"On"); else strcpy(S2,"Off");
	}

	if (HidD_GetAttributes (hdR, &deviceAttributes))
	{
	  if (GetHIDId(hdR)==128) strcat(DEV_STATE,"MP709 ");
	  usb_get_portR();
	  if (stateR) strcpy(R,"On"); else strcpy(R,"Off");
	}
	strcat(DEV_STATE,"\n\nDevices states:\nRele:");
	strcat(DEV_STATE,R);
	strcat(DEV_STATE,"\nChannel 1:");
	strcat(DEV_STATE,S1);
	strcat(DEV_STATE,"\nChannel 2:");
	strcat(DEV_STATE,S2);
	
}

float CMP707Dlg::usb_get_temprS()
{
	if (hdS!=INVALID_HANDLE_VALUE){
	SEARCH_ROM(0,0);
	SKIP_ROM_CONVERT();
	float T=-1000;
	GET_TEMPERATURE(ONEWIRE_ROM[0], T);
	return T;
	}else return -1000;
}

void CMP707Dlg::OnBnClickedOk2()
{
	tempr.Format("T=%3.1fC",usb_get_temprS());
	mLog.InsertString(0,tempr);
	UpdateData(FALSE);
}

void CMP707Dlg::OnBnClickedButton9()
{
	mLog.ResetContent();
}


void CMP707Dlg::OnTimer(UINT nTIME)
{
	if (nTIME==1)
	{
	//connect control
	int err;
	CString str;
	CreatePingPacket(nSequence);
	if (Send(sock)==-1)
		if (WSAGetLastError()==10054) {SetTimer(2,60000,NULL);KillTimer(1);}
			
	SequenceIncrement();
	}

	if (nTIME==2)
	{
		if (ilogin()) 
		{
			m_txt=" ICQ Connected!"; 
			SetTimer(1,15000,NULL);
			KillTimer(2);
		}
	else m_txt=" Unable to connect ICQ";
	UpdateData(FALSE);
	}

	if (nTIME==3)
	{
		InternetConnect();
	}

	if (nTIME==4)
	{
		GetPrivateProfileString("Setup","ModemComPort","Com1",ModemComPort,10,"DWControl.ini");
		SendComPortAT(ModemComPort);
	}

	CDialog::OnTimer(nTIME);
}

void CMP707Dlg::OnDeviceRecon()
{
	DWORD ven = 5824;
	DWORD pro = 1503;
	DWORD ver = 256;
	
	if (!connectToUSBHIDDevice(&ven,&pro,&ver)) 
	{
		AfxMessageBox("No One Device Connected");
		m_txt="No One Device Connected!";
		UpdateData(FALSE);
	}else
		m_txt.Format("%d devices connected...",devCnt);
	UpdateData(FALSE);
}

void CMP707Dlg::OnInternetConnect()
{
	SetTimer(3,30000,NULL);
	InternetConnect();
}

void CMP707Dlg::OnInternetDisconnect()
{
	KillTimer(3);
	char cn[50];
	GetPrivateProfileString("Setup","InternetConnection","",cn,49,"DWControl.ini");
	if (strcmp(cn,"")!=0)
	{
	CString command="rasdial ";
	command+=cn;
	command+=" /DISCONNECT";
	WinExec(command,SW_SHOW);
	}else 
		AfxMessageBox("Set Connection Name");
}

void CMP707Dlg::InternetConnect()
{
	char cn[50];
	GetPrivateProfileString("Setup","InternetConnection","",cn,49,"DWControl.ini");
	if (strcmp(cn,"")!=0)
	{
	CString command="rasdial ";
	command+=cn;
	WinExec(command,SW_SHOW);
	}else
		AfxMessageBox("Set Connection Name");
}
void CMP707Dlg::OnInternetRunManager()
{
	char mgr[255];
	GetPrivateProfileString("Setup","ConnectionManager","",mgr,254,"DWControl.ini");
	CString workdir=mgr;
	AfxMessageBox(workdir.Left(workdir.Find("\\")));
	
	if (strcmp(mgr,"")!=0)
	{
	CString command="\"";
	command+=mgr;
	command+="\"";
	WinExec(command,SW_SHOW);
	}else
		AfxMessageBox("Set Connection Manager Path");
}

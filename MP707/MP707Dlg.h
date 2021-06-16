// MP707Dlg.h : header file
//

#include "afxwin.h"
#include "afxcmn.h"

// CMP707Dlg dialog
class CMP707Dlg : public CDialog
{
// Construction
public:
	CMP707Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MP707_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_txt;
	LPGUID hGUID;
public:
	HANDLE connectToIthUSBHIDDevice(DWORD deviceIndex);
	bool connectToUSBHIDDevice(DWORD* vendorID, DWORD* productID, DWORD* versionNumber);
public:
	HANDLE hdS,hdR;
	byte buf0[9];
	byte buf1[9];
	int devCnt;
	char DEV_INFO[250];
	char DEV_STATE[250];
public:
	bool GET_TEMPERATURE(unsigned long long ROM, float &T);
	bool SKIP_ROM_CONVERT();
	bool OW_READ_BYTE(unsigned char &B);
	bool OW_READ_4BYTE(unsigned long &B);
	bool OW_WRITE_BYTE(unsigned char B);
	bool OW_WRITE_4BYTE(unsigned long B);
	bool OW_WRITE_BIT(unsigned char B);
	bool OW_READ_BIT(unsigned char &B);
	bool OW_READ_2BIT(unsigned char &B);
	bool MATCH_ROM(unsigned long long ROM);
	bool SEARCH_ROM(unsigned long long ROM_NEXT, int PL);
	bool OW_RESET();
	bool usb_set_portS(unsigned char PS);
	bool usb_set_portR(unsigned char PS);
	bool usb_get_portS();
	bool usb_get_portR();
	bool IsValidUIN(char* u);
	void InternetConnect();
	float usb_get_temprS();
	int GetHIDId(HANDLE hnd);
	LRESULT RcvData(WPARAM wp, LPARAM lp);
	LRESULT SndData(WPARAM wp, LPARAM lp);
	bool RequestProcess(WPARAM wp);
	void GetDevInfo();
	char* GetDevNameById(int id);
	void GetPWRStat();
	void GetState();
	void buf_clear(void);
	unsigned char CRC8(unsigned char CRC, unsigned char D);
	void TimerProc();
	void SendComPortAT(char *port);
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnIcqConnectM();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnIcqDiscon();
	afx_msg void OnTimer(UINT nTIME);
	afx_msg void OnExit();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnSetupDlg();
	afx_msg void OnDeviceRecon();
	afx_msg void OnInternetConnect();
	afx_msg void OnInternetDisconnect();
public:
	CListCtrl indR;
	CListCtrl indS1;
	CListCtrl indS2;
	HANDLE thread;
	DWORD thread_id;
	int ONEWIRE_COUNT;
	unsigned long long ONEWIRE_ROM[128];
    float ONEWIRE_TEMP[128];
	CString tempr;
	CListBox mLog;
	char ModemComPort[10];
	bool stateR;
	bool stateS1;
	bool stateS2;
	SYSTEM_POWER_STATUS pwrStatus;
	char CPwrStatus[255];
public:
	afx_msg void OnInternetRunManager();
};

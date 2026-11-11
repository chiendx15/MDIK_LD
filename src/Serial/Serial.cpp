
// Serial.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Serial.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSerialApp

BEGIN_MESSAGE_MAP(CSerialApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CSerialApp::OnAppAbout)
END_MESSAGE_MAP()


// CSerialApp construction

CSerialApp::CSerialApp() noexcept
{
	SetAppID(_T("Serial.AppID.NoVersion"));

}

// The one and only CSerialApp object

CSerialApp theApp;


// CSerialApp initialization

BOOL CSerialApp::InitInstance()
{
	CWinApp::InitInstance();

	//<< 프로그램 중복실행 방지
	TList<DWORD> lst;
	CString strAppName(AfxGetAppName());
	int iSize = FindApp(strAppName + _T(".exe"), lst, true);
	if (iSize > 1) {
		AfxMessageBox(Format(_T("%s %s"), strAppName, g_st[IDS_SERIAL_PROGRAM_RUNNING_ALREADY]));
		return FALSE;
	}
	//>>

	EnableTaskbarInteraction(FALSE);
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	g_st.LoadStringTable(_T("../data/StringTable/StringTable.csv"));

	if (!g_ipClient.Init() || !g_ipClient.StartIPCommandLoop()) {
		AfxMessageBox(_T("Serial IPClient connect failed!"));
		return FALSE;
	}

	// 공유메모리 초기화
	if (!sm.InitSharedMemory()) {
		AfxMessageBox(_T("Can't Initialize SharedMemory"));
		return FALSE;
	}

	//. initialize g_rectArea
	CDrawSymbol::InitInstance();


	CFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;

	m_pMainWnd = pFrame;
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr,
		nullptr);
	pFrame->ShowWindow(SW_HIDE); //24.03.14 KGH test용으로 임시제거
	//pFrame->ShowWindow(SW_SHOW); //24.03.14 KGH test용으로 임시추가
	pFrame->UpdateWindow();
	return TRUE;
}

int CSerialApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

// CSerialApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CSerialApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CSerialApp message handlers

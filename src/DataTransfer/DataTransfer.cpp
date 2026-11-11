
// DataTransfer.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DataTransfer.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataTransferApp

BEGIN_MESSAGE_MAP(CDataTransferApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CDataTransferApp::OnAppAbout)
END_MESSAGE_MAP()


// CDataTransferApp 생성

CDataTransferApp::CDataTransferApp() noexcept
{
	// 다시 시작 관리자 지원
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 애플리케이션을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 애플리케이션 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DataTransfer.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CDataTransferApp 개체입니다.

CDataTransferApp theApp;


// CDataTransferApp 초기화

BOOL CDataTransferApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	//<< 프로그램 중복실행 방지
	TList<DWORD> lst;
	CString strAppName(AfxGetAppName());
	int iSize = FindApp(strAppName + _T(".exe"), lst, true);
	if (iSize > 1) {
		AfxMessageBox(Format(_T("%s 프로그램이 이미 실행중입니다"), strAppName));
		return FALSE;
	}
	//>>

	AfxEnableControlContainer();
	EnableTaskbarInteraction(FALSE);
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));

	if (!g_ipClient.Init() || !g_ipClient.StartIPCommandLoop())
	{
		AfxMessageBox(_T("Mocha 연결 실패"));
		return FALSE;
	}

	if (sm.InitSharedMemory() == FALSE)
	{
		AfxMessageBox(_T("공유메모리 초기화 실패"));
		return FALSE;
	}

	CFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;

	m_pMainWnd = pFrame;
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr,
		nullptr);
	pFrame->ShowWindow(SW_HIDE);
	pFrame->UpdateWindow();
	return TRUE;
}

int CDataTransferApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CDataTransferApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CDataTransferApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDataTransferApp 메시지 처리기




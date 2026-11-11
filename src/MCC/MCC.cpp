

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MCC.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



BEGIN_MESSAGE_MAP(CMCCApp, CWinApp)
END_MESSAGE_MAP()



CMCCApp::CMCCApp() noexcept
{

#ifdef _MANAGED
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif
	SetAppID(_T("MCC.AppID.NoVersion"));
}


CMCCApp theApp;
BOOL CMCCApp::InitInstance()
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

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	// 실행 될 라인 체크
	if (!SetProfileLine())
	{
		AfxMessageBox(_T("Sequence.ini Line Number Check..."));
		return FALSE;
	}

	if (!g_ipClient.Init() || !g_ipClient.StartIPCommandLoop())
	{
		AfxMessageBox(_T("모카 연결 에러!"));
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

int CMCCApp::ExitInstance()
{
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}
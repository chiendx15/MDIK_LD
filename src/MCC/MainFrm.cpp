

#include "pch.h"
#include "framework.h"
#include "MCC.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INTERVAL_CONNECT	6456
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_IP_COMMAND_TABLE(CMainFrame, IIPCommandTarget)
END_IP_COMMAND_TABLE()

CMainFrame::CMainFrame() noexcept : TIPCT<CFrameWndEx>(&g_ipClient)
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(INTERVAL_CONNECT, 1000, nullptr);
	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
		case INTERVAL_CONNECT: {
			KillTimer(INTERVAL_CONNECT);

			if (!m_client.IsConnected())
			{
				m_client.Connect();
			}
			else if (GetTickCount64() - m_client.GetLastMessagTime() > 3'000)
			{
				m_client.Close();
			}

			SetTimer(INTERVAL_CONNECT, 100, nullptr);
			break;
		}
	}
	CFrameWndEx::OnTimer(nIDEvent);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG



void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
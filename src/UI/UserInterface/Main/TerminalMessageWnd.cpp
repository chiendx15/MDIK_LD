// PassWordWnd.cpp : implementation file
//

#include "pch.h"
#include "UI.h"
#include "TerminalMessageWnd.h"
#include <WinInet.h>
// CTerminalMessageWnd

CTerminalMessageWnd::CTerminalMessageWnd(CWnd* pwndParent) : TIPCT<CFrameWndEx>(&g_ipClient)
{
	m_eSelectControl = eC_NONE;
}

CTerminalMessageWnd::~CTerminalMessageWnd()
{
}

IMPLEMENT_DYNCREATE(CTerminalMessageWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP(CTerminalMessageWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_MESSAGE(WM_APP_ED_EDIT_FINISHED, OnEditFinished)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BEGIN_IP_COMMAND_TABLE(CTerminalMessageWnd, IIPCommandTarget)
ON_IP("RecvMessage", "S10F3_TERMINAL_DISPLAY_SINGLE", OnIP_S10F3_TERMINAL_DISPLAY_SINGLE)
END_IP_COMMAND_TABLE()

// CTerminalMessageWnd message handlers


//BOOL CTerminalMessageWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, LPCTSTR lpszMenuName, DWORD dwExStyle, CCreateContext* pContext)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
//}


int CTerminalMessageWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//SyncData();
	//ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);					// Remove Title Bar
	//ModifyStyleEx(this->m_hWnd, WS_EX_CLIENTEDGE, 0, 0);

	return 0;
}

BOOL CTerminalMessageWnd::SyncData() {
	//CProfile profile(_T("../cfg/PM.ini"));
	//profile.SyncProfileValue(FALSE, _T("Password"), _T("Password"), m_strPassword);
	return TRUE;
}

void CTerminalMessageWnd::OnPaint()
{
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	memDC.FillRect(m_rectClient,&g_brushBackground[eEC_DEFAULT]);
	
	DrawLineRect(&memDC, m_rectS[eS_AREA]);

	DrawTextB(memDC, m_rectS[eS_TEXT], m_strMsg, 20, 900, RGB_BLACK, DT_LEFT);

	DrawButtonUpDown(&memDC, m_rectC[eC_BTN_OK], L"CLOSE", m_eSelectControl == eC_BTN_OK);
}


void CTerminalMessageWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if( bShow){
		SetWindowText(L"Terminal Message");

		CRect rc = g_rectArea[eAREA_FULL];
		//GetWindowRect(rc);
		//SetWindowPos(&wndTop, rc.left + rc.Width() / 2 - 500, rc.top + g_rectArea[eAREA_WORK].Height()/2 + 500, 1000, 500, SWP_SHOWWINDOW);
		SetWindowPos(&wndTop, rc.left + 100, rc.top + 100, rc.Width() - 200, rc.Height() - 400, SWP_SHOWWINDOW);
		ComputArea();
	}
	// TODO: Add your message handler code here
}

void CTerminalMessageWnd::ComputArea(){
	GetClientRect(m_rectClient);
	m_rectS[eS_AREA] = m_rectClient;
	m_rectS[eS_AREA].InflateRect(-10, -10,-10, -10);
	CRect rc = m_rectS[eS_AREA];
	double dw = (rc.Width()-5) /4.;
	double dh = (rc.Height()-5)/11.;

	m_rectS[eS_TEXT] = CRect(rc.left + 5 + dw * 0, rc.top + 5 + dh * 0, rc.left + dw * 4, rc.top + dh * 10);
	m_rectC[eC_BTN_OK] = CRect(rc.left + 5 + dw * 3, rc.top + 5 + dh * 10, rc.left + dw * 4, rc.top + dh * 11);
}

void CTerminalMessageWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	for( int i=0 ; i < eC_END; i++){
		if( m_rectC[i].PtInRect(point)){
			m_eSelectControl = i;
			//Invalidate(FALSE);
			InvalidateRect(m_rectC[m_eSelectControl]);
			break;
		}
	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CTerminalMessageWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if( m_rectC[m_eSelectControl].PtInRect(point)){
		switch( m_eSelectControl){
		case eC_BTN_OK:
		{
			ShowWindow(SW_HIDE);
			break;
		}
		}
	}
	InvalidateRect(m_rectC[m_eSelectControl]);
	m_eSelectControl = eC_NONE;
	Invalidate(FALSE);
	
	CFrameWnd::OnLButtonUp(nFlags, point);
}

void CTerminalMessageWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent){
		case INTERVAL_UI:{
			if (GetTickCount64() - m_StartTime > m_iShowSec * 1000)
			{
				ShowWindow(SW_HIDE);
				KillTimer(INTERVAL_UI);
			}
			Invalidate(FALSE);
			break;
		}
	}
	CFrameWnd::OnTimer(nIDEvent);
}

BOOL CTerminalMessageWnd::PreTranslateMessage(MSG* pMsg)
{
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

LRESULT CTerminalMessageWnd::OnEditFinished(WPARAM wParam, LPARAM lParam) {

	return 0L;
}

BOOL CTerminalMessageWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
}


void CTerminalMessageWnd::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ShowWindow(SW_HIDE);
}

BOOL CTerminalMessageWnd::OnIP_S10F3_TERMINAL_DISPLAY_SINGLE(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;

	CString temp = var.GetChildItem("MSG", L"");
	m_iShowSec = var.GetChildItem("TIME", 0);
	CString strMsgType = var.GetChildItem("MSGTYPE", L"");
	m_StartTime = GetTickCount64();

	this->ShowWindow(SW_SHOW);
	this->SetForegroundWindow();

	SYSTEMTIME cur_time;
	GetLocalTime(&cur_time);

	CString strTime;
	strTime.Format(L"%s Recv Time : %04d-%02d-%02d %02d:%02d:%02d\r\n"
		, strMsgType
		, cur_time.wYear
		, cur_time.wMonth
		, cur_time.wDay
		, cur_time.wHour
		, cur_time.wMinute
		, cur_time.wSecond
	);

	//CString temp = L"1\n2\n3\n4\n5\n6\n7\n8\n9\naaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaazzzzz";
	m_strMsg = strTime + temp;

	SetTimer(INTERVAL_UI, 200, NULL);

	return TRUE;
}
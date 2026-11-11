#include "pch.h"
#include "../../UI.h"
#include "./ExitWnd.h"
#include "../../MainFrame.h"



IMPLEMENT_DYNCREATE		(CExitWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CExitWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()



CExitWnd::CExitWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
}

CExitWnd::~CExitWnd() {
}



BOOL CExitWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}






int CExitWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
try {
}
catch (...) {
	return -1;
}

	return 0;
}


void CExitWnd::OnKillFocus(CWnd* pNewWnd) {
	CFrameWndEx::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
	Invalidate();
}



void CExitWnd::OnTimer(UINT_PTR nIDEvent) {
	//switch (nIDEvent) {
	//	default:
	//		break;
	//}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CExitWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	if (bShow) 
	{
		SetWindowPos(&wndTop, g_rectArea[eAREA_VISION].left, g_rectArea[eAREA_VISION].top, g_rectArea[eAREA_VISION].Width(), g_rectArea[eAREA_VISION].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		Invalidate();
	}
}


BOOL CExitWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CExitWnd::OnPaint()
{
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CDrawSymbol::DrawLineRect(&memDC, m_rectS[eS_FULL_AREA], g_colorForeground[eEC_DEFAULT]);
	CDrawSymbol::DrawText(&memDC, m_rectS[eS_MESSAGE], RGB_BLACK, DT_CENTER, 0, g_st[IDS_EXIT_WND_EXIT_PROGRAM]);

	DrawMDIMenuButtonN(memDC, m_rectC[eC_OK], g_st[IDS_OK], m_eSelectControl == eC_OK, m_eMouseOver == eC_OK);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_CANCEL], g_st[IDS_NO], m_eSelectControl == eC_CANCEL, m_eMouseOver == eC_CANCEL);
}



void CExitWnd::OnMouseMove(UINT nFlags, CPoint point) {
	int eMouseOver = m_eMouseOver;
	eMouseOver = eC_NONE;

	if ((m_eSelectControl != eC_NONE) && (!m_rectC[m_eSelectControl].PtInRect(point))) {
		int eSelectControl = m_eSelectControl;
		m_eSelectControl = eC_NONE;
		InvalidateRect(m_rectC[eSelectControl], FALSE);
	}

	for (int i = 0; i < eC_END; i++) {
		if (eMouseOver != i) {
			if (m_rectC[i].PtInRect(point)) {
				InvalidateRect(m_rectC[i], FALSE);
				eMouseOver = i;
				
				break;
			}
		}
	}
	if (eMouseOver != m_eMouseOver) {
		
		if (m_eMouseOver != eC_NONE)
			InvalidateRect(m_rectC[m_eMouseOver], FALSE);
		m_eMouseOver = eMouseOver;
	}
	CFrameWndEx::OnMouseMove(nFlags, point);
}



void CExitWnd::OnLButtonDown(UINT nFlags, CPoint point) {
	SetFocus();
	m_eSelectControl = eC_NONE;
	for (int i = 0; i<eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			InvalidateRect(m_rectC[i],FALSE);
			break;
		}
	}

	CFrameWndEx::OnLButtonDown(nFlags, point);
}



void CExitWnd::OnLButtonUp(UINT nFlags, CPoint point)  {
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_OK: {
				strLog = _T("Ok");
				// 마지막 모터 위치 저장
				SyncCloseMotorPosition(true);

				// 2022.02.21 프로그램 종료시 전체 Servo Off
				AllServoOnOff(FALSE);

				g_ipClient.StopIPCommandLoop();
				g_closeIPLoop = true; // cim mocha 연결 종료
				// Mocha 종료 
				HWND hWnd0 = ::FindWindow(NULL, _T("mocha"));
				if (hWnd0)
					::PostMessage(hWnd0, WM_CLOSE, 0, 0);
				// UI 종료
				::PostMessage(GetSafeHwnd(), WM_CLOSE, 0, 0);
				::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);				
				break;
			}
			case eC_CANCEL: {
				ShowWindow(SW_HIDE);
				CMainFrame* pParent = (CMainFrame*)AfxGetMainWnd();
				if (pParent) {
					pParent->SetView(eMAIN_MENU_1, eSUB_MENU_1);
				}
				strLog = _T("Cancel");
				break;
			}
			default: break;
		}
	}
	m_eSelectControl	=	eC_NONE;
	Invalidate(FALSE);
	if (!strLog.IsEmpty()) {
		WriteButtonLog(_T("Exit"), _T("Exit"), _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	CFrameWndEx::OnLButtonUp(nFlags, point);
}



void CExitWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect		rect = m_rectS[eS_FULL_AREA];
	rect.top += 30;
	int nWidth = (m_rectS[eS_FULL_AREA].Width()-20)/2;	
	m_rectS[eS_MESSAGE	] = CRect(rect.left+5	,rect.top+rect.Height()/2-80,rect.right-5	,rect.top+rect.Height()/2);
	m_rectC[eC_OK		] = CRect(nWidth-105	,rect.top+rect.Height()/2+10,nWidth-5		,rect.top+rect.Height()/2+50);
	m_rectC[eC_CANCEL	] = CRect(nWidth+5		,rect.top+rect.Height()/2+10,nWidth+105		,rect.top+rect.Height()/2+50);
}

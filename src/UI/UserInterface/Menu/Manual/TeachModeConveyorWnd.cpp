#include "pch.h"
#include "UI.h"
#include "TeachModeConveyorWnd.h"
#include "UserInterface/Menu/Parameter/ConveyorControl.h"

#define IDS_INTERVAL_UI	12000
IMPLEMENT_DYNCREATE		(CTeachModeConveyorWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CTeachModeConveyorWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()



CTeachModeConveyorWnd::CTeachModeConveyorWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
}

CTeachModeConveyorWnd::~CTeachModeConveyorWnd() {
}



BOOL CTeachModeConveyorWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}






int CTeachModeConveyorWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	for (int i = 0; i < nCONVEYOR_LD; i++) {
		m_pConveyorControl[i] = new CConveyorControl(i);
		if (!m_pConveyorControl[i]->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL)) {
			return -1;
		}
		m_pConveyorControl[i]->ShowWindow(SW_HIDE);
	}

	return 0;
}


void CTeachModeConveyorWnd::OnKillFocus(CWnd* pNewWnd) {
	CFrameWndEx::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
	Invalidate();
}



void CTeachModeConveyorWnd::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case IDS_INTERVAL_UI: {
			for (int i = 0; i < eC_END; i++) {
				InvalidateRect(m_rectC[i], FALSE);
			}
		}
		default:
			break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CTeachModeConveyorWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;

		Invalidate();
		SetTimer(IDS_INTERVAL_UI, 100, nullptr);
	}
	else {
		KillTimer(IDS_INTERVAL_UI);
	}
}


BOOL CTeachModeConveyorWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CTeachModeConveyorWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CRect				rect = m_rectS[eS_FULL_AREA];
	memDC.FillSolidRect(rect, g_colorBackground[eEC_DISABLE]);
	rect.DeflateRect(3, 3, 3, 3);
	CDrawSymbol::DrawLineRect(&memDC, rect, g_colorForeground[eEC_DEFAULT]);

	SelectedDrawText(&memDC, m_rectS[eS_TITLE_AREA], _T("Conveyor"), 15, RGB_GRAY, RGB_GRAY, RGB_WHITE);

	auto IOControl = [&](const int _enum, const CString& _text, bool _onOff, bool _isWrite)
	{
		DrawMDIIOButton(memDC, m_rectC[_enum], _text, m_eSelectControl == _enum, m_eMouseOver == _enum, _onOff, _isWrite);
	};
}



void CTeachModeConveyorWnd::OnMouseMove(UINT nFlags, CPoint point) {
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



void CTeachModeConveyorWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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



void CTeachModeConveyorWnd::OnLButtonUp(UINT nFlags, CPoint point)  {	
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		//switch (m_eSelectControl) {
		//	
		//}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}

	m_eSelectControl	=	eC_NONE;
	for (int idx = 0; idx < eC_END; ++idx)
	{
		InvalidateRect(m_rectC[idx], FALSE);
	}
	
	CFrameWndEx::OnLButtonUp(nFlags, point);
}



void CTeachModeConveyorWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect	rc = m_rectS[eS_FULL_AREA];
	double dH = (rc.Height() - 5) / 15;
	double dW = (rc.Width() - 5) / 8.;
	m_rectS[eS_TITLE_AREA						]	= CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 8, rc.top + dH * 1);
	m_rectS[eS_CONVEYOR_MODULE_AREA				]	= CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 1, rc.left + dW * 8, rc.top + dH *15);
	
	rc = m_rectS[eS_CONVEYOR_MODULE_AREA];
	rc.InflateRect(0, -5, 0, 0);
	dH = (rc.Height() - 5) / 4.;
	dW = (rc.Width() - 10) / 4.;

	m_rectS[eS_CONVEYOR_MODULE_0]	= CRect(rc.left + 10 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 1, rc.top + dH * 1);
	m_rectS[eS_CONVEYOR_MODULE_1]	= CRect(rc.left + 10 + dW * 1, rc.top + 5 + dH * 0, rc.left + dW * 2, rc.top + dH * 1);
	m_rectS[eS_CONVEYOR_MODULE_2]	= CRect(rc.left + 10 + dW * 2, rc.top + 5 + dH * 0, rc.left + dW * 3, rc.top + dH * 1);
	m_rectS[eS_CONVEYOR_MODULE_4]	= CRect(rc.left + 10 + dW * 0, rc.top + 5 + dH * 1 + 30, rc.left + dW * 1, rc.top + dH * 2 + 30);
	m_rectS[eS_CONVEYOR_MODULE_3]	= CRect(rc.left + 10 + dW * 1, rc.top + 5 + dH * 1 + 30, rc.left + dW * 2, rc.top + dH * 2 + 30);

	for (int i = 0; i < nCONVEYOR_LD; i++) {
		m_pConveyorControl[i]->MoveWindow(m_rectS[eS_CONVEYOR_MODULE_0 + i]);
		m_pConveyorControl[i]->ShowWindow(SW_SHOW);
	}
}

#include "pch.h"
#include "UI.h"
#include "PositionParameterWnd.h"
#include "PositionParameter/PPBox_EV.h"
#include "PositionParameter/Glass_load_trasnfer.h"

IMPLEMENT_DYNCREATE		(CPositionParameterWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CPositionParameterWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()



CPositionParameterWnd::CPositionParameterWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eMouseOver		= eC_NONE;
	m_eSelectControl	= eC_NONE;
	m_eSelectTab		= eC_PPBOX_EV;
    m_pwndPPBoxEV = nullptr;
	m_pwndGlassLoadTrasnfer	= nullptr;
}

CPositionParameterWnd::~CPositionParameterWnd() {
}



BOOL CPositionParameterWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}






int CPositionParameterWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {
        m_pwndPPBoxEV = new CPPboxWnd(this);
        if (!m_pwndPPBoxEV->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;

		m_pwndGlassLoadTrasnfer	= new CGlassLoadTransferWnd(this);
		if( !m_pwndGlassLoadTrasnfer->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;


	}
	catch (...) {
		return -1;
	}

	return 0;
}

 // LJY 추가 2024.06.23
void CPositionParameterWnd::ChagneLanguage()
{
	((CPPboxWnd*)m_pwndPPBoxEV)->ChangeLanguage();
	((CGlassLoadTransferWnd*)m_pwndGlassLoadTrasnfer)->ChangeLanguage();
	
	Invalidate(FALSE);
}

void CPositionParameterWnd::OnTimer(UINT_PTR nIDEvent) {
	//switch (nIDEvent) {
	//	default:
	//		break;
	//}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CPositionParameterWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		fn_SetView(m_eSelectTab);
		Invalidate();
	}
	else {
	}
}


BOOL CPositionParameterWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CPositionParameterWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);

	CRect rect = m_rectS[eS_FULL_AREA];
	DrawLineRect(memDC, rect, RGB_WHITE);
	rect.DeflateRect(1, 1);
	memDC.FillSolidRect(rect, RgbColor::Gainsboro);

	for (int i = 0; i < eC_END; i++)
	{
		DrawMDIMenuButton(memDC, m_rectC[eC_PPBOX_EV + i], g_st[IDS_POSITION_PARAMETER_PPBOX_EV + i], m_eSelectControl == eC_PPBOX_EV + i || m_eSelectTab == eC_PPBOX_EV + i, m_eMouseOver == eC_PPBOX_EV + i, 11);
	}
}



void CPositionParameterWnd::OnMouseMove(UINT nFlags, CPoint point) {
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



void CPositionParameterWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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



void CPositionParameterWnd::OnLButtonUp(UINT nFlags, CPoint point)  {	
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
            case eC_PPBOX_EV:
			case eC_GLASS_LOAD_TRANSFER: 
			{
				fn_SetView(m_eSelectControl);
				break;
			}
			default: break;
		}
	}
	m_eSelectControl	=	eC_NONE;
	Invalidate(FALSE);
	//if (!strALogMsg.IsEmpty() && !strALogPos.IsEmpty())
	//	WRITE_LOG(eLOG_BUTTON, TapString(strALogMsg, strALogPos, theApp.m_UserManager.GetCurrentUserData().szID));
	CFrameWndEx::OnLButtonUp(nFlags, point);
}



void CPositionParameterWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect		rect = m_rectS[eS_FULL_AREA];
	rect.DeflateRect(5, 5, 5, 5);
	double dH = (rect.Height() - 5.) / 15.;
	double dW = (rect.Width() - 5.) / 20.;

	m_rectS[eS_DEVICE_LIST		] = CRect(rect.left + dW*0 +5, rect.top + dH*0 +5, rect.left + dW*20, rect.top+dH*1+5);
	m_rectS[eS_WORK_AREA		] = CRect(rect.left + dW*0 +5,	rect.top + dH*1,rect.left + dW*20, rect.top+dH*15);

	rect = m_rectS[eS_DEVICE_LIST];
	dH = (rect.Height() - 5.) / 1.;
	dW = (rect.Width() - 5.) / 4.;
    m_rectC[eC_PPBOX_EV]             = CRect(rect.left + dW * 0, rect.top + dH * 0 + 5, rect.left + dW * 1, rect.top + dH * 1);
	m_rectC[eC_GLASS_LOAD_TRANSFER]  = CRect(rect.left + dW * 1, rect.top + dH * 0 + 5, rect.left + dW * 2, rect.top + dH * 1);
}



void CPositionParameterWnd::fn_SetView(int eSelectedTab) {
	if (eSelectedTab < eC_PPBOX_EV || eSelectedTab > eC_GLASS_LOAD_TRANSFER)
		return;

	HWND	hwndWork = NULL;
	switch (eSelectedTab) {
        case eC_PPBOX_EV:                   hwndWork = m_pwndPPBoxEV->m_hWnd; break;
		case eC_GLASS_LOAD_TRANSFER	:		hwndWork = m_pwndGlassLoadTrasnfer->m_hWnd; break;
	}

	if (hwndWork) {
		CRect rect = m_rectS[eS_WORK_AREA];
		rect.DeflateRect(1, 1);
		::MoveWindow(hwndWork, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

		if (hwndWork)
			::ShowWindow(hwndWork, SW_SHOW);
		if (m_hwndLast != hwndWork)
			::ShowWindow(m_hwndLast, SW_HIDE);

        if (hwndWork)
		    m_hwndLast = hwndWork;

		m_eSelectTab= eSelectedTab;
	}

}
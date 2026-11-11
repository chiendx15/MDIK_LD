#include "pch.h"
#include "UI.h"
#include "./DeviceParameterWnd.h"
#include "DeviceParameter/DeviceParameterDefaultWnd.h"
#define ID_PARAMETER_LIST		1000
#define ID_PARAMETER_LIST_EDIT	1001


CString strTab[] = {
	g_st[IDS_DEVICE_PARAM_BASIC_PARAMETER]
};
IMPLEMENT_DYNCREATE		(CDeviceParameterWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CDeviceParameterWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()



CDeviceParameterWnd::CDeviceParameterWnd(CWnd* pwndParent) {
	m_pwndParent				= pwndParent;
	m_eSelectControl			= eC_NONE;
	m_eMouseOver				= eC_NONE;
	m_bEdit						= FALSE;
	m_eSelectTab				= eC_DEFAULT;
	m_pwndDefault				= nullptr;

}

CDeviceParameterWnd::~CDeviceParameterWnd() {
}



BOOL CDeviceParameterWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}






int CDeviceParameterWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
try {
		m_pwndDefault = new CDeviceParameterDefaultWnd(this);
		if (!m_pwndDefault->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;

}
catch (...) {
	return -1;
}

	return 0;
}



void CDeviceParameterWnd::OnTimer(UINT_PTR nIDEvent) {
	//switch (nIDEvent) {
	//	default:
	//		break;
	//}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CDeviceParameterWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height() , SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		ShowTabWindow(m_eSelectTab);
	}
	else {
	}
}


BOOL CDeviceParameterWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CDeviceParameterWnd::OnPaint() 
{
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CRect rect = m_rectS[eS_FULL_AREA];
	DrawLineRect(memDC, rect, RGB_WHITE);
	rect.DeflateRect(1, 1);
	memDC.FillSolidRect(rect, RgbColor::Gainsboro);
			
	DrawMDIMenuButton(memDC, m_rectC[eC_DEFAULT], g_st[IDS_DEVOCE_PARAMETER_DEFAULT], m_eSelectControl == eC_DEFAULT || m_eSelectTab == eC_DEFAULT, m_eMouseOver == eC_DEFAULT, 11);
}

 // LJY 추가 2024.06.23
void CDeviceParameterWnd::ChangeLanguage()
{
	Invalidate(FALSE);
}

void CDeviceParameterWnd::OnMouseMove(UINT nFlags, CPoint point) {
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



void CDeviceParameterWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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



void CDeviceParameterWnd::OnLButtonUp(UINT nFlags, CPoint point)  {	
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_DEFAULT:
		
			{
				m_eSelectTab = m_eSelectControl;
				ShowTabWindow(m_eSelectTab);
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



void CDeviceParameterWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
	CRect		rc = m_rectS[eS_FULL_AREA];
	rc.DeflateRect(5, 5, 5, 5);
	double dH = (rc.Height() - 5.) / 15.;
	double dW = (rc.Width() - 5.) / 2.;
	m_rectS[eS_TAB				] = CRect(rc.left+dW*0+5,	rc.top+dH*0+5, rc.left+dW*2, rc.top+dH*1+5);
	m_rectS[eS_DEVICE_PARAMETER	] = CRect(rc.left+dW*0+5,	rc.top+dH*1, rc.left+dW*2, rc.top+dH*15);
	rc = m_rectS[eS_TAB];
	dH = (rc.Height() - 5.) / 1.;
	dW = (rc.Width() - 5.) / countof(strTab);
	m_rectC[eC_DEFAULT	] = CRect(rc.left+dW*0, rc.top+dH*0+5, rc.left+dW*1, rc.top+dH*1);

	if (m_pwndDefault)
		m_pwndDefault->MoveWindow(m_rectS[eS_DEVICE_PARAMETER]);

}
void CDeviceParameterWnd::ShowTabWindow(int iTab) {
	m_pwndDefault->ShowWindow(SW_HIDE);
	switch (iTab) {
		case eC_DEFAULT: {
			m_pwndDefault->ShowWindow(SW_SHOW);
			break;
		}
	
	}
	Invalidate(FALSE);
}
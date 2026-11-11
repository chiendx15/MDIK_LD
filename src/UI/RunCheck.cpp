#include "pch.h"
#include "UI.h"
#include "./RunCheck.h"
//#include "../../MainFrame.h"



IMPLEMENT_DYNCREATE		(CRunCheck, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CRunCheck, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()



CRunCheck::CRunCheck(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
}

CRunCheck::~CRunCheck() {
}



BOOL CRunCheck::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}






int CRunCheck::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {
		
	}
	catch (...) {
		return -1;
	}

	return 0;
}


void CRunCheck::OnKillFocus(CWnd* pNewWnd) {
	CFrameWndEx::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
	Invalidate();
}



void CRunCheck::OnTimer(UINT_PTR nIDEvent) {
	//switch (nIDEvent) {
	//	default:
	//		break;
	//}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CRunCheck::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width()/2., g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		Invalidate();
	}
	else {
	}
}


BOOL CRunCheck::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CRunCheck::OnPaint() {
	CPaintDC			dc(this);
try {
	CMemDCEx::CMemDC		memDC(&dc);
	CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_NORMAL]);
	CBrush* pOldBrush = (CBrush*)memDC.SelectObject(&g_brushBackground[eEC_NORMAL]);
	CPen* pOldPen = (CPen*)memDC.SelectObject(&pen);
	CRect					rect = m_rectS[eS_FULL_AREA];
	memDC.SelectObject(&g_brushForeground[eEC_DEFAULT]);
	memDC.FillSolidRect(rect, g_colorBackground[eEC_DEFAULT]);
	rect.DeflateRect(3, 3, 3, 3);
	memDC.SelectObject(&g_brushForeground[eEC_DEFAULT]);
	CDrawSymbol::DrawLineRect(&memDC, rect, g_colorForeground[eEC_DEFAULT]);

	
	DrawButtonUpDown(&memDC, m_rectC[eC_CLOSE], _T("Close"), m_eSelectControl == eC_CLOSE);
	
	memDC.SelectObject(pOldBrush);
	memDC.SelectObject(pOldPen);
}
catch (...) {
}
}



void CRunCheck::OnMouseMove(UINT nFlags, CPoint point) {
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



void CRunCheck::OnLButtonDown(UINT nFlags, CPoint point) {
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



void CRunCheck::OnLButtonUp(UINT nFlags, CPoint point)  {	
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_CLOSE: {
				ShowWindow(SW_HIDE);
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



void CRunCheck::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect	rc = m_rectS[eS_FULL_AREA];
	double dH = (rc.Height() - 5.) / 15.;
	double dW = (rc.Width() - 5.) / 4.;
	m_rectC[eC_CLOSE] = CRect(rc.left + dW * 3.5 + 5 , rc.top + dH * 14 + 5 , rc.left + dW * 4, rc.top + dH  * 15);
}

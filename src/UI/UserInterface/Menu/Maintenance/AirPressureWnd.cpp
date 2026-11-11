#include "pch.h"
#include "../../../UI.h"
#include "./AirPressureWnd.h"


CVacuumView::CVacuumView(int iVac, int iBlow, CString strTitle) {
	m_iVac	= iVac;
	m_iBlow = iBlow;
	m_ptSize = Size(100, 100);
	m_strTitle = strTitle;
}
CVacuumView::~CVacuumView() {

}
void CVacuumView::ComputArea() {
	int	iIndex = m_iVac + m_iBlow + 1;
	CRect rc = m_rectS[eS_FULL_AREA];
	double dW = (rc.Width()-5) / 2;
	double dH = (rc.Height()-5)/ iIndex;

	m_rectS[eS_TITLE]	= CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*2, rc.top+dH*1);
	int iCount = 0;
	for (int i = 1; i < iIndex; i++) {
		m_rectS[eS_VAC_BLOW + iCount]	= CRect(rc.left+5+dW*0, rc.top+5+dH*i, rc.left+dW*1, rc.top+dH*(i+1));
		m_rectC[eC_VAC_BLOW + iCount]	= CRect(rc.left+5+dW*1, rc.top+5+dH*i, rc.left+dW*2, rc.top+dH*(i+1));
		double dTemp = 0.;
		m_vecValue.push_back(dTemp);
		iCount++;
	}
	
}
void CVacuumView::Draw(CDC* pDC) {
	
	Draw3dGradationButton(pDC, m_rectS[eS_TITLE], m_strTitle, FALSE, 9, 0, RGB_WHITE, RGB_GRAY_DARK, RGB_GRAY_LIGHT);
	for (int i = 0; i < m_iVac + m_iBlow; i++) {
		CString str;
		if (i < m_iVac)
			str = Format(_T("Vac%d"), i + 1);
		else 
			str = Format(_T("Blow%d"), i - m_iVac + 1);
		Draw3dGradationButton(pDC, m_rectS[eS_VAC_BLOW + i], str, FALSE, 9, 0, RGB_WHITE, RGB_GRAY_DARK, RGB_GRAY_LIGHT);
		SelectedDrawDouble(pDC, m_rectC[eC_VAC_BLOW + i], m_vecValue[i]);
	}
}


IMPLEMENT_DYNCREATE		(CAirPressureWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CAirPressureWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()



CAirPressureWnd::CAirPressureWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
}
CAirPressureWnd::~CAirPressureWnd() {
}



BOOL CAirPressureWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}






int CAirPressureWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {
		for (int i = 0; i < eAM_END; i++) {
			
			m_AirMonitor[i].ComputArea();
		}

	}
	catch (...) {
		return -1;
	}

	return 0;
}



void CAirPressureWnd::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case INTERVAL_AIR: {
			
			for (int i = 0; i < eAM_END; i++) {
				m_AirMonitor[i].SetAll(i, i, i);
			}
			Invalidate(FALSE);
			break;
		}
		default:
			break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CAirPressureWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		Invalidate();
		SetTimer(INTERVAL_AIR, 100, nullptr);
	}
	else {
		KillTimer(INTERVAL_AIR);
	}
}


BOOL CAirPressureWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CAirPressureWnd::OnPaint() {
	CPaintDC			dc(this);
try {
	CMemDCEx::CMemDC		memDC(&dc);
	CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_NORMAL]);
	CBrush* pOldBrush = (CBrush*)memDC.SelectObject(&g_brushBackground[eEC_NORMAL]);
	CPen* pOldPen = (CPen*)memDC.SelectObject(&pen);
	CRect					rect = m_rectS[eS_FULL_AREA];
	memDC.SelectObject(&g_brushBackground[eEC_NORMAL]);
	memDC.FillSolidRect(rect, g_colorBackground[eEC_DISABLE]);
	rect.DeflateRect(3, 3, 0, 0);
	memDC.SelectObject(&g_brushBackground[eEC_DEFAULT]);
	CDrawSymbol::DrawLineRect(&memDC, rect, g_colorForeground[eEC_DEFAULT]);
	SelectedDrawText(&memDC, m_rectS[eS_TITLE_AREA], _T("Air/Vacum Status"), 9, RGB_GRAY, RGB_GRAY, RGB_WHITE);
	for (int i = 0; i < eAM_END; i++) {
		m_AirMonitor[i].Draw(memDC);
	}
	DrawLineRect(&memDC, m_rectS[eS_IMG_1]);
	DrawLineRect(&memDC, m_rectS[eS_IMG_2]);
	DrawLineRect(&memDC, m_rectS[eS_IMG_3]);
	DrawLineRect(&memDC, m_rectS[eS_IMG_4]);
	memDC.SelectObject(pOldBrush);
	memDC.SelectObject(pOldPen);
}
catch (...) {
}
}



void CAirPressureWnd::OnMouseMove(UINT nFlags, CPoint point) {
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



void CAirPressureWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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



void CAirPressureWnd::OnLButtonUp(UINT nFlags, CPoint point)  {
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_OK: {				
				break;
			}
			case eC_CANCEL: {
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



void CAirPressureWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect		rc = m_rectS[eS_FULL_AREA];

	rc.DeflateRect(1, 1);
	double dH = (rc.Height() - 5) / 15.;
	double dW = (rc.Width() - 5) / 2.;
	m_rectS[eS_TITLE_AREA	]	= CRect(rc.left + 5 + dW * 0, rc.top + 5 +	dH * 0, rc.left + dW * 2, rc.top + dH * 1);
	m_rectS[eS_WORK_AREA	]	= CRect(rc.left + 5 + dW * 0, rc.top + 10 + dH * 1, rc.left + dW * 2, rc.top + dH * 15);

	rc = m_rectS[eS_WORK_AREA];
	dH = (rc.Height() - 5) / 4.;
	dW = (rc.Width() - 5) / 7.;
	m_rectS[eS_IMG_1	]	= CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 2, rc.top + dH * 2);
	m_rectS[eS_IMG_2	]	= CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 2, rc.left + dW * 2, rc.top + dH * 4);
	m_rectS[eS_IMG_3	]	= CRect(rc.left + 5 + dW * 3, rc.top + 5 + dH * 0, rc.left + dW * 5, rc.top + dH * 2);
	m_rectS[eS_IMG_4	]	= CRect(rc.left + 5 + dW * 3, rc.top + 5 + dH * 2, rc.left + dW * 5, rc.top + dH * 4);

	m_AirMonitor[eAM_1].SetVacBlow(3, 0);
	m_AirMonitor[eAM_1].SetTitle(_T("Robot"));
	m_AirMonitor[eAM_1].SetSize(Size(dW, dH));
	m_AirMonitor[eAM_1].SetStart(CPoint(rc.left + dW * 2, rc.top + dH * 0));
	
	m_AirMonitor[eAM_2].SetVacBlow(2, 2);
	m_AirMonitor[eAM_2].SetTitle(_T("Align"));
	m_AirMonitor[eAM_2].SetSize(Size(dW, dH));
	m_AirMonitor[eAM_2].SetStart(CPoint(rc.left + dW * 2, rc.top + dH * 2));

	m_AirMonitor[eAM_3].SetVacBlow(2, 1);
	m_AirMonitor[eAM_3].SetTitle(_T("TR1"));
	m_AirMonitor[eAM_3].SetSize(Size(dW, dH));
	m_AirMonitor[eAM_3].SetStart(CPoint(rc.left + dW * 5, rc.top + dH * 0));

	m_AirMonitor[eAM_4].SetVacBlow(2, 1);
	m_AirMonitor[eAM_4].SetTitle(_T("SCT1 TABLE"));
	m_AirMonitor[eAM_4].SetSize(Size(dW, dH));
	m_AirMonitor[eAM_4].SetStart(CPoint(rc.left + dW * 5, rc.top + dH * 1));

	m_AirMonitor[eAM_5].SetVacBlow(2, 1);
	m_AirMonitor[eAM_5].SetTitle(_T("TR2"));
	m_AirMonitor[eAM_5].SetSize(Size(dW, dH));
	m_AirMonitor[eAM_5].SetStart(CPoint(rc.left + dW * 6, rc.top + dH * 0));

	m_AirMonitor[eAM_6].SetVacBlow(2, 1);
	m_AirMonitor[eAM_6].SetTitle(_T("SCT2 TABLE"));
	m_AirMonitor[eAM_6].SetSize(Size(dW, dH));
	m_AirMonitor[eAM_6].SetStart(CPoint(rc.left + dW * 6, rc.top + dH * 1));

	m_AirMonitor[eAM_7].SetVacBlow(4, 1);
	m_AirMonitor[eAM_7].SetTitle(_T("TR3"));
	m_AirMonitor[eAM_7].SetSize(Size(dW, dH * 2));
	m_AirMonitor[eAM_7].SetStart(CPoint(rc.left + dW * 5, rc.top + dH * 2));
	
	//m_rectS[eS_MESSAGE	] = CRect(rect.left+5	,rect.top+rect.Height()/2-80,rect.right-5	,rect.top+rect.Height()/2);
	//m_rectC[eC_OK		] = CRect(nWidth-105	,rect.top+rect.Height()/2+10,nWidth-5		,rect.top+rect.Height()/2+50);
	//m_rectC[eC_CANCEL	] = CRect(nWidth+5		,rect.top+rect.Height()/2+10,nWidth+105		,rect.top+rect.Height()/2+50);
}

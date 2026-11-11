#include "pch.h"
#include "UI.h"
#include "./DrawControlTestWnd.h"
//#include "../../MainFrame.h"



IMPLEMENT_DYNCREATE		(CDrawControlTestWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CDrawControlTestWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()



CDrawControlTestWnd::CDrawControlTestWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
}

CDrawControlTestWnd::~CDrawControlTestWnd() {
}



BOOL CDrawControlTestWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}






int CDrawControlTestWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {
		
	}
	catch (...) {
		return -1;
	}

	return 0;
}


void CDrawControlTestWnd::OnKillFocus(CWnd* pNewWnd) {
	CFrameWndEx::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
	Invalidate();
}



void CDrawControlTestWnd::OnTimer(UINT_PTR nIDEvent) {
	//switch (nIDEvent) {
	//	default:
	//		break;
	//}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CDrawControlTestWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		Invalidate();
	}
	else {
	}
}


BOOL CDrawControlTestWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CDrawControlTestWnd::OnPaint() {
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

	//for (int i = 0; i < 40; i++) {
	//	DrawLineRect(&memDC, m_rectS[eS_DRAW_1 + i]);
	//}
	DrawText(memDC, m_rectS[eS_DRAW_1], _T("DrawLineRect"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawLineRect(&memDC, m_rectC[eC_DRAW_1], RGB_GRAY_DARK);

	DrawText(memDC, m_rectS[eS_DRAW_2], _T("DrawFitRect"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawFitRect(&memDC, m_rectC[eC_DRAW_2], RGB_GRAY_DARK);

	DrawText(memDC, m_rectS[eS_DRAW_3], _T("DrawRoundRect"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	//DrawRoundRect(&memDC, m_rectC[eC_DRAW_3], RGB_GRAY_DARK, CPoint(20,20));

	DrawText(memDC, m_rectS[eS_DRAW_4], _T("DrawLine"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawLine(&memDC, m_rectC[eC_DRAW_4].TopLeft(), m_rectC[eC_DRAW_4].BottomRight(), RGB_GRAY_DARK, 5);

	DrawText(memDC, m_rectS[eS_DRAW_5], _T("DrawGroupBox"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawGroupBox(&memDC, m_rectC[eC_DRAW_5], _T("Group"), CSize(-1,-1), CPoint(15,-12), 9, RGB_BLACK, RGB_BLACK);

	DrawText(memDC, m_rectS[eS_DRAW_6], _T("DrawRoundGroupBox"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawRoundGroupBox(&memDC, m_rectC[eC_DRAW_6], _T("RGruop"), CSize(100, 25), CPoint(15, -12), CPoint(10,10), 9, RGB_BLACK, RGB_BLACK);

	DrawText(memDC, m_rectS[eS_DRAW_7], _T("DrawText"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectC[eC_DRAW_7], _T("DrawText"), 15, RGB_BLACK, DT_CENTER | DT_VCENTER);
	
	DrawText(memDC, m_rectS[eS_DRAW_8], _T("SelectedDrawText"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	SelectedDrawText(&memDC, m_rectC[eC_DRAW_8], _T("SelectedDraw"), 9, RGB_BLACK, RGB_WHITE, RGB_RED, DT_CENTER | DT_VCENTER);
	
	DrawText(memDC, m_rectS[eS_DRAW_9], _T("SelectedDrawEditText"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	SelectedDrawEditText(&memDC, m_rectC[eC_DRAW_9], _T("SelectedEditDraw"));

	DrawText(memDC, m_rectS[eS_DRAW_10], _T("DrawEditBrowse"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawEditBrowse(&memDC, m_rectC[eC_DRAW_10], _T("EditBrowse"));
	
	DrawText(memDC, m_rectS[eS_DRAW_11], _T("DrawButtonUpDown"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawButtonUpDown(&memDC, m_rectC[eC_DRAW_11], _T("DrawButtonUpDown"),m_eSelectControl == eC_DRAW_11);

	DrawText(memDC, m_rectS[eS_DRAW_12], _T("DrawToggleButton"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawToggleButton(&memDC, m_rectC[eC_DRAW_12], m_eSelectControl == eC_DRAW_12, _T("ToggleOn"), _T("ToggleOff"), 9, RGB_BLACK, RGB_WHITE, RGB_WHITE, RGB_BLACK, RGB_WHITE, RGB_BLACK);

	DrawText(memDC, m_rectS[eS_DRAW_13], _T("DrawRoundButtonUpDown"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawRoundButtonUpDown(&memDC, m_rectC[eC_DRAW_13], _T("DrawRoundButtonUpDown"), m_eSelectControl == eC_DRAW_13, CPoint(20,20));

	DrawText(memDC, m_rectS[eS_DRAW_14], _T("DrawRoundButtonUpDown"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawRoundButtonUpDown(&memDC, m_rectC[eC_DRAW_14], _T("DrawRoundButtonUpDown"), m_eSelectControl == eC_DRAW_14, CPoint(50, 50));

	DrawText(memDC, m_rectS[eS_DRAW_15], _T("Draw3dButton"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	Draw3dButton(&memDC, m_rectC[eC_DRAW_15], _T("Draw3dButton"), m_eSelectControl == eC_DRAW_15);

	DrawText(memDC, m_rectS[eS_DRAW_16], _T("DrawSensor"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawSensor(&memDC, m_rectC[eC_DRAW_16], _T("DrawSensor"), m_eSelectControl == eC_DRAW_16, RGB_GREEN, RGB_RED);

	DrawText(memDC, m_rectS[eS_DRAW_17], _T("DrawRoundSensor"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawRoundSensor(&memDC, m_rectC[eC_DRAW_17], _T("DrawRoundSensor"), m_eSelectControl == eC_DRAW_17, CPoint(10,10), RGB_GREEN, RGB_RED);

	DrawText(memDC, m_rectS[eS_DRAW_18], _T("DrawEllipseSensor"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawEllipseSensor(&memDC, m_rectC[eC_DRAW_18], _T("DrawEllipseSensor"), m_eSelectControl == eC_DRAW_18, CPoint(10, 10), RGB_GREEN, RGB_RED);

	DrawText(memDC, m_rectS[eS_DRAW_19], _T("DrawMoveDirect"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawMoveDirect(&memDC, m_rectC[eC_DRAW_19], m_eSelectControl == eC_DRAW_19, eMD_UP, _T("Move \nDirect Up"));
	
	DrawText(memDC, m_rectS[eS_DRAW_20], _T("Draw3dMoveDirect"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	Draw3dMoveDirect(&memDC, m_rectC[eC_DRAW_20], m_eSelectControl == eC_DRAW_20, eMD_UP, _T("3d Move \nDirect Up"));

	DrawText(memDC, m_rectS[eS_DRAW_21], _T("Draw3dGradationButton"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	Draw3dGradationButton(&memDC, m_rectC[eC_DRAW_21], _T("Draw3dGradationButton H"), m_eSelectControl == eC_DRAW_21, 9, eGO_H, RGB_WHITE, RGB_RED, RGB_YELLOW);

	DrawText(memDC, m_rectS[eS_DRAW_22], _T("Draw3dGradationButton"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	Draw3dGradationButton(&memDC, m_rectC[eC_DRAW_22], _T("Draw3dGradationButton V"), m_eSelectControl == eC_DRAW_22, 9, eGO_V, RGB_WHITE, RGB_YELLOW , RGB_BLACK);
	
	DrawText(memDC, m_rectS[eS_DRAW_23], _T("DrawButtonUpDownMouseMove"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawButtonUpDownMouseMove(&memDC, m_rectC[eC_DRAW_23], _T("DrawButtonUpDownMouseMove"), eC_DRAW_23, m_eMouseOver, m_eSelectControl);

	DrawText(memDC, m_rectS[eS_DRAW_24], _T("Draw3dGradationMouseMoveButton"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	Draw3dGradationMouseMoveButton(&memDC, m_rectC[eC_DRAW_24], _T("Draw3dGradationMouseMoveButton H"), eC_DRAW_24, m_eMouseOver, m_eSelectControl,9, eGO_H, RGB_WHITE,
		RGB_YELLOW, RGB_RED, RGB_PINK, RGB_GRAY, RGB_GREEN, RGB_BLUE);

	DrawText(memDC, m_rectS[eS_DRAW_25], _T("Draw3dGradationMouseMoveButton"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	Draw3dGradationMouseMoveButton(&memDC, m_rectC[eC_DRAW_25], _T("Draw3dGradationMouseMoveButton V"), eC_DRAW_25, m_eMouseOver, m_eSelectControl, 9, eGO_V, RGB_WHITE,
		RGB_YELLOW, RGB_RED, RGB_PINK, RGB_GRAY, RGB_GREEN, RGB_BLUE);

	DrawText(memDC, m_rectS[eS_DRAW_26], _T("DrawRoundButtonUpDownSensor"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawRoundButtonUpDownSensor(&memDC, m_rectC[eC_DRAW_26], _T("DrawRoundButtonUpDownSensor\nOn"), m_eSelectControl == eC_DRAW_26, TRUE);
	
	DrawText(memDC, m_rectS[eS_DRAW_27], _T("DrawRoundButtonUpDownSensor"), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawRoundButtonUpDownSensor(&memDC, m_rectC[eC_DRAW_27], _T("DrawRoundButtonUpDownSensor\nOff"), m_eSelectControl == eS_DRAW_27, FALSE);
	
	DrawButtonUpDown(&memDC, m_rectC[eC_CLOSE], _T("Close"), m_eSelectControl == eC_CLOSE);
	
	memDC.SelectObject(pOldBrush);
	memDC.SelectObject(pOldPen);
}
catch (...) {
}
}



void CDrawControlTestWnd::OnMouseMove(UINT nFlags, CPoint point) {
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



void CDrawControlTestWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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



void CDrawControlTestWnd::OnLButtonUp(UINT nFlags, CPoint point)  {	
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



void CDrawControlTestWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect	rc = m_rectS[eS_FULL_AREA];
	double dH = (rc.Height() - 5.) / 15.;
	double dW = (rc.Width() - 5.) / 4.;
	for (int i = 0; i < 14; i++) {
		m_rectS[eS_DRAW_1 +i]	= CRect(rc.left + dW * 0 + 5 , rc.top + dH * i + 5 , rc.left + dW * 1, rc.top + dH  * (i+1));
		m_rectC[eC_DRAW_1 +i]	= CRect(rc.left + dW * 1 + 5 , rc.top + dH * i + 5 , rc.left + dW * 2, rc.top + dH  * (i+1));
		m_rectS[eS_DRAW_15 +i]	= CRect(rc.left + dW * 2 + 5 , rc.top + dH * i + 5 , rc.left + dW * 3, rc.top + dH  * (i+1));
		m_rectC[eC_DRAW_15 +i]	= CRect(rc.left + dW * 3 + 5 , rc.top + dH * i + 5 , rc.left + dW * 4, rc.top + dH  * (i+1));
	}
	m_rectC[eC_CLOSE] = CRect(rc.left + dW * 3.5 + 5 , rc.top + dH * 14 + 5 , rc.left + dW * 4, rc.top + dH  * 15);
}

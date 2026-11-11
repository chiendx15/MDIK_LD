#include "pch.h"
#include "UI.h"
#include "AGVPIOStatusWnd.h"



IMPLEMENT_DYNCREATE		(CAGVPIOStatusWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CAGVPIOStatusWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()

CAGVPIOStatusWnd::CAGVPIOStatusWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
}
CAGVPIOStatusWnd::~CAGVPIOStatusWnd() {
}

BOOL CAGVPIOStatusWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CAGVPIOStatusWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}



void CAGVPIOStatusWnd::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case TIMER_REDRAW: {
			Invalidate(FALSE);
			break;
		}
		default:
			break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CAGVPIOStatusWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		Invalidate();
		SetTimer(TIMER_REDRAW, 100, NULL);
	}
	else {
		KillTimer(TIMER_REDRAW);
	}
}


BOOL CAGVPIOStatusWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}	

void CAGVPIOStatusWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CRect				rect = m_rectS[eS_FULL_AREA];
	memDC.FillSolidRect(rect, g_colorBackground[eEC_DISABLE]);
	rect.DeflateRect(3, 3, 0, 0);
	CDrawSymbol::DrawLineRect(&memDC, rect, g_colorForeground[eEC_DEFAULT]);

	DrawLineRect(memDC, m_rectS[eS_BF_AREA]);
	DrawLineRect(memDC, m_rectS[eS_EQP_AREA]);

	SelectedDrawText(memDC, m_rectS[eS_BF_TITLE], _T("Buffer Lift"), DT_FONTSIZE, DT_PEN, RGB_BLUE, RGB_WHITE);
	SelectedDrawText(memDC, m_rectS[eS_EQP_TITLE], _T("EQP"), DT_FONTSIZE, DT_PEN, RGB_BLUE, RGB_WHITE);


	BOOL bBF_PIO[] = {
		GetPIO_BF_CONNECT(TRUE),
		GetPIO_BF_REQUEST(TRUE),
		GetPIO_BF_RUN(TRUE),
		GetPIO_BF_ERROR(TRUE),
		GetPIO_BF_INPUT(TRUE),
		GetPIO_BF_OUTPUT(TRUE),
		GetPIO_BF_LOAD(TRUE),
		GetPIO_BF_UNLOAD(TRUE),
	};

	BOOL bEQ_PIO[] = {
		GetPIO_EQ_CONNECT(TRUE),
		GetPIO_EQ_REQUEST(TRUE),
		GetPIO_EQ_RUN(TRUE),
		GetPIO_EQ_ERROR(TRUE),
		GetPIO_EQ_INPUT(TRUE),
		GetPIO_EQ_OUTPUT(TRUE),
		GetPIO_EQ_LOAD_REQUEST(TRUE),
		GetPIO_EQ_UNLOAD_REQUEST(TRUE),
	};

	int nItem = _min(countof(bBF_PIO), countof(bEQ_PIO));
	for (int i = 0; i < nItem; i++) {
		DrawEllipseSensor(memDC, m_rectC[eC_BF_PIO_1 + i], Format(_T("[BIT %d] %s"), i + 1, g_st[IDS_PIO_STATUS_WND_BF_PIO_1 + i]), bBF_PIO[i], CPoint(10, 10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, RGB_BLACK, RGB_GRAY_LIGHT, DT_LEFT | DT_VCENTER);
		DrawEllipseSensor(memDC, m_rectC[eC_EQ_PIO_1 + i], Format(_T("[BIT %d] %s"), i + 1, g_st[IDS_PIO_STATUS_WND_EQ_PIO_1 + i]), bEQ_PIO[i], CPoint(10, 10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, RGB_BLACK, RGB_GRAY_LIGHT, DT_LEFT | DT_VCENTER);

	}
}

void CAGVPIOStatusWnd::OnMouseMove(UINT nFlags, CPoint point) {
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



void CAGVPIOStatusWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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



void CAGVPIOStatusWnd::OnLButtonUp(UINT nFlags, CPoint point)  {
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
        switch (m_eSelectControl) {
            // 아래 내용 테스트 후 추후 주석 처리 해야됨.
            case eC_EQ_PIO_1: {
				SetPIO_EQ_CONNECT(!GetPIO_EQ_CONNECT(TRUE));
                break;
            }
            case eC_EQ_PIO_2: {
				SetPIO_EQ_REQUEST(!GetPIO_EQ_REQUEST(TRUE));
                break;
            }
            case eC_EQ_PIO_3: {
				SetPIO_EQ_RUN(!GetPIO_EQ_RUN(TRUE));
                break;
            }
            case eC_EQ_PIO_4: {
				SetPIO_EQ_ERROR(!GetPIO_EQ_ERROR(TRUE));
                break;
            }
            case eC_EQ_PIO_5: {
				SetPIO_EQ_INPUT(!GetPIO_EQ_INPUT(TRUE));
                break;
            }
            case eC_EQ_PIO_6: {
				SetPIO_EQ_OUTPUT(!GetPIO_EQ_OUTPUT(TRUE));
                break;
            }
            case eC_EQ_PIO_7: {
				SetPIO_EQ_LOAD_REQUEST(!GetPIO_EQ_LOAD_REQUEST(TRUE));
                break;
            }
            case eC_EQ_PIO_8: {
				SetPIO_EQ_UNLOAD_REQUEST(!GetPIO_EQ_UNLOAD_REQUEST(TRUE));
                break;
            }
        }
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl	=	eC_NONE;
	fn_ComputeArea();
	Invalidate(FALSE);

	CFrameWndEx::OnLButtonUp(nFlags, point);
}



void CAGVPIOStatusWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect	rc = m_rectS[eS_FULL_AREA];
	double	dW = (rc.Width() - 5.) / 3.;
	double	dH = (rc.Height() - 5.) /15.;

	m_rectS[eS_BF_AREA] = CRect(rc.left + dW * 0 + 5 + dH, rc.top + dH * 1 + 5, rc.left + dW * 1 + dH, rc.top + dH * 10);
    m_rectS[eS_EQP_AREA] = CRect(rc.left + dW * 1 + 5 + dH, rc.top + dH * 1 + 5, rc.left + dW * 2 + dH, rc.top + dH * 10);

	rc = m_rectS[eS_BF_AREA];
	dW = (rc.Width() - 5.) / 1;
	dH = (rc.Height() - 5.) / 9.;

	m_rectS[eS_BF_TITLE]		= CRect(rc.left + 0, rc.top + dH * 0 + 0, rc.left + dW * 1 + 5, rc.top + dH * 1);

	for (int i = 0; i <= eC_BF_PIO_8 - eC_BF_PIO_1; i++) {
		m_rectC[eC_BF_PIO_1 + i]		= CRect(rc.left + 5, rc.top + dH * (i + 1) + 5, rc.left + dW * 1 + 0, rc.top + dH * (i + 2));
	}

	rc = m_rectS[eS_EQP_AREA];
	dW = (rc.Width() - 5.) / 1;
    dH = (rc.Height() - 5.) / 9.;

	
	m_rectS[eS_EQP_TITLE]		= CRect(rc.left + 0, rc.top + dH * 0 + 0, rc.left + dW * 1 + 5, rc.top + dH * 1);
	for (int i = 0; i <= eC_BF_PIO_8 - eC_BF_PIO_1; i++) {
		m_rectC[eC_EQ_PIO_1 + i] = CRect(rc.left + 5, rc.top + dH * (i + 1) + 5, rc.left + dW * 1 + 0, rc.top + dH * (i + 2));
	}
}
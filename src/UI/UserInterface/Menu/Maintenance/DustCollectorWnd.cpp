#include "pch.h"
#include "UI.h"
#include "DustCollectorWnd.h"

#define ID_DUST_COLLECTOR_LIFESPAN   1003

CDustCollectorWnd::CDustCollectorWnd(CWnd* pwndParent) {
	m_iDustSetLimit = 0;
	m_iDustPressLimit[0] = 0;
	m_iDustPressLimit[1] = 0;
	SyncDataDustCollector(FALSE);
}

CDustCollectorWnd::~CDustCollectorWnd() {
}


IMPLEMENT_DYNCREATE		(CDustCollectorWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CDustCollectorWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()

BOOL CDustCollectorWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CDustCollectorWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CDustCollectorWnd::OnKillFocus(CWnd* pNewWnd) {
	CFrameWndEx::OnKillFocus(pNewWnd);
	Invalidate();
}



void CDustCollectorWnd::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case ID_DUST_COLLECTOR_LIFESPAN: {
			Invalidate(FALSE);
			break;
		}
		default:
			break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}

void CDustCollectorWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		Invalidate(FALSE);
		m_iDustSetLimit = sm.Option->tDustColletorOption.iFilterCycleLimit;
		m_iDustPressLimit[0] = sm.Option->tDustColletorOption.iLightAlarmLimit;
		m_iDustPressLimit[1] = sm.Option->tDustColletorOption.iHeavyAlarmLimit;
		SetTimer(ID_DUST_COLLECTOR_LIFESPAN, 100, nullptr);
	}
	else {
		KillTimer(ID_DUST_COLLECTOR_LIFESPAN);
	}
}

BOOL CDustCollectorWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}

void CDustCollectorWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
    CRect	rc = m_rectS[eS_FULL_AREA];
	double dW = (rc.Width() - 5.) / 15.;
	double dH = (rc.Height() - 5.) / 19;

	m_rectS[eS_DUST_AREA] = CRect(rc.left+5+dW*1, rc.top+5+dH*1,  rc.left+dW*7,  rc.top+dH*13);

	    rc = m_rectS[eS_DUST_AREA];
	    dH = (rc.Height() - 5.) / 8.;
	    dW = (rc.Width() - 5.) / 5.;
	    m_rectS[eS_TEXT_DUST_PRESS]				= CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*2, rc.top+dH*2);	// 압력 문구
	    m_rectS[eS_DUST_CURRENT_PRESS]			= CRect(rc.left+5+dW*2, rc.top+5+dH*0, rc.left+dW*4, rc.top+dH*2);	// 현재 압력

		m_rectS[eS_TEXT_DUST_LIGHT_ALARM_LIMIT]	= CRect(rc.left+5+dW*0, rc.top+5+dH*2, rc.left+dW*2, rc.top+dH*3);	
		m_rectC[eC_EDIT_DUST_LIGHT_ALARM_LIMIT]	= CRect(rc.left+5+dW*2, rc.top+5+dH*2, rc.left+dW*4, rc.top+dH*3);	
		m_rectS[eS_TEXT_DUST_HEAVY_ALARM_LIMIT]	= CRect(rc.left+5+dW*0, rc.top+5+dH*3, rc.left+dW*2, rc.top+dH*4);	
		m_rectC[eC_EDIT_DUST_HEAVY_ALARM_LIMIT]	= CRect(rc.left+5+dW*2, rc.top+5+dH*3, rc.left+dW*4, rc.top+dH*4);	
		m_rectC[eC_BTN_DUST_ALARM_LIMIT_SET]	= CRect(rc.left+5+dW*4, rc.top+5+dH*2, rc.left+dW*5, rc.top+dH*4);


	    m_rectS[eS_TEXT_DUST_RUNNING_TIME]		= CRect(rc.left+5+dW*0, rc.top+5+dH*4, rc.left+dW*2, rc.top+dH*6);	// 운전시간 문구
	    m_rectS[eS_DUST_CURRENT_RUNNING_TIME]	= CRect(rc.left+5+dW*2, rc.top+5+dH*4, rc.left+dW*4, rc.top+dH*6);	// 현제 운전시간

		m_rectS[eS_TEXT_DUST_FILTER_CYCLE_TIME]	= CRect(rc.left+5+dW*0, rc.top+5+dH*6, rc.left+dW*2, rc.top+dH*8);	// 필터 수명 문구
	    m_rectC[eC_EDIT_DUST_FILTER_LIMIT]		= CRect(rc.left+5+dW*2, rc.top+5+dH*6, rc.left+dW*4, rc.top+dH*8);	// 필터 제한치 설정
		m_rectC[eC_BTN_DUST_FILTER_LIMIT_SET]	= CRect(rc.left+5+dW*4, rc.top+5+dH*6, rc.left+dW*5, rc.top+dH*8);	// 설정 버튼
}




void CDustCollectorWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CRect				rect = m_rectS[eS_FULL_AREA];
	memDC.FillSolidRect(rect, g_colorBackground[eEC_DISABLE]);
	rect.DeflateRect(3, 3, 3, 3);
	CDrawSymbol::DrawLineRect(&memDC, rect, g_colorForeground[eEC_DEFAULT]);

	    DrawGroupBox(&memDC, m_rectS[eS_DUST_AREA],					g_st[IDS_DUST_COLLECTOR_WND_DUST], RGB_BLACK, RGB_BLACK);

	    DrawTextB(memDC, m_rectS[eS_TEXT_DUST_PRESS],				g_st[IDS_DUST_COLLECTOR_WND_PRESS], 15, FW_HEAVY, RGB_BLACK, DT_CENTER | DT_VCENTER);
		DrawTextB(memDC, m_rectS[eS_TEXT_DUST_LIGHT_ALARM_LIMIT],	g_st[IDS_DUST_COLLECTOR_WND_LTIHGT_ALARM_LIMIT], 15, FW_HEAVY, RGB_RED, DT_CENTER | DT_VCENTER);
		DrawTextB(memDC, m_rectS[eS_TEXT_DUST_HEAVY_ALARM_LIMIT],	g_st[IDS_DUST_COLLECTOR_WND_HEAVY_ALARM_LIMIT], 15, FW_HEAVY, RGB_RED, DT_CENTER | DT_VCENTER);
	    DrawTextB(memDC, m_rectS[eS_TEXT_DUST_RUNNING_TIME],		g_st[IDS_DUST_COLLECTOR_WND_RUNNING_TIME], 15, FW_HEAVY, RGB_BLACK, DT_CENTER | DT_VCENTER);
		DrawTextB(memDC, m_rectS[eS_TEXT_DUST_FILTER_CYCLE_TIME],	g_st[IDS_DUST_COLLECTOR_WND_FILTER_CYCLE_LIMIT], 15, FW_HEAVY, RGB_RED, DT_CENTER | DT_VCENTER);


	    SelectedDrawInt(&memDC, m_rectS[eS_DUST_CURRENT_PRESS], sm.DustCollector->data[eDUST_COLLECTOR_1].iPressure, 15, RGB_WHITE, RGB_GRAY_DARK, RGB_WHITE);
		SelectedDrawInt(&memDC, m_rectS[eS_DUST_CURRENT_RUNNING_TIME], sm.DustCollector->data[eDUST_COLLECTOR_1].iRunningTime, 15, RGB_WHITE, RGB_GRAY_DARK, RGB_WHITE);
		SelectedDrawEditInt(&memDC, m_rectC[eC_EDIT_DUST_LIGHT_ALARM_LIMIT], m_iDustPressLimit[0], TRUE, 15, DT_DT);
		SelectedDrawEditInt(&memDC, m_rectC[eC_EDIT_DUST_HEAVY_ALARM_LIMIT], m_iDustPressLimit[1], TRUE, 15, DT_DT);
		SelectedDrawEditInt(&memDC, m_rectC[eC_EDIT_DUST_FILTER_LIMIT], m_iDustSetLimit, TRUE, 15, DT_DT);

	    DrawMDIMenuButton(memDC, m_rectC[eC_BTN_DUST_ALARM_LIMIT_SET],	g_st[IDS_DUST_COLLECTOR_WND_SETTING], m_eSelectControl == (eC_BTN_DUST_ALARM_LIMIT_SET), m_eMouseOver == (eC_BTN_DUST_ALARM_LIMIT_SET));
		DrawMDIMenuButton(memDC, m_rectC[eC_BTN_DUST_FILTER_LIMIT_SET], g_st[IDS_DUST_COLLECTOR_WND_SETTING], m_eSelectControl == (eC_BTN_DUST_FILTER_LIMIT_SET), m_eMouseOver == (eC_BTN_DUST_FILTER_LIMIT_SET));
		
}



void CDustCollectorWnd::OnMouseMove(UINT nFlags, CPoint point) {
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



void CDustCollectorWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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



void CDustCollectorWnd::OnLButtonUp(UINT nFlags, CPoint point)  {	
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_BTN_DUST_ALARM_LIMIT_SET: {
				if (RespMessage(g_st[IDS_DUST_COLLECTOR_WND_ALARM_LIMIT_SET_MESSAGE]).Compare(g_st[IDS_OK]) == 0) {
					sm.Option->tDustColletorOption.iLightAlarmLimit = m_iDustPressLimit[0];
					sm.Option->tDustColletorOption.iHeavyAlarmLimit= m_iDustPressLimit[1];
					SyncDataDustCollector(TRUE);		
					strLog = _T("집진기 알람 제한치 설정");
				}
				break;
			}
			case eC_BTN_DUST_FILTER_LIMIT_SET: {
				if (RespMessage(g_st[IDS_DUST_COLLECTOR_WND_FILTER_LIMIT_SET_MESSAGE]).Compare(g_st[IDS_OK]) == 0) {
					sm.Option->tDustColletorOption.iFilterCycleLimit = m_iDustSetLimit;
					SyncDataDustCollector(TRUE);		
					strLog = _T("집진기 필터수명 알람 제한치 설정");
				}
				break;
			}
			
			case eC_EDIT_DUST_LIGHT_ALARM_LIMIT:
			case eC_EDIT_DUST_HEAVY_ALARM_LIMIT:	{
				int iSelectAlarm = m_eSelectControl - eC_EDIT_DUST_LIGHT_ALARM_LIMIT;
					EditStart(this, m_rectC[m_eSelectControl], m_iDustPressLimit[m_eSelectControl], FALSE, FALSE, 0, INT_MAX, DT_DT, 15);
				break;
			}
			
			case eC_EDIT_DUST_FILTER_LIMIT:{
				EditStart(this, m_rectC[m_eSelectControl], m_iDustSetLimit, FALSE, FALSE, 0, INT_MAX, DT_DT, 15);
				break;
			}
			
			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl	=	eC_NONE;
	Invalidate(FALSE);
	
	CFrameWndEx::OnLButtonUp(nFlags, point);
	
}

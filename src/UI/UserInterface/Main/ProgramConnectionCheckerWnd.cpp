#include "pch.h"
#include "UI.h"
#include "ProgramConnectionCheckerWnd.h"
#include "MainFrame.h"

/// <summary>
/// 반드시 모카 프로세스 이름과 같아야한다.
/// </summary>
static LPCTSTR lpszProcessList[] = {
	_T("UI")
	, _T("PLCLinker")
	, _T("Sequence")
	, _T("DataTransfer")
	, _T("SystemAlarm")
	//, _T("Vision")
	//, _T("Serial")	//. 추후에 추가
};

CProgramConnectionCheckerWnd::CProgramConnectionCheckerWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
}

CProgramConnectionCheckerWnd::~CProgramConnectionCheckerWnd() {
}

IMPLEMENT_DYNCREATE(CProgramConnectionCheckerWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP(CProgramConnectionCheckerWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


BOOL CProgramConnectionCheckerWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CProgramConnectionCheckerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	try {

		if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
			return -1;
	}				
	catch (CException* pException) {
		pException->ReportError();
		return -1;
	}

	return 0;
}

void CProgramConnectionCheckerWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);

	CRect rc = m_rectS[eS_FULL_AREA];

	double dW = (rc.Width() - 5.) / 1.;
	double dH = (rc.Height() - 5.) / 8.;

	m_rectC[eC_CONNECT_UI			] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 1, rc.top + dH * 1);
	m_rectC[eC_CONNECT_PLCLINKER	] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 1, rc.left + dW * 1, rc.top + dH * 2);
	m_rectC[eC_CONNECT_SEQUENCE		] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 2, rc.left + dW * 1, rc.top + dH * 3);
	m_rectC[eC_CONNECT_DATATRANSFER	] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 3, rc.left + dW * 1, rc.top + dH * 4);
	m_rectC[eC_CONNECT_ALARM		] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 4, rc.left + dW * 1, rc.top + dH * 5);
	//m_rectC[eC_CONNECT_VISION		] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 5, rc.left + dW * 1, rc.top + dH * 6);
	//m_rectC[eC_CONNECT_SERIAL		] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 6, rc.left + dW * 1, rc.top + dH * 7);
}

void CProgramConnectionCheckerWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		//. SetWindowPos할 필요 없이, 상위 부모가 위치 잡아줘야함.
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;

		Invalidate(FALSE);
	}
	else {
	}
}

BOOL CProgramConnectionCheckerWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}

void CProgramConnectionCheckerWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	//memDC.FillSolidRect(m_rectS[eS_FULL_AREA], RgbColor::AliceBlue);
	CRect rect = m_rectS[eS_FULL_AREA];
	rect.InflateRect(1, 1);
	memDC.FillSolidRect(rect, RgbColor::AliceBlue);

	DrawSensor(&memDC, m_rectC[eC_CONNECT_UI], Format(_T(" %s"), lpszProcessList[ePROGRAM_UI]), GetProcessConnected(ePROGRAM_UI), DT_SENSOR_ON, DT_SENSOR_OFF, 9, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawSensor(&memDC, m_rectC[eC_CONNECT_PLCLINKER], Format(_T(" %s"), lpszProcessList[ePROGRAM_PLCLINKER]), GetProcessConnected(ePROGRAM_PLCLINKER), DT_SENSOR_ON, DT_SENSOR_OFF, 9, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawSensor(&memDC, m_rectC[eC_CONNECT_SEQUENCE], Format(_T(" %s"), lpszProcessList[ePROGRAM_SEQUENCE]), GetProcessConnected(ePROGRAM_SEQUENCE), DT_SENSOR_ON, DT_SENSOR_OFF, 9, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawSensor(&memDC, m_rectC[eC_CONNECT_DATATRANSFER], Format(_T(" %s"), lpszProcessList[ePROGRAM_DATATRANSFER]), GetProcessConnected(ePROGRAM_DATATRANSFER), DT_SENSOR_ON, DT_SENSOR_OFF, 9, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawSensor(&memDC, m_rectC[eC_CONNECT_ALARM], Format(_T(" %s"), lpszProcessList[ePROGRAM_ALARM]), GetProcessConnected(ePROGRAM_ALARM), DT_SENSOR_ON, DT_SENSOR_OFF, 9, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
}

void CProgramConnectionCheckerWnd::OnMouseMove(UINT nFlags, CPoint point) {
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

void CProgramConnectionCheckerWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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

void CProgramConnectionCheckerWnd::OnLButtonUp(UINT nFlags, CPoint point)  {
	if (m_eSelectControl != eC_NONE && m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			default: break;
		}
	}
	m_eSelectControl	=	eC_NONE;
	Invalidate(FALSE);
	CFrameWndEx::OnLButtonUp(nFlags, point);
}

void CProgramConnectionCheckerWnd::OnKillFocus(CWnd* pNewWnd) {
	CFrameWndEx::OnKillFocus(pNewWnd);
	Invalidate();
}

void CProgramConnectionCheckerWnd::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {

		default: break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}


int	CProgramConnectionCheckerWnd::SearchMochaAttachedProcessIndex(ePROGRAM_LIST eProcess) {
	
	for (int i = 0; i < MOCHA_MAX_PROCESS; ++i) {

		//. char -> multi byte char 변환
		CStringW strMochaProcess(sm.Process[i].szProcessName);
		if (strMochaProcess == lpszProcessList[eProcess]) {
			return i;
		}
	}

	return -1;
}

BOOL CProgramConnectionCheckerWnd::GetProcessConnected(ePROGRAM_LIST eProcess) {
	
	int iProcessIndexMocha = SearchMochaAttachedProcessIndex(eProcess);

	if (-1 == iProcessIndexMocha || MOCHA_MAX_PROCESS <= iProcessIndexMocha) {
		return FALSE;
	}

	return sm.Process[iProcessIndexMocha].bConnected;
}

BOOL CProgramConnectionCheckerWnd::IsProcessAllConncted() {

	BOOL bIsAllConnected = TRUE;
	bIsAllConnected &= GetProcessConnected(ePROGRAM_UI			 );
	bIsAllConnected &= GetProcessConnected(ePROGRAM_PLCLINKER	 );
	bIsAllConnected &= GetProcessConnected(ePROGRAM_SEQUENCE	 );
	bIsAllConnected &= GetProcessConnected(ePROGRAM_DATATRANSFER );
	bIsAllConnected &= GetProcessConnected(ePROGRAM_ALARM		 );
	//bIsAllConnected &= GetProcessConnected(ePROGRAM_VISION		 );
	//bIsAllConnected &= GetProcessConnected(ePROGRAM_SERIAL		 );

	return bIsAllConnected;
}

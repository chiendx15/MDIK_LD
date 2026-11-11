#include "pch.h"
#include "UI.h"
#include "ConveyorMonitorWnd.h"

#define RE_DRAW 9090

CConveyorMonitorWnd::CConveyorMonitorWnd(CWnd* pwndParent)
	: m_pwndParent(pwndParent)
	, m_eSelectConveyor(0)
{
}

CConveyorMonitorWnd::~CConveyorMonitorWnd() {
}

#define ID_MOTOR_LIST	10002

IMPLEMENT_DYNCREATE(CConveyorMonitorWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP(CConveyorMonitorWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, ID_MOTOR_LIST, OnItemchangedLst)
END_MESSAGE_MAP()

BOOL CConveyorMonitorWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CConveyorMonitorWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {
		m_lstConveyor.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, m_rectS[eS_CONVEYOR_LIST], this, ID_MOTOR_LIST);
		m_lstConveyor.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
		m_lstConveyor.ModifyStyle(0, LVS_SHOWSELALWAYS);
		m_lstConveyor.SetRowHeight(45);
		m_lstConveyor.ShowScrollBar(SB_VERT);
		m_lstConveyor.ShowScrollBar(SB_HORZ);
		m_lstConveyor.InsertColumn(eLI_NONE, _T(""), LVCFMT_LEFT, 0, 1);
		m_lstConveyor.InsertColumn(eLI_CONVEYOR_INDEX, _T("Axis"), LVCFMT_LEFT, 100, 1);
		m_lstConveyor.InsertColumn(eLI_CONVEYOR_NAME, _T("Name"), LVCFMT_LEFT, 200, 1);

		for (int i = 0; i < nCONVEYOR_LD; i++) {
			m_lstConveyor.InsertItem(i, _T(""));
			m_lstConveyor.SetItemInt(i, eLI_CONVEYOR_INDEX, i);
			m_lstConveyor.SetItemText(i, eLI_CONVEYOR_NAME, g_stMotor[IDS_CONVEYOR_NAME_01 + i]);
			m_lstConveyor.SetAlignText(eLI_CONVEYOR_NAME, eLCETA_LEFT);
		}
	}
	catch (...) {
		return -1;
	}

	return 0;
}

void CConveyorMonitorWnd::ChangeLanguage()
{
	for (int i = 0; i < nCONVEYOR_LD; i++) {

		m_lstConveyor.SetItemText(i, eLI_CONVEYOR_NAME, g_stMotor[IDS_CONVEYOR_NAME_01 + i]);
	}
	Invalidate(FALSE);
}

void CConveyorMonitorWnd::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case RE_DRAW: {
			InvalidateRect(m_rectS[eS_WORK_AREA]);
			break;
		}
	}
	CFrameWndEx::OnTimer(nIDEvent);
}

void CConveyorMonitorWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_lstConveyor.SetItemState(m_eSelectConveyor, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

		SetTimer(RE_DRAW, 100, NULL);
		Invalidate();
	}
	else {
		KillTimer(RE_DRAW);
	}
}

BOOL CConveyorMonitorWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}

void CConveyorMonitorWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);

	CRect rect = m_rectS[eS_FULL_AREA];
	DrawLineRect(memDC, rect, RGB_WHITE);
	rect.DeflateRect(1, 1);
	memDC.FillSolidRect(rect, RgbColor::Gainsboro);
	memDC.FillSolidRect(m_rectS[eS_WORK_AREA], RgbColor::AliceBlue);

	auto Conveyor = sm.InverterData[m_eSelectConveyor];
	eMODULEs _eModule = (eMODULEs)(eMODULE_PC1_LOAD_CONVEYOR_1 + m_eSelectConveyor);

	SelectedDrawText(memDC, m_rectS[eS_CONVERYO_NAME], g_stMotor[IDS_CONVEYOR_NAME_01 + m_eSelectConveyor], DT_FONTSIZE, DT_PEN, RGB_GRAY_LIGHT, RGB_BLACK);

	DrawLineRect(memDC, m_rectS[eS_CONVEYOR_STATUS_AREA], RGB_BLACK);

	DrawText(memDC, m_rectS[eS_READY], g_st[IDS_INVERTER_MONITOR_READY_NOT_READY], 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_RUN_STOP], g_st[IDS_INVERTER_MONITOR_RUN_STOP], 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_DIRECTION], g_st[IDS_INVERTER_MONITOR_DIRECTION], 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_FREQUENCY], g_st[IDS_INVERTER_MONITOR_FREQUENCY], 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_ACC1], g_st[IDS_INVERTER_MONITOR_ACC1], 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_ACC2], g_st[IDS_INVERTER_MONITOR_ACC2], 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_DEC1], g_st[IDS_INVERTER_MONITOR_DEC1], 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_DEC2], g_st[IDS_INVERTER_MONITOR_DEC2], 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_TRIP_FACTOR_UPPER_1], Format(_T("%s_%d"), g_st[IDS_INVERTER_MONITOR_INVERTER_TRIP_UPPER], 1), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_TRIP_FACTOR_LOWER_1], Format(_T("%s_%d"), g_st[IDS_INVERTER_MONITOR_INVERTER_TRIP_LOWER], 1), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_TRIP_FACTOR_UPPER_2], Format(_T("%s_%d"), g_st[IDS_INVERTER_MONITOR_INVERTER_TRIP_UPPER], 2), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_TRIP_FACTOR_LOWER_2], Format(_T("%s_%d"), g_st[IDS_INVERTER_MONITOR_INVERTER_TRIP_LOWER], 2), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_TRIP_FACTOR_UPPER_3], Format(_T("%s_%d"), g_st[IDS_INVERTER_MONITOR_INVERTER_TRIP_UPPER], 3), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_TRIP_FACTOR_LOWER_3], Format(_T("%s_%d"), g_st[IDS_INVERTER_MONITOR_INVERTER_TRIP_LOWER], 3), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_TRIP_FACTOR_UPPER_4], Format(_T("%s_%d"), g_st[IDS_INVERTER_MONITOR_INVERTER_TRIP_UPPER], 4), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_TRIP_FACTOR_LOWER_4], Format(_T("%s_%d"), g_st[IDS_INVERTER_MONITOR_INVERTER_TRIP_LOWER], 4), 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);

	SelectedDrawEditText(memDC, m_rectS[eS_EDIT_READY], Serial::Inverter::GetInverterReady(_eModule) ? g_st[IDS_INVERTER_MONITOR_READY] : g_st[IDS_INVERTER_MONITOR_NOT_READY], FALSE);
	SelectedDrawEditText(memDC, m_rectS[eS_EDIT_RUN_STOP], Serial::Inverter::GetInverterRun(_eModule) ? g_st[IDS_INVERTER_MONITOR_RUN] : g_st[IDS_INVERTER_MONITOR_STOP], FALSE);
	SelectedDrawEditText(memDC, m_rectS[eS_EDIT_DIRECTION], Serial::Inverter::GetInverterDirection(_eModule, TRUE) ? g_st[IDS_INVERTER_MONITOR_FORWARD] : g_st[IDS_INVERTER_MONITOR_BACKWARD], FALSE);

	SelectedDrawEditInt(memDC, m_rectS[eS_EDIT_FREQUENCY], Serial::Inverter::GetInverterFrequency(_eModule), FALSE);
	SelectedDrawEditInt(memDC, m_rectS[eS_EDIT_ACC1], Serial::Inverter::GetInverteriAccTime1(_eModule), FALSE);
	SelectedDrawEditInt(memDC, m_rectS[eS_EDIT_ACC2], Serial::Inverter::GetInverteriAccTime2(_eModule), FALSE);
	SelectedDrawEditInt(memDC, m_rectS[eS_EDIT_DEC1], Serial::Inverter::GetInverteriDecTime1(_eModule), FALSE);
	SelectedDrawEditInt(memDC, m_rectS[eS_EDIT_DEC2], Serial::Inverter::GetInverteriDecTime2(_eModule), FALSE);



	for (int i = 0; i < countof(Conveyor.iTripCause); i++) {
		if (Conveyor.iTripCause[i] < 0 || Conveyor.iTripCause[i] > nINVERTER_TRIP_UPPER)
			SelectedDrawEditText(memDC, m_rectS[eS_EDIT_TRIP_FACTOR_LOWER_1 + i], g_st[IDS_INVERTER_TRIP_UNKNOW], FALSE);
		else
			SelectedDrawEditText(memDC, m_rectS[eS_EDIT_TRIP_FACTOR_UPPER_1 + i], g_st[IDS_INVERTER_TRIP_UPPER_NONE + Conveyor.iTripCause[i]], FALSE);

		if (Conveyor.iStatus[i] < 0 || Conveyor.iStatus[i] > nINVERTER_TRIP_LOWER)
			SelectedDrawEditText(memDC, m_rectS[eS_EDIT_TRIP_FACTOR_LOWER_1 + i], g_st[IDS_INVERTER_TRIP_UNKNOW], FALSE);
		else
			SelectedDrawEditText(memDC, m_rectS[eS_EDIT_TRIP_FACTOR_LOWER_1 + i], g_st[IDS_INVERTER_TRIP_LOWER_RESETTING + Conveyor.iStatus[i]], FALSE);

	}
}


void CConveyorMonitorWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);

	CRect		rect = m_rectS[eS_FULL_AREA];

	double dW = (rect.Width() - 5.) / 20.;
	double dH = (rect.Height() - 5.) / 16.;

	m_rectS[eS_CONVEYOR_LIST]	= CRect(rect.left + dW * 0 + 5, rect.top + dH * 0 + 5, rect.left + dW * 5, rect.top + dH * 16);
	m_rectS[eS_WORK_AREA]		= CRect(rect.left + dW * 5 + 5, rect.top + dH * 0 + 5, rect.left + dW * 20, rect.top + dH * 16);

	rect = m_rectS[eS_WORK_AREA];

	dW = (rect.Width() - 5.) / 8.;
	dH = (rect.Height() - 5.) / 18.;

	m_rectS[eS_CONVERYO_NAME]			= CRect(rect.left + dW * 0 + 5, rect.top + dH * 0 + 5, rect.left + dW * 5, rect.top + dH * 1);
	m_rectS[eS_CONVEYOR_STATUS_AREA]	= CRect(rect.left + dW * 0 + 5, rect.top + dH * 1 + 5, rect.left + dW * 5, rect.top + dH * 18);

	rect = m_rectS[eS_CONVEYOR_STATUS_AREA];
	rect.top += 20;
	dW = (rect.Width() - 5.) / 5.;
	dH = (rect.Height() - 5.) / 18.;

	m_rectS[eS_READY]					= CRect(rect.left + dW * 0 + 5, rect.top + dH * 0 + 5, rect.left + dW * 1, rect.top + dH * 1);
	m_rectS[eS_RUN_STOP]				= CRect(rect.left + dW * 0 + 5, rect.top + dH * 1 + 5, rect.left + dW * 1, rect.top + dH * 2);
	m_rectS[eS_DIRECTION]				= CRect(rect.left + dW * 0 + 5, rect.top + dH * 2 + 5, rect.left + dW * 1, rect.top + dH * 3);

	m_rectS[eS_FREQUENCY]				= CRect(rect.left + dW * 0 + 5, rect.top + dH * 4 + 5, rect.left + dW * 1, rect.top + dH * 5);
	m_rectS[eS_ACC1]					= CRect(rect.left + dW * 0 + 5, rect.top + dH * 5 + 5, rect.left + dW * 1, rect.top + dH * 6);
	m_rectS[eS_ACC2]					= CRect(rect.left + dW * 0 + 5, rect.top + dH * 6 + 5, rect.left + dW * 1, rect.top + dH * 7);
	m_rectS[eS_DEC1]					= CRect(rect.left + dW * 0 + 5, rect.top + dH * 7 + 5, rect.left + dW * 1, rect.top + dH * 8);
	m_rectS[eS_DEC2]					= CRect(rect.left + dW * 0 + 5, rect.top + dH * 8 + 5, rect.left + dW * 1, rect.top + dH * 9);

	m_rectS[eS_TRIP_FACTOR_UPPER_1]		= CRect(rect.left + dW * 2 + 5, rect.top + dH * 0 + 5, rect.left + dW * 3, rect.top + dH * 1);
	m_rectS[eS_TRIP_FACTOR_LOWER_1]		= CRect(rect.left + dW * 2 + 5, rect.top + dH * 1 + 5, rect.left + dW * 3, rect.top + dH * 2);
	m_rectS[eS_TRIP_FACTOR_UPPER_2]		= CRect(rect.left + dW * 2 + 5, rect.top + dH * 2 + 5, rect.left + dW * 3, rect.top + dH * 3);
	m_rectS[eS_TRIP_FACTOR_LOWER_2]		= CRect(rect.left + dW * 2 + 5, rect.top + dH * 3 + 5, rect.left + dW * 3, rect.top + dH * 4);
	m_rectS[eS_TRIP_FACTOR_UPPER_3]		= CRect(rect.left + dW * 2 + 5, rect.top + dH * 4 + 5, rect.left + dW * 3, rect.top + dH * 5);
	m_rectS[eS_TRIP_FACTOR_LOWER_3]		= CRect(rect.left + dW * 2 + 5, rect.top + dH * 5 + 5, rect.left + dW * 3, rect.top + dH * 6);
	m_rectS[eS_TRIP_FACTOR_UPPER_4]		= CRect(rect.left + dW * 2 + 5, rect.top + dH * 6 + 5, rect.left + dW * 3, rect.top + dH * 7);
	m_rectS[eS_TRIP_FACTOR_LOWER_4]		= CRect(rect.left + dW * 2 + 5, rect.top + dH * 7 + 5, rect.left + dW * 3, rect.top + dH * 8);

	// Status 표시.
	m_rectS[eS_EDIT_READY]				= CRect(rect.left + dW * 1 + 5, rect.top + dH * 0 + 5, rect.left + dW * 2, rect.top + dH * 1);
	m_rectS[eS_EDIT_RUN_STOP]			= CRect(rect.left + dW * 1 + 5, rect.top + dH * 1 + 5, rect.left + dW * 2, rect.top + dH * 2);
	m_rectS[eS_EDIT_DIRECTION]			= CRect(rect.left + dW * 1 + 5, rect.top + dH * 2 + 5, rect.left + dW * 2, rect.top + dH * 3);

	m_rectS[eS_EDIT_FREQUENCY]			= CRect(rect.left + dW * 1 + 5, rect.top + dH * 4 + 5, rect.left + dW * 2, rect.top + dH * 5);
	m_rectS[eS_EDIT_ACC1]				= CRect(rect.left + dW * 1 + 5, rect.top + dH * 5 + 5, rect.left + dW * 2, rect.top + dH * 6);
	m_rectS[eS_EDIT_ACC2]				= CRect(rect.left + dW * 1 + 5, rect.top + dH * 6 + 5, rect.left + dW * 2, rect.top + dH * 7);
	m_rectS[eS_EDIT_DEC1]				= CRect(rect.left + dW * 1 + 5, rect.top + dH * 7 + 5, rect.left + dW * 2, rect.top + dH * 8);
	m_rectS[eS_EDIT_DEC2]				= CRect(rect.left + dW * 1 + 5, rect.top + dH * 8 + 5, rect.left + dW * 2, rect.top + dH * 9);

	m_rectS[eS_EDIT_TRIP_FACTOR_UPPER_1]	= CRect(rect.left + dW * 3 + 5, rect.top + dH * 0 + 5, rect.left + dW * 5, rect.top + dH * 1);
	m_rectS[eS_EDIT_TRIP_FACTOR_LOWER_1]	= CRect(rect.left + dW * 3 + 5, rect.top + dH * 1 + 5, rect.left + dW * 5, rect.top + dH * 2);
	m_rectS[eS_EDIT_TRIP_FACTOR_UPPER_2]	= CRect(rect.left + dW * 3 + 5, rect.top + dH * 2 + 5, rect.left + dW * 5, rect.top + dH * 3);
	m_rectS[eS_EDIT_TRIP_FACTOR_LOWER_2]	= CRect(rect.left + dW * 3 + 5, rect.top + dH * 3 + 5, rect.left + dW * 5, rect.top + dH * 4);
	m_rectS[eS_EDIT_TRIP_FACTOR_UPPER_3]	= CRect(rect.left + dW * 3 + 5, rect.top + dH * 4 + 5, rect.left + dW * 5, rect.top + dH * 5);
	m_rectS[eS_EDIT_TRIP_FACTOR_LOWER_3]	= CRect(rect.left + dW * 3 + 5, rect.top + dH * 5 + 5, rect.left + dW * 5, rect.top + dH * 6);
	m_rectS[eS_EDIT_TRIP_FACTOR_UPPER_4]	= CRect(rect.left + dW * 3 + 5, rect.top + dH * 6 + 5, rect.left + dW * 5, rect.top + dH * 7);
	m_rectS[eS_EDIT_TRIP_FACTOR_LOWER_4]	= CRect(rect.left + dW * 3 + 5, rect.top + dH * 7 + 5, rect.left + dW * 5, rect.top + dH * 8);


	m_lstConveyor.MoveWindow(m_rectS[eS_CONVEYOR_LIST]);
}

void CConveyorMonitorWnd::OnItemchangedLst(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if ((pNMLV->uNewState & LVIS_SELECTED) && !(pNMLV->uOldState & LVIS_SELECTED)) { // Select 

		if (!sm.Option->tKeyPad.fShowKeyPad) {

			m_eSelectConveyor = pNMLV->iItem;

			for (int i = 0; i < m_lstConveyor.GetItemCount(); i++)
				m_lstConveyor.SetItemBKColor(i, RGB_WHITE);

			m_lstConveyor.SetItemBKColor(pNMLV->iItem, RGB_GRAY_LIGHT);

			Invalidate(FALSE);
		}
	}
}
// MotorStatusWnd.cpp: 구현 파일
//

#include "pch.h"
#include "PLCLinker.h"
#include "MotorStatusWnd.h"
#include "YaskawaMP.h"

// CMotorStatusWnd
#define ID_LIST_MOTOR_STATUS 10002
#define ID_INTERVAL_UI 10003
IMPLEMENT_DYNCREATE(CMotorStatusWnd, CFrameWnd)

CMotorStatusWnd::CMotorStatusWnd()
{
	m_iAxisSelect = eMOTOR_0;
	m_eSelectControl = -1;
}

CMotorStatusWnd::~CMotorStatusWnd()
{
}


BEGIN_MESSAGE_MAP(CMotorStatusWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(LVN_ITEMCHANGED, ID_LIST_MOTOR_STATUS, OnItemchangedLst)
END_MESSAGE_MAP()


// CMotorStatusWnd 메시지 처리기


int CMotorStatusWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_lstAxis.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, m_rectS[eS_LIST], this, ID_LIST_MOTOR_STATUS);
	m_lstAxis.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_lstAxis.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_lstAxis.SetRowHeight();
	m_lstAxis.ShowScrollBar(SB_VERT);
	m_lstAxis.ShowScrollBar(SB_HORZ);
	m_lstAxis.InsertColumn(eLI_NONE, _T(""), LVCFMT_CENTER, 0, 1);
	m_lstAxis.InsertColumn(eLI_INDEX, _T("No."), LVCFMT_CENTER, 40, 1);
	m_lstAxis.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	for (int i = 0; i < eMOTOR_END; i++) {
		m_lstAxis.InsertItem(i, _T(""));
		m_lstAxis.SetItemInt(i, eLI_INDEX,i+1);
	}
	return 0;
}


void CMotorStatusWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	if (bShow) {
		int iW = 1920 / 3;
		int iH = 960;
		SetWindowPos(&wndTopMost, 0, 0, iW, iH, SWP_SHOWWINDOW);
		ComputeArea();
		Invalidate(TRUE);
		SetTimer(ID_INTERVAL_UI, 100, NULL);
	}
	else
	{
		KillTimer(ID_INTERVAL_UI);
	}
}


void CMotorStatusWnd::OnPaint()
{
	CPaintDC dc(this); 
	try {
		CMemDCEx::CMemDC		memDC(&dc);
		CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_DEFAULT]);
		CBrush* pOldBrush = (CBrush*)memDC.SelectStockObject(NULL_BRUSH);
		CPen* pOldPen = (CPen*)memDC.SelectObject(&pen);
		CRect					rect = m_rectClient;

		memDC.SelectObject(&g_brushBackground[eEC_DEFAULT]);
		// Operation Ready
		DrawText(&memDC, m_rectS[eS_OPERATIONREADY], _T("Operation Ready"));
		
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bOperationReady)
			SelectedDrawText(memDC, m_rectC[eC_OPERATIONREADY], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else 
			SelectedDrawText(memDC, m_rectC[eC_OPERATIONREADY], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);

		// Servo On
		DrawText(memDC, m_rectS[eS_SEERVOON], _T("Servo On"));
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bServoOn)
			SelectedDrawText(memDC, m_rectC[eC_SEERVOON], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_SEERVOON], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);

		// System Busy
		DrawText(memDC, m_rectS[eS_SYSTEMBUSY], _T("System Busy"));
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bSystemBusy)
			SelectedDrawText(memDC, m_rectC[eC_SYSTEMBUSY], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_SYSTEMBUSY], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);

		// Servo Ready
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bServoReady)
			SelectedDrawText(memDC, m_rectC[eC_SERVOREADY], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_SERVOREADY], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_SERVOREADY], _T("Servo Ready"));

		// Servo Error
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bServoError)
			SelectedDrawText(memDC, m_rectC[eC_SERVOERROR], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_SERVOERROR], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_SERVOERROR], _T("Servo Error"));

		// Flowing Error
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bFollowingError)
			SelectedDrawText(memDC, m_rectC[eC_FOLLOWINGERROR], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_FOLLOWINGERROR], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_FOLLOWINGERROR], _T("Flowing Error"));

		// Speed Error
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bSpeedError)
			SelectedDrawText(memDC, m_rectC[eC_SPEEDERROR], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_SPEEDERROR], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_SPEEDERROR], _T("Speed Error"));

		// Zero Position No Set
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bZeroPointNotSet)
			SelectedDrawText(memDC, m_rectC[eC_ZEROPOINTNOTSET], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_ZEROPOINTNOTSET], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_ZEROPOINTNOTSET], _T("Zero Position No Set"));

		// Inposition
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bInPosition)
			SelectedDrawText(memDC, m_rectC[eC_INPOSITION], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_INPOSITION], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_INPOSITION], _T("Inposition"));

		// Zero Point Position
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bZeroPointPosition)
			SelectedDrawText(memDC, m_rectC[eC_ZEROPOINTPOSITION], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_ZEROPOINTPOSITION], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_ZEROPOINTPOSITION], _T("Zero Position Position"));

		// Zero Point Return Set
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bZeroPointReturnSet)
			SelectedDrawText(memDC, m_rectC[eC_ZEROPOINTRETURNSET], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_ZEROPOINTRETURNSET], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_ZEROPOINTRETURNSET], _T("Zero Position Return Set"));

		// Alarm
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bAlarm)
			SelectedDrawText(memDC, m_rectC[eC_ALARM], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_ALARM], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_ALARM], _T("Alarm"));

		// Warning
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bWarning)
			SelectedDrawText(memDC, m_rectC[eC_WARNING], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_WARNING], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_WARNING], _T("Warning"));

		// cmd Ready
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bCmdReady)
			SelectedDrawText(memDC, m_rectC[eC_CMDREADY], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_CMDREADY], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_CMDREADY], _T("Cmd Ready"));

		// Main Power
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bMainPower)
			SelectedDrawText(memDC, m_rectC[eC_MAINPOWER], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_MAINPOWER], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_MAINPOWER], _T("Main Power"));

		// TorqueLimit
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bTorqueLimit)
			SelectedDrawText(memDC, m_rectC[eC_TORQUELIMIT], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_TORQUELIMIT], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_TORQUELIMIT], _T("TorqueLimit"));

		// SoftPLimit
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bSoftPLimit)
			SelectedDrawText(memDC, m_rectC[eC_SOFTPLIMIT], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_SOFTPLIMIT], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_SOFTPLIMIT], _T("Soft P Limit"));

		// SoftNLimit
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bSoftNLimit)
			SelectedDrawText(memDC, m_rectC[eC_SOFTNLIMIT], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_SOFTNLIMIT], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_SOFTNLIMIT], _T("Soft N Limit"));

		// PLimit
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bPLimit)
			SelectedDrawText(memDC, m_rectC[eC_PLIMIT], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_PLIMIT], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_PLIMIT], _T("PLimit"));
		// NLimit
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bNLimit)
			SelectedDrawText(memDC, m_rectC[eC_NLIMIT], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_NLIMIT], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_NLIMIT], _T("NLimit"));

		// Home Sensor
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bHomeSensor)
			SelectedDrawText(memDC, m_rectC[eC_HOMESENSOR], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_HOMESENSOR], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_HOMESENSOR], _T("Home Sensor"));

		// BREAK
		if (sm.MotorStatus[m_iAxisSelect].tCrt.bBreak)
			SelectedDrawText(memDC, m_rectC[eC_BREAK], _T(""), 9, RGB_GREEN, RGB_GREEN, RGB_WHITE, DT_CENTER);
		else
			SelectedDrawText(memDC, m_rectC[eC_BREAK], _T(""), 9, RGB_RED, RGB_RED, RGB_WHITE, DT_CENTER);
		DrawText(memDC, m_rectS[eS_BREAK], _T("Break"));

		//for(int i =0 ; i < 32 ; i++){
		//	DrawSensor(memDC, m_rectC[eC_IN_0 + i], _T(""), sm.IO->tIn_CRT.fIO_IN[i]);
		//	DrawSensor(memDC, m_rectC[eC_OUT_0 + i], _T(""), sm.IO->tOut_CRT.fIO_OUT[i]);
		//}
		memDC.SelectObject(pOldBrush);
		memDC.SelectObject(pOldPen);
	}
	catch (...) {}
}


void CMotorStatusWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case ID_INTERVAL_UI:
		for (int i = eC_NONE; i < eC_END; i++) {
			InvalidateRect(m_rectC[i], FALSE);
		}
		break;
	}
	CFrameWnd::OnTimer(nIDEvent);
}


void CMotorStatusWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	for(int i = 0 ; i < eC_END; i++){
		if( m_rectC[i].PtInRect(point)){
			m_eSelectControl = i;
			break;
		}
	}
	CFrameWnd::OnLButtonDown(nFlags, point);
}


void CMotorStatusWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( m_rectC[ m_eSelectControl ].PtInRect( point )){
		
		int iIndex = m_eSelectControl - eC_OUT_0;
		//if( eC_OUT_0 <= m_eSelectControl  && eC_OUT_31 >= m_eSelectControl ){
		//	sm.IO->tOut_NEW.fIO_OUT[iIndex] = !sm.IO->tOut_NEW.fIO_OUT[iIndex];
		//}
	}

	CFrameWnd::OnLButtonUp(nFlags, point);
}



void CMotorStatusWnd::ComputeArea()
{
	CRect rect;
	GetClientRect(m_rectClient);
	rect = m_rectClient;
	double dH, dW;
	dH = (rect.Height() - 5)/ 23;
	dW = (rect.Width() -5) / 10;

	m_rectS[eS_LIST] = CRect(rect.left + dW*0 +5 , rect.top+5, rect.left + dW*1 , rect.bottom);
	for (int i = 0; i < eS_END - eS_OPERATIONREADY; i++) {
		m_rectC[eC_OPERATIONREADY + i] = CRect(rect.left + dW*1+5 , rect.top+ dH*i+5, rect.left + dW*2, rect.top + dH * (1 + i));
		m_rectS[eS_OPERATIONREADY + i] = CRect(rect.left + dW*2+5 , rect.top+ dH*i+5, rect.left + dW*3, rect.top + dH * (1 + i));
	}
	
	dH = (rect.Height() - 5)/ 32;
	for (int i = 0; i < 32; i++) {
		m_rectC[eC_IN_0 + i] = CRect(rect.left + dW*4+5 , rect.top+ dH*i+5, rect.left + dW*5, rect.top + dH * (1 + i));
		m_rectC[eC_OUT_0 + i] = CRect(rect.left + dW*5+5 , rect.top+ dH*i+5, rect.left + dW*6, rect.top + dH * (1 + i));
	}
	m_lstAxis.MoveWindow(m_rectS[eS_LIST]);
	m_lstAxis.ShowWindow(SW_SHOW);
}


void CMotorStatusWnd::OnItemchangedLst(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if ((pNMLV->uNewState & LVIS_SELECTED) && !(pNMLV->uOldState & LVIS_SELECTED)) { // Select		
		m_iAxisSelect = pNMLV->iItem;
		Invalidate(FALSE);
	}
}
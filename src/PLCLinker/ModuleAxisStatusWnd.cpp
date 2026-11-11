// CModuleAxisStatusWnd.cpp: 구현 파일
//

#include "pch.h"
#include "PLCLinker.h"
#include "ModuleAxisStatusWnd.h"

CModuleAxisStatusWnd::CModuleAxisStatusWnd()  : TIPCT<CFrameWnd>(&g_ipClient)
{}

CModuleAxisStatusWnd::~CModuleAxisStatusWnd()
{}

void CModuleAxisStatusWnd::ComputArea() 
{
	for (int idx = 0; idx < eC_END; ++idx)
	{
		m_rectC[idx] = CRect();
	}
	for (int idx = 0; idx < eS_END; ++idx)
	{
		m_rectS[idx] = CRect();
	}

	GetClientRect(m_rectClient);
	CRect rc = m_rectClient;

	m_rectS[eS_AREA_AXIS_STATUS]			= CRect(rc.left, rc.top, rc.right, rc.bottom - 60);
	m_rectS[eS_AREA_SELECT_MOTOR_CONTROL]	= CRect(rc.left, rc.bottom - 60, rc.right, rc.bottom);

	if (m_AxisView)
	{
		m_AxisView.MoveWindow(m_rectS[eS_AREA_AXIS_STATUS]);
		m_AxisView.ShowWindow(SW_SHOW);
	}

	rc = m_rectS[eS_AREA_SELECT_MOTOR_CONTROL];
	rc.DeflateRect(5, 5);
	double dW = rc.Width() / 9.;

	if (!sm.Sequence->fAutoMode)
	{
		m_rectS[eS_SELECT_MOTOR_NAME]			= CRect(rc.left + dW * 0, rc.top, rc.left + dW * 3, rc.bottom);
		m_rectC[eC_STEP_OR_JOG]					= CRect(rc.left + dW * 3, rc.top, rc.left + dW * 4, rc.bottom);
		m_rectC[eC_SENSOR_MOVE_SPEED]			= CRect(rc.left + dW * 4, rc.top, rc.left + dW * 5, rc.bottom);
		if (m_bStepMove)
		{
			m_rectC[eC_EDIT_STEP_DISTANCE]			= CRect(rc.left + dW * 5, rc.top, rc.left + dW * 6, rc.bottom);
		}
		m_rectC[eC_BTN_MINUS_MOVE]				= CRect(rc.left + dW * 6, rc.top, rc.left + dW * 7, rc.bottom);
		m_rectC[eC_BTN_PLUS_MOVE]				= CRect(rc.left + dW * 7, rc.top, rc.left + dW * 8, rc.bottom);
	}
	m_rectC[eC_CLOSE]						= CRect(rc.left + dW * 8, rc.top, rc.left + dW * 9, rc.bottom);
}


IMPLEMENT_DYNCREATE(CModuleAxisStatusWnd, CFrameWnd)
BEGIN_MESSAGE_MAP(CModuleAxisStatusWnd, CFrameWnd)
	ON_WM_CREATE ()
	ON_WM_SHOWWINDOW ()
	ON_WM_PAINT ()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP ()
	ON_WM_LBUTTONDOWN ()
	ON_WM_MOUSEMOVE ()
	ON_WM_TIMER ()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

int CModuleAxisStatusWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_AxisView.Create(NULL, NULL, NULL, CRect(), this, 0);
	
	T_AXIS_STATUS_VIEW tData;
	for (int iMotor = 0; iMotor < eMOTOR_END; ++iMotor)
	{
		tData.strText = g_stMotor[iMotor];
		tData.bSetSub = sm.MotorParameter[iMotor].tYaskawa.bSetGantry;
		tData.bmm = CString(sm.MotorParameter[iMotor].tHome.szExternalHomeSensorAddress).IsEmpty();
		tData.iAxis = iMotor;
		
		m_AxisView.InsertAxisItem(tData);
	}

	SetWindowText(g_st[IDS_MODULE_AXIS_STATUS_WND_TITLE]);

	m_AxisView.ShowWindow(SW_HIDE);
	return 0;
}

void CModuleAxisStatusWnd::OnShowWindow (BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow (bShow, nStatus);

	if (bShow)
	{
		m_bBeforeAutoRunState = sm.Sequence->fAutoMode;

		int iW = 1920 / 2;
		int iH = 1080 / 6;
		SetWindowPos(&wndTopMost, iW, 100, iW, iH*5, SWP_SHOWWINDOW);
		ComputArea();
		Invalidate(FALSE);
		SetTimer(INTERVAL_MODULE_AXIS_STATUS, 100, nullptr);
	}
	else
	{
		KillTimer(INTERVAL_MODULE_AXIS_STATUS);
	}
}

constexpr unsigned int DrawFontSize = 11;

void CModuleAxisStatusWnd::OnPaint ()
{
	CPaintDC dc(this); 
	CMemDCEx::CMemDC		memDC(&dc);
	memDC.FillSolidRect(m_rectClient, RGB_GRAY_DARK);


	if (auto* pAxisData = m_AxisView.GetSelectAxisData(); pAxisData != nullptr)
	{
		SelectedDrawTextB(memDC, m_rectS[eS_SELECT_MOTOR_NAME], pAxisData->strText, FW_HEAVY, DrawFontSize, DT_PEN, RGB_GRAY_LIGHT, RGB_BLACK);
	}

	// KES 중속 추가 2024.1.30
	switch (m_eSpeed) {
		case eMOTOR_SPEED_LOW: {
			DrawMDIMenuButtonN(memDC, m_rectC[eC_SENSOR_MOVE_SPEED], g_st[IDS_MODULE_AXIS_STATUS_WND_LOW_SPEED_MOVE], false, false, DrawFontSize);
			//SelectedDrawText(memDC, m_rectC[eC_SENSOR_MOVE_SPEED], L"저속 이동", DrawFontSize, DT_PEN, RGB_RED_DARK, RGB_WHITE);
			break;
		}
		case eMOTOR_SPEED_MEDIUM: {
			DrawMDIMenuButtonN(memDC, m_rectC[eC_SENSOR_MOVE_SPEED], g_st[IDS_MODULE_AXIS_STATUS_WND_MIDDLE_SPEED_MOVE], false, false, DrawFontSize, RGB_ORANGE_LIGHT);
			//SelectedDrawText(memDC, m_rectC[eC_SENSOR_MOVE_SPEED], L"고속 이동", DrawFontSize, DT_PEN, RGB_BLUE_DARK, RGB_WHITE);
			break;
		}
		case eMOTOR_SPEED_HIGH: {
			DrawMDIMenuButtonN(memDC, m_rectC[eC_SENSOR_MOVE_SPEED], g_st[IDS_MODULE_AXIS_STATUS_WND_HIGH_SPEED_MOVE], false, false, DrawFontSize, RGB_ORANGE);
			//SelectedDrawText(memDC, m_rectC[eC_SENSOR_MOVE_SPEED], L"고속 이동", DrawFontSize, DT_PEN, RGB_BLUE_DARK, RGB_WHITE);
			break;
		}
	}
	
	if (m_bStepMove)
	{
		DrawMDIMenuButtonN(memDC, m_rectC[eC_STEP_OR_JOG], g_st[IDS_MODULE_AXIS_STATUS_WND_STEP], true, false, DrawFontSize);
		//SelectedDrawText(memDC, m_rectC[eC_STEP_OR_JOG], L"스탭", DrawFontSize, DT_PEN, RGB_GOLDENROD, RGB_BLACK);
		SelectedDrawEditText(memDC, m_rectC[eC_EDIT_STEP_DISTANCE], Format(_T("%.3fmm "), m_dStepDistance), TRUE, DrawFontSize, DT_RIGHT | DT_VCENTER);
	}
	else
	{
		DrawMDIMenuButtonN(memDC, m_rectC[eC_STEP_OR_JOG], g_st[IDS_MODULE_AXIS_STATUS_WND_JOG], false, false, DrawFontSize);
		//SelectedDrawText(memDC, m_rectC[eC_STEP_OR_JOG], L"조그", DrawFontSize, DT_PEN, RGB_VIOLET_DARK, RGB_BLACK);
	}

	DrawMDIMoveDirect(memDC, m_rectC[eC_BTN_MINUS_MOVE], (m_eSelectControl == eC_BTN_MINUS_MOVE), false, eMD_LEFT, L"-");
	DrawMDIMoveDirect(memDC, m_rectC[eC_BTN_PLUS_MOVE], (m_eSelectControl == eC_BTN_PLUS_MOVE), false, eMD_RIGHT, L"+");
	//Draw3dMoveDirect(memDC, m_rectC[eC_BTN_MINUS_MOVE], (m_eSelectControl == eC_BTN_MINUS_MOVE), eMD_LEFT, L"-");
	//Draw3dMoveDirect(memDC, m_rectC[eC_BTN_PLUS_MOVE], (m_eSelectControl == eC_BTN_PLUS_MOVE), eMD_RIGHT, L"+");

	DrawMDIMenuButtonN(&memDC, m_rectC[eC_CLOSE], g_st[IDS_MODULE_AXIS_STATUS_WND_CLOSE], m_eSelectControl == eC_CLOSE, false, DrawFontSize);
	//DrawButtonUpDown(memDC, m_rectC[eC_CLOSE], _T("닫기"), m_eSelectControl == eC_CLOSE, DT_BTN_DOWN_BK, DT_BTN_UP_BK, DrawFontSize);
}


void CModuleAxisStatusWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_eSelectControl != eC_NONE && !m_rectC[m_eSelectControl].PtInRect(point)) 
	{
		switch (m_eSelectControl) 
		{
			case eC_BTN_MINUS_MOVE:
			case eC_BTN_PLUS_MOVE: 
			{
				if (auto* pAxisData = m_AxisView.GetSelectAxisData(); pAxisData != nullptr)
				{
					sm.MotorParameter[pAxisData->iAxis].tYaskawa.iGantryCh = eCH_AXIS_ALL;
					CIPVar var;
					var.SetChildItem("Axis", pAxisData->iAxis);
					var.SetChildItem("Ch", eCH_AXIS_ALL);
					__POST_IP_COMMAND__("Stage", "MoveJogStop", var);
				}
			}
			break;
		}
		m_eSelectControl = eC_NONE;
	}

	CFrameWnd::OnMouseMove(nFlags, point);
}

void CModuleAxisStatusWnd::OnLButtonDown (UINT nFlags, CPoint point)
{
	int iC[] = {
		eC_BTN_MINUS_MOVE,
		eC_BTN_PLUS_MOVE,
	};

	BOOL bNotSafety = FALSE;
	for (int i = 0; i < countof(iC); i++) {
		if (m_rectC[iC[i]].PtInRect(point) && sm.Option->tSequence.eDriveMode != eDRIVE_MODE_MANUAL) {
			bNotSafety = TRUE;
			break;
		}
	}

	if (bNotSafety) {
		IPMessage(g_st[IDS_MS_NOT_DRIVE_MODE_MANUAL]);
		return;
	}

	m_eSelectControl = eC_NONE;
	for(int idx = 0; idx < eC_END; ++idx)
	{
		if (m_rectC[idx].PtInRect(point))
		{
			m_eSelectControl = idx;
			InvalidateRect(m_rectC[idx], FALSE);

			if (m_bStepMove == false && (m_eSelectControl == eC_BTN_MINUS_MOVE || m_eSelectControl == eC_BTN_PLUS_MOVE))
			{
				if (auto* pAxisData = m_AxisView.GetSelectAxisData(); pAxisData != nullptr)
				{
					sm.MotorParameter[pAxisData->iAxis].tYaskawa.iGantryCh = eCH_AXIS_ALL;
					CIPVar var;
					var.SetChildItem("Axis", pAxisData->iAxis);
					// KES 중속 추가 2024.1.30
					var.SetChildItem("Speed", m_eSpeed == eMOTOR_SPEED_HIGH ? sm.MotorParameter[pAxisData->iAxis].dHighSpeed : (m_eSpeed == eMOTOR_SPEED_MEDIUM ? sm.MotorParameter[pAxisData->iAxis].dMediumSpeed : sm.MotorParameter[pAxisData->iAxis].dLowSpeed));
					var.SetChildItem("Dir", m_eSelectControl - eC_BTN_MINUS_MOVE);
					var.SetChildItem("Ch", eCH_AXIS_ALL);
					__POST_IP_COMMAND__("Stage", "MoveJog", var);
				}
			}

			break;
		}
	}

	CFrameWnd::OnLButtonDown (nFlags, point);
}

void CModuleAxisStatusWnd::OnLButtonUp (UINT nFlags, CPoint point)
{
	auto cleanup = make_scope_exit
	(
		[&]()
		{
			m_eSelectControl = eC_NONE;
			InvalidateRect(m_rectC[m_eSelectControl], FALSE);
			CFrameWnd::OnLButtonUp(nFlags, point);
		}
	);

	if (m_eSelectControl <= eC_NONE || m_eSelectControl >= eC_END)
		return;

	if (!m_rectC[m_eSelectControl].PtInRect(point))
		return;

	//<< KES 추가 2024.05.31 - 운전모드가 Manual이 아니면 수동 동작 X
	if (sm.Option->tSequence.eDriveMode != eDRIVE_MODE_MANUAL) {
		if (m_eSelectControl != eC_CLOSE) {
			IPMessage(g_st[IDS_MS_NOT_DRIVE_MODE_MANUAL]);
			return;
		}
	}
	//>>

	switch(m_eSelectControl)
	{
		case eC_STEP_OR_JOG:
		{
			m_bStepMove = !m_bStepMove;
			ComputArea();
		}
		break;

		case eC_SENSOR_MOVE_SPEED:
		{
			m_eSpeed = (eMOTOR_SPEED)((m_eSpeed + 1) % nMOTOR_SPEED);	// KES 중속 추가 2024.1.30
		}
		break;

		case eC_EDIT_STEP_DISTANCE:
		{
			EditStart(this, m_rectC[m_eSelectControl], m_dStepDistance, FALSE, FALSE, -DBL_MAX, DBL_MAX, DT_RIGHT | DT_VCENTER, DrawFontSize);
		}
		break;  

		case eC_BTN_MINUS_MOVE:
		case eC_BTN_PLUS_MOVE:
		{
			if (auto* pAxisData = m_AxisView.GetSelectAxisData(); pAxisData != nullptr)
			{
				sm.MotorParameter[pAxisData->iAxis].tYaskawa.iGantryCh = eCH_AXIS_ALL;
				
				CIPVar var;
				var.SetChildItem("Axis", pAxisData->iAxis);
				var.SetChildItem("Ch", eCH_AXIS_ALL);

				// 스탭 이동
				if (m_bStepMove) 
				{
					// KES 중속 추가 2024.1.30
					var.SetChildItem("Speed", m_eSpeed == eMOTOR_SPEED_HIGH ? sm.MotorParameter[pAxisData->iAxis].dHighSpeed : (m_eSpeed == eMOTOR_SPEED_MEDIUM ? sm.MotorParameter[pAxisData->iAxis].dMediumSpeed : sm.MotorParameter[pAxisData->iAxis].dLowSpeed));
					var.SetChildItem("Distacne", m_eSelectControl != eC_BTN_MINUS_MOVE ? m_dStepDistance : m_dStepDistance * -1);
					__POST_IP_COMMAND__("Stage", "MoveStep", var);
				}
				// 조그 이동 정지
				else 
				{
					__POST_IP_COMMAND__("Stage", "MoveJogStop", var);
				}
			}
		}
		break;

		case eC_CLOSE:
		{
			ShowWindow(SW_HIDE);
		}
		break;

		default: break;
	}
}

void CModuleAxisStatusWnd::OnTimer (UINT_PTR nIDEvent)
{
	if (nIDEvent == INTERVAL_MODULE_AXIS_STATUS)
	{
		if (m_bBeforeAutoRunState != sm.Sequence->fAutoMode)
		{
			m_bBeforeAutoRunState = sm.Sequence->fAutoMode;
			ComputArea();
			Invalidate(FALSE);
		}

		for (int iMotor = 0; iMotor < eMOTOR_END; ++iMotor)
		{
			auto& tMotor = sm.MotorStatus[iMotor].tCrt;
			auto& tMotorEtc = sm.MotorStatus[iMotor].tEtc;
			m_AxisView.SetAxisData(iMotor, tMotor.dSpeed, tMotor.dPos, tMotor.dTorque);

			if (sm.MotorParameter[iMotor].tYaskawa.bSetGantry)
			{
				m_AxisView.SetSubAxisData(iMotor, tMotor.dSpeed, tMotor.tGantryData.dCh2Pos, tMotor.tGantryData.dCh2Torque);
				BOOL IsServoOn[2];
				BOOL IsAlarm[2];
				BOOL CheckPlusLimit[2];
				BOOL CheckMinusLimit[2];
				BOOL CheckInPosition[2];
				BOOL CheckHomeSensor[2];

				for (int idx = 0; idx < 2; ++idx)
				{
					DWORD STATE = idx == 0 ? eCH_GANTRY_AXIS_1_STATE : eCH_GANTRY_AXIS_2_STATE;
					IsServoOn[idx]			= tMotor.tGantryData.dwChServoOnStatus & STATE;
					IsAlarm[idx]			= tMotor.tGantryData.dwChAlarmOnStatus & STATE;
					CheckPlusLimit[idx]		= tMotor.tGantryData.dwChPLimitStatus & STATE;
					CheckMinusLimit[idx]	= tMotor.tGantryData.dwChNLimitStatus & STATE;
					CheckInPosition[idx]	= tMotor.tGantryData.dwChInPosStatus & STATE;
					CheckHomeSensor[idx]	= tMotor.tGantryData.dwChHomeStatus & STATE;
				}
				
				m_AxisView.SetAxisSensor(iMotor, IsServoOn[0], tMotorEtc.dwHomeStatus == eHOME_STATUS_COMPLETED, tMotorEtc.dwHomeStatus == eHOME_STATUS_HOMMING, IsAlarm[0], CheckPlusLimit[0], CheckMinusLimit[0], CheckInPosition[0], CheckHomeSensor[0]);
				m_AxisView.SetSubAxisSensor(iMotor, IsServoOn[1], IsAlarm[1], CheckPlusLimit[1], CheckMinusLimit[1], CheckInPosition[1], CheckHomeSensor[1]);
			}
			else
			{
				m_AxisView.SetAxisSensor(iMotor, tMotor.bServoOn, tMotorEtc.dwHomeStatus == eHOME_STATUS_COMPLETED, tMotorEtc.dwHomeStatus == eHOME_STATUS_HOMMING,tMotor.bAlarm, tMotor.bPLimit, tMotor.bNLimit, tMotor.bInPosition, tMotor.bHomeSensor);
			}
		}
	}
	
	CFrameWnd::OnTimer (nIDEvent);
}

void CModuleAxisStatusWnd::OnClose()
{
	ShowWindow(SW_HIDE);
}


BEGIN_IP_COMMAND_TABLE(CModuleAxisStatusWnd, IIPCommandTarget)
	ON_IP("AxisStatusControl", "Show", OnIP_Show)
END_IP_COMMAND_TABLE()

BOOL CModuleAxisStatusWnd::OnIP_Show(CIPCommand& cmd) 
{
	const CIPVar& var = cmd.m_var;

	BOOL bShow = var.GetChildItem(_T("Show"), TRUE);

	if (bShow) 
	{
		ShowWindow(SW_SHOW);
		//SetForegroundWindow();
		m_AxisView.SetFocus();
	}
	else
	{
		ShowWindow(SW_HIDE);
	}

	return TRUE;
}
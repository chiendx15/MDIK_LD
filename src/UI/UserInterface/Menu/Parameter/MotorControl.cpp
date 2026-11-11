#include "pch.h"
#include "UI.h"
#include "./MotorControl.h"

#define ID_MOTOR_LIST	10002

IMPLEMENT_DYNCREATE		(CMotorControl, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CMotorControl, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()



CMotorControl::CMotorControl(CWnd* pwndParent, int iMotorNo) {
	m_pwndParent		= pwndParent;
	m_eMouseOver		= eC_NONE;
	m_eSelectControl	= eC_NONE;
	m_dSpeed			= 0.;
	m_dPosition			= 0.;
	memset(&m_tPos, 0x00, sizeof(T_POS));
}

CMotorControl::~CMotorControl() {
}



BOOL CMotorControl::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}






int CMotorControl::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {
	
	}
	catch (...) {
		return -1;
	}

	return 0;
}



void CMotorControl::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case INTERVAL_UI: {
			UpdataMotorState();
			break;
		}
		default:
			break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CMotorControl::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		SetTimer(INTERVAL_UI, 100, NULL);
	}
	else {
		KillTimer(INTERVAL_UI);
	}
}


BOOL CMotorControl::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CMotorControl::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CRect				rect = m_rectS[eS_FULL_AREA];

	memDC.FillSolidRect(rect, RgbColor::Gainsboro);
	DrawLineRect(&memDC, m_rectS[eS_FULL_AREA], RGB_BLUE);

	memDC.FillSolidRect(m_rectS[eS_TITLE], RgbColor::Gainsboro);
	DrawLineRect(memDC, m_rectS[eS_TITLE], RGB_BLUE);

	CString strMotorName = Format(_T("%s %s"), m_strMotorName, g_stMotor[m_tPos.iAxis]);
	DrawText(memDC, m_rectS[eS_MOTOR_NAME], strMotorName, 11, RGB_BLACK, DT_LEFT | DT_VCENTER);

	DrawText(memDC, m_rectS[eS_POSITION], _T("Pos:\n(mm)"), 9, RGB_BLACK, DT_LEFT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_VELOCITY], _T("Spd:\n(mm/s)"), 9, RGB_BLACK, DT_LEFT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_TORQUE], _T("TQ:\n(%)"), 9, RGB_BLACK, DT_LEFT | DT_VCENTER);

	DrawText(memDC, m_rectS[eS_SET_POSITION_UNIT], g_st[IDS_MOTOR_CONTROL_POSITION_UNIT], 10, RGB_BLACK, DT_LEFT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_SET_VELOCITY_UNIT], g_st[IDS_MOTOR_CONTROL_VELOCITY_UNIT], 10, RGB_BLACK, DT_LEFT | DT_VCENTER);

	CString str = g_st[IDS_POSITION_PARAMETER_SET_POS];
	str += _T(" : ");
	DrawText(memDC, m_rectS[eS_SET_POSITION], str, 10, RGB_BLACK, DT_CENTER | DT_VCENTER);

	str = g_st[IDS_POSITION_PARAMETER_SET_VELOCITY];
	str += _T(" : ");
	DrawText(memDC, m_rectS[eS_SET_VELOCITY], str, 10, RGB_BLACK, DT_CENTER | DT_VCENTER);

	if (sm.MotorStatus[m_tPos.iAxis].tCrt.bServoOn)	//상태받아서 처리
		SelectedDrawText(memDC, m_rectS[eS_SERVO], _T("Servo\nOn"), 9, g_colorForeground[eEC_MOUSE_UP], RGB_GREEN_DARK, RGB_WHITE, DT_CENTER | DT_VCENTER);
	else
		SelectedDrawText(memDC, m_rectS[eS_SERVO], _T("Servo\nOff"), 9, g_colorForeground[eEC_MOUSE_UP], RGB_RED_DARK, RGB_WHITE, DT_CENTER | DT_VCENTER);

	if (sm.MotorStatus[m_tPos.iAxis].tEtc.dwHomeStatus == eHOME_STATUS_COMPLETED) {
		SelectedDrawText(memDC, m_rectS[eS_ORG], _T("Origin"), 9, g_colorForeground[eEC_MOUSE_UP], RGB_GREEN, RGB_BLACK, DT_CENTER | DT_VCENTER);
	}
	else if (sm.MotorStatus[m_tPos.iAxis].tEtc.dwHomeStatus == eHOME_STATUS_HOMMING) {
		SelectedDrawText(memDC, m_rectS[eS_ORG], _T("Origin"), 9, RGB_WHITE, RGB_YELLOW, RGB_BLACK, DT_CENTER | DT_VCENTER);
	}
	else if (sm.MotorStatus[m_tPos.iAxis].tEtc.dwHomeStatus == eHOME_STATUS_NOT_COMPLETED) {
		SelectedDrawText(memDC, m_rectS[eS_ORG], _T("Origin"), 9, RGB_WHITE, RGB_RED_DARK, RGB_WHITE, DT_CENTER | DT_VCENTER);
	}

	COLORREF backColor;
	for (int i = 0; i <= eS_INPOS - eS_HOME; i++) {
		i == 1 ? backColor = RGB_RED_DARK : backColor = RGB_GREEN_DARK;
		if (m_bMotorStatus[i] == TRUE)
			SelectedDrawText(memDC, m_rectS[eS_HOME + i], g_st[IDS_MOTOR_CONTROL_HOME + i], 9, g_colorForeground[eEC_MOUSE_UP], backColor, RGB_WHITE, DT_CENTER | DT_VCENTER);
		else
			SelectedDrawText(memDC, m_rectS[eS_HOME + i], g_st[IDS_MOTOR_CONTROL_HOME + i], 9, RGB_WHITE, RGB_GRAY_DARK, RGB_WHITE, DT_CENTER | DT_VCENTER);
	}

	if (sm.MotorParameter[m_tPos.iAxis].tYaskawa.bSetGantry) {
		SelectedDrawDouble(memDC, m_rectS[eS_POSITION_VAL], sm.MotorStatus[m_tPos.iAxis].tCrt.tGantryData.dCh1Pos, 3, 12, RGB_WHITE, RGB_GREEN_DARK, RGB_BLACK, DT_RIGHT | DT_VCENTER);
		SelectedDrawDouble(memDC, m_rectS[eS_POSITION_VAL_2], sm.MotorStatus[m_tPos.iAxis].tCrt.tGantryData.dCh2Pos, 3, 12, RGB_WHITE, RGB_SKY_LIGHT, RGB_BLACK, DT_RIGHT | DT_VCENTER);
		SelectedDrawDouble(memDC, m_rectS[eS_TORQUE_VAL], sm.MotorStatus[m_tPos.iAxis].tCrt.tGantryData.dCh1Torque, 3, 12, RGB_WHITE, RGB_GREEN_DARK, RGB_BLACK, DT_RIGHT | DT_VCENTER);
		SelectedDrawDouble(memDC, m_rectS[eS_TORQUE_VAL_2], sm.MotorStatus[m_tPos.iAxis].tCrt.tGantryData.dCh2Torque, 3, 12, RGB_WHITE, RGB_SKY_LIGHT, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	}
	else {
		SelectedDrawDouble(memDC, m_rectS[eS_POSITION_VAL], m_tPos.dTarget, 3, 12, RGB_WHITE, RGB_GRAY_LIGHT, RGB_BLACK, DT_RIGHT | DT_VCENTER);
		SelectedDrawDouble(memDC, m_rectS[eS_TORQUE_VAL], sm.MotorStatus[m_tPos.iAxis].tCrt.dTorque, 3, 12, RGB_WHITE, RGB_GRAY_LIGHT, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	}
	SelectedDrawDouble(memDC, m_rectS[eS_VELOCITY_VAL], m_tPos.dSpeed, 3, 12, RGB_WHITE, RGB_GRAY_LIGHT, RGB_BLACK, DT_RIGHT | DT_VCENTER);


	SelectedDrawEditDouble(memDC, m_rectC[eC_EDIT_POSITION], m_dPosition, 3, TRUE, 12, DT_RIGHT | DT_VCENTER);
	SelectedDrawEditDouble(memDC, m_rectC[eC_EDIT_VELOCITY], m_dSpeed, 3, TRUE, 12, DT_RIGHT | DT_VCENTER);

	DrawMDIMenuButton(memDC, m_rectC[eC_ABS_MOVE], g_st[IDS_MOTOR_CONTROL_ABS_MOVE], m_eSelectControl == eC_ABS_MOVE, m_eMouseOver == eC_ABS_MOVE);
	DrawMDIMenuButton(memDC, m_rectC[eC_HOME], g_st[IDS_MOTOR_CONTROL_HOME], m_eSelectControl == eC_HOME, m_eMouseOver == eC_HOME);
	DrawMDIMenuButton(memDC, m_rectC[eC_ESTOP], g_st[IDS_MOTOR_CONTROL_ESTOP], m_eSelectControl == eC_ESTOP, m_eMouseOver == eC_ESTOP);

	DrawMDIMoveDirect(memDC, m_rectC[eC_REL_NEG], m_eSelectControl == eC_REL_NEG, m_eMouseOver == eC_REL_NEG, eMD_LEFT, g_st[IDS_MOTOR_CONTROL_REL_NEG]);
	DrawMDIMoveDirect(memDC, m_rectC[eC_REL_POS], m_eSelectControl == eC_REL_POS, m_eMouseOver == eC_REL_POS, eMD_RIGHT, g_st[IDS_MOTOR_CONTROL_REL_POS]);
	DrawMDIMoveDirect(memDC, m_rectC[eC_JOG_NEG], m_eSelectControl == eC_JOG_NEG, m_eMouseOver == eC_JOG_NEG, eMD_LEFT, g_st[IDS_MOTOR_CONTROL_JOG_NEG]);
	DrawMDIMoveDirect(memDC, m_rectC[eC_JOG_POS], m_eSelectControl == eC_JOG_POS, m_eMouseOver == eC_JOG_POS, eMD_RIGHT, g_st[IDS_MOTOR_CONTROL_JOG_POS]);
}



void CMotorControl::OnMouseMove(UINT nFlags, CPoint point) {
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



void CMotorControl::OnLButtonDown(UINT nFlags, CPoint point) {
	int iC[] = {
		eC_JOG_NEG,
		eC_JOG_POS,
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
	
	SetFocus();
	m_eSelectControl = eC_NONE;
	for (int i = 0; i<eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			if (m_eSelectControl == eC_JOG_NEG || m_eSelectControl == eC_JOG_POS) {
				BOOL bDirection = m_eSelectControl == eC_JOG_NEG ? bDirection = 0 : bDirection = 1;
				JogMove((eAxis)m_tPos.iAxis, bDirection, m_dSpeed);
			}
			InvalidateRect(m_rectC[i],FALSE);
			break;
		}
	}
	CFrameWndEx::OnLButtonDown(nFlags, point);
}



void CMotorControl::OnLButtonUp(UINT nFlags, CPoint point)  {	
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		//<< KES 추가 2024.4.9 - 운전모드가 Manual이 아니면 수동 동작 X
		if (sm.Option->tSequence.eDriveMode != eDRIVE_MODE_MANUAL) {
			IPMessage(g_st[IDS_MS_NOT_DRIVE_MODE_MANUAL]);
			return;
		}
		//>>	

		switch (m_eSelectControl) {
			case eC_JOG_NEG:	
			case eC_JOG_POS:{
				JogStop((eAxis)m_tPos.iAxis);
				break;
			}
			case eC_ESTOP: {
				PTPStop((eAxis)m_tPos.iAxis);
				break;
			}
			case eC_ABS_MOVE: {
				PTPMove((eAxis)m_tPos.iAxis, m_dPosition, m_dSpeed, nullptr);
				break;
			}
			case eC_REL_NEG: 
			case eC_REL_POS: {
				double dTarget;
				m_eSelectControl == eC_REL_NEG ? dTarget = m_dPosition * -1 : dTarget = m_dPosition;
				StepMove((eAxis)m_tPos.iAxis, dTarget, m_dSpeed, nullptr);
				break;
			}
			case eC_HOME: {
				// Home 관련 함수 나중에 추가 
				break;
			}
			case eC_EDIT_POSITION: {
				EditStart(this, m_rectC[m_eSelectControl], m_dPosition);
				break;
			}
			case eC_EDIT_VELOCITY: {
				EditStart(this, m_rectC[m_eSelectControl], m_dSpeed);
				break;
			}

		}
	}
	m_eSelectControl	=	eC_NONE;
	Invalidate(FALSE);

	CFrameWndEx::OnLButtonUp(nFlags, point);
}



void CMotorControl::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect rect = m_rectS[eS_FULL_AREA];
	
	m_rectS[eS_TITLE]		= CRect(rect.left,	rect.top,		rect.right,	rect.top+35);	
	m_rectS[eS_ITEM_AREA]	= CRect(rect.left+5,	rect.top+40,	rect.right-5,	rect.bottom);

	rect = m_rectS[eS_TITLE];
	double dW = rect.Width();
	double dH = rect.Height();

	m_rectS[eS_MOTOR_NAME]	= CRect(rect.left+dW*0+5,		rect.top,	rect.right,		rect.bottom);
	
	rect = m_rectS[eS_ITEM_AREA];
	dW = (rect.Width() - 5.) / 6;
	dH = (rect.Height() - 5.);

	m_rectS[eS_SET_AREA]		= CRect(rect.left+5+dW*0,		rect.top+5+dH*0,	rect.left+dW*3,	rect.top+dH*1);
	m_rectS[eS_MOVE_AREA]		= CRect(rect.left+5+dW*3,		rect.top+5+dH*0,	rect.left+dW*6,	rect.top+dH*1);

	rect = m_rectS[eS_SET_AREA];
	dW = (rect.Width() - 5.) / 9;
	dH = (rect.Height() - 5.) / 4;
	if (sm.MotorParameter[m_tPos.iAxis].tYaskawa.bSetGantry) {
		m_rectS[eS_POSITION_VAL]		= CRect(rect.left+5+dW*1.2,		rect.top+5+dH*0,	rect.left+dW*3,		rect.top+dH*1);	
		m_rectS[eS_POSITION_VAL_2]		= CRect(rect.left+5+dW*1.2,		rect.top+5+dH*1,	rect.left+dW*3,		rect.top+dH*2);

		m_rectS[eS_TORQUE_VAL]			= CRect(rect.left+5+dW*7.2,		rect.top+5+dH*0,	rect.left+dW*9,		rect.top+dH*1);	
		m_rectS[eS_TORQUE_VAL_2]		= CRect(rect.left+5+dW*7.2,		rect.top+5+dH*1,	rect.left+dW*9,		rect.top+dH*2);
	}
	else {
		m_rectS[eS_POSITION_VAL]		= CRect(rect.left+5+dW*1.2,		rect.top+5+dH*0.5,	rect.left+dW*3,		rect.top+dH*1.5);	
		m_rectS[eS_TORQUE_VAL]			= CRect(rect.left+5+dW*7.2,		rect.top+5+dH*0.5,	rect.left+dW*9,		rect.top+dH*1.5);
	}
	m_rectS[eS_POSITION]			= CRect(rect.left+5+dW*0,		rect.top+5+dH*0,	rect.left+dW*1.2,	rect.top+dH*2);	
	m_rectS[eS_VELOCITY]			= CRect(rect.left+5+dW*3,		rect.top+5+dH*0,	rect.left+dW*4.2,	rect.top+dH*2);
	m_rectS[eS_VELOCITY_VAL]		= CRect(rect.left+5+dW*4.2,		rect.top+5+dH*0.5,	rect.left+dW*6,		rect.top+dH*1.5);
	m_rectS[eS_TORQUE]				= CRect(rect.left+5+dW*6,		rect.top+5+dH*0,	rect.left+dW*7.2,	rect.top+dH*2);
	

	m_rectS[eS_SET_POSITION]		= CRect(rect.left+5+dW*0,		rect.top+5+dH*2,	rect.left+dW*4,		rect.top+dH*3);	
	m_rectC[eC_EDIT_POSITION]		= CRect(rect.left+5+dW*4,		rect.top+5+dH*2,	rect.left+dW*7,		rect.top+dH*3);	
	m_rectS[eS_SET_POSITION_UNIT]	= CRect(rect.left+5+dW*7,		rect.top+5+dH*2,	rect.left+dW*9,		rect.top+dH*3);	
	m_rectS[eS_SET_VELOCITY]		= CRect(rect.left+5+dW*0,		rect.top+5+dH*3,	rect.left+dW*4,		rect.top+dH*4);
	m_rectC[eC_EDIT_VELOCITY]		= CRect(rect.left+5+dW*4,		rect.top+5+dH*3,	rect.left+dW*7,		rect.top+dH*4);	
	m_rectS[eS_SET_VELOCITY_UNIT]	= CRect(rect.left+5+dW*7,		rect.top+5+dH*3,	rect.left+dW*9,		rect.top+dH*4);

	rect = m_rectS[eS_MOVE_AREA];
	dW = (rect.Width() - 5.) / 9;
	dH = (rect.Height() - 5.) / 3;

	//m_rectC[eC_SERVO_ON]		= CRect(rect.left+20+dW*0,		rect.top+5+dH*0,	rect.left+20+dW*1,		rect.top+dH*1);
	//m_rectC[eC_SERVO_OFF]		= CRect(rect.left+25+dW*1,		rect.top+5+dH*0,	rect.left+25+dW*2,		rect.top+dH*1);

	m_rectS[eS_SERVO]			= CRect(rect.left+dW*2,			rect.top+5+dH*0,		rect.left+dW*3,		rect.top+dH*1);
	m_rectS[eS_ORG ]			= CRect(rect.left+dW*3,			rect.top+5+dH*0,		rect.left+dW*4,		rect.top+dH*1);
	m_rectS[eS_HOME]			= CRect(rect.left+dW*4,			rect.top+5+dH*0,		rect.left+dW*5,		rect.top+dH*1);
	m_rectS[eS_ALARM]			= CRect(rect.left+dW*5,			rect.top+5+dH*0,		rect.left+dW*6,		rect.top+dH*1);
	m_rectS[eS_LIMIT_POS]		= CRect(rect.left+dW*6,			rect.top+5+dH*0,		rect.left+dW*7,		rect.top+dH*1);
	m_rectS[eS_LIMIT_NED]		= CRect(rect.left+dW*7,			rect.top+5+dH*0,		rect.left+dW*8,		rect.top+dH*1);
	m_rectS[eS_INPOS]			= CRect(rect.left+dW*8,			rect.top+5+dH*0,		rect.left+dW*9,		rect.top+dH*1);

	
	dW = (rect.Width() - 10.) / 7;
	dH = (rect.Height() - 10.) / 7;

	m_rectC[eC_ABS_MOVE]		= CRect(rect.left+10+dW*0,		rect.top+10+dH*3,	rect.left+dW*1,		rect.top+dH*7);
	m_rectC[eC_REL_NEG]			= CRect(rect.left+10+dW*1,		rect.top+10+dH*3,	rect.left+dW*2,		rect.top+dH*7);
	m_rectC[eC_REL_POS]			= CRect(rect.left+10+dW*2,		rect.top+10+dH*3,	rect.left+dW*3,		rect.top+dH*7);
	m_rectC[eC_JOG_NEG]			= CRect(rect.left+10+dW*3,		rect.top+10+dH*3,	rect.left+dW*4,		rect.top+dH*7);
	m_rectC[eC_JOG_POS]			= CRect(rect.left+10+dW*4,		rect.top+10+dH*3,	rect.left+dW*5,		rect.top+dH*7);
	m_rectC[eC_HOME]			= CRect(rect.left+10+dW*5,		rect.top+10+dH*3,	rect.left+dW*6,		rect.top+dH*7);
	m_rectC[eC_ESTOP]			= CRect(rect.left+10+dW*6,		rect.top+10+dH*3,	rect.left+dW*7,		rect.top+dH*7);

}

void CMotorControl::SetMotorInfo(T_POS Pos) {
	m_tPos.iAxis = Pos.iAxis;

	//<< KES 추가 2024.5.1 - 티칭 축 선택하면 티칭위치, 티칭속도 정보 세팅
	m_dPosition = Pos.dTarget;
	m_dSpeed = Pos.dSpeed;
	//>>

	m_strMotorName = Format(_T("[ Motor %d ]"), m_tPos.iAxis+1);
	//	m_strMotorName += g_st[IDS_MOTOR_X1]; // 나중에 Motor 이름 추가
	fn_ComputeArea();
	//InvalidateRect(m_rectS[eS_MOTOR_NAME]);
	Invalidate(FALSE);
}
void CMotorControl::UpdataMotorState() {

	m_tPos.dTarget	= sm.MotorStatus[m_tPos.iAxis].tCrt.dPos;
	m_tPos.dSpeed	= sm.MotorStatus[m_tPos.iAxis].tCrt.dSpeed;

	m_bMotorStatus[0] = sm.MotorStatus[m_tPos.iAxis].tCrt.bHomeSensor;
	m_bMotorStatus[1] = sm.MotorStatus[m_tPos.iAxis].tCrt.bAlarm;
	m_bMotorStatus[2] = sm.MotorStatus[m_tPos.iAxis].tCrt.bPLimit;
	m_bMotorStatus[3] = sm.MotorStatus[m_tPos.iAxis].tCrt.bNLimit;
	m_bMotorStatus[4] = sm.MotorStatus[m_tPos.iAxis].tCrt.bInPosition;

	Invalidate(FALSE);
}
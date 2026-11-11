#include "pch.h"
#include "UI.h"
#include "ConveyorControl.h"

#define INTERVAL_UI	9000


IMPLEMENT_DYNCREATE		(CConveyorControl, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CConveyorControl, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()



CConveyorControl::CConveyorControl(int iConveyorNo) {
	m_eMouseOver		= eC_NONE;
	m_eSelectControl	= eC_NONE;

	m_iConveyor = iConveyorNo;
	m_eModule = (eMODULEs)(eMODULE_PC1_LOAD_CONVEYOR_1 + m_iConveyor);

	//m_eConveyorSpeed = eConveyorMoveType::LowSpeed;	// KES 중속 추가 2024.2.15
}

CConveyorControl::~CConveyorControl() {
}



BOOL CConveyorControl::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}


int CConveyorControl::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}



void CConveyorControl::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case INTERVAL_UI: {
			Invalidate(FALSE);
			break;
		}
		default:
			break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CConveyorControl::OnShowWindow(BOOL bShow, UINT nStatus) {
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


BOOL CConveyorControl::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}

eMODULEs GetConveorModule(int iIndex) {
	eMODULEs module;
	switch (iIndex) {
		case eCONVEYOR_LD_1:	module = eMODULE_PC1_LOAD_CONVEYOR_1;			break;
		case eCONVEYOR_LD_2:	module = eMODULE_PC1_LOAD_CONVEYOR_2;			break;
		case eCONVEYOR_EL:		module = eMODULE_PC1_ELEVATOR_CONVEYOR;			break;
		case eCONVEYOR_ULD_1:	module = eMODULE_PC1_UNLOAD_CONVEYOR_1;			break;
		case eCONVEYOR_ULD_2:	module = eMODULE_PC1_UNLOAD_CONVEYOR_2;			break;
	}
	return module;
}

void CConveyorControl::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CRect rect = m_rectS[eS_FULL_AREA];
	CDrawSymbol::DrawLineRect(&memDC, m_rectS[eS_FULL_AREA], RGB_SKY);
	rect.DeflateRect(1,1);
	//memDC.FillSolidRect(rect, RgbColor::Ivory);
	memDC.FillSolidRect(rect, RgbColor::LightGray);
	memDC.FillSolidRect(m_rectS[eS_TITLE], RGB_GRAY);
	DrawLineRect(memDC, m_rectS[eS_TITLE], RGB_SKY);

	CString strConveyorName = g_stMotor[IDS_CONVEYOR_NAME_01 + m_iConveyor];
	DrawText(memDC, m_rectS[eS_CONVEYOR_NAME], strConveyorName, 11, RGB_BLACK, DT_LEFT | DT_VCENTER);
	
	auto ConveyorParam = sm.Conveyor_Parameter[m_iConveyor];
	BOOL bConnect = sm.SerialConnection->bInverter[m_iConveyor];
	BOOL bRun = Serial::Inverter::GetInverterRun(m_eModule);
	BOOL bFwd = Serial::Inverter::GetInverterDirection(m_eModule, !ConveyorParam.bForward);
	BOOL bStop = Serial::Inverter::GetInverterStop(m_eModule);
	BOOL bReady = Serial::Inverter::GetInverterReady(m_eModule);
	double dSpeed = Serial::Inverter::GetInverterFrequency(m_eModule) / sm.Conveyor_Parameter[m_iConveyor].dSpeedRate;

	DrawMDIMenuButtonN(&memDC, m_rectC[eC_CONVEYOR_JOG], m_bJog ? _T("Jog") : _T("Move"), m_eSelectControl == eC_CONVEYOR_JOG);

	DrawMDIMenuButtonN(&memDC, m_rectC[eC_CONVEYOR_BTN_CONNECT], _T("Connect"), m_eSelectControl == eC_CONVEYOR_BTN_CONNECT || bConnect);
	DrawMDIMenuButtonN(&memDC, m_rectC[eC_CONVEYOR_BTN_READY], _T("Ready"), m_eSelectControl == eC_CONVEYOR_BTN_READY || bReady);

	// KES 중속 추가 2024.2.15
	SelectedDrawText(&memDC, m_rectC[eC_CONVEYOR_BTN_SPEED_TYPE], m_bSpeedHigh ? _T("High") : _T("Low") , 9, RGB_BLACK, RGB_GRAY_LIGHT , RGB_BLACK, DT_CENTER | DT_VCENTER);

	DrawMDIMoveDirect(memDC, m_rectC[eC_CONVEYOR_BTN_MINUS], m_eSelectControl == eC_CONVEYOR_BTN_MINUS || (bRun && bFwd), FALSE, eMD_LEFT, _T(" - "));
	DrawMDIMoveDirect(memDC, m_rectC[eC_CONVEYOR_BTN_PLUS], m_eSelectControl == eC_CONVEYOR_BTN_PLUS || (bRun && !bFwd), FALSE, eMD_RIGHT, _T(" + "));
	DrawMDIMenuButtonN(memDC, m_rectC[eC_CONVEYOR_BTN_STOP], _T("Stop"), m_eSelectControl == eC_CONVEYOR_BTN_STOP, 2);

	DrawText(memDC, m_rectS[eS_CURRENT_SPEED], _T("Current Speed:"), DT_FONTSIZE, RGB_BLACK);
	SelectedDrawInt(&memDC, m_rectC[eC_CURRENT_SPEED], dSpeed, DT_FONTSIZE, RGB_WHITE, RGB_GRAY, RGB_WHITE);
	
	if (bRun) {
		if (m_bSpeedHigh) {
			DrawProgressBar(memDC, m_rectS[eS_PROGRESS_AREA], bFwd ? 0 : 1, m_ulStartTime, 200, 40, RGB_WHITE, RGB_GRAY, RGB_GREEN);
		}
		else {
			DrawProgressBar(memDC, m_rectS[eS_PROGRESS_AREA], bFwd ? 0 : 1, m_ulStartTime, 200, 20, RGB_WHITE, RGB_GRAY, RGB_GREEN);
		}
	}
	else {
		DrawLineRect(memDC, m_rectS[eS_PROGRESS_AREA], RGB_WHITE);
	}
}


void CConveyorControl::OnMouseMove(UINT nFlags, CPoint point) {
	
	if (!m_rectC[m_eSelectControl].PtInRect(point)){
		switch( m_eSelectControl){
			case eC_CONVEYOR_BTN_PLUS:
			case eC_CONVEYOR_BTN_MINUS: {
				Serial::Inverter::SetInverterStop((eMODULEs)(eMODULE_PC1_LOAD_CONVEYOR_1 + m_iConveyor));
				break;
			}
		}
		m_eSelectControl = eC_NONE;
	}
		
	CFrameWndEx::OnMouseMove(nFlags, point);
}



void CConveyorControl::OnLButtonDown(UINT nFlags, CPoint point) {
	SetFocus();
	m_eSelectControl = eC_NONE;
	for (int i = 0; i < eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			switch (m_eSelectControl) {
				case eC_CONVEYOR_BTN_MINUS: {
					if (m_bJog) {
						m_ulStartTime = GetTickCount();
						ConveyorMove(!sm.Conveyor_Parameter[m_iConveyor].bForward);
					}
					break;
				}
				case eC_CONVEYOR_BTN_PLUS: {
					if (m_bJog) {
						// Mute 감지 알람뜨는 경우 동작 X 
						m_ulStartTime = GetTickCount();
						ConveyorMove(sm.Conveyor_Parameter[m_iConveyor].bForward);
					}
					break;
				}
			}
			InvalidateRect(m_rectC[i], FALSE);
		}
	}

	CFrameWndEx::OnLButtonDown(nFlags, point);
}



void CConveyorControl::OnLButtonUp(UINT nFlags, CPoint point) {
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		//<< KES 추가 2024.4.9 - 운전모드가 Manual이 아니면 수동 동작 X
		if (sm.Option->tSequence.eDriveMode != eDRIVE_MODE_MANUAL) {
			IPMessage(g_st[IDS_MS_NOT_DRIVE_MODE_MANUAL]);
			return;
		}
		//>>	

		switch (m_eSelectControl) {
			case eC_CONVEYOR_JOG: {
				strLog = _T("Jog/Move");
				Serial::Inverter::SetInverterStop((eMODULEs)(eMODULE_PC1_LOAD_CONVEYOR_1 + m_iConveyor));
				m_bJog = !m_bJog;
				break;
			}
			case eC_CONVEYOR_BTN_SPEED_TYPE: {
				strLog = _T("Speed Type");
				m_bSpeedHigh = !m_bSpeedHigh;
				break;
			}
			case eC_CONVEYOR_BTN_MINUS: {
				strLog = _T("Conveyor Move Minus");
				if (m_bJog) {
					Serial::Inverter::SetInverterStop((eMODULEs)(eMODULE_PC1_LOAD_CONVEYOR_1 + m_iConveyor));
					m_ulStartTime = 0;
				}
				else {
					eALARM_ID eAlarm =  eAI_GLASS_BOX_CV_LIGHT_CURTAIN_NORMAL;
					m_ulStartTime = GetTickCount64();
					ConveyorMove(!sm.Conveyor_Parameter[m_iConveyor].bForward);
				}
				break;
				
			}
			case eC_CONVEYOR_BTN_STOP: {
				strLog = _T("Conveyor Stop");
				Serial::Inverter::SetInverterStop((eMODULEs)(eMODULE_PC1_LOAD_CONVEYOR_1 + m_iConveyor));
				m_ulStartTime = 0;
				break;

			}
			case eC_CONVEYOR_BTN_PLUS: {
				strLog = _T("Conveyor Move Plus");
				if (m_bJog) {
					Serial::Inverter::SetInverterStop((eMODULEs)(eMODULE_PC1_LOAD_CONVEYOR_1 + m_iConveyor));
					m_ulStartTime = 0;
				}
				else {
					eALARM_ID eAlarm =  eAI_GLASS_BOX_CV_LIGHT_CURTAIN_NORMAL;
					m_ulStartTime = GetTickCount();
					ConveyorMove(sm.Conveyor_Parameter[m_iConveyor].bForward);
				}
				break;
			}
			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], g_stMotor[IDS_CONVEYOR_NAME_01 + m_iConveyor], strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl = eC_NONE;
	Invalidate(FALSE);
	CFrameWndEx::OnLButtonUp(nFlags, point);
}


void CConveyorControl::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect rect = m_rectS[eS_FULL_AREA];
	double dW;
	double dH = rect.Height() / 6.;

	m_rectS[eS_TITLE]			= CRect(rect.left,		rect.top + dH * 0,		rect.right,		rect.top + dH * 1);	
	m_rectS[eS_ITEM_AREA]		= CRect(rect.left+5,	rect.top + 5 + dH * 1,	rect.right-5,	rect.top + dH * 5);	
	m_rectS[eS_PROGRESS_AREA]	= CRect(rect.left+5,	rect.top + 5 + dH * 5,	rect.right-5,	rect.top + dH * 6-5);				
	rect = m_rectS[eS_TITLE];
	dW = rect.Width() / 4;
	dH = (rect.Height() - 5) / 2.;

	m_rectS[eS_CONVEYOR_NAME]	= CRect(rect.left+dW * 0 + 5,	rect.top,				rect.right,				rect.bottom);
	m_rectC[eC_CONVEYOR_JOG	]	= CRect(rect.left+dW * 3,	rect.top+ 2 + dH * 0,	rect.left+dW * 4,	rect.top + dH * 2 + 3);
	
	rect = m_rectS[eS_ITEM_AREA];
	dH = (rect.Height() - 5) / 2.;
	dW = (rect.Width() - 5) / 6.;

	m_rectC[eC_CONVEYOR_BTN_CONNECT		] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 0, rect.left + dW * 1, rect.top + dH * 1);
	m_rectC[eC_CONVEYOR_BTN_READY		] = CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 0, rect.left + dW * 2, rect.top + dH * 1);

	m_rectC[eC_CONVEYOR_BTN_SPEED_TYPE	] = CRect(rect.left + 5 + dW * 2, rect.top + 5 + dH * 0, rect.left + dW * 3, rect.top + dH * 1);

	m_rectS[eS_CURRENT_SPEED			] =	CRect(rect.left + 5 + dW * 3, rect.top + 5 + dH * 0, rect.left + dW * 5, rect.top + dH * 1);		
	m_rectC[eC_CURRENT_SPEED			] =	CRect(rect.left + 5 + dW * 5, rect.top + 5 + dH * 0, rect.left + dW * 6, rect.top + dH * 1);

	m_rectC[eC_CONVEYOR_BTN_MINUS		] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 1, rect.left + dW * 2, rect.top + dH * 2);
	m_rectC[eC_CONVEYOR_BTN_STOP		] = CRect(rect.left + 5 + dW * 2, rect.top + 5 + dH * 1, rect.left + dW * 4, rect.top + dH * 2);
	m_rectC[eC_CONVEYOR_BTN_PLUS		] = CRect(rect.left + 5 + dW * 4, rect.top + 5 + dH * 1, rect.left + dW * 6, rect.top + dH * 2);
}


void CConveyorControl::ConveyorMove(BOOL bFwd) {
	//eALARM_ID eAlarm = eAI_GLASS_BOX_CV_LIGHT_CURTAIN_NORMAL1;
	//
	//if (m_iConveyor > eCONVEYOR_LD_9) 
	//	eAlarm =  eAI_PAPER_BOX_CV_LIGHT_CURTAIN_NORMAL1;
	//
	//// Test 를 위해서 알람 부분 일단은 무시 
	//if (sm.Alarm->ALARMs[eAlarm].bOccurred) {
	//	IPMessage(_T("외부 라이트 커튼 알람이 발생되어 있습니다"));
	//	return;
	//}

	double dSpeed = g_pRecipe->conveyorSpeed[m_iConveyor].dLowSpeed;
	if (m_bSpeedHigh)
		dSpeed = g_pRecipe->conveyorSpeed[m_iConveyor].dHighSpeed;

	dSpeed *= sm.Conveyor_Parameter[m_iConveyor].dSpeedRate;

	Serial::Inverter::SetInverterRun(m_eModule, bFwd, (int)dSpeed);
}
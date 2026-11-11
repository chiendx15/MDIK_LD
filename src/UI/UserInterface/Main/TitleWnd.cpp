#include "pch.h"
#include "UI.h"
#include "TitleWnd.h"
#include "MainFrame.h"
#include "DrawControlTestWnd.h"
#include "Simulator/DemoWnd.h"
#include "PassWordWnd.h"
#include "AutoRunCheckerWnd.h"
#include "TPMCodeSelectWnd.h"
#include "MainStatusWnd.h"
#include "SubMenuWnd.h"
#include "MessageHistoryWnd.h"

IMPLEMENT_DYNCREATE		(CTitleWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CTitleWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP			()

#define BUTTON_COMPLETE_TIME 3000


CTitleWnd::CTitleWnd(CWnd* pwndParent) {

#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
	
	m_pwndDemo			= nullptr;
	m_pwndDrawControl	= nullptr;
	m_pwndPassword		= nullptr;
	m_pwndTPMCodeSelect = nullptr;
	m_pwndMessageHistory = nullptr;

	m_blinkStartTime	= GetTickCount64();
}

CTitleWnd::~CTitleWnd() {
	if (m_pwndParent)
		m_pwndParent = nullptr;

	SAFE_DELETE( m_pwndDemo			);
	SAFE_DELETE( m_pwndDrawControl	);
	SAFE_DELETE( m_pwndPassword		);
	SAFE_DELETE( m_pwndTPMCodeSelect);
	SAFE_DELETE(m_pwndMessageHistory);
}



BOOL CTitleWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}


int CTitleWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
try { 
	
	m_pwndDrawControl = new CDrawControlTestWnd();
	if (!m_pwndDrawControl->Create(NULL, NULL, NULL, CRect())) return -1;
	
	m_pwndDemo = new CDemoWnd();
	if (!m_pwndDemo->Create(NULL, NULL, NULL, CRect()))	return -1;

	m_pwndPassword = new CPassWordWnd(this);
	if (!m_pwndPassword->Create(NULL, NULL, NULL, CRect()))	return -1;

	m_pwndTPMCodeSelect = new CTPMCodeSelectWnd(this);
	if (!m_pwndTPMCodeSelect->Create(NULL, NULL, NULL, CRect()))	return -1;
	
	m_pwndAutoRunChecker = new CAutoRunCheckerWnd(this);
	if(!m_pwndAutoRunChecker->Create(NULL, NULL, NULL, CRect())) return -1;

	m_pwndMessageHistory = new CMessageHistoryWnd();
	if (!m_pwndMessageHistory->Create(NULL, NULL, NULL, CRect()))	return -1;

	LPCTSTR		szNames[] = {
		_T("../img/TouchPanel/ServoOFF.png"),
		_T("../img/TouchPanel/ServoON.png"),
		_T("../img/TouchPanel/OperationStart.png"),
		_T("../img/TouchPanel/OperationStop.png"),
		_T("../img/TouchPanel/LoadingStop.png"),
		_T("../img/TouchPanel/UnloadingStop.png"),
		_T("../img/TouchPanel/OriginReturn.png"),
		_T("../img/TouchPanel/BuzzerStop.png"),
		_T("../img/TouchPanel/AlarmReset.png"),
	};
	for (int i = 0; i < eIMAGE_BUTTON_END; i++)
	{
		m_imgButton[i] = new Gdiplus::Image(szNames[i]);
	}

	// LJY 추가 2024.3.20
	LPCTSTR        szFlags[] = {
		_T("../img/Flags/Flag_of_South_Korea.png"),
		_T("../img/Flags/Flag_of_the_United_States.png"),
		_T("../img/Flags/Flag_of_Vietnam.png"),
	};

	// LJY 추가 2024.3.20
	for (int idx = 0; idx < eIMAGE_FLAG_END; ++idx)
	{
		m_imgFlag[idx] = new Gdiplus::Image(szFlags[idx]);
	}

	// LJY 추가 2024.6.23 - 처음 UI 켜졌을 때 공유메모리에 이상한 값 들어있으면 베트남어로 언어 설정
	if (sm.DeviceParameter->System.eLanguage < eL_KOREAN || sm.DeviceParameter->System.eLanguage > eL_VIETNAM) {
		sm.DeviceParameter->System.eLanguage = eL_VIETNAM;
	}
	//SetLanguage(sm.DeviceParameter->System.eLanguage);
	
	ShowWindow(SW_HIDE);
}
catch (...) {
	return -1;
}

	return 0;
}



void CTitleWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) 
	{
		SetWindowPos(&wndBottom, g_rectArea[eAREA_TITLE].left, g_rectArea[eAREA_TITLE].top, g_rectArea[eAREA_TITLE].Width(), g_rectArea[eAREA_TITLE].Height(), SWP_SHOWWINDOW);
		m_eSelectControl	= eC_NONE;
		fn_ComputeArea();
		SetTimer(INTERVAL_TITLE, 100, NULL);
	}
	else 
	{
		KillTimer(INTERVAL_TITLE);
	}
}



void CTitleWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
		case INTERVAL_TITLE: {
			//<< KES 추가 2024.4.9 - Key Switch + Door 상태에 따라서 운전모드 변경
			//Auto Mode = Door Close + Auto Key Switch + User선택
			//Manual Mode = Door Close + Auto Key Switch + User선택
			//Teach Mode = Teach Key Switch
			//if (sm.IO->tIn_CRT.fLD_OP_SELECT_KEY_SWITCH_AUTO_POSITION == FALSE) {
			//	m_bFirstDoorKeyChange = TRUE;
			//	sm.Option->tSequence.eDriveMode = eDRIVE_MODE_TEACH;
			//}
			//else {
			//	if (m_bFirstDoorKeyChange) {
			//		m_bFirstDoorKeyChange = FALSE;
			//		sm.Option->tSequence.eDriveMode = eDRIVE_MODE_MANUAL;
			//	}
			//}
			//>>

			BOOL bServoOnOff[2] = { TRUE, TRUE };
			for (int iMotor = eMOTOR_0; iMotor < eMOTOR_END; iMotor++) 
			{
				bServoOnOff[0] &= sm.MotorStatus[iMotor].tCrt.bServoOn;
				bServoOnOff[1] &= !sm.MotorStatus[iMotor].tCrt.bServoOn;
			}

			// 2014.03.26 LBH - 추후 확인 후 사용 미사용 결정.
			//for (int idx = 0; idx < nCONVEYOR_LD; ++idx) 
			//{
			//	bServoOnOff[0] &= Serial::Inverter::GetInverterReady((eMODULEs)(eMODULE_PC1_LOAD_CONVEYOR_1 + idx));
			//	bServoOnOff[1] &= !Serial::Inverter::GetInverterReady((eMODULEs)(eMODULE_PC1_LOAD_CONVEYOR_1 + idx));
			//}
			m_bServoOn = bServoOnOff[0];
			m_bServoOff = bServoOnOff[1];

			// Auto Run 중 체크 (일시정지도 아닌상태)
			if (sm.Sequence->fAutoMode && !sm.Sequence->fCycleStop)
			{
				// CIM PC Connect Check
				if (!g_ipCIMClient.IsServerConnected() || !g_ipCIMClient.IsIPCommandLoopStarted())
				{
					if (!sm.Sequence->bCIMSkip)
						SetAlarm(eAI_DISCONNECT_CIM, IDS_DES_NOT_USE);
				}
			
				// Process Connect Check
				auto CheckConnectProcess = [&](const CStringA _processName, eALARM_ID _alarmId)
				{
					for (int iProcess = 0; iProcess < MOCHA_MAX_PROCESS; ++iProcess) 
					{
						CStringA mochaProcessName(sm.Process[iProcess].szProcessName);
						if (mochaProcessName.IsEmpty()) continue;
			
						if (_processName.Compare(mochaProcessName) == 0)
						{
							if (sm.Process[iProcess].bConnected)
								return; // 찾고 연결된 상태
							else
								break; // 연결이 안되어 있으면 알람 발생
						}
					}
			
					SetAlarm(_alarmId, IDS_DES_NOT_USE);
				};
				
				CheckConnectProcess("UI", eAI_DISCONNECT_UI);
				CheckConnectProcess("PLCLinker", eAI_DISCONNECT_PLCLINKER);
				CheckConnectProcess("Sequence", eAI_DISCONNECT_SEQUENCE);
				CheckConnectProcess("DataTransfer", eAI_DISCONNECT_DATATRANSFER);
				CheckConnectProcess("SystemAlarm", eAI_DISCONNECT_SYSTEMALARM);
				//CheckConnectProcess("Serial", eAI_DISCONNECT_SERIAL);
			}

			Invalidate(FALSE);
			break;
		}
	}
	CFrameWndEx::OnTimer(nIDEvent);
}



BOOL CTitleWnd::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}



void CTitleWnd::OnPaint() {
	CPaintDC				dc(this);
	CMemDCEx::CMemDC		memDC(&dc);

	memDC.FillSolidRect(m_rectS[eS_FULL_AREA], RGB_STILL_BLUE);
	DrawLineRect(memDC, m_rectS[eS_FULL_AREA], RGB_WHITE);
	memDC.FillSolidRect(m_rectS[eS_AREA_EQP_STATE], g_colorBackground[eEC_DEFAULT]);
	memDC.FillSolidRect(m_rectS[eS_AREA_DEFAULT_STATUS], g_colorBackground[eEC_DEFAULT]);
	memDC.FillSolidRect(m_rectS[eS_AREA_RECIPE_INFO], g_colorBackground[eEC_DEFAULT]);
	memDC.FillSolidRect(m_rectS[eS_AREA_OP_DRIVE_MODE_STATUS], g_colorBackground[eEC_DEFAULT]);

	SelectedDrawText(&memDC, m_rectS[eS_AVAILABILITY], _T("AVAILABILITY"), 9, RGB_WHITE, RGB_BLACK, RGB_WHITE, DT_CENTER | DT_VCENTER);
	SelectedDrawText(&memDC, m_rectS[eS_INTERLOCK], _T("INTERLOCK"), 9, RGB_WHITE, RGB_BLACK, RGB_WHITE, DT_CENTER | DT_VCENTER);
	SelectedDrawText(&memDC, m_rectS[eS_MOVE], _T("MOVE"), 9, RGB_WHITE, RGB_BLACK, RGB_WHITE, DT_CENTER | DT_VCENTER);
	SelectedDrawText(&memDC, m_rectS[eS_RUN], _T("RUN"), 9, RGB_WHITE, RGB_BLACK, RGB_WHITE, DT_CENTER | DT_VCENTER);

	//24.07.17 KGH EDIT : Module Status 상태 EQP 오설정 수정
	const auto& EQPStatus = sm.Sequence->CIMData.EQPStatus[eEQP_SCT].EQP;
	//const auto& EQPStatus = sm.Sequence->CIMData.EQPStatus[g_iLine ? eEQP_CCT_A : eEQP_CCT_B].EQP;

	eAVAILABILITY_STATE eAVILABILITY = EQPStatus.eAvailabilityState;
	SelectedDrawTextB(&memDC, m_rectS[eS_AVAILABILITY_UP], _T("UP"), FW_HEAVY, 9, RGB_BLACK, eAVILABILITY == eAS_UP ? RGB_GREEN : RGB_WHITE, RGB_BLACK, DT_CENTER | DT_VCENTER);
	SelectedDrawTextB(&memDC, m_rectS[eS_AVAILABILITY_DOWN], _T("DOWN"), FW_HEAVY, 9, RGB_BLACK, eAVILABILITY == eAS_UP ? RGB_WHITE : RGB_RED, RGB_BLACK, DT_CENTER | DT_VCENTER);

	eINTERLOCK_STATE eINTERLOCK = EQPStatus.eInterlockState;
	SelectedDrawTextB(&memDC, m_rectS[eS_INTERLOCK_OFF], _T("OFF"), FW_HEAVY, 9, RGB_BLACK, eINTERLOCK == eIS_OFF ? RGB_GREEN : RGB_WHITE, RGB_BLACK, DT_CENTER | DT_VCENTER);
	SelectedDrawTextB(&memDC, m_rectS[eS_INTERLOCK_ON], _T("ON"), FW_HEAVY, 9, RGB_BLACK, eINTERLOCK == eIS_OFF ? RGB_WHITE : RGB_RED, RGB_BLACK, DT_CENTER | DT_VCENTER);

	eMOVE_STATE eMOVE = EQPStatus.eMoveState;
	SelectedDrawTextB(&memDC, m_rectS[eS_MOVE_RUNNING], _T("RUNNING"), FW_HEAVY, 9, RGB_BLACK, eMOVE == eMS_RUNNING ? RGB_GREEN : RGB_WHITE, RGB_BLACK, DT_CENTER | DT_VCENTER);
	SelectedDrawTextB(&memDC, m_rectS[eS_MOVE_PAUSE], _T("PAUSE"), FW_HEAVY, 9, RGB_BLACK, eMOVE == eMS_RUNNING ? RGB_WHITE : RGB_RED, RGB_BLACK, DT_CENTER | DT_VCENTER);

	eRUN_STATE eRUN = EQPStatus.eRunState;
	SelectedDrawTextB(&memDC, m_rectS[eS_RUN_RUN], _T("RUN"), FW_HEAVY, 9, RGB_BLACK, eRUN == eRS_RUN ? RGB_GREEN : RGB_WHITE, RGB_BLACK, DT_CENTER | DT_VCENTER);
	SelectedDrawTextB(&memDC, m_rectS[eS_RUN_IDLE], _T("IDLE"), FW_HEAVY, 9, RGB_BLACK, eRUN == eRS_RUN ? RGB_WHITE : RGB_RED, RGB_BLACK, DT_CENTER | DT_VCENTER);

	if (sm.Alarm->iCurAlarmIndex > -1)
		SelectedDrawTextB(&memDC, m_rectS[eS_ALARM], Format(_T("Alarm Code %d \n %s"), sm.Alarm->iCurAlarmIndex, CString(sm.Alarm->ALARMs[sm.Alarm->iCurAlarmIndex].AlarmData[sm.DeviceParameter->System.eLanguage].szText)), FW_HEAVY, 9, RGB_BLACK, RGB_ORANGE, RGB_WHITE);
	else
		SelectedDrawTextB(&memDC, m_rectS[eS_ALARM], g_st[IDS_TITLE_NONE_ALARM], FW_HEAVY, 9, RGB_BLACK, RGB_GREEN, RGB_BLACK);

	//SelectedDrawText(&memDC, m_rectS[eS_CONNECT_STATUS], _T("장치 통신 상태[ 절단 ]"), 9, RGB_WHITE, RGB_RED, RGB_WHITE);

	//<< 2023.01.26 깜빡임 제거
	static int OffCount = 0;
	if (sm.Register->In.dw200A) OffCount = 0;
	else ++OffCount;

	if (OffCount <= 10)
		SelectedDrawText(&memDC, m_rectS[eS_CONNECT_STATUS], g_st[IDS_TITLE_CONNECT_STATUS_CONNECT], 9, RGB_BLACK, RGB_GREEN, RGB_BLACK);
	else
		SelectedDrawText(&memDC, m_rectS[eS_CONNECT_STATUS], g_st[IDS_TITLE_CONNECT_STATUS_DISCONNECT], 9, RGB_WHITE, RGB_RED, RGB_WHITE);
	//>>

	CString strTime;
	CTime time = CTime::GetCurrentTime();
	strTime.Format(_T("%04d-%02d-%02d %s %d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour() > 12 ? _T("PM") : _T("AM")
		, time.GetHour() > 12 ? time.GetHour() - 12 : time.GetHour(), time.GetMinute(), time.GetSecond());

	SelectedDrawText(&memDC, m_rectS[eS_TIME			], strTime, 9, RGB_WHITE, RGB_BLUE, RGB_WHITE);

	SelectedDrawText(&memDC, m_rectS[eS_NAME			], _T("LD"), 9, RGB_WHITE, RGB_BLUE, RGB_WHITE);

	SelectedDrawText(&memDC, m_rectS[eS_PPID_1			], g_st[IDS_TITLE_PPID], 9, RGB_WHITE, RGB_BLUE, RGB_WHITE);
	SelectedDrawText(&memDC, m_rectS[eS_PPID_2			], Format(_T("%s"), g_pRecipe->ppid), 9, RGB_WHITE, RGB_BLUE, RGB_WHITE);
	SelectedDrawText(&memDC, m_rectS[eS_RECIPEID_1		], g_st[IDS_TITLE_RECIPE_ID], 9, RGB_WHITE, RGB_BLUE, RGB_WHITE);
	SelectedDrawText(&memDC, m_rectS[eS_RECIPEID_2		], Format(_T("%s"), g_pRecipe->recipeName), 9, RGB_WHITE, RGB_BLUE, RGB_WHITE);

	if (sm.Sequence->bCIMSkip)
		SelectedDrawText(&memDC, m_rectC[eC_CIM_STATUS], g_st[IDS_TITLE_OFF_LINE], 9, RGB_BLACK, RGB_RED, RGB_BLACK);
	else
		SelectedDrawText(&memDC, m_rectC[eC_CIM_STATUS], g_st[IDS_TITLE_ONLINE], 9, RGB_BLACK, RGB_GREEN, RGB_BLACK);


	SelectedDrawText(&memDC, m_rectC[eC_OP_CHANGE		], g_st[IDS_TITLE_OP_CHANGE], 9, RGB_GRAY_DARK, RGB_GRAY_LIGHT, RGB_BLACK);
	
	SelectedDrawText(&memDC, m_rectS[eS_OPID_1			], g_st[IDS_TITLE_OPID], 9, RGB_WHITE, RGB_BLUE, RGB_WHITE);
	CString strUser = Format(_T("%s [ %s ]"), theApp.m_UserManager.GetCurrentUserData().szID, theApp.m_UserManager.GetUserGradeText(sm.DeviceParameter->eUserGrade));
	SelectedDrawText(&memDC, m_rectS[eS_OPID_2			], strUser, 9, RGB_WHITE, RGB_BLUE, RGB_WHITE);

	if (sm.Option->tSequence.eDriveMode != eDRIVE_MODE_TEACH) {
		DrawMDIMenuButtonN(&memDC, m_rectC[eC_MANUAL_AUTO	], sm.Option->tSequence.eDriveMode == eDRIVE_MODE_MANUAL ? g_st[IDS_TITLE_KEYSWITCH_MANUAL] : g_st[IDS_TITLE_KEYSWITCH_AUTO], m_eSelectControl == eC_MANUAL_AUTO, m_eMouseOver == eC_MANUAL_AUTO);
	}
	else {
		SelectedDrawText(&memDC, m_rectC[eC_MANUAL_AUTO		], g_st[IDS_TITLE_KEYSWITCH_TEACH], 9, RGB_WHITE, RGB_BLUE, RGB_WHITE);
	}

	CString strDir = sm.Option->tSequence.eDriveMode == eDRIVE_MODE_TEACH ? _T("▶") : _T("◀");
	SelectedDrawText(&memDC, m_rectS[eS_SELECT			], strDir, 9, RGB_BLACK, RGB_WHITE,	RGB_BLACK);				
	DrawMDIMenuButtonN(&memDC, m_rectC[eC_TEACH			], g_st[IDS_TITLE_KEYSWITCH_TEACH], m_eSelectControl == eC_TEACH, m_eMouseOver == eC_TEACH);

	//<<<< 타이틀 메뉴 버튼 >>>>//
	// Servo On (전체 On 점등, 일부 On 점멸)
	DrawMDIImageBlinkFillButton(memDC, m_rectC[eC_SERVO_ON], g_st[IDS_TITLE_SERVO_ON], m_imgButton[eIMAGE_BUTTON_SERVO_ON], m_bServoOn, !m_bServoOn && !m_bServoOff, m_eSelectControl == eC_SERVO_ON, m_eMouseOver == eC_SERVO_ON, GetTickCount64() - m_clickStartTime, BUTTON_COMPLETE_TIME, m_blinkStartTime, 500, 15);
	// Servo Off (전체 Off 점등, 점멸 X, 버튼 동작 바로 됨[그라데이션 효과 X])
	DrawMDIImageButton(memDC, m_rectC[eC_SERVO_OFF], g_st[IDS_TITLE_SERVO_OFF], m_imgButton[eIMAGE_BUTTON_SERVO_OFF], m_bServoOff || m_eSelectControl == eC_SERVO_OFF, m_eMouseOver == eC_SERVO_OFF, 15);
	// 운전 (점등, 그라데이션 X)
	DrawMDIImageButton(memDC, m_rectC[eC_RUN], g_st[IDS_TITLE_RUN], m_imgButton[eIMAGE_BUTTON_RUN], sm.Sequence->fAutoMode || m_eSelectControl == eC_RUN, m_eMouseOver == eC_RUN, 15);
	// 정지/일시정지 (점멸, 그라데이션)
	DrawMDIImageBlinkFillButton(memDC, m_rectC[eC_STOP], sm.Sequence->fCycleStop ? g_st[IDS_TITLE_CYCLE_STOP] : g_st[IDS_TITLE_STOP], m_imgButton[eIMAGE_BUTTON_STOP], false, sm.Sequence->fCycleStop, m_eSelectControl == eC_STOP, m_eMouseOver == eC_STOP, GetTickCount64() - m_clickStartTime, BUTTON_COMPLETE_TIME, m_blinkStartTime, 500, 15);
	// 투입금지 (점등)
	DrawMDIImageTimeFillButton(memDC, m_rectC[eC_IN], g_st[IDS_TITLE_INPUT_STOP], m_imgButton[eIMAGE_BUTTON_IN], !sm.Option->tSequence.fIN, m_eSelectControl == eC_IN, m_eMouseOver == eC_IN, GetTickCount64() - m_clickStartTime, BUTTON_COMPLETE_TIME, 15);
	// 배출금지 (점등)
	DrawMDIImageTimeFillButton(memDC, m_rectC[eC_OUT], g_st[IDS_TITLE_OUTPUT_STOP], m_imgButton[eIMAGE_BUTTON_OUT], !sm.Option->tSequence.fOUT, m_eSelectControl == eC_OUT, m_eMouseOver == eC_OUT, GetTickCount64() - m_clickStartTime, BUTTON_COMPLETE_TIME, 15);
	// 원점 복귀 (전체 원점 점등, 원점 진행 중 점멸, 기본[그라데이션]) 
	DrawMDIImageBlinkFillButton(memDC, m_rectC[eC_INIT], g_st[IDS_TITLE_INIT], m_imgButton[eIMAGE_BUTTON_INIT], sm.Sequence->bHomeComplete, sm.Sequence->bHomming, m_eSelectControl == eC_INIT, m_eMouseOver == eC_INIT, GetTickCount64() - m_clickStartTime, BUTTON_COMPLETE_TIME, m_blinkStartTime, 500, 15);
	// 부저정지 (기본 버튼)
	DrawMDIImageTimeFillButton(memDC, m_rectC[eC_BUZZER], g_st[IDS_TITLE_BUZZER_STOP], m_imgButton[eIMAGE_BUTTON_BUZZER], sm.Option->tSequence.bBuzzerStop, m_eSelectControl == eC_BUZZER, m_eMouseOver == eC_BUZZER, GetTickCount64() - m_clickStartTime, BUTTON_COMPLETE_TIME, 15);
//	DrawMDIImageButton(memDC, m_rectC[eC_BUZZER], g_st[IDS_TITLE_BUZZER_STOP], m_imgButton[eIMAGE_BUTTON_BUZZER], m_eSelectControl == eC_BUZZER, m_eMouseOver == eC_BUZZER, 15);
	// 리셋 (기본 버튼, 현재 기능 없음)
	DrawMDIImageButton(memDC, m_rectC[eC_RESET], g_st[IDS_TITLE_RESET], m_imgButton[eIMAGE_BUTTON_RESET], m_eSelectControl == eC_RESET, m_eMouseOver == eC_RESET, 15);

	// AGV 사용 미사용 (기본 버튼)
	if (sm.Option->tSequence.bUseAGV)
		DrawMDIImageTimeFillButton(memDC, m_rectC[eC_AGB_USE], g_st[IDS_TITLE_AGV_USE], nullptr, sm.Option->tSequence.bUseAGV, m_eSelectControl == eC_AGB_USE, m_eMouseOver == eC_OUT, GetTickCount64() - m_clickStartTime, BUTTON_COMPLETE_TIME, 15);
	else
		DrawMDIImageTimeFillButton(memDC, m_rectC[eC_AGB_USE], g_st[IDS_TITLE_AGV_UNUSE], nullptr, sm.Option->tSequence.bUseAGV, m_eSelectControl == eC_AGB_USE, m_eMouseOver == eC_OUT, GetTickCount64() - m_clickStartTime, BUTTON_COMPLETE_TIME, 15);

	
	//<<<< 타이틀 메뉴 버튼 >>>>//


//<<<< 우측 서브 메뉴 >>>>//
	DrawButtonImage(memDC, m_rectC[eC_LANGUAGE_CHANGE], m_imgFlag[sm.DeviceParameter->System.eLanguage], m_eSelectControl == eC_LANGUAGE_CHANGE);	// LJY 추가 2024.3.28
	DrawMDIMenuButton(memDC, m_rectC[eC_OP_CHANGE], g_st[IDS_TITLE_OP_CHANGE], m_eSelectControl == eC_OP_CHANGE, m_eMouseOver == eC_OP_CHANGE);
	DrawMDIMenuButton(memDC, m_rectC[eC_MESSAGE_HISTORY], _T("Message\nHistory"), m_eSelectControl == eC_MESSAGE_HISTORY, m_eMouseOver == eC_MESSAGE_HISTORY);	// KES 추가 2024.3.20
	DrawMDIMenuButton(memDC, m_rectC[eC_SERIAL], g_st[IDS_TITLE_SERIAL], m_eSelectControl == eC_SERIAL, m_eMouseOver == eC_SERIAL);	// KES 추가 2024.3.20
	DrawMDIMenuButton(memDC, m_rectC[eC_AXIS], g_st[IDS_TITLE_AXIS], m_eSelectControl == eC_AXIS, m_eMouseOver == eC_AXIS);
	DrawMDIMenuButton(memDC, m_rectC[eC_DEMO], g_st[IDS_TITLE_DEMO], m_eSelectControl == eC_DEMO, m_eMouseOver == eC_DEMO);
	DrawMDIMenuButton(memDC, m_rectC[eC_LOG_DELETE], g_st[IDS_TITLE_LOG_DELETE], m_eSelectControl == eC_LOG_DELETE, m_eMouseOver == eC_LOG_DELETE);
	// 물류 진행 방향 기준으로 g_bUIRotateDirection == FALSE 인 경우 진행방향의 좌측에서 보는 경우 / TRUE 인 경우 진행방향의 우측에서 보는 경우
	DrawMDIMenuButton(&memDC, m_rectC[eC_REVERSE], g_bUIRotateDirection ? g_st[IDS_TITLE_REVERSE] : g_st[IDS_TITLE_FORWARD], g_bUIRotateDirection || m_eSelectControl == eC_REVERSE, m_eMouseOver == eC_REVERSE);
//<<<< 우측 서브 메뉴 >>>>//
}


void CTitleWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 마우스 다운 상태인지 체크해서 다운 상태가 아니면 시간 초기화
	if (!(nFlags & MK_LBUTTON))
	{
		m_clickStartTime = GetTickCount64();
	}

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



void CTitleWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for (int idx = 0; idx < eC_END; idx++) 
	{
		if (m_rectC[idx].PtInRect(point)) 
		{
			m_clickStartTime = GetTickCount64();
			m_eSelectControl = idx;
			InvalidateRect(m_rectC[idx], FALSE);
			break;
		}
	}

	CFrameWndEx::OnLButtonDown(nFlags, point);
}



void CTitleWnd::OnLButtonUp(UINT nFlags, CPoint point)  {
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		// 해당 신호 들어오면 Run/Stop/In/Out/Init 버튼 동작 하면 안됨 
		if ((m_eSelectControl >= eC_RUN && m_eSelectControl <= eC_INIT) && sm.IO->tIn_CRT.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
			return;
		switch (m_eSelectControl) {		
			case eC_SERVO_OFF: {
				strLog = _T("Servo Off");
				AllServoOnOff(FALSE);
				//for (int i = 0; i < nCONVEYOR_LD; i++) 
				//{
				//	ConveyorServoOnOff((eCONVEYOR_LD)i, false);
				//}
				break;
			}
			case eC_SERVO_ON: {
				strLog = _T("Servo On");
				if (GetTickCount64() - m_clickStartTime > BUTTON_COMPLETE_TIME)
				{
					CString strResult = RespMessage(g_st[IDS_TITLE_ALL_SERVO_ON], _T("OK;CANCLE"));
					if (strResult.CompareNoCase(_T("OK")) == 0)
					{
						AllServoOnOff(TRUE);
						//for (int i = 0; i < nCONVEYOR_LD; i++) 
						//{
						//	ConveyorServoOnOff((eCONVEYOR_LD)i, true);
						//}
					}
				}
				break;
			}
			case eC_RUN: {
				if (!sm.Sequence->fAutoMode) {
					strLog = _T("운전");
					CMainFrame* pParent = (CMainFrame*)AfxGetMainWnd();
					pParent->SetView(eMAIN_MENU_1, g_eSelectedSubMenu, sm.DeviceParameter->eUserGrade);
					m_pwndAutoRunChecker->ShowWindow(SW_SHOW);
				}
				break;
			}
			case eC_STOP: {
				strLog = _T("정지");
				// 단동 동작 정지
				__POST_IP_COMMAND__("Sequence", "Manual", "Stop");
				
				//m_pwndTPMCodeSelect->ShowWindow(SW_SHOW);
				//m_pwndTPMCodeSelect->SetForegroundWindow();

				// 오토 모드가 아니면 일시 정지 해제
				if (!sm.Sequence->fAutoMode) {
					ManualStop();
				}
				else {
					if (GetTickCount64() - m_clickStartTime > BUTTON_COMPLETE_TIME) {
						ManualStop();
					}
					else {
						auto CycleStop = !sm.Sequence->fCycleStop;
					
						// 일시 정지 상태면 컨베이어 정지
						if (CycleStop) {
							if (sm.Sequence->bCIMSkip) {
								ManualStop();
							}
							else {
								if (GetTickCount64() - m_clickStartTime > BUTTON_COMPLETE_TIME) {
									ManualStop();
								}
								else {
									m_pwndTPMCodeSelect->ShowWindow(SW_SHOW);
									m_pwndTPMCodeSelect->SetForegroundWindow();
								}
							}
						}
						// 일시 정지 해제
						else {
							BOOL bAutoRun = TRUE;
							if (!sm.Sequence->bCIMSkip) {
								const auto [result, msgs] = LoadMSG(eEQP_SCT, eMSG_TYPE_INTERLOCK);
								if (!result || (int)msgs.size())
									bAutoRun = FALSE;
							}

							if (bAutoRun) {
								for (int idx = 0; idx < sm.Alarm->nALARM; ++idx)
								{
									if (sm.Alarm->ALARMs[idx].bOccurred && sm.Alarm->ALARMs[idx].iALCD == 2)
									{
										IPMessage(g_st[IDS_TITLE_HEAVY_ALARM_NOT_CLEAR]);
										return;
									}
								}
								if (RespMessage(g_st[IDS_TITLE_AUTO_RUN_AGAIN]).Compare(_T("OK")) != 0)
									break;


								CMainFrame* pParent = (CMainFrame*)AfxGetMainWnd();
								pParent->SetView(eMAIN_MENU_1, g_eSelectedSubMenu, sm.DeviceParameter->eUserGrade);


								__POST_IP_COMMAND__("Sequence", "Sequence", "AutoRun");
							}
							else {
								IPMessage(_T("Interlock Message Confirm후 진행 해주세요."));
								break;
							}
						}
					}
				}
				break;
			}
			case eC_IN: {
				//sm.Module[eMODULE_PC1_LOAD_CONVEYOR_1].Status.bInputStop = !sm.Module[eMODULE_PC1_LOAD_CONVEYOR_1].Status.bInputStop;
				sm.Option->tSequence.fIN = !sm.Option->tSequence.fIN;
				strLog = _T("투입금지");
				break;
			}
			case eC_OUT: {
				sm.Option->tSequence.fOUT = !sm.Option->tSequence.fOUT;
				strLog = _T("배출금지");
				break;
			}
			case eC_INIT: {
				strLog = _T("원점복귀");
				if (GetTickCount64() - m_clickStartTime > BUTTON_COMPLETE_TIME) 
				{
					g_ipClient.PostIPCommand("Sequence", "Return", "ZeroPoint");
				}
				break;
			}
			case eC_BUZZER: {
				strLog = _T("부저정지");
				//-->24.05.02 KGH EDIT : 멜로디 정상화
				//sm.IO->tOut_NEW.fLD_MELODY_2 = FALSE;
				//sm.IO->tOut_NEW.fLD_MELODY_1 = FALSE;
				//<--24.05.02 KGH EDIT : 멜로디 정상화
				sm.Option->tSequence.bBuzzerStop = !sm.Option->tSequence.bBuzzerStop;
				break;
			}
			case eC_RESET: {
				strLog = _T("리셋");
				// 2022.09.14 리셋버튼 기능 추가 (시스템 알람 클리어, 모터 알람 클리어)
				sm.Register->Out_New.dw1000_AlarmReset = TRUE;
				ClearAlarm();
				break;
			}
			case eC_AGB_USE: {
				sm.Option->tSequence.bUseAGV = !sm.Option->tSequence.bUseAGV;
				break;
			}
			case eC_SERIAL: 
			{
				strLog = _T("Serial");
				CIPVar var;
				var.SetChildItem("Show", TRUE);
				if (FALSE == g_ipClient.PostIPCommand("Serial", "SerialUI", "Show")) 
				{
					IPMessage(g_st[IDS_TITLE_MS_SERIAL_CONNECT_CHECK]);
				}
				break;
			}
			case eC_AXIS: {
				strLog = _T("Axis Status Control");
				g_ipClient.PostIPCommand("PLCLinker", "AxisStatusControl", "Show");
				break;
			}
			case eC_DEMO: {
				// 2021.05.05 MDIK 이성령 선임님 요청
				if (sm.DeviceParameter->eUserGrade >= eUSER_ADMIN)
				{
					strLog = _T("Demo");
					m_pwndDemo->ShowWindow(SW_SHOW);
					m_pwndDemo->SetForegroundWindow();
				}
				else
				{
					IPMessage(g_st[IDS_TITLE_DO_NOT_HAVE_PERMISSION]);
				}
				break;
			}
			case eC_LOG_DELETE:
			{
				if (sm.DeviceParameter->eUserGrade >= eUSER_ADMIN)
				{
					strLog = _T("로그 주기");
					__POST_IP_COMMAND__("AutoDelete", "AutoDelete", "Show");
				}
				else
				{
					IPMessage(g_st[IDS_TITLE_DO_NOT_HAVE_PERMISSION]);
				}
			}
			break;

			case eC_KEYPAD: {
				strLog = _T("KeyPad");
				sm.Option->tKeyPad.fKeyPadUseMode = !sm.Option->tKeyPad.fKeyPadUseMode;
				break;
			}

			case eC_MESSAGE_HISTORY: {
				strLog = _T("MessageHistory");
				m_pwndMessageHistory->ShowWindow(SW_SHOW);
				m_pwndMessageHistory->SetForegroundWindow();
				break;
			}

			case eC_REVERSE: {
				strLog = _T("Reverse");
				g_bUIRotateDirection = !g_bUIRotateDirection;
				CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
				CMainStatusWnd* pMainStatus = (CMainStatusWnd*)pMain->getMainStatusWnd();
				pMainStatus->fn_ComputeArea();
				pMainStatus->SetSimulatorView();
				pMainStatus->Invalidate(FALSE);
				break;
			}
			case eC_MANUAL_AUTO: {
				//<< KES 추가 2024.4.9 - LD 또는 SCT Door Open 상태에서는 Key Switch 잠금을 해제할 수 없음
				if (CheckDoorOpen_SCT_LD()) {
					IPMessage(g_st[IDS_TITLE_CANNOT_UNLOCK_KEY_SWITCH_SOL_DURING_DOOR_OPEN]);
					break;
				}

				if (!sm.IO->tOut_CRT.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST) {
					IPMessage(_T("Door Lock Release 상태에서는 Auto / Manual Mode로 변경 할 수 없습니다."));
					break;
				}
				//24.06.12 KGH DEL : KEY SWITCH CONDITION DELETE
				//if (!sm.IO->tIn_CRT.fLD_OP_SELECT_KEY_SWITCH_AUTO_POSITION) {
				//	IPMessage(_T("Key Switch Teach에서는 Auto / Manual Mode로 변경 할 수 없습니다."));
				//	break;
				//}
				//>>

				eDRIVE_MODE eDriveMode = sm.Option->tSequence.eDriveMode;
				if (sm.Option->tSequence.eDriveMode == eDRIVE_MODE_TEACH)  {
					//-->24.06.25 KGH ADD: Teach ->Manual Condition Add
					if (!sm.IO->tIn_CRT.fLD_OP_SELECT_KEY_SWITCH_AUTO_POSITION)
					{
						IPMessage(_T("Key Switch Teach에서는 Manual Mode로 변경 할 수 없습니다."));
						break;
					}
					//<--24.06.25 KGH ADD: Teach ->Manual Condition Add
					// Manual Mode 변경 가능.
					eDriveMode = eDRIVE_MODE_MANUAL;
					strLog = _T("Manual");
				}
				else if (sm.Option->tSequence.eDriveMode == eDRIVE_MODE_AUTO) {
					if (sm.Sequence->fAutoMode) {
						IPMessage(g_st[IDS_ALARM_CHECKER_LD_AUTO_RUNNING]);
						break;
					}
					// Manual Mode 변경 가능.
					eDriveMode = eDRIVE_MODE_MANUAL;
					strLog = _T("Manual");
				}
				else {
					//24.06.14 kwr add : key s/w teach 일 때 auto mode 변경 불가
					if (!sm.IO->tIn_CRT.fLD_OP_SELECT_KEY_SWITCH_AUTO_POSITION) {
							IPMessage(_T("Key Switch Teach에서는 Auto Mode로 변경 할 수 없습니다."));
							break;
						}
					// Auto 변경 가능
					eDriveMode = eDRIVE_MODE_AUTO;
					strLog = _T("Auto");
 				}

				m_pwndPassword->SetDriveMode(eDriveMode);
				m_pwndPassword->ShowWindow(SW_SHOW);
				m_pwndPassword->SetForegroundWindow();
				
				break;
			}
			case eC_TEACH: {
				if (sm.Option->tSequence.eDriveMode == eDRIVE_MODE_AUTO) {
					IPMessage(_T("Auto Mode에서 Teach Mode로 바로 변경 할 수 없습니다.\n Manual Mode 변경 후 진행해 주세요."));
					break;
				}

				strLog = _T("Key Switch Unlock");
				// 2024.06.10 LBH - Auto Run중이 아니고, Servo All Off 상태일 때만 티치모드 변경 가능.
				BOOL bServoOn = FALSE;
				for (int iMotor = eMOTOR_0; iMotor < eMOTOR_END; iMotor++) {
					bServoOn |= sm.MotorStatus[iMotor].tCrt.bServoOn;
				}

				//-->24.06.12 KGH EDIT : TEACH 버튼 기능 정상화
				if (bServoOn || sm.Sequence->fAutoMode) {
					IPMessage(_T("Servo 가 하나라도 켜져 있거나 자동운전 중에는 Teach Mode로 변경 할 수 없습니다."));
				}
				else {
					m_pwndPassword->SetDriveMode(eDRIVE_MODE_TEACH);
					m_pwndPassword->ShowWindow(SW_SHOW);
					m_pwndPassword->SetForegroundWindow();
				}

				//BOOL bCheckIO = sm.IO->tOut_CRT.fLD_SELECT_KEY_ENABLE;
				//if (bCheckIO) {
				//	IPMessage(g_st[IDS_TITLE_KEY_SWITCH_SOL_UNLOCK_ALREADY]);
				//}
				//else {
				//	if (bServoOn || sm.Sequence->fAutoMode) {
				//		IPMessage(_T("Servo 가 하나라도 켜져 있거나 자동운전 중에는 Teach Mode로 변경 할 수 없습니다."));
				//	}
				//	else {
				//		m_pwndPassword->SetDriveMode(eDRIVE_MODE_TEACH);
				//		m_pwndPassword->ShowWindow(SW_SHOW);
				//		m_pwndPassword->SetForegroundWindow();
				//	}
				//}
				break;
			}
			case eC_OP_CHANGE: {
				strLog = _T("OP 변경");
				((CMainFrame*)m_pwndParent)->LogOut();
				break;
			}
			case eC_CIM_STATUS: {
				// 자재 정보 X 
				// Auto Mode 가 아니거나 Cycle Stop 인 경우
				//BOOL bCheck = TRUE;
				//for (int i = eMODULE_PC1_BEGIN; i < eMODULE_PC1_END; i++) {
				//
				//}
				//sm.Sequence->bCIMSkip = !sm.Sequence->bCIMSkip;
				break;
			}
            case eC_LANGUAGE_CHANGE:
            {
                sm.DeviceParameter->System.eLanguage = (eLANGUAGE)((sm.DeviceParameter->System.eLanguage + 1) % nLANGUAGE);
                SetLanguage(sm.DeviceParameter->System.eLanguage);	// LJY 추가 2024.3.21
            }

            break;
			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(_T("Title"), _T("Title"), _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl	=	eC_NONE;
	Invalidate(FALSE);

	CFrameWndEx::OnLButtonUp(nFlags, point);
}



void CTitleWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);

	CRect rc(m_rectS[eS_FULL_AREA]);
	//rc.top += 40;
	rc.DeflateRect(1, 1);

	double dH = (rc.Height() - 5) / 5.;
	double dW = (rc.Width() - 5) / 17.;

	m_rectS[eS_AREA_EQP_STATE]				= CRect(rc.left+5+dW*0		, rc.top+dH*0+5	,rc.left+dW*2.5		, rc.top+dH*5);
	m_rectS[eS_ALARM]						= CRect(rc.left+5+dW*2.5	, rc.top+dH*0+5	,rc.left+dW*5		, rc.top+dH*2);
	m_rectS[eS_AREA_DEFAULT_STATUS]			= CRect(rc.left+5+dW*5		, rc.top+dH*0+5	,rc.left+dW*8		, rc.top+dH*2);
	m_rectS[eS_AREA_RECIPE_INFO]			= CRect(rc.left+5+dW*8		, rc.top+dH*0+5	,rc.left+dW*10.5	, rc.top+dH*2);
	m_rectC[eC_CIM_STATUS]					= CRect(rc.left+5+dW*10.5	, rc.top+dH*0+5	,rc.left+dW*12.5-5	, rc.top+dH*2);
	m_rectC[eC_LANGUAGE_CHANGE]				= CRect(rc.left+5+dW*13.8	, rc.top+dH*0+5	,rc.left+dW*14.4	, rc.top+dH*2);
	m_rectC[eC_OP_CHANGE]					= CRect(rc.left+5+dW*14.4	, rc.top+dH*0+5	,rc.left+dW*15		, rc.top+dH*2);
	m_rectS[eS_AREA_OP_DRIVE_MODE_STATUS]	= CRect(rc.left+5+dW*15		, rc.top+dH*0+5	,rc.left+dW*17		, rc.top+dH*2);
	m_rectS[eS_AREA_CONTROL_BTN_1]			= CRect(rc.left+dW*2.5+5	, rc.top+dH*2+5	,rc.left+dW*15		, rc.top+dH*5);
	m_rectS[eS_AREA_CONTROL_BTN_2]			= CRect(rc.left+dW*15+5		, rc.top+dH*2+5	,rc.left+dW*17		, rc.top+dH*5);

	rc = m_rectS[eS_AREA_EQP_STATE];
	dH = (rc.Height() - 5) / 4.;
	dW = (rc.Width() - 5) / 4.;
	m_rectS[eS_AVAILABILITY]		= CRect(rc.left+dW*0+5	, rc.top+dH*0+5	,rc.left+dW*2	, rc.top+dH*1);
	m_rectS[eS_INTERLOCK]			= CRect(rc.left+dW*0+5	, rc.top+dH*1+5	,rc.left+dW*2	, rc.top+dH*2);
	m_rectS[eS_MOVE]				= CRect(rc.left+dW*0+5	, rc.top+dH*2+5	,rc.left+dW*2	, rc.top+dH*3);
	m_rectS[eS_RUN]					= CRect(rc.left+dW*0+5	, rc.top+dH*3+5	,rc.left+dW*2	, rc.top+dH*4);
	
	m_rectS[eS_AVAILABILITY_UP]		= CRect(rc.left+dW*2+5	, rc.top+dH*0+5	,rc.left+dW*3	, rc.top+dH*1);
	m_rectS[eS_AVAILABILITY_DOWN]	= CRect(rc.left+dW*3+5	, rc.top+dH*0+5	,rc.left+dW*4	, rc.top+dH*1);
	m_rectS[eS_INTERLOCK_OFF]		= CRect(rc.left+dW*2+5	, rc.top+dH*1+5	,rc.left+dW*3	, rc.top+dH*2);
	m_rectS[eS_INTERLOCK_ON]		= CRect(rc.left+dW*3+5	, rc.top+dH*1+5	,rc.left+dW*4	, rc.top+dH*2);
	m_rectS[eS_MOVE_RUNNING]		= CRect(rc.left+dW*2+5	, rc.top+dH*2+5	,rc.left+dW*3	, rc.top+dH*3);
	m_rectS[eS_MOVE_PAUSE]			= CRect(rc.left+dW*3+5	, rc.top+dH*2+5	,rc.left+dW*4	, rc.top+dH*3);
	m_rectS[eS_RUN_RUN]				= CRect(rc.left+dW*2+5	, rc.top+dH*3+5	,rc.left+dW*3	, rc.top+dH*4);
	m_rectS[eS_RUN_IDLE]			= CRect(rc.left+dW*3+5	, rc.top+dH*3+5	,rc.left+dW*4	, rc.top+dH*4);

	rc = m_rectS[eS_AREA_DEFAULT_STATUS];
	dH = (rc.Height() - 5) / 2.;
	dW = (rc.Width() - 5) / 2.;
	m_rectS[eS_CONNECT_STATUS	]	= CRect(rc.left+dW*0+5	, rc.top+dH*0+5	,rc.left+dW*1	, rc.top+dH*1); 
	m_rectS[eS_TIME				]	= CRect(rc.left+dW*1+5	, rc.top+dH*0+5	,rc.left+dW*2	, rc.top+dH*1); 
	m_rectS[eS_NAME				]	= CRect(rc.left+dW*0+5	, rc.top+dH*1+5	,rc.left+dW*2	, rc.top+dH*2); 

	rc = m_rectS[eS_AREA_RECIPE_INFO];
	dH = (rc.Height() - 5) / 2.;
	dW = (rc.Width() - 5) / 7.;
	m_rectS[eS_RECIPEID_1	]	= CRect(rc.left+dW*0+5	, rc.top+dH*0+5	,rc.left+dW*2	, rc.top+dH*1); 
	m_rectS[eS_RECIPEID_2	]	= CRect(rc.left+dW*2+5	, rc.top+dH*0+5	,rc.left+dW*7	, rc.top+dH*1); 
	m_rectS[eS_PPID_1		]	= CRect(rc.left+dW*0+5	, rc.top+dH*1+5	,rc.left+dW*2	, rc.top+dH*2); 
	m_rectS[eS_PPID_2		]	= CRect(rc.left+dW*2+5	, rc.top+dH*1+5	,rc.left+dW*7	, rc.top+dH*2); 

	rc = m_rectS[eS_AREA_OP_DRIVE_MODE_STATUS];
	dH = (rc.Height() - 5) / 2.;
	dW = (rc.Width() - 5) / 12.;
	m_rectS[eS_OPID_1		]	= CRect(rc.left+dW*0+5	, rc.top+dH*0+5	,rc.left+dW*3	, rc.top+dH*1);
	m_rectS[eS_OPID_2		]	= CRect(rc.left+dW*3+5	, rc.top+dH*0+5	,rc.left+dW*12	, rc.top+dH*1);
	m_rectC[eC_MANUAL_AUTO	]	= CRect(rc.left+dW*0+5	, rc.top+dH*1+5	,rc.left+dW*4	, rc.top+dH*2); 
	m_rectS[eS_SELECT		]	= CRect(rc.left+dW*4+5	, rc.top+dH*1+5	,rc.left+dW*8	, rc.top+dH*2);
	m_rectC[eC_TEACH	    ]	= CRect(rc.left+dW*8+5	, rc.top+dH*1+5	,rc.left+dW*12	, rc.top+dH*2);
	
	rc = m_rectS[eS_AREA_CONTROL_BTN_1];
	dH = (rc.Height()) / 1.;
	dW = (rc.Width()) / 10.;
	m_rectC[eC_SERVO_OFF]	= CRect(rc.left+dW*0	, rc.top+dH*0	,rc.left+dW*1-5	, rc.top+dH*1); 
	m_rectC[eC_SERVO_ON	]	= CRect(rc.left+dW*1	, rc.top+dH*0	,rc.left+dW*2-5	, rc.top+dH*1); 
	m_rectC[eC_RUN		]	= CRect(rc.left+dW*2	, rc.top+dH*0	,rc.left+dW*3-5	, rc.top+dH*1); 
	m_rectC[eC_STOP		]	= CRect(rc.left+dW*3	, rc.top+dH*0	,rc.left+dW*4-5	, rc.top+dH*1); 
	m_rectC[eC_IN		]	= CRect(rc.left+dW*4	, rc.top+dH*0	,rc.left+dW*5-5	, rc.top+dH*1); 
	m_rectC[eC_OUT		]	= CRect(rc.left+dW*5	, rc.top+dH*0	,rc.left+dW*6-5	, rc.top+dH*1); 
	m_rectC[eC_INIT		]	= CRect(rc.left+dW*6	, rc.top+dH*0	,rc.left+dW*7-5	, rc.top+dH*1); 
	m_rectC[eC_BUZZER	]	= CRect(rc.left+dW*7	, rc.top+dH*0	,rc.left+dW*8-5	, rc.top+dH*1); 
	m_rectC[eC_RESET	]	= CRect(rc.left+dW*8	, rc.top+dH*0	,rc.left+dW*9-5	, rc.top+dH*1); 
	m_rectC[eC_AGB_USE	]	= CRect(rc.left+dW*9	, rc.top+dH*0	,rc.left+dW*10-5, rc.top+dH*1); 

	rc = m_rectS[eS_AREA_CONTROL_BTN_2];
	dH = (rc.Height() - 5) / 2.;
	dW = (rc.Width() - 5) / 3.;
	m_rectC[eC_MESSAGE_HISTORY]	= CRect(rc.left+dW*0+5	, rc.top+dH*0+5	,rc.left+dW*1	, rc.top+dH*1);
	m_rectC[eC_LOG_DELETE]		= CRect(rc.left+dW*1+5	, rc.top+dH*0+5	,rc.left+dW*2	, rc.top+dH*1);	
	m_rectC[eC_AXIS		]		= CRect(rc.left+dW*2+5	, rc.top+dH*0+5	,rc.left+dW*3	, rc.top+dH*1);
	m_rectC[eC_DEMO		]		= CRect(rc.left+dW*0+5	, rc.top+dH*1+5	,rc.left+dW*1	, rc.top+dH*2);
	m_rectC[eC_SERIAL	]		= CRect(rc.left+dW*1+5	, rc.top+dH*1+5	,rc.left+dW*2	, rc.top+dH*2);
	m_rectC[eC_REVERSE	]		= CRect(rc.left+dW*2+5	, rc.top+dH*1+5	,rc.left+dW*3	, rc.top+dH*2);
	m_rectC[eC_KEYPAD	]		= CRect();//CRect(rc.left+dW*11+5		, rc.top+dH*0+5	,rc.left+dW*11.5, rc.top+dH*1);
}



void CTitleWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (sm.Option->tKeyPad.fShowKeyPad)
		return;
	SetFocus();
	for (int i=0; i<eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl			= i;
			InvalidateRect(m_rectC[i],FALSE);
			break;
		}
	}

	CFrameWndEx::OnRButtonDown(nFlags, point);
}


void CTitleWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	CString strLog;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (sm.Option->tKeyPad.fShowKeyPad)
		return;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_AXIS: {
				strLog = _T("Axis Control");
				CString strInput = InPutMessage(g_st[IDS_TITLE_INSERT_PASSWORD], TRUE);
				if (strInput.CompareNoCase(_T("MDI")) == 0) {
					g_ipClient.PostIPCommand("PLCLinker", "AxisControl", "Show");
				}
				else {
					IPMessage(g_st[IDS_TITLE_PASSWORD_INCORRECT]);
				}
			}
			
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(_T("Title"), _T("Title"), _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}

	m_eSelectControl	=	eC_NONE;
	Invalidate(FALSE);
	CFrameWndEx::OnRButtonUp(nFlags, point);
}


// KES 추가 2024.2.15
void CTitleWnd::ClosePopupUI() 
{
    if (m_pwndAutoRunChecker != nullptr) {
        m_pwndAutoRunChecker->ShowWindow(SW_HIDE);
    }
    if (m_pwndPassword != nullptr) {
        m_pwndPassword->ShowWindow(SW_HIDE);
    }
	if (m_pwndTPMCodeSelect != nullptr) {
		m_pwndTPMCodeSelect->ShowWindow(SW_HIDE);
	}
	if (m_pwndMessageHistory != nullptr) {
		m_pwndMessageHistory->ShowWindow(SW_HIDE);
	}
}

void CTitleWnd::DrawButtonImage(CDC* pDC, CRect rect, Gdiplus::Image* img, BOOL bDown, int iTick) {
	CRect rectImg(rect);

	rectImg.DeflateRect(iTick, iTick);
	DrawButtonUpDown(pDC, rect, _T(""), bDown);
	if (bDown) {
		rectImg.left += 1;
		rectImg.right += 1;
		rectImg.top += 1;
		rectImg.bottom += 1;
	}

	DrawImage(pDC, rectImg, img);
}

void CTitleWnd::SetLanguage(eLANGUAGE eLanguage)
{	
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if (pMain) {
 		pMain->ChagneLanguage();
	}
}

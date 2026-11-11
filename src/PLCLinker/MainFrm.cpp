
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "PLCLinker.h"
#include "MainFrm.h"
#include "AxisControlWnd.h"
#include "ModuleAxisStatusWnd.h"
#include "MotorStatusWnd.h"
#include "RegistryMapWnd.h"
#include "AxisAlarmWnd.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()



BEGIN_IP_COMMAND_TABLE(CMainFrame, IIPCommandTarget)


ON_IP_EXCL("Stage", "YaskawaInit", OnIPYaskawaInit)

ON_IP_EXCL("Stage", "SetMotorLimit", OnIPSetMotorLimit)
ON_IP("Stage", "Homming", OnIPHomming)
ON_IP("Stage", "HommingHome", OnIPHommingHome)
ON_IP("Stage", "HommingNPSensor", OnIPHommingNPSensor)
ON_IP("Stage", "HommingTorque", OnIPHommingTorque)

ON_IP_EXCL("Stage", "ServoAllOnOFF", OnIPServoAllOnOff)
ON_IP("Stage", "ServoOnOff", OnIPServoOnOff)
ON_IP("Stage", "AlarmClear", OnIPAlarmClear)


ON_IP("Stage", "MoveStep", OnIpMoveStep)
ON_IP("Stage", "MoveJog", OnIPMoveJog)
ON_IP("Stage", "MoveJogStop", OnIPMoveJogStop)
ON_IP("Stage", "MoveStop", OnIPMoveStop)


ON_IP("Stage", "MoveLiner", OnIPMoveLiner)
ON_IP("Stage", "MoveMulti", OnIPMoveMulti)

ON_IP("Stage", "PTPMove", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove0", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove1", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove2", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove3", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove4", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove5", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove6", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove7", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove8", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove9", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove10", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove11", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove12", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove13", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove14", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove15", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove16", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove17", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove18", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove19", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove20", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove21", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove22", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove23", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove24", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove25", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove26", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove27", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove28", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove29", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove30", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove31", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove32", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove33", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove34", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove35", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove36", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove37", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove38", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove39", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove40", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove41", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove42", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove43", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove44", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove45", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove46", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove47", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove48", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove49", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove50", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove51", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove52", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove53", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove54", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove55", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove56", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove57", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove58", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove59", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove60", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove61", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove62", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove63", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove64", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove65", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove66", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove67", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove68", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove69", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove70", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove71", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove72", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove73", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove74", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove75", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove76", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove77", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove78", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove79", OnIPPTPMove)
ON_IP_EXCL("Stage", "PTPMove80", OnIPPTPMove)

ON_IP("Stage", "TorqueMove", OnIPTorqueMove)
ON_IP("Stage", "AverageTorqueMove", OnIPAverageTorqueMove)

ON_IP_EXCL("Stage", "VisionMove", OnIPVisionMove)
END_IP_COMMAND_TABLE()
// CMainFrame 생성/소멸

void  GetAxsLineriData(const CIPVar& var, std::vector<T_POS>& _Liner){
	int iCount = var.GetChildItem("Count", 0);
	CStringA strAxis, strTarget;
	for(int i =0 ; i < iCount ;i++){
		T_POS liner;
		strAxis.Format("Axis%d", i);
		strTarget.Format("Target%d", i);
		liner.iAxis = var.GetChildItem(strAxis, -1);
		liner.dTarget = var.GetChildItem(strTarget, 0.);
		_Liner.push_back(liner);
	}
}

void  GetAxsMultiData(const CIPVar& var, std::vector<T_POS>& _Mult){
	int iCount = var.GetChildItem("Count", 0);
	CStringA strAxis, strSpeed, strTarget;
	for(int i =0 ; i < iCount ;i++){
		T_POS Mult;
		strAxis.Format("Axis%d", i);
		strSpeed.Format("Speed%d", i);
		strTarget.Format("Target%d", i);
		Mult.iAxis = var.GetChildItem(strAxis, -1);
		Mult.dSpeed = var.GetChildItem(strSpeed, sm.MotorParameter[Mult.iAxis].dPTPSpeed);
		//2020.12.27 LJC DoorOpen 시 움직임 속도 100으로 제한
		//2024.4.9 KES 인터락 강화 - 운전모드 Teach면 속도 100으로 제한
		if ((sm.Option->tSequence.eDriveMode == eDRIVE_MODE_TEACH || CheckDoorOpen_SCT_LD()) && Mult.dSpeed > 100)
			Mult.dSpeed = 100.;
		
		Mult.dTarget = var.GetChildItem(strTarget, 0.);
		_Mult.push_back(Mult);
	}
}
double GetLinerMoveTime(const std::vector<T_POS>& _Mult,const double dSpeed){
	double dTime = 0;
	double dDistance = 0;
	double dAddDistance = 0;
	double dCtr = 0;
	double dPercent = 0.;
	double dCtrSpeed = 0;
	for(int i = 0 ; i <_Mult.size(); i++){
		dCtr = GetAxisC2T(_Mult[i]);
		if( dDistance < dCtr){
			dDistance = dCtr;
		}
		dAddDistance += dCtr;
	}
	dPercent = dDistance / dAddDistance;
	dCtrSpeed = dSpeed * dPercent;
	dTime = (dDistance / dCtrSpeed) * 1000;
	return dTime;
}

CMainFrame::CMainFrame(): TIPCT<CFrameWnd>(&g_ipClient)
{
	m_pwndAxisControl	= nullptr;
	m_pwndMotorStatus	= nullptr;
	m_pwndAxisAlarm = nullptr;
}

CMainFrame::~CMainFrame()
{
	SAFE_DELETE( m_pwndAxisControl );
	SAFE_DELETE( m_pwndMotorStatus );

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("뷰 창을 만들지 못했습니다.\n");
		return -1;
	}

	m_pwndAxisControl = new CAxisControlWnd();
	if( !m_pwndAxisControl->Create( NULL, NULL, NULL)){
		return -1;
	}
	m_pwndMotorStatus = new CMotorStatusWnd();
	if( !m_pwndMotorStatus->Create( NULL, NULL, NULL)){
		return -1;
	}
	m_pwndRegistryMap = new CRegistryMapWnd();
	if( !m_pwndRegistryMap->Create( NULL, NULL, NULL)){
		return -1;
	}
	m_pwndAxisAlarm = new CAxisAlarmWnd();
	if (!m_pwndAxisAlarm->Create(NULL, NULL, NULL)) {
		return -1;
	}
	m_pwndModuleAxis = new CModuleAxisStatusWnd();
	if (!m_pwndModuleAxis->Create(NULL, NULL, NULL)) {
		return -1;
	}
	//m_pwndRegistryMap->ShowWindow(SW_SHOW);
	//m_pwndMotorStatus->ShowWindow(SW_SHOW);
	//m_pwndAxisControl->ShowWindow(SW_SHOW);
	//m_pwndModuleAxis->ShowWindow(SW_SHOW);
	//m_pwndAxisAlarm->ShowWindow(SW_SHOW);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 그렇지 않으면 기본 처리합니다.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL	CMainFrame::OnIPMoveJog(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	enum {
		S_INIT,
		S_FIRST_LIMIT_CHECK,
		S_MOVE_START,
		S_CHECK_LIMIT_N_STOP,
		S_END,
	};

	int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	BOOL bDir = var.GetChildItem("Dir", -1);
	double dSpeed = var.GetChildItem("Speed", -1.);
	int iCount = var.GetChildItem("Count", 0);
	int iCh = var.GetChildItem("Ch", 0);

	std::vector<T_POS> Data;

	if (iAxis == eMOTOR_END) {
		cmd.ReserveErrorStep();
		return FALSE;
	}
	
	if (!cmd.IsChildOK() || !cmd.IsOK()) {
		sm.MotorStatus[iAxis].tEtc.bJogStop = TRUE;
		GetAxsMultiData(var, Data);
		ys.JogStop(iAxis);
		for(int i = 0 ; i < Data.size() ; i++){
			ys.JogStop(Data[i].iAxis);
		}

		TRACE(__FUNCTION__  _T("-Step : %d"));
		return FALSE;
	}

	//<< 동작 함수 구동 중 ServoOff가 되어 있으면 에러 알림 후 종료.
	if (FALSE == sm.MotorStatus[iAxis].tCrt.bServoOn) {
		cmd.ReserveErrorStep();
		return TRUE;
	}
	//>>

	if (bDir < 0) {
		cmd.ReserveErrorStep();
		return TRUE;
	}
	if (dSpeed < 0.) {
		dSpeed = sm.MotorParameter[iAxis].dLowSpeed;
	}
	double& dPos = varCookie.ChildLValue<double>("Position", 0.);

	//2020.12.27 LJC DoorOpen 시 움직임 속도 100으로 제한
	//2024.4.9 KES 인터락 강화 - 운전모드 Teach면 속도 100으로 제한
	if ((sm.Option->tSequence.eDriveMode == eDRIVE_MODE_TEACH || CheckDoorOpen_SCT_LD()) && dSpeed > 100)
		dSpeed = 100.;
	
	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			
			sm.MotorStatus[iAxis].tEtc.bCmdJogStop		= FALSE;
			sm.MotorStatus[iAxis].tEtc.bJogStop			= FALSE;
			sm.MotorStatus[iAxis].tEtc.ullJogStartTick	=0;
			sm.MotorStatus[iAxis].tEtc.ullJogEndTick	=0;
			dPos = GetPosition(iAxis);
			cmd.EnqueueNextStep();
			break;
		}
		case S_FIRST_LIMIT_CHECK: {
			if (sm.IsSimultation()) {
				sm.MotorStatus[iAxis].tCrt.bInPosition = FALSE;
				sm.MotorStatus[iAxis].tCrt.dSpeed = dSpeed;
				cmd.EnqueueNextStep();
			}
			else {
				if (GetJogInterLock(iAxis, bDir, dSpeed, 1)) {// 맨마지막 변수는 앞으로 변할 위치의 시간값. 기본 0.1 , 0보다 작을 경우 0.01 단위는 SEC
					cmd.ReserveErrorStep();
				}
				else {
				cmd.EnqueueNextStep();
				}
			}
			break;
		}
		case S_MOVE_START: {
			sm.MotorStatus[iAxis].tEtc.ullJogStartTick = GetTickCount64();
			if (!sm.IsSimultation()) {
				if(iCount > 0){
					GetAxsMultiData(var, Data);
					if(!ys.MultiJog(Data, bDir)){
						cmd.ReserveErrorStep();
						break;
					}
				}else{
					if(!ys.JogMove(iAxis, bDir, dSpeed)){
						cmd.ReserveErrorStep();
						break;
					}
				}
			}
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK_LIMIT_N_STOP: {
			sm.MotorStatus[iAxis].tEtc.ullJogEndTick = GetTickCount64();
			double dTime = (sm.MotorStatus[iAxis].tEtc.ullJogEndTick - sm.MotorStatus[iAxis].tEtc.ullJogStartTick) / 1000.;
			if (sm.MotorStatus[iAxis].tEtc.bCmdJogStop) {
				if (sm.IsSimultation() ){
					sm.MotorStatus[iAxis].tCrt.bInPosition = TRUE;
					sm.MotorStatus[iAxis].tCrt.dSpeed = 0;
				}
				cmd.EnqueueNextStep();
				break;
			}
			if (sm.IsSimultation()) {
				double dDistance = dTime * dSpeed * (bDir ? 1.: -1.);
				sm.MotorStatus[iAxis].tCrt.dPos = dDistance + dPos;
				cmd.EnqueueNextStepCurrent();
			}
			else {
				if (GetJogInterLock(iAxis, bDir, dSpeed, 1)) {// 맨마지막 변수는 앞으로 변할 위치의 시간값. 기본 0.1 , 0보다 작을 경우 0.01 단위는 SEC
				
					cmd.ReserveErrorStep();
				}
				else {

				cmd.EnqueueNextStepCurrent();
				}
			}
			break;
		}
		case S_END: {
			cmd.EnqueueNextStep();
			break;
		}
	}
	return TRUE;
}
BOOL	CMainFrame::OnIPMoveJogStop(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	enum {
		S_INIT,
		S_JOG_STOP,
		S_CHECK_STOP,
		S_END,
	};
	int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	int iCount = var.GetChildItem("Count", 0);

	std::vector<T_POS> Data;
	
	if (iAxis == eMOTOR_END){
		cmd.ReserveErrorStep();
		return FALSE;
	}
	if (!cmd.IsChildOK() || !cmd.IsOK()) {
		
		GetAxsMultiData(var, Data);
		ys.MultiJogStop(Data);
		ys.JogStop(iAxis);
		TRACE(__FUNCTION__  _T("-Step : %d"));
		return FALSE;
	}
	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			if (iAxis < 0 ) {
				for (int i = 0; i < eMOTOR_END; i++) {
					ys.JogStop(i);
				}				
				cmd.EnqueueNextStep(S_END);

			}
			else {
				sm.MotorStatus[iAxis].tEtc.bCmdJogStop = TRUE;
				cmd.EnqueueNextStep();

			}
			
			break;
		}
		case S_JOG_STOP: {
			if (sm.IsSimultation()) {
				//sm.MotorStatus[iAxis].tCrt.bInPosition = TRUE;
			}
			else {
				if(iCount > 0){
					GetAxsMultiData(var, Data);
					ys.MultiJogStop(Data);
				}else{
					ys.JogStop(iAxis);
				}
			}
			
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK_STOP: {
			if (sm.MotorStatus[iAxis].tCrt.bInPosition ) {
				cmd.EnqueueNextStep();
			}
			else {
				cmd.EnqueueNextStepCurrent();
			}
			break;
		}
		case S_END: {
			cmd.EnqueueNextStep();
			break;
		}
	}

	return TRUE;
}
BOOL	CMainFrame::OnIpMoveStep(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;
	
	enum {
		S_INIT,
		S_CHECK_INTER_LOCK,
		S_MOVE_START,
		S_CHECK_TIME_OUT,
		S_END,
	};
	int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	double dSpeed = var.GetChildItem("Speed", -1);
	double dDistance = var.GetChildItem("Distacne", 0.);
	int iCh = var.GetChildItem("Chnnel", -1);
	double &dTarget = varCookie.ChildLValue<double>("Target", 0.);
	double &dTime = varCookie.ChildLValue<double>("Time", 0.);
	BOOL& bDir = varCookie.ChildLValue<BOOL>("Dir", 0);
	
	//2020.12.27 LJC DoorOpen 시 움직임 속도 100으로 제한
	//2024.4.9 KES 인터락 강화 - 운전모드 Teach면 속도 100으로 제한
	if ((sm.Option->tSequence.eDriveMode == eDRIVE_MODE_TEACH || CheckDoorOpen_SCT_LD()) && dSpeed > 100)
		dSpeed = 100.;	
	
	if (iAxis == eMOTOR_END){
		cmd.ReserveErrorStep();
		return FALSE;
	}
	if (!cmd.IsChildOK() || !cmd.IsOK()) {
		ys.MotionStop(iAxis);

		sm.MotorStatus[iAxis].tEtc.bIPRun = FALSE;
		sm.MotorStatus[iAxis].tEtc.bMoveStop = TRUE;
		TRACE(__FUNCTION__  _T("-Step : %d"));
		return FALSE;
	}
	if (dSpeed < 0) {
		dSpeed = sm.MotorParameter[iAxis].dLowSpeed;
	}
	if (fabs(dDistance) < 0.001) {
		cmd.ReserveErrorStep();
		return TRUE;
	}
	//<< 동작 함수 구동 중 ServoOff가 되어 있으면 에러 알림 후 종료.
	if (FALSE == sm.MotorStatus[iAxis].tCrt.bServoOn) {
		cmd.ReserveErrorStep();
		return TRUE;
	}
	//>>

	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			sm.MotorStatus[iAxis].tEtc.iMotorDesc = IDS_DES_NOT_USE;	// KES 추가 2024.3.26
			sm.MotorStatus[iAxis].tEtc.bIPRun = TRUE;
			sm.MotorStatus[iAxis].tEtc.ullEndTick = 0;
			sm.MotorStatus[iAxis].tEtc.bCmdMoveStop = FALSE;
			sm.MotorStatus[iAxis].tEtc.bMoveStop = FALSE;
			dTarget = sm.MotorStatus[iAxis].tCrt.dPos + dDistance;
			ys.ClearServoAlarm(iAxis);
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK_INTER_LOCK: {
			if (dDistance > 0 ) {
				bDir = 1;
			}
			else {
				bDir = 0;
			}
			if (GetPTPInterLcok(iAxis, dTarget, bDir)) {
				cmd.ReserveErrorStep();
			}
			else {
				
				cmd.EnqueueNextStep();
			}
			
			break;
		}
		case S_MOVE_START: {
			//2022.08.02 스토퍼 과부하 발생 후 서보On시 목표위치 이동으로 인해 현상 반복됨
			SetMoveTimeout(iAxis, ceil(GetTimeDistance(dDistance, dSpeed)));
			//sm.MotorStatus[iAxis].tEtc.dLastStartPos = GetPosition(iAxis);
			//sm.MotorStatus[iAxis].tEtc.dLastTargetPos = dTarget;
			//sm.MotorStatus[iAxis].tEtc.dLastTargetSpeed = dSpeed;

			if (!sm.IsSimultation()) {
				
				ys.StepMove(iAxis, dDistance, dSpeed);
				double dPos = GetAxisPos(iAxis) + dDistance;
				//ys.PTPMove(iAxis, dPos, dSpeed);
				
			}
			
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK_TIME_OUT: {
			BOOL bInpos = TRUE;
			sm.MotorStatus[iAxis].tEtc.ullEndTick = GetTickCount64();
			double dTime = sm.MotorStatus[iAxis].tEtc.ullEndTick - sm.MotorStatus[iAxis].tEtc.ullStartTick;
			if (sm.MotorStatus[iAxis].tEtc.bCmdMoveStop) {
				cmd.ReserveErrorStep();
				break;
			}
				
			if (sm.IsSimultation()) {
				if (dTime >= sm.MotorStatus[iAxis].tEtc.ullEstimated_time) {
					sm.MotorStatus[iAxis].tEtc.ullTactTime = sm.MotorStatus[iAxis].tEtc.ullEndTick - sm.MotorStatus[iAxis].tEtc.ullStartTick;
					sm.MotorStatus[iAxis].tCrt.dPos = dTarget;
					cmd.EnqueueNextStep();
				}
				else if (GetPTPInterLcok(iAxis, dTarget, bDir)) {
					cmd.ReserveErrorStep();
				}
				else {
					 sm.MotorStatus[iAxis].tCrt.dPos = sm.MotorStatus[iAxis].tEtc.dLastStartPos + (dTime * dSpeed);
					 cmd.EnqueueNextStepCurrent();
				}
			}else {
				if (GetInPosition(iAxis, dTarget)) {
					sm.MotorStatus[iAxis].tEtc.ullTactTime = sm.MotorStatus[iAxis].tEtc.ullEndTick - sm.MotorStatus[iAxis].tEtc.ullStartTick;
					cmd.EnqueueNextStep();
				}
				else if (GetPTPInterLcok(iAxis, dTarget, bDir)) {
					cmd.ReserveErrorStep();
				}
				else {
					ULONGLONG ullOverride = sm.MotorStatus[iAxis].tEtc.ullEstimated_time * (10000 / sm.Register->Reg_Word_1.dwOverdrive);
					if(sm.MotorStatus[iAxis].tEtc.ullEstimated_time < ullOverride){
						sm.MotorStatus[iAxis].tEtc.ullEstimated_time = ullOverride;
					}
					if (GetTimeout(iAxis)) {
						cmd.ReserveErrorStep();
					}
					else {
						cmd.EnqueueNextStepCurrent();
					}
				}
			}
			break;
		}
		case S_END: {
			//2022.08.02 스토퍼 과부하 발생 후 서보On시 목표위치 이동으로 인해 현상 반복됨
			sm.MotorStatus[iAxis].tEtc.dLastStartPos = GetPosition(iAxis);
			sm.MotorStatus[iAxis].tEtc.dLastTargetPos = dTarget;
			sm.MotorStatus[iAxis].tEtc.dLastTargetSpeed = dSpeed;

			sm.MotorStatus[iAxis].tEtc.bIPRun = FALSE;
			sm.MotorStatus[iAxis].tEtc.bMoveStop = TRUE;
			cmd.EnqueueNextStep();
			break;
		}
	}
	return TRUE;
}
BOOL	CMainFrame::OnIPMoveStop(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	enum {
		S_INIT,
		S_CMD_MOVE_STOP,
		S_CHECK_STOP,
		S_END,
	};
	int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	if (iAxis == eMOTOR_END){
		cmd.ReserveErrorStep();
		return FALSE;
	}
	if (!cmd.IsChildOK() || !cmd.IsOK()) {
		ys.MotionStop(iAxis);
		TRACE(__FUNCTION__  _T("-Step : %d"));
		return FALSE;
	}
	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			if( !sm.MotorStatus[iAxis].tEtc.bIPRun){
				ys.MotionStop(iAxis);
				ys.JogStop(iAxis);
				cmd.EnqueueNextStep(S_END, 100);
				break;
			}
			cmd.EnqueueNextStep();
			break;
		}
		case S_CMD_MOVE_STOP: {
			sm.MotorStatus[iAxis].tEtc.bCmdMoveStop = TRUE;
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK_STOP: {
			if (sm.MotorStatus[iAxis].tCrt.bInPosition){
				cmd.EnqueueNextStep();
			}
			else {
				cmd.EnqueueNextStepCurrent();
			}

			
			break;
		}
		case S_END: {
			cmd.EnqueueNextStep();
			break;
		}
	}
	return TRUE;
}
BOOL CMainFrame::OnIPHomming(CIPCommand& cmd) 
{
enum 
{
	S_INIT,
	S_SIGNAL_RESET,
	S_HOMMING,
	S_TIMEOUT_SET,
	S_HOME_SIGNAL_CHECK,
	S_END,
};
	CIPVar& var = cmd.m_var;
	int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	BOOL bCheck = TRUE;

	if (!cmd.IsChildOK() || !cmd.IsOK())
	{
		if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) 
		{
			sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = FALSE;
			sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = FALSE;
			sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = FALSE;
			sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = FALSE;
		}
		else 
		{
			sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = FALSE;
			sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = FALSE;
		}

		return FALSE;
	}

	switch (cmd.GetCurrentStep()) {
		case S_INIT: 
		{
			sm.MotorStatus[iAxis].tEtc.dwHomeStatus = eHOME_STATUS_HOMMING;
			cmd.m_dwCookie = GetTickCount64();
		}
		cmd.EnqueueNextStep();
		break;

		case S_SIGNAL_RESET:
		{
			
			// 홈완료 비트 끄고 홈중(ABS 상태 리셋) 비트 살림
			if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
				// MP 홈비트 Off
				bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] == FALSE;
				bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] == FALSE;
				// 홈 완료비트 Off
				bCheck &= sm.Register->Axis_ORG_PC.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] == FALSE;
				bCheck &= sm.Register->Axis_ORG_PC.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] == FALSE;
				// Homming 비트 On
				bCheck &= sm.Register->Axis_ABS_RESET_PC.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] == TRUE;
				bCheck &= sm.Register->Axis_ABS_RESET_PC.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] == TRUE;

				// PC Write
				sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = FALSE;
				sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = FALSE;
				sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = TRUE;
				sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = TRUE;
			}
			else 
			{
				// MP 홈비트 Off
				bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] == FALSE;
				// 홈 완료비트 Off
				bCheck &= sm.Register->Axis_ORG_PC.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] == FALSE;
				// Homming 비트 On
				bCheck &= sm.Register->Axis_ABS_RESET_PC.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] == TRUE;

				// PC Write
				sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = FALSE;
				sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = TRUE;
			}

			if (bCheck) 
			{
				cmd.EnqueueNextStep();
			}
			else 
			{
				if (GetTickCount64() - cmd.m_dwCookie > 5000)
				{
					cmd.ReserveErrorStep(S_SIGNAL_RESET);
				}
				else
				{
					cmd.EnqueueNextStepCurrent();
				}
			}
		}
		break;

		case S_HOMMING: {
			switch (sm.MotorParameter[iAxis].tHome.iHomeType) {
				case eHT_TORQUE: {
					SendIPStepCommand("Stage", "HommingTorque", var);
					break;
				}
				case eHT_HOME: {
					SendIPStepCommand("Stage", "HommingHome", var);
					break;
				}
				case eHT_NP_CENTER: {
					var.SetChildItem("Sensor", eHST_NP_CENTER); // Center
					SendIPStepCommand("Stage", "HommingNPSensor", var);
					break;
				}
				case eHT_N_SENSOR: {
					var.SetChildItem("Sensor", eHST_N_SENSOR); // N Sensor
					SendIPStepCommand("Stage", "HommingNPSensor", var);
					break;
				}
				case eHT_P_SENSOR: {
					var.SetChildItem("Sensor", eHST_P_SENSOR); // P Sensor
					SendIPStepCommand("Stage", "HommingNPSensor", var);
					break;
				}
				case eHT_DEFAULT: 
				{
					if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
						ys.SetOrizinAxis(sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1, sm.MotorParameter[iAxis].tHome.dOffset);
						ys.SetOrizinAxis(sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1, sm.MotorParameter[iAxis].tHome.dOffset+ sm.MotorParameter[iAxis].tHome.dOffset2Ch);
						
						sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = TRUE;
						sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = TRUE;
						
						sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = FALSE;
						sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = FALSE;

						bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] == TRUE;
						bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] == TRUE;
					}
					else {
						ys.SetOrizinLogical(iAxis, sm.MotorParameter[iAxis].tHome.dOffset);

						sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = TRUE;

						sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = TRUE;

						bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] == TRUE;
					}
					if (bCheck) {
						sm.MotorStatus[iAxis].tEtc.dwHomeStatus = eHOME_STATUS_COMPLETED;
						cmd.EnqueueNextStep();
					}
					else {
						cmd.EnqueueNextStepCurrent();
					}
					break;
				}
			}
			break;
		}

		case S_TIMEOUT_SET:
		{
			cmd.m_dwCookie = GetTickCount64();
			cmd.EnqueueNextStep();
		}
		break;

		case S_HOME_SIGNAL_CHECK:
		{
			BOOL bSensor = TRUE;
			if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry)
			{
				bSensor &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1];
				bSensor &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1];
			}
			else 
			{
				bSensor &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1];
			}

			if (bSensor)
			{
				if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry)
				{
					sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = FALSE;
					sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = FALSE;
				}
				else 
				{
					sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = FALSE;
				}

				cmd.EnqueueNextStep();
			}
			else
			{
				if (GetTickCount64() - cmd.m_dwCookie > 5000)
				{
					cmd.ReserveErrorStep();
				}
				else
				{
					cmd.EnqueueNextStepCurrent();
				}
			}
		}
		break;

		case S_END:
		{

		}
		break;
	}

	
	return TRUE;
}
BOOL	CMainFrame::OnIPHommingNPSensor(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	enum {
		S_INIT,
		S_SERVO_OFF,
		S_SERVO_ALARM_CLEAR,
		S_SERVO_ON,
		S_1ST_MOVE_DIR,
		S_1ST_SENSOR_ON_CHECK,
		S_1ST_MOVE_OFF_DIR,
		S_1ST_SENSOR_OFF_CHECK,
		S_1ST_POS,
		S_2ST_MOVE_DIR,
		S_2ST_SENSOR_ON_CHECK,
		S_2ST_MOVE_OFF_CHECK,
		S_2ST_SENSOR_OFF_CHECK,
		S_2ST_POS,
		S_CENTER_MOVE,
		S_SET_ZERO,
		S_MOVE_INIT_POS,
		S_END,
	};
	int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	int eTypeSensor = var.GetChildItem("Sensor", 1); // N Sensor
	BOOL bSensor1ST = 0;
	BOOL bSensor2ST = 0;

	if (eTypeSensor == eHST_NP_CENTER || eTypeSensor == eHST_N_SENSOR) {
		bSensor1ST = sm.MotorStatus[iAxis].tCrt.bNLimit;
		bSensor2ST = sm.MotorStatus[iAxis].tCrt.bPLimit;
	}
	else if (eTypeSensor == eHST_P_SENSOR) {
		bSensor1ST = sm.MotorStatus[iAxis].tCrt.bPLimit;
		bSensor2ST = sm.MotorStatus[iAxis].tCrt.bNLimit;
	}
	auto& MPh = sm.MotorParameter[iAxis].tHome;
	BOOL &bDir = varCookie.ChildLValue<BOOL>("Dir",0);
	BOOL& bServoOn = varCookie.ChildLValue<BOOL>("ServoOn", 0);
	double &dLimitH = varCookie.ChildLValue<double>("LimitH", 0.);
	double& dLimitL = varCookie.ChildLValue<double>("LimitL", 0.);
	double& bRate = varCookie.ChildLValue<double>("bRate", 0.);
	DWORD& dwTick = varCookie.ChildLValue<DWORD>("Tick", 0);
	if (!cmd.IsChildOK() || !cmd.IsOK()) {

		ys.JogStop(iAxis);
		ys.MotionStop(iAxis);
		sm.MotorStatus[iAxis].tEtc.dwHomeStatus = eHOME_STATUS_NOT_COMPLETED;

		return FALSE;
	}

	//<< 동작 함수 구동 중 ServoOff가 되어 있으면 에러 알림 후 종료.
	if (FALSE == sm.MotorStatus[iAxis].tCrt.bServoOn && bServoOn == TRUE) {
		cmd.ReserveErrorStep();
		return TRUE;
	}
	//>>
	if (cmd.GetCurrentStep() > S_INIT) {
		if ((GetTickCount() - dwTick) > sm.MotorParameter[iAxis].tHome.iHomeSerchTimeout* 1000) {
			cmd.ReserveErrorStep();
			return TRUE;
		}
	}
	BOOL bCheck = TRUE;
	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			dwTick = GetTickCount();
			sm.MotorStatus[iAxis].tEtc.dwHomeStatus = eHOME_STATUS_HOMMING;
			if (eTypeSensor == eHST_P_SENSOR) {
				bDir = 1;
			}
			else {
				bDir = 0;
			}
			bRate = sm.MotorParameter[iAxis].dPositionRate / 1000.;
			ys.SetOrizinLogical(iAxis, 0);
			ys.ClearServoAlarm(iAxis);
			cmd.EnqueueNextStep();
			break;
		}
		case S_SERVO_OFF: {
			//ys.ServoOnOff(iAxis, FALSE);
			bServoOn = FALSE;
			ServoOnOff(iAxis, FALSE, &cmd);
			cmd.PauseStep(500);
			break;
		}
		case S_SERVO_ALARM_CLEAR: {
			ys.ClearServoAlarm(iAxis);
			cmd.PauseStep(500);
			break;
		}
		case S_SERVO_ON: {
			//ys.ServoOnOff(iAxis, TRUE);
			ServoOnOff(iAxis, TRUE, &cmd);
			//. 500us wait.
			cmd.EnqueueNextStep(S_1ST_MOVE_DIR, 500);
			break;
		}
		case S_1ST_MOVE_DIR: {
			bServoOn = TRUE;

			if (bSensor1ST) {
				cmd.EnqueueNextStep(S_1ST_MOVE_OFF_DIR);
			}
			else {
				ys.ClearServoAlarm(iAxis);
				ys.JogMove(iAxis, bDir, MPh.dApproachVelocity);
				cmd.EnqueueNextStep();
			}

			break;
		}
		case S_1ST_SENSOR_ON_CHECK: {
			if (bSensor1ST) {
				ys.JogStop(iAxis);
				
				cmd.EnqueueNextStep(S_1ST_MOVE_OFF_DIR, 500);
			}
			else {
				cmd.EnqueueNextStepCurrent();
			}
			
			break;
		}
		case S_1ST_MOVE_OFF_DIR: {
			ys.ClearServoAlarm(iAxis);
			ys.JogMove(iAxis, !bDir, MPh.dCreepVelocity);
			cmd.EnqueueNextStep();
			break;
		}
		case S_1ST_SENSOR_OFF_CHECK: {
			if (!bSensor1ST) {
				ys.JogStop(iAxis);
				ys.ClearServoAlarm(iAxis);
				
				cmd.EnqueueNextStep(S_1ST_POS, 500);
			}
			else {
				cmd.EnqueueNextStepCurrent();
			}

			break;
		}
		case S_1ST_POS: {
			dLimitL = GetAxisPos(iAxis);
			if (eTypeSensor == eHST_N_SENSOR || eTypeSensor == eHST_P_SENSOR) {
				cmd.EnqueueNextStep(S_SET_ZERO);
			}
			else {
				cmd.EnqueueNextStep();
			}
			break;
		}
		case S_2ST_MOVE_DIR: {
			ys.ClearServoAlarm(iAxis);
			ys.JogMove(iAxis, !bDir, MPh.dApproachVelocity);
			cmd.EnqueueNextStep();
			break;
		}
		case S_2ST_SENSOR_ON_CHECK: {
			if (bSensor2ST) {
				ys.JogStop(iAxis);
				
				cmd.EnqueueNextStep();
			}else{
				cmd.EnqueueNextStepCurrent();
			}
			
			break;
		}
		case S_2ST_MOVE_OFF_CHECK: {
			ys.ClearServoAlarm(iAxis);
			ys.JogMove(iAxis, bDir, MPh.dCreepVelocity);
			cmd.EnqueueNextStep();
			break;
		}
		case S_2ST_SENSOR_OFF_CHECK: {
			if (!bSensor2ST) {
				ys.JogStop(iAxis);
				
				cmd.EnqueueNextStep(S_2ST_POS, 500);
			}
			else {
				cmd.EnqueueNextStepCurrent();
			}

			
			break;
		}
		case S_2ST_POS: {
			ys.ClearServoAlarm(iAxis);
			dLimitH = GetAxisPos(iAxis);
			cmd.EnqueueNextStep();
			break;
		}
		case S_CENTER_MOVE: {
			double dCenter = (dLimitL + dLimitH) / 2.;
			PTPMove((eAxis)iAxis, dCenter, MPh.dApproachVelocity, &cmd);
			cmd.PauseStep(500);
			break;
		}
		case S_SET_ZERO: {
			ys.SetOrizinLogical(iAxis, (-GetAxisPos(iAxis) + MPh.dOffset) *bRate);
			cmd.EnqueueNextStep(S_MOVE_INIT_POS, 500);
			break;
		}
		case S_MOVE_INIT_POS: {
			double dSpeed = sm.MotorParameter[iAxis].tHome.dVelocity;
			
			if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
				sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = TRUE;
				sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = TRUE;
				sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = FALSE;
				sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = FALSE;
			}
			else {
				sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = TRUE;
				sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = FALSE;
			}
			PTPMove((eAxis)iAxis, sm.MotorParameter[iAxis].tHome.dInitPosisition, dSpeed, &cmd);
			cmd.EnqueueNextStep(S_END, 500);
			break;
		}
		case S_END: {
			if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
				
				bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] == TRUE;
				bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] == TRUE;
			}
			else {
				bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] == TRUE;
			}
			if (bCheck) {
				if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry)
				{
					sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = FALSE;
					sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = FALSE;
				}
				else 
				{
					sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = FALSE;
				}

				sm.MotorParameter[iAxis].tYaskawa.iGantryCh = eCH_AXIS_ALL;
				sm.MotorStatus[iAxis].tEtc.dwHomeStatus = eHOME_STATUS_COMPLETED;
			
				cmd.EnqueueNextStep();
			}
			else {
				cmd.EnqueueNextStepCurrent();
			}
			break;
		}
	}

	return TRUE;
}
BOOL	CMainFrame::OnIPHommingHome(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	enum {
		S_INIT,
		S_SERVO_OFF,
		S_ALARM_CLEAR,
		S_SERVO_ON,
		S_N_DIR_MOVE,
		S_N_OR_H_SENSOR_CHECK,
		S_H_P_DIR_MOVE,
		S_N_CHECK_P_DIR_MOVE,
		S_H_ON_P_DIR_CHECK,
		S_H_OFF_P_DIR_CHECK,
		S_H_CHECK_N_DIR_MOVE,
		S_H_ON_CHECK,
		S_H_SET_OFFSET,
		S_GANTRY_2CH_HOME_MOVE,
		S_GANTRY_2CH_OFFSET_MOVE,
		S_GANTRY_2CH_OFFSET_CHECK,
		S_GANTRY_2CH_ZERO_SET,
		S_CHECK_END,
		S_MOVE_INIT_POS,
		S_END,
	};
	int iAxis = var.GetChildItem("Axis", eMOTOR_END);

	if (iAxis == eMOTOR_END){
		cmd.ReserveErrorStep();
		return FALSE;
	}

	if (!cmd.IsChildOK() || !cmd.IsOK()) {
		ys.MotionStop(iAxis);
		ys.JogStop(iAxis);
		sm.MotorStatus[iAxis].tEtc.dwHomeStatus = eHOME_STATUS_NOT_COMPLETED;

		sm.MotorParameter[iAxis].tYaskawa.iGantryCh = eCH_AXIS_ALL;

		TRACE(__FUNCTION__  _T("-Step : %d"));
		return FALSE;
	}


	//<< Cookie
	int& dwStart = varCookie.ChildLValue<int>("Start", 0);
	BOOL& bDir = varCookie.ChildLValue<BOOL>("Dir", 1);
	BOOL& bServoOn = varCookie.ChildLValue<BOOL>("ServoOn", 0);
	BOOL& bGantryCh = varCookie.ChildLValue<BOOL>("GantryCh", 0);
	double& b1to2Offset = varCookie.ChildLValue<double>("1to2Offset", 0.);
	DWORD& dwTick = varCookie.ChildLValue<DWORD>("Tick", 0);
	DWORD& dwCheckTick = varCookie.ChildLValue<DWORD>("CheckTick", 0);
	double& bRate = varCookie.ChildLValue<double>("bRate", 0.);
	BOOL& bTorque = varCookie.ChildLValue<BOOL>("Torque", 0);
	//>>

	//<< 동작 함수 구동 중 ServoOff가 되어 있으면 에러 알림 후 종료.
	if (FALSE == sm.MotorStatus[iAxis].tCrt.bServoOn && bServoOn == TRUE) {
	
		cmd.ReserveErrorStep();
		return TRUE;
	}
	//>>
	if (cmd.GetCurrentStep() > S_INIT) {
		if ((GetTickCount() - dwTick) > sm.MotorParameter[iAxis].tHome.iHomeSerchTimeout* 1000) {
			cmd.ReserveErrorStep();
			return TRUE;
		}
	}
	BOOL bCheck = TRUE;
	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			sm.MotorParameter[iAxis].tYaskawa.iGantryCh = eCH_AXIS_ALL;
			//sm.MotorStatus[iAxis].tEtc.bHomming = TRUE;
			//sm.MotorStatus[iAxis].tEtc.bHommingCompleted = FALSE;
			sm.MotorStatus[iAxis].tEtc.dwHomeStatus = eHOME_STATUS_HOMMING;
			dwTick = GetTickCount();

			bGantryCh = FALSE;
			if (sm.MotorParameter[iAxis].tHome.iDirection) {
				bDir = 1;
			}
			else {
				bDir = 0;
			}
			bRate = sm.MotorParameter[iAxis].dPositionRate / 1000.;
			cmd.EnqueueNextStep();
			break;
		}
		case S_SERVO_OFF: {
			bServoOn = FALSE;
			ServoOnOff(iAxis, FALSE, &cmd);
			//ys.ServoOnOff(iAxis, FALSE);
			cmd.EnqueueNextStep(S_ALARM_CLEAR, 500);
			break;
		}
		case S_ALARM_CLEAR: {
			ys.ClearServoAlarm(iAxis);
			cmd.EnqueueNextStep(S_SERVO_ON, 100);
			break;
		}
		case S_SERVO_ON: {
			if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
				ys.SetOrizinAxis(sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1, 0);
				ys.SetOrizinAxis(sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1, 0);
			}
			else {
				ys.SetOrizinAxis(sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1, 0);
			}

			ServoOnOff(iAxis, TRUE, &cmd);
			cmd.EnqueueNextStep(S_N_DIR_MOVE, 500);
			break;
		}
		case S_N_DIR_MOVE: {
			bServoOn = TRUE;
			ys.ClearServoAlarm(iAxis);
			if (sm.MotorStatus[iAxis].tCrt.bHomeSensor) {

				ys.JogMove(iAxis, !bDir, sm.MotorParameter[iAxis].tHome.dApproachVelocity);
				cmd.EnqueueNextStep(S_H_OFF_P_DIR_CHECK);
			}
			else {
				BOOL bSensor = sm.MotorStatus[iAxis].tCrt.bNLimit;
			
				if (bDir) {
					bSensor = sm.MotorStatus[iAxis].tCrt.bPLimit;
				}
				if(bSensor){
					dwCheckTick = GetTickCount();
					cmd.EnqueueNextStep(S_H_P_DIR_MOVE);
					
					break;
				}
				//ys.JogMove(iAxis, bDir, sm.MotorParameter[iAxis].tHome.dVelocity);
				double dSpeed = bDir == 1? sm.MotorParameter[iAxis].tHome.dVelocity : -sm.MotorParameter[iAxis].tHome.dVelocity;
				if (sm.MotorParameter[iAxis].tHome.dTorque < 10 || sm.MotorParameter[iAxis].tYaskawa.bSetGantry == TRUE)
				{
					ys.JogMove(iAxis, bDir, sm.MotorParameter[iAxis].tHome.dVelocity);
				}
				else {
					bTorque = TRUE;
					ys.TorqueMove(iAxis, sm.MotorParameter[iAxis].tHome.dTorque,  dSpeed);
				}
				dwCheckTick = GetTickCount();
				cmd.EnqueueNextStep();
			}
			break;
		}
		case S_N_OR_H_SENSOR_CHECK: {
			BOOL bSensor = sm.MotorStatus[iAxis].tCrt.bNLimit;
			
			if (bDir) {
				bSensor = sm.MotorStatus[iAxis].tCrt.bPLimit;
			}
			if (sm.MotorStatus[iAxis].tCrt.bHomeSensor) {
				if (bTorque) {
					ys.MotionStop(iAxis);
				}
				else {
					ys.JogStop(iAxis);
				}
				
				
				cmd.EnqueueNextStep(S_H_P_DIR_MOVE, 500);
			}
			else if(bSensor){
				ys.JogStop(iAxis);
				cmd.EnqueueNextStep(S_N_CHECK_P_DIR_MOVE, 500);
			}
			else {
				if (bGantryCh) {
					if (fabs(b1to2Offset - sm.MotorStatus[iAxis].tCrt.dPos) > 20.) {
						cmd.ReserveErrorStep();
						break;
					}
				}
				if( sm.MotorParameter[iAxis].tHome.dTorque > 10. && sm.MotorParameter[iAxis].tYaskawa.bSetGantry == FALSE){
					if (fabs(sm.MotorParameter[iAxis].tHome.dTorque - sm.MotorStatus[iAxis].tCrt.dTorque) < 0.01) {
						if (GetTickCount() - dwCheckTick > 3000) {
							ys.MotionStop(iAxis);
							cmd.EnqueueNextStep(S_N_CHECK_P_DIR_MOVE, 500);
							break;
						}
					}
					else {
						dwCheckTick = GetTickCount();
					}
				}
				cmd.EnqueueNextStepCurrent();
			}
			
			break;
		}
		case S_H_P_DIR_MOVE: {
			ys.ClearServoAlarm(iAxis);
			ys.JogMove(iAxis, !bDir, sm.MotorParameter[iAxis].tHome.dApproachVelocity);
			cmd.EnqueueNextStep(S_H_OFF_P_DIR_CHECK);
			break;
		}
		case S_N_CHECK_P_DIR_MOVE: {
			ys.ClearServoAlarm(iAxis);
			ys.JogMove(iAxis, !bDir, sm.MotorParameter[iAxis].tHome.dApproachVelocity);
			cmd.EnqueueNextStep();
			break;
		}
		case S_H_ON_P_DIR_CHECK: {
			if (sm.MotorStatus[iAxis].tCrt.bHomeSensor) {
				cmd.EnqueueNextStep();
			}
			else {
				cmd.EnqueueNextStepCurrent();
			}
			break;
		}
		case S_H_OFF_P_DIR_CHECK: {
			if (!sm.MotorStatus[iAxis].tCrt.bHomeSensor) {
				ys.JogStop(iAxis);
				cmd.EnqueueNextStep(S_H_CHECK_N_DIR_MOVE, 500);
			}
			else {
				cmd.EnqueueNextStepCurrent();
			}
			break;
		}
		case S_H_CHECK_N_DIR_MOVE: {
			ys.ClearServoAlarm(iAxis);
			ys.JogMove(iAxis, bDir, sm.MotorParameter[iAxis].tHome.dCreepVelocity);

			cmd.EnqueueNextStep();
			break;
		}
		case S_H_ON_CHECK: {
			if (sm.MotorStatus[iAxis].tCrt.bHomeSensor) {
				ys.JogStop(iAxis);
				cmd.EnqueueNextStep(S_H_SET_OFFSET, 500);
			}
			else {
				cmd.EnqueueNextStepCurrent();
			}
			break;
		}
		case S_H_SET_OFFSET: {
			if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
				if (bGantryCh) {
					ys.SetOrizinAxis(sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1, (-sm.MotorStatus[iAxis].tCrt.dPos) *bRate);
					cmd.EnqueueNextStep(S_GANTRY_2CH_OFFSET_MOVE, 500);
				}
				else {
					ys.SetOrizinAxis(sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1, (-sm.MotorStatus[iAxis].tCrt.dPos + sm.MotorParameter[iAxis].tHome.dOffset) *bRate);
					
					cmd.EnqueueNextStep(S_GANTRY_2CH_HOME_MOVE, 500);
				}
				
			}
			else {
				ys.SetOrizinAxis(sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1, (-sm.MotorStatus[iAxis].tCrt.dPos + sm.MotorParameter[iAxis].tHome.dOffset) *bRate);
				cmd.EnqueueNextStep(S_CHECK_END, 500);
			}

			break;
		}
		case S_GANTRY_2CH_HOME_MOVE: {
			if (sm.MotorParameter[iAxis].tHome.bGantry2ChUse && sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
				ys.SetOrizinAxis(sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1, 0);
				b1to2Offset = sm.MotorStatus[iAxis].tCrt.tGantryData.dCh2Pos;
				sm.MotorParameter[iAxis].tYaskawa.iGantryCh = eCH_AXIS_2;
				
				bGantryCh = TRUE;

				cmd.EnqueueNextStep(S_N_DIR_MOVE, 500);
			}
			else {
				ys.SetOrizinAxis(sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1, (-sm.MotorStatus[iAxis].tCrt.tGantryData.dCh2Pos + sm.MotorParameter[iAxis].tHome.dOffset) *bRate);
				cmd.EnqueueNextStep(S_CHECK_END);
			}
			break;
		}
		case S_GANTRY_2CH_OFFSET_MOVE: {
			if (fabs(sm.MotorParameter[iAxis].tHome.dOffset2Ch) > 0.) {
				ys.PTPMove(iAxis, sm.MotorParameter[iAxis].tHome.dOffset2Ch, sm.MotorParameter[iAxis].tHome.dApproachVelocity);
				dwTick = GetTickCount();
				cmd.EnqueueNextStep();
			}
			else {
				
				cmd.EnqueueNextStep(S_GANTRY_2CH_ZERO_SET, 500);
			}
			break;
		}
		case S_GANTRY_2CH_OFFSET_CHECK: {

			if (fabs(sm.MotorStatus[iAxis].tCrt.dPos - sm.MotorParameter[iAxis].tHome.dOffset2Ch) < sm.MotorParameter[iAxis].dInPosition) {
				if ((GetTickCount() - dwTick) > 1000) {
					cmd.EnqueueNextStep(S_GANTRY_2CH_ZERO_SET, 500);
				}
				else {
					cmd.EnqueueNextStepCurrent();
				}
			}
			else {
				dwTick = GetTickCount();
				cmd.EnqueueNextStepCurrent();
			}
			break;
		}
		case S_GANTRY_2CH_ZERO_SET: {
			double dPosOffset = ys.GetOffset0x48Logcal(sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis);
			ys.SetOrizinAxis(sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1, (dPosOffset - sm.MotorStatus[iAxis].tCrt.dPos + sm.MotorParameter[iAxis].tHome.dOffset) *bRate);
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK_END: {
			
			cmd.EnqueueNextStep(S_MOVE_INIT_POS, 500);
			break;
		}
		case S_MOVE_INIT_POS: {
			sm.MotorParameter[iAxis].tYaskawa.iGantryCh = eCH_AXIS_ALL;
			double dSpeed = sm.MotorParameter[iAxis].tHome.dVelocity;

			if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
				sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = TRUE;
				sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = TRUE;
				sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = FALSE;
				sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = FALSE;
			}
			else {
				sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = TRUE;
				sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = FALSE;
			}
			PTPMove((eAxis)iAxis, sm.MotorParameter[iAxis].tHome.dInitPosisition, dSpeed, &cmd);
			cmd.EnqueueNextStep(S_END, 500);
			break;
		}
		case S_END: {
			if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
				
				bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] == TRUE;
				bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] == TRUE;
			}
			else {
				bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] == TRUE;
			}
			if (bCheck) {
				if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry)
				{
					sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = FALSE;
					sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = FALSE;
				}
				else 
				{
					sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = FALSE;
				}

				sm.MotorParameter[iAxis].tYaskawa.iGantryCh = eCH_AXIS_ALL;
				sm.MotorStatus[iAxis].tEtc.dwHomeStatus = eHOME_STATUS_COMPLETED;
			
				cmd.EnqueueNextStep();
			}
			else {
				cmd.EnqueueNextStepCurrent();
			}
			
			break;
		}
	}
	return TRUE;
}
BOOL CMainFrame::OnIPHommingTorque(CIPCommand& cmd){
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	enum {
		S_INIT,
		S_SERVO_OFF,
		S_ALARM_CLEAR,
		S_SERVO_ON,
		S_MOVE_N_LIMIT,
		S_N_CHECK,
		S_SET_ZERO,
		S_MOVE_INIT_POS,
		S_END,
	};

	int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	BOOL bOnOff = var.GetChildItem("OnOff", FALSE);
	BOOL& bServoOn = varCookie.ChildLValue<BOOL>("ServoOn", 0);
	DWORD &dwStart = varCookie.ChildLValue<DWORD>("STick", 0);
	int& iDir = varCookie.ChildLValue<int>("Dir", 1);
	double& bRate = varCookie.ChildLValue<double>("bRate", 0.);
	DWORD& dwTick = varCookie.ChildLValue<DWORD>("Tick", 0);
	if (iAxis == eMOTOR_END){
		cmd.ReserveErrorStep();
		return FALSE;
	}
	
	if (!cmd.IsChildOK() || !cmd.IsOK()) {
		ys.JogStop(iAxis);
		ys.MotionStop(iAxis);
		sm.MotorStatus[iAxis].tEtc.dwHomeStatus = eHOME_STATUS_NOT_COMPLETED;
		TRACE(__FUNCTION__  _T("-Step : %d"));
		return FALSE;
	}

	//<< 동작 함수 구동 중 ServoOff가 되어 있으면 에러 알림 후 종료.
	if (FALSE == sm.MotorStatus[iAxis].tCrt.bServoOn && bServoOn == TRUE) {
		cmd.ReserveErrorStep();
		return TRUE;
	}
	//>>
	if (cmd.GetCurrentStep() > S_INIT) {
		if ((GetTickCount() - dwTick) > sm.MotorParameter[iAxis].tHome.iHomeSerchTimeout* 1000) {
			cmd.ReserveErrorStep();
			return TRUE;
		}
	}
	BOOL bCheck = TRUE;
	switch (cmd.GetCurrentStep()) {
	case S_INIT: {
		dwTick = GetTickCount();
		sm.MotorStatus[iAxis].tEtc.dwHomeStatus = eHOME_STATUS_HOMMING;

		if (sm.MotorParameter[iAxis].tHome.iDirection) {
			iDir = 1;
		}
		else {
			iDir = -1;
		}
		bRate = sm.MotorParameter[iAxis].dPositionRate / 1000.;
		ys.SetOrizinResetLogical(iAxis);
		cmd.EnqueueNextStep();
		break;
	}
	case S_SERVO_OFF:{
		//ys.ServoOnOff(iAxis, FALSE);
		ServoOnOff(iAxis, FALSE, &cmd);
		bServoOn = FALSE;
		cmd.EnqueueNextStep();
		break;
	}
	case S_ALARM_CLEAR:{
		ys.ClearServoAlarm(iAxis);
		cmd.EnqueueNextStep();
		break;
	}
	case S_SERVO_ON:{
		//ys.ServoOnOff(iAxis, TRUE);
		ServoOnOff(iAxis, TRUE, &cmd);
		//. 500us wait.
		bServoOn = TRUE;
		cmd.EnqueueNextStep(S_MOVE_N_LIMIT, 500);
		break;
	}
	
	case S_MOVE_N_LIMIT:{
		ys.TorqueMove(iAxis, sm.MotorParameter[iAxis].tHome.dTorque, iDir*sm.MotorParameter[iAxis].tHome.dCreepVelocity);
		cmd.EnqueueNextStep();
		break;
	}
	case S_N_CHECK:{
		if( fabs(sm.MotorStatus[iAxis].tCrt.dTorque - sm.MotorParameter[iAxis].tHome.dTorque) < 0.05){
			if((GetTickCount() - dwStart) >3*1000){
				
				cmd.EnqueueNextStep(-1, 300);
			}else{
				cmd.EnqueueNextStepCurrent();
			}
		}else{
			dwStart = GetTickCount();
			cmd.EnqueueNextStepCurrent();
		}
		
		break;
	}
	case S_SET_ZERO:{
		double dPos = -GetPos(iAxis) + sm.MotorParameter[iAxis].tHome.dOffset;
		ys.SetOrizinLogical(iAxis, dPos *bRate);

		ys.MotionStop(iAxis);
		cmd.EnqueueNextStep(S_MOVE_INIT_POS, 500);
		break;
	}
	case S_MOVE_INIT_POS: {
		double dSpeed = sm.MotorParameter[iAxis].tHome.dApproachVelocity;
		
		if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
			sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = TRUE;
			sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = TRUE;
			sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = FALSE;
			sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = FALSE;
		}
		else {
			sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = TRUE;
			sm.Register->Axis_ABS_RESET_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = FALSE;
		}
		PTPMove((eAxis)iAxis, sm.MotorParameter[iAxis].tHome.dInitPosisition, dSpeed, &cmd);
		cmd.EnqueueNextStep(S_END, 500);
		break;
	}
	case S_END: {
		if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
			bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] == TRUE;
			bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] == TRUE;
		}
		else {
			bCheck &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] == TRUE;
		}
		if (bCheck) {
			if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry)
			{
				sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1] = FALSE;
				sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1] = FALSE;
			}
			else 
			{
				sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1] = FALSE;
			}

			sm.MotorParameter[iAxis].tYaskawa.iGantryCh = eCH_AXIS_ALL;
			sm.MotorStatus[iAxis].tEtc.dwHomeStatus = eHOME_STATUS_COMPLETED;
		
			cmd.EnqueueNextStep();
		}
		else {
			cmd.EnqueueNextStepCurrent();
		}
		break;
	}
	}
	return TRUE;
}
BOOL	CMainFrame::OnIPServoOnOff(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	enum {
		S_INIT,

		S_SERVO_OFF_ON,
		S_SERVO_OFF_OFF,

		S_SERVO_CMD,
		S_SERVO_CHECK,
		S_SERVO_ON_OFF,
		S_GANTRY_CHECK,
		//2022.07.29 수정
		S_SERVO_ON_COMPLETE,
		S_RECOVERY_POS, 
		S_END,
	};
	int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	BOOL bOnOff = var.GetChildItem("OnOff", FALSE);
	BOOL bAll = var.GetChildItem("All", FALSE);
	
	double& dTarget = varCookie.ChildLValue<double>("Target", 0.);
	if (iAxis == eMOTOR_END){
		cmd.ReserveErrorStep();
		return FALSE;
	}
	if (!cmd.IsChildOK() || !cmd.IsOK()) {
		TRACE(__FUNCTION__  _T("-Step : %d"));
		return FALSE;
	}
	auto& MP = sm.MotorParameter[iAxis].tYaskawa;
	BOOL bCheck = TRUE;
	DWORD& dwTick = varCookie.ChildLValue<DWORD>("Tick", 0);
	switch (cmd.GetCurrentStep()) {
	case S_INIT: {
		if (!bOnOff) {
			cmd.EnqueueNextStep(S_SERVO_CMD);
			break;
		}
		sm.Register->Out_New.dw1000_AlarmReset = TRUE;
		if (sm.Register->In.dw2000_AlarmResetCheck) {
				cmd.EnqueueNextStep();
		}
		else {
			cmd.EnqueueNextStepCurrent();
		}
		break;
	}
	case S_SERVO_OFF_ON: {
		if (MP.bSetGantry) {
			sm.Register->Axis_Off_NEW.dwAxis[MP.iCh1_Axis - 1] = TRUE;
			sm.Register->Axis_Off_NEW.dwAxis[MP.iCh2_Axis - 1] = TRUE;
			bCheck &= sm.Register->Axis_Off_NEW.dwAxis[MP.iCh1_Axis - 1] == sm.Register->Axis_Off.dwAxis[MP.iCh1_Axis - 1];
			bCheck &= sm.Register->Axis_Off_NEW.dwAxis[MP.iCh2_Axis - 1] == sm.Register->Axis_Off.dwAxis[MP.iCh2_Axis - 1];
		}
		else {
			sm.Register->Axis_Off_NEW.dwAxis[MP.iLogicalAxisNo - 1] = TRUE;
			bCheck &= sm.Register->Axis_Off_NEW.dwAxis[MP.iLogicalAxisNo - 1] == sm.Register->Axis_Off.dwAxis[MP.iLogicalAxisNo - 1];
		}
		if (bCheck) {
			cmd.EnqueueNextStep();
		}
		else {
			cmd.EnqueueNextStepCurrent();
		}
		break;
	}
	case S_SERVO_OFF_OFF: {
		if (MP.bSetGantry) {
			sm.Register->Axis_Off_NEW.dwAxis[MP.iCh1_Axis - 1] = FALSE;
			sm.Register->Axis_Off_NEW.dwAxis[MP.iCh2_Axis - 1] = FALSE;
			bCheck &= sm.Register->Axis_Off_NEW.dwAxis[MP.iCh1_Axis - 1] == sm.Register->Axis_Off.dwAxis[MP.iCh1_Axis - 1];
			bCheck &= sm.Register->Axis_Off_NEW.dwAxis[MP.iCh2_Axis - 1] == sm.Register->Axis_Off.dwAxis[MP.iCh2_Axis - 1];
		}
		else {
			sm.Register->Axis_Off_NEW.dwAxis[MP.iLogicalAxisNo - 1] = FALSE;
			bCheck &= sm.Register->Axis_Off_NEW.dwAxis[MP.iLogicalAxisNo - 1] == sm.Register->Axis_Off.dwAxis[MP.iLogicalAxisNo - 1];
		}
		if (bCheck) {
			cmd.EnqueueNextStep();
		}
		else {
			cmd.EnqueueNextStepCurrent();
		}
		break;
	}
	case S_SERVO_CMD: {
		
		if (MP.bSetGantry) {
			if (bOnOff) {
				sm.Register->Axis_On_NEW.dwAxis[MP.iCh1_Axis - 1] = TRUE;
				sm.Register->Axis_On_NEW.dwAxis[MP.iCh2_Axis - 1] = TRUE;
				bCheck &= sm.Register->Axis_On_NEW.dwAxis[MP.iCh1_Axis - 1] == sm.Register->Axis_On.dwAxis[MP.iCh1_Axis - 1];
				bCheck &= sm.Register->Axis_On_NEW.dwAxis[MP.iCh2_Axis - 1] == sm.Register->Axis_On.dwAxis[MP.iCh2_Axis - 1];
			}
			else {
				sm.Register->Axis_Off_NEW.dwAxis[MP.iCh1_Axis - 1] = TRUE;
				sm.Register->Axis_Off_NEW.dwAxis[MP.iCh2_Axis - 1] = TRUE;
				bCheck &= sm.Register->Axis_Off_NEW.dwAxis[MP.iCh1_Axis - 1] == sm.Register->Axis_Off.dwAxis[MP.iCh1_Axis - 1];
				bCheck &= sm.Register->Axis_Off_NEW.dwAxis[MP.iCh2_Axis - 1] == sm.Register->Axis_Off.dwAxis[MP.iCh2_Axis - 1];
			}
		}
		else {
			if (bOnOff) {
				sm.Register->Axis_On_NEW.dwAxis[MP.iLogicalAxisNo - 1] = TRUE;
				bCheck &= sm.Register->Axis_On_NEW.dwAxis[MP.iLogicalAxisNo - 1] == sm.Register->Axis_On.dwAxis[MP.iLogicalAxisNo - 1];
 			}
			else {
				sm.Register->Axis_Off_NEW.dwAxis[MP.iLogicalAxisNo - 1] = TRUE;
				bCheck &= sm.Register->Axis_Off_NEW.dwAxis[MP.iLogicalAxisNo - 1] == sm.Register->Axis_Off.dwAxis[MP.iLogicalAxisNo - 1];
				
			}
		}
		if (bCheck) {
			dwTick = GetTickCount();
			cmd.EnqueueNextStep();
		}
		else {
			cmd.EnqueueNextStepCurrent();
		}
		break;
	}
	case S_SERVO_CHECK: {
		
		if (bOnOff) {
			if (MP.bSetGantry) {
				bCheck &= sm.MotorStatus[iAxis].tCrt.tGantryData.dwChServoOnStatus == eCH_GANTRY_AXIS_ALL_STATE;
			}
			else {
				bCheck &= sm.MotorStatus[iAxis].tCrt.bServoOn == TRUE;
			}
			if (bCheck) {
				
				cmd.EnqueueNextStep();
				break;
			}
			else {
				if (GetTickCount() - dwTick > 1000) {
					cmd.EnqueueNextStep(S_SERVO_ON_COMPLETE);
					break;
				}
				cmd.EnqueueNextStepCurrent();
				break;
			}
		}
		else {
			
			cmd.EnqueueNextStep(S_SERVO_ON_COMPLETE);
			break;
		}
	}
	case S_SERVO_ON_OFF: {
		
		//ys.ServoOnOff(iAxis, bOnOff);
		if( bAll){
			cmd.EnqueueNextStep(S_SERVO_ON_COMPLETE);
			break;
		}
		
		double dOffset = sm.MotorStatus[iAxis].tCrt.tGantryData.dCh1Pos - sm.MotorStatus[iAxis].tCrt.tGantryData.dCh2Pos;
		if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry && sm.MotorStatus[iAxis].tEtc.dwHomeStatus == eHOME_STATUS_COMPLETED) {
			if (fabs(dOffset) > 0.001) {
				dTarget = sm.MotorStatus[iAxis].tCrt.tGantryData.dCh1Pos;
				sm.MotorParameter[iAxis].tYaskawa.iGantryCh = eCH_AXIS_2;
				cmd.EnqueueNextStep(S_GANTRY_CHECK, 1000);
				break;
			}
			cmd.EnqueueNextStep(S_SERVO_ON_COMPLETE);
			break;
		}
		else {
			cmd.EnqueueNextStep(S_SERVO_ON_COMPLETE);
		}
		
		
		break;
	}
	case S_GANTRY_CHECK: {
			
		CIPVar varMove;
		varMove.SetChildItem("Axis", iAxis);
		varMove.SetChildItem("Speed", 20.);
		varMove.SetChildItem("Target", dTarget);
		SendIPStepCommand("Stage", "PTPMove", varMove);
		
		cmd.EnqueueNextStep();
		break;
	}
	case S_SERVO_ON_COMPLETE: {
		if (MP.bSetGantry) {
			sm.Register->Axis_On_NEW.dwAxis[MP.iCh1_Axis - 1] = FALSE;
			sm.Register->Axis_On_NEW.dwAxis[MP.iCh2_Axis - 1] = FALSE;
			sm.Register->Axis_Off_NEW.dwAxis[MP.iCh1_Axis - 1] = FALSE;
			sm.Register->Axis_Off_NEW.dwAxis[MP.iCh2_Axis - 1] = FALSE;
		}
		else {
			sm.Register->Axis_On_NEW.dwAxis[MP.iLogicalAxisNo - 1] = FALSE;
			sm.Register->Axis_Off_NEW.dwAxis[MP.iLogicalAxisNo - 1] = FALSE;			
		}
		sm.MotorParameter[iAxis].tYaskawa.iGantryCh = eCH_AXIS_ALL;
		cmd.EnqueueNextStep();
		break;
	}

	case S_RECOVERY_POS:
	{
		if (bOnOff) 
			LastTargetMove(iAxis, &cmd);
	}
	cmd.EnqueueNextStep();
	break;

	case S_END:
	{
		// .. 아직 내용 없음
	}
	cmd.EnqueueNextStep();
	break;

	}
	return TRUE;
}
BOOL	CMainFrame::OnIPAlarmClear(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	enum {
		S_INIT,
		S_CLEAR_ALARM,
		S_END,
	};
	int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	
	if (iAxis == eMOTOR_END){
		cmd.ReserveErrorStep();
		return FALSE;
	}
	if (!cmd.IsChildOK() || !cmd.IsOK()) {
		TRACE(__FUNCTION__  _T("-Step : %d"));
		return FALSE;
	}
	switch (cmd.GetCurrentStep()) {
	case S_INIT: {
		cmd.EnqueueNextStep();
		break;
	}
	case S_CLEAR_ALARM: {
		ys.ClearServoAlarm(iAxis);
		cmd.EnqueueNextStep();
		break;
	}
	case S_END: {
		cmd.EnqueueNextStep();
		break;
	}
	}
	return TRUE;
}
BOOL	CMainFrame::OnIPServoAllOnOff(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;
enum {
		S_INIT,
		S_ALL_ONOFF,
		S_GANTRY_OFFSET_CHECK,
		S_GANTRY_OFFSET_MOVE,

		//2022.07.29 수정
		S_SERVO_ON_COMPLETE,
		S_RECOVERY_POS,
		S_END,
	};
	
	BOOL bOnOff = var.GetChildItem("OnOff", FALSE);
	

	if (!cmd.IsChildOK() || !cmd.IsOK()) {
		TRACE(__FUNCTION__  _T("-Step : %d"));
		return FALSE;
	}
	switch (cmd.GetCurrentStep()) {
	case S_INIT: {

		for(int i =0 ; i < eMOTOR_END; i++){ 
			if (!bOnOff) {
				if( sm.MotorStatus[i].tCrt.bServoOn){
					ys.JogStop(i);
					ys.MotionStop(i);
				}
			}
			sm.MotorParameter[i].tYaskawa.iGantryCh = eCH_AXIS_ALL;
		}
		cmd.EnqueueNextStep();
		break;
	}
	case S_ALL_ONOFF: 
	{	
		for (int i =0 ; i < eMOTOR_END; i++)
		{ 
			ServoOnOff(i, bOnOff, &cmd, TRUE);			
		}
			
		if (!bOnOff)
		{
			cmd.EnqueueNextStep(S_SERVO_ON_COMPLETE);
			break;
		}

		cmd.EnqueueNextStep();
		break;
	}
	case S_GANTRY_OFFSET_CHECK:
	{
		// 2022.07.27 수정 - 전체 Servo Off -> On 할때 갠트리 보정이 안되는 축이 있음
		int nAxis = 0;
		for (int iAxis = 0; iAxis < eMOTOR_END; ++iAxis)
		{ 
			if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry && sm.MotorStatus[iAxis].tEtc.dwHomeStatus != eHOME_STATUS_COMPLETED)
				continue;

			double dOffset = sm.MotorStatus[iAxis].tCrt.tGantryData.dCh1Pos - sm.MotorStatus[iAxis].tCrt.tGantryData.dCh2Pos;
			if (fabs(dOffset) < 0.001)
				continue;

			varCookie.SetChildItem(FormatA("Axis%d", nAxis), iAxis);
			varCookie.SetChildItem(FormatA("Target%d", nAxis), sm.MotorStatus[iAxis].tCrt.tGantryData.dCh1Pos);
			sm.MotorParameter[iAxis].tYaskawa.iGantryCh = eCH_AXIS_2;
			nAxis++;
		}
		varCookie.SetChildItem("Count", nAxis);
	}
	cmd.EnqueueNextStep(S_GANTRY_OFFSET_MOVE, 1000);
	break;

	case S_GANTRY_OFFSET_MOVE:
	{
		//<< 2024.06.10 LBH
		if (!bOnOff) {
			cmd.EnqueueNextStep();
			break;
		}
		//>>
		int nAxis = varCookie.GetChildItem("Count", 0);
		if (nAxis == 0)
		{
			cmd.EnqueueNextStep();
			break;
		}
		
		for (int idx = 0; idx < nAxis; ++idx)
		{
			int iAxis = varCookie.GetChildItem(FormatA("Axis%d", idx), idx);
			double dTarget = varCookie.GetChildItem(FormatA("Target%d", idx), sm.MotorStatus[iAxis].tCrt.tGantryData.dCh1Pos);
				
			CIPVar varMove;
			varMove.SetChildItem("Axis", iAxis);
			varMove.SetChildItem("Speed", 20.);
			varMove.SetChildItem("Target", dTarget);
			SendIPStepCommand("Stage", "PTPMove", varMove);
		}
	}
	cmd.EnqueueNextStep();
	break;

	case S_SERVO_ON_COMPLETE: 
	{
		for(int i =0 ; i < eMOTOR_END; i++){ 
			sm.MotorParameter[i].tYaskawa.iGantryCh = eCH_AXIS_ALL;
		}
		cmd.EnqueueNextStep();
		break;
	}

	case S_RECOVERY_POS:
	{
		//<< 2024.06.10 LBH
		if (!bOnOff) {
			cmd.EnqueueNextStep();
			break;
		}
		//>>
		for (int iAxis = 0; iAxis < eMOTOR_END; ++iAxis)
		{
			LastTargetMove(iAxis, &cmd);
		}
	}
	cmd.EnqueueNextStep();
	break;

	case S_END:
	{
		// .. 아직 내용 없음
	}
	cmd.EnqueueNextStep();
	break;

	}
	return TRUE;
}

BOOL	CMainFrame::OnIPPTPMove(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	enum {
		S_INIT,
		S_CHECK_INTER_LOCK,
		S_MOVE_START,
		S_CHECK_TIME_OUT,
		S_END,
	};
	int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	double dSpeed = var.GetChildItem("Speed", -1);
	double dTarget = var.GetChildItem("Target", 0.);
	int iCh = var.GetChildItem("Ch", 0);

	double &dDistance = varCookie.ChildLValue<double>("Distacne", 0.);
	double &dTime = varCookie.ChildLValue<double>("Time", 0.);
	double& dDirVal = varCookie.ChildLValue<double>("dDirVal", 1.);
	BOOL& bDir = varCookie.ChildLValue<BOOL>("Dir", 0);
	
	//2020.12.27 LJC DoorOpen 시 움직임 속도 100으로 제한
	//2024.4.9 KES 인터락 강화 - 운전모드 Teach면 속도 100으로 제한
	if ((sm.Option->tSequence.eDriveMode == eDRIVE_MODE_TEACH || CheckDoorOpen_SCT_LD()) && dSpeed > 100)
		dSpeed = 100.;
	
	if (iAxis == eMOTOR_END){
		cmd.ReserveErrorStep();
		return FALSE;
	}
	if (!cmd.IsChildOK() || !cmd.IsOK()) {
		ys.MotionStop(iAxis);
		TRACE(__FUNCTION__  _T("-Step : %d\n"));
		sm.MotorStatus[iAxis].tEtc.bIPRun = FALSE;
		sm.MotorStatus[iAxis].tEtc.bMoveStop = TRUE;
		SetMoveTimeout_abnormal(iAxis, 1000);
		return FALSE;
	}
	if (dSpeed < 0) {
		dSpeed = sm.MotorParameter[iAxis].dLowSpeed;
	}
	
	//<< 동작 함수 구동 중 ServoOff가 되어 있으면 에러 알림 후 종료.
	if (FALSE == sm.MotorStatus[iAxis].tCrt.bServoOn) {
		// KES 추가 2024.3.26
		sm.MotorStatus[iAxis].tEtc.iMotorDesc = IDS_DES_AXIS_SERVO_OFF_MSG;
		IPMessage(g_stDescription[IDS_DES_AXIS_SERVO_OFF_MSG]);
		cmd.ReserveErrorStep();
		return TRUE;
	}
	//>>
	
	switch (cmd.GetCurrentStep()) {
	case S_INIT: {
		sm.MotorStatus[iAxis].tEtc.iMotorDesc = IDS_DES_NOT_USE;	// KES 추가 2024.3.26
		sm.MotorStatus[iAxis].tEtc.bIPRun = TRUE;
		sm.MotorStatus[iAxis].tEtc.bCmdMoveStop = FALSE;
		sm.MotorStatus[iAxis].tEtc.bMoveStop = FALSE;
		ys.ClearServoAlarm(iAxis);

		if (GetAxisPos(iAxis) > dTarget) {
			bDir = FALSE;
		}
		else {
			bDir = TRUE;
		}
		cmd.EnqueueNextStep();
		break;
	}
	case S_CHECK_INTER_LOCK: {

		if (GetPTPInterLcok(iAxis, dTarget, bDir)) {
			cmd.ReserveErrorStep();
		}
		else {
			dDistance = GetAxisC2T(iAxis, dTarget);
			if (sm.MotorStatus[iAxis].tCrt.dPos - dTarget > 0) {
				dDirVal = -1.;
			}
			
			
			if (fabs(dDistance) < 0.001) {
				cmd.EnqueueNextStep(S_END);
				break;
			}
			cmd.EnqueueNextStep();
		}
		break;
	}
	case S_MOVE_START: {
	
		//SetMoveTimeout(iAxis, ceil(GetTimeDistance(dDistance, dSpeed)));
		SetMoveTimeout(iAxis, dTarget, dSpeed);

		//2022.08.02 스토퍼 과부하 발생 후 서보On시 목표위치 이동으로 인해 현상 반복됨
		//sm.MotorStatus[iAxis].tEtc.dLastStartPos = GetPosition(iAxis);
		//sm.MotorStatus[iAxis].tEtc.dLastTargetPos = dTarget;
		//sm.MotorStatus[iAxis].tEtc.dLastTargetSpeed = dSpeed;
		if (!sm.IsSimultation()) {
			
			ys.PTPMove(iAxis, dTarget, dSpeed);
		}
		else {
			sm.MotorStatus[iAxis].tCrt.dSpeed = dSpeed;
			sm.MotorStatus[iAxis].tCrt.bInPosition = FALSE;
		}
		cmd.EnqueueNextStep();
		break;
	}
	case S_CHECK_TIME_OUT: {
		sm.MotorStatus[iAxis].tEtc.ullEndTick = GetTickCount64();
		double dTime = sm.MotorStatus[iAxis].tEtc.ullEndTick - sm.MotorStatus[iAxis].tEtc.ullStartTick;
		if (sm.MotorStatus[iAxis].tEtc.bCmdMoveStop) {
			// KES 추가 2024.3.26
			sm.MotorStatus[iAxis].tEtc.iMotorDesc = IDS_DES_AXIS_CMD_MOVE_STOP_FAIL_MSG;
			IPMessage(g_stDescription[IDS_DES_AXIS_CMD_MOVE_STOP_FAIL_MSG]);
			cmd.ReserveErrorStep();
			break;
		}
		if (sm.IsSimultation()) {
			if (dTime > sm.MotorStatus[iAxis].tEtc.ullEstimated_time) {
				sm.MotorStatus[iAxis].tEtc.ullTactTime = sm.MotorStatus[iAxis].tEtc.ullEndTick - sm.MotorStatus[iAxis].tEtc.ullStartTick;
				sm.MotorStatus[iAxis].tCrt.dPos = dTarget;
				sm.MotorStatus[iAxis].tCrt.dSpeed = 0.;
				sm.MotorStatus[iAxis].tCrt.bInPosition = TRUE;
				cmd.EnqueueNextStep();
			}
			else if (GetPTPInterLcok(iAxis, dTarget, bDir)) {
				cmd.ReserveErrorStep();
			}
			else {
				 sm.MotorStatus[iAxis].tCrt.dPos = sm.MotorStatus[iAxis].tEtc.dLastStartPos + ((dTime/1000.) * dSpeed * dDirVal);
				 cmd.EnqueueNextStepCurrent();
			}
		}
		else {
			if (GetInPosition(iAxis, dTarget)) {
				sm.MotorStatus[iAxis].tEtc.ullTactTime = sm.MotorStatus[iAxis].tEtc.ullEndTick - sm.MotorStatus[iAxis].tEtc.ullStartTick;
				cmd.EnqueueNextStep();
			}
			else if (GetPTPInterLcok(iAxis, dTarget, bDir)) {
				cmd.ReserveErrorStep();
			}
			else {
				ULONGLONG ullOverride = sm.MotorStatus[iAxis].tEtc.ullEstimated_time * (10000 / sm.Register->Reg_Word_1.dwOverdrive);
				if(sm.MotorStatus[iAxis].tEtc.ullEstimated_time < ullOverride){
					sm.MotorStatus[iAxis].tEtc.ullEstimated_time = ullOverride;
				}

				//if (GetTimeout(iAxis)) {
				//	cmd.ReserveErrorStep();
				//}
				if (GetMoveTimeout(iAxis)) {
					// KES 추가 2024.3.26
					sm.MotorStatus[iAxis].tEtc.iMotorDesc = IDS_DES_AXIS_MOVE_TIMEOUT_MSG;
					IPMessage(g_stDescription[IDS_DES_AXIS_MOVE_TIMEOUT_MSG]);
					cmd.ReserveErrorStep();
				}
				else {
					cmd.EnqueueNextStepCurrent();
				}
			}
			
		}
		
		break;
	}
	case S_END: {
		//2022.08.02 스토퍼 과부하 발생 후 서보On시 목표위치 이동으로 인해 현상 반복됨
		sm.MotorStatus[iAxis].tEtc.dLastStartPos = GetPosition(iAxis);
		sm.MotorStatus[iAxis].tEtc.dLastTargetPos = dTarget;
		sm.MotorStatus[iAxis].tEtc.dLastTargetSpeed = dSpeed;

		sm.MotorStatus[iAxis].tEtc.bIPRun = FALSE;
		sm.MotorStatus[iAxis].tEtc.bMoveStop = TRUE;
		break;
	}
	}
	return TRUE;
}
BOOL CMainFrame::OnIPMoveLiner(CIPCommand& cmd){
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;
	enum{
		S_INIT,
		S_CHECK_INTER_LOCK,
		S_MOVE_START,
		S_CHECK_TIME_OUT,
		S_END,
	};
	int iCount = var.GetChildItem("Count", 0);
	double dSpeed = var.GetChildItem("Speed", -1);
	

	std::vector<T_POS> Data;
	if( !cmd.IsChildOK() || !cmd.IsOK())
	{
		//TODO 이거 GetAxsLineriData(var, Data); 써야하지 않나?
		GetAxsMultiData(var, Data);
		if (!sm.IsSimultation()) {
			for(int i = 0 ; i < iCount ;i++){
				ys.MotionStop(Data[i].iAxis);;
				sm.MotorStatus[Data[i].iAxis].tEtc.bIPRun = FALSE;
				sm.MotorStatus[Data[i].iAxis].tEtc.bCmdMoveStop = FALSE;
				sm.MotorStatus[Data[i].iAxis].tEtc.bMoveStop = TRUE;
			}
		}

		TRACE(__FUNCTION__  _T("-Step : %d"));
		return FALSE;
	}

	//<< 동작 함수 구동 중 ServoOff가 되어 있으면 에러 알림 후 종료.
	//. 모든 스텝마다 필요.
	GetAxsLineriData(var, Data);
	for (int i = 0; i < iCount; ++i) {

		if (FALSE == sm.MotorStatus[Data[i].iAxis].tCrt.bServoOn) {
			cmd.ReserveErrorStep();
			return TRUE;
		}
	}
	//>>

	if( dSpeed < 0.001){
		cmd.ReserveErrorStep();
		return TRUE;
	}
	if( iCount < 1){
		cmd.ReserveErrorStep();
		return TRUE;
	}
	BOOL bCheck = TRUE;
	//2020.12.27 LJC DoorOpen 시 움직임 속도 100으로 제한
	//2024.4.9 KES 인터락 강화 - 운전모드 Teach면 속도 100으로 제한
	if ((sm.Option->tSequence.eDriveMode == eDRIVE_MODE_TEACH || CheckDoorOpen_SCT_LD()) && dSpeed > 100)
		dSpeed = 100.;
	
	switch( cmd.GetCurrentStep()){
		case S_INIT:{
			for(int i = 0 ; i < iCount ;i++){
				const auto& data = Data[i];
				sm.MotorStatus[data.iAxis].tEtc.iMotorDesc = IDS_DES_NOT_USE;	// KES 추가 2024.3.26
				sm.MotorStatus[data.iAxis].tEtc.bIPRun = TRUE;
				sm.MotorStatus[data.iAxis].tEtc.bCmdMoveStop = FALSE;
				sm.MotorStatus[data.iAxis].tEtc.bMoveStop = FALSE;

				sm.MotorStatus[data.iAxis].tEtc.dLastStartPos = GetPosition(data.iAxis);
				sm.MotorStatus[data.iAxis].tEtc.dLastTargetPos = data.dTarget;
				sm.MotorStatus[data.iAxis].tEtc.dLastTargetSpeed = data.dSpeed;
			}
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK_INTER_LOCK:{
			
			for(int i = 0 ; i < iCount ;i++){
				
				BOOL bDir = GetAxisPos(Data[i].iAxis) > Data[i].dTarget ? 0 : 1;
				if (GetPTPInterLcok(Data[i].iAxis, Data[i].dTarget, bDir)) {
					cmd.ReserveErrorStep();
					return TRUE;
				}
				
			}
			
			cmd.EnqueueNextStep();
			break;
		}
		case S_MOVE_START:{
			
			// 1번 축 이동시간 계산
			double dMoveTime = 0.;
			{
				if (iCount > 1) {
					CPoint2d pt = CPoint2d(GetAxisC2T(Data[0]), GetAxisC2T(Data[1]));
					double dDistance = pt.Distance(CPoint2d());
					dMoveTime = dDistance / dSpeed;
				}
				else {
					double dDistance = GetAxisC2T(Data[0]);
					dMoveTime = dDistance / dSpeed;
				}
			}
			ULONG64 ullMaxTime = 0;			
			// 도착 스피드 세팅 (1번 축 이동시간 기준 스피드 세팅)
			for (int idx = 0; idx < Data.size(); ++idx)
			{
				Data[idx].dSpeed = GetAxisC2T(Data[idx]) / dMoveTime;				
				
				if( idx == 0){
					double dDistance = GetAxisC2T(Data[idx].iAxis, Data[idx].dTarget);
					if( ullMaxTime < GetTimeDistance(dDistance, Data[idx].dSpeed)){
						ullMaxTime = GetTimeDistance(dDistance, Data[idx].dSpeed);
					}
				}
				SetMoveTimeout(Data[idx].iAxis, ullMaxTime);
			}
			
			
			if (sm.IsSimultation()) {
				for (int i = 0; i < Data.size(); i++) {
					
					PTPMove(Data[i], &cmd);
				}	

			}
			else {
				ys.MultiMove(Data);
			}
			cmd.EnqueueNextStep();
			break;
		}
		
		case S_CHECK_TIME_OUT:{
			if( GetInPosition(Data)){
				cmd.EnqueueNextStep();
			}else{
				for(int i = 0 ; i < iCount ;i++){
					if (sm.MotorStatus[Data[i].iAxis].tEtc.bCmdMoveStop) {
						cmd.ReserveErrorStep();
						break;
					}
					if( !GetInPosition(Data[i])){
						ULONGLONG ullOverride = sm.MotorStatus[Data[i].iAxis].tEtc.ullEstimated_time * (10000 / sm.Register->Reg_Word_1.dwOverdrive);
						if(sm.MotorStatus[Data[i].iAxis].tEtc.ullEstimated_time < ullOverride){
							sm.MotorStatus[Data[i].iAxis].tEtc.ullEstimated_time = ullOverride;
						}
						if( GetTimeout(Data[i].iAxis) ){
							cmd.ReserveErrorStep();
							return TRUE;
						}
						BOOL bDir = GetAxisPos(Data[i].iAxis) > Data[i].dTarget ? 0 : 1;
						if (GetPTPInterLcok(Data[i].iAxis, Data[i].dTarget, bDir)) {
							cmd.ReserveErrorStep();
							return TRUE;
						}
					}
					
				}
				cmd.EnqueueNextStepCurrent();
			}
			break;
		}
		case S_END:{
			for(int i = 0 ; i < iCount ;i++){
				sm.MotorStatus[Data[i].iAxis].tEtc.bIPRun = FALSE;
				sm.MotorStatus[Data[i].iAxis].tEtc.bCmdMoveStop = FALSE;
				sm.MotorStatus[Data[i].iAxis].tEtc.bMoveStop = TRUE;
			}
			cmd.EnqueueNextStep();
			break;
		}
	}

	return TRUE;
}
BOOL CMainFrame::OnIPMoveMulti(CIPCommand& cmd){
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;
	enum{
		S_INIT,
		S_CHECK_INTER_LOCK,
		S_MOVE_START,
		S_CHECK_TIME_OUT,
		S_END,
	};
	int iCount = var.GetChildItem("Count", 0);
	

	std::vector<T_POS> Data;
	if( !cmd.IsChildOK() || !cmd.IsOK()){
		
		GetAxsMultiData(var, Data);
		for(int i = 0 ; i < iCount ;i++){
			ys.MotionStop(Data[i].iAxis);
			sm.MotorStatus[Data[i].iAxis].tEtc.bIPRun = FALSE;
			sm.MotorStatus[Data[i].iAxis].tEtc.bCmdMoveStop = FALSE;
			sm.MotorStatus[Data[i].iAxis].tEtc.bMoveStop = TRUE;
			
		}

		TRACE(__FUNCTION__  _T("-Step : %d"));
		return FALSE;
	}
	if( iCount < 1){
		cmd.ReserveErrorStep();
		return TRUE;
	}


	//<< 동작 함수 구동 중 ServoOff가 되어 있으면 에러 알림 후 종료.
	//. 모든 스텝마다 필요.
	GetAxsMultiData(var, Data);
	for (int i = 0; i < iCount; ++i) {

		if (FALSE == sm.MotorStatus[Data[i].iAxis].tCrt.bServoOn) {
			cmd.ReserveErrorStep();
			return TRUE;
		}
	}
	//>>

	double &dTime = varCookie.ChildLValue<double>("Time",0);
	BOOL bCheck = TRUE;

	switch( cmd.GetCurrentStep()){
		case S_INIT:{
			for(int i = 0 ; i < iCount ;i++){
				const auto& data = Data[i];
				sm.MotorStatus[data.iAxis].tEtc.iMotorDesc = IDS_DES_NOT_USE;	// KES 추가 2024.3.26
				sm.MotorStatus[data.iAxis].tEtc.bIPRun = TRUE;
				sm.MotorStatus[data.iAxis].tEtc.bCmdMoveStop = FALSE;
				sm.MotorStatus[data.iAxis].tEtc.bMoveStop = FALSE;

				sm.MotorStatus[data.iAxis].tEtc.dLastStartPos = GetPosition(data.iAxis);
				sm.MotorStatus[data.iAxis].tEtc.dLastTargetPos = data.dTarget;
				sm.MotorStatus[data.iAxis].tEtc.dLastTargetSpeed = data.dSpeed;
			}
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK_INTER_LOCK:{
			//. 12.12 수정. ServoOff 상태인지 검사하면서 Vector Data는 채워져있음.
			//GetAxsMultiData(var, Data);
			double dDistance = 0; 
			ULONG64 ullMaxTime = 0;
			for(int i = 0 ; i < iCount ;i++){
				BOOL bDir = GetAxisPos(Data[i].iAxis) > Data[i].dTarget ? 0 : 1;
				if (GetPTPInterLcok(Data[i].iAxis, Data[i].dTarget, bDir)) {
					cmd.ReserveErrorStep();
					return TRUE;
				}
				dDistance = fabs(sm.MotorStatus[Data[i].iAxis].tCrt.dPos - Data[i].dTarget);
				if (ullMaxTime < GetTimeDistance(dDistance, Data[i].dSpeed)) {
					ullMaxTime = GetTimeDistance(dDistance, Data[i].dSpeed);
				}
			}
			for (int i = 0; i < iCount; i++) {
				SetMoveTimeout(Data[i].iAxis, ullMaxTime);
			}
			cmd.EnqueueNextStep();
			break;
		}
		case S_MOVE_START:{
			//. 12.12 수정. ServoOff 상태인지 검사하면서 Vector Data는 채워져있음.
			//GetAxsMultiData(var, Data);
			if (sm.IsSimultation()) {
				for (int i = 0; i < Data.size(); i++) {
					T_POS tPos;
					tPos.iAxis	 = Data[i].iAxis;
					tPos.dTarget = Data[i].dTarget;
					tPos.dSpeed	 = Data[i].dSpeed;
					PTPMove(tPos, &cmd);
				}

			}
			else {
				ys.MultiMove(Data);
			}
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK_TIME_OUT:{
			//. 12.12 수정. ServoOff 상태인지 검사하면서 Vector Data는 채워져있음.
			//GetAxsMultiData(var, Data);
			BOOL bTimeout = FALSE;
			if( GetInPosition(Data)){
				cmd.EnqueueNextStep();
			}else{
				for(int i = 0 ; i < iCount ;i++){
					if (sm.MotorStatus[Data[i].iAxis].tEtc.bCmdMoveStop) {
						cmd.ReserveErrorStep();
						break;
					}
					if( !GetInPosition(Data[i])){
						ULONGLONG ullOverride = sm.MotorStatus[Data[i].iAxis].tEtc.ullEstimated_time * (10000 / sm.Register->Reg_Word_1.dwOverdrive);
						if(sm.MotorStatus[Data[i].iAxis].tEtc.ullEstimated_time < ullOverride){
							sm.MotorStatus[Data[i].iAxis].tEtc.ullEstimated_time = ullOverride;
						}
						if( GetTimeout(Data[i].iAxis) ){
							bTimeout = TRUE;
							break;
						}
						BOOL bDir = GetAxisPos(Data[i].iAxis) > Data[i].dTarget ? 0 : 1;
						if (GetPTPInterLcok(Data[i].iAxis, Data[i].dTarget, bDir)) {
							cmd.ReserveErrorStep();
							return TRUE;
						}
					}
				}
				if (bTimeout) {
					cmd.ReserveErrorStep();
					return TRUE;
				}
				cmd.EnqueueNextStepCurrent();
			}
			
			break;
		}
		
		case S_END:{
			for(int i = 0 ; i < iCount ;i++){
				sm.MotorStatus[Data[i].iAxis].tEtc.bIPRun = FALSE;
				sm.MotorStatus[Data[i].iAxis].tEtc.bCmdMoveStop = FALSE;
				sm.MotorStatus[Data[i].iAxis].tEtc.bMoveStop = TRUE;
			}
			cmd.EnqueueNextStep();
			break;
		}
	}
	return TRUE;
}

BOOL CMainFrame::OnIPTorqueMove(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;
	CIPVar& varResult = cmd.m_varResult;

	int iAxis = var.GetChildItem("Axis", -1);
	double dTorque = var.GetChildItem("Torque", -1.);
	double dSpeed = var.GetChildItem("Speed", -1.);
	BOOL bDir = var.GetChildItem("Dir", FALSE);

	// 데이터 세팅이 덜된 경우
	if (iAxis < 0 || dTorque < 0. || dSpeed < 0.)
	{
		return FALSE;
	}

	if (!cmd.IsChildOK() || !cmd.IsOK())
	{
		ys.MotionStop(iAxis);
		return FALSE;
	}

	if (bDir == FALSE) dSpeed *= -1.;

	ys.TorqueMove(iAxis, dTorque, dSpeed);

	return TRUE;
}

BOOL CMainFrame::OnIPAverageTorqueMove(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;
	CIPVar& varResult = cmd.m_varResult;

enum 
{
	S_INIT,
	S_TORQUE_DEFAULT_VALUE,
	S_TORQUE_MOVE,
	S_CHECK_TORQUE,
	S_START_TIME_CHECK,
	S_CHECK_TIME,
};
	
	int iAxis = var.GetChildItem("Axis", -1);
	double dTorque = var.GetChildItem("Torque", -1.);
	double dSpeed = var.GetChildItem("Speed", -1.);
	double dCheckTime = var.GetChildItem("Time", -1.);
	double dWaitTime = var.GetChildItem("WaitTime", -1.);
	CString strAddress = var.GetChildItem("Address", _T(""));
	BOOL bDir = var.GetChildItem("Dir", TRUE);

	// 데이터 세팅이 덜된 경우
	if (iAxis < 0 || dTorque < 0.1 || dSpeed < 0.1 || dCheckTime <= 1. || dWaitTime <= 1. || strAddress.IsEmpty())
	{
		IPMessage(g_st[IDS_MAIN_FRAME_DATA_SETTING_NOT_YET]);
		return FALSE;
	}

	if (bDir == FALSE) dSpeed *= -1.;

	if (!cmd.IsChildOK() || !cmd.IsOK())
	{
		ys.MotionStop(iAxis);
		return FALSE;
	}

	ULONGLONG& ullStartTime = varCookie.ChildLValue<ULONGLONG>("StartTime", 0);
	int& iCount = varCookie.ChildLValue<int>("iCount", 0);
	int& iDefaultValue = varCookie.ChildLValue<int>("DefaultValue", 0);
	int& iValue = varCookie.ChildLValue<int>("Value", 0);

	switch (cmd.GetCurrentStep()) 
	{
		case S_INIT:
		{
			ullStartTime = GetTickCount64();
		}
		cmd.EnqueueNextStep();
		break;

		case S_TORQUE_DEFAULT_VALUE:
		{
			if ((GetTickCount64() - ullStartTime) * 0.001 > dWaitTime)
			{
				iDefaultValue /= iCount;
				iCount = 0;
				cmd.EnqueueNextStep();
			}
			else
			{
				iCount++;
				sm.MotorStatus[iAxis].tEtc.iLoadCell = ys.GetRegDataInt(strAddress);
				iDefaultValue += sm.MotorStatus[iAxis].tEtc.iLoadCell; // TODO Register
				cmd.EnqueueNextStepCurrent(20);
			}
		}

		break;

		case S_TORQUE_MOVE: 
		{
			ullStartTime = GetTickCount64();
			ys.TorqueMove(iAxis, dTorque, dSpeed);
		}
		cmd.EnqueueNextStep();
		break;

		case S_CHECK_TORQUE:
		{
			if (fabs(sm.MotorStatus[iAxis].tCrt.dTorque - dTorque) < 0.01)
			{
				cmd.EnqueueNextStep(S_START_TIME_CHECK, ceil(dWaitTime * 1000));
			}
			else
			{
				if ((GetTickCount64() - ullStartTime) > 10 * 1000)
				{
					return FALSE;
				}
				cmd.EnqueueNextStepCurrent(30);
			}
		}
		break;

		case S_START_TIME_CHECK:
		{
			ullStartTime = GetTickCount64();
		}
		cmd.EnqueueNextStep();
		break;

		case S_CHECK_TIME: 
		{
			double durationTime = (GetTickCount64() - ullStartTime) * 0.001;

			if (dCheckTime < durationTime)
			{
				ys.MotionStop(iAxis);
				iValue = (iValue / iCount) - iDefaultValue;
				varResult.SetChildItem("Value", iValue);
			}
			else
			{
				iCount++;
				sm.MotorStatus[iAxis].tEtc.iLoadCell = ys.GetRegDataInt(strAddress);
				iValue += sm.MotorStatus[iAxis].tEtc.iLoadCell; // TODO Register
				cmd.EnqueueNextStepCurrent(20);
			}
		}
		break;
	}

	return TRUE;
}
BOOL	CMainFrame::OnIPYaskawaInit(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	enum {
		S_INIT,
		S_END,
	};

	switch (cmd.GetCurrentStep()) {
	case S_INIT: {
		ys.InitAxis();
		cmd.EnqueueNextStep();
		break;
	}
	case S_END: {
		cmd.EnqueueNextStep();
		break;
	}
	}
	return TRUE;
}


BOOL CMainFrame::OnIPSetMotorLimit(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	for (int i = 0; i < eMOTOR_END; i++) {
		ys.SetLimit(i, sm.MotorParameter[i].dLimitLow, sm.MotorParameter[i].dLimitHigh);
	}

	return TRUE;
}

BOOL CMainFrame::OnIPVisionMove(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	int iAxisX = var.GetChildItem("AxisX", 0);
	int iAxisY = var.GetChildItem("AxisY", 0);

	double dSpeedX = var.GetChildItem("SpeedX", sm.MotorParameter[iAxisX].dPTPSpeed);
	double dSpeedY = var.GetChildItem("SpeedY", sm.MotorParameter[iAxisY].dPTPSpeed);

	double dTargetX = var.GetChildItem("TargetX", GetPos(iAxisX));
	double dTargetY = var.GetChildItem("TargetY", GetPos(iAxisY));
	
	enum {
		S_INIT,
		S_MOVE,
		S_END,
	};

	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		PTPStop(eAxis(iAxisX));
		PTPStop(eAxis(iAxisY));
		return FALSE;
	}

	if (sm.Sequence->fAutoMode && !sm.Sequence->fCycleStop) {
		IPMessage(g_st[IDS_MAIN_FRAME_CAN_NOT_MOVE_IN_AUTO_MODE]);
		return FALSE;
	}

	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			cmd.EnqueueNextStep();
			break;
		}
		case S_MOVE:{
			PTPMove((eAxis)iAxisX, dTargetX, dSpeedX, &cmd);
			PTPMove((eAxis)iAxisY, dTargetY, dSpeedY, &cmd);
			cmd.EnqueueNextStep();
			break;
		}
		case S_END:{
			break;
		}
	
	}
	return TRUE;
}



// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "Sequence.h"
#include "ChildView.h"
#include "LoaderSeq.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);
	//sm.SetSimultation(TRUE);

	//<< MCC
	sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI01].A[eMCC_ACTION_LD_LI_01_MANUAL_STOP] = 1;
	sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI02].A[eMCC_ACTION_LD_LI_02_MANUAL_STOP] = 1;
	sm.MCC_EQP->Layer[eMCC_LAYER_LD_LE01].A[eMCC_ACTION_LD_LE_01_MANUAL_STOP] = 1;
	sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO01].A[eMCC_ACTION_LD_LO_01_MANUAL_STOP] = 1;
	sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO02].A[eMCC_ACTION_LD_LO_02_MANUAL_STOP] = 1;
	sm.MCC_EQP->Layer[eMCC_LAYER_LD_TR01].A[eMCC_ACTION_LD_TR_01_MANUAL_STOP] = 1;
	//>>

	sm.Sequence->fAutoMode = FALSE;
	// UI 에 현재 적용된 Recipe 정보를 받아올 수 있는 IP 함수 만들어야함 
	// UI 로 보내면 UI 에서는 Send Recipe 호출 에서 진행하면 될듯
	g_ipClient.SendIPCommand("UI", "Recv", "Recipe");
	m_ThreadMain = std::thread([this]() {MainT(); });
	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}
void CChildView::MainT() {
	// MC Parameter 들은 전부 Device Parameter 로 이동됨 
	//sm.MCParmeter->tTime.dTime				= 3	 * 1000;
	//sm.MCParmeter->tTime.dConveyorH			= 10 * 1000;
	//sm.MCParmeter->tTime.dConveyorL			= 10 * 1000;
	//sm.MCParmeter->tTime.dConveyorStoper	= 3	 * 1000;
	//sm.MCParmeter->tTime.dCurtain			= 3	 * 1000;
	//sm.MCParmeter->tTime.dCleaner			= 3	 * 1000;
	//sm.MCParmeter->tTime.dDefaultCylinder	= 3	 * 1000;
	//sm.MCParmeter->tTime.dVacuum			= 3	 * 1000;
	//sm.MCParmeter->tTime.dBlow				= 3	 * 1000;
	// ????
	//for (int i = 0; i < eMOTOR_END; i++) {
	//	sm.MotorParameter[i].dInPosition == 0.002;
	//}
	while (TRUE)
	{
		//<< 2024.04.29 LBH - OP Lamp
		{
			// Mute On Lamp
			sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW = GetMute(TRUE);
			sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW = GetMute(TRUE);

			// AGV 사용/미사용 Mode Lamp
			sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_RED_LAMP = !sm.Option->tSequence.bUseAGV;
			sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_RED_LAMP = !sm.Option->tSequence.bUseAGV;
		}
		//>>
		
		//-->24.07.29 KGH EDIT : AGV PIO 중알람에서는 안되게 수정
		//<< KES 추가 2024.07.13 - AGV PIO중이면 오토런 상관없이 컨베이어 PIO 진행.
		WORD& seqStepLoad = sm.Module[eMODULE_PC1_LOAD_CONVEYOR_1].Status.wSeqStep;
		if (g_pRecipe != nullptr && sm.Sequence->fAutoMode == TRUE && sm.Sequence->fCycleStop == FALSE)
		{
			LoadConveyor1();
		}
		else if (
				(sm.Sequence->fIsAlarm==FALSE)&& !GetConveyorAlarm(eMODULE_PC1_LOAD_CONVEYOR_1)&&
				((seqStepLoad >= S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_CONNECTION_ON)	&& (seqStepLoad <= S_LDCV1_SEQ_AUTO_PPBOX_LOAD_END))
				|| (seqStepLoad == S_LDCV1_SEQ_LOAD_S6F11_CEID_351_CASSETTE_LOAD_COMPLETE)
				|| (seqStepLoad == S_LDCV1_SEQ_LOAD_S6F11_CEID_351_CASSETTE_LOAD_COMPLETE_CHECK)
				|| (seqStepLoad == S_LDCV1_SEQ_PPBOX_LOAD_END)
			)
		{
			if (g_pRecipe != nullptr) {
				LoadConveyor1();
			}
		}
		else if ((sm.Sequence->fIsAlarm == TRUE) && (sm.Sequence->fAutoMode == FALSE || sm.Sequence->fCycleStop == TRUE)) 
		{
			if (Serial::Inverter::GetInverterRun(eMODULE_PC1_LOAD_CONVEYOR_1)) {
				Serial::Inverter::SetInverterStop(eMODULE_PC1_LOAD_CONVEYOR_1);
			}
		}

		//if ((sm.Sequence->fAutoMode == TRUE ||
		//	((seqStepLoad >= S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_CONNECTION_ON && seqStepLoad <= S_LDCV1_SEQ_AUTO_PPBOX_LOAD_END)
		//		//((seqStepLoad >= S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START && seqStepLoad <= S_LDCV1_SEQ_AUTO_PPBOX_LOAD_END)
		//		|| seqStepLoad == S_LDCV1_SEQ_LOAD_S6F11_CEID_351_CASSETTE_LOAD_COMPLETE
		//		|| seqStepLoad == S_LDCV1_SEQ_LOAD_S6F11_CEID_351_CASSETTE_LOAD_COMPLETE_CHECK
		//		|| seqStepLoad == S_LDCV1_SEQ_PPBOX_LOAD_END)) && !GetConveyorAlarm(eMODULE_PC1_LOAD_CONVEYOR_1)) {
		//	if (g_pRecipe != nullptr) {
		//		//LoadConveyor1();
		//	}
		//}
		//else if (sm.Sequence->fAutoMode == FALSE || sm.Sequence->fCycleStop == TRUE) {
		//	if (Serial::Inverter::GetInverterRun(eMODULE_PC1_LOAD_CONVEYOR_1)) {
		//		//Serial::Inverter::SetInverterStop(eMODULE_PC1_LOAD_CONVEYOR_1);
		//	}
		//}
		//<--24.07.29 KGH EDIT : AGV PIO 중알람에서는 안되게 수정


		//-->24.07.29 KGH EDIT : AGV PIO 중알람에서는 안되게 수정
		WORD& seqStepUnload = sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep;
		if (g_pRecipe != nullptr && sm.Sequence->fAutoMode == TRUE && sm.Sequence->fCycleStop == FALSE)
		{
			UnloadConveyor2();
		}
		else if (
			(sm.Sequence->fIsAlarm == FALSE) && !GetConveyorAlarm(eMODULE_PC1_UNLOAD_CONVEYOR_2) &&
			((seqStepUnload >= S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_CONNECT_ON) && (seqStepUnload <= S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_END))
			|| (seqStepUnload == S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID353_CASSETTE_UNLOAD_COMPLETE)
			|| (seqStepUnload == S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID353_CASSETTE_UNLOAD_COMPLETE_CHECK)
			|| (seqStepUnload == S_ULDCV2_SEQ_PPBOX_UNLOAD_END)
			)
		{
			if (g_pRecipe != nullptr) {
				UnloadConveyor2();
			}
		}
		else if ((sm.Sequence->fIsAlarm == TRUE) && (sm.Sequence->fAutoMode == FALSE || sm.Sequence->fCycleStop == TRUE)) {
			
			if (Serial::Inverter::GetInverterRun(eMODULE_PC1_UNLOAD_CONVEYOR_2)) {
				Serial::Inverter::SetInverterStop(eMODULE_PC1_UNLOAD_CONVEYOR_2);
			}
			if (Serial::Inverter::GetInverterRun(eMODULE_PC1_LOAD_CONVEYOR_2)) {
				Serial::Inverter::SetInverterStop(eMODULE_PC1_LOAD_CONVEYOR_2);
			}
			if (Serial::Inverter::GetInverterRun(eMODULE_PC1_ELEVATOR_CONVEYOR)) {
				Serial::Inverter::SetInverterStop(eMODULE_PC1_ELEVATOR_CONVEYOR);
			}
			if (Serial::Inverter::GetInverterRun(eMODULE_PC1_UNLOAD_CONVEYOR_1)) {
				Serial::Inverter::SetInverterStop(eMODULE_PC1_UNLOAD_CONVEYOR_1);
			}
		}
		//>>

		//if ((sm.Sequence->fAutoMode == TRUE || ((seqStepUnload >= S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_CONNECT_ON && seqStepUnload <= S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_END)
		//	//if ((sm.Sequence->fAutoMode == TRUE || ((seqStepUnload >= S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START && seqStepUnload <= S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_END)
		//	|| seqStepUnload == S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID353_CASSETTE_UNLOAD_COMPLETE
		//	|| seqStepUnload == S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID353_CASSETTE_UNLOAD_COMPLETE_CHECK
		//	|| seqStepUnload == S_ULDCV2_SEQ_PPBOX_UNLOAD_END)) && !GetConveyorAlarm(eMODULE_PC1_UNLOAD_CONVEYOR_2)) {
		//	if (g_pRecipe != nullptr) {
		//		//UnloadConveyor2();
		//	}
		//}
		//else if (sm.Sequence->fAutoMode == FALSE || sm.Sequence->fCycleStop == TRUE) {
		//	if (Serial::Inverter::GetInverterRun(eMODULE_PC1_UNLOAD_CONVEYOR_2)) {
		//		//Serial::Inverter::SetInverterStop(eMODULE_PC1_UNLOAD_CONVEYOR_2);
		//	}
		//}
		////>>
		//else if (sm.Sequence->fAutoMode == FALSE || sm.Sequence->fCycleStop == TRUE) {
		//	if (Serial::Inverter::GetInverterRun(eMODULE_PC1_LOAD_CONVEYOR_2)) {
		//		Serial::Inverter::SetInverterStop(eMODULE_PC1_LOAD_CONVEYOR_2);
		//	}
		//	if (Serial::Inverter::GetInverterRun(eMODULE_PC1_ELEVATOR_CONVEYOR)) {
		//		Serial::Inverter::SetInverterStop(eMODULE_PC1_ELEVATOR_CONVEYOR);
		//	}
		//	if (Serial::Inverter::GetInverterRun(eMODULE_PC1_UNLOAD_CONVEYOR_1)) {
		//		Serial::Inverter::SetInverterStop(eMODULE_PC1_UNLOAD_CONVEYOR_1);
		//	}
		//}
		//<--24.07.29 KGH EDIT : AGV PIO 중알람에서는 안되게 수정

		if (sm.Sequence->fAutoMode == FALSE)
		{
			sm.IO->tOut_NEW.fLD_DUST_CHATHER1_REMOTE_ON_OFF = FALSE;
			continue;
		}
		if (sm.Sequence->fCycleStop == TRUE)
			continue;
		
		//<< 2024.06.22 kms
		CStringA mcc_recipe(sm.MCC_EQP->szPPID);
		CStringA eqp_recipe(g_pRecipe->recipeName);
		if (mcc_recipe != eqp_recipe) {
			strncpy_s(sm.MCC_EQP->szPPID, countof(sm.MCC_EQP->szPPID), eqp_recipe, countof(sm.MCC_EQP->szPPID) - 1);
		}
		//>>

		// Ionizer 제어 
		sm.IO->tOut_NEW.fLD_IONIZER_POWER_ON_CR = TRUE;
		sm.IO->tOut_NEW.fLD_ION1_STOP = FALSE;
		// 2024.05.31 MDIK KKW ADD 이오나이저 킬 때 블로우 SOL 같이 켜야함
		sm.IO->tOut_NEW.fLD_IONIZER_BLOW_ON_OFF_SOL = TRUE;
		// 2024.06.20 MDIK KKW Dust Add
		//24.08.04 KWR 집진기 작업 관련 임시 주석, 해제 필요
		//sm.IO->tOut_CRT.fLD_DUST_CHATHER1_INV1_SPEED3 == FALSE;
		//sm.IO->tOut_CRT.fLD_DUST_CHATHER1_INV1_SPEED2 == FALSE;
		//sm.IO->tOut_NEW.fLD_DUST_CHATHER1_INV1_SPEED1 = TRUE;
		//if (sm.IO->tIn_CRT.fLD_DUST_CHATHER1_INVERTER_ON_SIGNAL == FALSE) {
		//	sm.IO->tOut_NEW.fLD_DUST_CHATHER1_REMOTE_ON_OFF = TRUE;
		//}

		// Conveyor 
		{
			// Glass 
			//Conveyor_Nomal(eMODULE_LOAD_GLASS_2_CONVEYOR);			// 일반적인 컨베어 동작 
			// Dry Run 이면서 Linker Disconnect 상태일 때 (SCT 시퀀스 넣어줘야함)
			if (g_pRecipe != nullptr && sm.Sequence->fAutoMode == TRUE && sm.Sequence->fCycleStop == FALSE)
				LoadConveyor2();
			if (g_pRecipe != nullptr && sm.Sequence->fAutoMode == TRUE && sm.Sequence->fCycleStop == FALSE)
				ElevatorConveyor();
			if (g_pRecipe != nullptr && sm.Sequence->fAutoMode == TRUE && sm.Sequence->fCycleStop == FALSE)
				UnloadConveyor1();
			if (g_pRecipe != nullptr && sm.Sequence->fAutoMode == TRUE && sm.Sequence->fCycleStop == FALSE)
				GlassTransfer();
		}
		std::this_thread::sleep_for(1ms);
			
	}
}


#include "pch.h"
#include "LoaderSeq.h"
#include "Sequence.h"

#define PIO_TIMEOUT	30 * 1000
#define PIO_REQUEST_WAIT_TIME 10 * 60 * 1000
#define MANUAL_BUTTON_PUSH_TIME 1000

#define IO_RETRY 3
#define MOVE_RETRY 3

// 2023.08.03 러프한 체크 함수 추가
bool IsTargetPosition(const T_POS& _pos, const double _inPos = 0.1)  //Hàm ktra 1 phần tử _pos
{
	if (sm.MotorStatus[_pos.iAxis].tEtc.bIPRun)  //Servo Run => Return false
	{
		return false;
	}

	return (fabs(GetPos(_pos.iAxis) - _pos.dTarget) <= _inPos); // |Now Pos - Target Pos| <= 0.1 ??
}

bool IsTargetPosition(const std::vector<T_POS>& _pos, const double _inPos = 0.1)  //Hàm ktra nhiều ptu trong 1 vector
{
	for (auto& data : _pos)
	{
		if (!IsTargetPosition(data, _inPos))
		{
			return false;
		}
	}
	return true;
}

//2024.07.12 LBH
//Abort: Huy bo interface EQ vs AGV
void SetPIO_Abort(eMODULEs eModule) {
	//8bit (interface EQP vs AGV)
	SetPIO_EQ_CONNECT(FALSE);
	SetPIO_EQ_REQUEST(FALSE);
	SetPIO_EQ_RUN(FALSE);
	SetPIO_EQ_INPUT(FALSE);
	SetPIO_EQ_OUTPUT(FALSE);
	SetPIO_EQ_ERROR(TRUE);

	Serial::Inverter::SetInverterStop(eModule);
}

void LoadConveyor1(PIPCOMMAND _pCmd)
{
	T_SEQUENCE_MODULE_INFO& pThis = sm.Module[eMODULE_PC1_LOAD_CONVEYOR_1];
	T_SEQUENCE_MODULE_INFO& pLower = sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2];

	WORD& seqStep = pThis.Status.wSeqStep;
	WORD& seqPrevStep = pThis.Status.wSeqPrevStep;


	// log용 설명이 필요하면 description에 넣으면 됨
	// 구분자로 세미콜론(;)을 사용하니 description에는 세미콜론 사용 금지
	CString description = _T(""); 
	auto cleanup = make_scope_exit
	(
		[&]()
		{
			// 스탭이 기존과 바뀌거나 추가 내용이 있으면 로그로 남긴다
			if (seqStep != seqPrevStep || !description.IsEmpty())
			{
				WriteSeqLog(pThis.eModule, seqPrevStep, seqStep, description);
				seqPrevStep = seqStep;
			}
		}
	);

	// 2022.09.15 MoveFail 알람 상세용
	CString StepName(magic_enum::enum_name(static_cast<eLOAD_CONVEYOR_1>(seqStep)).data(), magic_enum::enum_name(static_cast<eLOAD_CONVEYOR_1>(seqStep)).size());

	BOOL bCheck = TRUE;
	WORD& wRetry = pThis.Status.wRetryCount;
	WORD& wRepeat = pThis.Status.wRepeatCount;

	BOOL bUseAGV = sm.Option->tSequence.bUseAGV;
	if (!sm.Sequence->bCIMSkip) {
		bUseAGV = (strcmp(sm.Sequence->CIMData.EQPFunction[eEQP_SCT].Item[eEFID_AGV_ACCESS_MODE].szEFST, lpEFST[eEFST_AUTO]) == 0);
	}

	auto Conveyor = sm.Conveyor_Parameter[eCONVEYOR_LD_1];

	//-->24.05.26 KGH ADD : 재석 조건에 따라서 시퀀스 이동하도록 설정
	if (pThis.Glass.bMaterial == FALSE && (seqStep >=S_LDCV1_SEQ_PPBOX_UNLOAD_START) && seqStep<=S_LDCV1_SEQ_PPBOX_UNLOAD_SEND_COMPLETED)
	{
		seqStep = S_LDCV1_SEQ_PPBOX_CHECK;
	}
	//<--24.05.26 KGH ADD : 재석 조건에 따라서 시퀀스 이동하도록 설정


	//<< 2024.06.15 kms - mcc
	T_MCC_LAYER* MCC_LAYER_LI01 = &sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI01];
	//>>

	//<< 2024.06.24 LBH - Abnormal Stopper up Check
	if (seqStep > S_LDCV1_SEQ_PPBOX_LOAD_STOPPER_UP_CHECK && seqStep < S_LDCV1_SEQ_PPBOX_LOAD_END) {
		if (!GetConveyor_Stoper(pThis.eModule, TRUE, TRUE)) {
			//SetConveyor_Stoper(pThis.eModule, TRUE); // 해당 동작만 하면 무언정지 걸리수 있기 때문에 Alarm 추가.
			SetAlarm(eAI_CONVEYOR_1_STOPER_UP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
			return;
		}
	}
	//>>
	BOOL bConnect	= GetPIO_BF_CONNECT(TRUE);
	BOOL bRequest	= GetPIO_BF_REQUEST(TRUE);
	BOOL bRun		= GetPIO_BF_RUN(TRUE);
	BOOL bError		= GetPIO_BF_ERROR(TRUE);
	BOOL bInput		= GetPIO_BF_INPUT(TRUE);
	BOOL bOutput	= GetPIO_BF_OUTPUT(TRUE);
	BOOL bLoad		= GetPIO_BF_LOAD(TRUE);
	BOOL bUnload	= GetPIO_BF_UNLOAD(TRUE);
	MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_X_CONNECTION] = bConnect;
	MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_X_REQUEST] = bRequest;
	MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_X_RUN] = bRun;
	MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_X_ERROR] = bError;
	MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_X_POSITION_INPUT] = bInput;
	MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_X_POSITION_OUTPUT] = bOutput;
	MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_X_LOAD_REQUEST] = bLoad;
	MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_X_UNLOAD_REQUEST] = bUnload;

	BOOL bConveyorAlarm = GetConveyorAlarm(pThis.eModule);

	switch (seqStep) {
		case S_LDCV1_SEQ_INIT: {
			INFO_INIT_MCC(MCC_LAYER_LI01);

			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.runningStartTime = 0;
			sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_GREEN_LAMP = FALSE;
			seqStep = S_LDCV1_SEQ_PPBOX_CHECK;
			break;
		}
		case S_LDCV1_SEQ_PPBOX_CHECK: {
			wRetry = 0;
			wRepeat = 0;

			// 자재 정보와, 센서가 다른 경우 Alarm 처리 해야 할듯.
			// Dry Run Mode 일 경우 자재 정보로 센서 대체.
			bCheck = (sm.DeviceParameter->tUse.bDryRun) ? pThis.Glass.bMaterial : GetPPBoxSensor(pThis.eModule, TRUE);
			
			if (bCheck) { // PPBOX 가 있을 경우 (Sensor 확인)
				if (pThis.Glass.bMaterial != GetPPBoxSensor(pThis.eModule, TRUE)) {
					pThis.Glass.bMaterial = TRUE;
					if (sm.Sequence->bCIMSkip) {
						seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_START;
					}
					else {
						seqStep = S_LDCV1_SEQ_LOAD_S6F11_CEID_351_CASSETTE_LOAD_COMPLETE;
					}
				}
				else {
					pThis.Glass.bMaterial = TRUE;
					seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_START;
				}
			}
			else {
				// 투입 정지 일 경우 더이상 투입받지 않음.
				//if (pThis.Status.bInputStop) {
				if (!sm.Option->tSequence.fIN || pThis.Status.bInputStop || sm.Sequence->bConveyor1_BoxInStop) {
					break;
				}
				else {
					// 2024.06.01 LBH - Load, Unload 동시에 진행하면 안됨.
					//24.06.05 KGH EDIT : LOADING 시퀀스 타는 조건 수정 AND->OR
					BOOL bCheckLoadingSeq = TRUE;
					if (sm.Option->tSequence.fOUT) {	// KES 조건 추가 2024.07.15 - 배출정지가 아닐 때만 Unload 상태 체크
						bCheckLoadingSeq = (sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep < S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START);
						bCheckLoadingSeq |= (sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep > S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_END);
						//bCheck &= (sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep < S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START);
						//bCheck &= (sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep > S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_END);

						//2024.06.27 LBH - Unload CV2에 PPBox가 없을 때만 투입. (배출우선)
						bCheckLoadingSeq &= !sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Glass.bMaterial;
					}

					if (bCheckLoadingSeq) {
						//<< MCC
						MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_INIT_ORIGIN] = 1;
						//>>

					//if (bCheck) {
						// LoadConveyor2에 자재가 없거나,  자재정보 확인 완료시 사용할 수 있는 자재 인 경우 로딩 가능 (CIM 문제로 배출 해야 되는 경우에는 비어 있어야함.).
						BOOL bLoadReq = sm.DeviceParameter->tUse.bDryRun || !sm.Sequence->bConveyor1_BoxInStop;
						if (!pLower.Glass.bMaterial || bLoadReq)
							seqStep = S_LDCV1_SEQ_PPBOX_LOAD_START;
					}
				}
			}
			break;
		}

		// AGV 자동 모드.
		case S_LDCV1_SEQ_PPBOX_LOAD_START: {
			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;
			wRetry = 0;

			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[0], 0, "");
			seqStep = S_LDCV1_SEQ_PPBOX_LOAD_STOPPER_UP;

			break;
		}
		case S_LDCV1_SEQ_PPBOX_LOAD_STOPPER_UP: {
			//<< MCC
			MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_STOPPER_CYL_UP] = 1;
			//>>

			SetTimeout(&pThis);
			SetConveyor_Stoper(pThis.eModule, TRUE);
			seqStep = S_LDCV1_SEQ_PPBOX_LOAD_STOPPER_UP_CHECK;
			break;
		}
		case S_LDCV1_SEQ_PPBOX_LOAD_STOPPER_UP_CHECK: {
			if (GetConveyor_Stoper(pThis.eModule, TRUE, TRUE)) {
				//<< MCC
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_INIT_ORIGIN] = 0;
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_STOPPER_CYL_UP] = 0;
				//>>

				wRetry = 0;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_S6F11_CEID_350_CASSETTE_LOAD_REQUEST;
				
			}
			//24.03.23 KGH EDIT : 실린더 Timeout 추가
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_1_STOPER_UP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);	//실린더 Timeout Alarm 추가 필요
				}
				seqStep = S_LDCV1_SEQ_PPBOX_LOAD_STOPPER_UP;
			}
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_S6F11_CEID_350_CASSETTE_LOAD_REQUEST: {
			if (sm.Sequence->bCIMSkip) {
				pThis.Status.eReportStatus = eCIM_COMPLETE;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_S6F11_CEID_350_CASSETTE_LOAD_REQUEST_CHECK;
				break;
			}

			// 2024.04.29 LBH - AGV 호출
			sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_GREEN_LAMP = TRUE;

			// CIM Report
			S6F11_CASSETTE_LOAD_REQUEST(pThis.eModule, ePPBOX_LI_01, _pCmd);
			seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_S6F11_CEID_350_CASSETTE_LOAD_REQUEST_CHECK;
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_S6F11_CEID_350_CASSETTE_LOAD_REQUEST_CHECK: {
			bCheck &= pThis.Status.eReportStatus == eCIM_COMPLETE;

			if (bCheck) {
				if (bUseAGV)
					seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
				else
					seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_START;

				break;

				wRetry = 0;
				pThis.Status.eReportStatus = eCIM_IDLE;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_BF_CONNECT_UNLOAD_ON_CHECK;
			}
			else if ((pThis.Status.eReportStatus == eCIM_ERROR) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_LD_SEQ_CEID_350_CASSETTE_LOAD_REQUEST_CHECK_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_S6F11_CEID_350_CASSETTE_LOAD_REQUEST;
			}
			break;
		}

		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START: {
			// 2024.06.01 LBH - Load, Unload 동시에 진행하면 안됨.
			BOOL bCheckLoadingSeq = TRUE;
			if (sm.Option->tSequence.fOUT) {	// KES 조건 추가 2024.07.15 - 배출정지가 아닐 때만 Unload 상태 체크
				bCheckLoadingSeq = (sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep < S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START);
				bCheckLoadingSeq |= (sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep > S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_END);
				
				//2024.06.27 LBH - Unload CV2에 PPBox가 없을 때만 투입. (배출우선)
				bCheckLoadingSeq &= !sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Glass.bMaterial;
			}

			if (bCheckLoadingSeq) {
				//<< MCC
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_IN_WAIT_TIME] = 1;
				//>>

				wRetry = 0;
				SetTimeout(&pThis);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_INIT;
			}
			else {
				seqStep = S_LDCV1_SEQ_PPBOX_CHECK;	// Unload PIO 시작했으면 다시 대기스탭으로 보냄
			}
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_INIT: {
			//<< KES 추가 2024.07.13 - pio에 방해되는 알람 있으면 다음 스텝으로 안넘긴다
			if (bConveyorAlarm) {
				// 문서상에는 없는데 Error 비트 살려줘야 할수도 있을듯함
				break;
			}
			//>>

			//<< MCC
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_ERROR] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_OUTPUT] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_LOAD_REQUEST] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_UNLOAD_REQUEST] = 0;
			//>>

			SetPIO_EQ_ALL_OFF();
			SetTimeout(&pThis);
			seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_INIT_CHCEK;
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_INIT_CHCEK: {
			if (GetPIO_EQ_ALL_OFF()) {
				wRetry = 0;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_LOAD_ON;
			}
			else if (GetTimeout(&pThis, PIO_TIMEOUT)) {
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}

			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_LOAD_ON: {
			//<< MCC
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_LOAD_REQUEST] = 1;
			//>>

			SetTimeout(&pThis);
			SetPIO_EQ_LOAD_REQUEST(TRUE);
			seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_LOAD_ON_CHECK;
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_LOAD_ON_CHECK: {
			if (GetPIO_EQ_LOAD_REQUEST(TRUE)) {
				//<< MCC
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_IN_WAIT_TIME] = 0;
				//>>

				wRetry = 0;
				//24.07.29 KGH EDIT : MUNTING ON TIMING CHAGE TO AFTER PIO START
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_BF_CONNECT_UNLOAD_ON_CHECK;
				//seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_ON;
			}
			break;
		}
		//-->24.07.29 KGH DEL : MUNTING ON TIMING CHAGE TO AFTER PIO START
		//case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_ON: {

		//	MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_COMPONENT_IN_PPBOX] = 1;
		//	MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_PPBOX_LD_MUTE_ON] = 1;

		//	SetTimeout(&pThis);

		//	// 꺼져 있을때만 키면 됨.
		//	SetMute(pThis.eModule, TRUE, _pCmd);

		//	seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_ON_CHECK;
		//	break;
		//}
		//case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_ON_CHECK: {
		//	bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
		//	bCheck &= GetMute(TRUE);
		//	if (bCheck) {

		//		MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_PPBOX_LD_MUTE_ON] = 0;

		//		wRetry = 0;
		//		pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
		//		SetTimeout(&pThis);
		//		seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_BF_CONNECT_UNLOAD_ON_CHECK;
		//	}
		//	else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
		//		if (++wRetry > IO_RETRY) {
		//			wRetry = 0;
		//			MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
		//			SetAlarm(eAI_MUTE_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
		//		}
		//		seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_ON;
		//	}
		//	break;
		//}
		//<--24.07.29 KGH DEL : MUNTING ON TIMING CHAGE TO AFTER PIO START
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_BF_CONNECT_UNLOAD_ON_CHECK: {
			// 2024.06.01 LBH - Load, Unload 동시에 진행하면 안됨.
			BOOL bCheckLoadingSeq = TRUE;
			if (sm.Option->tSequence.fOUT) {
				bCheckLoadingSeq &= ((sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep < S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START));
				bCheckLoadingSeq |= ((sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep > S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_END));
				
				//2024.06.27 LBH - Unload CV2에 PPBox가 없을 때만 투입. (배출우선)
				bCheckLoadingSeq &= !sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Glass.bMaterial;
			}

			bCheck &= bCheckLoadingSeq;
			bCheck &= bConnect;
			bCheck &= bUnload;
			
			// 2024.07.12 LBH - Abnormal
			bCheck &= !bRequest;
			bCheck &= !bRun;
			bCheck &= !bError;
			bCheck &= !bInput;
			bCheck &= !bOutput;
			bCheck &= !bLoad;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				wRetry = 0;
				//24.07.29 KGH EDIT : MUNTING ON TIMING CHAGE TO AFTER PIO START
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_ON;
				//seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_CONNECTION_ON;
			}
			else {
				if (!bUseAGV) {
					//<< MCC
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_ERROR] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_OUTPUT] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_LOAD_REQUEST] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_UNLOAD_REQUEST] = 0;
					//>>

					SetPIO_EQ_ALL_OFF();
					seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_START;
				}
				else if (!bCheckLoadingSeq) {	// Unload PIO 시작했다면 Unload 우선 진행
					//<< MCC
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_LOAD_REQUEST] = 0;
					//>>

					SetPIO_EQ_LOAD_REQUEST(FALSE);
					seqStep = S_LDCV1_SEQ_PPBOX_CHECK;
				}
				else if (!sm.Option->tSequence.fIN) {	// AGV 대기중 투입정지 걸렸을 때
					//<< MCC
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_LOAD_REQUEST] = 0;
					//>>

					SetPIO_EQ_LOAD_REQUEST(FALSE);
					seqStep = S_LDCV1_SEQ_PPBOX_CHECK;
				}
				else if (GetPIO_EQ_LOAD_REQUEST(FALSE) && GetTimeout(&pThis, PIO_TIMEOUT)) {
					seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
				}
				else if (bConveyorAlarm) {
					seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
				}
				else if (GetTimeout(&pThis, PIO_REQUEST_WAIT_TIME)) {	// PIO_REQUEST_WAIT_TIME 동안 대기했는데도 AGV 안오면 다시 Load Request 요청.
					seqStep = S_LDCV1_SEQ_PPBOX_CHECK;
				}
			}
			break;
		}
			//-->24.07.29 KGH ADD : MUNTING ON TIMING CHAGE TO AFTER PIO START
		   case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_ON: {

		   	MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_COMPONENT_IN_PPBOX] = 1;
		   	MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_PPBOX_LD_MUTE_ON] = 1;

		   	SetTimeout(&pThis);

		   	// 꺼져 있을때만 키면 됨.
		   	SetMute(pThis.eModule, TRUE, _pCmd);

		   	seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_ON_CHECK;
		   	break;
		   }
		   case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_ON_CHECK: {
		   	bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
		   	bCheck &= GetMute(TRUE);
		   	if (bCheck) {

		   		MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_PPBOX_LD_MUTE_ON] = 0;

		   		wRetry = 0;
		   		pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
		   		SetTimeout(&pThis);
		   		seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_CONNECTION_ON;
		   	}
		   	else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
		   		if (++wRetry > IO_RETRY) {
		   			wRetry = 0;
		   			MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
		   			SetAlarm(eAI_MUTE_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
		   		}
		   		seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_ON;
		   	}
		   	break;
		   }
		   //<--24.07.29 KGH ADD : MUNTING ON TIMING CHAGE TO AFTER PIO START

		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_CONNECTION_ON: {
			//<< MCC
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 1;
			//>>

			SetTimeout(&pThis);
			SetPIO_EQ_CONNECT(TRUE);
			seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_CONNECTION_ON_CHECK;
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_CONNECTION_ON_CHECK: {
			bCheck &= GetPIO_EQ_CONNECT(TRUE);
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				wRetry = 0;
				SetTimeout(&pThis);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_BF_REQUEST_INPUT_ON_CHECK;
			}
			else if (bConveyorAlarm) {
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (GetTimeout(&pThis, PIO_TIMEOUT)) {
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_BF_REQUEST_INPUT_ON_CHECK: {
			bCheck &= bConnect;
			bCheck &= bUnload;
			bCheck &= bRequest;
			bCheck &= bInput;

			// 2024.07.12 LBH - Abnormal
			bCheck &= !bRun;
			bCheck &= !bError;
			bCheck &= !bOutput;
			bCheck &= !bLoad;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				wRetry = 0;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_FWD;
			}
			else if (!bConnect || !bUnload) {
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bRun || bError || bOutput || bLoad) {
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bConveyorAlarm) {
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (GetTimeout(&pThis, PIO_TIMEOUT)) {
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_FWD: {
			Serial::Inverter::SetInverterRun(pThis.eModule, Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dHighSpeed, _pCmd);
			seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_FWD_CHECK;
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_FWD_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dHighSpeed);
			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				SetTimeout(&pThis);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_REQUEST_RUN_INPUT_ON;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_1_RUN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_FWD;
			}
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_REQUEST_RUN_INPUT_ON: {
			bCheck &= bConnect;
			bCheck &= bUnload;
			bCheck &= bRequest;
			bCheck &= bInput;

			bCheck &= !bRun;
			bCheck &= !bError;
			bCheck &= !bOutput;
			bCheck &= !bLoad;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 1;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 1;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 1;
				//>>

				SetTimeout(&pThis);
				SetPIO_EQ_REQUEST(TRUE);
				SetPIO_EQ_RUN(TRUE);
				SetPIO_EQ_INPUT(TRUE);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_REQUEST_RUN_INPUT_ON_CHECK;
			}
			else if (!bConnect || !bUnload || !bRequest || !bInput) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				if (!bConnect) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_CONNECT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bUnload) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_UNLOAD_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRequest) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_REQUEST_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bInput) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_INPUT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bRun || bError || bOutput || bLoad) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				if (bRun) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_RUN_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bError) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_ERROR_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bOutput) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_OUTPUT_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bLoad) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_LOAD_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bConveyorAlarm) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (GetTimeout(&pThis, sm.DeviceParameter->tTime.dConveyorH)) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				SetAlarm(eAI_LOAD_CONVEYOR1_AGV_PIO_TIMEOUT, IDS_DES_NOT_USE, StepName, _pCmd);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_REQUEST_RUN_INPUT_ON_CHECK: {
			bCheck &= GetPIO_EQ_REQUEST(TRUE);
			bCheck &= GetPIO_EQ_RUN(TRUE);
			bCheck &= GetPIO_EQ_INPUT(TRUE);

			bCheck &= bConnect;
			bCheck &= bUnload;
			bCheck &= bRequest;
			bCheck &= bInput;

			bCheck &= !bRun;
			bCheck &= !bError;
			bCheck &= !bOutput;
			bCheck &= !bLoad;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				wRetry = 0;
				SetTimeout(&pThis);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_BF_RUN_ON_CHECK;
			}
			else if (!bConnect || !bUnload || !bRequest || !bInput) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				if (!bConnect) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_CONNECT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bUnload) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_UNLOAD_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRequest) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_REQUEST_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bInput) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_INPUT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bRun || bError || bOutput || bLoad) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				if (bRun) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_RUN_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bError) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_ERROR_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bOutput) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_OUTPUT_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bLoad) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_LOAD_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bConveyorAlarm) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (GetTimeout(&pThis, PIO_TIMEOUT)) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				SetAlarm(eAI_LOAD_CONVEYOR1_AGV_PIO_TIMEOUT, IDS_DES_NOT_USE, StepName, _pCmd);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_BF_RUN_ON_CHECK: {
			bCheck &= bConnect;
			bCheck &= bUnload;
			bCheck &= bRequest;
			bCheck &= bInput;
			bCheck &= bRun;

			bCheck &= !bError;
			bCheck &= !bOutput;
			bCheck &= !bLoad;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				wRetry = 0;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CV_PPBOX_CKECK;
			}
			else if (!bConnect || !bUnload || !bRequest || !bInput) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				if (!bConnect) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_CONNECT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bUnload) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_UNLOAD_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRequest) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_REQUEST_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bInput) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_INPUT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bError || bOutput || bLoad) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				if (bError) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_ERROR_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bOutput) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_OUTPUT_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bLoad) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_LOAD_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bConveyorAlarm) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (GetTimeout(&pThis, PIO_TIMEOUT)) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				SetAlarm(eAI_LOAD_CONVEYOR1_AGV_PIO_TIMEOUT, IDS_DES_NOT_USE, StepName, _pCmd);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			break;
		}
		case S_LDCV1_ABNORMAL_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_FWD: {
			CELL_INFO_TO_MCC(pThis.Glass, MCC_LAYER_LI01);
			//MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_COMPONENT_IN_PPBOX] = 1;
			MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_GET_CV_RUN] = 1;

			Serial::Inverter::SetInverterRun(pThis.eModule, Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dHighSpeed, _pCmd);
			seqStep = S_LDCV1_ABNORMAL_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_FWD_CHECK;
			break;
		}
		case S_LDCV1_ABNORMAL_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_FWD_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dHighSpeed);

			bCheck &= bConnect;
			bCheck &= bUnload;
			bCheck &= bRequest;
			bCheck &= bInput;
			bCheck &= bRun;

			bCheck &= !bError;
			bCheck &= !bOutput;
			bCheck &= !bLoad;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CV_PPBOX_CKECK;
			}
			else if (!bConnect || !bUnload || !bRequest || !bInput || !bRun) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				if (!bConnect) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_CONNECT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bUnload) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_UNLOAD_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRequest) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_REQUEST_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bInput) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_INPUT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRun) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_RUN_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bError || bOutput || bLoad) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				if (bError) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_ERROR_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bOutput) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_OUTPUT_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bLoad) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_LOAD_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bConveyorAlarm) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_1_RUN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_ABNORMAL_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_FWD;
			}
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CV_PPBOX_CKECK: {
			bCheck &= GetPPBoxSensor(pThis.eModule, TRUE);
			bCheck &= bConnect;
			bCheck &= bUnload;
			bCheck &= bRequest;
			bCheck &= bInput;
			bCheck &= bRun;

			bCheck &= !bError;
			bCheck &= !bOutput;
			bCheck &= !bLoad;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				wRetry = 0;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_LOW_SPEED;
			}
			else {
				if (!bConnect || !bUnload || !bRequest || !bInput || !bRun) {
					//<< MCC
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
					//>>

					Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
					SetPIO_EQ_CONNECT(FALSE);
					SetPIO_EQ_REQUEST(FALSE);
					SetPIO_EQ_RUN(FALSE);
					SetPIO_EQ_INPUT(FALSE);
					if (!bConnect) {
						SetAlarm(eAI_LOAD_CONVEYOR1_AGV_CONNECT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (!bUnload) {
						SetAlarm(eAI_LOAD_CONVEYOR1_AGV_UNLOAD_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (!bRequest) {
						SetAlarm(eAI_LOAD_CONVEYOR1_AGV_REQUEST_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (!bInput) {
						SetAlarm(eAI_LOAD_CONVEYOR1_AGV_INPUT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (!bRun) {
						SetAlarm(eAI_LOAD_CONVEYOR1_AGV_RUN_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
				}
				else if (bError || bOutput || bLoad) {
					//<< MCC
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
					//>>

					Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
					SetPIO_EQ_CONNECT(FALSE);
					SetPIO_EQ_REQUEST(FALSE);
					SetPIO_EQ_RUN(FALSE);
					SetPIO_EQ_INPUT(FALSE);
					if (bError) {
						SetAlarm(eAI_LOAD_CONVEYOR1_AGV_ERROR_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (bOutput) {
						SetAlarm(eAI_LOAD_CONVEYOR1_AGV_OUTPUT_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (bLoad) {
						SetAlarm(eAI_LOAD_CONVEYOR1_AGV_LOAD_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
				}
				else if (bConveyorAlarm) {
					//<< MCC
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
					MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
					//>>

					Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
					SetPIO_EQ_CONNECT(FALSE);
					SetPIO_EQ_REQUEST(FALSE);
					SetPIO_EQ_RUN(FALSE);
					SetPIO_EQ_INPUT(FALSE);
					seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
				}
				else if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorH)) {
					Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_1_SENSOR_CHECK, IDS_DES_NOT_USE, StepName, _pCmd);
					seqStep = S_LDCV1_ABNORMAL_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_FWD;
				}
			}
			break;
		}

		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_LOW_SPEED: {
			if (Serial::Inverter::GetInverterRun(pThis.eModule))
				Serial::Inverter::SetInverterFrequency(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dLowSpeed);
			else
				Serial::Inverter::SetInverterRun(pThis.eModule, Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dLowSpeed, _pCmd);

			seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_LOW_SPEED_CHECK;
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_LOW_SPEED_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dLowSpeed);

			bCheck &= bConnect;
			bCheck &= bUnload;
			bCheck &= bRequest;
			bCheck &= bInput;
			bCheck &= bRun;

			bCheck &= !bError;
			bCheck &= !bOutput;
			bCheck &= !bLoad;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PPBOX_CHECK_AFTER_DELAY;
			}
			else if (!bConnect || !bUnload || !bRequest || !bInput || !bRun) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				if (!bConnect) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_CONNECT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bUnload) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_UNLOAD_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRequest) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_REQUEST_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bInput) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_INPUT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRun) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_RUN_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bError || bOutput || bLoad) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				if (bError) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_ERROR_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bOutput) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_OUTPUT_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bLoad) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_LOAD_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bConveyorAlarm) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_1_LOW_SPEED_SET_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_LOW_SPEED;
			}
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PPBOX_CHECK_AFTER_DELAY: {
			//24.03.23 KGH EDIT : CV 센서 감지 후 CV Run On 딜레이 시간 추가
			if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorMove)) { // 센서 감지 후 일정 시간이 지난 뒤에 컨베어 멈춤
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_STOP;
			}
			//else { // 필요시 추가. Abnormal SetInverterRun(LowSpeed) 추가 필요.
			//	if (GetTimeout(&pThis, sm.DeviceParameter->tTime.dConveyorL)) {
			//		Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			//		MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
			//		SetAlarm(eAI_CONVEYOR_1_SENSOR_CHECK, _T("S_LDCV1_SEQ_CV_PPBOX_CKECK Sensor On 확인"));
			//		seqStep = S_LDCV1_SEQ_PIO_CONVEYOR_FWD;
			//		break;
			//	}
			//}
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_STOP: {
			Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_RUN_OFF;
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_RUN_OFF: {
			//<< MCC
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
			//>>

			SetTimeout(&pThis);
			SetPIO_EQ_RUN(FALSE);
			seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_RUN_OFF_CHECK;
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_RUN_OFF_CHECK: {
			bCheck &= GetPIO_EQ_RUN(FALSE);

			bCheck &= bConnect;
			bCheck &= bUnload;
			bCheck &= bRequest;
			bCheck &= bInput;
			bCheck &= bRun;

			bCheck &= !bError;
			bCheck &= !bOutput;
			bCheck &= !bLoad;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				wRetry = 0;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_ALL_OFF;
			}
			else if (!bConnect || !bUnload || !bRequest || !bInput || !bRun) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				if (!bConnect) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_CONNECT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bUnload) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_UNLOAD_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRequest) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_REQUEST_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bInput) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_INPUT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRun) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_RUN_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bError || bOutput || bLoad) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				if (bError) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_ERROR_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bOutput) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_OUTPUT_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bLoad) {
					SetAlarm(eAI_LOAD_CONVEYOR1_AGV_LOAD_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (bConveyorAlarm) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			else if (GetTimeout(&pThis, PIO_TIMEOUT)) {
				//<< MCC
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
				MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_INPUT(FALSE);
				SetAlarm(eAI_LOAD_CONVEYOR1_AGV_PIO_TIMEOUT, IDS_DES_NOT_USE, StepName, _pCmd);
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
			}
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_ALL_OFF: {
			//<< MCC
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_CONNECTION] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_REQUEST] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_RUN] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_ERROR] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_INPUT] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_POSITION_OUTPUT] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_LOAD_REQUEST] = 0;
			MCC_LAYER_LI01->S[eMCC_SIGNAL_LD_LI_01_Y_UNLOAD_REQUEST] = 0;
			//>>

			SetTimeout(&pThis);
			SetPIO_EQ_ALL_OFF();
			seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_ALL_OFF_CHECK;
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_ALL_OFF_CHECK: {
			bCheck &= GetPIO_EQ_ALL_OFF();
			if (bCheck) {
				wRetry = 0;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_BF_ALL_OFF_CHECK;
			}
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_BF_ALL_OFF_CHECK: {
			bCheck &= GetPIO_BF_ALL_OFF();
			if (bCheck) {
				wRetry = 0;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_STOP_CHECK;
			}
			break;
		}
		case S_LDCV1_ABNORMAL_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_STOP: {
			Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_STOP_CHECK;
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_STOP_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterStop(pThis.eModule);
			if (bCheck) {
				//MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_COMPONENT_IN_PPBOX] = 0;
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_GET_CV_RUN] = 0;

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_OFF;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_1_STOP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_ABNORMAL_SEQ_AUTO_PPBOX_LOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_OFF: {
			MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_PPBOX_LD_MUTE_OFF] = 1;

			SetTimeout(&pThis);
			SetMute(pThis.eModule, FALSE, _pCmd);
			seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_OFF_CHECK;
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_OFF_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= GetMute(FALSE);
			if (bCheck) {
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_PPBOX_LD_MUTE_OFF] = 0;
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_COMPONENT_IN_PPBOX] = 0;

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_END;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_MUTE_OFF_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_MUTE_OFF;
			}
			break;
		}
		case S_LDCV1_SEQ_AUTO_PPBOX_LOAD_END: {
			seqStep = S_LDCV1_SEQ_LOAD_S6F11_CEID_351_CASSETTE_LOAD_COMPLETE;
			break;
		}

		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_START: {
			if (bUseAGV) {
				seqStep = S_LDCV1_SEQ_AUTO_PPBOX_LOAD_START;
				break;
			}

			if (GetPPBoxManualInStart()) {
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MANUAL_IN;
			}
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MANUAL_IN: {
			if (bUseAGV) {
				seqStep = S_LDCV1_SEQ_INIT;
				break;
			}
			
			if (GetPPBoxManualInStart()) {
				SetTimeout(&pThis);
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MANUAL_IN_CHECK;
			}
			
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MANUAL_IN_CHECK: {
			if (bUseAGV) {
				seqStep = S_LDCV1_SEQ_INIT;
				break;
			}

			if (GetPPBoxManualInStart()) {
				if (GetTimeout(&pThis, MANUAL_BUTTON_PUSH_TIME)) {
					wRetry = 0;
					seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MUTE_ON;
				}
			}
			else {
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MANUAL_IN;
			}
			
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MUTE_ON: {
			bCheck = GetPPBoxManualInStart();

			if (bCheck) {
				//<< MCC
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_COMPONENT_IN_PPBOX] = 1;
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_PPBOX_LD_MUTE_ON] = 1;
				//>>

				SetTimeout(&pThis);

				// 꺼져 있을때만 키면 됨.
				SetMute(pThis.eModule, TRUE, _pCmd);

				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MUTE_ON_CHECK;
			}
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MUTE_ON_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= GetMute(TRUE);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_PPBOX_LD_MUTE_ON] = 0;
				//>>

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_FWD;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_MUTE_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MUTE_ON;
			}
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_FWD: {
			//<< MCC
			//MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_COMPONENT_IN_PPBOX] = 1;
			MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_GET_CV_RUN] = 1;
			//>>

			Serial::Inverter::SetInverterRun(pThis.eModule, Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dHighSpeed, _pCmd);
			seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_FWD_CHECK;
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_FWD_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dHighSpeed);
			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_PPBOX_CKECK;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_1_RUN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_FWD;
			}
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_PPBOX_CKECK: {
			if (bUseAGV) { // 중간에 AGV로 변경 대비.
				seqStep = S_LDCV1_SEQ_INIT;
				break;
			}

			if (GetPPBoxSensor(pThis.eModule, TRUE)) {
				wRetry = 0;
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_LOW_SPEED;
			}
			else {
				// 2024.06.01 LBH - 여기는 생각좀 해봐야함.
				//if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorH)) {
				//	Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				//	MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
				//	SetAlarm(eAI_CONVEYOR_1_SENSOR_CHECK, IDS_DES_NOT_USE, StepName, _pCmd);
				//	seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_FWD;
				//	break;
				//}
			}
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_LOW_SPEED: {
			if (Serial::Inverter::GetInverterRun(pThis.eModule))
				Serial::Inverter::SetInverterFrequency(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dLowSpeed);
			else
				Serial::Inverter::SetInverterRun(pThis.eModule, Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dLowSpeed, _pCmd);

			seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_LOW_SPEED_CHECK;
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_LOW_SPEED_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dLowSpeed);
			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MANUAL_COMPLETED;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_1_LOW_SPEED_SET_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_LOW_SPEED;
			}
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MANUAL_COMPLETED: {
			if (GetPPBoxManualInCompleted()) {
				SetTimeout(&pThis);
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MANUAL_COMPLETED_CHECK;
			}
			else {
				bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
				bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, Conveyor.bForward);
				bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dHighSpeed);

				if (!bCheck)
					seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_LOW_SPEED;
			}
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MANUAL_COMPLETED_CHECK: {
			if (GetPPBoxManualInCompleted()) {
				if (GetTimeout(&pThis, MANUAL_BUTTON_PUSH_TIME)) {
					seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_PPBOX_CKECK2;
				}
			}
			else {
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MANUAL_COMPLETED;
			}
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_PPBOX_CKECK2: {
			if (GetPPBoxSensor(pThis.eModule, TRUE)) {
				wRetry = 0;
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_STOP;
			}
			else {
				if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorH)) {
					Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_1_SENSOR_CHECK, IDS_DES_NOT_USE, StepName, _pCmd);
					seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_LOW_SPEED;
				}
			}
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_STOP: {
			Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_STOP_CHECK;
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_STOP_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterStop(pThis.eModule);
			if (bCheck) {
				//<< MCC
				//MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_COMPONENT_IN_PPBOX] = 0;
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_GET_CV_RUN] = 0;
				//>>

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MUTE_OFF;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_1_STOP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MUTE_OFF: {
			//<< MCC
			MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_PPBOX_LD_MUTE_OFF] = 1;
			//>>

			SetTimeout(&pThis);
			SetMute(pThis.eModule, FALSE, _pCmd);
			seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MUTE_OFF_CHECK;
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MUTE_OFF_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= GetMute(FALSE);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_PPBOX_LD_MUTE_OFF] = 0;
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_COMPONENT_IN_PPBOX] = 0;
				//>>

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_END;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					SetAlarm(eAI_MUTE_OFF_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_MUTE_OFF;
			}
			break;
		}
		case S_LDCV1_SEQ_MANUAL_PPBOX_LOAD_END: {
			seqStep = S_LDCV1_SEQ_LOAD_S6F11_CEID_351_CASSETTE_LOAD_COMPLETE;
			break;
		}


		case S_LDCV1_SEQ_LOAD_S6F11_CEID_351_CASSETTE_LOAD_COMPLETE: {
			if (sm.Sequence->bCIMSkip) {
				seqStep = S_LDCV1_SEQ_PPBOX_LOAD_END;
				break;
			}

			// CIM Report
			S6F11_CASSETTE_LOAD_COMPLETE(pThis.eModule, ePPBOX_LI_01, bUseAGV, _pCmd);
			seqStep = S_LDCV1_SEQ_LOAD_S6F11_CEID_351_CASSETTE_LOAD_COMPLETE_CHECK;
			break;
		}
		case S_LDCV1_SEQ_LOAD_S6F11_CEID_351_CASSETTE_LOAD_COMPLETE_CHECK: {
			bCheck &= pThis.Status.eReportStatus == eCIM_COMPLETE;

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eReportStatus = eCIM_IDLE;
				seqStep = S_LDCV1_SEQ_PPBOX_LOAD_END;
			}
			else if ((pThis.Status.eReportStatus == eCIM_ERROR) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_LD_SEQ_CEID_351_CASSETTE_LOAD_REQUEST_CHECK_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_LOAD_S6F11_CEID_351_CASSETTE_LOAD_COMPLETE;
			}
			break;
		}
		case S_LDCV1_SEQ_PPBOX_LOAD_END: {
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[0], 2, GetTimeStringYMDHMS());

			// 임시 PPBox ID 정보 생성
			CStringA strCstID(_T("PPBox"));
			strncpy_s(pThis.Glass.szCELLID, countof(pThis.Glass.szCELLID), strCstID, countof(pThis.Glass.szCELLID) - 1);
			pThis.Glass.bMaterial = TRUE; 

			// 2024.04.29 LBH - AGV 종료
			sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_GREEN_LAMP = FALSE;

			pThis.Status.wTargetIndex = eMODULE_NONE;
			seqStep = S_LDCV1_SEQ_INIT;
			break;
		}

		case S_LDCV1_SEQ_PPBOX_UNLOAD_START: {
			MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_OUT_WAIT_TIME] = 1;
			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;

			seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_SEND_ABLE; //24.03.23 KGH EDIT : 시퀀스 Step 정상화
			//seqStep = S_LDCV1_SEQ_INIT;
			break;
		}
		case S_LDCV1_SEQ_PPBOX_UNLOAD_SEND_ABLE: {
			pThis.ToLower.fSend_Able = TRUE;
			seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_RECV_ABLE_CHECK;
			break;
		}
		case S_LDCV1_SEQ_PPBOX_UNLOAD_RECV_ABLE_CHECK: {
			if (pLower.ToUpper.fReceive_Able) {
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_OUT_WAIT_TIME] = 0;
				seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_RECV_START_CHECK;
			}
			break;
		}
		case S_LDCV1_SEQ_PPBOX_UNLOAD_RECV_START_CHECK: {
			if (pLower.ToUpper.fReceive_Start)
				seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_SEND_START;
			break;
		}
		case S_LDCV1_SEQ_PPBOX_UNLOAD_SEND_START: {
			pThis.ToLower.fSend_Start = TRUE;
			wRetry = 0;
			seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_STOPPER_DOWN;
			break;
		}

		case S_LDCV1_SEQ_PPBOX_UNLOAD_STOPPER_DOWN: {
			MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_STOPPER_CYL_DOWN] = 1;

			SetTimeout(&pThis);
			SetConveyor_Stoper(pThis.eModule, FALSE);
			seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_STOPPER_DOWN_CHECK;
			break;
		}
		case S_LDCV1_SEQ_PPBOX_UNLOAD_STOPPER_DOWN_CHECK: {
			if (GetConveyor_Stoper(pThis.eModule, FALSE, TRUE)) {
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_STOPPER_CYL_DOWN] = 0;

				wRetry = 0;
				seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_FWD;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) { //24.03.24 KGH EDIT : 실린더 Timeout 추가
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_1_STOPER_DOWN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_STOPPER_DOWN;
			}
			break;
		}
		case S_LDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_FWD: {
			MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_COMPONENT_OUT_PPBOX] = 1;
			MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_PUT_CV_RUN] = 1;

			Serial::Inverter::SetInverterRun(pThis.eModule, Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dHighSpeed, _pCmd);
			seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_FWD_CHECK;
			break;
		}
		case S_LDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_FWD_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_1].dHighSpeed);
			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_RECV_COMPLETED_CHECK;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_1_RUN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_FWD;
			}
			break;
		}

		case S_LDCV1_SEQ_PPBOX_UNLOAD_RECV_COMPLETED_CHECK: {
			if (pLower.ToUpper.fReceive_Complete) {
				// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
				SET_STEP_CELL(sm.FDC->Conveyor[0], 0, "");

				wRetry = 0;
				seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_LDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP: {
			Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP_CHECK;
			break;
		}
		case S_LDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterStop(pThis.eModule);
			if (bCheck) {
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_COMPONENT_OUT_PPBOX] = 0;
				MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_PUT_CV_RUN] = 0;

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_SEND_COMPLETED;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI01->A[eMCC_ACTION_LD_LI_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_1_STOP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_LDCV1_SEQ_PPBOX_UNLOAD_SEND_COMPLETED: {
			pThis.ToLower.fSend_Complete = TRUE;
			seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_RECV_INIT_CHECK;
			break;
		}
		case S_LDCV1_SEQ_PPBOX_UNLOAD_RECV_INIT_CHECK: {
			if (IsInitToLower(&pLower))
				seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_SEND_INIT;
			break;
		}
		case S_LDCV1_SEQ_PPBOX_UNLOAD_SEND_INIT: {
			SetToUpperInit(&pThis);
			seqStep = S_LDCV1_SEQ_PPBOX_UNLOAD_END;
			break;
		}
		case S_LDCV1_SEQ_PPBOX_UNLOAD_END: {
			pThis.Status.wTargetIndex = eMODULE_NONE;
			seqStep = S_LDCV1_SEQ_END;
			break;
		}
		case S_LDCV1_SEQ_END: {
			seqStep = S_LDCV1_SEQ_INIT;
			break;
		}
	}
}


void LoadConveyor2(PIPCOMMAND _pCmd)
{
	T_SEQUENCE_MODULE_INFO& pUpper = sm.Module[eMODULE_PC1_LOAD_CONVEYOR_1];
	T_SEQUENCE_MODULE_INFO& pThis = sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2];
	T_SEQUENCE_MODULE_INFO& pLowerTR = sm.Module[eMODULE_PC1_GLASS_TRANSFER];
	T_SEQUENCE_MODULE_INFO& pLowerEV = sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR];

	WORD& seqStep = pThis.Status.wSeqStep;
	WORD& seqPrevStep = pThis.Status.wSeqPrevStep;

	// log용 설명이 필요하면 description에 넣으면 됨
	// 구분자로 세미콜론(;)을 사용하니 description에는 세미콜론 사용 금지
	CString description = _T("");
	auto cleanup = make_scope_exit
	(
		[&]()
		{
			// 스탭이 기존과 바뀌거나 추가 내용이 있으면 로그로 남긴다
			if (seqStep != seqPrevStep || !description.IsEmpty())
			{
				WriteSeqLog(pThis.eModule, seqPrevStep, seqStep, description);
				seqPrevStep = seqStep;
			}
		}
	);

	T_MCC_LAYER* MCC_LAYER_LI02 = &sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI02];
	T_MCC_LAYER* MCC_LAYER_LE01 = &sm.MCC_EQP->Layer[eMCC_LAYER_LD_LE01];

	// 2022.09.15 MoveFail 알람 상세용
	CString StepName(magic_enum::enum_name(static_cast<eLOAD_CONVEYOR_2>(seqStep)).data(), magic_enum::enum_name(static_cast<eLOAD_CONVEYOR_2>(seqStep)).size());

	BOOL bCheck = TRUE;
	WORD& wRetry = pThis.Status.wRetryCount;
	WORD& wRepeat = pThis.Status.wRepeatCount;

	auto Conveyor = sm.Conveyor_Parameter[eCONVEYOR_LD_2];
	int& iAlignRetry = pThis.Status.iReserve3;


	//<< 2024.06.24 LBH - EV Z축 인터락
	if (seqStep >= S_LDCV2_SEQ_PPBOX_UNLOAD_SEND_ABLE && seqStep < S_LDCV2_SEQ_PPBOX_UNLOAD_SEND_COMPLETED) {
		if (!CheckInPos(sm.MotorPos_1->EL_Convryor.Load_Z)) {
			seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_START;
		}
	}
	//

	//<< 2024.06.24 LBH - Abnormal Stopper up Check
	if (seqStep > S_LDCV2_SEQ_PPBOX_LOAD_STOPPER_UP_CHECK && seqStep < S_LDCV2_SEQ_PPBOX_LOAD_END) {
		if (!GetConveyor_Stoper(pThis.eModule, TRUE, TRUE)) {
			//SetConveyor_Stoper(pThis.eModule, TRUE); // 해당 동작만 하면 무언정지 걸리수 있기 때문에 Alarm 추가.
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
			SetAlarm(eAI_CONVEYOR_2_STOPER_UP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
			return;
		}
	}
	//>>

	switch (seqStep) {
		case S_LDCV2_SEQ_INIT: {
			//<< MCC
			INFO_INIT_MCC(MCC_LAYER_LI02);
			//>>

			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;

			pThis.Status.runningStartTime = 0;
			seqStep = S_LDCV2_SEQ_PPBOX_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_CHECK: {
			wRetry = 0;
			wRepeat = 0;

			// 자재 정보와, 센서가 다른 경우 Alarm 처리 해야 할듯.
			bCheck = (sm.DeviceParameter->tUse.bDryRun) ? pThis.Glass.bMaterial : GetPPBoxSensor(pThis.eModule, TRUE);
			if (bCheck) {
				// PPBox Empty Check (특정 Bit 추가하여 Galss 정보 Make 됬는지 확인 할 필요 있음. - Abnormal)
				//if (pThis.Status.nCrtCount) {
				if (pThis.Status.nCrtCount > 1) { //2024.07.12 LBH - 1 이면 간지 한장 남은거여서 배출 해도 됨.
					if (pThis.Status.nCrtCount % 2) {
						pThis.Status.wMaterialType = eMT_PAPER;
					}
					else {
						pThis.Status.wMaterialType = eMT_ORG_GLASS;
					}

					seqStep = S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_START;
				}
				else {
					seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_START;
				}
			}
			else {
				if (!pThis.Status.bInputStop) {
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_START;
				}
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_START: {
			//<< MCC
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_INIT_ORIGIN] = 1;
			//>>

			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;

			wRetry = 0;
			wRepeat = 0;
			iAlignRetry = 0;

			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[1], 0, "");
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_BWD;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_BWD: {
			SetTimeout(&pThis);
			SetPPBoxAlignerXY(FALSE, ePPBOX_ALIGNER_ALL);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_BWD_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_BWD_CHECK: {
			if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_ALL)) {
				wRetry = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_UP;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_X_L)) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_ALIGNER_X_L_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_X_R)) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_ALIGNER_X_R_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_Y)) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_ALIGNER_Y_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_BWD;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_UP: {
			if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_ALL)) {
				SetTimeout(&pThis);
				SetPPBoxAlignerZ(TRUE);
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_UP_CHECK;
			}
			else {
				if (++wRepeat >= IO_RETRY) {
					if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_X_L)) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_ALIGNER_X_L_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_X_R)) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_ALIGNER_X_R_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_Y)) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_ALIGNER_Y_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_BWD;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_UP_CHECK: {
			if (GetPPBoxAlignerZ(TRUE, TRUE)) {
				wRetry = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_STOPPER_UP;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_ALIGNER_Y_UP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_UP;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_STOPPER_UP: {
			//<< MCC
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_STOPPER_CYL_UP] = 1;
			//>>

			SetTimeout(&pThis);
			SetConveyor_Stoper(pThis.eModule, TRUE);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_STOPPER_UP_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_STOPPER_UP_CHECK: {
			if (GetConveyor_Stoper(pThis.eModule, TRUE, TRUE)) {
				//<< MCC
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_INIT_ORIGIN] = 0;
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_STOPPER_CYL_UP] = 0;
				//>>

				wRetry = 0;
				wRepeat = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_RECV_ABLE;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_STOPER_UP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_STOPPER_UP;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_RECV_ABLE: {
			//<< MCC
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_IN_WAIT_TIME] = 1;
			//>>

			pThis.ToUpper.fReceive_Able = TRUE;
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_SEND_ABLE_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_SEND_ABLE_CHECK: {
			// 2024.06.24 LBH - 투입 정지 관련 추가. (Able 대기때도 투입 정지면 동작하면 안됨.
			if (!pThis.Status.bInputStop) {
				if (pUpper.ToLower.fSend_Able) {
					// 2024.04.24 LBH 위치 이동.
					// Conveyor 1 Glass Input Stop
					if (!sm.Sequence->bCIMSkip)
						sm.Sequence->bConveyor1_BoxInStop = TRUE;

					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_FWD;
				}
			}

			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_FWD: {
			//<< MCC
			CELL_INFO_TO_MCC(pUpper.Glass, MCC_LAYER_LI02);
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_IN_WAIT_TIME] = 0;
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_COMPONENT_IN_PPBOX] = 1;
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_GET_CV_RUN] = 1;
			//>>

			Serial::Inverter::SetInverterRun(pThis.eModule, Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_2].dHighSpeed, _pCmd);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_FWD_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_FWD_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_2].dHighSpeed);
			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_RECV_START;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_RUN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_FWD;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_RECV_START: {
			pThis.ToUpper.fReceive_Start = TRUE;
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_SEND_START_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_SEND_START_CHECK: {
			if (pUpper.ToLower.fSend_Start) {
				SetTimeout(&pThis);
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_CHECK;
			}

			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_CHECK: {
			if (GetPPBoxSensor(pThis.eModule, TRUE)) { // Detect Sensor
				wRetry = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED;
			}
			else {
				if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorH)) {
					Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_SENSOR_CHECK, IDS_DES_NOT_USE, StepName, _pCmd);
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_FWD;
					break;
				}
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED: {
			if (Serial::Inverter::GetInverterRun(pThis.eModule))
				Serial::Inverter::SetInverterFrequency(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_2].dLowSpeed);
			else
				Serial::Inverter::SetInverterRun(pThis.eModule, Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_2].dLowSpeed, _pCmd);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_2].dLowSpeed);
			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_STOP_DELAY;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_LOW_SPEED_SET_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_STOP_DELAY: {
			bCheck = GetPPBoxSensor(pThis.eModule, TRUE);
			bCheck &= GetPPBoxSensor_Conveyor2Front(TRUE);
			
			if (sm.DeviceParameter->tUse.bDryRun) {
				bCheck &= (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorMove));
			}

			if (bCheck) {
				wRetry = 0;
				wRepeat = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_STOP;
			}
			else {
				if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorMove)) {
					if (++wRepeat > IO_RETRY) {
						wRepeat = 0;
						
						if (GetPPBoxSensor(pThis.eModule, TRUE)) {
							MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
							SetAlarm(eAI_CONVEYOR_2_SENSOR_CHECK, IDS_DES_NOT_USE, StepName, _pCmd);
						}
						if (GetPPBoxSensor_Conveyor2Front(TRUE)) {
							MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
							SetAlarm(eAI_CONVEYOR_2_FRONT_SENSOR_CHECK, IDS_DES_NOT_USE, StepName, _pCmd);
						}

						Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
					}
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_FWD;
				}
			}
				
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_STOP: {
			Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_STOP_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_STOP_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterStop(pThis.eModule);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_GET_CV_RUN] = 0;
				//>>

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_RECV_COMPLETED;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_STOP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_RECV_COMPLETED: {
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[1], 2, GetTimeStringYMDHMS());


			pThis.ToUpper.fReceive_Complete = TRUE;
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_SEND_COMPLETED_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_SEND_COMPLETED_CHECK: {
			if (pUpper.ToLower.fSend_Complete)
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_DATA_COPY;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_DATA_COPY: {
			MovePPBoxData(pUpper, pThis);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_RECV_INIT;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_RECV_INIT: {
			SetToUpperInit(&pThis);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_SEND_INIT_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_SEND_INIT_CHECK: {
			if (IsInitToLower(&pUpper)) {
				wRetry = 0;
				wRepeat = 0;
				// LJY 수정 2024.04.18 Abnomal Step은 아래의 조건 일때만 타도록 수정
				if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_X_L) || GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_X_R) || GetPPBoxAlignerZ(FALSE))
				{
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ABNORMAL_ALIGNER_BWD;
				}
				else {
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_Y_DOWN;
				}
			}
			break;
		}

		// Abnormal
		case S_LDCV2_SEQ_PPBOX_LOAD_ABNORMAL_ALIGNER_BWD: {
			SetTimeout(&pThis);
			SetPPBoxAlignerXY(FALSE, ePPBOX_ALIGNER_ALL);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ABNORMAL_ALIGNER_BWD_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ABNORMAL_ALIGNER_BWD_CHECK: {
			if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_ALL)) {
				wRetry = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ABNORMAL_ALIGNER_UP;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_X_L)) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_ALIGNER_X_L_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_X_R)) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_ALIGNER_X_R_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_Y)) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_ALIGNER_Y_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ABNORMAL_ALIGNER_BWD;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ABNORMAL_ALIGNER_UP: {
			if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_ALL)) {
				wRetry = 0;
				SetTimeout(&pThis);
				SetPPBoxAlignerZ(TRUE);
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ABNORMAL_ALIGNER_UP_CHECK;
			}
			else {
				if (++wRepeat >= IO_RETRY) {
					wRetry = 0;
					wRepeat = 0;
					if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_X_L)) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_ALIGNER_X_L_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_X_R)) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_ALIGNER_X_R_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_Y)) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_ALIGNER_Y_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ABNORMAL_ALIGNER_BWD;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ABNORMAL_ALIGNER_UP_CHECK: {
			if (GetPPBoxAlignerZ(TRUE, TRUE)) {
				wRetry = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_Y_DOWN;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_ALIGNER_Y_UP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ABNORMAL_ALIGNER_UP;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_Y_DOWN: {
			//<< MCC
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_GET_GUIDE_CYL_FWD] = 1;
			//>>

			SetTimeout(&pThis);
			SetPPBoxAlignerZ(FALSE);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_Y_DOWN_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_Y_DOWN_CHECK: {
			if (GetPPBoxAlignerZ(FALSE, TRUE)) {
				wRetry = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_L_FWD;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_ALIGNER_Y_DOWN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_Y_DOWN;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_L_FWD: {
			SetTimeout(&pThis);
			SetPPBoxAlignerXY(TRUE, ePPBOX_ALIGNER_X_L);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_L_FWD_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_L_FWD_CHECK: {
			if (GetPPBoxAlignerXY(TRUE, TRUE, ePPBOX_ALIGNER_X_L)) {
				wRetry = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_FWD;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_ALIGNER_X_L_FWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_L_FWD;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_FWD: {
			SetTimeout(&pThis);
			SetPPBoxAlignerXY(TRUE, ePPBOX_ALIGNER_X_R);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_FWD_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_FWD_CHECK: {
			if (GetPPBoxAlignerXY(TRUE, TRUE, ePPBOX_ALIGNER_X_R)) {
				wRetry = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_DELAY;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_ALIGNER_X_R_FWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_FWD;
			}
			break;
		}
		// LJY 추가 2024.04.24 - FWD후 즉시 BWD하지 않도록 딜레이 추가
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_DELAY: {
			if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dAlignTime)) {
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_BWD;
			}
			break;
		}
		// LJY 추가 2024.04.15<<
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_BWD: {
			SetTimeout(&pThis);
			SetPPBoxAlignerXY(FALSE, ePPBOX_ALIGNER_X_R);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_BWD_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_BWD_CHECK: {
			if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_X_R)) {
				wRetry = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_Y_FWD;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_X_R)) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_ALIGNER_X_R_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_BWD;
			}
			break;
		}		
		//>>
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_Y_FWD: {
			SetTimeout(&pThis);
			SetPPBoxAlignerXY(TRUE, ePPBOX_ALIGNER_Y);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_Y_FWD_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_Y_FWD_CHECK: {
			if (GetPPBoxAlignerXY(TRUE, TRUE, ePPBOX_ALIGNER_Y)) {
				if (GetPPBoxSensor_Conveyor2Front(TRUE)) {
					wRetry = 0;
					wRepeat = 0;
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_FWD_AGAIN;
				}
				else if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorMove)) {
					if (++wRepeat > IO_RETRY) { // Align 후 PPBox가 틀어져서 Front 센서가 감지 안될때.
						wRetry = 0;
						wRepeat = 0;
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_CONVEYOR_2_FRONT_SENSOR_CHECK, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ABNORMAL_ALIGNER_BWD;
				}
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_ALIGNER_Y_FWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_Y_FWD;
			}
			break;
		}
		// LJY 추가 2024.04.15 <<
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_FWD_AGAIN: {
			SetTimeout(&pThis);
			SetPPBoxAlignerXY(TRUE, ePPBOX_ALIGNER_X_R);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_FWD_AGAIN_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_FWD_AGAIN_CHECK: {
			if (GetPPBoxAlignerXY(TRUE, TRUE, ePPBOX_ALIGNER_X_R)) {
				//<< MCC
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_GET_GUIDE_CYL_FWD] = 0;
				//>>

				// 2024.06.12 LBH - MDIK 요청사항
				if (++iAlignRetry < sm.DeviceParameter->tCount.iAlignRetryCount) {
					wRetry = 0;
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ABNORMAL_ALIGNER_BWD;
				}
				else {
					wRetry = 0;
					iAlignRetry = 0;
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_READING;
				}
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_ALIGNER_X_R_FWD_AGAIN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_ALIGNER_X_R_FWD_AGAIN;
			}
			break;
		}
		//>>
		case S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_READING: {
			if (sm.DeviceParameter->tUse.bDryRun) {
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_MAKE_DATA;
			}
			else 
			{
				//<< MCC
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_GET_BARCODE_READ_PROCESS] = 1;
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_GET_BARCODE_READ_COUNT] = 1;
				//>>

				SetTimeout(&pThis);
				Serial::PPBoxCodeReader::SetPPBoxIDReading(pThis.eModule, _pCmd);
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_READING_CHECK;
			}
	
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_READING_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			//bCheck &= GetMute(TRUE);
			if (bCheck) {
				std::vector<CStringA> CurCstIDs;
				int Modules[] = {
					// eMODULE_PC1_LOAD_CONVEYOR_1, // ID 가 없음.
					// eMODULE_PC1_LOAD_CONVEYOR_2,
					eMODULE_PC1_ELEVATOR_CONVEYOR,
					eMODULE_PC1_UNLOAD_CONVEYOR_1,
					eMODULE_PC1_UNLOAD_CONVEYOR_2
				};

				for (int idx = 0; idx < _countof(Modules); ++idx) {
					auto& tempModule = sm.Module[Modules[idx]];

					if (tempModule.Glass.bMaterial) {
						CurCstIDs.push_back(tempModule.Glass.szCELL_ID_BCR);
					}
				}

				bool isExistBox = false;
				for (auto& CstId : CurCstIDs) {
					isExistBox |= CStringA(pThis.Glass.szCELL_ID_BCR) == CstId;
				}

				if (CStringA(pThis.Glass.szCELL_ID_BCR).IsEmpty()) {
					description += g_st[IDS_LOADER_SEQ_NONE_BARCODE_ID];
					if (++wRetry > IO_RETRY) {
						seqStep = S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_MANUAL_INPUT;
						break;
					}
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_READING;
					break;
				}
				else if (isExistBox && !sm.DeviceParameter->tUse.bDryRun) {
					// BarCode 값이 이전과 동일한 경우 (다시 읽어야 함)
					description += Format(_T("%s: %s"), g_st[IDS_LOADER_SEQ_BARCODE_ID_IS_SAME],CString(pThis.Glass.szCELL_ID_BCR));
					if (++wRetry > IO_RETRY) {
						seqStep = S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_MANUAL_INPUT;
						break;
					}
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_READING;
					break;
				}

				wRetry = 0;

				description += Format(_T("%s : %s"), g_st[IDS_LOADER_SEQ_BARCODE_ID],CString(pThis.Glass.szCELL_ID_BCR));
				
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_262_CARRIER_INFORMATION_REQUEST_RESET;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				//<< MCC
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_GET_BARCODE_READ_COUNT] = 0;
				//>>

				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_MANUAL_INPUT;
					break;
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_READING;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_MANUAL_INPUT: {
			CString strdescription = _T("");
			if (CStringA(pThis.Glass.szCELL_ID_BCR) == CStringA(pLowerEV.Glass.szCELL_ID_BCR)) {
				strdescription = g_st[IDS_LOADER_SEQ_SAME_BEFORE_BARCODE];
			}
			else if (CStringA(pThis.Glass.szCELL_ID_BCR).IsEmpty()) {
				strdescription = g_st[IDS_LOADER_SEQ_BARCODE_IS_EMPTY];
			}

			PPBoxIDManualInput(pThis.eModule, strdescription, _pCmd);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_MANUAL_INPUT_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_MANUAL_INPUT_CHECK: {
			if (sm.Module[pThis.eModule].Status.eMessageStatus == eMESSAGE_RESPONSE) {
				wRetry = 0;
				if (sm.Module[pThis.eModule].Status.wMessageResult == eBCR_MESSAGE_RETRY) {
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_READING;
				}
				else {
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_262_CARRIER_INFORMATION_REQUEST_RESET;
				}
			}
			else {
				//if (GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (GetTimeout(&pThis, 1 * 60 * 1000)) {
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_PPBOX_ID_MANUAL_INPUT;
				}
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_262_CARRIER_INFORMATION_REQUEST_RESET: {
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[1], 2, pThis.Glass.szCELL_ID_BCR);

			if (sm.Sequence->bCIMSkip) {
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_MAKE_DATA;
				break;
			}


			// CIM Report
			S6F11_CARRIER_INFORMATION_REQUEST_RESET(pThis.eModule, ePPBOX_LI_01, _pCmd);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_262_CARRIER_INFORMATION_REQUEST_RESET_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_262_CARRIER_INFORMATION_REQUEST_RESET_CHECK: {
			bCheck &= pThis.Status.eReportStatus == eCIM_COMPLETE;
			bCheck &= !sm.Sequence->CIMData.SCT.PPBox.bDataSet;
			if (bCheck) {
				pThis.Status.iTimeOut_CIM = GetTickCount64();

				wRetry = 0;
				pThis.Status.eReportStatus = eCIM_IDLE;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_262_CARRIER_INFORMATION_REQUEST;
				break;
			}
			else if ((pThis.Status.eReportStatus == eCIM_ERROR) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_262_CARRIER_INFORMATION_REQUEST_RESET;
			}

			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_262_CARRIER_INFORMATION_REQUEST: {
			if (sm.Sequence->bCIMSkip) {
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_MAKE_DATA;
				break;
			}

			strncpy_s(pThis.Glass.szCELLID, sizeof(pThis.Glass.szCELLID), pThis.Glass.szCELL_ID_BCR, sizeof(pThis.Glass.szCELLID) - 1);

			// CIM Report
			S6F11_CARRIER_INFORMATION_REQUEST(pThis.eModule, ePPBOX_LI_01, pThis.Glass.szCELL_ID_BCR, _pCmd);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_262_CARRIER_INFORMATION_REQUEST_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_262_CARRIER_INFORMATION_REQUEST_CHECK: {
			bCheck &= pThis.Status.eReportStatus == eCIM_COMPLETE;

			if (bCheck) {
				pThis.Status.iTimeOut_CIM = GetTickCount64();

				wRetry = 0;
				pThis.Status.eReportStatus = eCIM_IDLE;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S3F115_CARRIER_INFORMATION_DATA_RECV_WAIT;
				break;
			}
			else if ((pThis.Status.eReportStatus == eCIM_ERROR) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_262_CARRIER_INFORMATION_REQUEST_CHECK_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_262_CARRIER_INFORMATION_REQUEST;
			}

			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_S3F115_CARRIER_INFORMATION_DATA_RECV_WAIT: {
			if (!sm.Sequence->CIMData.SCT.PPBox.bDataSet) {
				BOOL bRecvTimeout = (GetTickCount64() - pThis.Status.iTimeOut_CIM) > 5 * 60 * 1000;
				if (bRecvTimeout) {
					pThis.Status.iTimeOut_CIM = GetTickCount64();
					if (pThis.Glass.bMaterial) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_LDCV2_SEQ_PPBOX_LOAD_RECV_LOT_DATA_CHECK_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					else {
						seqStep = S_LDCV2_SEQ_INIT;
						break;
					}
				}
			}
			else {
				// 응답이 왔는데 REPLYCODE가 PASS 아닐때
				if (strcmp(sm.Sequence->CIMData.SCT.PPBox.szREPLYCODE, "PASS") == 0) {
					// 2024.04.17 LBH - 자재정보 REPLYCODE가 PASS로 내려 왔기 때문에 다음 PPBox 받아도 됨.
					// release Conveyor 1 Glass In Stop
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_257_CARRIER_RELEASE_COMPLETE_RESET;
				}
				else {
					if (pThis.Glass.bMaterial) {
						// S3F115 CARRIER_INFORMATION_DATA - REPLYCODE FAIL Alarm (Light Alarm)
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_LDCV2_SEQ_PPBOX_LOAD_RECV_LOT_DATA_CHECK_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					else {
						sm.Sequence->bConveyor1_BoxInStop = FALSE;
						seqStep = S_LDCV2_SEQ_INIT;
						break;
					}
				}
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_257_CARRIER_RELEASE_COMPLETE_RESET: {
			if (sm.Sequence->bCIMSkip) {
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_MAKE_DATA;
				break;
			}

			// CIM Report
			S6F11_CARRIER_RELEASE_COMPLETE_RESET(pThis.eModule, ePPBOX_LI_01, _pCmd);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_257_CARRIER_RELEASE_COMPLETE_RESET_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_257_CARRIER_RELEASE_COMPLETE_RESET_CHECK: {
			bCheck &= pThis.Status.eReportStatus == eCIM_COMPLETE;
			bCheck &= !sm.Sequence->CIMData.SCT.PPBox.bDataSet;

			if (bCheck) {
				wRetry = 0;
				pThis.Status.iTimeOut_CIM = GetTickCount64();
				pThis.Status.eReportStatus = eCIM_IDLE;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_257_CARRIER_RELEASE_COMPLETE;
				break;
			}
			else if ((pThis.Status.eReportStatus == eCIM_ERROR) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_257_CARRIER_RELEASE_COMPLETE_RESET;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_257_CARRIER_RELEASE_COMPLETE: {
			if (sm.Sequence->bCIMSkip) {
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_MAKE_DATA;
				break;
			}

			// CIM Report
			S6F11_CARRIER_RELEASE_COMPLETE(pThis.eModule, ePPBOX_LI_01, pThis.Glass.szCELL_ID_BCR, _pCmd);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_257_CARRIER_RELEASE_COMPLETE_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_257_CARRIER_RELEASE_COMPLETE_CHECK: {
			bCheck &= pThis.Status.eReportStatus == eCIM_COMPLETE;

			if (bCheck) {
				wRetry = 0;
				pThis.Status.iTimeOut_CIM = GetTickCount64();
				pThis.Status.eReportStatus = eCIM_IDLE;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S3F115_CARRIER_RELEASE_REPLY_CHECK;
				break;
			}
			else if ((pThis.Status.eReportStatus == eCIM_ERROR) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					// Alarm 명 추가.
					//SetAlarm(eAI_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_262_CARRIER_INFORMATION_REQUEST_CHECK_FAIL, IDS_DES_LD_SEQ_PPBOX_LOAD_S6F1_CEID_262_CARRIER_INFO_REQUEST_CHECK_FAIL, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S6F11_CEID_257_CARRIER_RELEASE_COMPLETE;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_S3F115_CARRIER_RELEASE_REPLY_CHECK: {
			if (!sm.Sequence->CIMData.SCT.PPBox.bDataSet) {
				BOOL bRecvTimeout = (GetTickCount64() - pThis.Status.iTimeOut_CIM) > 5 * 60 * 1000;
				if (bRecvTimeout) {
					pThis.Status.iTimeOut_CIM = GetTickCount64();
					if (pThis.Glass.bMaterial) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_LDCV2_SEQ_S3F115_CARRIER_RELEASE_REPLY_TIMEOUT, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					else {
						seqStep = S_LDCV2_SEQ_INIT;
						break;
					}
				}
			}
			else {
				// 응답이 왔는데 REPLYCODE가 PASS 아닐때
				if (strcmp(sm.Sequence->CIMData.SCT.PPBox.szREPLYCODE, "PASS") == 0) {
					// 2024.04.17 LBH - 자재정보 REPLYCODE가 PASS로 내려 왔기 때문에 다음 PPBox 받아도 됨.
					// release Conveyor 1 Glass In Stop
					sm.Sequence->bConveyor1_BoxInStop = FALSE;
					seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S3F115_CARRIER_INFORMATION_DATA_RECV;
				}
				else {
					if (pThis.Glass.bMaterial) {
						MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
						SetAlarm(eAI_LDCV2_SEQ_S3F115_CARRIER_RELEASE_REPLY_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					else {
						sm.Sequence->bConveyor1_BoxInStop = FALSE;
						seqStep = S_LDCV2_SEQ_INIT;
						break;
					}
				}
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_S3F115_CARRIER_INFORMATION_DATA_RECV: {
			if (sm.Sequence->bCIMSkip) {
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_MAKE_DATA;
				break;
			}

			// DB에서 Lot Data Recv 하기
			RECV_PPBOX_LOT_DATA(pThis.eModule, pThis.Glass.szCELL_ID_BCR, _pCmd);
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S3F115_CARRIER_INFORMATION_DATA_RECV_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_S3F115_CARRIER_INFORMATION_DATA_RECV_CHECK: {
			bCheck &= pThis.Status.eReportStatus == eCIM_COMPLETE;

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eReportStatus = eCIM_IDLE;
				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_MAKE_DATA;
				break;
			}
			//else if ((pThis.Status.eReportStatus == eCIM_ERROR) || (GetTimeout(&pThis, POST_MESSAGE_TIME_OUT))) {
			else if ((pThis.Status.eReportStatus == eCIM_ERROR) || (GetTimeout(&pThis, 5 * 60 * 1000))) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_LDCV2_SEQ_PPBOX_LOAD_RECV_LOT_DATA_CHECK_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}

				seqStep = S_LDCV2_SEQ_PPBOX_LOAD_S3F115_CARRIER_INFORMATION_DATA_RECV;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_MAKE_DATA: {
			// 자재 정보를 만들어서 넣어주거나 CIM 에서 받아서 넣어주면 됨.
			// 자재 Count Setting
			// 상위에서 받지 않고 Recipe 에서 받는 경우
			auto& PPBox = sm.LoadPPBox->TRAY_INFOs[0];
			if (sm.Sequence->bCIMSkip) { // Off Line Mode로 바꿔야 할 수도.
				// CIM Skip Mode 일 때 자재 정보 셋팅.
				if (pLowerEV.Glass.bMaterial) {
					PPBox.nCell = g_pRecipe->ppbox.iGlassCount;
					CStringA strCstID(pThis.Glass.szCELL_ID_BCR);
					for (int iGlass = 0; iGlass < g_pRecipe->ppbox.iGlassCount; iGlass++) {
						auto& Cell = PPBox.CELL_INFOs[iGlass];
						CStringA GlassID(Format("%sAA%02d", strCstID, iGlass));
						strncpy_s(Cell.szCELLID, countof(Cell.szCELLID), strCstID, countof(Cell.szCELLID) - 1);
						strncpy_s(Cell.szCELL_ID_BCR, countof(Cell.szCELL_ID_BCR), GlassID, countof(Cell.szCELL_ID_BCR) - 1);
						
						Cell.bMaterial = TRUE;
						// Make Glass ID 해야 할수도?
					}
				}
				else {
					PPBox.nCell = 0;
					pThis.Status.nCrtCount = 0;
				}
			}

			// 2024.06.12 LBH - 주석처리 (여기에는 필요 없을것 같음)
			//else {
			//	// PASS 가 와야함.
			//	if (strcmp(sm.LoadPPBox->CassetteInfo.szREPLYCODE, "PASS") != 0) { // FAIL
			//		if (pThis.Glass.bMaterial) { // Fail Or Time Alarm.
			//			// PPBox 자재 정보 Light Alarm (PPBox 배출 할 때 까지 PPBox 투입 불가.
			//			//SetAlarm();
			//		}
			//		else {
			//			// Scrap로 PPBox제거 하면 여기 (안해도 될수도 제거 하면서 바로 Alarm)
			//			seqStep = S_LDCV2_SEQ_INIT;
			//		}
			//		break;
			//	}
			//}

			// 상위에서 데이터를 받는 경우 
			if (PPBox.nCell != 0) {
				pThis.Status.nCrtCount = PPBox.nCell * 2; //CV1의 nMaxCount를 보는게 맞는지?
				if (!g_pRecipe->ppbox.bTopMaterialType) {
					pThis.Status.nCrtCount++;
				}
			}

			pThis.Glass.iGlassIndex = 0;
			seqStep = S_LDCV2_SEQ_PPBOX_LOAD_END;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_LOAD_END: {
			//<< MCC
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_GET_BARCODE_READ_PROCESS] = 0;
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_COMPONENT_IN_PPBOX] = 0;
			//>>
		
			// 2024.06.20 LBH - PPBox Load 완료시 Offset 초기화
			sm.Sequence->dGlassTransferOffsetZ = 0.;
			sm.Sequence->bConveyor1_BoxInStop = FALSE;
			pThis.Status.wTargetIndex = eMODULE_NONE;
			seqStep = S_LDCV2_SEQ_INIT;
			break;
		}

		// Glass or Paper 있으면.
		case S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_START: {
			//<< MCC
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_OUT_WAIT_TIME] = 1;
			//>>

			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;

			wRetry = 0;
			pThis.Status.wTargetIndex = eMODULE_PC1_GLASS_TRANSFER;
			seqStep = S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_SEND_ABLE;
			break;
		}
		case S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_SEND_ABLE: {
			pThis.ToLower.fSend_Able = TRUE;
			seqStep = S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_RECV_ABLE_CHECK;
			break;
		}
		case S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_RECV_ABLE_CHECK: {
			if (pLowerTR.ToUpper.fReceive_Able)
				seqStep = S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_SEND_START;
			break;
		}
		case S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_SEND_START: {
			pThis.ToLower.fSend_Start = TRUE;
			seqStep = S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_RECV_START_CHECK;
			break;
		}
		case S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_RECV_START_CHECK: {
			if (pLowerTR.ToUpper.fReceive_Start) {
				////<< MCC
				//MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_OUT_WAIT_TIME] = 0;
				////>>
				
				seqStep = S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_RECV_COMPLETED_CHECK;
			}
			break;
		}
		case S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_RECV_COMPLETED_CHECK: {
			if (pLowerTR.ToUpper.fReceive_Complete)
				seqStep = S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_SEND_COMPLETED;
			break;
		}
		case S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_SEND_COMPLETED: {
			pThis.ToLower.fSend_Complete = TRUE;
			seqStep = S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_RECV_INIT_CHECK;
			break;
		}
		case S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_RECV_INIT_CHECK: {
			if (IsInitToUpper(&pLowerTR))
				seqStep = S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_SEND_INIT;
			break;
		}
		case S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_SEND_INIT: {
			SetToLowerInit(&pThis);
			seqStep = S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_END;
			break;
		}
		case S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_END: {
			pThis.Status.wTargetIndex = eMODULE_NONE;
			seqStep = S_LDCV2_SEQ_INIT;
			break;
		}

		// Glass or Paper 없으면.
		case S_LDCV2_SEQ_PPBOX_UNLOAD_START: {
			//<< MCC
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_OUT_WAIT_TIME_PPBOX] = 1;
			//>>
		
			// 2024.06.20 LBH - 혹시 몰라서 이중 초기화
			sm.Sequence->dGlassTransferOffsetZ = 0.;

			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;

			wRetry = 0;
			pThis.Status.wTargetIndex = eMODULE_PC1_ELEVATOR_CONVEYOR;
			seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_SEND_ABLE;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_SEND_ABLE: {
			pThis.ToLower.fSend_Able = TRUE;
			seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_RECV_ABLE_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_RECV_ABLE_CHECK: {
			bCheck = pLowerEV.Status.wTargetIndex == pThis.eModule;
			bCheck &= pLowerEV.ToUpper.fReceive_Able;

			if (bCheck)
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_RECV_START_CHECK;

			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_RECV_START_CHECK: {
			if (pLowerEV.ToUpper.fReceive_Start) {
				//<< MCC
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_OUT_WAIT_TIME_PPBOX] = 0;
				//>>

				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_SEND_START;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_SEND_START: {
			wRetry = 0;
			wRepeat = 0;
			pThis.ToLower.fSend_Start = TRUE;
			seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_ALIGNER_BWD;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_ALIGNER_BWD: {
			//<< MCC
			CELL_INFO_TO_MCC(pThis.Glass, MCC_LAYER_LE01);
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_COMPONENT_OUT_PPBOX] = 1;
			MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_COMPONENT_IN_PPBOX] = 1;
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_GUIDE_CYL_BWD] = 1;
			//>>

			SetTimeout(&pThis);
			SetPPBoxAlignerXY(FALSE, ePPBOX_ALIGNER_ALL);
			seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_ALIGNER_BWD_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_ALIGNER_BWD_CHECK: {
			if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_ALL)) {
				wRetry = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_ALIGNER_UP;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					//SetAlarm(); //알람 내용 추가 필요
				}
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_ALIGNER_BWD;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_ALIGNER_UP: {
			if (GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_ALL)) {
				SetTimeout(&pThis);
				SetPPBoxAlignerZ(TRUE);
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_ALIGNER_UP_CHECK;
			}
			else {
				if (++wRepeat >= IO_RETRY) {
					//wRepeat = 0;
					//SetAlarm(); //알람 내용 추가 필요
				}
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_ALIGNER_BWD;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_ALIGNER_UP_CHECK: {
			if (GetPPBoxAlignerZ(TRUE, TRUE)) {
				//<< MCC
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_GUIDE_CYL_BWD] = 0;
				//>>

				wRetry = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_STOPPER_DOWN;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					//SetAlarm();
				}
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_ALIGNER_UP;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_STOPPER_DOWN: {
			//<< MCC
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_STOPPER_CYL_DOWN] = 1;
			//>>

			SetTimeout(&pThis);
			SetConveyor_Stoper(pThis.eModule, FALSE);
			seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_STOPPER_DOWN_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_STOPPER_DOWN_CHECK: {
			if (GetConveyor_Stoper(pThis.eModule, FALSE, TRUE)) {
				//<< MCC
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_STOPPER_CYL_DOWN] = 0;
				//>>

				wRetry = 0;
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_CONVEYOR_FWD;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_STOPER_DOWN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_STOPPER_DOWN;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_CONVEYOR_FWD: {
			//<< MCC
			MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_CV_RUN] = 1;
			//>>

			Serial::Inverter::SetInverterRun(pThis.eModule, Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_2].dHighSpeed, _pCmd);
			seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_CONVEYOR_FWD_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_CONVEYOR_FWD_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_LD_2].dHighSpeed);
			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_RECV_COMPLETED_CHECK;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_RUN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_CONVEYOR_FWD;
			}
			break;
		}

		case S_LDCV2_SEQ_PPBOX_UNLOAD_RECV_COMPLETED_CHECK: {
			if (pLowerEV.ToUpper.fReceive_Complete)
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_SEND_COMPLETED;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_SEND_COMPLETED: {
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[1], 0, "");

			pThis.ToLower.fSend_Complete = TRUE;
			seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_RECV_INIT_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_RECV_INIT_CHECK: {
			if (IsInitToUpper(&pLowerEV))
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_SEND_INIT;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_SEND_INIT: {
			pThis.Status.wTargetIndex = eMODULE_NONE;
			SetToLowerInit(&pThis);

			seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP: {
			Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP_CHECK;
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterStop(pThis.eModule);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_COMPONENT_OUT_PPBOX] = 0;
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_COMPONENT_IN_PPBOX] = 0;
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_CV_RUN] = 0;
				//>>

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_END;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_2_STOP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_LDCV2_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_LDCV2_SEQ_PPBOX_UNLOAD_END: {
			pThis.Status.wTargetIndex = eMODULE_NONE;
			seqStep = S_LDCV2_SEQ_END;
			break;
		}
		case S_LDCV2_SEQ_END: {
			seqStep = S_LDCV2_SEQ_INIT;
			break;
		}
	}
}


void ElevatorConveyor(PIPCOMMAND _pCmd)
{
	T_SEQUENCE_MODULE_INFO& pUpperCV = sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2];
	T_SEQUENCE_MODULE_INFO& pUpperTR = sm.Module[eMODULE_PC1_GLASS_TRANSFER];
	T_SEQUENCE_MODULE_INFO& pThis = sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR];
	T_SEQUENCE_MODULE_INFO& pLower = sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_1];

	WORD& seqStep = pThis.Status.wSeqStep;
	WORD& seqPrevStep = pThis.Status.wSeqPrevStep;

	// log용 설명이 필요하면 description에 넣으면 됨
	// 구분자로 세미콜론(;)을 사용하니 description에는 세미콜론 사용 금지
	CString description = _T("");
	auto cleanup = make_scope_exit
	(
		[&]()
		{
			// 스탭이 기존과 바뀌거나 추가 내용이 있으면 로그로 남긴다
			if (seqStep != seqPrevStep || !description.IsEmpty())
			{
				WriteSeqLog(pThis.eModule, seqPrevStep, seqStep, description);
				seqPrevStep = seqStep;
			}
		}
	);

	T_MCC_LAYER* MCC_LAYER_LE01 = &sm.MCC_EQP->Layer[eMCC_LAYER_LD_LE01];
	T_MCC_LAYER* MCC_LAYER_LO01 = &sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO01];

	// 2022.09.15 MoveFail 알람 상세용
	CString StepName(magic_enum::enum_name(static_cast<eELEVATOR_CONVEYOR>(seqStep)).data(), magic_enum::enum_name(static_cast<eELEVATOR_CONVEYOR>(seqStep)).size());

	BOOL bCheck = TRUE;
	WORD& wRetry = pThis.Status.wRetryCount;
	WORD& wRepeat = pThis.Status.wRepeatCount;

	auto Conveyor = sm.Conveyor_Parameter[eCONVEYOR_EL];

	//<< 2024.06.24 LBH - EV Z축 인터락

	// Conveyor2에서 PPBox 받을 때.
	if (seqStep > S_ELCV_SEQ_PPBOX_LOAD_Z_UP_CHECK && seqStep < S_ELCV_SEQ_PPBOX_LOAD_DATA_COPY) {
		if (!CheckInPos(sm.MotorPos_1->EL_Convryor.Load_Z)) {
			seqStep = S_ELCV_SEQ_PPBOX_LOAD_START;
		}
	}

	// Glass Transfer에서 간지 받을 때.
	if (seqStep >= S_ELCV_SEQ_PAPER_LOAD_RECV_ABLE && seqStep < S_ELCV_SEQ_PAPER_LOAD_RECV_COMPLETED) {
		if (!CheckInPos(sm.MotorPos_1->EL_Convryor.Load_Z)) {
			seqStep = S_ELCV_SEQ_PAPER_LOAD_START;
		}
	}
	//>>

	switch (seqStep) {
		case S_ELCV_SEQ_INIT: {
			//<< MCC
			INFO_INIT_MCC(MCC_LAYER_LE01);
			//>>

			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;
			pThis.Status.runningStartTime = 0;
			seqStep = S_ELCV_SEQ_PPBOX_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_CHECK: {
			// Elevator Conveyor에 PPBox 가 있을 떄, Load Conveyor 2에 PPBOX가 있고, 자재가 없을 경우
			BOOL bPPBox = (sm.DeviceParameter->tUse.bDryRun) ? pThis.Glass.bMaterial : GetPPBoxSensor(pThis.eModule, TRUE);
			bCheck = bPPBox;
			if (bCheck) {
				if (bPPBox) { // PPBOX가 있을 때,
					// 2024.07.12 LBH - nCrtCount가 1이면 간지 한장만 남은 상황이여서 배출 해도 됨.
					//if ((pUpperCV.Status.nCrtCount) || (pUpperTR.Glass.bMaterial && pUpperTR.Status.wMaterialType == eMT_PAPER)) { // PPBOX에 자재가 있을 떄.
					if ((pUpperCV.Status.nCrtCount > 1) || (pUpperTR.Glass.bMaterial && pUpperTR.Status.wMaterialType == eMT_PAPER)) { // PPBOX에 자재가 있을 떄.
						seqStep = S_ELCV_SEQ_PAPER_LOAD_START;
					}
					//else if ((pUpperCV.Glass.bMaterial) && (pUpperCV.Status.nCrtCount <= 0) && (pUpperCV.Status.wTargetIndex == pThis.eModule) && (!pUpperTR.Glass.bMaterial)) {
					 else if ((pUpperCV.Glass.bMaterial) && (pUpperCV.Status.nCrtCount <= 1) && (pUpperCV.Status.wTargetIndex == pThis.eModule) && (!pUpperTR.Glass.bMaterial)) {
						// 자재가 없는 경우 Load Conveyor에 있는 PPBOX를 받아야 하기 때문에 배출.
						seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_START;
					}
				}
			}
			else {
				if (!pThis.Status.bInputStop)
					seqStep = S_ELCV_SEQ_PPBOX_LOAD_START;
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_START: {
			//<< MCC
			MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_INIT_ORIGIN] = 1;
			//>>

			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;

			wRetry = 0;
			pThis.Status.wTargetIndex = pUpperCV.eModule;

			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[2], 0, "");
			seqStep = S_ELCV_SEQ_PPBOX_LOAD_Z_UP;
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_Z_UP: {
			//<< MCC
			MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_MOVE_Z_LD_POS] = 1;
			//>>

			PTPMove(sm.MotorPos_1->EL_Convryor.Load_Z);
			seqStep = S_ELCV_SEQ_PPBOX_LOAD_Z_UP_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_Z_UP_CHECK: {
			bCheck &= CheckInPos(sm.MotorPos_1->EL_Convryor.Load_Z);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_MOVE_Z_LD_POS] = 0;
				//>>

				seqStep = S_ELCV_SEQ_PPBOX_LOAD_STOPPER_DOWN;
			}
			else {
				if (GetMoveTimeout(sm.MotorPos_1->EL_Convryor.Load_Z)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_BOX_LIFT_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, sm.MotorPos_1->EL_Convryor.Load_Z, _pCmd);
					}
					seqStep = S_ELCV_SEQ_PPBOX_LOAD_Z_UP;
				}
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_STOPPER_DOWN: {
			SetTimeout(&pThis);
			SetConveyor_Stoper(pThis.eModule, FALSE);
			seqStep = S_ELCV_SEQ_PPBOX_LOAD_STOPPER_DOWN_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_STOPPER_DOWN_CHECK: {
			if (GetConveyor_Stoper(pThis.eModule, FALSE, TRUE)) {
				//<< MCC
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_INIT_ORIGIN] = 0;
				//>>

				wRetry = 0;
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_RECV_ABLE;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_3_STOPER_DOWN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_STOPPER_DOWN;
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_RECV_ABLE: {
			//<< MCC
			MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_IN_WAIT_TIME_PPBOX] = 1;
			//>>

			pThis.ToUpper.fReceive_Able = TRUE;
			seqStep = S_ELCV_SEQ_PPBOX_LOAD_SEND_ABLE_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_SEND_ABLE_CHECK: {
			bCheck = pUpperCV.Status.wTargetIndex == pThis.eModule;
			bCheck &= pUpperCV.ToLower.fSend_Able;

			if (bCheck) {
				//<< MCC
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_IN_WAIT_TIME_PPBOX] = 0;
				//>>

				seqStep = S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_FWD;
			}

			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_FWD: {
			//<< MCC
			MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_GET_CV_RUN] = 1;
			//>>

			Serial::Inverter::SetInverterRun(pThis.eModule, Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_EL].dHighSpeed, _pCmd);
			seqStep = S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_FWD_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_FWD_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_EL].dHighSpeed);
			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_RECV_START;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_3_RUN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_FWD;
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_RECV_START: {
			pThis.ToUpper.fReceive_Start = TRUE;
			seqStep = S_ELCV_SEQ_PPBOX_LOAD_SEND_START_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_SEND_START_CHECK: {
			if (pUpperCV.ToLower.fSend_Start)
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_PPBOX_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_PPBOX_CHECK: {
			if (GetPPBoxSensor(pThis.eModule, TRUE)) {
				wRetry = 0;
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED; //24.03.23 KGH EDIT : 시퀀스 Step 오설정 수정
				//seqStep = S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_STOP_DELAY;
			}
			else {
				if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorH)) {
					Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
					MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_3_SENSOR_CHECK, IDS_DES_NOT_USE, StepName, _pCmd);
					seqStep = S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_FWD;
					break;
				}
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED: {
			if (Serial::Inverter::GetInverterRun(pThis.eModule))
				Serial::Inverter::SetInverterFrequency(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_EL].dLowSpeed);
			else
				Serial::Inverter::SetInverterRun(pThis.eModule, Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_EL].dLowSpeed, _pCmd);

			seqStep = S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_EL].dLowSpeed);
			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_STOP_DELAY;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_3_LOW_SPEED_SET_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED;
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_STOP_DELAY: {
			if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorMove)) {
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_STOP: {
			Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			seqStep = S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_STOP_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_STOP_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterStop(pThis.eModule);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_GET_CV_RUN] = 0;
				//>>

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_AFTER_STOPPER_UP;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_3_STOP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_AFTER_STOPPER_UP: {
			SetTimeout(&pThis);
			SetConveyor_Stoper(pThis.eModule, TRUE);
			seqStep = S_ELCV_SEQ_PPBOX_LOAD_AFTER_STOPPER_UP_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_AFTER_STOPPER_UP_CHECK: {
			if (GetConveyor_Stoper(pThis.eModule, TRUE, TRUE)) {
				wRetry = 0;
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_RECV_COMPLETED;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_3_STOPER_UP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_AFTER_STOPPER_UP;
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_RECV_COMPLETED: {
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[2], 2, GetTimeStringYMDHMS());

			pThis.ToUpper.fReceive_Complete = TRUE;
			seqStep = S_ELCV_SEQ_PPBOX_LOAD_SEND_COMPLETED_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_SEND_COMPLETED_CHECK: {
			if (pUpperCV.ToLower.fSend_Complete)
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_DATA_COPY;

			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_DATA_COPY: {
			MovePPBoxData(pUpperCV, pThis);
			seqStep = S_ELCV_SEQ_PPBOX_LOAD_SEQ_RECV_INIT;
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_SEQ_RECV_INIT: {
			SetToUpperInit(&pThis);
			seqStep = S_ELCV_SEQ_PPBOX_LOAD_SEQ_SEND_INIT_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_SEQ_SEND_INIT_CHECK: {
			if (IsInitToLower(&pUpperCV))
				seqStep = S_ELCV_SEQ_PPBOX_LOAD_SEQ_END;

			break;
		}
		case S_ELCV_SEQ_PPBOX_LOAD_SEQ_END: {
			pThis.Status.wTargetIndex = eMODULE_NONE;
			seqStep = S_ELCV_SEQ_INIT;
			break;
		}

		case S_ELCV_SEQ_PAPER_LOAD_START: {
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			if (sm.FDC->Conveyor[2].iSTEP_ID == 0)
				SET_STEP_CELL(sm.FDC->Conveyor[2], 2, GetTimeStringYMDHMS());

			//<< MCC
			MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_IN_WAIT_TIME_PAPER] = 1;
			//>>

			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;

			wRetry = 0;
			pThis.Status.wTargetIndex = pUpperTR.eModule;
			if (CheckInPos(sm.MotorPos_1->EL_Convryor.Load_Z)) {
				seqStep = S_ELCV_SEQ_PAPER_LOAD_RECV_ABLE;
			}
			else {
				seqStep = S_ELCV_SEQ_PAPER_LOAD_Z_UP_ABNORMAL;
			}
			break;
		}


		case S_ELCV_SEQ_PAPER_LOAD_Z_UP_ABNORMAL: {
			PTPMove(sm.MotorPos_1->EL_Convryor.Load_Z);
			seqStep = S_ELCV_SEQ_PAPER_LOAD_Z_UP_ABNORMAL_CHECK;
			break;
		}
		case S_ELCV_SEQ_PAPER_LOAD_Z_UP_ABNORMAL_CHECK: {
			bCheck &= CheckInPos(sm.MotorPos_1->EL_Convryor.Load_Z);
			if (bCheck) {
				seqStep = S_ELCV_SEQ_PAPER_LOAD_RECV_ABLE;
			}
			else {
				if (GetMoveTimeout(sm.MotorPos_1->EL_Convryor.Load_Z)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_BOX_LIFT_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, sm.MotorPos_1->EL_Convryor.Load_Z, _pCmd);
					}
					seqStep = S_ELCV_SEQ_PAPER_LOAD_Z_UP_ABNORMAL;
				}
			}
			break;
		}
		case S_ELCV_SEQ_PAPER_LOAD_RECV_ABLE: {
			pThis.ToUpper.fReceive_Able = TRUE;
			seqStep = S_ELCV_SEQ_PAPER_LOAD_SEND_ABLE_CHECK;
			break;
		}
		case S_ELCV_SEQ_PAPER_LOAD_SEND_ABLE_CHECK: {
			// 2024.06.24 LBH - Abnormal Case (PPBox UI에서 Count 를 0으로 변경 했을 때.
			// 2024.07.12 EDIT BY KKW, nCrtCount <= 0 --> nCrtCount <= 1 로 수정
			if ((!pUpperTR.Glass.bMaterial) && (pUpperCV.Glass.bMaterial) && (pUpperCV.Status.nCrtCount <= 1) && (pUpperCV.Status.wTargetIndex == pThis.eModule)) {
				// 자재가 없는 경우 Load Conveyor에 있는 PPBOX를 받아야 하기 때문에 배출.
				seqStep = S_ELCV_SEQ_PPBOX_CHECK;
				break;
			}

			bCheck = pUpperTR.Status.wTargetIndex == pThis.eModule;
			bCheck &= pUpperTR.ToLower.fSend_Able;

			if (bCheck)
				seqStep = S_ELCV_SEQ_PAPER_LOAD_RECV_START;
			break;
		}
		case S_ELCV_SEQ_PAPER_LOAD_RECV_START: {
			pThis.ToUpper.fReceive_Start = TRUE;
			seqStep = S_ELCV_SEQ_PAPER_LOAD_SEND_START_CHECK;
			break;
		}
		case S_ELCV_SEQ_PAPER_LOAD_SEND_START_CHECK: {
			if (pUpperTR.ToLower.fSend_Start) {
				////<< MCC
				//MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_IN_WAIT_TIME_PAPER] = 0;
				////>>
				seqStep = S_ELCV_SEQ_PAPER_LOAD_SEND_COMPLETED_CHECK;
			}
			break;
		}
		case S_ELCV_SEQ_PAPER_LOAD_SEND_COMPLETED_CHECK: {
			if (pUpperTR.ToLower.fSend_Complete) {
				// Paper은 따로 수량 및 자재 관리 안함. 자재정보 Copy 할 필요 없음.
				seqStep = S_ELCV_SEQ_PAPER_LOAD_RECV_COMPLETED;
			}

			break;
		}
		case S_ELCV_SEQ_PAPER_LOAD_RECV_COMPLETED: {
			pThis.ToUpper.fReceive_Complete = TRUE;
			seqStep = S_ELCV_SEQ_PAPER_LOAD_SEND_INIT_CHECK;
			break;
		}
		case S_ELCV_SEQ_PAPER_LOAD_SEND_INIT_CHECK: {
			if (IsInitToLower(&pUpperTR))
				seqStep = S_ELCV_SEQ_PAPER_LOAD_RECV_INIT;
			break;
		}
		case S_ELCV_SEQ_PAPER_LOAD_RECV_INIT: {
			SetToUpperInit(&pThis);

			seqStep = S_ELCV_SEQ_PAPER_LOAD_END;
			break;
		}
		case S_ELCV_SEQ_PAPER_LOAD_END: {
			pThis.Status.wTargetIndex = eMODULE_NONE;
			seqStep = S_ELCV_SEQ_INIT;
			break;
		}

		case S_ELCV_SEQ_PPBOX_UNLOAD_START: {
			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;

			seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_Z_DOWN;
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_Z_DOWN: {
			//<< MCC
			MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_MOVE_Z_ULD_POS] = 1;
			//>>

			PTPMove(sm.MotorPos_1->EL_Convryor.Unload_Z);
			seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_Z_DOWN_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_Z_DOWN_CHECK: {
			bCheck &= CheckInPos(sm.MotorPos_1->EL_Convryor.Unload_Z);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_MOVE_Z_ULD_POS] = 0;
				//>>

				seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_SEND_ABLE;
			}
			else {
				if (GetMoveTimeout(sm.MotorPos_1->EL_Convryor.Unload_Z)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_BOX_LIFT_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, sm.MotorPos_1->EL_Convryor.Unload_Z, _pCmd);
					}
					seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_Z_DOWN;
				}
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_SEND_ABLE: {
			//<< MCC
			MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_OUT_WAIT_TIME] = 1;
			//>>

			pThis.ToLower.fSend_Able = TRUE;
			seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_RECV_ABLE_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_RECV_ABLE_CHECK: {
			if (pLower.ToUpper.fReceive_Able)
				seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_RECV_START_CHECK;

			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_RECV_START_CHECK: {
			if (pLower.ToUpper.fReceive_Start) {
				//<< MCC
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_OUT_WAIT_TIME] = 0;
				//>>

				seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_SEND_START;
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_SEND_START: {
			wRetry = 0;
			pThis.ToLower.fSend_Start = TRUE;
			seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_STOPPER_DOWN;
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_STOPPER_DOWN: {
			SetTimeout(&pThis);
			SetConveyor_Stoper(pThis.eModule, FALSE);
			seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_STOPPER_DOWN_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_STOPPER_DOWN_CHECK: {
			if (GetConveyor_Stoper(pThis.eModule, FALSE, TRUE)) {
				wRetry = 0;
				seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_CONVEYOR_BWD;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_3_STOPER_DOWN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_STOPPER_DOWN;
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_CONVEYOR_BWD: {
			//<< MCC
			CELL_INFO_TO_MCC(pThis.Glass, MCC_LAYER_LO01);
			MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_COMPONENT_OUT_PPBOX] = 1;
			MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_COMPONENT_IN_PPBOX] = 1;
			MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_PUT_CV_RUN] = 1;
			//>>

			Serial::Inverter::SetInverterRun(pThis.eModule, !Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_EL].dHighSpeed, _pCmd);
			seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_CONVEYOR_BWD_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_CONVEYOR_BWD_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, !Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_EL].dHighSpeed);

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_RECV_COMPLETED_CHECK;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_3_RUN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_CONVEYOR_BWD;
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_RECV_COMPLETED_CHECK: {
			if (pLower.ToUpper.fReceive_Complete) {
				wRetry = 0;
				seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP;
			}

			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP: {
			Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterStop(pThis.eModule);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_COMPONENT_OUT_PPBOX] = 0;
				MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_COMPONENT_IN_PPBOX] = 0;
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_PUT_CV_RUN] = 0;
				//>>

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_SEND_COMPLETED;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_3_STOP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_SEND_COMPLETED: {
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[2], 0, "");

			pThis.ToLower.fSend_Complete = TRUE;
			seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_RECV_INIT_CHECK;
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_RECV_INIT_CHECK: {
			if (IsInitToUpper(&pLower))
				seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_SEND_INIT;
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_SEND_INIT: {
			SetToLowerInit(&pThis);
			seqStep = S_ELCV_SEQ_PPBOX_UNLOAD_SEQ_END;
			break;
		}
		case S_ELCV_SEQ_PPBOX_UNLOAD_SEQ_END: {
			pThis.Status.wTargetIndex = eMODULE_NONE;
			seqStep = S_ELCV_SEQ_END;
			break;
		}
		case S_ELCV_SEQ_END: {
			seqStep = S_ELCV_SEQ_INIT;
			break;
		}
	}
}


void UnloadConveyor1(PIPCOMMAND _pCmd)
{
	T_SEQUENCE_MODULE_INFO& pUpper = sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR];
	T_SEQUENCE_MODULE_INFO& pThis = sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_1];
	T_SEQUENCE_MODULE_INFO& pLower = sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2];

	WORD& seqStep = pThis.Status.wSeqStep;
	WORD& seqPrevStep = pThis.Status.wSeqPrevStep;

	// log용 설명이 필요하면 description에 넣으면 됨
	// 구분자로 세미콜론(;)을 사용하니 description에는 세미콜론 사용 금지
	CString description = _T("");
	auto cleanup = make_scope_exit
	(
		[&]()
		{
			// 스탭이 기존과 바뀌거나 추가 내용이 있으면 로그로 남긴다
			if (seqStep != seqPrevStep || !description.IsEmpty())
			{
				WriteSeqLog(pThis.eModule, seqPrevStep, seqStep, description);
				seqPrevStep = seqStep;
			}
		}
	);

	T_MCC_LAYER* MCC_LAYER_LO01 = &sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO01];
	T_MCC_LAYER* MCC_LAYER_LO02 = &sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO02];

	// 2022.09.15 MoveFail 알람 상세용
	CString StepName(magic_enum::enum_name(static_cast<eUNLOAD_CONVEYOR_1>(seqStep)).data(), magic_enum::enum_name(static_cast<eUNLOAD_CONVEYOR_1>(seqStep)).size());

	BOOL bCheck = TRUE;
	WORD& wRetry = pThis.Status.wRetryCount;
	WORD& wRepeat = pThis.Status.wRepeatCount;

	auto Conveyor = sm.Conveyor_Parameter[eCONVEYOR_ULD_1];

	switch (seqStep) {
		case S_ULDCV1_SEQ_INIT: {
			//<< MCC
			INFO_INIT_MCC(MCC_LAYER_LO01);
			//>>

			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;
			pThis.Status.runningStartTime = 0;
			seqStep = S_ULDCV1_SEQ_PPBOX_CHECK;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_CHECK: {
			bCheck = (sm.DeviceParameter->tUse.bDryRun) ? pThis.Glass.bMaterial : GetPPBoxSensor(pThis.eModule, TRUE);

			if (bCheck) {
				seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_START;
			}
			else {
				if (!pThis.Status.bInputStop) {
					seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_START;
				}
			}
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_START: {
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[3], 0, "");
			//<< MCC
			MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_INIT_ORIGIN] = 1;
			//>>

			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;
			seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_STOPPER_UP;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_STOPPER_UP: {
			//<< MCC
			MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_CYL_STOPPER_UP] = 1;
			//>>

			SetTimeout(&pThis);
			SetConveyor_Stoper(pThis.eModule, TRUE);
			seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_STOPPER_UP_CHECK;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_STOPPER_UP_CHECK: {
			if (GetConveyor_Stoper(pThis.eModule, TRUE, TRUE)) {
				//<< MCC
				MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_INIT_ORIGIN] = 0;
				MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_CYL_STOPPER_UP] = 0;
				//>>

				wRetry = 0;
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_RECV_ABLE;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					//SetAlarm();
				}
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_STOPPER_UP;
			}
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_RECV_ABLE: {
			//<< MCC
			MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_IN_WAIT_TIME] = 1;
			//>>

			pThis.ToUpper.fReceive_Able = TRUE;
			seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_SEND_ABLE_CHECK;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_SEND_ABLE_CHECK: {
			if (pUpper.ToLower.fSend_Able) {
				//<< MCC
				MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_IN_WAIT_TIME] = 0;
				//>>

				wRetry = 0;
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_BWD;
			}
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_BWD: {
			//<< MCC
			MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_GET_CV_RUN] = 1;
			//>>

			Serial::Inverter::SetInverterRun(pThis.eModule, !Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_1].dHighSpeed, _pCmd);
			seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_BWD_CHECK;
			break;
		}

		case S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_BWD_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, !Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_1].dHighSpeed);

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_RECV_START;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_4_RUN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_BWD;
			}
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_RECV_START: {
			pThis.ToUpper.fReceive_Start = TRUE;
			seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_SEND_START_CHECK;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_SEND_START_CHECK: {
			if (pUpper.ToLower.fSend_Start)
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_PPBOX_CHECK;

			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_PPBOX_CHECK: {
			if (GetPPBoxSensor(pThis.eModule, TRUE)) {
				wRetry = 0;
				SetTimeout(&pThis);
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED;
			}
			else {
				if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorH)) {
					Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
					MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_4_SENSOR_CHECK, IDS_DES_NOT_USE, StepName, _pCmd);
					seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_BWD;
					break;
				}
			}
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED: {
			if (Serial::Inverter::GetInverterRun(pThis.eModule))
				Serial::Inverter::SetInverterFrequency(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_1].dLowSpeed);
			else
				Serial::Inverter::SetInverterRun(pThis.eModule, !Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_1].dLowSpeed, _pCmd);

			seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED_CHECK;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, !Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_1].dLowSpeed);

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_STOP_DELAY;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_4_LOW_SPEED_SET_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED;
			}
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_STOP_DELAY: {
			if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorMove)) {
				wRetry = 0;
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_STOP: {
			Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_STOP_CHECK;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_STOP_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterStop(pThis.eModule);

			if (bCheck) {
				//<< MCC
				MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_GET_CV_RUN] = 0;
				//>>

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_RECV_COMPLETED;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_4_STOP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_RECV_COMPLETED: {
			pThis.ToUpper.fReceive_Complete = TRUE;
			seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_SEND_COMPLETED_CHECK;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_SEND_COMPLETED_CHECK: {
			if (pUpper.ToLower.fSend_Complete)
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_DATA_COPY;

			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_DATA_COPY: {
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[3], 2, GetTimeStringYMDHMS());

			MovePPBoxData(pUpper, pThis);
			seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_RECV_INIT;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_RECV_INIT: {
			SetToUpperInit(&pThis);
			seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_SEND_INIT_CHECK;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_SEND_INIT_CHECK: {
			if (IsInitToLower(&pUpper))
				seqStep = S_ULDCV1_SEQ_PPBOX_LOAD_END;

			break;
		}
		case S_ULDCV1_SEQ_PPBOX_LOAD_END: {
			seqStep = S_ULDCV1_SEQ_INIT;
			break;
		}

		case S_ULDCV1_SEQ_PPBOX_UNLOAD_START: {
			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;
			seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_SEND_ABLE;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_SEND_ABLE: {
			//<< MCC
			MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_OUT_WAIT_TIME] = 1;
			//>>

			pThis.ToLower.fSend_Able = TRUE;
			seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_RECV_ABLE_CHECK;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_RECV_ABLE_CHECK: {
			if (pLower.ToUpper.fReceive_Able)
				seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_RECV_START_CHECK;

			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_RECV_START_CHECK: {
			if (pLower.ToUpper.fReceive_Start) {
				//<< MCC
				MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_OUT_WAIT_TIME] = 0;
				MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_COMPONENT_OUT_PPBOX] = 1;
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_COMPONENT_IN_PPBOX] = 1;
				//>>

				seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_SEND_START;
			}

			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_SEND_START: {
			pThis.ToLower.fSend_Start = TRUE;
			wRetry = 0;
			seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_STOPPER_DOWN;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_STOPPER_DOWN: {
			//<< MCC
			MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_CYL_STOPPER_DOWN] = 1;
			//>>

			SetTimeout(&pThis);
			SetConveyor_Stoper(pThis.eModule, FALSE);
			seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_STOPPER_DOWN_CHECK;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_STOPPER_DOWN_CHECK: {
			if (GetConveyor_Stoper(pThis.eModule, FALSE, TRUE)) {
				//<< MCC
				MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_CYL_STOPPER_DOWN] = 0;
				//>>

				wRetry = 0;
				seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_BWD;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_4_STOPER_DOWN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_STOPPER_DOWN;
			}
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_BWD: {
			//<< MCC
			CELL_INFO_TO_MCC(pThis.Glass, MCC_LAYER_LO02);
			//MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_COMPONENT_OUT_PPBOX] = 1;
			//MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_COMPONENT_IN_PPBOX] = 1;
			MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_PUT_CV_RUN] = 1;
			//>>

			Serial::Inverter::SetInverterRun(pThis.eModule, !Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_1].dHighSpeed, _pCmd);
			seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_BWD_CHECK;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_BWD_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, !Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_1].dHighSpeed);

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_RECV_COMPLETED_CHECK;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_4_RUN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_BWD;
			}
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_RECV_COMPLETED_CHECK: {
			// ULDCV2 에서 센서확인 및 일정 Delay 후 Complete 주는것을 먼저 확인 해야됨.
			if (pLower.ToUpper.fReceive_Complete) {
				wRetry = 0;
				seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP;
			}

			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP: {
			Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP_CHECK;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterStop(pThis.eModule);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_COMPONENT_OUT_PPBOX] = 0;
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_COMPONENT_IN_PPBOX] = 0;
				MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_PUT_CV_RUN] = 0;
				//>>

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_SEND_COMPLETED;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO01->A[eMCC_ACTION_LD_LO_01_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_4_STOP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_SEND_COMPLETED: {
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[3], 0, "");

			pThis.ToLower.fSend_Complete = TRUE;
			seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_RECV_INIT_CHECK;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_RECV_INIT_CHECK: {
			//-->24.03.23 KGH EDIT : PIO 오매칭 수정
			if (IsInitToUpper(&pLower))
			//if (IsInitToUpper(&pUpper))
			//<--24.03.23 KGH EDIT : PIO 오매칭 수정
					seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_SEND_INIT;
			
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_SEND_INIT: {
			SetToLowerInit(&pThis);
			seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_END;
			break;
		}
		case S_ULDCV1_SEQ_PPBOX_UNLOAD_END: {
			pThis.Status.wTargetIndex = eMODULE_NONE;
			seqStep = S_ULDCV1_SEQ_END;
			break;
		}
		case S_ULDCV1_SEQ_END: {
			seqStep = S_ULDCV1_SEQ_INIT;
			break;
		}
	}
}


void UnloadConveyor2(PIPCOMMAND _pCmd)
{
	T_SEQUENCE_MODULE_INFO& pUpper = sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_1];
	T_SEQUENCE_MODULE_INFO& pThis = sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2];

	WORD& seqStep = pThis.Status.wSeqStep;
	WORD& seqPrevStep = pThis.Status.wSeqPrevStep;

	// log용 설명이 필요하면 description에 넣으면 됨
	// 구분자로 세미콜론(;)을 사용하니 description에는 세미콜론 사용 금지
	CString description = _T("");
	auto cleanup = make_scope_exit
	(
		[&]()
		{
			// 스탭이 기존과 바뀌거나 추가 내용이 있으면 로그로 남긴다
			if (seqStep != seqPrevStep || !description.IsEmpty())
			{
				WriteSeqLog(pThis.eModule, seqPrevStep, seqStep, description);
				seqPrevStep = seqStep;
			}
		}
	);

	T_MCC_LAYER* MCC_LAYER_LO02 = &sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO02];

	// 2022.09.15 MoveFail 알람 상세용
	CString StepName(magic_enum::enum_name(static_cast<eUNLOAD_CONVEYOR_2>(seqStep)).data(), magic_enum::enum_name(static_cast<eUNLOAD_CONVEYOR_2>(seqStep)).size());

	BOOL bCheck = TRUE;
	WORD& wRetry = pThis.Status.wRetryCount;
	WORD& wRepeat = pThis.Status.wRepeatCount;

	BOOL bUseAGV = sm.Option->tSequence.bUseAGV;
	if (!sm.Sequence->bCIMSkip) {
		bUseAGV = (strcmp(sm.Sequence->CIMData.EQPFunction[eEQP_SCT].Item[eEFID_AGV_ACCESS_MODE].szEFST, lpEFST[eEFST_AUTO]) == 0);
	}
	auto Conveyor = sm.Conveyor_Parameter[eCONVEYOR_ULD_2];

	BOOL bConnect	= GetPIO_BF_CONNECT(TRUE);
	BOOL bRequest	= GetPIO_BF_REQUEST(TRUE);
	BOOL bRun		= GetPIO_BF_RUN(TRUE);
	BOOL bError		= GetPIO_BF_ERROR(TRUE);
	BOOL bInput		= GetPIO_BF_INPUT(TRUE);
	BOOL bOutput	= GetPIO_BF_OUTPUT(TRUE);
	BOOL bLoad		= GetPIO_BF_LOAD(TRUE);
	BOOL bUnload	= GetPIO_BF_UNLOAD(TRUE);
	MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_X_CONNECTION] = bConnect;
	MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_X_REQUEST] = bRequest;
	MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_X_RUN] = bRun;
	MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_X_ERROR] = bError;
	MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_X_POSITION_INPUT] = bInput;
	MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_X_POSITION_OUTPUT] = bOutput;
	MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_X_LOAD_REQUEST] = bLoad;
	MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_X_UNLOAD_REQUEST] = bUnload;
	
	BOOL bConveyorAlarm = GetConveyorAlarm(pThis.eModule);
	switch (seqStep) {
		case S_ULDCV2_SEQ_INIT: {
			//<< MCC
			INFO_INIT_MCC(MCC_LAYER_LO02);
			//>>

			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;
			pThis.Status.runningStartTime = 0;
			sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_GREEN_LAMP = FALSE;
			seqStep = S_ULDCV2_SEQ_PPBOX_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_CHECK: {
			bCheck = (sm.DeviceParameter->tUse.bDryRun) ? pThis.Glass.bMaterial : GetPPBoxSensor(pThis.eModule, TRUE);
			if (bCheck) {
				BOOL bCheckUnloadingSeq;
				bCheckUnloadingSeq = (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_1].Status.wSeqStep < S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_CONNECTION_ON);
				bCheckUnloadingSeq |= (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_1].Status.wSeqStep > S_LDCV1_SEQ_AUTO_PPBOX_LOAD_END);

				if (bCheckUnloadingSeq && sm.Option->tSequence.fOUT) {
					pThis.Glass.bMaterial = TRUE;
					seqStep = S_ULDCV2_SEQ_PPBOX_UNLOAD_START; // Unlaod
				}
			}
			else {
				if (!pThis.Status.bInputStop) {
					seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_START;
				}
			}
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_START: {
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[4], 0, "");

			//<< MCC
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_INIT_ORIGIN] = 1;
			//>>

			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;
			wRetry = 0;
			seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_STOPPER_UP;
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_STOPPER_UP: {
			//<< MCC
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_CYL_STOPPER_UP] = 1;
			//>>

			SetTimeout(&pThis);
			SetConveyor_Stoper(pThis.eModule, TRUE);
			seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_STOPPER_UP_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_STOPPER_UP_CHECK: {
			if (GetConveyor_Stoper(pThis.eModule, TRUE, TRUE)) {
				//<< MCC
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_INIT_ORIGIN] = 0;
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_CYL_STOPPER_UP] = 0;
				//>>

				wRetry = 0;
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_RECV_ABLE;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_5_STOPER_UP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_STOPPER_UP;
			}
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_RECV_ABLE: {
			//<< MCC
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_IN_WAIT_TIME] = 1;
			//>>

			pThis.ToUpper.fReceive_Able = TRUE;
			seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_SEND_ABLE_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_SEND_ABLE_CHECK: {
			if (pUpper.ToLower.fSend_Able) {
				//<< MCC
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_IN_WAIT_TIME] = 0;
				//>>

				wRetry = 0;
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_CONVEYOR_BWD;
			}

			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_CONVEYOR_BWD: {
			//<< MCC
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_GET_CV_RUN] = 1;
			//>>

			Serial::Inverter::SetInverterRun(pThis.eModule, !Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_2].dHighSpeed, _pCmd);
			seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_CONVEYOR_BWD_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_CONVEYOR_BWD_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, !Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_2].dHighSpeed);

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_RECV_START;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_5_RUN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_CONVEYOR_BWD;
			}
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_RECV_START: {
			pThis.ToUpper.fReceive_Start = TRUE;
			seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_SEND_START_CHECK;

			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_SEND_START_CHECK: {
			if (pUpper.ToLower.fSend_Start)
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_PPBOX_CHECK;

			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_PPBOX_CHECK: {
			if (GetPPBoxSensor(pThis.eModule, TRUE)) { // Detect Sensor
				wRetry = 0;
				SetTimeout(&pThis);
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED;
			}
			else {
				if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorH)) {
					Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_5_SENSOR_CHECK, IDS_DES_NOT_USE, StepName, _pCmd);
					seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_CONVEYOR_BWD;
					break;
				}
			}
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED: {
			if (Serial::Inverter::GetInverterRun(pThis.eModule))
				Serial::Inverter::SetInverterFrequency(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_2].dLowSpeed);
			else
				Serial::Inverter::SetInverterRun(pThis.eModule, !Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_2].dLowSpeed, _pCmd);
			seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED_CHECK;

			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, !Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_2].dLowSpeed);

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_PPBOX_STOP_DELAY;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_5_LOW_SPEED_SET_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_CONVEYOR_LOW_SPEED;
			}
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_PPBOX_STOP_DELAY: {
			if (GetTimeoutSec(&pThis, sm.DeviceParameter->tTime.dConveyorMove)) {
				wRetry = 0;
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_PPBOX_STOP;
			}
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_PPBOX_STOP: {
			Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_PPBOX_STOP_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_PPBOX_STOP_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterStop(pThis.eModule);

			if (bCheck) {
				//<< MCC
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_GET_CV_RUN] = 0;
				//>>

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_RECV_COMPLETED;
				break;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_5_STOP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_PPBOX_STOP;
			}
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_RECV_COMPLETED: {
			pThis.ToUpper.fReceive_Complete = TRUE;
			seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_SEND_COMPLETED_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_SEND_COMPLETED_CHECK: {
			if (pUpper.ToLower.fSend_Complete)
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_DATA_COPY;

			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_DATA_COPY: {
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[4], 2, GetTimeStringYMDHMS());

			MovePPBoxData(pUpper, pThis);
			seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_RECV_INIT;
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_RECV_INIT: {
			SetToUpperInit(&pThis);
			seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_SEND_INIT_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_SEND_INIT_CHECK: {
			if (IsInitToLower(&pUpper))
				seqStep = S_ULDCV2_SEQ_PPBOX_LOAD_END;

			break;
		}
		case S_ULDCV2_SEQ_PPBOX_LOAD_END: {
			seqStep = S_ULDCV2_SEQ_INIT;
			break;
		}

		case S_ULDCV2_SEQ_PPBOX_UNLOAD_START: {
			//<< MCC
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_OUT_WAIT_TIME] = 1;
			//>>

			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID_352_CASSETTE_UNLOAD_REQUEST;
			break;
		}

		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID_352_CASSETTE_UNLOAD_REQUEST: {
			if (sm.Sequence->bCIMSkip) {
				pThis.Status.eReportStatus = eCIM_COMPLETE;
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID_352_CASSETTE_UNLOAD_REQUESTCHECK;
				break;
			}
			//-->24.07.20 KGH ADD : AGV 미사용모드일 때도 보고 하지 않는다.
			else if(!bUseAGV){
				pThis.Status.eReportStatus = eCIM_COMPLETE;
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID_352_CASSETTE_UNLOAD_REQUESTCHECK;
				break;
			}
			//<--24.07.20 KGH ADD : AGV 미사용모드일 때도 보고 하지 않는다.

			// 2024.04.29 LBH - AGV 호출
			sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_GREEN_LAMP = TRUE;

			// CIM Report
			S6F11_CASSETTE_UNLOAD_REQUEST(pThis.eModule, ePPBOX_LO_01, _pCmd);
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID_352_CASSETTE_UNLOAD_REQUESTCHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID_352_CASSETTE_UNLOAD_REQUESTCHECK: {
			bCheck &= pThis.Status.eReportStatus == eCIM_COMPLETE;

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eReportStatus = eCIM_IDLE;
				if (bUseAGV)
					seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
				else
					seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_START;

			}
			else if ((pThis.Status.eReportStatus == eCIM_ERROR) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_ULDCV2_SEQ_PIO_S6F11_CEID_352_CASSETTE_UNLOAD_REQUESTCHECK_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID_352_CASSETTE_UNLOAD_REQUEST;
			}
			break;
		}

		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START: {
			BOOL bCheckUnloadingSeq;
			bCheckUnloadingSeq = (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_1].Status.wSeqStep < S_LDCV1_SEQ_AUTO_PPBOX_LOAD_PIO_EQ_CONNECTION_ON);
			bCheckUnloadingSeq |= (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_1].Status.wSeqStep > S_LDCV1_SEQ_AUTO_PPBOX_LOAD_END);
			
			bCheck &= bCheckUnloadingSeq;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_INIT;
			}
			else if (!bCheckUnloadingSeq) {	// 로드 컨베이어 PIO 시작했으면 대기 스탭으로 이동
				seqStep = S_ULDCV2_SEQ_PPBOX_CHECK;
			}
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_INIT: {
			SetPIO_EQ_ALL_OFF();
			SetTimeout(&pThis);
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_INIT_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_INIT_CHECK: {
			if (GetPIO_EQ_ALL_OFF()) {
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_UNLOAD_ON;
			}
			else if (GetTimeout(&pThis, PIO_TIMEOUT)) {
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}

			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_UNLOAD_ON: {
			//<< MCC
			MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_UNLOAD_REQUEST] = 1;
			//>>

			SetPIO_EQ_UNLOAD_REQUEST(TRUE);
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_UNLOAD_ON_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_UNLOAD_ON_CHECK: {
			bCheck &= GetPIO_EQ_UNLOAD_REQUEST(TRUE);
			if (bCheck)
				//24.07.29 KGH ADD : MUNTING ON TIMING CHAGE TO AFTER PIO START
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_BF_CONNECT_LOAD_ON_CHECK;
				//seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON;

			break;
		}
		//-->24.07.29 KGH DEL : MUNTING ON TIMING CHAGE TO AFTER PIO START
		//case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON: {
		//	//<< MCC
		//	MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_OUT_WAIT_TIME] = 0;
		//	MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_COMPONENT_OUT_PPBOX] = 1;
		//	MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_PPBOX_ULD_MUTE_ON] = 1;
		//	//>>
		//	SetMute(pThis.eModule, TRUE, _pCmd);
		//	seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON_CHECK;
		//	break;
		//}
		//case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON_CHECK: {
		//	bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
		//	bCheck &= GetMute(TRUE);
		//	if (bCheck) {
		//		wRetry = 0;
		//		pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
		//		SetTimeout(&pThis);
		//		seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_BF_CONNECT_LOAD_ON_CHECK;
		//	}
		//	else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
		//		if (++wRetry > IO_RETRY) {
		//			wRetry = 0;
		//			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
		//			SetAlarm(eAI_MUTE_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
		//		}
		//		seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON;
		//	}
		//	break;
		//}
		//<--24.07.29 KGH DEL : MUNTING ON TIMING CHAGE TO AFTER PIO START
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_BF_CONNECT_LOAD_ON_CHECK: {
			BOOL bCheckUnloadingSeq =TRUE;
			//-->24.07.29 KGH EDIT : INCORRECT MODULE, SEQUENCE FIX
			bCheckUnloadingSeq &= ((sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep < S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_CONNECT_ON));
			bCheckUnloadingSeq |= ((sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep > S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_END));
			//<--24.07.29 KGH EDIT : INCORRECT MODULE, SEQUENCE FIX

			bCheck &= bCheckUnloadingSeq;
			bCheck &= bConnect;
			bCheck &= bLoad;

			bCheck &= !bRequest;
			bCheck &= !bRun;
			bCheck &= !bError;
			bCheck &= !bInput;
			bCheck &= !bOutput;
			bCheck &= !bUnload;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				//24.07.29 KGH ADD : MUNTING ON TIMING CHAGE TO AFTER PIO START
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON;
				//seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_CONNECT_ON;
			}
			else {
				if (!bUseAGV) {
					//<< MCC
					MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
					MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
					MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
					MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_ERROR] = 0;
					MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_INPUT] = 0;
					MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
					MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_LOAD_REQUEST] = 0;
					MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_UNLOAD_REQUEST] = 0;
					//>>

					SetPIO_EQ_ALL_OFF();
					seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_START;
				}
				else if (!bCheckUnloadingSeq) {	// Load 쪽이 AGV와 통신 중이라면 대기스탭으로 복귀
					//<< MCC
					MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_UNLOAD_REQUEST] = 0;
					//>>

					SetPIO_EQ_UNLOAD_REQUEST(FALSE);
					seqStep = S_ULDCV2_SEQ_PPBOX_CHECK;
				}
				else if (!sm.Option->tSequence.fOUT) {	// AGV 대기중 배출정지 걸렸을 때
					//<< MCC
					MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_UNLOAD_REQUEST] = 0;
					//>>

					SetPIO_EQ_UNLOAD_REQUEST(FALSE);
					seqStep = S_ULDCV2_SEQ_PPBOX_CHECK;
				}
				else if (bConveyorAlarm) {
					seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
				}
				else if (GetPIO_EQ_UNLOAD_REQUEST(FALSE) && GetTimeout(&pThis, PIO_TIMEOUT)) {
					seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_INIT;
				}
				else if (GetTimeout(&pThis, PIO_REQUEST_WAIT_TIME)) {	// PIO_REQUEST_WAIT_TIME 동안 대기했는데도 AGV 안오면 다시 Unload Request 요청.
					seqStep = S_ULDCV2_SEQ_PPBOX_CHECK;
				}
			}

			break;
		}
		//-->24.07.29 KGH ADD : MUNTING ON TIMING CHAGE TO AFTER PIO START
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON: {
				//<< MCC
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_OUT_WAIT_TIME] = 0;
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_COMPONENT_OUT_PPBOX] = 1;
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_PPBOX_ULD_MUTE_ON] = 1;
				//>>

				SetMute(pThis.eModule, TRUE, _pCmd);
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON_CHECK;
				break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= GetMute(TRUE);

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				SetTimeout(&pThis);
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_CONNECT_ON;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_MUTE_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON;
			}
			break;
		}
		//-->24.07.29 KGH ADD : MUNTING ON TIMING CHAGE TO AFTER PIO START
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_CONNECT_ON: {
			//<< MCC
			MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 1;
			//>>

			SetPIO_EQ_CONNECT(TRUE);
			SetTimeout(&pThis);
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_CONNECT_ON_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_CONNECT_ON_CHECK: {
			bCheck &= GetPIO_EQ_CONNECT(TRUE);

			bCheck &= bConnect;
			bCheck &= bLoad;

			bCheck &= !bRequest;
			bCheck &= !bRun;
			bCheck &= !bError;
			bCheck &= !bInput;
			bCheck &= !bOutput;
			bCheck &= !bUnload;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				SetTimeout(&pThis);
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_BF_REQUEST_OUTPUT_ON_CHECK;
			}
			else if (!bConnect || !bLoad) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				if (!bConnect) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_CONNECT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bLoad) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_LOAD_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (bRequest || bRun || bError || bInput || bOutput || bUnload) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				if (bRequest) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_REQUEST_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bRun) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_RUN_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bError) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_ERROR_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bInput) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_INPUT_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bOutput) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_OUTPUT_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bUnload) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_UNLOAD_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (bConveyorAlarm) {
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (GetTimeout(&pThis, PIO_TIMEOUT)) {
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_BF_REQUEST_OUTPUT_ON_CHECK: {
			bCheck &= bConnect;
			bCheck &= bLoad;
			bCheck &= bRequest;
			bCheck &= bOutput;

			bCheck &= !bRun;
			bCheck &= !bError;
			bCheck &= !bInput;
			bCheck &= !bUnload;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_REQUEST_OUTPUT_ON;
			}
			else if (!bConnect || !bLoad) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				if (!bConnect) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_CONNECT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bLoad) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_LOAD_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (bRun || bError || bInput || bUnload || bConveyorAlarm) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				if (bRun) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_RUN_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bError) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_ERROR_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bInput) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_INPUT_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bUnload) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_UNLOAD_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (bConveyorAlarm) {
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (GetTimeout(&pThis, PIO_TIMEOUT)) {
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_REQUEST_OUTPUT_ON: {
			//<< MCC
			MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 1;
			MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 1;
			//>>

			SetPIO_EQ_REQUEST(TRUE);
			SetPIO_EQ_OUTPUT(TRUE);
			SetTimeout(&pThis);
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_REQUEST_OUTPUT_ON_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_REQUEST_OUTPUT_ON_CHECK: {
			bCheck &= GetPIO_EQ_REQUEST(TRUE);
			bCheck &= GetPIO_EQ_OUTPUT(TRUE);
			if (bCheck) {
				SetTimeout(&pThis);
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_BF_RUN_ON_CHECK;
			}
			else if (GetTimeout(&pThis, PIO_TIMEOUT)) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_PIO_TIMEOUT, IDS_DES_NOT_USE, StepName, _pCmd);
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_BF_RUN_ON_CHECK: {
			bCheck &= bConnect;
			bCheck &= bLoad;
			bCheck &= bRequest;
			bCheck &= bOutput;
			bCheck &= bRun;

			bCheck &= !bError;
			bCheck &= !bInput;
			bCheck &= !bUnload;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_RUN_ON;
			}
			else if (!bConnect || !bLoad || !bRequest || !bOutput) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				if (!bConnect) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_CONNECT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bLoad) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_LOAD_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRequest) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_REQUEST_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bOutput) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_OUTPUT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (bError || bInput || bUnload) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				if (bError) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_ERROR_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bInput) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_INPUT_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bUnload) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_UNLOAD_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (bConveyorAlarm) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (GetTimeout(&pThis, PIO_TIMEOUT)) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_PIO_TIMEOUT, IDS_DES_NOT_USE, StepName, _pCmd);
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_RUN_ON: {
			//<< MCC
			MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 1;
			//>>

			SetPIO_EQ_RUN(TRUE);
			SetTimeout(&pThis);
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_RUN_ON_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_RUN_ON_CHECK: {
			bCheck &= GetPIO_EQ_RUN(TRUE);

			if (bCheck) {
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON2;
			}
			else if (GetTimeout(&pThis, PIO_TIMEOUT)) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_PIO_TIMEOUT, IDS_DES_NOT_USE, StepName, _pCmd);
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON2: {
			SetMute(pThis.eModule, TRUE, _pCmd);
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON2_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON2_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= GetMute(TRUE);

			if (bCheck) {
				//<< MCC
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_PPBOX_ULD_MUTE_ON] = 0;
				//>>

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_STOPPER_DOWN;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_MUTE_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_ON2;
			}
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_STOPPER_DOWN: {
			//<< MCC
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_CYL_STOPPER_DOWN] = 1;
			//>>

			SetTimeout(&pThis);
			SetConveyor_Stoper(pThis.eModule, FALSE);
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_STOPPER_DOWN_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_STOPPER_DOWN_CHECK: {
			bCheck &= GetConveyor_Stoper(pThis.eModule, FALSE, TRUE);

			bCheck &= bConnect;
			bCheck &= bLoad;
			bCheck &= bRequest;
			bCheck &= bOutput;
			bCheck &= bRun;

			bCheck &= !bError;
			bCheck &= !bInput;
			bCheck &= !bUnload;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				//<< MCC
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_CYL_STOPPER_DOWN] = 0;
				//>>

				wRetry = 0;
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_CONVEYOR_BWD;
			}
			else if (!bConnect || !bLoad || !bRequest || !bOutput || !bRun) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				if (!bConnect) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_CONNECT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bLoad) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_LOAD_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRequest) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_REQUEST_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bOutput) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_OUTPUT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRun) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_RUN_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (bError || bInput || bUnload) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				if (bError) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_ERROR_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bInput) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_INPUT_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bUnload) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_UNLOAD_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (bConveyorAlarm) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_5_STOPER_DOWN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_STOPPER_DOWN;
			}
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_CONVEYOR_BWD: {
			//<< MCC
			//MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_COMPONENT_OUT_PPBOX] = 1;
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_PUT_CV_RUN] = 1;
			//>>

			Serial::Inverter::SetInverterRun(pThis.eModule, !Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_2].dHighSpeed, _pCmd);
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_CONVEYOR_BWD_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_CONVEYOR_BWD_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, !Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_2].dHighSpeed);

			bCheck &= bConnect;
			bCheck &= bLoad;
			bCheck &= bRequest;
			bCheck &= bOutput;
			bCheck &= bRun;

			bCheck &= !bError;
			bCheck &= !bInput;
			bCheck &= !bUnload;
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				SetTimeout(&pThis);
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_BF_ALL_OFF_PPBOX_OUT_SENSOR_CHECK;
			}
			else if (!bConnect || !bLoad || !bRequest || !bOutput || !bRun) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				if (!bConnect) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_CONNECT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bLoad) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_LOAD_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRequest) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_REQUEST_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bOutput) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_OUTPUT_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (!bRun) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_RUN_ABNORMAL_OFF, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (bError || bInput || bUnload) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				if (bError) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_ERROR_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bInput) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_INPUT_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				if (bUnload) {
					SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_UNLOAD_ABNORMAL_ON, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (bConveyorAlarm) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_5_RUN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_CONVEYOR_BWD;
			}
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_BF_ALL_OFF_PPBOX_OUT_SENSOR_CHECK: {
			bCheck &= GetPIO_BF_ALL_OFF();
			bCheck &= GetPPBoxSensor(pThis.eModule, FALSE);
			bCheck &= !bConveyorAlarm;

			if (bCheck) {
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_ALL_OFF;
			}
			else if (bConveyorAlarm) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			else if (GetTimeout(&pThis, PIO_TIMEOUT)) {
				//<< MCC
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN] = 0;
				MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
				//>>

				Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
				SetPIO_EQ_CONNECT(FALSE);
				SetPIO_EQ_REQUEST(FALSE);
				SetPIO_EQ_RUN(FALSE);
				SetPIO_EQ_OUTPUT(FALSE);
				SetAlarm(eAI_UNLOAD_CONVEYOR2_AGV_PIO_TIMEOUT, IDS_DES_NOT_USE, StepName, _pCmd);
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_START;
			}
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_ALL_OFF: {
			//<< MCC
			MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_CONNECTION		] = 0;
			MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_REQUEST		] = 0;
			MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_RUN			] = 0;
			MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_ERROR			] = 0;
			MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_INPUT	] = 0;
			MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_POSITION_OUTPUT] = 0;
			MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_LOAD_REQUEST	] = 0;
			MCC_LAYER_LO02->S[eMCC_SIGNAL_LD_L0_02_Y_UNLOAD_REQUEST	] = 0;
			//>>

			SetPIO_EQ_ALL_OFF();
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_ALL_OFF_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_PIO_EQ_ALL_OFF_CHECK: {
			bCheck &= GetPIO_EQ_ALL_OFF();
			if (bCheck)
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_CONVEYOR_STOP;

			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_CONVEYOR_STOP: {
			Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_CONVEYOR_STOP_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_CONVEYOR_STOP_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterStop(pThis.eModule);

			if (bCheck) {
				//<< MCC
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_COMPONENT_OUT_PPBOX] = 0;
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_PUT_CV_RUN] = 0;
				//>>
				
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;

				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_OFF;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_5_STOP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_OFF: {
			//<< MCC
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_PPBOX_ULD_MUTE_OFF] = 1;
			//>>

			SetMute(pThis.eModule, FALSE, _pCmd);
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_OFF_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_OFF_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= GetMute(FALSE);

			if (bCheck) {
				//<< MCC
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_PPBOX_ULD_MUTE_OFF] = 0;
				//>>

				// Dry Run, UseAGV와 상관없이 자재정보 지워야 함. (DB에서 지우는 기능 필요 할 수도?)
				//if (sm.DeviceParameter->tUse.bDryRun || !sm.Option->tSequence.bUseAGV)
				pThis.Glass.bMaterial = FALSE;

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_DELETE_PPBOX_TO_DB;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_MUTE_OFF_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_MUTE_OFF;
			}
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_DELETE_PPBOX_TO_DB: {
			// ID 확인 해봐야 함.
			DELETE_PPBOX_LOT_DATA(pThis.eModule, pThis.Glass.szCELL_ID_BCR, _pCmd);
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_DELETE_PPBOX_TO_DB_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_DELETE_PPBOX_TO_DB_CHECK: {
			bCheck &= pThis.Status.eReportStatus == eCIM_COMPLETE;

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eReportStatus = eCIM_IDLE;
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_END;
			}
			else if ((pThis.Status.eReportStatus == eCIM_ERROR) || (GetTimeout(&pThis, POST_MESSAGE_TIME_OUT))) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					//SetAlarm(eAI_LDCV2_SEQ_PPBOX_LOAD_RECV_LOT_DATA_CHECK_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}

				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_DELETE_PPBOX_TO_DB;
			}
			break;
		}

		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_END: {
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID353_CASSETTE_UNLOAD_COMPLETE;
			break;
		}



		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_START: {
			seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MANUAL_OUT;
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MANUAL_OUT: {
			if (bUseAGV) {
				seqStep = S_ULDCV2_SEQ_INIT;
			}
			else {
				if (GetPPBoxManualOutStart()) {
					SetTimeout(&pThis);
					seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MANUAL_OUT_CHECK;
				}
			}
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MANUAL_OUT_CHECK: {
			if (bUseAGV) {
				seqStep = S_ULDCV2_SEQ_INIT;
			}
			else {
				if (GetPPBoxManualOutStart()) {
					if (GetTimeout(&pThis, MANUAL_BUTTON_PUSH_TIME)) {
						wRetry = 0;
						seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MUTE_ON;
					}
				}
				else {
					seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MANUAL_OUT;
				}

			}
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MUTE_ON: {
			//<< MCC
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_OUT_WAIT_TIME] = 0;
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_COMPONENT_OUT_PPBOX] = 1;
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_PPBOX_ULD_MUTE_ON] = 1;
			//>>

			SetMute(pThis.eModule, TRUE, _pCmd);
			seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MUTE_ON_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MUTE_ON_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= GetMute(TRUE);

			if (bCheck) {
				//<< MCC
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_PPBOX_ULD_MUTE_ON] = 0;
				//>>

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;

				seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_STOPPER_DOWN;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_MUTE_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MUTE_ON;
			}
			break;
		}

		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_STOPPER_DOWN: {
			//<< MCC
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_CYL_STOPPER_DOWN] = 1;
			//>>

			SetTimeout(&pThis);
			SetConveyor_Stoper(pThis.eModule, FALSE);
			seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_STOPPER_DOWN_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_STOPPER_DOWN_CHECK: {
			if (GetConveyor_Stoper(pThis.eModule, FALSE, TRUE)) {
				//<< MCC
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_CYL_STOPPER_DOWN] = 0;
				//>>

				wRetry = 0;
				seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_CONVEYOR_BWD;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_5_STOPER_DOWN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_STOPPER_DOWN;
			}
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_CONVEYOR_BWD: {
			//<< MCC
			//MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_COMPONENT_OUT_PPBOX] = 1;
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_PUT_CV_RUN] = 1;
			//>>

			Serial::Inverter::SetInverterRun(pThis.eModule, !Conveyor.bForward, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_2].dHighSpeed, _pCmd);
			seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_CONVEYOR_BWD_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_CONVEYOR_BWD_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterRun(pThis.eModule);
			bCheck &= Serial::Inverter::GetInverterDirection(pThis.eModule, !Conveyor.bForward);
			bCheck &= Serial::Inverter::GetInverterSpeedCompare(pThis.eModule, g_pRecipe->conveyorSpeed[eCONVEYOR_ULD_2].dHighSpeed);

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;

				seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MANUAL_OUT_COMPLETED;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_5_RUN_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_CONVEYOR_BWD;
			}
			break;
		}

		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MANUAL_OUT_COMPLETED: {
			if (GetPPBoxManualOutCompleted()) {
				SetTimeout(&pThis);
				seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MANUAL_OUT_COMPLETED_CHECK;
			}
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MANUAL_OUT_COMPLETED_CHECK: {
			if (GetPPBoxManualOutCompleted()) {
				if (GetTimeout(&pThis, MANUAL_BUTTON_PUSH_TIME)) {
					wRetry = 0;
					seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_CONVEYOR_STOP;
				}
			}
			else {
				seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MANUAL_OUT_COMPLETED;
			}
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_CONVEYOR_STOP: {
			Serial::Inverter::SetInverterStop(pThis.eModule, _pCmd);
			seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_CONVEYOR_STOP_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_CONVEYOR_STOP_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= Serial::Inverter::GetInverterStop(pThis.eModule);

			if (bCheck) {
				//<< MCC
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_COMPONENT_OUT_PPBOX] = 0;
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_PUT_CV_RUN] = 0;
				//>>

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;

				seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_PPBOX_SENSOR_OFF_CHECK;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_5_STOP_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_CONVEYOR_STOP;
			}
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_PPBOX_SENSOR_OFF_CHECK: {
			if (GetPPBoxSensor(pThis.eModule, FALSE)) {
				seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MUTE_OFF;
			}
			else {
				// 2024.06.01 LBH - Conveyor Stop후에도 센서가 감지되면 Alarm
				if (GetTimeoutSec(pThis.eModule, sm.DeviceParameter->tTime.dConveyorMove)) {
					// 배출 완료 후에도 센서 감지 되어 있음.
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_CONVEYOR_5_SENSOR_CHECK, IDS_CONVEYOR_5_PPBOX_OUT_SENSOR_NOT_OFF, StepName, _pCmd);

					// 2024.07.11 LBH - PPBox가 배출되기 전에 Complete버튼을 눌렀을때.
					seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_STOPPER_DOWN;
				}
			}
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MUTE_OFF: {
			//<< MCC
			MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_PPBOX_ULD_MUTE_OFF] = 1;
			//>>

			SetMute(pThis.eModule, FALSE, _pCmd);
			seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MUTE_OFF_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MUTE_OFF_CHECK: {
			bCheck &= pThis.Status.eStatus == eMODULE_STATUS_COMPLETED;
			bCheck &= GetMute(FALSE);

			if (bCheck) {
				//<< MCC
				MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_PPBOX_ULD_MUTE_OFF] = 0;
				//>>

				// Dry Run, UseAGV와 상관없이 자재정보 지워야 함. (DB에서 지우는 기능 필요 할 수도?)
				//if (sm.DeviceParameter->tUse.bDryRun || !sm.Option->tSequence.bUseAGV)
				pThis.Glass.bMaterial = FALSE;

				wRetry = 0;
				pThis.Status.eStatus = eMODULE_STATUS_INITIAL;
				seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_END;
			}
			else if ((pThis.Status.eStatus == eMODULE_STATUS_DOWN) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_MUTE_OFF_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_MUTE_OFF;
			}
			break;
		}
		case S_ULDCV2_SEQ_MANUAL_PPBOX_UNLOAD_END: {
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID353_CASSETTE_UNLOAD_COMPLETE;
			break;
		}

		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID353_CASSETTE_UNLOAD_COMPLETE: {
			if (sm.Sequence->bCIMSkip) {
				seqStep = S_ULDCV2_SEQ_END;
				break;
			}

			// CIM Report
			S6F11_CASSETTE_UNLOAD_COMPLETE(pThis.eModule, ePPBOX_LO_01, _pCmd);
			seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID353_CASSETTE_UNLOAD_COMPLETE_CHECK;
			break;
		}
		case S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID353_CASSETTE_UNLOAD_COMPLETE_CHECK: {
			bCheck &= pThis.Status.eReportStatus == eCIM_COMPLETE;

			if (bCheck) {
				wRetry = 0;
				pThis.Status.eReportStatus = eCIM_IDLE;
				seqStep = S_ULDCV2_SEQ_PPBOX_UNLOAD_END;
			}
			else if ((pThis.Status.eReportStatus == eCIM_ERROR) || GetTimeout(&pThis, POST_MESSAGE_TIME_OUT)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_LO02->A[eMCC_ACTION_LD_LO_02_ALARM] = 1;
					SetAlarm(eAI_ULDCV2_SEQ_PIO_S6F11_CEID_353_CASSETTE_UNLOAD_COMPLETE_CHECK, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV2_SEQ_AUTO_PPBOX_UNLOAD_S6F11_CEID353_CASSETTE_UNLOAD_COMPLETE;
			}
			break;
		}
		case S_ULDCV2_SEQ_PPBOX_UNLOAD_END: {
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->Conveyor[4], 0, "");
			seqStep = S_ULDCV2_SEQ_END;
			break;
		}

		
		case S_ULDCV2_SEQ_END: {
			seqStep = S_ULDCV2_SEQ_INIT;
			break;
		}
	}
}


void GlassTransfer(PIPCOMMAND _pCmd)
{
	T_SEQUENCE_MODULE_INFO& pUpper = sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2];
	T_SEQUENCE_MODULE_INFO& pThis = sm.Module[eMODULE_PC1_GLASS_TRANSFER];
	T_SEQUENCE_MODULE_INFO& pLowerEV = sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR];
	T_SEQUENCE_MODULE_INFO& pLowerSCT = sm.Module[eMODULE_PC2_GLASS_SHIFT_TABLE];

	WORD& seqStep = pThis.Status.wSeqStep;
	WORD& seqPrevStep = pThis.Status.wSeqPrevStep;

	// log용 설명이 필요하면 description에 넣으면 됨
	// 구분자로 세미콜론(;)을 사용하니 description에는 세미콜론 사용 금지
	CString description = _T("");
	auto cleanup = make_scope_exit
	(
		[&]()
		{
			// 스탭이 기존과 바뀌거나 추가 내용이 있으면 로그로 남긴다
			if (seqStep != seqPrevStep || !description.IsEmpty())
			{
				WriteSeqLog(pThis.eModule, seqPrevStep, seqStep, description);
				seqPrevStep = seqStep;
			}


			//<< 2024.07.07 LBH - STEP_ID 
			{
				CStringA strGlassID;
				if (seqStep < S_GLASS_TR_SEQ_GLASS_TR_PAPER_SENSOR_VACUUM_ON_CHECK2) {
					// Cell ID Empty
				}
				else if (seqStep < S_GLASS_TR_SEQ_DATA_COPY) {
					CStringA szPAIRCELLID;

					// Glass 인 경우에만 하는게 맞음.
					if (pUpper.Status.wMaterialType == eMT_ORG_GLASS) {
						// PPBox Cell ID
						int iGlassIndex = pUpper.Glass.iGlassIndex; // Max - iGlass - 1 해야 할 수도 있음.
						auto& PPBoxGlass = sm.LoadPPBox->TRAY_INFOs[0].CELL_INFOs[iGlassIndex];
						szPAIRCELLID = CStringA(PPBoxGlass.szCELLID).Left(12);
						int iCheckSum = 0;
						for (int i = 0; i < szPAIRCELLID.GetLength(); i++) {
							iCheckSum += szPAIRCELLID[i];
						}

						iCheckSum %= 10;

						szPAIRCELLID += Format("%d", iCheckSum);
						strGlassID = szPAIRCELLID;
					}
				}
				else if (seqStep < S_GLASS_TR_SEQ_GLASS_UNLOAD_VACUUM_OFF) {
					if (pThis.Status.wMaterialType == eMT_ORG_GLASS) {
						strGlassID = pThis.Glass.szPAIRCELLID;
					}
				}


				BOOL bMove = sm.MotorStatus[eMOTOR_GLASS_TRANSFER_Y].tEtc.bIPRun;
				bMove |= sm.MotorStatus[eMOTOR_GLASS_TRANSFER_Z].tEtc.bIPRun;

				int iStep_ID = 0;
				if (!strGlassID.IsEmpty()) {
					iStep_ID = bMove ? 2 : 1;
				}

				SET_STEP_CELL(sm.FDC->GlassTransfer, iStep_ID, strGlassID);
			}
			//>>
		}
	);

	T_MCC_LAYER* MCC_LAYER_LI02 = &sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI02];
	T_MCC_LAYER* MCC_LAYER_LE01 = &sm.MCC_EQP->Layer[eMCC_LAYER_LD_LE01];
	T_MCC_LAYER* MCC_LAYER_TR01 = &sm.MCC_EQP->Layer[eMCC_LAYER_LD_TR01];

	// 2022.09.15 MoveFail 알람 상세용
	CString StepName(magic_enum::enum_name(static_cast<eGLASS_TRANSFER>(seqStep)).data(), magic_enum::enum_name(static_cast<eGLASS_TRANSFER>(seqStep)).size());

	BOOL bCheck = TRUE;
	WORD& wRetry = pThis.Status.wRetryCount;
	WORD& wRepeat = pThis.Status.wRepeatCount;
	int& iGlassRetry = pThis.Status.iReserve3;

	//<< 2024.06.24 LBH - EV Z축 인터락.
	if (seqStep >= S_GLASS_TR_SEQ_PAPER_UNLOAD_SEND_ABLE && seqStep < S_GLASS_TR_SEQ_PAPER_UNLOAD_VACUUM_OFF) {
		if (!CheckInPos(sm.MotorPos_1->EL_Convryor.Load_Z)) {
			seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_START;
		}
	}
	//>>

	switch (seqStep) {
		case S_GLASS_TR_SEQ_INIT: {
			//<< MCC
			INFO_INIT_MCC(MCC_LAYER_TR01);
			//>>

			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;
			pThis.Status.runningStartTime = 0;
			wRetry = 0;
			iGlassRetry = 0;
			seqStep = S_GLASS_TR_MATERIAL_CHECK;
			break;
		}
		case S_GLASS_TR_MATERIAL_CHECK: {
			//if (GetGlassTransferVacuum)
			if (pThis.Glass.bMaterial) { // 자재 정보로 진행 할 지, 아니면 Vacuum 상태 등으로 진행 할지 고민.( Vacuum이 안 들어올 수도 있어서 자재로 확인하는게 좋을것 같음)
				if (sm.Option->tSequence.fOUT) {
					if (pThis.Status.wMaterialType == eMT_ORG_GLASS) {
						seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_START;
					}
					else {
						seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_START;
					}
				}
			}
			else {
				// 2024.07.02 LBH - Auto Run Stop을 CIM 에서 전달 하도록 수정.
				////<< 2024.06.03 LBH - Interlock Loading Stop 처리.
				//if (!sm.Sequence->bCIMSkip && sm.Sequence->fInterlockLoadingStop) {
				//	// (Loader에 Glass가 없을 때)Transfer에 자재가 없고, PPBox에 Glass가 없고, LoadConveyor2에 nCrtCount가 0보다 작거나 같을 때
				//	if (!pThis.Glass.bMaterial && sm.LoadPPBox->TRAY_INFOs[0].nCell == 0 && pUpper.Status.nCrtCount <= 0) {
				//		sm.Sequence->fManualStop = FALSE;
				//		sm.Sequence->fAutoMode = FALSE;
				//		// Message 또는 Alarm 출력. (Interlock Loading Step - Glass 배출 완료)
				//		//SetAlarm();
				//	}
				//}
				////>>
				//<< 2024.06.29 LBH - Loading 할 자재가 Paper이거나 SCT Table에 자재가 없는 경우에만 로딩 (Glass 오래 들고 있으면 안좋음)
				BOOL bLonading = ((pUpper.ToLower.fSend_Able) && (pUpper.Status.wMaterialType == eMT_PAPER));
				bLonading |= pLowerSCT.Glass.bMaterial == FALSE;

				// 202407.12 LBH - Glass + Paper Count 추가.
				bLonading &= pUpper.Status.nCrtCount > 1; // Glass가 있을 때.

				// 2024.07.29 LBH - Interlock Loading Stop시 자재 투입 금지.
				if (!sm.Sequence->bCIMSkip && sm.Sequence->fCIM_LoadingStop) {
					bLonading = FALSE;
				}

				if (bLonading) {
					seqStep = S_GLASS_TR_SEQ_GLASS_PAPER_LOAD_START;
				}
				//>
			}
			break;
		}

		case S_GLASS_TR_SEQ_GLASS_PAPER_LOAD_START: {
			//<< MCC
			MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_INIT_ORIGIN] = 1;
			//>>

			wRetry = 0;
			wRepeat = 0;

			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			//SET_STEP_CELL(sm.FDC->GlassTransfer, 0, "");
			seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_INIT_MOVE;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_Z_INIT_MOVE: {
			T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, TRUE, eMOTOR_Z_UP);
			PTPMove(posZ);
			seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_INIT_MOVE_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_Z_INIT_MOVE_CHECK: {
			T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, TRUE, eMOTOR_Z_UP);
			if (CheckInPos(posZ)) {
				wRetry = 0;
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_INIT_ORIGIN] = 0;
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_Y_GLASS_LOADING_POS_MOVE;
			}
			else {
				if (GetMoveTimeout(posZ)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, posZ, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_INIT_MOVE;
				}
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_Y_GLASS_LOADING_POS_MOVE: {
			T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, TRUE, eMOTOR_Z_UP);
			bCheck &= CheckInPos(posZ);

			if (bCheck) {
				//<< MCC
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_MOVE_Y_LD_POS] = 1;
				//>>

				wRetry = 0;

				PTPMove(sm.MotorPos_1->Glass_TR.Load_Y);
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_Y_GLASS_LOADING_POS_MOVE_CHECK;
			}
			else {
				if (GetMoveTimeout(posZ)) {
					if (++wRepeat >= MOVE_RETRY) {
						wRepeat = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, posZ, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_INIT_MOVE;
				}
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_Y_GLASS_LOADING_POS_MOVE_CHECK: {
			bCheck &= CheckInPos(sm.MotorPos_1->Glass_TR.Load_Y);
			T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, TRUE, eMOTOR_Z_UP);
			bool bUpZ = CheckInPos(posZ); 
			if (bCheck & bUpZ) {
				//<< MCC
				//MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_INIT_ORIGIN] = 0;
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_MOVE_Y_LD_POS] = 0;
				//>>

				seqStep = S_GLASS_TR_SEQ_GLASS_TR_RECV_ABLE;
			}
			else {
				if (!bUpZ)
				{
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_INIT_MOVE;

				}
				if (GetMoveTimeout(sm.MotorPos_1->Glass_TR.Load_Y)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Y_MOVE_FAIL, IDS_DES_NOT_USE, StepName, sm.MotorPos_1->Glass_TR.Load_Y, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_TR_Y_GLASS_LOADING_POS_MOVE;
				}
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_RECV_ABLE: {
			//<< MCC
			MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_IN_WAIT_TIME] = 1;
			//>>

			pThis.ToUpper.fReceive_Able = TRUE;
			seqStep = S_GLASS_TR_SEQ_LDCV2_SEND_ABLE_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_LDCV2_SEND_ABLE_CHECK: {
			// 2024.06.24 LBH - Load Stop & Out Stop 시 자재 Pick Up 안함.
			//24.07.29 KGH EDIT : Title Wnd's Input/Outputstop -> PPBOX  Input/Outputstop
			if (!pThis.Status.bInputStop) {
			//if (!pThis.Status.bInputStop && sm.Option->tSequence.fOUT) {
				bCheck = pUpper.Status.wTargetIndex == pThis.eModule;
				bCheck &= pUpper.ToLower.fSend_Able;
			
				if (bCheck)
					seqStep = S_GLASS_TR_SEQ_LDCV2_SEND_START_CHECK;
			}
			
			break;
		}
		case S_GLASS_TR_SEQ_LDCV2_SEND_START_CHECK: {
			if (pUpper.ToLower.fSend_Start) {
				////<< MCC
				//MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_IN_WAIT_TIME] = 0;
				////>>
				
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_RECV_START;
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_RECV_START: {
			pThis.ToUpper.fReceive_Start = TRUE;
			wRetry = 0;
			seqStep = S_GLASS_TR_SEQ_GLASS_TR_PAPER_CYLINDER_SET;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_PAPER_CYLINDER_SET: {
			SetTimeout(&pThis);
			SetGlassTransferCylinderPusher(FALSE); // LJY 수정 2024.04.19 Glass Load 동작시 Paper Cylinder Down상태로 수정(Vacuum은 OFf)
			seqStep = S_GLASS_TR_SEQ_GLASS_TR_PAPER_CYLINDER_SET_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_PAPER_CYLINDER_SET_CHECK: {
			if (GetGlassTransferCylinderPusher(FALSE)) { 
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_GLASS_PICK_UP_DOWN_MOVE;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
					SetAlarm(eAI_GLASS_TRANSFER_PAPER_PUSER_CYLINDER_FWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_ULDCV1_SEQ_PPBOX_UNLOAD_STOPPER_DOWN;
			}

			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_Z_GLASS_PICK_UP_DOWN_MOVE: {
			bCheck &= CheckInPos(sm.MotorPos_1->Glass_TR.Load_Y);
			if (bCheck) {
				iGlassRetry = 0;
				T_POS posZ = GetGlassTransferPosZ((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, eMOTOR_Z_BEFOR);
				PTPMove(posZ);
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_GLASS_PICK_UP_DOWN_MOVE_CHECK;
			}
			else {
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_Y_GLASS_LOADING_POS_MOVE;
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_Z_GLASS_PICK_UP_DOWN_MOVE_CHECK: {
			T_POS posZ = GetGlassTransferPosZ((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, eMOTOR_Z_BEFOR);

			bCheck &= CheckInPos(posZ);
			if (bCheck) {
				wRetry = 0; 
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_GLASS_PICK_UP_SLOW_DOWN_MOVE;
			}
			else {
				if (GetMoveTimeout(posZ)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, posZ, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_GLASS_PICK_UP_DOWN_MOVE;
				}
			}

			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_Z_GLASS_PICK_UP_SLOW_DOWN_MOVE: {
			bCheck &= CheckInPos(sm.MotorPos_1->Glass_TR.Load_Y);
			if (bCheck) {
				//<< MCC, 2024.07.09 move by kkw
				CELL_INFO_TO_MCC(pUpper.Glass, MCC_LAYER_TR01);
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_OUT_WAIT_TIME] = 0;
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_IN_WAIT_TIME] = 0;
				//>>
				if ((eMATERIAL_TYPE)pUpper.Status.wMaterialType == eMT_ORG_GLASS) {
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_COMPONENT_OUT_GLASS] = 1;
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_COMPONENT_IN_GLASS] = 1;
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_MOVE_Z_LD_POS_1] = 1;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_GLASS_OUT_PROCESS] = 1;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_GLASS_OUT_COUNT] = 1;
				}
				else {
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_COMPONENT_OUT_PAPER] = 1;
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_COMPONENT_IN_PAPER] = 1;
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_MOVE_Z_LD_POS_2] = 1;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_PAPER_OUT_PROCESS] = 1;
					MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_PAPER_OUT_COUNT] = 1;
				}
				//>>
				
				T_POS posZ = GetGlassTransferPosZ((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, eMOTOR_Z_DOWN);

				// 자재 못집었을 경우 Retry;
				if (posZ.dTarget + iGlassRetry <= sm.MotorPos_1->Glass_TR.Load_Z.dTarget) {
					posZ.dTarget += iGlassRetry;
				}
				else {
					posZ.dTarget = sm.MotorPos_1->Glass_TR.Load_Z.dTarget;
				}

				PTPMove(posZ);
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_GLASS_PICK_UP_SLOW_DOWN_MOVE_CHECK;
			}
			else {
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_Y_GLASS_LOADING_POS_MOVE;
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_Z_GLASS_PICK_UP_SLOW_DOWN_MOVE_CHECK: {
			T_POS posZ = GetGlassTransferPosZ((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, eMOTOR_Z_DOWN);
			// 자재 못집었을 경우 Retry;

			if (posZ.dTarget + iGlassRetry <= sm.MotorPos_1->Glass_TR.Load_Z.dTarget) {
				posZ.dTarget += iGlassRetry;
			}
			else {
				posZ.dTarget = sm.MotorPos_1->Glass_TR.Load_Z.dTarget;
			}

			bCheck &= CheckInPos(posZ);
			if (bCheck) {
				//<< MCC
				if ((eMATERIAL_TYPE)pUpper.Status.wMaterialType == eMT_ORG_GLASS) {
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_MOVE_Z_LD_POS_1] = 0;
				}
				else {
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_MOVE_Z_LD_POS_2] = 0;
				}
				//>>
			
				wRetry = 0;
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_VACUUM_ON;
			}
			else {
				if (GetMoveTimeout(posZ)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, posZ, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_GLASS_PICK_UP_SLOW_DOWN_MOVE;
				}
			}

			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_VACUUM_ON: {
			//<< MCC
			if ((eMATERIAL_TYPE)pUpper.Status.wMaterialType == eMT_ORG_GLASS) {
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_VAC_ON_1] = 1;
			}
			else {
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_VAC_ON_2] = 1;
			}
			//>>

			SetTimeout(&pThis);
			SetGlassTransferVacuum((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE);
			seqStep = S_GLASS_TR_SEQ_GLASS_TR_VACUUM_ON_CHECK;
			break;
		}
		//>>Edit

		//case S_GLASS_TR_SEQ_GLASS_TR_VACUUM_ON_CHECK: {
		//	if (GetGlassTransferVacuum((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, TRUE)) {
		//		if (GetTimeoutSec(pThis.eModule, g_pRecipe->vacuumBlowTime.dVacuumOnDelay)) {
		//			//<< MCC
		//			if ((eMATERIAL_TYPE)pUpper.Status.wMaterialType == eMT_ORG_GLASS) {
		//				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_VAC_ON_1] = 0;
		//			}
		//			else {
		//				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_VAC_ON_2] = 0;
		//			}
		//			//>>				
		//			T_POS CalcPosZ = GetGlassTransferPosZ((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, eMOTOR_Z_DOWN, FALSE);
		//			double dCurrent = GetMotorPosition((eAxis)CalcPosZ.iAxis);
		//			// 2024.06.20 LBH - Target 30, Current 50; (Current - Target) 더해주면 됨.
		//			double dOffset = dCurrent - CalcPosZ.dTarget;
		//			// Load Z보다 클 수 없다.
		//			if (CalcPosZ.dTarget + dOffset <= sm.MotorPos_1->Glass_TR.Load_Z.dTarget) {
		//				sm.Sequence->dGlassTransferOffsetZ = dOffset;
		//			}
		//			else {
		//				dOffset = sm.MotorPos_1->Glass_TR.Load_Z.dTarget - CalcPosZ.dTarget;
		//				sm.Sequence->dGlassTransferOffsetZ = dOffset;
		//			}
		//			iGlassRetry = 0;
		//			wRetry = 0;
		//			////2024.07.01 Move FDC Step ID
		//			//int iGlassIndex = pUpper.Glass.iGlassIndex; // Max - iGlass - 1 해야 할 수도 있음.
		//			//auto& PPBoxGlass = sm.LoadPPBox->TRAY_INFOs[0].CELL_INFOs[iGlassIndex];
		//			//CStringA szPAIRCELLID(CStringA(PPBoxGlass.szCELLID).Left(12));
		//			//int iCheckSum = 0;
		//			//for (int i = 0; i < szPAIRCELLID.GetLength(); i++) {
		//			//	iCheckSum += szPAIRCELLID[i];
		//			//}
		//			//
		//			//iCheckSum %= 10;
		//			//
		//			//szPAIRCELLID += Format("%d", iCheckSum);
		//			//SET_STEP_CELL(sm.FDC->GlassTransfer, 1, szPAIRCELLID);
		//			seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_UP_SLOW_MOVE;
		//		}
		//	}
		//	else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dVacuum)) {
		//		if (iGlassRetry++ < sm.DeviceParameter->tCount.GlassRetryCount) {
		//			seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_GLASS_PICK_UP_SLOW_DOWN_MOVE;
		//			break;
		//		}
		//		else {
		//			if (++wRetry > IO_RETRY) {
		//				iGlassRetry = 0;
		//				wRetry = 0;
		//				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
		//				SetAlarm(eAI_GLASS_TRANSFER_PAPER_VACUUM_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
		//			}
		//			seqStep = S_GLASS_TR_SEQ_GLASS_TR_VACUUM_ON;
		//		}
		//	}
		//	break;
		//}

		case S_GLASS_TR_SEQ_GLASS_TR_VACUUM_ON_CHECK: {
			if (GetGlassTransferVacuum((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, TRUE)) {
				if (GetTimeoutSec(pThis.eModule, 0.5)) {
					//<< MCC
					if ((eMATERIAL_TYPE)pUpper.Status.wMaterialType == eMT_ORG_GLASS) {
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_VAC_ON_1] = 0;
					}
					else {
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_VAC_ON_2] = 0;
					}
					//>>
				}
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_VACUUM_ON_DELAY;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dVacuum)) {
				if (iGlassRetry++ < sm.DeviceParameter->tCount.GlassRetryCount) {
					seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_GLASS_PICK_UP_SLOW_DOWN_MOVE;
					break;
				}
				else {
					if (++wRetry > IO_RETRY) {
						iGlassRetry = 0;
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_PAPER_VACUUM_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					}

					seqStep = S_GLASS_TR_SEQ_GLASS_TR_VACUUM_ON;
				}

			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_VACUUM_ON_DELAY: {		
			//<< MCC
			if ((eMATERIAL_TYPE)pUpper.Status.wMaterialType == eMT_ORG_GLASS) {
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_VAC_ON_1_DELAY] = 1;
			}
			else {
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_VAC_ON_2_DELAY] = 1;
			}
			//>>	
			SetTimeout(&pThis);
			seqStep = S_GLASS_TR_SEQ_GLASS_TR_VACUUM_ON_DELAY_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_VACUUM_ON_DELAY_CHECK: {
			if (!GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dVacuumOnDelay)) {
				break;
			}
			//<< MCC
			if ((eMATERIAL_TYPE)pUpper.Status.wMaterialType == eMT_ORG_GLASS) {
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_VAC_ON_1_DELAY] = 0;
			}
			else {
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_VAC_ON_2_DELAY] = 0;
			}
			//>>	
			T_POS CalcPosZ = GetGlassTransferPosZ((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, eMOTOR_Z_DOWN, FALSE);
			double dCurrent = GetMotorPosition((eAxis)CalcPosZ.iAxis);

			// 2024.06.20 LBH - Target 30, Current 50; (Current - Target) 더해주면 됨.

			double dOffset = dCurrent - CalcPosZ.dTarget;
			// Load Z보다 클 수 없다.
			if (CalcPosZ.dTarget + dOffset <= sm.MotorPos_1->Glass_TR.Load_Z.dTarget) {
				sm.Sequence->dGlassTransferOffsetZ = dOffset;
			}
			else {
				dOffset = sm.MotorPos_1->Glass_TR.Load_Z.dTarget - CalcPosZ.dTarget;
				sm.Sequence->dGlassTransferOffsetZ = dOffset;
			}

			iGlassRetry = 0;
			wRetry = 0;

			////2024.07.01 Move FDC Step ID
			//int iGlassIndex = pUpper.Glass.iGlassIndex; // Max - iGlass - 1 해야 할 수도 있음.
			//auto& PPBoxGlass = sm.LoadPPBox->TRAY_INFOs[0].CELL_INFOs[iGlassIndex];
			//CStringA szPAIRCELLID(CStringA(PPBoxGlass.szCELLID).Left(12));
			//int iCheckSum = 0;
			//for (int i = 0; i < szPAIRCELLID.GetLength(); i++) {
			//	iCheckSum += szPAIRCELLID[i];
			//}
			//
			//iCheckSum %= 10;
			//
			//szPAIRCELLID += Format("%d", iCheckSum);
			//SET_STEP_CELL(sm.FDC->GlassTransfer, 1, szPAIRCELLID);
			seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_UP_SLOW_MOVE;
			break;
		}
		//<<Edit
		case S_GLASS_TR_SEQ_GLASS_TR_Z_UP_SLOW_MOVE: {
			//2024.07.01 move FDC Step ID
			//SET_STEP_CELL(sm.FDC->GlassTransfer, 2);
			T_POS posZ = GetGlassTransferPosZ((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, eMOTOR_Z_AFTER);
			PTPMove(posZ);
			seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_UP_SLOW_MOVE_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_Z_UP_SLOW_MOVE_CHECK: {
			T_POS posZ = GetGlassTransferPosZ((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, eMOTOR_Z_AFTER);
			bCheck &= CheckInPos(posZ);
			if (bCheck) {
				wRetry = 0;
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_PAPER_SENSOR_VACUUM_ON_CHECK2;
			}
			else {
				if (GetMoveTimeout(posZ)) {
					if (++wRetry > MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, posZ, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_UP_SLOW_MOVE;
				}
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_PAPER_SENSOR_VACUUM_ON_CHECK2: {
			bCheck = GetGlassTransferVacuum((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, TRUE);
			bCheck &= GetMaterialSensorCheck((eMATERIAL_TYPE)pUpper.Status.wMaterialType);

			if (bCheck) {
				wRetry = 0;
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_UP_MOVE;
			}
			else { // Up 한 후 Vacuum일 경우는 Alarm이 맞는것 같음?
				
				if (++wRepeat > IO_RETRY) {
					if (!GetMaterialSensorCheck((eMATERIAL_TYPE)pUpper.Status.wMaterialType)) {
						if (pUpper.Status.wMaterialType == eMT_ORG_GLASS) {
							MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
							SetAlarm(eAI_GLASS_TRANSFER_GLASS_DECTED_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
						}
						else {
							MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
							SetAlarm(eAI_GLASS_TRANSFER_PAPER_DECTED_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
						}
					}

					if (!GetGlassTransferVacuum((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, TRUE)) {
						if (!sm.DeviceParameter->tUse.bGanziAlarmSkip || pUpper.Status.wMaterialType == eMT_ORG_GLASS) {
							MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
							SetAlarm(eAI_GLASS_TRANSFER_PAPER_VACUUM_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
						}
						else {
							// 2024.07.12 LBH - 간지알람 스킵 추가.
							if (GetMaterialSensorCheck((eMATERIAL_TYPE)pUpper.Status.wMaterialType)) {
								wRetry = 0;
								seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_UP_MOVE;
							}
						}
					}
				}

				seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_GLASS_PICK_UP_SLOW_DOWN_MOVE;
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_Z_UP_MOVE: {
			//<< MCC
			if ((eMATERIAL_TYPE)pUpper.Status.wMaterialType == eMT_ORG_GLASS) {
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_MOVE_Z_READY_POS_1] = 1;
			}
			else {
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_MOVE_Z_READY_POS_2] = 1;
			}
			//>>

			T_POS posZ = GetGlassTransferPosZ((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, eMOTOR_Z_UP);
			PTPMove(posZ);
			seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_UP_MOVE_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_Z_UP_MOVE_CHECK: {
			T_POS posZ = GetGlassTransferPosZ((eMATERIAL_TYPE)pUpper.Status.wMaterialType, TRUE, eMOTOR_Z_UP);
			bCheck &= CheckInPos(posZ);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_MOVE_Z_READY_POS_1] = 0;
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_GET_MOVE_Z_READY_POS_2] = 0;
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_COMPONENT_OUT_GLASS] = 0;
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_COMPONENT_IN_GLASS] = 0;
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_GLASS_OUT_PROCESS] = 0;
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_GLASS_OUT_COUNT] = 0;
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_COMPONENT_OUT_PAPER] = 0;
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_COMPONENT_IN_PAPER] = 0;
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_PAPER_OUT_PROCESS] = 0;
				MCC_LAYER_LI02->A[eMCC_ACTION_LD_LI_02_PUT_PAPER_OUT_COUNT] = 0;
				//>>

				wRetry = 0;
				if (pUpper.Status.wMaterialType == eMT_ORG_GLASS) {
					seqStep = S_GLASS_TR_SEQ_GLASS_TR_PAPER_CYLINDER_UP;
				}
				else {
					seqStep = S_GLASS_TR_SEQ_DATA_COPY;
				}
			}
			else {
				if (GetMoveTimeout(posZ)) {
					if (++wRetry > MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, posZ, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_TR_Z_UP_MOVE;
				}
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_PAPER_CYLINDER_UP: {
			SetTimeout(&pThis);
			SetGlassTransferCylinderPusher(TRUE);
			seqStep = S_GLASS_TR_SEQ_GLASS_TR_PAPER_CYLINDER_UP_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_PAPER_CYLINDER_UP_CHECK: {
			if (GetGlassTransferCylinderPusher(TRUE)) {
				seqStep = S_GLASS_TR_SEQ_DATA_COPY;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dDefaultCylinder)) {
				if (++wRetry >= IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
					SetAlarm(eAI_GLASS_TRANSFER_PAPER_PUSER_CYLINDER_BWD_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_PAPER_CYLINDER_UP;
			}
			break;
		}
		case S_GLASS_TR_SEQ_DATA_COPY: {
			pThis.Status.wMaterialType = pUpper.Status.wMaterialType;

			if (pThis.Status.wMaterialType == eMT_ORG_GLASS) {
				int iGlassIndex = pUpper.Glass.iGlassIndex++; // Max - iGlass - 1 해야 할 수도 있음.
				auto& PPBoxGlass = sm.LoadPPBox->TRAY_INFOs[0].CELL_INFOs[iGlassIndex];
				auto& Glass = pThis.Glass;
				memcpy(&Glass, &PPBoxGlass, sizeof(T_CELL_INFO));

				Glass.iGlassMaxCount = sm.LoadPPBox->TRAY_INFOs[0].nCell; // Transfer 의 Glass Info  부분에 Max Count / Index 를 넣어서 보내준다
				Glass.iGlassIndex = iGlassIndex;

				//<< 2024.06.14 LBH - CIM 보고 때문에 처음에 GOOD 만들어줌.
				pThis.Glass.eCellJudgeType = eJUDGE_GOOD;
				//>>

				PPBoxGlass.bMaterial = FALSE;
				Glass.bMaterial = TRUE;

				CStringA szPAIRCELLID(CStringA(Glass.szCELLID).Left(12));
				int iCheckSum = 0;
				for (int i = 0; i < szPAIRCELLID.GetLength(); i++) {
					iCheckSum += szPAIRCELLID[i];
				}

				iCheckSum %= 10;

				szPAIRCELLID += Format("%d", iCheckSum);
				strncpy_s(Glass.szPAIRCELLID, sizeof(Glass.szPAIRCELLID), szPAIRCELLID, sizeof(Glass.szPAIRCELLID) - 1);
				strncpy_s(Glass.szCELLTYPE, sizeof(Glass.szCELLTYPE), "GLASS", sizeof(Glass.szCELLTYPE) - 1);

				// 2024.07.12 LBH - PPID Copy (recipeName가 아닌 PPID로 변경 해야 할 수도 있음)
				strncpy_s(Glass.szPPID, sizeof(Glass.szPPID), CStringA(g_pRecipe->recipeName), sizeof(Glass.szPPID) - 1);


				//2024.07.01 Move to Vacuum Check
				//SET_STEP_CELL(sm.FDC->GlassTransfer, 1, pThis.Glass.szPAIRCELLID);

				// 2024.07.01 LBH - Glass Input
				GlassInput();
			}
			else if (pThis.Status.wMaterialType == eMT_PAPER) {
				memcpy(&pThis.Glass, &pUpper.Glass, sizeof(pThis.Glass));
			}

			pUpper.Status.nCrtCount -= 1; //PPBOX 배출시에만 해줘야 되는 것 아닌지?
			seqStep = S_GLASS_TR_SEQ_GLASS_TR_RECV_COMPLETED;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_RECV_COMPLETED: {
			pThis.ToUpper.fReceive_Complete = TRUE;
			seqStep = S_GLASS_TR_SEQ_LDCV2_SEND_COMPLETED_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_LDCV2_SEND_COMPLETED_CHECK: {
			if (pUpper.ToLower.fSend_Complete)
				seqStep = S_GLASS_TR_SEQ_GLASS_TR_RECV_INIT;
			//<< MCC
			MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_OUT_WAIT_TIME_PAPER] = 1;
			//>>
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_TR_RECV_INIT: {
			SetToUpperInit(&pThis);
			seqStep = S_GLASS_TR_SEQ_LDCV2_SEND_INIT_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_LDCV2_SEND_INIT_CHECK: {
			if (IsInitToLower(&pUpper))
				seqStep = S_GLASS_TR_SEQ_GLASS_PAPER_LOAD_END;

			break;
		}
		case S_GLASS_TR_SEQ_GLASS_PAPER_LOAD_END: {
			pThis.Status.wTargetIndex = eMODULE_NONE;
			seqStep = S_GLASS_TR_SEQ_INIT;
			break;
		}

		case S_GLASS_TR_SEQ_GLASS_UNLOAD_START: {
			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;

			pThis.Status.wTargetIndex = pLowerSCT.eModule;

			// 2024.07.01 Move to Slow Z Up
			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			//SET_STEP_CELL(sm.FDC->GlassTransfer, 2, pThis.Glass.szCELLID);
			seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_INIT_MOVE;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_INIT_MOVE: {
			T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, TRUE, eMOTOR_Z_UP);
			PTPMove(posZ);
			seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_INIT_MOVE_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_INIT_MOVE_CHECK: {
			T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, TRUE, eMOTOR_Z_UP);
			bCheck &= CheckInPos(posZ);
			if (bCheck) {
				wRetry = 0;
				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Y_SCT_POS_MOVE;
			}
			else {
				if (GetMoveTimeout(posZ)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, posZ, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_INIT_MOVE;
				}
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_Y_SCT_POS_MOVE: {
			T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, TRUE, eMOTOR_Z_UP);
			bCheck &= CheckInPos(posZ);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_MOVE_Y_GT01_ULD_POS] = 1;
				//>>

				PTPMove(sm.MotorPos_1->Glass_TR.Unload_Glass_Y);
				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Y_SCT_POS_MOVE_CHECK;
			}
			else {
				if (++wRepeat >= MOVE_RETRY) {
					wRepeat = 0;
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
					SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_INIT_MOVE;
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_Y_SCT_POS_MOVE_CHECK: {
			bCheck &= CheckInPos(sm.MotorPos_1->Glass_TR.Unload_Glass_Y);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_MOVE_Y_GT01_ULD_POS] = 0;
				//>>

				wRetry = 0;
				if (sm.DeviceParameter->tUse.bPCLinkerUse) {
					seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_SEND_ABLE;
				}
				else {
					if (sm.DeviceParameter->tUse.bDryRun)
						pThis.Glass.bMaterial = FALSE;

					seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Y_LOADING_MOVE;
				}
			}
			else {
				if (GetMoveTimeout(sm.MotorPos_1->Glass_TR.Unload_Glass_Y)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Y_MOVE_FAIL, IDS_DES_NOT_USE, StepName, sm.MotorPos_1->Glass_TR.Unload_Glass_Y, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Y_SCT_POS_MOVE;
				}
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_SEND_ABLE: {
			//<< MCC
			MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_OUT_WAIT_TIME_GLASS] = 1;
			//>>

			pThis.ToLower.fSend_Able = TRUE;
			seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_RECV_ABLE_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_RECV_ABLE_CHECK: {
			if (pLowerSCT.ToUpper.fReceive_Able)
				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_RECV_START_CHECK;

			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_RECV_START_CHECK: {
			if (pLowerSCT.ToUpper.fReceive_Start) {
				////<< MCC
				//MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_OUT_WAIT_TIME_GLASS] = 0;
				////>>

				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_SEND_START;
			}

			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_SEND_START: {
			pThis.ToLower.fSend_Start = TRUE;

			wRepeat = 0;
			wRetry = 0;

			seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_DOWN_MOVE;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_DOWN_MOVE: {
			bCheck &= CheckInPos(sm.MotorPos_1->Glass_TR.Unload_Glass_Y);
			if (bCheck) {
				//<< MCC, 2024.07.09 move by kkw
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_OUT_WAIT_TIME_GLASS] = 0;
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_COMPONENT_OUT_GLASS] = 1;
				CIPVar var;
				var.SetChildItem("On", 1);
				g_ipClient.PostIPCommand("UI", "MCC", "GTComponentIn", var);
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_MOVE_Z_ULD_GLASS_POS] = 1;
				//>>
				T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, FALSE, eMOTOR_Z_BEFOR);
				PTPMove(posZ);
				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_DOWN_MOVE_CHECK;
			}
			else {
				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Y_SCT_POS_MOVE;
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_DOWN_MOVE_CHECK: {
			T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, FALSE, eMOTOR_Z_BEFOR);
			bCheck &= CheckInPos(posZ);
			if (bCheck) {
				if (sm.DeviceParameter->tUse.bPCLinkerUse) {
					wRetry = 0;
					seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_SLOW_DOWN_MOVE;
				}
				else {

				}
			}
			else {
				if (GetMoveTimeout(posZ)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, posZ, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_DOWN_MOVE;
				}
			}

			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_SLOW_DOWN_MOVE: {
			bCheck &= CheckInPos(sm.MotorPos_1->Glass_TR.Unload_Glass_Y);
			if (bCheck) {
				////<< MCC
				//MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_COMPONENT_OUT_GLASS] = 1;
				//CIPVar var;
				//var.SetChildItem("On", 1);
				//g_ipClient.PostIPCommand("UI", "MCC", "GTComponentIn", var);
				//MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_MOVE_Z_ULD_GLASS_POS] = 1;
				////>>

				T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, FALSE, eMOTOR_Z_DOWN);
				PTPMove(posZ);
				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_SLOW_DOWN_MOVE_CHECK;
			}
			else {
				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Y_SCT_POS_MOVE;
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_SLOW_DOWN_MOVE_CHECK: {
			T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, FALSE, eMOTOR_Z_DOWN);
			bCheck &= CheckInPos(posZ);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_MOVE_Z_ULD_GLASS_POS] = 0;
				//>>

				wRetry = 0;
				seqStep = S_GLASS_TR_SEQ_SEND_MG_SHIFT_TABLE_VACUUM_ON;
			}
			else {
				if (GetMoveTimeout(posZ)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, posZ, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_SLOW_DOWN_MOVE;
				}
			}
			break;
		}
		//<< KES 추가 2024.4.5														  
		case S_GLASS_TR_SEQ_SEND_MG_SHIFT_TABLE_VACUUM_ON: {
			// 제위치에 없으면 알람
			bool bInPosY = CheckInPos(sm.MotorPos_1->Glass_TR.Unload_Glass_Y);//2024.04.13 PSS EDIT: 글래스 언로드 위치로 수정
			bool bInPosZ = CheckInPos(GetGlassTransferPosZ(eMT_ORG_GLASS, FALSE, eMOTOR_Z_DOWN));//2024.04.13 PSS EDIT: 글래스 언로드로 수정
			bool bSensor = TRUE;

			bSensor &= bInPosY && bInPosZ;

			if (bSensor) {
				//<< MCC
				CIPVar varMCC;
				varMCC.SetChildItem("On", 1);
				g_ipClient.PostIPCommand("UI", "MCC", "GTVacOn", varMCC);
				//>>

				SetTimeout(&pThis);
				CIPVar var;
				var.SetChildItem("OnOff", TRUE);
				g_ipClient.PostIPCommand("UI", "ShiftTable", "Vacuum", var);
				seqStep = S_GLASS_TR_SEQ_SEND_MG_SHIFT_TABLE_VACUUM_ON_OUT_CHECK;
			}
			else {
				if (!bInPosZ) {
					seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_SLOW_DOWN_MOVE;//2024.04.13 PSS EDIT: 글래스 언로드  Z 위치로 수정
				}
				else if (!bInPosY) {
					seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Y_SCT_POS_MOVE;//2024.04.13 PSS EDIT: 글래스 언로드 Y 위치로 수정
				}
			}
			break;
		}
		// LJY 2024.05.01 추가
		case S_GLASS_TR_SEQ_SEND_MG_SHIFT_TABLE_VACUUM_ON_OUT_CHECK: {
			if (!sm.DeviceParameter->tUse.bDryRun) {
				bCheck &= sm.IOs->SCT.Out.fSCT_MG_SHIFT_TABLE_AREA_1_VACUUM_ON_OFF_SOL == TRUE;
				bCheck &= sm.IOs->SCT.Out.fSCT_MG_SHIFT_TABLE_AREA_2_VACUUM_ON_OFF_SOL == TRUE;
				bCheck &= sm.IOs->SCT.Out.fSCT_MG_SHIFT_TABLE_AREA_3_VACUUM_ON_OFF_SOL == TRUE;
				bCheck &= sm.IOs->SCT.Out.fSCT_MG_SHIFT_TABLE_AREA_4_VACUUM_ON_OFF_SOL == TRUE;
			}

			if (bCheck) {
				//<< MCC
				CIPVar var;
				var.SetChildItem("On", 0);
				g_ipClient.PostIPCommand("UI", "MCC", "GTVacOn", var);
				//>>

				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_VACUUM_OFF;
			}			
			else {
				if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dVacuum)) {
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
					SetAlarm(eAI_SHIFT_TALBE_VACUUM_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
					seqStep = S_GLASS_TR_SEQ_SEND_MG_SHIFT_TABLE_VACUUM_ON;
				}
			}
			break;
		}
		//>> 
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_VACUUM_OFF: {
			//<< 2024.05.01 LBH - SCT  Vacuum 실행 Vacuum On Delay 후에 Transfer Vacuum Off 하도록 수정.
			//  기존에는 Table Vacuum On Check 후에 Transfer Vacuum Off여서 Delay 를 줄 필요가 없었음.
			if (!GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dVacuumOnDelay)) { // 다른 값으로 바꿔야 할수도 있음.
				break;
			}
			//>>

			//<< MCC
			MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_VAC_OFF_1] = 1;
			//>>

			SetTimeout(&pThis);
			SetGlassTransferVacuum((eMATERIAL_TYPE)pThis.Status.wMaterialType, FALSE);
			// LJY 수정 2024.04.09 Blow 동작시 Paper & Glass 구분없이 Blow 동작하도록 수정
			SetGlassTransferBlow(TRUE);

			// 2024.06.28 LBH - 함수 내부에서 CIM Skip Interlock 작업 되어 있음.
			SET_STEP_CELL(sm.FDC->GlassTransfer, 0, "");
			seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_VACUUM_OFF_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_VACUUM_OFF_CHECK: {
			if (GetGlassTransferVacuum((eMATERIAL_TYPE)pThis.Status.wMaterialType, FALSE, TRUE)) {
				if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dBlowOnWait)) {
					////<< MCC
					//MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_VAC_OFF_1] = 0;
					////>>


					wRetry = 0;
					seqStep = S_GLASS_TR_SEQ_SEND_MG_SHIFT_TABLE_VACUUM_ON_CHECK;
				}
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dBlow)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
					SetAlarm(eAI_GLASS_TRANSFER_PAPER_VACUUM_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}

				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_VACUUM_OFF;
			}
			break;
		}
		case S_GLASS_TR_SEQ_SEND_MG_SHIFT_TABLE_VACUUM_ON_CHECK: {
			// 2024.05.01 LBH - Dry Run이 아닐 때 만 확인하도록 수정.
			if (!sm.DeviceParameter->tUse.bDryRun) {
				bCheck &= sm.IOs->SCT.Out.fSCT_MG_SHIFT_TABLE_AREA_1_VACUUM_ON_OFF_SOL == TRUE;
				bCheck &= sm.IOs->SCT.Out.fSCT_MG_SHIFT_TABLE_AREA_2_VACUUM_ON_OFF_SOL == TRUE;
				bCheck &= sm.IOs->SCT.Out.fSCT_MG_SHIFT_TABLE_AREA_3_VACUUM_ON_OFF_SOL == TRUE;
				bCheck &= sm.IOs->SCT.Out.fSCT_MG_SHIFT_TABLE_AREA_4_VACUUM_ON_OFF_SOL == TRUE;

				// InSensor 확인.
				bCheck &= sm.IOs->SCT.In.fSCT_MG_SHIFT_TABLE_VACUUM_AREA_1_MAIN_CHECK_SWITCH == TRUE;
				bCheck &= sm.IOs->SCT.In.fSCT_MG_SHIFT_TABLE_VACUUM_AREA_2_MAIN_CHECK_SWITCH == TRUE;
				bCheck &= sm.IOs->SCT.In.fSCT_MG_SHIFT_TABLE_VACUUM_AREA_3_MAIN_CHECK_SWITCH == TRUE;
				bCheck &= sm.IOs->SCT.In.fSCT_MG_SHIFT_TABLE_VACUUM_AREA_4_MAIN_CHECK_SWITCH == TRUE;
			}

			if (bCheck) {
				//<< MCC, 2024.07.09 move by kkw
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_VAC_OFF_1] = 0;
				//>>
				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_SLOW_UP_MOVE;
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dVacuum)) {
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
				SetAlarm(eAI_SHIFT_TALBE_VACUUM_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				seqStep = S_GLASS_TR_SEQ_SEND_MG_SHIFT_TABLE_VACUUM_ON;
			}

			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_SLOW_UP_MOVE: {
			T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, FALSE, eMOTOR_Z_AFTER);
			PTPMove(posZ);
			seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_SLOW_UP_MOVE_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_SLOW_UP_MOVE_CHECK: {
			T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, FALSE, eMOTOR_Z_AFTER);
			bCheck &= CheckInPos(posZ);
			if (bCheck) {
				wRetry = 0;
				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_UP_MOVE;
			}
			else {
				if (GetMoveTimeout(posZ)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, posZ, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_SLOW_UP_MOVE;
				}
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_UP_MOVE: {
			//<< MCC
			MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_MOVE_Z_READY_POS_1] = 1;
			//>>

			T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, FALSE, eMOTOR_Z_UP);
			PTPMove(posZ);
			seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_UP_MOVE_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_UP_MOVE_CHECK: {
			T_POS posZ = GetGlassTransferPosZ(eMT_ORG_GLASS, FALSE, eMOTOR_Z_UP);
			bCheck &= CheckInPos(posZ);
			if (bCheck) {
				////<< MCC
				//MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_COMPONENT_OUT_GLASS] = 0;
				//CIPVar var;
				//var.SetChildItem("On", 0);
				//g_ipClient.PostIPCommand("UI", "MCC", "GTComponentIn", var);
				//MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_MOVE_Z_READY_POS_1] = 0;
				////>>

				wRetry = 0;
				// LJY 수정 2024.04.09 Blow 동작시 Paper & Glass 구분없이 Blow 동작하도록 수정
				SetGlassTransferBlow(FALSE); 
				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Y_LOADING_MOVE;
			}
			else {
				if (GetMoveTimeout(posZ)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, posZ, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_Z_UP_MOVE;
				}
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_Y_LOADING_MOVE: {
			//PTPMove(sm.MotorPos_1->Glass_TR.Load_Y); // Post 개념으로 보내놓고 체크는 하지 않음.
			seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_SEND_COMPLETED;

			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_SEND_COMPLETED: {
			if (sm.DeviceParameter->tUse.bPCLinkerUse)
				pThis.ToLower.fSend_Complete = TRUE;

			seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_RECV_COMPLETED_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_RECV_COMPLETED_CHECK: {
			if (pLowerSCT.ToUpper.fReceive_Complete || !sm.DeviceParameter->tUse.bPCLinkerUse)
			//2024.08.01 PSS TODO CHECK
			{
				//<< MCC
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_COMPONENT_OUT_GLASS] = 0;
				CIPVar var;
				var.SetChildItem("On", 0);
				g_ipClient.PostIPCommand("UI", "MCC", "GTComponentIn", var);
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_MOVE_Z_READY_POS_1] = 0;
				//>>
				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_SEND_INIT;
			}
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_SEND_INIT: {
			//pThis.Glass.bMaterial = FALSE;
			memset(&pThis.Glass, 0x00, sizeof(pThis.Glass));
			SetToLowerInit(&pThis);
			seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_RECV_INIT_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_RECV_INIT_CHECK: {
			if (IsInitToUpper(&pLowerSCT) || !sm.DeviceParameter->tUse.bPCLinkerUse)
				seqStep = S_GLASS_TR_SEQ_GLASS_UNLOAD_END;

			break;
		}
		case S_GLASS_TR_SEQ_GLASS_UNLOAD_END: {
			pThis.Status.wMaterialType = eMT_NONE;
			pThis.Status.wTargetIndex = eMODULE_NONE;
			// 2024.07.01 LBH - Glass Input
			GlassOutput();
			seqStep = S_GLASS_TR_SEQ_INIT;
			break;
		}

		case S_GLASS_TR_SEQ_PAPER_UNLOAD_START: {
			SetToUpperInit(&pThis);
			SetToLowerInit(&pThis);
			pThis.Status.wTargetIndex = eMODULE_NONE;

			pThis.Status.wTargetIndex = pLowerEV.eModule;
			seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_SEND_ABLE;
			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_SEND_ABLE: {
			////<< MCC
			//MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_OUT_WAIT_TIME_PAPER] = 1;
			////>>
			pThis.ToLower.fSend_Able = TRUE;
			seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_RECV_ABLE_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_RECV_ABLE_CHECK: {
			bCheck = pLowerEV.Status.wTargetIndex == pThis.eModule;
			bCheck = pLowerEV.ToUpper.fReceive_Able;

			if (bCheck)
				seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_RECV_START_CHECK;

			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_RECV_START_CHECK: {
			if (pLowerEV.ToUpper.fReceive_Start) {
				//<< MCC
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_OUT_WAIT_TIME_PAPER] = 0;
				//>>

				seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_SEND_START;
			}

			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_SEND_START: {
			wRetry = 0;
			wRepeat = 0;
			pThis.ToLower.fSend_Start = TRUE;
			seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_INIT_MOVE;
			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_INIT_MOVE: {
			T_POS posZ = GetGlassTransferPosZ(eMT_PAPER, FALSE, eMOTOR_Z_UP);
			PTPMove(posZ);
			seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_INIT_MOVE_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_INIT_MOVE_CHECK: {
			T_POS posZ = GetGlassTransferPosZ(eMT_PAPER, FALSE, eMOTOR_Z_UP);
			if (CheckInPos(posZ)) {
				//24.07.11 KWR FDC 정합성 수정 중
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_COMPONENT_OUT_PAPER] = 1;
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_COMPONENT_IN_PAPER] = 1;
				wRetry = 0;
				seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Y_EL_POS_MOVE;
			}
			else {
				if (GetMoveTimeout(posZ)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, posZ, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_INIT_MOVE;
				}
			}

			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_Y_EL_POS_MOVE: {
			T_POS posZ = GetGlassTransferPosZ(eMT_PAPER, FALSE, eMOTOR_Z_UP);
			bCheck &= CheckInPos(posZ);
			if (bCheck) {
				//<< MCC, 2024.07.09 edit by kkw
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_MOVE_Y_LE01_ULD_POS] = 1;
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_IN_WAIT_TIME_PAPER] = 0;
				//24.07.11 KWR FDC 정합성 수정 중
				/*MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_COMPONENT_OUT_PAPER] = 1;
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_COMPONENT_IN_PAPER] = 1;*/
				//>>

				PTPMove(sm.MotorPos_1->Glass_TR.Unload_Paper_Y);
				seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Y_EL_POS_MOVE_CHECK;
			}
			else {
				if (++wRepeat > MOVE_RETRY) {
					//wRepeat = 0;
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
					SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_INIT_MOVE;
			}
			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_Y_EL_POS_MOVE_CHECK: {
			bCheck &= CheckInPos(sm.MotorPos_1->Glass_TR.Unload_Paper_Y);
			if (bCheck) {
				//<< MCC
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_MOVE_Y_LE01_ULD_POS] = 0;
				//>>

				wRetry = 0;
				seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_DOWN_MOVE;
			}
			else {
				if (GetMoveTimeout(sm.MotorPos_1->Glass_TR.Unload_Paper_Y)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Y_MOVE_FAIL, IDS_DES_NOT_USE, StepName, sm.MotorPos_1->Glass_TR.Unload_Paper_Y, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Y_EL_POS_MOVE;
				}
			}

			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_DOWN_MOVE: {
			bCheck &= CheckInPos(sm.MotorPos_1->Glass_TR.Unload_Paper_Y);
			if (bCheck) {
				//<< MCC, 2024.07.09 move by kkw (in_wait_time_paper_off)
				//MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_IN_WAIT_TIME_PAPER] = 0;
				//MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_COMPONENT_OUT_PAPER] = 1;
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_MOVE_Z_ULD_PAPER_POS] = 1;
				//MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_COMPONENT_IN_PAPER] = 1;
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_GET_PAPER_IN_PROCESS] = 1;
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_GET_PAPER_IN_COUNT] = 1;
				//>>

				T_POS posZ = GetGlassTransferPosZ(eMT_PAPER, FALSE, eMOTOR_Z_DOWN);
				PTPMove(posZ);
				seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_DOWN_MOVE_CHECK;
			}
			else {
				if (++wRepeat >= MOVE_RETRY) {
					wRepeat = 0;
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
					SetAlarm(eAI_GLASS_TRANSFER_Y_MOVE_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}
				seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Y_EL_POS_MOVE; //24.03.23 KGH ADD :Retry 추가

			}
			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_DOWN_MOVE_CHECK: {
			T_POS posZ = GetGlassTransferPosZ(eMT_PAPER, FALSE, eMOTOR_Z_DOWN);
			if (CheckInPos(posZ)) {
				//<< MCC
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_MOVE_Z_ULD_PAPER_POS] = 0;
				//<<

				wRetry = 0;
				seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_VACUUM_OFF;
			}
			else {
				if (GetMoveTimeout(posZ)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, posZ, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_DOWN_MOVE;
				}
			}
			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_VACUUM_OFF: {
			//<< MCC
			MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_VAC_OFF_2] = 1;
			//>>

			SetTimeout(&pThis);
			SetGlassTransferVacuum((eMATERIAL_TYPE)pThis.Status.wMaterialType, FALSE);
			// LJY 수정 2024.04.09 Blow 동작시 Paper & Glass 구분없이 Blow 동작하도록 수정
			SetGlassTransferBlow(TRUE);
			seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_VACUUM_OFF_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_VACUUM_OFF_CHECK: {
			if (GetGlassTransferVacuum((eMATERIAL_TYPE)pThis.Status.wMaterialType, FALSE, TRUE)) {
				if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dBlowOnWait)) {
					//<< MCC
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_VAC_OFF_2] = 0;
					//>>

					//pThis.Glass.bMaterial = FALSE;
					wRetry = 0;
					seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_UP_MOVE;
				}
			}
			else if (GetTimeoutSec(&pThis, g_pRecipe->vacuumBlowTime.dBlow)) {
				if (++wRetry > IO_RETRY) {
					wRetry = 0;
					MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
					SetAlarm(eAI_GLASS_TRANSFER_PAPER_VACUUM_ON_FAIL, IDS_DES_NOT_USE, StepName, _pCmd);
				}

				seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_VACUUM_OFF;
			}

			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_UP_MOVE: {
			//<< MCC
			MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_MOVE_Z_READY_POS_2] = 1;
			//>>

			T_POS posZ = GetGlassTransferPosZ(eMT_PAPER, FALSE, eMOTOR_Z_UP);
			PTPMove(posZ);
			seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_UP_MOVE_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_UP_MOVE_CHECK: {
			T_POS posZ = GetGlassTransferPosZ(eMT_PAPER, FALSE, eMOTOR_Z_UP);
			if (CheckInPos(posZ)) {
				//<< MCC
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_PUT_MOVE_Z_READY_POS_2] = 0;
				MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_COMPONENT_OUT_PAPER] = 0;
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_COMPONENT_IN_PAPER] = 0;
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_GET_PAPER_IN_PROCESS] = 0;
				MCC_LAYER_LE01->A[eMCC_ACTION_LD_LE_01_GET_PAPER_IN_COUNT] = 0;
				//>>

				wRetry = 0;
				
				SetGlassTransferBlow(FALSE);
				seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Y_LOADING_MOVE;
			}
			else {
				if (GetMoveTimeout(sm.MotorPos_1->Glass_TR.Wait_Z)) {
					if (++wRetry >= MOVE_RETRY) {
						wRetry = 0;
						MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_ALARM] = 1;
						SetAlarm(eAI_GLASS_TRANSFER_Z_MOVE_FAIL, IDS_DES_NOT_USE, StepName, sm.MotorPos_1->Glass_TR.Wait_Z, _pCmd);
					}
					seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_Z_UP_MOVE;
				}
			}

			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_Y_LOADING_MOVE: {
			//<< MCC
			MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_MOVE_Y_LD_POS] = 1;
			//>>

			PTPMove(sm.MotorPos_1->Glass_TR.Load_Y); // Post 개념  
			seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_SEND_COMPLETED;
			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_SEND_COMPLETED: {
			pThis.ToLower.fSend_Complete = TRUE;
			seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_RECV_COMPLETED_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_RECV_COMPLETED_CHECK: {
			if (pLowerEV.ToUpper.fReceive_Complete)
				seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_SEND_INIT;
			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_SEND_INIT: {
			pThis.Glass.bMaterial = FALSE;
			SetToLowerInit(&pThis);
			seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_RECV_INIT_CHECK;
			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_RECV_INIT_CHECK: {
			if (IsInitToUpper(&pLowerEV))
				seqStep = S_GLASS_TR_SEQ_PAPER_UNLOAD_END;

			break;
		}
		case S_GLASS_TR_SEQ_PAPER_UNLOAD_END: {
			//<< MCC
			MCC_LAYER_TR01->A[eMCC_ACTION_LD_TR_01_MOVE_Y_LD_POS] = 0;
			//>>

			pThis.Status.wMaterialType = eMT_NONE;
			pThis.Status.wTargetIndex = eMODULE_NONE;
			seqStep = S_GLASS_TR_SEQ_END;
			break;
		}
		case S_GLASS_TR_SEQ_END: {
			seqStep = S_GLASS_TR_SEQ_INIT;
			break;
		}
	}
}
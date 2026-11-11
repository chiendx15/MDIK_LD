#include "pch.h"
#include "SharedFunctionLoader.h"


void SetPIO_EQ_CONNECT(BOOL _bOn) {
	sm.IO->tOut_NEW.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_1 = _bOn;
}

void SetPIO_EQ_REQUEST(BOOL _bOn) {
	sm.IO->tOut_NEW.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_2 = _bOn;
}

void SetPIO_EQ_RUN(BOOL _bOn) {
	sm.IO->tOut_NEW.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_3 = _bOn;
}

void SetPIO_EQ_ERROR(BOOL _bOn) {
	sm.IO->tOut_NEW.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_4 = _bOn;
}

void SetPIO_EQ_INPUT(BOOL _bOn) {
	sm.IO->tOut_NEW.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_5 = _bOn;
}

void SetPIO_EQ_OUTPUT(BOOL _bOn) {
	sm.IO->tOut_NEW.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_6 = _bOn;
}

void SetPIO_EQ_LOAD_REQUEST(BOOL _bOn) {
	sm.IO->tOut_NEW.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_7 = _bOn;
}

void SetPIO_EQ_UNLOAD_REQUEST(BOOL _bOn) {
	sm.IO->tOut_NEW.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_8 = _bOn;
}

void SetPIO_EQ_ALL_OFF() {
	SetPIO_EQ_CONNECT(FALSE);
	SetPIO_EQ_REQUEST(FALSE);
	SetPIO_EQ_RUN(FALSE);
	SetPIO_EQ_ERROR(FALSE);
	SetPIO_EQ_INPUT(FALSE);
	SetPIO_EQ_OUTPUT(FALSE);
	SetPIO_EQ_LOAD_REQUEST(FALSE);
	SetPIO_EQ_UNLOAD_REQUEST(FALSE);
}

BOOL GetPIO_EQ_CONNECT(BOOL _bOn) {
	return sm.IO->tOut_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_1 == _bOn;
}

BOOL GetPIO_EQ_REQUEST(BOOL _bOn) {
	return sm.IO->tOut_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_2 == _bOn;
}

BOOL GetPIO_EQ_RUN(BOOL _bOn) {
	return sm.IO->tOut_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_3 == _bOn;
}

BOOL GetPIO_EQ_ERROR(BOOL _bOn) {
	return sm.IO->tOut_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_4 == _bOn;
}

BOOL GetPIO_EQ_INPUT(BOOL _bOn) {
	return sm.IO->tOut_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_5 == _bOn;
}

BOOL GetPIO_EQ_OUTPUT(BOOL _bOn) {
	return sm.IO->tOut_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_6 == _bOn;
}

BOOL GetPIO_EQ_LOAD_REQUEST(BOOL _bOn) {
	return sm.IO->tOut_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_7 == _bOn;
}

BOOL GetPIO_EQ_UNLOAD_REQUEST(BOOL _bOn) {
	return sm.IO->tOut_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_OUTPUT_8 == _bOn;
}

BOOL GetPIO_EQ_ALL_OFF() {
	BOOL bReturn = TRUE;

	bReturn &= GetPIO_EQ_CONNECT(FALSE);
	bReturn &= GetPIO_EQ_REQUEST(FALSE);
	bReturn &= GetPIO_EQ_RUN(FALSE);
	bReturn &= GetPIO_EQ_ERROR(FALSE);
	bReturn &= GetPIO_EQ_INPUT(FALSE);
	bReturn &= GetPIO_EQ_OUTPUT(FALSE);
	bReturn &= GetPIO_EQ_LOAD_REQUEST(FALSE);
	bReturn &= GetPIO_EQ_UNLOAD_REQUEST(FALSE);

	return bReturn;
}


BOOL GetPIO_BF_CONNECT(BOOL _bOn) {
	return sm.IO->tIn_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_INPUT_1 == _bOn;
}

BOOL GetPIO_BF_REQUEST(BOOL _bOn) {
	return sm.IO->tIn_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_INPUT_2 == _bOn;
}

BOOL GetPIO_BF_RUN(BOOL _bOn) {
	return sm.IO->tIn_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_INPUT_3 == _bOn;
}

BOOL GetPIO_BF_ERROR(BOOL _bOn) {
	return sm.IO->tIn_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_INPUT_4 == _bOn;
}

BOOL GetPIO_BF_INPUT(BOOL _bOn) {
	return sm.IO->tIn_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_INPUT_5 == _bOn;
}

BOOL GetPIO_BF_OUTPUT(BOOL _bOn) {
	return sm.IO->tIn_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_INPUT_6 == _bOn;
}

BOOL GetPIO_BF_LOAD(BOOL _bOn) {
	return sm.IO->tIn_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_INPUT_7 == _bOn;
}

BOOL GetPIO_BF_UNLOAD(BOOL _bOn) {
	return sm.IO->tIn_CRT.fLD_AGV_INTRANSFEREFACE_SIGNAL_INPUT_8 == _bOn;
}

BOOL GetPIO_BF_ALL_OFF() {
	BOOL bReturn = TRUE;

	bReturn &= GetPIO_BF_CONNECT(FALSE);
	bReturn &= GetPIO_BF_REQUEST(FALSE);
	bReturn &= GetPIO_BF_RUN(FALSE);
	bReturn &= GetPIO_BF_ERROR(FALSE);
	bReturn &= GetPIO_BF_INPUT(FALSE);
	bReturn &= GetPIO_BF_OUTPUT(FALSE);
	bReturn &= GetPIO_BF_LOAD(FALSE);
	bReturn &= GetPIO_BF_UNLOAD(FALSE);

	return bReturn;
}

BOOL GetConveyorAlarm(eMODULEs _eModule) {
	BOOL bAlarm = FALSE;

	// MC
	bAlarm |= sm.Alarm->ALARMs[eAL_GPS1_ECLB_OFF].bOccurred;
	bAlarm |= sm.Alarm->ALARMs[eAI_UPS_MC01A_OFF].bOccurred;
	bAlarm |= sm.Alarm->ALARMs[eAI_UPS_MC01B_OFF].bOccurred;
	bAlarm |= sm.Alarm->ALARMs[eAI_GPS_EMO_MC31_OFF].bOccurred;
	bAlarm |= sm.Alarm->ALARMs[eAl_UPS1_MAIN_ELCB_ON].bOccurred;
	// EMS
	bAlarm |= sm.Alarm->ALARMs[eAI_EMO01_ON].bOccurred;
	bAlarm |= sm.Alarm->ALARMs[eAI_EMO02_ON].bOccurred;
	bAlarm |= sm.Alarm->ALARMs[eAI_EMO03_ON].bOccurred;
	bAlarm |= sm.Alarm->ALARMs[eAI_EMO04_ON].bOccurred;
	bAlarm |= sm.Alarm->ALARMs[eAI_EMO05_ON].bOccurred;
	bAlarm |= sm.Alarm->ALARMs[eAI_EMO06_ON].bOccurred;
	// LightCurtain
	bAlarm |= sm.Alarm->ALARMs[eAI_GLASS_BOX_CV_LIGHT_CURTAIN_NORMAL].bOccurred;
	// Door
	bAlarm |= sm.IOs->LD.In.fLD_DOOR01_SIGNAL;
	bAlarm |= sm.IOs->LD.In.fLD_DOOR02_SIGNAL;
	// Inverter
	bAlarm |= !Serial::Inverter::GetInverterReady(_eModule);
	// PIO Alarm
	bAlarm |= (_eModule == eMODULE_PC1_LOAD_CONVEYOR_1 ? sm.Alarm->ALARMs[eAI_LOAD_CONVEYOR1_AGV_PIO_TIMEOUT].bOccurred : sm.Alarm->ALARMs[eAI_UNLOAD_CONVEYOR2_AGV_PIO_TIMEOUT].bOccurred);
	for (int i = 0; i < 8; i++) {
		bAlarm |= (_eModule == eMODULE_PC1_LOAD_CONVEYOR_1 ? sm.Alarm->ALARMs[eAI_LOAD_CONVEYOR1_AGV_CONNECT_ABNORMAL_ON + i].bOccurred : sm.Alarm->ALARMs[eAI_UNLOAD_CONVEYOR2_AGV_CONNECT_ABNORMAL_ON + i].bOccurred);
		bAlarm |= (_eModule == eMODULE_PC1_LOAD_CONVEYOR_1 ? sm.Alarm->ALARMs[eAI_LOAD_CONVEYOR1_AGV_CONNECT_ABNORMAL_OFF + i].bOccurred : sm.Alarm->ALARMs[eAI_UNLOAD_CONVEYOR2_AGV_CONNECT_ABNORMAL_OFF + i + i].bOccurred);
	}

	return bAlarm;
}

void SetMute(eMODULEs _eModule, BOOL _bOn, PIPCOMMAND _pCMD) {
	SetTimeout(_eModule);
	if (GetMute(_bOn)) {
		sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_COMPLETED;
	}
	else {
		sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_READY;

		CIPVar var;
		var.SetChildItem("Module", _eModule);
		var.SetChildItem("OnOff", _bOn);

		if (_pCMD)
			g_ipClient.SendIPStepCommand(_pCMD, "Sequence", NULL, "SubSequence", "Mute", var);
		else
			g_ipClient.PostIPCommand("Sequence", NULL, "SubSequence", "Mute", var);
	}
}

BOOL GetMute(BOOL _bOn) {
	BOOL bResult = TRUE;
	bResult &= sm.IO->tOut_CRT.fLD_LIGHT_CURTAIN_MUTE_A_ON == _bOn;
	bResult &= sm.IO->tOut_CRT.fLD_LIGHT_CURTAIN_MUTE_B_ON == _bOn;

	bResult &= sm.IO->tIn_CRT.fLD_GLASS_BOX_CV_LIGHT_CURTAIN_MUTING_ON_SIGNAL == _bOn;
	return bResult;
}

void PPBoxIDManualInput(eMODULEs _eModule, LPCTSTR strMessage, PIPCOMMAND _pCMD) {
	SetTimeout(_eModule);
	sm.Module[_eModule].Status.eMessageStatus = eMESSAGE_REQUEST;

	CIPVar var;
	var.SetChildItem("Module", _eModule);
	var.SetChildItem("Message", strMessage);
	var.SetChildItem("Button1", g_st[IDS_SHARED_FUNCTION_RETRY]);
	var.SetChildItem("Button2", g_st[IDS_SHARED_FUNCTION_MANUAL_INSERT]);
	var.SetChildItem("Button3", g_st[IDS_SHARED_FUNCTION_DUPLICATED_ACTION]);
	if (_pCMD)
		g_ipClient.SendIPStepCommand(_pCMD, "Sequence", NULL, "Sequence", "BarCodeMessage", var);
	else
		g_ipClient.PostIPCommand("Sequence", NULL, "Sequence", "BarCodeMessage", var);

	SetAlarm(eAI_LD_CONVEYOR_BARCODE_READ_FAIL, IDS_DES_NOT_USE, strMessage);
}

// LJY 추가 53R 2024.04.09
BOOL GetAnkleDetectionSensorLD(BOOL bOn) {
	BOOL bReturn = FALSE;
	
	bReturn |= sm.IO->tIn_CRT.fLD_LIGHT_CURTAINANKLE_DETECTION_01_LEFT == bOn;
	bReturn |= sm.IO->tIn_CRT.fLD_ANKLE_DETECTION_01 == bOn;
	bReturn |= sm.IO->tIn_CRT.fLD_ANKLE_DETECTION_02 == bOn;
	bReturn |= sm.IO->tIn_CRT.fLD_ANKLE_DETECTION_03 == bOn;
	bReturn |= sm.IO->tIn_CRT.fLD_ANKLE_DETECTION_04 == bOn;
	bReturn |= sm.IO->tIn_CRT.fLD_ANKLE_DETECTION_05 == bOn;
	bReturn |= sm.IO->tIn_CRT.fLD_ANKLE_DETECTION_06 == bOn;
	bReturn |= sm.IO->tIn_CRT.fLD_ANKLE_DETECTION_07 == bOn;
	bReturn |= sm.IO->tIn_CRT.fLD_ANKLE_DETECTION_08 == bOn;
	bReturn |= sm.IO->tIn_CRT.fLD_ANKLE_DETECTION_09 == bOn;
	bReturn |= sm.IO->tIn_CRT.fLD_ANKLE_DETECTION_10 == bOn;
	bReturn |= sm.IO->tIn_CRT.fLD_ANKLE_DETECTION_11 == bOn;
	bReturn |= sm.IO->tIn_CRT.fLD_LIGHT_CURTAINANKLE_DETECTION_02_RIGHT == bOn;
	bReturn |= sm.IO->tIn_CRT.fLD_LIGHT_CURTAINANKLE_DETECTION_03_RIGHT == bOn;
	// 2024.07.12 move from sct by kkw
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_06 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_07 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_08 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_09 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_10 == bOn;

	return bReturn;
}

BOOL GetAnkleDetectionSensorSCT(BOOL bOn) {
	BOOL bReturn = FALSE;
	
	//Left
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_01 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_02 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_03 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_04 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_05 == bOn;
	//// 2024.07.12 move to ld by kkw
	//bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_06 == bOn;
	//bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_07 == bOn;
	//bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_08 == bOn;
	//bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_09 == bOn;
	//bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_LEFT_10 == bOn;

	//Right
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_RIGHT_01 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_RIGHT_02 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_RIGHT_03 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_RIGHT_04 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_RIGHT_05 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_RIGHT_06 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_RIGHT_07 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_RIGHT_08 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_RIGHT_09 == bOn;
	bReturn |= sm.IOs->SCT.In.fSCT_ANKLE_DETECTION_RIGHT_10 == bOn;

	//24.07.12 KWR 환경 안전 사양 적용
	//CCTL
	bReturn |= sm.IOs->CCTL.In.fANKLE_DETECTION_01 == bOn;
	bReturn |= sm.IOs->CCTL.In.fANKLE_DETECTION_02 == bOn;
	bReturn |= sm.IOs->CCTL.In.fANKLE_DETECTION_03 == bOn;
	bReturn |= sm.IOs->CCTL.In.fANKLE_DETECTION_04 == bOn;

	//CCTR
	bReturn |= sm.IOs->CCTR.In.fANKLE_DETECTION_01 == bOn;
	bReturn |= sm.IOs->CCTR.In.fANKLE_DETECTION_02 == bOn;
	bReturn |= sm.IOs->CCTR.In.fANKLE_DETECTION_03 == bOn;
	bReturn |= sm.IOs->CCTR.In.fANKLE_DETECTION_04 == bOn;

	return bReturn;
}

BOOL GetPPBoxSensor(eMODULEs _eModule, BOOL bOn)
{
	if (sm.DeviceParameter->tUse.bDryRun) {
		return GetTimeoutSec(_eModule, sm.DeviceParameter->tTime.dConveyorMove);
		//return TRUE;
	}

	switch (_eModule) 
	{
		case eMODULE_PC1_LOAD_CONVEYOR_1: 
		{
			return sm.IO->tIn_CRT.fLD_GLASS_BOX_CV1_CHECK_1 == bOn;
		}
		break;

		case eMODULE_PC1_LOAD_CONVEYOR_2: 
		{
			// LJY 수정 56R 2024.04.15 
			return sm.IO->tIn_CRT.fLD_GLASS_BOX_CV2_CHECK_1 == bOn &&
				   sm.IO->tIn_CRT.fLD_CV2_PPBOX_FORWARD_CHECK_1 == bOn &&
				   sm.IO->tIn_CRT.fLD_CV2_PPBOX_FORWARD_CHECK_2 == bOn;
		}
		break;
	
		case eMODULE_PC1_ELEVATOR_CONVEYOR: 
		{
			return sm.IO->tIn_CRT.fLD_GLASS_BOX_CV3_CHECK_1 == bOn;
		}
		break;

		case eMODULE_PC1_UNLOAD_CONVEYOR_1: 
		{
			return sm.IO->tIn_CRT.fLD_GLASS_BOX_CV4_CHECK_1 == bOn;
		}
		break;

		case eMODULE_PC1_UNLOAD_CONVEYOR_2: 
		{
			return sm.IO->tIn_CRT.fLD_GLASS_BOX_CV5_CHECK_1 == bOn;
		}
		break;
	}

	return TRUE;
}


BOOL GetPPBoxSensor_Conveyor2Front(BOOL bOn) {
	if (sm.DeviceParameter->tUse.bDryRun)
		return TRUE;

	return sm.IO->tIn_CRT.fLD_GLASS_BOX_CV2_CHECK_1 == bOn;
}

void SetConveyor_Stoper(eMODULEs _eModule, BOOL bUp) {
	if (bUp < 0) {
		return;
	}

	DWORD* pStoperUp	= nullptr;
	DWORD* pStoperDown	= nullptr;
	switch (_eModule) {
		case eMODULE_PC1_LOAD_CONVEYOR_1: 
		{
			sm.IO->tOut_NEW.fLD_GLASS_BOX_CV1_STOPPER_CYLINDER_ON_SOL_1 = bUp;
			sm.IO->tOut_NEW.fLD_GLASS_BOX_CV1_STOPPER_CYLINDER_OFF_SOL_1 = !bUp;
			break;
		}
		break;

		case eMODULE_PC1_LOAD_CONVEYOR_2: 
		{
			sm.IO->tOut_NEW.fLD_GLASS_BOX_CV2_STOPPER_CYLINDER_ON_SOL_1 = bUp;
			sm.IO->tOut_NEW.fLD_GLASS_BOX_CV2_STOPPER_CYLINDER_OFF_SOL_1 = !bUp;
			break;
		}
		break;
	
		case eMODULE_PC1_ELEVATOR_CONVEYOR: 
		{
			sm.IO->tOut_NEW.fLD_GLASS_BOX_CV3_STOPPER_CYLINDER_ON_SOL_1 = bUp;
			sm.IO->tOut_NEW.fLD_GLASS_BOX_CV3_STOPPER_CYLINDER_OFF_SOL_1 = !bUp;
			break;
		}
		break;

		case eMODULE_PC1_UNLOAD_CONVEYOR_1: 
		{
			sm.IO->tOut_NEW.fLD_GLASS_BOX_CV4_STOPPER_CYLINDER_ON_SOL_1 = bUp;
			sm.IO->tOut_NEW.fLD_GLASS_BOX_CV4_STOPPER_CYLINDER_OFF_SOL_1 = !bUp;
			break;
		}
		break;

		case eMODULE_PC1_UNLOAD_CONVEYOR_2: 
		{
			//<< 2024.05.23 LBH - Down동작 시 Mute 켜져 있을때만 동작 할 수 있도록 수정.
			if (GetMute(FALSE) && !bUp) {
				if (!sm.Sequence->fAutoMode || sm.Sequence->fCycleStop) {
					IPMessage(g_st[IDS_SHARED_FUNCTION_LD_LIGHT_CURTAIN_DETECTED]);
				}
				break;
			}
			//>>

			sm.IO->tOut_NEW.fLD_GLASS_BOX_CV5_STOPPER_CYLINDER_ON_SOL_1 = bUp;
			sm.IO->tOut_NEW.fLD_GLASS_BOX_CV5_STOPPER_CYLINDER_OFF_SOL_1 = !bUp;
			break;
		}
	}
}
BOOL GetConveyor_Stoper(eMODULEs _eModule, BOOL bUp, BOOL bIn) {
	BOOL bReturn = TRUE;
	switch (_eModule) {
		case eMODULE_PC1_LOAD_CONVEYOR_1: 
		{
			bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_CV1_STOPPER_CYLINDER_ON_SOL_1	 == bUp;
			bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_CV1_STOPPER_CYLINDER_OFF_SOL_1	 == !bUp;
			if (bIn) {
				bReturn &= sm.IO->tIn_CRT.fLD_GLASS_BOX_IN_CV1_STOPPER_CYLINDER_FWD  == bUp;
				bReturn &= sm.IO->tIn_CRT.fLD_GLASS_BOX_IN_CV1_STOPPER_CYLINDER_BWD  == !bUp;
			}
			break;
		}
		break;

		case eMODULE_PC1_LOAD_CONVEYOR_2: 
		{
			bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_CV2_STOPPER_CYLINDER_ON_SOL_1	 == bUp;
			bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_CV2_STOPPER_CYLINDER_OFF_SOL_1	 == !bUp;
			if (bIn) {
				bReturn &= sm.IO->tIn_CRT.fLD_GLASS_BOX_IN_CV2_STOPPER_CYLINDER_FWD  == bUp;
				bReturn &= sm.IO->tIn_CRT.fLD_GLASS_BOX_IN_CV2_STOPPER_CYLINDER_BWD  == !bUp;
			}
			break;
		}
		break;
	
		case eMODULE_PC1_ELEVATOR_CONVEYOR: 
		{
			bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_CV3_STOPPER_CYLINDER_ON_SOL_1	 == bUp;
			bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_CV3_STOPPER_CYLINDER_OFF_SOL_1	 == !bUp;
			if (bIn) {
				bReturn &= sm.IO->tIn_CRT.fLD_GLASS_BOX_IN_CV3_STOPPER_CYLINDER_FWD  == bUp;
				bReturn &= sm.IO->tIn_CRT.fLD_GLASS_BOX_IN_CV3_STOPPER_CYLINDER_BWD  == !bUp;
			}
			break;
		}
		break;

		case eMODULE_PC1_UNLOAD_CONVEYOR_1: 
		{
			bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_CV4_STOPPER_CYLINDER_ON_SOL_1	 == bUp;
			bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_CV4_STOPPER_CYLINDER_OFF_SOL_1	 == !bUp;
			if (bIn) {
				bReturn &= sm.IO->tIn_CRT.fLD_GLASS_BOX_IN_CV4_STOPPER_CYLINDER_FWD  == bUp;
				bReturn &= sm.IO->tIn_CRT.fLD_GLASS_BOX_IN_CV4_STOPPER_CYLINDER_BWD  == !bUp;
			}
			break;
		}
		break;

		case eMODULE_PC1_UNLOAD_CONVEYOR_2: 
		{
			bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_CV5_STOPPER_CYLINDER_ON_SOL_1	 == bUp;
			bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_CV5_STOPPER_CYLINDER_OFF_SOL_1	 == !bUp;
			if (bIn) {
				bReturn &= sm.IO->tIn_CRT.fLD_GLASS_BOX_IN_CV5_STOPPER_CYLINDER_FWD  == bUp;
				bReturn &= sm.IO->tIn_CRT.fLD_GLASS_BOX_IN_CV5_STOPPER_CYLINDER_BWD  == !bUp;
			}
			break;
		}
	}
	return bReturn;
}
void SetPPBoxAlignerXY(BOOL bFWD, ePPBOX_ALIGNER ePPBoxAlign) {
	if ((ePPBoxAlign == ePPBOX_ALIGNER_X_L) || (ePPBoxAlign == ePPBOX_ALIGNER_ALL)) {
		sm.IO->tOut_NEW.fLD_GLASS_BOX_ALIGN_L_CYLINDER_ON_SOL = bFWD;
		sm.IO->tOut_NEW.fLD_GLASS_BOX_ALIGN_L_CYLINDER_OFF_SOL = !bFWD;
	}

	if ((ePPBoxAlign == ePPBOX_ALIGNER_X_R) || (ePPBoxAlign == ePPBOX_ALIGNER_ALL)) {
		sm.IO->tOut_NEW.fLD_GLASS_BOX_ALIGN_R_CYLINDER_ON_SOL = bFWD;
		sm.IO->tOut_NEW.fLD_GLASS_BOX_ALIGN_R_CYLINDER_OFF_SOL = !bFWD;
	}

	if ((ePPBoxAlign == ePPBOX_ALIGNER_Y) || (ePPBoxAlign == ePPBOX_ALIGNER_ALL)) {
		sm.IO->tOut_NEW.fLD_PPBOX_ALIGN_FRONT_FWD_SOL = bFWD;
		sm.IO->tOut_NEW.fLD_PPBOX_ALIGN_FRONT_BWD_SOL = !bFWD;
	}
}

BOOL GetPPBoxAlignerXY(BOOL bFWD, BOOL bIn, ePPBOX_ALIGNER ePPBoxAlign) {
	BOOL bReturn = TRUE;

	// KES 수정 R39 Version
	if ((ePPBoxAlign == ePPBOX_ALIGNER_X_L) || (ePPBoxAlign == ePPBOX_ALIGNER_ALL)) {
		bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_ALIGN_L_CYLINDER_ON_SOL	== bFWD;
		bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_ALIGN_L_CYLINDER_OFF_SOL	== !bFWD;
	}

	if ((ePPBoxAlign == ePPBOX_ALIGNER_X_R) || (ePPBoxAlign == ePPBOX_ALIGNER_ALL)) {
		bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_ALIGN_R_CYLINDER_ON_SOL	== bFWD;
		bReturn &= sm.IO->tOut_CRT.fLD_GLASS_BOX_ALIGN_R_CYLINDER_OFF_SOL	== !bFWD;
	}

	if ((ePPBoxAlign == ePPBOX_ALIGNER_Y) || (ePPBoxAlign == ePPBOX_ALIGNER_ALL)) {
		bReturn &= sm.IO->tOut_CRT.fLD_PPBOX_ALIGN_FRONT_FWD_SOL == bFWD;
		bReturn &= sm.IO->tOut_CRT.fLD_PPBOX_ALIGN_FRONT_BWD_SOL == !bFWD;
	}

	if (bIn) {
		if ((ePPBoxAlign == ePPBOX_ALIGNER_X_L) || (ePPBoxAlign == ePPBOX_ALIGNER_ALL)) {
			bReturn &= sm.IO->tIn_CRT.fLD_PPBOX_ALIGN_L1_CYLINDER_FWD == bFWD;
			bReturn &= sm.IO->tIn_CRT.fLD_PPBOX_ALIGN_L1_CYLINDER_BWD == !bFWD;
			bReturn &= sm.IO->tIn_CRT.fLD_PPBOX_ALIGN_L2_CYLINDER_FWD == bFWD;
			bReturn &= sm.IO->tIn_CRT.fLD_PPBOX_ALIGN_L2_CYLINDER_BWD == !bFWD;
		}

		if ((ePPBoxAlign == ePPBOX_ALIGNER_X_R) || (ePPBoxAlign == ePPBOX_ALIGNER_ALL)) {
			bReturn &= sm.IO->tIn_CRT.fLD_PPBOX_ALIGN_R1_CYLINDER_FWD == bFWD;
			bReturn &= sm.IO->tIn_CRT.fLD_PPBOX_ALIGN_R1_CYLINDER_BWD == !bFWD;
			bReturn &= sm.IO->tIn_CRT.fLD_PPBOX_ALIGN_R2_CYLINDER_FWD == bFWD;
			bReturn &= sm.IO->tIn_CRT.fLD_PPBOX_ALIGN_R2_CYLINDER_BWD == !bFWD;
		}

		if ((ePPBoxAlign == ePPBOX_ALIGNER_Y) || (ePPBoxAlign == ePPBOX_ALIGNER_ALL)) {
			bReturn &= sm.IO->tIn_CRT.fLD_PPBOX_ALIGN_FRONT_CYLINDER_FWD == bFWD;
			bReturn &= sm.IO->tIn_CRT.fLD_PPBOX_ALIGN_FRONT_CYLINDER_BWD == !bFWD;
		}
	}

	return bReturn;
}

void SetPPBoxAlignerZ(BOOL bUp) {
	sm.IO->tOut_NEW.fLD_PPBOX_ALIGN_FRONT_UP_SOL = bUp;
	sm.IO->tOut_NEW.fLD_PPBOX_ALIGN_FRONT_DOWN_SOL = !bUp;
}

BOOL GetPPBoxAlignerZ(BOOL bUp, BOOL bIn) {
	BOOL bReturn = TRUE;
	bReturn &= sm.IO->tOut_CRT.fLD_PPBOX_ALIGN_FRONT_UP_SOL == bUp;
	bReturn &= sm.IO->tOut_CRT.fLD_PPBOX_ALIGN_FRONT_DOWN_SOL == !bUp;

	if (bIn) {
		bReturn &= sm.IO->tIn_CRT.fLD_PPBOX_ALIGN_FRONT_CYLINDER_UP == bUp;
		bReturn &= sm.IO->tIn_CRT.fLD_PPBOX_ALIGN_FRONT_CYLINDER_DOWN == !bUp;
	}

	return bReturn;
}

BOOL GetPPBoxManualInStart() {
	//<< 2024.05.01 LBH - Dry Run이고 Switch 미사용인 경우
	if (sm.DeviceParameter->tUse.bDryRun && !sm.DeviceParameter->tUse.bPPBoxInOutSwitchUse) {
		return TRUE;
	}
	//>>

	return sm.IO->tIn_CRT.fLD_IN_PPBOX_MANUAL_OP_PANEL_IN_OUT_SWITCH == TRUE;
}

BOOL GetPPBoxManualInCompleted() {
	//<< 2024.05.01 LBH - Dry Run이고 Switch 미사용인 경우
	if (sm.DeviceParameter->tUse.bDryRun && !sm.DeviceParameter->tUse.bPPBoxInOutSwitchUse) {
		return TRUE;
	}
	//>>

	return sm.IO->tIn_CRT.fLD_IN_PPBOX_MANUAL_OP_PANEL_COMPLETE_SWITCH == TRUE;
}

BOOL GetPPBoxManualOutStart() {
	//<< 2024.05.01 LBH - Dry Run이고 Switch 미사용인 경우
	if (sm.DeviceParameter->tUse.bDryRun && !sm.DeviceParameter->tUse.bPPBoxInOutSwitchUse) {
		return TRUE;
	}
	//>>

	return sm.IO->tIn_CRT.fLD_OUT_PPBOX_MANUAL_OP_PANEL_IN_OUT_SWITCH == TRUE;
}

BOOL GetPPBoxManualOutCompleted() {
	//<< 2024.05.01 LBH - Dry Run이고 Switch 미사용인 경우
	if (sm.DeviceParameter->tUse.bDryRun && !sm.DeviceParameter->tUse.bPPBoxInOutSwitchUse) {
		return TRUE;
	}
	//>>

	return sm.IO->tIn_CRT.fLD_OUT_PPBOX_MANUAL_OP_PANEL_COMPLETE_SWITCH == TRUE;
}

void SetAGVLamp_CALL(BOOL bLoad, BOOL bOn) {
	if (bLoad)
		sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_GREEN_LAMP = bOn;
	else
		sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_GREEN_LAMP = bOn;

}

void SetAGVLamp_MANUAL_MODE(BOOL bLoad, BOOL bOn) {
	// Manual Mode 일 경우 On
	if (bLoad)
		sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_RED_LAMP = bOn;
	else
		sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_RED_LAMP = bOn;
}

void SetAGVLamp_MUTING(BOOL bLoad, BOOL bOn) {
	if (bLoad)
		sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW = bOn;
	else
		sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW = bOn;
}

BOOL GetAGVLamp_CALL(BOOL bLoad, BOOL bOn) {
	if (bLoad)
		return sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_GREEN_LAMP == bOn;
	else
		return sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_GREEN_LAMP == bOn;

}

BOOL GetAGVLamp_MANUAL_MODE(BOOL bLoad, BOOL bOn) {
	// 
	if (bLoad)
		return sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_RED_LAMP == bOn;
	else
		return sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_RED_LAMP == bOn;
}

BOOL GetAGVLamp_MUTING(BOOL bLoad, BOOL bOn) {
	if (bLoad)
		return sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW == bOn;
	else
		return sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW == bOn;
}


void SetGlassTransferCylinderPusher(BOOL bUp) {
	// KES 수정 R39 Version
	sm.IO->tOut_NEW.fLD_TRANSFER_PAPER_PUSHER_CYLINDER_ON_SOL = !bUp;
	sm.IO->tOut_NEW.fLD_TRANSFER_PAPER_PUSHER_CYLINDER_OFF_SOL = bUp;
}

BOOL GetGlassTransferCylinderPusher(BOOL bUp, BOOL bIn) {
	BOOL bReturn = TRUE;

	// KES 수정 R39 Version
	bReturn &=  sm.IO->tOut_CRT.fLD_TRANSFER_PAPER_PUSHER_CYLINDER_ON_SOL		== !bUp;
	bReturn &=  sm.IO->tOut_CRT.fLD_TRANSFER_PAPER_PUSHER_CYLINDER_OFF_SOL	== bUp;
	
	if (bIn) {
		bReturn &= sm.IO->tIn_CRT.fLD_PAPER_PUSHER_CYLINDER_FWD_1 == !bUp;
		bReturn &= sm.IO->tIn_CRT.fLD_PAPER_PUSHER_CYLINDER_BWD_1 == bUp;
		bReturn &= sm.IO->tIn_CRT.fLD_PAPER_PUSHER_CYLINDER_FWD_2 == !bUp;
		bReturn &= sm.IO->tIn_CRT.fLD_PAPER_PUSHER_CYLINDER_BWD_2 == bUp;
		bReturn &= sm.IO->tIn_CRT.fLD_PAPER_PUSHER_CYLINDER_FWD_3 == !bUp;
		bReturn &= sm.IO->tIn_CRT.fLD_PAPER_PUSHER_CYLINDER_BWD_3 == bUp;
		bReturn &= sm.IO->tIn_CRT.fLD_PAPER_PUSHER_CYLINDER_FWD_4 == !bUp;
		bReturn &= sm.IO->tIn_CRT.fLD_PAPER_PUSHER_CYLINDER_BWD_4 == bUp;
	}
	return bReturn;
}

void SetGlassTransferVacuum(eMATERIAL_TYPE eMT, BOOL bOn) {
	// KES 수정 R39 Version

	// 블로우 끈다
	sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PUSHER_PAD_BLOW_ON_OFF_SOL = FALSE;

	// 2024.4.23 - 베트남 설비에서 Vacuum B접- A접 변경 (2024.04.25 PSS EDIT )
	switch (eMT) {
		case eMT_ORG_GLASS: {
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_1_ON_SOL	= bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_1_OFF_SOL = !bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_2_ON_SOL	= bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_2_OFF_SOL = !bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_3_ON_SOL	= bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_3_OFF_SOL = !bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_4_ON_SOL	= bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_4_OFF_SOL = !bOn;
			
			//sm.IO->tOut_NEW.fLD_TRANSFER_PUSHER_VAC_AREA_1_ON_SOL = FALSE;
			//sm.IO->tOut_NEW.fLD_TRANSFER_PUSHER_VAC_AREA_1_OFF_SOL = FALSE;
			break;
		}
		case eMT_PAPER: {
			// LJY 2024.04.09 Paper Vacuum시 Transfer Vacuum 전부 On 되게 수정
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_1_ON_SOL = bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_1_OFF_SOL = !bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_2_ON_SOL = bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_2_OFF_SOL = !bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_3_ON_SOL = bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_3_OFF_SOL = !bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_4_ON_SOL = bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_4_OFF_SOL = !bOn;

			sm.IO->tOut_NEW.fLD_TRANSFER_PUSHER_VAC_AREA_1_ON_SOL = bOn;
			sm.IO->tOut_NEW.fLD_TRANSFER_PUSHER_VAC_AREA_1_OFF_SOL = !bOn;
			break;
		}
	}
}
BOOL GetGlassTransferVacuum(eMATERIAL_TYPE eMT, BOOL bOn, BOOL bIn) {
	BOOL bReturn = TRUE;

	// KES 수정 R39 Version
	// 2024.4.23 - 베트남 설비에서 Vacuum B접 - A접 변경
	BOOL bVacuum = bOn;
	switch (eMT) {
		case eMT_ORG_GLASS: {
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_1_ON_SOL	== bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_1_OFF_SOL == !bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_2_ON_SOL	== bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_2_OFF_SOL == !bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_3_ON_SOL	== bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_3_OFF_SOL == !bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_4_ON_SOL	== bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_4_OFF_SOL == !bVacuum;
			
			if (bIn && !sm.DeviceParameter->tUse.bDryRun) {
				//bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_VACUUM_MAIN_CHECK_SWITCH == bOn;
				
				//<< 이부분 IO명칭이 모호해서 설비 테스트 필요.
				bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_GLASS_PAD_1_VACUUM_ON_CHECK_SWITCH == bOn;
				bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_GLASS_PAD_2_VACUUM_ON_CHECK_SWITCH == bOn;
				bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_GLASS_PAD_3_VACUUM_ON_CHECK_SWITCH == bOn;
				bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_GLASS_PAD_4_VACUUM_ON_CHECK_SWITCH == bOn;
				//>>
			}
			break;
		}
		case eMT_PAPER: {
			// 확실하지 않음. 설비에서 확인 필요
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_1_ON_SOL	== bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_1_OFF_SOL == !bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_2_ON_SOL	== bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_2_OFF_SOL == !bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_3_ON_SOL	== bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_3_OFF_SOL == !bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_4_ON_SOL	== bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PAD_VAC_AREA_4_OFF_SOL == !bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_PUSHER_VAC_AREA_1_ON_SOL	== bVacuum;
			bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_PUSHER_VAC_AREA_1_OFF_SOL == !bVacuum;

			if (bIn && !sm.DeviceParameter->tUse.bDryRun) {
				//bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_VACUUM_MAIN_CHECK_SWITCH == bOn;

				//<< 이부분 IO명칭이 모호해서 설비 테스트 필요.
				bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_PAPER_PUSHER_VACUUM_ON_CHECK_SWITCH == bOn;
				//>>
			}
			break;
		}
	}
	return bReturn;

}
void SetGlassTransferBlow(BOOL bOn) {
	// KES 수정 R39 Version
	// LJY 수정 2024.04.15 Blow Off 동작시 Vacuum On 되는 부분 수정

	BOOL bVacuumOff = FALSE; // 2024.04.22 피커는 B접이므로 반대로 한다
	sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_1_ON_SOL	= bVacuumOff;
	sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_1_OFF_SOL	= !bVacuumOff;
	sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_2_ON_SOL	= bVacuumOff;
	sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_2_OFF_SOL	= !bVacuumOff;
	sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_3_ON_SOL	= bVacuumOff;
	sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_3_OFF_SOL	= !bVacuumOff;
	sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_4_ON_SOL	= bVacuumOff;
	sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PAD_VAC_AREA_4_OFF_SOL	= !bVacuumOff;
	sm.IO->tOut_NEW.fLD_TRANSFER_PUSHER_VAC_AREA_1_ON_SOL		= bVacuumOff;
	sm.IO->tOut_NEW.fLD_TRANSFER_PUSHER_VAC_AREA_1_OFF_SOL		= !bVacuumOff;

	sm.IO->tOut_NEW.fLD_TRANSFER_GLASS_PUSHER_PAD_BLOW_ON_OFF_SOL = bOn;
}

BOOL GetGlassTransferBlow(BOOL bOn, BOOL bIn) {
	BOOL bReturn = TRUE;

	// KES 수정 R39 Version
	// LJY 수정 2024.03.29
	bReturn &= sm.IO->tOut_CRT.fLD_TRANSFER_GLASS_PUSHER_PAD_BLOW_ON_OFF_SOL == bOn;
	if (bIn && !sm.DeviceParameter->tUse.bDryRun) {
		//bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_BLOW_MAIN_CHECK_SWITCH == bOn;
	}
	return bReturn;
}

BOOL GetMaterialSensorCheck(eMATERIAL_TYPE eMT) {
	BOOL bReturn = TRUE;
	//if (sm.DeviceParameter->tUse.bDryRun || sm.DeviceParameter->tUse.bGanziAlarmSkip)
	//	return TRUE;

	if (eMT == eMT_ORG_GLASS) {
		bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_GLASS_CHECK_1;
		bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_GLASS_CHECK_2;
	}
	else {
		bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_PAPER_CHECK_1;
		bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_PAPER_CHECK_2;
		bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_PAPER_CHECK_3;
		bReturn &= sm.IO->tIn_CRT.fLD_TRANSFER_PAPER_CHECK_4;
	}
	
	return bReturn;
}

T_POS GetGlassTransferPosZ(eMATERIAL_TYPE eMT, BOOL bLoad, eMOTOR_Z_POS_TYPE eMotorZ_Type, BOOL bAddOffset) {
	T_POS PosZ;
	if (bLoad) {
		PosZ = sm.MotorPos_1->Glass_TR.Load_Z;
		switch (eMotorZ_Type) {
			case eMOTOR_Z_UP: {
				PosZ = sm.MotorPos_1->Glass_TR.Wait_Z;
				break;
			}
			case eMOTOR_Z_BEFOR: {
				// Befor 움직일 때는 티칭 속도
				PosZ.dTarget -= sm.DeviceParameter->tDistance.dLoadBeforDistance;
				break;
			}
			case eMOTOR_Z_AFTER: {
				PosZ.dTarget -= sm.DeviceParameter->tDistance.dLoadAfterDistance;
				PosZ.dSpeed = sm.DeviceParameter->tDistance.dLoadAfterSpeed;
				break;
			}
			case eMOTOR_Z_DOWN: {
				PosZ.dSpeed = sm.DeviceParameter->tDistance.dLoadBeforSpeed;
				// 아무것도 안함.
				break;
			}
		}

		if (eMotorZ_Type != eMOTOR_Z_UP) {
			// Load때 만 하면 될듯
			if (eMT == eMT_PAPER) {
				PosZ.dTarget -= sm.DeviceParameter->tDistance.dTransferPaperCylinderDown;
			}
			PosZ.dTarget -= GetPPBoxDistance();

			if (bAddOffset) {
				PosZ.dTarget += sm.Sequence->dGlassTransferOffsetZ;

				if (PosZ.dTarget > sm.MotorPos_1->Glass_TR.Load_Z.dTarget) {
					PosZ.dTarget = sm.MotorPos_1->Glass_TR.Load_Z.dTarget;
				}
			}
		}

		// KES 2024.07.16 - Z축 Offset 추가
		if (eMotorZ_Type == eMOTOR_Z_DOWN) {
			PosZ.dTarget += g_pRecipe->pickerOffsetZ.dGlassPickerLoad;
		}
	}
	else {
		if (eMT == eMT_ORG_GLASS) {
			PosZ = sm.MotorPos_1->Glass_TR.Unload_Glass_Z;
			PosZ.dTarget -= g_pRecipe->ppbox.dGlassThickness; // 해야하는지 확인 필요.

			switch (eMotorZ_Type) {
				case eMOTOR_Z_UP: {
					PosZ = sm.MotorPos_1->Glass_TR.Wait_Z;
					break;
				}
				case eMOTOR_Z_BEFOR: {
					// 티칭 속도 그대로 사용.
					PosZ.dTarget -= sm.DeviceParameter->tDistance.dUnloadBeforDistance;
					break;
				}
				case eMOTOR_Z_AFTER: {
					PosZ.dTarget -= sm.DeviceParameter->tDistance.dUnloadAfterDistance;
					PosZ.dSpeed = sm.DeviceParameter->tDistance.dUnloadAfterSpeed;
					break;
				}
				case eMOTOR_Z_DOWN: {
					// KES 2024.07.16 - Z축 Offset 추가
					PosZ.dTarget += g_pRecipe->pickerOffsetZ.dGlassPickerUnload;
					PosZ.dSpeed = sm.DeviceParameter->tDistance.dUnloadBeforSpeed;
					// 아무것도 안함.
					break;
				}
			}
		}
		else {
			switch (eMotorZ_Type) {
				case eMOTOR_Z_UP: {
					PosZ = sm.MotorPos_1->Glass_TR.Wait_Z;
					break;
				}
				case eMOTOR_Z_BEFOR:
				case eMOTOR_Z_AFTER:
				case eMOTOR_Z_DOWN: {
					PosZ = sm.MotorPos_1->Glass_TR.Unload_Paper_Z;
					break;
				}
			}
		}
	}

	return PosZ;
}
//<--24.07.19 KWR 환경 안전 사양 적용
BOOL CheckDoorOpen_CCT(int iLine)
{
	BOOL bCheck = FALSE;
	if (iLine)
	{
		bCheck |= sm.IOs->CCTR.In.fCCT_DOOR1_SIGNAL;
		bCheck |= sm.IOs->CCTR.In.fCCT_DOOR2_SIGNAL;
		bCheck |= sm.IOs->CCTR.In.fCCT_DOOR3_SIGNAL;
		//24.07.12 kwr 환경 안전 사양 적용
		//bCheck |= sm.IOs->CCTR.In.fCCT_DOOR4_SIGNAL;
		bCheck |= sm.IOs->CCTR.In.fCCT_DOOR5_SIGNAL;
		bCheck |= sm.IOs->CCTR.In.fCCT_DOOR6_SIGNAL;
		bCheck |= sm.IOs->CCTR.In.fCCT_DOOR7_SIGNAL;
		bCheck |= sm.IOs->CCTR.In.fCCT_DOOR8_SIGNAL;

		bCheck |= sm.IOs->CCTR.In.fCCT_DUMMY_DOOR1_SIGNAL;
		bCheck |= sm.IOs->CCTR.In.fCCT_DUMMY_DOOR2_SIGNAL;
	}
	else
	{
		//24.07.12 kwr 환경 안전 사양 적용
		//bCheck |= sm.IOs->CCTL.In.fCCT_DOOR1_SIGNAL;
		bCheck |= sm.IOs->CCTL.In.fCCT_DOOR2_SIGNAL;
		bCheck |= sm.IOs->CCTL.In.fCCT_DOOR3_SIGNAL;
		bCheck |= sm.IOs->CCTL.In.fCCT_DOOR4_SIGNAL;
		bCheck |= sm.IOs->CCTL.In.fCCT_DOOR5_SIGNAL;
		bCheck |= sm.IOs->CCTL.In.fCCT_DOOR6_SIGNAL;
		bCheck |= sm.IOs->CCTL.In.fCCT_DOOR7_SIGNAL;
		bCheck |= sm.IOs->CCTL.In.fCCT_DOOR8_SIGNAL;

		bCheck |= sm.IOs->CCTL.In.fCCT_DUMMY_DOOR1_SIGNAL;
		bCheck |= sm.IOs->CCTL.In.fCCT_DUMMY_DOOR2_SIGNAL;
	}

	return bCheck;
}
BOOL CheckDoorOpen_ULD(int iLine)
{
	BOOL bCheck = FALSE;
	if (iLine)
	{
		bCheck |= sm.IOs->UNDR.In.fULD_DOOR1_SIGNAL;
		bCheck |= sm.IOs->UNDR.In.fULD_DOOR2_SIGNAL;
		bCheck |= sm.IOs->UNDR.In.fULD_DOOR3_SIGNAL;
		bCheck |= sm.IOs->UNDR.In.fULD_DOOR4_SIGNAL;
		bCheck |= sm.IOs->UNDR.In.fULD_DOOR5_SIGNAL;
		bCheck |= sm.IOs->UNDR.In.fULD_DOOR6_SIGNAL;
		bCheck |= sm.IOs->UNDR.In.fULD_DOOR7_SIGNAL;
		bCheck |= sm.IOs->UNDR.In.fULD_DOOR8_SIGNAL;

		bCheck |= sm.IOs->UNDR.In.fULD_DUMMY_DOOR1_SIGNAL;
		bCheck |= sm.IOs->UNDR.In.fULD_DUMMY_DOOR2_SIGNAL;
	}
	else
	{
		bCheck |= sm.IOs->UNDL.In.fULD_DOOR1_SIGNAL;
		bCheck |= sm.IOs->UNDL.In.fULD_DOOR2_SIGNAL;
		bCheck |= sm.IOs->UNDL.In.fULD_DOOR3_SIGNAL;
		bCheck |= sm.IOs->UNDL.In.fULD_DOOR4_SIGNAL;
		bCheck |= sm.IOs->UNDL.In.fULD_DOOR5_SIGNAL;
		bCheck |= sm.IOs->UNDL.In.fULD_DOOR6_SIGNAL;
		bCheck |= sm.IOs->UNDL.In.fULD_DOOR7_SIGNAL;
		bCheck |= sm.IOs->UNDL.In.fULD_DOOR8_SIGNAL;

		bCheck |= sm.IOs->UNDL.In.fULD_DUMMY_DOOR1_SIGNAL;
		bCheck |= sm.IOs->UNDL.In.fULD_DUMMY_DOOR2_SIGNAL;
	}

	return bCheck;
}
//-->24.07.19 KWR 환경 안전 사양 적용
BOOL CheckAllDoorCloseLD()
{
	BOOL bCheck = TRUE;
	bCheck &= ((sm.IO->tIn_CRT.fLD_DOOR01_SIGNAL == FALSE && sm.IO->tIn_CRT.fLD_DOOR01_OUTSIDE_KEY_SIGNAL == TRUE) && (sm.IO->tIn_CRT.fLD_DOOR01_INSIDE_KEY_SIGNAL == FALSE));
	bCheck &= ((sm.IO->tIn_CRT.fLD_DOOR02_SIGNAL == FALSE && sm.IO->tIn_CRT.fLD_DOOR02_OUTSIDE_KEY_SIGNAL == TRUE) && (sm.IO->tIn_CRT.fLD_DOOR02_INSIDE_KEY_SIGNAL == FALSE));
	return bCheck;
}

BOOL CheckAllDoorCloseSCT()
{
	BOOL bCheck = TRUE;

	bCheck &= ((sm.IOs->SCT.In.fSCT_DOOR1_SIGNAL == FALSE && sm.IOs->SCT.In.fSCT_DOOR1_OUTSIDE_KEY_SIGNAL == TRUE) && (sm.IOs->SCT.In.fSCT_DOOR1_INSIDE_KEY_SIGNAL == FALSE));
	bCheck &= ((sm.IOs->SCT.In.fSCT_DOOR2_SIGNAL == FALSE && sm.IOs->SCT.In.fSCT_DOOR2_OUTSIDE_KEY_SIGNAL == TRUE) && (sm.IOs->SCT.In.fSCT_DOOR2_INSIDE_KEY_SIGNAL == FALSE));
	bCheck &= ((sm.IOs->SCT.In.fSCT_DOOR3_SIGNAL == FALSE && sm.IOs->SCT.In.fSCT_DOOR3_OUTSIDE_KEY_SIGNAL == TRUE) && (sm.IOs->SCT.In.fSCT_DOOR3_INSIDE_KEY_SIGNAL == FALSE));
	bCheck &= ((sm.IOs->SCT.In.fSCT_DOOR4_SIGNAL == FALSE && sm.IOs->SCT.In.fSCT_DOOR4_OUTSIDE_KEY_SIGNAL == TRUE) && (sm.IOs->SCT.In.fSCT_DOOR4_INSIDE_KEY_SIGNAL == FALSE));
	bCheck &= ((sm.IOs->SCT.In.fSCT_DOOR5_SIGNAL == FALSE && sm.IOs->SCT.In.fSCT_DOOR5_OUTSIDE_KEY_SIGNAL == TRUE) && (sm.IOs->SCT.In.fSCT_DOOR5_INSIDE_KEY_SIGNAL == FALSE));
	bCheck &= ((sm.IOs->SCT.In.fSCT_DOOR6_SIGNAL == FALSE && sm.IOs->SCT.In.fSCT_DOOR6_OUTSIDE_KEY_SIGNAL == TRUE) && (sm.IOs->SCT.In.fSCT_DOOR6_INSIDE_KEY_SIGNAL == FALSE));
	//24.07.12 KWR 환경 안전 사양 적용
	bCheck &= ((sm.IOs->SCT.In.fSCT_DOOR8_SIGNAL == FALSE && sm.IOs->SCT.In.fSCT_DOOR8_OUTSIDE_KEY_SIGNAL == TRUE) && (sm.IOs->SCT.In.fSCT_DOOR8_INSIDE_KEY_SIGNAL == FALSE));
	bCheck &= ((sm.IOs->SCT.In.fSCT_DOOR9_SIGNAL == FALSE && sm.IOs->SCT.In.fSCT_DOOR9_OUTSIDE_KEY_SIGNAL == TRUE) && (sm.IOs->SCT.In.fSCT_DOOR9_INSIDE_KEY_SIGNAL == FALSE));
	
	bCheck &= (sm.IOs->SCT.In.fSCT_DUMMY_DOOR_SIGNAL == FALSE);
	return bCheck;
}

BOOL CheckDoorOpen_SCT_LD()
{
	BOOL bCheck = FALSE;
	
	bCheck |= sm.IOs->SCT.In.fSCT_DOOR1_SIGNAL;
	bCheck |= sm.IOs->SCT.In.fSCT_DOOR2_SIGNAL;
	bCheck |= sm.IOs->SCT.In.fSCT_DOOR3_SIGNAL;
	bCheck |= sm.IOs->SCT.In.fSCT_DOOR4_SIGNAL;
	bCheck |= sm.IOs->SCT.In.fSCT_DOOR5_SIGNAL;
	bCheck |= sm.IOs->SCT.In.fSCT_DOOR6_SIGNAL;

	bCheck |= sm.IOs->SCT.In.fSCT_DUMMY_DOOR_SIGNAL;
	
	bCheck |= sm.IOs->LD.In.fLD_DOOR01_SIGNAL;
	bCheck |= sm.IOs->LD.In.fLD_DOOR02_SIGNAL;

	//24.07.12 KWR 환경 안전 사양 적용
	bCheck |= sm.IOs->SCT.In.fSCT_DOOR7_SIGNAL;
	bCheck |= sm.IOs->SCT.In.fSCT_DOOR8_SIGNAL;
	bCheck |= sm.IOs->SCT.In.fSCT_DOOR9_SIGNAL;

	return bCheck;
}

void GetOpenDoorNumberLD(vector<int>& a)
{
	if (sm.IO->tIn_CRT.fLD_DOOR01_SIGNAL || sm.IO->tIn_CRT.fLD_DOOR01_INSIDE_KEY_SIGNAL || sm.IO->tIn_CRT.fLD_DOOR01_OUTSIDE_KEY_SIGNAL == FALSE)	a.push_back(1);
	if (sm.IO->tIn_CRT.fLD_DOOR02_SIGNAL || sm.IO->tIn_CRT.fLD_DOOR02_INSIDE_KEY_SIGNAL || sm.IO->tIn_CRT.fLD_DOOR02_OUTSIDE_KEY_SIGNAL == FALSE)	a.push_back(2);
}

void GetOpenDoorNumberSCT(vector<int>& a)
{
	if (sm.IOs->SCT.In.fSCT_DOOR1_SIGNAL || sm.IOs->SCT.In.fSCT_DOOR1_OUTSIDE_KEY_SIGNAL == FALSE || sm.IOs->SCT.In.fSCT_DOOR1_INSIDE_KEY_SIGNAL)		a.push_back(1);
	if (sm.IOs->SCT.In.fSCT_DOOR2_SIGNAL || sm.IOs->SCT.In.fSCT_DOOR2_OUTSIDE_KEY_SIGNAL == FALSE || sm.IOs->SCT.In.fSCT_DOOR2_INSIDE_KEY_SIGNAL)		a.push_back(2);
	if (sm.IOs->SCT.In.fSCT_DOOR3_SIGNAL || sm.IOs->SCT.In.fSCT_DOOR3_OUTSIDE_KEY_SIGNAL == FALSE || sm.IOs->SCT.In.fSCT_DOOR3_INSIDE_KEY_SIGNAL)		a.push_back(3);
	if (sm.IOs->SCT.In.fSCT_DOOR4_SIGNAL || sm.IOs->SCT.In.fSCT_DOOR4_OUTSIDE_KEY_SIGNAL == FALSE || sm.IOs->SCT.In.fSCT_DOOR4_INSIDE_KEY_SIGNAL)		a.push_back(4);
	if (sm.IOs->SCT.In.fSCT_DOOR5_SIGNAL || sm.IOs->SCT.In.fSCT_DOOR5_OUTSIDE_KEY_SIGNAL == FALSE || sm.IOs->SCT.In.fSCT_DOOR5_INSIDE_KEY_SIGNAL)		a.push_back(5);
	if (sm.IOs->SCT.In.fSCT_DOOR6_SIGNAL || sm.IOs->SCT.In.fSCT_DOOR6_OUTSIDE_KEY_SIGNAL == FALSE || sm.IOs->SCT.In.fSCT_DOOR6_INSIDE_KEY_SIGNAL)		a.push_back(6);
	//24.07.12 KWR 환경 안전 사양 적용
	if (sm.IOs->SCT.In.fSCT_DOOR7_SIGNAL || sm.IOs->SCT.In.fSCT_DOOR7_OUTSIDE_KEY_SIGNAL == FALSE || sm.IOs->SCT.In.fSCT_DOOR7_INSIDE_KEY_SIGNAL)	a.push_back(7);
	if (sm.IOs->SCT.In.fSCT_DOOR8_SIGNAL || sm.IOs->SCT.In.fSCT_DOOR8_OUTSIDE_KEY_SIGNAL == FALSE || sm.IOs->SCT.In.fSCT_DOOR8_INSIDE_KEY_SIGNAL)	a.push_back(8);
	if (sm.IOs->SCT.In.fSCT_DOOR9_SIGNAL || sm.IOs->SCT.In.fSCT_DOOR9_OUTSIDE_KEY_SIGNAL == FALSE || sm.IOs->SCT.In.fSCT_DOOR9_INSIDE_KEY_SIGNAL)	a.push_back(9);
	if (sm.IOs->SCT.In.fSCT_DUMMY_DOOR_SIGNAL)																																a.push_back(11);
}

BOOL GetShiftTableVacuum(BOOL bOn, BOOL bIn) {
	BOOL bCheck = TRUE;

	// 원본
	//bCheck &= sm.IOs->SCT.Out.fMG_SHIFT_TABLE_AREA_1_VACUUM_ON_SOL == bOn;
	//bCheck &= sm.IOs->SCT.Out.fMG_SHIFT_TABLE_AREA_1_VACUUM_OFF_SOL == !bOn;
	//bCheck &= sm.IOs->SCT.Out.fMG_SHIFT_TABLE_AREA_2_VACUUM_ON_SOL == bOn;
	//bCheck &= sm.IOs->SCT.Out.fMG_SHIFT_TABLE_AREA_2_VACUUM_OFF_SOL == !bOn;
	//bCheck &= sm.IOs->SCT.Out.fMG_SHIFT_TABLE_AREA_3_VACUUM_ON_SOL == bOn;
	//bCheck &= sm.IOs->SCT.Out.fMG_SHIFT_TABLE_AREA_3_VACUUM_OFF_SOL == !bOn;
	//bCheck &= sm.IOs->SCT.Out.fMG_SHIFT_TABLE_AREA_4_VACUUM_ON_SOL == bOn;
	//bCheck &= sm.IOs->SCT.Out.fMG_SHIFT_TABLE_AREA_4_VACUUM_OFF_SOL == !bOn;
	//bCheck &= sm.IOs->SCT.Out.fMG_SHIFT_TABLE_AREA_5_VACUUM_ON_SOL == bOn;
	//bCheck &= sm.IOs->SCT.Out.fMG_SHIFT_TABLE_AREA_5_VACUUM_OFF_SOL == !bOn;
	//bCheck &= sm.IOs->SCT.Out.fMG_SHIFT_TABLE_AREA_6_VACUUM_ON_SOL == bOn;
	//bCheck &= sm.IOs->SCT.Out.fMG_SHIFT_TABLE_AREA_6_VACUUM_OFF_SOL == !bOn;
	//if (bIn) {
	//	bCheck &= sm.IOs->SCT.In.fMG_SHIFT_VACUUM_CHECK_SWITCH;
	//	bCheck &= sm.IOs->SCT.In.fMG_SHIFT_TABLE_VACUUM_ON_CHECK_SWITCH == bOn;
	//}

	// KES 수정 R39 Version
	// 관련 IO 찾을 수 없음. 설비에서 확인 필요.
	return bCheck;
}

double	GetPPBoxDistance() {		// Recipe 에서 정보를 가지고 와서 집는위치에 대한 Pitch 를 받아옴
	double dPitch = 0;

	// Recipe 가 비어 있는 경우 일단 Pitch 를 0으로 셋팅
	if (g_pRecipe == nullptr) {
		return 0.;
	}

	// 현재까지 진행 된 Glass / Paper Count 를 받아옴
	int iCurGlassCount = sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount / 2;		// 남은 Glass/Paper 의 갯수를 계산
	int iCurPaperCount = sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount / 2;
	if (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount % 2) {
		iCurPaperCount++;
	}
	double dGlassPitch = iCurGlassCount * g_pRecipe->ppbox.dGlassThickness;
	double dPaperPitch = iCurPaperCount * g_pRecipe->ppbox.dPaperThickness;

	//int iCurGlassCount = sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.nMaterial / 2;	// 갯수 설정 
	//int iCurPaperCount = sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.nMaterial / 2;
	//
	//if (sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.nMaterial % 2) {
	//	if (g_pRecipe->ppbox.bTopMaterialType) {
	//		iCurGlassCount++;
	//	}
	//	else {
	//		iCurPaperCount++;
	//	}
	//}
	//double dGlassPitch = (g_pRecipe->ppbox.iGlassCount - iCurGlassCount) * g_pRecipe->ppbox.dGlassThickness;
	//double dPaperPitch = (g_pRecipe->ppbox.iPaperCount - iCurPaperCount) * g_pRecipe->ppbox.dPaperThickness;
	
	dPitch = dGlassPitch + dPaperPitch;

	return dPitch;
}

void SetDoorReleaseState(BOOL bRelease)
{
	sm.IO->tOut_NEW.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST = !bRelease;
	sm.IO->tOut_NEW.fSCT_DUMMY_DOOR_LOCK_RELEASE_REQUEST = !bRelease;

	//<< 2024.06.10 LBH - 주석 처리
		//sm.Option->tSequence.bKeepOnDoorOpenMelody = bRelease; //24.06.06 KGH DEL : DOOR OPEN시에 MELODY 울리도록 수정
		//24.06.06 KGH EDIT : DOOR OPEN시에만 MELODY ON 되도록 수정
		//sm.IO->tOut_NEW.fLD_MELODY_1 = bRelease;
		// 
		//램프 다 켜줌
		//sm.IO->tOut_NEW.fLD_DOOR01_RELEASE_SW = bRelease;
		//sm.IO->tOut_NEW.fLD_DOOR02_RELEASE_SW = bRelease;
		//sm.IO->tOut_NEW.fLD_DOOR03_RELEASE_SW = bRelease;
		//sm.IO->tOut_NEW.fLD_DOOR04_RELEASE_SW = bRelease;
		//sm.IO->tOut_NEW.fLD_DOOR05_RELEASE_SW = bRelease;
		//sm.IO->tOut_NEW.fLD_DOOR06_RELEASE_SW = bRelease;

		////SCT
		//CIPVar var;
		//var.SetChildItem("SET", bRelease);
		//g_ipClient.PostIPCommand("UI", "SET", "DoorRelease", var);
	//>>
}

int GetOpenDoorCount()
{
	int iCountDoorOpen = 0;

	if (sm.IO->tIn_CRT.fLD_DOOR01_SIGNAL == TRUE)
		iCountDoorOpen++;
	if (sm.IO->tIn_CRT.fLD_DOOR02_SIGNAL == TRUE)
		iCountDoorOpen++;

	if (sm.IOs->SCT.In.fSCT_DOOR1_SIGNAL == TRUE)
		iCountDoorOpen++;
	if (sm.IOs->SCT.In.fSCT_DOOR2_SIGNAL == TRUE)
		iCountDoorOpen++;
	if (sm.IOs->SCT.In.fSCT_DOOR3_SIGNAL == TRUE)
		iCountDoorOpen++;
	if (sm.IOs->SCT.In.fSCT_DOOR4_SIGNAL == TRUE)
		iCountDoorOpen++;
	if (sm.IOs->SCT.In.fSCT_DOOR5_SIGNAL == TRUE)
		iCountDoorOpen++;
	if (sm.IOs->SCT.In.fSCT_DOOR6_SIGNAL == TRUE)
		iCountDoorOpen++;
	//-->24.07.12 KWR 환경 안전 사양 적용
	if (sm.IOs->SCT.In.fSCT_DOOR7_SIGNAL == TRUE)
		iCountDoorOpen++;
	if (sm.IOs->SCT.In.fSCT_DOOR8_SIGNAL == TRUE)
		iCountDoorOpen++;
	if (sm.IOs->SCT.In.fSCT_DOOR9_SIGNAL == TRUE)
		iCountDoorOpen++;
	//<--24.07.12 KWR 환경 안전 사양 적용
	return iCountDoorOpen;
}
int GetInsideKeyCount()
{
	int iCountKeyIn = 0;

	if (sm.IO->tIn_CRT.fLD_DOOR01_INSIDE_KEY_SIGNAL == TRUE)
		iCountKeyIn++;
	if (sm.IO->tIn_CRT.fLD_DOOR02_INSIDE_KEY_SIGNAL == TRUE)
		iCountKeyIn++;

	if (sm.IOs->SCT.In.fSCT_DOOR1_INSIDE_KEY_SIGNAL == TRUE)
		iCountKeyIn++;
	if (sm.IOs->SCT.In.fSCT_DOOR2_INSIDE_KEY_SIGNAL == TRUE)
		iCountKeyIn++;
	if (sm.IOs->SCT.In.fSCT_DOOR3_INSIDE_KEY_SIGNAL == TRUE)
		iCountKeyIn++;
	if (sm.IOs->SCT.In.fSCT_DOOR4_INSIDE_KEY_SIGNAL == TRUE)
		iCountKeyIn++;
	if (sm.IOs->SCT.In.fSCT_DOOR5_INSIDE_KEY_SIGNAL == TRUE)
		iCountKeyIn++;
	if (sm.IOs->SCT.In.fSCT_DOOR6_INSIDE_KEY_SIGNAL == TRUE)
		iCountKeyIn++;
	//-->24.07.12 KWR 환경 안전 사양 적용
	if (sm.IOs->SCT.In.fSCT_DOOR7_INSIDE_KEY_SIGNAL == TRUE)
		iCountKeyIn++;
	if (sm.IOs->SCT.In.fSCT_DOOR8_INSIDE_KEY_SIGNAL == TRUE)
		iCountKeyIn++;
	if (sm.IOs->SCT.In.fSCT_DOOR9_INSIDE_KEY_SIGNAL == TRUE)
		iCountKeyIn++;
	//<--24.07.12 KWR 환경 안전 사양 적용
	return iCountKeyIn;
}

void SetTowerLamp(eTOWER_LAMP eTL, BOOL bOn)
{
	switch (eTL) 
	{
		case eTL_NOMAL: 
		{
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_RED		= FALSE;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_YELLOW  = bOn;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_GREEN	= FALSE;
			break;
		}

		case eTL_AUTO: 
		{
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_RED		= FALSE;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_YELLOW  = FALSE;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_GREEN	= bOn;
			break;
		}
		case eTL_AUTO_STOP: 
		{
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_RED		= FALSE;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_YELLOW  = bOn;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_GREEN	= bOn;
			break;
		}
		case eTL_HOME: 
		{
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_RED		= bOn;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_YELLOW  = bOn;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_GREEN	= bOn;
			break;
		}
		case eTL_ALARM: 
		{
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_RED		= bOn;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_YELLOW  = FALSE;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_GREEN	= FALSE;
			break;
		}
		case eTL_PM: 
		{
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_RED		= bOn;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_YELLOW  = FALSE;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_GREEN	= bOn;
			break;
		}
		case eTL_OPCALL: 
		{
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_RED		= FALSE;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_YELLOW  = bOn;
			sm.IO->tOut_NEW.fLD_OP_SIGNAL_TOWER_GREEN	= FALSE;
			break;
		}
	}
}
eCONVEYOR_LD GetConveyor(eMODULEs _eModule) 
{
	switch (_eModule) 
	{
		case eMODULE_PC1_LOAD_CONVEYOR_1:			return eCONVEYOR_LD_1;
		case eMODULE_PC1_LOAD_CONVEYOR_2:			return eCONVEYOR_LD_2;
		case eMODULE_PC1_ELEVATOR_CONVEYOR:			return eCONVEYOR_EL;
		case eMODULE_PC1_UNLOAD_CONVEYOR_1:			return eCONVEYOR_ULD_1;
		case eMODULE_PC1_UNLOAD_CONVEYOR_2:			return eCONVEYOR_ULD_2;
		default: return eCONVEYOR_LD_NONE;
	}
}

BOOL CheckIsMaterialInEQP()
{
	BOOL bResult = FALSE;

	//int eStartModule = eMODULE_PC1_GLASS_TRANSFER;
	//int eEndModule = eMODULE_PC1_GLASS_TRANSFER;
	//
	//for (int eModule = eStartModule; eModule <= eEndModule; eModule++)
	//{
	//	bResult |= sm.Module[eModule].Glass.bMaterial;
	//	if (bResult)
	//		break;
	//}
	
	bResult |= sm.Module[eMODULE_PC1_GLASS_TRANSFER].Glass.bMaterial;

	return bResult;
}

BOOL CheckIsMaterialInUNIT(int iUnit) {
	BOOL bResult = FALSE;

	//int eStartModule = eMODULE_PC1_GLASS_TRANSFER;
	//int eEndModule = eMODULE_PC1_GLASS_TRANSFER;
	//
	//for (int eModule = eStartModule; eModule <= eEndModule; eModule++)
	//{
	//	bResult |= sm.Module[eModule].Glass.bMaterial;
	//	if (bResult)
	//		break;
	//}
	//
	//bResult |= sm.Module[eMODULE_PC1_GLASS_TRANSFER].Glass.bMaterial;

	return bResult;
}

BOOL GetGlassTRSafety() {
	BOOL bReturn = TRUE;
	if (sm.MotorPos_1->Glass_TR.Wait_Y.dTarget - 10 > GetPos(eMOTOR_GLASS_TRANSFER_Y)) {
		bReturn = FALSE;
	}
	return bReturn;
}

void WriteModuleTactTimeLog(const CString& _moduleName, uint64 _tactTime)
{
	if (!g_pRecipe)
		return;

	CFile file;
	CFileException e;
	CString path = Format(_T("D:/MDIK/TACT Log/Module/%s/"), _moduleName);
	CreateSubDirectories(path);		// <- 폴더가 없으면 생성하도록 수정

	COleDateTime odtCurrent = COleDateTime::GetCurrentTime();
	path = path + odtCurrent.Format(_T("%Y%m%d.csv"));
	// 파일 열기
	file.Open(path, CFile::shareDenyWrite | CFile::modeNoTruncate | CFile::modeWrite | CFile::modeCreate, &e);
	if (e.m_cause != CFileException::none)
	{
		TRACE(_T("%s File Exception Error : %d\n"), path, e.m_cause);
		return;
	}

	// log 파일 쓰기
	CArchiveU arU(&file, CArchiveU::store);

	if (file.SeekToEnd() == 0)
	{
		arU.WriteStringU(_T("WriteTime, Recipe, CIM, Mode, TactTime"));
		arU.WriteStringU(_T("\r\n"));
	}

	CString strTactLog
	(
		Format
		(
			_T("%s,%s,%s,%s,%.3f")
			, odtCurrent.Format(_T("%H:%M:%S"))
			, g_pRecipe->recipeName
			, sm.Sequence->bCIMSkip ? _T("Offline") : _T("Online")
			, sm.Sequence->fAutoMode ? _T("Auto") : _T("Manual")
			, _tactTime * 0.001
		)
	);

	arU.WriteStringU(strTactLog);
	arU.WriteStringU(_T("\r\n"));

	arU.Close();
	file.Close();
}
//<< 2021.03.10 글라스 카운트
void GlassInput()
{
	CTime time = CTime::GetCurrentTime();
	int iYear = time.GetYear() - STANDARD_YEAR;
	int iMonth = time.GetMonth();
	int iDay = time.GetDay();
	int iHour = time.GetHour();

	sm.GlassEmissions->y[iYear].count.nIn++;
	sm.GlassEmissions->y[iYear].m[iMonth-1].count.nIn++;
	sm.GlassEmissions->y[iYear].m[iMonth-1].d[iDay-1].count.nIn++;
	sm.GlassEmissions->y[iYear].m[iMonth-1].d[iDay-1].h[iHour].count.nIn++;
}

void GlassOutput()
{
	CTime time = CTime::GetCurrentTime();
	int iYear = time.GetYear() - STANDARD_YEAR;
	int iMonth = time.GetMonth();
	int iDay = time.GetDay();
	int iHour = time.GetHour();

	int iY_OK = 0;
	int iM_OK = 0;
	int iD_OK = 0;
	int iH_OK = 0;

	int iY_Out = 0;
	int iM_Out = 0;
	int iD_Out = 0;
	int iH_Out = 0;

	iY_Out = ++sm.GlassEmissions->y[iYear].count.nOut;
	iM_Out = ++sm.GlassEmissions->y[iYear].m[iMonth-1].count.nOut;
	iD_Out = ++sm.GlassEmissions->y[iYear].m[iMonth-1].d[iDay-1].count.nOut;
	iH_Out = ++sm.GlassEmissions->y[iYear].m[iMonth-1].d[iDay-1].h[iHour].count.nOut;
}
//>>

// KES 추가 2024.4.25
void SetAutoStop() {
	sm.Sequence->fCycleStop = FALSE;
	sm.Sequence->fAutoMode = FALSE;
	for (int i = 0; i < nCONVEYOR_LD; i++) 
	{
		Serial::Inverter::SetInverterStop((eMODULEs)(eMODULE_PC1_LOAD_CONVEYOR_1 + i));
	}
}

//>> KES 추가 2024.05.23
BOOL CheckMotorInterLock(int iAxis, double dTarget, BOOL bDir, BOOL bBeforeMove) {
	auto [bInterlock, msg] = CheckMotorInterLockMsg(iAxis, dTarget, bDir, bBeforeMove);
	return bInterlock;
}

std::pair<BOOL, CString> CheckMotorInterLockMsg(int iAxis, double dTarget, BOOL bDir, BOOL bBeforeMove) {
	// bDir [TRUE +방향] [FALSE -방향]
	
	double dInPos = 0.05;
	double dOffset = 10.;
	
	const DWORD MessageTime = 10'000;

	const auto& GlassTransfer = sm.MotorPos_1->Glass_TR;
	
	// KES 추가 2024.1.5
	CString str(_T(""));
	BOOL bInterlock = FALSE;
	auto fn_set_description = [&](int id) {     //BIỂU THỨC LAMDA => GÁN CHO BIẾN fn_set_description || Tham số truyền vào biểu thức lamda: id || Truyền tham chiếu
		bInterlock = TRUE;
		if (g_stDescription.size() <= id)  {
			IPMessage(g_st[IDS_SHARED_FUNCTION_LD_DESCRIPTION_STRINGTABLE_NOT_LOAD], MessageTime);
			return;
		}

		if (!str.IsEmpty())
			str += _T("\n");
		str += g_stDescription[id];
		if (bBeforeMove)
			sm.MotorStatus[iAxis].tEtc.iMotorDesc = id;
	};
	
	switch (iAxis)
	{
		case eMOTOR_GLASS_BOX_EV_Z: { 
			break;
		}
		case eMOTOR_GLASS_TRANSFER_Y: {
			double dPosZ = GetPos(eMOTOR_GLASS_TRANSFER_Z);

			// Z축 
			if (fabs(GlassTransfer.Wait_Z.dTarget - dPosZ) > 1) {
				if (sm.Sequence->fAutoMode) {
					// KES 주석 2024.3.18
					//if (sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep < S_GT_RECV_ABLE || sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep > S_GT_RECV_ZY_AFTER_PPBOX_BOX_CHECK)
					//{
					//	IPMessage(_T("Z축이 대기위치가 아니거나 Glass Transfer 가 Loading 중이 아닙니다"));
					//	bCrush = TRUE;
					//}
				}
				else {
					fn_set_description(IDS_DES_MOTOR_GLASS_TRANSFER_Y_INTERLOCK_MSG_1);
				}
			}
			break;
		}
		case eMOTOR_GLASS_TRANSFER_Z: {
			// Y축이 티칭 포지션에 있을 때만 Z축 동작 할 수 있도록 수정.
			if (bDir 
				&& !CheckInPos(sm.MotorPos_1->Glass_TR.Load_Y)
				&& !CheckInPos(sm.MotorPos_1->Glass_TR.Wait_Y)
				&& !CheckInPos(sm.MotorPos_1->Glass_TR.Unload_Glass_Y)
				&& !CheckInPos(sm.MotorPos_1->Glass_TR.Unload_Paper_Y)
				&& sm.MotorStatus[eMOTOR_GLASS_TRANSFER_Z].tEtc.dwHomeStatus != eHOME_STATUS_HOMMING) //2024.06.12 add by kkw : During ABS ignore
			{
					
				fn_set_description(IDS_DES_MOTOR_GLASS_TRANSFER_Z_INTERLOCK_MSG_1);
				
			}
			

			if (GlassTransfer.Load_Z.dTarget < dTarget) { // PSS ADD 03.29: PTOP로 LD TR Z가 로드 Z위치보다 더하강할경우 인터락)
				fn_set_description(IDS_DES_LD_TRANSFER_Z_DOWN_INTERLOCK);
			}
			break;
		}
	}

	//KES 추가 2024.3.26
	if (bInterlock == TRUE && !sm.Sequence->fAutoMode && bBeforeMove) {
		if (!str.IsEmpty()) {
			IPMessage(str, MessageTime);
		}
		else {
			str = Format(_T("[%s] Interlock -"), g_stMotor[iAxis]);
			IPMessage(Format(_T("[%s] Interlock -"), g_stMotor[iAxis]), MessageTime);
		}
		g_ipClient.m_log.WriteErrorLog(str);
	}

	return { bInterlock, str };
}

std::pair<BOOL, CString> CheckMotorInterLockMsg(T_POS tPos) {
	return CheckMotorInterLockMsg(tPos.iAxis, tPos.dTarget, GetPos(tPos.iAxis) < tPos.dTarget, FALSE);  //1 hàm khác cùng tên
}
//>>

namespace Serial {
	namespace Inverter {
		void SetInverterRun(eMODULEs _eModule, BOOL bFwd, int iFrequency, CIPCommand* _pCMD) {
			SetTimeout(_eModule);
			sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_READY;

			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;

			CIPVar var;
			var.SetChildItem("eMODULEs", _eModule);
			var.SetChildItem("Frequency", iFrequency);
			var.SetChildItem("Fwd", bFwd);

			if (_pCMD)
				g_ipClient.SendIPStepCommand(_pCMD, "Serial", FormatA("Inverter%d", iInverter), "Inverter", "Run", var);
			else
				g_ipClient.PostIPCommand("Serial", FormatA("Inverter%d", iInverter), "Inverter", "Run", var);
		}

		void SetInverterRun(eMODULEs _eModule, BOOL bFwd, double dSpeed, CIPCommand* _pCMD) {
			SetTimeout(_eModule);
			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;
			int iFrequency = (int)(dSpeed * sm.Conveyor_Parameter[iInverter].dSpeedRate);

			SetInverterRun(_eModule, bFwd, iFrequency, _pCMD);
		}

		void SetInverterStop(eMODULEs _eModule, CIPCommand* _pCMD) {
			SetTimeout(_eModule);
			sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_READY;

			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;

			CIPVar var;
			var.SetChildItem("eMODULEs", _eModule);

			if (_pCMD)
				g_ipClient.SendIPStepCommand(_pCMD, "Serial", FormatA("Inverter%d", iInverter), "Inverter", "Stop", var);
			else
				g_ipClient.PostIPCommand("Serial", FormatA("Inverter%d", iInverter), "Inverter", "Stop", var);
		}

		void SetInverterFrequency(eMODULEs _eModule, int iFrequency, CIPCommand* _pCMD) {
			SetTimeout(_eModule);
			sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_READY;

			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;

			CIPVar var;
			var.SetChildItem("eMODULEs", _eModule);
			var.SetChildItem("Frequency", iFrequency);

			if (_pCMD)
				g_ipClient.SendIPStepCommand(_pCMD, "Serial", FormatA("Inverter%d", iInverter), "Inverter", "SetFrequency", var);
			else
				g_ipClient.PostIPCommand("Serial", FormatA("Inverter%d", iInverter), "Inverter", "SetFrequency", var);
		}

		void SetInverterSpeed(eMODULEs _eModule, double dSpeed, CIPCommand* _pCMD) {
			SetTimeout(_eModule);
			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;
			int iFrequency = (int)(dSpeed * sm.Conveyor_Parameter[iInverter].dSpeedRate);
			SetInverterFrequency(_eModule, iFrequency, _pCMD);
		}

		void SetInverterSetting(eMODULEs _eModule, int iAcc1, int iAcc2, int iDec1, int iDec2, CIPCommand* _pCMD) {
			SetTimeout(_eModule);
			sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_READY;

			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;

			CIPVar var;
			var.SetChildItem("eMODULEs", _eModule);
			var.SetChildItem("ACC1", iAcc1);
			var.SetChildItem("DEC1", iDec1);
			var.SetChildItem("ACC2", iAcc2);
			var.SetChildItem("DEC2", iDec2);

			if (_pCMD)
				g_ipClient.SendIPStepCommand(_pCMD, "Serial", FormatA("Inverter%d", iInverter), "Inverter", "DataSetting", var);
			else
				g_ipClient.PostIPCommand("Serial", FormatA("Inverter%d", iInverter), "Inverter", "DataSetting", var);
		}

		BOOL GetInverterRun(eMODULEs _eModule) {
			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;
			return sm.InverterData[iInverter].iRun == 1;
		}

		BOOL GetInverterStop(eMODULEs _eModule) {
			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;
			return sm.InverterData[iInverter].iRun == 0;
		}

		BOOL GetInverterReady(eMODULEs _eModule) {
			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;
			return sm.InverterData[iInverter].iReady == TRUE;
		}

		int GetInverterFrequency(eMODULEs _eModule) {
			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;
			return sm.InverterData[iInverter].iFreaquency;
		}

		BOOL GetInverterSpeedCompare(eMODULEs _eModule, double _dSpeed, int _iPoint) {
			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;
			int iFrequency = GetInverterFrequency(_eModule);
			double dSpeed = iFrequency / sm.Conveyor_Parameter[iInverter].dSpeedRate;

			double dTemp = 1;
			for (int i = 0; i < _iPoint; i++)
				dTemp /= 10.;

			return (fabs(dSpeed - _dSpeed) < dTemp);
		}

		BOOL GetInverterDirection(eMODULEs _eModule, BOOL _bFwd) {
			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;
			return sm.InverterData[iInverter].iDirection == (_bFwd ? 0 : 1);
		}

		int GetInverteriAccTime1(eMODULEs _eModule) {
			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;
			return sm.InverterData[iInverter].iAccTime1;
		}

		int GetInverteriAccTime2(eMODULEs _eModule) {
			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;
			return sm.InverterData[iInverter].iAccTime2;
		}

		int GetInverteriDecTime1(eMODULEs _eModule) {
			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;
			return sm.InverterData[iInverter].iDecTime1;
		}

		int GetInverteriDecTime2(eMODULEs _eModule) {
			int iInverter = _eModule - eMODULE_PC1_LOAD_CONVEYOR_1;
			return sm.InverterData[iInverter].iDecTime2;
		}
	}

	namespace PPBoxCodeReader {
		void SetPPBoxIDReading(eMODULEs _eModule, CIPCommand* _pCMD) {
			SetTimeout(_eModule);

			if (sm.DeviceParameter->tUse.bDryRun) {
				sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_COMPLETED;

				if (_eModule != eMODULE_NONE) {
					// 임시로 szCstID 에 처리.
					CStringA strGlassID(_T("PPBox"));
					strncpy_s(sm.Module[_eModule].Glass.szCELLID, sizeof(sm.Module[_eModule].Glass.szCELLID), strGlassID, sizeof(sm.Module[_eModule].Glass.szCELLID) - 1);
				}
			}
			else {
				if (_eModule != eMODULE_NONE) {
					memset(sm.Module[_eModule].Glass.szCELLID, 0x00, sizeof(sm.Module[_eModule].Glass.szCELLID));
				}
				sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_READY;

				CIPVar var;
				var.SetChildItem("eMODULEs", _eModule);

				if (_pCMD)
					g_ipClient.SendIPStepCommand(_pCMD, "Serial", "", "PPBoxCodeReader", "ReadCode", var);
				else
					g_ipClient.PostIPCommand("Serial", "", "PPBoxCodeReader", "ReadCode", var);
			}
		}
	}
}

void INFO_INIT_MCC(T_MCC_LAYER* mcc_layer)
{
	memset(mcc_layer->szCELL_ID, 0x00, sizeof(mcc_layer->szCELL_ID));
	memset(mcc_layer->szStepID, 0x00, sizeof(mcc_layer->szStepID));
	memset(mcc_layer->szLOT_ID, 0x00, sizeof(mcc_layer->szLOT_ID));
}

void CELL_INFO_TO_MCC(const T_CELL_INFO& glass, T_MCC_LAYER* mcc_layer)
{
	strncpy_s(mcc_layer->szCELL_ID, countof(mcc_layer->szCELL_ID), glass.szCELLID, countof(mcc_layer->szCELL_ID) - 1);
	strncpy_s(mcc_layer->szStepID, countof(mcc_layer->szStepID), glass.szSTEPID, countof(mcc_layer->szStepID) - 1);
	strncpy_s(mcc_layer->szLOT_ID, countof(mcc_layer->szLOT_ID), glass.szCST_ID, countof(mcc_layer->szLOT_ID) - 1);
}

// CIM
void S6F11_INTERLOCK_CONFIRM() {
	g_ipClient.PostIPCommand("UI", NULL, "SendMessage", "S6F11_INTERLOCK_CONFIRM");
}

void S6F11_OPCALL_CONFIRM() {
	g_ipClient.PostIPCommand("UI", NULL, "SendMessage", "S6F11_OPCALL_CONFIRM");
}

void S5F1_ALARM_REPORT_SEND(T_SYSTEM_ALARM_ITEM _Alarm, CIPCommand* _pCMD) {

	CIPVar var;
	var.SetChildItemT<T_SYSTEM_ALARM_ITEM>("T_SYSTEM_ALARM_ITEM", _Alarm);

	if (_pCMD)
		g_ipClient.SendIPStepCommand(_pCMD, "UI", NULL, "SendMessage", "S5F1_ALARM_REPORT_SEND", var);
	else
		g_ipClient.PostIPCommand("UI", NULL, "SendMessage", "S5F1_ALARM_REPORT_SEND", var);
}

void S6F11_CASSETTE_LOAD_REQUEST(eMODULEs _eModule, int _iPort, CIPCommand* _pCMD) {
	SetTimeout(_eModule);
	sm.Module[_eModule].Status.eReportStatus = eCIM_READY;

	CIPVar var;
	var.SetChildItem("eMODULEs", _eModule);
	var.SetChildItem("PORT", _iPort);

	if (_pCMD)
		g_ipClient.SendIPStepCommand(_pCMD, "UI", NULL, "SendMessage", "S6F11_CASSETTE_LOAD_REQUEST", var);
	else
		g_ipClient.PostIPCommand("UI", NULL, "SendMessage", "S6F11_CASSETTE_LOAD_REQUEST", var);
}

void S6F11_CASSETTE_LOAD_COMPLETE(eMODULEs _eModule, int _iPort, BOOL _bAGV_USE, CIPCommand* _pCMD) {
	SetTimeout(_eModule);
	sm.Module[_eModule].Status.eReportStatus = eCIM_READY;

	CIPVar var;
	var.SetChildItem("eMODULEs", _eModule);
	var.SetChildItem("PORT", _iPort);
	var.SetChildItem("USE_AGV", _bAGV_USE);

	if (_pCMD)
		g_ipClient.SendIPStepCommand(_pCMD, "UI", NULL, "SendMessage", "S6F11_CASSETTE_LOAD_COMPLETE", var);
	else
		g_ipClient.PostIPCommand("UI", NULL, "SendMessage", "S6F11_CASSETTE_LOAD_COMPLETE", var);
}

void S6F11_CARRIER_RELEASE_COMPLETE(eMODULEs _eModule, int _iPort, CStringA _szCSTID, CIPCommand* _pCMD) {
	SetTimeout(_eModule);
	sm.Module[_eModule].Status.eReportStatus = eCIM_READY;

	CIPVar var;
	var.SetChildItem("eMODULEs", _eModule);
	var.SetChildItem("PORT", _iPort);
	var.SetChildItem("CSTID", _szCSTID);

	if (_pCMD)
		g_ipClient.SendIPStepCommand(_pCMD, "UI", NULL, "SendMessage", "S6F11_CARRIER_RELEASE_COMPLETE", var);
	else
		g_ipClient.PostIPCommand("UI", NULL, "SendMessage", "S6F11_CARRIER_RELEASE_COMPLETE", var);
}

void S6F11_CARRIER_INFORMATION_REQUEST(eMODULEs _eModule, int _iPort, CStringA _szCSTID, CIPCommand* _pCMD) {
	SetTimeout(_eModule);
	sm.Module[_eModule].Status.eReportStatus = eCIM_READY;

	CIPVar var;
	var.SetChildItem("eMODULEs", _eModule);
	var.SetChildItem("PORT", _iPort);
	var.SetChildItem("CSTID", _szCSTID);

	if (_pCMD)
		g_ipClient.SendIPStepCommand(_pCMD, "UI", NULL, "SendMessage", "S6F11_CARRIER_INFORMATION_REQUEST", var);
	else
		g_ipClient.PostIPCommand("UI", NULL, "SendMessage", "S6F11_CARRIER_INFORMATION_REQUEST", var);
}

void S6F11_CASSETTE_UNLOAD_REQUEST(eMODULEs _eModule, int _iPort, CIPCommand* _pCMD) {
	SetTimeout(_eModule);
	sm.Module[_eModule].Status.eReportStatus = eCIM_READY;

	CIPVar var;
	var.SetChildItem("eMODULEs", _eModule);
	var.SetChildItem("PORT", _iPort);

	if (_pCMD)
		g_ipClient.SendIPStepCommand(_pCMD, "UI", NULL, "SendMessage", "S6F11_CASSETTE_UNLOAD_REQUEST", var);
	else
		g_ipClient.PostIPCommand("UI", NULL, "SendMessage", "S6F11_CASSETTE_UNLOAD_REQUEST", var);
}

void S6F11_CASSETTE_UNLOAD_COMPLETE(eMODULEs _eModule, int _iPort, CIPCommand* _pCMD) {
	SetTimeout(_eModule);
	sm.Module[_eModule].Status.eReportStatus = eCIM_READY;

	CIPVar var;
	var.SetChildItem("eMODULEs", _eModule);
	var.SetChildItem("PORT", _iPort);

	if (_pCMD)
		g_ipClient.SendIPStepCommand(_pCMD, "UI", NULL, "SendMessage", "S6F11_CASSETTE_UNLOAD_COMPLETE", var);
	else
		g_ipClient.PostIPCommand("UI", NULL, "SendMessage", "S6F11_CASSETTE_UNLOAD_COMPLETE", var);
}

void RECV_PPBOX_LOT_DATA(eMODULEs _eModule, CStringA _strPPBOX, CIPCommand* _pCMD) {
	SetTimeout(_eModule);
	sm.Module[_eModule].Status.eReportStatus = eCIM_READY; 
	
	CIPVar var;
	var.SetChildItem("eMODULEs", _eModule);
	var.SetChildItem("CSTID", _strPPBOX);

	if (_pCMD)
		g_ipClient.SendIPStepCommand(_pCMD, "UI", NULL, "Sequence", "RECV_PPBOX_LOT_DATA", var);
	else
		g_ipClient.PostIPCommand("UI", NULL, "Sequence", "RECV_PPBOX_LOT_DATA", var);
}

void DELETE_PPBOX_LOT_DATA(eMODULEs _eModule, CStringA _strPPBOX, CIPCommand* _pCMD) {
	SetTimeout(_eModule);
	sm.Module[_eModule].Status.eReportStatus = eCIM_READY;

	CIPVar var;
	var.SetChildItem("eMODULEs", _eModule);
	var.SetChildItem("CSTID", _strPPBOX);

	if (_pCMD)
		g_ipClient.SendIPStepCommand(_pCMD, "UI", NULL, "Sequence", "DELETE_PPBOX_LOT_DATA", var);
	else
		g_ipClient.PostIPCommand("UI", NULL, "Sequence", "DELETE_PPBOX_LOT_DATA", var);
}


//---------- Reset ----------
void S6F11_CARRIER_INFORMATION_REQUEST_RESET(eMODULEs _eModule, int _iPort, CIPCommand* _pCMD) {
	SetTimeout(_eModule);
	sm.Module[_eModule].Status.eReportStatus = eCIM_READY;

	CIPVar var;
	var.SetChildItem("eMODULEs", _eModule);
	var.SetChildItem("PORT", _iPort);

	if (_pCMD)
		g_ipClient.SendIPStepCommand(_pCMD, "UI", NULL, "SendMessage", "S6F11_CARRIER_INFORMATION_REQUEST_RESET", var);
	else
		g_ipClient.PostIPCommand("UI", NULL, "SendMessage", "S6F11_CARRIER_INFORMATION_REQUEST_RESET", var);
}

void S6F11_CARRIER_RELEASE_COMPLETE_RESET(eMODULEs _eModule, int _iPort, CIPCommand* _pCMD) {
	SetTimeout(_eModule);
	sm.Module[_eModule].Status.eReportStatus = eCIM_READY;

	CIPVar var;
	var.SetChildItem("eMODULEs", _eModule);
	var.SetChildItem("PORT", _iPort);

	if (_pCMD)
		g_ipClient.SendIPStepCommand(_pCMD, "UI", NULL, "SendMessage", "S6F11_CARRIER_RELEASE_COMPLETE_RESET", var);
	else
		g_ipClient.PostIPCommand("UI", NULL, "SendMessage", "S6F11_CARRIER_RELEASE_COMPLETE_RESET", var);
}

void SyncDataDustCollector(BOOL bStore)
{
	CProfile profile(_T("../cfg/DustCollector.ini"));

	if (bStore)
	{
		profile.GetSections().DeleteAll();
	}


	CString strSection = Format(_T("Dust Collector Filter Cycle Limit Setting"));
	profile.SyncProfileValue(bStore, strSection, _T("Press Light Limit"), sm.Option->tDustColletorOption.iLightAlarmLimit);
	profile.SyncProfileValue(bStore, strSection, _T("Press Heavy Limit"), sm.Option->tDustColletorOption.iHeavyAlarmLimit);
	profile.SyncProfileValue(bStore, strSection, _T("Filter Cycle Limit"), sm.Option->tDustColletorOption.iFilterCycleLimit);

	if (bStore)
	{
		profile.Save();
	}
}
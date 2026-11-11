#include "pch.h"
#include "PLCLinker.h"
#include "AlarmChecker.h"

AlarmChecker::AlarmChecker() {}
AlarmChecker::~AlarmChecker() {}

BOOL AlarmChecker::s_bMotorPlusLimit[eMOTOR_END] = { FALSE, };
size_t AlarmChecker::s_plusLimitElapsedTime[eMOTOR_END] = { 0, };

BOOL AlarmChecker::s_bMotorMinusLimit[eMOTOR_END] = { FALSE, };
size_t AlarmChecker::s_minusLimitElapsedTime[eMOTOR_END] = { 0, };
BOOL AlarmChecker::isMainPressAlarmOuccured = false;
ULONGLONG AlarmChecker::elapsedTime = 0;

BOOL AlarmChecker::s_bAlarmOccurCheck = FALSE;
size_t AlarmChecker::s_AlarmoccurTime = 0.;
//24.07.19 KWR ADD
BOOL AlarmChecker::s_bBuzzerStayAlarm = TRUE;

void AlarmChecker::AlarmChecker_MOTOR() {

	//<< 모터 관련 알람 탐지 및 발생
	for (int iAxis = eMOTOR_0; iAxis < eMOTOR_END; ++iAxis) {

		//. homing 동작이 아닌 경우 검사할 항목.
		if (eHOME_STATUS_COMPLETED == sm.MotorStatus[iAxis].tEtc.dwHomeStatus) {

			// limit alarm 1초 이상 유지될때 발생하도록 수정 2021.08.03
			if (sm.MotorStatus[iAxis].tCrt.bPLimit)
			{
				if (!s_bMotorPlusLimit[iAxis])
				{
					s_bMotorPlusLimit[iAxis] = TRUE;
					s_plusLimitElapsedTime[iAxis] = GetTickCount64();
				}
				else if (s_bMotorPlusLimit[iAxis] && GetTickCount64() - s_plusLimitElapsedTime[iAxis] > 1000)
				{
					SetAlarm(iAxis + eAI_GLASS_BOX_EV_Z_PLUS_LIMIT, IDS_DES_NOT_USE);
				}
			}
			else
			{
				s_bMotorPlusLimit[iAxis] = FALSE;
			}

			// limit alarm 1초 이상 유지될때 발생하도록 수정 2021.08.03
			if (sm.MotorStatus[iAxis].tCrt.bNLimit) 
			{
				if (!s_bMotorMinusLimit[iAxis])
				{
					s_bMotorMinusLimit[iAxis] = TRUE;
					s_minusLimitElapsedTime[iAxis] = GetTickCount64();
				}
				else if (s_bMotorMinusLimit[iAxis] && GetTickCount64() - s_minusLimitElapsedTime[iAxis] > 1000)
				{
					SetAlarm(iAxis + eAI_GLASS_BOX_EV_Z_MINUS_LIMIT, IDS_DES_NOT_USE);
				}
			}
			else
			{
				s_bMotorMinusLimit[iAxis] = FALSE;
			}

			//. 해당 Motor가 Gantry일 경우
			if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {

				//. 해당 Gantry 축 위치와 해당 Gantry 축의 Ch2 축 위치 차이값
				double dGantryOffset = fabs(sm.MotorStatus[iAxis].tCrt.dPos - sm.MotorStatus[iAxis].tCrt.tGantryData.dCh2Pos);
				BOOL bNewTick = TRUE;
				//. 설정된 GantryOffsetLimitAlert 값보다 현재 위치 Offset값이 크다면.
				if (sm.MotorParameter[iAxis].dGantryOffsetLimitAlert < dGantryOffset) {
					bNewTick = FALSE;
					if (GetTickCount64() - sm.MotorStatus[iAxis].tEtc.ullPosErrorTick > 500) {
						sm.MotorStatus[iAxis].tEtc.tGantryData.dwGantryWarningLevel = eCH_GANTRY_WARNING_LEVEL_ALERT;

						if (sm.MotorStatus[iAxis].tCrt.bServoOn == TRUE) {

							sm.MotorParameter[iAxis].tYaskawa.iGantryCh = eCH_AXIS_ALL;
							//. SetAlarm보다 ServoOff가 더 중요.
							ys.ServoOnOff(iAxis, FALSE);

							int iAlarm = iAxis + eAI_GLASS_BOX_EV_Z_GANTRY_OFFSET_LIMIT_ALERT;

							//. 알람 내용에 현재 offset값 표시.
							// KES 수정 2024.1.11
							CString str(Format(_T("offSet : %.3f"), dGantryOffset));
							SetAlarm(iAlarm, IDS_DES_NOT_USE, str);
						}
					}
				}
				// 2021.03.22 서보파라미터 수정할 때 변경하지 않은 데이터가 뜨는 문제 수정
				////. Warning값이 세팅 안되어 있을 경우. 이걸 계속 검사해야하나?? 한 번만 해도 될 것 같은데
				//else if (true == DBL_COMPARE(sm.MotorParameter[iAxis].dGantryOffsetLimitWarning, 0.0)) {
				//	
				//	//. Alert값이 세팅 되어 있을 경우.
				//	//if (false == DBL_COMPARE(sm.MotorParameter[iAxis].dGantryOffsetLimitAlert, 0.0)) {
				//
				//	sm.MotorParameter[iAxis].dGantryOffsetLimitWarning = sm.MotorParameter[iAxis].dGantryOffsetLimitAlert / 2.;
				//	//}
				//}
				//. 그보다 한 단계 적은 GantryOffsetLimitWarning 값보다 현재 위치 Offset값이 크다면
				else if (sm.MotorParameter[iAxis].dGantryOffsetLimitWarning < dGantryOffset) {
					
					//TODO thread safe한가??
					sm.MotorStatus[iAxis].tEtc.tGantryData.dwGantryWarningLevel = eCH_GANTRY_WARNING_LEVEL_WARNING;
				}
				else {
					
					sm.MotorStatus[iAxis].tEtc.tGantryData.dwGantryWarningLevel = eCH_GANTRY_WARNING_LEVEL_NONE;
				}

				if (bNewTick) {
					sm.MotorStatus[iAxis].tEtc.ullPosErrorTick = GetTickCount64();
				}
			}
		}
		// KES 주석 2024.3.18 - 삭제된 모터 체크하는 부분 주석
		//else if (eHOME_STATUS_NOT_COMPLETED == sm.MotorStatus[iAxis].tEtc.dwHomeStatus) {
		//	if (iAxis != eMOTOR_GLASS_BOX_TILTING)							// 01.07 LGW Tilt 축 Home Not Complete 임시로 조치
		//		SetAlarm(iAxis + eAI_GLASS_BOX_EV_Z_HOME_NOT_COMPLETED);
		//}

		if (TRUE == sm.MotorStatus[iAxis].tCrt.bAlarm) {
			SetAlarm(iAxis + eAI_GLASS_BOX_EV_Z_MOTOR_ALARM, IDS_DES_NOT_USE);
		}

		//20.12.23 LJC Servo alarm automode 일때만 발생되도록 수정 - order by van (횡전개 해주세욤)
		if (FALSE == sm.MotorStatus[iAxis].tCrt.bServoOn && sm.Sequence->fAutoMode == TRUE) {
			SetAlarm(iAxis + eAI_GLASS_BOX_EV_Z_SERVO_OFF, IDS_DES_NOT_USE);
		}

		//<< TODO
		if (TRUE == sm.MotorStatus[iAxis].tCrt.bServoError) {
		}

		if (TRUE == sm.MotorStatus[iAxis].tCrt.bSystemBusy) {
		}
		//>>
	}
	//>>

}

void AlarmChecker::AlarmChecker_IO() {
	SafetyAlarm();
	DoorCheck();
	MuteCheck();
	TowerLamp();
	//24.07.19 KWR 환경 안전 사양 적용
	BuzzerStayAlarmChecker(FALSE);
	
	// 2024.06.05 LBH
	if (sm.Option->tSequence.bBuzzerStop) {
		//sm.IO->tOut_NEW.fLD_MELODY_1 = FALSE; // Door관련 Melody라서 제외.
		sm.IO->tOut_NEW.fLD_MELODY_2 = FALSE;
		sm.IO->tOut_NEW.fLD_MELODY_3 = FALSE;
		sm.IO->tOut_NEW.fLD_MELODY_4 = FALSE;
		sm.IO->tOut_NEW.fLD_MELODY_5 = FALSE;
		sm.IO->tOut_NEW.fLD_MELODY_6 = FALSE;
		sm.IO->tOut_NEW.fLD_MELODY_7 = FALSE;
		sm.IO->tOut_NEW.fLD_MELODY_8 = FALSE;
	}
	//24.06.25 kwr 환경 안전 추가
	bool bPDTcheck = false;
	bPDTcheck |= sm.Option->tSequence.eDriveMode == eDRIVE_MODE_AUTO;
	bPDTcheck |= sm.Option->tSequence.eDriveMode == eDRIVE_MODE_MANUAL;
	//bPDTcheck |= sm.IO->tIn_CRT.fULD_OP_SELECT_KEY_SWITCH_AUTO_POSITION;
	if (bPDTcheck) {
		sm.Register->Out_New.dw1002_PDT_TEACH_MODE = FALSE;
	}
	else
	{
		sm.Register->Out_New.dw1002_PDT_TEACH_MODE = TRUE;
	}
}

void AlarmChecker::SafetyAlarm() {
	//MC 관련
	//2024.06.07 MDIK KKW GPS1_MAIN_ELCB_ON ADD
	if (sm.IO->tIn_CRT.fGPS1_MAIN_ELCB_ON == FALSE)
	{
		if (sm.Alarm->ALARMs[eAL_GPS1_ECLB_OFF].bOccurred == FALSE)
			SetAlarm(eAL_GPS1_ECLB_OFF, IDS_DES_NOT_USE, nullptr, FALSE);
	}
	else
		sm.Alarm->ALARMs[eAL_GPS1_ECLB_OFF].bReset = TRUE;

	if (sm.IO->tIn_CRT.fUPS_MC01A_ON == FALSE)
	{
		if (sm.Alarm->ALARMs[eAI_UPS_MC01A_OFF].bOccurred == FALSE)
			SetAlarm(eAI_UPS_MC01A_OFF, IDS_DES_NOT_USE, nullptr, FALSE);
	}
	else
		sm.Alarm->ALARMs[eAI_UPS_MC01A_OFF].bReset = TRUE;

	if (sm.IO->tIn_CRT.fUPS_MC01B_ON == FALSE)
	{
		if (sm.Alarm->ALARMs[eAI_UPS_MC01B_OFF].bOccurred == FALSE)
			SetAlarm(eAI_UPS_MC01B_OFF, IDS_DES_NOT_USE, nullptr, FALSE);
	}
	else
		sm.Alarm->ALARMs[eAI_UPS_MC01B_OFF].bReset = TRUE;

	if (sm.IO->tIn_CRT.fGPS_EMO_MC31_ON == FALSE)
	{
		if (sm.Alarm->ALARMs[eAI_GPS_EMO_MC31_OFF].bOccurred == FALSE)
			SetAlarm(eAI_GPS_EMO_MC31_OFF, IDS_DES_NOT_USE, nullptr, FALSE);
	}
	else
		sm.Alarm->ALARMs[eAI_GPS_EMO_MC31_OFF].bReset = TRUE;

	if (sm.IO->tIn_CRT.fUPS1_MAIN_ELCB_ON == FALSE)
	{
		if (sm.Alarm->ALARMs[eAl_UPS1_MAIN_ELCB_ON].bOccurred == FALSE)
			SetAlarm(eAl_UPS1_MAIN_ELCB_ON, IDS_DES_NOT_USE, nullptr, FALSE);
	}
	else
		sm.Alarm->ALARMs[eAl_UPS1_MAIN_ELCB_ON].bReset = TRUE;

	//SAFETY 관련 - 상시체크
	if (sm.IO->tIn_CRT.fSAFETY_SMPS1_CP_ON == FALSE)
	{
		if (sm.Alarm->ALARMs[eAI_SAFETY_SMPS1_CP_OFF].bOccurred == FALSE)
			SetAlarm(eAI_SAFETY_SMPS1_CP_OFF, IDS_DES_NOT_USE, nullptr, FALSE);
	}
	else
		sm.Alarm->ALARMs[eAI_SAFETY_SMPS1_CP_OFF].bReset = TRUE;

	if (sm.IO->tIn_CRT.fSAFETY_SMPS2_CP_ON == FALSE)
	{
		if (sm.Alarm->ALARMs[eAI_SAFETY_SMPS2_CP_OFF].bOccurred == FALSE)
			SetAlarm(eAI_SAFETY_SMPS2_CP_OFF, IDS_DES_NOT_USE, nullptr, FALSE);
	}
	else
		sm.Alarm->ALARMs[eAI_SAFETY_SMPS2_CP_OFF].bReset = TRUE;

	if (sm.IO->tIn_CRT.fSAFETY_SMPS_ON == FALSE)
	{
		if (sm.Alarm->ALARMs[eAI_SAFETY_SMPS_OFF].bOccurred == FALSE)
			SetAlarm(eAI_SAFETY_SMPS_OFF, IDS_DES_NOT_USE, nullptr, FALSE);
	}
	else
		sm.Alarm->ALARMs[eAI_SAFETY_SMPS_OFF].bReset = TRUE;

	if (sm.IO->tIn_CRT.fSAFETY_TWO_WAY_UNIT_ON == FALSE)
	{
		if (sm.Alarm->ALARMs[eAI_SAFETY_TWO_WAY_UNIT_OFF].bOccurred == FALSE)
			SetAlarm(eAI_SAFETY_TWO_WAY_UNIT_OFF, IDS_DES_NOT_USE, nullptr, FALSE);
	}
	else
		sm.Alarm->ALARMs[eAI_SAFETY_TWO_WAY_UNIT_OFF].bReset = TRUE;

	//PDT 관련
	if (sm.IO->tIn_CRT.fLD_PDT_EMO_NORMAL == TRUE && sm.IO->tIn_CRT.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION == TRUE && sm.Alarm->ALARMs[eAI_PDT_EMS_NORMAL_ON].bOccurred == FALSE)
		SetAlarm(eAI_PDT_EMS_NORMAL_ON, IDS_DES_NOT_USE);

	//temp 관련 - 1 헤비알람 2 경알람
	if (sm.IO->tIn_CRT.fGPS_MAIN_BOX_TEMP_ALARM2 == FALSE && sm.Alarm->ALARMs[eAI_GPS_MAIN_BOX_TEMPERATURE_L].bOccurred == FALSE)
		SetAlarm(eAI_GPS_MAIN_BOX_TEMPERATURE_L, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fUPS_MAIN_BOX_TEMP_ALARM2 == FALSE && sm.Alarm->ALARMs[eAI_UPS_MAIN_BOX_TEMPERATURE_L].bOccurred == FALSE)
		SetAlarm(eAI_UPS_MAIN_BOX_TEMPERATURE_L, IDS_DES_NOT_USE);

	if (sm.IO->tIn_CRT.fGPS_MAIN_BOX_TEMP_ALARM1 == FALSE)
	{
		if (sm.Alarm->ALARMs[eAI_GPS_MAIN_BOX_TEMPERATURE_H].bOccurred == FALSE)
			SetAlarm(eAI_GPS_MAIN_BOX_TEMPERATURE_H, IDS_DES_NOT_USE, nullptr, FALSE);
	}
	else
		sm.Alarm->ALARMs[eAI_GPS_MAIN_BOX_TEMPERATURE_H].bReset = TRUE;

	if (sm.IO->tIn_CRT.fUPS_MAIN_BOX_TEMP_ALARM1 == FALSE)
	{
		if (sm.Alarm->ALARMs[eAI_UPS_MAIN_BOX_TEMPERATURE_H].bOccurred == FALSE)
			SetAlarm(eAI_UPS_MAIN_BOX_TEMPERATURE_H, IDS_DES_NOT_USE, nullptr, FALSE);
	}
	else
		sm.Alarm->ALARMs[eAI_UPS_MAIN_BOX_TEMPERATURE_H].bReset = TRUE;

	//smoke 관련
	// 2024.06.06 KKW SDV 사양으로 기존 A접에서 B접으로 변경하여 수정
//	if (sm.IO->tIn_CRT.fGPS_MAIN_BOX_SMOKE_DETECT_ALARM == TRUE && sm.Alarm->ALARMs[eAI_GPS_MAIN_BOX_SMOKE_DETECT].bOccurred == FALSE)
	if (sm.IO->tIn_CRT.fGPS_MAIN_BOX_SMOKE_DETECT_ALARM == FALSE && sm.Alarm->ALARMs[eAI_GPS_MAIN_BOX_SMOKE_DETECT].bOccurred == FALSE)
		SetAlarm(eAI_GPS_MAIN_BOX_SMOKE_DETECT, IDS_DES_NOT_USE);	//UND SMOKE Alarm
//	if (sm.IO->tIn_CRT.fUPS_MAIN_BOX_SMOKE_DETECT_ALARM == TRUE && sm.Alarm->ALARMs[eAI_UPS_MAIN_BOX_SMOKE_DETECT].bOccurred == FALSE)
	if (sm.IO->tIn_CRT.fUPS_MAIN_BOX_SMOKE_DETECT_ALARM == FALSE && sm.Alarm->ALARMs[eAI_UPS_MAIN_BOX_SMOKE_DETECT].bOccurred == FALSE)
		SetAlarm(eAI_UPS_MAIN_BOX_SMOKE_DETECT, IDS_DES_NOT_USE);	//UND SMOKE Alarm

	//fan 관련
	if (sm.IO->tIn_CRT.fGPS_MAIN_BOX_FAN1A_ON == FALSE && sm.Alarm->ALARMs[eAI_GPS_MAIN_BOX_FAN1A_OFF].bOccurred == FALSE)
		SetAlarm(eAI_GPS_MAIN_BOX_FAN1A_OFF, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fGPS_MAIN_BOX_FAN1B_ON == FALSE && sm.Alarm->ALARMs[eAI_GPS_MAIN_BOX_FAN1B_OFF].bOccurred == FALSE)
		SetAlarm(eAI_GPS_MAIN_BOX_FAN1B_OFF, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fGPS_MAIN_BOX_FAN1C_ON == FALSE && sm.Alarm->ALARMs[eAI_GPS_MAIN_BOX_FAN1C_OFF].bOccurred == FALSE)
		SetAlarm(eAI_GPS_MAIN_BOX_FAN1C_OFF, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fGPS_MAIN_BOX_FAN1D_ON == FALSE && sm.Alarm->ALARMs[eAI_GPS_MAIN_BOX_FAN1D_OFF].bOccurred == FALSE)
		SetAlarm(eAI_GPS_MAIN_BOX_FAN1D_OFF, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fUPS_MAIN_BOX_FAN1A_ON == FALSE && sm.Alarm->ALARMs[eAI_UPS_MAIN_BOX_FAN1A_OFF].bOccurred == FALSE)
		SetAlarm(eAI_UPS_MAIN_BOX_FAN1A_OFF, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fUPS_MAIN_BOX_FAN1B_ON == FALSE && sm.Alarm->ALARMs[eAI_UPS_MAIN_BOX_FAN1B_OFF].bOccurred == FALSE)
		SetAlarm(eAI_UPS_MAIN_BOX_FAN1B_OFF, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fUPS_MAIN_BOX_FAN1C_ON == FALSE && sm.Alarm->ALARMs[eAI_UPS_MAIN_BOX_FAN1C_OFF].bOccurred == FALSE)
		SetAlarm(eAI_UPS_MAIN_BOX_FAN1C_OFF, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fUPS_MAIN_BOX_FAN1D_ON == FALSE && sm.Alarm->ALARMs[eAI_UPS_MAIN_BOX_FAN1D_OFF].bOccurred == FALSE)
		SetAlarm(eAI_UPS_MAIN_BOX_FAN1D_OFF, IDS_DES_NOT_USE);
	//24.08.04 KWR 집진기 작업 관련 임시 주석, 해제 필요
	//if (sm.IO->tIn_CRT.fLD_DUST_CHATHER1_ELEC_BOX_SMOKE_DETECT == FALSE && sm.Alarm->ALARMs[eAI_DUST_CHATHER1_ELEC_BOX_SMOKE_DETECT].bOccurred == FALSE)
	//	SetAlarm(eAI_DUST_CHATHER1_ELEC_BOX_SMOKE_DETECT, IDS_DES_NOT_USE);
	//if (sm.IO->tIn_CRT.fLD_DUST_CHATHER1_ELEC_BOX_EMO_ON == FALSE && sm.Alarm->ALARMs[eAI_DUST_CHATHER1_ELEC_BOX_EMO_ON].bOccurred == FALSE)
	//	SetAlarm(eAI_DUST_CHATHER1_ELEC_BOX_EMO_ON, IDS_DES_NOT_USE);
	//if (sm.IO->tIn_CRT.fLD_DUST_CHATHER1_THERMOMETER_OUT1 == FALSE && sm.Alarm->ALARMs[eAI_DUST_CHATHER1_THERMOMETER_OUT1_FAN_OFF].bOccurred == FALSE)
	//	SetAlarm(eAI_DUST_CHATHER1_THERMOMETER_OUT1_FAN_OFF, IDS_DES_NOT_USE);
	//if (sm.IO->tIn_CRT.fLD_DUST_CHATHER1_THERMOMETER_OUT2 == FALSE && sm.Alarm->ALARMs[eAI_DUST_CHATHER1_THERMOMETER_OUT2_FAN_OFF].bOccurred == FALSE)
	//	SetAlarm(eAI_DUST_CHATHER1_THERMOMETER_OUT2_FAN_OFF, IDS_DES_NOT_USE);
	//if (sm.IO->tIn_CRT.fLD_DUST_CHATHER1_ELEC_BOX_FAN1_ON == FALSE && sm.Alarm->ALARMs[eAI_DUST_CHATHER1_ELEC_BOX_FAN1_OFF].bOccurred == FALSE)
	//	SetAlarm(eAI_DUST_CHATHER1_ELEC_BOX_FAN1_OFF, IDS_DES_NOT_USE);
	//if (sm.IO->tIn_CRT.fLD_DUST_CHATHER1_ELEC_BOX_FAN2_ON == FALSE && sm.Alarm->ALARMs[eAI_DUST_CHATHER1_ELEC_BOX_FAN2_OFF].bOccurred == FALSE)
	//	SetAlarm(eAI_DUST_CHATHER1_ELEC_BOX_FAN2_OFF, IDS_DES_NOT_USE);
	//<< Dust Chather, 이오나이저는 Autorun 중일 때만 체크
	if (sm.Sequence->fAutoMode == TRUE && sm.Sequence->fCycleStop == FALSE) {

		//이오나이저 관련
		BOOL bAlarmCheck = TRUE;
		bAlarmCheck &= sm.IO->tIn_CRT.fLD_ION1_RUN_STOP_STATE;
		bAlarmCheck &= sm.IO->tIn_CRT.fLD_IONIZER_PRESSURE_CHECK_SWITCH;
		//24.08.04 KWR 집진기 작업 관련 임시 주석, 해제 필요
		//bAlarmCheck &= sm.IO->tIn_CRT.fLD_DUST_CHATHER1_ON_SIGNAL;
		//bAlarmCheck &= sm.IO->tIn_CRT.fLD_DUST_CHATHER1_INVERTER_ON_SIGNAL;
		if (!bAlarmCheck && !s_bAlarmOccurCheck) {
			s_bAlarmOccurCheck = TRUE;
			s_AlarmoccurTime = GetTickCount64();
		}
		if (bAlarmCheck) {
			s_bAlarmOccurCheck = FALSE;
		}
		if (s_bAlarmOccurCheck && GetTickCount64() - s_AlarmoccurTime > 3000) {
			if (sm.IO->tIn_CRT.fLD_IONIZER_PRESSURE_CHECK_SWITCH == FALSE && sm.Alarm->ALARMs[eAl_IONIZER_PRESSURE_CHECK_SWITCH].bOccurred == FALSE)
				SetAlarm(eAl_IONIZER_PRESSURE_CHECK_SWITCH, IDS_DES_NOT_USE);
			if (sm.IO->tIn_CRT.fLD_ION1_RUN_STOP_STATE == FALSE && sm.Alarm->ALARMs[eAl_ION1_STOP_STATE].bOccurred == FALSE)
				SetAlarm(eAl_ION1_STOP_STATE, IDS_DES_NOT_USE);
			//24.08.04 KWR 집진기 작업 관련 임시 주석, 해제 필요
			//Dust Chather 관련
			//if (sm.IO->tIn_CRT.fLD_DUST_CHATHER1_ON_SIGNAL == FALSE && sm.Alarm->ALARMs[eAl_DUST_CHATHER1_OFF].bOccurred == FALSE)
			//	SetAlarm(eAl_DUST_CHATHER1_OFF, IDS_DES_NOT_USE);
			//if (sm.IO->tIn_CRT.fLD_DUST_CHATHER1_INVERTER_ON_SIGNAL == FALSE && sm.Alarm->ALARMs[eAl_DUST_CHATHER1_INVERTER_OFF].bOccurred == FALSE)
			//	SetAlarm(eAl_DUST_CHATHER1_INVERTER_OFF, IDS_DES_NOT_USE);
			//s_bAlarmOccurCheck = FALSE;
		}
		if (sm.IO->tIn_CRT.fLD_ION1_HIGH_VOLTAGE_ABNORMAL == TRUE && sm.Alarm->ALARMs[eAl_ION1_HIGH_VOLTAGE_ABNORMAL].bOccurred == FALSE)
			SetAlarm(eAl_ION1_HIGH_VOLTAGE_ABNORMAL, IDS_DES_NOT_USE);
		if (sm.IO->tIn_CRT.fLD_ION1_TIP_CLEANING_ALARM == TRUE && sm.Alarm->ALARMs[eAl_ION1_TIP_CLEANING_ALARM].bOccurred == FALSE)
			SetAlarm(eAl_ION1_TIP_CLEANING_ALARM, IDS_DES_NOT_USE);
	}
	//>>

	//24.08.04 KWR 집진기 작업 관련 임시 주석, 해제 필요
	//<< 집진기 압력 수치에 따른 집진기 알람 추가 LJY 2024.07.24
	//if (sm.SerialConnection->bDustCollectorPressure[eDUST_COLLECTOR_1] == TRUE && sm.SerialConnection->bDustCollectorControlPanel[eDUST_COLLECTOR_1] == TRUE) {
	//	if ((sm.DustCollector->data[eDUST_COLLECTOR_1].iPressure >= sm.Option->tDustColletorOption.iLightAlarmLimit)
	//		&& sm.Alarm->ALARMs[eAI_DUST_CHAPTER_PRESS_LIGHT_ALARM_LIMIT].bOccurred == FALSE)
	//		SetAlarm(eAI_DUST_CHAPTER_PRESS_LIGHT_ALARM_LIMIT, IDS_DES_NOT_USE);
	//}
	//if (sm.SerialConnection->bDustCollectorPressure[eDUST_COLLECTOR_1] == TRUE && sm.SerialConnection->bDustCollectorControlPanel[eDUST_COLLECTOR_1] == TRUE) {
	//	if ((sm.DustCollector->data[eDUST_COLLECTOR_1].iPressure >= sm.Option->tDustColletorOption.iHeavyAlarmLimit)
	//		&& sm.Alarm->ALARMs[eAI_DUST_CHAPTER_PRESS_HEAVY_ALARM_LIMIT].bOccurred == FALSE)
	//		SetAlarm(eAI_DUST_CHAPTER_PRESS_HEAVY_ALARM_LIMIT, IDS_DES_NOT_USE);
	//}

	//if (sm.SerialConnection->bDustCollectorPressure[eDUST_COLLECTOR_1] == TRUE && sm.SerialConnection->bDustCollectorControlPanel[eDUST_COLLECTOR_1] == TRUE) {
	//	if ((sm.DustCollector->data[eDUST_COLLECTOR_1].iRunningTime >= sm.Option->tDustColletorOption.iFilterCycleLimit)
	//		&& sm.Alarm->ALARMs[eAI_DUST_CHAPTER_FILTER_CYCLE_LIMIT].bOccurred == FALSE)
	//		SetAlarm(eAI_DUST_CHAPTER_FILTER_CYCLE_LIMIT, IDS_DES_NOT_USE);
	//}
	//>>

	//EMO 관련
	if (sm.IO->tIn_CRT.fLD_EMO_01_ON == FALSE && sm.Alarm->ALARMs[eAI_EMO01_ON].bOccurred == FALSE)
		SetAlarm(eAI_EMO01_ON, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fLD_EMO_02_ON == FALSE && sm.Alarm->ALARMs[eAI_EMO02_ON].bOccurred == FALSE)
		SetAlarm(eAI_EMO02_ON, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fLD_EMO_03_ON == FALSE && sm.Alarm->ALARMs[eAI_EMO03_ON].bOccurred == FALSE)
		SetAlarm(eAI_EMO03_ON, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fLD_EMO_04_ON == FALSE && sm.Alarm->ALARMs[eAI_EMO04_ON].bOccurred == FALSE)
		SetAlarm(eAI_EMO04_ON, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fLD_EMO_05_ON == FALSE && sm.Alarm->ALARMs[eAI_EMO05_ON].bOccurred == FALSE)
		SetAlarm(eAI_EMO05_ON, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fLD_EMO_06_ON == FALSE && sm.Alarm->ALARMs[eAI_EMO06_ON].bOccurred == FALSE)
		SetAlarm(eAI_EMO06_ON, IDS_DES_NOT_USE);

	//Box Door 관련
	if (sm.IO->tIn_CRT.fUPS1_BOX_DOOR_LIMIT_S_W == FALSE && sm.Alarm->ALARMs[eAl_UPS1_BOX_DOOR_LIMIT_S_W].bOccurred == FALSE)
		SetAlarm(eAl_UPS1_BOX_DOOR_LIMIT_S_W, IDS_DES_NOT_USE);
	if (sm.IO->tIn_CRT.fGPS_BOX_DOOR_LIMIT_S_W == FALSE && sm.Alarm->ALARMs[eAl_GPS_BOX_DOOR_LIMIT_S_W].bOccurred == FALSE)
		SetAlarm(eAl_GPS_BOX_DOOR_LIMIT_S_W, IDS_DES_NOT_USE);


	auto currentTime = GetTickCount64();
	if (sm.IO->tIn_CRT.fLD_MAIN_CDA_A1_1_PRESSURE_CHECK_SWITCH == FALSE ||
		sm.IO->tIn_CRT.fLD_MAIN_CDA_A1_2_PRESSURE_CHECK_SWITCH == FALSE ||
		sm.IO->tIn_CRT.fLD_TRANSFER_VACUUM_MAIN_CHECK_SWITCH == FALSE ){
		if (currentTime - elapsedTime > 5000.)
			isMainPressAlarmOuccured = true;
	}
	else{
		elapsedTime = currentTime;
		isMainPressAlarmOuccured = false;
	}
	if (isMainPressAlarmOuccured && (sm.Alarm->ALARMs[eAl_MAIN_CDA_A1_1_PRESSURE_CHECK_SWITCH].bOccurred == FALSE ||
		sm.Alarm->ALARMs[eAl_MAIN_CDA_A1_2_PRESSURE_CHECK_SWITCH].bOccurred == FALSE ||
		sm.Alarm->ALARMs[eAl_MG_TRANSFER_1_VACUUM_MAIN_CHECK_SWITCH].bOccurred == FALSE)) {
		//기타
		if (sm.IO->tIn_CRT.fLD_MAIN_CDA_A1_1_PRESSURE_CHECK_SWITCH == FALSE && sm.Alarm->ALARMs[eAl_MAIN_CDA_A1_1_PRESSURE_CHECK_SWITCH].bOccurred == FALSE)
			SetAlarm(eAl_MAIN_CDA_A1_1_PRESSURE_CHECK_SWITCH, IDS_DES_NOT_USE);
		if (sm.IO->tIn_CRT.fLD_MAIN_CDA_A1_2_PRESSURE_CHECK_SWITCH == FALSE && sm.Alarm->ALARMs[eAl_MAIN_CDA_A1_2_PRESSURE_CHECK_SWITCH].bOccurred == FALSE)
			SetAlarm(eAl_MAIN_CDA_A1_2_PRESSURE_CHECK_SWITCH, IDS_DES_NOT_USE);
		//2024.06.24 LBH - 주기적으로 Alarm 발생하여 주석 처리. 2024.07.17 alive by kkw
		if (sm.IO->tIn_CRT.fLD_TRANSFER_VACUUM_MAIN_CHECK_SWITCH == FALSE && sm.Alarm->ALARMs[eAl_MG_TRANSFER_1_VACUUM_MAIN_CHECK_SWITCH].bOccurred == FALSE)
			SetAlarm(eAl_MG_TRANSFER_1_VACUUM_MAIN_CHECK_SWITCH, IDS_DES_NOT_USE);
	}
	//발목감지센서 관련
	// LD 감지시 Servo Down + Auto Stop
	if (GetAnkleDetectionSensorLD(TRUE) && sm.Alarm->ALARMs[eAI_LD_ANKLE_DETECTION_SENSOR_DETECTED].bOccurred == FALSE) {
		AllServoOnOff(FALSE);
		SetAlarm(eAI_LD_ANKLE_DETECTION_SENSOR_DETECTED, IDS_DES_NOT_USE);
		SetAutoStop();
	}
	// SCT 감지시 CycleStop
	if (GetAnkleDetectionSensorSCT(TRUE) && sm.Alarm->ALARMs[eAI_SCT_ANKLE_DETECTION_SENSOR_DETECTED].bOccurred == FALSE) {
		SetAlarm(eAI_SCT_ANKLE_DETECTION_SENSOR_DETECTED, IDS_DES_NOT_USE);
	}
}
void AlarmChecker::DoorCheck() {
	BOOL bKeySwitch_Teach = TRUE;
	bKeySwitch_Teach &= sm.IO->tIn_CRT.fLD_OP_SELECT_KEY_SWITCH_AUTO_POSITION; 
	bKeySwitch_Teach &= sm.IOs->SCT.In.fSCT_OP_SELECT_KEY_SWITCH_AUTO_POSITION;

	// Door Release
	BOOL bDoorReleaseLD = FALSE;
	BOOL bDoorReleaseSTC = FALSE;
	bDoorReleaseLD |= (sm.IO->tIn_CRT.fLD_DOOR_RELEASE_SW01 == TRUE);
	bDoorReleaseLD |= (sm.IO->tIn_CRT.fLD_DOOR_RELEASE_SW02 == TRUE);
	bDoorReleaseLD |= (sm.IO->tIn_CRT.fLD_DOOR_RELEASE_SW03 == TRUE);
	bDoorReleaseLD |= (sm.IO->tIn_CRT.fLD_DOOR_RELEASE_SW04 == TRUE);
	bDoorReleaseLD |= (sm.IO->tIn_CRT.fLD_DOOR_RELEASE_SW05 == TRUE);
	bDoorReleaseLD |= (sm.IO->tIn_CRT.fLD_DOOR_RELEASE_SW06 == TRUE);

	bDoorReleaseSTC |= (sm.IOs->SCT.In.fSCT_DOOR_RLEASE_SW01== TRUE);
	bDoorReleaseSTC |= (sm.IOs->SCT.In.fSCT_DOOR_RLEASE_SW02 == TRUE);
	bDoorReleaseSTC |= (sm.IOs->SCT.In.fSCT_DOOR_RLEASE_SW03 == TRUE);
	bDoorReleaseSTC |= (sm.IOs->SCT.In.fSCT_DOOR_RLEASE_SW04 == TRUE);

	//<--24.07.02 KWR 환경 안전 사양 추가
	BOOL bLDKeyLock = FALSE;
	bLDKeyLock |= !sm.IO->tIn_CRT.fLD_DOOR01_OUTSIDE_KEY_SIGNAL;
	bLDKeyLock |= !sm.IO->tIn_CRT.fLD_DOOR02_OUTSIDE_KEY_SIGNAL;
	
	DWORD dwSctDoorOutsideKey[] = {
	sm.IOs->SCT.In.fSCT_DOOR1_OUTSIDE_KEY_SIGNAL,
	sm.IOs->SCT.In.fSCT_DOOR2_OUTSIDE_KEY_SIGNAL,
	sm.IOs->SCT.In.fSCT_DOOR3_OUTSIDE_KEY_SIGNAL,
	sm.IOs->SCT.In.fSCT_DOOR4_OUTSIDE_KEY_SIGNAL,
	sm.IOs->SCT.In.fSCT_DOOR5_OUTSIDE_KEY_SIGNAL,
	sm.IOs->SCT.In.fSCT_DOOR6_OUTSIDE_KEY_SIGNAL,
	sm.IOs->SCT.In.fSCT_DOOR7_OUTSIDE_KEY_SIGNAL,
	//24.07.12 KWR 환경 안전 사양 적용
	sm.IOs->SCT.In.fSCT_DOOR8_OUTSIDE_KEY_SIGNAL,
	sm.IOs->SCT.In.fSCT_DOOR9_OUTSIDE_KEY_SIGNAL,
//
	sm.IOs->SCT.In.fSCT_DUMMY_DOOR_OUTSIDE_KEY_SIGNAL,
	};
	BOOL bSctKeyLock = FALSE;
	for (int i = 0; i < countof(dwSctDoorOutsideKey); i++) {
		bSctKeyLock |= !dwSctDoorOutsideKey[i];
	}
	// 2024.06.24 LBH - MDIK 요청 사항. (Door Open 하려면 E/V Z축이 Unlaod위치에 있어야 함.
	T_POS posEL = sm.MotorPos_1->EL_Convryor.Unload_Z;
	double dCurrentEL = GetPos(posEL.iAxis);
	//BOOL bELCV_SAFETY = (fabs(posEL.dTarget - dCurrentEL) > 5.) ? FALSE : TRUE;

	//도어 릴리즈 스위치가 한개라도 눌리면
	if (bDoorReleaseLD | bDoorReleaseSTC) {
		if (sm.Option->tSequence.bDoorOpenLampOn == FALSE) {
			sm.Option->tSequence.bDoorOpenLampOn = TRUE;
			CString str;
			BOOL bCheck = TRUE;
			
			//문을 열떄
			if (sm.IO->tOut_CRT.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST) {
				//24.07.02 KWR 환경 안전 사양 추가
				bCheck &= (bLDKeyLock || bSctKeyLock);
				bCheck &= (sm.Sequence->fAutoMode == FALSE || (sm.Sequence->fAutoMode == TRUE && sm.Sequence->fCycleStop == TRUE));																//LD 설비 RUN 체크
				//<<설비별 IO 수정필요
				bCheck &= sm.IO->tIn_CRT.fLD_OP_SELECT_KEY_SWITCH_AUTO_POSITION == FALSE;																										//LD 설비 키 스위치 체크
				bCheck &= (sm.PCs->ControlPC[eMACHINE_SCT].fAutoMode == FALSE || (sm.PCs->ControlPC[eMACHINE_SCT].fAutoMode == TRUE && sm.PCs->ControlPC[eMACHINE_SCT].fCycleStop == TRUE));	//SCT 설비 RUN 체크
				bCheck &= sm.IOs->SCT.In.fSCT_OP_SELECT_KEY_SWITCH_AUTO_POSITION == FALSE;	//SCT 설비 키 스위치 체크
				//bCheck &= bELCV_SAFETY;
				bCheck &= (sm.IOs->CCTL.In.fCCT_SHUTTER_CLOSE == TRUE && sm.IOs->CCTR.In.fCCT_SHUTTER_CLOSE == TRUE);
				//>>
				if (bCheck) {
					SetDoorReleaseState(TRUE);
					str += g_st[IDS_ALARM_CHECKER_DOOR_UNLOCK_SUCCESS];
				}
				else
				{
					str += g_st[IDS_ALARM_CHECKER_DOOR_UNLOCK_FAIL];
					if (sm.Sequence->fAutoMode == TRUE && sm.Sequence->fCycleStop == FALSE)
						str += g_st[IDS_ALARM_CHECKER_LD_AUTO_RUNNING];
					if (sm.IO->tIn_CRT.fLD_OP_SELECT_KEY_SWITCH_AUTO_POSITION == TRUE)
						str += g_st[IDS_ALARM_CHECKER_LD_KEY_SWITCH_AUTO];
					if ((sm.PCs->ControlPC[eMACHINE_SCT].fAutoMode == TRUE && sm.PCs->ControlPC[eMACHINE_SCT].fCycleStop == FALSE))
						str += g_st[IDS_ALARM_CHECKER_SCT_AUTO_RUNNING];
					if (sm.IOs->SCT.In.fSCT_OP_SELECT_KEY_SWITCH_AUTO_POSITION == TRUE)
						str += g_st[IDS_ALARM_CHECKER_SCT_KEY_SWITCH_AUTO];
					//if (!bELCV_SAFETY)
					//	str += _T("ELEVATOR Z 축이 Unload 위치가 아닙니다.");
					if (!(bLDKeyLock || bSctKeyLock)) {
						str += _T("Door Key Open");
					}
					if (!(sm.IOs->CCTL.In.fCCT_SHUTTER_CLOSE == TRUE && sm.IOs->CCTR.In.fCCT_SHUTTER_CLOSE == TRUE)) {
						str += _T("\nCLOSE SHUTTER OF CCTA AND CCTB");
					}
				}
			}
			//문을 잠글때
			else {
				if (CheckAllDoorCloseLD() && CheckAllDoorCloseSCT()) {
					SetDoorReleaseState(FALSE);
					str += g_st[IDS_ALARM_CHECKER_DOOR_ROCK_SUCCESS];
				}
				else {
					vector<int> vecDoorNum;
					str += g_st[IDS_ALARM_CHECKER_DOOR_LOCK_FAIL];

					if (!CheckAllDoorCloseSCT()) {
						str += g_st[IDS_ALARM_CHECKER_DOOR_NOT_CLOSED_OR_KEY_INTERNAL_KEY_BOX_SCT];
						str += g_st[IDS_ALARM_CHECKER_RELATED_DOOR];
						GetOpenDoorNumberSCT(vecDoorNum);

						for (int i = 0; i < vecDoorNum.size(); i++) {
							if (vecDoorNum.at(i) > 10)
								str += Format(L"Dummy Number : %d", vecDoorNum.at(i) - 10);
							else
								str += Format(L"Number : %d", vecDoorNum.at(i));

							if (i == vecDoorNum.size() - 1)
								str += "\r\n";
							else
								str += ", ";
						}
					}

					vecDoorNum.clear();
					if (!CheckAllDoorCloseLD()) {
						str += g_st[IDS_ALARM_CHECKER_IDS_ALARM_CHECKER_DOOR_NOT_CLOSED_OR_KEY_INTERNAL_KEY_BOX_LD];
						str += g_st[IDS_ALARM_CHECKER_RELATED_DOOR];
						GetOpenDoorNumberLD(vecDoorNum);
						for (int i = 0; i < vecDoorNum.size(); i++)
						{
							if (vecDoorNum.at(i) > 10)
								str += Format(L"Dummy Number : %d", vecDoorNum.at(i) - 10);
							else
								str += Format(L"Number : %d", vecDoorNum.at(i));

							if (i == vecDoorNum.size() - 1)
								str += "\r\n";
							else
								str += ", ";
						}
					}
				}
			}

			if (bDoorReleaseLD) {
				IPMessage(str, 10 * 1000);
			}
		}
	}
	else {
		sm.Option->tSequence.bDoorOpenLampOn = FALSE;
	}
	//24.06.28 kwr 임시 추가 환경 안전 검수 이후 정합성 확인 필요
	if (!sm.IO->tIn_CRT.fLD_DOOR01_OUTSIDE_KEY_SIGNAL) {
		SetAlarm(eAI_DOOR01_OUTSIDE_KEY, IDS_DES_NOT_USE);
	}
	if (!sm.IO->tIn_CRT.fLD_DOOR02_OUTSIDE_KEY_SIGNAL) {
		SetAlarm(eAI_DOOR02_OUTSIDE_KEY, IDS_DES_NOT_USE);
	}
	if (CheckDoorOpen_SCT_LD()) {
		if (sm.IO->tOut_CRT.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST)
			SetAlarm(eAI_ALL_DOOR_NOT_CLOSE, IDS_DES_NOT_USE);
		else
			sm.Alarm->ALARMs[eAI_ALL_DOOR_NOT_CLOSE].bReset = TRUE;
		//sm.Option->tSequence.bKeepOnDoorOpenMelody = TRUE; //24.06.06 KGH ADD : DOOR OPEN시에 MELODY 울리도록 수정
		if (sm.Sequence->fPMMode == FALSE) {
			g_ipClient.PostIPCommand("UI", "PM", "Show");
		}

		int iCountDoorOpen = GetOpenDoorCount();
		int iCountKeyIn = GetInsideKeyCount();

		if (iCountDoorOpen == iCountKeyIn)
			sm.IO->tOut_NEW.fLD_MELODY_1 = FALSE;	//멜로디 해제
		else
			sm.IO->tOut_NEW.fLD_MELODY_1 = TRUE;
	}
	else {
		sm.Alarm->ALARMs[eAI_ALL_DOOR_NOT_CLOSE].bReset = TRUE;
		sm.IO->tOut_NEW.fLD_MELODY_1 = FALSE;	//멜로디 해제
	}

	sm.IO->tOut_NEW.fLD_DOOR01_RELEASE_SW = !sm.IO->tOut_CRT.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST;
	sm.IO->tOut_NEW.fLD_DOOR02_RELEASE_SW = !sm.IO->tOut_CRT.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST;
	sm.IO->tOut_NEW.fLD_DOOR03_RELEASE_SW = !sm.IO->tOut_CRT.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST;
	sm.IO->tOut_NEW.fLD_DOOR04_RELEASE_SW = !sm.IO->tOut_CRT.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST;
	sm.IO->tOut_NEW.fLD_DOOR05_RELEASE_SW = !sm.IO->tOut_CRT.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST;
	sm.IO->tOut_NEW.fLD_DOOR06_RELEASE_SW = !sm.IO->tOut_CRT.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST;

	//2024.06.07 MDIK KKW IONIZER WHEN DOOR OPEN & KEY SWITCH TEACH
	if (!sm.IO->tIn_CRT.fLD_OP_SELECT_KEY_SWITCH_AUTO_POSITION || CheckDoorOpen_SCT_LD()) {
		sm.IO->tOut_NEW.fLD_IONIZER_POWER_ON_CR = FALSE;
		sm.IO->tOut_NEW.fLD_ION1_STOP = TRUE;
		sm.IO->tOut_NEW.fLD_IONIZER_BLOW_ON_OFF_SOL = FALSE;
	}
	else if (!CheckDoorOpen_SCT_LD() && sm.IO->tIn_CRT.fLD_OP_SELECT_KEY_SWITCH_AUTO_POSITION && !sm.IO->tIn_CRT.fLD_IONIZER_PRESSURE_CHECK_SWITCH) {
		sm.IO->tOut_NEW.fLD_IONIZER_POWER_ON_CR = TRUE;
		sm.IO->tOut_NEW.fLD_ION1_STOP = FALSE;
		sm.IO->tOut_NEW.fLD_IONIZER_BLOW_ON_OFF_SOL = TRUE;
	}
	//<--24.07.19 KWR 환경 안전 사양 적용
	for(int i = 0; i < 2; i++){
		bool bShutterCheck = i == 0 ? !sm.IOs->CCTL.In.fCCT_SHUTTER_CLOSE : !sm.IOs->CCTR.In.fCCT_SHUTTER_CLOSE;
		if (CheckDoorOpen_CCT(i)) {
			if (bShutterCheck) {
				SetAlarm(eAI_CCT_B_DOOR_OPEN_ALARM_H + (i*4), IDS_DES_NOT_USE , FALSE);
			}
			else {
				SetAlarm(eAI_CCT_B_DOOR_OPEN_ALARM_L + (i * 4), IDS_DES_NOT_USE, FALSE);
			}
		}
		if (CheckDoorOpen_ULD(i)) {
			if (bShutterCheck) {
				SetAlarm(eAI_ULD_B_DOOR_OPEN_ALARM_H + (i * 4), IDS_DES_NOT_USE, FALSE);
			}
			else {
				SetAlarm(eAI_ULD_B_DOOR_OPEN_ALARM_L + (i * 4), IDS_DES_NOT_USE, FALSE);
			}
		}
	}
	//-->24.07.19 KWR 환경 안전 사양 적용
}

void AlarmChecker::TowerLamp() {
	//2024.06.05 LBH
	//-->24.07.22 KGH 환경안전 사양 수정
		// RED
		// : Alarm Occured
		// YELLOW
		// : Not AutoRun or Cycle Stop
		// : Material can't move(Apply Later)
		// : Auto Run & Non Material
		// GREEN
		// : AutoRun & Material can move
	if (sm.Sequence->fIsAlarm) {
		SetTowerLamp(eTL_ALARM, TRUE);
	}
	else {
		
		//24.07.20 KWR MFQ 사양 적용
		if (!sm.Sequence->fAutoMode || sm.Sequence->fCycleStop ||sm.Sequence->CIMData.EQPStatus[eEQP_SCT].EQP.eRunState == eRS_IDLE)
			SetTowerLamp(eTL_NOMAL, TRUE);
		else if (sm.Sequence->fAutoMode/* && sm.Sequence->CIMData.EQPStatus[eEQP_SCT].EQP.eRunState == eRS_RUN*/)
			SetTowerLamp(eTL_AUTO, TRUE);
		//if (sm.Sequence->fAutoMode && sm.Sequence->CIMData.EQPStatus[eEQP_SCT].EQP.eRunState == eRS_RUN)
		//	SetTowerLamp(eTL_AUTO, TRUE);
		//else if(!sm.Sequence->fAutoMode || sm.Sequence->CIMData.EQPStatus[eEQP_SCT].EQP.eRunState == eRS_IDLE)
		//	SetTowerLamp(eTL_NOMAL, TRUE);
	}
	//<--24.07.22 KGH 환경안전 사양 수정

}

void AlarmChecker::MuteCheck() {

	//MUte 관련
	//if (sm.IO->tIn_CRT.fLD_OP_SELECT_KEY_SWITCH_AUTO_POSITION == TRUE)
	//{
		//// Glass Box 외부 1차 뮤팅 신호 들어올 때 처리
		//if (sm.IO->tIn_CRT.fLD_GLASS_BOX_CV_LIGHT_CURTAIN_MUTING_ON_SIGNAL == TRUE)
		//{
		//	if (sm.Option->tSequence.bMuteOn[0] == FALSE)
		//	{
		//		sm.Option->tSequence.bMuteOn[0] = TRUE;
		//
		//		// 1차 라이트 커튼 On (2차 라이트 커튼 Off)
		//		if (sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW == FALSE || sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW == FALSE)
		//		{
		//			// 로드 컨베이어 부분 안전 상태 체크
		//			BOOL isSafetyLoadConveyor1 = Serial::Inverter::GetInverterStop(eMODULE_PC1_LOAD_CONVEYOR_1);
		//			//BOOL isSafetyLoadConveyor2 = GetConveyorSensor(eMODULE_PC1_LOAD_CONVEYOR_2, FALSE);	// KES 주석 2024.3.18
		//			BOOL isSafetyLoadConveyor2 = TRUE;
		//
		//			// 언로드 컨베이어 부분 안전 상태 체크
		//			BOOL isSafetyUnloadConveyor1 = Serial::Inverter::GetInverterStop(eMODULE_PC1_UNLOAD_CONVEYOR_2);
		//			//BOOL isSafetyUnloadConveyor2 = GetConveyorSensor(eMODULE_PC1_UNLOAD_CONVEYOR_1, FALSE);	// KES 주석 2024.3.18
		//			BOOL isSafetyUnloadConveyor2 = TRUE;
		//
		//			if (isSafetyLoadConveyor1 && isSafetyLoadConveyor2 && isSafetyUnloadConveyor1 && isSafetyUnloadConveyor2)
		//			{
		//				sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW = TRUE;
		//				sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW = TRUE;
		//				sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_A_ON = TRUE;
		//				sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_B_ON = FALSE;
		//			}
		//		}
		//		// 1차 라이트 커튼 Off (2차 라이트 커튼 On)
		//		else
		//		{
		//			sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW = FALSE;
		//			sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW = FALSE;
		//			sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_A_ON = FALSE;
		//			sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_B_ON = TRUE;
		//		}
		//	}
		//}
		//else
		//{
		//	sm.Option->tSequence.bMuteOn[0] = FALSE;
		//}

		//커튼감지시 처리 - 감지상태이면 FALSE
	if (sm.IO->tIn_CRT.fLD_GLASS_BOX_CV_LIGHT_CURTAIN_MUTING_ON_SIGNAL == FALSE && sm.Alarm->ALARMs[eAI_GLASS_BOX_CV_LIGHT_CURTAIN_NORMAL].bOccurred == FALSE)
	{
		if (sm.IO->tIn_CRT.fLD_GLASS_BOX_CV_LIGHT_CURTAIN_OSSD_1_ON == FALSE || sm.IO->tIn_CRT.fLD_GLASS_BOX_CV_LIGHT_CURTAIN_OSSD_2_ON == FALSE)
		{
			//1. 관련축 정지 - 코드내용 추가바람
			Serial::Inverter::SetInverterStop(eMODULE_PC1_LOAD_CONVEYOR_1);
			Serial::Inverter::SetInverterStop(eMODULE_PC1_UNLOAD_CONVEYOR_2);
			//2. 알람발생 (중알람)
			SetAlarm(eAI_GLASS_BOX_CV_LIGHT_CURTAIN_NORMAL, IDS_DES_NOT_USE);
		}
	}
	//}
	// 21.01.24 LGW  -> 2021.07.01 LIH 함수화 및 Load Conveyor Stopper 동작 추가
	// 1차 뮤팅이 켜져있으면 Load Conveyor Stopper Up, Unload Conveyor Stopper Down
	// 2021.08.24 1차 라이트 커튼이 고장나서 처리 추가 - 라이트 커튼이 안켜져있으면 스토퍼 모두 Up상태 유지
	// KES 주석 2024.3.18 - 에러나서 임시 주석
	//if (IsOnLightCurtain(eBoxType::Glass, eLightCurtain::On_1st))
	//{
	//	SetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1, TRUE);
	//	SetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1, FALSE);
	//}
	//else if (IsOnLightCurtain(eBoxType::Glass, eLightCurtain::On_2nd))
	//{
	//	SetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1, FALSE);
	//	SetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1, TRUE);
	//}
	//else 
	//{
	//	SetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1, TRUE);
	//	SetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1, TRUE);
	//}
	//
	//if (IsOnLightCurtain(eBoxType::Paper, eLightCurtain::On_1st))
	//{
	//	SetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1, TRUE);
	//	SetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1, FALSE);
	//}
	//else if (IsOnLightCurtain(eBoxType::Paper, eLightCurtain::On_2nd))
	//{
	//	SetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1, FALSE);
	//	SetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1, TRUE);
	//}
	//else
	//{
	//	SetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1, TRUE);
	//	SetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1, TRUE);
	//}


	// 21.02.25 
	// 외부에서 Mute Alarm 클리어 신호를 살려주는 경우 뮤팅관련 알람들을 클리어 하자
	//if (sm.Register->In.dw2001) {		// 명칭은 나중에 변경
	//	CIPVar var; 
	//	int iCount  = 0;
	//	// 이렇게 알람 4개 클리어 해줘야함
	//	if (sm.Alarm->ALARMs[eAI_GLASS_BOX_CV_LIGHT_CURTAIN_NORMAL1].bOccurred) {
	//		var.SetChildItem(Format("Alarm_%d", iCount), eAI_GLASS_BOX_CV_LIGHT_CURTAIN_NORMAL1);
	//		iCount++;
	//	}
	//	if (sm.Alarm->ALARMs[eAI_PAPER_BOX_CV_LIGHT_CURTAIN_NORMAL1].bOccurred) {
	//		var.SetChildItem(Format("Alarm_%d", iCount), eAI_PAPER_BOX_CV_LIGHT_CURTAIN_NORMAL1);
	//		iCount++;
	//	}
	//	if (sm.Alarm->ALARMs[eAI_GLASS_BOX_CV_LIGHT_CURTAIN_NORMAL2].bOccurred) {
	//		var.SetChildItem(Format("Alarm_%d", iCount), eAI_GLASS_BOX_CV_LIGHT_CURTAIN_NORMAL2);
	//		iCount++;
	//	}
	//	if (sm.Alarm->ALARMs[eAI_PAPER_BOX_CV_LIGHT_CURTAIN_NORMAL2].bOccurred) {
	//		var.SetChildItem(Format("Alarm_%d", iCount), eAI_PAPER_BOX_CV_LIGHT_CURTAIN_NORMAL2);
	//		iCount++;
	//	}
	//	
	//	if (iCount != 0) {
	//		var.SetChildItem("Count", iCount);
	//		g_ipClient.PostIPCommand("SystemAlarm", "Alarm", "Clear", var);
	//	}
	//}
}
//24.07.19 KWR 환경 안전 사양 적용
void AlarmChecker::BuzzerStayAlarmChecker(BOOL bAlarmChecker) {
	bAlarmChecker |= sm.Alarm->ALARMs[eAI_GPS_MAIN_BOX_SMOKE_DETECT].bOccurred == TRUE;
	bAlarmChecker |= sm.Alarm->ALARMs[eAI_UPS_MAIN_BOX_SMOKE_DETECT].bOccurred == TRUE;
	//24.08.04 KWR 집진기 작업 관련 임시 주석, 해제 필요
	//bAlarmChecker |= sm.Alarm->ALARMs[eAI_DUST_CHATHER1_THERMOMETER_OUT1_FAN_OFF].bOccurred == TRUE;
	bAlarmChecker |= sm.Alarm->ALARMs[eAI_GPS_MAIN_BOX_TEMPERATURE_H].bOccurred == TRUE;
	bAlarmChecker |= sm.Alarm->ALARMs[eAI_UPS_MAIN_BOX_TEMPERATURE_H].bOccurred == TRUE;
	s_bBuzzerStayAlarm = !bAlarmChecker;
}
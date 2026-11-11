#include "pch.h"
#include "PLCLinker.h"
#include "../Sequence/LoaderSeq.h"
BOOL GetJogInterLock(int iAxis, BOOL bDir, double dSpeed, double dFutureSec) {
	// LD 
	if (sm.IO->tIn_CRT.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
		return TRUE;
	//// SCT
	//if (sm.IOs->LD.In.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
	//	return TRUE;
	//// CCTL
	//if (sm.IOs->UNDL.In.fUND_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
	//	return TRUE;
	//// CCTR
	//if (sm.IOs->UNDR.In.fUND_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
	//	return TRUE;
	//// ULDL
	//if (sm.IO->tIn_CRT.fUND_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
	//	return TRUE;
	//// ULDR
	//if (sm.IO->tIn_CRT.fUND_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
	//	return TRUE;
	
	BOOL bReturn = FALSE;
	if (dSpeed <= 0.) {
		return bReturn = TRUE;
	}
	if (dFutureSec < 0.) {
		dFutureSec = 0.01;
	}
	
	//double dFutureTarget = (dSpeed * dFutureSec);
	//sm.MotorStatus[iAxis].tCrt.dPos + 
	
	// 2024.04.02 LBH - 현재 속도로 계산 하도록 수정.
	double dFutureTarget = (sm.MotorStatus[iAxis].tCrt.dSpeed * dFutureSec);
	if (bDir) { // true +
		dFutureTarget += sm.MotorStatus[iAxis].tCrt.dPos;
		if (sm.MotorParameter[iAxis].dLimitHigh <= dFutureTarget) {
			bReturn = TRUE;
		}
		if (sm.MotorStatus[iAxis].tCrt.bPLimit) {
			bReturn = TRUE;
		}
	}
	else { // False -
		// 2024.04.02 LBH - 현재 속도로 계산 하도록 수정.
		//dFutureTarget *= -1.;
		dFutureTarget += sm.MotorStatus[iAxis].tCrt.dPos;
		if (sm.MotorParameter[iAxis].dLimitLow >= dFutureTarget) {
			bReturn = TRUE;
		}
		if (sm.MotorStatus[iAxis].tCrt.bNLimit) {
			bReturn = TRUE;
		}
	}
	//bReturn |= CheckMotorInterLock(iAxis, dFutureTarget, bDir, true);
	return bReturn;
}

BOOL GetPTPInterLcok(int iAxis, double dTarget, BOOL bDir) {
	// LD 
	if (sm.IO->tIn_CRT.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
		return TRUE;
	//// SCT
	//if (sm.IOs->LD.In.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
	//	return TRUE;
	//// CCTL
	//if (sm.IOs->UNDL.In.fUND_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
	//	return TRUE;
	//// CCTR
	//if (sm.IOs->UNDR.In.fUND_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
	//	return TRUE;
	//// ULDL
	//if (sm.IO->tIn_CRT.fUND_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
	//	return TRUE;
	//// ULDR
	//if (sm.IO->tIn_CRT.fUND_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
	//	return TRUE;
	
	BOOL bReturn = FALSE;
	if (sm.IsSimultation()) {
		return FALSE;
	}

	//<< KES 수정 2024.1.10
	if (dTarget > sm.MotorParameter[iAxis].dLimitHigh) {	
		sm.MotorStatus[iAxis].tEtc.iMotorDesc = IDS_DES_AXIS_TARGET_SW_P_LIMIT;
		bReturn = TRUE;
	}
	if (dTarget < sm.MotorParameter[iAxis].dLimitLow) {
		sm.MotorStatus[iAxis].tEtc.iMotorDesc = IDS_DES_AXIS_TARGET_SW_N_LIMIT;
		bReturn = TRUE;
	}

	if (bDir) {
		if (sm.MotorStatus[iAxis].tCrt.bPLimit) {
			sm.MotorStatus[iAxis].tEtc.iMotorDesc = IDS_DES_AXIS_P_LIMIT;
			bReturn = TRUE;
		}
	}
	else {
		if (sm.MotorStatus[iAxis].tCrt.bNLimit) {
			sm.MotorStatus[iAxis].tEtc.iMotorDesc = IDS_DES_AXIS_N_LIMIT;
			bReturn = TRUE;
		}
	}
	//>>

	bReturn |= CheckMotorInterLock(iAxis, dTarget, bDir);
	return bReturn;
}
BOOL GetInPosition(int iAxis, double dPos) {
	BOOL bReturn = FALSE;
	double dPosTarget = fabs( sm.MotorStatus[iAxis].tCrt.dPos - dPos);
	double dInPos = sm.MotorParameter[iAxis].dInPosition ; 
	if (sm.IsSimultation()) {
		dInPos = 0.01;
	}
	if (dPosTarget < sm.MotorParameter[iAxis].dInPosition && sm.MotorStatus[iAxis].tCrt.bInPosition) {
		bReturn = TRUE;
	}

	return bReturn;
}
BOOL GetInPosition(const T_POS& data){
	return GetInPosition(data.iAxis, data.dTarget);
}
BOOL GetInPosition(const T_MOTOR_LINER& data){
	return GetInPosition(data.iAxis, data.dTarget);
}
BOOL GetInPosition(const std::vector<T_MOTOR_LINER>& data){
	BOOL bReturn = TRUE;
	for(int i = 0 ; i < data.size() ;i++){
		bReturn &= GetInPosition(data[i]);
	}
	return bReturn;
}
BOOL GetInPosition(const std::vector<T_POS>& data){
	BOOL bReturn = TRUE;
	for(int i = 0 ; i < data.size() ;i++){
		bReturn &= GetInPosition(data[i]);
	}
	return bReturn;
}
BOOL GetTimeout(int iAxis){
	return GetTimeout(iAxis, sm.MotorStatus[iAxis].tEtc.ullEstimated_time);
}
BOOL GetTimeout(int iAxis, ULONG64 ullDistanceInTime){
	BOOL bReturn = FALSE;
	sm.MotorStatus[iAxis].tEtc.ullEndTick = GetTickCount64();
	ULONG64 ulTime = (sm.MotorStatus[iAxis].tEtc.ullEndTick - sm.MotorStatus[iAxis].tEtc.ullStartTick);
	ullDistanceInTime += (sm.MotorParameter[iAxis].iMoveTimeout * 1000);
	if( ulTime > ullDistanceInTime)
		bReturn = TRUE;

	return bReturn;
}
ULONG64 GetTimeDistance(const double& dDistance, const double& dSpeed) { // sec
	ULONG64 ullReturnTime;
	ullReturnTime = fabs(dDistance/dSpeed) * 1000; // 12.20 수정
	return ullReturnTime;
}

double GetPosition(int iAxis) {
	return sm.MotorStatus[iAxis].tCrt.dPos;
}


double GetAxisC2T(T_POS Data){
	return GetAxisC2T(Data.iAxis, Data.dTarget);

}
double GetAxisC2T(T_MOTOR_LINER Data){
	return GetAxisC2T(Data.iAxis, Data.dTarget);

}
double GetAxisC2T(int iAxis, double dTarget){
	return fabs(sm.MotorStatus[iAxis].tCrt.dPos - dTarget);

}
void SetTargetPosSet(int iAxis, double dTarget, double dSpeed){
	sm.MotorStatus[iAxis].tEtc.dLastTargetPos = dTarget;
	sm.MotorStatus[iAxis].tEtc.dLastTargetSpeed = dSpeed;
}
void SetLastPosSet(int iAxis){
	sm.MotorStatus[iAxis].tEtc.dLastStartPos = sm.MotorStatus[iAxis].tCrt.dPos;
}
void SetMoveTimeout(int iAxis, ULONG64 ullDistanceInTime){

	sm.MotorStatus[iAxis].tEtc.ullStartTick = GetTickCount64();
	sm.MotorStatus[iAxis].tEtc.ullEstimated_time = ullDistanceInTime;
}

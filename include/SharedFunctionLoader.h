#pragma once
#include "SharedMemory.h"
#include "SharedFunction.h"

typedef enum {
	eTL_NOMAL,
	eTL_AUTO,
	eTL_AUTO_STOP,
	eTL_HOME,
	eTL_ALARM,
	eTL_PM,
	eTL_OPCALL,
} eTOWER_LAMP;

//<< BUFFER_LIFT PIO
void SetPIO_EQ_CONNECT(BOOL _bOn);
void SetPIO_EQ_REQUEST(BOOL _bOn);
void SetPIO_EQ_RUN(BOOL _bOn);
void SetPIO_EQ_ERROR(BOOL _bOn);
void SetPIO_EQ_INPUT(BOOL _bOn);
void SetPIO_EQ_OUTPUT(BOOL _bOn);
void SetPIO_EQ_LOAD_REQUEST(BOOL _bOn);
void SetPIO_EQ_UNLOAD_REQUEST(BOOL _bOn);
void SetPIO_EQ_ALL_OFF();

BOOL GetPIO_EQ_CONNECT(BOOL _bOn);
BOOL GetPIO_EQ_REQUEST(BOOL _bOn);
BOOL GetPIO_EQ_RUN(BOOL _bOn);
BOOL GetPIO_EQ_ERROR(BOOL _bOn);
BOOL GetPIO_EQ_INPUT(BOOL _bOn);
BOOL GetPIO_EQ_OUTPUT(BOOL _bOn);
BOOL GetPIO_EQ_LOAD_REQUEST(BOOL _bOn);
BOOL GetPIO_EQ_UNLOAD_REQUEST(BOOL _bOn);
BOOL GetPIO_EQ_ALL_OFF();

BOOL GetPIO_BF_CONNECT(BOOL _bOn);
BOOL GetPIO_BF_REQUEST(BOOL _bOn);
BOOL GetPIO_BF_RUN(BOOL _bOn);
BOOL GetPIO_BF_ERROR(BOOL _bOn);
BOOL GetPIO_BF_INPUT(BOOL _bOn);
BOOL GetPIO_BF_OUTPUT(BOOL _bOn);
BOOL GetPIO_BF_LOAD(BOOL _bOn);
BOOL GetPIO_BF_UNLOAD(BOOL _bOn);
BOOL GetPIO_BF_ALL_OFF();
//>>

BOOL GetConveyorAlarm(eMODULEs _eModule);

void SetMute(eMODULEs _eModule, BOOL _bOn, PIPCOMMAND _pCMD = nullptr);
BOOL GetMute(BOOL _bOn);

void PPBoxIDManualInput(eMODULEs _eModule, LPCTSTR strMessage, PIPCOMMAND _pCMD = nullptr);

BOOL GetAnkleDetectionSensorLD(BOOL bOn);	
BOOL GetAnkleDetectionSensorSCT(BOOL bOn);	
BOOL GetPPBoxSensor(eMODULEs _eModule, BOOL bOn = TRUE);
BOOL GetPPBoxSensor_Conveyor2Front(BOOL bOn = TRUE);

typedef enum {
	ePPBOX_ALIGNER_X_L,
	ePPBOX_ALIGNER_X_R,
	ePPBOX_ALIGNER_Y,
	ePPBOX_ALIGNER_ALL,
} ePPBOX_ALIGNER;

void SetPPBoxAlignerXY(BOOL bFWD, ePPBOX_ALIGNER ePPBoxAlign = ePPBOX_ALIGNER_ALL);
BOOL GetPPBoxAlignerXY(BOOL bFWD, BOOL bIn = FALSE, ePPBOX_ALIGNER ePPBoxAlign = ePPBOX_ALIGNER_ALL);

void SetPPBoxAlignerZ(BOOL bUp);
BOOL GetPPBoxAlignerZ(BOOL bUp, BOOL bIn = FALSE);

void SetConveyor_Stoper(eMODULEs _eModule, BOOL bUp);
BOOL GetConveyor_Stoper(eMODULEs _eModule, BOOL bUp, BOOL bIn = FALSE);

// 해당 I/O가 켜지면 Mute On 및 시퀀스 진행.
BOOL GetPPBoxManualInStart();
BOOL GetPPBoxManualInCompleted();
BOOL GetPPBoxManualOutStart();
BOOL GetPPBoxManualOutCompleted();

void SetAGVLamp_CALL(BOOL bLoad, BOOL bOn);
void SetAGVLamp_MANUAL_MODE(BOOL bLoad, BOOL bOn);
void SetAGVLamp_MUTING(BOOL bLoad, BOOL bOn);

BOOL GetAGVLamp_CALL(BOOL bLoad, BOOL bOn);
BOOL GetAGVLamp_MANUAL_MODE(BOOL bLoad, BOOL bOn);
BOOL GetAGVLamp_MUTING(BOOL bLoad, BOOL bOn);




// Transfer
enum eMOTOR_Z_POS_TYPE {
	eMOTOR_Z_UP,
	eMOTOR_Z_BEFOR,
	eMOTOR_Z_AFTER,
	eMOTOR_Z_DOWN,
};

T_POS GetGlassTransferPosZ(eMATERIAL_TYPE eMT, BOOL bLoad, eMOTOR_Z_POS_TYPE eMotorZ_Type, BOOL bAddOffset = TRUE);

void SetGlassTransferCylinderPusher(BOOL bUp);
BOOL GetGlassTransferCylinderPusher(BOOL bUp, BOOL bIn = FALSE);



//------------------- ORG -------------------
void SetGlassTransferVacuum(eMATERIAL_TYPE eMT, BOOL bOn);
BOOL GetGlassTransferVacuum(eMATERIAL_TYPE eMT, BOOL bOn, BOOL bIn = FALSE);
void SetGlassTransferBlow(BOOL bOn);
BOOL GetGlassTransferBlow(BOOL bOn, BOOL bIn = FALSE);
BOOL GetMaterialSensorCheck(eMATERIAL_TYPE eMT);

// 12.18 LGW

//문이 다 닫혔는지(문닫힘+외부키잠금)와 내부키잠금이 없는지 체크
BOOL CheckAllDoorCloseLD();
//문이 다 닫혔는지(문닫힘+외부키잠금)와 내부키잠금이 없는지 체크
BOOL CheckAllDoorCloseSCT();
//열린문이 있는지 체크
BOOL CheckDoorOpen_SCT_LD();
//24.07.19 KWR 환경 안전 사양 적용
BOOL CheckDoorOpen_CCT(int iLine);
BOOL CheckDoorOpen_ULD(int iLine);

//열린문 번호를 겟
void GetOpenDoorNumberLD(vector<int>& a);
//열린문 번호를 겟
void GetOpenDoorNumberSCT(vector<int>& a);
//열린문 갯수를 겟 (ULD Door 2번 6번에 내부키 잠금 관련 설비가 미장착 된 관계로 일단 2번 6번 주석처리)
int GetOpenDoorCount();						
//내부키잠금 갯수를 겟 (ULD Door 2번 6번에 내부키 잠금 관련 설비가 미장착 된 관계로 일단 2번 6번 주석처리)
int GetInsideKeyCount();						
//Door Release 시 필요한것 일괄 셋
void SetDoorReleaseState(BOOL bRelease);

void SetTowerLamp(eTOWER_LAMP eTL, BOOL bOn);

// SCT Shift Table Vacuum Check 관련 함수 선언 
BOOL GetShiftTableVacuum(BOOL bOn, BOOL bIn = FALSE);

double GetPPBoxDistance();

// Module 별 Conveyor 받아오는 함수 
eCONVEYOR_LD GetConveyor(eMODULEs _eModule);

//<< 2021.03.10 글라스 카운트
void GlassInput();
void GlassOutput();
//>>

BOOL CheckIsMaterialInEQP();
BOOL CheckIsMaterialInUNIT(int iUnit);
// Transfer Safty Check
BOOL GetGlassTRSafety();
// Module TactTime Log
void WriteModuleTactTimeLog(const CString& _moduleName, uint64 _tactTime);

// KES 추가 2024.4.25
void SetAutoStop();

// KES CheckMotorInterLock 함수 SharedFunction으로 위치 변경 2024.05.23
// bBeforeMove: 실제 동작하기전 인터락 체크일 경우 TRUE, 그렇지 않고 인터락 체크만 하는 경우 FALSE
BOOL CheckMotorInterLock(int iAxis, double dTarget, BOOL bDir, BOOL bBeforeMove = TRUE);
std::pair<BOOL, CString> CheckMotorInterLockMsg(int iAxis, double dTarget, BOOL bDir, BOOL bBeforeMove = FALSE);
std::pair<BOOL, CString> CheckMotorInterLockMsg(T_POS tPos);

namespace Serial {
	namespace Inverter {
		void SetInverterRun(eMODULEs _eModule, BOOL bFwd, int iFrequency, CIPCommand* _pCMD = nullptr);
		void SetInverterRun(eMODULEs _eModule, BOOL bFwd, double dSpeed, CIPCommand* _pCMD = nullptr);
		void SetInverterStop(eMODULEs _eModule, CIPCommand* _pCMD = nullptr);
		void SetInverterFrequency(eMODULEs _eModule, int iFrequency, CIPCommand* _pCMD = nullptr);
		void SetInverterSpeed(eMODULEs _eModule, double dSpeed, CIPCommand* _pCMD = nullptr);
		void SetInverterSetting(eMODULEs _eModule, int iAcc1, int iAcc2, int iDec1, int iDec2, CIPCommand* _pCMD = nullptr);

		BOOL GetInverterRun(eMODULEs _eModule);
		BOOL GetInverterStop(eMODULEs _eModule);
		BOOL GetInverterReady(eMODULEs _eModule);
		int GetInverterFrequency(eMODULEs _eModule);
		BOOL GetInverterSpeedCompare(eMODULEs _eModule, double _dSpeed, int _iPoint = 1);
		BOOL GetInverterDirection(eMODULEs _eModule, BOOL _bFwd);
		int GetInverteriAccTime1(eMODULEs _eModule);
		int GetInverteriAccTime2(eMODULEs _eModule);
		int GetInverteriDecTime1(eMODULEs _eModule);
		int GetInverteriDecTime2(eMODULEs _eModule);

		//BOOL GetConveorAlarm(eCONVEYOR_LD _ePort);
		//BOOL GetConveorAlarm(eCONVEYOR_LD _ePort);
	}

	namespace PPBoxCodeReader {
		void SetPPBoxIDReading(eMODULEs _eModule, CIPCommand* _pCMD = nullptr);
	}
}

// MCC
void INFO_INIT_MCC(T_MCC_LAYER* mcc_layer);
void CELL_INFO_TO_MCC(const T_CELL_INFO& galss, T_MCC_LAYER* mcc_layer);

// CIM
void S6F11_INTERLOCK_CONFIRM();
void S6F11_OPCALL_CONFIRM();

// Alarm
void S5F1_ALARM_REPORT_SEND(T_SYSTEM_ALARM_ITEM _Alarm, CIPCommand* _pCMD = nullptr);

// Sequence
void S6F11_CASSETTE_LOAD_REQUEST(eMODULEs _eModule, int _iPort, CIPCommand* _pCMD = nullptr);
void S6F11_CASSETTE_LOAD_COMPLETE(eMODULEs _eModule, int _iPort, BOOL _bAGV_USE, CIPCommand* _pCMD = nullptr);
void S6F11_CARRIER_RELEASE_COMPLETE(eMODULEs _eModule, int _iPort, CStringA _szCSTID, CIPCommand* _pCMD = nullptr);

void S6F11_CARRIER_INFORMATION_REQUEST(eMODULEs _eModule, int _iPort, CStringA _szCSTID, CIPCommand* _pCMD = nullptr);
void S6F11_CASSETTE_UNLOAD_REQUEST(eMODULEs _eModule, int _iPort, CIPCommand* _pCMD = nullptr);
void S6F11_CASSETTE_UNLOAD_COMPLETE(eMODULEs _eModule, int _iPort, CIPCommand* _pCMD = nullptr);

void RECV_PPBOX_LOT_DATA(eMODULEs _eModule, CStringA _strPPBOX, CIPCommand* _pCMD = nullptr);
void DELETE_PPBOX_LOT_DATA(eMODULEs _eModule, CStringA _strPPBOX, CIPCommand* _pCMD = nullptr);


//---------- Reset
void S6F11_CARRIER_INFORMATION_REQUEST_RESET(eMODULEs _eModule, int _iPort, CIPCommand* _pCMD = nullptr);
void S6F11_CARRIER_RELEASE_COMPLETE_RESET(eMODULEs _eModule, int _iPort, CIPCommand* _pCMD = nullptr);

// 2024.07.19 LJY 추가
void SyncDataDustCollector(BOOL bStore);
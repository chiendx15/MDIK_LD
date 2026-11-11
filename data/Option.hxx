
#pragma once

#include "Mocha.type.hxx"
#pragma pack(push, 1)

//<< KES 추가 2024.4.9
typedef enum {
	eDRIVE_MODE_TEACH,
	eDRIVE_MODE_MANUAL,
	eDRIVE_MODE_AUTO,
} eDRIVE_MODE;
//>>

typedef struct {
	// 공통 사항 
	BOOL						fIN;				// Title/투입금지 표시
	BOOL						fOUT;				// Title/배출금지 표시
	BOOL						bUseAGV;			// Title/AGV 사용 유무

	// 2024.06.05 LBH
	BOOL						bBuzzerStop;		// Title/Buzzer 정지 유무 (Door Buzzer 제외)

	// SCT / CCT 전용? 
	BOOL						fCuttingLineInsp_1;		// 컷팅라인 검사 1	
	BOOL						fBreakConfirmMode_1;	// 브레이크 컨펌모드 1
	BOOL						fCuttingLineInsp_2;		// 컷팅라인 검사 2	
	BOOL						fBreakConfirmMode_2;	// 브레이크 컨펌모드 2
	BOOL						fIdling;				// 공운전
	BOOL						fInfeed;				// 절입
	BOOL						fAlignConfirmMode;		// 얼라인먼트 컨펌모드

	BOOL						fInputStop1;			// 1번 라인 투입 정지
	BOOL						fInputStop2;			// 2번 라인 투입 정지
	// 환경안전 관련 
	BOOL						bPMShow;				//PM 화면상태 공유메모리
	BOOL						bKeepOnDoorOpenMelody;	//DoorOpen 멜로디 유지시키기 위한 공유메모리
	BOOL						bMuteOn[5];				//Mute 램프 유지시키기 위한 공유메모리
	BOOL						bDoorOpenLampOn;		//DoorOpen 램프 유지시키기 위한 공유메모리
	BOOL						bTowerLamp;
	BOOL						bReserve2;
	BOOL						bReserve3;
	BOOL						bReserve4;
	BOOL						bReserve5;
	ULONGLONG					ullTowerLampTickCount;
	//<< 2024.4.9 KES 추가 - 설비 운전모드 (TEACH, MANUAL, AUTO)
	eDRIVE_MODE					eDriveMode;
	//>>

	int							iReserve2;
	int							iReserve3;
	double						dReserve1;
	double						dReserve2;
}T_SEQUENCE_OPTION;

typedef struct {
	BOOL						fKeyPadUseMode;		// Key Pad 사용 모드 / 임시로 Title 에 토글 표시
	BOOL						fShowKeyPad;
}T_KEYPAD_OPTION;

//<< 2024.07.19 LJY  추가
typedef struct {
	int		iFilterCycleLimit;
	int		iLightAlarmLimit;
	int		iHeavyAlarmLimit;
} T_DUSTCOLLECTOR;
//>>

typedef struct {
	T_SEQUENCE_OPTION	tSequence;
	T_KEYPAD_OPTION		tKeyPad;
	T_DUSTCOLLECTOR		tDustColletorOption;
}T_OPTION;


#pragma pack(pop)
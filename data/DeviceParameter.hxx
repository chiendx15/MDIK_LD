#pragma once

#include "Mocha.type.hxx"
#include "Online.hxx"
#pragma pack(push, 1)

typedef enum {
	eUSER_NONE,
	eUSER_OPERATOR,
	eUSER_EQUIPMENT,
	eUSER_ADMIN,
	eUSER_SUPER,
} eUSER_GRADE;

typedef struct {
	// PPBox에서 MG Transfer로 Glass Loading 할 때
	double dLoadBeforDistance;		// [PPBox]글래스 흡착 전 거리.
	double dLoadBeforSpeed;			// [PPBox]글래스 흡착 전 속도.
	double dLoadAfterDistance;		// [PPBox]글래스 흡착 후 거리.
	double dLoadAfterSpeed;			// [PPBox]글래스 흡착 후 속도.

	// MG Transfer 에서 SCT Table로 Glass Unload 할 때.
	double dUnloadBeforDistance;	// [SCT]글래스 배출 전 거리.
	double dUnloadBeforSpeed;		// [SCT]글래스 배출 전 속도.
	double dUnloadAfterDistance;	// [SCT]글래스 배출 후 거리.
	double dUnloadAfterSpeed;		// [SCT]글래스 배출 후 속도.
	
	// Paper 로딩.
	double dTransferPaperCylinderDown;			// Cylinder Down 거리 (Paper 집는 경우 다운 거리만큼 적게 내려가야함)
}T_DEVICE_PARAMETER_DISTANCE;

typedef struct {
	double dConveyorH;				// 컨베이어가 고속으로 움직이는 경우 Time Out
	double dConveyorL;				// 컨베이어가 저속으로 움직이는 경우 Time Out
	double dConveyorStoper;			// 컨베이어 Stopper Time Out
	double dCurtain;				// Air Shower Door Time Out
	double dCleaner;				// Cleaner Time Out 
	double dAirShowerDoor;			// Air Shower Open 하고 대기하는 시간 (Open 관련 In 신호가 없어서 임시로 변수 추가)
	double dConveyorMove;			// Conveyor 가 Stop Sensor 를 감지하고 나서도 일정 시간 움직이기 위해서 추가 
	double dLogoutTime;				// KES 추가 2024.2.14 - 조작없을 시 대기화면으로 전환되는 시간
	double dAlignTime;				// LJY 추가 2024.04.24 - PPBOX Align R FWD후 BWD 동작정에 딜레이 추가
}T_DEVICE_PARAMETER_TIME;

typedef struct {
	int GlassRetryCount;			// MG Transfer 가 Glass 를 집는 경우 Vacuum 이 들어오지 않을 때 재시도 하는 횟수 
	int PaperRetryCount;			// MG Transfer 가 Paper 를 집는 경우 Vacuum 이 들어오지 않을 때 재시도 하는 횟수
	int iAlignRetryCount;			// Glass Lift 에서 Align 진행 시 반복 횟수
}T_DEVICE_PARAMETER_COUNT;

typedef struct {
	BOOL bDryRun;
	BOOL bPPBoxInOutSwitchUse;		// 2024.05.01 LBH - Dry Run시 In / Out Switch 사용 할것인지.
	BOOL bGanziAlarmSkip;
	BOOL bPCLinkerUse;				// TRUE : 뒷장비랑 연결 / FALSE : 단동
}T_DEVICE_PARAMETER_USE;

// LJY 추가 2024.3.20
typedef struct {
	eLANGUAGE eLanguage;
} T_DEVICE_PARAMETER_SYSTEM;

typedef struct {
	T_DEVICE_PARAMETER_DISTANCE	tDistance;
	T_DEVICE_PARAMETER_TIME		tTime;
	T_DEVICE_PARAMETER_COUNT	tCount;
	T_DEVICE_PARAMETER_USE		tUse;
	T_DEVICE_PARAMETER_SYSTEM	System;					// LJY 추가 2024.3.20
	eUSER_GRADE					eUserGrade;				// 다른 Project 에서도 권한이 필요한 경우가 있을 수 있어서 사용
	wchar_t						szCurUserID[256];		// 다른 Project 에서도 현재 Login 한 유저의 ID가 필요한 경우가 있을 수 있어서 추가
}T_DEVICE_PARAMETER;
#pragma pack(pop)


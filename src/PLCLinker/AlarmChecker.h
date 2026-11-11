#pragma once

//. PC 모듈마다 외부적으로 하는 일은 같아도, 내부적으로 하는 동작은 다르므로 함수화.
// Singleton 처럼 사용하고 싶긴한데... 그냥 static 함수만 호출
class AlarmChecker
{
public:
	AlarmChecker();
	~AlarmChecker();

	static void AlarmChecker_MOTOR();
	static void AlarmChecker_IO();

	//. 아직은 사용안하는데, 해당 파일이 어느 PC에서 사용하는지 구분.
	static inline LPCTSTR m_pszModuleName = _T("LD");

	static BOOL s_bMotorPlusLimit[eMOTOR_END];
	static size_t s_plusLimitElapsedTime[eMOTOR_END];
	static BOOL s_bMotorMinusLimit[eMOTOR_END];
	static size_t s_minusLimitElapsedTime[eMOTOR_END];
	//24.07.29 kwr add
	static BOOL isMainPressAlarmOuccured;
	static ULONGLONG AlarmChecker::elapsedTime;
	//24.07.19 kkw add
	static BOOL s_bBuzzerStayAlarm;

	static void SafetyAlarm();
	static void DoorCheck();
	static void MuteCheck();
	static void TowerLamp();
	//24.07.19 KWR 환경 안전 사양 적용
	static void BuzzerStayAlarmChecker(BOOL bAlarmChecker);
	
	static BOOL s_bAlarmOccurCheck;
	static size_t s_AlarmoccurTime;

	ULONGLONG m_uTickDoorRelease;
};


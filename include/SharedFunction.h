#pragma once
#include "SharedMemory.h"
#include "MCC_Define.h"
#include "magic_enum.hpp"
#include "Recipe.h"
#include "AlarmEnum.h"
#include "SequenceLoader.hxx"
#include "StringTable/StringTable.h"  // LJY 추가 2024.02.29
#include "StringTable/StringTable_Description.h"	// KES 추가 2024.3.26

#define DBL_COMPARE(a, b) fabs(a - b) <= 0.0001

extern CSharedMemory		sm;
extern CIPClient			g_ipClient;
extern CIPClient			g_ipCIMClient;
// 0 : A Line
// 1 : B Line
// -1 : false
extern int g_iLine;
extern T_MDI_RECIPE*		g_pRecipe;
extern const CString		DEFAULT_DIRECTORY;
extern DStringTable			g_stDescription;	// KES 추가 2024.05.23
extern DStringTable			g_stMotor;	// KES 추가 2024.05.23
extern DStringTable         g_st;    // LJY 추가 2024.2.29

inline constexpr DWORD SENSOR_ON = 1;
inline constexpr DWORD SENSOR_OFF = 0;


// scope exit - 함수 종료 시점에 지정해둔 람다함수가 호출 됨
// return으로 함수 종료시킬 때 꼭 실행시키고 싶은 부분에 사용
// 이외에 .. 파일 닫기, Window message 호출, 포인터 삭제, 문자 마지막에 원하는 내용 추가 등
// 사용 방법 -> auto cleanup = make_scope_exit (람다함수);
template <typename T>
struct scope_exit
{
	scope_exit(T&& _t) : func{ std::move(_t) } {}
	~scope_exit() { func(); }
	T func;
};

template <typename T>
scope_exit<T> make_scope_exit(T &&t)
{
	return scope_exit<T>
	{
		std::move(t)
	};
}
// g_line 세팅
bool SetProfileLine();

// Alarm
BOOL LoadAlarm(const CString& _file = _T("LOADER_AlarmCode.csv"));
// KES 수정 2024.3.26 - SetAlarm 함수 최신버전으로 수정
void SetAlarm(const int _eAlarmCode, const int _iAlarmDesc, const CString& _szSeqStep, const T_POS& _ptTarget, PIPCOMMAND _pCmd = nullptr, const BOOL _bReset = true);
void SetAlarm(const int _eAlarmCode, const int _iAlarmDesc, const CString& _szSeqStep, PIPCOMMAND _pCmd = nullptr, const BOOL _bReset = true);
void SetAlarm(const int _eAlarmCode, const int _iAlarmDesc, PIPCOMMAND _pCmd = nullptr, const BOOL _bReset = true);

void ResetAlarm(const int _eAlarmCode);
void ClearAlarm();
bool GetAlarmState();

// part time 타이머
void SetTimeout(eMODULEs _eModule);
void SetTimeout(T_SEQUENCE_MODULE_INFO* _pModule);
bool GetTimeout(eMODULEs _eModule, __uint64 _timeout_ms);
bool GetTimeoutSec(eMODULEs _eModule, double _dtimeout_ms);
bool GetTimeoutSec(T_SEQUENCE_MODULE_INFO* _pModule, double _dtimeout_ms);
bool GetTimeout(T_SEQUENCE_MODULE_INFO* _pModule, __uint64 _timeout_ms);

ULONGLONG GetDelayTime(eMODULEs _eModule);
ULONGLONG GetDelayTime(T_SEQUENCE_MODULE_INFO* _pModule);
void SetMoveTimeout(const T_POS& _pos, bool _bReset = true);
void SetMoveTimeout(int _axis, double _pos, double _speed, bool _bReset = true);

// 2024.04.26 LBH - Move IP함수 호출 실패 또는 PTPMove에서 Interlock등의 이유로 Return FALSE 됬을 때 Timeout 시간 짧게 셋팅 하는함수
void SetMoveTimeout_abnormal(int _axis, int iTimeout);

bool GetMoveTimeout(const T_POS& _pos);
bool GetMoveTimeout(int _axis) ;
bool GetMultMoveTimeout(const std::vector<T_POS>& _pos);

// 모터 이동 타이머 - T_POS 정의 되면 추가
//void SetMoveTimeout(eMODULEs _eModule, const T_POS& _pos, bool _bReset = true);
//void SetMoveTimeout(T_SEQUENCE_MODULE_INFO* _pModule, const T_POS& _pos, bool _bReset = true);
//bool GetMoveTimeout(eMODULEs _eModule);
//bool GetMoveTimeout(T_SEQUENCE_MODULE_INFO* _pModule);

// Module 상태
void SetToUpperInit(eMODULEs _eModule);
void SetToUpperInit(T_SEQUENCE_MODULE_INFO* _pModule);
void SetToLowerInit(eMODULEs _eModule);
void SetToLowerInit(T_SEQUENCE_MODULE_INFO* _pModule);

bool IsInitToUpper(eMODULEs _eModule);
bool IsInitToUpper(const T_SEQUENCE_MODULE_INFO* _pModule);
bool IsInitToLower(eMODULEs _eModule);
bool IsInitToLower(const T_SEQUENCE_MODULE_INFO* _pModule);

// 모든 모듈 상태 초기화 (자재 및 데이터 초기화 후 오토런 할 때 혹은 장비 완전 정지에 사용)
void InitModules(); 

// 자재정보 Copy
void MovePPBoxData(T_SEQUENCE_MODULE_INFO& _pModuleSource, T_SEQUENCE_MODULE_INFO& _pModuleTarget);

// 모터 이동, 정지, 위치, 인포지션
// 2022.07.29 LastPosRecovery
void LastTargetMove(int iAxis, PIPCOMMAND pCmd);

void PTPMove(const T_POS& _pos, PIPCOMMAND pCmd = nullptr, eCH_AXIS _channel = eCH_AXIS_ALL);
void PTPMove(eAxis _axis, double _dPos, double _dSpeed, PIPCOMMAND pCmd = nullptr, eCH_AXIS _channel = eCH_AXIS_ALL);
void StepMove(const T_POS& _pos, PIPCOMMAND pCmd = nullptr, eCH_AXIS _channel = eCH_AXIS_ALL);
void StepMove(eAxis _axis, double _dPos, double _dSpeed, PIPCOMMAND pCmd = nullptr, eCH_AXIS _channel = eCH_AXIS_ALL);
void MultMove(const std::vector<T_POS>& _pos, PIPCOMMAND pCmd = nullptr, eCH_AXIS _channel = eCH_AXIS_ALL);
void LinerMove(const std::vector<T_POS>& _pos, double dSpeed, PIPCOMMAND pCmd = nullptr, eCH_AXIS _channel = eCH_AXIS_ALL);
void PTPStop(eAxis _axis, PIPCOMMAND pCmd = nullptr, eCH_AXIS _channel = eCH_AXIS_ALL);
void TorqueMove(eAxis _axis, BOOL _bDir, double dTorque, double _dSpeed);
void JogMove(eAxis _axis, BOOL _bDir, double  _dSpeed);
void JogStop(eAxis _axis, eCH_AXIS _channel = eCH_AXIS_ALL);
double GetPos(int _axis);
double GetMotorPosition(eAxis _axis);
bool CheckInPos(const T_POS& _pos);
bool CheckInPos(eAxis _axis, double _dPos);
DWORD  CheckInPos(const std::vector<T_POS> &_pos);
bool CheckMultInPos(const std::vector<T_POS>& pos);
double GetAxisPos(int _axis);
void ServoOnOff(int _axis, BOOL bOn = FALSE, PIPCOMMAND pCmd = nullptr, BOOL bAll = FALSE);
void AllServoOnOff(BOOL bOn = FALSE);
void YaskawaInit();
std::vector<T_MOTOR_STATUS> SyncCloseMotorPosition(bool _bStore);

// 모터 동작 
//- 홈, 홈체크, 에러체크, 서버온, 에러리셋, 에러체크

// IO 동작..
//void SetLoadingVacuum(bool _bOnOff);
//bool GetLoadingVacuum();


// 시퀀스 로그 관련 함수
void WriteSeqLog(eMODULEs _eModule, WORD _step, WORD _nextStep, CString _description);

CString GetSeqLogFilePath(eMODULEs _eModule, CTime cTime = CTime::GetCurrentTime(), CString _directory = DEFAULT_DIRECTORY + _T("SequenceLog/"));

CString EnumToString(eMODULEs _eModule, WORD _step);

CString GetTimeStringHMS(bool _include_ms = true);
CStringA GetTimeStringYMDHMS();

// cstring to value (필요하시면 추가해서 쓰세요)
CString ToCString(bool _value);
CString ToCString(int _value);
CString ToCString(double _value, int _point = 3);
CString ToCString(CPoint2d _value, int _point = 3);
CString ToCString(const CString& _value);

// value to cstring
template <typename T>
T FromCString(const CString& _value)
{
	assert(false);
	return T{ 0, };
}
template <>
bool FromCString<bool>(const CString& _value);
template <>
int FromCString<int>(const CString& _value);
template <>
float FromCString<float>(const CString& _value);
template <>
double FromCString<double>(const CString& _value);
template <>
CString FromCString<CString>(const CString& _value);

// Message Box 함수 추가 
void IPMessage(const CString szMessage,	DWORD dwTimeout = 3000, LPCTSTR szButton = _T("OK"), LPCTSTR szTitle = _T(""));
void IPMessage(const CString szMessage, LPCTSTR szTitle, DWORD dwTimeout = 3000, LPCTSTR szButton = _T("OK"));
CString RespMessage(LPCTSTR szMessage, LPCTSTR szButtonText = _T("OK;CANCEL"), LPCTSTR szTitle = _T(""));
void RespMessageCmd(PIPCOMMAND cmd, LPCTSTR szMessage, LPCTSTR szButton, LPCTSTR szTitle = _T(""));
void RespUnitMessageCmd(PIPCOMMAND cmd, LPCSTR szUnit, LPCTSTR szMessage, LPCTSTR szButton, LPCTSTR szTitle = _T(""));

CString InPutMessage(LPCTSTR szMessage, BOOL bPassWord = FALSE, LPCTSTR szTitle = _T(""));
void AnimationMessage(LPCTSTR szMessage, int iInterval = 1000, CIPCommand* pCMD = nullptr, LPCTSTR szTitle = _T(""));


// CString 대소문자 구분 없이 검색하는 함수
inline int FindNoCase(const CString& _string, const CString& _word)
{
	if (_string.IsEmpty() || _word.IsEmpty())
	{
		return -1;
	}

	CString str = _string;
	CString word = _word;	

	str.MakeLower();
	word.MakeLower();

	return str.Find(word);
}


// char배열 DB 데이터를 _dest로 복사
bool GetDBCharValue(CADORecordset& _rs, const CString& _fieldName, char* _dest);
//
// 공백 문자 포함 char배열 DB 데이터를 _dest로 복사
bool GetDBCharString(CADORecordset& _rs, const CString& _fieldName, char* _dest, size_t _destSize = 28);


// 21.02.10 비정상 값 로그
void WriteMotorAbnoramlLog(CString strLog);
// Parameter 변경 로그
void WriteParameterLog(CString strParameter, CString strName, std::vector<CString> vec, CString strUser);

// 버튼 로그
void WriteButtonLog(CString strMain, CString strSub, CString strTab, CString strButton, CString strUser);


// 스크랩 관련 데이터 저장/불러오기
extern const CString GlassDataPath;
extern const CString CellDataPath;

bool SaveGlassData(T_CELL_INFO& _glass);
bool SaveCellData(T_CELL_INFO& _cell);

std::pair<bool, T_CELL_INFO> LoadGlassData(const CString _id);
std::pair<bool, T_CELL_INFO> LoadCellData(const CString _id);

std::list<T_CELL_INFO> GetGlassDataList();
std::list<T_CELL_INFO> GetCellDataList();

void RemoveGlassData(const CString _id);
void RemoveCellData(const CString _id);

void SET_STEP_CELL(T_STEP_CELL& itemTarget, int iSTEP, CStringA szID = "");

void WriteRecipeLog(std::vector<CString> vec);
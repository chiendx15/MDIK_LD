#include "pch.h"
#include "SharedFunction.h"
using namespace magic_enum;

CSharedMemory		sm;
CMessageFunction	mf;
CIPClient			g_ipClient;
CIPClient			g_ipCIMClient;
int					g_iLine				= -1;
T_MDI_RECIPE*		g_pRecipe			= nullptr;
const CString		DEFAULT_DIRECTORY = _T("D:/MDIK/");
DStringTable		g_stDescription;	// KES 추가 2024.05.23
DStringTable		g_stMotor;	// KES 추가 2024.05.23
DStringTable		g_st; // LJY 추가 2024.02.29

bool SetProfileLine()
{
	// 실행 될 라인 체크
	CProfile profile(_T("../cfg/Mocha.ini"));
	g_iLine = profile.GetProfileValue(_T("Line"), _T("Number"), -1);
	if (g_iLine != 0 && g_iLine != 1) // <- 0, 1 라인이 아니면 에러
	{
		return false;
	}
	
	return true;
}

BOOL LoadAlarm(const CString& _file)
{
#define CSV_COLUMN_COUNT 10
	//. 24.01.09. JYL StringTable 경로 수정
	CString alarmFilePath = g_ipClient.GetAbsPath(_T("../data/StringTable/") + _file);
	auto& Alarm = *sm.Alarm;
	Alarm.nALARM = 0;
	try
	{
		CFile f(alarmFilePath, CFile::modeRead | CFile::shareDenyNone);
		CArchiveU ar(&f, CArchiveU::load);
		ar.ReadEncodingSignature();

		CString line;
		ar.ReadStringU(line);
		while (ar.ReadStringU(line) && Alarm.nALARM < _countof(Alarm.ALARMs))
		{
			if (line.IsEmpty())
				continue;
			CStrings strs;
			SplitString(line, ',', strs);

			if (strs.N() < CSV_COLUMN_COUNT)
				continue;

			// KES 수정 2024.1.9 
			// Start Index, ID, Type(Light = 1, Heavy = 2), Unit, Area(발생 구역, 한글,영문,베트남), Text(한글,영문,베트남)
			auto& item = Alarm.ALARMs[Alarm.nALARM++];
			
			item.iIndex = Alarm.nALARM - 1;
			item.iUnit = _ttoi(strs[3]);
			item.iALID = _ttoi(strs[0]) + _ttoi(strs[1]);
			item.iALCD = _ttoi(strs[2]);
			strcpy_s(item.szALTX	, _countof(item.szALTX)	, CStringA(strs[8]));
			for (int iLanguage = 0; iLanguage < nLANGUAGE; iLanguage++) {
				wcscpy_s(item.AlarmData[iLanguage].szArea, _countof(item.AlarmData[iLanguage].szArea), strs[4 + iLanguage]);
				wcscpy_s(item.AlarmData[iLanguage].szText, _countof(item.AlarmData[iLanguage].szText), strs[7 + iLanguage]);
			}
		}
	}
	catch (...)
	{
		//assert(false);
		IPMessage(_T("Alarm File Read Fail"));
		return FALSE;
	}

#undef CSV_COLUMN_COUNT
	return TRUE;
}

void SetAlarm(const int _eAlarmCode, const int _iAlarmDesc, const CString& _szSeqStep, const T_POS& _ptTarget, PIPCOMMAND _pCmd, const BOOL _bReset)
{
	T_SYSTEM_ALARM_ITEM* pAlarm = nullptr;
	if (_eAlarmCode < sm.Alarm->nALARM) {
		pAlarm = &sm.Alarm->ALARMs[_eAlarmCode];
	}

	if (pAlarm == nullptr)
	{
		IPMessage(Format(_T("Not Register Alarm Index : %d"), _eAlarmCode));
		return;
	}

	T_SYSTEM_ALARM_ITEM& Alarm = *pAlarm;

	Alarm.bOccurred = TRUE;
	Alarm.bReset = _bReset;

	CStringA strSeqStep(_szSeqStep);
	strncpy_s(Alarm.szSeqStep,		countof(Alarm.szSeqStep),		strSeqStep,		countof(Alarm.szSeqStep) - 1);

	Alarm.iAlarmDesc = _iAlarmDesc;

	if (_ptTarget.iAxis != -1) {
		Alarm.iMotorDesc = sm.MotorStatus[_ptTarget.iAxis].tEtc.iMotorDesc;
		double dCurrentPos = GetMotorPosition((eAxis)_ptTarget.iAxis);
		CStringA strTarget = FormatA("Axis: %d / Current: %.3f / Target: %.3f", _ptTarget.iAxis+1, dCurrentPos, _ptTarget.dTarget);
		strncpy_s(Alarm.szTargetPos,	countof(Alarm.szTargetPos),		strTarget,		countof(Alarm.szTargetPos) - 1);
	}
	else {
		// KES 2024.1.10
		// StringTable_Description.h에 있는 IDS_DES_NOT_USE 로 선언할 경우(Alarm.iMotorDesc = IDS_DES_NOT_USE) StringTable을 
		// 사용하지 않는 모든 프로젝트에 각각 StringTable_Description.h를 Include 해줘야 때문에 비효율적이므로 -1을 넣도록 함
		Alarm.iMotorDesc = -1;	
		memset(Alarm.szTargetPos, 0x00, sizeof(Alarm.szTargetPos));
	}
	
	// 오토 상태 + 헤비 알람인 경우 일시정지
	// KES 수정 2024.1.17 - 기존 MDI 코드와 동일하게 수정
	if (sm.Sequence->fAutoMode && Alarm.iALCD == 2)
	{
		sm.Sequence->fCycleStop = TRUE;
	}

	// 단동 동작 중 Alarm 발생시 자동으로 에러 발생 시켜서 동작 멈춤
	if (_pCmd != nullptr)
	{
		_pCmd->ReserveError();
	}
}

void SetAlarm(const int _eAlarmCode, const int _iAlarmDesc, const CString& _szSeqStep, PIPCOMMAND _pCmd, const BOOL _bReset)
{
	SetAlarm(_eAlarmCode, _iAlarmDesc, _szSeqStep, T_POS(), _pCmd, _bReset);
}

void SetAlarm(const int _eAlarmCode, const int _iAlarmDesc, PIPCOMMAND _pCmd, const BOOL _bReset)
{
	SetAlarm(_eAlarmCode, _iAlarmDesc, _T(""), T_POS(), _pCmd, _bReset);
}

bool GetAlarmState()
{
	bool bAlarmOccurred = false;
	for (int iAlarm = 0; iAlarm < sm.Alarm->nALARM; ++iAlarm)
	{
		auto& alarm = sm.Alarm->ALARMs[iAlarm];
		bAlarmOccurred |= alarm.bOccurred;
		if (bAlarmOccurred)
			break;
	}
	return bAlarmOccurred;
}

void ResetAlarm(const int _eAlarmCode)
{	
	sm.Alarm->ALARMs[_eAlarmCode].bReset = true;
}

void ClearAlarm()
{
	g_ipClient.PostIPCommand("SystemAlarm", "Alarm", "Clear");
}

void SetTimeout(eMODULEs _eModule)
{
	assert(_eModule > eMODULE_NONE && _eModule < nMODULEs);
	SetTimeout(&sm.Module[_eModule]);
}

void SetTimeout(T_SEQUENCE_MODULE_INFO* _pModule)
{
	if (_pModule == nullptr)
		return;

	_pModule->Status.stepTime = GetTickCount64();
}

bool GetTimeout(eMODULEs _eModule, __uint64 _timeout_ms)
{
	assert(_eModule > eMODULE_NONE && _eModule < nMODULEs);  //Ktra điều kiện. Sai => Hiện thông báo lỗi??
	return GetTimeout(&sm.Module[_eModule], _timeout_ms);
}

bool GetTimeoutSec(eMODULEs _eModule, double _dtimeout_ms)
{
	assert(_eModule > eMODULE_NONE && _eModule < nMODULEs);
	return GetTimeout(&sm.Module[_eModule], (__uint64)(_dtimeout_ms * 1000));
}
bool GetTimeoutSec(T_SEQUENCE_MODULE_INFO* _pModule, double _dtimeout_ms)
{
	if (_pModule == nullptr)
		return false;

	return GetTimeout(_pModule, (__uint64)(_dtimeout_ms * 1000));
}
bool GetTimeout(T_SEQUENCE_MODULE_INFO* _pModule, __uint64 _timeout_ms)
{
	if (_pModule == nullptr)
		return false;

	if ((GetTickCount64() - _pModule->Status.stepTime) < _timeout_ms)
		return false;

	return true;
}
//
ULONGLONG GetDelayTime(eMODULEs _eModule)
{
	assert(_eModule > eMODULE_NONE && _eModule < nMODULEs);
	return GetDelayTime(&sm.Module[_eModule]);
}

ULONGLONG GetDelayTime(T_SEQUENCE_MODULE_INFO* _pModule)
{
	if (_pModule)
		return GetTickCount64() - _pModule->Status.stepTime;
	return 0;
}

void SetMoveTimeout(const T_POS& _pos, bool _bReset)
{
	SetMoveTimeout(_pos.iAxis, _pos.dTarget, _pos.dSpeed, _bReset);
}

void SetMoveTimeout(int _axis, double _pos, double _speed, bool _bReset)
{
	sm.MotorStatus[_axis].tEtc.ullStartTick = GetTickCount64();

	double dCurrentPos = sm.MotorStatus[_axis].tCrt.dPos;
	double dCalcTimeOut = fabs((dCurrentPos - _pos) / _speed);
	dCalcTimeOut *= 1000.; // msec

	if (_bReset)
		sm.MotorStatus[_axis].tEtc.ullEstimated_time = (ULONGLONG)ceil(dCalcTimeOut);
	else {
		if (sm.MotorStatus[_axis].tEtc.ullEstimated_time < dCalcTimeOut)
			sm.MotorStatus[_axis].tEtc.ullEstimated_time = (ULONGLONG)ceil(dCalcTimeOut);
	}
}

// 2024.04.26 LBH - Move IP함수 호출 실패 또는 PTPMove에서 Interlock등의 이유로 Return FALSE 됬을 때 Timeout 시간 짧게 셋팅 하는함수
void SetMoveTimeout_abnormal(int _axis, int iTimeout) {
	sm.MotorStatus[_axis].tEtc.ullEstimated_time = iTimeout;
}

bool GetMoveTimeout(const T_POS& _pos)
{
	if (CheckInPos(_pos))
		return false;
	return GetMoveTimeout(_pos.iAxis);
}

bool GetMoveTimeout(int _axis) 
{
	if (_axis < eMOTOR_0 || _axis >= eMOTOR_END)   // Axis가 범위 밖인 경우 true로 리턴
		return true;
	
	// 3분 이상이면 이상있는 값이 아닐까...
	sm.MotorStatus[_axis].tEtc.ullEstimated_time = min(sm.MotorStatus[_axis].tEtc.ullEstimated_time, ULONGLONG(180 * 1000));
	if ((GetTickCount64() - sm.MotorStatus[_axis].tEtc.ullStartTick) > (sm.MotorStatus[_axis].tEtc.ullEstimated_time + (sm.MotorParameter[_axis].iMoveTimeout*1000)))
		return true;

	return false;
}

bool GetMultMoveTimeout(const std::vector<T_POS>& _pos)
{
	for (auto& data : _pos)
	{		
		if (CheckInPos(data)) continue;
		
		if (GetMoveTimeout(data))
		{
			return true;
		}
	}
	
	return false;
}

//
void SetToUpperInit(eMODULEs _eModule)
{
	assert(_eModule > eMODULE_NONE && _eModule < nMODULEs);
	SetToUpperInit(&sm.Module[_eModule]);
}

void SetToUpperInit(T_SEQUENCE_MODULE_INFO* _pModule)
{
	if (_pModule == nullptr)
		return;

	_pModule->ToUpper.fReceive_Able = SENSOR_OFF;
	_pModule->ToUpper.fReceive_Start = SENSOR_OFF;
	_pModule->ToUpper.fReceive_Complete = SENSOR_OFF;
}

void SetToLowerInit(eMODULEs _eModule)
{
	assert(_eModule > eMODULE_NONE && _eModule < nMODULEs);
	SetToLowerInit(&sm.Module[_eModule]);
}

void SetToLowerInit(T_SEQUENCE_MODULE_INFO* _pModule)
{
	if (_pModule == nullptr)
		return;

	_pModule->ToLower.fSend_Able = SENSOR_OFF;
	_pModule->ToLower.fSend_Start = SENSOR_OFF;
	_pModule->ToLower.fSend_Complete = SENSOR_OFF;
}

bool IsInitToUpper(eMODULEs _eModule)
{
	assert(_eModule > eMODULE_NONE && _eModule < nMODULEs);
	return IsInitToUpper(&sm.Module[_eModule]);
}

bool IsInitToUpper(const T_SEQUENCE_MODULE_INFO* _pModule)
{
	if (_pModule == nullptr)
		return false;

	// able은 init 이후 금방 다시 켜질 수도 있음
	// 넣는게 나을지도.. 편하신대로 수정하세요
	if (_pModule->ToUpper.fReceive_Able == SENSOR_ON
		|| _pModule->ToUpper.fReceive_Start == SENSOR_ON
		|| _pModule->ToUpper.fReceive_Complete == SENSOR_ON)
		return false;

	return true;
}

bool IsInitToLower(eMODULEs _eModule)
{
	assert(_eModule > eMODULE_NONE && _eModule < nMODULEs);
	return IsInitToLower(&sm.Module[_eModule]);
}

bool IsInitToLower(const T_SEQUENCE_MODULE_INFO* _pModule)
{
	if (_pModule == nullptr)
		return false;

	// able은 init 이후 금방 다시 켜질 수도 있음
	// 넣는게 나을지도.. 편하신대로 수정하세요
	if (_pModule->ToLower.fSend_Able == SENSOR_ON
		|| _pModule->ToLower.fSend_Start == SENSOR_ON
		|| _pModule->ToLower.fSend_Complete == SENSOR_ON)
		return false;

	return true;
}

void InitModules()
{
	for (int iModule = 0; iModule < nMODULEs; ++iModule)
	{
		memset(&sm.Module[iModule], 0x00, sizeof(T_SEQUENCE_MODULE_INFO));
		sm.Module[iModule].eModule = (eMODULEs)iModule; // 자신의 모듈 번호는 다시 세팅
	}
}

void MovePPBoxData(T_SEQUENCE_MODULE_INFO& _pModuleSource, T_SEQUENCE_MODULE_INFO& _pModuleTarget) {
	memcpy(&_pModuleTarget.Glass, &_pModuleSource.Glass, sizeof(_pModuleSource.Glass));
	memset(&_pModuleSource.Glass, 0x00, sizeof(_pModuleSource.Glass));
	_pModuleTarget.Glass.bMaterial = TRUE;
}



// 시퀀스 로그 함수 - 파일을 매번 읽지 않도록 클래스를 만들어서 처리할까...
void WriteSeqLog(eMODULEs _eModule, WORD _step, WORD _nextStep, CString _description)
{
	CFile file;
	CFileException e;
	CString path = GetSeqLogFilePath(_eModule);

	// 파일 열기
	file.Open(path, CFile::shareDenyWrite | CFile::modeNoTruncate | CFile::modeWrite | CFile::modeCreate, &e);
	if (e.m_cause != CFileException::none)
	{
		// to do message
		TRACE(_T("%s File Exception Error : %d\n"), path, e.m_cause);
		return;
	}

	// log 파일 쓰기
	CArchiveU arU(&file, CArchiveU::store);
	if (file.SeekToEnd() == 0)
	{ // 파일 처음 생성시 항목을 적어주고 시작한다
		arU.WriteStringU(_T("시간;현재 스탭;다음 스탭;현재 스탭 명칭;추가 내용\r\n"));
	}

	arU.WriteStringU(GetTimeStringHMS(true) + _T(";"));
	arU.WriteStringU(Format(_T("%d;%d;"), _step, _nextStep));
	arU.WriteStringU(EnumToString(_eModule, _step) + _T(";"));
	arU.WriteStringU(_description + _T(";"));
	arU.WriteStringU(_T("\r\n"));

	arU.Close();
	file.Close();
}

// 로그 위치(날짜 포함) + 파일명 반환
CString GetSeqLogFilePath(eMODULEs _eModule, CTime cTime, CString _directory)
{
	assert(_eModule > eMODULE_NONE && _eModule < nMODULEs);
	//CTime cTime = CTime::GetCurrentTime(); 
	CString time = Format(_T("%04d/%02d/%02d/"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());
	CString path = _directory + time;
	CreateSubDirectories(path);		// <- 폴더가 없으면 생성하도록 수정
	
	CString moduleName = CString(enum_name(_eModule).data(), (int)enum_name(_eModule).length()) + Format(_T("_%04d%02d%02d.csv"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());
	return path + moduleName;
}

// 현재 스탭을 명칭으로 변환
CString EnumToString(eMODULEs _eModule, WORD _step)
{
	switch (_eModule)
	{
		case eMODULE_PC1_LOAD_CONVEYOR_1: return CString(enum_name((eLOAD_CONVEYOR_1)_step).data());
		case eMODULE_PC1_LOAD_CONVEYOR_2: return CString(enum_name((eLOAD_CONVEYOR_2)_step).data());
		case eMODULE_PC1_ELEVATOR_CONVEYOR: return CString(enum_name((eELEVATOR_CONVEYOR)_step).data());
		case eMODULE_PC1_UNLOAD_CONVEYOR_1: return CString(enum_name((eUNLOAD_CONVEYOR_1)_step).data());
		case eMODULE_PC1_UNLOAD_CONVEYOR_2: return CString(enum_name((eUNLOAD_CONVEYOR_2)_step).data());
		case eMODULE_PC1_GLASS_TRANSFER: return CString(enum_name((eGLASS_TRANSFER)_step).data());
		break;
	}

	// 오류 체크부분 삭제하면 여기서 값 반환
	return Format(_T("<enum to string error : %d>"), _eModule);
}

// 현재 시간 문자열 반환(시,분,초,밀리세컨드)
CString GetTimeStringHMS(bool _include_ms)
{
	SYSTEMTIME cur_time;
	GetLocalTime(&cur_time);
	CString HMS = Format(_T("%02d:%02d:%02d"),
		cur_time.wHour,
		cur_time.wMinute,
		cur_time.wSecond);

	if (_include_ms)
		HMS += Format(_T(":%03d"), cur_time.wMilliseconds);

	return HMS;
}

// 2024.06.28 LBH
CStringA GetTimeStringYMDHMS() {
	SYSTEMTIME cur_time;
	GetLocalTime(&cur_time);
	CStringA szTime;

	szTime += Format("%04d", cur_time.wYear);
	szTime += Format("%02d", cur_time.wMonth);
	szTime += Format("%02d", cur_time.wDay);
	szTime += Format("%02d", cur_time.wHour);
	szTime += Format("%02d", cur_time.wMinute);
	szTime += Format("%02d", cur_time.wSecond);


	return szTime;
}

void LastTargetMove(int iAxis, PIPCOMMAND pCmd)
{
	auto& dPos = sm.MotorStatus[iAxis].tEtc.dLastTargetPos;
	auto dSpd = sm.MotorStatus[iAxis].tEtc.dLastTargetSpeed;
	// KES 수정 2024.4.14 - dLastTargetSpeed 값이 0이면 속도 5로 세팅해서 이동
	if (DBL_COMPARE(dSpd, 0))
		dSpd = 5.;
	
	if (CheckInPos(static_cast<eAxis>(iAxis), dPos)) return;

	// 5mm 이내 인 경우만 마지막 포지션 회복
	if (fabs(GetPos(iAxis) - sm.MotorStatus[iAxis].tEtc.dLastTargetPos) > dSpd) return;

	SetMoveTimeout(iAxis, dPos, dSpd);
	CIPVar var;
	CStringA strCmd = FormatA("PTPMove%d", iAxis);
	var.SetChildItem("Axis", iAxis);		//int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	var.SetChildItem("Speed", dSpd);		//double dSpeed = var.GetChildItem("Speed", -1);
	var.SetChildItem("Target", dPos);		//double dTarget = var.GetChildItem("Target", 0.);
	var.SetChildItem("Interlock", FALSE);
	sm.MotorParameter[iAxis].tYaskawa.iGantryCh = eCH_AXIS_ALL;

	sm.MotorStatus[iAxis].tEtc.bIPRun = TRUE;
	if (pCmd) 
	{
		g_ipClient.SendIPStepCommand(pCmd, "PLCLinker", NULL, "Stage", strCmd, var);
	}
	else 
	{
		g_ipClient.PostIPCommand("PLCLinker", "Stage", strCmd, var);
	}
}

void PTPMove(const T_POS& _pos, PIPCOMMAND pCmd, eCH_AXIS _channel) 
{
	PTPMove((eAxis)_pos.iAxis, _pos.dTarget, _pos.dSpeed, pCmd, _channel);
}

void PTPMove(eAxis _axis, double _dPos, double _dSpeed, PIPCOMMAND pCmd, eCH_AXIS _channel) 
{
	SetMoveTimeout(_axis, _dPos, _dSpeed);
	CIPVar var;
	CStringA strCmd = FormatA("PTPMove%d", _axis);
	var.SetChildItem("Axis", _axis);		//int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	var.SetChildItem("Speed", _dSpeed);		//double dSpeed = var.GetChildItem("Speed", -1);
	var.SetChildItem("Target", _dPos);		//double dTarget = var.GetChildItem("Target", 0.);
	sm.MotorParameter[_axis].tYaskawa.iGantryCh = _channel;

	if (CheckInPos(_axis, _dPos)) return; // 이미 도착해있으면 IPMessage를 보내지 않는다

	sm.MotorStatus[_axis].tEtc.bIPRun = TRUE;
	if (pCmd) {
		if (!g_ipClient.SendIPStepCommand(pCmd, "PLCLinker", NULL, "Stage", strCmd, var)) {
			SetMoveTimeout_abnormal(_axis, 1000);
		}
	}
	else {
		if (!g_ipClient.PostIPCommand("PLCLinker", "Stage", strCmd, var)) {
			SetMoveTimeout_abnormal(_axis, 1000);
		}
	}
}
void StepMove(const T_POS& _pos, PIPCOMMAND pCmd, eCH_AXIS _channel) {
	StepMove((eAxis)_pos.iAxis, _pos.dTarget, _pos.dSpeed, pCmd, _channel);
}
void StepMove(eAxis _axis, double _dPos, double _dSpeed, PIPCOMMAND pCmd, eCH_AXIS _channel) {
	SetMoveTimeout(_axis, _dPos, _dSpeed);
	CIPVar var;
	var.SetChildItem("Axis", _axis);		//int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	var.SetChildItem("Speed", _dSpeed);		//double dSpeed = var.GetChildItem("Speed", -1);
	var.SetChildItem("Distacne", _dPos);		//double dTarget = var.GetChildItem("Target", 0.);
	sm.MotorParameter[_axis].tYaskawa.iGantryCh = _channel;

	if (CheckInPos(_axis, _dPos)) return; // 이미 도착해있으면 IPMessage를 보내지 않는다

	if (pCmd) {
		g_ipClient.SendIPStepCommand(pCmd, "PLCLinker",NULL, "Stage", "MoveStep", var);
	}
	else {
		g_ipClient.PostIPCommand("PLCLinker", "Stage", "MoveStep", var);
	}
}
void MultMove(const std::vector<T_POS>& _pos, PIPCOMMAND pCmd, eCH_AXIS _channel) {
	size_t iSize = _pos.size();

	if (CheckMultInPos(_pos)) return;

	if (iSize) {
		CIPVar var;
		var.SetChildItem("Count", iSize);
		CStringA strAxis, strSpeed, strTarget;
		for (int i = 0; i < iSize; i++) {
			SetMoveTimeout(_pos[i].iAxis, _pos[i].dSpeed, _pos[i].dTarget);
			strAxis.Format("Axis%d", i);
			strSpeed.Format("Speed%d", i);
			strTarget.Format("Target%d", i);
			var.SetChildItem(strAxis, _pos[i].iAxis);
			var.SetChildItem(strSpeed, _pos[i].dSpeed);
			var.SetChildItem(strTarget, _pos[i].dTarget);
			sm.MotorParameter[_pos[i].iAxis].tYaskawa.iGantryCh = _channel;
		}
		if (pCmd) {
			g_ipClient.SendIPStepCommand(pCmd, "PLCLinker",NULL, "Stage", "MoveMulti", var);
		}
		else {
			g_ipClient.PostIPCommand("PLCLinker", "Stage", "MoveMulti", var);
		}
	}
}
void LinerMove(const std::vector<T_POS>& _pos, double dSpeed, PIPCOMMAND pCmd, eCH_AXIS _channel) {
	size_t iSize = _pos.size();
	if (iSize) {
		CIPVar var;
		var.SetChildItem("Count", iSize);
		var.SetChildItem("Speed", dSpeed);
		CStringA strAxis, strSpeed, strTarget;
		for (int i = 0; i < iSize; i++) {
			SetMoveTimeout(_pos[i].iAxis, _pos[i].dSpeed, _pos[i].dTarget);
			strAxis.Format("Axis%d", i);
			strTarget.Format("Target%d", i);
			var.SetChildItem(strAxis, _pos[i].iAxis);
			var.SetChildItem(strTarget, _pos[i].dTarget);	
			sm.MotorParameter[_pos[i].iAxis].tYaskawa.iGantryCh = _channel;
		}
		if (pCmd) {
			g_ipClient.SendIPStepCommand(pCmd, "PLCLinker",NULL, "Stage", "MoveLiner", var);
		}
		else {
			g_ipClient.PostIPCommand("PLCLinker", "Stage", "MoveLiner", var);
		}
	}
}
void PTPStop(eAxis _axis, PIPCOMMAND pCmd, eCH_AXIS _channel) {
	CIPVar var;
	var.SetChildItem("Axis", _axis);
	sm.MotorParameter[_axis].tYaskawa.iGantryCh = _channel;

	if (pCmd)
		g_ipClient.SendIPStepCommand(pCmd, "PLCLinker", NULL, "Stage", "MoveStop", var);
	else
		g_ipClient.PostIPCommand("PLCLinker", "Stage", "MoveStop", var);
}
void TorqueMove(eAxis _axis, BOOL _bDir, double dTorque, double _dSpeed) {
	CIPVar var;
	var.SetChildItem("Axis", _axis);		//int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	var.SetChildItem("Torque", dTorque);		//double dTarget = var.GetChildItem("Target", 0.);
	var.SetChildItem("Speed", _dSpeed);		//double dSpeed = var.GetChildItem("Speed", -1);
	var.SetChildItem("Dir", _bDir);		//double dTarget = var.GetChildItem("Target", 0.);
	g_ipClient.PostIPCommand("PLCLinker", "Stage", "TorqueMove", var);
}
void JogMove(eAxis _axis, BOOL _bDir, double  _dSpeed) {
	CIPVar var;
	var.SetChildItem("Axis", _axis);		//int iAxis = var.GetChildItem("Axis", eMOTOR_END);
	var.SetChildItem("Speed", _dSpeed);		//double dSpeed = var.GetChildItem("Speed", -1);
	var.SetChildItem("Dir", _bDir);		//double dTarget = var.GetChildItem("Target", 0.);
	g_ipClient.PostIPCommand("PLCLinker", "Stage", "MoveJog", var);
}
void JogStop(eAxis _axis, eCH_AXIS _channel) {
	CIPVar var;
	var.SetChildItem("Axis", _axis);
	sm.MotorParameter[_axis].tYaskawa.iGantryCh = _channel;
	g_ipClient.PostIPCommand("PLCLinker", "Stage", "MoveJogStop", var);
}
double GetMotorPosition(eAxis _axis) {
	return sm.MotorStatus[_axis].tCrt.dPos;
}
double GetPos(int _axis) {
	return sm.MotorStatus[_axis].tCrt.dPos;
}	
bool CheckInPos(const T_POS& _pos) {
	return CheckInPos((eAxis)_pos.iAxis, _pos.dTarget);
}
bool CheckInPos(eAxis _axis, double _dPos) {
	double dOffset = fabs(GetMotorPosition(_axis) - _dPos);
	if (sm.MotorStatus[_axis].tEtc.bIPRun) {
		return false;
	}
	return (dOffset <= sm.MotorParameter[_axis].dInPosition);
}
DWORD  CheckInPos(const std::vector<T_POS>& _pos) {
	DWORD dwReturn = 0;

	for (int i = 0; i < _pos.size(); i++) {
		if (CheckInPos(_pos[i])) {
			dwReturn |= BIT(i);
		}
	}
	return dwReturn;
}
bool CheckMultInPos(const std::vector<T_POS>& pos)
{
	for (auto& data : pos)
	{
		if (!CheckInPos(data))
		{
			return false;
		}
	}
	return true;
}
void YaskawaInit(){
	g_ipClient.PostIPCommand("PLCLinker", "Stage", "YaskawaInit");
}

std::vector<T_MOTOR_STATUS> SyncCloseMotorPosition(bool _bStore)
{
	CProfile profile(_T("../cfg/ClosePosition.ini"));

	if (_bStore)
	{
		profile.GetSections().DeleteAll();
	}

	std::vector<T_MOTOR_STATUS> status;
	status.resize(eMOTOR_END);

	CProfileSection& section = profile.GetSection(_T("Position"));
	for (int iMotor = 0; iMotor < eMOTOR_END; ++iMotor)
	{
		CString motorName = Format(_T("Motor_%02d"), iMotor);
		CProfileSection& subSection = section.GetSubSection(motorName);

		memcpy(&status[iMotor], &sm.MotorStatus[iMotor], sizeof(status[iMotor]));
		subSection.SyncItemValue(_bStore, _T("ClosePosition"), status[iMotor].tCrt.dPos, _T("%.3f"));

		BOOL bHomeComplete = status[iMotor].tEtc.dwHomeStatus == eHOME_STATUS_COMPLETED;
		subSection.SyncItemValue(_bStore, _T("HomeComplete"), bHomeComplete);
	}

	if (_bStore)
		profile.Save();

	return status;
}

double GetAxisPos(int _axis)
{
	return sm.MotorStatus[_axis].tCrt.dPos;
}
void ServoOnOff(int _axis, BOOL bOn, PIPCOMMAND pCmd, BOOL bAll) {
	CIPVar var;
	var.SetChildItem("Axis", _axis);
	var.SetChildItem("OnOff", bOn);
	var.SetChildItem("All", bAll);
	if (pCmd) {
		g_ipClient.SendIPStepCommand(pCmd, "PLCLinker",NULL, "Stage", "ServoOnOff", var);
	}else{
		g_ipClient.PostIPCommand("PLCLinker", "Stage", "ServoOnOff", var);
	}
}
void AllServoOnOff(BOOL bOn) {
	CIPVar var;
	var.SetChildItem("OnOff", bOn);
	
	g_ipClient.PostIPCommand("PLCLinker", "Stage", "ServoAllOnOFF", var);
}

// cstring to value
CString ToCString(bool _value)
{
	return _value ? _T("O") : _T("X");
}

CString ToCString(int _value)
{
	return Format(_T("%d"), _value);
}

CString ToCString(double _value, int _point)
{
	return Format(Format(_T("%%.%df"), _point), _value);
}

CString ToCString(CPoint2d _value, int _point)
{
	return Format(Format(_T("%%.%df, %%.%df"), _point, _point), _value.x, _value.y);
}

CString ToCString(const CString& _value)
{
	return _value;
}

// value to CString
template <>
bool FromCString<bool>(const CString& _value)
{
	return _value.Compare(_T("O")) == 0;
}

template <>
int FromCString<int>(const CString& _value)
{
	return _ttoi(_value);
}

template <>
float FromCString<float>(const CString& _value)
{
	return (float)_ttof(_value);
}

template <>
double FromCString<double>(const CString& _value)
{
	return _ttof(_value);
}

template <>
CString FromCString<CString>(const CString& _value)
{
	return _value;
}

bool GetDBCharValue(CADORecordset& _rs, const CString& _fieldName, char* _dest)
{
	CString value;
	if (!_rs.GetFieldValue(_fieldName, value))
	{
		return false;
	}

	value.Trim();
	memcpy(_dest, CStringA(value).GetString(), CStringA(value).GetLength());
	return true;
}

bool GetDBCharString(CADORecordset& _rs, const CString& _fieldName, char* _dest, size_t _destSize)
{
	char temp[240] { 0,};
	if (!GetDBCharValue(_rs, _fieldName, temp))
	{
		return false;
	}

	sprintf_s(_dest, _destSize, "%s", temp);
	return true;
}

// Message
void IPMessage(const CString szMessage, LPCTSTR szTitle, DWORD dwTimeout, LPCTSTR szButton) {
	IPMessage(szMessage, dwTimeout, szButton, szTitle);
}

void IPMessage(const CString szMessage, DWORD dwTimeout, LPCTSTR szButton, LPCTSTR szTitle) {
	T_MESSAGE_DATA tMD;
	
	tMD.iTimeout = dwTimeout;
	tMD.strMessage = szMessage;
	tMD.strButton = szButton;
	tMD.strTitle = szTitle;
	
	CString				str;
	CString				strPath,
						strFile;
	CFile				file;
	CFileException		e;

	COleDateTime		odtDayClose = COleDateTime::GetCurrentTime();
	COleDateTime		odtCurrent = COleDateTime::GetCurrentTime();

	strPath.Format(_T("D:\\Log\\InputMessage\\%04d%02d%02d\\%02d\\"),odtDayClose.GetYear(), odtDayClose.GetMonth(), odtDayClose.GetDay(), odtDayClose.GetHour());
	CreateSubDirectories(strPath);
	strFile.Format(_T("%sInputMessage.log"), strPath);

	if (!file.Open(strFile, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate | CFile::shareDenyNone, &e)) {
		e.ReportError();
		return;
	}
	file.SeekToEnd();
	CArchiveU			arU(&file, CArchive::store);
	if (file.GetPosition() == 0) {
		arU.WriteEncodingSignature(CArchiveU::E_ANSI);
		arU.WriteStringA("DATE\tTIME\tWINDOW\tNAME\tCURRENT\tCHANGE");
		arU.WriteStringA("\r\n");
	}
	str.Format(_T("!DATE!\t!TIME!\t!TEXT!\t!MSG!\t")); 
	str.Replace(_T("!DATE!"),		odtCurrent.Format(_T("%Y-%m-%d")));
	str.Replace(_T("!TIME!"), odtCurrent.Format(_T("%H:%M:%S")));
	str.Replace(_T("!TEXT!"), szTitle);
	str.Replace(_T("!MSG!"), szMessage);
	arU.WriteStringU(str);
	arU.WriteStringU(_T("\r\n"));
	arU.Close();
	file.Close();
	mf.SendIPMessage(g_ipClient, tMD);
}

CString RespMessage(LPCTSTR szMessage, LPCTSTR szButton, LPCTSTR szTitle) {
	T_MESSAGE_DATA tMD;
	tMD.strMessage = szMessage;
	tMD.strButton = szButton;
	tMD.strTitle = szTitle;
	return mf.ShowResMessage(g_ipClient, tMD);
}

void RespMessageCmd(PIPCOMMAND cmd, LPCTSTR szMessage, LPCTSTR szButton, LPCTSTR szTitle) {
	T_MESSAGE_DATA tMD;
	tMD.strMessage = szMessage;
	tMD.strButton = szButton;
	tMD.strTitle = szTitle;
	mf.ShowResMessageCmd(cmd, g_ipClient, tMD);
}

void RespUnitMessageCmd(PIPCOMMAND cmd, LPCSTR szUnit, LPCTSTR szMessage, LPCTSTR szButton, LPCTSTR szTitle) {
	//T_MESSAGE_DATA tMD;
	//tMD.strMessage = szMessage;
	//tMD.strButton = szButton;
	//tMD.strTitle = szTitle;
	//mf.ShowResUnitMessageCmd(cmd, g_ipClient, szUnit, tMD);
}

CString InPutMessage(LPCTSTR szMessage, BOOL bPassWord, LPCTSTR szTitle) {
	T_MESSAGE_DATA tMD;
	tMD.strMessage = szMessage;
	tMD.strTitle = szTitle;
	tMD.bPassWord = bPassWord;
	return mf.SendInPutMessage(g_ipClient, tMD);
}

void AnimationMessage(LPCTSTR szMessage, int iInterval, CIPCommand* pCMD, LPCTSTR szTitle) {
	T_MESSAGE_DATA tMD;
	tMD.strTitle = szTitle;
	tMD.strMessage = szMessage;
	tMD.iAnimationInterval = iInterval;
	mf.SendAnimationMessage(g_ipClient, tMD, pCMD);
}



//<< 21.02.10 추가
void WriteMotorAbnoramlLog(CString strLog) {
	CFile file;
	CFileException e;
	CTime cTime = CTime::GetCurrentTime();
	CString path = Format(_T("D:/MDIK/AbnormalMotorLog/%04d/%02d/"), cTime.GetYear(), cTime.GetMonth());
	CreateSubDirectories(path);		// <- 폴더가 없으면 생성하도록 수정

	path = path + Format(_T("%02d.csv"), cTime.GetDay());
	//log AxisNo, 1ch, 1ch0E, 1ch10, 1ch12, 2ch, 2ch0E, 2ch10, 2ch12
	// 파일 열기
	file.Open(path, CFile::shareDenyWrite | CFile::modeNoTruncate | CFile::modeWrite | CFile::modeCreate, &e);
	if (e.m_cause != CFileException::none)
	{
		// to do message
		TRACE(_T("%s File Exception Error : %d\n"), path, e.m_cause);
		return;
	}

	// log 파일 쓰기
	CArchiveU arU(&file, CArchiveU::store);
	if (file.SeekToEnd() == 0)
	{ // 파일 처음 생성시 항목을 적어주고 시작한다
		arU.WriteStringU(_T("시간,모터번호,현재 위치,기존 위치,타겟 위치,동작 상태,ILxx0E,ILxx10,ILxx12,ILxx16\r\n"));
	}

	arU.WriteStringU(GetTimeStringHMS(true) + _T(","));
	arU.WriteStringU(strLog);
	arU.WriteStringU(_T("\r\n"));

	arU.Close();
	file.Close();
}
//>>

void WriteParameterLog(CString strParamterName,  CString strName, std::vector<CString> vec, CString strUser)
{
	CFile file;
	CFileException e;

	CTime cTime = CTime::GetCurrentTime();
	CString time = Format(_T("%04d/%02d/"), cTime.GetYear(), cTime.GetMonth());
	CString path = DEFAULT_DIRECTORY + _T("ParameterLog/") + time;
	CreateSubDirectories(path);		// <- 폴더가 없으면 생성하도록 수정
	
	CString fileName = Format(_T("%02d.csv"), cTime.GetDay());
	path += fileName;
	// 파일 열기
	file.Open(path, CFile::shareDenyWrite | CFile::modeNoTruncate | CFile::modeWrite | CFile::modeCreate, &e);
	if (e.m_cause != CFileException::none)
	{
		// to do message
		TRACE(_T("%s File Exception Error : %d\n"), path, e.m_cause);
		return;
	}

	// log 파일 쓰기
	CArchiveU arU(&file, CArchiveU::store);
	if (file.SeekToEnd() == 0)
	{ // 파일 처음 생성시 항목을 적어주고 시작한다
		arU.WriteStringU(_T("시간;메뉴;분류;파라미터;타입;기존 값;변경 값;사용자\r\n"));
	}

	for (int i = 0; i < vec.size(); i++) {
		arU.WriteStringU(GetTimeStringHMS(true) + _T(";"));
		arU.WriteStringU(strParamterName + _T(";"));
		arU.WriteStringU(strName + _T(";"));
		arU.WriteStringU(vec[i] + _T(";"));
		arU.WriteStringU(strUser);			// User 명 
		arU.WriteStringU(_T("\r\n"));
	}
	arU.Close();
	file.Close();
}

void WriteButtonLog(CString strMain, CString strSub, CString strTab, CString strButton, CString strUser)
{
	CFile file;
	CFileException e;

	CTime cTime = CTime::GetCurrentTime();
	CString time = Format(_T("%04d/%02d/"), cTime.GetYear(), cTime.GetMonth());
	CString path = DEFAULT_DIRECTORY + _T("ButtonLog/") + time;
	CreateSubDirectories(path);		// <- 폴더가 없으면 생성하도록 수정

	CString fileName = Format(_T("%02d.csv"), cTime.GetDay());
	path += fileName;
	// 파일 열기
	file.Open(path, CFile::shareDenyWrite | CFile::modeNoTruncate | CFile::modeWrite | CFile::modeCreate, &e);
	if (e.m_cause != CFileException::none)
	{
		// to do message
		TRACE(_T("%s File Exception Error : %d\n"), path, e.m_cause);
		return;
	}

	// 2021.10.15 \n 제거 추가
	{
		strMain.Remove('\n');
		strSub.Remove('\n');
		strTab.Remove('\n');
		strButton.Remove('\n');
		strUser.Remove('\n');
	}

	// log 파일 쓰기
	CArchiveU arU(&file, CArchiveU::store);
	if (file.SeekToEnd() == 0)
	{ // 파일 처음 생성시 항목을 적어주고 시작한다
		arU.WriteStringU(_T("시간;메인메뉴;서브메뉴;세부메뉴;버튼;사용자\r\n"));
	}
	arU.WriteStringU(GetTimeStringHMS(true) + _T(";"));
	arU.WriteStringU(strMain	+ _T(";"));
	arU.WriteStringU(strSub		+ _T(";"));
	arU.WriteStringU(strTab		+ _T(";"));
	arU.WriteStringU(strButton	+ _T(";"));
	arU.WriteStringU(strUser);			// User 명 
	arU.WriteStringU(_T("\r\n"));
	arU.Close();
	file.Close();
}

// 2021.06.14 스크랩 관련 데이터 저장/불러오기
const CString GlassDataPath = L"D:/MDIK/GlassData/";
const CString CellDataPath = L"D:/MDIK/CellData/";

bool SaveGlassData(T_CELL_INFO& _glass) {
	CString currentTime = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
	strncpy_s(_glass.szTimeOfRemoval, CStringA(currentTime).GetBuffer(), sizeof(_glass.szTimeOfRemoval));
	currentTime.Replace(L":", L"_");

	CreateSubDirectories(GlassDataPath);

	CFile file;
	CFileException e;
	file.Open(GlassDataPath + currentTime + L".glass", CFile::modeCreate | CFile::modeWrite, &e);
	if (e.m_cause != CFileException::none) {
		TRACE(_T("Save Glass Fail....(%s)(%d)\n"), GlassDataPath + currentTime + L".glass", e.m_cause);
		return false;
	}

	CArchive ar(&file, CArchive::store);
	std::vector<BYTE> buf(sizeof(T_CELL_INFO));
	memcpy(buf.data(), &_glass, sizeof(T_CELL_INFO));
	ar << buf;
	ar.Close();
	file.Close();

	return true;
}

bool SaveCellData(T_CELL_INFO& _cell) {
	CString currentTime = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
	strncpy_s(_cell.szTimeOfRemoval, CStringA(currentTime).GetBuffer(), sizeof(_cell.szTimeOfRemoval));
	currentTime.Replace(L":", L"_");

	CreateSubDirectories(CellDataPath);

	CFile file;
	CFileException e;
	file.Open(CellDataPath + currentTime + L".cell", CFile::modeCreate | CFile::modeWrite, &e);
	if (e.m_cause != CFileException::none) {
		TRACE(_T("Save Cell Fail....(%s)(%d)\n"), CellDataPath + currentTime + L".cell", e.m_cause);
		return false;
	}

	CArchive ar(&file, CArchive::store);
	std::vector<BYTE> buf(sizeof(T_CELL_INFO));
	memcpy(buf.data(), &_cell, sizeof(T_CELL_INFO));
	ar << buf;
	ar.Close();
	file.Close();

	return true;
}

std::pair<bool, T_CELL_INFO> LoadGlassData(const CString _id) {
	T_CELL_INFO glass;

	CFile file;
	CFileException e;
	file.Open(GlassDataPath + _id + L".glass", CFile::modeRead, &e);
	if (e.m_cause != CFileException::none) {
		TRACE(_T("Load Glass Fail....(%s)(%d)\n"), GlassDataPath + _id + L".glass", e.m_cause);
		return { false, glass };
	}

	CArchive ar(&file, CArchive::load);
	std::vector<BYTE> buf(sizeof(T_CELL_INFO));
	ar >> buf;
	memcpy(&glass, buf.data(), sizeof(T_CELL_INFO));
	ar.Close();
	file.Close();
	return { true, glass };
}

std::pair<bool, T_CELL_INFO> LoadCellData(const CString _id) {
	T_CELL_INFO cell;

	CFile file;
	CFileException e;
	file.Open(CellDataPath + _id + L".cell", CFile::modeRead, &e);
	if (e.m_cause != CFileException::none) {
		TRACE(_T("Load Cell Fail....(%s)(%d)\n"), CellDataPath + _id + L".cell", e.m_cause);
		return { false, cell };
	}

	CArchive ar(&file, CArchive::load);
	std::vector<BYTE> buf(sizeof(T_CELL_INFO));
	ar >> buf;
	memcpy(&cell, buf.data(), sizeof(T_CELL_INFO));
	ar.Close();
	file.Close();
	return { true, cell };
}

std::list<T_CELL_INFO> GetGlassDataList() {
	CString tpath = GlassDataPath + L"*.*";

	//검색 클래스
	CFileFind finder;

	//CFileFind는 파일, 디렉터리가 존재하면 TRUE 를 반환함
	BOOL bWorking = finder.FindFile(tpath); //

	std::list<T_CELL_INFO> list;
	while (bWorking) {
		//다음 파일 / 폴더 가 존재하면다면 TRUE 반환
		bWorking = finder.FindNextFile();
		//파일 일때
		if (finder.IsArchived()) {
			//파일의 이름
			CString _fileName = finder.GetFileName();

			// 현재폴더 상위폴더 썸네일파일은 제외
			if (_fileName == _T(".") ||
				_fileName == _T("..") ||
				_fileName == _T("Thumbs.db")) continue;

			auto [result, data] = LoadGlassData(finder.GetFileTitle());
			if (result) {
				list.push_back(data);
			}
		}
	}

	return list;
}

std::list<T_CELL_INFO> GetCellDataList() {
	CString tpath = CellDataPath + L"*.*";

	//검색 클래스
	CFileFind finder;

	//CFileFind는 파일, 디렉터리가 존재하면 TRUE 를 반환함
	BOOL bWorking = finder.FindFile(tpath); //

	std::list<T_CELL_INFO> list;
	while (bWorking) {
		//다음 파일 / 폴더 가 존재하면다면 TRUE 반환
		bWorking = finder.FindNextFile();
		//파일 일때
		if (finder.IsArchived()) {
			//파일의 이름
			CString _fileName = finder.GetFileName();

			// 현재폴더 상위폴더 썸네일파일은 제외
			if (_fileName == _T(".") ||
				_fileName == _T("..") ||
				_fileName == _T("Thumbs.db")) continue;

			auto [result, data] = LoadCellData(finder.GetFileTitle());
			if (result) {
				list.push_back(data);
			}
		}
	}

	return list;
}

void RemoveGlassData(const CString _id) {
	CString id = _id;
	id.Replace(L":", L"_");
	CString tpath = GlassDataPath + id + L".glass";
	DeleteFile(tpath);
}

void RemoveCellData(const CString _id) {
	CString id = _id;
	id.Replace(L":", L"_");
	CString tpath = CellDataPath + id + L".cell";
	DeleteFile(tpath);
}

void SET_STEP_CELL(T_STEP_CELL& itemTarget, int iSTEP, CStringA szID) {
	//TODO : KGH FDC 테스트용 임시조치
	if (1||!sm.Sequence->bCIMSkip) {
		if (iSTEP == 0) {
			memset(&itemTarget, 0x00, sizeof(itemTarget));
			return;
		}

		// CIM Skip Mode가 아닌 경우에만 데이터 셋팅.
		T_STEP_CELL item;
		memset(&item, 0x00, sizeof(item));
		if (iSTEP != 0) {
			item.iSTEP_ID = iSTEP;

			if (szID.IsEmpty())
				strncpy_s(item.szCELL_ID, countof(item.szCELL_ID), itemTarget.szCELL_ID, countof(item.szCELL_ID) - 1);
			else
				strncpy_s(item.szCELL_ID, countof(item.szCELL_ID), szID, countof(item.szCELL_ID) - 1);
		}

		memcpy(&itemTarget, &item, sizeof(itemTarget));
	}
	else {
		// CIM Skip 모드인 경우 메모리 초기화
		memset(&itemTarget, 0x00, sizeof(itemTarget));
	}
};

//<< 2024.07.24 LJY 추가
void WriteRecipeLog(std::vector<CString> vec) {
	CFile file;
	CFileException e;

	CTime cTime = CTime::GetCurrentTime();
	CString time = Format(_T("%04d/%02d/"), cTime.GetYear(), cTime.GetMonth());
	CString path = DEFAULT_DIRECTORY + _T("RecipeLog/") + time;
	CreateSubDirectories(path);		// <- 폴더가 없으면 생성하도록 수정

	CString fileName = Format(_T("%02d.csv"), cTime.GetDay());
	path += fileName;
	// 파일 열기
	file.Open(path, CFile::shareDenyWrite | CFile::modeNoTruncate | CFile::modeWrite | CFile::modeCreate, &e);
	if (e.m_cause != CFileException::none)
	{
		// to do message
		TRACE(_T("%s File Exception Error : %d\n"), path, e.m_cause);
		return;
	}

	// log 파일 쓰기
	CArchiveU arU(&file, CArchiveU::store);
	if (file.SeekToEnd() == 0)
	{ // 파일 처음 생성시 항목을 적어주고 시작한다
		arU.WriteStringU(_T("시간;Recipe 명;Recipe 항목;기존 값;변경 값;사용자\r\n"));
	}

	for (int i = 0; i < vec.size(); i++) {
		arU.WriteStringU(GetTimeStringHMS(true) + _T(";"));
		arU.WriteStringU(vec[i] + _T(";"));
		arU.WriteStringU(_T("\r\n"));
	}
	arU.Close();
	file.Close();

}
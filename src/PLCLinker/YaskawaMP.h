#pragma once

#include "YASKAWA/ymcPCAPI.h"
#define SHIFT_BIT 16

typedef enum eTYPE_MEMORY {
	eTM_BIT,
	eTM_LONG,
	eTM_WORD,
} eTYPE_MEMORY;

class YaskawaMP
{
	
	typedef struct{
		int iAxis;
		HAXIS hMainAxis;
		int iAxisSub[eCH_AXIS_END];
		HAXIS hAxisSub[eCH_AXIS_END];
	}T_HAXIS;
public:
	YaskawaMP();
	~YaskawaMP();

public:
	BOOL Init();
	BOOL Release();
	BOOL Close();

	BOOL InitAxis();
	BOOL SetMotionData(int iMaxMotor = eMOTOR_END);

	
	BOOL StartHome(int iAxis = -1);
	BOOL ServoOnOff(int iAxis = -1, BOOL bOn = FALSE);
	BOOL JogMove(int iAxis = -1, BOOL bDir = -1, double dSpeed = -1.);
	BOOL StepMove(int iAxis = -1, double dStep = 0, double dSpeed = -1.);
	BOOL PTPMove(int iAxis = -1, double dTarget = 0, double dSpeed = -1.);
	BOOL JogStop(int iAxis = -1);
	//BOOL LinearMove(const std::vector<T_MOTOR_LINER>& ATData, double dSpeed = -1.);
	BOOL MultiMove(const std::vector<T_POS>& ATData, BOOL bPTP = TRUE);
	BOOL MultiJog(const std::vector<T_POS>& ATData, BOOL bDir);
	BOOL MultiStop(const std::vector<T_POS>& ATData, BOOL bJog);
	BOOL MultiJogStop(const std::vector<T_POS>& ATData);
	BOOL MultiMotionStop(const std::vector<T_POS>& ATData);
	BOOL TorqueMove(int iAxis = -1, double iTorque = 0., double dSpeed = -1.);
	BOOL MotionStop(int iAxis = -1);
	BOOL SetRegisterData(CStringA strAddress, double dData);
	BOOL SetRegisterBit(int iAddress,int iShiftBit, DWORD dwData);
	BOOL ClearAlarm();
	BOOL ClearServoAlarm(int iAxis = -1);
	
	//BOOL GetRegisterData(CStringA strAddress, double &bData);
	//BOOL GetRegisterData(CStringA strAddress, int &iData);
	BOOL GetRegisterDataBit(BOOL bIn, int iAddress, int iDWordCount, DWORD *bData);
	BOOL GetRegisterData(eTYPE_MEMORY eTM, int iAddress, int iDWordCount, DWORD* bData);
	
	double GetRegDataDouble(CString strAddresss);
	int GetRegDataInt(CString strAddresss);
	void SetRegData(CString strAddresss, double dData);
	void SetRegData(CString strAddresss, int iData);
	void SetRegConveyorData(CString strAddresss, int iData);
	BOOL SetOrizinLogical(int iAxis, double dPos = 0);
	BOOL SetOrizinResetLogical(int iAxis);
	BOOL SetOrizinAxis(int iAxis, double dPos = 0);
	BOOL SetOrizinResetAxis(int iAxis);
	
	BOOL SetTorque(int iAxis, double dRate = 1.);
	BOOL SetLimit(int iAxis, double dLimitN, double dLimitP);
	BOOL SetAlarmClearServo(int iAxis = -1);
	
	double GetOffset0x48Logcal(int iAxis);
protected:
	double GetOffset0x48(int iAxis);
	BOOL	Set0x03(int iAxis);
	BOOL	Set0x0E(int iAxis, BOOL bOn);
	BOOL	Set0x0F(int iAxis, BOOL bOn);
	BOOL InitMotionCmd(int iAxis);
	//. 12.12 HGH 사용 안함.
	//BOOL SetGantry(int iAxis, BOOL bSet);
	int GetRegData(CString strAddresss);
	BOOL SetController(int iThread);
	HDEVICE DeclareDevice(int iAxis);
	HDEVICE DeclareDeviceGanTry(int Axis, eCH_AXIS eCh);
	HDEVICE DeclareDevice(const std::vector<int>& Axis);
	BOOL ClearDevice(HDEVICE hDevice);
	//BOOL ClearDevice(int iAxis = -1); // -1 AllAxis, Ohter input Number Axis
	BOOL MoveStop(int iAxis = -1, BOOL bJog = -1);
	BOOL Move(int iAxis = -1,BOOL bPTP = TRUE, double dPosData = 0, double dSpeed = -1.);
	MOTION_DATA GetMotionData(int iAxis = -1, BOOL bHome = FALSE);
	
	BOOL ClearAxis(int iAxis = -1); // -1 AllAxis, Ohter input Number Axis
	T_MOTOR_STATUS_CRT GetAxisStatus(int iAxisArray, int iAxisLogical);
	T_MOTOR_STATUS_CRT GetAxisStatusGantry(int iAxis);
	
	BOOL OpenController(BOOL bSub = FALSE);
	double GetOriainOffset(int iAxis);
	void SetRegisterWORD(const WORD* _cur, const WORD* _new, const int _count, const int _address);
	BOOL GetRegisterDataWord(eTYPE_MEMORY eTM, int iAddress, int iDWordCount, WORD* bData);
	BOOL GetRegisterDataWordOut(eTYPE_MEMORY eTM, int iAddress, int iDWordCount, WORD* bData);
	void LoopRegist(BOOL bFirst);
	void LoopIO(BOOL bFirst);
	void RegisterMemory();
	void LoopAlarm();
	//<< Thread function
	void th_Axis();
	void th_etc();
	void th_IO();
	//>>

	void WriteErrorLogYMC(LPCTSTR szFuncName, DWORD rc);
protected:

	HCONTROLLER	m_hController;

	//. 초기화 상태 조건 변수.
	bool m_bInitClearAxis, m_bInitSetMotionData;
	std::atomic<bool> m_bIsThreadKill;
	std::unique_ptr<std::thread> m_thAxis, m_thEtc, m_thIO;
	
	//. 스레드간 공유 변수. 메인스레드의 Init()함수에서 OpenController가 성공인지 여부.
	//. thMain, thEtc, thIO 는 해당 변수가 true일 때 까지 대기한다.
	std::atomic<bool> m_bIsControllerMainOpened;

	COM_DEVICE	m_Device;

	//. 12.12. HGH. thread-safe한 vector 사용. 기본 벡터는 동시 push_back에 안전하지 않음.
	//std::vector<HCONTROLLER>	m_hControllers;
	Concurrency::concurrent_vector<HCONTROLLER>	m_hControllers;
	//std::vector<HAXIS>	m_hAxiss;
	std::vector<T_HAXIS>	m_tAxiss;
	//std::vector<HDEVICE>	m_hDevices;

};
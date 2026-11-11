#pragma once

#include "pch.h"
#include "PLCLinker.h"
#include "YaskawaMP.h"

#include "AlarmChecker.h"

#ifdef _WIN64
	#pragma comment(lib, "ymcPCAPI_x64.lib")
#else
	#pragma comment(lib, "ymcPCAPI.lib")
#endif

typedef struct{
	char MS[10];
	eTYPE_MEMORY eTM;
	DWORD wPos;
}T_MOTION_STATUS;

T_MOTION_STATUS MotionStatus[] = {
	{"IW", eTM_WORD, 0x00},
	{"IW", eTM_WORD, 0x01},
	{"IL", eTM_LONG, 0x02},
	{"IL", eTM_LONG, 0x04},
	{"IW", eTM_WORD, 0x08},
	{"IW", eTM_WORD, 0x09},
	{"IW", eTM_WORD, 0x0A},
	{"IW", eTM_WORD, 0x0B},
	{"IW", eTM_WORD, 0x0C},
	{"IL", eTM_LONG, 0x0E},
	{"IL", eTM_LONG, 0x10},
	{"IL", eTM_LONG, 0x12},
	{"IL", eTM_LONG, 0x14},
	{"IL", eTM_LONG, 0x16},
	{"IL", eTM_LONG, 0x18},
	{"IL", eTM_LONG, 0x1A},
	{"IL", eTM_LONG, 0x1C},
	{"IL", eTM_LONG, 0x1E},
	{"IL", eTM_LONG, 0x20},
	{"IL", eTM_LONG, 0x24},
	{"IL", eTM_LONG, 0x26},
	{"IL", eTM_LONG, 0x28},
	{"IL", eTM_LONG, 0x2A},
	{"IW", eTM_WORD, 0x2C},
	{"IW", eTM_WORD, 0x2D},
	{"IW", eTM_WORD, 0x2E},
	{"IW", eTM_WORD, 0x2F},
	{"IL", eTM_LONG, 0x30},
	{"IL", eTM_LONG, 0x32},
	{"IL", eTM_LONG, 0x34},
	{"IW", eTM_WORD, 0x36},
	{"IW", eTM_WORD, 0x37},
	{"IL", eTM_LONG, 0x38},
	{"IL", eTM_LONG, 0x3A},
	{"IW", eTM_WORD, 0x3F},
	{"IL", eTM_LONG, 0x40},
	{"IL", eTM_LONG, 0x42},
	{"IW", eTM_WORD, 0x44},
	{"IW", eTM_WORD, 0x45},
	{"IL", eTM_LONG, 0x4A},
	{"IL", eTM_LONG, 0x4C},
	{"IL", eTM_LONG, 0x56},
	{"IW", eTM_WORD, 0x58},
	{"IW", eTM_WORD, 0x59},
	{"IW", eTM_WORD, 0x5A},
	{"IW", eTM_WORD, 0x5B},
	{"IL", eTM_LONG, 0x5E},
	{"IL", eTM_LONG, 0x60},
	{"IL", eTM_LONG, 0x62},
	{"IL", eTM_LONG, 0x64},
	{"IL", eTM_LONG, 0x66},
	{"IL", eTM_LONG, 0x68},
	{"IL", eTM_LONG, 0x6A},
	{"IL", eTM_LONG, 0x6C},
	{"IL", eTM_LONG, 0x6E},
}; 

typedef struct {
	eTYPE_MEMORY eTM;
	DWORD dwStart;
	int	iIndex;
	int iWordCount;
}T_REGISTER_ADDRESS;

T_REGISTER_ADDRESS RegisterAddress[] {
	//{eTM_WORD,	0,		0,	499},
	{eTM_LONG,	1000,	0,	100},			// sm.Register->Reg_Word_1
	{eTM_LONG,	2000,	1,	250},			// sm.Register->Reg_Word_2
	{eTM_LONG,	5000,	2,	220},			// sm.Register->Reg_Word_3	(읽기 전용 메모리 / LD 바코드 정보 읽어 들여야함)

	{eTM_BIT,	100,	0,	13},			// sm.Register->Out
	{eTM_BIT,	200,	1,	13},			// sm.Register->In
	{eTM_BIT,	120,	2,	7},			// sm.Register->Axis_On
	{eTM_BIT,	130,	3,	7},			// sm.Register->Axis_Off
	{eTM_BIT,	140,	4,	7},			// sm.Register->Axis_ORG_PC
	{eTM_BIT,	240,	5,	7},			// sm.Register->Axis_ORG_MP
	{eTM_BIT,	150,	6,	7},			// sm.Register->Axis_ABS_RESET_PC
	//{eTM_BIT,	0,		0,	6},
	//{eTM_BIT,	1000,	1,	6},
};

T_REGISTER_ADDRESS RegisterAddressIO_In[] {
	{eTM_WORD,	0x000,	0,	2},
	{eTM_WORD,	0x100,	0,	63}, // 최대 16개 밖에 못받는지 확인 필요.
	// 16, 16, 16, 15
};

T_REGISTER_ADDRESS RegisterAddressIO_Out[] {
	{eTM_WORD,	0x000,	1,	2},
	{eTM_WORD,	0x100,	1,	63},
};

T_REGISTER_ADDRESS Register_Alarm[]{
	{eTM_WORD,	170,	0,	2},
};


#define AXIS_PITCH		128
#define AXIS_BASE		0x8000
#define AXIS_BASE_OUT	0x9000

YaskawaMP::YaskawaMP() 
	: m_bIsThreadKill(false)
	, m_bInitClearAxis(false)
	, m_bInitSetMotionData(false)
	, m_bIsControllerMainOpened(false)
{
}

YaskawaMP::~YaskawaMP() {

	Release();
}

BOOL YaskawaMP::SetController(int iThread){
	
	BOOL bReturn = TRUE;
	
	//if (m_hControllers.empty()) {
	//	IPMessage(_T("Controller가 비어있습니다."));
	//	return FALSE;
	//}
	DWORD rc = ymcSetController(m_hControllers[iThread]);
	if(rc != MP_SUCCESS)
    {
		TRACE(_T("ymcSetController Fail - %d \n"), iThread);
		bReturn = FALSE;
    }

	return bReturn;
}

void YaskawaMP::WriteErrorLogYMC(LPCTSTR szFuncName, DWORD rc) {
	CStringW strResult(Format(_T("%x"), rc));


	TRACE(_T("%s Failed. - %s \n"), szFuncName, strResult);

	if (1) {
		//. ipmessage가 로그 남겨줄 것임.
		IPMessage(Format(_T("%s Failed. - %x"), szFuncName, rc));
		//g_ipClient.
	}
}

BOOL YaskawaMP::OpenController(BOOL bSub){
	CS cs;
	HCONTROLLER	hController;
	BOOL bReturn = TRUE;
	COM_DEVICE         ComDevice;
	ComDevice.ComDeviceType = COMDEVICETYPE_PCI_MODE;
    ComDevice.PortNumber    = 1;
    ComDevice.CpuNumber     = 1;    //CPU number (value set in the window)
    ComDevice.NetworkNumber = NULL;
    ComDevice.StationNumber = NULL;
    ComDevice.UnitNumber    = NULL;
    ComDevice.IPAddress     = NULL;
    ComDevice.Timeout       = 10000;
	
	DWORD rc =0;
	
	rc = ymcOpenController(&ComDevice, &hController);

	if (rc != MP_SUCCESS) {

		WriteErrorLogYMC(_T("ymcOpenController"), rc);

		sm.SetSimultation(TRUE);
		return FALSE;
	}

	m_hController = hController;

	//DWORD dwAlarm = ymcGetLastError();
	//if (dwAlarm != 0) {
	//	rc = ymcClearAlarm(dwAlarm);
	//	if (rc != MP_SUCCESS) {

	//		WriteErrorLogYMC(_T("ymcSetController"), rc);

	//		bReturn = FALSE;
	//	}
	//}


	
	rc = ymcSetController(hController);
	if (rc != MP_SUCCESS) {
		
		WriteErrorLogYMC(_T("ymcSetController"), rc);

		bReturn = FALSE;
	}
	rc = ymcSetAPITimeoutValue(3000);
    if(rc != MP_SUCCESS)
    {
		WriteErrorLogYMC(_T("ymcSetAPITimeoutValue Fail"), rc);
		bReturn = FALSE;
    }
	
	m_hControllers.push_back(hController);
	return bReturn;
}

BOOL YaskawaMP::InitAxis(){

	BOOL bReturn = TRUE;

	bReturn &= ClearAxis(); // iAxis < 0: all Axis , iAxis >= 0: Select Axis
	bReturn &= SetMotionData();

	if (bReturn) {
		BOOL bAxis = TRUE;
		for(int i =0 ; i < eMOTOR_END ;i++){
			bAxis = TRUE;
			if(sm.MotorParameter[i].tYaskawa.iLogicalAxisNo == 0){
				continue;
			}
			bAxis &= InitMotionCmd(i);
			bAxis &= ClearServoAlarm(i);	
		
			sm.MotorStatus[i].tEtc.bInit = bAxis;
		
		}
	}
	return bReturn;
}

BOOL YaskawaMP::Init() {
	
	BOOL bReturn = TRUE;
	DWORD rc =0;
	
	bReturn &= OpenController();

	m_bIsControllerMainOpened = bReturn;

	if (sm.IsSimultation()) {
		m_thIO.reset(new std::thread([this]() { th_IO(); }));
		return TRUE;
	}

	bReturn &= InitAxis();

	//if (FALSE == bReturn) {
	//	IPMessage(_T("YaskawaMP::Init() - InitAxis() 실패."));
	//	return FALSE;
	//}

	if( bReturn) {
		
		m_thAxis.reset(new std::thread([this]() {  th_Axis(); }));
		m_thEtc.reset(new std::thread([this]() { th_etc(); }));
		m_thIO.reset(new std::thread([this]() { th_IO(); }));
	}
	return bReturn;
}

BOOL YaskawaMP::Release() {

	m_bIsThreadKill = true;

	if (m_thAxis && m_thAxis->joinable()) {
		m_thAxis->join();
	}

	if (m_thIO && m_thIO->joinable()) {
		m_thIO->join();
	}

	if (m_thEtc && m_thEtc->joinable()) {
		m_thEtc->join();
	}

	return Close();
}

BOOL YaskawaMP::ClearAxis(int iAxis) {
	DWORD rc;
	BOOL bReturn = TRUE;
	if (iAxis >= 0 ) {
		if (m_tAxiss.size()) {
			rc = ymcClearAxis(m_tAxiss[iAxis].hMainAxis);
			if(rc != MP_SUCCESS)
			{
				TRACE(_T("ymcClearAxis Fail\n"));
			    bReturn = FALSE;
			}
			if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
				for (int i = 0; i < eCH_AXIS_END; i++) {
					if (m_tAxiss[iAxis].iAxisSub[i] > 0) {
						rc = ymcClearAxis(m_tAxiss[iAxis].hAxisSub[i]);
						if (rc != MP_SUCCESS)
						{
							WriteErrorLogYMC(_T("ymcClearAxis Fail"), rc);
							bReturn = FALSE;
						}
					}
				}
			}
		}
	}
	else {
		rc = ymcClearAllAxes();
		if (rc != MP_SUCCESS)
		{
			//ymcClearAlarm(0);
			// ymcResetController 함수는 MP 전원일 꺼졌다 켜져서 사용하면 안될듯.
			//DWORD dwAlarm = ymcGetLastError();
			//if (dwAlarm != 0)
			//	ymcResetController(m_hController);
			TRACE(_T("ymcSetAPITimeoutValue Fail\n"));
			bReturn = FALSE;
			
		}
	}
	m_bInitClearAxis = TRUE;
	return bReturn;
}

BOOL YaskawaMP::SetMotionData(int iMaxMotor) {
	BOOL bReturn = TRUE;
	DWORD rc;
	char AxisName[10] = {0,};
	//m_hAxiss.clear();
	m_tAxiss.clear();
	for (int i = 0; i < iMaxMotor; i++) {
		auto& MPYs = sm.MotorParameter[i].tYaskawa;
		auto& MP = sm.MotorParameter[i];
	
		sprintf_s(AxisName, 9,"Axis_%02d", MPYs.iLogicalAxisNo);
		
		HAXIS hAxis;
		T_HAXIS tAxis;
		memset(&tAxis, 0x00, sizeof(tAxis));
		tAxis.iAxis = MPYs.iLogicalAxisNo;
		MPYs.iGantryCh = eCH_AXIS_ALL;
		if (MPYs.bSetGantry) {
			
			//<< KES 주석 2024.4.5 - 불필요한 겐트리 설정 주석 (추후 문제 생길시 다시 풀어야 할수도 있음)
			//sprintf_s(AxisName, 9,"Axis_%02d", MPYs.iLogicalAxisNo);
			//rc = ymcDeclareAxis(MPYs.iRackNo, MPYs.iSlotNo, MPYs.iSubSlotNo, MPYs.iMotorNo, MPYs.iLogicalAxisNo, MPYs.iAxisType, (LPBYTE)AxisName, &hAxis);
			//if(rc != MP_SUCCESS){}
			//
			//tAxis.hMainAxis = hAxis;
			//>>
			
			HAXIS hAxisGantry1,hAxisGantry2;
			sprintf_s(AxisName, 9,"Axis_%02d", MPYs.iCh1_Axis);
			rc = ymcDeclareAxis(MPYs.iGantryRackNo, MPYs.iGantrySlotNo, MPYs.iGantrySubSlotNo, MPYs.iCh1_Motor, MPYs.iCh1_Axis, MPYs.iAxisType, (LPBYTE)AxisName, &hAxisGantry1);
			if(rc != MP_SUCCESS){}
			tAxis.iAxisSub[eCH_AXIS_1] = MPYs.iCh1_Axis;
			tAxis.hAxisSub[eCH_AXIS_1] = hAxisGantry1;
			sprintf_s(AxisName, 9,"Axis_%02d", MPYs.iCh2_Axis);
			rc = ymcDeclareAxis(MPYs.iGantryRackNo, MPYs.iGantrySlotNo, MPYs.iGantrySubSlotNo, MPYs.iCh2_Motor, MPYs.iCh2_Axis, MPYs.iAxisType, (LPBYTE)AxisName, &hAxisGantry2);
			if(rc != MP_SUCCESS){}
			tAxis.iAxisSub[eCH_AXIS_2] = MPYs.iCh2_Axis;
			tAxis.hAxisSub[eCH_AXIS_2] = hAxisGantry2;
			

		}
		else {
			rc = ymcDeclareAxis(MPYs.iRackNo, MPYs.iSlotNo, MPYs.iSubSlotNo, MPYs.iMotorNo, MPYs.iLogicalAxisNo, MPYs.iAxisType, (LPBYTE)AxisName, &hAxis);
			tAxis.hMainAxis = hAxis;
		}

		if (rc != MP_SUCCESS)
        {
            TRACE(_T("ymcDeclareAxis Fail - Axis_%02d \n"), MPYs.iAxisType);
            //MessageBox( m_ErrMsg, AfxGetApp()->m_pszAppName, MB_ICONERROR );
            bReturn = FALSE;
			break;
        }

		// 2024.04.09 LBH - Limit 설정.
		SetLimit(i, sm.MotorParameter[i].dLimitLow, sm.MotorParameter[i].dLimitHigh);
		m_tAxiss.push_back(tAxis);
	}
	if (!bReturn) {
		m_tAxiss.clear();
	}
	m_bInitSetMotionData = TRUE;
	return bReturn;
}

HDEVICE YaskawaMP::DeclareDeviceGanTry(int iAxis, eCH_AXIS eCh){
	HDEVICE hDevice;
	
	int iCount = 1; 
	if (eCh == eCH_AXIS_ALL) {
		iCount = 2;
	}
	auto pAxis = std::make_unique<HAXIS[]>(iCount);
	if (eCh == eCH_AXIS_ALL) {
		for (int i = 0; i < iCount; i++) {
			pAxis[i] = m_tAxiss[iAxis].hAxisSub[i];
		}
	}
	else {
		pAxis[0] = m_tAxiss[iAxis].hAxisSub[eCh];
	}
	
	DWORD rc = ymcDeclareDevice(iCount, pAxis.get(), &hDevice);
	if( rc != MP_SUCCESS){
		hDevice = NULL;
	}
	return hDevice;
}
HDEVICE YaskawaMP::DeclareDevice(int iAxis) {
	std::vector<int> Axis;
	Axis.push_back(iAxis);
	return DeclareDevice(Axis);
}
HDEVICE YaskawaMP::DeclareDevice(const std::vector<int>& Axis) {
	HDEVICE hDevice;
	int iCount = 0;
	for (int i = 0; i < Axis.size(); i++) {
		iCount ++;
		if (sm.MotorParameter[Axis[i]].tYaskawa.bSetGantry
			&& sm.MotorParameter[Axis[i]].tYaskawa.iGantryCh == eCH_AXIS_ALL) {
			iCount++;
		}
	}
	auto pAxis = std::make_unique<HAXIS[]>(iCount);
	int iIndex = 0;
	for (int i = 0; i < Axis.size(); i++) {
		if (sm.MotorParameter[Axis[i]].tYaskawa.bSetGantry){
			if (sm.MotorParameter[Axis[i]].tYaskawa.iGantryCh == eCH_AXIS_ALL) {
				pAxis[iIndex++] = m_tAxiss[Axis[i]].hAxisSub[eCH_AXIS_1];
				pAxis[iIndex++] = m_tAxiss[Axis[i]].hAxisSub[eCH_AXIS_2];
			}
			else {
				pAxis[iIndex++] = m_tAxiss[Axis[i]].hAxisSub[sm.MotorParameter[Axis[i]].tYaskawa.iGantryCh];
			}
		}
		else {
			pAxis[iIndex++] = m_tAxiss[Axis[i]].hMainAxis;
		}
	}
	DWORD rc = ymcDeclareDevice(iCount, pAxis.get(), &hDevice);

	return hDevice;
}
BOOL YaskawaMP::ClearDevice(HDEVICE hDevice) {
	BOOL bReturn = TRUE;
	bReturn &= SetController(0);
	DWORD rc;
	rc = ymcClearDevice(hDevice);
	if (rc != MP_SUCCESS)
	{
		TRACE(_T("ymcClearDevice Fail\n"));
		bReturn = FALSE;

	}
	return bReturn;
}
BOOL YaskawaMP::ClearAlarm() {
	BOOL bReturn = TRUE;
	bReturn &= SetController(0);
	DWORD rc = 0;
	HALARM        Alarm[MAX_CURRENT_ALARM];
    ALARM_INFO    AlarmInfo[MAX_CURRENT_ALARM];
    DWORD         AlarmNumber;                                 
    
	memset(Alarm, 0, sizeof( HALARM ) *MAX_CURRENT_ALARM);
	rc = ymcGetAlarm( 0, Alarm, AlarmInfo, &AlarmNumber);

	for( int i = 0 ; i< AlarmNumber; i++){
		rc = ymcClearAlarm(Alarm[i]);
		if (rc != MP_SUCCESS)
		{
			TRACE(_T("ymcClearAlarm Fail\n"));
			
			bReturn = FALSE;
		}
	}
	return bReturn;

}
BOOL	YaskawaMP::Set0x0E(int iAxis, BOOL bOn) {
	BOOL bReturn = SetController(0);
	CString strCmd;
	
	int iLogAxis = iAxis;
	strCmd.Format(_T("OB%04x"), AXIS_BASE + 0x000E + (iLogAxis * AXIS_PITCH));
	int iData = bOn;
	SetRegData(strCmd, iData);
	return TRUE;
}
BOOL	YaskawaMP::Set0x0F(int iAxis, BOOL bOn){
	BOOL bReturn = SetController(0);
	CString strCmd;
	
	int iLogAxis = iAxis;
	strCmd.Format(_T("OB%04x"), AXIS_BASE + 0x000F + (iLogAxis * AXIS_PITCH));
	int iData = bOn;
	SetRegData(strCmd, iData);
	return TRUE;
}
BOOL YaskawaMP::ClearServoAlarm(int iAxis){
	BOOL bReturn  = TRUE;
	bReturn &= SetController(0);
	DWORD rc = 0;
	rc = ymcSetAPITimeoutValue(sm.MotorParameter[iAxis].iAPITimeout);
	if (rc != MP_SUCCESS && rc != ALM_MK_MDWARNING)
	{
		TRACE(_T("ymcSetAPITimeoutValue Fail\n"));
		bReturn = FALSE;
	}

	// 2024.07.07 LBH - 프로그램 죽어서 인터락.
	if (iAxis >= m_tAxiss.size())
		return FALSE;

	if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
		for (int i = 0; i < eCH_AXIS_END; i++) {
			if (m_tAxiss[iAxis].iAxisSub[i] > 0) {
				Set0x0F(m_tAxiss[iAxis].iAxisSub[i] - 1, TRUE);
				Set0x0E(m_tAxiss[iAxis].iAxisSub[i] - 1, TRUE);
				rc = ymcClearServoAlarm(m_tAxiss[iAxis].hAxisSub[i]);
				if (rc != MP_SUCCESS && rc != ALM_MK_MDWARNING)
				{
					TRACE(_T("ymcClearServoAlarm Fail\n"));
					bReturn = FALSE;
				}
				Set0x0F(m_tAxiss[iAxis].iAxisSub[i] - 1, FALSE);
				Set0x0E(m_tAxiss[iAxis].iAxisSub[i] - 1, FALSE);
			}
		}
	}
	else {
		Set0x0F(sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1, TRUE);
		Set0x0E(sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1, TRUE);

		rc = ymcClearServoAlarm(m_tAxiss[iAxis].hMainAxis);
		if (rc != MP_SUCCESS && rc != ALM_MK_MDWARNING)
		{
			TRACE(_T("ymcClearServoAlarm Fail\n"));
			bReturn = FALSE;
		}
		Set0x0F(sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1, FALSE);
		Set0x0E(sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1, FALSE);


	}
	
	return bReturn; 
}
BOOL YaskawaMP::StartHome(int iAxis) {
	BOOL bReturn = TRUE;
	bReturn &= SetController(0);
	DWORD rc;
	if (iAxis < 0) {
		return FALSE;
	}
	auto& MP = sm.MotorParameter[iAxis].tHome;
	MOTION_DATA MotionData = GetMotionData(iAxis, TRUE);
	POSITION_DATA Offset;
	Offset.DataType			= DATATYPE_IMMEDIATE;
	Offset.PositionData		= MP.dOffset*1000;
	WORD wHomeMethod		= MP.iMethod;
	WORD wHomeDir			= MP.iDirection;
	WORD wWaitForCompletion	= COMMAND_STARTED;
	HDEVICE hDevice = DeclareDevice(iAxis);
	rc = ymcMoveHomePosition(hDevice, &MotionData, &Offset, &wHomeMethod, &wHomeDir, 0, NULL, &wWaitForCompletion, 0);
	if (rc != MP_SUCCESS)
	{
		TRACE(_T("ymcMoveHomePosition Fail\n"));
		bReturn = FALSE;
	}
	ClearDevice(hDevice);
	return bReturn;
}
BOOL YaskawaMP::MultiJog(const std::vector<T_POS>& ATData,BOOL bDir){
	BOOL bReturn = TRUE;
	bReturn &= SetController(0);
	DWORD rc;
	int iSpeed =0;
	int iCount = 0;  //ATData.size();
	int iArraySize = ATData.size();
	for (int i = 0; i < iArraySize; i++) {
		iCount++;
		if (sm.MotorParameter[ATData[i].iAxis].tYaskawa.bSetGantry
			&& sm.MotorParameter[ATData[i].iAxis].tYaskawa.iGantryCh == eCH_AXIS_ALL) {
			iCount ++;
		}
	}
	auto PosData  =  std::make_unique<POSITION_DATA[]>(iCount);
	auto MotionData = std::make_unique<MOTION_DATA[]>(iCount);
	auto Dir = std::make_unique<WORD[]>(iCount);
	auto Timeout = std::make_unique<WORD[]>(iCount);
	auto wWaitforCompletion = std::make_unique<WORD[]>(iCount);
	std::vector<int> axis;
	int iIndex = 0, iIndexBefor = 0; 
	for (int i = 0; i < iArraySize; i++) {
		int iAxis = ATData[i].iAxis ;
		if (sm.MotorStatus[iAxis].tCrt.bNLimit || sm.MotorStatus[iAxis].tCrt.bPLimit) {
			ClearServoAlarm(iAxis);
		}
		InitMotionCmd(iAxis);
		//SetGantry(iAxis, FALSE);
		axis.push_back(iAxis);
		auto& MP = sm.MotorParameter[iAxis];
		MotionData[iIndex] = GetMotionData(iAxis);
		
		PosData[iIndex].DataType = DATATYPE_IMMEDIATE;
		PosData[iIndex].PositionData = ATData[i].dTarget * MP.dPositionRate;
		iSpeed = ATData[i].dSpeed * MP.dSpeedRate;
		if (ATData[i].dSpeed < 0.) {
			iSpeed = MP.dDefaultSpped * MP.dSpeedRate;
		}
		MotionData[iIndex].Velocity = iSpeed;
		
		wWaitforCompletion[iIndex]= COMMAND_STARTED;
		Dir[iIndex] = bDir ? 0 :1;
		Timeout[iIndex] = 0;
		iIndex++;
		if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry
			&& sm.MotorParameter[iAxis].tYaskawa.iGantryCh == eCH_AXIS_ALL) {
			iIndexBefor = iIndex - 1;
			MotionData[iIndex]		= MotionData[iIndexBefor];
			PosData[iIndex]			= PosData[iIndexBefor];
			wWaitforCompletion[iIndex] = wWaitforCompletion[iIndexBefor];
			Dir[iIndex] = Dir[iIndexBefor];
			Timeout[iIndex] = Timeout[iIndexBefor];
			iIndex++;
		}
	}
	HDEVICE hDevice = DeclareDevice(axis);
	
	rc = ymcMoveJOG( hDevice, MotionData.get(), Dir.get(), Timeout.get(), 0, NULL, 0);
	if (rc != MP_SUCCESS)
	{
		TRACE(_T("ymcMoveJOG Fail\n"));
		bReturn = FALSE;

	}
	
	ClearDevice(hDevice);
	return TRUE;
}
BOOL YaskawaMP::MultiStop(const std::vector<T_POS>& ATData, BOOL bJog){
	BOOL bReturn = TRUE;
	
	bReturn &= SetController(0);
	
	int iCount = 0;  //ATData.size();
	int iArraySize = ATData.size();
	for (int i = 0; i < iArraySize; i++) {
		iCount++;
		if (sm.MotorParameter[ATData[i].iAxis].tYaskawa.bSetGantry
			&& sm.MotorParameter[ATData[i].iAxis].tYaskawa.iGantryCh == eCH_AXIS_ALL) {
			iCount++;
		}
	}
	DWORD rc;
	std::vector<int> axis;
	auto MotionData = std::make_unique<MOTION_DATA[]>(iCount);
	auto wWaitforCompletion = std::make_unique<WORD[]>(iCount);
	int iIndex = 0, iIndexBefor = 0;
	for (int i = 0; i < iArraySize; i++) {
		int iAxis = ATData[i].iAxis;
		axis.push_back(iAxis);
		MotionData[iIndex] = GetMotionData(iAxis);
		wWaitforCompletion[iIndex] = COMMAND_STARTED;
		iIndex++;
		if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry
			&& sm.MotorParameter[iAxis].tYaskawa.iGantryCh == eCH_AXIS_ALL) {
			iIndexBefor = iIndex - 1;
			MotionData[iIndex] = MotionData[iIndexBefor];
			wWaitforCompletion[iIndex] = wWaitforCompletion[iIndexBefor];
		}
	}

	HDEVICE hDevice = DeclareDevice(axis);

	if (bJog) {
		rc = ymcStopJOG(hDevice, 0, NULL, wWaitforCompletion.get(), 0);

	}
	else {
		rc = ymcStopMotion(hDevice, MotionData.get(), NULL, wWaitforCompletion.get(), 0);
	}
	
	if (rc != MP_SUCCESS)
	{
		if (bJog) {
			TRACE(_T("ymcStopJOG Fail\n"));
		}
		else {
			TRACE(_T("ymcStopMotion Fail\n"));
		}
		bReturn = FALSE;
	}
	
	ClearDevice(hDevice);
	return TRUE;
}
BOOL YaskawaMP::MultiJogStop(const std::vector<T_POS>& ATData){
	MultiStop(ATData, TRUE);
	return TRUE;
}
BOOL YaskawaMP::MultiMotionStop(const std::vector<T_POS>& ATData){
	MultiStop(ATData, FALSE);
	return TRUE;
}
	
BOOL YaskawaMP::JogMove(int iAxis, BOOL bDir, double dSpeed) {
	std::vector<T_POS> ATData;
	T_POS tPos; 
	tPos.iAxis = iAxis;
	tPos.dTarget = 0;
	tPos.dSpeed = dSpeed;
	ATData.push_back(tPos);
	return MultiJog(ATData, bDir);
}
BOOL YaskawaMP::JogStop(int iAxis) {
	if (iAxis == eMOTOR_END) {
		return TRUE;
	}
	return MoveStop(iAxis, TRUE);
}
BOOL YaskawaMP::MotionStop(int iAxis) {
	if (iAxis == eMOTOR_END) {
		return TRUE;
	}
	return MoveStop(iAxis, FALSE);
}
BOOL YaskawaMP::MoveStop(int iAxis, BOOL bJog) {
	std::vector<T_POS> ATData;
	T_POS tPos;
	tPos.iAxis = iAxis;
	tPos.dTarget = 0;
	tPos.dSpeed = 0;
	ATData.push_back(tPos);

	return MultiStop(ATData, bJog);
}
BOOL YaskawaMP::StepMove(int iAxis, double dStep, double dSpeed) {
	return Move(iAxis, FALSE, dStep, dSpeed);
}
BOOL YaskawaMP::PTPMove(int iAxis, double dTarget, double dSpeed) {
	return Move(iAxis, TRUE, dTarget, dSpeed);
}
BOOL YaskawaMP::Move(int iAxis, BOOL bPTP, double dPosData, double dSpeed) {
	std::vector<T_POS> ATData;
	T_POS tPos;
	tPos.iAxis = iAxis;
	tPos.dTarget = dPosData;
	tPos.dSpeed = dSpeed;
	ATData.push_back(tPos);
	return MultiMove(ATData, bPTP);
}
//
//BOOL YaskawaMP::LinearMove(const std::vector<T_MOTOR_LINER>& ATData, double dSpeed) {
//	
//	BOOL bReturn = TRUE;
//	bReturn &= SetController(0);
//	DWORD rc;
//	int iCount = ATData.size();
//	if (!iCount) {
//		return FALSE;
//	}
//	std::vector<int> Axis;
//	
//	for (int i = 0; i < iCount; i++) {
//		Axis.push_back( ATData[i].iAxis);
//	}
//	HDEVICE hDevice = DeclareDevice(Axis);
//	
//	auto &MP = sm.MotorParameter[ATData[0].iAxis];
//	//MOTION_DATA MotionData = GetMotionData(ATData[0].iAxis);
//	//WORD wWaitforCompletion =  COMMAND_STARTED;
//	// change 
//	// unique_ptr
//	
//	auto PosData = std::make_unique<POSITION_DATA[]>(iCount);
//	auto MD = std::make_unique<MOTION_DATA[]>(iCount);
//	//auto wWaitforCompletion = std::make_unique<WORD[]>(iCount);
//	int iSpeed = dSpeed * MP.dPositionRate;
//	if (dSpeed < 0.) {
//		iSpeed = MP.dDefaultSpped * MP.dPositionRate;
//	}
//	
//	for (int i = 0; i < iCount; i++) {
//		
//		PosData[i].DataType		= DATATYPE_IMMEDIATE;
//		PosData[i].PositionData	= ATData[i].dTarget * MP.dPositionRate;
//		MD[i] = GetMotionData(ATData[0].iAxis);
//		MD[i].Velocity = iSpeed;
//		
//		
//	}
//	
//	WORD wWaitforCompletion =  POSITIONING_COMPLETED;
//	
//	rc = ymcMoveLinear(hDevice, MD.get(), PosData.get(), 0, NULL, wWaitforCompletion, 0); 
//			
//	ClearDevice(hDevice);
//	
//	return bReturn;
//}

BOOL YaskawaMP::MultiMove(const std::vector<T_POS>& ATData, BOOL bPTP) {
	BOOL bReturn = TRUE;
	bReturn &= SetController(0);
	DWORD rc;
	int iSpeed =0;
	int iCount = 0;  //ATData.size();
	int iArraySize = (int)ATData.size();
	for (int i = 0; i < iArraySize; i++) {
		iCount++;
		if (sm.MotorParameter[ATData[i].iAxis].tYaskawa.bSetGantry 
			&& sm.MotorParameter[ATData[i].iAxis].tYaskawa.iGantryCh == eCH_AXIS_ALL) {
			iCount++;
		}
	}
	auto dDistance = std::make_unique<double[]>(iCount);
	auto d2ChDistance = std::make_unique<double[]>(iCount);
	auto PosData  =  std::make_unique<POSITION_DATA[]>(iCount);
	auto MotionData = std::make_unique<MOTION_DATA[]>(iCount);
	auto wWaitforCompletion = std::make_unique<WORD[]>(iCount);

	

	std::vector<int> axis;
	int iIndex = 0, iIndexBefor = 0;
	for (int i = 0; i < iArraySize; i++) {
		
		int iAxis = ATData[i].iAxis ;
		InitMotionCmd(iAxis);
		if (sm.MotorStatus[iAxis].tCrt.bNLimit || sm.MotorStatus[iAxis].tCrt.bPLimit) {
			ClearServoAlarm(iAxis);
		}
		MoveStop(iAxis);
		//SetGantry(iAxis, FALSE);
		double dPosData = ATData[i].dTarget;
		
		axis.push_back(iAxis);
		auto &MP = sm.MotorParameter[iAxis];
		
		MotionData[iIndex] = GetMotionData(iAxis);
		PosData[iIndex].DataType = DATATYPE_IMMEDIATE;
		PosData[iIndex].PositionData = ATData[i].dTarget * MP.dPositionRate;
		iSpeed = ATData[i].dSpeed * MP.dSpeedRate;
		if (ATData[i].dSpeed < 0.) {
			iSpeed = MP.dDefaultSpped * MP.dSpeedRate;
		}
		MotionData[iIndex].Velocity = iSpeed;
		wWaitforCompletion[iIndex]= COMMAND_STARTED;
		MotionData[iIndex].MoveType = MTYPE_RELATIVE;
		if (bPTP) {

			d2ChDistance[iIndex] = dPosData - sm.MotorStatus[iAxis].tCrt.dPos;
			dDistance[iIndex] = fabs(d2ChDistance[iIndex]);
			MotionData[iIndex].MoveType = MTYPE_ABSOLUTE;
		}
		iIndex++;
		if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry
			&& sm.MotorParameter[iAxis].tYaskawa.iGantryCh == eCH_AXIS_ALL) {
			iIndexBefor = iIndex - 1;
			PosData[iIndex] = PosData[iIndexBefor];
			MotionData[iIndex] = MotionData[iIndexBefor];
			wWaitforCompletion[iIndex] = wWaitforCompletion[iIndexBefor];
			if (bPTP) {
				if (sm.MotorStatus[iIndex].tEtc.dwHomeStatus == eHOME_STATUS_COMPLETED) {
					MotionData[iIndex].MoveType = MTYPE_ABSOLUTE;
					PosData[iIndex].PositionData = ATData[i].dTarget * MP.dPositionRate;
				}
				else {
					MotionData[iIndex].MoveType = MTYPE_RELATIVE;
					PosData[iIndex].PositionData = d2ChDistance[iIndexBefor] * MP.dPositionRate;
				}
			}
			iIndex++;
		}
	}
	HDEVICE hDevice = DeclareDevice(axis);
	rc = ymcMoveDriverPositioning( hDevice, MotionData.get(), PosData.get(), NULL, NULL, wWaitforCompletion.get(), 0);
	if (rc != MP_SUCCESS)
	{
		TRACE(_T("ymcMoveDriverPositioning Fail \n"));
		bReturn = FALSE;

	}
	ClearDevice(hDevice);
	return bReturn;
}

BOOL YaskawaMP::TorqueMove(int iAxis, double dTorque, double dSpeed) {
	
	BOOL bReturn = TRUE;
	bReturn &= SetController(0);
	DWORD rc;
	HDEVICE hDevice = DeclareDevice(iAxis);
	POSITION_DATA pdS;
	POSITION_DATA pdT;
	auto& MP = sm.MotorParameter[iAxis];
	int iDir = dSpeed > 0 ? 1:-1;
	pdS.DataType = DATATYPE_IMMEDIATE;
	pdS.PositionData = dSpeed * MP.dSpeedRate;
	pdT.DataType = DATATYPE_IMMEDIATE;
	pdT.PositionData = dTorque*100 * iDir;
	rc = ymcMoveTorque(hDevice, &pdS, &pdT,0 , NULL, 0);

	//. 12.13 추가.
	ClearDevice(hDevice);

	return bReturn;
}

//BOOL YaskawaMP::SetGantry(int iAxis, BOOL bSet){
//	BOOL bReturn = TRUE;
//	auto &YS = sm.MotorParameter[iAxis].tYaskawa;
//	//SetOrizinLogical(iAxis, 0);
//	CString strCmd(sm.MotorParameter[iAxis].tYaskawa.szGantryAddress);
//	if(!strCmd.IsEmpty()){
//		SetRegData( strCmd, bSet);
//	}
//	if(bSet){
//		
//		double dPosOffset =  sm.MotorStatus[iAxis].tCrt.dPos - sm.MotorStatus[iAxis].tCrt.tGantryData.dCh1Pos;
//		//SetOrizinLogical(iAxis, dPosOffset);
//		
//	}
//	return bReturn;
//}
BOOL YaskawaMP::Set0x03(int iAxis) {

	BOOL bReturn = SetController(0);
	CString strCmd;
	int iLogAxis = iAxis;
	int iDATA = AXIS_BASE + 0x0003 + (iLogAxis * AXIS_PITCH);
	strCmd.Format(_T("OW%04x"), AXIS_BASE + 0x0003 + (iLogAxis * AXIS_PITCH));
	int iData = 0x0200;
	SetRegData(strCmd, iData);

	return bReturn;
}
double YaskawaMP::GetOffset0x48(int iAxis){
	double dPos =0; 
	CString strCmd;
	int iLogAxis = sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo-1;
	strCmd.Format(_T("OL%04x"), AXIS_BASE+0x0048+ (iLogAxis* AXIS_PITCH));
	dPos = GetRegDataDouble(strCmd);
	return dPos;
}
double YaskawaMP::GetOffset0x48Logcal(int iAxis) {
	double dPos = 0;
	CString strCmd;
	int iLogAxis = iAxis - 1;
	strCmd.Format(_T("OL%04x"), AXIS_BASE + 0x0048 + (iLogAxis * AXIS_PITCH));
	dPos = GetRegDataDouble(strCmd);
	return dPos;
}
BOOL YaskawaMP::ServoOnOff(int iAxis, BOOL bOn) {
	CS cs;
	BOOL bReturn = TRUE;
	DWORD rc = MP_SUCCESS;
	if (iAxis < 0) {
		return FALSE;
	}
	auto& MP = sm.MotorParameter[iAxis];
	auto& MPYs = MP.tYaskawa;
	HDEVICE hDevice = DeclareDevice(iAxis);
	ys.ClearServoAlarm(iAxis);
	if (!sm.MotorStatus[iAxis].tCrt.bServoReady) {
		IPMessage(_T("Not ServoReady"));
		return FALSE;
	}
	if(bOn){
		if (sm.MotorStatus[iAxis].tCrt.bServoOn) {
			return TRUE;
		}
	}
	if(MP.tYaskawa.bSetGantry == TRUE){
		HDEVICE hDevice1 = DeclareDeviceGanTry(iAxis, eCH_AXIS_1);
		HDEVICE hDevice2 = DeclareDeviceGanTry(iAxis, eCH_AXIS_2);
		if (bOn) {
			
			rc = ymcServoControl(hDevice1, !bOn, MP.iAPITimeout);
			if (rc != MP_SUCCESS)
			{
				TRACE(_T("ymcServoControl Fail"));
				bReturn = FALSE;

			}

			rc = ymcServoControl(hDevice2, !bOn, MP.iAPITimeout);
			if (rc != MP_SUCCESS)
			{
				TRACE(_T("ymcServoControl Fail"));
				bReturn = FALSE;

			}
			ys.ClearServoAlarm(iAxis);
			if (!sm.MotorStatus[iAxis].tCrt.bServoReady) {
				IPMessage(_T("Not ServoReady"));
				return FALSE;
			}
		}
		rc = ymcServoControl(hDevice1, bOn, MP.iAPITimeout);
		if (rc != MP_SUCCESS)
		{
			TRACE(_T("ymcServoControl Fail"));
			bReturn = FALSE;

		}
		rc = ymcServoControl(hDevice2, bOn, MP.iAPITimeout);
		if (rc != MP_SUCCESS)
		{
			TRACE(_T("ymcServoControl Fail"));
			bReturn = FALSE;

		}
		GetAxisStatusGantry(iAxis);
		GetAxisStatusGantry(iAxis);
		if (bOn) {
			Set0x03(MPYs.iCh1_Axis - 1);
			Set0x03(MPYs.iCh2_Axis - 1);
		}
		ClearDevice(hDevice1);
		ClearDevice(hDevice2);
	}
	else {
		if (bOn) {
			
			rc = ymcServoControl(hDevice, !bOn, MP.iAPITimeout);
			if (rc != MP_SUCCESS)
			{
				TRACE(_T("ymcServoControl Fail"));
				bReturn = FALSE;

			}

			ys.ClearServoAlarm(iAxis);
			if (!sm.MotorStatus[iAxis].tCrt.bServoReady) {
				IPMessage(_T("Not ServoReady"));
				return FALSE;
			}
		}

		//rc = ymcServoControl(hDevice, bOn, MP.iAPITimeout);
		//double dPosOffsetG = GetOffset0x48Logcal(MPYs.iLogicalAxisNo);
		//SetOrizinLogical(iAxis, dPosOffsetG + MP.tHome.dOffsetCrt);
		

	}
	if (bOn) {
		Set0x03(MPYs.iLogicalAxisNo - 1);
	}
	MP.tHome.dOffsetCrt = 0;
	
	if (rc != MP_SUCCESS)
	{
		TRACE(_T("ymcServoControl Fail \n"));
		bReturn = FALSE;

	}
	
	ClearDevice(hDevice);
	return bReturn;
}
MOTION_DATA YaskawaMP::GetMotionData(int iAxis, BOOL bHome) {
	MOTION_DATA MotionData;
	auto& MPYa = sm.MotorParameter[iAxis].tYaskawa;
	auto& MPHome = sm.MotorParameter[iAxis].tHome;
	auto& MP = sm.MotorParameter[iAxis];
	

	MotionData.CoordinateSystem		= MP.iCoordinateSystem;
	MotionData.MoveType				= MP.iMoveType;  
	MotionData.VelocityType			= MP.iVelocityType; //VTYPE_UNIT_PAR
	MotionData.AccDecType			= MP.iAccDecType; //ATYPE_TIME
	MotionData.FilterType			= MP.iFilterType;
	MotionData.DataType				= MP.iDataType;
	MotionData.MaxVelocity			= MP.iMaxVelocity;
	MotionData.Acceleration			= MP.iAcc;
	MotionData.Deceleration			= MP.iDec;
	MotionData.FilterTime			= MP.dFilterTime * 1000;  //us to s
	
	MotionData.DataType			= 0;
	if (bHome) {
		MotionData.Velocity = MPHome.dVelocity* MP.dPositionRate;
		MotionData.ApproachVelocity = MPHome.dApproachVelocity * MP.dPositionRate;
		MotionData.CreepVelocity = MPHome.dCreepVelocity * MP.dPositionRate;
	}
	return MotionData;
}

BOOL YaskawaMP::Close() {

	DWORD rc;
	for(int i= 0 ; i < m_hControllers.size() ; i++){
		rc = ymcCloseController(m_hControllers[i]);
		if (rc != MP_SUCCESS) {
			WriteErrorLogYMC(Format(_T("Yaskawa Device %d Close Fail"), i), rc);
		}
	}
	return TRUE;
}

T_MOTOR_STATUS_CRT YaskawaMP::GetAxisStatusGantry(int iAxis){
	
	
	T_MOTOR_STATUS_CRT StatusCh1 = GetAxisStatus(iAxis, sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis);
	T_MOTOR_STATUS_CRT StatusCh2 = GetAxisStatus(iAxis, sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis);
	T_MOTOR_STATUS_CRT StatusReturn;

	if (sm.MotorParameter[iAxis].tYaskawa.iGantryCh == eCH_AXIS_2) {
		StatusReturn = StatusCh2;
		StatusReturn.tSubGantryData = StatusCh1.tMonitoring;
		StatusReturn.tGantryData.dCh1Pos = StatusCh2.dPos;
		StatusReturn.tGantryData.dCh2Pos = StatusCh1.dPos;
		StatusReturn.tGantryData.dCh1Torque = StatusCh2.dTorque;
		StatusReturn.tGantryData.dCh2Torque = StatusCh1.dTorque;
	}
	else {
		StatusReturn = StatusCh1;
		StatusReturn.tSubGantryData = StatusCh2.tMonitoring;
		StatusReturn.tGantryData.dCh1Pos = StatusCh1.dPos;
		StatusReturn.tGantryData.dCh2Pos = StatusCh2.dPos;
		StatusReturn.tGantryData.dCh1Torque = StatusCh1.dTorque;
		StatusReturn.tGantryData.dCh2Torque = StatusCh2.dTorque;
	}

	//<< 2024.05.29 kms
	sm.MotorTorque->torque[iAxis].bGantry = TRUE;
	sm.MotorTorque->torque[iAxis].dTorque[0] = StatusCh1.dTorque;
	sm.MotorTorque->torque[iAxis].dTorque[1] = StatusCh2.dTorque;
	//>>

	if (StatusCh1.bServoOn) {

		StatusReturn.tGantryData.dwChServoOnStatus |=  eCH_GANTRY_AXIS_STATE::eCH_GANTRY_AXIS_1_STATE;
	}
	if (StatusCh2.bServoOn) {

		StatusReturn.tGantryData.dwChServoOnStatus |= eCH_GANTRY_AXIS_STATE::eCH_GANTRY_AXIS_2_STATE;
	}

	if (StatusCh1.bAlarm) {

		StatusReturn.tGantryData.dwChAlarmOnStatus |= eCH_GANTRY_AXIS_STATE::eCH_GANTRY_AXIS_1_STATE;
	}
	if (StatusCh2.bAlarm) {

		StatusReturn.tGantryData.dwChAlarmOnStatus |= eCH_GANTRY_AXIS_STATE::eCH_GANTRY_AXIS_2_STATE;
	}

	if (StatusCh1.bPLimit) {

		StatusReturn.tGantryData.dwChPLimitStatus |= eCH_GANTRY_AXIS_STATE::eCH_GANTRY_AXIS_1_STATE;
	}
	if (StatusCh2.bPLimit) {

		StatusReturn.tGantryData.dwChPLimitStatus |= eCH_GANTRY_AXIS_STATE::eCH_GANTRY_AXIS_2_STATE;
	}

	if (StatusCh1.bNLimit) {

		StatusReturn.tGantryData.dwChNLimitStatus |= eCH_GANTRY_AXIS_STATE::eCH_GANTRY_AXIS_1_STATE;
	}
	if (StatusCh2.bNLimit) {

		StatusReturn.tGantryData.dwChNLimitStatus |= eCH_GANTRY_AXIS_STATE::eCH_GANTRY_AXIS_2_STATE;
	}

	if (StatusCh1.bHomeSensor) {

		StatusReturn.tGantryData.dwChHomeStatus |= eCH_GANTRY_AXIS_STATE::eCH_GANTRY_AXIS_1_STATE;
	}
	if (StatusCh2.bHomeSensor) {

		StatusReturn.tGantryData.dwChHomeStatus |= eCH_GANTRY_AXIS_STATE::eCH_GANTRY_AXIS_2_STATE;
	}

	if (StatusCh1.bInPosition) {

		StatusReturn.tGantryData.dwChInPosStatus |= eCH_GANTRY_AXIS_STATE::eCH_GANTRY_AXIS_1_STATE;
	}
	if (StatusCh2.bInPosition) {

		StatusReturn.tGantryData.dwChInPosStatus |= eCH_GANTRY_AXIS_STATE::eCH_GANTRY_AXIS_2_STATE;
	}


	//. Gantry인 경우에 tCrt.bServoOn 검사를 할때 .tGantryData.dwChServoOnStatus 에만 업데이트 되므로
	//. tCrt.bServoOn 에도 업데이트 해준다.
	if (StatusReturn.tGantryData.dwChServoOnStatus != eCH_GANTRY_AXIS_ALL_STATE) {
		StatusReturn.bServoOn = FALSE;
	}
	else {
		StatusReturn.bServoOn = TRUE;
	}

	//<< 12.22. Hot fix. gantry 축에 대해서는 flag를 다르게 봄.

	//. NONE state 필요
	if (StatusReturn.tGantryData.dwChPLimitStatus == 0) {
		StatusReturn.bPLimit = FALSE;
	}
	else {
		StatusReturn.bPLimit = TRUE;
	}

	if (StatusReturn.tGantryData.dwChNLimitStatus == 0) {
		StatusReturn.bNLimit = FALSE;
	}
	else {
		StatusReturn.bNLimit = TRUE;
	}

	if (StatusReturn.tGantryData.dwChAlarmOnStatus == 0) {
		StatusReturn.bAlarm = FALSE;
	}
	else {
		StatusReturn.bAlarm = TRUE;
	}
	//>>

	

	return StatusReturn;
}

T_MOTOR_STATUS_CRT YaskawaMP::GetAxisStatus(int iAxisArray,int iAxisLogical) {
	BOOL bReturn = SetController(0);
	T_MOTOR_STATUS_CRT tStatus;
	T_MONITORING_PARAM tMonitonP;
	HREGISTERDATA qwRegister =0;
	DWORD dwReadDataNumber =0;
	DWORD rc =0;;
	LONG lData = 0;
	WORD wData = 0;

	memset(&tStatus, 0x00, sizeof(T_MOTOR_STATUS_CRT));
	memset(&tMonitonP, 0x00, sizeof(T_MONITORING_PARAM));
	char cmd[10] = {0,};
	int iCount = countof(MotionStatus);
	
	int iLogAxis = iAxisLogical - 1;
	sprintf_s(cmd, "%s%04x", MotionStatus[0].MS, AXIS_BASE+MotionStatus[0].wPos+ (iLogAxis* AXIS_PITCH));
	rc = ymcGetRegisterDataHandle((LPBYTE)cmd, &qwRegister);
	int iWordSize = (sizeof( T_MONITORING_PARAM ) / 2);
	rc = ymcGetRegisterData(qwRegister, iWordSize/*499*/, &tMonitonP, &dwReadDataNumber);
	 
	if (!sm.IsSimultation()) {
		tStatus.dPos				= (int)tMonitonP.ILxx16 / sm.MotorParameter[iAxisArray].dPositionRate;
		tStatus.dSpeed				= (int)tMonitonP.ILxx40 / sm.MotorParameter[iAxisArray].dSpeedRate;
		tStatus.dTorque				= fabs((int)tMonitonP.ILxx42 / 100.);
		tStatus.iOverride			= (int)tMonitonP.ILxx18;
	}
	
	//<< 2024.05.29 kms
	sm.MotorTorque->torque[iAxisArray].bGantry = FALSE;
	sm.MotorTorque->torque[iAxisArray].dTorque[0] = tStatus.dTorque;
	//>>
	
	//tStatus.bAlarmH				= tMonitonP.ILxx02;
	//tStatus.bAlarmL				= tMonitonP.ILxx04;
	tStatus.tMonitoring			= tMonitonP;
	tStatus.bOperationReady		= (tMonitonP.IWxx00 & BIT(0)) ? TRUE : FALSE;
	tStatus.bServoOn			= (tMonitonP.IWxx00 & BIT(1)) ? TRUE : FALSE;
	tStatus.bSystemBusy			= (tMonitonP.IWxx00 & BIT(2)) ? TRUE : FALSE;
	tStatus.bServoReady			= (tMonitonP.IWxx00 & BIT(3)) ? TRUE : FALSE;

	

	//end ILxx04
	tStatus.bServoError			= (tMonitonP.ILxx04 & BIT(0)) ? TRUE : FALSE;//Bit0
	tStatus.bFollowingError		= (tMonitonP.ILxx04 & BIT(9)) ? TRUE : FALSE;//Bit9
	tStatus.bSpeedError			= (tMonitonP.ILxx04 & BIT(8)) ? TRUE : FALSE;//Bit8
	tStatus.bZeroPointNotSet	= (tMonitonP.ILxx04 & BIT(13)) ? TRUE : FALSE;//Bit13 Origin??;
	
	//IWxx0C
	tStatus.bInPosition			= (tMonitonP.IWxx0C & BIT(1)) ? TRUE : FALSE;//Bit1
	tStatus.bNEARPosition		= (tMonitonP.IWxx0C & BIT(3)) ? TRUE : FALSE;//Bit3
	tStatus.bZeroPointPosition	= (tMonitonP.IWxx0C & BIT(4)) ? TRUE : FALSE;//Bit4
	tStatus.bZeroPointReturnSet	= (tMonitonP.IWxx0C & BIT(5)) ? TRUE : FALSE;//Bit5
	
	//IWxx2C
	tStatus.bAlarm				= (tMonitonP.IWxx2C & BIT(0)) ? TRUE : FALSE;//Bit0
	tStatus.bWarning			= (tMonitonP.IWxx2C & BIT(1)) ? TRUE : FALSE;//Bit1
	tStatus.bCmdReady			= (tMonitonP.IWxx2C & BIT(2)) ? TRUE : FALSE;//Bit2
	tStatus.bMainPower			= (tMonitonP.IWxx2C & BIT(4)) ? TRUE : FALSE;//Bit4
	tStatus.bZeroSpeed			= (tMonitonP.IWxx2C & BIT(8)) ? TRUE : FALSE;//Bit8
	tStatus.bTorqueLimit		= (tMonitonP.IWxx2C & BIT(9)) ? TRUE : FALSE;//Bit9
	tStatus.bSoftPLimit			= (tMonitonP.IWxx2C & BIT(12)) ? TRUE : FALSE;//Bit12
	tStatus.bSoftNLimit			= (tMonitonP.IWxx2C & BIT(13)) ? TRUE : FALSE;//Bit13
	
	//IWxx2E
	tStatus.bPLimit				= (tMonitonP.ILxx28 & BIT(2)) ? TRUE : FALSE;//BIT2
	tStatus.bNLimit				= (tMonitonP.ILxx28 & BIT(3)) ? TRUE : FALSE;//BIT3
	//2024.03.18 edit by MDIK
	tStatus.bHomeSensor = (tMonitonP.ILxx28 & BIT(4)) ? TRUE : FALSE; //BIT4 (EXT1) 
	//tStatus.bHomeSensor			= (tMonitonP.ILxx28 & BIT(1)) ? TRUE : FALSE;//BIT1 // 제 1외부 래치 입력(EXT1)을 나타냅니다.

	
	//tStatus.bBreak				= (tMonitonP.IWxx2E_ & BIT(9)) ? TRUE : FALSE;//BIT9
	return tStatus;
}
BOOL YaskawaMP::SetRegisterData(CStringA strAddress, double dData) {
	BOOL bReturn = TRUE;
	DWORD rc = 0;

	return bReturn;
}

BOOL YaskawaMP::GetRegisterData(eTYPE_MEMORY eTM, int iAddress, int iDWordCount, DWORD *bData){
	BOOL bReturn = TRUE;
	DWORD rc =0;
	char RegistAddress[10] = {0,};
	switch(eTM){
		case eTM_BIT:
		case eTM_WORD:{
			sprintf_s(RegistAddress, "MW%d", iAddress);
			break;
		}
		case eTM_LONG: {
			sprintf_s(RegistAddress, "ML%d", iAddress);
			break;
		}
	}
	HREGISTERDATA hRegist;
	
	DWORD ReadDataNumber;
	
	rc = ymcGetRegisterDataHandle((LPBYTE)RegistAddress ,&hRegist);
	if (rc != MP_SUCCESS) {
		TRACE(_T("ymcGetRegisterDataHandle Fail\r\n"));
	}
	
	
	
	if( eTM == eTM_BIT){
		auto pData = std::make_unique<WORD[]>(iDWordCount);
		rc = ymcGetRegisterData(hRegist, iDWordCount, pData.get(), &ReadDataNumber);
		if (rc != MP_SUCCESS) {
			TRACE(_T("ymcGetRegisterData Fail \n"));
		}
		int iLoop = iDWordCount * SHIFT_BIT;
		for(int i = 0 ; i < iLoop ;i++){
			int iUper = i / SHIFT_BIT ;
			int iLower = i % SHIFT_BIT;
			DWORD dwDATA = pData[iUper];
			//bDataIO[i] = dwDATA && BIT( iLower );
			bData[i] = ((dwDATA >> iLower) & 0x01);
		}
	}else{
		rc = ymcGetRegisterData(hRegist, iDWordCount, bData, &ReadDataNumber);
		if (rc != MP_SUCCESS) {
			TRACE(_T("ymcGetRegisterData Fail\r\n"));
		}
	}
	return bReturn;
}
BOOL YaskawaMP::GetRegisterDataWordOut(eTYPE_MEMORY eTM, int iAddress, int iDWordCount, WORD* bData) {
		BOOL bReturn = TRUE;
	DWORD rc = 0;
	char RegistAddress[10] = { 0, };
	switch (eTM) {
	case eTM_BIT:
	case eTM_WORD: {
		sprintf_s(RegistAddress, "OW%04X", iAddress);
		break;
	}
	case eTM_LONG: {
		sprintf_s(RegistAddress, "OL%04X", iAddress);
		break;
	}
	}
	HREGISTERDATA hRegist;

	DWORD ReadDataNumber;

	rc = ymcGetRegisterDataHandle((LPBYTE)RegistAddress, &hRegist);
	if (rc != MP_SUCCESS) {
		TRACE(_T("ymcGetRegisterDataHandle Fail\r\n"));
	}
	if (eTM == eTM_BIT) {
		auto pData = std::make_unique<WORD[]>(iDWordCount);
		rc = ymcGetRegisterData(hRegist, iDWordCount, pData.get(), &ReadDataNumber);
		if (rc != MP_SUCCESS) {
			TRACE(_T("ymcGetRegisterData Fail \n"));
		}
		int iLoop = iDWordCount * SHIFT_BIT;
		for (int i = 0; i < iLoop; i++) {
			int iUper = i / SHIFT_BIT;
			int iLower = i % SHIFT_BIT;
			DWORD dwDATA = pData[iUper];
			//bDataIO[i] = dwDATA && BIT( iLower );
			bData[i] = ((dwDATA >> iLower) & 0x01);
		}
	}
	else {
		rc = ymcGetRegisterData(hRegist, iDWordCount, bData, &ReadDataNumber);
		if (rc != MP_SUCCESS) {
			TRACE(_T("ymcGetRegisterData Fail\r\n"));
		}
	}
	return bReturn;
}
BOOL YaskawaMP::GetRegisterDataWord(eTYPE_MEMORY eTM, int iAddress, int iDWordCount, WORD* bData) {
	BOOL bReturn = TRUE;
	DWORD rc = 0;
	char RegistAddress[10] = { 0, };
	switch (eTM) {
	case eTM_BIT:
	case eTM_WORD: {
		sprintf_s(RegistAddress, "MW%d", iAddress);
		break;
	}
	case eTM_LONG: {
		sprintf_s(RegistAddress, "ML%d", iAddress);
		break;
	}
	}
	HREGISTERDATA hRegist;

	DWORD ReadDataNumber;

	rc = ymcGetRegisterDataHandle((LPBYTE)RegistAddress, &hRegist);
	if (rc != MP_SUCCESS) {
		TRACE(_T("ymcGetRegisterDataHandle Fail\r\n"));
	}
	if (eTM == eTM_BIT) {
		auto pData = std::make_unique<WORD[]>(iDWordCount);
		rc = ymcGetRegisterData(hRegist, iDWordCount, pData.get(), &ReadDataNumber);
		if (rc != MP_SUCCESS) {
			TRACE(_T("ymcGetRegisterData Fail \n"));
		}
		int iLoop = iDWordCount * SHIFT_BIT;
		for (int i = 0; i < iLoop; i++) {
			int iUper = i / SHIFT_BIT;
			int iLower = i % SHIFT_BIT;
			DWORD dwDATA = pData[iUper];
			//bDataIO[i] = dwDATA && BIT( iLower );
			bData[i] = ((dwDATA >> iLower) & 0x01);
		}
	}
	else {
		rc = ymcGetRegisterData(hRegist, iDWordCount, bData, &ReadDataNumber);
		if (rc != MP_SUCCESS) {
			TRACE(_T("ymcGetRegisterData Fail\r\n"));
		}
	}
	return bReturn;
}

BOOL YaskawaMP::SetRegisterBit(int iAddress,int iShiftBit, DWORD dwData){
	BOOL bReturn = TRUE;
	DWORD rc = 0;
	
	int iHi = iShiftBit/ SHIFT_BIT;
	int iLow = iShiftBit% SHIFT_BIT;
	iHi += iAddress;
	char RegistAddress[10] = {0,};
	HREGISTERDATA hRegist =0;
	sprintf_s(RegistAddress, "OB%04X%X", iHi, iLow);
	
	rc = ymcGetRegisterDataHandle((LPBYTE)RegistAddress ,&hRegist);
	if (rc != MP_SUCCESS) {
		TRACE(_T("ymcGetRegisterDataHandle Fail \n"));
	}
	rc = ymcSetRegisterData(hRegist, 1, &dwData);
	return bReturn;
}

BOOL YaskawaMP::GetRegisterDataBit(BOOL bIn, int iAddress, int iDWordCount, DWORD *bDataIO){
	BOOL bReturn = TRUE;
	DWORD rc =0;
	char RegistAddress[10] = {0,};
	if(bIn){
		sprintf_s(RegistAddress, "IW%04X", iAddress);
	}else{
		sprintf_s(RegistAddress, "OW%04X", iAddress);
	}
	HREGISTERDATA hRegist;
	auto pData = std::make_unique<WORD[]>(iDWordCount);
	DWORD ReadDataNumber;
	
	rc = ymcGetRegisterDataHandle((LPBYTE)RegistAddress ,&hRegist);
	if (rc != MP_SUCCESS) {
		TRACE(_T("ymcGetRegisterDataHandle Fail \n"));
	}
	rc = ymcGetRegisterData(hRegist, iDWordCount, pData.get(), &ReadDataNumber);
	if (rc != MP_SUCCESS) {
		TRACE(_T("ymcGetRegisterData Fail \n"));
	}
	int iLoop = iDWordCount * SHIFT_BIT;
	
	for(int i = 0 ; i < iLoop ;i++){
		
		int iUper = i / SHIFT_BIT ;
		int iLower = i % SHIFT_BIT;
		DWORD dwDATA = pData[iUper];
		//bDataIO[i] = dwDATA && BIT( iLower );
		bDataIO[i] = ((dwDATA >> iLower) & 0x01);
	}
	return bReturn;
}

void YaskawaMP::th_Axis() {

	enum THREAD_EXIT_CODE {
		OPEN_CONTROLLER_FAILED = 0,
		INIT_CLEAR_AXIS_FAILED,
		SET_MOTION_DATA_FAILED,
	};
	THREAD_EXIT_CODE exitCode;
	//. waiting.
	while (!m_bIsControllerMainOpened) {
		TRACE(_T("THMain Wait\n"));
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	BOOL bCheck = TRUE;
	bCheck &= OpenController(TRUE);

	if (FALSE == bCheck) {
		IPMessage(_T("th_Axis() - OpenController() 실패."));
		exitCode = OPEN_CONTROLLER_FAILED;
		return;
	}

	//for (int i = 0; i < eMOTOR_END; i++) {
	//	sm.MotorStatus[i].tEtc.iOverride = 100;
	//}

	BOOL bIsInitCompleted = m_bInitClearAxis && m_bInitSetMotionData;
	
	// 프로그램 시작할 때 현재 위치와 종료 위치 비교해서 Home 신호 Off

	T_MOTOR_STATUS_CRT tCrtStatus[eMOTOR_END];
	memset(&tCrtStatus, 0x00, sizeof(T_MOTOR_STATUS_CRT));

	DWORD dwTick = GetTickCount();
	if (FALSE == bCheck) {
		return;
	}

	while (!m_bIsThreadKill) {
		if (FALSE == bIsInitCompleted) {
			continue;
		}

		for (int i = 0; i < eMOTOR_END; i++) {
			//if (sm.MotorStatus[i].tEtc.bInit == FALSE) {
			//	continue;
			//}
			if (sm.MotorParameter[i].tYaskawa.iLogicalAxisNo == 0) {
				continue;
			}

			T_MOTOR_STATUS_CRT tStatus;
			if (sm.MotorParameter[i].tYaskawa.bSetGantry) {
				tStatus = GetAxisStatusGantry(i);
			}
			else {
				tStatus = GetAxisStatus(i, sm.MotorParameter[i].tYaskawa.iLogicalAxisNo);
			}

			//<< 21.02.10 값이 갑자기 튀는 경우 로그
			if (fabs(tStatus.dPos - sm.MotorStatus[i].tCrt.dPos) > 100.)
			{
				auto& status = sm.MotorStatus[i];

				// 모터 번호, 현재 리딩된 위치 값, 기존 위치 값, 타겟 위치, 
				// 동작 상태, ILxx0E, ILxx10, ILxx12, ILxx16
				CString strLog = Format
				(
					_T("%d,%.3f,%.3f,%.3f,%s,%d,%d,%d,%d")
					, i
					, tStatus.dPos, status.tCrt.dPos, status.tEtc.dLastTargetPos
					, status.tEtc.bIPRun ? _T("O") : _T("X")
					, status.tCrt.tMonitoring.ILxx0E
					, status.tCrt.tMonitoring.ILxx10
					, status.tCrt.tMonitoring.ILxx12
					, status.tCrt.tMonitoring.ILxx16
				);

				WriteMotorAbnoramlLog(strLog);
			}
			//>>

			////<< 노이즈 필터
			//if (/*fabs(tStatus.dPos) < 3500.*/ 
			//	// 21.02.15 홈이 안되어있을 때 +- 3500이 넘어가서 데이터 세팅이 안되는 문제 수정
			//	/*||*/ sm.MotorStatus[i].tEtc.dwHomeStatus != eHOME_STATUS_COMPLETED) {
			//	tCrtStatus[i] = tStatus;

			//} else {
			//	tCrtStatus[i] = sm.MotorStatus[i].tCrt;
			//}
			////>>
			tCrtStatus[i] = tStatus;

			//<< 외부 홈센서 주소 세팅

			//. 첫 문자가 null이 아니면 주소값이 있다고 판단.
			if (sm.MotorParameter[i].tHome.szExternalHomeSensorAddress[0] != '\0') {

				tCrtStatus[i].bHomeSensor = GetRegDataInt(CString(sm.MotorParameter[i].tHome.szExternalHomeSensorAddress));
			}
			//>>

			//BOOL bOrg = FALSE;
			//if (sm.MotorParameter[i].tYaskawa.bSetGantry) {
			//	if (sm.Register->Axis_ORG_PC.dwAxis[sm.MotorParameter[i].tYaskawa.iCh1_Axis - 1] && sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[i].tYaskawa.iCh1_Axis-1]) {
			//		sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[i].tYaskawa.iCh1_Axis - 1] = FALSE;
			//	}
			//	if (sm.Register->Axis_ORG_PC.dwAxis[sm.MotorParameter[i].tYaskawa.iCh2_Axis - 1] && sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[i].tYaskawa.iCh2_Axis-1]) {
			//		sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[i].tYaskawa.iCh2_Axis - 1] = FALSE;
			//	}
			//	
			//	bOrg |= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[i].tYaskawa.iCh1_Axis-1] == FALSE;
			//	bOrg |= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[i].tYaskawa.iCh2_Axis-1] == FALSE;
			//}
			//else {
			//	if (sm.Register->Axis_ORG_PC.dwAxis[sm.MotorParameter[i].tYaskawa.iLogicalAxisNo - 1] && sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[i].tYaskawa.iLogicalAxisNo-1]) {
			//		sm.Register->Axis_ORG_PC_NEW.dwAxis[sm.MotorParameter[i].tYaskawa.iLogicalAxisNo - 1] = FALSE;
			//	}
			//	bOrg |= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[i].tYaskawa.iLogicalAxisNo-1] == FALSE;
			//}
			//if (bOrg)
			//{
			//	if (sm.MotorStatus[i].tEtc.dwHomeStatus != eHOME_STATUS_HOMMING) {
			//		sm.MotorStatus[i].tEtc.dwHomeStatus = eHOME_STATUS_NOT_COMPLETED;
			//	}
			//}

			BOOL bHomeComplete = TRUE;
			if (sm.MotorParameter[i].tYaskawa.bSetGantry)
			{
				bHomeComplete &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[i].tYaskawa.iCh1_Axis - 1];
				bHomeComplete &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[i].tYaskawa.iCh2_Axis - 1];
			}
			else 
			{
				bHomeComplete &= sm.Register->Axis_ORG_MP.dwAxis[sm.MotorParameter[i].tYaskawa.iLogicalAxisNo - 1];
			}

			// 2021.05.02 홈 동작 수행 중 알람 뜨는 문제 수정
			if (bHomeComplete && sm.MotorStatus[i].tEtc.dwHomeStatus != eHOME_STATUS_HOMMING)
			{
				sm.MotorStatus[i].tEtc.dwHomeStatus = eHOME_STATUS_COMPLETED;
			}
			else if (sm.MotorStatus[i].tEtc.dwHomeStatus != eHOME_STATUS_HOMMING) 
			{
				sm.MotorStatus[i].tEtc.dwHomeStatus = eHOME_STATUS_NOT_COMPLETED;
			}
		}
		for (int i = 0; i < eMOTOR_END; i++) {
			sm.MotorStatus[i].tCrt = tCrtStatus[i];
		}

		bool bHomeComplete = true;

		const auto& GlassTransfer	= sm.MotorPos_1->Glass_TR;
		
		// Glass Transfer 
		bHomeComplete &= CheckInPos(GlassTransfer.Wait_Y);
		bHomeComplete &= CheckInPos(GlassTransfer.Wait_Z);
	
		sm.Sequence->bHomeComplete = bHomeComplete;

		DWORD dwCrt = GetTickCount();
		g_dwTick = dwCrt - dwTick;
		dwTick = dwCrt;

		//memcpy(, tStatus, sizeof(T_MOTOR_STATUS_CRT)* eMOTOR_END);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void YaskawaMP::LoopRegist(BOOL bFirst) {

	int iRegistAddCount = countof(RegisterAddress);

	//<< 2024.03.18 kms - 컨베이어 바껴서 일단 주석처리함 (무슨 용도인지는 모르겠음
	// Conveyor Material 에 따라서 값을 써주자
	//for (int iModule = eMODULE_PC1_BEGIN; iModule < eMODULE_PC1_END; iModule++) {
	//	SetConveyorMaterial(GetConveyor((eMODULEs)iModule), sm.Module[iModule].Glass.bMaterial);
	//}
	//>>


	BOOL bServo = TRUE;
	BOOL bAlarm_H = FALSE;
	BOOL bAlarm_L = FALSE; 
	BOOL bMuteAlarm = FALSE;
	for (int i = 0; i < eMOTOR_END; i++) {
		bServo &= sm.MotorStatus[i].tCrt.bServoOn;
	}
	for (int idx = 0; idx < sm.Alarm->nALARM; ++idx)
	{
		if (sm.Alarm->ALARMs[idx].bOccurred && sm.Alarm->ALARMs[idx].iALCD == 2)		// Heavy Alarm Check
		{
			bAlarm_H = TRUE;
		}
		if (sm.Alarm->ALARMs[idx].bOccurred && sm.Alarm->ALARMs[idx].iALCD == 1)		// Light Alarm Check
		{
			bAlarm_L = TRUE;
		}
	}
	bMuteAlarm = sm.Alarm->ALARMs[eAI_GLASS_BOX_CV_LIGHT_CURTAIN_NORMAL].bOccurred;

	if (sm.Register->Reg_Word_1.dwServoOn != bServo)
		sm.Register->Reg_Word_1_NEW.dwServoOn = bServo;
	if (sm.Register->Reg_Word_1.dwAutoRun != sm.Sequence->fAutoMode)
		sm.Register->Reg_Word_1_NEW.dwAutoRun = sm.Sequence->fAutoMode;
	if (bAlarm_H && bAlarm_L) {			// Heavy/Light Alarm 이 같이 발생 한 경우 Heavy 상태만 살리고 Light Alarm 는 0으로 하자
		bAlarm_L = FALSE;
	}
	if (sm.Register->Reg_Word_1.dwHeavyAlarm != bAlarm_H)
		sm.Register->Reg_Word_1_NEW.dwHeavyAlarm = bAlarm_H;
	if (sm.Register->Reg_Word_1.dwLightAlarm != bAlarm_L)
		sm.Register->Reg_Word_1_NEW.dwLightAlarm = bAlarm_L;
	if (sm.Register->Reg_Word_1.dwMuteAlarmClear != bMuteAlarm)
		sm.Register->Reg_Word_1_NEW.dwMuteAlarmClear = bMuteAlarm;
	
	for (int i = 0; i < iRegistAddCount; i++) {
		DWORD* pDword = nullptr;
		if (RegisterAddress[i].eTM == eTM_BIT) {
			switch (RegisterAddress[i].iIndex) {
				case 0: {
					pDword = (DWORD*)&sm.Register->Out;
					break;
				}
				case 1: {
					pDword = (DWORD*)&sm.Register->In;
					break;
				}
				case 2: {
					pDword = (DWORD*)&sm.Register->Axis_On;
					break;
				}
				case 3: {
					pDword = (DWORD*)&sm.Register->Axis_Off;
					break;
				}
				case 4: {
					pDword = (DWORD*)&sm.Register->Axis_ORG_PC;
					break;
				}
				case 5: {
					pDword = (DWORD*)&sm.Register->Axis_ORG_MP;
					break;
				}
				case 6: 
				{
					pDword = (DWORD*)&sm.Register->Axis_ABS_RESET_PC;
				}
				break;
			}
		}
		else if (RegisterAddress[i].eTM == eTM_LONG) {
			switch (RegisterAddress[i].iIndex) {
			case 0: {
				pDword = (DWORD*)&sm.Register->Reg_Word_1;
				break;
			}
			case 1: {
				pDword = (DWORD*)&sm.Register->Reg_Word_2;
				break;
			}
			case 2: {
				pDword = (DWORD*)&sm.Register->Reg_Word_3;
				break;
			}
			}
		}
		if (pDword) {
			// ABS 상태 확인 허용치 
			GetRegisterData(RegisterAddress[i].eTM, RegisterAddress[i].dwStart, RegisterAddress[i].iWordCount, pDword);
			// 해당 신호가 들어온 경우 New 에 CRT 데이터를 넣어줌 
			// SCT
			//if( bFirst && !sm.IOs->LD.In.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
			// CCTL
			//if( bFirst && !sm.IOs->UNDL.In.fUND_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
			// CCTR
			//if( bFirst && !sm.IOs->UNDR.In.fUND_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
			// ULDL
			//if( bFirst && !sm.IO->tIn_CRT.fUND_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
			// ULDR
			//if( bFirst && !sm.IO->tIn_CRT.fUND_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
			// LD
			//if( bFirst && !sm.IO->tIn_CRT.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION)
			//{
			//}
			//else{
			//	sm.Register->Reg_Word_1_NEW = sm.Register->Reg_Word_1;
			//	sm.Register->Reg_Word_2_NEW = sm.Register->Reg_Word_2;
			//	sm.Register->Reg_IO_1_NEW = sm.Register->Reg_IO_1;
			//	sm.Register->Reg_IO_2_NEW = sm.Register->Reg_IO_2;
			//	
			//	//. 12.17. default 값.
			//	sm.Register->Reg_Word_1_NEW.dwOverdrive = 10000;
			//}
		}
	}
	// 21.03.03 오버라이드 값이 0이면 죽는 곳이 많음...
	// 기본 값으로 1넣어줌
	sm.Register->Reg_Word_1_NEW.dwOverdrive == 0 ? sm.Register->Reg_Word_1_NEW.dwOverdrive = 1 : sm.Register->Reg_Word_1_NEW.dwOverdrive;
	// write
	RegisterMemory();
}
void YaskawaMP::RegisterMemory() {
	int iItemCount = 0;
	DWORD* pDw = nullptr;
	DWORD* pDwOld = nullptr;
	CString strCmd;
	
	{
		iItemCount = sizeof(sm.Register->Reg_Word_1)/sizeof(DWORD);
		pDw = (DWORD*)(&sm.Register->Reg_Word_1_NEW);
		pDwOld = (DWORD*)(&sm.Register->Reg_Word_1);
		for (int i = 0; i < iItemCount; i++) 
		{
			if(pDwOld[i] != pDw[i]) 
			{
				strCmd.Format(_T("ML%d"), (i * 2 ) + RegisterAddress[0].dwStart);
				int iData = pDw[i];
				SetRegData(strCmd, iData);
			}
		}
	}

	{
		iItemCount = sizeof(sm.Register->Reg_Word_2) / sizeof(DWORD);
		pDw = (DWORD*)(&sm.Register->Reg_Word_2_NEW);
		pDwOld = (DWORD*)(&sm.Register->Reg_Word_2);
		for (int i = 0; i < iItemCount; i++) 
		{
			if (pDwOld[i] != pDw[i]) 
			{
				strCmd.Format(_T("ML%d"), (i * 2) + RegisterAddress[1].dwStart);
				int iData = pDw[i];
				SetRegData(strCmd, iData);
			}
		}
	}

	{
		iItemCount = sizeof(sm.Register->Out) / sizeof(DWORD);
		pDw = (DWORD*)(&sm.Register->Out_New);
		pDwOld = (DWORD*)(&sm.Register->Out);
		for (int i = 0; i < iItemCount; i++) 
		{
			if (pDwOld[i] != pDw[i]) 
			{
				strCmd.Format(_T("MB%d%X"), RegisterAddress[3].dwStart + i / 16, i % 16);
				int iData = pDw[i];
				SetRegData(strCmd, iData);
			}
		}
	}

	{
		iItemCount = sizeof(sm.Register->Axis_On) / sizeof(DWORD);
		pDw = (DWORD*)(&sm.Register->Axis_On_NEW);
		pDwOld = (DWORD*)(&sm.Register->Axis_On);
		for (int i = 0; i < iItemCount; i++) 
		{
			if (pDwOld[i] != pDw[i]) 
			{
				strCmd.Format(_T("MB%d%X"), RegisterAddress[5].dwStart + i / 16, i % 16);
				int iData = pDw[i];
				SetRegData(strCmd, iData);
			}
		}
	}
	{
		iItemCount = sizeof(sm.Register->Axis_Off) / sizeof(DWORD);
		pDw = (DWORD*)(&sm.Register->Axis_Off_NEW);
		pDwOld = (DWORD*)(&sm.Register->Axis_Off);
		for (int i = 0; i < iItemCount; i++) 
		{
			if (pDwOld[i] != pDw[i]) 
			{
				strCmd.Format(_T("MB%d%X"), RegisterAddress[6].dwStart + i / 16, i % 16);
				int iData = pDw[i];
				SetRegData(strCmd, iData);
			}
		}
	}
	{
		iItemCount = sizeof(sm.Register->Axis_ORG_PC) / sizeof(DWORD);
		pDw = (DWORD*)(&sm.Register->Axis_ORG_PC_NEW);
		pDwOld = (DWORD*)(&sm.Register->Axis_ORG_PC);
		for (int i = 0; i < iItemCount; i++) 
		{
			if (pDwOld[i] != pDw[i]) 
			{
				strCmd.Format(_T("MB%d%X"), RegisterAddress[7].dwStart + i / 16, i % 16);
				int iData = pDw[i];
				SetRegData(strCmd, iData);
			}
		}
	}
	{
		iItemCount = sizeof(sm.Register->Axis_ABS_RESET_PC) / sizeof(DWORD);
		pDw = (DWORD*)(&sm.Register->Axis_ABS_RESET_PC_NEW);
		pDwOld = (DWORD*)(&sm.Register->Axis_ABS_RESET_PC);
		for (int i = 0; i < iItemCount; i++)
		{
			if (pDwOld[i] != pDw[i])
			{
				strCmd.Format(_T("MB%d%X"), RegisterAddress[9].dwStart + i / 16, i % 16);
				int iData = pDw[i];
				SetRegData(strCmd, iData);
			}
		}
	}
	//iItemCount = sizeof(sm.Register->Reg_Word_3)/sizeof(DWORD);
	//pDw =  &sm.Register->Reg_Word_3_NEW.dw1_MaxLimitSpeed;
	//pDwOld = &sm.Register->Reg_Word_3.dw1_MaxLimitSpeed;
	//for (int i = 0; i < iItemCount; i++) {
	//	if(pDwOld[i] != pDw[i]){
	//		strCmd.Format(_T("ML%d"),(i*2)+2000);
	//		int iData = pDw[i];
	//		SetRegData(strCmd, iData);
	//	}
	//}

	//{eTM_BIT, 100, 0, 6},
	//{ eTM_BIT,	200,	1,	6 },
	//DWORD* pdwData = (DWORD*)&sm.Register->;
	//GetRegisterDataBit(TRUE, iStart, RegisterAddressIO_In[i].iWordCount, &pdwData[iOffsetCount_i]);
	//iOffsetCount_i += RegisterAddressIO_In[i].iWordCount * 16;

}

void YaskawaMP::LoopAlarm() {
	int nRegistCount = countof(Register_Alarm);
	typedef struct T_REG_ALARM {
		WORD w170;
		WORD w171;
	} T_REG_ALARM;

	for (int i = 0; i < nRegistCount; i++) {
		T_REG_ALARM tRegAlarm;
		GetRegisterDataWord(Register_Alarm[i].eTM, Register_Alarm[i].dwStart, Register_Alarm[i].iWordCount, &tRegAlarm.w170);

		// 2024.06.08 LBH - MIDK 요청 알람.
		if (tRegAlarm.w170 > 0 && tRegAlarm.w170 <= 255) {
			if (!sm.Alarm->ALARMs[eAI_IO_MODULE_ALARM_170].bOccurred)
				SetAlarm(eAI_IO_MODULE_ALARM_170, IDS_IO_MODULE_ALARM_1 + tRegAlarm.w170 - 1, nullptr, FALSE);
		}
		else {
			sm.Alarm->ALARMs[eAI_IO_MODULE_ALARM_170].bReset = TRUE;
		}

		if (tRegAlarm.w171 > 0 && tRegAlarm.w170 <= 255) {
			if (!sm.Alarm->ALARMs[eAI_IO_MODULE_ALARM_171].bOccurred)
				SetAlarm(eAI_IO_MODULE_ALARM_171, IDS_IO_MODULE_ALARM_1 + tRegAlarm.w171 - 1, nullptr, FALSE);
		}
		else {
			sm.Alarm->ALARMs[eAI_IO_MODULE_ALARM_171].bReset = TRUE;
		}
	}

	
}

void YaskawaMP::LoopIO(BOOL bFirst) {
	{// IO-1;
		int iLoop_i = countof(RegisterAddressIO_In);
		int iLoop_o = countof(RegisterAddressIO_Out);
		int iOffsetCount, iOffsetCount_i, iOffsetCount_o;
		iOffsetCount = iOffsetCount_i = iOffsetCount_o = 0;
		for (int i = 0; i < iLoop_i; i++) {
			int iStart = RegisterAddressIO_In[i].dwStart;
			DWORD* pdwData = (DWORD*)&sm.IO->tIn_CRT;
			GetRegisterDataBit(TRUE, iStart, RegisterAddressIO_In[i].iWordCount, &pdwData[iOffsetCount_i]);
			iOffsetCount_i += RegisterAddressIO_In[i].iWordCount * 16;
		}
		for (int i = 0; i < iLoop_o; i++) {
			int iStart = RegisterAddressIO_Out[i].dwStart;
			DWORD* pdwData = (DWORD*)&sm.IO->tOut_CRT;
			GetRegisterDataBit(FALSE, iStart, RegisterAddressIO_Out[i].iWordCount, &pdwData[iOffsetCount_o]);

			if (bFirst) {
				int iLoopCount = RegisterAddressIO_Out[i].iWordCount * 16;
				DWORD* pCrt = (DWORD*)&sm.IO->tOut_CRT;
				DWORD* pNew = (DWORD*)&sm.IO->tOut_NEW;
				DWORD* pRelease= (DWORD*)&sm.IO->tOut_Release;
				for (int idx = 0; idx < iLoopCount; idx++) {
					if (pRelease[iOffsetCount_o + idx] == eIO_LOCK_CONTROL)
						continue;
					if (pCrt[iOffsetCount_o+idx] != pNew[iOffsetCount_o+idx]) {
						SetRegisterBit(iStart, idx, pNew[iOffsetCount_o+idx]);
					}
				}
			}
			iOffsetCount_o += RegisterAddressIO_Out[i].iWordCount * 16;
		}

		if (!bFirst) {
			//memcpy(&sm.IO->tOut_NEW, &sm.IO->tOut_CRT, sizeof(sm.IO->tOut_CRT));
			//. LD / UND만 있음.
			sm.IO->tOut_Release.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST = eIO_LOCK_UI;
		}
	}
}

void YaskawaMP::th_IO(){
	
	BOOL bCheck = TRUE;

	if (FALSE == sm.IsSimultation()) {

		while (!m_bIsControllerMainOpened) {
			TRACE(_T("THIO Wait\n"));

			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
	}
	bCheck &= OpenController(TRUE);

	//if (FALSE == bCheck) {
	//	IPMessage(_T("THETC() - OpenController() 실패."));
	//	return;
	//}

	BOOL bFirst = FALSE;
	char szAddress[10] = {0,};
	
	while (!m_bIsThreadKill) {
		//GetIOBitData(FALSE, &sm.IO->tOut_CRT.fIO_OUT[0]);
		
		if (sm.IsSimultation()) {
			//memcpy(&sm.IO->tOut_CRT, &sm.IO->tOut_NEW, sizeof(T_IO_OUT));
		}
		else {
			
			//<< 2024.03.18 kms - 베트남 장비에선 안쓰므로 일단 주석처리 했음
			LoopRegist(bFirst);
			LoopIO(bFirst);
			LoopAlarm();
			//LoopConveyor(bFirst);
			//>>
			
			if (sm.Register->In.dw2000_AlarmResetCheck) {
				sm.Register->Out_New.dw1000_AlarmReset = FALSE;
			}
			bFirst = TRUE;
			
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	
}
void YaskawaMP::th_etc() {
	BOOL bCheck = TRUE;
	while (!m_bIsControllerMainOpened) {
		TRACE(_T("THETC Wait\n"));
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	bCheck &= OpenController(TRUE);

	//if (FALSE == bCheck) {
	//	IPMessage(_T("THETC() - OpenController() 실패."));
	//	return;
	//}

	while (!m_bIsThreadKill) {
		AlarmChecker::AlarmChecker_MOTOR();
		AlarmChecker::AlarmChecker_IO();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}


int YaskawaMP::GetRegData(CString strAddresss){
	BOOL bReturn = SetController(0);
	int iReturn = 0;
	DWORD rc =0;
	char RegistAddress[10] = {0,};
	strAddresss.MakeUpper();
	sprintf_s(RegistAddress, "%s", CStringA(strAddresss).GetBuffer());
	BOOL bDword = FALSE;
	int iWordCount =1;
	if( strAddresss.Mid(1,1) == _T("L") || strAddresss.Mid(1,1) == _T("l")){
		bDword = TRUE;
	}
	HREGISTERDATA hRegist;
	
	WORD word = 0;	
	DWORD dword = 0;
	DWORD ReadDataNumber;
	
	rc = ymcGetRegisterDataHandle((LPBYTE)RegistAddress ,&hRegist);
	if (rc != MP_SUCCESS) {
		TRACE(_T("ymcGetRegisterDataHandle Fail"));
	}
	if( bDword){
		rc = ymcGetRegisterData(hRegist, iWordCount, &dword, &ReadDataNumber);
	}else{
		rc = ymcGetRegisterData(hRegist, iWordCount, &word, &ReadDataNumber);
	}
	if (rc != MP_SUCCESS) {
		TRACE(_T("ymcGetRegisterData Fail \n"));
	}
	
	if( bDword){
		iReturn = (int)dword;
		
	}else{
		iReturn = (int)word;
	}
	
	return iReturn;
}

double YaskawaMP::GetRegDataDouble(CString strAddresss){
	int lData = GetRegData(strAddresss);
	double dData = double(lData);
	dData /= 1000.;
	
	return dData;
}

int YaskawaMP::GetRegDataInt(CString strAddresss){
	int dwData = GetRegData(strAddresss);
	int iData = int(dwData);
	return iData;

}

void YaskawaMP::SetRegData(CString strAddresss, double dData){
	 int iData = dData*1000;
	SetRegData(strAddresss, iData);
}

void YaskawaMP::SetRegData(CString strAddresss, int iData){
	if( strAddresss.Mid(0,1) == _T("I") || strAddresss.Mid(0,1) == _T("i")){
		return;
	}
	BOOL bReturn = SetController(0);
	
	DWORD rc =0;
	char RegistAddress[10] = {0,};
	strAddresss.MakeUpper();
	sprintf_s(RegistAddress, "%s", CStringA(strAddresss).GetBuffer());
	int iWordCount = 1;
	if( strAddresss.Mid(1,1) == _T("L") || strAddresss.Mid(1,1) == _T("l")){
		iWordCount = 1;
	}
	HREGISTERDATA hRegist;
	
	DWORD dwData = iData;
	rc = ymcGetRegisterDataHandle((LPBYTE)RegistAddress ,&hRegist);
	if (rc != MP_SUCCESS) {
		TRACE(_T("ymcGetRegisterDataHandle Fail \n"));
	}
	rc = ymcSetRegisterData(hRegist, iWordCount, &dwData);
	if (rc != MP_SUCCESS) {
		TRACE(_T("ymcGetRegisterData Fail \n"));
	}

}


void YaskawaMP::SetRegConveyorData(CString strAddresss, int iData) {
	if (strAddresss.Mid(0, 1) == _T("I") || strAddresss.Mid(0, 1) == _T("i")) {
		return;
	}
	BOOL bReturn = SetController(0);

	DWORD rc = 0;
	char RegistAddress[10] = { 0, };
	strAddresss.MakeUpper();
	sprintf_s(RegistAddress, "%s", CStringA(strAddresss).GetBuffer());
	int iWordCount = 1;
	if (strAddresss.Mid(1, 1) == _T("W") || strAddresss.Mid(1, 1) == _T("w")) {
		iWordCount = 1;
	}
	HREGISTERDATA hRegist;

	DWORD dwData = iData;
	rc = ymcGetRegisterDataHandle((LPBYTE)RegistAddress, &hRegist);
	if (rc != MP_SUCCESS) {
		TRACE(_T("ymcGetRegisterDataHandle Fail \n"));
	}
	rc = ymcSetRegisterData(hRegist, iWordCount, &dwData);
	if (rc != MP_SUCCESS) {
		TRACE(_T("ymcGetRegisterData Fail \n"));
	}

}


BOOL YaskawaMP::InitMotionCmd(int iAxis){
	BOOL bReturn = SetController(0);
	CString strCmd;
	auto& MPYs = sm.MotorParameter[iAxis].tYaskawa;
	
	if(MPYs.bSetGantry){
		int iLogAxis1 = MPYs.iCh1_Axis - 1;
		int iLogAxis2 = MPYs.iCh2_Axis - 1;
		strCmd.Format(_T("OL%04x"), AXIS_BASE+0x0008+ (iLogAxis1* AXIS_PITCH));
		SetRegData(strCmd, 0);
		strCmd.Format(_T("OL%04x"), AXIS_BASE+0x0008+ (iLogAxis2* AXIS_PITCH));
		SetRegData(strCmd, 0);
		
	}
	else {
		int iLogAxis = MPYs.iLogicalAxisNo - 1;
		strCmd.Format(_T("OL%04x"), AXIS_BASE + 0x0008 + (iLogAxis * AXIS_PITCH));
		SetRegData(strCmd, 0);
	}

	return bReturn;
}

double YaskawaMP::GetOriainOffset(int iAxis){
	BOOL bReturn = SetController(0);
	CString strCmd;
	int iLogAxis = sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo -1;
	strCmd.Format(_T("OL%04x"), AXIS_BASE+0x0048+ (iLogAxis* AXIS_PITCH));
	return GetRegDataDouble(strCmd);
}

void YaskawaMP::SetRegisterWORD(const WORD* _cur, const WORD* _new, const int _count, const int _address)
{
	CString strCommand;
	for (int iIndex = 0; iIndex < _count; ++iIndex)
	{
		if (_cur[iIndex] != _new[iIndex])
		{
			strCommand = Format(_T("OW%X"), _address + iIndex);
			SetRegData(strCommand, _new[iIndex]);
		}
	}
}

BOOL YaskawaMP::SetOrizinAxis(int iAxis, double dPos){
	BOOL bReturn = SetController(0);
	CString strCmd;
	int iLogAxis = iAxis;
	int iDATA = AXIS_BASE + 0x0048 + (iLogAxis * AXIS_PITCH);
	strCmd.Format(_T("OL%04x"), AXIS_BASE+0x0048+ (iLogAxis* AXIS_PITCH));
	SetRegData(strCmd, dPos);
	return bReturn;
}

BOOL YaskawaMP::SetOrizinResetAxis(int iAxis){
	return SetOrizinAxis(iAxis);
}
	
BOOL YaskawaMP::SetOrizinLogical(int iAxis, double dPos){
	int iLogAxis = sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1;
	return SetOrizinAxis(iLogAxis, dPos);
}

BOOL YaskawaMP::SetOrizinResetLogical(int iAxis){
	int iLogAxis = sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1;
	return SetOrizinAxis(iLogAxis, 0);
}

BOOL YaskawaMP::SetTorque(int iAxis, double dRate) {
	// 1= 0.01%
	dRate *= 100;
	BOOL bReturn = SetController(0);
	CString strCmd;
	int iLogAxis = sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1;
	strCmd.Format(_T("OL%04x"), AXIS_BASE+0x008c+ (iLogAxis* AXIS_PITCH));
	SetRegData(strCmd, dRate);
	return bReturn;
}


BOOL YaskawaMP::SetLimit(int iAxis, double dLimitN, double dLimitP) {
	BOOL bReturn = SetController(0);
	CString strCmd;
	if (sm.MotorParameter[iAxis].tYaskawa.bSetGantry) {
		int iLogAxis = sm.MotorParameter[iAxis].tYaskawa.iCh1_Axis - 1;
		//dLimitN *= sm.MotorParameter[iAxis].dPositionRate;
		//dLimitP *= sm.MotorParameter[iAxis].dPositionRate;
		// -Limit
		strCmd.Format(_T("OL%04x"), AXIS_BASE + 0x0068 + (iLogAxis * AXIS_PITCH));
		SetRegData(strCmd, dLimitN);

		// +Limit
		strCmd.Format(_T("OL%04x"), AXIS_BASE + 0x0066 + (iLogAxis * AXIS_PITCH));
		SetRegData(strCmd, dLimitP);


		iLogAxis = sm.MotorParameter[iAxis].tYaskawa.iCh2_Axis - 1;
		//dLimitN *= sm.MotorParameter[iAxis].dPositionRate;
		//dLimitP *= sm.MotorParameter[iAxis].dPositionRate;
		// -Limit
		strCmd.Format(_T("OL%04x"), AXIS_BASE + 0x0068 + (iLogAxis * AXIS_PITCH));
		SetRegData(strCmd, dLimitN);

		// +Limit
		strCmd.Format(_T("OL%04x"), AXIS_BASE + 0x0066 + (iLogAxis * AXIS_PITCH));
		SetRegData(strCmd, dLimitP);

	}
	else {
		int iLogAxis = sm.MotorParameter[iAxis].tYaskawa.iLogicalAxisNo - 1;
		//dLimitN *= sm.MotorParameter[iAxis].dPositionRate;
		//dLimitP *= sm.MotorParameter[iAxis].dPositionRate;
		// -Limit
		strCmd.Format(_T("OL%04x"), AXIS_BASE + 0x0068 + (iLogAxis * AXIS_PITCH));
		SetRegData(strCmd, dLimitN);

		// +Limit
		strCmd.Format(_T("OL%04x"), AXIS_BASE + 0x0066 + (iLogAxis * AXIS_PITCH));
		SetRegData(strCmd, dLimitP);
	}
	return bReturn;
}
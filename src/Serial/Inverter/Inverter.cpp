
#include "pch.h"
#include "Serial.h"
#include "Inverter.h"
//#include "CInverterWnd.h"
#include "MainFrm.h"
#include "afxwin.h"
//#include "stdafx.h"

T_COMMAND g_command[] = {
	// iCommandStep,		eCmdType,	iSlaveAddress, iStartAddress, iData/ iByteCount

	// eWRITE_COIL
	{ eIC_W_RUN,			eWRITE_COIL,	0x01,	0x0000,	0xFF00, 6},
	{ eIC_W_STOP,			eWRITE_COIL,	0x01,	0x0000,	0x0000, 6},
	{ eIC_W_DIRECTION_FWD,	eWRITE_COIL,	0x01,	0x0001,	0x0000, 6},
	{ eIC_W_DIRECTION_BWD,	eWRITE_COIL,	0x01,	0x0001,	0xFF00, 6},

		// eWRITE_REG
	{ eIC_W_ACC1,			eWRITE_REG,		0x01,	0x1102, 0x0002, 6},
	{ eIC_W_DEC1,			eWRITE_REG,		0x01,	0x1104, 0x0002, 6},
	{ eIC_W_ACC2,			eWRITE_REG,		0x01,	0x1273, 0x0002, 6},
	{ eIC_W_DEC2,			eWRITE_REG,		0x01,	0x1275, 0x0002, 6},

	{ eIC_W_FREQUENCY,		eWRITE_REG,		0x01,	0x0000, 0x0002, 5},

	// eREAD_COIL
	{ eIC_R_RUN,			eREAD_COIL,		0x01,	0x000E,	0x0001, 6},
	{ eIC_R_DIRECTION,		eREAD_COIL,		0x01,	0x0001,	0x0001, 6},
	{ eIC_R_READY,			eREAD_COIL,		0x01,	0x0010,	0x0001, 6},

	// eREAD_REG
	{ eIC_R_FREQUENCY,		eREAD_REG,		0x01,	0x0000,	0x0002, 9},

	{ eIC_R_ALARM1,			eREAD_REG,		0x01,	0x0011,	0x0002, 7},
	{ eIC_R_ALARM2,			eREAD_REG,		0x01,	0x001B,	0x0002, 7},
	{ eIC_R_ALARM3,			eREAD_REG,		0x01,	0x0025,	0x0002, 7},
	{ eIC_R_ALARM4,			eREAD_REG,		0x01,	0x002F,	0x0002, 7},

	{ eIC_R_ACC1,			eREAD_REG,		0x01,	0x1102,	0x0002, 7},
	{ eIC_R_DEC1,			eREAD_REG,		0x01,	0x1104,	0x0002, 7},
	{ eIC_R_ACC2,			eREAD_REG,		0x01,	0x1273,	0x0002, 7},
	{ eIC_R_DEC2,			eREAD_REG,		0x01,	0x1275,	0x0002, 7},



};


CInverter::CInverter(CWnd* pwndParent, int iInverter) : TIPCT<CFrameWnd>(&g_ipClient)
{
	m_pwndParent = pwndParent;
	m_iInverter = iInverter;
	SetUnitName(Format("Inverter%d", iInverter));
	m_tCommandCurrent.iCommand = eIC_R_RUN;

	memset(m_eRecvType, eRECV_START, sizeof(m_eRecvType));
}

CInverter::~CInverter()
{
	m_setCommand.clear();
	m_xcomm.DeleteBuffer();
	m_xcomm.Close();
}

BEGIN_MESSAGE_MAP(CInverter, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

BEGIN_IP_COMMAND_TABLE(CInverter, IIPCommandTarget)
	ON_IP("Inverter",		"Initialize",				OnIPInitialize)
	ON_IP_EXCL("Inverter",	"Run",						OnIPRun)
	ON_IP_EXCL("Inverter",	"Stop",						OnIPStop)
	ON_IP_EXCL("Inverter",	"SetFrequency",				OnIPSetFrequency)
	ON_IP_EXCL("Inverter",	"DataSetting",				OnIPSetting) // 가감속 셋팅
END_IP_COMMAND_TABLE()

IMPLEMENT_DYNCREATE(CInverter, CFrameWnd)


//-->24.03.15 KGH ADD

int CInverter::OnCreate(LPCREATESTRUCT lpCreateStruct) {

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(_T("InverterUI"));

	Initialize();
	m_ThreadReader = std::thread([this]() { ReaderT(); });
	return 0;
}

BOOL CInverter::Initialize() {

	PostIPCommand(NULL, GetUnitName(), "Inverter", "Initialize");
	//m_pReader->Open();
	return TRUE;
}

BOOL CInverter::PortOpenClose(BOOL _bOpen) {
	CProfile profile(Format(_T("../cfg/Serial/Inverter%d.ini"), m_iInverter + 1));
	CProfileSection section = profile.GetSection(_T("CommPort"));

	int iPortNo = section.GetItemValue(_T("PortNo"), 7);
	int iBaud = section.GetItemValue(_T("Baud"), 9600);
	int iDataBit = section.GetItemValue(_T("DataBits"), 8);
	int iStopBit = section.GetItemValue(_T("StopBits"), 0);
	int iParity = section.GetItemValue(_T("Parity"), 0);

	if (_bOpen) {
		if (!m_xcomm.Open(iPortNo, iBaud, TRUE, iDataBit, iParity, iStopBit, RTS_CONTROL_DISABLE, DTR_CONTROL_DISABLE)) {
			sm.SerialConnection->bInverter[m_iInverter] = FALSE;
			return FALSE;
		}
		else {
			sm.SerialConnection->bInverter[m_iInverter] = TRUE;
		}
	}
	else {
		m_xcomm.Close();
		sm.SerialConnection->bInverter[m_iInverter] = FALSE;
	}

	return TRUE;
}


void CInverter::ReaderT() {
	while (TRUE)
	{
		std::this_thread::sleep_for(10ms);

		if (!m_xcomm.IsOpen())
			continue;

		if (m_setCommand.size() > 0) {
			m_tCommandCurrent = m_setCommand[0];
			m_setCommand.erase(m_setCommand.begin());
		}
		else {
			if ((m_tCommandCurrent.iCommand < eIC_R_RUN) || (m_tCommandCurrent.iCommand > eIC_R_DEC2))
				m_tCommandCurrent.iCommand = eIC_R_RUN;
		}

		SendRecvCommand(m_tCommandCurrent.iCommand++, m_tCommandCurrent.iValue);
	}
}

void CInverter::AddCommand(int iCommand, int iValue) {
	T_COMMAND_DATA command;
	command.iCommand = iCommand;
	command.iValue = iValue;
	m_eRecvType[iCommand] = eRECV_START;

	m_setCommand.push_back(command);
}

BOOL CInverter::SendRecvCommand(int iCommand, int iRegisterData) {
	BYTE data[100], recvdata[100];
	memset(data, 0x00, sizeof(data));
	memset(recvdata, 0x00, sizeof(recvdata));
	int iDataSize = 0, iRecvDataSize = 0;

	T_COMMAND command = g_command[iCommand];
	data[iDataSize++] = command.iSlaveAddress;
	data[iDataSize++] = command.eCmdType;

	data[iDataSize++] = HIBYTE(command.iStartAddress);
	data[iDataSize++] = LOBYTE(command.iStartAddress);

	data[iDataSize++] = HIBYTE(command.iDataOrCount);
	data[iDataSize++] = LOBYTE(command.iDataOrCount);

	if (command.eCmdType == eWRITE_REG) {
		// iByteCount
		data[iDataSize++] = command.iDataOrCount * 2;

		// 쓰기 데이터 1
		int iRegisterData1 = 0; // 임시
		data[iDataSize++] = HIBYTE(iRegisterData1);
		data[iDataSize++] = LOBYTE(iRegisterData1);

		// 쓰기 데이터 2
		data[iDataSize++] = HIBYTE(iRegisterData);
		data[iDataSize++] = LOBYTE(iRegisterData);
	}

	MakeCRC16(data, iDataSize);

	// 임시
	CStringA strCmd;
	for (int i = 0; i < iDataSize; i++) {
		strCmd += Format("%02x ", data[i]);
	}
	int iRetry = 0;

	while (TRUE) {
		m_xcomm.DeleteBuffer();
		m_xcomm.Purge();
		if (!m_xcomm.Write(data, iDataSize, 100)) {
			if (++iRetry < 2) {
				m_xcomm.DeleteBuffer();
				std::this_thread::sleep_for(30ms);
				continue;
			}

			//PortOpenClose(FALSE);

			m_eRecvType[iCommand] = eRECV_NG;
			return FALSE;
		}


		std::this_thread::sleep_for(10ms);
		iRecvDataSize = command.iRecvCount;

		if (!m_xcomm.Read(recvdata, iRecvDataSize, 100)) {
			if (++iRetry < 2) {
				m_xcomm.DeleteBuffer();
				std::this_thread::sleep_for(30ms);
				continue;
			}

			//PortOpenClose(FALSE);

			m_eRecvType[iCommand] = eRECV_NG;
			return FALSE;
		}

		break;
	}

	CString strRecv;
	for (int i = 0; i < iRecvDataSize; i++) {
		strRecv += Format("%02x ", recvdata[i]);
	}

	m_eRecvType[iCommand] = eRECV_OK;
	DataParsing(iCommand, recvdata, iRecvDataSize);
	return TRUE;
}

void CInverter::DataParsing(int iCommand, BYTE* nData, int iSize) {
	CString strRecv;
	for (int i = 0; i < iSize; i++) {
		strRecv += Format("%02x ", nData[i]);
	}

	switch (iCommand) {
		case eIC_R_RUN : {
			sm.InverterData[m_iInverter].iRun = nData[3];
			break;
		}
		case eIC_R_DIRECTION: {
			sm.InverterData[m_iInverter].iDirection = nData[3];
			break;
		}
		case eIC_R_READY: {
			sm.InverterData[m_iInverter].iReady = nData[3];
			break;
		}
		case eIC_R_FREQUENCY: {
			sm.InverterData[m_iInverter].iFreaquency = MAKEWORD(nData[6], nData[5]);
			break;
		}
		case eIC_R_ALARM1:
		case eIC_R_ALARM2:
		case eIC_R_ALARM3:
		case eIC_R_ALARM4: {
			int iAlarm = iCommand - eIC_R_ALARM1;
			sm.InverterData[m_iInverter].iTripCause[iAlarm] = nData[4];
			sm.InverterData[m_iInverter].iStatus[iAlarm] = nData[5];
			break;
		}
		case eIC_R_ACC1: {
			sm.InverterData[m_iInverter].iAccTime1 = MAKEWORD(nData[6], nData[5]);
			break;
		}
		case eIC_R_DEC1: {
			sm.InverterData[m_iInverter].iDecTime1 = MAKEWORD(nData[6], nData[5]);
			break;
		}
		case eIC_R_ACC2: {
			sm.InverterData[m_iInverter].iAccTime2 = MAKEWORD(nData[6], nData[5]);
			break;
		}
		case eIC_R_DEC2: {
			sm.InverterData[m_iInverter].iDecTime2 = MAKEWORD(nData[6], nData[5]);
			break;
		}
	}
}


void CInverter::MakeCRC16(BYTE* Bytes, int& iSize)
{
	int iCRC = CRC16(Bytes, iSize);
	Bytes[iSize++] = LOBYTE(iCRC);
	Bytes[iSize++] = HIBYTE(iCRC);
}

WORD CInverter::CRC16(BYTE* nData, int wLength)
{
	static const WORD wCRCTable[] = {
	0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
	0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
	0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
	0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
	0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
	0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
	0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
	0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
	0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
	0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
	0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
	0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
	0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
	0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
	0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
	0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
	0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
	0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
	0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
	0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
	0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
	0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
	0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
	0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
	0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
	0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
	0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
	0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
	0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
	0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
	0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
	0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

	BYTE nTemp;
	WORD wCRCWord = 0xFFFF;

	while (wLength--) {
		nTemp = *nData++ ^ wCRCWord;
		wCRCWord >>= 8;
		wCRCWord ^= wCRCTable[nTemp];
	}
	return wCRCWord;
}

BOOL CInverter::OnIPInitialize(CIPCommand& cmd)
{
	const CIPVar&		var			= cmd.m_var;
	CIPVar&				varResult	= cmd.m_varResult;
	
	cmd.Log(__TFUNCTION__ _T("[Step = %d][Error = %d]"), cmd.GetCurrentStep(), cmd.IsOK() ? FALSE : TRUE);
	enum {
		S_INIT,
		S_CONNECT,
		S_END,
	};

	if (!cmd.IsOK() || !cmd.IsChildOK())
		return FALSE;

	BOOL bOpen = var("Open", TRUE);
	switch (cmd.GetCurrentStep()) {
		case S_INIT : {
			m_xcomm.DeleteBuffer();
			BOOL bResut = m_xcomm.Close();

			if (!bOpen)
				return bResut;

			cmd.EnqueueNextStep();
			break;
		}
		case S_CONNECT : {
			if (PortOpenClose(bOpen)) {
				cmd.EnqueueNextStep(S_END, 100);
			}
			else {
				cmd.EnqueueNextStep(S_CONNECT, 100);
			}

			break;
		}
		case S_END : {
			break;
		}
		default : break;
	}

	return TRUE;
}

BOOL CInverter::OnIPRun(CIPCommand& cmd)
{
	const CIPVar&		var			= cmd.m_var;
	CIPVar&				varResult	= cmd.m_varResult;
	
	if (cmd.GetCurrentStep() != cmd.GetPreviousStep())
		cmd.Log(__TFUNCTION__ _T("[Step = %d][Error = %d]"), cmd.GetCurrentStep(), cmd.IsOK() ? FALSE : TRUE);
	
	enum {
		S_INIT,
		S_SPEED_SET,
		S_SPEED_SET_CHECK,
		S_DIRECTION_SET,
		S_DIRECTION_SET_SET_CHECK,
		S_RUN,
		S_RUN_CHECK,
		S_END,
	};

	int _eModule = var("eMODULEs", eMODULE_NONE);
	int iFrequency = var("Frequency", 3000);
	BOOL bFwd = var("Fwd", FALSE);
	
	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_DOWN;
		return FALSE;
	}

	if (!m_xcomm.IsOpen()) {
		cmd.ReserveErrorStep();
		return TRUE;
	}

	int iDirection = bFwd ? eIC_W_DIRECTION_FWD : eIC_W_DIRECTION_BWD;
	int iTimeout = 3 * 1000;
	BOOL bCheck = TRUE;
	switch (cmd.GetCurrentStep()) {
		case S_INIT : {
			sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_RUN;
			cmd.EnqueueNextStep();
			break;
		}
		case S_SPEED_SET: {
			cmd.m_dwCookie = GetTickCount64();
			AddCommand(eIC_W_FREQUENCY, iFrequency);
			cmd.EnqueueNextStep();
			break;
		}
		case S_SPEED_SET_CHECK: {
			bCheck = m_eRecvType[eIC_W_FREQUENCY] == eRECV_OK;
			bCheck &= Serial::Inverter::GetInverterFrequency((eMODULEs)_eModule) == iFrequency;

			if (bCheck) {
				cmd.EnqueueNextStep(-1, 100);
			}
			else {
				if (GetTickCount64() - cmd.m_dwCookie < iTimeout) {
					if (m_eRecvType[eIC_W_FREQUENCY] == eRECV_NG)
						cmd.ReserveErrorStep();
					else
						cmd.EnqueueNextStepCurrent();
				}
				else {
					cmd.ReserveErrorStep();
				}
			}
			break;
		}
		case S_DIRECTION_SET: {
			cmd.m_dwCookie = GetTickCount64();
			AddCommand(iDirection);
			cmd.EnqueueNextStep();
			break;
		}
		case S_DIRECTION_SET_SET_CHECK: {
			bCheck = m_eRecvType[iDirection] == eRECV_OK;
			bCheck &= Serial::Inverter::GetInverterDirection((eMODULEs)_eModule, bFwd);

			if (bCheck) {
				cmd.EnqueueNextStep(-1, 100);
			}
			else {
				if (GetTickCount64() - cmd.m_dwCookie < iTimeout) {
					if (m_eRecvType[iDirection] == eRECV_NG)
						cmd.ReserveErrorStep();
					else
						cmd.EnqueueNextStepCurrent();
				}
				else {
					cmd.ReserveErrorStep();
				}
			}
			break;
		}
		case S_RUN: {
			cmd.m_dwCookie = GetTickCount64();
			AddCommand(eIC_W_RUN);
			cmd.EnqueueNextStep();
			break;
		}
		case S_RUN_CHECK: {
			bCheck = m_eRecvType[eIC_W_RUN] == eRECV_OK;
			bCheck &= Serial::Inverter::GetInverterRun((eMODULEs)_eModule);
			if (bCheck) {
				cmd.EnqueueNextStep(-1,100);
			}
			else {
				if (GetTickCount64() - cmd.m_dwCookie < iTimeout) {
					if (m_eRecvType[eIC_W_RUN] == eRECV_NG)
						cmd.ReserveErrorStep();
					else
						cmd.EnqueueNextStepCurrent();
				}
				else {
					cmd.ReserveErrorStep();
				}
			}
			break;
		}
		case S_END : {
			sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_COMPLETED;
			break;
		}
		default : break;
	}

	return TRUE;
}

BOOL CInverter::OnIPStop(CIPCommand& cmd)
{
	const CIPVar&		var			= cmd.m_var;
	CIPVar&				varResult	= cmd.m_varResult;
	
	cmd.Log(__TFUNCTION__ _T("[Step = %d][Error = %d]"), cmd.GetCurrentStep(), cmd.IsOK() ? FALSE : TRUE);
	enum {
		S_INIT,
		S_STOP,
		S_STOP_CHECK,
		S_END,
	};

	int _eModule = var("eMODULEs", eMODULE_NONE);
	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_DOWN;
		return FALSE;
	}

	if (!m_xcomm.IsOpen()) {
		cmd.ReserveErrorStep();
		return TRUE;
	}

	int iTimeout = 3 * 1000;
	BOOL bCheck = TRUE;
	switch (cmd.GetCurrentStep()) {
		case S_INIT : {
			sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_RUN;
			cmd.EnqueueNextStep();
			break;
		}
		case S_STOP: {
			cmd.m_dwCookie = GetTickCount64();
			AddCommand(eIC_W_STOP);
			cmd.EnqueueNextStep();
			break;
		}
		case S_STOP_CHECK: {
			bCheck = m_eRecvType[eIC_W_STOP] == eRECV_OK;
			bCheck &= Serial::Inverter::GetInverterStop((eMODULEs)_eModule);;

			if (bCheck) {
				cmd.EnqueueNextStep();
			}
			else {
				if (GetTickCount64() - cmd.m_dwCookie < iTimeout) {
					if (m_eRecvType[eIC_W_STOP] == eRECV_NG)
						cmd.ReserveErrorStep();
					else
						cmd.EnqueueNextStepCurrent();
				}
				else {
					cmd.ReserveErrorStep();
				}
			}
			break;
		}
		case S_END : {
			sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_COMPLETED;
			break;
		}
	}

	return TRUE;
}

BOOL CInverter::OnIPSetFrequency(CIPCommand& cmd)
{
	const CIPVar&		var			= cmd.m_var;
	CIPVar&				varResult	= cmd.m_varResult;
	
	cmd.Log(__TFUNCTION__ _T("[Step = %d][Error = %d]"), cmd.GetCurrentStep(), cmd.IsOK() ? FALSE : TRUE);
	enum {
		S_INIT,
		S_SET_FREAUENCY,
		S_SET_FREAUENCY_CHECK,
		S_END,
	};

	int _eModule = var("eMODULEs", eMODULE_NONE);
	int iFrequency = var("Frequency", 3000);

	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_DOWN;
		return FALSE;
	}

	if (!m_xcomm.IsOpen()) {
		cmd.ReserveErrorStep();
		return TRUE;
	}

	int iTimeout = 3 * 1000;
	BOOL bCheck = TRUE;
	switch (cmd.GetCurrentStep()) {
		case S_INIT : {
			sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_RUN;
			cmd.EnqueueNextStep();
			break;
		}
		case S_SET_FREAUENCY: {
			cmd.m_dwCookie = GetTickCount64();
			AddCommand(eIC_W_FREQUENCY, iFrequency);
			cmd.EnqueueNextStep();
			break;
		}
		case S_SET_FREAUENCY_CHECK: {
			bCheck = m_eRecvType[eIC_W_FREQUENCY] == eRECV_OK;
			bCheck &= Serial::Inverter::GetInverterFrequency((eMODULEs)_eModule) == iFrequency;

			if (bCheck) {
				cmd.EnqueueNextStep();
			}
			else {
				if (GetTickCount64() - cmd.m_dwCookie < iTimeout) {
					if (m_eRecvType[eIC_W_FREQUENCY] == eRECV_NG)
						cmd.ReserveErrorStep();
					else
						cmd.EnqueueNextStepCurrent();
				}
				else {
					cmd.ReserveErrorStep();
				}
			}
			break;
		}
		case S_END : {
			sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_COMPLETED;
			break;
		}
	}

	return TRUE;
}


BOOL CInverter::OnIPSetting(CIPCommand& cmd)
{
	const CIPVar&		var			= cmd.m_var;
	CIPVar&				varResult	= cmd.m_varResult;
	
	cmd.Log(__TFUNCTION__ _T("[Step = %d][Error = %d]"), cmd.GetCurrentStep(), cmd.IsOK() ? FALSE : TRUE);
	enum {
		S_INIT,
		S_SET_FREAUENCY,
		S_SET_FREAUENCY_CHECK,
		S_END,
	};

	int _eModule = var("eMODULEs", eMODULE_NONE);

	int iACC1 = var("ACC1", 3000);
	int iDEC1 = var("DEC1", 3000);
	int iACC2 = var("ACC2", 3000);
	int iDEC2 = var("DEC2", 3000);

	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_DOWN;
		return FALSE;
	}

	if (!m_xcomm.IsOpen()) {
		cmd.ReserveErrorStep();
		return TRUE;
	}

	int iTimeout = 3 * 1000;
	BOOL bCheck = TRUE;
	switch (cmd.GetCurrentStep()) {
		case S_INIT : {
			sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_RUN;
			cmd.EnqueueNextStep();
			break;
		}
		case S_SET_FREAUENCY: {
			cmd.m_dwCookie = GetTickCount64();
			AddCommand(eIC_W_ACC1, iACC1);
			AddCommand(eIC_W_DEC1, iDEC1);
			AddCommand(eIC_W_ACC2, iACC2);
			AddCommand(eIC_W_DEC2, iDEC2);
			cmd.EnqueueNextStep();
			break;
		}
		case S_SET_FREAUENCY_CHECK: {
			bCheck = m_eRecvType[eIC_W_ACC1] == eRECV_OK;
			bCheck &= m_eRecvType[eIC_W_DEC1] == eRECV_OK;
			bCheck &= m_eRecvType[eIC_W_ACC2] == eRECV_OK;
			bCheck &= m_eRecvType[eIC_W_DEC2] == eRECV_OK;

			bCheck &= Serial::Inverter::GetInverteriAccTime1((eMODULEs)_eModule) == iACC1;
			bCheck &= Serial::Inverter::GetInverteriAccTime2((eMODULEs)_eModule) == iACC2;
			bCheck &= Serial::Inverter::GetInverteriDecTime1((eMODULEs)_eModule) == iDEC1;
			bCheck &= Serial::Inverter::GetInverteriDecTime2((eMODULEs)_eModule) == iDEC2;

			if (bCheck) {
				cmd.EnqueueNextStep();
			}
			else {
				if (GetTickCount64() - cmd.m_dwCookie < iTimeout) {
					bCheck = m_eRecvType[eIC_W_ACC1] == eRECV_NG;
					bCheck |= m_eRecvType[eIC_W_DEC1] == eRECV_NG;
					bCheck |= m_eRecvType[eIC_W_ACC2] == eRECV_NG;
					bCheck |= m_eRecvType[eIC_W_DEC2] == eRECV_NG;

					if (bCheck)
						cmd.ReserveErrorStep();
					else
						cmd.EnqueueNextStepCurrent();
				}
				else {
					cmd.ReserveErrorStep();
				}
			}
			break;
		}
		case S_END : {
			sm.Module[_eModule].Status.eStatus = eMODULE_STATUS_COMPLETED;
			break;
		}
	}

	return TRUE;
}


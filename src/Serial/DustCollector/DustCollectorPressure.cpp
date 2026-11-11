#include "pch.h"
#include "DustCollectorPressure.h"

#define INTERVAL_REDRAW		4350

CDustCollectorPressure::CDustCollectorPressure(int iIndex) : TIPCommandTarget<CFrameWnd>(&g_ipClient)
{
	m_eIndex = iIndex;
	SetUnitName(Format("DustCollectorPressure_%d", iIndex + 1));
	// 다른 곳에서 안되면 국번 변경필요 (iSlaveAddress)
	m_command_list = {
		{ eCMD_GET_PRESSURE, 0x01, 0x03, 0x0000, 0x0001, 7 },
	};
}

CDustCollectorPressure::~CDustCollectorPressure()
{
	bThreadStop = TRUE;
	if (m_thread) {
		m_thread->join();
		m_thread.reset();
	}

	m_xcomm.DeleteBuffer();
	m_xcomm.Close();
}

IMPLEMENT_DYNAMIC(CDustCollectorPressure, CFrameWnd)

BEGIN_MESSAGE_MAP(CDustCollectorPressure, CFrameWnd)
END_MESSAGE_MAP()

BEGIN_IP_COMMAND_TABLE(CDustCollectorPressure, IIPCommandTarget)
END_IP_COMMAND_TABLE()

void CDustCollectorPressure::Initialize()
{
	CProfile profile(_T("../cfg/Serial/DustCollectorPressure.ini"));
	CProfileSection section = profile.GetSection(_T("CommPort"));

	m_com_info.iPortNo = section.GetItemValue(_T("PortNo"), 46);
	m_com_info.iBaud = section.GetItemValue(_T("Baud"), 9600);
	m_com_info.iDataBits = section.GetItemValue(_T("DataBits"), 8);
	m_com_info.iStopBitsIndex = section.GetItemValue(_T("StopBitsIndex"), 0);
	m_com_info.iParity = section.GetItemValue(_T("Parity"), 0);
	m_eSlave = section.GetItemValue(_T("Slave"), 1);

	Connect();
	m_thread = std::make_unique<std::thread>(std::bind(&CDustCollectorPressure::Run, this));
}

BOOL CDustCollectorPressure::Connect()
{
	if (m_xcomm.Open(m_com_info.iPortNo, m_com_info.iBaud, TRUE, m_com_info.iDataBits, m_com_info.iParity, m_com_info.iStopBitsIndex, RTS_CONTROL_DISABLE, DTR_CONTROL_DISABLE))
	{
		sm.SerialConnection->bDustCollectorPressure[m_eIndex] = TRUE;
	}
	else
	{
		sm.SerialConnection->bDustCollectorPressure[m_eIndex] = FALSE;
		return FALSE;
	}

	return TRUE;
}

std::pair<int, std::vector<BYTE>> CDustCollectorPressure::MakeCommandGetPressure()
{
	std::vector<BYTE> data;
	data.reserve(100);
	
	data.push_back(m_eSlave);
	data.push_back(m_command_list[eCMD_GET_PRESSURE].iFuncCode);
	data.push_back(HIBYTE(m_command_list[eCMD_GET_PRESSURE].iStartAddress));
	data.push_back(LOBYTE(m_command_list[eCMD_GET_PRESSURE].iStartAddress));
	data.push_back(HIBYTE(m_command_list[eCMD_GET_PRESSURE].iDataCount));
	data.push_back(LOBYTE(m_command_list[eCMD_GET_PRESSURE].iDataCount));

	auto crc = CRC16(&data[0], (int)data.size());
	data.push_back(LOBYTE(crc));
	data.push_back(HIBYTE(crc));

	m_bCommandComplete[eCMD_GET_PRESSURE].store(eCOMAMND_STATUS_NONE);
	return { eCMD_GET_PRESSURE , data };
}

BOOL CDustCollectorPressure::ReadRecvCommand(int eCommand)
{
	auto fn_delete_buffer = [&]() {
		m_xcomm.DeleteBuffer();
		m_bCommandComplete[eCommand].store(eCOMMAND_STATUS_OK);
	};

	BYTE ch;
	BOOL bSlave = FALSE;
	BOOL bCmdType = FALSE;
	int iCount = 0;
	while (m_xcomm.Read(&ch, 1, 3000) == 1) {
		iCount++;

		if (ch == m_eSlave) { // 국번이 나왔는지 매칭
			bSlave = TRUE;
		}
		if (ch == m_command_list[eCommand].iFuncCode) // 커맨드 타입이 맞는지
			bCmdType = TRUE;

		if (bSlave && bCmdType) { // 둘다 찾았으면 그만 읽는다
			break;
		}

		// 실제로 보내보면 데이터가 지 맘대로 들어올때가 있음. 3번 읽었는데도 국번 안들어오면 버퍼 지우고 다시 읽는다
		if (iCount >= 3 && !bSlave) { 
			fn_delete_buffer();
			return FALSE;
		}
	}

	if (!m_xcomm.Read(&ch, 1, 3000)) { // 국번과 커맨드 타입이 나왔으면 그다음은 데이터 사이즈다
		fn_delete_buffer();
		return FALSE;
	}

	int iDataSize = ch; // 데이터 사이즈
	std::vector<BYTE> data;
	data.resize(iDataSize + 2); // data + crc(2byte)

	if (!m_xcomm.Read(&data[0], data.size(), 3000)) { // CRC까지 다 읽어 들여서 버퍼를 비운다
		fn_delete_buffer();
		return FALSE;
	}
	m_xcomm.DeleteBuffer(); // 근데 쓰레기 값이 있는 관계로 안전하게 다 비운다

	sm.DustCollector->data[m_eIndex].iPressure = MAKEWORD(data[1], data[0]);
	m_bCommandComplete[eCommand].store(eCOMMAND_STATUS_OK);
	return TRUE;
}

void CDustCollectorPressure::Run()
{
	while (!bThreadStop) {

		if (!m_xcomm.IsOpen()) {
			sm.SerialConnection->bDustCollectorPressure[m_eIndex] = FALSE;
			Connect();
			std::this_thread::sleep_for(100ms);
			continue;
		}

		auto SendCommand = [&](int step, const std::vector<BYTE>& data) {
			m_xcomm.DeleteBuffer();
			m_xcomm.Purge();
			if (!m_xcomm.Write(&data[0], data.size(), 1000)) {
				m_bCommandComplete[step].store(eCOMMAND_STATUS_NG);
				return FALSE;
			}

			return TRUE;
		};

		{ // 읽기만
			auto [command_step, command] = MakeCommandGetPressure();
			if (SendCommand(command_step, command))
				ReadRecvCommand(command_step);
		}

		std::this_thread::sleep_for(50ms);
	}
}

WORD CDustCollectorPressure::CRC16(BYTE* nData, int wLength)
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
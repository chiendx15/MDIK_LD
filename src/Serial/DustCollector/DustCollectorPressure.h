#pragma once

class CDustCollectorPressure : public TIPCommandTarget<CFrameWnd>
{
	enum ID_TIMER {
		ID_TIMER_UPDATE_CONNECT = 0x1234
	};
	DECLARE_DYNAMIC(CDustCollectorPressure)

	enum eCOMMAND {
		eCMD_GET_PRESSURE,
		nCMD,
	};

	enum {
		eCOMAMND_STATUS_NONE,
		eCOMMAND_STATUS_OK,
		eCOMMAND_STATUS_NG,
	};

public:
	CDustCollectorPressure(int iIndex);
	virtual ~CDustCollectorPressure();

	void Initialize();

public:

	typedef struct {
		int iCommandStep;
		int iSlaveAddress;
		int iFuncCode;
		int iStartAddress;
		int iDataCount;
		int iRecvCount;
	} T_COMMAND;

	typedef struct T_COM_PORT_INFO {
		int iPortNo;
		int iBaud;
		int iDataBits;
		int iStopBitsIndex;
		int iParity;
	} T_COM_PORT_INFO;

protected:
	int m_eIndex = 0;
	int m_eSlave = 0;
	std::vector<T_COMMAND> m_command_list;

	CXComm m_xcomm;
	T_COM_PORT_INFO m_com_info;
	int m_eCommandStep = 0;

	BOOL bThreadStop = FALSE;
	std::unique_ptr<std::thread> m_thread;
	std::atomic<int> m_bCommandComplete[nCMD];

protected:
	BOOL Connect();
	void Run();

	std::pair<int, std::vector<BYTE>> MakeCommandGetPressure();
	BOOL ReadRecvCommand(int eCommand);

	WORD CRC16(BYTE* nData, int wLength);

protected:

	DECLARE_IP()
	DECLARE_MESSAGE_MAP()
};


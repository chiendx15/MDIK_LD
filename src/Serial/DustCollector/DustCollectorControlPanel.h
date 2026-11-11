#pragma once

class CDustCollectorControlPanel : public TIPCommandTarget<CFrameWnd>
{
	enum ID_TIMER {
		ID_TIMER_UPDATE_CONNECT = 0x1234
	};
	DECLARE_DYNAMIC(CDustCollectorControlPanel)

	typedef enum {
		eREAD_COIL	= 0x01,
		eWRITE_COIL = 0x05,
		eREAD_REG	= 0x04,
		eWRITE_REG	= 0x10,
	} eCMD_TYPE;

	enum eCOMMAND {
		eCMD_SET_FILTER_TIMER,
		eCMD_SET_FILTER_TIMER_RESET,
		eCMD_SET_INVERTER_ON_OFF,
		eCMD_GET_PRESSURE,
		eCMD_GET_INVERTER_OUTPUT,
		eCMD_GET_ALARM_HISTORY,
		eCMD_GET_RUNNING_STATUS,
		eCMD_GET_ALARM_CODE,
		eCMD_GET_RUNNING_TIME,
		nCMD,
	};

	enum {
		eCOMAMND_STATUS_NONE,
		eCOMMAND_STATUS_OK,
		eCOMMAND_STATUS_NG,
	};

public:
	CDustCollectorControlPanel(int iIndex);
	virtual ~CDustCollectorControlPanel();

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
	int m_eSlave = 1;
	std::vector<T_COMMAND> m_command_list;

	CXComm m_xcomm;
	T_COM_PORT_INFO m_com_info;
	int m_eCommandStep = 0;

	BOOL bThreadStop = FALSE;
	std::unique_ptr<std::thread> m_thread;
	std::mutex m_mutex;
	std::queue<std::pair<int, std::vector<BYTE>>> m_commands;
	std::atomic<int> m_bCommandComplete[nCMD];

protected:
	BOOL Connect();
	void Run();

	void SetFilterTimer(WORD wValue);
	void SetFilterTimerReset();
	void SetInverterOnOff(WORD wValue);

	std::pair<int, std::vector<BYTE>> MakeSetCommand(int iCommandStep, WORD wValue);
	std::pair<int, std::vector<BYTE>> MakeGetCommand(int iCommandStep);
	BOOL ReadWriteCommand(int eCommand);
	BOOL ReadRecvCommand(int eCommand);

	WORD CRC16(BYTE* nData, int wLength);

protected:

	DECLARE_IP()
	ip_handler BOOL OnIP_SetFilterTimer(CIPCommand& cmd);
	DECLARE_MESSAGE_MAP()
};


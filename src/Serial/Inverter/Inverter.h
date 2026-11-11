
#pragma once
#include "pch.h"
#include "framework.h"
#include "misc/XComm.h"
#include <bitset>
#include <thread>
#include <concurrent_vector.h>
	
typedef enum {
	// Func Code로 사용.
	eREAD_COIL = 0x01,
	eWRITE_COIL = 0x05,
	eREAD_REG = 0x03,
	eWRITE_REG = 0x10,
} eCMD_TYPE;

typedef enum {
	eRECV_START,
	eRECV_OK,
	eRECV_NG,
} eRECV_TYPE;

typedef struct {
	int			iCommandStep;
	eCMD_TYPE	eCmdType; // == Functioncode

	int			iSlaveAddress;
	int			iStartAddress;
	int			iDataOrCount; // 코일수, 쓰기데이터, 레지스터 수

	int			iRecvCount;
} T_COMMAND;

typedef struct {
	int iCommand = 0;
	int iValue = 0;
} T_COMMAND_DATA;


class CInverter : public TIPCT<CFrameWnd>
{
	DECLARE_DYNCREATE(CInverter)
public:
	CInverter(CWnd* pwndParent = NULL, int iInverter = 0);           // protected constructor used by dynamic creation
	virtual ~CInverter();
	CXComm					m_xcomm;
	//-->24.03.15 KGH ADD
	BOOL					Initialize();
	CWnd*					m_pwndParent;

	int						m_iInverter = 0;

	T_COMMAND_DATA			m_tCommandCurrent;
	//Concurrency::concurrent_vector<T_COMMAND_DATA> m_setCommand;
	std::vector<T_COMMAND_DATA> m_setCommand;
	eRECV_TYPE					m_eRecvType[nINVERTER_CMD];

	std::thread	m_ThreadReader;
protected:

	DECLARE_MESSAGE_MAP()
	afx_msg int				OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	DECLARE_IP()
	ip_handler	BOOL	OnIPInitialize(CIPCommand& cmd);
	ip_handler	BOOL	OnIPRun(CIPCommand& cmd);
	ip_handler	BOOL	OnIPStop(CIPCommand& cmd);
	ip_handler	BOOL	OnIPSetFrequency(CIPCommand& cmd);
	ip_handler	BOOL	OnIPSetting(CIPCommand& cmd);

protected:
	void ReaderT();
	BOOL PortOpenClose(BOOL _bOpen);
	void AddCommand(int iCommand, int iRegisterData = 0);
	BOOL SendRecvCommand(int iCommand, int iRegisterData = 0);
	void DataParsing(int iCommand, BYTE* nData, int iSize);

	WORD CRC16(BYTE* nData, int wLength);
	void MakeCRC16(BYTE* Bytes, int& iSize);
};



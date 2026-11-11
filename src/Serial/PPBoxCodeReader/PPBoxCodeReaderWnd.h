#pragma once

#include "PPBoxCodeReaderProtocol.h"

#define RETRY_COUNT 3

class CPPBoxCodeReaderWnd : public TIPCommandTarget<CFrameWnd>
{
	enum ID_TIMER{
		ID_TIMER_UPDATE_STATE = 0x1234
	};

	DECLARE_DYNAMIC(CPPBoxCodeReaderWnd)

	//. RS232 사용하는 class는 공통으로 사용하고 싶은데...
	typedef struct {
		int iPortNo;
		int iBaud;
		int iDataBits;
		int iStopBitsIndex;
		int iParity;
		int iMaxValue;
		int nCh;
	} T_RS232_PORT;

	typedef struct {
		CString strIP;
		int iPort;
	} T_SOCKET_PORT;

	enum eS {
		eS_FULL_AREA
		, eS_TEXT_COM_PORT
		, eS_TEXT_ETHERNET_IP
		, eS_TEXT_ETHERNET_PORT
		, eS_RECV_MSG
		, eS_RESULT_CODE
		, eS_END
	};

	enum eC {
		eC_NONE = -1
		, eC_EDIT_COM_PORT
		, eC_EDIT_ETHERNET_IP
		, eC_EDIT_ETHERNET_PORT
		, eC_BTN_CONNECT
		, eC_STATE_CONNECTION
		, eC_BTN_READ_CODE

		, eC_END
	};
public:
	CPPBoxCodeReaderWnd();
	virtual ~CPPBoxCodeReaderWnd();

public:
	BOOL Initialize();
	BOOL RequestConnect();

	/// <summary>
	/// 통신 방법을 RS232로 할지, Ethernet으로 할지.
	/// </summary>
	PPBoxCodeReader::eCONNECTION_OPTION m_eConnectionOption;
	BOOL m_bIsConnected;

	BOOL ChangeLanguage();
protected:
	void ComputeArea();

	BOOL StartPacketThread();
	BOOL StopPacketThread();

	void PacketWorkerT();
	BOOL Connect();
	BOOL IsValidConnection();
	BOOL WaitSendCommandEvent();


	//<< Command 처리 부분
	BOOL	SendCommand(CStringA& strCommand);
	BOOL	RecvCommand();
	BOOL	ParseRecvCommand(char* data, int size);
	BOOL	ParseRecvErrorCommand(char* data, int size);
	//>>
	
	//. TCP 통신인 경우
	BOOL OpenSocket();
	BOOL SendTo(LPCSTR lpBuffer);

	DECLARE_IP()
	ip_handler	BOOL	OnIPInitialize(CIPCommand& cmd);
	ip_handler	BOOL	OnIPReadCode(CIPCommand& cmd);
	ip_handler	BOOL	OnIPReadOFF(CIPCommand& cmd);
	
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	CRect m_rectClient, m_rectS[eS_END], m_rectC[eC_END];
	int m_iSelectedControl;

	//. serial 통신인 경우
	T_RS232_PORT m_tPort;
	CXComm	m_xcomm;

	//. ethernet 통신인 경우.
	CSocket2 m_socket;
	T_SOCKET_PORT m_tSocket;

	std::unique_ptr<std::thread> m_threadPacketWorker;

	CEvent m_evtRequestSendCommand;
	CEvent m_evtRequestCompleted;

	//. 프로그램 종료시에 true로 활성화
	std::atomic<bool> m_bIsStopThread;

	//. 메세지 분석용
	CString m_strReceivedMsg;
	CString m_strSentMsg;

	//. 결과
	CString m_strResultReadCode;

	CStringW m_strCommandEdit;
	uint64	m_ullSendCmdTick;

	std::atomic<bool> m_bReceived;
	
	CCriticalSection m_csReceived;

	static const int MAX_BUFFER_SIZE = 512;

	// KES 추가 2024.3.11
	int m_iRetry = 0;
};

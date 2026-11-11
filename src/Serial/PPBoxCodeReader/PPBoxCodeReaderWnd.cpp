#include "pch.h"
#include "PPBoxCodeReaderWnd.h"

CPPBoxCodeReaderWnd::CPPBoxCodeReaderWnd() 
	: TIPCommandTarget<CFrameWnd>(&g_ipClient)
	//. 이건 입력을 받을지 말지... 해당 프로젝트는 일단 ethernet 통신
	, m_eConnectionOption(PPBoxCodeReader::eCONNECTION_OPTION::eETHERNET)
	, m_tPort{}
	, m_tSocket{}
{
	m_bIsConnected = FALSE;
	m_xcomm.DeleteBuffer();
}

CPPBoxCodeReaderWnd::~CPPBoxCodeReaderWnd() {
	
	StopPacketThread();
}

BEGIN_MESSAGE_MAP(CPPBoxCodeReaderWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CPPBoxCodeReaderWnd, CFrameWnd)

BEGIN_IP_COMMAND_TABLE(CPPBoxCodeReaderWnd, IIPCommandTarget)
	ON_IP_EXCL("PPBoxCodeReader", "Initialize", OnIPInitialize)
	ON_IP_EXCL("PPBoxCodeReader", "ReadCode", OnIPReadCode)
	ON_IP_EXCL("PPBoxCodeReader", "ReadOff", OnIPReadOFF)
END_IP_COMMAND_TABLE()

static CString GetLastErrorString(DWORD dwError)
{
	LPVOID lpMsgBuf;

	/*
	*
	* 함수 원형.
	DWORD FormatMessage(
		DWORD dwFlags,
		LPCVOID lpSource,
		DWORD dwMessageId,
		DWORD dwLanguageId,
		LPTSTR lpBuffer,
		DWORD nSize,
		va_list *Arguments
	)
	*/

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_IGNORE_INSERTS
		, NULL
		, dwError
		, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
		, (LPTSTR)&lpMsgBuf
		, 0
		, NULL
	);

	CString str;
	str.Format(_T("%s"), lpMsgBuf);

	LocalFree(lpMsgBuf);
	return str;
}

int CPPBoxCodeReaderWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ModifyStyle(0, WS_SYSMENU);
	SetWindowText(_T("PPBoxCodeReader"));

	return 0;
}

void CPPBoxCodeReaderWnd::OnDestroy() {
	KillTimer(ID_TIMER_UPDATE_STATE);
	CFrameWnd::OnDestroy();
}

void CPPBoxCodeReaderWnd::OnClose() {
	ShowWindow(SW_HIDE);
}

BOOL CPPBoxCodeReaderWnd::OnEraseBkgnd(CDC* pDC) {
	GetClientRect(m_rectClient);
	return TRUE;
}

BOOL CPPBoxCodeReaderWnd::Initialize() {
	PostIPCommand("PPBoxCodeReader", "Initialize");

	//m_pwndPrint->Initialize();

	SetTimer(ID_TIMER_UPDATE_STATE, 300, NULL);

	return TRUE;
}

void CPPBoxCodeReaderWnd::ComputeArea() {
	GetClientRect(m_rectClient);
	CRect rc = m_rectClient;

	double dW = (rc.Width() - 5.);
	double dH = (rc.Height() - 5.);

	m_rectS[eS_FULL_AREA]                = CRect(rc.left + dW * 0 + 5, rc.top + dH * 0 + 5, rc.left + dW * 1 , rc.top + dH * 1 );

	rc = m_rectS[eS_FULL_AREA];

	dW = (rc.Width() - 5.) / 7;
	dH = (rc.Height() - 5.) / 7;

	if (m_eConnectionOption == PPBoxCodeReader::eCONNECTION_OPTION::eETHERNET) {
		m_rectS[eS_TEXT_ETHERNET_IP]         = CRect(rc.left + dW * 0 + 5, rc.top + dH * 0 + 5, rc.left + dW * 1, rc.top + dH * 1);
		m_rectC[eC_EDIT_ETHERNET_IP]         = CRect(rc.left + dW * 1 + 5, rc.top + dH * 0 + 5, rc.left + dW * 2, rc.top + dH * 1);
		m_rectS[eS_TEXT_ETHERNET_PORT]       = CRect(rc.left + dW * 0 + 5, rc.top + dH * 1 + 5, rc.left + dW * 1, rc.top + dH * 2);
		m_rectC[eC_EDIT_ETHERNET_PORT]       = CRect(rc.left + dW * 1 + 5, rc.top + dH * 1 + 5, rc.left + dW * 2, rc.top + dH * 2);
	}
	else if (m_eConnectionOption == PPBoxCodeReader::eCONNECTION_OPTION::eRS232C) {
		m_rectS[eS_TEXT_COM_PORT]            = CRect(rc.left + dW * 0 + 5, rc.top + dH * 0 + 5, rc.left + dW * 1, rc.top + dH * 1);
		m_rectC[eC_EDIT_COM_PORT]            = CRect(rc.left + dW * 1 + 5, rc.top + dH * 0 + 5, rc.left + dW * 2, rc.top + dH * 1);
	}

	m_rectC[eC_BTN_CONNECT]			     = CRect(rc.left + dW * 2 + 5, rc.top + dH * 0 + 5, rc.left + dW * 3, rc.top + dH * 1);
	m_rectC[eC_STATE_CONNECTION]		 = CRect(rc.left + dW * 3 + 5, rc.top + dH * 0 + 5, rc.left + dW * 4, rc.top + dH * 1);
	m_rectC[eC_BTN_READ_CODE]            = CRect(rc.left + dW * 4 + 5, rc.top + dH * 0 + 5, rc.left + dW * 5, rc.top + dH * 1);
	m_rectS[eS_RECV_MSG]				 = CRect(rc.left + dW * 5 + 5, rc.top + dH * 0 + 5, rc.left + dW * 7, rc.top + dH * 1);
	m_rectS[eS_RESULT_CODE]				 = CRect(rc.left + dW * 5 + 5, rc.top + dH * 1 + 5, rc.left + dW * 7, rc.top + dH * 2);

}

void CPPBoxCodeReaderWnd::OnPaint() {
	CPaintDC			dc(this);

	CMemDCEx::CMemDC			memDC(&dc);
	CBrush                      brush(RGB_GRAY_LIGHT);
	CPen						pen(PS_SOLID, 1, RGB(120, 120, 120));
	CRect						rect(m_rectClient);
	
	memDC.SelectObject(&brush);
	memDC.FillRect(m_rectClient, &brush);

	memDC.SetBkMode(TRANSPARENT);
	memDC.SetStretchBltMode(HALFTONE);

	DrawLineRect(memDC, m_rectClient);
	DrawLineRect(memDC, m_rectS[eS_FULL_AREA]);
	
	if (m_eConnectionOption == PPBoxCodeReader::eCONNECTION_OPTION::eETHERNET) {
		SelectedDrawText(memDC, m_rectS[eS_TEXT_ETHERNET_IP], _T("IP"));
		SelectedDrawEditText(memDC, m_rectC[eC_EDIT_ETHERNET_IP], m_tSocket.strIP, FALSE, 9, DT_RIGHT | DT_VCENTER);
		SelectedDrawText(memDC, m_rectS[eS_TEXT_ETHERNET_PORT], _T("Port"));
		SelectedDrawEditInt(memDC, m_rectC[eC_EDIT_ETHERNET_PORT], m_tSocket.iPort, FALSE, 9, DT_RIGHT | DT_VCENTER);
	}
	else if (m_eConnectionOption == PPBoxCodeReader::eCONNECTION_OPTION::eRS232C) {
		SelectedDrawText(memDC, m_rectS[eS_TEXT_COM_PORT], _T("Com Port"));
		SelectedDrawEditInt(memDC, m_rectC[eC_EDIT_COM_PORT], m_tPort.iPortNo, FALSE);
	}

	DrawRoundSensor(memDC, m_rectC[eC_STATE_CONNECTION], sm.SerialConnection->bPPBoxCodeReader ? _T("Connected") : _T("Disconnected"), sm.SerialConnection->bPPBoxCodeReader);

	DrawRoundButtonUpDownSensor(memDC, m_rectC[eC_BTN_CONNECT], sm.SerialConnection->bPPBoxCodeReader ? _T("Disconnect") : _T("TryConnect"), m_iSelectedControl == eC_BTN_CONNECT, sm.SerialConnection->bPPBoxCodeReader);
	DrawRoundButtonUpDownSensor(memDC, m_rectC[eC_BTN_READ_CODE], _T("ReadCode"), m_iSelectedControl == eC_BTN_READ_CODE);

	SelectedDrawText(memDC, m_rectS[eS_RECV_MSG], m_strReceivedMsg);
	SelectedDrawText(memDC, m_rectS[eS_RESULT_CODE], m_strResultReadCode);
}

void CPPBoxCodeReaderWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWnd::OnShowWindow(bShow, nStatus);
	if (bShow) {

		CRect rect = g_rectArea[eAREA_WORK];
		
		double iW = rect.Width() / 2;
		double iH = rect.Height() / 2;
		double iX = (g_rectArea[eAREA_FULL].Width() - iW) / 2.;
		double iY = (g_rectArea[eAREA_FULL].Height() - iH) / 2.;

		SetWindowPos(&wndTop, iX, iY, iW, iH, SWP_SHOWWINDOW);

		ComputeArea();
		Invalidate(TRUE);

	}
	//m_pwndPrint->ShowWindow(bShow);
}

void CPPBoxCodeReaderWnd::OnMouseMove(UINT nFlags, CPoint point) {
	CFrameWnd::OnMouseMove(nFlags, point);
}

void CPPBoxCodeReaderWnd::OnLButtonDown(UINT nFlags, CPoint point) {
	SetFocus(); // Edit End
	for (int i = 0; i < eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_iSelectedControl = i;
			Invalidate(FALSE);
			break;
		}
	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CPPBoxCodeReaderWnd::OnTimer(UINT_PTR nIDEvent) {

	switch (nIDEvent) {
	case ID_TIMER_UPDATE_STATE: {

		break;
	}
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CPPBoxCodeReaderWnd::OnLButtonUp(UINT nFlags, CPoint point) {

	if ((m_iSelectedControl > eC_NONE) && (m_rectC[m_iSelectedControl].PtInRect(point))) {

		switch (m_iSelectedControl) {
			case eC_EDIT_COM_PORT: {
				if (m_eConnectionOption == PPBoxCodeReader::eCONNECTION_OPTION::eETHERNET) {
					break;
				}
				EditStart(this, m_rectC[eC_EDIT_COM_PORT], m_tPort.iPortNo);
				break;
			}
			case eC_BTN_CONNECT: {
				RequestConnect();
				break;
			}
			case eC_BTN_READ_CODE: {
				PostIPCommand("PPBoxCodeReader", "ReadCode");
				break;
			}
		}
	}

	m_iSelectedControl = eC_NONE;
	Invalidate(FALSE);
	CFrameWnd::OnLButtonUp(nFlags, point);
}

BOOL CPPBoxCodeReaderWnd::ChangeLanguage()
{
	//PPBoxCodeReader::lpszErrorMessage[0] = g_st[IDS_];
	//PPBoxCodeReader::lpszErrorMessage[1] = g_st[IDS_];
	//PPBoxCodeReader::lpszErrorMessage[2] = g_st[IDS_];
	//PPBoxCodeReader::lpszErrorMessage[3] = g_st[IDS_];
	//PPBoxCodeReader::lpszErrorMessage[4] = g_st[IDS_];
	//PPBoxCodeReader::lpszErrorMessage[5] = g_st[IDS_];
	//PPBoxCodeReader::lpszErrorMessage[6] = g_st[IDS_];
	
	return TRUE;
}

BOOL CPPBoxCodeReaderWnd::RequestConnect() {

	CProfile profile(_T("../cfg/Serial/PPBoxCodeReader.ini"));

	BOOL bIsRS232C = profile.GetProfileValue(_T("ConnectionOption"), _T("isRS232C"), FALSE);

	if (bIsRS232C) {
		m_eConnectionOption = PPBoxCodeReader::eCONNECTION_OPTION::eRS232C;
	}
	else {
		m_eConnectionOption = PPBoxCodeReader::eCONNECTION_OPTION::eETHERNET;
	}
	
	switch (m_eConnectionOption) {
		case PPBoxCodeReader::eCONNECTION_OPTION::eETHERNET: {
			CProfileSection section = profile.GetSection(_T("Ethernet"));

			m_tSocket.strIP = section.GetItemValue(_T("IP"), PPBoxCodeReader::lpszDefaultIP);
			m_tSocket.iPort = section.GetItemValue(_T("Port"), PPBoxCodeReader::iDefaultTCPPort);

			break;
		}
		case PPBoxCodeReader::eCONNECTION_OPTION::eRS232C: {

			CProfileSection section = profile.GetSection(_T("ComPort"));

			m_tPort.iPortNo			= section.GetItemValue(_T("PortNo")		, -1);		//PC 내 연결되어 있는 포트 테스트 후 확인.
			m_tPort.iBaud			= section.GetItemValue(_T("Baud")		, 115200);
			m_tPort.iDataBits		= section.GetItemValue(_T("DataBits")	, 8);
			m_tPort.iStopBitsIndex	= section.GetItemValue(_T("StopBits")	, 0);		//짝수
			m_tPort.iParity			= section.GetItemValue(_T("Parity")		, 0);		//짝수

			break;
		}
	}

	if (FALSE == StopPacketThread()) {
		g_ipClient.m_log.WriteErrorLog(Format(_T("[PPBoxCodeReader] Connect - StopPacketThread() %s."), g_st[IDS_PPBOX_CODE_READER_WND_FAIL]));
		return FALSE;
	}

	if (FALSE == StartPacketThread()) {
		g_ipClient.m_log.WriteErrorLog(Format(_T("[PPBoxCodeReader] Connect - StartPacketThread() %s."), g_st[IDS_PPBOX_CODE_READER_WND_FAIL]));
		return FALSE;
	}

	return TRUE;
}

BOOL CPPBoxCodeReaderWnd::OnIPInitialize(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;
	CIPVar& varResult = cmd.m_varResult;

	cmd.Log(__TFUNCTION__ _T("[Step = %d][Error = %d]"), cmd.GetCurrentStep(), cmd.IsOK() ? FALSE : TRUE);
	
	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		varResult.SetChildItem("OpenStatus", FALSE);
		return FALSE;
	}

	BOOL	bOpen = var("Open", TRUE);
	
	sm.SerialConnection->bPPBoxCodeReader = FALSE;
	
	if (!RequestConnect()) {
		varResult.SetChildItem("OpenStatus", FALSE);
		return FALSE;
	}

	return TRUE;
}

BOOL CPPBoxCodeReaderWnd::OnIPReadCode(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;
	CIPVar& varResult = cmd.m_varResult;

	enum
	{
		eINIT,
		eREQUEST_READ_ON,
		eWAIT_FOR_READ_ON,
		eEND
	};

	int eModule = var("eMODULEs", eMODULE_NONE);
	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		// Read 실패 했을 때 Off 하기.
		PostIPCommand("PPBoxCodeReader", "ReadOff");
		sm.Module[eModule].Status.eStatus = eMODULE_STATUS_DOWN;
		return FALSE;
	}


	switch (cmd.GetCurrentStep()) {
		case eINIT:	{
			sm.Module[eModule].Status.eStatus = eMODULE_STATUS_RUN;
			if (!sm.SerialConnection->bPPBoxCodeReader) {
				cmd.ReserveErrorStep();
				return TRUE;
			}

			m_strReceivedMsg = CString();
			m_strResultReadCode = CString();

			cmd.EnqueueNextStep();
			break;
		}
		case eREQUEST_READ_ON: {

			char packet[MAX_BUFFER_SIZE] = { 0 };
			packet[0] = 'L';
			packet[1] = 'O';
			packet[2] = 'N';
			packet[3] = eCR;

			m_strCommandEdit = packet;
			m_evtRequestSendCommand.SetEvent();

			cmd.m_varCookie.SetChildItem("RequestTime", GetTickCount64());
			cmd.EnqueueNextStep();
			break;
		}
		case eWAIT_FOR_READ_ON: {
			
			DWORD dwResult = WaitForSingleObjectEx(m_evtRequestCompleted.m_hObject, 100, FALSE);

			if (dwResult == WAIT_OBJECT_0)
			{
				m_evtRequestCompleted.ResetEvent();
				cmd.EnqueueNextStep();
			}
			else
			{
				unsigned long long ullRequestTime_ms = cmd.m_varCookie.GetChildItem("RequestTime", INFINITE);

				if (ullRequestTime_ms == INFINITE)
				{
					cmd.ReserveErrorStep();
					return TRUE;
				}
				unsigned long long ullElapsedTime_ms = GetTickCount64() - ullRequestTime_ms;
				unsigned long long ullLimitTime_ms = 5000;

				if (ullLimitTime_ms < ullElapsedTime_ms)
				{
					// KES 수정 2024.3.11 - Retry 3회 기능 추가
					if (m_iRetry++ >= RETRY_COUNT) {
						m_strResultReadCode = _T("Read Fail");
						//cmd.EnqueueNextStep(eEND);
						cmd.ReserveErrorStep();
						return TRUE;
					}
					else {
						cmd.EnqueueNextStep(eREQUEST_READ_ON);
						break;
					}
				}

				cmd.EnqueueNextStepCurrent(100);
			}
			break;
		}
		case eEND: {
			sm.Module[eModule].Status.eStatus = eMODULE_STATUS_COMPLETED;

			if (eModule != eMODULE_NONE) {
				// 임시로 szCstID 에 처리.
				CStringA strGlassID(m_strResultReadCode);
				strncpy_s(sm.Module[eModule].Glass.szCELL_ID_BCR, sizeof(sm.Module[eModule].Glass.szCELL_ID_BCR), strGlassID, sizeof(sm.Module[eModule].Glass.szCELL_ID_BCR) - 1);
			}

			cmd.m_varResult.SetChildItem("ReadData", m_strResultReadCode);

			break;
		}
	}
	
	return TRUE;
}



BOOL CPPBoxCodeReaderWnd::OnIPReadOFF(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;
	CIPVar& varResult = cmd.m_varResult;

	enum
	{
		eINIT,
		eREUQEST_READ_OFF,
		eWAIT_FOR_READ_OFF,
		eEND
	};

	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		return FALSE;
	}


	switch (cmd.GetCurrentStep()) {
		case eINIT:	{
			if (!sm.SerialConnection->bPPBoxCodeReader) {
				cmd.ReserveErrorStep();
				return TRUE;
			}

			cmd.EnqueueNextStep();
			break;
		}
		case eREUQEST_READ_OFF:	{ 
			char packet[MAX_BUFFER_SIZE] = { 0 };
			packet[0] = 'L';
			packet[1] = 'O';
			packet[2] = 'F';
			packet[3] = 'F';
			packet[4] = eCR;
		
			m_strCommandEdit = packet;
			m_evtRequestSendCommand.SetEvent();
		
			cmd.m_varCookie.SetChildItem("RequestTime", GetTickCount64());
			cmd.EnqueueNextStep();
			break;
		}
		case eWAIT_FOR_READ_OFF: {
		
			DWORD dwResult = WaitForSingleObjectEx(m_evtRequestCompleted.m_hObject, 100, FALSE);
		
			if (dwResult == WAIT_OBJECT_0)
			{
				m_evtRequestCompleted.ResetEvent();
				cmd.EnqueueNextStep();
			}
			else
			{
				unsigned long long ullRequestTime_ms = cmd.m_varCookie.GetChildItem("RequestTime", INFINITE);
		
				if (ullRequestTime_ms == INFINITE)
				{
					cmd.ReserveErrorStep();
					return FALSE;
				}
				unsigned long long ullElapsedTime_ms = GetTickCount64() - ullRequestTime_ms;
				unsigned long long ullLimitTime_ms = 5000;
		
				if (ullLimitTime_ms < ullElapsedTime_ms)
				{
					cmd.ReserveErrorStep();
					return FALSE;
				}
		
				cmd.EnqueueNextStepCurrent(100);
			}
			
			break;
		}
		case eEND: {
			break;
		}
	}
	
	return TRUE;
}

BOOL CPPBoxCodeReaderWnd::SendTo(LPCSTR lpBuffer) {
	
	if (m_socket.m_hSocket == INVALID_SOCKET)
		return FALSE;

	//int nPacket = strlen(lpBuffer) + 1;
	int nPacket = strlen(lpBuffer);

	// << Add By LBH (Buffer에 데이터가 남아 있는지 확인 하는 함수, 데이터가 남아 있을경우 Receive로 모두 받아 Buffer를 비운 뒤 실행)
	u_long lTemp = 0;
	ioctlsocket(m_socket, FIONREAD, &lTemp);
	if (lTemp > 0) {
		char data[MAX_BUFFER_SIZE] = { 0 };
		m_socket.Receive(data, lTemp);
	}
	// >>

	int nSend = m_socket.Send(lpBuffer, nPacket);

	if (nSend != nPacket) {

		DWORD dwLastError = WSAGetLastError();
		g_ipClient.m_log.WriteErrorLog(_T("[PPBoxCodeReader] Failed to Send(). err : %s"), GetLastErrorString(dwLastError));
		return FALSE;
	}

	return TRUE;
}

BOOL CPPBoxCodeReaderWnd::SendCommand(CStringA& strCommand) {

	//. 응답을 받아야만 true
	m_bReceived = false;
	
	m_strSentMsg = CString(strCommand);

	switch (m_eConnectionOption) {
		case PPBoxCodeReader::eCONNECTION_OPTION::eETHERNET: {
			if (!SendTo(strCommand)) {
				strCommand = "Write Error";
				sm.SerialConnection->bPPBoxCodeReader = FALSE;
				return FALSE;
			}
			break;
		}
		case PPBoxCodeReader::eCONNECTION_OPTION::eRS232C: {
			if (!m_xcomm.Write(strCommand, strCommand.GetLength(), 3000)) {
				strCommand = "Write Error";
				sm.SerialConnection->bPPBoxCodeReader = FALSE;
				return FALSE;
			}
			break;
		}
	}
	return TRUE;
}

BOOL CPPBoxCodeReaderWnd::ParseRecvErrorCommand(char* data, int size)
{
	char* pszOffsetBase = &data[3];
	char* pszOffsetCommand = strchr(pszOffsetBase, ',');
	char* pszOffsetErrorCode = strchr(pszOffsetCommand, eCR);

	//<< command parsing
	uint8_t nStringLengthCommand = pszOffsetCommand - pszOffsetBase;
	char cBufferCommand[MAX_BUFFER_SIZE] = { 0 };
	memcpy(cBufferCommand, pszOffsetBase, nStringLengthCommand);
	cBufferCommand[strlen(cBufferCommand)] = '\0';
	//>>

	//<< error code parsing
	uint8_t nStringLengthErrorCode = pszOffsetErrorCode - pszOffsetCommand;
	char cBufferErrorCode[MAX_BUFFER_SIZE] = { 0 };
	memcpy(cBufferErrorCode, pszOffsetCommand, nStringLengthErrorCode);
	cBufferErrorCode[strlen(cBufferErrorCode)] = '\0';
	//>>

	auto ParseErrorMessage = [](char* cBufferErrorCode) -> LPCTSTR
	{
#define MATCH_ERROR_CODE(first, second) ((cBufferErrorCode[0] == first) && (cBufferErrorCode[1] == second))

		if (MATCH_ERROR_CODE(0, 0))	return PPBoxCodeReader::lpszErrorMessage[0];
		if (MATCH_ERROR_CODE(0, 1))	return PPBoxCodeReader::lpszErrorMessage[1];
		if (MATCH_ERROR_CODE(0, 2))	return PPBoxCodeReader::lpszErrorMessage[2];
		if (MATCH_ERROR_CODE(0, 3))	return PPBoxCodeReader::lpszErrorMessage[3];
		if (MATCH_ERROR_CODE(0, 4))	return PPBoxCodeReader::lpszErrorMessage[4];
		if (MATCH_ERROR_CODE(0, 5))	return PPBoxCodeReader::lpszErrorMessage[5];
		if (MATCH_ERROR_CODE(1, 0))	return PPBoxCodeReader::lpszErrorMessage[6];
		if (MATCH_ERROR_CODE(1, 1))	return PPBoxCodeReader::lpszErrorMessage[7];
		if (MATCH_ERROR_CODE(1, 2))	return PPBoxCodeReader::lpszErrorMessage[8];
		if (MATCH_ERROR_CODE(1, 3))	return PPBoxCodeReader::lpszErrorMessage[9];
		if (MATCH_ERROR_CODE(1, 4))	return PPBoxCodeReader::lpszErrorMessage[10];
		if (MATCH_ERROR_CODE(2, 0))	return PPBoxCodeReader::lpszErrorMessage[11];
		if (MATCH_ERROR_CODE(2, 1))	return PPBoxCodeReader::lpszErrorMessage[12];
		if (MATCH_ERROR_CODE(2, 2))	return PPBoxCodeReader::lpszErrorMessage[13];
		if (MATCH_ERROR_CODE(2, 3))	return PPBoxCodeReader::lpszErrorMessage[14];
		if (MATCH_ERROR_CODE(9, 9))	return PPBoxCodeReader::lpszErrorMessage[15];

		return _T("");
	};

	//TODO. cBufferCommand, cBufferErrorCode를 어떻게 전달할지?
	LPCTSTR lpszErrorMessage = ParseErrorMessage(cBufferErrorCode);

	return TRUE;
}

BOOL CPPBoxCodeReaderWnd::ParseRecvCommand(char* data, int size)
{
	CString strBufferLog;
	//<< 에러 감지
	//
	// ERROR 회신 (| 는 버퍼에 담겨있지는 않고, 문서작성시 구분표기를 위해)
	// 헤더 | ER, | 커맨드명 | , | 에러 코드 | 터미네이터
	// 

	//. cfg로 사용여부 결정하자. 일단 기능만 만들어 둠.
	//. 수신 문자열을 읽을줄 알면, 굳이 parsing 할 필요는 없긴함.
	bool bUseErrorParsing = false;
	bool isError = true;
	isError &= (data[0] == 'E');
	isError &= (data[1] == 'R');
	isError &= (data[2] == ',');
	//>>

	if (isError)
	{
		if (bUseErrorParsing)
		{
			//ParseRecvErrorCommand(data, size);
		}
		//. recv 자체가 실패한건 아님...
		return FALSE;
	}
	else
	{
		//. iIndex만큼 메세지를 받았으면 수신 길이가 됨.
		//. 마지막 문자열이 0xCR 1 Byte이므로 수신길이 - 1 Byte면 판독 데이터가 됨.
		UINT nLength = size;

		//. CR을 null로 지우면 판독데이터만 남는다.
		data[nLength - 1] = '\0';
		//>>
	}

	//TODO recv 실패 감지를 어떻게 할까...
	m_bReceived = true;

	g_ipClient.m_log.WriteLog(_T("[PPBoxCodeReader][Recv] %s"), data);
	return TRUE;
}

BOOL CPPBoxCodeReaderWnd::IsValidConnection()
{
	switch (m_eConnectionOption) 
	{
		case PPBoxCodeReader::eCONNECTION_OPTION::eETHERNET:  return m_socket.m_hSocket != INVALID_SOCKET; break;
		case PPBoxCodeReader::eCONNECTION_OPTION::eRS232C:    return m_xcomm.IsOpen() && m_xcomm.IsOpened(); break;
	}

	return TRUE;
}

BOOL CPPBoxCodeReaderWnd::RecvCommand() {

	char data[MAX_BUFFER_SIZE] = { 0 };
	int nSize = 0;
	
	switch (m_eConnectionOption) {
		case PPBoxCodeReader::eCONNECTION_OPTION::eETHERNET: {
			//. 1. 기존 방법
			//BYTE data[1];
			//ZeroVar(data);
			//
			//if (!ReceiveFrom(data)) {
			//	break;
			//}

			//. 2. 전체 다 받기.

			// << Time Out 처리
			u_long lTemp = 0;
			ULONGLONG tick = GetTickCount64();
			while (GetTickCount64() - tick <= 3 * 1000) {
				ioctlsocket(m_socket, FIONREAD, &lTemp);

				if (lTemp > 0)
					break;
			}

			if (lTemp <= 0)
				return FALSE;
			// >>

			if (m_socket.Receive(data, MAX_BUFFER_SIZE) <= 0) {
				DWORD dwLastError = WSAGetLastError();
				g_ipClient.m_log.WriteErrorLog(_T("[PPBoxCodeReader] Failed to Receive(). err : %s"), GetLastErrorString(dwLastError));
				IPMessage(Format(_T("[PPBoxCodeReader] %s."), g_st[IDS_PPBOX_CODE_READER_WND_DATA_RECIEVE_FAIL]));
				break;
			}

			break;
		}
		case PPBoxCodeReader::eCONNECTION_OPTION::eRS232C: {
			BYTE ch = 0;
			DWORD dwTimeout = 3000;
			int iIndex = 0;

			while (m_xcomm.Read(&ch, 1, dwTimeout) == 1) {
				data[iIndex++] = ch;

				//. 반드시 설정 프로그램에서 CR로 지정할 것.
				if (ch == ASCII_CODE::eCR) {
					m_xcomm.DeleteBuffer();
					break;
				}

			}
			nSize = iIndex;
			break;
		}
	}

	{
		CS cs(&m_csReceived);
		CString strRecv(data);

		m_strReceivedMsg += strRecv;

		nSize = strlen(data);
		if (!ParseRecvCommand(data, nSize))
		{
			m_strReceivedMsg = _T("");
		}
		else {
			m_strResultReadCode = data;
		}
	}
	if (m_strResultReadCode.IsEmpty()) {
		return FALSE;
	}
	else {
		m_strCommandEdit.Empty();
		return TRUE;
	}
}

BOOL CPPBoxCodeReaderWnd::StartPacketThread() {

	//. 프로그램 종료시에 true로 활성화
	m_bIsStopThread = false;

	m_threadPacketWorker.reset(
		new std::thread(
			[this]() {
				PacketWorkerT();
			}
		)
	);
	return TRUE;
}

BOOL CPPBoxCodeReaderWnd::StopPacketThread() {

	m_bIsStopThread = true;

	switch (m_eConnectionOption) {
		case PPBoxCodeReader::eCONNECTION_OPTION::eETHERNET: {
			m_socket.Close();
			break;
		}
		case PPBoxCodeReader::eCONNECTION_OPTION::eRS232C: {
			m_xcomm.DeleteBuffer();
			m_xcomm.Close();
			break;
		}
	}

	if (m_threadPacketWorker && m_threadPacketWorker->joinable()) {
		m_threadPacketWorker->join();
	}

	g_ipClient.m_log.WriteLog(Format(_T("[PPBoxCodeReader] StopPacketThread() %s."), g_st[IDS_PPBOX_CODE_READER_WND_SUCCESS]));

	return TRUE;
}

BOOL CPPBoxCodeReaderWnd::Connect()
{
	//<< connect 시도
	switch (m_eConnectionOption) {
	case PPBoxCodeReader::eCONNECTION_OPTION::eETHERNET: {

		if (OpenSocket()) {
			g_ipClient.m_log.WriteLog(_T("[PPBoxCodeReader] connect to server. IP : %s, Port : %d"), m_tSocket.strIP, m_tSocket.iPort);
		}
		else {
			//. g_ipClient.m_log.WriteLog(_T("[PPBoxCodeReader] try to connect... IP : %s, Port : %d"), m_tSocket.strIP, m_tSocket.iPort);
			return FALSE;
		}

		break;
	}
	case PPBoxCodeReader::eCONNECTION_OPTION::eRS232C: {

		m_xcomm.DeleteBuffer();
		BOOL bResult = m_xcomm.Close();

		BOOL isSuccess = m_xcomm.Open(
			m_tPort.iPortNo
			, m_tPort.iBaud
			, TRUE
			, m_tPort.iDataBits
			, m_tPort.iParity
			, m_tPort.iStopBitsIndex
			, RTS_CONTROL_DISABLE
			, DTR_CONTROL_DISABLE
		);

		if (!isSuccess) {
			g_ipClient.m_log.WriteErrorLog(Format(_T("[PPBoxCodeReader] Connect - xComm Open() %s."),g_st[IDS_PPBOX_CODE_READER_WND_FAIL]));
			return FALSE;
		}

		break;
	}
	}
	

	return TRUE;
}

BOOL CPPBoxCodeReaderWnd::WaitSendCommandEvent() {

	//. wait for send event.
	DWORD dwWaitTime = 3000;
	DWORD dwResult = WaitForSingleObjectEx(m_evtRequestSendCommand.m_hObject, dwWaitTime, FALSE);

	BOOL isWaitFailed = FALSE;
	switch (dwResult) {
		case WAIT_FAILED:	 { isWaitFailed = TRUE;	break; }
		case WAIT_ABANDONED: { isWaitFailed = TRUE;	break; }
	}

	if (isWaitFailed) {
		return FALSE;
	}

	// KES 수정 2024.3.11
	if (dwResult == WAIT_OBJECT_0) {
		//. signaled.
		m_evtRequestSendCommand.ResetEvent();
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void CPPBoxCodeReaderWnd::PacketWorkerT() {
	m_socket.Detach();
	m_socket.ShutDown();

	if (!m_socket.Create()) {
		g_ipClient.m_log.WriteErrorLog(Format(_T("[PPBoxCodeReader] Socket create() %s"), g_st[IDS_PPBOX_CODE_READER_WND_FAIL]));
		return;
	}


	BOOL bTCPNoDelay = 1;
	m_socket.SetSockOpt(TCP_NODELAY, &bTCPNoDelay, sizeof(bTCPNoDelay), IPPROTO_TCP);

	while (false == m_bIsStopThread) {

		//. 아래의 ReadPacketT()는 blocking임.
		//. blocking을 빠져나왔다는건, 
		//. Socket handle값이 INVALID_SOCKET이므로, 연결이 끊겼음.
		sm.SerialConnection->bPPBoxCodeReader = FALSE;

		//. 재접속 시도할경우 다른 스레드에게 양보.
		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		if (Connect()) {
			sm.SerialConnection->bPPBoxCodeReader = TRUE;
		}
		else {
			continue;
		}

		while (IsValidConnection()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if (!WaitSendCommandEvent()) {
				continue;
			}

			if (m_strCommandEdit.IsEmpty()) {
				continue;
			}
			
			int nRetry = 3;
			//. process.
			BOOL isSendFailed = !SendCommand(CStringA(m_strCommandEdit.GetBuffer()));

			if (isSendFailed) {
				//. retry...
			}

			//. wait for recv.
			BOOL isRecvFailed = !RecvCommand();

			if (isRecvFailed) {
				//Sleep(100);
				//. retry...
			}
			else
			{
				m_evtRequestCompleted.SetEvent();
			}
			//. loop
		}
	}
}

BOOL CPPBoxCodeReaderWnd::OpenSocket() {
	BOOL bIsConnected = TRUE;
	bIsConnected = m_socket.Connect(m_tSocket.strIP, (UINT)m_tSocket.iPort);
	//bIsConnected = m_socket.Connect(_T("192.168.100.100"), 9004);

	if (FALSE == bIsConnected) {
		DWORD dwLastError = WSAGetLastError();
		g_ipClient.m_log.WriteErrorLog(_T("[PPBoxCodeReader] Connect(). %s. err : %s"), g_st[IDS_PPBOX_CODE_READER_WND_FAIL],GetLastErrorString(dwLastError));

		//TODO error code 별 처리 루틴
		switch (dwLastError) {
			case WSAENETUNREACH: {
				break;
			}
		}
		//>>

		return FALSE;
	}

	return TRUE;
}

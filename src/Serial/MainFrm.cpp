
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "Serial.h"

#include "MainFrm.h"
#include "Inverter/Inverter.h"
#include "PPBoxCodeReader/PPBoxCodeReaderWnd.h"
#include "DustCollector/DustCollectorPressure.h"
#include "DustCollector/DustCollectorControlPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMainFrame::CMainFrame() : TIPCT<CFrameWnd>(&g_ipClient)
	, m_iSelectedControl(-1)
	, m_bSerialUse{}
	//, m_pWndSEM(nullptr)
	, m_pPPBoxCodeReader(nullptr)
	//, m_pWndLabelPrinter(nullptr)
	//, m_pWndTrayPocket(nullptr)
	//, m_pWndPressure(nullptr)
{
	for (int i = 0; i < countof(m_pInverter); i++)
		m_pInverter[i] = nullptr;
	for (int i = 0; i < countof(m_pDustCollectorPressure); ++i) {
		m_pDustCollectorPressure[i] = nullptr;
	}

	for (int i = 0; i < countof(m_pDustCollectorControlPanel); ++i) {
		m_pDustCollectorControlPanel[i] = nullptr;
	}
}

CMainFrame::~CMainFrame()
{
}

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_IP_COMMAND_TABLE(CMainFrame, IIPCommandTarget)
	ON_IP("SerialUI", "Show", OnIPShow)
END_IP_COMMAND_TABLE()

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

#define SHOW_WND

#ifdef SHOW_WND
	//the CREATESTRUCT cs
	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

#else
	//cs.style = WS_POPUP;
#endif

	return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//. menu disable
	::SetMenu(m_hWnd, NULL);

	CWnd* pLogWnd = g_ipClient.m_log.CreateLogWndStatic(LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(), this, 1025);
	g_ipClient.m_log.SetLogWnd(pLogWnd);

	DWORD CHILD_STYLE = WS_CHILD | WS_BORDER | WS_VISIBLE;

	CProfile profile(_T("../cfg/Serial/SerialUseOption.ini"));
	CProfileSection& section = profile.GetSection(_T("SerialUseOption"));

	for (int i = 0; i < countof(m_pInverter); i++) {
		//if (i != 0)
		//	continue;

		m_bSerialUse[eUSE_INVERTER_1 + i] = section.GetItemValue(Format(_T("INVERTER%d"), i + 1), FALSE);
		if (m_bSerialUse[eUSE_INVERTER_1 + i])
		{
			sm.SerialConnection->bInverter[i] = FALSE;

			m_pInverter[i] = new CInverter(this, i);

			if(!m_pInverter[i]->Create(nullptr, nullptr, NULL, CRect(), this)) {
				return -1;
			}
		}
	}

	m_bSerialUse[eUSE_PPBOX_CODE_READER] = section.GetItemValue(_T("PPBoxCodeReader"), FALSE);
	if (m_bSerialUse[eUSE_PPBOX_CODE_READER])
	{
		sm.SerialConnection->bPPBoxCodeReader = FALSE;

		m_pPPBoxCodeReader = new CPPBoxCodeReaderWnd();

		if(!m_pPPBoxCodeReader->Create(nullptr, nullptr, NULL, CRect(), this)) {
			return -1;
		}
	}
	
	m_bSerialUse[eUSE_DUST_COLLECTOR_PRESSURE] = section.GetItemValue(_T("DustCollectorPressure"), FALSE);
	if (m_bSerialUse[eUSE_DUST_COLLECTOR_PRESSURE])
	{
		for (int i = 0; i < nDUST_COLLECTOR; ++i) {
			sm.SerialConnection->bDustCollectorPressure[i] = FALSE;
			m_pDustCollectorPressure[i] = new CDustCollectorPressure(i);
			if (!m_pDustCollectorPressure[i]->Create(nullptr, nullptr, NULL, CRect(), this)) {
				return -1;
			}
		}
	}

	m_bSerialUse[eUSE_DUST_COLLECTOR_CONTROL_PANEL] = section.GetItemValue(_T("DustCollectorControlPanel"), FALSE);
	if (m_bSerialUse[eUSE_DUST_COLLECTOR_CONTROL_PANEL])
	{
		for (int i = 0; i < nDUST_COLLECTOR; ++i) {
			sm.SerialConnection->bDustCollectorControlPanel[i] = FALSE;
			m_pDustCollectorControlPanel[i] = new CDustCollectorControlPanel(i);
			if (!m_pDustCollectorControlPanel[i]->Create(nullptr, nullptr, NULL, CRect(), this)) {
				return -1;
			}
		}
	}
	
	//m_bSerialUse[eUSE_SEM] = section.GetItemValue(_T("SEM"), FALSE);
	//
	//if (m_bSerialUse[eUSE_SEM])
	//{
	//	sm.SerialConnection->bSEM = FALSE;
	//
	//	T_SEM_DATA* temp_SEM_DATA = new T_SEM_DATA[6];
	//	ZeroMemory(temp_SEM_DATA, sizeof(T_SEM_DATA) * 6);
	//
	//	m_SEMReader = new CSEMReader(temp_SEM_DATA);
	//	if (!m_SEMReader->Create(nullptr, nullptr, WS_VISIBLE | WS_POPUP | WS_CHILD, CRect(), this)) {
	//		return -1;
	//	}
	//
	//	m_pWndSEM = new CSEMWnd(this, temp_SEM_DATA, m_SEMReader);
	//	if (!m_pWndSEM->Create(nullptr, nullptr, NULL, CRect(), this)) {
	//		return -1;
	//	}
	//
	//}
	//
	//m_bSerialUse[eUSE_LABEL_PRINTER] = section.GetItemValue(_T("LabelPrinter"), FALSE);
	//if (m_bSerialUse[eUSE_LABEL_PRINTER]) {
	//
	//	sm.SerialConnection->bLabelPrinter = FALSE;
	//
	//
	//	m_pWndLabelPrinter = new CLablePrinterWnd();
	//
	//	if (!m_pWndLabelPrinter->Create(nullptr, nullptr, NULL, CRect(), this)) {
	//		return -1;
	//	}
	//}

	//m_bSerialUse[eUSE_TRAY_POCKET] = section.GetItemValue(_T("TrayPocket"), FALSE);

	//if (m_bSerialUse[eUSE_TRAY_POCKET])
	//{
	//	sm.SerialConnection->bTrayPocketClient[eTRAY_POCKET_CLIENT_1] = FALSE;
	//	sm.SerialConnection->bTrayPocketClient[eTRAY_POCKET_CLIENT_2] = FALSE;
	//
	//	m_pTrayPocketServer = new CTrayPocketServer();
	//	
	//	m_pWndTrayPocket = new CTrayPocketWnd(m_pTrayPocketServer, TRAY_POCKET_COUNT);
	//	if (!m_pWndTrayPocket->Create(nullptr, nullptr, NULL, CRect(), this)) {
	//		return -1;
	//	}
	//
	//}
	//m_bSerialUse[eUSE_PRESSURE_SENSOR_BOARD] = section.GetItemValue(_T("PressureSensorBoard"), FALSE);
	//if (m_bSerialUse[eUSE_PRESSURE_SENSOR_BOARD])
	//{
	//	sm.SerialConnection->bPressureSensorBoard = FALSE;
	//
	//	m_pPressureReader = new PressureSensorReaderSMC();
	//	m_pWndPressure = new CPressureSensorReaderSMCWnd(this, m_pPressureReader);
	//
	//	if (!m_pWndPressure->Create(nullptr, nullptr, NULL, CRect(), this)) {
	//		return -1;
	//	}
	//}
	return 0;
}

void CMainFrame::OnDestroy()
{
	SAFE_DELETE(m_pPPBoxCodeReader);
	for (int i = 0; i < countof(m_pInverter); i++)
		SAFE_DELETE(m_pInverter[i]);

	for (int i = 0; i < nDUST_COLLECTOR; ++i) {
		SAFE_DELETE(m_pDustCollectorPressure[i]);
		SAFE_DELETE(m_pDustCollectorControlPanel[i]);
	}
	
	CFrameWnd::OnDestroy();
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {

	if (!CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext)) {
		return FALSE;
	}

	//. LoadFrame 단계에서 ClientRect를 받아올 수 있음.
	ComputeArea();

	for (int i = 0; i < countof(m_pInverter); i++) {
		if (m_bSerialUse[eUSE_INVERTER_1 + i]) {
			m_pInverter[i]->Initialize();
		}
	}

	if (m_bSerialUse[eUSE_PPBOX_CODE_READER]) {
		m_pPPBoxCodeReader->Initialize();
	}
	if (m_bSerialUse[eUSE_DUST_COLLECTOR_PRESSURE]) {
		for (int i = 0; i < nDUST_COLLECTOR; ++i)
			m_pDustCollectorPressure[i]->Initialize();
	}

	if (m_bSerialUse[eUSE_DUST_COLLECTOR_CONTROL_PANEL]) {
		for (int i = 0; i < nDUST_COLLECTOR; ++i)
			m_pDustCollectorControlPanel[i]->Initialize();
	}
	return TRUE;
}

void CMainFrame::ComputeArea() {

	CRect rect;
	GetClientRect(rect);

	rect.DeflateRect(1, 1);
	double dW = (rect.Width() - 5) / 31;
	double dH = (rect.Height() - 5) / 31;

	m_rectA[eWORK] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 0, rect.left + dW * 28, rect.top + dH * 31);
	m_rectA[eMENU] = CRect(rect.left + 5 + dW * 28, rect.top + 5 + dH * 0, rect.left + dW * 31, rect.top + dH * 31);

	rect = m_rectA[eMENU];
	dW = (rect.Width() - 5);
	dH = (rect.Height() - 5) / 31;

	m_rectC[eC_INVERTER				] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH *  0, rect.left + dW * 1, rect.top + dH *  3);
	m_rectC[eC_PPBOX_CODE_READER	] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH *  4, rect.left + dW * 1, rect.top + dH *  7);
	m_rectC[eC_MCR					] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH *  8, rect.left + dW * 1, rect.top + dH *  11);
	//m_rectC[eC_LABEL_PRINTER_OPEN		] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 13, rect.left + dW * 1, rect.top + dH * 16);
	//m_rectC[eC_TRAY_POCEKT_COMM			] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 17, rect.left + dW * 1, rect.top + dH * 20);
	//m_rectC[eC_PRESSURE_SENSOR_READER	] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 21, rect.left + dW * 1, rect.top + dH * 24);
	m_rectC[eC_CLOSE					] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 28, rect.left + dW * 1, rect.top + dH * 31);

	CWnd* pWndLog = g_ipClient.m_log.GetLogWnd();

	if (pWndLog->m_hWnd) {
		pWndLog->MoveWindow(m_rectA[eWORK]);
	}
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	if (bShow) {

	}
}


void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_rectC[m_iSelectedControl].PtInRect(point)) {

		switch (m_iSelectedControl) {
			case eC_INVERTER: {
				//if (FALSE == m_bSerialUse[eUSE_INVERTER]) {
				//	break;
				//}
				//
				//static bool bOpen = TRUE;
				////m_pWndInverter->ShowWindow(bOpen);
				//m_pInverter->ShowWindow(bOpen);
				//
				//if (bOpen) {
				//
				//	//m_pWndInverter->SetForegroundWindow();
				//	m_pInverter->SetForegroundWindow();
				//}
				//
				//bOpen = !bOpen;
				break;
			}
			
			case eC_PPBOX_CODE_READER: {
				if (FALSE == m_bSerialUse[eUSE_PPBOX_CODE_READER]) {
					break;
				}
				m_pPPBoxCodeReader->ShowWindow(SW_SHOW);
				m_pPPBoxCodeReader->SetForegroundWindow();
				break;
			}

			//case eC_SEM_START: {
			//	if (FALSE == m_bSerialUse[eUSE_SEM]) {
			//		break;
			//	}

			//	if (FALSE == m_SEMReader->StartWorker()) {

			//	}
			//	break;
			//}
			//case eC_SEM_STOP: {

			//	if (FALSE == m_bSerialUse[eUSE_SEM]) {
			//		break;
			//	}
			//	m_SEMReader->StopWorker();
			//	break;
			//}

			case eC_MCR: {
				//if (FALSE == m_bSerialUse[eUSE_MCR]) {
				//	break;
				//}
				//static bool bOpen = TRUE;
				//
				//m_pWndMCRReaderJST->ShowWindow(bOpen);
				//
				//if (bOpen) {
				//
				//	m_pWndMCRReaderJST->SetForegroundWindow();
				//}
				//bOpen = !bOpen;
				break;
			}
				
			//case eC_LABEL_PRINTER_OPEN: {
			//
			//	if (FALSE == m_bSerialUse[eUSE_LABEL_PRINTER]) {
			//		break;
			//	}
			//	static bool bOpen = TRUE;
			//	m_pWndLabelPrinter->ShowWindow(bOpen);
			//
			//	if (bOpen) {
			//
			//		m_pWndLabelPrinter->SetForegroundWindow();
			//	}
			//	bOpen = !bOpen;
			//	break;
			//}
			//case eC_TRAY_POCEKT_COMM: {
			//	if (FALSE == m_bSerialUse[eUSE_TRAY_POCKET]) {
			//		break;
			//	}
			//	static bool bOpen = TRUE;
			//	m_pWndTrayPocket->ShowWindow(bOpen);
			//	if (bOpen) {
			//
			//		m_pWndTrayPocket->SetForegroundWindow();
			//	}
			//	bOpen = !bOpen;
			//	break;
			//}
			//case eC_PRESSURE_SENSOR_READER: {
			//	if (FALSE == m_bSerialUse[eUSE_PRESSURE_SENSOR_BOARD]) {
			//		break;
			//	}
			//	static bool bOpen = TRUE;
			//
			//	m_pWndPressure->ShowWindow(bOpen);
			//	if (bOpen) {
			//
			//		m_pWndPressure->SetForegroundWindow();
			//	}
			//	bOpen = !bOpen;
			//
			//	break;
			//}

			case eC_CLOSE: {
				//if (m_bSerialUse[eUSE_SEM]) {
				//	m_pWndSEM->ShowWindow(SW_HIDE);
				//}
				//if (m_bSerialUse[eUSE_MCR]) {
				//	m_pWndMCRReaderJST->ShowWindow(SW_HIDE);
				//}
				//if (m_bSerialUse[eUSE_LABEL_PRINTER]) {
				//	m_pWndLabelPrinter->ShowWindow(SW_HIDE);
				//}
				//if (m_bSerialUse[eUSE_TRAY_POCKET]) {
				//	m_pWndTrayPocket->ShowWindow(SW_HIDE);
				//}
				//if (m_bSerialUse[eUSE_PRESSURE_SENSOR_BOARD]) {
				//	m_pWndPressure->ShowWindow(SW_HIDE);
				//}
				if (m_bSerialUse[eUSE_PPBOX_CODE_READER]) {
					m_pPPBoxCodeReader->ShowWindow(SW_HIDE);
				}
				if (m_bSerialUse[eUSE_DUST_COLLECTOR_PRESSURE]) {
					for (int i = 0; i < nDUST_COLLECTOR; ++i)
						m_pDustCollectorPressure[i]->ShowWindow(SW_HIDE);
				}

				if (m_bSerialUse[eUSE_DUST_COLLECTOR_CONTROL_PANEL]) {
					for (int i = 0; i < nDUST_COLLECTOR; ++i)
						m_pDustCollectorControlPanel[i]->ShowWindow(SW_HIDE);
				}
				ShowWindow(SW_HIDE);//24.03.14 KGH test용으로 임시제거
				break;
			}
		}
		
	}
	m_iSelectedControl = eC_NONE;
	Invalidate(FALSE);
	CFrameWnd::OnLButtonUp(nFlags, point);
}


void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_iSelectedControl = eC_NONE;

	for (int i = 0; i < eC_LIST; ++i) {

		if (m_rectC[i].PtInRect(point)) {

			m_iSelectedControl = i;
			InvalidateRect(m_rectC[i], FALSE);
			break;
		}
	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}


void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting

					   // Do not call CFrameWnd::OnPaint() for painting messages

	CMemDCEx::CMemDC		memDC(&dc);

	DrawButtonUpDown(memDC, m_rectC[eC_INVERTER], _T("INVERTER"), m_iSelectedControl == eC_INVERTER);
	//DrawButtonUpDown(memDC, m_rectC[eC_SEM_START], _T("SEM StartThread"), m_iSelectedControl == eC_SEM_START);
	//DrawButtonUpDown(memDC, m_rectC[eC_SEM_STOP], _T("SEM StopThread"), m_iSelectedControl == eC_SEM_STOP);
	DrawButtonUpDown(memDC, m_rectC[eC_PPBOX_CODE_READER], _T("PPBoxCodeReader"), m_iSelectedControl == eC_PPBOX_CODE_READER);
	DrawButtonUpDown(memDC, m_rectC[eC_MCR], _T("MCR"), m_iSelectedControl == eC_MCR);

	//DrawButtonUpDown(memDC, m_rectC[eC_LABEL_PRINTER_OPEN], _T("LabelPrinter"), m_iSelectedControl == eC_LABEL_PRINTER_OPEN);
	//DrawButtonUpDown(memDC, m_rectC[eC_TRAY_POCEKT_COMM], _T("TrayPocket"), m_iSelectedControl == eC_TRAY_POCEKT_COMM);
	//DrawButtonUpDown(memDC, m_rectC[eC_PRESSURE_SENSOR_READER], _T("PressureSensor"), m_iSelectedControl == eC_PRESSURE_SENSOR_READER);

	DrawButtonUpDown(memDC, m_rectC[eC_CLOSE], _T("Close"), m_iSelectedControl == eC_CLOSE);
	//TODO 미구현
	//DrawButtonUpDown(memDC, m_rectC[eC_EXIT], _T("Exit"), m_iSelectedControl == eC_EXIT);
}

BOOL CMainFrame::OnIPShow(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	BOOL bShow = var.GetChildItem(_T("Show"), TRUE);

	if (bShow) {
		//ShowWindow(SW_SHOW);
		//int iW = 1920 / 2;
		//int iH = 1080 / 6;
		//SetWindowPos(&wndTopMost, iW, 0, iW, iH * 5, SWP_SHOWWINDOW);
		SetForegroundWindow();
		ShowWindow(SW_SHOW);
	}
	else {
		//if (m_bSerialUse[eUSE_SEM]) {
		//	m_pWndSEM->ShowWindow(SW_HIDE);
		//}
		//if (m_bSerialUse[eUSE_MCR]) {
		//	m_pWndMCRReaderJST->ShowWindow(SW_HIDE);
		//}
		//if (m_bSerialUse[eUSE_LABEL_PRINTER]) {
		//	m_pWndLabelPrinter->ShowWindow(SW_HIDE);
		//}
		//if (m_bSerialUse[eUSE_TRAY_POCKET]) {
		//	m_pWndTrayPocket->ShowWindow(SW_HIDE);
		//}
		//if (m_bSerialUse[eUSE_PRESSURE_SENSOR_BOARD]) {
		//	m_pWndPressure->ShowWindow(SW_HIDE);
		//}
		if (m_bSerialUse[eUSE_PPBOX_CODE_READER]) {
			m_pPPBoxCodeReader->ShowWindow(SW_HIDE);
		}
		
		if (m_bSerialUse[eUSE_DUST_COLLECTOR_PRESSURE]) {
			for (int i = 0; i < nDUST_COLLECTOR; ++i)
				m_pDustCollectorPressure[i]->ShowWindow(SW_HIDE);
		}

		if (m_bSerialUse[eUSE_DUST_COLLECTOR_CONTROL_PANEL]) {
			for (int i = 0; i < nDUST_COLLECTOR; ++i)
				m_pDustCollectorControlPanel[i]->ShowWindow(SW_HIDE);
		}
		
		ShowWindow(SW_HIDE);
	}
	return TRUE;
}
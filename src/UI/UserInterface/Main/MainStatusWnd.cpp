#include "pch.h"
#include "UI.h"
#include "MainStatusWnd.h"
#include "MainFrame.h"
#include "../Main/MainStatus/ModuleStatusWnd.h"
#include "TitleWnd.h"
#include "Simulator/DemoWnd.h"
#define IDS_INTERVAL_MAIN 22222
IMPLEMENT_DYNCREATE		(CMainStatusWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CMainStatusWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()

CMainStatusWnd::CMainStatusWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
}

CMainStatusWnd::~CMainStatusWnd() {
}



BOOL CMainStatusWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}






int CMainStatusWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

try {

	for (int i = 0; i < eMODULE_PC1_END - eMODULE_PC1_BEGIN; i++) {
		m_ModuleStatus[i] = new CModuleStatusWnd(g_st[IDS_MAIN_STATUS_WND_LOAD_GLASS_COVEYOR_1 + i], (eMODULEs)(eMODULE_PC1_BEGIN + i));
		if (!m_ModuleStatus[i]->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL)) return -1;
		m_ModuleStatus[i]->ShowWindow(SW_HIDE);
	}
	HRESULT		hResult;
	LPCTSTR		szNames = 	_T("../img/01. LOADER_01.png");	
		hResult = m_imgMain.Load(szNames);
		if (FAILED(hResult)) {
			CString strTmp = _T("ERROR: Failed to load ");
		}
	

}
catch (...) {
	return -1;
}
	return 0;
}


void CMainStatusWnd::OnKillFocus(CWnd* pNewWnd) {
	CFrameWndEx::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
	Invalidate();
}



void CMainStatusWnd::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case IDS_INTERVAL_MAIN: {
			InvalidateRect(m_rectC[eC_GLASS_CONVEYOR_MUTE], FALSE);
			InvalidateRect(m_rectC[eC_PAPER_CONVEYOR_MUTE], FALSE);
			break;
		}
		default:
			break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CMainStatusWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_VISION].left, g_rectArea[eAREA_VISION].top, g_rectArea[eAREA_VISION].Width(), g_rectArea[eAREA_VISION].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		Invalidate();

		SetSimulatorView();
		SetTimer(IDS_INTERVAL_MAIN, 100, nullptr);
	}
	else {
		KillTimer(IDS_INTERVAL_MAIN);
	}
}


BOOL CMainStatusWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CMainStatusWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	memDC.FillSolidRect(m_rectS[eS_FULL_AREA], g_colorBackground[eEC_DEFAULT]);
	CDrawSymbol::DrawLineRect(&memDC, m_rectS[eS_FULL_AREA], g_colorForeground[eEC_DEFAULT]);


	//if (sm.IO->tIn_CRT.fGLASS_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_ON_SIGNAL)
	//{
	//	DrawMDIMenuButton(memDC, m_rectC[eC_GLASS_CONVEYOR_MUTE], _T("글래스 1차 뮤팅 On"), sm.IO->tIn_CRT.fGLASS_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_ON_SIGNAL || m_eSelectControl == eC_GLASS_CONVEYOR_MUTE
	//		, m_eMouseOver == eC_GLASS_CONVEYOR_MUTE, 11, RGB_GRAY_LIGHT, RGB_GREEN);
	//}
	//else if (sm.IO->tIn_CRT.fGLASS_BOX_CV_2ND_LIGHT_CURTAIN_MUTING_ON_SIGNAL)
	//{
	//	DrawMDIMenuButton(memDC, m_rectC[eC_GLASS_CONVEYOR_MUTE], _T("글래스 2차 뮤팅 On"), sm.IO->tIn_CRT.fGLASS_BOX_CV_2ND_LIGHT_CURTAIN_MUTING_ON_SIGNAL || m_eSelectControl == eC_GLASS_CONVEYOR_MUTE
	//		, m_eMouseOver == eC_GLASS_CONVEYOR_MUTE, 11, RGB_GRAY_LIGHT, RGB_YELLOW);
	//}
	//else
	//{
	//	DrawMDIMenuButton(memDC, m_rectC[eC_GLASS_CONVEYOR_MUTE], _T("글래스 1,2차 뮤팅 Off"), m_eSelectControl == eC_GLASS_CONVEYOR_MUTE	, m_eMouseOver == eC_GLASS_CONVEYOR_MUTE, 11, RGB_GRAY, RGB_GRAY_LIGHT);
	//}
    //
	//if (sm.IO->tIn_CRT.fPAPER_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_ON_SIGNAL)
	//{
	//	DrawMDIMenuButton(memDC, m_rectC[eC_PAPER_CONVEYOR_MUTE], _T("간지 1차 뮤팅 On"), sm.IO->tIn_CRT.fPAPER_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_ON_SIGNAL || m_eSelectControl == eC_PAPER_CONVEYOR_MUTE
	//		, m_eMouseOver == eC_PAPER_CONVEYOR_MUTE, 11, RGB_GRAY_LIGHT, RGB_GREEN);
	//}
	//else if (sm.IO->tIn_CRT.fPAPER_BOX_CV_2ND_LIGHT_CURTAIN_MUTING_ON_SIGNAL)
	//{
	//	DrawMDIMenuButton(memDC, m_rectC[eC_PAPER_CONVEYOR_MUTE], _T("간지 2차 뮤팅 On"), sm.IO->tIn_CRT.fPAPER_BOX_CV_2ND_LIGHT_CURTAIN_MUTING_ON_SIGNAL || m_eSelectControl == eC_PAPER_CONVEYOR_MUTE
	//		, m_eMouseOver == eC_PAPER_CONVEYOR_MUTE, 11, RGB_GRAY_LIGHT, RGB_YELLOW);
	//}
	//else
	//{
	//	DrawMDIMenuButton(memDC, m_rectC[eC_PAPER_CONVEYOR_MUTE], _T("간지 1,2차 뮤팅 Off"), m_eSelectControl == eC_PAPER_CONVEYOR_MUTE, m_eMouseOver == eC_PAPER_CONVEYOR_MUTE, 11, RGB_GRAY, RGB_GRAY_LIGHT);
	//}
}



void CMainStatusWnd::OnMouseMove(UINT nFlags, CPoint point) {
	int eMouseOver = m_eMouseOver;
	eMouseOver = eC_NONE;

	if ((m_eSelectControl != eC_NONE) && (!m_rectC[m_eSelectControl].PtInRect(point))) {
		int eSelectControl = m_eSelectControl;
		m_eSelectControl = eC_NONE;
		InvalidateRect(m_rectC[eSelectControl], FALSE);
	}

	for (int i = 0; i < eC_END; i++) {
		if (eMouseOver != i) {
			if (m_rectC[i].PtInRect(point)) {
				InvalidateRect(m_rectC[i], FALSE);
				eMouseOver = i;
				
				break;
			}
		}
	}
	if (eMouseOver != m_eMouseOver) {
		
		if (m_eMouseOver != eC_NONE)
			InvalidateRect(m_rectC[m_eMouseOver], FALSE);
		m_eMouseOver = eMouseOver;
	}
	CFrameWndEx::OnMouseMove(nFlags, point);
}



void CMainStatusWnd::OnLButtonDown(UINT nFlags, CPoint point) {
	SetFocus();
	m_eSelectControl = eC_NONE;
	for (int i = 0; i<eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			InvalidateRect(m_rectC[i],FALSE);
			break;
		}
	}

	CFrameWndEx::OnLButtonDown(nFlags, point);
}



void CMainStatusWnd::OnLButtonUp(UINT nFlags, CPoint point)  {
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_GLASS_CONVEYOR_MUTE: {
				strLog = _T("Glass Conveyor Mute");
				
                // 원본
				//// 1차 라이트 커튼 On (2차 라이트 커튼 Off)
				//if (sm.IO->tOut_NEW.fGLASS_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_SW_LAMP_ON == FALSE)
				//{
				//	//// 로드 컨베이어 부분 안전 상태 체크
				//	//auto eLoadConveyor1 = GetConveyor(eMODULE_PC1_LOAD_CONVEYOR_1);
				//	//BOOL isSafetyLoadConveyor1 = sm.Conveyor[eLoadConveyor1].out.wForRun == FALSE && sm.Conveyor[eLoadConveyor1].out.wRevRun == FALSE;
				//	//BOOL isSafetyLoadConveyor2 = GetPPBOXSensor(eMODULE_LOAD_GLASS_2_CONVEYOR, eConveyor_Sensor1, FALSE);
				//	//
				//	//// 언로드 컨베이어 부분 안전 상태 체크
				//	//auto eUnloadConveyor1 = GetConveyor(eMODULE_UNLOAD_GLASS_1_CONVEYOR);
				//	//BOOL isSafetyUnloadConveyor1 = sm.Conveyor[eUnloadConveyor1].out.wForRun == FALSE && sm.Conveyor[eUnloadConveyor1].out.wRevRun == FALSE;
				//	//BOOL isSafetyUnloadConveyor2 = GetPPBOXSensor(eMODULE_UNLOAD_GLASS_2_CONVEYOR, eConveyor_Sensor1, FALSE);
				//	//
				//	//if (isSafetyLoadConveyor1 && isSafetyLoadConveyor2 && isSafetyUnloadConveyor1 && isSafetyUnloadConveyor2)
				//	//{
				//	//	sm.IO->tOut_NEW.fGLASS_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_SW_LAMP_ON = TRUE;
				//	//	sm.IO->tOut_NEW.fGLASS_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_ON = TRUE;
				//	//	sm.IO->tOut_NEW.fGLASS_BOX_CV_2ND_LIGHT_CURTAIN_MUTING_ON = FALSE;
				//	//}
				//}
				//// 1차 라이트 커튼 Off (2차 라이트 커튼 On)
				//else
				//{
				//	sm.IO->tOut_NEW.fGLASS_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_SW_LAMP_ON = FALSE;
				//	sm.IO->tOut_NEW.fGLASS_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_ON = FALSE;
				//	sm.IO->tOut_NEW.fGLASS_BOX_CV_2ND_LIGHT_CURTAIN_MUTING_ON = TRUE;
				//}

                // KES 수정 R43 Version
                // 1차 라이트 커튼 On (2차 라이트 커튼 Off)
				//if (sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW == FALSE)   // 정확하지 않음 - KES 
				//{
				//	// 로드 컨베이어 부분 안전 상태 체크
				//	auto eLoadConveyor1 = GetConveyor(eMODULE_PC1_LOAD_CONVEYOR_1);
				//	BOOL isSafetyLoadConveyor1 = sm.Conveyor[eLoadConveyor1].out.wForRun == FALSE && sm.Conveyor[eLoadConveyor1].out.wRevRun == FALSE;
				//	BOOL isSafetyLoadConveyor2 = GetPPBOXSensor(eMODULE_PC1_LOAD_CONVEYOR_2, FALSE);
				//	
				//	// 언로드 컨베이어 부분 안전 상태 체크
				//	auto eUnloadConveyor1 = GetConveyor(eMODULE_PC1_UNLOAD_CONVEYOR_2);
				//	BOOL isSafetyUnloadConveyor1 = sm.Conveyor[eUnloadConveyor1].out.wForRun == FALSE && sm.Conveyor[eUnloadConveyor1].out.wRevRun == FALSE;
				//	BOOL isSafetyUnloadConveyor2 = GetPPBOXSensor(eMODULE_PC1_UNLOAD_CONVEYOR_1, FALSE);
				//	
				//	if (isSafetyLoadConveyor1 && isSafetyLoadConveyor2 && isSafetyUnloadConveyor1 && isSafetyUnloadConveyor2)
				//	{
				//		sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW = TRUE;
				//		sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW = TRUE;
                //        sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_A_ON = TRUE;
				//		sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_B_ON = FALSE;
				//	}
				//}
				//// 1차 라이트 커튼 Off (2차 라이트 커튼 On)
				//else
				//{
				//	sm.IO->tOut_NEW.fLD_IN_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW = FALSE;
				//	sm.IO->tOut_NEW.fLD_OUT_PPBOX_MANUAL_OP_PANEL_MUTING_LAMP_YELLOW = FALSE;
                //    sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_A_ON = FALSE;
				//	sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_B_ON = TRUE;
				//}

				break;
			}
			case eC_PAPER_CONVEYOR_MUTE: {
				strLog = _T("Paper Conveyor Mute");

                // R43 Version 에서 Paper Box 부분 필요 없음 -KES

                // 원본
				//// 1차 라이트 커튼 On (2차 라이트 커튼 Off)
				//if (sm.IO->tOut_NEW.fPAPER_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_SW_LAMP_ON == FALSE)
				//{
				//	// 로드 컨베이어 부분 안전 상태 체크
				//	//auto eLoadConveyor1 = GetConveyor(eMODULE_LOAD_PPBOX_1_CONVEYOR);
				//	//BOOL isSafetyLoadConveyor1 = sm.Conveyor[eLoadConveyor1].out.wForRun == FALSE && sm.Conveyor[eLoadConveyor1].out.wRevRun == FALSE;
				//	//BOOL isSafetyLoadConveyor2 = GetPPBOXSensor(eMODULE_LOAD_PPBOX_2_CONVEYOR, eConveyor_Sensor1, FALSE);
				//	//
				//	//// 언로드 컨베이어 부분 안전 상태 체크
				//	//auto eUnloadConveyor1 = GetConveyor(eMODULE_UNLOAD_PPBOX_1_CONVEYOR);
				//	//BOOL isSafetyUnloadConveyor1 = sm.Conveyor[eUnloadConveyor1].out.wForRun == FALSE && sm.Conveyor[eUnloadConveyor1].out.wRevRun == FALSE;
				//	//BOOL isSafetyUnloadConveyor2 = GetPPBOXSensor(eMODULE_UNLOAD_PPBOX_2_CONVEYOR, eConveyor_Sensor1, FALSE);
				//	//
				//	//if (isSafetyLoadConveyor1 && isSafetyLoadConveyor2 && isSafetyUnloadConveyor1 && isSafetyUnloadConveyor2)
				//	//{
				//	//	sm.IO->tOut_NEW.fPAPER_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_SW_LAMP_ON = TRUE;
				//	//	sm.IO->tOut_NEW.fPAPER_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_ON = TRUE;
				//	//	sm.IO->tOut_NEW.fPAPER_BOX_CV_2ND_LIGHT_CURTAIN_MUTING_ON = FALSE;
				//	//}
				//}
				//// 1차 라이트 커튼 Off (2차 라이트 커튼 On)
				//else
				//{
				//	sm.IO->tOut_NEW.fPAPER_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_SW_LAMP_ON = FALSE;
				//	sm.IO->tOut_NEW.fPAPER_BOX_CV_1ST_LIGHT_CURTAIN_MUTING_ON = FALSE;
				//	sm.IO->tOut_NEW.fPAPER_BOX_CV_2ND_LIGHT_CURTAIN_MUTING_ON = TRUE;
				//}
				break;
			}
			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], _T("Main Status"), _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl	=	eC_NONE;
	CFrameWndEx::OnLButtonUp(nFlags, point);
}



void CMainStatusWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect	rc = m_rectS[eS_FULL_AREA];

	double dH = rc.Height() / 8.;
	double dW = rc.Width() / 12.;

	CRect rectCT = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 12, rc.top + dH * 8);
	CCoordTrans ct;
	ct.SetTransformMatrixT(1, M_PI, rectCT.CenterPoint().x, rectCT.CenterPoint().y, rectCT.CenterPoint().x, rectCT.CenterPoint().y);

	m_rectS[eS_ITEM_AREA] = CRect(rc.left+dW*0,		rc.top+dH*0,	rc.left+dW*1,	rc.bottom);
	m_rectS[eS_WORK_AREA] = CRect(rc.left+5+dW*1,	rc.top+5+dH*0,	rc.left+dW*12,	rc.bottom);

	rc = m_rectS[eS_WORK_AREA];
	dW = (rc.Width() - 5.) / 7.;
	dH = (rc.Height() - 5.) / 8.;

	m_rectS[eS_MODULE_STATUS_LOAD_GLASS_1_CONVEYOR	] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0.5, rc.left + dW * 1.5, rc.top + dH * 1.5);
	m_rectS[eS_MODULE_STATUS_LOAD_GLASS_2_CONVEYOR	] = CRect(rc.left + 5 + dW * 1.5, rc.top + 5 + dH * 0.5, rc.left + dW * 3, rc.top + dH * 1.5);
	m_rectS[eS_MODULE_STATUS_ELEVATOR_GLASS_CONVEYOR] = CRect(rc.left + 5 + dW * 3, rc.top + 5 + dH * 0.5, rc.left + dW * 4.5, rc.top + dH * 1.5);
	m_rectS[eS_MODULE_STATUS_GLASS_LOAD_TRANSFER	] = CRect(rc.left + 5 + dW * 4.5, rc.top + 5 + dH * 0.5, rc.left + dW * 6, rc.top + dH * 1.5);
	
	m_rectS[eS_IMG_AREA								] = CRect(rc.left+5+dW*0, rc.top+5+dH*2, rc.left+dW*5.75 - 10, rc.top+dH*6);	// Simulater Area

	m_rectS[eS_MODULE_STATUS_UNLOAD_GLASS_2_CONVEYOR] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 6.5, rc.left + dW * 1.5, rc.top + dH * 7.5);
	m_rectS[eS_MODULE_STATUS_UNLOAD_GLASS_1_CONVEYOR] = CRect(rc.left + 5 + dW * 1.5, rc.top + 5 + dH * 6.5, rc.left + dW * 3, rc.top + dH * 7.5);


	rc = m_rectS[eS_ITEM_AREA];
	dW = (rc.Width() - 5.) / 2.;
	dH = (rc.Height() - 5.) / 8.;
	m_rectC[eC_GLASS_CONVEYOR_MUTE] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0.5, rc.left + dW * 2 + 5, rc.top + dH * 1.5);
	m_rectC[eC_PAPER_CONVEYOR_MUTE] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 6.5, rc.left + dW * 2 + 5, rc.top + dH * 7.5);
	
	if (!g_bUIRotateDirection) {
		for (int i = eS_IMG_AREA; i < eS_END; i++) {
			CPoint2d ptS, ptE;
			ptS = m_rectS[i].TopLeft();
			ptE = m_rectS[i].BottomRight();
			ptS = ct.Trans(ptS);
			ptE = ct.Trans(ptE);
			m_rectS[i] = CRect(ptE, ptS);
		}
		for (int i = 0; i < eC_END; i++) {
			CPoint2d ptS, ptE;
			ptS = m_rectC[i].TopLeft();
			ptE = m_rectC[i].BottomRight();
			ptS = ct.Trans(ptS);
			ptE = ct.Trans(ptE);
			m_rectC[i] = CRect(ptE, ptS);
		}
	}
	for (int i = 0; i < eS_END - eS_MODULE_STATUS_LOAD_GLASS_1_CONVEYOR; i++) {
		m_ModuleStatus[i]->MoveWindow(m_rectS[eS_MODULE_STATUS_LOAD_GLASS_1_CONVEYOR +i]);
		m_ModuleStatus[i]->ShowWindow(SW_SHOW);
	}
}

void CMainStatusWnd::SetSimulatorView() {
	CMainFrame* pMain =  (CMainFrame*)AfxGetMainWnd();
	CTitleWnd* pTitle = (CTitleWnd*)pMain->getTitleWnd();
	CDemoWnd* pDemo = (CDemoWnd*)pTitle->GetDemoWnd();
	m_pSimulatorView = (CSimulator*)pDemo->getSimulator();
	if (m_pSimulatorView) {
		m_pSimulatorView->SetDirection(g_bUIRotateDirection);
		m_pSimulatorView->SetParent(this);
		m_pSimulatorView->MoveWindow(m_rectS[eS_IMG_AREA]);
	}
}

void CMainStatusWnd::CloseAllScrapWnd() {
	for (int i = 0; i < eMODULE_PC1_END - eMODULE_PC1_BEGIN; i++) {
        if (m_ModuleStatus[i] != nullptr) {
            m_ModuleStatus[i]->CloseScrapWnd();
        }
	}
}
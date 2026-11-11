// ScrapeWnd.cpp : implementation file
//

#include "pch.h"
#include "UI.h"
#include "ScrapeWnd.h"
#include "InsertMaterialListWnd.h"

// CScrapeWnd
IMPLEMENT_DYNCREATE(CScrapeWnd, CFrameWndEx)
#define IDS_CELL_STATUS 9999

CScrapeWnd::CScrapeWnd(CWnd* pwndParent) : TIPCT<CFrameWndEx>(&g_ipClient)
{
	m_eSelectControl	= eC_NONE;
}

CScrapeWnd::~CScrapeWnd()
{
}

void CScrapeWnd::ComputArea() {
	GetClientRect(m_rectClient);

	m_rectS[eS_FULL_AREA] = m_rectClient;
	CRect rc = m_rectS[eS_FULL_AREA];

	double dW = (rc.Width() - 5.) / 5.;
	double dH = (rc.Height() - 5.) / 10.;
	m_rectS[eS_TITLE]		= CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 5, rc.top + dH * 1);
	m_rectS[eS_STATUS_TEXT]	= CRect(rc.left + 5 + dW * 0, rc.top + dH * 1, rc.left + dW * 5, rc.top + dH * 9);
	m_rectC[eC_INPUT_STOP]	= CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 9, rc.left + dW * 1, rc.top + dH * 10);
	m_rectC[eC_SCRAPE]		= CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 9, rc.left + dW * 2, rc.top + dH * 10);
	m_rectC[eC_CLOSE]		= CRect(rc.left + 5 + dW * 4, rc.top + 5 + dH * 9, rc.left + dW * 5, rc.top + dH * 10);

}

void CScrapeWnd::RemoveMaterial() {
	auto& step = sm.Module[m_eModule].Status.wSeqStep;
	auto& Module = sm.Module[m_eModule];

	// LD 는 투입/배출 하는 Conveyor 와 Transfer 만 Scrape 진행
	// 나머지는 Abort 로 처리해야할듯
	switch (m_eModule) {
		// 2021.04.30 리프트 박스 스크랩 기능
		case eMODULE_PC1_LOAD_CONVEYOR_1: {
			// Load Conveyor 2
			{
				BOOL bInitCheck		= (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep > S_LDCV2_SEQ_PPBOX_LOAD_SEND_ABLE_CHECK
									&& sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep < S_LDCV2_SEQ_PPBOX_LOAD_SEND_INIT_CHECK);
				if (bInitCheck) {
					sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep = S_LDCV2_SEQ_INIT;
					SetToUpperInit(eMODULE_PC1_LOAD_CONVEYOR_2);
					SetToLowerInit(eMODULE_PC1_LOAD_CONVEYOR_2);
				}
			}

			//<< 2024.06.29 FDC
			SET_STEP_CELL(sm.FDC->Conveyor[0], 0, "");
			//>>

			SetToUpperInit(&Module);
			SetToLowerInit(&Module);
			Module.Glass.bMaterial = FALSE;
			break;
		}
		case eMODULE_PC1_LOAD_CONVEYOR_2: {
			// Load Conveyor 1
			{
				BOOL bInitCheck		= (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_1].Status.wSeqStep > S_LDCV1_SEQ_PPBOX_UNLOAD_RECV_ABLE_CHECK
									&& sm.Module[eMODULE_PC1_LOAD_CONVEYOR_1].Status.wSeqStep < S_LDCV1_SEQ_PPBOX_UNLOAD_END);
				if (bInitCheck) {
					sm.Module[eMODULE_PC1_LOAD_CONVEYOR_1].Status.wSeqStep = S_LDCV1_SEQ_INIT;
					SetToUpperInit(eMODULE_PC1_LOAD_CONVEYOR_1);
					SetToLowerInit(eMODULE_PC1_LOAD_CONVEYOR_1);
				}
			}

			// Elevator Conver
			{
				BOOL bInitCheck	= (sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Status.wSeqStep > S_ELCV_SEQ_PPBOX_LOAD_SEND_ABLE_CHECK
								&& sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Status.wSeqStep < S_ELCV_SEQ_PPBOX_LOAD_SEQ_END);

				if (bInitCheck) {
					sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Status.wSeqStep = S_ELCV_SEQ_INIT;
					SetToUpperInit(eMODULE_PC1_ELEVATOR_CONVEYOR);
					SetToLowerInit(eMODULE_PC1_ELEVATOR_CONVEYOR);
				}
			
			}
			// Glass Transfer
			{
				BOOL bInitCheck	= (sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep > S_GLASS_TR_SEQ_LDCV2_SEND_ABLE_CHECK
								&& sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep < S_GLASS_TR_SEQ_GLASS_PAPER_LOAD_END);

				if (bInitCheck) {
					sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep = S_GLASS_TR_SEQ_INIT;
					SetToUpperInit(eMODULE_PC1_GLASS_TRANSFER);
					SetToLowerInit(eMODULE_PC1_GLASS_TRANSFER);
				}
			}

			//<< 2024.06.29 FDC
			SET_STEP_CELL(sm.FDC->Conveyor[1], 0, "");
			//>>

			SetToUpperInit(&Module);
			SetToLowerInit(&Module);
			Module.Glass.bMaterial = FALSE;
			step = S_LDCV2_SEQ_INIT;
			break;
		}
		
		case eMODULE_PC1_ELEVATOR_CONVEYOR: {
			// Load Conveyor 2
			{
				BOOL bInitCheck = (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep > S_LDCV2_SEQ_PPBOX_UNLOAD_RECV_ABLE_CHECK
					&& sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep < S_LDCV2_SEQ_PPBOX_UNLOAD_END);

				if (bInitCheck) {
					sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep = S_LDCV2_SEQ_INIT;
					SetToUpperInit(eMODULE_PC1_LOAD_CONVEYOR_2);
					SetToLowerInit(eMODULE_PC1_LOAD_CONVEYOR_2);
				}
			}
			// Glass Transfer
			{
				BOOL bInitCheck = (sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep > S_GLASS_TR_SEQ_PAPER_UNLOAD_RECV_ABLE_CHECK
					&& sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep < S_GLASS_TR_SEQ_PAPER_UNLOAD_END);

				if (bInitCheck) {
					sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep = S_GLASS_TR_SEQ_INIT;
					SetToUpperInit(eMODULE_PC1_GLASS_TRANSFER);
					SetToLowerInit(eMODULE_PC1_GLASS_TRANSFER);
				}
			}

			//<< 2024.06.29 FDC
			SET_STEP_CELL(sm.FDC->Conveyor[2], 0, "");
			//>>

			SetToUpperInit(&Module);
			SetToLowerInit(&Module);
			Module.Glass.bMaterial = FALSE;
			step = S_ELCV_SEQ_INIT;
			break;
		}
		case eMODULE_PC1_UNLOAD_CONVEYOR_1: {
			// Elevator Conveyor
			{
				BOOL bInitCheck = (sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Status.wSeqStep > S_ELCV_SEQ_PPBOX_UNLOAD_RECV_ABLE_CHECK
					&& sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Status.wSeqStep < S_ELCV_SEQ_PPBOX_UNLOAD_SEQ_END);

				if (bInitCheck) {
					sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Status.wSeqStep = S_ELCV_SEQ_INIT;
					SetToUpperInit(eMODULE_PC1_ELEVATOR_CONVEYOR);
					SetToLowerInit(eMODULE_PC1_ELEVATOR_CONVEYOR);
				}
			}
			// Unload Conveyor 2
			{
				BOOL bInitCheck = (sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep > S_ULDCV2_SEQ_PPBOX_LOAD_SEND_ABLE_CHECK
					&& sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep < S_ULDCV2_SEQ_PPBOX_LOAD_END);

				if (bInitCheck) {
					sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2].Status.wSeqStep = S_ULDCV2_SEQ_INIT;
					SetToUpperInit(eMODULE_PC1_UNLOAD_CONVEYOR_2);
					SetToLowerInit(eMODULE_PC1_UNLOAD_CONVEYOR_2);
				}
			}

			//<< 2024.06.29 FDC
			SET_STEP_CELL(sm.FDC->Conveyor[3], 0, "");
			//>>

			SetToUpperInit(&Module);
			SetToLowerInit(&Module);
			Module.Glass.bMaterial = FALSE;
			step = S_ULDCV1_SEQ_INIT;
			break;
		}
		case eMODULE_PC1_UNLOAD_CONVEYOR_2: {
			// Unload Conveyor 1
			{
				BOOL bInitCheck = (sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_1].Status.wSeqStep > S_ULDCV1_SEQ_PPBOX_UNLOAD_RECV_ABLE_CHECK
					&& sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_1].Status.wSeqStep < S_ULDCV1_SEQ_PPBOX_UNLOAD_END);

				if (bInitCheck) {
					// CV Stop 해야 할수도
					sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_1].Status.wSeqStep = S_ULDCV1_SEQ_INIT;
					SetToUpperInit(eMODULE_PC1_UNLOAD_CONVEYOR_1);
					SetToLowerInit(eMODULE_PC1_UNLOAD_CONVEYOR_1);
				}
			}

			//<< 2024.06.29 FDC
			SET_STEP_CELL(sm.FDC->Conveyor[4], 0, "");
			//>>

			SetToUpperInit(&Module);
			SetToLowerInit(&Module);
			Module.Glass.bMaterial = FALSE;
			step = S_ULDCV1_SEQ_INIT;
			break;
		}
		case eMODULE_PC1_GLASS_TRANSFER: {
			// Load Conveyor 2
			{
				BOOL bInitCheck = (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep > S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_RECV_ABLE_CHECK
					&& sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep < S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_END);

				if (bInitCheck) {
					// CV Stop 해야 할수도
					sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep = S_LDCV2_SEQ_INIT;
					SetToUpperInit(eMODULE_PC1_LOAD_CONVEYOR_2);
					SetToLowerInit(eMODULE_PC1_LOAD_CONVEYOR_2);
				}
			}

			// Elevator Conveyor
			{
				BOOL bInitCheck = (sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Status.wSeqStep > S_ELCV_SEQ_PAPER_LOAD_SEND_ABLE_CHECK
					&& sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Status.wSeqStep < S_ELCV_SEQ_PAPER_LOAD_END);

				if (bInitCheck) {
					// CV Stop 해야 할수도
					sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Status.wSeqStep = S_ELCV_SEQ_INIT;
					SetToUpperInit(eMODULE_PC1_ELEVATOR_CONVEYOR);
					SetToLowerInit(eMODULE_PC1_ELEVATOR_CONVEYOR);
				}
			}

			// SCT Table ?? 어떻게? IPFunction ? 
			//

			//<< 2024.06.29 FDC
			SET_STEP_CELL(sm.FDC->GlassTransfer, 0, "");
			//>>

			SetToUpperInit(&Module);
			SetToLowerInit(&Module);
			// 21.06.14 Glass 데이터 저장
			if (!sm.Sequence->bCIMSkip) {
				SaveGlassData(sm.Module[m_eModule].Glass);
			}
			Module.Glass.bMaterial = FALSE;
			// Glass Transfer 초기 상태로 변경
			step = S_GLASS_TR_SEQ_INIT;
		
			break;
		}
	}
}

void CScrapeWnd::InsertMaterial()
{
	auto& step = sm.Module[m_eModule].Status.wSeqStep;

	// 21.01.06 TODO 주는 모듈의 Send Begin 스탭으로 바꾸는 것 추가와 시그널 초기화 필요
	switch (m_eModule) {
		case eMODULE_PC1_GLASS_TRANSFER: {
			if (sm.Sequence->bCIMSkip) {
				CString strInput = InPutMessage(g_st[IDS_SCRAP_WND_INSERT_ID_MANUALLY]);
				if (!strInput.IsEmpty()) {
					sprintf_s(sm.Module[m_eModule].Glass.szCELLID, sizeof(sm.Module[m_eModule].Glass.szCELLID), "%s", CStringA(strInput).GetBuffer());
		
					sm.Module[m_eModule].Glass.bMaterial = TRUE;
					sm.Module[m_eModule].Status.wMaterialType = eMT_ORG_GLASS;
		
					SetToUpperInit(m_eModule);
					SetToLowerInit(m_eModule);
					step = S_GLASS_TR_SEQ_INIT;
		
					WriteButtonLog(_T("Main Status"), _T("Scrap"), m_strName, CString(magic_enum::enum_name(m_eModule).data()) + g_st[IDS_SCRAP_WND_BE_MATERIAL_CIM_SKIP], theApp.m_UserManager.GetCurrentUserData().szID);
				}
			}
			else {
				// Module / 바뀔 Step / 넣어서 전달
				CIPVar var;
				var.SetChildItem("Step", S_GLASS_TR_SEQ_INIT);
				var.SetChildItem("Module", m_eModule);
				__POST_IP_COMMAND__("Insert", "Material", var);
			}
		}
		break;
		
		case eMODULE_PC1_LOAD_CONVEYOR_2:
		{
			//<< 2024.06.29 FDC
			SET_STEP_CELL(sm.FDC->Conveyor[1], 2, GetTimeStringYMDHMS());
			//>>

			sm.Module[m_eModule].Glass.bMaterial = TRUE;
			SetToUpperInit(m_eModule);
			SetToLowerInit(m_eModule);
			step = S_ULDCV2_SEQ_INIT;
		}
		break;
		case eMODULE_PC1_ELEVATOR_CONVEYOR:
		{
			//<< 2024.06.29 FDC
			SET_STEP_CELL(sm.FDC->Conveyor[2], 2, GetTimeStringYMDHMS());
			//>>

			// 2024.06.12 LBH - MDIK 요청사항
			sm.Module[m_eModule].Glass.bMaterial = TRUE;
			SetToUpperInit(m_eModule);
			SetToLowerInit(m_eModule);
			step = S_ELCV_SEQ_INIT;
		}
		break;
	}
}


BEGIN_MESSAGE_MAP(CScrapeWnd, CFrameWndEx)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


BEGIN_IP_COMMAND_TABLE(CScrapeWnd, IIPCommandTarget)
END_IP_COMMAND_TABLE()


int CScrapeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pwndInsertMaterialList = new CInsertMaterialListWnd();
	if (!m_pwndInsertMaterialList->Create(NULL, NULL, NULL))
		return -1;
	// TODO:  Add your specialized creation code here
	return 0;
}

void CScrapeWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	if( bShow){
		SetWindowText(Format(_T("%s Scrape"), m_strName));
		//SetWindowText(_T("Scrape"));
		int iW = (g_rectArea[eAREA_WORK].Width() - 5) / 4;
		int iH = (g_rectArea[eAREA_WORK].Height() - 5) / 1.5 ;
		SetWindowPos(&wndTopMost, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, iW, iH, SWP_SHOWWINDOW);
		SetTimer(IDS_CELL_STATUS, 100, nullptr);
		ComputArea();
		Invalidate();
	}

	else {
		KillTimer(IDS_CELL_STATUS);
	}
}

void CScrapeWnd::OnPaint()
{
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CDrawSymbol::DrawLineRect(memDC, m_rectS[eS_FULL_AREA], g_colorForeground[eEC_DEFAULT]);
	
	auto& Module = sm.Module[m_eModule];
	DrawLineRect(&memDC, m_rectS[eS_FULL_AREA]);
	SelectedDrawTextB(&memDC, m_rectS[eS_TITLE], m_strName, FW_HEAVY, 9, RGB_BLACK, RGB_SKY_LIGHT, RGB_BLACK);

	CString strHostID	= CString(Module.Glass.szCELLID);
	if (strHostID.IsEmpty()) strHostID = g_st[IDS_SCRAP_WND_EMPTY];
	CString strEID		= CString(Module.Glass.szCELL_ID_BCR);
	if (strEID.IsEmpty()) strEID = g_st[IDS_SCRAP_WND_EMPTY];
	CString strLotID	= CString(Module.Glass.szCST_ID);
	if (strLotID.IsEmpty()) strLotID = g_st[IDS_SCRAP_WND_EMPTY];

	//<< KES 수정 2024.4.8 - LD Conveyer 1,2는 <현재 Glass 수량 / 처음 Glass 수량> 표시하는 항목 추가
	CString strScrape(_T(""));
	if (m_eModule == eMODULE_PC1_LOAD_CONVEYOR_1 || m_eModule == eMODULE_PC1_LOAD_CONVEYOR_2) {
		strScrape = Format(_T("Host Glass ID : %s\nGlass ID: %s\nLot ID : %s\nGlass Count : %d / %d"), strHostID, strEID, strLotID, Module.Status.nCrtCount / 2, sm.LoadPPBox->TRAY_INFOs[0].nCell);
	}
	else {
		strScrape = Format(_T("Host Glass ID : %s\nGlass ID: %s\nLot ID : %s\n"), strHostID, strEID, strLotID);
	}
	//>>

	BOOL bMaterial = Module.Glass.bMaterial;
	if (bMaterial) {
		SelectedDrawText(&memDC, m_rectS[eS_STATUS_TEXT], strScrape, 12, RGB_BLACK, RGB_GREEN, RGB_BLACK, DT_CENTER | DT_VCENTER);
	}
	else {
		SelectedDrawText(&memDC, m_rectS[eS_STATUS_TEXT], strScrape, 12, RGB_BLACK, RGB_GRAY_LIGHT, RGB_BLACK, DT_CENTER | DT_VCENTER);
	}

	DrawMDIMenuButtonN(memDC, m_rectC[eC_SCRAPE], bMaterial ? g_st[IDS_SCRAP_WND_SCRAP] : g_st[IDS_SCRAP_WND_INPUT_CELL_FORCIBLY], m_eSelectControl == eC_SCRAPE);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_INPUT_STOP], g_st[IDS_SCRAP_WND_INPUT_STOP], Module.Status.bInputStop || m_eSelectControl == eC_INPUT_STOP, FALSE, DT_FONTSIZE, RGB_GRAY_LIGHT, Module.Status.bInputStop ? RGB_RED : RGB_ORANGE_LIGHT);
	DrawMDIMenuButtonN(&memDC, m_rectC[eC_CLOSE], g_st[IDS_CLOSE], m_eSelectControl == eC_CLOSE);
}

void CScrapeWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	for(int i =0 ; i < eC_END; i++){
		if( m_rectC[i].PtInRect(point)){
			m_eSelectControl = i;
			Invalidate(FALSE);
			break;
		}
	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}


void CScrapeWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString strLog;
	auto cleanup = make_scope_exit
	(
		[&]()
		{
			m_eSelectControl = eC_NONE;
			Invalidate(FALSE);
			if (!strLog.IsEmpty()) {
				WriteButtonLog(_T("Main Status"), _T("Scrape"), m_strName, strLog, theApp.m_UserManager.GetCurrentUserData().szID);
			}
			CFrameWndEx::OnLButtonUp(nFlags, point);

		}
	);

	if (m_eSelectControl <= eC_NONE || m_eSelectControl >= eC_END)
	{
		return;
	}

	if (!m_rectC[m_eSelectControl].PtInRect(point))
	{
		return;
	}

	switch (m_eSelectControl)
	{
		case eC_INPUT_STOP:
		{
			sm.Module[m_eModule].Status.bInputStop = !sm.Module[m_eModule].Status.bInputStop;
		}
		break;

		case eC_CLOSE: 
		{
			ShowWindow(SW_HIDE);
			strLog = _T("Close");
		}
		break;

		case eC_SCRAPE: 
		{
			if (sm.Sequence->fAutoMode && !sm.Sequence->fCycleStop) 
			{
				IPMessage(g_st[IDS_SCRAP_WND_CAN_NOT_SCRAP_MUCHIN_RUNNING]);
				return;
			}
			
			// 자재가 있는 경우 해당 데이터 저장
			if (sm.Module[m_eModule].Glass.bMaterial) 
			{
				if (RespMessage(Format(_T("%s\n%s"), g_st[IDS_SCRAP_WND_DO_YOU_WANT_SCRAP], m_strName)).Compare(_T("OK")) == 0) {
					RemoveMaterial();
					strLog = CString(magic_enum::enum_name(m_eModule).data()) + g_st[IDS_SCRAP_WND_SCRAP];
					IPMessage(g_st[IDS_SCRAP_WND_SCRAP_COMPLITE]);
					ShowWindow(SW_HIDE);
				}
			}
			// 자재가 없는 경우 재석 정보 파일로 불러 들여서 재석을 다시 생성 및 Step (Recv End 로)변경
			else if (eMODULE_PC1_GLASS_TRANSFER == m_eModule)
			{
				if (RespMessage(Format(_T("%s\n%s"), g_st[IDS_SCRAP_WND_DO_YOU_WANT_MAKE_MATERIAL], m_strName)).Compare(_T("OK")) == 0) {
					InsertMaterial();
				}
			}
			// 2023.01.27 Unload Conveyor 1,2 강제 재석 기능 추가 (빈박스)
			else if ((eMODULE_PC1_LOAD_CONVEYOR_2 == m_eModule) || (eMODULE_PC1_ELEVATOR_CONVEYOR == m_eModule)) {
				if (RespMessage(Format(_T("%s\n%s"), g_st[IDS_SCRAP_WND_DO_YOU_WANT_MAKE_MATERIAL],m_strName)).Compare(_T("OK")) == 0)
				{
					InsertMaterial();
				}
			}
			else
			{
				IPMessage(g_st[IDS_SCRAP_WND_BE_MATERIAL_ONLY_GLASS]);
			}
		}
		break;
	}
}

void CScrapeWnd::OnClose()
{
	ShowWindow(SW_HIDE);
	return;
}


void CScrapeWnd::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	ComputArea();
}
void CScrapeWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
		case IDS_CELL_STATUS: {
			InvalidateRect(m_rectC[eC_SCRAPE], FALSE);
			break;
		}
	}
	CFrameWnd::OnTimer(nIDEvent);
}


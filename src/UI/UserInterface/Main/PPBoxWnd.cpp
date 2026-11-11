#include "pch.h"
#include "UI.h"
#include "PPBoxWnd.h"
#include "MainFrame.h"
#define IDS_INTERVAL_MAIN 22222
IMPLEMENT_DYNCREATE		(CPPBoxWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CPPBoxWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()

CPPBoxWnd::CPPBoxWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
	m_iSelectGlass		= 0;
	m_iTotalCount		= 0;
}

CPPBoxWnd::~CPPBoxWnd() {
}

BOOL CPPBoxWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CPPBoxWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
try {
	
}
catch (...) {
	return -1;
}

	return 0;
}


void CPPBoxWnd::OnKillFocus(CWnd* pNewWnd) {
	CFrameWndEx::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
	Invalidate();
}



void CPPBoxWnd::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case IDS_INTERVAL_MAIN: {
			for (int i = 0; i < eC_END; i++) {
				InvalidateRect(m_rectC[i], FALSE);
			}
			//if (m_iTotalCount != sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount) {
			//	m_iTotalCount = sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount;
			//}
			break;
		}
		default:
			break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CPPBoxWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_VISION].left, g_rectArea[eAREA_VISION].top, g_rectArea[eAREA_VISION].Width()-5, g_rectArea[eAREA_VISION].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		Invalidate();
		m_iSelectGlass = 0;
		m_iTotalCount = sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount;
		SetTimer(IDS_INTERVAL_MAIN, 100, nullptr);
	}
	else {
		KillTimer(IDS_INTERVAL_MAIN);
	}
}


BOOL CPPBoxWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CPPBoxWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	memDC.FillSolidRect(m_rectS[eS_FULL_AREA], g_colorBackground[eEC_DEFAULT]);
	CDrawSymbol::DrawLineRect(&memDC, m_rectS[eS_FULL_AREA], g_colorForeground[eEC_DEFAULT]);


	SelectedDrawText(memDC, m_rectS[eS_WORK_TITLE], g_stMotor[IDS_CONVEYOR_NAME_02], DT_FONTSIZE, DT_PEN, RGB_GRAY_LIGHT, RGB_BLACK);
	DrawLineRect(memDC, m_rectS[eS_WORK_AREA], RGB_BLACK);

	auto& PPBox = *sm.LoadPPBox;
	for (int i = 0; i < MAX_CELL; i++) {
		const auto& tGlass = PPBox.TRAY_INFOs[0].CELL_INFOs[i];
		COLORREF color = RGB_GRAY;
		COLORREF colorPen = RGB_BLACK;
		CString strGlassID = (CString)tGlass.szCELLID;
		CString strEGlassID = (CString)tGlass.szCELL_ID_BCR;
		if (strGlassID.IsEmpty()) 
			strGlassID = g_st[IDS_PPBOX_WND_IS_EMPTY];
		if (i == m_iSelectGlass) {
			colorPen = RGB_RED;
		}
		if (tGlass.bMaterial) {
			color = RGB_GREEN;
		}
		if (sm.Sequence->bCIMSkip) {
			if (g_pRecipe->ppbox.iGlassCount <= i) {
				color = RGB_BLACK;
			}
		}
		else {
			if (PPBox.TRAY_INFOs[0].nCell <= i) {
				color = RGB_BLACK;
			}
		}
		if (strEGlassID.IsEmpty())
			strEGlassID = g_st[IDS_PPBOX_WND_IS_EMPTY];

		SelectedDrawText(&memDC, m_rectC[eC_GLASS + i], Format(_T("Glass %d [%s]/[%s]"), i+1, strGlassID, strEGlassID), DT_FONTSIZE, colorPen, color, RGB_BLACK);
	}

	DrawLineRect(&memDC, m_rectS[eS_PPBOX_AREA], RGB_BLACK);
	DrawLineRect(&memDC, m_rectS[eS_GLASS_AREA], RGB_BLACK);

	auto& tGlass = PPBox.TRAY_INFOs[0].CELL_INFOs[m_iSelectGlass];

	CString strCstID	= (CString)PPBox.CassetteInfo.szID;
	//CString strLotID	= (CString)tGlass.szLotID;
	CString strHGlassID = (CString)tGlass.szCELLID;
	CString strEGlassID = (CString)tGlass.szCELL_ID_BCR;
	if (strCstID.IsEmpty())		strCstID	=	g_st[IDS_PPBOX_WND_IS_EMPTY];
	//if (strLotID.IsEmpty())		strLotID	=	g_st[IDS_PPBOX_WND_IS_EMPTY];
	if (strHGlassID.IsEmpty())	strHGlassID =	g_st[IDS_PPBOX_WND_IS_EMPTY];
	if (strEGlassID.IsEmpty())	strEGlassID =	g_st[IDS_PPBOX_WND_IS_EMPTY];
	if (tGlass.bMaterial) {
		//SelectedDrawText(&memDC, m_rectC[eC_SELECT_GLASS], Format(_T("Glass %d \n\n CST ID : %s\n Lot ID : %s \n HGlass ID : %s \n EGlass ID : %s "), m_iSelectGlass + 1, strCstID, strLotID, strHGlassID, strEGlassID), 20, RGB_BLACK, RGB_GREEN, RGB_BLACK);
		SelectedDrawText(&memDC, m_rectC[eC_SELECT_GLASS], Format(_T("Glass %d \n\n CST ID : %s\Glass ID : %s \n MCR ID : %s "), m_iSelectGlass + 1, strCstID, strHGlassID, strEGlassID), 20, RGB_BLACK, RGB_GREEN, RGB_BLACK);
	}
	else {
		//SelectedDrawText(&memDC, m_rectC[eC_SELECT_GLASS], Format(_T("Glass %d \n\n CST ID : %s\n Lot ID : %s \n HGlass ID : %s \n EGlass ID : %s "), m_iSelectGlass+1, strCstID, strLotID, strHGlassID, strEGlassID), 20, RGB_BLACK, RGB_GRAY, RGB_BLACK);
		SelectedDrawText(&memDC, m_rectC[eC_SELECT_GLASS], Format(_T("Glass %d \n\n CST ID : %s\n Glass ID : %s \n MCR ID : %s "), m_iSelectGlass+1, strCstID, strHGlassID, strEGlassID), 20, RGB_BLACK, RGB_GRAY, RGB_BLACK);
	}

	DrawGroupBox(&memDC, m_rectS[eS_ABORT_AREA], _T("ABORT"), RGB_BLACK, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_GLASS_ABORT	], _T("Glass Box Abort"), DT_FONTSIZE, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_PAPER_ABORT	], _T("Paper Box Abort"), DT_FONTSIZE, RGB_BLACK);
	DrawToggleButton(&memDC, m_rectC[eC_GLASS_CONVEYOR_ABORT] , sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].ToUpper.fHS_Abort_Request, g_st[IDS_PPBOX_WND_GLASS_BOX_ABORT_RESERVATION], _T("Glass Box Normal"), DT_FONTSIZE, RGB_GREEN, RGB_BLACK);
	DrawToggleButton(&memDC, m_rectC[eC_PAPER_CONVEYOR_ABORT] , sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].ToUpper.fHS_Abort_Request, g_st[IDS_PPBOX_WND_PAPER_BOX_ABORT_RESERVATION], _T("Paper Box Normal"), DT_FONTSIZE, RGB_GREEN, RGB_BLACK);

	DrawMDIMenuButtonN(&memDC, m_rectC[eC_COMPULSION_GLASS_ABORT], g_st[IDS_PPBOX_WND_GLASS_BOX_ABORT], m_eSelectControl == eC_COMPULSION_GLASS_ABORT);
    DrawMDIMenuButtonN(&memDC, m_rectC[eC_COMPULSION_PAPER_ABORT], g_st[IDS_PPBOX_WND_PAPER_BOX_ABORT], m_eSelectControl == eC_COMPULSION_PAPER_ABORT);

	DrawGroupBox(&memDC, m_rectS[eS_COUNT_AREA], _T("PPBox Count"), RGB_BLACK, RGB_BLACK);
	
	DrawText(memDC, m_rectS[eS_GLASS_COUNT], _T("Glass Count"), DT_FONTSIZE, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_PAPER_COUNT], _T("Paper Count"), DT_FONTSIZE, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_TOTAL_COUNT], _T("Setting Count"), DT_FONTSIZE, RGB_BLACK);
	int iGlassCount = m_iTotalCount / 2;
	int iPaperCount = m_iTotalCount / 2;
	if (m_iTotalCount%2)
		iPaperCount++;

	SelectedDrawInt(&memDC, m_rectC[eC_GLASS_COUNT], iGlassCount, DT_FONTSIZE, RGB_BLACK, sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount%2 ? RGB_WHITE : RGB_GREEN, RGB_BLACK);
	SelectedDrawInt(&memDC, m_rectC[eC_PAPER_COUNT], iPaperCount, DT_FONTSIZE, RGB_BLACK, sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount%2 ? RGB_GREEN : RGB_WHITE, RGB_BLACK);
	SelectedDrawEditInt(&memDC, m_rectC[eC_TOTAL_COUNT], m_iTotalCount, TRUE, DT_FONTSIZE, DT_CENTER|DT_VCENTER);
	DrawMDIMenuButtonN(&memDC, m_rectC[eC_TOTAL_COUNT_SET], g_st[IDS_PPBOX_WND_APPLY], m_eSelectControl == eC_TOTAL_COUNT_SET);

	DrawText(memDC, m_rectS[eS_CURRENT], g_st[IDS_PPBOX_WND_CURRENT_COUNT_TYPE], 9, RGB_BLACK);
	if (sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wMaterialType == eMT_PAPER) {
		SelectedDrawText(&memDC, m_rectC[eC_CURRENT_MATERIAL], _T("PAPER"), 9, RGB_BLACK, RGB_WHITE, RGB_BLACK);
	}
	else if (sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wMaterialType == eMT_ORG_GLASS){
		SelectedDrawText(&memDC, m_rectC[eC_CURRENT_MATERIAL], _T("GLASS"), 9, RGB_BLACK, RGB_GREEN, RGB_BLACK);
	}
	else {
		SelectedDrawText(&memDC, m_rectC[eC_CURRENT_MATERIAL], _T("NONE"), 9, RGB_BLACK, RGB_YELLOW, RGB_BLACK);
	}
	SelectedDrawInt(&memDC, m_rectC[eC_CURRENT_COUNT],sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount , DT_FONTSIZE, RGB_BLACK, RGB_WHITE, RGB_BLACK);



	//<< 2024.06.11 LBH - PPBox 최상위 자재 타입 표시
	DrawText(&memDC, m_rectS[eS_PPBOX_TOP_MATERIAL], _T("PPBox Top Material Type :"), DT_FONTSIZE, RGB_BLACK);
	eMATERIAL_TYPE ePPBox_MaterialType = eMT_NONE;
	if (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount) {
		if (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount % 2) {
			ePPBox_MaterialType = eMT_PAPER;
		}
		else {
			ePPBox_MaterialType = eMT_ORG_GLASS;
		}
	}
	if (ePPBox_MaterialType == eMT_PAPER) {
		SelectedDrawText(&memDC, m_rectC[eC_PPBOX_TOP_MATERIAL], _T("PAPER"), 9, RGB_BLACK, RGB_WHITE, RGB_BLACK);
	}
	else if (ePPBox_MaterialType == eMT_ORG_GLASS) {
		SelectedDrawText(&memDC, m_rectC[eC_PPBOX_TOP_MATERIAL], _T("GLASS"), 9, RGB_BLACK, RGB_GREEN, RGB_BLACK);
	}
	else {
		SelectedDrawText(&memDC, m_rectC[eC_PPBOX_TOP_MATERIAL], _T("NONE"), 9, RGB_BLACK, RGB_YELLOW, RGB_BLACK);
	}



	DrawGroupBox(&memDC, m_rectS[eS_DB_AREA], _T("DB"), RGB_BLACK, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_PPBOX_ID], _T("PPBox Lable ID : "), DT_FONTSIZE, RGB_BLACK);
	SelectedDrawEditText(&memDC, m_rectC[eC_EDIT_PPBOX_ID], CString(m_strPPBoxID));
	DrawMDIMenuButtonN(&memDC, m_rectC[eC_LOAD_PPBOX_DATA], _T("PPBox Data Load To DB"), m_eSelectControl == eC_LOAD_PPBOX_DATA);
	//>>
}



void CPPBoxWnd::OnMouseMove(UINT nFlags, CPoint point) {
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



void CPPBoxWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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



void CPPBoxWnd::OnLButtonUp(UINT nFlags, CPoint point)  {
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		if (m_eSelectControl >= eC_GLASS && m_eSelectControl < eC_GLASS_END) {
			m_iSelectGlass = m_eSelectControl - eC_GLASS;
		}
		switch (m_eSelectControl) {
			case eC_GLASS_CONVEYOR_ABORT: {
				sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].ToUpper.fHS_Abort_Request = !sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].ToUpper.fHS_Abort_Request;
				strLog = _T("Glass Conveyor Abort");
				break;
			}
			case eC_PAPER_CONVEYOR_ABORT: {
				sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].ToUpper.fHS_Abort_Request = !sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].ToUpper.fHS_Abort_Request;
				strLog = _T("Paper Conveyor Abort");
				break;
			}
			case eC_TOTAL_COUNT: {
				EditStart(this, m_rectC[m_eSelectControl], m_iTotalCount, FALSE, FALSE);
				break;
			}
			case eC_TOTAL_COUNT_SET: {
				strLog = _T("적용");
				if (sm.Sequence->fAutoMode && !sm.Sequence->fCycleStop) {
					IPMessage(g_st[IDS_PPBOX_WND_CAN_NOT_CHANGE_NUMBR_OF_CELL]);
					break;
				}

				//<<2024.06.11 LBH - 범위 설정.
				int nMaxCount = g_pRecipe->ppbox.iGlassCount * 2;
				if (g_pRecipe->ppbox.bTopMaterialType == FALSE)
					nMaxCount++;


				if (m_iTotalCount < 0 || m_iTotalCount > nMaxCount) {
					IPMessage(Format(_T("Count Over Recipe Glass Count is %d -> (Input Range : 0 ~ %d)"), g_pRecipe->ppbox.iGlassCount, nMaxCount), 10 * 1000);
					break;
				}
				//>>

				CString strResult = InPutMessage(Format(_T("%s %d -> %d\n %s"), g_st[IDS_PPBOX_WND_CHANGE_NUMBER_OF_CELL], sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount, m_iTotalCount, g_st[IDS_PPBOX_WND_IF_YOU_CHANGE_MOVE_AGAIN]), TRUE);
				if (strResult.CompareNoCase(_T("mdi")) == 0) {
					sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount = m_iTotalCount;
					auto& PPBox = *sm.LoadPPBox;

					//sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount = m_iTotalCount;
					if (sm.Sequence->bCIMSkip) {
						PPBox.TRAY_INFOs[0].nCell = g_pRecipe->ppbox.iGlassCount;
					}
					else {
						PPBox.TRAY_INFOs[0].nCell = sm.LoadPPBox->TRAY_INFOs[0].nCell;
					}

					sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Glass.iGlassIndex = PPBox.TRAY_INFOs[0].nCell - m_iTotalCount / 2;
				
					if (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Glass.bMaterial)
						sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep = S_LDCV2_SEQ_PPBOX_CHECK;

					for (int i = 0; i < MAX_CELL; i++) {
						BOOL bMaterial = FALSE;
						if (i >= sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Glass.iGlassIndex && i <= PPBox.TRAY_INFOs[0].nCell) {
							bMaterial = TRUE;
						}

						sm.LoadPPBox->TRAY_INFOs[0].CELL_INFOs[i].bMaterial = bMaterial;
					}

							
					// 2024.06.11 LBH - 필요없는 구문인것 같아서 주석.
					// 아래 신호 초기화 부분도 수정 필요 할것 같음. (PIO를 아무 조건 없이 그냥 초기화 하면 시퀀스 꼬일 가능성이 큼)
					// ex) Load Conveyor이 아닌 Elevator이나 STC Table와 통신 PIO 중이였을 경우 무언정지 발생 가능성이 다분함.
					//sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wMaterialType = eMT_ORG_GLASS;
					//SetToUpperInit(eMODULE_PC1_GLASS_TRANSFER);
					//SetToLowerInit(eMODULE_PC1_GLASS_TRANSFER);

					//<< 2024.06.11 LBH - Glass Transfer가 Glass/Paper Loading 중이였을 경우 시퀀스 초기화 필요 할것 같음. 협의 후 작업
					// 이미 자재정보가 넘어 갔을 경우 어떻게 처리할지에 대한 방한도 필요함.
					auto& wSeqStep = sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep;
					if ((wSeqStep >= S_GLASS_TR_SEQ_GLASS_PAPER_LOAD_START) && (wSeqStep <= S_GLASS_TR_SEQ_GLASS_PAPER_LOAD_END)) {
						SetToLowerInit(eMODULE_PC1_LOAD_CONVEYOR_2);
						SetToUpperInit(eMODULE_PC1_GLASS_TRANSFER);
						
						wSeqStep = S_GLASS_TR_SEQ_INIT;
					}

					// 2024.06.29 LBH - 강제 투입 했을 경우 대비 초기화
					sm.Sequence->dGlassTransferOffsetZ = 0.;
					//>>
				}
				break;
			}
			case eC_COMPULSION_GLASS_ABORT: {
				// 강제 배출 전에 상태 체크
				if (sm.Sequence->fAutoMode && !sm.Sequence->fCycleStop) {
					IPMessage(g_st[IDS_PPBOX_WND_CAN_NOT_ABORT_SETTING]);
					return;
				}
				if (RespMessage(g_st[IDS_PPBOX_WND_WANT_GLASS_BOX_ABORT]).Compare(_T("OK")) == 0) {
					// Box 가 있다고 판단되는 경우에만 진행
					if (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Glass.bMaterial) {
						SetToLowerInit(eMODULE_PC1_LOAD_CONVEYOR_2);
						sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.nCrtCount = 0;	// Lift 갯수 초기화
						//sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep = S_GUL_RECV_MODULE_SET_COVER_PICKER;	// CIM 보고 이후 스탭으로 변경

						// Transfer 스탭 초기화
						// 자재가 없는 경우 스탭 초기화 
						// 자재가 있는 경우 그대로 진행
						if (!sm.Module[eMODULE_PC1_GLASS_TRANSFER].Glass.bMaterial) {
							//sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep = S_GT_INIT;
							SetToUpperInit(eMODULE_PC1_GLASS_TRANSFER);
							SetToLowerInit(eMODULE_PC1_GLASS_TRANSFER);
						}
					}
					else {
						IPMessage(g_st[IDS_PPBOX_WND_NONE_GLASS_BOX]);
					}
				}
				break;
			}
			case eC_COMPULSION_PAPER_ABORT	: {
				// 강제 배출 전에 상태 체크
				if (sm.Sequence->fAutoMode && !sm.Sequence->fCycleStop) {
					IPMessage(g_st[IDS_PPBOX_WND_CAN_NOT_ABORT_SETTING]);
					return;
				}
				// 21.03.22 
				// 메세지를 띄울 때 알려줘야할듯 
				// Scrape 하지 않으면 들고 있던 Paper 를 Box 가 없어도 내려놓음
				if (RespMessage(g_st[IDS_PPBOX_WND_WANT_PAPER_BOX_ABORT]).Compare(_T("OK")) == 0) {
					// Box 가 있다고 판단되는 경우에만 진행
					if (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Glass.bMaterial) {

						SetToUpperInit(eMODULE_PC1_LOAD_CONVEYOR_2);
						sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Glass.nStickCount = 0;
						//sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep = S_PL_SEND_MODULE_SET_UNLOAD_CONVEYOR;	// Paper Box 배출 스탭으로 변경
						
						// Glass Transfer 의 경우 
						// Load/Unload Table 로 동작하는 경우에는 문제가 없음 
						// Unload PPBox 동작 하는 경우에 Glass Transfer 가 Paper를 들고 있고 이미 Glass Transfer 가 이동하고 있는 경우에는 
						// 다음 PPBox 가 들어오는 경우 알아서 진행 될 예정
						// 문제는 이미 들고있던 Paper는 Box 가 없어도 그냥 내려놓을꺼라 해당 부분에 대한 조치는 방법이 없음 
					}
					else {
						IPMessage(g_st[IDS_PPBOX_WND_WANT_NONE_PAPER_BOX]);
					}
				}
				break;
			}
			case eC_EDIT_PPBOX_ID: {
				EditStart(this, m_rectC[m_eSelectControl], m_strPPBoxID);
				break;
			}
			case eC_LOAD_PPBOX_DATA: {
				if (RespMessage(_T("PPBox Data Load to DB?"), _T("YES;NO")) == _T("YES"))
					RECV_PPBOX_LOT_DATA(eMODULE_PC1_LOAD_CONVEYOR_2, m_strPPBoxID);
				break;
			}
			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl	=	eC_NONE;
	Invalidate(FALSE);
	CFrameWndEx::OnLButtonUp(nFlags, point);
}



void CPPBoxWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect	rc = m_rectS[eS_FULL_AREA];

	double dH = (rc.Height() - 5.) / 15.;
	double dW = (rc.Width() - 5.) / 12.;
	//m_rectS[eS_ITEM_AREA] = CRect(rc.left + dW * 0, rc.top + dH * 0, rc.left + dW * 1, rc.bottom);
	
	m_rectS[eS_WORK_TITLE] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 12, rc.top + dH * 1);
	m_rectS[eS_WORK_AREA] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 1, rc.left + dW * 12, rc.bottom - 5);


	//rc = m_rectS[eS_ITEM_AREA];
	//m_rectC[eC_CONVEYOR_1] = CRect(rc.left+5, rc.top+70*0+10, rc.right+10, rc.top+70*1);

	rc = m_rectS[eS_WORK_AREA];
	rc.top += 5;

	dW = (rc.Width() - 5.) / 8.;
	dH = (rc.Height() - 5.) / 8.;
	m_rectS[eS_PPBOX_AREA]	= CRect(rc.left + dW * 0 + 5, rc.top + dH * 0 + 5, rc.left + dW * 3, rc.top + dH * 8);
	m_rectS[eS_GLASS_AREA]	= CRect(rc.left + dW * 3 + 5, rc.top + dH * 0 + 5, rc.left + dW * 6, rc.top + dH * 8);

	m_rectS[eS_COUNT_AREA]	= CRect(rc.left + dW * 6 + 5, rc.top + dH * 0 + 5, rc.left + dW * 7.5, rc.top + dH * 3);
	m_rectS[eS_ABORT_AREA]	= CRect(rc.left + dW * 6 + 5, rc.top + dH * 3.5 + 5, rc.left + dW * 7.5, rc.top + dH * 6);
	m_rectS[eS_DB_AREA]		= CRect(rc.left + dW * 6 + 5, rc.top + dH * 6.5 + 5, rc.left + dW * 7.5, rc.top + dH * 8);
	

	rc = m_rectS[eS_PPBOX_AREA];
	dW = (rc.Width() - 5.) / 2.;
	dH = (rc.Height() - 5.) / MAX_CELL;

	for (int i = 0; i < MAX_CELL; i++) {
		m_rectC[eC_GLASS+i] = CRect(rc.left+dW*0+5, rc.top+dH*i+5, rc.left+dW*2, rc.top+dH*(i+1));
	}

	rc = m_rectS[eS_GLASS_AREA];
	dW = (rc.Width() - 5.) / 1.;
	dH = (rc.Height() - 5.) / 6.;

	m_rectC[eC_SELECT_GLASS] = CRect(rc.left+dW*0+5, rc.top+dH*1+5, rc.left+dW*1, rc.top+dH*5);
	
	rc = m_rectS[eS_ABORT_AREA];
	rc.top += 20;

	dW = (rc.Width() - 5.) / 3.;
	dH = (rc.Height() - 5.) / 5.;
	// 21.03.18 강제 배출
	m_rectS[eS_GLASS_ABORT				] = CRect(rc.left+dW*0+5, rc.top+dH*0+5, rc.left+dW*1, rc.top+dH*1); 
	m_rectC[eC_GLASS_CONVEYOR_ABORT		] = CRect(rc.left+dW*1+5, rc.top+dH*0+5, rc.left+dW*2.5, rc.top+dH*1); 
	m_rectC[eC_COMPULSION_GLASS_ABORT	] = CRect(rc.left+dW*1+5, rc.top+dH*1+5, rc.left+dW*2.5, rc.top+dH*2); 	

	m_rectS[eS_PAPER_ABORT				] = CRect(rc.left+dW*0+5, rc.top+dH*2+5, rc.left+dW*1, rc.top+dH*3); 
	m_rectC[eC_PAPER_CONVEYOR_ABORT		] = CRect(rc.left+dW*1+5, rc.top+dH*2+5, rc.left+dW*2.5, rc.top+dH*3); 
	m_rectC[eC_COMPULSION_PAPER_ABORT	] = CRect(rc.left+dW*1+5, rc.top+dH*3+5, rc.left+dW*2.5, rc.top+dH*4); 	

	rc = m_rectS[eS_DB_AREA];
	rc.top += 20;

	m_rectS[eS_PPBOX_ID]				= CRect(rc.left + dW * 0 + 5, rc.top + dH * 0 + 5, rc.left + dW * 1, rc.top + dH * 1);
	m_rectC[eC_EDIT_PPBOX_ID]			= CRect(rc.left + dW * 1 + 5, rc.top + dH * 0 + 5, rc.left + dW * 2.5, rc.top + dH * 1);
	m_rectC[eC_LOAD_PPBOX_DATA]			= CRect(rc.left + dW * 0 + 5, rc.top + dH * 1 + 5, rc.left + dW * 2.5, rc.top + dH * 3);

	dW = (rc.Width() - 5.) / 3.;
	dH = (rc.Height() - 5.) / 5.;
	

	rc = m_rectS[eS_COUNT_AREA];
	rc.top += 20;
	dW = (rc.Width() - 5.) / 3.;
	dH = (rc.Height() - 5.) / 5.;
	
	m_rectS[eS_GLASS_COUNT		] = CRect(rc.left+dW*0+5, rc.top+dH*0+5, rc.left+dW*1, rc.top+dH*1); 
	m_rectS[eS_PAPER_COUNT		] = CRect(rc.left+dW*0+5, rc.top+dH*1+5, rc.left+dW*1, rc.top+dH*2); 
	m_rectS[eS_TOTAL_COUNT		] = CRect(rc.left+dW*0+5, rc.top+dH*2+5, rc.left+dW*1, rc.top+dH*3); 
	m_rectC[eC_GLASS_COUNT		] = CRect(rc.left+dW*1+5, rc.top+dH*0+5, rc.left+dW*2, rc.top+dH*1); 
	m_rectC[eC_PAPER_COUNT		] = CRect(rc.left+dW*1+5, rc.top+dH*1+5, rc.left+dW*2, rc.top+dH*2); 
	m_rectC[eC_TOTAL_COUNT		] = CRect(rc.left+dW*1+5, rc.top+dH*2+5, rc.left+dW*2, rc.top+dH*3); 
	m_rectC[eC_TOTAL_COUNT_SET	] = CRect(rc.left+dW*2+5, rc.top+dH*2+5, rc.left+dW*3, rc.top+dH*3); 

	m_rectS[eS_CURRENT			] = CRect(rc.left+dW*0+5, rc.top+dH*3+5, rc.left+dW*1, rc.top+dH*4); 
	m_rectC[eC_CURRENT_COUNT	] = CRect(rc.left+dW*1+5, rc.top+dH*3+5, rc.left+dW*2, rc.top+dH*4); 
	m_rectC[eC_CURRENT_MATERIAL	] = CRect(rc.left+dW*2+5, rc.top+dH*3+5, rc.left+dW*3, rc.top+dH*4);
	
	m_rectS[eS_PPBOX_TOP_MATERIAL] = CRect(rc.left+dW*0+5, rc.top+dH*4+5, rc.left+dW*2, rc.top+dH*5);
	m_rectC[eC_PPBOX_TOP_MATERIAL] = CRect(rc.left+dW*2+5, rc.top+dH*4+5, rc.left+dW*3, rc.top+dH*5);
	
}

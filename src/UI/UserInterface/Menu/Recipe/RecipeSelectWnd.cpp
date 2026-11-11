// RecipeSelectWnd.cpp: 구현 파일
//

#include "pch.h"
#include "UI.h"
#include "RecipeSelectWnd.h"

// CRecipeSelectWnd

IMPLEMENT_DYNAMIC(CRecipeSelectWnd, CFrameWndEx)

CRecipeSelectWnd::CRecipeSelectWnd()
{


}

CRecipeSelectWnd::~CRecipeSelectWnd()
{
}

#define IDC_LIST_RECIPE			WM_APP + 4205
BEGIN_MESSAGE_MAP(CRecipeSelectWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RECIPE, &CRecipeSelectWnd::OnRecipeListChanged)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_RECIPE, &CRecipeSelectWnd::OnLvnItemBeginAbleRecipeList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_RECIPE, &CRecipeSelectWnd::OnLvnItemEndAbleRecipeList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_RECIPE, &CRecipeSelectWnd::OnCustomdrawList)  
END_MESSAGE_MAP()



// CRecipeSelectWnd 메시지 처리기

void CRecipeSelectWnd::computeArea()
{
	GetClientRect(m_rectClient);
	CRect rect = m_rectClient;
	rect.DeflateRect(1, 1);

	m_rectS[eS_TITLE_AREA]			= CRect(rect.left, rect.top, rect.right, rect.top + 40);
	m_rectS[eS_LIST_AREA]			= CRect(rect.left, rect.top + 40, rect.right, rect.bottom);

	if (m_lstRecipe.m_hWnd)
	{
		m_lstRecipe.MoveWindow(m_rectS[eS_LIST_AREA]);
	}

	rect = m_rectS[eS_TITLE_AREA];
	rect.DeflateRect(0, 5);
	double dW = rect.Width() / 15.3;

	// 검색
	m_rectS[eS_SEARCH_TEXT]		= CRect(rect.left + dW * 0 + 3, rect.top, rect.left + dW * 1 - 3, rect.bottom);
	m_rectC[eC_EDIT_SEARCH_ID]	= CRect(rect.left + dW * 1 + 3, rect.top, rect.left + dW * 2 - 3, rect.bottom);
	m_rectC[eC_BTN_SEARCH]		= CRect(rect.left + dW * 2 + 3, rect.top, rect.left + dW * 3 - 3, rect.bottom);
	
	// 적용 상태
	m_rectS[eS_APPLY_TEXT]		= CRect(rect.left + dW * 4 + 3, rect.top, rect.left + dW * 5 - 3, rect.bottom);
	m_rectS[eS_APPLY_RECIPE]	= CRect(rect.left + dW * 5 + 3, rect.top, rect.left + dW * 9 - 3, rect.bottom);
	
	// 컨트롤
	//m_rectC[eC_BTN_CIM_APPLY]	= CRect(rect.left + dW * 8 + 3, rect.top, rect.left + dW * 9 - 3, rect.bottom);
	//m_rectC[eC_BTN_CIM_DELETE]	= CRect(rect.left + dW * 9 + 3, rect.top, rect.left + dW * 10 - 3, rect.bottom);

	m_rectC[eC_BTN_ALL_APPLY]	= CRect(rect.left + dW *  9 + 3, rect.top, rect.left + dW * 10 - 3, rect.bottom);
	m_rectC[eC_BTN_LOAD]		= CRect(rect.left + dW * 10 + 3, rect.top, rect.left + dW * 11 - 3, rect.bottom);
	m_rectC[eC_BTN_APPLY]		= CRect(rect.left + dW * 11 + 3, rect.top, rect.left + dW * 12 - 3, rect.bottom);
	//m_rectC[eC_BTN_NEW]			= CRect(rect.left + dW * 12 + 3, rect.top, rect.left + dW * 13 - 3, rect.bottom);	// KES 주석 2024.07.25 - MDI 요청 새로만들기 기능 삭제
	m_rectC[eC_BTN_DELETE]		= CRect(rect.left + dW * 13 + 3, rect.top, rect.left + dW * 14 - 3, rect.bottom);
	m_rectC[eC_BTN_COPY]		= CRect(rect.left + dW * 14 + 3, rect.top, rect.left + dW * 15 - 3, rect.bottom);
}

int CRecipeSelectWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_lstRecipe.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, CRect(), this, IDC_LIST_RECIPE);
	m_lstRecipe.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstRecipe.SetRowHeight(40);

	m_lstRecipe.InsertColumn(eRC_NONE				, _T("")					                    , LVCFMT_CENTER, 0);
	m_lstRecipe.InsertColumn(eRC_INDEX				, _T("Recipe No.")			                    , LVCFMT_CENTER, 70);
	m_lstRecipe.InsertColumn(eRC_PPID				, _T("PPID")				                    , LVCFMT_LEFT, 145);
	m_lstRecipe.InsertColumn(eRC_RECIPE_ID			, _T("NAME (FILE NAME)")						, LVCFMT_LEFT, 350);
	m_lstRecipe.InsertColumn(eRC_PANNEL_SIZE		, _T("PANNEL SIZE")								, LVCFMT_LEFT, 190);
	m_lstRecipe.InsertColumn(eRC_MODIFIED_COUNT		, _T("MODIFIED COUNT")							, LVCFMT_LEFT, 120);
	m_lstRecipe.InsertColumn(eRC_LASTUPDATE_TIME	, _T("LAST UPDATE")								, LVCFMT_LEFT, 220);
	m_lstRecipe.InsertColumn(eRC_DESCRIPTION		, _T("DESCRIPTION")							    , LVCFMT_LEFT, 805);

	m_lstRecipe.SetAlignText(eRC_NONE			, eLCETA_LEFT);
	m_lstRecipe.SetAlignText(eRC_INDEX			, eLCETA_LEFT);
	m_lstRecipe.SetAlignText(eRC_PPID			, eLCETA_LEFT);
	m_lstRecipe.SetAlignText(eRC_RECIPE_ID		, eLCETA_LEFT);
	m_lstRecipe.SetAlignText(eRC_PANNEL_SIZE	, eLCETA_LEFT);
	m_lstRecipe.SetAlignText(eRC_MODIFIED_COUNT	, eLCETA_LEFT);
	m_lstRecipe.SetAlignText(eRC_LASTUPDATE_TIME, eLCETA_LEFT);
	m_lstRecipe.SetAlignText(eRC_DESCRIPTION	, eLCETA_LEFT);

	m_lstRecipe.SetColumnEdit(eRC_RECIPE_ID, 40);

	g_recipes.SetListCtrl(&m_lstRecipe);
	if (g_recipes.LoadRecipeList() == false) 
		return -1;

	g_pRecipe = g_recipes.GetAppliedData();
	g_pRecipe->ppid = g_recipes.GetAppliedData()->ppid;

	SendRecipe();
	return 0;
}


void CRecipeSelectWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
	SetWindowPos(&wndTop, g_rectArea[eAREA_VISION].left, g_rectArea[eAREA_VISION].top, g_rectArea[eAREA_VISION].Width(), g_rectArea[eAREA_VISION].Height(), SWP_SHOWWINDOW);
	computeArea();
	}
}


void CRecipeSelectWnd::OnPaint()
{
	CPaintDC dc(this);
	CMemDCEx::CMemDC memDC(&dc);
	memDC.FillSolidRect(m_rectClient, RgbColor::Gainsboro);

	DrawText(memDC, m_rectS[eS_SEARCH_TEXT], g_st[IDS_RECIPE_SELECT_SEARCH], DT_FONTSIZE, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	SelectedDrawEditText(memDC, m_rectC[eC_EDIT_SEARCH_ID], m_searchID);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_SEARCH], g_st[IDS_RECIPE_SELECT_WND_SERCH], m_iSelectControl == eC_BTN_SEARCH, m_iMoveControl == eC_BTN_SEARCH);
	
	DrawText(memDC, m_rectS[eS_APPLY_TEXT], g_st[IDS_RECIPE_SELECT_APPLY_RECIPE], DT_FONTSIZE, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	if (g_recipes.GetAppliedIndex() != -1)
	{
		SelectedDrawText(memDC, m_rectS[eS_APPLY_RECIPE], Format(_T("(%d) [%s]"), g_recipes.GetAppliedIndex() + 1, g_recipes.GetAppliedData()->recipeName));
	}
	else
	{
		SelectedDrawText(memDC, m_rectS[eS_APPLY_RECIPE], g_st[IDS_RECIPE_SELECT_WND_NONE]);
	}

	//DrawButtonUpDownMouseMove(memDC, m_rectC[eC_BTN_CIM_APPLY]	, g_st[IDS_RECIPE_SELECT_CIM_APPLY	], m_iSelectControl, m_iMoveControl, eC_BTN_CIM_APPLY);
	//DrawButtonUpDownMouseMove(memDC, m_rectC[eC_BTN_CIM_DELETE]	, g_st[IDS_RECIPE_SELECT_CIM_DELETE	], m_iSelectControl, m_iMoveControl, eC_BTN_CIM_DELETE);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_ALL_APPLY]	, g_st[IDS_RECIPE_SELECT_WND_ALL_APPLY] , m_iSelectControl == eC_BTN_LOAD, m_iMoveControl == eC_BTN_LOAD);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_LOAD]		, g_st[IDS_RECIPE_SELECT_LOAD	]	    , m_iSelectControl == eC_BTN_LOAD, m_iMoveControl == eC_BTN_LOAD);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_APPLY]		, g_st[IDS_RECIPE_SELECT_APPLY	]	    , m_iSelectControl == eC_BTN_APPLY, m_iMoveControl == eC_BTN_APPLY);
	//DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_NEW]		, g_st[IDS_RECIPE_SELECT_NEW	]	    , m_iSelectControl == eC_BTN_NEW, m_iMoveControl == eC_BTN_NEW);	// KES 주석 2024.07.25 - MDI 요청 새로만들기 기능 삭제
	DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_DELETE]	, g_st[IDS_RECIPE_SELECT_DELETE	]	    , m_iSelectControl == eC_BTN_DELETE, m_iMoveControl == eC_BTN_DELETE);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_COPY]		, g_st[IDS_RECIPE_SELECT_COPY	]	    , m_iSelectControl == eC_BTN_COPY, m_iMoveControl == eC_BTN_COPY);
}


void CRecipeSelectWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	m_iSelectControl = eC_NONE;
	for (int idx = 0; idx < eC_END; idx++) 
	{
		if (m_rectC[idx].PtInRect(point)) 
		{
			m_iSelectControl = idx;
			InvalidateRect(m_rectC[idx], FALSE);
			break;
		}
	}

	CFrameWndEx::OnLButtonDown(nFlags, point);
}


void CRecipeSelectWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString strLog;
	auto cleanup = make_scope_exit
	(
		[this, nFlags, point]()
		{
			if (m_iSelectControl != eC_NONE) {
				auto name = magic_enum::enum_name((CRecipeSelectWnd::eCONTROL)m_iSelectControl);
				CString te(name.data(), name.size());
				WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T(""), te, theApp.m_UserManager.GetCurrentUserData().szID);
			}
			m_iSelectControl = eC_NONE;
			Invalidate(FALSE);
			CFrameWndEx::OnLButtonUp(nFlags, point);
		}
	);

	// 선택한 범위가 아닌 경우 종료
	if (!m_rectC[m_iSelectControl].PtInRect(point))
	{
		return;
	}
	
	//<< KES 레시피 변경 가능 조건 추가 2024.07.25
	BOOL bOtherEQPAuto = (sm.PCs->ControlPC[eMACHINE_SCT].fAutoMode && !sm.PCs->ControlPC[eMACHINE_SCT].fCycleStop);
	if ((sm.Sequence->fAutoMode && !sm.Sequence->fCycleStop) || bOtherEQPAuto) {
		if (m_iSelectControl != eC_EDIT_SEARCH_ID && m_iSelectControl != eC_BTN_SEARCH) {
			IPMessage(g_st[IDS_RECIPE_SELECT_WND_LD_SCT_AUTO_RUN]);
			return;
		}
	}
	//>>

	switch (m_iSelectControl) {
		case eC_EDIT_SEARCH_ID:
		{
			EditStart(this, m_rectC[eC_EDIT_SEARCH_ID], m_searchID);
		}
		break;

		case eC_BTN_SEARCH:
		{
			SetFocus();
			auto searchList = g_recipes.SearchRecipe(m_searchID);
			CString msg(_T("Find Recipe\n"));
			for (auto [index, name] : searchList)
			{
				msg += Format(_T("[%04d] -> %s\n"), index, name);
			}
			AfxMessageBox(msg);
			strLog = _T("검색");
		}
		break;
		case eC_BTN_LOAD:
		{
			int selectIndex = m_lstRecipe.GetSelectionMark();
			if (selectIndex == -1)
			{
				AfxMessageBox(g_st[IDS_RECIPE_SELECT_WND_NONE_SELECT_RECIPE]);
				return;
			}
			m_iMoveControl = eC_NONE; // 불러오기 dlg 후에 mouse over 초기화 안되는 문제 수정
			CString			recipePath;
			CFileDialog		fileOpenDlg(TRUE, _T("Recipe File"), nullptr, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR, _T("RecipeFile(*.json)|*.json|All Files(*.*)|*.*||"));	// KES 수정 2024.2.19 - *.rcp -> *.json
			GetFullPathName(_T("../data/recipe/"), 1024, recipePath.GetBuffer(1024), nullptr);
			recipePath.ReleaseBuffer();
			fileOpenDlg.m_ofn.lpstrInitialDir = recipePath;
			if (fileOpenDlg.DoModal() != IDOK)
				return;
			recipePath = fileOpenDlg.GetPathName();
			
			//<< KES 수정 2024.2.19 - json 형식으로 파일 저장 
			T_MDI_RECIPE recipe;
			json j;
			std::ifstream in(recipePath);
			if (in.is_open()) {
			  in >> j;
			  recipe = j;
			}
			else {
				AfxMessageBox(Format(g_st[IDS_RECIPE_SELECT_WND_FILE_OPEN]));
			}
			//>>

			T_MDI_RECIPE* copyRecipe = g_recipes.CopyRecipe(&recipe, selectIndex);
			CString orgRecipeName = fileOpenDlg.GetFileName();
			orgRecipeName = orgRecipeName.Left(orgRecipeName.Find(_T(".json")));	// KES 수정 2024.2.19 - .rcp -> .json
			if (copyRecipe->recipeName != orgRecipeName)
			{
				AfxMessageBox(Format(_T("%s. %s -> %s "), g_st[IDS_RECIPE_SELECT_WND_DUPLICATED_NAME_CHANGE],orgRecipeName, copyRecipe->recipeName));
			}
			strLog = _T("불러오기");
		}
		break;

		case eC_BTN_ALL_APPLY: {
			int selectIndex = m_lstRecipe.GetSelectionMark();

			if (L"OK" != RespMessage(Format(_T("%s %d",g_st[IDS_RECIPE_SELECT_WND_DO_YOU_WANT_TO_CHANGE_ALL_RECIPES_TO], selectIndex + 1))))
			{
				return;
			}

			if (selectIndex == -1)
			{
				IPMessage(g_st[IDS_RECIPE_SELECT_WND_NO_RECIPE_NUMBER_SELECTED]);
				return;
			}			
			if (g_recipes[selectIndex] == nullptr || g_recipes[selectIndex]->ppid.IsEmpty())
			{
				IPMessage(g_st[IDS_RECIPE_SELECT_WND_NO_RECIPE_NUMBER_OR_PPID_SELECTED]);
				return;
			}
			if (g_db.get() == nullptr || !g_db->IsOpen())
			{
				IPMessage(g_st[IDS_RECIPE_SELECT_WND_DB_CONNECT_FAIL]);
				return;
			}

			// 레시피 전체 변경시 체크할 항목
			// - Auto Mode 아닌 것 확인
			// - 자재 없어야 함
			// - 온라인 모드 체크
			// - 알람 상태 체크
			// - 파라미터 편집 중 아니여야 함		
			// - 수동모드 동작 중도 아니여야 함	

			CString ControlPCName[nMACHINE_END] =
			{
				L"LD", L"SCT", L"CCT-B", L"CCT-A", L"ULD-B", L"ULD-A"
			};

			for (int pcNumber = 0; pcNumber < nMACHINE_END; ++pcNumber)
			{
				auto& Name = ControlPCName[pcNumber];
				if (sm.PCs->ControlPC[pcNumber].fPMMode)
				{
					IPMessage(Name + g_st[IDS_RECIPE_SELECT_WND_PM_MODE_STATE]);
					return;
				}

				if (sm.PCs->ControlPC[pcNumber].fIsAlarm)
				{
					IPMessage(Name + g_st[IDS_RECIPE_SELECT_WND_ALARM_STATE]);
					return;
				}

				if (sm.PCs->ControlPC[pcNumber].fAutoMode)
				{
					IPMessage(Name + g_st[IDS_RECIPE_SELECT_WND_AUTO_RUN_STATE]);
					return;
				}

				if (sm.PCs->ControlPC[pcNumber].fIsMaterial)
				{
					IPMessage(Name + g_st[IDS_RECIPE_SELECT_WND_MATERIAL_STATE]);
					return;
				}

				if (!sm.PCs->ControlPC[pcNumber].fOnlineMode)
				{
					IPMessage(Name + g_st[IDS_RECIPE_SELECT_WND_IS_NOT_CIM_REPORT_STATE]);
					return;
				}
			}

			// TODO PC 별로 Recipe Change 날려서 응답받아서 메세지 발생
			CIPVar var, varResult;
			var.SetChildItem("Index", selectIndex);
			var.SetChildItem("PPID", g_recipes[selectIndex]->ppid);

			CStringA PCs[nMACHINE_END] = 
			{
				"LD", "SCT", "CCTL", "CCTR", "ULDL", "ULDR"
			};

			bool isOK = true;
			CString ResultMessage;
			for (int pcNumber = 0; pcNumber < nMACHINE_END; ++pcNumber)
			{
				BOOL bResult =g_ipCIMClient.SendIPCommand(PCs[pcNumber], "Recipe", "Change", var, varResult);
				bResult &= varResult.GetChildItem("Success", FALSE);
				if (bResult == FALSE)
				{
					CString Description = varResult.GetChildItem("Description", L"");
					if (Description.IsEmpty())
					{
						Description = g_st[IDS_RECIPE_SELECT_WND_CHANGE_FAIL];
					}
					ResultMessage += L"[" + ControlPCName[pcNumber] + L"] " + Description + L"\n";
					isOK = false;
				}
			}

			if (isOK)
			{
				IPMessage(Format(L"%s: %d", g_st[IDS_RECIPE_SELECT_WND_ALL_RECIPE_CHANGE_TO], selectIndex + 1), INFINITE);
			}
			else
			{
				IPMessage(ResultMessage, INFINITE);
			}
		}
		break;

		case eC_BTN_APPLY: {
			//<< KES 추가 2024.07.21 - CIM 프로그램 꺼져 있으면 수정 불가
			if (!g_ipCIMClient.IsServerConnected() || !g_ipCIMClient.IsIPCommandLoopStarted())
			{
				IPMessage(g_st[IDS_MS_CIM_PROGRAM_IS_CLOSED]);
				return;
			}
			//>>

			int selectIndex = m_lstRecipe.GetSelectionMark();
			if (selectIndex == -1) {
				AfxMessageBox(g_st[IDS_RECIPE_SELECT_WND_NONE_SELECT_RECIPE]);
				return;
			}

			if (g_recipes[selectIndex] != nullptr && !g_recipes[selectIndex]->recipeName.IsEmpty()) {
				BOOL bApply = FALSE;
				if (!sm.Sequence->bCIMSkip) {
					CIPVar var;
					var.SetChildItem("PPID_NO", selectIndex);
					var.SetChildItem("PPID", g_recipes.GetRecipeData(selectIndex)->recipeName);
					var.SetChildItem("APPLY", TRUE);
					bApply = g_ipClient.SendIPCommand("RECIPE", "SAVE_AND_APPLY", var);

					if (bApply) {
						g_recipes.setCIMApplyRecipe(selectIndex);
					}
				
				}
				else {
					bApply = TRUE;
				}

				if (bApply) {
					CString strApplyLog = Format(_T("%s;%s;%s;%s;%s"), _T("[Recipe Change]"), _T(""), g_recipes.GetAppliedData()->recipeName, g_recipes.GetRecipeData(selectIndex)->recipeName, theApp.m_UserManager.GetCurrentUserData().szID);
					std::vector<CString> vecApplyLog;
					vecApplyLog.push_back(strApplyLog);
					WriteRecipeLog(vecApplyLog);
					g_recipes.ApplyRecipe();
					SendRecipe(); // <- 다른 프로세스 레시피 적용
					strLog = _T("적용");
				}
			}
		}
		break;

		// KES 주석 2024.07.25 - MDI 요청 새로만들기 기능 삭제
		//case eC_BTN_NEW: 
		//{
		//	// KES 수정 2024.4.4
		//	CreateNewRecipe(TRUE);
		//	strLog = _T("새로 만들기");
		//}
		//break;

		case eC_BTN_COPY: 
		{
			int selectIndex = m_lstRecipe.GetSelectionMark();

			BOOL bCheck = CopyAppliedRecipe();
			if (!bCheck) {
				break;
			}

			if (g_recipes[selectIndex] != nullptr && !g_recipes[selectIndex]->recipeName.IsEmpty()) {
				if (!sm.Sequence->bCIMSkip) {
					CIPVar var;
					var.SetChildItem("PPID_NO", selectIndex);
					var.SetChildItem("PPID", g_recipes.GetRecipeData(selectIndex)->recipeName);
					var.SetChildItem("APPLY", FALSE);
					if (g_ipClient.SendIPCommand("RECIPE", "SAVE_AND_APPLY", var)) {
						g_recipes.setCIMApplyRecipe(selectIndex);
					}
				}
			}

			strLog = _T("복사");
		}
		break;

		case eC_BTN_DELETE: {
			int selectIndex = m_lstRecipe.GetSelectionMark();
			if (selectIndex == -1)
			{
				AfxMessageBox(g_st[IDS_RECIPE_SELECT_WND_NONE_SELECT_RECIPE]);
				return;
			}
			else if (selectIndex < 90) {
				//24.07.23 KGH 임시주석
				//AfxMessageBox(g_st[IDS_RECIPE_SELECT_WND_CAN_NOT_DELETE]);
				//return;
			}

			if (g_recipes[selectIndex] != nullptr) {
				BOOL bDelete = FALSE;
				if (IsPPID_REPORT(selectIndex + 1) && g_recipes.GetAppliedIndex() != selectIndex) {
					// 삭제 시 CIM에서 DB 삭제 및 PC Recipe 삭제 명력 내리도록 ex) LD, SCT
					CIPVar var;
					var.SetChildItem("PPID_NO", selectIndex + 1);
					var.SetChildItem("PPID", g_recipes.GetRecipeData(selectIndex)->recipeName);
					bDelete = g_ipCIMClient.SendIPCommand("CIMTransfer", lpEQP[eEQP_SCT], "RECIPE", "DELETE", var);
				}
				else {
					if (g_recipes.GetAppliedIndex() != selectIndex)
						bDelete = TRUE;
				}

				if (bDelete) {
					g_recipes.DeleteRecipe();
					strLog = _T("삭제");
				}
				else {
					if (g_recipes.GetAppliedIndex() == selectIndex)
						IPMessage(g_st[IDS_RECIPE_SELECT_WND_DO_NOT_DELETE_RECIPE]);
					else
						IPMessage(g_st[IDS_RECIPE_SELECT_WND_CAN_NOT_DELETE_CIM_APPLIED]);
				}
			}
			
		}
		break;
	}

	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
}


void CRecipeSelectWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// 이전 위치 저장
	int oldControl = m_iMoveControl;
	m_iMoveControl = eC_NONE;

	// 현재 위치 찾기 및 새로 그리기
	for (int idx = 0; idx < eC_END; idx++) 
	{
		if (m_rectC[idx].PtInRect(point)) 
		{
			m_iMoveControl = idx;
			InvalidateRect(m_rectC[idx], FALSE);
			break;;
		}
	}

	// 이전 위치와 현재 위치가 다른 경우 이전 위치 다시 그리기
	// 마우스 위치가 이전 위치와 다른 경우 선택 위치도 초기화
	if (oldControl != eC_NONE && oldControl != m_iMoveControl)
	{
		m_iSelectControl = eC_NONE;
		InvalidateRect(m_rectC[oldControl], FALSE);
	}

	CFrameWndEx::OnMouseMove(nFlags, point);
}

void CRecipeSelectWnd::OnRecipeListChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}

void CRecipeSelectWnd::OnLvnItemBeginAbleRecipeList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	int iItem = pDispInfo->item.iItem;
	int iSubItem = pDispInfo->item.iSubItem;

	*pResult = -1;
	// KES 2024.07.25 - 설비 이름 및 PPID 변경 불가하게 수정 
	//if (iSubItem == eRC_RECIPE_ID && iItem >= 90) {
	//	*pResult = 0;
	//}
}

void CRecipeSelectWnd::OnLvnItemEndAbleRecipeList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	int iItem = pDispInfo->item.iItem;
	int iSubItem = pDispInfo->item.iSubItem;
	*pResult = -1;

	// KES 2024.07.25 - 설비 이름 및 PPID 변경 불가하게 수정 
	//if (iSubItem == eRC_RECIPE_ID && iItem >= 90) {
	//	CString strRecipeID(pDispInfo->item.pszText);
	//	BOOL bCompare = strRecipeID.Left(3).Compare(_T("TT_")) == 0;
	//	if (bCompare && !IsPPID_REPORT(iItem + 1)) {
	//		if (g_recipes.setRecipeName(iItem, pDispInfo->item.pszText))
	//			*pResult = 0;
	//	}
	//	else {
	//		if (!bCompare)
	//			IPMessage(g_st[IDS_RECIPE_SELECT_WND_NONE_TT_NAME_CAN_NOT_CHANGE]);
	//		else
	//			IPMessage(g_st[IDS_RECIPE_SELECT_WND_CAN_NOT_CHANGE_CIM_APPLIED]);
	//
	//	}
	//}
}

void CRecipeSelectWnd::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
	*pResult = 0;

	if( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )   
	{  
		*pResult = CDRF_NOTIFYITEMDRAW;  

	}   
	else if( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )   
	{  
		CString text = m_lstRecipe.GetItemText(pLVCD->nmcd.dwItemSpec, eRC_INDEX);
		pLVCD->clrText = RGB(0, 0, 0);

		if (text.Find(_T("<")) != -1)
		{
			pLVCD->clrTextBk = RGB(250, 190, 190);
		}
		else
		{
			bool gray = _ttoi(text) % 2;
			if (gray)
			{
				pLVCD->clrTextBk = RGB(230, 230, 250);
			}
			else
			{
				pLVCD->clrTextBk = RGB(255, 255, 255);
			}
		}

		*pResult = CDRF_DODEFAULT; 
	}
}

// KES 추가 2024.4.4
BOOL CRecipeSelectWnd::CreateNewRecipe(BOOL bBtn)
{
	int iSelect = m_lstRecipe.GetSelectionMark();
	if (bBtn && iSelect >= 0 && iSelect <= CIM_RECIPE_COUNT - 1) {
	    AfxMessageBox(g_st[IDS_RECIPE_SELECT_WND_CREATE_WRONG_RECIPE_INDEX]);
		return FALSE;
	}
	if (g_recipes[iSelect] != nullptr) {
		if (bBtn) {
			AfxMessageBox(g_st[IDS_RECIPE_SELECT_WND_RECIPE_ALEADY_EXIST]);
		}
		// Cim에서 내려온 경우 Nak
		return FALSE;
	}

	CString strPPID = InPutMessage(g_st[IDS_RECIPE_SELECT_WND_INSERT_NEW_RECIPE_NAME]);
	BOOL bCompare = strPPID.Left(3).Compare(_T("TT_")) == 0;
	if (!strPPID.IsEmpty() && bCompare) {
		if (bBtn) {	// User 버튼 동작인 경우
			g_recipes.CreateRecipe();
			g_recipes[iSelect]->recipeName = strPPID;
			g_recipes.saveRecipe(iSelect);
		}
	}
	else {
		IPMessage(g_st[IDS_RECIPE_SELECT_WND_TEST_RECIPE_NAME_ERROR]);
		return FALSE;
	}
	
	return TRUE;
}

// KES 추가 2024.4.4
BOOL CRecipeSelectWnd::CopyAppliedRecipe()
{
	int iSelect = m_lstRecipe.GetSelectionMark();
	if (iSelect >= 0 && iSelect <= CIM_RECIPE_COUNT - 1) {
		//24.07.23 KGH 임시주석
	    //AfxMessageBox(g_st[IDS_RECIPE_SELECT_WND_COPY_WRONG_RECIPE_INDEX]);
	    //return FALSE;
	}
	if (g_recipes[iSelect] != nullptr) {
		AfxMessageBox(g_st[IDS_RECIPE_SELECT_WND_RECIPE_ALEADY_EXIST]);
		return FALSE;
	}
	
	CString strPPID = InPutMessage(g_st[IDS_RECIPE_SELECT_WND_INSERT_NEW_RECIPE_NAME]);
	BOOL bCompare = strPPID.Left(3).Compare(_T("TT_")) == 0;
	if (!strPPID.IsEmpty() && bCompare) {
		g_recipes.CopyRecipe();
		g_recipes[iSelect]->recipeName = strPPID;
		g_recipes.saveRecipe(iSelect);
	}
	else {
		IPMessage(g_st[IDS_RECIPE_SELECT_WND_TEST_RECIPE_NAME_ERROR]);
		return FALSE;
	}

	return TRUE;
}
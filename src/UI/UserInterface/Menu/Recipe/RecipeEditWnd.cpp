// RecipeEditWnd.cpp: 구현 파일
//

#include "pch.h"
#include "UI.h"
#include "RecipeEditWnd.h"

// CRecipeEditWnd
IMPLEMENT_DYNAMIC(CRecipeEditWnd, CFrameWndEx)

CRecipeEditWnd::CRecipeEditWnd()
{}

CRecipeEditWnd::~CRecipeEditWnd()
{}

void CRecipeEditWnd::LoadInitialize()
{
	// 레시피 데이터 로드
	m_recipe = *g_recipes.GetAppliedData();
	m_recipe.ppid = (*g_recipes.GetAppliedData()).ppid;
}

void CRecipeEditWnd::ChangeEditColor()
{

}

void CRecipeEditWnd::ShowRecipeInfoTab()
{
	
}

void CRecipeEditWnd::InitFuncPannelData()
{
	
}

void CRecipeEditWnd::ShowRecipeScribeTab()
{
}

void CRecipeEditWnd::ComputeArea()
{
WARNING_PUSH_AND_DISABLE(4244)
	GetClientRect(m_rectClient);
	CRect rect = m_rectClient;
	rect.DeflateRect(1, 1);
	double dW = rect.Width() / 26.;
	double dH = rect.Height() / 1.;

	m_rectS[eS_TITLE_AREA]			= CRect(rect.left, rect.top, rect.right, rect.top + 40);
	//m_rectS[eS_INFO_AREA]			= CRect(rect.left + dW * 0, rect.top + 40, rect.left + dW * 6, rect.bottom);
	m_rectS[eS_INDIVIDUAL_AREA]		= CRect(rect.left + dW * 0, rect.top + 40, rect.left + dW * 26, rect.bottom);
	m_rectS[eS_TITLE_AREA].DeflateRect(3, 1);
	m_rectS[eS_INFO_AREA].DeflateRect(3, 1);
	m_rectS[eS_INDIVIDUAL_AREA].DeflateRect(3, 1);

	// title
	rect = m_rectS[eS_TITLE_AREA];
	rect.DeflateRect(0, 5);
	dW = rect.Width() / 30.3;

	m_rectS[eS_INDEX]				= CRect(rect.left + dW * 0 + 3, rect.top, rect.left + dW * 2 - 3, rect.bottom);
	m_rectS[eS_NAME]				= CRect(rect.left + dW * 2 + 3, rect.top, rect.left + dW * 3 - 3, rect.bottom);
	m_rectC[eC_EDIT_NAME]			= CRect(rect.left + dW * 3 + 3, rect.top, rect.left + dW * 7 - 3, rect.bottom);
	m_rectS[eS_PPID]				= CRect(rect.left + dW * 7 + 3, rect.top, rect.left + dW * 8 - 3, rect.bottom);
	m_rectC[eC_EDIT_PPID]			= CRect(rect.left + dW * 8 + 3, rect.top, rect.left + dW * 11 - 3, rect.bottom);
	m_rectS[eS_DESCRIPTION]			= CRect(rect.left + dW * 11 + 3, rect.top, rect.left + dW * 12 - 3, rect.bottom);
	m_rectC[eC_EDIT_DESCRIPTION]	= CRect(rect.left + dW * 12 + 3, rect.top, rect.left + dW * 17 - 3, rect.bottom);

	//m_rectC[eC_BTN_DXF_SIMULATOR]	= CRect(rect.left + dW * 20 + 3, rect.top, rect.left + dW * 22 - 3, rect.bottom);
	m_rectC[eC_BTN_AUTO_CALCULATION]= CRect(rect.left + dW * 22 + 3, rect.top, rect.left + dW * 24 - 3, rect.bottom);
	m_rectC[eC_BTN_EDIT]			= CRect(rect.left + dW * 24 + 3, rect.top, rect.left + dW * 26 - 3, rect.bottom);
	m_rectC[eC_BTN_UNDO]			= CRect(rect.left + dW * 26 + 3, rect.top, rect.left + dW * 28 - 3, rect.bottom);
	m_rectC[eC_BTN_SAVE]			= CRect(rect.left + dW * 28 + 3, rect.top, rect.left + dW * 30 - 3, rect.bottom);
	
	// info
	rect = m_rectS[eS_INFO_AREA];
	dW = rect.Width() / 16.;

	//m_rectS[eS_INFO_DATA_AREA]		= CRect(rect.left, rect.top, rect.right, rect.bottom - 60);
	//m_rectS[eS_INFO_TAB_AREA]		= CRect(rect.left, rect.bottom - 60, rect.right, rect.bottom);
	int tabHeight = (nRIT / 5 + 1) * 30;
	m_rectS[eS_INFO_TAB_AREA]		= CRect(rect.left, rect.top, rect.right, rect.top + tabHeight);
	m_rectS[eS_INFO_DATA_AREA]		= CRect(rect.left, rect.top + tabHeight, rect.right, rect.bottom);

	rect = m_rectS[eS_INFO_TAB_AREA];
	rect.DeflateRect(0, 1);
	dW = rect.Width() / 15.;
	dH = rect.Height() / (nRIT / 5 + 1);

	// individual
	rect = m_rectS[eS_INDIVIDUAL_AREA];
	dW = (rect.Width () -5.)/ 24.;
	dH = (rect.Height() - 5.) / 20.;
	m_rectS[eS_PPBOX_AREA]		      = CRect(rect.left + 5 + dW * 0, rect.top + dH * 0 + 10,  rect.left + dW * 4,  rect.top + dH * 10);
    m_rectS[eS_TIME_AREA]			  = CRect(rect.left + 5 + dW * 0, rect.top + dH * 10 + 15, rect.left + dW * 4,  rect.top + dH * 20);
	m_rectS[eS_PICKER_OFFSET_Z_AREA]  = CRect(rect.left + 5 + dW * 4, rect.top + dH * 10 + 15, rect.left + dW * 8,  rect.top + dH * 20);

	m_rectS[eS_CONVEYOR_1_SPEED_AREA] = CRect(rect.left + 5 + dW * 4,  rect.top + dH * 0 + 10, rect.left + dW * 8,  rect.top + dH * 10);
	m_rectS[eS_CONVEYOR_2_SPEED_AREA] = CRect(rect.left + 5 + dW * 8,  rect.top + dH * 0 + 10, rect.left + dW * 12, rect.top + dH * 10);
	m_rectS[eS_CONVEYOR_3_SPEED_AREA] = CRect(rect.left + 5 + dW * 12, rect.top + dH * 0 + 10, rect.left + dW * 16, rect.top + dH * 10);
	m_rectS[eS_CONVEYOR_4_SPEED_AREA] = CRect(rect.left + 5 + dW * 16, rect.top + dH * 0 + 10, rect.left + dW * 20, rect.top + dH * 10);
	m_rectS[eS_CONVEYOR_5_SPEED_AREA] = CRect(rect.left + 5 + dW * 20, rect.top + dH * 0 + 10, rect.left + dW * 24, rect.top + dH * 10);

	rect = m_rectS[eS_PPBOX_AREA];
	rect.DeflateRect(5, 5);
	dW = (rect.Width () -5.)/ 2.;
	dH = (rect.Height () -5.)/ 7.;
	m_rectS[eS_PPBOX_GLASS_THICKNESS		] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 0, rect.left + dW * 1, rect.top + dH * 1);
	m_rectS[eS_PPBOX_PAPER_THICKNESS		] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 1, rect.left + dW * 1, rect.top + dH * 2);
	m_rectS[eS_PPBOX_GLASS_COUNT			] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 2, rect.left + dW * 1, rect.top + dH * 3);
	m_rectS[eS_TOP_MATERIAL					] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 3, rect.left + dW * 1, rect.top + dH * 4);
	m_rectS[eS_UNLOAD_OFFSET_X				] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 4, rect.left + dW * 1, rect.top + dH * 5);
	m_rectS[eS_UNLOAD_OFFSET_Y				] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 5, rect.left + dW * 1, rect.top + dH * 6);

	m_rectC[eC_EDIT_PPBOX_GLASS_THICKNESS	] = CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 0, rect.left + dW * 2, rect.top + dH * 1);
	m_rectC[eC_EDIT_PPBOX_PAPER_THICKNESS	] = CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 1, rect.left + dW * 2, rect.top + dH * 2);
	m_rectC[eC_EDIT_PPBOX_GLASS_COUNT		] = CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 2, rect.left + dW * 2, rect.top + dH * 3);
	m_rectC[eC_BTN_TOP_MATERIAL				] = CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 3, rect.left + dW * 2, rect.top + dH * 4);
	m_rectC[eC_EDIT_UNLOAD_OFFSET_X			] = CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 4, rect.left + dW * 2, rect.top + dH * 5);
	m_rectC[eC_EDIT_UNLOAD_OFFSET_Y			] = CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 5, rect.left + dW * 2, rect.top + dH * 6);

    rect = m_rectS[eS_TIME_AREA];
	rect.DeflateRect(5, 5);
	dW = (rect.Width () -5.)/ 2.;
	dH = (rect.Height () -5.)/ 7.;
	m_rectS[eS_TIME_DEFAULT_CYLINDER_TIME_OUT   ] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 0, rect.left + dW * 1, rect.top + dH * 1);
	m_rectS[eS_TIME_VACUUM_TIME_OUT             ] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 1, rect.left + dW * 1, rect.top + dH * 2);
	m_rectS[eS_TIME_BLOW_TIME_OUT               ] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 2, rect.left + dW * 1, rect.top + dH * 3);
	m_rectS[eS_TIME_VACUUM_ON_DELAY             ] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 3, rect.left + dW * 1, rect.top + dH * 4);
	m_rectS[eS_TIME_BLOW_ON_DELAY               ] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 4, rect.left + dW * 1, rect.top + dH * 5);
	
	m_rectC[eC_TIME_DEFAULT_CYLINDER_TIME_OUT	] = CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 0, rect.left + dW * 2, rect.top + dH * 1);
	m_rectC[eC_TIME_VACUUM_TIME_OUT             ] = CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 1, rect.left + dW * 2, rect.top + dH * 2);
	m_rectC[eC_TIME_BLOW_TIME_OUT               ] = CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 2, rect.left + dW * 2, rect.top + dH * 3);
	m_rectC[eC_TIME_VACUUM_ON_DELAY             ] = CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 3, rect.left + dW * 2, rect.top + dH * 4);
	m_rectC[eC_TIME_BLOW_ON_DELAY               ] = CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 4, rect.left + dW * 2, rect.top + dH * 5);

	 rect = m_rectS[eS_PICKER_OFFSET_Z_AREA];
	rect.DeflateRect(5, 5);
	dW = (rect.Width () -5.)/ 2.;
	dH = (rect.Height () -5.)/ 7.;
	m_rectS[eS_PICKER_OFFSET_Z_GLASS_PICKER_LOAD]	= CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 0, rect.left + dW * 1, rect.top + dH * 1);
	m_rectS[eS_PICKER_OFFSET_Z_GLASS_PICKER_UNLOAD] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 1, rect.left + dW * 1, rect.top + dH * 2);
	
	m_rectC[eC_PICKER_OFFSET_Z_GLASS_PICKER_LOAD]	= CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 0, rect.left + dW * 2, rect.top + dH * 1);
	m_rectC[eC_PICKER_OFFSET_Z_GLASS_PICKER_UNLOAD] = CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 1, rect.left + dW * 2, rect.top + dH * 2);
	
	for (int i = 0; i < nCONVEYOR_LD; i++) {
		rect = m_rectS[eS_CONVEYOR_1_SPEED_AREA + i];
		rect.DeflateRect(5, 5);
		dW = (rect.Width() - 5.) / 2.;
		dH = (rect.Height() - 5.) / 6.;

		m_rectS[eS_CONVEYOR_1_HIGH_SPEED + i]	= CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 0, rect.left + dW * 1, rect.top + dH * 1);
		m_rectS[eS_CONVEYOR_1_LOW_SPEED + i]	= CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 1, rect.left + dW * 1, rect.top + dH * 2);
		m_rectS[eS_CONVEYOR_1_ACC1 + i]			= CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 2, rect.left + dW * 1, rect.top + dH * 3);
		m_rectS[eS_CONVEYOR_1_ACC2 + i]			= CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 3, rect.left + dW * 1, rect.top + dH * 4);
		m_rectS[eS_CONVEYOR_1_DEC1 + i]			= CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 4, rect.left + dW * 1, rect.top + dH * 5);
		m_rectS[eS_CONVEYOR_1_DEC2 + i]			= CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 5, rect.left + dW * 1, rect.top + dH * 6);
	
		m_rectC[eC_CONVEYOR_1_HIGH_SPEED + i]	= CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 0, rect.left + dW * 2, rect.top + dH * 1);
		m_rectC[eC_CONVEYOR_1_LOW_SPEED + i]	= CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 1, rect.left + dW * 2, rect.top + dH * 2);
		m_rectC[eC_CONVEYOR_1_ACC1 + i]			= CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 2, rect.left + dW * 2, rect.top + dH * 3);
		m_rectC[eC_CONVEYOR_1_ACC2 + i]			= CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 3, rect.left + dW * 2, rect.top + dH * 4);
		m_rectC[eC_CONVEYOR_1_DEC1 + i]			= CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 4, rect.left + dW * 2, rect.top + dH * 5);
		m_rectC[eC_CONVEYOR_1_DEC2 + i]			= CRect(rect.left + 5 + dW * 1, rect.top + 5 + dH * 5, rect.left + dW * 2, rect.top + dH * 6);
	}

WARNING_POP()
}

void CRecipeEditWnd::SaveRecipe()
{
	std::vector<std::pair<CString, COLORREF>> changeDatas;
	auto& recipe = *g_recipes.GetAppliedData();

	std::vector<CString> vec;
	auto cleanup = make_scope_exit
	(
		[this, &changeDatas, &recipe, &vec]()
		{
			//change += _T("저장하시겠습니까?");
			//if (IDYES == AfxMessageBox(change, MB_YESNO)) // to do 모카 연결하면 IPMessage로 수정
			//{
			if (vec.empty() || changeDatas.empty()) {
				m_editAble = false;
				ChangeEditColor();
				return;
			}
			CChangeParamDlg dlg;
			dlg.SetChangeData(changeDatas);
			if (dlg.DoModal() == IDOK)
			{
				++m_recipe.modifiedCount;
				recipe = m_recipe;
				recipe.applied = true;
				recipe.ppid = m_recipe.ppid;
				g_recipes.SaveAppliedRecipe();
				SendRecipe(); // <- 다른 프로세스 레시피 적용
				//WriteRecipeLog(vec);
				m_editAble = false;
				ChangeEditColor();
				//Update_PPID_PARAM(g_recipes.GetAppliedIndex());

				if (!sm.Sequence->bCIMSkip) {
					CIPVar var;
					var.SetChildItem("PPID_NO", g_recipes.GetAppliedIndex());
					var.SetChildItem("PPID", m_recipe.recipeName);
					var.SetChildItem("APPLY", TRUE);
					if (g_ipClient.SendIPCommand("RECIPE", "SAVE_AND_APPLY", var)) {
						g_recipes.setCIMApplyRecipe(g_recipes.GetAppliedIndex());
					}
				}
			}
			//}
		}
	);

	CString strRecipeName = recipe.recipeName;
	if (*(T_RECIPE*)&m_recipe != *(T_RECIPE*)&recipe)
	{
		//change += _T("T_RECIPE 데이터 수정 됨\n");
		changeDatas.push_back({ Format(_T("[%s]"), strRecipeName), RGB_SKY_LIGHT });
		if (m_recipe.ppid != recipe.ppid) {
			changeDatas.push_back({ Format(_T("PPID %s -> %s"), recipe.ppid, m_recipe.ppid), RGB_WHITE });
			CString strLog = Format(_T("%s;PPID;%s;%s"), strRecipeName, recipe.ppid, m_recipe.ppid);
			vec.push_back(strLog);
		}
		if (m_recipe.description != recipe.description) {
			changeDatas.push_back({ Format(_T("%s %s -> %s"), g_st[IDS_RECIPE_EDIT_WND_DESCRIPTION], recipe.description, m_recipe.description), RGB_WHITE });
			CString strLog = Format(_T("%s;%s;%s;%s"), strRecipeName, g_st[IDS_RECIPE_EDIT_WND_DESCRIPTION], recipe.description, m_recipe.description);
			vec.push_back(strLog);
		}
		if (m_recipe.recipeName != recipe.recipeName) {
			changeDatas.push_back({ Format(_T("%s %s -> %s"), g_st[IDS_RECIPE_EDIT_WND_NAME], recipe.recipeName, m_recipe.recipeName), RGB_WHITE });
			CString strLog = Format(_T("%s;%s;%s;%s"), strRecipeName, g_st[IDS_RECIPE_EDIT_WND_NAME], recipe.recipeName, m_recipe.recipeName);
			vec.push_back(strLog);
		}
	}

	if (m_recipe.ppbox != recipe.ppbox)
	{
		//change += _T("ppbox 데이터 수정 됨\n");

		changeDatas.push_back({ Format(_T("[%s] PPBox"), strRecipeName), RGB_SKY_LIGHT });
		if (m_recipe.ppbox.dGlassThickness != recipe.ppbox.dGlassThickness) {
			changeDatas.push_back({ Format(_T("Glss Thickness %.3f -> %.3f"), recipe.ppbox.dGlassThickness, m_recipe.ppbox.dGlassThickness), RGB_WHITE });
			CString strLog = Format(_T("%s;Glass Thickness;%.3f;%.3f"), strRecipeName, recipe.ppbox.dGlassThickness, m_recipe.ppbox.dGlassThickness);
			vec.push_back(strLog);
		}
		if (m_recipe.ppbox.dPaperThickness != recipe.ppbox.dPaperThickness) {
			changeDatas.push_back({ Format(_T("Paper Thickness %.3f -> %.3f"), recipe.ppbox.dPaperThickness, m_recipe.ppbox.dPaperThickness), RGB_WHITE });
			CString strLog = Format(_T("%s;Paper Thickness;%.3f;%.3f"), strRecipeName, recipe.ppbox.dPaperThickness, m_recipe.ppbox.dPaperThickness);
			vec.push_back(strLog);
		}
		if (m_recipe.ppbox.iGlassCount != recipe.ppbox.iGlassCount) {
			changeDatas.push_back({ Format(_T("Glass Count %d -> %d"), recipe.ppbox.iGlassCount, m_recipe.ppbox.iGlassCount), RGB_WHITE });
			CString strLog = Format(_T("%s;Glass Count;%d;%d"), strRecipeName, recipe.ppbox.iGlassCount, m_recipe.ppbox.iGlassCount);
			vec.push_back(strLog);
		}
		if (m_recipe.ppbox.dUnloadTableOffsetX != recipe.ppbox.dUnloadTableOffsetX) {
			changeDatas.push_back({ Format(_T("Unload Offset X %.3f -> %.3f"), recipe.ppbox.dUnloadTableOffsetX, m_recipe.ppbox.dUnloadTableOffsetX), RGB_WHITE });
			CString strLog = Format(_T("%s;Unload Offset X;%.3f;%.3f"), strRecipeName, recipe.ppbox.dUnloadTableOffsetX, m_recipe.ppbox.dUnloadTableOffsetX);
			vec.push_back(strLog);
		}
		if (m_recipe.ppbox.dUnloadTableOffsetY != recipe.ppbox.dUnloadTableOffsetY) {
			changeDatas.push_back({ Format(_T("Unload Offset Y %.3f -> %.3f"), recipe.ppbox.dUnloadTableOffsetY, m_recipe.ppbox.dUnloadTableOffsetY), RGB_WHITE });
			CString strLog = Format(_T("%s;Unload Offset Y;%.3f;%.3f"), strRecipeName, recipe.ppbox.dUnloadTableOffsetY, m_recipe.ppbox.dUnloadTableOffsetY);
			vec.push_back(strLog);
		}
		if (m_recipe.ppbox.bTopMaterialType != recipe.ppbox.bTopMaterialType) {

			changeDatas.push_back({ Format(_T("Top Material Type %s -> %s"), recipe.ppbox.bTopMaterialType ? _T("Glass") : _T("Paper"), m_recipe.ppbox.bTopMaterialType ? _T("Glass") : _T("Paper")), RGB_WHITE });
			CString strLog = Format(_T("%s;Top Material Type;%s;%s"), strRecipeName, recipe.ppbox.bTopMaterialType ? _T("Glass") : _T("Paper"), m_recipe.ppbox.bTopMaterialType ? _T("Glass") : _T("Paper"));
			vec.push_back(strLog);
		}
	}

    if (m_recipe.vacuumBlowTime != recipe.vacuumBlowTime)
	{
		//change += _T("ppbox 데이터 수정 됨\n");

		changeDatas.push_back({ Format(_T("[%s] VacuumBlowTime"), strRecipeName), RGB_SKY_LIGHT });
		if (m_recipe.vacuumBlowTime.dDefaultCylinder != recipe.vacuumBlowTime.dDefaultCylinder) {
			changeDatas.push_back({ Format(_T("Default Cylinder Timeout %.3f -> %.3f"), recipe.vacuumBlowTime.dDefaultCylinder, m_recipe.vacuumBlowTime.dDefaultCylinder), RGB_WHITE });
			CString strLog = Format(_T("%s;Default Cylinder Timeout;%.3f;%.3f"), strRecipeName, recipe.vacuumBlowTime.dDefaultCylinder, m_recipe.vacuumBlowTime.dDefaultCylinder);
			vec.push_back(strLog);
		}
		if (m_recipe.vacuumBlowTime.dVacuum != recipe.vacuumBlowTime.dVacuum) {
			changeDatas.push_back({ Format(_T("Vacuum Timeout %.3f -> %.3f"), recipe.vacuumBlowTime.dVacuum, m_recipe.vacuumBlowTime.dVacuum), RGB_WHITE });
			CString strLog = Format(_T("%s;Vacuum Timeout;%.3f;%.3f"), strRecipeName, recipe.vacuumBlowTime.dVacuum, m_recipe.vacuumBlowTime.dVacuum);
			vec.push_back(strLog);
		}
        if (m_recipe.vacuumBlowTime.dBlow != recipe.vacuumBlowTime.dBlow) {
			changeDatas.push_back({ Format(_T("Blow Timeout %.3f -> %.3f"), recipe.vacuumBlowTime.dBlow, m_recipe.vacuumBlowTime.dBlow), RGB_WHITE });
			CString strLog = Format(_T("%s;Blow Timeout;%.3f;%.3f"), strRecipeName, recipe.vacuumBlowTime.dBlow, m_recipe.vacuumBlowTime.dBlow);
			vec.push_back(strLog);
		}
        if (m_recipe.vacuumBlowTime.dVacuumOnDelay != recipe.vacuumBlowTime.dVacuumOnDelay) {
			changeDatas.push_back({ Format(_T("Vacuum On Delay %.3f -> %.3f"), recipe.vacuumBlowTime.dVacuumOnDelay, m_recipe.vacuumBlowTime.dVacuumOnDelay), RGB_WHITE });
			CString strLog = Format(_T("%s;Vacuum On Delay;%.3f;%.3f"), strRecipeName, recipe.vacuumBlowTime.dVacuumOnDelay, m_recipe.vacuumBlowTime.dVacuumOnDelay);
			vec.push_back(strLog);
		}
        if (m_recipe.vacuumBlowTime.dBlowOnWait != recipe.vacuumBlowTime.dBlowOnWait) {
			changeDatas.push_back({ Format(_T("Blow On Delay %.3f -> %.3f"), recipe.vacuumBlowTime.dBlowOnWait, m_recipe.vacuumBlowTime.dBlowOnWait), RGB_WHITE });
			CString strLog = Format(_T("%s;Blow On Delay;%.3f;%.3f"), strRecipeName, recipe.vacuumBlowTime.dBlowOnWait, m_recipe.vacuumBlowTime.dBlowOnWait);
			vec.push_back(strLog);
		}
	}

	if (m_recipe.pickerOffsetZ != recipe.pickerOffsetZ)
	{
		changeDatas.push_back({ Format(_T("[%s] PickerOffsetZ"), strRecipeName), RGB_SKY_LIGHT });
		if (m_recipe.pickerOffsetZ.dGlassPickerLoad != recipe.pickerOffsetZ.dGlassPickerLoad) {
			changeDatas.push_back({ Format(_T("Glass Picker Load Offset Z %.3f -> %.3f"), recipe.pickerOffsetZ.dGlassPickerLoad, m_recipe.pickerOffsetZ.dGlassPickerLoad), RGB_WHITE });
			CString strLog = Format(_T("%s;Glass Picker Load Offset Z;%.3f;%.3f"), strRecipeName, recipe.pickerOffsetZ.dGlassPickerLoad, m_recipe.pickerOffsetZ.dGlassPickerLoad);
			vec.push_back(strLog);
		}
		if (m_recipe.pickerOffsetZ.dGlassPickerUnload != recipe.pickerOffsetZ.dGlassPickerUnload) {
			changeDatas.push_back({ Format(_T("Glass Picker Unload Offset Z %.3f -> %.3f"), recipe.pickerOffsetZ.dGlassPickerUnload, m_recipe.pickerOffsetZ.dGlassPickerUnload), RGB_WHITE });
			CString strLog = Format(_T("%s;Glass Picker Unload Offset Z;%.3f;%.3f"), strRecipeName, recipe.pickerOffsetZ.dGlassPickerUnload, m_recipe.pickerOffsetZ.dGlassPickerUnload);
			vec.push_back(strLog);
		}
	}

	for (int i = 0; i < nCONVEYOR_LD; i++) {
		if (m_recipe.conveyorSpeed[i] != recipe.conveyorSpeed[i]) {
			changeDatas.push_back({ Format(_T("[%s] %s Speed"), strRecipeName, g_st[IDS_RECIPE_EDIT_LD_CONVEYOR_1 + i]), RGB_SKY_LIGHT });
			if (m_recipe.conveyorSpeed[i].dHighSpeed != recipe.conveyorSpeed[i].dHighSpeed) {
				changeDatas.push_back({ Format(_T("High Speed %.3f -> %.3f"), recipe.conveyorSpeed[i].dHighSpeed, m_recipe.conveyorSpeed[i].dHighSpeed), RGB_WHITE });
				CString strLog = Format(_T("%s;High Speed;%.3f;%.3f"), strRecipeName, recipe.conveyorSpeed[i].dHighSpeed, m_recipe.conveyorSpeed[i].dHighSpeed);
				vec.push_back(strLog);
			}
			if (m_recipe.conveyorSpeed[i].dLowSpeed != recipe.conveyorSpeed[i].dLowSpeed) {
				changeDatas.push_back({ Format(_T("Low Speed %.3f -> %.3f"), recipe.conveyorSpeed[i].dLowSpeed, m_recipe.conveyorSpeed[i].dLowSpeed), RGB_WHITE });
				CString strLog = Format(_T("%s;Low Speed;%.3f;%.3f"), strRecipeName, recipe.conveyorSpeed[i].dLowSpeed, m_recipe.conveyorSpeed[i].dLowSpeed);
				vec.push_back(strLog);
			}
			if (m_recipe.conveyorSpeed[i].iACC1 != recipe.conveyorSpeed[i].iACC1) {
				changeDatas.push_back({ Format(_T("ACC1 %d -> %d"), recipe.conveyorSpeed[i].iACC1, m_recipe.conveyorSpeed[i].iACC1), RGB_WHITE });
				CString strLog = Format(_T("%s;ACC1;%d;%d"), strRecipeName, recipe.conveyorSpeed[i].iACC1, m_recipe.conveyorSpeed[i].iACC1);
				vec.push_back(strLog);
			}
			if (m_recipe.conveyorSpeed[i].iACC2 != recipe.conveyorSpeed[i].iACC2) {
				changeDatas.push_back({ Format(_T("ACC2 %d -> %d"), recipe.conveyorSpeed[i].iACC2, m_recipe.conveyorSpeed[i].iACC2), RGB_WHITE });
				CString strLog = Format(_T("%s;ACC2;%d;%d"), strRecipeName, recipe.conveyorSpeed[i].iACC2, m_recipe.conveyorSpeed[i].iACC2);
				vec.push_back(strLog);
			}
			if (m_recipe.conveyorSpeed[i].iDEC1 != recipe.conveyorSpeed[i].iDEC1) {
				changeDatas.push_back({ Format(_T("DEC1 %d -> %d"), recipe.conveyorSpeed[i].iDEC1, m_recipe.conveyorSpeed[i].iDEC1), RGB_WHITE });
				CString strLog = Format(_T("%s;DEC1;%d;%d"), strRecipeName, recipe.conveyorSpeed[i].iDEC1, m_recipe.conveyorSpeed[i].iDEC1);
				vec.push_back(strLog);
			}
			if (m_recipe.conveyorSpeed[i].iDEC2 != recipe.conveyorSpeed[i].iDEC2) {
				changeDatas.push_back({ Format(_T("DEC2 %d -> %d"), recipe.conveyorSpeed[i].iDEC2, m_recipe.conveyorSpeed[i].iDEC2), RGB_WHITE });
				CString strLog = Format(_T("%s;DEC2;%d;%d"), strRecipeName, recipe.conveyorSpeed[i].iDEC2, m_recipe.conveyorSpeed[i].iDEC2);
				vec.push_back(strLog);
			}
		}
	}
}



#define IDC_SCRIBE_INFO_WND				WM_APP + 5201
#define IDC_SCRIBE_BREAK_EDIT_WND		WM_APP + 5301

BEGIN_MESSAGE_MAP(CRecipeEditWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_APP_ED_EDIT_FINISHED, OnEditFinished)
END_MESSAGE_MAP()



// CRecipeEditWnd 메시지 처리기

int CRecipeEditWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	ShowRecipeInfoTab();

	// 스크라이브/브레이크 리스트
	for (int idx = 0; idx < 2; ++idx)
	{
		CString imgPath = Format(_T("../img/CellCut%d.png"), idx + 1);
	}
	ShowRecipeScribeTab();

	// 레시피 정보 리스트 업데이트 용 함수 세팅
	InitFuncPannelData();

	return 0;
}


void CRecipeEditWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	
	if (bShow)
	{
		SetWindowPos(&wndTop, g_rectArea[eAREA_VISION].left, g_rectArea[eAREA_VISION].top, g_rectArea[eAREA_VISION].Width(), g_rectArea[eAREA_VISION].Height(), SWP_SHOWWINDOW);
		ComputeArea();
		if (g_recipes.GetAppliedData() != nullptr)
		{
			LoadInitialize();
		}
	}
	else
	{
		//<< KES 추가 2024.07.21 - CIM 프로그램 꺼져 있으면 수정 불가
		BOOL bCIMRunning = TRUE;
		if (!g_ipCIMClient.IsServerConnected() || !g_ipCIMClient.IsIPCommandLoopStarted())
		{
			bCIMRunning = FALSE;
		}
		//>>

		if ((!sm.Sequence->fAutoMode || sm.Sequence->fCycleStop) && bCIMRunning)
		{
			SaveRecipe();
		}
		m_editAble = false;
	}
}


void CRecipeEditWnd::OnPaint()
{
	CPaintDC dc(this); 
	CMemDCEx::CMemDC memDC(&dc);
	memDC.FillSolidRect(m_rectClient, RgbColor::Gainsboro);

	// 상단 (Title Area)
	SelectedDrawText(memDC, m_rectS[eS_INDEX], Format(_T("Index : %d"), g_recipes.GetAppliedIndex() + 1));
	SelectedDrawText(memDC, m_rectS[eS_NAME], g_st[IDS_RECIPE_EDIT_RECIPE_NAME]);
	SelectedDrawText(memDC, m_rectS[eS_PPID], g_st[IDS_RECIPE_EDIT_RECIPE_PPID]);
	SelectedDrawText(memDC, m_rectS[eS_DESCRIPTION], g_st[IDS_RECIPE_EDIT_RECIPE_DESCRIPTION]);

	BOOL bCIMEdit = m_editAble;
	//<< 2024.07.16 LBH - 1 ~ 90번 레시피 CIM 항목 수정 안되도록 인터락 작업 할때 주석 해제 하면됨
	if (g_recipes.GetAppliedIndex() < 90)
		bCIMEdit = FALSE;
	//>>

	if (m_editAble)
	{
		SelectedDrawEditText(memDC, m_rectC[eC_EDIT_NAME], _T(" ") + m_recipe.recipeName, FALSE);
		SelectedDrawEditText(memDC, m_rectC[eC_EDIT_PPID], _T(" ") + m_recipe.ppid, FALSE);
		SelectedDrawEditText(memDC, m_rectC[eC_EDIT_DESCRIPTION], _T(" ") + m_recipe.description);
	}
	else
	{
		SelectedDrawText(memDC, m_rectC[eC_EDIT_NAME], _T(" ") + m_recipe.recipeName, DT_FONTSIZE, DT_PEN, DT_BK, DT_TEXT, DT_LEFT | DT_VCENTER);
		SelectedDrawText(memDC, m_rectC[eC_EDIT_PPID], _T(" ") + m_recipe.ppid, DT_FONTSIZE, DT_PEN, DT_BK, DT_TEXT, DT_LEFT | DT_VCENTER);
		SelectedDrawText(memDC, m_rectC[eC_EDIT_DESCRIPTION], _T(" ") + m_recipe.description, DT_FONTSIZE, DT_PEN, DT_BK, DT_TEXT, DT_LEFT | DT_VCENTER);
	}

	DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_AUTO_CALCULATION]	, g_st[IDS_RECIPE_EDIT_RECIPE_AUTO_CALCULATION]	, m_iSelectControl == eC_BTN_AUTO_CALCULATION, m_iMoveControl == eC_BTN_AUTO_CALCULATION);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_EDIT]				, g_st[IDS_RECIPE_EDIT_RECIPE_EDIT]				, m_iSelectControl == eC_BTN_EDIT, m_iMoveControl == eC_BTN_EDIT);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_UNDO]				, g_st[IDS_RECIPE_EDIT_RECIPE_UNDO]				, m_iSelectControl == eC_BTN_UNDO, m_iMoveControl == eC_BTN_UNDO);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_SAVE]				, g_st[IDS_SAVE]								, m_iSelectControl == eC_BTN_SAVE, m_iMoveControl == eC_BTN_SAVE);

	// 좌측 (Info List Area) - 레시피 데이터 값 설정
	DrawLineRect(memDC, m_rectS[eS_INFO_DATA_AREA]);

	DrawGroupBox(memDC, m_rectS[eS_PPBOX_AREA], g_st[IDS_RECIPE_EDIT_AREA_MG_TRANSFER], RGB_BLACK, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_PPBOX_GLASS_THICKNESS]	, g_st[IDS_RECIPE_EDIT_GLASS_THICKNESS]	, 10, RGB_BLACK);	
	DrawText(memDC, m_rectS[eS_PPBOX_PAPER_THICKNESS]	, g_st[IDS_RECIPE_EDIT_PAPER_THICKNESS]	, 10, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_PPBOX_GLASS_COUNT]		, g_st[IDS_RECIPE_EDIT_GLASS_COUNT], 10, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_TOP_MATERIAL]			, g_st[IDS_RECIPE_EDIT_TOP_MATERIAL_TYPE], 10, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_UNLOAD_OFFSET_X]			, g_st[IDS_RECIPE_EDIT_UNLOAD_TABLE_OFFSET_X]	, 10, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_UNLOAD_OFFSET_Y]			, g_st[IDS_RECIPE_EDIT_UNLOAD_TABLE_OFFSET_Y]	, 10, RGB_BLACK);

	SelectedDrawEditDouble(memDC, m_rectC[eC_EDIT_PPBOX_GLASS_THICKNESS], m_recipe.ppbox.dGlassThickness, 3, bCIMEdit, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
	SelectedDrawEditDouble(memDC, m_rectC[eC_EDIT_PPBOX_PAPER_THICKNESS], m_recipe.ppbox.dPaperThickness, 3, bCIMEdit, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
	SelectedDrawEditInt(memDC, m_rectC[eC_EDIT_PPBOX_GLASS_COUNT]	, m_recipe.ppbox.iGlassCount, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);

	SelectedDrawEditDouble(memDC, m_rectC[eC_EDIT_UNLOAD_OFFSET_X], m_recipe.ppbox.dUnloadTableOffsetX, 3, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
	SelectedDrawEditDouble(memDC, m_rectC[eC_EDIT_UNLOAD_OFFSET_Y], m_recipe.ppbox.dUnloadTableOffsetY, 3, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
	
	if (m_recipe.ppbox.bTopMaterialType)
		SelectedDrawText(memDC, m_rectC[eC_BTN_TOP_MATERIAL], g_st[IDS_RECIPE_EDIT_TYPE_GLASS], 10, RGB_WHITE, RGB_GREEN_DARK, RGB_BLACK);
	else 																  
		SelectedDrawText(memDC, m_rectC[eC_BTN_TOP_MATERIAL], g_st[IDS_RECIPE_EDIT_TYPE_PAPER], 10, RGB_BLACK, RGB_WHITE, RGB_BLACK);

    DrawGroupBox(memDC, m_rectS[eS_TIME_AREA]                   , g_st[IDS_RECIPE_EDIT_AREA_VACUUM_BLOW_TIME], RGB_BLACK, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_TIME_DEFAULT_CYLINDER_TIME_OUT]	, g_st[IDS_RECIPE_EDIT_DEFAULT_CYLINDER_TIME_OUT], 10, RGB_BLACK);	
	DrawText(memDC, m_rectS[eS_TIME_VACUUM_TIME_OUT]	        , g_st[IDS_RECIPE_EDIT_VACUUM_TIME_OUT], 10, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_TIME_BLOW_TIME_OUT]		        , g_st[IDS_RECIPE_EDIT_BLOW_TIME_OUT], 10, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_TIME_VACUUM_ON_DELAY]			, g_st[IDS_RECIPE_EDIT_VACUUM_ON_DELAY], 10, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_TIME_BLOW_ON_DELAY]			    , g_st[IDS_RECIPE_EDIT_BLOW_ON_DELAY], 10, RGB_BLACK);

    SelectedDrawEditDouble(memDC, m_rectC[eC_TIME_DEFAULT_CYLINDER_TIME_OUT], m_recipe.vacuumBlowTime.dDefaultCylinder, 3, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
	SelectedDrawEditDouble(memDC, m_rectC[eC_TIME_VACUUM_TIME_OUT], m_recipe.vacuumBlowTime.dVacuum, 3, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
	SelectedDrawEditDouble(memDC, m_rectC[eC_TIME_BLOW_TIME_OUT], m_recipe.vacuumBlowTime.dBlow, 3, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
	SelectedDrawEditDouble(memDC, m_rectC[eC_TIME_VACUUM_ON_DELAY], m_recipe.vacuumBlowTime.dVacuumOnDelay, 3, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
	SelectedDrawEditDouble(memDC, m_rectC[eC_TIME_BLOW_ON_DELAY], m_recipe.vacuumBlowTime.dBlowOnWait, 3, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);

	DrawGroupBox(memDC, m_rectS[eS_PICKER_OFFSET_Z_AREA],				g_st[IDS_RECIPE_EDIT_AREA_PICKER_OFFSET_Z], RGB_BLACK, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_PICKER_OFFSET_Z_GLASS_PICKER_LOAD],		g_st[IDS_RECIPE_EDIT_GLASS_PICKER_LOAD_Z], 10, RGB_BLACK);
	DrawText(memDC, m_rectS[eS_PICKER_OFFSET_Z_GLASS_PICKER_UNLOAD],	g_st[IDS_RECIPE_EDIT_GLASS_PICKER_UNLOAD_Z], 10, RGB_BLACK);
	
    SelectedDrawEditDouble(memDC, m_rectC[eC_PICKER_OFFSET_Z_GLASS_PICKER_LOAD],	m_recipe.pickerOffsetZ.dGlassPickerLoad, 3, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
	SelectedDrawEditDouble(memDC, m_rectC[eC_PICKER_OFFSET_Z_GLASS_PICKER_UNLOAD],	m_recipe.pickerOffsetZ.dGlassPickerUnload, 3, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);

	for (int i = 0; i < nCONVEYOR_LD; i++) {
		DrawGroupBox(memDC, m_rectS[eS_CONVEYOR_1_SPEED_AREA + i],	g_st[IDS_RECIPE_EDIT_LD_CONVEYOR_1 + i], RGB_BLACK, RGB_BLACK);
		DrawText(memDC, m_rectS[eS_CONVEYOR_1_HIGH_SPEED + i],		_T("High Speed"), 10, RGB_BLACK);
		DrawText(memDC, m_rectS[eS_CONVEYOR_1_LOW_SPEED + i],		_T("Low Speed"), 10, RGB_BLACK);
		DrawText(memDC, m_rectS[eS_CONVEYOR_1_ACC1 + i],			_T("ACC1"), 10, RGB_BLACK);
		DrawText(memDC, m_rectS[eS_CONVEYOR_1_ACC2 + i],			_T("ACC2"), 10, RGB_BLACK);
		DrawText(memDC, m_rectS[eS_CONVEYOR_1_DEC1 + i],			_T("DEC1"), 10, RGB_BLACK);
		DrawText(memDC, m_rectS[eS_CONVEYOR_1_DEC2 + i],			_T("DEC2"), 10, RGB_BLACK);

		SelectedDrawEditDouble(memDC, m_rectC[eC_CONVEYOR_1_HIGH_SPEED + i],	m_recipe.conveyorSpeed[i].dHighSpeed,	3, bCIMEdit, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
		SelectedDrawEditDouble(memDC, m_rectC[eC_CONVEYOR_1_LOW_SPEED + i],		m_recipe.conveyorSpeed[i].dLowSpeed,	3, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);

		SelectedDrawEditInt(memDC, m_rectC[eC_CONVEYOR_1_ACC1 + i], m_recipe.conveyorSpeed[i].iACC1, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
		SelectedDrawEditInt(memDC, m_rectC[eC_CONVEYOR_1_ACC2 + i], m_recipe.conveyorSpeed[i].iACC2, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
		SelectedDrawEditInt(memDC, m_rectC[eC_CONVEYOR_1_DEC1 + i], m_recipe.conveyorSpeed[i].iDEC1, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
		SelectedDrawEditInt(memDC, m_rectC[eC_CONVEYOR_1_DEC2 + i], m_recipe.conveyorSpeed[i].iDEC2, m_editAble, DT_FONTSIZE, DT_CENTER | DT_VCENTER);
	}
}


void CRecipeEditWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	//SetFocus();
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


void CRecipeEditWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString strLog;
	auto cleanup = make_scope_exit
	(
		[this, nFlags, point]()
		{
			m_iSelectEdit = m_iSelectControl;
			m_iSelectControl = eC_NONE;
			Invalidate(FALSE);
			CFrameWndEx::OnLButtonUp(nFlags, point);
		}
	);

	if (m_iSelectControl <= eC_NONE || m_iSelectControl >= eC_END)
	{
		return;
	}

	if (!m_rectC[m_iSelectControl].PtInRect(point))
	{
		return;
	}
	
	SetFocus();
	BOOL bCIMEdit = m_editAble;
	//<< 2024.07.16 LBH - 1 ~ 90번 레시피 CIM 항목 수정 안되도록 인터락 작업 할때 주석 해제 하면됨
	if (g_recipes.GetAppliedIndex() < 90)
		bCIMEdit = FALSE;
	//>>

	switch (m_iSelectControl)
	{
		case eC_EDIT_NAME:
		{
			//if (!m_editAble) return;
			//EditStart(this, m_rectC[eC_EDIT_NAME], m_recipe.recipeName, TRUE);
		}
		break;
		
		case eC_EDIT_PPID:
		{
			//if (!m_editAble) return;
			//EditStart(this, m_rectC[eC_EDIT_PPID], m_recipe.ppid);
		}
		break;

		case eC_EDIT_DESCRIPTION:
		{
			if (!m_editAble) return;
			EditStart(this, m_rectC[eC_EDIT_DESCRIPTION], m_recipe.description);
		}
		break;
		
		case eC_BTN_AUTO_CALCULATION:
		{
			strLog = _T("자동 계산");
		}
		break;

		case eC_BTN_EDIT:
		{
			//<< KES 레시피 변경 가능 조건 추가 2024.07.25
			BOOL bOtherEQPAuto = (sm.PCs->ControlPC[eMACHINE_SCT].fAutoMode && !sm.PCs->ControlPC[eMACHINE_SCT].fCycleStop);
			if ((sm.Sequence->fAutoMode && !sm.Sequence->fCycleStop) || bOtherEQPAuto) {
				IPMessage(g_st[IDS_RECIPE_SELECT_WND_LD_SCT_AUTO_RUN]);
				return;
			}
			else {
				m_editAble = true;
				ChangeEditColor();
				strLog = _T("편집");
			}
			//>>
		}
		break;

		case eC_BTN_UNDO:
		{
			m_editAble = false;
			if (g_recipes.GetAppliedData() != nullptr)
			{
				LoadInitialize(); // 기존 파일데이터로 세팅
			}
			strLog = _T("편집 취소");
		}
		break;

		case eC_BTN_SAVE:
		{
			//<< KES 추가 2024.07.21 - CIM 프로그램 꺼져 있으면 수정 불가
			if (!g_ipCIMClient.IsServerConnected() || !g_ipCIMClient.IsIPCommandLoopStarted())
			{
				IPMessage(g_st[IDS_MS_CIM_PROGRAM_IS_CLOSED]);
				return;
			}
			//>>

			// 2021.12.09 오토모드 중에는 edit 방지
			if (!sm.Sequence->fAutoMode || sm.Sequence->fCycleStop)
			{
				SaveRecipe();
				strLog = _T("저장");
			}
		}
		break;

		case eC_EDIT_PPBOX_GLASS_THICKNESS :
		{
			if (!m_editAble || !bCIMEdit) return;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.ppbox.dGlassThickness);
		}
		break;
		case eC_EDIT_PPBOX_PAPER_THICKNESS:
		{
			if (!m_editAble || !bCIMEdit) return;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.ppbox.dPaperThickness);
		}
		break;
		case eC_EDIT_PPBOX_GLASS_COUNT:
		{
			if (!m_editAble) return;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.ppbox.iGlassCount);
		}
		break;
		case eC_BTN_TOP_MATERIAL:
		{
			if (!m_editAble) return;
			m_recipe.ppbox.bTopMaterialType = !m_recipe.ppbox.bTopMaterialType;
		}
		break;
		case eC_EDIT_UNLOAD_OFFSET_X: {
			if (!m_editAble) return;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.ppbox.dUnloadTableOffsetX);
		}
		break;
		case eC_EDIT_UNLOAD_OFFSET_Y: {
			if (!m_editAble) return;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.ppbox.dUnloadTableOffsetY);
		}
        break;
        case eC_TIME_DEFAULT_CYLINDER_TIME_OUT :
		{
			if (!m_editAble) return;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.vacuumBlowTime.dDefaultCylinder);
		}
        break;
        case eC_TIME_VACUUM_TIME_OUT :
		{
			if (!m_editAble) return;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.vacuumBlowTime.dVacuum);
		}
        break;
        case eC_TIME_BLOW_TIME_OUT :
		{
			if (!m_editAble) return;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.vacuumBlowTime.dBlow);
		}
        break;
        case eC_TIME_VACUUM_ON_DELAY :
		{
			if (!m_editAble) return;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.vacuumBlowTime.dVacuumOnDelay);
		}
        break;
        case eC_TIME_BLOW_ON_DELAY :
		{
			if (!m_editAble) return;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.vacuumBlowTime.dBlowOnWait);
		}
		break;
		case eC_PICKER_OFFSET_Z_GLASS_PICKER_LOAD:
		{
			if (!m_editAble) return;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.pickerOffsetZ.dGlassPickerLoad);
		}
		break;
		case eC_PICKER_OFFSET_Z_GLASS_PICKER_UNLOAD:
		{
			if (!m_editAble) return;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.pickerOffsetZ.dGlassPickerUnload);
		}
		break;
		case eC_CONVEYOR_1_HIGH_SPEED:
		case eC_CONVEYOR_2_HIGH_SPEED:
		case eC_CONVEYOR_3_HIGH_SPEED:
		case eC_CONVEYOR_4_HIGH_SPEED:
		case eC_CONVEYOR_5_HIGH_SPEED:
		{
			if (!m_editAble || !bCIMEdit) return;
			int iIndex = m_iSelectControl - eC_CONVEYOR_1_HIGH_SPEED;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.conveyorSpeed[iIndex].dHighSpeed, FALSE, FALSE, 0);
		}
		break;
		case eC_CONVEYOR_1_LOW_SPEED:
		case eC_CONVEYOR_2_LOW_SPEED:
		case eC_CONVEYOR_3_LOW_SPEED:
		case eC_CONVEYOR_4_LOW_SPEED:
		case eC_CONVEYOR_5_LOW_SPEED:
		{
			if (!m_editAble) return;
			int iIndex = m_iSelectControl - eC_CONVEYOR_1_LOW_SPEED;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.conveyorSpeed[iIndex].dLowSpeed);
		}
		break;
		case eC_CONVEYOR_1_ACC1:
		case eC_CONVEYOR_2_ACC1:
		case eC_CONVEYOR_3_ACC1:
		case eC_CONVEYOR_4_ACC1:
		case eC_CONVEYOR_5_ACC1:
		{
			if (!m_editAble) return;
			int iIndex = m_iSelectControl - eC_CONVEYOR_1_ACC1;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.conveyorSpeed[iIndex].iACC1);
		}
		break;
		case eC_CONVEYOR_1_ACC2:
		case eC_CONVEYOR_2_ACC2:
		case eC_CONVEYOR_3_ACC2:
		case eC_CONVEYOR_4_ACC2:
		case eC_CONVEYOR_5_ACC2:
		{
			if (!m_editAble) return;
			int iIndex = m_iSelectControl - eC_CONVEYOR_1_ACC2;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.conveyorSpeed[iIndex].iACC2);
		}
		break;
		case eC_CONVEYOR_1_DEC1:
		case eC_CONVEYOR_2_DEC1:
		case eC_CONVEYOR_3_DEC1:
		case eC_CONVEYOR_4_DEC1:
		case eC_CONVEYOR_5_DEC1:
		{
			if (!m_editAble) return;
			int iIndex = m_iSelectControl - eC_CONVEYOR_1_DEC1;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.conveyorSpeed[iIndex].iDEC1);
		}
		break;
		case eC_CONVEYOR_1_DEC2:
		case eC_CONVEYOR_2_DEC2:
		case eC_CONVEYOR_3_DEC2:
		case eC_CONVEYOR_4_DEC2:
		case eC_CONVEYOR_5_DEC2:
		{
			if (!m_editAble) return;
			int iIndex = m_iSelectControl - eC_CONVEYOR_1_DEC2;
			EditStart(this, m_rectC[m_iSelectControl], m_recipe.conveyorSpeed[iIndex].iDEC2);
		}
		break;
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
}


void CRecipeEditWnd::OnMouseMove(UINT nFlags, CPoint point)
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

LRESULT CRecipeEditWnd::OnEditFinished(WPARAM wParam, LPARAM lParam)
{
	if (wParam) 
	{
		CString	strNewItem(GetEditString());
		switch (m_iSelectEdit)
		{
			case eC_EDIT_NAME: 
			{
				// KES 추가 2024.4.4 - 레시피명 수정할 때 Index에 따라서 레시피명 앞 PREFIX_TEST_RECIPE 체크
				CString str = strNewItem.Left(2);
				int iIndex = g_recipes.GetAppliedIndex();
				if (iIndex >= 0 && iIndex <= CIM_RECIPE_COUNT - 1) {
					if (str == PREFIX_TEST_RECIPE) {
						IPMessage(g_st[IDS_RECIPE_SELECT_WND_CIM_RECIPE_NAME_ERROR]);
						break;
					}
				}
				else {
					if (str != PREFIX_TEST_RECIPE) {
						IPMessage(g_st[IDS_RECIPE_SELECT_WND_TEST_RECIPE_NAME_ERROR]);
						break;
					}
				}

				int iOverlapIndex = 0;
				BOOL bOverlap = FALSE;
				// 중복 검사
				for (int i = 0; i < g_recipes.Size(); ++i) 
				{
					if (g_recipes.GetAppliedIndex() == i)
						continue;
					if (g_recipes[i] == nullptr)
						continue;
					if (g_recipes[i]->recipeName == strNewItem) 
					{
						bOverlap = TRUE;
						iOverlapIndex = i;
						break;
					}
				}

				// 중복이 없을 경우
				if (!bOverlap) 
				{
					// 현재 레시피 명칭으로 수정
					m_recipe.recipeName = strNewItem;
				}
				// 중복 발생 메세지
				else 
				{
					IPMessage(Format(_T("%s: [%s] & %d "), g_st[IDS_RECIPE_EDIT_WND_REICPE_NAME_IS_DUPLICATED], strNewItem, iOverlapIndex + 1));
				}
			}
			break;
		}

		m_iSelectEdit = eC_NONE;
		Invalidate(FALSE);
	}

	return 0;
}

//void CRecipeEditWnd::WriteRecipeLog(std::vector<CString> vec) {
//	CFile file;
//	CFileException e;
//
//	CTime cTime = CTime::GetCurrentTime();
//	CString time = Format(_T("%04d/%02d/"), cTime.GetYear(), cTime.GetMonth());
//	CString path = DEFAULT_DIRECTORY + _T("RecipeLog/") + time;
//	CreateSubDirectories(path);		// <- 폴더가 없으면 생성하도록 수정
//
//	CString fileName = Format(_T("%02d.csv"), cTime.GetDay());
//	path += fileName;
//	// 파일 열기
//	file.Open(path, CFile::shareDenyWrite | CFile::modeNoTruncate | CFile::modeWrite | CFile::modeCreate, &e);
//	if (e.m_cause != CFileException::none)
//	{
//		// to do message
//		TRACE(_T("%s File Exception Error : %d\n"), path, e.m_cause);
//		return;
//	}
//
//	// log 파일 쓰기
//	CArchiveU arU(&file, CArchiveU::store);
//	if (file.SeekToEnd() == 0)
//	{ // 파일 처음 생성시 항목을 적어주고 시작한다
//		arU.WriteStringU(_T("시간;Recipe 명;Recipe 항목;기존 값;변경 값;사용자\r\n"));
//	}
//
//	for (int i = 0; i < vec.size(); i++) {
//		arU.WriteStringU(GetTimeStringHMS(true) + _T(";"));
//		arU.WriteStringU(vec[i] + _T(";"));
//		arU.WriteStringU(theApp.m_UserManager.GetCurrentUserData().szID);			// User 명 
//		arU.WriteStringU(_T("\r\n"));
//	}
//	arU.Close();
//	file.Close();
//
//}
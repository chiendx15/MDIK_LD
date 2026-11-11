#include "pch.h"
#include "UI.h"
#include "RecipeInfoWnd.h"


// CRecipeInfoWnd

IMPLEMENT_DYNAMIC(CRecipeInfoWnd, CWnd)

CRecipeInfoWnd::CRecipeInfoWnd(bool& _editAble, T_MDI_RECIPE& _recipe, int _nameWidth, int _valueWidth, int _unitWidth)
	: m_editAble(_editAble)
	, m_recipe(_recipe)
	, m_nameWidth(_nameWidth)
	, m_valueWidth(_valueWidth)
	, m_unitWidth(_unitWidth)
{}

CRecipeInfoWnd::~CRecipeInfoWnd()
{}

void CRecipeInfoWnd::LoadInitialize()
{
	for (auto& func : m_infoSyncFunctions)
	{
		func(true);
	}
}

void CRecipeInfoWnd::ChangeEditColor()
{
	auto color = m_editAble ? RGB(250, 230, 230) : RGB(230, 230, 250);
	int size = m_infoList.GetItemCount();
	for (int item = 0; item < size; ++item)
	{
		m_infoList.SetItemBKColor(item, item % 2 ? RGB(255, 255, 255) : color);
	}
}

#define IDC_RECIPE_INFO_LIST			WM_APP + 5205
BEGIN_MESSAGE_MAP(CRecipeInfoWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_RECIPE_INFO_LIST, CRecipeInfoWnd::OnLvnInfoBeginEdit)
	ON_NOTIFY(LVN_ENDLABELEDIT	, IDC_RECIPE_INFO_LIST, CRecipeInfoWnd::OnLvnInfoEndEdit)
END_MESSAGE_MAP()

int CRecipeInfoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_infoList.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, CRect(), this, IDC_RECIPE_INFO_LIST);
	m_infoList.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_infoList.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_infoList.SetRowHeight(40);

	m_infoList.InsertColumn(eRIC_NONE	, _T("")		, LVCFMT_LEFT, 0, 1);
	m_infoList.InsertColumn(eRIC_NAME	, _T("항목명")	, LVCFMT_CENTER, m_nameWidth, 1);
	m_infoList.InsertColumn(eRIC_VALUE	, _T("값")		, LVCFMT_CENTER, m_valueWidth, 1);
	m_infoList.InsertColumn(eRIC_UNIT	, _T("단위")		, LVCFMT_CENTER, m_unitWidth, 1);

	m_infoList.SetColumnEdit(eRIC_VALUE); // 변경 가능하도록 수정
	m_infoList.SetAlignText(eRIC_NAME	, eLCETA_LEFT);
	m_infoList.SetAlignText(eRIC_VALUE	, eLCETA_RIGHT);
	m_infoList.SetAlignText(eRIC_UNIT	, eLCETA_CENTER);

	m_infoList.ShowWindow(SW_SHOW);

	return 0;
}

void CRecipeInfoWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	m_infoList.MoveWindow(0, 0, cx, cy);
}


void CRecipeInfoWnd::OnLvnInfoBeginEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!m_editAble)
	{
		*pResult = -1;
	}
}

void CRecipeInfoWnd::OnLvnInfoEndEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!m_editAble)
	{
		*pResult = -1;
		return;
	}

	*pResult = 0;
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	// 선택된 아이템만 업데이트
	int item = pDispInfo->item.iItem;
	m_infoSyncFunctions[item](false);
}

// InsertMaterialListWnd.cpp : implementation file
//

#include "pch.h"
#include "UI.h"
#include "InsertMaterialListWnd.h"

// CInsertMaterialListWnd
IMPLEMENT_DYNCREATE(CInsertMaterialListWnd, CFrameWndEx)

CInsertMaterialListWnd::CInsertMaterialListWnd(CWnd* pwndParent) : TIPCT<CFrameWndEx>(&g_ipClient)
{}

void CInsertMaterialListWnd::ComputArea() 
{
	GetClientRect(m_rectClient);

	m_rectS[eS_FULL_AREA] = m_rectClient;
	CRect rc = m_rectS[eS_FULL_AREA];

	double dW = (rc.Width() - 5.) / 5.;
	double dH = (rc.Height() - 5.) / 10.;
	m_rectS[eS_TITLE] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 5, rc.top + dH * 1);
	m_rectS[eS_LIST] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 5, rc.top + dH * 9);
	m_rectC[eC_CLOSE] = CRect(rc.left + 5 + dW * 4, rc.top + 5 + dH * 9, rc.left + dW * 5, rc.top + dH * 10);
	m_rectC[eC_APPLY] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 9, rc.left + dW * 1, rc.top + dH * 10);

	if (m_listMaterial.m_hWnd)
	{
		m_listMaterial.MoveWindow(m_rectS[eS_LIST]);
	}
}

void CInsertMaterialListWnd::GetMaterialList()
{
	m_glassInfoList = GetGlassDataList();

	if (m_listMaterial.GetItemCount())
		m_listMaterial.DeleteAllItems();

	int iCount = 0;
	for (auto& data : m_glassInfoList)
	{
		m_listMaterial.AddItem(_T(""));
		m_listMaterial.SetItemText(iCount, eLI_MATERIAL_REMOVAL_TIME, CString(data.szTimeOfRemoval));
		m_listMaterial.SetItemText(iCount, eLI_MCR_ID, CString(data.szCELLID));
		//m_listMaterial.SetItemText(iCount, eLI_GLASS_ID, CString(data.szCELLID));
		m_listMaterial.SetItemText(iCount, eLI_LOT_ID, CString(data.szCST_ID));
		++iCount;
	}

	m_listMaterial.Complete();
}

BEGIN_MESSAGE_MAP(CInsertMaterialListWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

int CInsertMaterialListWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_listMaterial.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_OWNERDATA, CRect(), this, 10001);
	m_listMaterial.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	m_listMaterial.InsertColumn(eLI_MATERIAL_REMOVAL_TIME, g_st[IDS_ISERT_MATERIAL_WND_SCRAP_POINT], LVCFMT_CENTER, 150);
	m_listMaterial.InsertColumn(eLI_MCR_ID, _T("MCR ID"), LVCFMT_CENTER, 140);
	m_listMaterial.InsertColumn(eLI_GLASS_ID, _T("GLASS ID"), LVCFMT_CENTER, 130);
	m_listMaterial.InsertColumn(eLI_LOT_ID, _T("LOT ID"), LVCFMT_CENTER, 130);
	return 0;
}

void CInsertMaterialListWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	
	if (bShow)
	{
		SetWindowText(_T("Insert Materail List"));
		int iW = (g_rectArea[eAREA_WORK].Width() - 5) / 3;
		int iH = (g_rectArea[eAREA_WORK].Height() - 5) / 1.5 ;
		SetWindowPos(&wndTopMost, g_rectArea[eAREA_WORK].left + iW, g_rectArea[eAREA_WORK].top, iW, iH, SWP_SHOWWINDOW);
		// 자재 리스트 불러오기
		GetMaterialList();

		ComputArea();
		Invalidate();
	}
}

void CInsertMaterialListWnd::OnPaint()
{
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CRect				rect = m_rectS[eS_FULL_AREA];
	memDC.FillSolidRect(rect, g_colorBackground[eEC_DISABLE]);
	rect.DeflateRect(3, 3, 3, 3);
	CDrawSymbol::DrawLineRect(&memDC, rect, g_colorForeground[eEC_DEFAULT]);

	DrawLineRect(&memDC, m_rectS[eS_FULL_AREA]);

	DrawMDIMenuButtonN(&memDC, m_rectC[eC_CLOSE], g_st[IDS_CLOSE], m_eSelectControl == eC_CLOSE);
	DrawMDIMenuButtonN(&memDC, m_rectC[eC_APPLY], g_st[IDS_ISERT_MATERIAL_WND_SELECT], m_eSelectControl == eC_APPLY);
}

void CInsertMaterialListWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	for (int idx = 0; idx < eC_END; ++idx)
	{
		if (m_rectC[idx].PtInRect(point))
		{
			m_eSelectControl = idx;
			Invalidate(FALSE);
			break;
		}
	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}


void CInsertMaterialListWnd::OnLButtonUp(UINT nFlags, CPoint point){
	
	CString strLog;
	auto cleanup = make_scope_exit
	(
		[&]()
		{
			if (!strLog.IsEmpty())
			{
				WriteButtonLog(_T("Scrape"), _T("Insert Material List"), L"", strLog, theApp.m_UserManager.GetCurrentUserData().szID);
			}
			m_eSelectControl = eC_NONE;
			Invalidate(FALSE);
			CFrameWnd::OnLButtonUp(nFlags, point);
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
		case eC_CLOSE: 
		{
			ShowWindow(SW_HIDE);
		}
		break;

		case eC_APPLY: 
		{
			int selectIndex = m_listMaterial.GetSelectionMark();
			if (selectIndex < 0)
			{
				IPMessage(g_st[IDS_ISERT_MATERIAL_WND_SELECT_LIST_PLEASE]);
				break;
			}

			auto iter = m_glassInfoList.begin();
			std::advance(iter, selectIndex);

			SetToUpperInit(m_eModule);
			SetToLowerInit(m_eModule);
			sm.Module[m_eModule].Status.wSeqStep = m_wStep;
			
			if (m_eModule == eMODULE_PC1_GLASS_TRANSFER)
				sm.Module[m_eModule].Status.wMaterialType = eMT_ORG_GLASS;

			auto& glass = sm.Module[m_eModule].Glass;
			memcpy(&glass, &(*iter), sizeof(T_CELL_INFO));
			glass.bMaterial = TRUE;
			
			RemoveCellData(CString(iter->szTimeOfRemoval));

			//<< 2024.06.29 FDC
			if (m_eModule == eMODULE_PC1_GLASS_TRANSFER) {
				SET_STEP_CELL(sm.FDC->GlassTransfer, 1, sm.Module[m_eModule].Glass.szCELLID);
			}
			//>>

			IPMessage(g_st[IDS_ISERT_MATERIAL_WND_ADD_SUCCESS]);
			strLog = Format(_T("%s %s"), CString(glass.szCELLID), g_st[IDS_ISERT_MATERIAL_WND_SELECT]);
			ShowWindow(SW_HIDE);
		}
		break;
	}
}

void CInsertMaterialListWnd::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	ComputArea();
}

void CInsertMaterialListWnd::OnClose()
{
	ShowWindow(SW_HIDE);
	return;
}

BEGIN_IP_COMMAND_TABLE(CInsertMaterialListWnd, IIPCommandTarget)
	ON_IP_EXCL("Insert", "Material", OnIPInsertMaterial)
END_IP_COMMAND_TABLE()

BOOL CInsertMaterialListWnd::OnIPInsertMaterial(CIPCommand& cmd) 
{
	const CIPVar& var = cmd.m_var;

	m_wStep		= var.GetChildItem("Step", -1);
	m_eModule	= (eMODULEs)var.GetChildItem("Module", eMODULE_NONE);
	ShowWindow(SW_SHOW);
	return TRUE;
}
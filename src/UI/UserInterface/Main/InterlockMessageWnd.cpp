#include "pch.h"
#include "UI.h"
#include "InterlockMessageWnd.h"
#include "MainFrame.h"

#define IDC_MSG_LIST 4088
#define IDC_TIMER_MSG_CHECK	9090

IMPLEMENT_DYNCREATE		(CInterlockMessageWnd , CFrameWndEx)
BEGIN_MESSAGE_MAP		(CInterlockMessageWnd , CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MSG_LIST, OnSelchanging)
	ON_WM_TIMER()
END_MESSAGE_MAP			()


BEGIN_IP_COMMAND_TABLE(CInterlockMessageWnd , IIPCommandTarget)
	ON_IP("INTERLOCK", "SHOW", OnIP_INTERLOCK_MESSAGE_SHOW)
END_IP_COMMAND_TABLE()

BOOL CInterlockMessageWnd ::OnIP_INTERLOCK_MESSAGE_SHOW(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	if (!cmd.IsChildOK() || !cmd.IsOK())
		return FALSE;

	UpdateData();
	return TRUE;
}



CInterlockMessageWnd ::CInterlockMessageWnd () 
	: IWNDIPCommandTarget(m_hWnd, &g_ipClient)
{

}

CInterlockMessageWnd ::~CInterlockMessageWnd () 
{}



BOOL CInterlockMessageWnd ::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}


int CInterlockMessageWnd ::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	typedef struct T_LIST_DATA
	{
		CString ColumnName;
		int ColumnSize;
	}T_LIST_DATA;

	T_LIST_DATA strListMSG[] =
	{
		{	L""					, 0  },
		{	L"CONFIRM"			, 150	},
		{	L"OCCURRED"			, 120	},
		{	L"TIME"				, 150	},		
		{	L"ID"				, 50	},
		{	L"MODULE"			, 150	},
		{	L"MESSAGE"			, 900	},
	};

	m_lst.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_OWNERDATA, CRect(), this, IDC_MSG_LIST);
	m_lst.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	for (int i = 0; i < countof(strListMSG); i++)
	{
		m_lst.InsertColumn(i, strListMSG[i].ColumnName, LVCFMT_CENTER, strListMSG[i].ColumnSize);
	}
	
	UpdateData();

	SetTimer(IDC_TIMER_MSG_CHECK, 500, NULL);
	return 0;
}


void CInterlockMessageWnd ::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) 
	{
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		ComputeArea();
		m_eSelectControl	= eC_NONE;
		UpdateData();
		Invalidate(FALSE);
	}
}


void CInterlockMessageWnd ::OnPaint() 
{
	CPaintDC				dc(this);
	CMemDCEx::CMemDC		memDC(&dc);
	CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_DEFAULT]);
	CBrush*					pOldBrush = (CBrush*)memDC.SelectObject(&g_brushBackground[eEC_DEFAULT]);
	CPen*					pOldPen = (CPen*)memDC.SelectObject(&pen);
	CRect					rect = m_rectS[eS_FULL_AREA];
	memDC.SelectObject(&g_brushBackground[eEC_DEFAULT]);
	rect.DeflateRect(1, 0);
	memDC.FillSolidRect(rect, DT_BK);

	DrawTextB(memDC, m_rectS[eS_LIST_TABLE], L"Interlock Message LIST", 15, FW_HEAVY, RGB_WHITE, DT_CENTER | DT_VCENTER);

	DrawTextB(memDC, m_rectS[eS_TEXT_MSG], L"MSG : ", 13);
	SelectedDrawEditText(memDC, m_rectC[eC_EDIT_MSG], m_strMSG, TRUE, 13);
	Draw3dGradationButton(memDC, m_rectC[eC_BTN_CONFIRM], L"Confirm", m_eSelectControl == eC_BTN_CONFIRM, DT_FONTSIZE, eGO_H, DT_TEXT, RGB_GRAY_DARK, RGB_WHITE);

	memDC.SelectObject(pOldBrush);
	memDC.SelectObject(pOldPen);
}



void CInterlockMessageWnd ::OnMouseMove(UINT nFlags, CPoint point) 
{
	int eOldMouseOver	= m_eMouseOver;
	m_eMouseOver		= eC_NONE;
	
	for (int idx = 0; idx < eC_END; ++idx) 
	{
		if (m_rectC[idx].PtInRect(point))
		{
			m_eMouseOver	= idx;
			break;
		}
	}

	if (m_eMouseOver != eOldMouseOver || eOldMouseOver != eC_NONE) 
	{
		m_eSelectControl = eC_NONE;
		InvalidateRect(m_rectC[eOldMouseOver], FALSE);
	}

	CFrameWndEx::OnMouseMove(nFlags, point);
}



void CInterlockMessageWnd ::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	m_eSelectControl = eC_NONE;	
	for (int idx = 0; idx < eC_END; ++idx) 
	{
		if (m_rectC[idx].PtInRect(point)) 
		{
			m_eSelectControl = idx;
			InvalidateRect(m_rectC[idx], FALSE);
			break;
		}
	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}



void CInterlockMessageWnd ::OnLButtonUp(UINT nFlags, CPoint point)  
{
	auto cleanup = make_scope_exit
	(
		[this, nFlags, point]()
		{
			InvalidateRect(m_rectC[m_eSelectControl], FALSE);
			m_eSelectControl = eC_NONE;
			CFrameWndEx::OnLButtonUp(nFlags, point);
		}
	);

	if (!m_rectC[m_eSelectControl].PtInRect(point)) {
		return;
	}

	switch (m_eSelectControl) {
		case eC_BTN_CONFIRM: {
			if (!sm.Sequence->bCIMSkip)
				CIM::Message::S6F11_INTERLOCK_CONFIRM(eEQP_SCT);
			else
				ShowWindow(SW_HIDE);
			break;
		}
	}
}

void CInterlockMessageWnd ::ComputeArea() 
{
	GetClientRect(m_rectS[eS_FULL_AREA]);

	CRect	rect	= m_rectS[eS_FULL_AREA];
	rect.DeflateRect(10, 10);	
	
	double dW = (rect.Width() - 5.) / 9.;
	double dH = (rect.Height() - 5.) / 25.;

	m_rectS[eS_LIST_TABLE]	= CRect(rect.left+5+dW*0, rect.top+5+dH*0, rect.left+dW*8, rect.top+dH*1);
	m_rectS[eS_LIST_AREA]	= CRect(rect.left+5+dW*0, rect.top+5+dH*2, rect.left+dW*8, rect.top+dH*18);
	m_rectS[eS_TEXT_MSG] = CRect(rect.left + 5 + dW * 0, rect.top + 5 + dH * 18, rect.left + dW * 0.5, rect.top + dH * 25);
	m_rectC[eC_EDIT_MSG] = CRect(rect.left + 5 + dW * 0.5, rect.top + 5 + dH * 18, rect.left + dW * 8, rect.top + dH * 25);
	m_rectS[eS_ETC_AREA] = CRect(rect.left + 5 + dW * 8, rect.top + 5 + dH * 2, rect.left + dW * 9, rect.top + dH * 25);

	rect = m_rectS[eS_ETC_AREA];
	dW = (rect.Width() - 5.) / 3.;
	dH = (rect.Height() - 5.) / 23.;

	m_rectC[eC_BTN_CONFIRM]				= CRect(rect.left + 5 + dW * 0, rect.top + dH * 0, rect.left + dW * 3, rect.top + dH * 6);

	m_lst.MoveWindow(m_rectS[eS_LIST_AREA]);
}

void CInterlockMessageWnd ::UpdateData()
{
	const auto [result, msgs] = LoadMSG(eEQP_SCT, eMSG_TYPE_INTERLOCK);
	if (!result)
		return;

	//<< KES 추가 2024.07.21 - 내용 바뀐게 없으면 리스트 갱신 안함
	BOOL bUpdate = FALSE;
	if (m_vecMessage.size() != msgs.size()) {
		bUpdate = TRUE;
	}
	//>>

	if (!bUpdate) {
		return;
	}

	if (m_lst.GetItemCount())
		m_lst.DeleteAllItems();

	m_vecMessage.clear();

	int nItem = (int)msgs.size();
	for (int i = 0; i < nItem; i++) {
		m_vecMessage.push_back(msgs[i]);
		auto Item = msgs[i];

		m_lst.AddItem(L"");
		m_lst.SetItemText(i, eLI_CONFIRM, Format(L"%s", Item.bConfirm ? L"TRUE":L"FALSE"));
		m_lst.SetItemText(i, eLI_OCCURRED,	CString(Item.szOCCURRED));
		m_lst.SetItemText(i, eLI_TIME,		CString(Item.szTIME));
		m_lst.SetItemText(i, eLI_ID,		CString(Item.szID));
		m_lst.SetItemText(i, eLI_MODULE,	CString(Item.szMODULEID));
		m_lst.SetItemText(i, eLI_MSG,		CString(Item.szMSG));
	}
	
	m_lst.Complete();

	if (!sm.Sequence->bCIMSkip) {
		if ((int)m_vecMessage.size()) {
			ShowWindow(SW_SHOW);
		}
		else {
			ShowWindow(SW_HIDE);
		}
	}
	else {
		ShowWindow(SW_HIDE);
	}
}

void CInterlockMessageWnd ::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	int iSelectIndex;
	if ((pNMLV->uNewState & LVIS_SELECTED) && !(pNMLV->uOldState & LVIS_SELECTED)) { // Select		
		iSelectIndex = pNMLV->iItem;
		m_strMSG = m_lst.GetItemText(iSelectIndex, eLI_MSG);

		InvalidateRect(m_rectC[eC_EDIT_MSG], FALSE);
	}
}


//
//BOOL CInterlockMessageWnd::GetInterlock(int iUnit) {
//	BOOL bInterlock = FALSE;
//
//	CStringA strEQP(sm.HSMSSystem[m_type].LAYER_1[0].Module.szId);
//	CStringA strUnit;
//	if (iUnit != -1) {
//		strUnit = CStringA(sm.HSMSSystem[m_type].LAYER_1[0].LAYER_2[iUnit].Module.szId);
//	}
//
//	for (int i = 0; i < m_vecMessage.size(); i++) {
//		const auto& Msg = m_vecMessage[i];
//		BOOL bCheck = FALSE;
//
//		if ((strEQP.Compare(Msg.szMODULEID) == 0) || (strUnit.Compare(Msg.szMODULEID) == 0)) {
//			if ((!Msg.bConfirm) && (bCheck)) {
//				bInterlock = TRUE;
//				break;
//			}
//		}
//	}
//
//	return bInterlock;
//}

void CInterlockMessageWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent) {
		case IDC_TIMER_MSG_CHECK: {
			UpdateData();
			break;
		}
	}
	//__super::OnTimer(nIDEvent);
	

	CFrameWndEx::OnTimer(nIDEvent);
}

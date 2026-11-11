
#include "pch.h"
#include "UI.h"
#include "TeachMoveWnd.h"

#define INTERVAL_UI	4885	// KES 추가 2024.05.24
#define IDC_TEACH_MOVE_LIST	4901

IMPLEMENT_DYNCREATE(CTeachMoveWnd, CFrameWndEx)

CTeachMoveWnd::CTeachMoveWnd(CWnd* pwndParent)
{
	m_pwndParent = pwndParent;

	//>> KES 추가 2024.05.23
	T_POS* Pos = &sm.MotorPos_1->EL_Convryor.Load_Z;
	for (int iIndex = 0; iIndex < (sizeof(sm.MotorPos_1->EL_Convryor) / sizeof(T_POS)); iIndex++) {
		if ((Pos + iIndex) != nullptr)
			g_MoveButtonStatus.Add(eMB_PPBOX_EV_TEACH_POS + iIndex,  (Pos + iIndex));
	}
	Pos = &sm.MotorPos_1->Glass_TR.Wait_Y;
	for (int iIndex = 0; iIndex < (sizeof(sm.MotorPos_1->Glass_TR) / sizeof(T_POS)); iIndex++) {
		if ((Pos + iIndex) != nullptr)
			g_MoveButtonStatus.Add(eMB_GLASS_LD_TR_TEACH_POS + iIndex, (Pos + iIndex));
	}
	//<<
}

CTeachMoveWnd::~CTeachMoveWnd()
{
}

BEGIN_MESSAGE_MAP(CTeachMoveWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_APP_ED_EDIT_FINISHED, OnEditFinished)
	ON_NOTIFY(LVN_ENDLABELEDIT,		IDC_TEACH_MOVE_LIST+0, OnLvnEndEditPPBoxEV)
	ON_NOTIFY(LVN_ENDLABELEDIT,		IDC_TEACH_MOVE_LIST+1, OnLvnEndEditGlassTransfer)
	
END_MESSAGE_MAP()

BOOL CTeachMoveWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CTeachMoveWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	for (int i = 0; i < TEACH_MOVE_COUNT; ++i) {
		m_lst[i].Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, m_rectS[eS_PPBOX_EV +i], this, IDC_TEACH_MOVE_LIST+i);
		m_lst[i].SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
		m_lst[i].ModifyStyle(0, LVS_SHOWSELALWAYS);
		m_lst[i].SetRowHeight(45);
		m_lst[i].ShowScrollBar(SB_VERT);
		m_lst[i].ShowScrollBar(SB_HORZ);

		m_lst[i].InsertColumn(eLN_NONE,			_T(""),				LVCFMT_LEFT, 0, 1);
		m_lst[i].InsertColumn(eLN_NAME,			_T("Name"),			LVCFMT_LEFT, 240, 1);
		m_lst[i].InsertColumn(eLN_MOVE,			_T(""),				LVCFMT_LEFT, 75, 1);
		m_lst[i].SetAlignText(eLN_NAME, eLCETA_LEFT);
		m_lst[i].SetColumnButton(eLN_MOVE,		_T("Move"),			RGB_GRAY);

		int nItem = 0;
		T_POS_PARAM* pPos = nullptr;
		int textID = 0;
		switch (i) {
		case 0: pPos = PPBoxEV;		        nItem = countof(PPBoxEV);		    textID = IDS_POSITION_PARAMETER_PPBOX_LOAD_Z;			break;
		case 1: pPos = GlassLoadTrasnfer;	nItem = countof(GlassLoadTrasnfer);	textID = IDS_POSITION_PARAMETER_GLASS_TRANSFER_WAIT_Y;	break;
		}
		
		// KES 불필요 코드 제거 2024.05.23
		for (int j = 0; j < nItem; ++j) {
			m_lst[i].InsertItem(j, _T(""));
			if (pPos[j].cr)
				m_lst[i].SetItemBKColor(j, pPos[j].cr);
			m_lst[i].SetItemText(j, eLN_NAME, g_st[textID + j]);//pPos[j].strName);
		}
	}

	return 0;
}

void CTeachMoveWnd::OnDestroy() {
	CFrameWndEx::OnDestroy();
}


void CTeachMoveWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left,g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		Invalidate(FALSE);
		m_eSelectControl = eC_NONE;
		m_eMouseOver = eC_NONE;
		SetTimer(INTERVAL_UI, 100, nullptr);
	}
	else {
		KillTimer(INTERVAL_UI);
	}
}

void CTeachMoveWnd::fn_ComputeArea()
{
	GetClientRect(m_rectS[eS_FULL_AREA]);

	CRect rect = m_rectS[eS_FULL_AREA];
	double dW = (rect.Width() - 10) / 5.;
	double dH = (rect.Height() - 5) / 42;

	m_rectS[eS_PPBOX_EV]	    = CRect(rect.left+10+dW*0, rect.top+5+dH*1, rect.left+dW*1, rect.top+dH*21);
	m_rectS[eS_GLASS_TRANSFER]	= CRect(rect.left+10+dW*1, rect.top+5+dH*1, rect.left+dW*2, rect.top+dH*21);
	
	for (int i = 0; i < TEACH_MOVE_COUNT; ++i) {
		if (m_lst[i].m_hWnd) {
			CRect rectList = m_rectS[eS_PPBOX_EV +i];
			rectList.DeflateRect(0, 10, 0, 0);
			m_lst[i].MoveWindow(rectList);
		}
	}
}

void CTeachMoveWnd::OnPaint()
{
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);

	CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_DEFAULT]);
	CBrush*					pOldBrush			= (CBrush*)memDC.SelectStockObject(NULL_BRUSH);
	CPen*					pOldPen				= (CPen*)memDC.SelectObject(&pen);

	memDC.SelectObject(g_brushBackground[eEC_DEFAULT]);
	memDC.FillRect(m_rectS[eS_FULL_AREA],&g_brushBackground[eEC_DEFAULT]);

	DrawLineRect(memDC, m_rectS[eS_FULL_AREA], RGB_GRAY);

	DrawGroupBox(&memDC, m_rectS[eS_PPBOX_EV            ],	_T("PPBox Elevator"),	RGB_BLACK, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_GLASS_TRANSFER		],	_T("Glass Transfer"),	RGB_BLACK, RGB_BLACK);

	memDC.SelectObject(pOldBrush);
	memDC.SelectObject(pOldPen);
}

void CTeachMoveWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	for (int i = 0; i < eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			InvalidateRect(m_rectC[i], FALSE);
			break;
		}
	}
	
	CFrameWndEx::OnLButtonDown(nFlags, point);
}

void CTeachMoveWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ((m_eSelectControl != eC_NONE) && m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			default: {
				break;
			}
		}
	}

	m_eSelectControl = eC_NONE;
	Invalidate(FALSE);
	CFrameWndEx::OnLButtonUp(nFlags, point);
}

void CTeachMoveWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	int eMouseOver = eC_NONE;
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

void CTeachMoveWnd::OnTimer(UINT_PTR nIDEvent)
{
	//>> KES 추가 2024.05.23
	switch (nIDEvent) {
		case INTERVAL_UI: {
			g_MoveButtonStatus.UpdateMoveButtonStatus(eMB_PPBOX_EV_TEACH_POS, eMB_GLASS_LD_TR_TEACH_POS_END);

			for (int iIndex = 0; iIndex < countof(PPBoxEV); iIndex++) {
				m_lst[0].SetTextColor(iIndex, eLN_MOVE, g_MoveButtonStatus.GetStatusColorText(eMB_PPBOX_EV_TEACH_POS + iIndex));
				m_lst[0].SetBKColor(iIndex, eLN_MOVE, g_MoveButtonStatus.GetStatusColorBK(eMB_PPBOX_EV_TEACH_POS + iIndex));
			}
			for (int iIndex = 0; iIndex < countof(GlassLoadTrasnfer); iIndex++) {
				m_lst[1].SetTextColor(iIndex, eLN_MOVE, g_MoveButtonStatus.GetStatusColorText(eMB_GLASS_LD_TR_TEACH_POS + iIndex));
				m_lst[1].SetBKColor(iIndex, eLN_MOVE, g_MoveButtonStatus.GetStatusColorBK(eMB_GLASS_LD_TR_TEACH_POS + iIndex));
			}

			// KES 수정 2024.06.19
			for (int i = 0; i < TEACH_MOVE_COUNT; i++) {
				m_lst[i].Invalidate(FALSE);
			}

			break;
		}
	}
	//<<
	CFrameWndEx::OnTimer(nIDEvent);
}

LRESULT CTeachMoveWnd::OnEditFinished(WPARAM wParam, LPARAM lParam)
{
	CString str = GetEditString();
	if (wParam) {
		switch (m_eEndEdit) {
			default: {
				break;
			}
		}
	}

	m_eEndEdit = eC_NONE;
	return 0L;
}

void CTeachMoveWnd::OnLvnEndEditPPBoxEV(NMHDR* pNMHDR, LRESULT* pResult) {
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;

	int iItem = pDispInfo->item.iItem;
	int iSubItem = pDispInfo->item.iSubItem;

	int nItem = sizeof(sm.MotorPos_1->EL_Convryor) / sizeof(T_POS);
	if (iItem < 0 || iItem >= nItem)
		return;
	
	//<< KES 추가 2024.05.23
	if (g_MoveButtonStatus.IsInterlock(eMB_PPBOX_EV_TEACH_POS + iItem, g_st[IDS_POSITION_PARAMETER_PPBOX_LOAD_Z + iItem]))	
		return;
	//>>

	//<< KES 추가 2024.4.9 - 운전모드가 Manual이 아니면 수동 동작 X
	if (sm.Option->tSequence.eDriveMode != eDRIVE_MODE_MANUAL) {
		IPMessage(g_st[IDS_MS_NOT_DRIVE_MODE_MANUAL]);
		return;
	}
	//>>	

	if (!g_pRecipe) {
		IPMessage(g_st[IDS_TEACH_MOVE_WND_NONE_APPLIED_RECIPE]);
		return;
	}
	
	const T_POS* pPos = (T_POS*)&sm.MotorPos_1->EL_Convryor;
	T_POS tPos = pPos[iItem];
	switch (iSubItem) {
		case eLN_MOVE: {
			switch (iItem) {
				default: {
					PTPMove(tPos);
					break;
				}
			}
			WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], 
				_T("Cover Picker"), PPBoxEV[iItem].strName, theApp.m_UserManager.GetCurrentUserData().szID);
			break;
		}
	}
}

 // LJY 추가 2024.06.23
void CTeachMoveWnd::ChangeLanguage()
{
	for (int i = 0; i < countof(PPBoxEV); i++) {
		m_lst[0].SetItemText(i, eLN_NAME, g_st[IDS_POSITION_PARAMETER_PPBOX_LOAD_Z + i]);
	}
	for (int i = 0; i < countof(GlassLoadTrasnfer); i++) {
		m_lst[1].SetItemText(i, eLN_NAME, g_st[IDS_POSITION_PARAMETER_GLASS_TRANSFER_WAIT_Y + i]);
	}
}


void CTeachMoveWnd::OnLvnEndEditGlassTransfer(NMHDR* pNMHDR, LRESULT* pResult) {

	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;
	int iModule = eMODULE_PC1_LOAD_CONVEYOR_2;
	int iItem = pDispInfo->item.iItem;
	int iSubItem = pDispInfo->item.iSubItem;

	//<< KES 추가 2024.05.23
	if (g_MoveButtonStatus.IsInterlock(eMB_GLASS_LD_TR_TEACH_POS + iItem, g_st[IDS_POSITION_PARAMETER_GLASS_TRANSFER_WAIT_Y + iItem]))	
		return;
	//>>

	//<< KES 추가 2024.4.9 - 운전모드가 Manual이 아니면 수동 동작 X
	if (sm.Option->tSequence.eDriveMode != eDRIVE_MODE_MANUAL) {
		IPMessage(g_st[IDS_MS_NOT_DRIVE_MODE_MANUAL]);
		return;
	}
	//>>	

	//<-- PSS ADD 03.29 : 수동 동작 시 CV2에 자재가 담긴  PPBOX 가 있을때 로드위치 이동 불가 인터락 추가  
    if (iItem == 3) // 3 = Load 위치 Z
	{
		if (sm.Module[iModule].Glass.bMaterial == TRUE)
		{
			IPMessage(g_st[IDS_TEACH_MOVE_WND_CAN_NOT_MOVE_LOAD_POS_Z]);
			return;
		}
	}
	//--> PSS ADD 03.29 : 수동 동작 시 CV2에 자재가 담긴  PPBOX 가있을때 로드위치 이동 불가 인터락 추가  
	int nItem = sizeof(sm.MotorPos_1->Glass_TR) / sizeof(T_POS);
	if (iItem < 0 || iItem >= nItem)
		return;

	if (!g_pRecipe) {
		IPMessage(g_st[IDS_TEACH_MOVE_WND_NONE_APPLIED_RECIPE]);
		return;
	}

	const T_POS* pPos = (T_POS*)&sm.MotorPos_1->Glass_TR;
	T_POS tPos = pPos[iItem];
	switch (iSubItem) {
		case eLN_MOVE: {
			switch (iItem) {
				default: {
					PTPMove(tPos);
					break;
				}
			}
			WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], 
				_T("Glass Transfer"), GlassLoadTrasnfer[iItem].strName, theApp.m_UserManager.GetCurrentUserData().szID);
			break;
		}
	}
}

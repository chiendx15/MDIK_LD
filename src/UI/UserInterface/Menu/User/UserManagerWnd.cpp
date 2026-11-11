#include "pch.h"
#include "UI.h"
#include "UserManagerWnd.h"
IMPLEMENT_DYNAMIC(CUserManagerWnd, CWnd)

BEGIN_MESSAGE_MAP(CUserManagerWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CUserManagerWnd::CUserManagerWnd(CWnd* pwndParent)
{
	m_pwndParent			= pwndParent;
	m_eSelectControl		= eC_NONE;
}

CUserManagerWnd::~CUserManagerWnd()
{
	m_pwndParent			= NULL;
}


void CUserManagerWnd::ComputeArea(){
	GetClientRect(m_rectClient);
	CRect rect = m_rectClient ; 
	double dW = (rect.Width()-5)/5;
	double dH = (rect.Height()-5)/5;
	m_rectS[eS_AREA] = CRect(rect.left+5+dW*0, rect.top+5+dH*0, rect.left+dW*1, rect.top+dH*5);
	m_rectS[eS_CONTROL_AREA] = CRect(rect.left+5+dW*1, rect.top+5+dH*0, rect.left+dW*2, rect.top+dH*1.5);

	m_rectS[eS_LIST_TITLE] = CRect(m_rectS[eS_AREA].left + 15, m_rectS[eS_AREA].top + 5, m_rectS[eS_AREA].right - 5, m_rectS[eS_AREA].top + 25);
	m_rectS[eS_LIST_AREA] = CRect(m_rectS[eS_AREA].left + 5, m_rectS[eS_AREA].top + 30, m_rectS[eS_AREA].right - 5, m_rectS[eS_AREA].bottom - 100);

	dW = (m_rectS[eS_AREA].Width() - 5.) / 3.;
	m_rectC[eC_BTN_ADD] = CRect(m_rectS[eS_AREA].left + 5, m_rectS[eS_LIST_AREA].bottom + 5, m_rectS[eS_AREA].left + dW, m_rectS[eS_AREA].bottom - 5);
	m_rectC[eC_BTN_MODIFY] = CRect(m_rectS[eS_AREA].left + 5 + dW, m_rectS[eS_LIST_AREA].bottom + 5, m_rectS[eS_AREA].left + dW * 2, m_rectS[eS_AREA].bottom - 5);
	m_rectC[eC_BTN_DELETE] = CRect(m_rectS[eS_AREA].left + 5 + dW * 2, m_rectS[eS_LIST_AREA].bottom + 5, m_rectS[eS_AREA].right - 5, m_rectS[eS_AREA].bottom - 5);
	if (m_lstUserInfo){
		m_lstUserInfo.MoveWindow(m_rectS[eS_LIST_AREA]);
	}
	if (m_UserAddwnd){
		rect = m_rectS[eS_CONTROL_AREA];
		ClientToScreen(rect);
		m_UserAddwnd.MoveWindow(rect);
	}
	if (m_UserModifyWnd){
		rect = m_rectS[eS_CONTROL_AREA];
		ClientToScreen(rect);
		m_UserModifyWnd.MoveWindow(rect);
	}
	

}
int CUserManagerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_brushBackground.DeleteObject();
	m_colorBackground						= GetSysColor(CTLCOLOR_DLG);
	m_brushBackground.CreateSolidBrush(m_colorBackground);

	ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);										// Remove Title Bar
	ModifyStyleEx(this->m_hWnd, WS_EX_CLIENTEDGE, 0, 0);
	SetWindowText(_T("User Manager"));

	m_lstUserInfo.Create(LVS_REPORT | LVS_SHOWSELALWAYS | WS_VISIBLE | WS_CHILD | WS_BORDER, m_rectS[eS_LIST_AREA], this, USER_INFO_ID);
	m_lstUserInfo.InsertColumn(LI_NONE, _T(""), LVCFMT_RIGHT, 0, 0);
	m_lstUserInfo.InsertColumn(LI_INDEX, _T("No."), LVCFMT_CENTER, 50, 0);
	m_lstUserInfo.InsertColumn(LI_USER_ID, _T("ID"), LVCFMT_CENTER, 150, 1);
	m_lstUserInfo.InsertColumn(LI_USER_GRADE, _T("Grade"), LVCFMT_CENTER, 150, 1);

	m_lstUserInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstUserInfo.SetEditable(FALSE);
	ListRenew();

	m_UserAddwnd.Create(NULL, NULL, NULL, CRect(-1, -1, -1, -1), this);
	m_UserModifyWnd.Create(NULL, NULL, NULL, CRect(-1, -1, -1, -1), this);
	return 0;
}

void CUserManagerWnd::OnDestroy() 
{
	CWnd::OnDestroy();
}

BOOL CUserManagerWnd::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

void CUserManagerWnd::OnPaint() 
{
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	memDC.FillRect(m_rectClient,&g_brushBackground[eEC_DEFAULT]);

	DrawLineRect(memDC, m_rectClient);
	DrawLineRect(memDC, m_rectS[eS_AREA]);
	DrawText(memDC, m_rectS[eS_LIST_TITLE], g_st[IDS_USER_MANAGER_WND_TILTE], 11, RGB_BLACK);
	
	DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_ADD]	, g_st[IDS_USER_ADD]	, m_eSelectControl == eC_BTN_ADD	, FALSE, 11);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_MODIFY], g_st[IDS_USER_MODIFY]	, m_eSelectControl == eC_BTN_MODIFY	, FALSE, 11);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_BTN_DELETE], g_st[IDS_USER_DELETE]	, m_eSelectControl == eC_BTN_DELETE	, FALSE, 11);
}

void CUserManagerWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	if (bShow) {
		m_eSelectControl	= eC_NONE;
		
		m_eSelectControl	= eC_NONE;
		TPointD ptScreen;
		ptScreen.x = GetSystemMetrics(SM_CXSCREEN);
		ptScreen.y = GetSystemMetrics(SM_CYSCREEN);
		SetWindowPos(&wndTop, g_rectArea[eAREA_VISION].left,g_rectArea[eAREA_VISION].top, g_rectArea[eAREA_VISION].Width(), g_rectArea[eAREA_VISION].Height(), SWP_SHOWWINDOW);
		ComputeArea();
		ListRenew();
		
		Invalidate(FALSE);
	}
	else {
		m_UserAddwnd.ShowWindow(SW_HIDE);
		m_UserModifyWnd.ShowWindow(SW_HIDE);
	}
}

void CUserManagerWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);
}

void CUserManagerWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	for (int iIndex=0; iIndex<eC_END; iIndex++) {
		if (m_rectC[iIndex].PtInRect(point)) {
			m_eSelectControl	= iIndex;
			Invalidate(FALSE);
			return;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CUserManagerWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CString strLog;
	if ((m_eSelectControl > eC_NONE) && (m_rectC[m_eSelectControl].PtInRect(point))) {

		switch(m_eSelectControl) {
			case eC_BTN_ADD: {
				m_UserModifyWnd.ShowWindow(SW_HIDE);
				m_UserAddwnd.ShowWindow(SW_SHOW);
				strLog = _T("Add");
				break;
			}
			case eC_BTN_MODIFY: {
				m_UserAddwnd.ShowWindow(SW_HIDE);
				int rIndex = m_lstUserInfo.GetNextItem(-1, LVNI_SELECTED);
				if (rIndex > -1) {
					const USER_DATA UserData = theApp.m_UserManager.GetUserData();
			
					if (rIndex < 0 || rIndex > UserData.nUserData)
						return;
			
					if (sm.DeviceParameter->eUserGrade < UserData.UserItem[rIndex].uGrade) {
						IPMessage(g_st[IDS_USER_MANAGER_WND_EDIT_USER_HIGHER_THAN_LOGIN_USER]);

						return;
					}
					m_UserModifyWnd.setData(rIndex);
					m_UserModifyWnd.ShowWindow(SW_SHOW);
				}else{
					IPMessage(g_st[IDS_USER_MANAGER_WND_SELECT_USER]);
				}
				strLog = _T("Modify");
				break;
			}
			case eC_BTN_DELETE: {
				int rIndex = m_lstUserInfo.GetNextItem(-1, LVNI_SELECTED);		

				if (rIndex > -1) {
					USER_ITEM User = theApp.m_UserManager.GetCurrentUserData();
					CString strID = m_lstUserInfo.GetItemText(rIndex, LI_USER_ID);

					BOOL bDelete = TRUE;
					int iTemp = m_lstUserInfo.GetItemCount();
					if (User.szID) {
						if (strID.Compare(User.szID) == 0) {
							bDelete = FALSE;
						}
					}
					else {
						if (m_lstUserInfo.GetItemCount() <= 1)
							bDelete = FALSE;
					}
					if (!bDelete) {
						IPMessage(g_st[IDS_USER_MANAGER_WND_CAN_NOT_DELETE_CURRENT_USER]);

					}
					else {
						if (MessageBox(g_st[IDS_USER_MANAGER_WND_DELETE], _T("User Manager"), MB_YESNO) == IDYES) {
							theApp.m_UserManager.DeleteUserData(rIndex);
							ListRenew();
						}
					}
				}else{
					IPMessage(g_st[IDS_USER_MANAGER_WND_SELECT_USER]);
				}
				strLog = _T("Delete");
				break;
			}
			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl						= eC_NONE;
	Invalidate(FALSE);
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CUserManagerWnd::ListRenew() {
	m_lstUserInfo.DeleteAllItems();

	const USER_DATA& tUserData = theApp.m_UserManager.GetUserData();
	for (int iIndex = 0; iIndex < tUserData.nUserData; iIndex++ ) {
		int rIndex = m_lstUserInfo.InsertItem(m_lstUserInfo.GetItemCount(), _T(""));
		if (rIndex < 0)
			continue;
	
		m_lstUserInfo.SetItemText(rIndex, LI_INDEX,			Format(_T("%d"), rIndex+1));
		m_lstUserInfo.SetItemText(rIndex, LI_USER_ID,		tUserData.UserItem[iIndex].szID);
		if ((tUserData.UserItem[iIndex].uGrade >= eUSER_NONE) && (tUserData.UserItem[iIndex].uGrade <= eUSER_ADMIN))
			m_lstUserInfo.SetItemText(rIndex, LI_USER_GRADE,	theApp.m_UserManager.GetUserGradeText(tUserData.UserItem[iIndex].uGrade));
	}
}
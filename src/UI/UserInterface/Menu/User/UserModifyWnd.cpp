#include "pch.h"
#include "UI.h"
#include "UserModifyWnd.h"
#include "UserManagerWnd.h"

IMPLEMENT_DYNAMIC(CUserModifyWnd, CFrameWnd)

BEGIN_MESSAGE_MAP(CUserModifyWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_APP_ED_EDIT_TAB, OnKeyDownTab)
	ON_LBN_SELCHANGE(IDC_EDIT_MODIFY_USER_GRADE,		OnSelchangeUserGrade)
END_MESSAGE_MAP()

CUserModifyWnd::CUserModifyWnd(CWnd* pwndParent)
{
	m_pwndParent			= pwndParent;
	m_eSelectControl		= eC_NONE;
	m_bIDCheck = FALSE;
	m_iKeyDownTab = eC_EDIT_CURRENT_PASS;
}

CUserModifyWnd::~CUserModifyWnd()
{
	m_pwndParent			= NULL;
}

void CUserModifyWnd::ComputeArea(){
	GetClientRect(m_rectClient);
	m_rectS[eS_AREA] = CRect(5, 5, m_rectClient.Width() - 5, m_rectClient.Height() - 35);
	m_rectC[eC_BTN_CLOSE] = CRect(m_rectClient.right - 100, m_rectClient.bottom - 30, m_rectClient.right - 5, m_rectClient.bottom - 5);

	m_rectC[eC_BTN_MODIFY] = CRect(m_rectClient.left + 5, m_rectClient.bottom - 30, m_rectClient.left + 100, m_rectClient.bottom - 5);

	CRect rectArr = m_rectS[eS_AREA];
	double dH = (rectArr.Height() - 5.) / 5.;
	int iHeight = 30;

	for (int i = 0; i <= eS_GRADE - eS_USER_ID; i++) {
		m_rectS[eS_USER_ID + i] = CRect(rectArr.left + 5, rectArr.top + 5 + i*dH, rectArr.left + 120, rectArr.top + dH + i*dH);
		m_rectC[eC_EDIT_USER_ID + i] = CRect(m_rectS[eS_USER_ID].right + 5, rectArr.top + 5 + i*dH, rectArr.right - 5, rectArr.top + dH + i*dH);
	}
	if (m_cboGrade){
		m_cboGrade.MoveWindow(m_rectC[eC_EDIT_GRADE]);
		::SendMessage(m_cboGrade.m_hWnd, CB_SETITEMHEIGHT, (WPARAM)-1, m_rectC[eC_EDIT_GRADE].Height() - 6);
	}
}
int CUserModifyWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_brushBackground.DeleteObject();
	m_colorBackground						= GetSysColor(CTLCOLOR_DLG);
	m_brushBackground.CreateSolidBrush(m_colorBackground);

	SetWindowText(_T("User Modify"));

	ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);										// Remove Title Bar
	ModifyStyleEx(this->m_hWnd, WS_EX_CLIENTEDGE, 0, 0);

	

	m_cboGrade.Create(
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | CBS_DROPDOWNLIST, 
				m_rectC[eC_EDIT_GRADE], 
				this, 
				IDC_EDIT_MODIFY_USER_GRADE);
	//SetCtrlFont(m_cboGrade, 45);
	//m_cboGrade.ShowWindow(SW_HIDE);
	m_cboGrade.ResetContent();
	
	CUserManagerWnd*	pWnd = (CUserManagerWnd*)m_pwndParent;
	
	for (int iIndex = 0; iIndex < 4; iIndex++) {
		m_cboGrade.AddString(theApp.m_UserManager.GetUserGradeText(iIndex));
	}
	m_cboGrade.UpdateData(FALSE);
	m_cboGrade.SetCurSel(0);
	OnSelchangeUserGrade();

	return 0;
}
//==================================================================================================



void CUserModifyWnd::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
}

BOOL CUserModifyWnd::OnEraseBkgnd(CDC* pDC) 
{
//	return CFrameWnd::OnEraseBkgnd(&memDC);
	return FALSE;
}

void CUserModifyWnd::OnPaint() 
{
	
	try {
		CPaintDC		dc(this);
		CMemDCEx::CMemDC		memDC(&dc);
		CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_DEFAULT]);
		CRect					rect(m_rectClient);

		memDC.SelectObject(&g_brushBackground[eEC_DEFAULT]);
		memDC.FillRect(m_rectClient,&g_brushBackground[eEC_DEFAULT]);
		CBrush*					pOldBrush			= (CBrush*)memDC.SelectObject(&g_brushBackground[eEC_DEFAULT]);
		CPen*					pOldPen				= (CPen*)memDC.SelectObject(&pen);

		memDC.SelectObject(pOldBrush);
		memDC.SelectObject(pOldPen);

		DrawLineRect(memDC, m_rectClient);
		DrawLineRect(memDC, m_rectS[eS_AREA]);
		DrawButtonUpDown(memDC, m_rectC[eC_BTN_CLOSE], g_st[IDS_CLOSE], (m_eSelectControl == eC_BTN_CLOSE));
		DrawButtonUpDown(memDC, m_rectC[eC_BTN_MODIFY], g_st[IDS_USER_MODIFY], (m_eSelectControl == eC_BTN_MODIFY));
		
		DrawText(memDC,	m_rectS[eS_USER_ID], g_st[IDS_USER_ID], DT_FONTSIZE, RGB_BLACK);
		DrawText(memDC, m_rectS[eS_CURRENT_PASS], g_st[IDS_USER_CURRENT_PASSWORD], DT_FONTSIZE, RGB_BLACK);
		DrawText(memDC, m_rectS[eS_CHANGE_PASS], g_st[IDS_USER_CHANGE_PASSWORD], DT_FONTSIZE, RGB_BLACK);
		DrawText(memDC, m_rectS[eS_PASS_CONFIRM], g_st[IDS_USER_CONFIRM], DT_FONTSIZE, RGB_BLACK);
		DrawText(memDC, m_rectS[eS_GRADE], g_st[IDS_USER_GRADE], DT_FONTSIZE, RGB_BLACK);
		
		CString strCurrnetPass, strChangePass, strConfirm;
		for (int i = 0; i < m_strCurrentPass.GetLength();i++)
			strCurrnetPass += _T("●");
		for (int i = 0; i < m_strChangePass.GetLength();i++)
			strChangePass += _T("●");
		for (int i = 0; i < m_strConfirm.GetLength();i++)
			strConfirm += _T("●");

		SelectedDrawEditText(&memDC, m_rectC[eC_EDIT_USER_ID], m_strUserID);
		SelectedDrawEditText(&memDC, m_rectC[eC_EDIT_CURRENT_PASS], strCurrnetPass);
		SelectedDrawEditText(&memDC, m_rectC[eC_EDIT_CHANGE_PASS], strChangePass);
		SelectedDrawEditText(&memDC, m_rectC[eC_EDIT_PASS_CONFIRM], strConfirm);

	}
	catch (...) {
	}
}

void CUserModifyWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	
	if (bShow) {
		ComputeArea();

		m_eSelectControl	= eC_NONE;
		m_iKeyDownTab		= eC_EDIT_CURRENT_PASS;
		
		EditStart(this, m_rectC[eC_EDIT_CURRENT_PASS], m_strCurrentPass);

		m_bIDCheck = FALSE;
		Invalidate(TRUE);
	}
}

void CUserModifyWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	for (int iIndex=0; iIndex<eC_END; iIndex++) {
		if (m_eSelectControl == iIndex) {
			if (!m_rectC[iIndex].PtInRect(point)) {
				m_eSelectControl	= eC_NONE;
				Invalidate(FALSE);
				break;
			}
		}
	}
	CFrameWnd::OnMouseMove(nFlags, point);
}

void CUserModifyWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	OnSelchangeUserGrade();
	if (m_rectS[eS_TITLE].PtInRect(point)) 
		SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
	
	for (int iIndex=0; iIndex<eC_END; iIndex++) {
		if (m_rectC[iIndex].PtInRect(point)) {
			m_eSelectControl	= iIndex;
			Invalidate(FALSE);
			return;
		}
	}
	
	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CUserModifyWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CString strLog;
	if ((m_eSelectControl > eC_NONE) && (m_rectC[m_eSelectControl].PtInRect(point))) {

		switch(m_eSelectControl) {
			case eC_BTN_CLOSE: {
				CUserManagerWnd*	pWnd = (CUserManagerWnd*)m_pwndParent;
				pWnd->ListRenew();
				pWnd->ShowWindow(SW_SHOW);
				ShowWindow(SW_HIDE);
				strLog = _T("Close");
				break;
			}
			case eC_BTN_MODIFY: {
				UserModity();
				strLog = _T("Modify");
				break;
			}
			case eC_EDIT_CURRENT_PASS: {
				EditStart(this, m_rectC[m_eSelectControl], m_strCurrentPass, FALSE, FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, 9);
				break;
			}
			case eC_EDIT_CHANGE_PASS: {
				EditStart(this, m_rectC[m_eSelectControl], m_strChangePass, FALSE, FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, 9);
				break;
			}
			case eC_EDIT_PASS_CONFIRM: {
				EditStart(this, m_rectC[m_eSelectControl], m_strConfirm, FALSE, FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, 9);
				break;
			}
			case eC_EDIT_GRADE: {
				m_cboGrade.ShowWindow(SW_SHOW);
				m_cboGrade.ShowDropDown();
				break;
			}
			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], _T("User Modify"), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl						= eC_NONE;
	Invalidate(FALSE);
	
	CFrameWnd::OnLButtonUp(nFlags, point);
}

void CUserModifyWnd::OnSelchangeUserGrade()
{
	CString			str;
	m_cboGrade.GetLBText(m_cboGrade.GetCurSel(), str);
	m_strGrade = str;
	InvalidateRect(FALSE);
}

BOOL CUserModifyWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CWnd* pParentWnd , LPCTSTR lpszMenuName , DWORD dwExStyle , CCreateContext* pContext)
{
	m_pwndParent = pParentWnd;
	return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
}

void CUserModifyWnd::setData(int iSelectUser) {
	m_iSelectUser = iSelectUser;
	const USER_DATA UserData = theApp.m_UserManager.GetUserData();
	
	if (iSelectUser < 0 || iSelectUser > UserData.nUserData)
		return;

	m_strUserID = UserData.UserItem[iSelectUser].szID;
	m_cboGrade.SetCurSel(UserData.UserItem[iSelectUser].uGrade);
	OnSelchangeUserGrade();
	m_strCurrentPass.Empty();
	m_strChangePass.Empty();
	m_strConfirm.Empty();
}


void CUserModifyWnd::UserModity() {
	if (!theApp.m_UserManager.IDPassCheck(m_strUserID, m_strCurrentPass)) {
		IPMessage(g_st[IDS_USER_MODIFY_WND_CURRENT_PASSWORD_IS_DIFFERENT]);
		m_strConfirm = _T("");
	}
	else if (m_strChangePass != m_strConfirm) {
		IPMessage(g_st[IDS_USER_MODIFY_WND_CHANGE_PASSWORD_IS_DEFFERENT]);
		m_strConfirm = _T("");
	}
	else {
			int iUserGrade = 0;
			CString str;
			for (int iIndex = 0; iIndex < eUSER_ADMIN+1; iIndex++) {
				m_cboGrade.GetLBText(iIndex, str);
				if (m_strGrade == str)
					iUserGrade = iIndex;
			}
		if (theApp.m_UserManager.ModifyUserData(m_iSelectUser, m_strChangePass, iUserGrade)) {
			CUserManagerWnd*	pWnd = (CUserManagerWnd*)m_pwndParent;
			pWnd->ListRenew();
			pWnd->ShowWindow(SW_SHOW);
			ShowWindow(SW_HIDE);
			IPMessage(g_st[IDS_USER_MODIFY_WND_EDIT_COMPLETE]);
		}
	}
	Invalidate(FALSE);
}

LRESULT CUserModifyWnd::OnKeyDownTab(WPARAM wParam, LPARAM lParam){	
	m_iKeyDownTab += wParam;
	if (m_iKeyDownTab < eC_EDIT_USER_ID)
		m_iKeyDownTab = eC_EDIT_PASS_CONFIRM;
	else if (m_iKeyDownTab > eC_EDIT_PASS_CONFIRM)
		m_iKeyDownTab = eC_EDIT_USER_ID;

	switch (m_iKeyDownTab) {
		case eC_EDIT_USER_ID : {
			EditStart(this, m_rectC[m_iKeyDownTab], m_strUserID, FALSE,FALSE, DT_CENTER | ES_AUTOHSCROLL, DT_FONTSIZE);
			break;
		}
		case eC_EDIT_CURRENT_PASS : {
			EditStart(this, m_rectC[m_iKeyDownTab], m_strCurrentPass, FALSE,FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, DT_FONTSIZE);
			break;
		}
		case eC_EDIT_CHANGE_PASS : {
			EditStart(this, m_rectC[m_iKeyDownTab], m_strChangePass, FALSE,FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, DT_FONTSIZE);
			break;
		}
		case eC_EDIT_PASS_CONFIRM : {
			EditStart(this, m_rectC[m_iKeyDownTab], m_strConfirm, FALSE,FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, DT_FONTSIZE);
			break;
		}
	}
	
	return FALSE;
}
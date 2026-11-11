#include "pch.h"
#include "UI.h"
#include "UserAddWnd.h"
#include "UserManagerWnd.h"

IMPLEMENT_DYNAMIC(CUserAddWnd, CFrameWnd)

BEGIN_MESSAGE_MAP(CUserAddWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_APP_ED_EDIT_TAB,				OnKeyDownTab)
	ON_LBN_SELCHANGE(IDC_EDIT_USER_GRADE,		OnSelchangeUserGrade)
END_MESSAGE_MAP()

CUserAddWnd::CUserAddWnd(CWnd* pwndParent)
{
	m_pwndParent			= pwndParent;
	m_eSelectControl		= eC_NONE;
	m_bIDCheck				= FALSE;
	m_iKeyDownTab			= eC_NONE;
}

CUserAddWnd::~CUserAddWnd()
{
	m_pwndParent			= NULL;
}
void CUserAddWnd::ComputeArea(){
	GetClientRect(m_rectClient);
	m_rectS[eS_AREA] = CRect(5, 5, m_rectClient.Width() - 5, m_rectClient.Height() - 35);
	m_rectC[eC_BTN_CLOSE] = CRect(m_rectClient.right - 100, m_rectClient.bottom - 30, m_rectClient.right - 5, m_rectClient.bottom - 5);

	m_rectC[eC_BTN_ADD] = CRect(m_rectClient.left + 5, m_rectClient.bottom - 30, m_rectClient.left + 100, m_rectClient.bottom - 5);

	CRect rectA = m_rectS[eS_AREA];
	double dH = (rectA.Height() - 5) / 4.;
	for (int i = 0; i <= eS_GRADE - eS_USER_ID; i++) {
		m_rectS[eS_USER_ID + i] = CRect(rectA.left + 5, rectA.top + 5 + i*dH, rectA.left + 80, rectA.top + dH + i*dH);

		if (i == 0)
			m_rectC[eC_EDIT_USER_ID + i] = CRect(rectA.left + 85, rectA.top + 5 + i*dH, rectA.right - 80, rectA.top + dH + i*dH);
		else
			m_rectC[eC_EDIT_USER_ID + i] = CRect(rectA.left + 85, rectA.top + 5 + i*dH, rectA.right - 5, rectA.top + dH + i*dH);
	}
	m_rectC[eC_BTN_USER_ID_CHECK] = CRect(rectA.right - 75, rectA.top + 5, rectA.right - 5, rectA.top + dH);
	if (m_cboGrade){
		m_cboGrade.MoveWindow(m_rectC[eC_EDIT_GRADE]);
		::SendMessage(m_cboGrade.m_hWnd, CB_SETITEMHEIGHT, (WPARAM)-1, m_rectC[eC_EDIT_GRADE].Height() - 6);
	}
}
int CUserAddWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(_T("User Add"));
	m_brushBackground.DeleteObject();
	m_colorBackground						= GetSysColor(CTLCOLOR_DLG);
	m_brushBackground.CreateSolidBrush(m_colorBackground);

	ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);										// Remove Title Bar
	ModifyStyleEx(this->m_hWnd, WS_EX_CLIENTEDGE, 0, 0);

	
	m_cboGrade.Create(
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | CBS_DROPDOWNLIST, 
				m_rectC[eC_EDIT_GRADE], 
				this, 
				IDC_EDIT_USER_GRADE);

	//SetCtrlFont(m_cboGrade, 45);

	//m_cboGrade.ShowWindow(SW_HIDE);
	m_cboGrade.ResetContent();
		
	for (int iIndex = 0; iIndex < 4; iIndex++) {
		m_cboGrade.AddString(theApp.m_UserManager.GetUserGradeText(iIndex));
	}
	m_cboGrade.UpdateData(FALSE);
	m_cboGrade.SetCurSel(0);

	OnSelchangeUserGrade();

	return 0;
}
//==================================================================================================



void CUserAddWnd::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
}

BOOL CUserAddWnd::OnEraseBkgnd(CDC* pDC) 
{
//	return CFrameWnd::OnEraseBkgnd(&memDC);
	return FALSE;
}

void CUserAddWnd::OnPaint() 
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
		DrawButtonUpDown(memDC, m_rectC[eC_BTN_ADD], g_st[IDS_USER_ADD], (m_eSelectControl == eC_BTN_ADD));

		DrawText(memDC, m_rectS[eS_USER_ID], g_st[IDS_USER_ID], DT_FONTSIZE, RGB_BLACK);
		DrawText(memDC, m_rectS[eS_USER_PASS], g_st[IDS_USER_PASSWORD], DT_FONTSIZE, RGB_BLACK);
		DrawText(memDC, m_rectS[eS_PASS_CONFIRM], g_st[IDS_USER_CONFIRM], DT_FONTSIZE, RGB_BLACK);
		DrawText(memDC, m_rectS[eS_GRADE], g_st[IDS_USER_GRADE], DT_FONTSIZE, RGB_BLACK);

		CString strPass, strConfirm;
		for (int iIndex = 0; iIndex < m_strPassword.GetLength();iIndex++)
			strPass += _T("●");

		for (int iIndex = 0; iIndex < m_strConfirm.GetLength();iIndex++)
			strConfirm += _T("●");

		SelectedDrawEditText(&memDC, m_rectC[eC_EDIT_USER_ID], m_strUserID);
		SelectedDrawEditText(&memDC, m_rectC[eC_EDIT_USER_PASS], strPass);
		SelectedDrawEditText(&memDC, m_rectC[eC_EDIT_PASS_CONFIRM], strConfirm);

		DrawButtonUpDown(memDC, m_rectC[eC_BTN_USER_ID_CHECK], g_st[IDS_USER_CHECK], (m_eSelectControl == eC_BTN_USER_ID_CHECK));
	}
	catch (...) {
	}
}

void CUserAddWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	
	if (bShow) {
		TPointD ptScreen;
		ComputeArea();
		m_bIDCheck = FALSE;
		m_strUserID.Empty();
		m_strPassword.Empty();
		m_strConfirm.Empty();
		Invalidate(TRUE);
		m_eSelectControl	= eC_NONE;
		m_iKeyDownTab		= eC_EDIT_USER_ID;
		EditStart(this, m_rectC[eC_EDIT_USER_ID], m_strUserID);
		
	}
}

void CUserAddWnd::OnMouseMove(UINT nFlags, CPoint point) 
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

void CUserAddWnd::OnLButtonDown(UINT nFlags, CPoint point) {
	SetFocus();
	OnSelchangeUserGrade();
	
	for (int iIndex=0; iIndex<eC_END; iIndex++) {
		if (m_rectC[iIndex].PtInRect(point)) {
			m_eSelectControl	= iIndex;
			Invalidate(FALSE);
			return;
		}
	}
	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CUserAddWnd::OnLButtonUp(UINT nFlags, CPoint point) {
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
			case eC_BTN_ADD: {
				UserAdd();
				strLog = _T("Add");
				break;
			}
			case eC_EDIT_USER_ID: {
				m_bIDCheck = FALSE;
				m_iKeyDownTab = m_eSelectControl;
				EditStart(this, m_rectC[m_eSelectControl], m_strUserID);
				break;
			}
			case eC_EDIT_USER_PASS: {
				m_iKeyDownTab = m_eSelectControl;
				EditStart(this,m_rectC[m_eSelectControl], m_strPassword, FALSE, FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, 9);
				break;
			}
			case eC_EDIT_PASS_CONFIRM: {
				m_iKeyDownTab = m_eSelectControl;
				EditStart(this,m_rectC[m_eSelectControl], m_strConfirm, FALSE, FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, 9);
				break;
			}
			case eC_EDIT_GRADE: {
				m_cboGrade.ShowWindow(SW_SHOW);
				m_cboGrade.ShowDropDown();
				break;
			}
			case eC_BTN_USER_ID_CHECK: {
				if (m_strUserID.IsEmpty())
					IPMessage( g_st[IDS_USER_ADD_WND_ID_IS_EMPTY]);
				else if (!theApp.m_UserManager.IDCheck(m_strUserID))
					IPMessage(g_st[IDS_USER_ADD_WND_DUPLICATED_ID]);
				else
					IPMessage(g_st[IDS_USER_ADD_WND_USEABLE]);
				strLog = _T("Check");
				break;
			}
			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], _T("User Add"), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl						= eC_NONE;
	Invalidate(FALSE);
	
	CFrameWnd::OnLButtonUp(nFlags, point);
}

void CUserAddWnd::OnSelchangeUserGrade()
{
	CString			str;
	m_cboGrade.GetLBText(m_cboGrade.GetCurSel(), str);
	m_strGrade = str;
	//m_cboGrade.ShowWindow(SW_HIDE);

	InvalidateRect(FALSE);
}

BOOL CUserAddWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CWnd* pParentWnd , LPCTSTR lpszMenuName , DWORD dwExStyle , CCreateContext* pContext)
{
	m_pwndParent = pParentWnd;
	return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
}

void CUserAddWnd::UserAdd() {
	CIPVar varMessage;
	if (theApp.m_UserManager.IDCheck(m_strUserID) && (!m_strUserID.IsEmpty())) {
		m_bIDCheck = TRUE;
	}
	CString strMsg;
	if (!m_bIDCheck) {
		strMsg += _T("ID Check.");
		strMsg += _T("\r\n");
	}
	else if (m_strPassword != m_strConfirm) {
		strMsg += _T("Password Check.");
		strMsg += _T("\r\n");
		EditStart(this, m_rectC[eC_EDIT_PASS_CONFIRM], m_strConfirm);
	}
	else {
		int iUserGrade = 0;
		CString str;
		for (int iIndex = 0; iIndex < 4; iIndex++) {
			m_cboGrade.GetLBText(iIndex, str);
			if (m_strGrade == str)
				iUserGrade = iIndex;
		}
		BOOL bCheck = theApp.m_UserManager.AddUserData(m_strUserID, m_strPassword, iUserGrade);
		m_strUserID = m_strPassword = m_strConfirm = _T("");
		m_bIDCheck = FALSE;
		CUserManagerWnd*	pWnd = (CUserManagerWnd*)m_pwndParent;
		pWnd->ListRenew();
		pWnd->ShowWindow(SW_SHOW);
		ShowWindow(SW_HIDE);

		if (bCheck) {
			strMsg += g_st[IDS_USER_ADD_WND_ADD_USER];
			strMsg += _T("\r\n");
			strMsg += g_st[IDS_USER_ADD_WND_COMPLETE];
		}
		
	}
	if (!strMsg.IsEmpty())
		IPMessage(strMsg);
	Invalidate(FALSE);
}

LRESULT CUserAddWnd::OnKeyDownTab(WPARAM wParam, LPARAM lParam) {
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
		case eC_EDIT_USER_PASS : {
			EditStart(this, m_rectC[m_iKeyDownTab], m_strPassword, FALSE,FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, DT_FONTSIZE);
			break;
		}
		case eC_EDIT_PASS_CONFIRM : {
			EditStart(this, m_rectC[m_iKeyDownTab], m_strConfirm, FALSE,FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, DT_FONTSIZE);
			break;
		}
	}
	return FALSE;
}
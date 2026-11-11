// PassWordWnd.cpp : implementation file
//

#include "pch.h"
#include "UI.h"
#include "PassWordWnd.h"
#include <WinInet.h>
// CPassWordWnd

CPassWordWnd::CPassWordWnd(CWnd* pwndParent)
{
	m_pwndParent = pwndParent;
	m_eSelectControl = eC_NONE;
}

CPassWordWnd::~CPassWordWnd()
{
}

IMPLEMENT_DYNCREATE(CPassWordWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP(CPassWordWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_MESSAGE(WM_APP_ED_EDIT_FINISHED, OnEditFinished)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPassWordWnd message handlers


//BOOL CPassWordWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, LPCTSTR lpszMenuName, DWORD dwExStyle, CCreateContext* pContext)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
//}


int CPassWordWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	SyncData();
	ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);					// Remove Title Bar
	ModifyStyleEx(this->m_hWnd, WS_EX_CLIENTEDGE, 0, 0);

	return 0;
}

BOOL CPassWordWnd::SyncData() {
	CProfile profile(_T("../cfg/PM.ini"));
	profile.SyncProfileValue(FALSE, _T("Password"), _T("Password"), m_strPassword);
	return TRUE;
}

void CPassWordWnd::OnPaint()
{
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	
	memDC.FillRect(m_rectClient, &g_brushBackground[eEC_DEFAULT]);
	DrawLineRect(&memDC, m_rectS[eS_AREA]);

	DrawTextB(memDC, m_rectS[eS_TEXT], g_st[IDS_PASSWORD_WND_INSERT_UNLOCK_PASSWORD], 15, 900, RGB_BLACK, DT_LEFT);
	CDrawSymbol::DrawText(memDC, m_rectS[eS_PASS], RGB_BLACK, DT_CENTER, 15, _T("Password : "));

	CString strTemp;
	for (int i = 0; i < m_strInputPassword.GetLength(); i++)
		strTemp += _T("●");
	CDrawSymbol::SelectedDrawText(memDC, m_rectC[eC_EDIT_PASS], RGB_BLACK, RGB_WHITE, RGB_BLACK, DT_CENTER, 13, strTemp);

	DrawMDIMenuButtonN(&memDC, m_rectC[eC_BTN_OK], L"OK", m_eSelectControl == eC_BTN_OK);
}


void CPassWordWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if( bShow){
		CRect rc;
		m_pwndParent->GetWindowRect(rc);
		SetWindowPos(&wndTopMost, rc.left + rc.Width() / 2 - 250, rc.top + g_rectArea[eAREA_WORK].Height()/2 + 100, 500, 100, SWP_SHOWWINDOW);
		ComputArea();
	}
	// TODO: Add your message handler code here
}

void CPassWordWnd::ComputArea(){
	GetClientRect(m_rectClient);
	m_rectS[eS_AREA] = m_rectClient;
	m_rectS[eS_AREA].InflateRect(-10, -10,-10, -10);
	CRect rc = m_rectS[eS_AREA];
	double dw = (rc.Width()-5) /4.;
	double dh = (rc.Height()-5)/2.;

	m_rectS[eS_TEXT] = CRect(rc.left + 5 + dw * 0, rc.top + 5 + dh * 0, rc.left + dw * 4, rc.top + dh * 1);
	m_rectS[eS_PASS] = CRect(rc.left + 5 + dw * 0, rc.top + 5 + dh * 1, rc.left + dw * 1, rc.top + dh * 2);
	m_rectC[eC_EDIT_PASS] = CRect(rc.left + 5 + dw * 1, rc.top + 5 + dh * 1, rc.left + dw * 3, rc.top + dh * 2);
	m_rectC[eC_BTN_OK] = CRect(rc.left + 5 + dw * 3, rc.top + 5 + dh * 1, rc.left + dw * 4, rc.top + dh * 2);
		
}

void CPassWordWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	// TODO: Add your message handler code here and/or call default
	for( int i=0 ; i < eC_END; i++){
		if( m_rectC[i].PtInRect(point)){
			m_eSelectControl = i;
			//Invalidate(FALSE);
			InvalidateRect(m_rectC[m_eSelectControl]);
			break;
		}
	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CPassWordWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString strLog;
	// TODO: Add your message handler code here and/or call default
	if( m_rectC[m_eSelectControl].PtInRect(point)){
		switch( m_eSelectControl){
		case eC_BTN_OK:
		{
			OnClickOk();
			strLog = _T("Ok");
			break;
		}
		case eC_EDIT_PASS:
		{
			EditStart(this, m_rectC[eC_EDIT_PASS], m_strInputPassword, TRUE, FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, 13);
			break;
		}
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(_T("Password"), _T("Password"), _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	InvalidateRect(m_rectC[m_eSelectControl]);
	m_eSelectControl = eC_NONE;
	Invalidate(FALSE);
	
	CFrameWnd::OnLButtonUp(nFlags, point);
}

void CPassWordWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent){
		case INTERVAL_UI:{
			
			break;
		}
	}
	CFrameWnd::OnTimer(nIDEvent);
}

BOOL CPassWordWnd::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			OnClickOk();
		}
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void  CPassWordWnd::OnClickOk()
{
	CString strResult;

	BOOL bCheckCondiPW = FALSE;
	bCheckCondiPW = m_strPassword.CompareNoCase(m_strInputPassword) == 0;
	if (bCheckCondiPW) {
		//-->24.06.12 KGH ADD : MANUAL MODE 전환시 솔 잠금 자동 해제(버튼 SOL 해제 기능은 일단 유지 해놓음)
		if (m_eDriveMode == eDRIVE_MODE_TEACH/* || m_eDriveMode == eDRIVE_MODE_MANUAL*/) {
			//ULD
			//sm.IO->tOut_NEW.fUND_SELECT_KEY_ENABLE = TRUE;
			//CCT
			//sm.IO->tOut_NEW.fCCT_SELECT_KEY_ENABLE = TRUE;
			//SCT
			//sm.IO->tOut_NEW.fSCT_SELECT_KEY_ENABLE = TRUE;
			//LD
			sm.IO->tOut_NEW.fLD_SELECT_KEY_ENABLE = TRUE;
		}
		else {
			sm.IO->tOut_NEW.fLD_SELECT_KEY_ENABLE = FALSE;

		}
		

		sm.Option->tSequence.eDriveMode = m_eDriveMode;
		//AllServoOnOff(FALSE);

		ShowWindow(SW_HIDE);

		if (sm.IO->tOut_NEW.fLD_SELECT_KEY_ENABLE)
			IPMessage(g_st[IDS_PASSWORD_WND_KEY_SWITCH_UNLOCK]);
	}
	else {
		IPMessage(g_st[IDS_PASSWORD_WND_PASSWORD_MISMATCH]);
		ShowWindow(SW_HIDE);
	}
	m_strInputPassword = _T("");
}

void CPassWordWnd::SetDriveMode(eDRIVE_MODE eDriveMode) {
	m_eDriveMode = eDriveMode;
}

LRESULT CPassWordWnd::OnEditFinished(WPARAM wParam, LPARAM lParam) {

	CString str = GetEditString();
	m_strInputPassword = str;

	return 0L;
}

BOOL CPassWordWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
}


void CPassWordWnd::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ShowWindow(SW_HIDE);
}

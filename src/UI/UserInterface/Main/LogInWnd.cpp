#include "pch.h"
#include "UI.h"
#include "LogInWnd.h"
#include "MainFrame.h"



IMPLEMENT_DYNAMIC		(CLogInWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CLogInWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_LBUTTONDOWN	()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONUP		()
	ON_MESSAGE			(WM_APP_ED_EDIT_TAB,		OnEditTab)
	ON_WM_CLOSE()
END_MESSAGE_MAP			()



CLogInWnd::CLogInWnd(CWnd* pwndParent) {
	 m_pwndParent = pwndParent;
	 m_eMouseOver = m_eSelectControl = eC_NONE;
	 m_iSelectEdit = eC_NONE;
}
CLogInWnd::~CLogInWnd() {
}

BOOL CLogInWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}



int CLogInWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {
		m_rectShowWindowPos = g_rectArea[eS_FULL_AREA];
	    m_imgLogo = new Gdiplus::Image(_T("../img/SamsungLogo.png"));
	}
	catch (...) {
		return -1;
	}

	return 0;
}



void CLogInWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowText(_T("Log - In"));
		CRect& rc = m_rectShowWindowPos;
		//SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
		SetWindowPos(&wndTop, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);

		fn_ComputeArea();
		m_eMouseOver		= eC_NONE;
		m_eSelectControl	= eC_NONE;

		m_strID = _T("OP");
	
        Invalidate(TRUE);
    }
}



BOOL CLogInWnd::OnEraseBkgnd(CDC* pDC) {
//	return CFrameWndEx::OnEraseBkgnd(pDC);
	return FALSE;	
}



void CLogInWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CDrawSymbol::DrawLineRect(&memDC, m_rectStatic[eS_FULL_AREA], RgbColor::AliceBlue);

    DrawImage(memDC, m_rectStatic[eS_IMAGE_AREA], m_imgLogo);

	DrawLineRect(memDC, m_rectStatic[eS_LOGIN_AREA], RGB_BLACK);
	DrawMDIMenuButtonN(memDC, m_rectControl[eC_RMS_PASSWORD_MODE], _T("RMS P/W\nMode"), m_eSelectControl == eC_RMS_PASSWORD_MODE, m_eMouseOver == eC_RMS_PASSWORD_MODE);
	DrawText(memDC, m_rectStatic[eS_USER_NAME], _T("ID"), DT_FONTSIZE, RGB_BLACK);
	DrawText(memDC, m_rectStatic[eS_USER_PASSWORD], _T("Password"), DT_FONTSIZE, RGB_BLACK);

	CDrawSymbol::SelectedDrawText(memDC, m_rectControl[eC_EDIT_NAME], RGB_BLACK, RGB_WHITE, RGB_BLACK, DT_CENTER, DT_FONTSIZE, m_strID);
	CString str;
	for (int i = 0; i < m_strPass.GetLength(); i++)
		str += _T("●");
	CDrawSymbol::SelectedDrawText(memDC, m_rectControl[eC_EDIT_PASSWORD], RGB_BLACK, RGB_WHITE, RGB_BLACK, DT_CENTER, DT_FONTSIZE, str);

	DrawMDIMenuButtonN(memDC, m_rectControl[eC_OK], _T("OK"), m_eSelectControl == eC_OK, m_eMouseOver == eC_OK);
}



void CLogInWnd::OnMouseMove(UINT nFlags, CPoint point) {
	int eSelectControl = m_eMouseOver;
	m_eMouseOver = eC_NONE;
	for (int i = 0; i <= eC_END; i++) {
		if (m_eSelectControl != i) {
			if (m_rectControl[i].PtInRect(point)) {
				InvalidateRect(m_rectControl[i], FALSE);
				m_eMouseOver = i;
				break;
			}
		}
	}
	if (eSelectControl != m_eMouseOver && eSelectControl != eC_NONE) {
		m_eMouseOver = eC_NONE;
		InvalidateRect(m_rectControl[eSelectControl], FALSE);
	}
	CFrameWndEx::OnMouseMove(nFlags, point);
}



void CLogInWnd::OnLButtonDown(UINT nFlags, CPoint point)  {
	SetFocus();
	m_eSelectControl = eC_NONE;
	for (int i = 0; i<eC_END; i++) {
		if (m_rectControl[i].PtInRect(point)) {
			m_eSelectControl = i;
			break;
		}
	}
	CFrameWndEx::OnLButtonDown(nFlags, point);
}



void CLogInWnd::OnLButtonUp(UINT nFlags, CPoint point) {
	if (m_eSelectControl > eC_NONE && m_rectControl[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_EDIT_NAME: {
				m_iSelectEdit = m_eSelectControl;
				EditStart(this, m_rectControl[m_eSelectControl], m_strID, FALSE, FALSE,DT_CENTER | ES_AUTOHSCROLL, 9);
				break;
			}
			case eC_EDIT_PASSWORD: {
				m_iSelectEdit = m_eSelectControl;
				EditStart(this, m_rectControl[m_eSelectControl], m_strPass, FALSE,FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, 9);
				break;
			}
			case eC_RMS_PASSWORD_MODE: {
				break;
			}
			case eC_OK: {
                OnOK();
				break;
			}
			default :
				break;
		}
	}

	m_eSelectControl	= eC_NONE;
	Invalidate(FALSE);
	CFrameWndEx::OnLButtonUp(nFlags, point);
}



void CLogInWnd::fn_ComputeArea() {
    GetClientRect(m_rectStatic[eS_FULL_AREA]);
	
	CRect rect = m_rectStatic[eS_FULL_AREA];
	double dW = (rect.Width() - 5.) / 10.;
	double dH = (rect.Height() - 5.) / 15.;

    m_rectStatic[eS_IMAGE_AREA] = CRect(rect.left + dW * 1.5 + 5,   rect.top + dH * 1 + 5,      rect.left + dW * 8.5,   rect.top + dH * 12);
    m_rectStatic[eS_LOGIN_AREA] = CRect(rect.left + dW * 4 + 5,     rect.top + dH * 12.5 + 5,   rect.left + dW * 6,     rect.top + dH * 14.5);
    
    rect = m_rectStatic[eS_LOGIN_AREA];
    rect.DeflateRect(5, 20, 5, 20);
	dW = (rect.Width() - 5.) / 5.;
	dH = (rect.Height() - 5.) / 2.;

	m_rectStatic[eS_USER_NAME]      = CRect(rect.left + dW * 0 + 5, rect.top + dH * 0 + 5, rect.left + dW * 1.5, rect.top + dH * 1);
    m_rectStatic[eS_USER_PASSWORD]  = CRect(rect.left + dW * 0 + 5, rect.top + dH * 1 + 5, rect.left + dW * 1.5, rect.top + dH * 2);
    m_rectControl[eC_EDIT_NAME]     = CRect(rect.left + dW * 1.5 + 5, rect.top + dH * 0 + 5, rect.left + dW * 4, rect.top + dH * 1);
    m_rectControl[eC_EDIT_PASSWORD] = CRect(rect.left + dW * 1.5 + 5, rect.top + dH * 1 + 5, rect.left + dW * 4, rect.top + dH * 2);
    m_rectControl[eC_OK]            = CRect(rect.left + dW * 4 + 5, rect.top + dH * 0 + 5, rect.left + dW * 5, rect.top + dH * 2);

	m_rectStatic[eS_LOGIN_AREA].InflateRect(85, 0, 0, 0);
	rect = m_rectStatic[eS_LOGIN_AREA];
	rect.DeflateRect(5, 20, 5, 20);
	dW = (rect.Width() - 5.) / 5.;
	dH = (rect.Height() - 5.) / 2.;

	m_rectControl[eC_RMS_PASSWORD_MODE] = CRect(rect.left + dW * 0 + 5, rect.top + dH * 0 + 5, rect.left + dW * 0 + 80, rect.top + dH * 2);
}



LRESULT CLogInWnd::OnEditTab(WPARAM wParam, LPARAM lParam) {
	m_iSelectEdit += wParam;
	if (m_iSelectEdit < eC_EDIT_NAME)
		m_iSelectEdit = eC_EDIT_PASSWORD;
	else if (m_iSelectEdit > eC_EDIT_PASSWORD)
		m_iSelectEdit = eC_EDIT_NAME;

	switch (m_iSelectEdit) {
		case eC_EDIT_NAME : {
			EditStart(this, m_rectControl[m_iSelectEdit], m_strID, FALSE,FALSE, DT_CENTER | ES_AUTOHSCROLL, 9);
			break;
		}
		case eC_EDIT_PASSWORD : {
			EditStart(this, m_rectControl[m_iSelectEdit], m_strPass, FALSE,FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, 9);
			break;
		}
	}
	
	return FALSE;
}

void CLogInWnd::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ShowWindow(SW_HIDE);
}

BOOL CLogInWnd::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
            OnOK();
		}
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CLogInWnd::OnOK() {
	if (theApp.m_UserManager.UserLogin(m_strID, m_strPass)) {
		m_strPass.Empty();
		((CMainFrame*)AfxGetMainWnd())->SetView(eMAIN_MENU_1, eSUB_MENU_1, theApp.m_UserManager.GetCurrentUserData().uGrade);
		// 21.02.18 죽는 현상 존재 원인 확인 필요
		CIPVar var;
		var.SetChildItem("Show", FALSE);
		__POST_IP_COMMAND__("PLCLinker", "AxisControl", "Show", var);
		ShowWindow(SW_HIDE);
		((CMainFrame*)m_pwndParent)->LogIn();
	}
	else {
		MessageBox(g_st[IDS_LOG_IN_WND_LOG_IN_FAIL]);
	}

    WriteButtonLog(_T("Log In"), _T("Log In"), _T(""), _T("Ok"), theApp.m_UserManager.GetCurrentUserData().szID);
}
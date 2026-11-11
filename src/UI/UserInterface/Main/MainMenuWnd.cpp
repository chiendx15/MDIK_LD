// MainMenuWnd.cpp: 구현 파일
//
#include "pch.h"
#include "../../UI.h"
#include "MainMenuWnd.h"
#include "../../MainFrame.h"



// CMainMenuWnd

IMPLEMENT_DYNCREATE		(CMainMenuWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CMainMenuWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_KILLFOCUS		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_LBUTTONDOWN	()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()



CMainMenuWnd::CMainMenuWnd(CWnd* pwndParent) {
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_pwndParent		= pwndParent;
	m_eMouseOver		= eMAIN_MENU_NONE;
	m_eSelectControl	= eMAIN_MENU_NONE;
}

CMainMenuWnd::~CMainMenuWnd() {
	if (m_pwndParent)
		m_pwndParent = nullptr;
}


BOOL CMainMenuWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CMainMenuWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

try {
	//__USER_GRADE__ = eUSER_NONE;
	ShowWindow(SW_HIDE);
}
catch (...) {
	return -1;
}

	return 0;
}
void CMainMenuWnd::OnKillFocus(CWnd* pNewWnd) {
	CFrameWndEx::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
	Invalidate();
}


void CMainMenuWnd::OnTimer(UINT_PTR nIDEvent) {
	//switch (nIDEvent) {
	//	case INTERVAL_MAIN_MENU: {
	//		KillTimer(INTERVAL_MAIN_MENU);
	//		//Invalidate(FALSE);
	//		SetTimer(INTERVAL_MAIN_MENU, 100, NULL);
	//		return;
	//	}
	//}
	CFrameWndEx::OnTimer(nIDEvent);
}

void CMainMenuWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWnd::OnShowWindow(bShow, nStatus);
	if (bShow) {		
		CRect  rect = g_rectArea[eAREA_MAIN_MENU];
 		SetWindowPos(&wndTop, rect.left	, rect.top	, rect.Width()	, rect.Height(),SWP_SHOWWINDOW);
		m_eSelectControl = eMAIN_MENU_NONE;
		fn_ComputeArea();
		Invalidate(TRUE);

		SetTimer(INTERVAL_MAIN_MENU, 100, NULL);
	}
	else {
		KillTimer(INTERVAL_MAIN_MENU);
	}
}



BOOL CMainMenuWnd::OnEraseBkgnd(CDC* pDC) {
	return FALSE;	
}



void CMainMenuWnd::OnPaint() {
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	CMemDCEx::CMemDC		memDC(&dc);
	memDC.FillSolidRect(m_rectS[eS_FULL_AREA], RGB_STILL_BLUE);
	DrawLineRect(memDC, m_rectS[eS_FULL_AREA], RGB_WHITE);

	for (int i = 0; i < eMAIN_MENU_END; i++) 
	{
		CString str(g_st[IDS_MAIN_MENU_0+i]);
		if (!str.IsEmpty()) 
		{
			DrawMDIMenuButton(memDC, m_rectC[eMAIN_MENU_1 + i], str, m_eSelectControl == eMAIN_MENU_1 + i || g_eSelectedMainMenu == eMAIN_MENU_1 + i, m_eMouseOver == eMAIN_MENU_1 + i, 15);
		}
	}
}


void CMainMenuWnd::OnMouseMove(UINT nFlags, CPoint point) {
	int eMouseOver = m_eMouseOver;
	eMouseOver = eMAIN_MENU_NONE;
	if ((m_eSelectControl != eMAIN_MENU_NONE) && (!m_rectC[m_eSelectControl].PtInRect(point))) {
		int eSelectControl = m_eSelectControl;
		m_eSelectControl = eMAIN_MENU_NONE;
		InvalidateRect(m_rectC[eSelectControl], FALSE);
	}

	for (int i = 0; i < eMAIN_MENU_END; i++) {
		if (eMouseOver != i) {
			if (m_rectC[i].PtInRect(point)) {
				InvalidateRect(m_rectC[i], FALSE);
				eMouseOver = i;
				
				break;
			}
		}
	}
	if (eMouseOver != m_eMouseOver) {
		
		if (m_eMouseOver != eMAIN_MENU_NONE) {
			CRect rect(m_rectC[m_eMouseOver]);
			rect.InflateRect(1, 1, 1, 1);
			InvalidateRect(rect, FALSE);
		}
		m_eMouseOver = eMouseOver;
	}
	CFrameWndEx::OnMouseMove(nFlags, point);
}



void CMainMenuWnd::OnLButtonDown(UINT nFlags, CPoint point) {
	SetFocus();
	for (int i = 0; i < eMAIN_MENU_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			CRect	rect = m_rectC[i];
			rect.InflateRect(2, 2);
			m_eSelectControl = i;
			InvalidateRect(rect);
			break;
		}
	}

	//sm.Sequence->ModuleStatus[eGLASS_LOADER].ToUpperHS.fReceive_Able = TRUE;
	CFrameWndEx::OnLButtonDown(nFlags, point);
}




void CMainMenuWnd::OnLButtonUp(UINT nFlags, CPoint point) {
	if (g_eSelectedMainMenu != m_eSelectControl) {
		if (m_eSelectControl > eMAIN_MENU_NONE && m_rectC[m_eSelectControl].PtInRect(point)) {
			CRect	rect;
			CString str(g_st[IDS_MAIN_MENU_0 + m_eSelectControl]);
			if (!str.IsEmpty()) {
                //<< KES User 권한 Interlock 추가 2024.2.15
                if (__USER_GRADE__ == eUSER_OPERATOR) {
                    // 수동모드, 레시피 편집, 레시피 선택, 파라미터, 유저 관리 접근 제한
                    if (m_eSelectControl == eMM_MANUAL 
                        || m_eSelectControl == eMM_RECIPE_EDIT
                        || m_eSelectControl == eMM_RECIPE_SELECT
                        || m_eSelectControl == eMM_PARAMETER
                        || m_eSelectControl == eMM_USER) {
                        IPMessage(g_st[IDS_MAIN_MENU_WND_NO_ACCESSIBLE]);
                        m_eSelectControl = eMAIN_MENU_NONE;
                        return;
                    }
                }
                if (__USER_GRADE__ == eUSER_EQUIPMENT) {
                    // 레시피 편집, 레시피 선택, 유저 관리 접근 제한
                    if (m_eSelectControl == eMM_RECIPE_EDIT 
                        || m_eSelectControl == eMM_RECIPE_SELECT
                        || m_eSelectControl == eMM_USER) {
                        IPMessage(g_st[IDS_MAIN_MENU_WND_NO_ACCESSIBLE]);
                        m_eSelectControl = eMAIN_MENU_NONE;
                        return;
                    }
                }
                //>>
				
                // KES 주석 2024.2.15
                //if (m_eSelectControl == eMM_LOGIN) {
				//	//if ((g_sm.SystemParameter->eUserGrade > eUSER_NONE) && (g_sm.SystemParameter->eUserGrade <= eUSER_SUPER)) {
				//		//if (MessageBox(g_st[IDS_MAIN_EXIT], theApp.m_UserManager.GetCurrentUserData().szID, MB_YESNO) == IDYES) {
				//			g_eSelectedSubMenu = eSUB_MENU_1;
				//			rect = m_rectC[m_eSelectControl];
				//			rect.InflateRect(2, 2);
				//			InvalidateRect(rect);
				//			//
				//		//	theApp.m_UserManager.UserLogOut();
				//		//	__USER_GRADE__ = eUSER_NONE;
				//			CMainFrame* pParent = (CMainFrame*)AfxGetMainWnd();
				//			pParent->SetView(m_eSelectControl, g_eSelectedSubMenu);//, sm.SystemParameter->eUserGrade);
				//		//}
				//	//}
				//}
	
                // KES 주석 2024.2.15
				//if ((m_eSelectControl != eMM_LOGIN) || (__USER_GRADE__ == eUSER_NONE)) {
					g_eSelectedSubMenu = eSUB_MENU_1;
	
					rect = m_rectC[m_eSelectControl];
					rect.InflateRect(2, 2);
					InvalidateRect(rect);
	
					CMainFrame* pParent = (CMainFrame*)AfxGetMainWnd();
					pParent->SetView(m_eSelectControl, g_eSelectedSubMenu);// , sm.SystemParameter->eUserGrade);
				//}
			}
		}
	}
	m_eSelectControl = eMAIN_MENU_NONE;
	//
	Invalidate(FALSE);

	CFrameWndEx::OnLButtonUp(nFlags, point);
}


void CMainMenuWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);

	m_rectS[eS_ITEM_AREA] = m_rectS[eS_FULL_AREA];
	m_rectS[eS_ITEM_AREA].DeflateRect(-5, -5, -5, -5);


	//double dWidth = (m_rectS[eS_ITEM_AREA].Width() - 15.) / eMAIN_MENU_END;
	double	dWidth = 150;
	int		nSpaces[eMAIN_MENU_END] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
	int		nSpace = m_rectS[eS_ITEM_AREA].left + 10;
	for (int i = 0; i < eMAIN_MENU_END; i++) {
		m_rectC[eMAIN_MENU_1 + i] = CRect(
			nSpace
			, m_rectS[eS_ITEM_AREA].top + 8
			, nSpace + dWidth
			, m_rectS[eS_ITEM_AREA].bottom - 10);
		nSpace += (int)(dWidth + nSpaces[i]);
	}
}

BOOL CMainMenuWnd::Initialize() {
	//	if (m_bInitialize)
	//		return TRUE;
	//	
	//	m_bInitialize		= TRUE;

	///	return m_bInitialize;
	return TRUE;
}
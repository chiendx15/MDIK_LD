// MainMenuWnd.cpp: 구현 파일
//
#include "pch.h"
#include "UI.h"
#include "SubMenuWnd.h"
#include "MainMenuWnd.h"
#include "MainFrame.h"


IMPLEMENT_DYNCREATE		(CSubMenuWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CSubMenuWnd, CFrameWnd)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
//	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()



CSubMenuWnd::CSubMenuWnd(CWnd* pParent) {
	m_pwndParent		= pParent;
	m_eSelectControl	= eSUB_MENU_NONE;
	m_eMouseOver		= eSUB_MENU_NONE;
}
CSubMenuWnd::~CSubMenuWnd() {
}


BOOL CSubMenuWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CSubMenuWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
try {
}
catch (...) {
	return -1;
}
	return 0;
}



void CSubMenuWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndBottom, g_rectArea[eAREA_SUB_MENU].left, g_rectArea[eAREA_SUB_MENU].top, g_rectArea[eAREA_SUB_MENU].Width(), g_rectArea[eAREA_SUB_MENU].Height(), SWP_SHOWWINDOW);
		SetSubMenu();
	}
}



BOOL CSubMenuWnd::OnEraseBkgnd(CDC* pDC) {
//	return CFrameWndEx::OnEraseBkgnd(pDC);
	return FALSE;

	
}


void CSubMenuWnd::OnPaint() 
{
	CPaintDC				dc(this);
	CMemDCEx::CMemDC		memDC(&dc);
	CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_DEFAULT	]);
	CBrush*					pOldBrush	= (CBrush*)memDC.SelectObject(&g_brushForeground[eEC_DEFAULT]);
	CPen*					pOldPen		= (CPen*)memDC.SelectObject(&pen);
	CRect					rect		= m_rectS[eS_FULL_AREA];
	memDC.SelectObject(&g_brushForeground[eEC_DEFAULT]);	
	memDC.FillRect(m_rectS[eS_FULL_AREA], &g_brushBackground[eEC_DISABLE]);
	memDC.FillSolidRect(m_rectS[eS_FULL_AREA], RGB_STILL_BLUE);

	if (m_eStringTableIndex > -1) 
	{
		for (int i = 0; i < eSUB_MENU_END; i++) 
		{
			CString str(g_st[m_eStringTableIndex + i]);
			if (!str.IsEmpty())
			{
				DrawMDIMenuButton(memDC, m_rectC[eSUB_MENU_1 + i], str, g_eSelectedSubMenu == eSUB_MENU_1 + i, m_eMouseOver == eSUB_MENU_1 + i, 13);
			}
		}
	}
}


void CSubMenuWnd::OnMouseMove(UINT nFlags, CPoint point)  {
	int eMouseOver = m_eMouseOver;
	eMouseOver = eSUB_MENU_NONE;
	if ((m_eSelectControl != eSUB_MENU_NONE) && (!m_rectC[m_eSelectControl].PtInRect(point))) {
		int eSelectControl = m_eSelectControl;
		m_eSelectControl = eSUB_MENU_NONE;
		InvalidateRect(m_rectC[eSelectControl], FALSE);
	}

	for (int i = 0; i < eSUB_MENU_END; i++) {
		if (eMouseOver != i) {
			if (m_rectC[i].PtInRect(point)) {
				InvalidateRect(m_rectC[i], FALSE);
				eMouseOver = i;				
				break;
			}
		}
	}
	if (eMouseOver != m_eMouseOver) {		
		if (m_eMouseOver != eSUB_MENU_NONE)
			InvalidateRect(m_rectC[m_eMouseOver], FALSE);
		m_eMouseOver = eMouseOver;
	}
	CFrameWnd::OnMouseMove(nFlags, point);
}


void CSubMenuWnd::OnLButtonDown(UINT nFlags, CPoint point) {
	SetFocus();
	for (int i = 0; i<eSUB_MENU_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			InvalidateRect(m_rectC[i], FALSE);
			break;
		}
	}

	CFrameWndEx::OnLButtonDown(nFlags, point);
}


void CSubMenuWnd::OnLButtonUp(UINT nFlags, CPoint point) {
	if ((m_eSelectControl > eSUB_MENU_NONE) && (m_eSelectControl < eSUB_MENU_END) && (m_rectC[m_eSelectControl].PtInRect(point))) {
		if (!CString(g_st[m_eStringTableIndex+m_eSelectControl]).IsEmpty()) {			
			CMainFrame* pParent = (CMainFrame*)m_pwndParent;
			if (pParent)
				pParent->SetView(g_eSelectedMainMenu, m_eSelectControl);// , sm.SystemParameter->eUserGrade);
		}
	}

	m_eSelectControl  = eSUB_MENU_NONE;

	Invalidate(FALSE);
	
	CFrameWndEx::OnLButtonUp(nFlags, point);
}



void CSubMenuWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
	m_rectS[eS_ITEM_AREA] = m_rectS[eS_FULL_AREA];
	m_rectS[eS_ITEM_AREA].DeflateRect(3,5,5,5);
	
	int iGroup[eMAIN_MENU_END][eSUB_MENU_END]={
		{eGROUP_1,},
		{eGROUP_1 },
		{eGROUP_1,},
		{eGROUP_1,},
		{eGROUP_1,},
		{eGROUP_1,eGROUP_2,eGROUP_2,eGROUP_2,eGROUP_3,eGROUP_3,eGROUP_3,eGROUP_3},
		{eGROUP_1,eGROUP_1,eGROUP_2,eGROUP_2,eGROUP_3,eGROUP_3,eGROUP_3,eGROUP_3,eGROUP_3},
		{eGROUP_1,eGROUP_2,eGROUP_2,eGROUP_2,eGROUP_3},
	};
	for (int i = eSUB_MENU_1; i < eSUB_MENU_END; i++) {
		if ((m_eStringTableIndex < 0) || (CString(g_st[m_eStringTableIndex+i]).IsEmpty())) {
			m_rectC[eSUB_MENU_1+i]	= CRect(-1,-1,-1,-1);
			continue;
		}
		m_rectC[eSUB_MENU_1+i]	= CRect(m_rectS[eS_ITEM_AREA].left + 5, m_rectS[eS_ITEM_AREA].top+5+i*70+iGroup[g_eSelectedMainMenu][i]*BLANK_HEIGHT, m_rectS[eS_ITEM_AREA].right+5, m_rectS[eS_ITEM_AREA].top+(i+1)*70+iGroup[g_eSelectedMainMenu][i]*BLANK_HEIGHT);
	}
}



// 메인메뉴에서 선택된 항목에 따라 서브 메뉴를 설정한다.
void CSubMenuWnd::SetSubMenu() {
	//m_eSelectControl = m_eSelectedControl = g_eSelectedSubMenu	= eSUB_MENU_NONE;
	m_eSelectControl = m_eMouseOver = eSUB_MENU_NONE;

	m_eStringTableIndex = -1;
	if ((g_eSelectedMainMenu > eMAIN_MENU_NONE) && (g_eSelectedMainMenu < eMAIN_MENU_END)) 
		m_eStringTableIndex = IDS_SM_1_1 + ((g_eSelectedMainMenu-eMAIN_MENU_1)*eSUB_MENU_END);

	//if (m_eStringTableIndex != -1) {
		fn_ComputeArea();
	//}
	Invalidate(TRUE);
};

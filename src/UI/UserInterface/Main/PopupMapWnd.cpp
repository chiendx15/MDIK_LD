#include "pch.h"
#include "UI.h"
#include "PopupMapWnd.h"
#include "MainFrame.h"
#include "TitleWnd.h"
#include "Simulator/DemoWnd.h"

BEGIN_MESSAGE_MAP(CPopupMapWnd, CFrameWnd)
	ON_WM_CREATE()
    ON_WM_CLOSE()
    ON_WM_SHOWWINDOW()
    ON_WM_PAINT()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CPopupMapWnd::ComputeArea() {
	GetClientRect(m_rectClient);
		
	CRect rc = m_rectClient;
	double dH = (rc.Height() - 5.) / 1.;
	double dW = (rc.Width() - 5.) / 1.;

    m_rectC[eC_REVERSE]  = CRect(rc.left + dW * 1 - 75, rc.top + dH * 0 + 5,  rc.left + dW * 1, rc.top + dH * 0 + 40);
	m_rectS[eS_MAP_AREA] = CRect(rc.left + dW * 0 + 5, rc.top + dH * 0 + 45, rc.left + dW * 1,      rc.top + dH * 1);
}


void CPopupMapWnd::OnPaint()
{
    CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	memDC.FillSolidRect(m_rectClient, RgbColor::Gainsboro);

    DrawMDIMenuButton(memDC, m_rectC[eC_REVERSE], g_bUIRotateDirection ? g_st[IDS_TITLE_REVERSE] : g_st[IDS_TITLE_FORWARD], g_bUIRotateDirection || m_eSelectControl == eC_REVERSE, m_eMouseOver == eC_REVERSE);
	if (g_bUIRotateDirection && m_imgMain)
	{
		m_imgMain.Draw(memDC.m_hDC, m_rectS[eS_MAP_AREA]);
	}
	else if (!g_bUIRotateDirection && m_imgMainRev)
	{
		m_imgMainRev.Draw(memDC.m_hDC, m_rectS[eS_MAP_AREA]);
	}
}


void CPopupMapWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CFrameWnd::OnShowWindow(bShow, nStatus);

    if (bShow) {
		int iW = 1452;      // Simulator Map 이미지 비율 때문에 Size 고정
		int iH = 460;       // Simulator Map 이미지 비율 때문에 Size 고정
		int iX = (GetSystemMetrics(SM_CXSCREEN) - iW) / 2.;
		int iY = (GetSystemMetrics(SM_CYSCREEN) - iH) / 2.;

		SetWindowPos(&wndTop, iX, iY, iW, iH, SWP_SHOWWINDOW);
		ComputeArea();
		m_eSelectControl = eC_NONE;
        Invalidate(FALSE);
		SetSimulatorView();
	}
}


int CPopupMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
    
    ModifyStyle(0, WS_SYSMENU);
	SetWindowText(_T("Map"));

    HRESULT hResult;
    LPCTSTR szNames = _T("../img/CCT.png");	
	hResult = m_imgMain.Load(szNames);
	if (FAILED(hResult)) {
		CString strTmp = _T("ERROR: Failed to load ");
	}        

    LPCTSTR	szRev = _T("../img/CCT_Rev.png");
	hResult = m_imgMainRev.Load(szRev);
	if (FAILED(hResult)) {
		CString strTmp = _T("ERROR: Failed to load ");
	}

	return 0;
}



void CPopupMapWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_rectC[m_eSelectControl].PtInRect(point)) {
    	switch (m_eSelectControl) {
            case eC_REVERSE: {
                g_bUIRotateDirection = !g_bUIRotateDirection;
                SetSimulatorView();
                break;
            }
        }
    }
    m_eSelectControl = eC_NONE;
	InvalidateRect(m_rectC[eC_REVERSE], FALSE);
    CFrameWnd::OnLButtonUp(nFlags, point);
}


void CPopupMapWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    SetFocus();
	m_eSelectControl = eC_NONE;
	for (int i = 0; i<eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			InvalidateRect(m_rectC[i],FALSE);
			break;
		}
	}
    CFrameWnd::OnLButtonDown(nFlags, point);
}


void CPopupMapWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    int eMouseOver = m_eMouseOver;
	eMouseOver = eC_NONE;

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
    CFrameWnd::OnMouseMove(nFlags, point);
}


void CPopupMapWnd::SetSimulatorView() {
	CMainFrame* pMain =  (CMainFrame*)AfxGetMainWnd();
	CTitleWnd* pTitle = (CTitleWnd*)pMain->getTitleWnd();
	CDemoWnd* pDemo = (CDemoWnd*)pTitle->GetDemoWnd();
	m_pSimulatorView = (CSimulator*)pDemo->getSimulator();

	if (m_pSimulatorView) {
		m_pSimulatorView->SetDirection(g_bUIRotateDirection);
		m_pSimulatorView->SetParent(this);
		m_pSimulatorView->MoveWindow(m_rectS[eS_MAP_AREA]);
		m_pSimulatorView->ShowWindow(SW_SHOW);
	}
}


void CPopupMapWnd::OnClose()
{
    ShowWindow(SW_HIDE);
}

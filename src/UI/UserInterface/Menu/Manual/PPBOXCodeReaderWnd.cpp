// PPBOXCodeReader.cpp: 구현 파일
//

#include "pch.h"
#include "UI.h"
#include "PPBOXCodeReaderWnd.h"

#define TIMER_REDRAW 4406
// CPPBOXCodeReaderWnd
IMPLEMENT_DYNCREATE(CPPBOXCodeReaderWnd, CFrameWndEx)

CPPBOXCodeReaderWnd::CPPBOXCodeReaderWnd(CWnd* pParent)
{
	m_eSelectControl = eC_NONE;
	m_bLinkCheck = TRUE;
}

CPPBOXCodeReaderWnd::~CPPBOXCodeReaderWnd()
{
}


BEGIN_MESSAGE_MAP(CPPBOXCodeReaderWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CPPBOXCodeReaderWnd 메시지 처리기

void CPPBOXCodeReaderWnd::fn_ComputeArea() {
	GetClientRect(m_rectClient);
	CRect rc = m_rectClient;
	double dW = (rc.Width() - 5) / 4.;
	double dH = (rc.Height() - 5) / 11.;

	m_rectS[eS_PPBOX_CODE_AREA]		= CRect(rc.left + 15 + dW * 0, rc.top + 15 + dH * 3, rc.left + dW * 1,	rc.top + dH * 4.2);

	rc = m_rectS[eS_PPBOX_CODE_AREA];
	dW = (rc.Width() - 5) / 2.;
	dH = (rc.Height() - 10) / 1.;

	m_rectC[eC_PPBOX_CODE_VIEW]		= CRect(rc.left + 15 + dW * 0, rc.top + 15 + dH * 0, rc.left + dW * 1,	rc.top + dH * 1);
	m_rectC[eC_PPBOX_CODE_READ]		= CRect(rc.left + 15 + dW * 1, rc.top + 15 + dH * 0, rc.left + dW * 2,	rc.top + dH * 1);
		
}
int CPPBOXCodeReaderWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void CPPBOXCodeReaderWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_bLinkCheck = TRUE;
		SetTimer(TIMER_REDRAW, 100, NULL);
	}
	else {
		KillTimer(TIMER_REDRAW);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CPPBOXCodeReaderWnd::OnPaint()
{
	CPaintDC			dc(this);
	CRect				rect(m_rectClient);
	CMemDCEx::CMemDC	memDC(&dc);
	memDC.FillRect(m_rectClient,&g_brushBackground[eEC_DEFAULT]);
	
	DrawGroupBox(&memDC, m_rectS[eS_PPBOX_CODE_AREA], _T("PPBOX Code Read"), RGB_BLACK, RGB_BLACK);
	//SelectedDrawText(&memDC, m_rectC[eC_PPBOX_CODE_VIEW], CString(sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Glass.szCstID));
	SelectedDrawText(&memDC, m_rectC[eC_PPBOX_CODE_VIEW], CString(sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Glass.szCELL_ID_BCR));
	DrawMDIMenuButtonN(&memDC, m_rectC[eC_PPBOX_CODE_READ], _T("Read"), m_eSelectControl == eC_PPBOX_CODE_READ);
}


void CPPBOXCodeReaderWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case TIMER_REDRAW:{
			Invalidate(FALSE);
			break;
		}
	}

	CFrameWndEx::OnTimer(nIDEvent);
}


void CPPBOXCodeReaderWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	for (int i = 0; i < eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			Invalidate(FALSE);
			break;
		}
	}
	CFrameWndEx::OnLButtonDown(nFlags, point);
}


void CPPBOXCodeReaderWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_PPBOX_CODE_READ : {
				Serial::PPBoxCodeReader::SetPPBoxIDReading(eMODULE_PC1_LOAD_CONVEYOR_2);
				break;		
			}				
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}

	m_eSelectControl = eC_NONE;
	Invalidate(FALSE);
	CFrameWndEx::OnLButtonUp(nFlags, point);
}

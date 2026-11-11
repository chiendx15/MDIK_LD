// ModuleStatusWnd.cpp : implementation file
//

#include "pch.h"
#include "UI.h"
#include "ModuleStatusWnd.h"
#include "ScrapeWnd.h"

// CModuleStatusWnd

IMPLEMENT_DYNAMIC(CModuleStatusWnd, CWnd)
CModuleStatusWnd::CModuleStatusWnd(CString strModule, eMODULEs eModule) 
{
	m_eModule = eModule;
	m_strModule = strModule;
}
CModuleStatusWnd::~CModuleStatusWnd() {

}
BEGIN_MESSAGE_MAP(CModuleStatusWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CModuleStatusWnd message handlers

void CModuleStatusWnd::ComputArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
	CRect rc = m_rectS[eS_FULL_AREA];
	
	double dW = (rc.Width() - 5) / 2;
	double dH = (rc.Height() - 5) / 3;

	m_rectS[eS_TITLE_AREA	] = CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*2, rc.top+dH*1);
	m_rectC[eC_STATUS		] = CRect(rc.left+5+dW*0, rc.top+5+dH*1, rc.left+dW*2, rc.top+dH*2);
	m_rectC[eC_LD_ULD		] = CRect(rc.left+5+dW*0, rc.top+5+dH*2, rc.left+dW*1, rc.top+dH*3);
	m_rectC[eC_TACK_TIME	] = CRect(rc.left+5+dW*1, rc.top+5+dH*2, rc.left+dW*2, rc.top+dH*3);
}



int CModuleStatusWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pwndScrape = new CScrapeWnd();
	if (!m_pwndScrape->Create(NULL, NULL, NULL)) return -1;
	m_pwndScrape->SetModule(m_eModule, m_strModule);
	// TODO:  Add your specialized creation code here
	return 0;
}

void CModuleStatusWnd::OnPaint()
{
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CDrawSymbol::DrawLineRect(&memDC, m_rectS[eS_FULL_AREA], g_colorForeground[eEC_DEFAULT]);

	SelectedDrawText(&memDC, m_rectS[eS_TITLE_AREA], m_strModule, 9, RGB_WHITE, RGB_BLACK, RGB_WHITE, DT_CENTER | DT_VCENTER);

	COLORREF colLD_ULD	= RGB_WHITE;
	if (sm.Module[m_eModule].ToUpper.fReceive_Able && !sm.Module[m_eModule].ToUpper.fReceive_Start && !sm.Module[m_eModule].ToUpper.fReceive_Complete) {
		colLD_ULD = RGB_YELLOW;
	}
	else if (sm.Module[m_eModule].ToUpper.fReceive_Able && sm.Module[m_eModule].ToUpper.fReceive_Start && !sm.Module[m_eModule].ToUpper.fReceive_Complete) {
		colLD_ULD = RGB_BLUE_LIGHT;
	}
	else if (sm.Module[m_eModule].ToUpper.fReceive_Able && sm.Module[m_eModule].ToUpper.fReceive_Start && sm.Module[m_eModule].ToUpper.fReceive_Complete) {
		colLD_ULD = RGB_GREEN;
	}
	if (sm.Module[m_eModule].ToLower.fSend_Able &&  !sm.Module[m_eModule].ToLower.fSend_Start && !sm.Module[m_eModule].ToLower.fSend_Complete) {
		colLD_ULD = RGB_YELLOW;
	}
	else if (sm.Module[m_eModule].ToLower.fSend_Able && sm.Module[m_eModule].ToLower.fSend_Start && !sm.Module[m_eModule].ToLower.fSend_Complete) {
		colLD_ULD = RGB_BLUE_LIGHT;
	}
	else if (sm.Module[m_eModule].ToLower.fSend_Able && sm.Module[m_eModule].ToLower.fSend_Start && sm.Module[m_eModule].ToLower.fSend_Complete) {
		colLD_ULD = RGB_GREEN;
	}
	
	// Module Status 를 보는 경우 
	CString strStatus;
	CString stepText = EnumToString(m_eModule, sm.Module[m_eModule].Status.wSeqStep);
	stepText = stepText.Mid(stepText.Find('_', 0) + 1);
	strStatus = Format(_T("[%d] %s"), sm.Module[m_eModule].Status.wSeqStep, stepText);
		
	BOOL bMaterial = sm.Module[m_eModule].Glass.bMaterial;

	//if (bMaterial)
	//	SelectedDrawText(&memDC, m_rectC[eC_STATUS], strStatus, 9, RGB_WHITE, RGB_GREEN, RGB_BLACK, DT_LEFT | DT_VCENTER);
	//else 
	//	SelectedDrawText(&memDC, m_rectC[eC_STATUS], strStatus, 9, RGB_WHITE, RGB_WHITE, RGB_BLACK, DT_LEFT | DT_VCENTER);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_STATUS], strStatus, bMaterial || m_eSelectControl == eC_STATUS, FALSE, DT_FONTSIZE, RGB_GRAY_LIGHT, RGB_GREEN);

	if (sm.Module[m_eModule].Status.bInputStop)
	{
		SelectedDrawText(&memDC, m_rectC[eC_LD_ULD], g_st[IDS_MAIN_STATUS_MODULE_STATUS_INPUT_STOP], 9, RGB_BLACK, RGB_RED, RGB_BLACK, DT_CENTER | DT_VCENTER);
	}
	else
	{
		// 2021.03.19 트랜스퍼에 자재 타입 표시
		if (m_eModule == eMODULE_PC1_GLASS_TRANSFER)
		{
			CString strMaterial;
			switch (sm.Module[m_eModule].Status.wMaterialType)
			{
				case eMT_PAPER:			colLD_ULD = RGB_YELLOW;	strMaterial = g_st[IDS_MDULE_STATUS_WND_PAPER]; break;
				case eMT_ORG_GLASS:		colLD_ULD = RGB_GREEN;	strMaterial = g_st[IDS_MDULE_STATUS_WND_GLASS]; break;
				case eMT_NONE:			colLD_ULD = RGB_CYAN;	strMaterial = _T("NONE"); break;
				default:				colLD_ULD = RGB_CYAN;	strMaterial = _T("..."); break;
			}
			SelectedDrawText(&memDC, m_rectC[eC_LD_ULD], strMaterial, 9, RGB_BLACK, colLD_ULD, RGB_BLACK, DT_CENTER | DT_VCENTER);
		}
		else
		{
			SelectedDrawText(&memDC, m_rectC[eC_LD_ULD], g_st[IDS_MAIN_STATUS_MODULE_STATUS_LOAD_UNLOAD], 9, RGB_BLACK, colLD_ULD, RGB_BLACK, DT_CENTER | DT_VCENTER);
		}
	}
	SelectedDrawText(&memDC, m_rectC[eC_TACK_TIME	], ToCString(sm.Module[m_eModule].Status.runningTime * 0.001),	9, RGB_BLACK, RGB_WHITE, RGB_BLACK, DT_CENTER | DT_VCENTER);
}

void CModuleStatusWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch( nIDEvent){
		case INTERVAL_UI:{
			Invalidate(FALSE);
			break;
		}
	}
	CWnd::OnTimer(nIDEvent);
}


void CModuleStatusWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	if( bShow){
		ComputArea();
		m_eSelectControl = eC_NONE;
		SetTimer(INTERVAL_UI, 100, NULL);
	}else{
		KillTimer(INTERVAL_UI);
	}
}


void CModuleStatusWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_eSelectControl = eC_NONE;
	for (int i = 0; i<eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			InvalidateRect(m_rectC[i],FALSE);
			break;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void CModuleStatusWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_STATUS: {
				m_pwndScrape->ShowWindow(SW_SHOW);
				strLog = Format(_T("%s"), m_strModule);
				break;
			}
			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(_T("Main Status"), _T("Module Status"), _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl	=	eC_NONE;
	Invalidate(FALSE);
	CWnd::OnLButtonUp(nFlags, point);
}


void CModuleStatusWnd::CloseScrapWnd()
{
    if (m_pwndScrape != nullptr) {
        m_pwndScrape->ShowWindow(SW_HIDE);
    }
}
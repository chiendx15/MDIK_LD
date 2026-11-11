// PPBOXCodeReader.cpp: 구현 파일
//

#include "pch.h"
#include "UI.h"
#include "CIMTestWnd.h"

#define TIMER_REDRAW 4406
// CCIMTestWnd
IMPLEMENT_DYNCREATE(CCIMTestWnd, CFrameWndEx)

CCIMTestWnd::CCIMTestWnd(CWnd* pParent)
{
	m_eSelectControl = eC_NONE;
	m_bLinkCheck = TRUE;
}

CCIMTestWnd::~CCIMTestWnd()
{
}


BEGIN_MESSAGE_MAP(CCIMTestWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CCIMTestWnd 메시지 처리기

void CCIMTestWnd::fn_ComputeArea() {
	GetClientRect(m_rectClient);
	CRect rc = m_rectClient;
	double dW = (rc.Width() - 5) / 4.;
	double dH = (rc.Height() - 5) / 11.;

	m_rectS[eS_AREA]		= CRect(rc.left + 15 + dW * 0, rc.top + 15 + dH * 0, rc.left + dW * 4,	rc.top + dH * 11);

	rc = m_rectS[eS_AREA];
	dW = (rc.Width() - 5) / 3.;
	dH = (rc.Height() - 10) / 20.;


	for (int i = eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_350_CASSETTE_LOAD_REQUEST; i < eC_END; i++) {
		m_rectC[i] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * i, rc.left + dW * 1,	rc.top + dH * (i + 1));
	}
		
}
int CCIMTestWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void CCIMTestWnd::OnShowWindow(BOOL bShow, UINT nStatus)
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


void CCIMTestWnd::OnPaint()
{
	CPaintDC			dc(this);
	CRect				rect(m_rectClient);
	CMemDCEx::CMemDC	memDC(&dc);
	memDC.FillRect(m_rectClient,&g_brushBackground[eEC_DEFAULT]);
	
	DrawGroupBox(&memDC, m_rectS[eS_AREA], _T("Test"), RGB_BLACK, RGB_BLACK);

	LPCTSTR lpSF[] = {
		_T("S6F11_CASSETTE_STATUS_CHANGE_CEID_350_CASSETTE_LOAD_REQUEST"),
		_T("S6F11_CASSETTE_STATUS_CHANGE_CEID_351_CASSETTE_LOAD_COMPLETE"),
		_T("S6F11_CARRIER_PROCESS_CHANGE_CEID_262_CARRIER_INFORMATION_REQUEST"),
		_T("S3F115_CARRIER_INFORMATION_DATA_RECV_WAIT"),
		_T("S6F11_S6F11_CARRIER_PROCESS_CHANGE_CEID_257_CARRIER_RELEASE_COMPLETE"),
		_T("eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_352_CASSETTE_UNLOAD_REQUEST"),
		_T("eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_353_CASSETTE_UNLOAD_COMPLETE"),
	};

	for (int i = eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_350_CASSETTE_LOAD_REQUEST; i < eC_END; i++) {
		int iItem = i - eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_350_CASSETTE_LOAD_REQUEST;

		if (iItem > countof(lpSF))
			break;

		DrawMDIMenuButtonN(&memDC, m_rectC[i], lpSF[iItem], m_eSelectControl == i);
	}
}


void CCIMTestWnd::OnTimer(UINT_PTR nIDEvent)
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


void CCIMTestWnd::OnLButtonDown(UINT nFlags, CPoint point)
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


void CCIMTestWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_350_CASSETTE_LOAD_REQUEST: {
				S6F11_CASSETTE_LOAD_REQUEST(eMODULE_PC1_LOAD_CONVEYOR_1, 0);
				//CIM::Sequence::Load::S6F11_CASSETTE_STATUS_CHANGE_CEID_350_CASSETTE_LOAD_REQUEST(eEQP_SCT, 0);
				break;		
			}
			case eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_351_CASSETTE_LOAD_COMPLETE: {
				S6F11_CASSETTE_LOAD_COMPLETE(eMODULE_PC1_LOAD_CONVEYOR_1, TRUE, 0);
				//CIM::Sequence::Load::S6F11_CASSETTE_STATUS_CHANGE_CEID_351_CASSETTE_LOAD_COMPLETE(eEQP_SCT, 0);
				break;
			}
			case eC_S6F11_CARRIER_PROCESS_CHANGE_CEID_262_CARRIER_INFORMATION_REQUEST: {
				S6F11_CARRIER_INFORMATION_REQUEST(eMODULE_PC1_LOAD_CONVEYOR_2, 1, "A3DV1D3A62G8");
				//CIM::Sequence::Load::S6F11_CARRIER_PROCESS_CHANGE_CEID_262_CARRIER_INFORMATION_REQUEST(eEQP_SCT, 0, "A3DV1D3A62G8");
				break;
			}
			case eC_S3F115_CARRIER_INFORMATION_DATA_RECV_WAIT: {
				break;
			}
			case eC_S6F11_S6F11_CARRIER_PROCESS_CHANGE_CEID_257_CARRIER_RELEASE_COMPLETE: {
				S6F11_CARRIER_RELEASE_COMPLETE(eMODULE_PC1_LOAD_CONVEYOR_2, 1, "A3DV1D3A62G8");
				//CIM::Sequence::Load::S6F11_CARRIER_PROCESS_CHANGE_CEID_257_CARRIER_RELEASE_COMPLETE(eEQP_SCT, 0, "A3DV1D3A62G8");
				break;
			}
			case eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_352_CASSETTE_UNLOAD_REQUEST: {
				S6F11_CASSETTE_UNLOAD_REQUEST(eMODULE_PC1_UNLOAD_CONVEYOR_2, 2);
				//CIM::Sequence::Unload::S6F11_CASSETTE_STATUS_CHANGE_CEID_352_CASSETTE_UNLOAD_REQUEST(eEQP_SCT, 0);
				break;
			}
			case eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_353_CASSETTE_UNLOAD_COMPLETE: {
				S6F11_CASSETTE_UNLOAD_COMPLETE(eMODULE_PC1_UNLOAD_CONVEYOR_2, 2);
				//CIM::Sequence::Unload::S6F11_CASSETTE_STATUS_CHANGE_CEID_353_CASSETTE_UNLOAD_COMPLETE(eEQP_SCT, 0);
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

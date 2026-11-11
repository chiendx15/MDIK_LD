#include "pch.h"
#include "UI.h"
#include "MainFrame.h"
#include "TPMCodeSelectWnd.h"

IMPLEMENT_DYNCREATE(CTPMCodeSelectWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP(CTPMCodeSelectWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


CTPMCodeSelectWnd::CTPMCodeSelectWnd(CWnd* pwndParent)
{
}


CTPMCodeSelectWnd::~CTPMCodeSelectWnd() {
}

int CTPMCodeSelectWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
try { 
	
	SetWindowText(_T("TPM CODE"));
	ShowWindow(SW_HIDE);

	//<< 창 위치 중앙 세팅
	m_rectShowWindowPos = g_rectArea[eS_FULL_AREA];
	double dW = m_rectShowWindowPos.Width() / 3.6;
	double dH = m_rectShowWindowPos.Height() / 4;
	m_rectShowWindowPos.DeflateRect(dW, dH, dW, dH);
	//>>

}
catch (CException* exp) {
	exp->ReportError();
	return -1;
}
	return 0;
}

void CTPMCodeSelectWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);

	CRect rc = m_rectS[eS_FULL_AREA];
	double dW = (rc.Width() - 5.) / 2.;
	double dH = (rc.Height() - 5.) / 1.;
	
	m_rectS[eS_CODE_LIST_AREA]	= CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0	  , rc.left + dW * 2, rc.top + dH * 1 - 40);
	m_rectC[eC_SEND]			= CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 1 - 40, rc.left + dW * 1, rc.top + dH * 1);
	m_rectC[eC_CANCEL]			= CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 1 - 40, rc.left + dW * 2, rc.top + dH * 1);

	rc = m_rectS[eS_CODE_LIST_AREA];
	dW = (rc.Width()) / 10.;
	dH = (rc.Height()) / 14.;

	m_rectS[eS_TITLE_EQP_STATE]						= CRect(rc.left+dW*0,		rc.top+dH*0,  rc.left+dW*1.5,	rc.top+dH*1);
	m_rectS[eS_TITLE_TP_INDEX]						= CRect(rc.left+dW*1.5,		rc.top+dH*0,  rc.left+dW*3,	    rc.top+dH*1);
	m_rectS[eS_TITLE_DESCRIPTION]					= CRect(rc.left+dW*3,		rc.top+dH*0,  rc.left+dW*8,	    rc.top+dH*1);
	m_rectS[eS_TITLE_CODE]							= CRect(rc.left+dW*8,		rc.top+dH*0,  rc.left+dW*10,	rc.top+dH*1);
																							  
	m_rectS[eS_EQP_STATE_DOWN]						= CRect(rc.left+dW*0,		rc.top+dH*1,  rc.left+dW*1.5,	rc.top+dH*14);

	m_rectS[eS_TP_INDEX_BM]							= CRect(rc.left+dW*1.5,		rc.top+dH*1,  rc.left+dW*3,		rc.top+dH*2);
	m_rectS[eS_TP_INDEX_PM]							= CRect(rc.left+dW*1.5,		rc.top+dH*2,  rc.left+dW*3,		rc.top+dH*7);
	m_rectS[eS_TP_INDEX_CM]							= CRect(rc.left+dW*1.5,		rc.top+dH*7,  rc.left+dW*3,		rc.top+dH*10);
	m_rectS[eS_TP_INDEX_CHANGE_MODEL]				= CRect(rc.left+dW*1.5,		rc.top+dH*10, rc.left+dW*3,		rc.top+dH*12);
	m_rectS[eS_TP_INDEX_MATERIAL]					= CRect(rc.left+dW*1.5,		rc.top+dH*12, rc.left+dW*3,		rc.top+dH*14);

	m_rectS[eS_DESCRIPTION_BREAKDOWN_MANUAL]		= CRect(rc.left+dW*3,		rc.top+dH*1,  rc.left+dW*8,		rc.top+dH*2);
	m_rectS[eS_DESCRIPTION_REGULAR_PM]				= CRect(rc.left+dW*3,		rc.top+dH*2,  rc.left+dW*8,		rc.top+dH*3);
	m_rectS[eS_DESCRIPTION_REGULAR_PM_TITLE]		= CRect(rc.left+dW*3,		rc.top+dH*3,  rc.left+dW*5.5,	rc.top+dH*7);
	m_rectS[eS_DESCRIPTION_CHECK_EQUIPMENT]			= CRect(rc.left+dW*5.5,		rc.top+dH*3,  rc.left+dW*8,		rc.top+dH*5);
	m_rectS[eS_DESCRIPTION_CHECK_QUALITY]			= CRect(rc.left+dW*5.5,		rc.top+dH*5,  rc.left+dW*8,		rc.top+dH*7);
	m_rectS[eS_DESCRIPTION_IMPROVE_EQUIPMENT_EE]	= CRect(rc.left+dW*3,		rc.top+dH*7,  rc.left+dW*8,		rc.top+dH*8);
	m_rectS[eS_DESCRIPTION_IMPROVE_PROCESSING_EL]	= CRect(rc.left+dW*3,		rc.top+dH*8,  rc.left+dW*8,		rc.top+dH*9);
	m_rectS[eS_DESCRIPTION_SETUP_NEW_PRODUCT]		= CRect(rc.left+dW*3,		rc.top+dH*9,  rc.left+dW*8,		rc.top+dH*10);
	m_rectS[eS_DESCRIPTION_CHANGE_SAME_MODEL]		= CRect(rc.left+dW*3,		rc.top+dH*10, rc.left+dW*8,		rc.top+dH*11);
	m_rectS[eS_DESCRIPTION_CHANGE_DIFFERENT_MODEL]	= CRect(rc.left+dW*3,		rc.top+dH*11, rc.left+dW*8,		rc.top+dH*12);
	m_rectS[eS_DESCRIPTION_CHANGE_MATERIAL]			= CRect(rc.left+dW*3,		rc.top+dH*12, rc.left+dW*8,		rc.top+dH*13);
	m_rectS[eS_DESCRIPTION_DOWN_MATERIAL]			= CRect(rc.left+dW*3,		rc.top+dH*13, rc.left+dW*8,		rc.top+dH*14);

	m_rectC[eC_CODE_BM]								= CRect(rc.left+dW*8,		rc.top+dH*1,  rc.left+dW*10,	rc.top+dH*2);
	m_rectC[eC_PM_REGULAR_PM]						= CRect(rc.left+dW*8,		rc.top+dH*2,  rc.left+dW*10,	rc.top+dH*3);
	m_rectC[eC_PM_IRREGULAR_PM_CHECK_EQUIPMENT]		= CRect(rc.left+dW*8,		rc.top+dH*3,  rc.left+dW*10,	rc.top+dH*5);
	m_rectC[eC_PM_IRREGULAR_PM_CHECK_QUALITY]		= CRect(rc.left+dW*8,		rc.top+dH*5,  rc.left+dW*10,	rc.top+dH*7);
	m_rectC[eC_CM_IMPROVE_EQUIPMENT_EE]				= CRect(rc.left+dW*8,		rc.top+dH*7,  rc.left+dW*10,	rc.top+dH*8);
	m_rectC[eC_CM_IMPROVE_PROCESSING_EL]			= CRect(rc.left+dW*8,		rc.top+dH*8,  rc.left+dW*10,	rc.top+dH*9);
	m_rectC[eC_CM_SETUP_NEW_PRODUCT]				= CRect(rc.left+dW*8,		rc.top+dH*9,  rc.left+dW*10,	rc.top+dH*10);
	m_rectC[eC_CHANGE_MODEL_CHANGE_SAME_MODEL]		= CRect(rc.left+dW*8,		rc.top+dH*10, rc.left+dW*10,	rc.top+dH*11);
	m_rectC[eC_CHANGE_MODEL_CHANGE_DIFFERENT_MODEL]	= CRect(rc.left+dW*8,		rc.top+dH*11, rc.left+dW*10,	rc.top+dH*12);
	m_rectC[eC_MATERIAL_CHANGE_MATERIAL]			= CRect(rc.left+dW*8,		rc.top+dH*12, rc.left+dW*10,	rc.top+dH*13);
	m_rectC[eC_MATERIAL_DOWN_MATERIAL]				= CRect(rc.left+dW*8,		rc.top+dH*13, rc.left+dW*10,	rc.top+dH*14);
}

void CTPMCodeSelectWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		
		CRect& rc = m_rectShowWindowPos;
		SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);

		m_iSelectMode = eC_PM_IRREGULAR_PM_CHECK_EQUIPMENT;

		fn_ComputeArea();
		m_eSelectedControl	= eC_NONE;
	}
}

void CTPMCodeSelectWnd::OnPaint() {
	CPaintDC				dc(this);
	CMemDCEx::CMemDC		memDC(&dc);
	memDC.FillSolidRect(m_rectS[eS_FULL_AREA], RgbColor::AliceBlue);

	DrawLineRect(memDC, m_rectS[eS_CODE_LIST_AREA], RGB_BLACK);
	
	DrawMDIMenuButtonN(memDC, m_rectC[eC_SEND], _T("전송"), m_eSelectedControl == eC_SEND, m_eMouseOver == eC_SEND);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_CANCEL], _T("Cancel"), m_eSelectedControl == eC_CANCEL, m_eMouseOver == eC_CANCEL);

	SelectedDrawText(memDC, m_rectS[eS_TITLE_EQP_STATE],		_T(" EQP STATE"), 9, RGB_BLACK, RGB_SKY, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_TITLE_TP_INDEX],			_T(" TP Index"), 9, RGB_BLACK, RGB_SKY, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_TITLE_DESCRIPTION],		_T(" Description"), 9, RGB_BLACK, RGB_SKY, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_TITLE_CODE],				_T(" Code"), 9, RGB_BLACK, RGB_SKY, RGB_BLACK, DT_LEFT | DT_VCENTER);

	SelectedDrawText(memDC, m_rectS[eS_EQP_STATE_DOWN],			_T(" DOWN"), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_TP_INDEX_BM],			_T(" BM"), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_TP_INDEX_PM],			_T(" PM"), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_TP_INDEX_CM],			_T(" CM"), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_TP_INDEX_CHANGE_MODEL],	_T(" CHANGE_MODEL"), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_TP_INDEX_MATERIAL],		_T(" MATERIAL"), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);

	SelectedDrawText(memDC, m_rectS[eS_DESCRIPTION_BREAKDOWN_MANUAL],		_T(" ") + CString(Description[eC_CODE_BM]), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_DESCRIPTION_REGULAR_PM],				_T(" ") + CString(Description[eC_PM_REGULAR_PM]), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_DESCRIPTION_REGULAR_PM_TITLE],		_T(" ") + CString(Description[eC_PM_REGULAR_PM]), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_DESCRIPTION_CHECK_EQUIPMENT],		_T(" ") + CString(Description[eC_PM_IRREGULAR_PM_CHECK_EQUIPMENT]), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_DESCRIPTION_CHECK_QUALITY],			_T(" ") + CString(Description[eC_PM_IRREGULAR_PM_CHECK_QUALITY]), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_DESCRIPTION_IMPROVE_EQUIPMENT_EE],	_T(" ") + CString(Description[eC_CM_IMPROVE_EQUIPMENT_EE]), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_DESCRIPTION_IMPROVE_PROCESSING_EL],	_T(" ") + CString(Description[eC_CM_IMPROVE_PROCESSING_EL]), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_DESCRIPTION_SETUP_NEW_PRODUCT],		_T(" ") + CString(Description[eC_CM_SETUP_NEW_PRODUCT]), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_DESCRIPTION_CHANGE_SAME_MODEL],		_T(" ") + CString(Description[eC_CHANGE_MODEL_CHANGE_SAME_MODEL]), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_DESCRIPTION_CHANGE_DIFFERENT_MODEL], _T(" ") + CString(Description[eC_CHANGE_MODEL_CHANGE_DIFFERENT_MODEL]), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_DESCRIPTION_CHANGE_MATERIAL],		_T(" ") + CString(Description[eC_MATERIAL_CHANGE_MATERIAL]), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_DESCRIPTION_DOWN_MATERIAL],			_T(" ") + CString(Description[eC_MATERIAL_DOWN_MATERIAL]), 9, RGB_BLACK, RgbColor::AliceBlue, RGB_BLACK, DT_LEFT | DT_VCENTER);
	
	for (int i = eC_CODE_BM; i <= eC_MATERIAL_DOWN_MATERIAL; i++) {
		int iItem = i - eC_CODE_BM;
		CString str(TPMCode[iItem]);
		SelectedDrawText(memDC, m_rectC[i], str, DT_FONTSIZE, DT_PEN, m_iSelectMode == i ? RGB_GREEN : DT_BK, m_iSelectMode == i ? RGB_BLACK : DT_TEXT);
	}
}


void CTPMCodeSelectWnd::OnMouseMove(UINT nFlags, CPoint point) {
	int eMouseOver = m_eMouseOver;
	eMouseOver = eC_NONE;
	if ((m_eSelectedControl != eC_NONE) && (!m_rectC[m_eSelectedControl].PtInRect(point))) {
		int eSelectControl = m_eSelectedControl;
		m_eSelectedControl = eC_NONE;
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
	CFrameWndEx::OnMouseMove(nFlags, point);
}

void CTPMCodeSelectWnd::OnLButtonDown(UINT nFlags, CPoint point) {
	
	//TODO 필요한가?
	if (sm.Option->tKeyPad.fShowKeyPad)
		return;
	
	SetFocus();

	for (int i = 0; i < eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectedControl = i;
			InvalidateRect(m_rectC[i], FALSE);
			break;
		}
	}

	CFrameWndEx::OnLButtonDown(nFlags, point);
}

void CTPMCodeSelectWnd::OnLButtonUp(UINT nFlags, CPoint point)  {
	
	CString strLog;
	auto cleanup = make_scope_exit
	(
		[&]()
		{
			m_eSelectedControl = eC_NONE;
			Invalidate(FALSE);
			CFrameWndEx::OnLButtonUp(nFlags, point);
		}
	);

	if (m_eSelectedControl != eC_NONE && m_rectC[m_eSelectedControl].PtInRect(point)) {
		switch (m_eSelectedControl) {	
			case eC_CODE_BM:
			case eC_PM_IRREGULAR_PM_CHECK_EQUIPMENT:
			case eC_PM_IRREGULAR_PM_CHECK_QUALITY:
			case eC_MATERIAL_CHANGE_MATERIAL:
			case eC_MATERIAL_DOWN_MATERIAL: {
				m_iSelectMode = m_eSelectedControl;
				break;
			}
			case eC_PM_REGULAR_PM:
			case eC_CM_IMPROVE_EQUIPMENT_EE:
			case eC_CM_IMPROVE_PROCESSING_EL:
			case eC_CM_SETUP_NEW_PRODUCT:
			case eC_CHANGE_MODEL_CHANGE_SAME_MODEL:
			case eC_CHANGE_MODEL_CHANGE_DIFFERENT_MODEL: {
				CString strPassword = InPutMessage(_T("Password"), TRUE);
				if (strPassword.Compare(_T("1234")) == 0) {
					m_iSelectMode = m_eSelectedControl;
				}
				else {
					IPMessage(_T("Password Mismatch."));
				}
				break;
			}
			case eC_SEND: {
				//<< MCC
				sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI01].A[eMCC_ACTION_LD_LI_01_MANUAL_STOP] = 1;
				sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI02].A[eMCC_ACTION_LD_LI_02_MANUAL_STOP] = 1;
				sm.MCC_EQP->Layer[eMCC_LAYER_LD_LE01].A[eMCC_ACTION_LD_LE_01_MANUAL_STOP] = 1;
				sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO01].A[eMCC_ACTION_LD_LO_01_MANUAL_STOP] = 1;
				sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO02].A[eMCC_ACTION_LD_LO_02_MANUAL_STOP] = 1;
				sm.MCC_EQP->Layer[eMCC_LAYER_LD_TR01].A[eMCC_ACTION_LD_TR_01_MANUAL_STOP] = 1;
				//>>


				CIPVar var;
				var.SetChildItem("LOSSCODE", TPMCode[m_iSelectMode - eC_CODE_BM]);
				var.SetChildItem("DESCRIPTION", Description[m_iSelectMode - eC_CODE_BM]);
				g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[eEQP_SCT], "Scenario", "MANUAL_STOP", var);
				ShowWindow(SW_HIDE);
				break;
			}
			case eC_CANCEL: {
				ShowWindow(SW_HIDE);
				break;
			}
		}
	}
}
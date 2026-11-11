#include "pch.h"
#include "UI.h"
#include "ConveyorParameterWnd.h"

CConveyorParameter::CConveyorParameter(CWnd* pwndParent)
	: m_pwndParent(pwndParent)
	, m_eMouseOver(eC_NONE)
	, m_eSelectControl(eC_NONE)
	, m_bEdit(FALSE)
	, m_eSelectMotor(0)
{
	memset(&m_ConveyorParameter, 0x00, sizeof(m_ConveyorParameter));
}

CConveyorParameter::~CConveyorParameter() {
}

#define ID_MOTOR_LIST	10002

IMPLEMENT_DYNCREATE(CConveyorParameter, CFrameWndEx)
BEGIN_MESSAGE_MAP(CConveyorParameter, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(LVN_ITEMCHANGED, ID_MOTOR_LIST, OnItemchangedLst)
	ON_MESSAGE(WM_APP_ED_EDIT_FINISHED, OnEditFinished)
END_MESSAGE_MAP()

BOOL CConveyorParameter::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CConveyorParameter::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {
		m_lstMotor.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, m_rectS[eS_CONVEYOR_LIST], this, ID_MOTOR_LIST);
		m_lstMotor.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
		m_lstMotor.ModifyStyle(0, LVS_SHOWSELALWAYS);
		m_lstMotor.SetRowHeight(45);
		m_lstMotor.ShowScrollBar(SB_VERT);
		m_lstMotor.ShowScrollBar(SB_HORZ);
		m_lstMotor.InsertColumn(eLI_NONE, _T(""), LVCFMT_LEFT, 0, 1);
		m_lstMotor.InsertColumn(eLI_CONVEYOR_INDEX, _T("Axis"), LVCFMT_LEFT, 100, 1);
		m_lstMotor.InsertColumn(eLI_CONVEYOR_NAME, _T("Name"), LVCFMT_LEFT, 200, 1);

		for (int i = 0; i < nCONVEYOR_LD; i++) {
			m_lstMotor.InsertItem(i, _T(""));
			m_lstMotor.SetItemInt(i, eLI_CONVEYOR_INDEX, i);
			m_lstMotor.SetItemText(i, eLI_CONVEYOR_NAME, g_stMotor[IDS_CONVEYOR_NAME_01 + i]);
			m_lstMotor.SetAlignText(eLI_CONVEYOR_NAME, eLCETA_LEFT);
		}

		SyncData();

	}
	catch (...) {
		return -1;
	}

	return 0;
}

void CConveyorParameter::ChangeLanguage()
{
	for (int i = 0; i < nCONVEYOR_LD; i++) {
		m_lstMotor.SetItemText(i, eLI_CONVEYOR_NAME, g_stMotor[IDS_CONVEYOR_NAME_01 + i]);
	}
	Invalidate(FALSE);
}

void CConveyorParameter::OnTimer(UINT_PTR nIDEvent) {
	//switch (nIDEvent) {
	//	default:
	//		break;
	//}
	CFrameWndEx::OnTimer(nIDEvent);
}

void CConveyorParameter::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl = eC_NONE;
		SyncData(FALSE);
		m_lstMotor.SetItemState(m_eSelectMotor, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);


		Invalidate();
	}
	else {
	}
}

BOOL CConveyorParameter::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}

void CConveyorParameter::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);

	CRect rect = m_rectS[eS_FULL_AREA];
	DrawLineRect(memDC, rect, RGB_WHITE);
	rect.DeflateRect(1, 1);
	memDC.FillSolidRect(rect, RgbColor::Gainsboro);
	memDC.FillSolidRect(m_rectS[eS_WORK_AREA], RgbColor::AliceBlue);

	auto Conveyor = m_ConveyorParameter[m_eSelectMotor];

	//<< 상단 UI
	DrawMDIMenuButton(memDC, m_rectC[eC_EDIT], m_bEdit ? g_st[IDS_PARAMETER_EDIT] : g_st[IDS_PARAMETER_LOCK], m_eSelectControl == eC_EDIT || !m_bEdit, m_eMouseOver == eC_EDIT);
	DrawMDIMenuButton(memDC, m_rectC[eC_SAVE], g_st[IDS_PARAMETER_SAVE], m_eSelectControl == eC_SAVE, m_eMouseOver == eC_SAVE);

	DrawMDIMenuButton(&memDC, m_rectC[eC_BTN_ALL_SET_PARAMETER], _T("All Set Parameter"), m_eSelectControl == eC_BTN_ALL_SET_PARAMETER, m_eMouseOver == eC_BTN_ALL_SET_PARAMETER);
	DrawGroupBox(&memDC, m_rectS[eS_CONVEYOR_PARAMETER_AREA], _T("Parameter"), CSize(-1, -1), CPoint(15, -5), 9, RGB_BLACK, RGB_BLACK);

	DrawText(memDC, m_rectS[eS_PARAMETER_FORWARD], g_st[IDS_CONVEYOR_PARAMETER_FORWARD], 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	DrawText(memDC, m_rectS[eS_PARAMETER_SPEED_RATE], g_st[IDS_CONVEYOR_PARAMETER_SPEED_RATE], 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);

	SelectedDrawText(memDC, m_rectC[eC_BTN_PARAMETER_FORWARD], Conveyor.bForward ? g_st[IDS_CONVEYOR_PARAMETER_FORWARD] : g_st[IDS_CONVEYOR_PARAMETER_BACKWARD]);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_EDIT_PARAMETER_SPEED_RATE], Conveyor.dSpeedRate, 1, m_bEdit);
}

void CConveyorParameter::OnMouseMove(UINT nFlags, CPoint point) {
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
	CFrameWndEx::OnMouseMove(nFlags, point);
}



void CConveyorParameter::OnLButtonDown(UINT nFlags, CPoint point) {
	SetFocus();
	m_eSelectControl = eC_NONE;
	for (int i = 0; i < eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			InvalidateRect(m_rectC[i], FALSE);
			break;
		}
	}
	CFrameWndEx::OnLButtonDown(nFlags, point);
}



void CConveyorParameter::OnLButtonUp(UINT nFlags, CPoint point) {

	CString strLog;
	auto cleanup = make_scope_exit
	(
		[&]()
		{
			if (!strLog.IsEmpty()) {
				WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
			}
			m_eSelectedEdit = m_eSelectControl;
			m_eSelectControl = eC_NONE;

			Invalidate(FALSE);

			CFrameWndEx::OnLButtonUp(nFlags, point);
		}
	);

	if ((m_eSelectControl != eC_NONE) && m_rectC[m_eSelectControl].PtInRect(point)) {

		//. Edit 영역에 대해 수정 가능한지 확인.
		if (!m_bEdit && (m_eSelectControl >= eC_BTN_PARAMETER_FORWARD && m_eSelectControl <= eC_EDIT_PARAMETER_SPEED_RATE))
			return;


		auto& Conveyor = m_ConveyorParameter[m_eSelectMotor];
		switch (m_eSelectControl) {
		case eC_EDIT: {
			if (m_bEdit || !sm.Sequence->fAutoMode || sm.Sequence->fCycleStop)
				m_bEdit = !m_bEdit;
			else
				IPMessage(g_st[IDS_CONVEYOR_PARAM_WND_CAN_NOT_CHANGE_AUTO_RUN]);
			strLog = _T("잠금/편집");
			break;
		}
		case eC_SAVE: {
			//SyncData(TRUE);
			strLog = _T("저장/불러오기");
			if (!m_bEdit) {
				SyncData(FALSE);
			}
			else
			{
				std::vector<CString> ChangeLog;
				std::vector<std::pair<CString, COLORREF>> changeDatas;
				for (int iIndex = 0; iIndex < nCONVEYOR_LD; iIndex++)
				{
					T_CONVEYOR_PARAMETER* pNew = (T_CONVEYOR_PARAMETER*)(&m_ConveyorParameter[iIndex]);
					T_CONVEYOR_PARAMETER* pOrg = (T_CONVEYOR_PARAMETER*)(&sm.Conveyor_Parameter[iIndex]);
					CString strName = g_stMotor[IDS_CONVEYOR_NAME_01 + iIndex];
					if (memcmp(&m_ConveyorParameter[iIndex], &sm.Conveyor_Parameter[iIndex], sizeof(sm.Conveyor_Parameter[iIndex]))) {
						changeDatas.push_back({ Format(_T("[%s]"), strName), RGB_SKY_LIGHT });

						if (pNew->bForward != pOrg->bForward) {
							changeDatas.push_back({ Format(_T("Direction %s -> %s"), pOrg->bForward ? _T("Forward") : _T("Backward"), pNew->bForward ? _T("Forward") : _T("Backward")), RGB_WHITE});
							ChangeLog.push_back(Format(_T("%s;%s;%s;%s"), Format(_T("[%s] Forward"), strName), _T("Conveyor"), pOrg->bForward ? _T("Forward") : _T("Backward"), pNew->bForward ? _T("Forward") : _T("Backward")));
						}
						if (pNew->dSpeedRate != pOrg->dSpeedRate) {
							changeDatas.push_back({ Format(_T("Speed Rate %.3f -> %.3f"), pOrg->dSpeedRate, pNew->dSpeedRate), RGB_WHITE });
							ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), Format(_T("[%s] Speed Rate"), strName), _T("Conveyor"), pOrg->dSpeedRate, pNew->dSpeedRate));
						}
					}
				}
				if (ChangeLog.empty() || changeDatas.empty()) {
					return;
				}
				CChangeParamDlg dlg;
				dlg.SetChangeData(changeDatas);
				if (dlg.DoModal() == IDOK)
				{
					WriteParameterLog(g_st[IDS_CONVEYOR_PARAM_WND_CONVEYOR_PARAM], _T(""), ChangeLog, theApp.m_UserManager.GetCurrentUserData().szID);
					SyncData(TRUE);
					m_bEdit = TRUE;
				}
			}
			break;
		}


		case eC_BTN_PARAMETER_FORWARD: {
			Conveyor.bForward = !Conveyor.bForward;
			break;
		}
		case eC_EDIT_PARAMETER_SPEED_RATE: {
			SHOW_KEYPAD_EDIT(this, m_eSelectControl, Conveyor.dSpeedRate);
			break;
		}
		case eC_BTN_ALL_SET_PARAMETER: {
			for (int i = 0; i < nCONVEYOR_LD; i++) {
				m_ConveyorParameter[i].dSpeedRate = Conveyor.dSpeedRate;
			}
			strLog = _T("All Set Parameter");
			break;
		}
		default: break;
		}
	}
}

void  CConveyorParameter::OnCbnSelchangeComboType(UINT nID) {


	static auto cboToValue = [&](const CComboBox& cbo) -> int {
		CStringW strData;
		cbo.GetWindowText(strData);

		int iValue = -1;
		try {

			iValue = std::stoi(CStringA(strData).GetBuffer());
		}
		catch (std::exception& exp) {

		}
		return iValue;
	};

	Invalidate(FALSE);
	//>>
}

void CConveyorParameter::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);

	CRect		rect = m_rectS[eS_FULL_AREA];
	//rect.top += 30;
	double dH = (rect.Height() - 5.) / 16.;
	double dW = (rect.Width() - 5.) / 20.;

	m_rectS[eS_CONVEYOR_LIST]	= CRect(rect.left + dW * 0 + 5, rect.top + dH * 0 + 5, rect.left + dW * 5, rect.top + dH * 16);
	m_rectS[eS_WORK_AREA]		= CRect(rect.left + dW * 5 + 5, rect.top + dH * 1 + 5, rect.left + dW * 20, rect.top + dH * 16);

	m_rectC[eC_BTN_ALL_SET_PARAMETER] = CRect(rect.left + dW * 5 + 5, rect.top + dH * 0 + 5, rect.left + dW * 7, rect.top + dH * 1);
	m_rectC[eC_EDIT] = CRect(rect.left + dW * 18 + 5, rect.top + dH * 0 + 5, rect.left + dW * 19, rect.top + dH * 1);
	m_rectC[eC_SAVE] = CRect(rect.left + dW * 19 + 5, rect.top + dH * 0 + 5, rect.left + dW * 20, rect.top + dH * 1);

	rect = m_rectS[eS_WORK_AREA];
	dH = (rect.Height() - 5.) / 15.;
	dW = (rect.Width() - 5.) / 12.;

	m_rectS[eS_CONVEYOR_PARAMETER_AREA] = CRect(rect.left + dW * 0 + 5, rect.top + dH * 0 + 5, rect.left + dW * 4, rect.top + dH * 8);

	rect = m_rectS[eS_CONVEYOR_PARAMETER_AREA];
	rect.top += 20;
	dH = (rect.Height() - 5.) / 8.;
	dW = (rect.Width() - 5.) / 4.;

	m_rectS[eS_PARAMETER_FORWARD]			= CRect(rect.left + dW * 0 + 5, rect.top + dH * 0 + 5, rect.left + dW * 2, rect.top + dH * 1);
	m_rectS[eS_PARAMETER_SPEED_RATE]		= CRect(rect.left + dW * 0 + 5, rect.top + dH * 1 + 5, rect.left + dW * 2, rect.top + dH * 2);
	m_rectC[eC_BTN_PARAMETER_FORWARD]		= CRect(rect.left + dW * 2 + 5, rect.top + dH * 0 + 5, rect.left + dW * 4, rect.top + dH * 1);
	m_rectC[eC_EDIT_PARAMETER_SPEED_RATE]	= CRect(rect.left + dW * 2 + 5, rect.top + dH * 1 + 5, rect.left + dW * 4, rect.top + dH * 2);

	m_lstMotor.MoveWindow(m_rectS[eS_CONVEYOR_LIST]);

}
void CConveyorParameter::SyncData(BOOL bStore) {

	CProfile profile(_T("../cfg/ConveyorParameter.ini"));
	if (bStore) {
		profile.GetSections().DeleteAll();
	}

	BOOL bSetting = TRUE;
	for (int i = 0; i < nCONVEYOR_LD; i++) {
		CString strMotorIndex = Format(_T("Conveyor_%02d"), i);
		CProfileSection& section = profile.GetSection(strMotorIndex);

		CProfileSection& subSection = section.GetSubSection(_T("Parameter"));
		subSection.SyncItemValue(bStore, _T("bForward"), m_ConveyorParameter[i].bForward);
		subSection.SyncItemValue(bStore, _T("dSpeedRate"), m_ConveyorParameter[i].dSpeedRate);

		memcpy(&sm.Conveyor_Parameter[i], &m_ConveyorParameter[i], sizeof(T_CONVEYOR_PARAMETER));
	}

	if (!bSetting)
		IPMessage(g_st[IDS_CONVEYOR_PARAM_WND_SETTING_FAIL]);

	if (bStore)
		profile.Save();
}


BOOL CConveyorParameter::ChangeParameter()
{
	CString strChange;
	for (int i = eMOTOR_0; i < eMOTOR_END; i++) {

	}
	return TRUE;
}

void CConveyorParameter::OnItemchangedLst(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if ((pNMLV->uNewState & LVIS_SELECTED) && !(pNMLV->uOldState & LVIS_SELECTED)) { // Select 

		if (!sm.Option->tKeyPad.fShowKeyPad) {

			m_eSelectMotor = pNMLV->iItem;

			for (int i = 0; i < m_lstMotor.GetItemCount(); i++)
				m_lstMotor.SetItemBKColor(i, RGB_WHITE);

			m_lstMotor.SetItemBKColor(pNMLV->iItem, RGB_GRAY_LIGHT);

			Invalidate(FALSE);
		}
	}
}

LRESULT CConveyorParameter::OnEditFinished(WPARAM wParam, LPARAM lParam)
{
	if (wParam) {

		CString	strNewItem(GetEditString());
		switch (m_eSelectedEdit) {
		}

		m_eSelectedEdit = eC_NONE;
		Invalidate(FALSE);
	}

	return 0;
}

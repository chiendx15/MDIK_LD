#include "pch.h"
#include "UI.h"
#include "./DeviceParameterDefaultWnd.h"

LPCTSTR	lpDistance[] = {
    // LJY 수정 및 추가 24.03.19
    _T("Load Before Distance"),
    _T("Load Before Speed"),
    _T("Load After Distance"),
    _T("Load After Speed"),
    _T("UnLoad Before Distance"),
    _T("UnLoad Before Speed"),
    _T("UnLoad After Distance"),
    _T("UnLoad After Speed"),
    _T("Transfer Paper Cylinder Down Size"),
};

LPCTSTR lpTime[] = {
	_T("Conveyor High Speed Time Out"),
	_T("Conveyor Low Speed Time Out"),
	_T("Conveyor Stoper Time Out"),
	_T("Air Shower Door Time Out"),
	_T("Cleaner Time Out"),
	_T("Air Shower Door Open Delay"),
	_T("Conveyor Move Delay"),
    _T("Logout Time"),  // KES 추가 2024.2.14
	_T("Align Delay Time"), // LJY 추가 2024.4.29
};

LPCTSTR lpCount[] = {
	_T("Glass Retry Count"),
	_T("Paper Retry Count"),
	_T("Glass CV Align Retry Count"),
};

LPCTSTR lpUse[] = {
	_T("Dry Run"),
	_T("PPBoxInOutSwitchUse"),
	_T("Ganzi Alarm"),
	_T("PC Link"),	// KES 수정 2024.4.24 - PC Link가 맞음
};

IMPLEMENT_DYNCREATE		(CDeviceParameterDefaultWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CDeviceParameterDefaultWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()



CDeviceParameterDefaultWnd::CDeviceParameterDefaultWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
	m_bLock				= TRUE;
	memset(&m_tDistance,	0x00, sizeof(m_tDistance));
	memset(&m_tTime,		0x00, sizeof(m_tTime));
	memset(&m_tCount,		0x00, sizeof(m_tCount));
	memset(&m_tUse,			0x00, sizeof(m_tUse));
}

CDeviceParameterDefaultWnd::~CDeviceParameterDefaultWnd() {
}

BOOL CDeviceParameterDefaultWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CDeviceParameterDefaultWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {
		SyncData(FALSE);
	}
	catch (...) {
		return -1;
	}
	return 0;
}

void CDeviceParameterDefaultWnd::OnKillFocus(CWnd* pNewWnd) {
	CFrameWndEx::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
	Invalidate();
}

void CDeviceParameterDefaultWnd::OnTimer(UINT_PTR nIDEvent) {
	CFrameWndEx::OnTimer(nIDEvent);
}

void CDeviceParameterDefaultWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		Invalidate();
		m_bLock = TRUE;
		SyncData(FALSE);
	}
	else {
	}
}

BOOL CDeviceParameterDefaultWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}

void CDeviceParameterDefaultWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	
	CRect				rect = m_rectS[eS_FULL_AREA];
	memDC.FillSolidRect(rect, RgbColor::AliceBlue);

	DrawGroupBox(&memDC, m_rectS[eS_DEVICE_DISTACNE_PARAMETER],		g_st[IDS_DEVICE_PARAM_DEF_WND_DISTANCE_SPEED_EP_REGULATOR], RGB_BLACK, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_DEVICE_TIME_PARAMETER],			g_st[IDS_DEVICE_PARAM_DEF_WND_TIME], RGB_BLACK, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_DEVICE_USE_PARAMETER],			g_st[IDS_DEVICE_PARAM_DEF_WND_USE_PARAM], RGB_BLACK, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_DEVICE_COUNT_PERCENT_PARAMETER],g_st[IDS_DEVICE_PARAM_DEF_WND_COUNT_PERCENT_PARAM], RGB_BLACK, RGB_BLACK);
	
	DrawMDIMenuButton(memDC, m_rectC[eC_EDIT], m_bLock ? g_st[IDS_PARAMETER_LOCK] : g_st[IDS_PARAMETER_EDIT], m_eSelectControl == eC_EDIT || m_bLock, m_eMouseOver == eC_EDIT);
	DrawMDIMenuButton(memDC, m_rectC[eC_SAVE], m_bLock ? g_st[IDS_PARAMETER_LOAD] : g_st[IDS_PARAMETER_SAVE], m_eSelectControl == eC_SAVE, m_eMouseOver == eC_SAVE);
	//if (m_bLock)
	//	SelectedDrawText(&memDC, m_rectC[eC_EDIT], g_st[IDS_PARAMETER_LOCK], 9, RGB_BLACK, RGB_GREEN, RGB_BLACK, DT_CENTER | DT_VCENTER);
	//else 
	//	DrawButtonUpDownMouseMove(memDC, m_rectC[eC_EDIT], g_st[IDS_PARAMETER_EDIT], m_eSelectControl, m_eMouseOver, eC_EDIT, 9, DT_CENTER | DT_VCENTER);
	//
	//DrawButtonUpDown(memDC, m_rectC[eC_SAVE], m_bLock ? g_st[IDS_PARAMETER_LOAD] : g_st[IDS_PARAMETER_SAVE], m_eSelectControl == eC_SAVE, 9, DT_RIGHT | DT_VCENTER);
	
	const int iDistanceSize = sizeof (m_tDistance) / sizeof (double);
    const CString strUnit[iDistanceSize] =
    {
        L"mm", L"mm/sec", L"mm", L"mm/sec", L"mm", L"mm/sec", L"mm", L"mm/sec", L"mm",
    };
	double* dDistance = (double*)&m_tDistance;
	for (int i = 0; i < iDistanceSize; i++) {
		DrawText(memDC, m_rectS[eS_LD_BEFORE_DISTANCE +i], g_st[IDS_DEVICE_PARAMETER_LOAD_BEFORE_DISTANCE + i], DT_FONTSIZE, RGB_BLACK);
		//if (eC_LD_BEFORE_DISTANCE + i == eC_ULD_AFTER_SPEED)
		//	SelectedDrawEditDouble(memDC, m_rectC[eC_LD_BEFORE_DISTANCE +i], dDistance[i], 1, !m_bLock);
		//else 
			SelectedDrawEditDouble(memDC, m_rectC[eC_LD_BEFORE_DISTANCE +i], dDistance[i], 3, !m_bLock);

		DrawText(memDC, m_rectS[eS_DISTANCE_UNIT_1+i], strUnit[i], DT_FONTSIZE, RGB_BLACK, DT_LEFT|DT_VCENTER);
	}
	const int iTimeSize = sizeof (m_tTime) / sizeof (double);
	double* dTime= (double*)&m_tTime;
	for (int i = 0; i < iTimeSize; i++) {
		DrawText(memDC, m_rectS[eS_CONVEYOR_H +i], g_st[IDS_DEVICE_PARAMETER_CONVEYOR_H + i], DT_FONTSIZE, RGB_BLACK);
		SelectedDrawEditDouble(memDC, m_rectC[eC_CONVEYOR_H +i], dTime[i], 2, !m_bLock);
		DrawText(memDC, m_rectS[eS_TIME_UNIT_1+i], m_strUnit[eDETAIL_TIME], DT_FONTSIZE, RGB_BLACK, DT_LEFT|DT_VCENTER);
	}
	const int iCountSize = sizeof (m_tCount) / sizeof (int);
	int* iCount = (int*)&m_tCount;
	for (int i = 0; i < iCountSize; i++) {
		DrawText(memDC, m_rectS[eS_GLASS_VACUUM_RETRY+i], g_st[IDS_DEVICE_PARAMETER_GLASS_RETRY_COUNT+i], DT_FONTSIZE, RGB_BLACK);
		SelectedDrawEditInt(memDC, m_rectC[eC_GLASS_VACUUM_RETRY+i], iCount[i], !m_bLock);
		DrawText(memDC, m_rectS[eS_COUNT_UNIT_1+i], m_strUnit[eDETAIL_COUNT_PERCENT], DT_FONTSIZE, RGB_BLACK, DT_LEFT|DT_VCENTER);
	}

	DrawText(memDC, m_rectS[eS_DRY_RUN], g_st[IDS_DEVICE_PARAMETER_DRY_RUN], DT_FONTSIZE, RGB_BLACK);
	DrawMDIMenuButton(memDC, m_rectC[eC_DRY_RUN], m_tUse.bDryRun ? g_st[IDS_DEVICE_PARAM_DEF_WND_USE] : g_st[IDS_DEVICE_PARAM_DEF_WND_UNUSE], m_tUse.bDryRun || (!m_bLock && m_eSelectControl == eC_DRY_RUN), !m_bLock && m_eMouseOver == eC_DRY_RUN);
	
	DrawText(memDC, m_rectS[eS_SWITCH_USE], g_st[IDS_DEVICE_PARAM_DEF_WND_SWITCH_USE], DT_FONTSIZE, RGB_BLACK);
	DrawMDIMenuButton(memDC, m_rectC[eC_SWITCH_USE], m_tUse.bPPBoxInOutSwitchUse ? g_st[IDS_DEVICE_PARAM_DEF_WND_USE] : g_st[IDS_DEVICE_PARAM_DEF_WND_UNUSE], m_tUse.bPPBoxInOutSwitchUse || (!m_bLock && m_eSelectControl == eC_SWITCH_USE), !m_bLock && m_eMouseOver == eC_SWITCH_USE);

	DrawText(memDC, m_rectS[eS_GANZI_ALARM_SKIP], g_st[IDS_DEVICE_PARAM_DEF_WND_PAPER_ALARM], DT_FONTSIZE, RGB_BLACK);
	DrawMDIMenuButton(memDC, m_rectC[eC_GANZI_ALARM_SKIP], m_tUse.bGanziAlarmSkip ? g_st[IDS_DEVICE_PARAM_DEF_WND_SKIP] : g_st[IDS_DEVICE_PARAM_DEF_WND_OCCURRENT], m_tUse.bGanziAlarmSkip || (!m_bLock && m_eSelectControl == eC_GANZI_ALARM_SKIP), !m_bLock && m_eMouseOver == eC_GANZI_ALARM_SKIP);

	DrawText(memDC, m_rectS[eS_PC_LINKER], g_st[IDS_DEVICE_PARAM_DEF_WND_PC_LINKER], DT_FONTSIZE, RGB_BLACK);
	DrawMDIMenuButton(memDC, m_rectC[eC_PC_LINKER], m_tUse.bPCLinkerUse ? g_st[IDS_DEVICE_PARAM_DEF_WND_USE] : g_st[IDS_DEVICE_PARAM_DEF_WND_UNUSE], m_tUse.bPCLinkerUse || (!m_bLock && m_eSelectControl == eC_PC_LINKER), !m_bLock && m_eMouseOver == eC_PC_LINKER);
}

void CDeviceParameterDefaultWnd::OnMouseMove(UINT nFlags, CPoint point) {
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

void CDeviceParameterDefaultWnd::OnLButtonDown(UINT nFlags, CPoint point) {
	SetFocus();
	m_eSelectControl = eC_NONE;
	for (int i = 0; i<eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			InvalidateRect(m_rectC[i],FALSE);
			break;
		}
	}
	CFrameWndEx::OnLButtonDown(nFlags, point);
}

void CDeviceParameterDefaultWnd::OnLButtonUp(UINT nFlags, CPoint point)  {	
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_EDIT: {
				if (!m_bLock || !sm.Sequence->fAutoMode || sm.Sequence->fCycleStop)
					m_bLock = !m_bLock;
				else
					IPMessage(g_st[IDS_DEVICE_PARAM_DEF_WND_CAN_NOT_CHANGE_AUTO_RUN]);
				strLog = _T("잠금/편집");
				break;
			}
			case eC_SAVE: {
				//SyncData(!m_bLock);
				strLog = _T("저장/불러오기");
				if (m_bLock) {
					SyncData(FALSE);
				}
				else {
					std::vector<CString> ChangeLog;
					std::vector<std::pair<CString, COLORREF>> changeDatas;
					double* pNew = (double*)(&m_tDistance);
					double* pOrg = (double*)(&sm.DeviceParameter->tDistance);
					for (int iIndex = 0; iIndex < countof(lpDistance); iIndex++)
					{
						auto& newParam = pNew[iIndex]; auto& orgParam = pOrg[iIndex];

						if (newParam != orgParam) 
						{
							CString strName(lpDistance[iIndex]);
							changeDatas.push_back({ Format(_T("[%s]"), strName), RGB_SKY_LIGHT });
							if (fabs(newParam- orgParam) > 0.001)
							{
								changeDatas.push_back({ Format(_T("Distance %.3f -> %.3f"), orgParam, newParam), RGB_WHITE });
								ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), strName, _T("Distance"), orgParam, newParam));
							}
						}
					}
					pNew = (double*)(&m_tTime);
					pOrg = (double*)(&sm.DeviceParameter->tTime);
					for (int iIndex = 0; iIndex < countof(lpTime); iIndex++)
					{
						auto& newParam = pNew[iIndex]; auto& orgParam = pOrg[iIndex];

						if (newParam != orgParam)
						{
							CString strName(lpTime[iIndex]);
							changeDatas.push_back({ Format(_T("[%s]"), strName), RGB_SKY_LIGHT });
							if (fabs(newParam - orgParam) > 0.001)
							{
								changeDatas.push_back({ Format(_T("Time %.3f -> %.3f"), orgParam, newParam), RGB_WHITE });
								ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), strName, _T("Time"), orgParam, newParam));
							}
						}
					}
					int* iNew = (int*)(&m_tCount);
					int* iOrg = (int*)(&sm.DeviceParameter->tCount);
					for (int iIndex = 0; iIndex < countof(lpCount); iIndex++)
					{
						auto& newParam = iNew[iIndex]; auto& orgParam = iOrg[iIndex];

						if (newParam != orgParam)
						{
							CString strName(lpCount[iIndex]);
							changeDatas.push_back({ Format(_T("[%s]"), strName), RGB_SKY_LIGHT });
							if (fabs(newParam - orgParam) > 0.001)
							{
								changeDatas.push_back({ Format(_T("Count %d -> %d"), orgParam, newParam), RGB_WHITE });
								ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), strName, _T("Count"), orgParam, newParam));
							}
						}
					}
					if (m_tUse.bDryRun != sm.DeviceParameter->tUse.bDryRun) {
						CString strName = g_st[IDS_DEVICE_PARAM_DEF_WND_PAPER_DRY_RUN];
						CString strNew = m_tUse.bDryRun ? _T("Use") : _T("Not Use");
						CString strOld = sm.DeviceParameter->tUse.bDryRun ? _T("Use") : _T("Not Use");
						changeDatas.push_back({ Format(_T("[%s]"), strName), RGB_SKY_LIGHT });
						changeDatas.push_back({ Format(_T("%s -> %s"), strOld, strNew), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%s;%s"), strName, _T("Use"), strOld, strNew));
					}
					if (m_tUse.bGanziAlarmSkip != sm.DeviceParameter->tUse.bGanziAlarmSkip) {
						CString strName = g_st[IDS_DEVICE_PARAM_DEF_WND_PAPER_ALARM_SKIP];
						CString strNew = m_tUse.bGanziAlarmSkip ? _T("Skip") : _T("Not Skip");
						CString strOld = sm.DeviceParameter->tUse.bGanziAlarmSkip ? _T("Skip") : _T("Not Skip");
						changeDatas.push_back({ Format(_T("[%s]"), strName), RGB_SKY_LIGHT });
						changeDatas.push_back({ Format(_T("%s -> %s"), strOld, strNew), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%s;%s"), strName, _T("Use"), strOld, strNew));
					}
					if (m_tUse.bPCLinkerUse != sm.DeviceParameter->tUse.bPCLinkerUse) {
						CString strName = g_st[IDS_DEVICE_PARAM_DEF_WND_PC_LINKER];
						CString strNew = m_tUse.bPCLinkerUse ? _T("Use") : _T("Not Use");
						CString strOld = sm.DeviceParameter->tUse.bPCLinkerUse ? _T("Use") : _T("Not Use");
						changeDatas.push_back({ Format(_T("[%s]"), strName), RGB_SKY_LIGHT });
						changeDatas.push_back({ Format(_T("%s -> %s"), strOld, strNew), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%s;%s"), strName, _T("Use"), strOld, strNew));
					}
					if (m_tUse.bPPBoxInOutSwitchUse != sm.DeviceParameter->tUse.bPPBoxInOutSwitchUse) {
						CString strName = g_st[IDS_DEVICE_PARAM_DEF_WND_SWITCH_USE];
						CString strNew = m_tUse.bPPBoxInOutSwitchUse ? _T("Use") : _T("Not Use");
						CString strOld = sm.DeviceParameter->tUse.bPPBoxInOutSwitchUse ? _T("Use") : _T("Not Use");
						changeDatas.push_back({ Format(_T("[%s]"), strName), RGB_SKY_LIGHT });
						changeDatas.push_back({ Format(_T("%s -> %s"), strOld, strNew), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%s;%s"), strName, _T("Use"), strOld, strNew));
					}

					if (ChangeLog.empty() || changeDatas.empty()) {
						return;
					}
					CChangeParamDlg dlg;
					dlg.SetChangeData(changeDatas);
					if (dlg.DoModal() == IDOK)
					{
						WriteParameterLog(g_st[IDS_DEVICE_PARAM_DEF_WND_DEVICE_PARAM], g_st[IDS_DEVICE_PARAM_DEF_WND_BASIC_PARAM], ChangeLog, theApp.m_UserManager.GetCurrentUserData().szID);
						// KES 주석 2024.4.3 - 베트남 장비에서 CimSkip 모드 UI에서 변경 불가능하게 수정함
						//if (m_tUse.bCIMSkip != sm.DeviceParameter->tUse.bCIMSkip) {
						//	CIPVar var;
						//	var.SetChildItem("CimReport", !m_tUse.bCIMSkip);	//LD만 부정적으로 사용중이다 그러므로 반대로 보내야 한다
						//	if (!g_ipClient.SendIPCommand("DeviceParam", "CimReportChange", var, 5 * 1000)) {
						//		if (MessageBox(_T("CIM Skip 모드가 다른 PC와 다릅니다 그래도 진행 하시겠습니까?"), _T("CIM Skip"), MB_YESNO) == IDNO) {
						//			m_tUse.bCIMSkip = sm.DeviceParameter->tUse.bCIMSkip;
						//		}
						//	}
						//}

						SyncData(TRUE);
						m_bLock = TRUE;
					}
				}
				break;
			}
			case eC_LD_BEFORE_DISTANCE:
            case eC_LD_BEFORE_SPEED:    
            case eC_LD_AFTER_DISTANCE:
            case eC_LD_AFTER_SPEED:     
            case eC_ULD_BEFORE_DISTANCE:
            case eC_ULD_BEFORE_SPEED:
            case eC_ULD_AFTER_DISTANCE:
            case eC_ULD_AFTER_SPEED:
            case eC_TRANSFET_PAPER_CYLINDER_DOWN_SIZE: {
				if (m_bLock)
					break;
				double* dDistance = (double*)&m_tDistance;
				EditStart(this, m_rectC[m_eSelectControl], dDistance[m_eSelectControl - eC_LD_BEFORE_DISTANCE]);
				break;
			}
			case eC_CONVEYOR_H					:
			case eC_CONVEYOR_L					:
			case eC_CONVEYOR_STOPER				:
			case eC_CURTAIN						:
			case eC_CLEANER						:
			case eC_AIR_SHOWER_DOOR				:
			case eC_CONVEYOR_MOVE				:
            case eC_LOGOUT_TIME		    		:// KES 추가 2024.2.14
			case eC_ALIGN_R_DELAY_TIME			:{  // LJY 추가 2024.4.29
				if (m_bLock)
					break;
				double* dTime = (double*)&m_tTime;
				EditStart(this, m_rectC[m_eSelectControl], dTime[m_eSelectControl - eC_CONVEYOR_H]);
				break;
			}
			case eC_GLASS_VACUUM_RETRY	:		
			case eC_PAPER_VACUUM_RETRY	:		
			case eC_ALIGN_RETRY_COUNT	:{
				if (m_bLock)
					break;
				int* iCount = (int*)&m_tCount;
				EditStart(this, m_rectC[m_eSelectControl], iCount[m_eSelectControl - eC_GLASS_VACUUM_RETRY]);
				break;
			}
			case eC_DRY_RUN :
			{
				if (m_bLock)
					break;
			
				// 21.03.09
				// 현재 드라이런이 아니고 온라인 상태면 변경 안된다.
				if (!m_tUse.bDryRun && !sm.Sequence->bCIMSkip)
				{
					IPMessage(g_st[IDS_DEVICE_PARAM_DEF_WND_CAN_NOT_CHANGE_DRYRUN]);
					break;
				}
			
				m_tUse.bDryRun = !m_tUse.bDryRun;
				break;
			}
			case eC_GANZI_ALARM_SKIP: 
			{
				if (m_bLock)
					break;

				m_tUse.bGanziAlarmSkip = !m_tUse.bGanziAlarmSkip;
				break;
			}
			case eC_PC_LINKER:
			{
				if (m_bLock)
					break;

				// 21.03.09
				// 현재 드라이런이 아니고 온라인 상태면 변경 안된다.
				if (!m_tUse.bPCLinkerUse && !sm.Sequence->bCIMSkip) {
					IPMessage(g_st[IDS_DEVICE_PARAM_DEF_WND_CAN_NOT_CHANGE_PLC_LINKER_UNUSED]);
					break;
				}

				m_tUse.bPCLinkerUse = !m_tUse.bPCLinkerUse;
				break;
			}
			case eC_SWITCH_USE: {
				if (m_bLock)
					break;

				m_tUse.bPPBoxInOutSwitchUse = !m_tUse.bPPBoxInOutSwitchUse;
				break;
			}

			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], g_st[IDS_DEVICE_PARAM_DEF_WND_BASIC_PARAM], strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl	=	eC_NONE;
	Invalidate(FALSE);

	CFrameWndEx::OnLButtonUp(nFlags, point);
}

void CDeviceParameterDefaultWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect		rc = m_rectS[eS_FULL_AREA];
	double dH = (rc.Height() - 5.) / 15.;
	double dW = (rc.Width() - 5.) / 20.;
	m_rectS[eS_DEVICE_DISTACNE_PARAMETER		] = CRect(rc.left+dW*0+5,	rc.top+dH*1+5, rc.left+dW*5,	rc.top+dH*15-5);
	m_rectS[eS_DEVICE_TIME_PARAMETER			] = CRect(rc.left+dW*5+5,	rc.top+dH*1+5, rc.left+dW*10,	rc.top+dH*15-5);
	m_rectS[eS_DEVICE_COUNT_PERCENT_PARAMETER	] = CRect(rc.left+dW*10+5,	rc.top+dH*1+5, rc.left+dW*15,	rc.top+dH*15-5);
	m_rectS[eS_DEVICE_USE_PARAMETER				] = CRect(rc.left+dW*15+5,	rc.top+dH*1+5, rc.left+dW*20,	rc.top+dH*15-5);

	m_rectC[eC_EDIT] = CRect(rc.left+dW*18+5,	rc.top+dH*0+5, rc.left+dW*19,	rc.top+dH*1);
	m_rectC[eC_SAVE] = CRect(rc.left+dW*19+5,	rc.top+dH*0+5, rc.left+dW*20,	rc.top+dH*1);

	rc = m_rectS[eS_DEVICE_DISTACNE_PARAMETER];
	int iSize = sizeof (m_tDistance) / sizeof (double);
	dH = (rc.Height() - 5.) / 14.;
	dW = (rc.Width() - 5.) / 10.;
	
	for (int i = 0; i < iSize; i++) {
		m_rectS[eS_LD_BEFORE_DISTANCE +i]	= CRect(rc.left+dW*0+5,	rc.top+dH*i+5, rc.left+dW*5,	rc.top+dH*(i+1));
		m_rectC[eC_LD_BEFORE_DISTANCE +i]	= CRect(rc.left+dW*5+5,	rc.top+dH*i+5, rc.left+dW*8,	rc.top+dH*(i+1));
		m_rectS[eS_DISTANCE_UNIT_1+i]	= CRect(rc.left+dW*8+5,	rc.top+dH*i+5, rc.left+dW*10,	rc.top+dH*(i+1));
	}

	rc = m_rectS[eS_DEVICE_TIME_PARAMETER];
	iSize = sizeof (m_tTime) / sizeof (double);
	dH = (rc.Height() - 5.) / 14.;
	dW = (rc.Width() - 5.) / 10.;
	
	for (int i = 0; i < iSize; i++) {
		m_rectS[eS_CONVEYOR_H +i]	= CRect(rc.left+dW*0+5,	rc.top+dH*i+5, rc.left+dW*6,	rc.top+dH*(i+1));
		m_rectC[eC_CONVEYOR_H +i]	= CRect(rc.left+dW*6+5,	rc.top+dH*i+5, rc.left+dW*9,	rc.top+dH*(i+1));
		m_rectS[eS_TIME_UNIT_1+i]	= CRect(rc.left+dW*9+5,	rc.top+dH*i+5, rc.left+dW*10,	rc.top+dH*(i+1));
	}

	rc = m_rectS[eS_DEVICE_COUNT_PERCENT_PARAMETER];
	iSize = sizeof (m_tCount) / sizeof (int);
	dH = (rc.Height() - 5.) / 14.;
	dW = (rc.Width() - 5.) / 10.;
	
	for (int i = 0; i < iSize; i++) {
		m_rectS[eS_GLASS_VACUUM_RETRY+i]	= CRect(rc.left+dW*0+5,	rc.top+dH*i+5, rc.left+dW*6,	rc.top+dH*(i+1));
		m_rectC[eC_GLASS_VACUUM_RETRY+i]	= CRect(rc.left+dW*6+5,	rc.top+dH*i+5, rc.left+dW*9,	rc.top+dH*(i+1));
		m_rectS[eS_COUNT_UNIT_1+i]			= CRect(rc.left+dW*9+5,	rc.top+dH*i+5, rc.left+dW*10,	rc.top+dH*(i+1));
	}
	rc = m_rectS[eS_DEVICE_USE_PARAMETER];
	iSize = sizeof (m_tUse) / sizeof (BOOL);
	dH = (rc.Height() - 5.) / 14.;
	dW = (rc.Width() - 5.) / 3.;
	
	//m_rectS[eS_CIM_SKIP]	= CRect(rc.left+dW*0+5,	rc.top+dH*0+5, rc.left+dW*2,	rc.top+dH*1);
	//m_rectC[eC_CIM_SKIP]	= CRect(rc.left+dW*2+5,	rc.top+dH*0+5, rc.left+dW*3,	rc.top+dH*1);
	//
	//m_rectS[eS_GANZI_ALARM_SKIP]	= CRect(rc.left+dW*0+5,	rc.top+dH*1+5, rc.left+dW*2,	rc.top+dH*2);
	//m_rectC[eC_GANZI_ALARM_SKIP]	= CRect(rc.left+dW*2+5,	rc.top+dH*1+5, rc.left+dW*3,	rc.top+dH*2);

	for (int i = 0; i < iSize; i++) {
		m_rectS[eS_DRY_RUN+i]	= CRect(rc.left+dW*0+5,	rc.top+dH*i+5, rc.left+dW*2,	rc.top+dH*(i+1));
		m_rectC[eC_DRY_RUN+i]	= CRect(rc.left+dW*2+5,	rc.top+dH*i+5, rc.left+dW*3,	rc.top+dH*(i+1));
	}
}

BOOL CDeviceParameterDefaultWnd::SyncData(BOOL bStore) {
	CProfile profile(_T("../cfg/DeviceParameter/DeviceParameterDefault.ini"));

	if (bStore)
		profile.GetSections().DeleteAll();

	CProfileSection& Section = profile.GetSection(_T("Device Parameter"));

	int iSize = sizeof(m_tDistance)/sizeof(double);
	double* dDistance = (double*)&m_tDistance;
	CProfileSection& subSectionPos = Section.GetSubSection(_T("Distance"));
	for (int i = 0; i < iSize; i++ )
		subSectionPos.SyncItemValue(bStore, lpDistance[i], dDistance[i], _T("%.3f"));


	iSize = sizeof(m_tTime)/sizeof(double);
	double* dTime = (double*)&m_tTime;
	CProfileSection& subSectionTime = Section.GetSubSection(_T("Time"));
	for (int i = 0; i < iSize; i++ )
		subSectionTime.SyncItemValue(bStore, lpTime[i], dTime[i], _T("%.2f"));

	iSize = sizeof(m_tCount)/sizeof(int);
	int* iCount= (int*)&m_tCount;
	CProfileSection& subSectionCount = Section.GetSubSection(_T("Count"));
	for (int i = 0; i < iSize; i++ )
		subSectionCount.SyncItemValue(bStore, lpCount[i], iCount[i]);

	
	iSize = sizeof(m_tUse)/sizeof(BOOL);
	BOOL* bUse= (BOOL*)&m_tUse;
	CProfileSection& subSectionUse = Section.GetSubSection(_T("USE"));
	for (int i = 0; i < iSize; i++ )
		subSectionUse.SyncItemValue(bStore, lpUse[i], bUse[i]);

	if (bStore)
		profile.Save();

	memcpy(&sm.DeviceParameter->tDistance,	&m_tDistance,	sizeof(T_DEVICE_PARAMETER_DISTANCE));
	memcpy(&sm.DeviceParameter->tTime,		&m_tTime ,		sizeof(T_DEVICE_PARAMETER_TIME	));
	memcpy(&sm.DeviceParameter->tCount,		&m_tCount,		sizeof(T_DEVICE_PARAMETER_COUNT));
	memcpy(&sm.DeviceParameter->tUse,		&m_tUse,		sizeof(T_DEVICE_PARAMETER_USE	));
	
	//sm.Register->Reg_Word_1_NEW.dwABS_Status_Tolerance = sm.DeviceParameter->tDistance.dABSTolerance * 1000;
	return TRUE;
}


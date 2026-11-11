#include "pch.h"
#include "UI.h"
#include "./Glass_load_trasnfer.h"
#include "../MotorControl.h"

#define ID_POSITION_LIST	10002
#define INTERVAL_UI_GLASS_TR 6650 

IMPLEMENT_DYNCREATE		(CGlassLoadTransferWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CGlassLoadTransferWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
	ON_NOTIFY(LVN_ITEMCHANGED,		ID_POSITION_LIST, &CGlassLoadTransferWnd::OnLvnChange)
	ON_NOTIFY(LVN_BEGINLABELEDIT,	ID_POSITION_LIST, OnLvnItemBeginAbleEdit)
	ON_NOTIFY(LVN_ENDLABELEDIT,		ID_POSITION_LIST, OnLvnItemEndAbleEdit)
END_MESSAGE_MAP			()

CGlassLoadTransferWnd::CGlassLoadTransferWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	memset(&m_tGlassTransfer, 0x00, sizeof(T_GLASS_TRANSFER));
	m_eMouseOver		= eC_NONE;
	m_eSelectControl	= eC_NONE;
	m_bLock				= TRUE;
}

CGlassLoadTransferWnd::~CGlassLoadTransferWnd() {
}



BOOL CGlassLoadTransferWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CGlassLoadTransferWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {
		m_lstTeaching.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, m_rectS[eS_PARAMETER_LIST], this, ID_POSITION_LIST);
		m_lstTeaching.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
		m_lstTeaching.ModifyStyle(0, LVS_SHOWSELALWAYS);
		m_lstTeaching.SetRowHeight(45);
		m_lstTeaching.ShowScrollBar(SB_VERT);
		m_lstTeaching.ShowScrollBar(SB_HORZ);

		m_lstTeaching.InsertColumn(eLI_NONE,			_T(""),					LVCFMT_LEFT, 0, 1);
		m_lstTeaching.InsertColumn(eLI_POSITION_NAME,	_T("Position Name"),	LVCFMT_LEFT, 400, 1);
		m_lstTeaching.InsertColumn(eLI_AXIS,			_T("Axis"),				LVCFMT_LEFT, 60, 1);
		m_lstTeaching.InsertColumn(eLI_POSITION,		_T("Position"),			LVCFMT_LEFT, 140, 1);
		m_lstTeaching.InsertColumn(eLI_SPEED,			_T("Speed"),			LVCFMT_LEFT, 115, 1);
		m_lstTeaching.InsertColumn(eLI_GET,				_T(""),					LVCFMT_LEFT, 115, 1);
		//m_lstTeaching.InsertColumn(eLI_MOVE,			_T(""),					LVCFMT_LEFT, 100, 1);
		m_lstTeaching.SetAlignText(eLI_POSITION_NAME, eLCETA_CENTER);
		m_lstTeaching.SetColumnEditDouble(eLI_POSITION);
		m_lstTeaching.SetColumnEditDouble(eLI_SPEED, 1);
		m_lstTeaching.SetColumnButton(eLI_GET, _T("Get Pos"), RGB_GRAY);
		//m_lstTeaching.SetColumnButton(eLI_MOVE,			_T("Move"),				RGB_GRAY);

		for (int i = 0; i < countof(GlassLoadTrasnfer); i++) {
			m_lstTeaching.InsertItem(i, _T(""));
			if (GlassLoadTrasnfer[i].cr)
				m_lstTeaching.SetItemBKColor(i, GlassLoadTrasnfer[i].cr);
			
            m_lstTeaching.SetItemText(i, eLI_POSITION_NAME, g_st[IDS_POSITION_PARAMETER_GLASS_TRANSFER_WAIT_Y + i]);
			m_lstTeaching.SetItemInt(i, eLI_AXIS, GlassLoadTrasnfer[i].eAxis);
		}
		m_pMotorControl = new CMotorControl();
		if (!m_pMotorControl->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL)) {
			throw  1;
		}
		m_pMotorControl->ShowWindow(SW_HIDE);
		SyncData(FALSE);
	}
	catch (...) {
		return -1;
	}

	return 0;
}

 // LJY 추가 2024.06.23
void CGlassLoadTransferWnd::ChangeLanguage()
{
	for (int i = 0; i < countof(GlassLoadTrasnfer); i++) {
		m_lstTeaching.SetItemText(i, eLI_POSITION_NAME, g_st[IDS_POSITION_PARAMETER_GLASS_TRANSFER_WAIT_Y + i]);
	}
	Invalidate(FALSE);
}

void CGlassLoadTransferWnd::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case INTERVAL_UI_GLASS_TR: {
			InvalidateRect(m_rectS[eS_IO_AREA]);
			break;
		}
	}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CGlassLoadTransferWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		SyncData(FALSE);
		SetListItem();
		m_bLock = TRUE;
		Invalidate();
		T_POS* pPos = (T_POS*)&m_tGlassTransfer;
		m_pMotorControl->SetMotorInfo(pPos[0]);
		m_pMotorControl->MoveWindow(m_rectS[eS_MOTOR_CONTROL]);
		m_pMotorControl->ShowWindow(SW_SHOW);
		m_lstTeaching.SetMark(0);	// KES 추가 2024.5.1
		SetTimer(INTERVAL_UI_GLASS_TR, 100, nullptr);
	}
	else {
		KillTimer(INTERVAL_UI_GLASS_TR);
	}
}


BOOL CGlassLoadTransferWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CGlassLoadTransferWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CRect				rect = m_rectS[eS_FULL_AREA];
	memDC.FillSolidRect(rect, RgbColor::AliceBlue);

	DrawGroupBox(&memDC, m_rectS[eS_IO_AREA], _T("IO"), RGB_SKY, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_STATUS_AREA], g_st[IDS_GLASS_LD_TRANSFER_STATUS], RGB_SKY, RGB_BLACK);

	DrawMDIMenuButton(memDC, m_rectC[eC_LOCK], m_bLock ? g_st[IDS_PARAMETER_LOCK] : g_st[IDS_PARAMETER_EDIT], m_eSelectControl == eC_LOCK || m_bLock, m_eMouseOver == eC_LOCK);
	DrawMDIMenuButton(memDC, m_rectC[eC_SAVE], g_st[IDS_PARAMETER_SAVE], m_eSelectControl == eC_SAVE, m_eMouseOver == eC_SAVE);
	
	auto IOControl = [&](const int _enum, const CString& _text, bool _onOff, bool _isWrite)
	{
		DrawMDIIOButton(memDC, m_rectC[_enum], _text, m_eSelectControl == _enum, m_eMouseOver == _enum, _onOff, _isWrite);
	};

	//IOControl(eC_PAPER_CYLINDER_A_UP,		 g_st[IDS_MANUAL_AREA_A_UP		],	GetGlassTransferCylinder(TRUE, eCH_A, TRUE),		GetGlassTransferCylinder(TRUE, eCH_A));
	//IOControl(eC_PAPER_CYLINDER_A_DOWN,	 g_st[IDS_MANUAL_AREA_A_DOWN	],	GetGlassTransferCylinder(FALSE, eCH_A, TRUE),		GetGlassTransferCylinder(FALSE, eCH_A));
	//IOControl(eC_PAPER_CYLINDER_B_UP,		 g_st[IDS_MANUAL_AREA_B_UP		],	GetGlassTransferCylinder(TRUE, eCH_B, TRUE),		GetGlassTransferCylinder(TRUE, eCH_B));
	//IOControl(eC_PAPER_CYLINDER_B_DOWN,	 g_st[IDS_MANUAL_AREA_B_DOWN	],	GetGlassTransferCylinder(FALSE, eCH_B, TRUE),		GetGlassTransferCylinder(FALSE, eCH_B));
	//IOControl(eC_PAPER_CYLINDER_C_UP,		 g_st[IDS_MANUAL_AREA_C_UP		],	GetGlassTransferCylinder(TRUE, eCH_C, TRUE),		GetGlassTransferCylinder(TRUE, eCH_C));
	//IOControl(eC_PAPER_CYLINDER_C_DOWN,	 g_st[IDS_MANUAL_AREA_C_DOWN	],	GetGlassTransferCylinder(FALSE, eCH_C, TRUE),		GetGlassTransferCylinder(FALSE, eCH_C));
	//IOControl(eC_PAPER_CYLINDER_D_UP,		 g_st[IDS_MANUAL_AREA_D_UP		],	GetGlassTransferCylinder(TRUE, eCH_D, TRUE),		GetGlassTransferCylinder(TRUE, eCH_D));
	//IOControl(eC_PAPER_CYLINDER_D_DOWN,	 g_st[IDS_MANUAL_AREA_D_DOWN	],	GetGlassTransferCylinder(FALSE, eCH_D, TRUE),		GetGlassTransferCylinder(FALSE, eCH_D));
	//IOControl(eC_PAPER_CYLINDER_ALL_UP,	 g_st[IDS_MANUAL_AREA_ALL_UP	],	GetGlassTransferCylinder(TRUE, eCH_ALL, TRUE),		GetGlassTransferCylinder(TRUE, eCH_ALL));
	//IOControl(eC_PAPER_CYLINDER_ALL_DOWN,	 g_st[IDS_MANUAL_AREA_ALL_DOWN	],	GetGlassTransferCylinder(FALSE, eCH_ALL, TRUE),		GetGlassTransferCylinder(FALSE, eCH_ALL));
	IOControl(eC_PAPER_CYLINDER_PUSHER_UP,	 g_st[IDS_MANUAL_PUSHER_UP		],	GetGlassTransferCylinderPusher(TRUE, TRUE),			GetGlassTransferCylinderPusher(TRUE));
	IOControl(eC_PAPER_CYLINDER_PUSHER_DOWN, g_st[IDS_MANUAL_PUSHER_DOWN	],	GetGlassTransferCylinderPusher(FALSE, TRUE),		GetGlassTransferCylinderPusher(FALSE));	
	
	IOControl(eC_GLASS_VACUUM_ON,			g_st[IDS_MANUAL_GLASS_VACUUM_ON	],	GetGlassTransferVacuum(eMT_ORG_GLASS, TRUE, TRUE),	GetGlassTransferVacuum(eMT_ORG_GLASS, TRUE));
	IOControl(eC_GLASS_VACUUM_OFF,			g_st[IDS_MANUAL_GLASS_VACUUM_OFF],	GetGlassTransferVacuum(eMT_ORG_GLASS, FALSE, TRUE),	GetGlassTransferVacuum(eMT_ORG_GLASS, FALSE));
	//IOControl(eC_GLASS_BLOW_ON,			g_st[IDS_MANUAL_GLASS_BLOW_ON	],	GetGlassTransferBlow(eMT_ORG_GLASS, TRUE, TRUE),	GetGlassTransferBlow(eMT_ORG_GLASS, TRUE));
	//IOControl(eC_GLASS_BLOW_OFF,			g_st[IDS_MANUAL_GLASS_BLOW_OFF	],	GetGlassTransferBlow(eMT_ORG_GLASS, FALSE, TRUE),	GetGlassTransferBlow(eMT_ORG_GLASS, FALSE));
	IOControl(eC_PAPER_VACUUM_ON,			g_st[IDS_MANUAL_PAPER_VACUUM_ON	],	GetGlassTransferVacuum(eMT_PAPER, TRUE, TRUE),		GetGlassTransferVacuum(eMT_PAPER, TRUE));
	IOControl(eC_PAPER_VACUUM_OFF,			g_st[IDS_MANUAL_PAPER_VACUUM_OFF],	GetGlassTransferVacuum(eMT_PAPER, FALSE, TRUE),		GetGlassTransferVacuum(eMT_PAPER, FALSE));
	//IOControl(eC_PAPER_BLOW_ON,			g_st[IDS_MANUAL_PAPER_BLOW_ON	],	GetGlassTransferBlow(eMT_PAPER, TRUE, TRUE),		GetGlassTransferBlow(eMT_PAPER, TRUE));
	//IOControl(eC_PAPER_BLOW_OFF,			g_st[IDS_MANUAL_PAPER_BLOW_OFF	],	GetGlassTransferBlow(eMT_PAPER, FALSE, TRUE),		GetGlassTransferBlow(eMT_PAPER, FALSE));
	//IOControl(eC_PAPER_PUSHER_VACUUM_ON,	g_st[IDS_MANUAL_PAPER_PUSHER_VACUUM_ON	],	GetGlassTransferPaperPusherVacuum(TRUE, TRUE),		GetGlassTransferPaperPusherVacuum(TRUE));
	//IOControl(eC_PAPER_PUSHER_VACUUM_OFF,	g_st[IDS_MANUAL_PAPER_PUSHER_VACUUM_OFF	],	GetGlassTransferPaperPusherVacuum(FALSE, TRUE),		GetGlassTransferPaperPusherVacuum(FALSE));
	IOControl(eC_PAPER_AND_GLASS_BLOW_ON,	g_st[IDS_MANUAL_PAPER_GLASS_BLOW_ON],		GetGlassTransferBlow(TRUE, TRUE),					GetGlassTransferBlow(TRUE, TRUE));
	IOControl(eC_PAPER_AND_GLASS_BLOW_OFF,	g_st[IDS_MANUAL_PAPER_GLASS_BLOW_OFF],		GetGlassTransferBlow(FALSE, TRUE),					GetGlassTransferBlow(FALSE, TRUE));
	//DrawMDIMenuButtonN(&memDC, m_rectC[eC_LOAD_CENTER_POS]			, _T("Load\nCenter")		, GetTransferCenterPoint(eMODULE_PC1_LOAD_CONVEYOR_2), FALSE, DT_FONTSIZE, RGB_GRAY);
	//DrawMDIMenuButtonN(&memDC, m_rectC[eC_UNLOAD_PPBOX_CENTER_POS]	, _T("Unload PPBox\nCenter"), GetTransferCenterPoint(eMODULE_PC1_LOAD_CONVEYOR_2), FALSE, DT_FONTSIZE, RGB_GRAY);
	//DrawMDIMenuButtonN(&memDC, m_rectC[eC_UNLOAD_TABLE_CENTER_POS]	, _T("Unload Table\nCenter"), GetTransferCenterPoint(eMODULE_PC2_GLASS_SHIFT_TABLE), FALSE, DT_FONTSIZE, RGB_GRAY);
}



void CGlassLoadTransferWnd::OnMouseMove(UINT nFlags, CPoint point) {
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



void CGlassLoadTransferWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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

void CGlassLoadTransferWnd::OnLButtonUp(UINT nFlags, CPoint point)  {	
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		if (m_eSelectControl != eC_SAVE && m_eSelectControl != eC_LOCK) {
			//<< KES 추가 2024.4.9 - 운전모드가 Manual이 아니면 수동 동작 X
			if (sm.Option->tSequence.eDriveMode != eDRIVE_MODE_MANUAL) {
				IPMessage(g_st[IDS_MS_NOT_DRIVE_MODE_MANUAL]);
				return;
			}
			//>>	
		}

		switch (m_eSelectControl) {
			case eC_SAVE: {
				if (m_bLock) {
					SyncData(FALSE);
				}
				else 
				{
					//<< KES 추가 2024.07.21 - CIM 프로그램 꺼져 있으면 수정 불가
					if (!g_ipCIMClient.IsServerConnected() || !g_ipCIMClient.IsIPCommandLoopStarted())
					{
						IPMessage(g_st[IDS_MS_CIM_PROGRAM_IS_CLOSED]);
						return;
					}
					//>>

					std::vector<CString> ChangeLog;
					std::vector<std::pair<CString, COLORREF>> changeDatas;
					T_POS* pNew = (T_POS*)(&m_tGlassTransfer);
					T_POS* pOrg = (T_POS*)(&sm.MotorPos_1->Glass_TR);
					for (int iIndex = 0; iIndex < countof(GlassLoadTrasnfer); iIndex++)
					{
						auto& newParam = pNew[iIndex]; auto& orgParam = pOrg[iIndex];
						if (newParam != orgParam)
						{
							CString strName = g_st[IDS_POSITION_PARAMETER_GLASS_TRANSFER_WAIT_Y + iIndex];//GlassLoadTrasnfer[iIndex].strName;
							changeDatas.push_back({ Format(_T("[%s]"), strName), RGB_SKY_LIGHT });
							if (fabs(newParam.dTarget - orgParam.dTarget) >= 0.001)
							{
								changeDatas.push_back({ Format(_T("%s %.3f -> %.3f"), g_st[IDS_GLASS_LD_TRANSFER_POS], orgParam.dTarget, newParam.dTarget), RGB_WHITE });
								ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), strName, g_st[IDS_GLASS_LD_TRANSFER_POS], orgParam.dTarget, newParam.dTarget));
							}
							if (fabs(newParam.dSpeed - orgParam.dSpeed) >= 0.001)
							{
								changeDatas.push_back({ Format(_T("%s %.3f -> %.3f"), g_st[IDS_GLASS_LD_TRANSFER_SPEED], orgParam.dSpeed, newParam.dSpeed), RGB_WHITE });
								ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), strName, g_st[g_st[IDS_GLASS_LD_TRANSFER_SPEED]], orgParam.dSpeed, newParam.dSpeed));
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
						WriteParameterLog(g_st[IDS_GLASS_LD_TRANSFER_AXIS_PARAM], _T("Glass Load Transfer"), ChangeLog, theApp.m_UserManager.GetCurrentUserData().szID);
						SyncData(TRUE);
						m_bLock = TRUE;
						Update_ECID();
					}
				}
				strLog = _T("저장/불러오기");
				break;
			}
			case eC_LOCK: {
				if (!m_bLock || !sm.Sequence->fAutoMode || sm.Sequence->fCycleStop)
					m_bLock = !m_bLock;
				else
					IPMessage(g_st[IDS_GLASS_LD_TRANSFER_CAN_NOT_CHANGE_WHILE_AUTO_RUN]);
				strLog = _T("잠금/편집");
				break;
			}
			//case eC_LOAD_CENTER_POS	: {
			//	SetTransferCenterPoint(eMODULE_PC1_LOAD_CONVEYOR_2);
			//	strLog = _T("Load Center Move");
			//	break;
			//}		
			////case eC_UNLOAD_PPBOX_CENTER_POS	:{
			////	SetTransferCenterPoint(eMODULE_PC1_LOAD_CONVEYOR_2);
			////	strLog = _T("Unload PPBox Center Move");
			////	break;
			////}
			//case eC_UNLOAD_TABLE_CENTER_POS: {
			//	SetTransferCenterPoint(eMODULE_PC2_GLASS_SHIFT_TABLE);
			//	strLog = _T("Unload Table Center Move");
			//	break;
			//}
			//case eC_PAPER_CYLINDER_A_UP		:
			//case eC_PAPER_CYLINDER_A_DOWN	: {
			//	SetGlassTransferCylinder(m_eSelectControl == eC_PAPER_CYLINDER_A_UP, eCH_A);
			//	strLog = Format(_T("Area A Cylinder %s"), m_eSelectControl == eC_PAPER_CYLINDER_A_UP ? _T("Up") : _T("Down"));
			//	break;
			//}
			//case eC_PAPER_CYLINDER_B_UP			:
			//case eC_PAPER_CYLINDER_B_DOWN		:{
			//	SetGlassTransferCylinder(m_eSelectControl == eC_PAPER_CYLINDER_B_UP, eCH_B);
			//	strLog = Format(_T("Area B Cylinder %s"), m_eSelectControl == eC_PAPER_CYLINDER_B_UP ? _T("Up") : _T("Down"));
			//	break;
			//}
			//case eC_PAPER_CYLINDER_C_UP			:
			//case eC_PAPER_CYLINDER_C_DOWN		:{
			//	SetGlassTransferCylinder(m_eSelectControl == eC_PAPER_CYLINDER_C_UP, eCH_C);
			//	strLog = Format(_T("Area C Cylinder %s"), m_eSelectControl == eC_PAPER_CYLINDER_C_UP ? _T("Up") : _T("Down"));
			//	break;
			//}
			//case eC_PAPER_CYLINDER_D_UP			:
			//case eC_PAPER_CYLINDER_D_DOWN		:{
			//	SetGlassTransferCylinder(m_eSelectControl == eC_PAPER_CYLINDER_D_UP, eCH_D);
			//	strLog = Format(_T("Area D Cylinder %s"), m_eSelectControl == eC_PAPER_CYLINDER_D_UP ? _T("Up") : _T("Down"));
			//	break;
			//}
			//case eC_PAPER_CYLINDER_ALL_UP		:
			//case eC_PAPER_CYLINDER_ALL_DOWN		:{
			//	SetGlassTransferCylinder(m_eSelectControl == eC_PAPER_CYLINDER_ALL_UP, eCH_ALL);
			//	strLog = Format(_T("Area All Cylinder %s"), m_eSelectControl == eC_PAPER_CYLINDER_ALL_UP ? _T("Up") : _T("Down"));
			//	break;
			//}
			case eC_PAPER_CYLINDER_PUSHER_UP	:	
			case eC_PAPER_CYLINDER_PUSHER_DOWN	:{
				SetGlassTransferCylinderPusher(m_eSelectControl == eC_PAPER_CYLINDER_PUSHER_UP);
				strLog = Format(_T("Pusher Cylinder %s"), m_eSelectControl == eC_PAPER_CYLINDER_PUSHER_UP ? _T("Up") : _T("Down"));
				break;
			}
			case eC_GLASS_VACUUM_ON				:
			case eC_GLASS_VACUUM_OFF:{
				SetGlassTransferVacuum(eMT_ORG_GLASS, m_eSelectControl == eC_GLASS_VACUUM_ON);
				strLog = Format(_T("Glass Vacuum %s"), m_eSelectControl == eC_GLASS_VACUUM_ON ? _T("On") : _T("Off"));
				break;
			}
			case eC_PAPER_VACUUM_ON				:
			case eC_PAPER_VACUUM_OFF			:	{
				SetGlassTransferVacuum(eMT_PAPER, m_eSelectControl == eC_PAPER_VACUUM_ON);
				strLog = Format(_T("Paper Vacuum %s"), m_eSelectControl == eC_PAPER_VACUUM_ON ? _T("On") : _T("Off"));
				break;
			}
			case eC_PAPER_AND_GLASS_BLOW_ON:
			case eC_PAPER_AND_GLASS_BLOW_OFF: {
				SetGlassTransferBlow(m_eSelectControl == eC_PAPER_AND_GLASS_BLOW_ON);
				strLog = Format(_T("Paper and Glass Blow %s"), m_eSelectControl == eC_PAPER_AND_GLASS_BLOW_ON ? _T("On") : _T("Off"));
				break;
			}
			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T("Glass Load Transfer"), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	for (int i = 0; i < eC_END; i++)
		InvalidateRect(m_rectC[i], FALSE);
	m_eSelectControl = eC_NONE;


	CFrameWndEx::OnLButtonUp(nFlags, point);
}

void CGlassLoadTransferWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
	CRect		rect = m_rectS[eS_FULL_AREA];
	double dH = (rect.Height() - 5.) / 15.;
	double dW = (rect.Width() - 5.) / 20.; 

	m_rectS[eS_PARAMETER_LIST		] = CRect(rect.left + dW*0 +5, rect.top + dH*0 +5, rect.left + dW*10,	rect.top+dH*15);
	m_rectS[eS_MOTOR_CONTROL		] = CRect(rect.left + dW*10 +5, rect.top + dH*1 +5, rect.left + dW*20,	rect.top+dH*5);

	m_rectC[eC_LOCK					] = CRect(rect.left + dW*18 +5,	rect.top + dH*0 +5, rect.left + dW*19,	rect.top+dH*1);			
	m_rectC[eC_SAVE					] = CRect(rect.left + dW*19 +5,	rect.top + dH*0 +5, rect.left + dW*20,	rect.top+dH*1);
	m_rectS[eS_STATUS_AREA			] = CRect(rect.left + dW*10 +5, rect.top + dH*5 +15, rect.left + dW*20,	rect.top+dH*7);
	m_rectS[eS_IO_AREA				] = CRect(rect.left + dW*10 +5, rect.top + dH*7 +15, rect.left + dW*20,	rect.top+dH*15);

	rect = m_rectS[eS_STATUS_AREA];
	dW = (rect.Width() - 5.) / 3.; 
	dH = (rect.Height() - 5.) / 1.;
	m_rectC[eC_LOAD_CENTER_POS			] = CRect(rect.left + dW*0 +5,	rect.top + dH*0 +5, rect.left + dW*1,	rect.top+dH*1);	
	m_rectC[eC_UNLOAD_PPBOX_CENTER_POS	] = CRect(rect.left + dW*1 +5,	rect.top + dH*0 +5, rect.left + dW*2,	rect.top+dH*1);	
	m_rectC[eC_UNLOAD_TABLE_CENTER_POS	] = CRect(rect.left + dW*2 +5,	rect.top + dH*0 +5, rect.left + dW*3,	rect.top+dH*1);	
	
	rect = m_rectS[eS_IO_AREA];
	dW = (rect.Width() - 5.) / 6.; 
	dH = (rect.Height() - 5.) / 4.;

	//m_rectC[eC_PAPER_CYLINDER_A_UP			] = CRect(rect.left + dW*0 +5, rect.top + dH*0 + 10, rect.left + dW*1,	rect.top+dH*1);		
	//m_rectC[eC_PAPER_CYLINDER_A_DOWN		] = CRect(rect.left + dW*0 +5, rect.top + dH*1 + 10, rect.left + dW*1,	rect.top+dH*2);		
	//m_rectC[eC_PAPER_CYLINDER_B_UP			] = CRect(rect.left + dW*1 +5, rect.top + dH*0 + 10, rect.left + dW*2,	rect.top+dH*1);		
	//m_rectC[eC_PAPER_CYLINDER_B_DOWN		] = CRect(rect.left + dW*1 +5, rect.top + dH*1 + 10, rect.left + dW*2,	rect.top+dH*2);		
	//m_rectC[eC_PAPER_CYLINDER_C_UP			] = CRect(rect.left + dW*2 +5, rect.top + dH*0 + 10, rect.left + dW*3,	rect.top+dH*1);		
	//m_rectC[eC_PAPER_CYLINDER_C_DOWN		] = CRect(rect.left + dW*2 +5, rect.top + dH*1 + 10, rect.left + dW*3,	rect.top+dH*2);		
	//m_rectC[eC_PAPER_CYLINDER_D_UP			] = CRect(rect.left + dW*3 +5, rect.top + dH*0 + 10, rect.left + dW*4,	rect.top+dH*1);		
	//m_rectC[eC_PAPER_CYLINDER_D_DOWN		] = CRect(rect.left + dW*3 +5, rect.top + dH*1 + 10, rect.left + dW*4,	rect.top+dH*2);		
	//m_rectC[eC_PAPER_CYLINDER_ALL_UP		] = CRect(rect.left + dW*4 +5, rect.top + dH*0 + 10, rect.left + dW*5,	rect.top+dH*1);		
	//m_rectC[eC_PAPER_CYLINDER_ALL_DOWN		] = CRect(rect.left + dW*4 +5, rect.top + dH*1 + 10, rect.left + dW*5,	rect.top+dH*2);		
	m_rectC[eC_PAPER_CYLINDER_PUSHER_UP		] = CRect(rect.left + dW*5 +5, rect.top + dH*0 + 10, rect.left + dW*6,	rect.top+dH*1);		
	m_rectC[eC_PAPER_CYLINDER_PUSHER_DOWN	] = CRect(rect.left + dW*5 +5, rect.top + dH*1 + 10, rect.left + dW*6,	rect.top+dH*2);		
	
	m_rectC[eC_GLASS_VACUUM_ON				] = CRect(rect.left + dW*0 +5, rect.top + dH*0 + 10, rect.left + dW*1,	rect.top+dH*1);	
	m_rectC[eC_GLASS_VACUUM_OFF				] = CRect(rect.left + dW*0 +5, rect.top + dH*1 + 10, rect.left + dW*1,	rect.top+dH*2);	
	//m_rectC[eC_GLASS_BLOW_ON				] = CRect(rect.left + dW*1 +5, rect.top + dH*2 + 10, rect.left + dW*2,	rect.top+dH*3);	
	//m_rectC[eC_GLASS_BLOW_OFF				] = CRect(rect.left + dW*1 +5, rect.top + dH*3 + 10, rect.left + dW*2,	rect.top+dH*4);	
	m_rectC[eC_PAPER_VACUUM_ON				] = CRect(rect.left + dW*1 +5, rect.top + dH*0 + 10, rect.left + dW*2,	rect.top+dH*1);	
	m_rectC[eC_PAPER_VACUUM_OFF				] = CRect(rect.left + dW*1 +5, rect.top + dH*1 + 10, rect.left + dW*2,	rect.top+dH*2);	
	//m_rectC[eC_PAPER_BLOW_ON				] = CRect(rect.left + dW*3 +5, rect.top + dH*2 + 10, rect.left + dW*4,	rect.top+dH*3);	
	//m_rectC[eC_PAPER_BLOW_OFF				] = CRect(rect.left + dW*3 +5, rect.top + dH*3 + 10, rect.left + dW*4,	rect.top+dH*4);	
	m_rectC[eC_PAPER_AND_GLASS_BLOW_ON		] = CRect(rect.left + dW*2 +5, rect.top + dH*0 + 10, rect.left + dW*3,	rect.top+dH*1);	
	m_rectC[eC_PAPER_AND_GLASS_BLOW_OFF		] = CRect(rect.left + dW*2 +5, rect.top + dH*1 + 10, rect.left + dW*3,	rect.top+dH*2);	

	//m_rectC[eC_PAPER_PUSHER_VACUUM_ON		] = CRect(rect.left + dW*4 +5, rect.top + dH*2 + 10, rect.left + dW*5,	rect.top+dH*3);	
	//m_rectC[eC_PAPER_PUSHER_VACUUM_OFF		] = CRect(rect.left + dW*4 +5, rect.top + dH*3 + 10, rect.left + dW*5,	rect.top+dH*4);	
			
	m_lstTeaching.MoveWindow(m_rectS[eS_PARAMETER_LIST]);
}
void CGlassLoadTransferWnd::SyncData(BOOL bStore) {
	
	CProfile profile(_T("../cfg/GlassTransfer.ini"));
	T_POS* pPos = (T_POS*)&m_tGlassTransfer;
	for (int i = 0; i < countof(GlassLoadTrasnfer); i++) {
		pPos[i].iAxis = GlassLoadTrasnfer[i].eAxis;

		CString strSection;
		strSection.Format(GlassLoadTrasnfer[i].strName);
		profile.SyncProfileValue(bStore, strSection, _T("Target"),	pPos[i].dTarget);
		profile.SyncProfileValue(bStore, strSection, _T("Speed"),	pPos[i].dSpeed);

	}
	if (bStore) {
		profile.Save();
	}
	memcpy(&sm.MotorPos_1->Glass_TR, &m_tGlassTransfer, sizeof(T_GLASS_TRANSFER));
	SetListItem();
}

void CGlassLoadTransferWnd::OnLvnItemBeginAbleEdit(NMHDR* pNMHDR, LRESULT* pResult) {
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;

	if (m_bLock)
		*pResult = -1;

	if (sm.Option->tKeyPad.fShowKeyPad)		// Key Pad 가 켜져 있는 경우 동작하지 않도록 
	{
		*pResult = -1;
	}

	if (sm.Option->tKeyPad.fKeyPadUseMode && !m_bLock && !sm.Option->tKeyPad.fShowKeyPad) {
		int iItem = pDispInfo->item.iItem;
		int iSubItem = pDispInfo->item.iSubItem;
		T_POS* pPos = (T_POS*)&m_tGlassTransfer;
		//if (sm.MotorStatus[pPos[iItem].iAxis].tEtc.dwHomeStatus != eHOME_STATUS_COMPLETED) {
		//	IPMessage(_T("현재 선택된 모터의 초기화 동작이 완료되지 않았습니다"));
		//	SetListItem();
		//	return;
		//}
		if (iSubItem == eLI_POSITION) {
			g_pwndNumber->SetDataValue
			(&pPos[iItem].dTarget,
				[this]()
				{
					SetListItem();
				}
			);
			g_pwndNumber->ShowWindow(SW_SHOW);
		}
		else if (iSubItem == eLI_SPEED) {
			g_pwndNumber->SetDataValue
			(&pPos[iItem].dSpeed,
				[this]()
				{
					SetListItem();
				}
			);
			g_pwndNumber->ShowWindow(SW_SHOW);
		}
	}
}

void CGlassLoadTransferWnd ::OnLvnChange(NMHDR* pNMHDR, LRESULT* pResult) {

	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int iItem = (int)pNMIA->iItem;
	if ((pNMIA->uNewState & LVIS_SELECTED) && !(pNMIA->uOldState & LVIS_SELECTED)) {
		if (iItem < 0) {
			return;
		}
		T_POS* pPos = (T_POS*)&m_tGlassTransfer;
		m_pMotorControl->SetMotorInfo(pPos[iItem]);
	}
	//Invalidate(FALSE);
}
void CGlassLoadTransferWnd::SetListItem() {
	T_POS* pPos = (T_POS*)&m_tGlassTransfer;
	int nItem1 = sizeof(m_tGlassTransfer) / sizeof(pPos[0]);
	int nItem2 = countof(GlassLoadTrasnfer);

	int nItem = _min(nItem1, nItem2);
	for (int i = 0; i < nItem; i++) {
		m_lstTeaching.SetItemDouble(i, eLI_POSITION, pPos[i].dTarget);
		m_lstTeaching.SetItemDouble(i, eLI_SPEED, pPos[i].dSpeed);
	}
}

void CGlassLoadTransferWnd::OnLvnItemEndAbleEdit(NMHDR* pNMHDR, LRESULT* pResult) {
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;

	int iItem = pDispInfo->item.iItem;
	int iSubItem = pDispInfo->item.iSubItem;

	int nItem = sizeof(m_tGlassTransfer) / sizeof(T_POS);
	if (iItem < 0 || iItem >= nItem)
		return;

	T_POS* pPos = (T_POS*)&m_tGlassTransfer;
	//if (sm.MotorStatus[pPos[iItem].iAxis].tEtc.dwHomeStatus != eHOME_STATUS_COMPLETED){
	//	IPMessage(_T("현재 선택된 모터의 초기화 동작이 완료되지 않았습니다"));
	//	SetListItem();
	//	return;
	//}
	switch (iSubItem) {
		case eLI_POSITION: {
			if (!sm.Option->tKeyPad.fKeyPadUseMode)
				pPos[iItem].dTarget = _ttof(pDispInfo->item.pszText);
			break;
		}
		case eLI_SPEED: {
			if (!sm.Option->tKeyPad.fKeyPadUseMode)
				pPos[iItem].dSpeed = _ttof(pDispInfo->item.pszText);
			break;
		}
		case eLI_GET: {
			if (!m_bLock && !sm.Option->tKeyPad.fKeyPadUseMode)
				pPos[iItem].dTarget = sm.MotorStatus[pPos[iItem].iAxis].tCrt.dPos;
			else if (sm.Option->tKeyPad.fKeyPadUseMode) {
				IPMessage(g_st[IDS_GLASS_LD_TRANSFER_NOT_AVAILABLE_IN_KEY_PAD_USE_MODE]);
			}
			break;
		}
		//case eLI_MOVE: {
		//	if (!m_bLock && !sm.Option->tKeyPad.fShowKeyPad)
		//		PTPMove(pPos[iItem]);
		//	break;
		//}
	}
	if (iSubItem == eLI_GET && !m_bLock)
		SetListItem();
}
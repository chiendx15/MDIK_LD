#include "pch.h"
#include "UI.h"
#include "PPBox_EV.h"
#include "../MotorControl.h"

#define ID_POSITION_LIST	10002
#define INTERVAL_UI_GLASS_PPBOX 6650 

IMPLEMENT_DYNCREATE		(CPPboxWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CPPboxWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
	ON_NOTIFY(LVN_ITEMCHANGED,		ID_POSITION_LIST, &CPPboxWnd::OnLvnChange)
	ON_NOTIFY(LVN_BEGINLABELEDIT,	ID_POSITION_LIST, OnLvnItemBeginAbleEdit)
	ON_NOTIFY(LVN_ENDLABELEDIT,		ID_POSITION_LIST, OnLvnItemEndAbleEdit)
END_MESSAGE_MAP			()

CPPboxWnd::CPPboxWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	memset(&m_tPPBoxEV, 0x00, sizeof(T_ELEVATOR_CONVEYOR));
	m_eMouseOver		= eC_NONE;
	m_eSelectControl	= eC_NONE;
	m_bLock				= TRUE;
}

CPPboxWnd::~CPPboxWnd() {
}



BOOL CPPboxWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CPPboxWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
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

		for (int i = 0; i < countof(PPBoxEV); i++) {
			m_lstTeaching.InsertItem(i, _T(""));
			if (PPBoxEV[i].cr)
				m_lstTeaching.SetItemBKColor(i, PPBoxEV[i].cr);
			m_lstTeaching.SetItemText(i, eLI_POSITION_NAME, g_st[IDS_POSITION_PARAMETER_PPBOX_LOAD_Z + i]);
			m_lstTeaching.SetItemInt(i, eLI_AXIS, PPBoxEV[i].eAxis);
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
void CPPboxWnd::ChangeLanguage()
{
	for (int i = 0; i < countof(PPBoxEV); i++) {
		m_lstTeaching.SetItemText(i, eLI_POSITION_NAME, g_st[IDS_POSITION_PARAMETER_PPBOX_LOAD_Z + i]);
	}

	Invalidate(FALSE);
}

void CPPboxWnd::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case INTERVAL_UI_GLASS_PPBOX: {
			InvalidateRect(m_rectS[eS_CONTROL_AREA]);
			break;
		}
	}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CPPboxWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		SyncData(FALSE);
		SetListItem();
		m_bLock = TRUE;
		Invalidate();
		T_POS* pPos = (T_POS*)&m_tPPBoxEV;
		m_pMotorControl->SetMotorInfo(pPos[0]);
		m_pMotorControl->MoveWindow(m_rectS[eS_MOTOR_CONTROL]);
		m_pMotorControl->ShowWindow(SW_SHOW);
		m_lstTeaching.SetMark(0);	// KES 추가 2024.5.1
		SetTimer(INTERVAL_UI_GLASS_PPBOX, 100, nullptr);
	}
	else {
		KillTimer(INTERVAL_UI_GLASS_PPBOX);
	}
}


BOOL CPPboxWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CPPboxWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CRect				rect = m_rectS[eS_FULL_AREA];
	memDC.FillSolidRect(rect, RgbColor::AliceBlue);

	DrawGroupBox(&memDC, m_rectS[eS_CONTROL_AREA], _T("Control"), RGB_SKY, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_ALIGN_AREA], _T("Align"), RGB_SKY, RGB_BLACK);

	DrawMDIMenuButton(memDC, m_rectC[eC_LOCK], m_bLock ? g_st[IDS_PARAMETER_LOCK] : g_st[IDS_PARAMETER_EDIT], m_eSelectControl == eC_LOCK || m_bLock, m_eMouseOver == eC_LOCK);
	DrawMDIMenuButton(memDC, m_rectC[eC_SAVE], g_st[IDS_PARAMETER_SAVE], m_eSelectControl == eC_SAVE, m_eMouseOver == eC_SAVE);
	
	auto IOControl = [&](const int _enum, const CString& _text, bool _onOff, bool _isWrite)
	{
		DrawMDIIOButton(memDC, m_rectC[_enum], _text, m_eSelectControl == _enum, m_eMouseOver == _enum, _onOff, _isWrite);
	};

	IOControl(eC_PPBOX_ALIGN_X_L_FWD	, _T("Aligner X_L FWD")				, GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_X_L),	GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_X_L));
	IOControl(eC_PPBOX_ALIGN_X_L_BWD	, _T("Aligner X_L BWD")				, GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_X_L),	GetPPBoxAlignerXY(TRUE,  TRUE,  ePPBOX_ALIGNER_X_L));
	IOControl(eC_PPBOX_ALIGN_X_R_FWD	, _T("Aligner X_R FWD")				, GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_X_R),	GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_X_R));
	IOControl(eC_PPBOX_ALIGN_X_R_BWD	, _T("Aligner X_R BWD")				, GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_X_R),	GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_X_R));
	IOControl(eC_PPBOX_ALIGN_Y_FWD		, _T("Aligner Y FWD")				, GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_Y),		GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_Y));
	IOControl(eC_PPBOX_ALIGN_Y_BWD		, _T("Aligner Y BWD")				, GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_Y),		GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_Y));
	IOControl(eC_PPBOX_ALIGN_Z_UP		, _T("Aligner Y Up")				, GetPPBoxAlignerZ(TRUE, TRUE),								GetPPBoxAlignerZ(FALSE, TRUE));
	IOControl(eC_PPBOX_ALIGN_Z_DOWN		, _T("Aligner Y Down")				, GetPPBoxAlignerZ(FALSE, TRUE),							GetPPBoxAlignerZ(TRUE,	TRUE));
	IOControl(eC_PPBOX_ALIGN_ALL_FWD	, _T("Aligner All FWD")				, GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_ALL),	GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_ALL));
	IOControl(eC_PPBOX_ALIGN_ALL_BWD	, _T("Aligner All BWD")	            , GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_ALL),	GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_ALL));	

	IOControl(eC_PPBOX_STOPPER_1_UP		, _T("CV1 Stopper Up")	            , GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1,	TRUE,  TRUE),	GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1,		FALSE, TRUE));
	IOControl(eC_PPBOX_STOPPER_1_DOWN	, _T("CV1 Stopper Down")	        , GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1,	FALSE, TRUE),	GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1,		TRUE,  TRUE));
	IOControl(eC_PPBOX_STOPPER_2_UP		, _T("CV2 Stopper Up")	            , GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_2,	TRUE,  TRUE),	GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_2,		FALSE, TRUE));
	IOControl(eC_PPBOX_STOPPER_2_DOWN	, _T("CV2 Stopper Down")	        , GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_2,	FALSE, TRUE),	GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_2,		TRUE,  TRUE));
	IOControl(eC_PPBOX_STOPPER_3_UP		, _T("CV3 Stopper Up")	            , GetConveyor_Stoper(eMODULE_PC1_ELEVATOR_CONVEYOR, TRUE,  TRUE),	GetConveyor_Stoper(eMODULE_PC1_ELEVATOR_CONVEYOR,	FALSE, TRUE));
	IOControl(eC_PPBOX_STOPPER_3_DOWN	, _T("CV3 Stopper Down")	        , GetConveyor_Stoper(eMODULE_PC1_ELEVATOR_CONVEYOR, FALSE, TRUE),	GetConveyor_Stoper(eMODULE_PC1_ELEVATOR_CONVEYOR,	TRUE,  TRUE));
	IOControl(eC_PPBOX_STOPPER_4_UP		, _T("CV4 Stopper Up")	            , GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1, TRUE,  TRUE),	GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1,	FALSE, TRUE));
	IOControl(eC_PPBOX_STOPPER_4_DOWN	, _T("CV4 Stopper Down")	        , GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1, FALSE, TRUE),   GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1,	TRUE,  TRUE));
	IOControl(eC_PPBOX_STOPPER_5_UP		, _T("CV5 Stopper Up")	            , GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_2, TRUE,  TRUE),	GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_2,	FALSE, TRUE));
	IOControl(eC_PPBOX_STOPPER_5_DOWN	, _T("CV5 Stopper Down")	        , GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_2, FALSE, TRUE),	GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_2,	TRUE,  TRUE));
    
	//IOControl(eC_CV4_IN, g_st[IDS_MANUAL_CV4_IN], GetGlassBoxCV4InOutCylinder(TRUE, TRUE), GetGlassBoxCV4InOutCylinder(TRUE));
	//IOControl(eC_CV4_OUT, g_st[IDS_MANUAL_CV4_OUT], GetGlassBoxCV4InOutCylinder(FALSE, TRUE), GetGlassBoxCV4InOutCylinder(FALSE));
	//IOControl(eC_CV4_UP, g_st[IDS_MANUAL_CV4_UP], GetGlassBoxCV4UpDownCylinder(TRUE, TRUE), GetGlassBoxCV4UpDownCylinder(TRUE));
	//IOControl(eC_CV4_DOWN, g_st[IDS_MANUAL_CV4_DOWN], GetGlassBoxCV4UpDownCylinder(FALSE, TRUE), GetGlassBoxCV4UpDownCylinder(FALSE));
}



void CPPboxWnd::OnMouseMove(UINT nFlags, CPoint point) {
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



void CPPboxWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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

void CPPboxWnd::OnLButtonUp(UINT nFlags, CPoint point)  {	
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
					T_POS* pNew = (T_POS*)(&m_tPPBoxEV);
					T_POS* pOrg = (T_POS*)(&sm.MotorPos_1->EL_Convryor);
					for (int iIndex = 0; iIndex < countof(PPBoxEV); iIndex++)
					{
						auto& newParam = pNew[iIndex]; auto& orgParam = pOrg[iIndex];
						if (newParam != orgParam)
						{
							CString strName = g_st[IDS_POSITION_PARAMETER_GLASS_LOAD_PPOBOX_EV_WAIT_T + iIndex];// GlassLoadPPBoxEV[iIndex].strName;
							CString strPos = g_st[IDS_GLASS_LD_TRANSFER_POS];
							CString strSpeed = g_st[IDS_GLASS_LD_TRANSFER_SPEED];
							changeDatas.push_back({ Format(_T("[%s]"), strName), RGB_SKY_LIGHT });
							if (fabs(newParam.dTarget - orgParam.dTarget) >= 0.001)
							{
								changeDatas.push_back({ Format(_T("%s %.3f -> %.3f"), strPos,orgParam.dTarget, newParam.dTarget), RGB_WHITE });
								ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"),strName, strPos, orgParam.dTarget, newParam.dTarget));
							}
							if (fabs(newParam.dSpeed - orgParam.dSpeed) >= 0.001)
							{
								changeDatas.push_back({ Format(_T("%s %.3f -> %.3f"), strSpeed,orgParam.dSpeed, newParam.dSpeed), RGB_WHITE });
								ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), strName, strSpeed, orgParam.dSpeed, newParam.dSpeed));
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
						WriteParameterLog(_T("축 파라미터"), _T("Glass Load PPBox EV"), ChangeLog, theApp.m_UserManager.GetCurrentUserData().szID);
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

			case eC_PPBOX_ALIGN_X_L_FWD:
		    case eC_PPBOX_ALIGN_X_L_BWD: {
		    	SetPPBoxAlignerXY(m_eSelectControl == eC_PPBOX_ALIGN_X_L_FWD, ePPBOX_ALIGNER_X_L);
		    	strLog = Format(_T("PPBOX Align X_L%s"), m_eSelectControl == eC_PPBOX_ALIGN_X_L_FWD ? _T("FWD") : _T("BWD"));
		    	break;
		    }

		    case eC_PPBOX_ALIGN_X_R_FWD:
		    case eC_PPBOX_ALIGN_X_R_BWD: {
				SetPPBoxAlignerXY(m_eSelectControl == eC_PPBOX_ALIGN_X_R_FWD, ePPBOX_ALIGNER_X_R);
		    	strLog = Format(_T("PPBOX Align X_R%s"), m_eSelectControl == eC_PPBOX_ALIGN_X_R_FWD ? _T("FWD") : _T("BWD"));
		    	break;
		    }

		    case eC_PPBOX_ALIGN_Y_FWD:
		    case eC_PPBOX_ALIGN_Y_BWD: {
		    	SetPPBoxAlignerXY(m_eSelectControl == eC_PPBOX_ALIGN_Y_FWD, ePPBOX_ALIGNER_Y);
		    	strLog = Format(_T("PPBOX Align Y%s"), m_eSelectControl == eC_PPBOX_ALIGN_Y_FWD ? _T("FWD") : _T("BWD"));
		    	break;
		    }

		    case eC_PPBOX_ALIGN_ALL_FWD:
		    case eC_PPBOX_ALIGN_ALL_BWD: {
		    	SetPPBoxAlignerXY(m_eSelectControl == eC_PPBOX_ALIGN_ALL_FWD, ePPBOX_ALIGNER_ALL);
		    	strLog = Format(_T("PPBOX Align %s"), m_eSelectControl == eC_PPBOX_ALIGN_ALL_FWD ? _T("FWD") : _T("BWD"));
		    	break;
		    }

			case eC_PPBOX_ALIGN_Z_UP: {
				if (!GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_Y)) {
					IPMessage(g_st[IDS_MANUAL_DO_NOT_UP_PLEASE_FWD]);
				}
				else {
					SetPPBoxAlignerZ(TRUE);
					strLog = _T("PPBOX Align Y UP");
				}
				break;

			}

		    case eC_PPBOX_ALIGN_Z_DOWN: {
				if (!GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_Y)) {
					IPMessage(g_st[IDS_MANUAL_DO_NOT_DOWN_PLEASE_FWD]);
				}
				else {
		    		SetPPBoxAlignerZ(FALSE);
					strLog = _T("PPBOX Align Y DOWN");
				}
		    	break;
		    }

		    case eC_PPBOX_STOPPER_1_UP:
		    case eC_PPBOX_STOPPER_1_DOWN: {
		    	SetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1, m_eSelectControl == eC_PPBOX_STOPPER_1_UP);
		    	strLog = Format(_T("PPBOX Stopper %s"), m_eSelectControl == eC_PPBOX_STOPPER_1_UP ? _T("Up") : _T("Down"));
		    	break;
		    }

		    case eC_PPBOX_STOPPER_2_UP:
		    case eC_PPBOX_STOPPER_2_DOWN: {
		    	SetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_2, m_eSelectControl == eC_PPBOX_STOPPER_2_UP);
		    	strLog = Format(_T("PPBOX Stopper %s"), m_eSelectControl == eC_PPBOX_STOPPER_2_UP ? _T("Up") : _T("Down"));
		    	break;
		    }

		    case eC_PPBOX_STOPPER_3_UP:
		    case eC_PPBOX_STOPPER_3_DOWN: {
		    	SetConveyor_Stoper(eMODULE_PC1_ELEVATOR_CONVEYOR, m_eSelectControl == eC_PPBOX_STOPPER_3_UP);
		    	strLog = Format(_T("PPBOX Stopper %s"), m_eSelectControl == eC_PPBOX_STOPPER_3_UP ? _T("Up") : _T("Down"));
		    	break;
		    }

		    case eC_PPBOX_STOPPER_4_UP:
		    case eC_PPBOX_STOPPER_4_DOWN: {
		    	SetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1, m_eSelectControl == eC_PPBOX_STOPPER_4_UP);
		    	strLog = Format(_T("PPBOX Stopper %s"), m_eSelectControl == eC_PPBOX_STOPPER_4_UP ? _T("Up") : _T("Down"));
		    	break;
		    }

		    case eC_PPBOX_STOPPER_5_UP:
		    case eC_PPBOX_STOPPER_5_DOWN: {
		    	SetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_2, m_eSelectControl == eC_PPBOX_STOPPER_5_UP);
		    	strLog = Format(_T("PPBOX Stopper %s"), m_eSelectControl == eC_PPBOX_STOPPER_5_UP ? _T("Up") : _T("Down"));
		    	break;
			}
			
			//case eC_CV4_IN		: 	
			//case eC_CV4_OUT: {
			//	SetGlassBoxCV4InOutCylinder(m_eSelectControl == eC_CV4_IN);
			//	strLog = Format(_T("CV4 %s"), m_eSelectControl == eC_CV4_IN ? _T("In") : _T("Out"));
			//	break;
			//}
			//case eC_CV4_UP		: 	
			//case eC_CV4_DOWN	: 	{
			//	SetGlassBoxCV4UpDownCylinder(m_eSelectControl == eC_CV4_UP);
			//	strLog = Format(_T("CV4 %s"), m_eSelectControl == eC_CV4_UP ? _T("Up") : _T("Down"));
			//	break;
			//}
			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T("Glass Load PPBox EV"), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	for (int i = 0; i < eC_END; i++)
		InvalidateRect(m_rectC[i], FALSE);
	m_eSelectControl = eC_NONE;


	CFrameWndEx::OnLButtonUp(nFlags, point);
}

void CPPboxWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
	CRect		rect = m_rectS[eS_FULL_AREA];
	double dH = (rect.Height() - 5.) / 15.;
	double dW = (rect.Width() - 5.) / 20.; 

	m_rectS[eS_PARAMETER_LIST		] = CRect(rect.left + dW*0 +5, rect.top + dH*0 +5, rect.left + dW*10,	rect.top+dH*15);
	
	m_rectS[eS_MOTOR_CONTROL		] = CRect(rect.left + dW*10 +5, rect.top + dH*1 +5, rect.left + dW*20,	rect.top+dH*5);

	m_rectC[eC_LOCK					] = CRect(rect.left + dW*18 +5,	rect.top + dH*0 +5, rect.left + dW*19,	rect.top+dH*1);			
	m_rectC[eC_SAVE					] = CRect(rect.left + dW*19 +5,	rect.top + dH*0 +5, rect.left + dW*20,	rect.top+dH*1);
	m_rectS[eS_ALIGN_AREA			] = CRect(rect.left + dW*10 +5, rect.top + dH*5 +10, rect.left + dW*20,	rect.top+dH*9);		
	m_rectS[eS_CONTROL_AREA			] = CRect(rect.left + dW*10 +5, rect.top + dH*9 +15, rect.left + dW*20,	rect.top+dH*15);
	

	rect = m_rectS[eS_ALIGN_AREA];
	dH = (rect.Height() - 5.) / 2.;
	dW = (rect.Width() - 5.) / 5.; 
	
	m_rectC[eC_PPBOX_ALIGN_X_L_FWD		]  = CRect(rect.left + dW*0 + 5, rect.top + dH*0 + 8,	rect.left + dW*1,	rect.top+dH*1);					
	m_rectC[eC_PPBOX_ALIGN_X_L_BWD		]  = CRect(rect.left + dW*0 + 5, rect.top + dH*1 + 5,	rect.left + dW*1,	rect.top+dH*2);					
	m_rectC[eC_PPBOX_ALIGN_X_R_FWD		]  = CRect(rect.left + dW*1 + 5, rect.top + dH*0 + 8,	rect.left + dW*2,	rect.top+dH*1);					
	m_rectC[eC_PPBOX_ALIGN_X_R_BWD		]  = CRect(rect.left + dW*1 + 5, rect.top + dH*1 + 5,	rect.left + dW*2,	rect.top+dH*2);					
	m_rectC[eC_PPBOX_ALIGN_Y_FWD		]  = CRect(rect.left + dW*2 + 5, rect.top + dH*0 + 8,	rect.left + dW*3,	rect.top+dH*1);					
	m_rectC[eC_PPBOX_ALIGN_Y_BWD		]  = CRect(rect.left + dW*2 + 5, rect.top + dH*1 + 5,	rect.left + dW*3,	rect.top+dH*2);					
	m_rectC[eC_PPBOX_ALIGN_Z_UP			]  = CRect(rect.left + dW*3 + 5, rect.top + dH*0 + 8,	rect.left + dW*4,	rect.top+dH*1);					
	m_rectC[eC_PPBOX_ALIGN_Z_DOWN		]  = CRect(rect.left + dW*3 + 5, rect.top + dH*1 + 5,	rect.left + dW*4,	rect.top+dH*2);					
	m_rectC[eC_PPBOX_ALIGN_ALL_FWD		]  = CRect(rect.left + dW*4 + 5, rect.top + dH*0 + 8,	rect.left + dW*5,	rect.top+dH*1);
	m_rectC[eC_PPBOX_ALIGN_ALL_BWD		]  = CRect(rect.left + dW*4 + 5, rect.top + dH*1 + 5,	rect.left + dW*5,	rect.top+dH*2);


	rect = m_rectS[eS_CONTROL_AREA];
	dW = (rect.Width() - 5.) / 5.; 
	dH = (rect.Height() - 5.) / 3.;
	
	m_rectC[eC_PPBOX_STOPPER_1_UP		]	= CRect(rect.left + dW*0 + 5, rect.top + dH*0 + 8, rect.left + dW*1,	rect.top+dH*1);
	m_rectC[eC_PPBOX_STOPPER_1_DOWN		]	= CRect(rect.left + dW*0 + 5, rect.top + dH*1 + 5, rect.left + dW*1,	rect.top+dH*2);
	m_rectC[eC_PPBOX_STOPPER_2_UP		]	= CRect(rect.left + dW*1 + 5, rect.top + dH*0 + 8, rect.left + dW*2,	rect.top+dH*1);
	m_rectC[eC_PPBOX_STOPPER_2_DOWN		]	= CRect(rect.left + dW*1 + 5, rect.top + dH*1 + 5, rect.left + dW*2,	rect.top+dH*2);
	m_rectC[eC_PPBOX_STOPPER_3_UP		]	= CRect(rect.left + dW*2 + 5, rect.top + dH*0 + 8, rect.left + dW*3,	rect.top+dH*1);
	m_rectC[eC_PPBOX_STOPPER_3_DOWN		]	= CRect(rect.left + dW*2 + 5, rect.top + dH*1 + 5, rect.left + dW*3,	rect.top+dH*2);
	m_rectC[eC_PPBOX_STOPPER_4_UP		]	= CRect(rect.left + dW*3 + 5, rect.top + dH*0 + 8, rect.left + dW*4,	rect.top+dH*1);
	m_rectC[eC_PPBOX_STOPPER_4_DOWN		]	= CRect(rect.left + dW*3 + 5, rect.top + dH*1 + 5, rect.left + dW*4,	rect.top+dH*2);
	m_rectC[eC_PPBOX_STOPPER_5_UP		]	= CRect(rect.left + dW*4 + 5, rect.top + dH*0 + 8, rect.left + dW*5,	rect.top+dH*1);
	m_rectC[eC_PPBOX_STOPPER_5_DOWN		]	= CRect(rect.left + dW*4 + 5, rect.top + dH*1 + 5, rect.left + dW*5,	rect.top+dH*2);
	
	m_lstTeaching.MoveWindow(m_rectS[eS_PARAMETER_LIST]);
}
void CPPboxWnd::SyncData(BOOL bStore) {

	CProfile profile(_T("../cfg/GlassLoadPPBoxEv.ini"));
	T_POS* pPos = (T_POS*)&m_tPPBoxEV;

	for (int i = 0; i < countof(PPBoxEV); i++) {
		pPos[i].iAxis = PPBoxEV[i].eAxis;

		CString strSection;
		strSection.Format(PPBoxEV[i].strName);
		profile.SyncProfileValue(bStore, strSection, _T("Target"), pPos[i].dTarget);
		profile.SyncProfileValue(bStore, strSection, _T("Speed"), pPos[i].dSpeed);
	}
	if (bStore) {
		profile.Save();
	}
	memcpy(&sm.MotorPos_1->EL_Convryor, &m_tPPBoxEV, sizeof(T_ELEVATOR_CONVEYOR));
	SetListItem();
}

void CPPboxWnd::OnLvnItemBeginAbleEdit(NMHDR* pNMHDR, LRESULT* pResult) {
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
		T_POS* pPos = (T_POS*)&m_tPPBoxEV;
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

void CPPboxWnd::OnLvnChange(NMHDR* pNMHDR, LRESULT* pResult) {

	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int iItem = (int)pNMIA->iItem;
	if ((pNMIA->uNewState & LVIS_SELECTED) && !(pNMIA->uOldState & LVIS_SELECTED)) {
		if (iItem < 0) {
			return;
		}
		T_POS* pPos = (T_POS*)&m_tPPBoxEV;
		m_pMotorControl->SetMotorInfo(pPos[iItem]);
	}
	//Invalidate(FALSE);
}
void CPPboxWnd::SetListItem() {
	T_POS* pPos = (T_POS*)&m_tPPBoxEV;
	int nItem = sizeof(m_tPPBoxEV) / sizeof(pPos[0]);

	for (int i = 0; i < nItem; i++) {
		m_lstTeaching.SetItemDouble(i, eLI_POSITION, pPos[i].dTarget);
		m_lstTeaching.SetItemDouble(i, eLI_SPEED, pPos[i].dSpeed);
	}
}

void CPPboxWnd::OnLvnItemEndAbleEdit(NMHDR* pNMHDR, LRESULT* pResult) {
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;

	int iItem = pDispInfo->item.iItem;
	int iSubItem = pDispInfo->item.iSubItem;

	int nItem = sizeof(m_tPPBoxEV) / sizeof(T_POS);
	if (iItem < 0 || iItem >= nItem)
		return;
	T_POS* pPos = (T_POS*)&m_tPPBoxEV;
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
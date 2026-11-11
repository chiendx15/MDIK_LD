#include "pch.h"
#include "framework.h"
#include "UI.h"

#include	"MainFrame.h"
#include	"UserInterface/Main/TitleWnd.h"
#include	"UserInterface/Main/MainMenuWnd.h"
#include	"UserInterface/Main/SubMenuWnd.h"

#include	"UserInterface/Main/ExitWnd.h"
#include	"UserInterface/Main/LogInWnd.h"

#include	"UserInterface/Main/MainStatusWnd.h"

#include	"UserInterface/Menu/Manual/TeachModeConveyorWnd.h"

#include	"UserInterface/Menu/Recipe/RecipeEditWnd.h"
#include	"UserInterface/Menu/Recipe/RecipeSelectWnd.h"

#include	"UserInterface/Main/PPBoxWnd.h"

#include	"UserInterface/Menu/Maintenance/IOStatusWnd.h"
#include	"UserInterface/Menu/Maintenance/MCMonitorWnd.h"
#include	"UserInterface/Menu/Maintenance/DustCollectorWnd.h"
#include	"UserInterface/Menu/Maintenance/AirPressureWnd.h"
#include	"UserInterface/Menu/Maintenance/MCCWnd.h"
#include	"UserInterface/Menu/Maintenance/SequenceStepWnd.h"
#include	"UserInterface/Menu/Maintenance/RegisterViewer.h"
#include	"UserInterface/Menu/Maintenance/AGVPIOStatusWnd.h"
#include	"UserInterface/Menu/Maintenance/ConveyorMonitorWnd.h"

#include	"UserInterface/Menu/Parameter/DeviceParameterWnd.h"
#include	"UserInterface/Menu/Parameter/MotorParameterWnd.h"
#include	"UserInterface/Menu/Parameter/ConveyorParameterWnd.h"
#include	"UserInterface/Menu/Parameter/PositionParameterWnd.h"

#include	"UserInterface/Main/Simulator/DemoWnd.h"
#include	"UserInterface/Menu/Maintenance/AlarmLogWnd.h"
#include	"UserInterface/Menu/Maintenance/ParameterChangeLogWnd.h"
#include	"UserInterface/Menu/Maintenance/RecipeChangeLogWnd.h"
#include	"UserInterface/Menu/Maintenance/ButtonLogWnd.h"
#include	"UserInterface/Menu/Maintenance/GlassEmissionsWnd.h"

#include	"UserInterface/Main/MainStatus/NumericKeypadWnd.h"
#include	"UserInterface/Menu/Manual/TransferThetaMoveWnd.h"
#include	"UserInterface/Menu/Manual/IOControl.h"
#include	"UserInterface/Menu/Manual/TeachMoveWnd.h"
#include	"UserInterface/Menu/Manual/CIMTestWnd.h"

#include	"UserInterface/Main/PMWnd.h"

#include	"UserInterface/Main/TerminalMessageWnd.h"
#include	"UserInterface/Main/InterlockMessageWnd.h"
#include	"UserInterface/Main/OPCallMessageWnd.h"

#include	"IPCommand/IPCommandWnd.h"
#include	"IPCommand/CIMIPCommandWnd.h"
#include	"UserInterface/Menu/Manual/PPBOXCodeReaderWnd.h"

#include	"UserInterface/Menu/User/UserManagerWnd.h"
#ifdef _DEBUG
#	define new DEBUG_NEW
#endif
CNumericKeypadWnd* g_pwndNumber = nullptr;

// CMainFrame
const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

CMainFrame::CMainFrame() noexcept {
		
	m_eIitializeStep				= 0;

	m_hwndWorkArea					= nullptr;
	m_hwndSubMenu					= nullptr;

	m_pwndTitle						= nullptr;
	m_pwndMainMenu					= nullptr;
	m_pwndSubMenu					= nullptr;
	m_pwndExit						= nullptr;
	m_pwndLogIn						= nullptr;
	
	//<< Main Menu 1
	m_pwndMainStatus				= nullptr;
	//>>

	//<< Main Menu 2
	m_pwndTransferThetaMove			= nullptr;
	m_pwndIOControl					= nullptr;
	m_pwndTeachModeConveyorWnd		= nullptr;
	m_pwndPPBOXCodeReader			= nullptr;
	m_pwndTeachMoveWnd				= nullptr;
	m_pwndAGVPIOStatus				= nullptr;
	m_pwndConveyorMonitor			= nullptr;
	m_pwndCIMTester					= nullptr;
	//>>
	
	//<< Main Menu 3
	m_pwndPPBox						= nullptr;
	//>>

	//<< Main Menu 4
	m_pwndRecipeSelect				= nullptr;
	m_pwndRecipeEdit				= nullptr;
	m_pwndRecipeImageSet			= nullptr;
	//>>

	//<< Main Menu 5
	m_pwndMCMonitor					= nullptr;
	m_pwndDustCollector				= nullptr;
	m_pwndIOStatus					= nullptr;
	//m_pwndAirPressure				= nullptr;
	m_pwndMCC						= nullptr;
	m_pwndRegisterViewer			= nullptr;
	m_pwndGlassEmissions			= nullptr;
	//>>

	//<< Main Menu 6
	m_pwndDeviceParameter			= nullptr;
	m_pwndMotorParameter			= nullptr;
	m_pwndConveyorParameter			= nullptr;
	m_pwndPositionParameter			= nullptr;
	//>>

	//<< Main Menu 7
	m_pwndAlarmLog					= nullptr;
	m_pwndParameterLog				= nullptr;
	m_pwndRecipeLog					= nullptr;
	m_pwndButtonLog					= nullptr;
	m_pwndSequence					= nullptr;
	//>>

	//<< Main Menu 8
	m_pwndPMMode					= nullptr;
	//>>

	//<< Main Menu 9
	m_pwndUserManager				= nullptr;
	//>>

	// IPCommand 
	m_pwndIPCommand					= nullptr;
	m_pwndCIMIPCommand				= nullptr;

}

CMainFrame::~CMainFrame() {
}


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CMainFrame 진단
#ifdef _DEBUG
void CMainFrame::AssertValid() const			{ CFrameWndEx::AssertValid();	}
void CMainFrame::Dump(CDumpContext& dc) const	{ CFrameWndEx::Dump(dc);		}
#endif //_DEBUG


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {
	
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE; 
	
	//cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
	//cs.style = WS_POPUP;

	CRect		rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
	
	cs.dwExStyle	&= ~WS_EX_CLIENTEDGE;
	cs.lpszClass	= AfxRegisterWndClass(0);
	cs.x			= rect.left;
	cs.y			= rect.top;
	int cx			= 1920;
	int cy			= 1080;
	cs.cx			= (rect.Width() > cx) ? cx : rect.Width();
	cs.cy			= (rect.Height() > cy) ? cy : rect.Height();

	return TRUE;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
	
	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	return TRUE;
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	//::SetWindowLong(m_hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP | WS_SYSMENU);
	::SetMenu(m_hWnd, NULL);

	//sm.SystemParameter->eUserGrade = eUSER_SUPER;

	g_pwndNumber = new CNumericKeypadWnd();
	if (!g_pwndNumber->Create(NULL, NULL, NULL))		return -1;


	//<< 큰 틀
	m_pwndTitle			= new CTitleWnd(this);
	if (!m_pwndTitle->Create			(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	m_pwndMainMenu		= new CMainMenuWnd(this);
	if (!m_pwndMainMenu->Create			(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	m_pwndSubMenu		= new CSubMenuWnd(this);
	if (!m_pwndSubMenu->Create			(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	m_pwndExit			= new CExitWnd(this);
	if (!m_pwndExit->Create				(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	m_pwndLogIn			= new CLogInWnd(this);
	if (!m_pwndLogIn->Create			(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	//>>	

	//<< Main Menu 1
	m_pwndMainStatus	= new CMainStatusWnd(this);
	if (!m_pwndMainStatus->Create		(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	//>>
	
	//<< Main Menu 2
	m_pwndTransferThetaMove = new CTransferThetaMoveWnd(this);
	if (!m_pwndTransferThetaMove->Create(NULL, NULL, NULL))		return -1;
	
	m_pwndIOControl = new CIOControl(this);
	if (!m_pwndIOControl->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	
	m_pwndPPBOXCodeReader = new CPPBOXCodeReaderWnd(this);
	if (!m_pwndPPBOXCodeReader->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndTeachModeConveyorWnd = new CTeachModeConveyorWnd(this);
	if (!m_pwndTeachModeConveyorWnd->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndTeachMoveWnd = new CTeachMoveWnd(this);
	if (!m_pwndTeachMoveWnd->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;


	m_pwndAGVPIOStatus = new CAGVPIOStatusWnd(this);
	if (!m_pwndAGVPIOStatus->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;

	m_pwndConveyorMonitor = new CConveyorMonitorWnd(this);
	if (!m_pwndConveyorMonitor->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	//>>

	// 2024.07.24 remove by kkw
	m_pwndCIMTester = new CCIMTestWnd(this);
	if (!m_pwndCIMTester->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	//>>
	
	
	//<< Main Menu 3
	m_pwndRecipeSelect = new CRecipeSelectWnd;
	if (!m_pwndRecipeSelect->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndRecipeEdit = new CRecipeEditWnd;
	if (!m_pwndRecipeEdit->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	//>>
	
	//<< Main Menu 4 
	m_pwndPPBox	= new CPPBoxWnd(this);
	if (!m_pwndPPBox->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))	return -1;
	//>>
	
	//<< Main Menu 5
	m_pwndMCMonitor	= new CMCMonitorWnd(this);
	if (!m_pwndMCMonitor->Create			(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;

	m_pwndDustCollector = new CDustCollectorWnd(this);
	if (!m_pwndDustCollector->Create		(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndIOStatus	= new CIOStatusWnd(this);
	if (!m_pwndIOStatus->Create			(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	//m_pwndAirPressure	= new CAirPressureWnd(this);
	//if (!m_pwndAirPressure->Create		(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndMCC		= new CMCCWnd(this);
	if (!m_pwndMCC->Create				(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndRegisterViewer	= new RegisterViewer();
	if (!m_pwndRegisterViewer->Create				(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndGlassEmissions = new CGlassEmissionsWnd(this);
	if (!m_pwndGlassEmissions->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	//>
	
	//<< Main Menu 6
	m_pwndDeviceParameter	= new CDeviceParameterWnd(this);
	if (!m_pwndDeviceParameter->Create	(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndPositionParameter = new CPositionParameterWnd(this);
	if (!m_pwndPositionParameter->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndMotorParameter	= new CMotorParameterWnd(this);
	if (!m_pwndMotorParameter->Create	(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndConveyorParameter = new CConveyorParameter(this);
	if (!m_pwndConveyorParameter->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	//>>
	
	//<< Main Menu 7
	m_pwndAlarmLog = new CAlarmLogWnd(this);
	if (!m_pwndAlarmLog->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndParameterLog = new CParameterChangeLogWnd(this);
	if (!m_pwndParameterLog->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndRecipeLog = new CRecipeChangeLogWnd(this);
	if (!m_pwndRecipeLog->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndButtonLog = new CButtonLogWnd(this);
	if (!m_pwndButtonLog->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndSequence = new CSequenceStepWnd(this);
	if (!m_pwndSequence->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	//>>
	
	//<< Main Menu 8
	m_pwndPMMode = new CPMWnd();
	if (!m_pwndPMMode->Create(NULL, NULL, NULL))		return -1;
	//>>
	m_pwndIPCommand = new CIPCommandWnd();
	if (!m_pwndIPCommand->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndCIMIPCommand = new CCIMIPCommandWnd();
	if (!m_pwndCIMIPCommand->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	
	m_pwndTerminalMessage = new CTerminalMessageWnd();
	if (!m_pwndTerminalMessage->Create(NULL, NULL, NULL))		return -1;

	m_pwndInterlockMessage = new CInterlockMessageWnd();
	if (!m_pwndInterlockMessage->Create(NULL, NULL, NULL))		return -1;

	m_pwndOPCallMessage= new COPCallMessageWnd();
	if (!m_pwndOPCallMessage->Create(NULL, NULL, NULL))		return -1;
	
	//<< Main Menu 9
	m_pwndUserManager = new CUserManagerWnd(this);
	if (!m_pwndUserManager->Create(NULL, NULL, WS_CHILD, CRect(), this, NULL))		return -1;
	//>>
	//ShowWindow(SW_SHOW);

	m_pwndTitle->ShowWindow(SW_SHOW);
	m_pwndMainMenu->ShowWindow(SW_SHOW);
	//m_pwndSubMenu->ShowWindow(SW_SHOW);
	//m_pwndMainStatus->ShowWindow(SW_SHOW);
	//SetView(eMM_AUTO);
	sm.Option->tKeyPad.fShowKeyPad = FALSE;
	//SetView(eMAIN_MENU_1, eSUB_MENU_1, sm.DeviceParameter->eUserGrade);

	// 2024.06.06 LBH - 로그인 아이디 초기화
	memset(&sm.DeviceParameter->szCurUserID, 0x00, sizeof(sm.DeviceParameter->szCurUserID));

#ifdef DEBUG
	theApp.m_UserManager.UserLogin(_T("QRSystem"), _T("QRS"));
	SetView(eMAIN_MENU_1, eSUB_MENU_1, sm.DeviceParameter->eUserGrade);
	LogIn();
#else
	if (!sm.Sequence->fPMMode) {
		LogOut();
	}
#endif // _DEBUG


	SetTimer(TIMER_CHECK_PM, 100, NULL);

	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent) {

	switch (nIDEvent) {
		case TIMER_LOGOUT: {
			if ((GetTickCount64() - g_iLogout_time >= sm.DeviceParameter->tTime.dLogoutTime * 1000)) {
				g_iLogout_time = GetTickCount64();
				LogOut();
			}
			break;
		}
		case TIMER_CHECK_PM: {
			if (m_pwndPMMode) {
				sm.Sequence->fPMMode = m_pwndPMMode->IsWindowVisible();
			}

			if (sm.Sequence->fPMMode) {
				if (m_pwndLogIn) {
					if (m_pwndLogIn->IsWindowVisible()) {
						m_pwndLogIn->ShowWindow(SW_HIDE);
					}

				}
				if (IsWindowVisible()) {
					ShowWindow(SW_HIDE);
					ClosePopupUI();
					//CloseLoginUI();
				}
			}
			else {
				if (m_bPMMode != sm.Sequence->fPMMode) {
					if (!IsWindowVisible()) {
						ShowWindow(SW_MAXIMIZE);
					}

					if (CString(theApp.m_UserManager.GetCurrentUserData().szID).IsEmpty()) {
						LogOut();
					}
					else {
						SetView(eMAIN_MENU_1, eSUB_MENU_1);
					}
				}
			}

			m_bPMMode = sm.Sequence->fPMMode;
			break;
		}
	}
	
	//switch (nIDEvent) {
	//	case INTERVAL_START_PROCESS : {
	//		if (theApp.GetCreatedWindows()) {
	//			KillTimer(INTERVAL_START_PROCESS);
	//			if (!fn_Initialize()) 
	//				SetTimer(INTERVAL_START_PROCESS, 500, NULL);
	//			else
	//				SetTimer(INTERVAL_MAIN_STATUS, 300, NULL);
	//		}
	//		break;
	//	}
	//	case INTERVAL_MAIN_STATUS: {
	//		KillTimer(INTERVAL_MAIN_STATUS);	
	//		SetTimer(INTERVAL_MAIN_STATUS, 100, NULL);
	//		break;
	//	}
	//	default:
	//		break;
	//}
	CFrameWndEx::OnTimer(nIDEvent);
}


void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWnd::OnShowWindow(bShow, nStatus);

	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_FULL].left, g_rectArea[eAREA_FULL].top, g_rectArea[eAREA_FULL].right, g_rectArea[eAREA_FULL].bottom, SWP_SHOWWINDOW);
		//fn_ComputeArea();
		Invalidate();
		m_bPMMode = !sm.Sequence->fPMMode;
	}
	else {
		//KillTimer(INTERVAL_INITIALIZE);
	}
}

void CMainFrame::OnKillFocus(CWnd* pNewWnd) {
	CFrameWndEx::OnKillFocus(pNewWnd);
	Invalidate();
}

void CMainFrame::OnSetFocus(CWnd* pOldWnd) {
	// forward focus to the view window
	CFrameWndEx::OnSetFocus(pOldWnd);
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC) {
	return FALSE;
}

void CMainFrame::OnPaint() {
	CPaintDC dc(this);
	try {
		CMemDCEx::CMemDC		memDC(&dc);
		CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_DEFAULT]);
		CBrush*					pOldBrush	= (CBrush*)memDC.SelectObject(&g_brushBackground[eEC_DEFAULT]);
		CPen*					pOldPen		= (CPen*)memDC.SelectObject(&pen);
		CRect					rect		= m_rectStatic[eS_FULL_AREA];
		memDC.SelectObject(&g_brushBackground[eEC_DEFAULT]);
		
		CDrawSymbol::DrawLineRect(memDC, g_rectArea[eAREA_FULL], g_colorBackground[eEC_DEFAULT]);

		memDC.SelectObject(pOldBrush);
		memDC.SelectObject(pOldPen);
	}
	catch (...) {
	}
}

void CMainFrame::fn_ComputeArea() {
	GetClientRect(m_rectStatic[eS_FULL_AREA]);
}

BOOL CMainFrame::fn_Initialize() {
	BOOL	bResult		= FALSE;
	switch (m_eIitializeStep) {
		case 0: {
			
			fn_ComputeArea(); 
			++m_eIitializeStep;
			break;
		}
		case 1: {
			
			++m_eIitializeStep;
			break;
		}
		case 2: {
			
			++m_eIitializeStep;						
			
			break;
		}
		case 3: {
			++m_eIitializeStep;	
			
			break;
		}
		case 4: {
			
			++m_eIitializeStep;
			break;
		}
		case 5: {
			
			++m_eIitializeStep;
			break;
		}
		case 6: {			
			++m_eIitializeStep;
			break;
		}
		case 7: {
			bResult = TRUE;	
			break;
		}
				
	}
	return bResult;
}

void CMainFrame::SetView(int eMainMenu, int eSubMenu, eUSER_GRADE eUser) {
	//strALogPos = _T("Main Menu ");
	HWND	hwndWork = NULL;
	HWND	hwndSub = NULL;
	BOOL	bSubChange = FALSE;
	if (sm.Option->tKeyPad.fShowKeyPad)
		return;
	// 해당 IO 가 들어온 경우 해당 두개의 메뉴 말고는 선택 못하도록
	if (sm.IO->tIn_CRT.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION) {
		if (eMainMenu == eMAIN_MENU_2) {
			return;
		}
	}

	if (eMainMenu != g_eSelectedMainMenu)
		bSubChange = TRUE;
	if (eMainMenu != g_eSelectedMainMenu || eSubMenu != g_eSelectedSubMenu) {
		//g_eSelectedMainMenu = eMainMenu;
		//switch (g_eSelectedMainMenu) {
		switch (eMainMenu) {
			case eMAIN_MENU_1: {
				g_eSelectedSubMenu = eSubMenu;
				if (m_pwndMainStatus)
					hwndWork = m_pwndMainStatus->m_hWnd;

				hwndSub = NULL;
				break;
			}
			case eMAIN_MENU_2: {
				if (sm.Sequence->fAutoMode && !sm.Sequence->fCycleStop)
				{
					IPMessage(g_st[IDS_MAIN_FRAME_CAN_NOT_BE_ACCESSED_DURING_AUTO]);
					return;
				}

				g_eSelectedSubMenu = eSubMenu;
				switch (eSubMenu) {
					case eSUB_MENU_1: {
						hwndWork = m_pwndIOControl->m_hWnd;
						break;
					}
					case eSUB_MENU_2: {
						hwndWork = m_pwndTeachMoveWnd->m_hWnd;
						break;
					}
					case eSUB_MENU_3: {
						hwndWork = m_pwndTeachModeConveyorWnd->m_hWnd;
						break;
					}
					case eSUB_MENU_4: {
						hwndWork = m_pwndPPBOXCodeReader->m_hWnd;
						break;
					}
					case eSUB_MENU_5: {
						hwndWork = m_pwndCIMTester->m_hWnd;
						break;
					}

				}
				hwndSub = m_pwndSubMenu->m_hWnd;
				break;
			}
			case eMAIN_MENU_3: {
				g_eSelectedSubMenu = eSubMenu;
				hwndWork = m_pwndRecipeEdit->m_hWnd;
				hwndSub = NULL;
				break;
			}
			case eMAIN_MENU_4: {
				g_eSelectedSubMenu = eSubMenu;
				hwndWork = m_pwndRecipeSelect->m_hWnd;
				hwndSub = NULL;
				break;
			}
			case eMAIN_MENU_5: {
				g_eSelectedSubMenu = eSubMenu;
				hwndWork = m_pwndPPBox->m_hWnd;
				hwndSub = NULL;
				break;
			}
			case eMAIN_MENU_6: {
				// 해당 IO 가 살아있는 경우에는 모니터 메뉴에선 IO 모니터만 선택 가능하도록
				g_eSelectedSubMenu = eSubMenu;
				switch (eSubMenu) {
					case eSUB_MENU_1: {
						hwndWork = m_pwndMCMonitor->m_hWnd;
						break;
					}
					case eSUB_MENU_2: {
						hwndWork = m_pwndDustCollector->m_hWnd;
						break;
					}
					case eSUB_MENU_3: {
						hwndWork = m_pwndIOStatus->m_hWnd;
						break;
					}
					case eSUB_MENU_4: {
						hwndWork = m_pwndMCC->m_hWnd;
						break;
					}
					case eSUB_MENU_5: {
						hwndWork = m_pwndRegisterViewer->m_hWnd;
						break;
					}
					case eSUB_MENU_6: {
						hwndWork = m_pwndGlassEmissions->m_hWnd;
						break;
					}
					case eSUB_MENU_7: {
						hwndWork = m_pwndAGVPIOStatus->m_hWnd;
						break;
					}
					case eSUB_MENU_8: {
						hwndWork = m_pwndConveyorMonitor->m_hWnd;
						break;
					}
				}
				hwndSub = m_pwndSubMenu->m_hWnd;
				break;
			}
			case eMAIN_MENU_7: {
				switch (eSubMenu) {
					case eSUB_MENU_1: {
						hwndWork = m_pwndDeviceParameter->m_hWnd;
						break;
					}
					case eSUB_MENU_2: {
						hwndWork = m_pwndPositionParameter->m_hWnd;

						break;
					}
					case eSUB_MENU_3: {
						if ((sm.Sequence->fAutoMode && !sm.Sequence->fCycleStop))
						{
							IPMessage(g_st[IDS_MAIN_FRAME_CAN_NOT_ACCESS_DURING_OPERATION]);
							return;
						}

						for (int iMotor = 0; iMotor < eMOTOR_END; ++iMotor)
						{
							if (sm.MotorStatus[iMotor].tEtc.bIPRun == TRUE)
							{
								IPMessage(g_st[IDS_MAIN_FRAME_CAN_NOT_ACCESS_MOVE_AXIS]);
								return;
							}
						}
						hwndWork = m_pwndMotorParameter->m_hWnd;
						break;
					}
					case eSUB_MENU_4: {
						hwndWork = m_pwndConveyorParameter->m_hWnd;
						break;
					}
				}
				g_eSelectedSubMenu = eSubMenu;
				hwndSub = m_pwndSubMenu->m_hWnd;
				break;
			}
			case eMAIN_MENU_8: {
			g_eSelectedSubMenu = eSubMenu;
				switch (eSubMenu) {
					case eSUB_MENU_1:  {
						hwndWork = m_pwndAlarmLog->m_hWnd;
						break;
					}
					case eSUB_MENU_2:  {
						hwndWork = m_pwndRecipeLog->m_hWnd;
						break;
					}
					case eSUB_MENU_3: {
						hwndWork = m_pwndParameterLog->m_hWnd;
						break;
					}
					case eSUB_MENU_4: {
						hwndWork = m_pwndButtonLog->m_hWnd;
						break;
					}
					case eSUB_MENU_5: {
						hwndWork = m_pwndSequence->m_hWnd;
						break;
					}
				}
				hwndSub = m_pwndSubMenu->m_hWnd;
				break;
			}
			case eMAIN_MENU_9: {
				if(!sm.Sequence->fAutoMode || (sm.Sequence->fAutoMode && sm.Sequence->fCycleStop))
				{
					hwndWork = m_pwndPMMode->m_hWnd;
					hwndSub = NULL;
					g_ipCIMClient.PostIPCommand("SCT", "Show", "PM");
				}
				else
				{
					IPMessage(g_st[IDS_MAIN_FRAME_CAN_NOT_BE_PM_MODE_DURING_AUTO_MODE]);
				}
				break;
			}
			case eMAIN_MENU_10: {
				hwndWork = m_pwndUserManager->m_hWnd;
				hwndSub = NULL;
				break;
			}
		}
		g_eSelectedMainMenu = eMainMenu;
	}

	if (hwndWork) 
	{
		if (m_hwndWorkArea && m_hwndWorkArea != hwndWork)	::ShowWindow(m_hwndWorkArea, SW_HIDE);
		if (hwndWork)										::ShowWindow(hwndWork, SW_SHOW);
		if (hwndSub == m_pwndSubMenu->m_hWnd)			((CSubMenuWnd*)m_pwndSubMenu)->SetSubMenu();
		m_hwndWorkArea = hwndWork;
	}

	if (bSubChange)
	{
		if (m_hwndSubMenu && m_hwndSubMenu != hwndSub)	::ShowWindow(m_hwndSubMenu, SW_HIDE);
		if (hwndSub)									::ShowWindow(hwndSub, SW_SHOW);
		m_hwndSubMenu = hwndSub;
	}

	::ShowWindow(m_pwndExit->m_hWnd, SW_HIDE);
	//Invalidate();
	m_pwndMainMenu->Invalidate(FALSE);
}



void CMainFrame::OnClose()
{
	g_eSelectedMainMenu = eMAIN_MENU_NONE;
	g_eSelectedSubMenu = eSUB_MENU_NONE;

	::ShowWindow(m_hwndWorkArea, SW_HIDE);
	::ShowWindow(m_hwndSubMenu, SW_HIDE);
	::ShowWindow(m_pwndExit->m_hWnd, SW_SHOW);
	 
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CFrameWndEx::OnClose();
}



void CMainFrame::LogOut() {
    theApp.m_UserManager.UserLogOut();  
    ClosePopupUI();
	m_pwndTitle->ShowWindow(SW_HIDE);
	m_pwndMainMenu->ShowWindow(SW_HIDE);
	::ShowWindow(m_hwndWorkArea, SW_HIDE);
	::ShowWindow(m_hwndSubMenu, SW_HIDE);
	m_pwndLogIn->ShowWindow(SW_SHOW);
    m_pwndLogIn->SetForegroundWindow();
	KillTimer(TIMER_LOGOUT);
}

void CMainFrame::LogIn() {
	g_iLogout_time = GetTickCount64();
	m_pwndTitle->ShowWindow(SW_SHOW);
	m_pwndMainMenu->ShowWindow(SW_SHOW);
	::ShowWindow(m_hwndWorkArea, SW_SHOW);
	::ShowWindow(m_hwndSubMenu, SW_SHOW);
	m_pwndLogIn->ShowWindow(SW_HIDE);
	SetTimer(TIMER_LOGOUT, 100, NULL);
}

void CMainFrame::ClosePopupUI() {	// KES 추가 2024.2.14
    CIPVar var;
	var.SetChildItem("Show", FALSE);
    // AutoDelete
    __POST_IP_COMMAND__("AutoDelete", "AutoDelete", "Show", var);
    // AxisControl
    __POST_IP_COMMAND__("PLCLinker", "AxisStatusControl", "Show", var);
	// AxisControl
	__POST_IP_COMMAND__("PLCLinker", "AxisControl", "Show", var);

    // Scrap
    ((CMainStatusWnd*)m_pwndMainStatus)->CloseAllScrapWnd();
    // AutoRunChecker, PasswordWnd (TitleWnd에 있음)
    ((CTitleWnd*)m_pwndTitle)->ClosePopupUI();
    // Serial (추후 추가될 수 있음)
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch (pMsg->message) {
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN: {
			g_iLogout_time = GetTickCount64();
			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: {
			g_iLogout_time = GetTickCount64();
		}
	}

	return CFrameWndEx::PreTranslateMessage(pMsg);
}

// LJY 추가 2024.06.23
void CMainFrame::ChagneLanguage()
{
	g_ipClient.PostIPCommand("SystemAlarm", "Alarm", "SetLanguage");	// LJY 추가 2024.3.21
	g_stDescription.SetCurrentLanguage(sm.DeviceParameter->System.eLanguage);
	g_stMotor.SetCurrentLanguage(sm.DeviceParameter->System.eLanguage);
	g_st.SetCurrentLanguage(sm.DeviceParameter->System.eLanguage);


	if (m_pwndConveyorParameter)
		((CConveyorParameter*)m_pwndConveyorParameter)->ChangeLanguage();
	if (m_pwndPositionParameter)
		((CPositionParameterWnd*)m_pwndPositionParameter)->ChagneLanguage();
	if (m_pwndMotorParameter)
		((CMotorParameterWnd*)m_pwndMotorParameter)->ChangeLanguage();
	if (m_pwndTeachMoveWnd)
		((CTeachMoveWnd*)m_pwndTeachMoveWnd)->ChangeLanguage();
	if (m_pwndMCMonitor)
		((CMCMonitorWnd*)m_pwndMCMonitor)->ChangeLanguage();
	if (m_pwndRegisterViewer)
		((RegisterViewer*)m_pwndRegisterViewer)->ChangeLanguage();
	if (m_pwndConveyorMonitor)
		((CConveyorMonitorWnd*)m_pwndConveyorMonitor)->ChangeLanguage();
	//<< KES 추가 2024.07.08
	if (m_pwndAlarmLog)
		((CAlarmLogWnd*)m_pwndAlarmLog)->ChangeLanguage();
	//>>

	if(m_pwndMainMenu)
		m_pwndMainMenu->Invalidate(FALSE);
	if (m_pwndSubMenu)
		m_pwndSubMenu->Invalidate(FALSE);
	if (m_pwndDeviceParameter)
		m_pwndDeviceParameter->Invalidate(FALSE);
	if (m_pwndIOControl)
		m_pwndIOControl->Invalidate(FALSE);
	if (m_pwndRecipeEdit)
		m_pwndRecipeEdit->Invalidate(FALSE);
	if (m_pwndPPBox)
		m_pwndPPBox->Invalidate(FALSE);
	
	if (m_pwndUserManager)
		m_pwndUserManager->Invalidate(FALSE);

}
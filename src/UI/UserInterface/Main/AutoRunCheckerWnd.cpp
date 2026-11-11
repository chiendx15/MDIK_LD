#include "pch.h"
#include "UI.h"
#include "MainFrame.h"
#include "AutoRunCheckerWnd.h"
#include "ProgramConnectionCheckerWnd.h"

static LPCTSTR lpszCheckList[] = {
	_T(" CHECK_PROCESS"),
	_T(" CHECK_RECIPE"),
	_T(" CHECK_DRIVE_MODE_AUTO"),
	_T(" CHECK_MC_ON"),
	_T(" CHECK_DOOR_CLOSE"),
	_T(" CHECK_NOT_FOUND_HEAVY_ALARM"),
	_T(" CHECK_OTHER_EQP_NOT_FOUND_HEAVY_ALARM"),
	_T(" CHECK_MOTOR_SERVO_ON"),
	_T(" CHECK_NOT_FOUND_SERVO_ALARM"),
	_T(" CHECK_SERVO_INIT"),
	_T(" CHECK_CONVEYOR_READY"),
	_T(" CHECK_CONTROL_PC_RECIPE"),
	_T(" CHECK_INTERLOCK_MESSAGE"),
	_T(" CHECK_EMPTY_PPBOX"),
};

#define IDC_TIMER_AUTORUN_CHECK 10342

CAutoRunCheckerWnd::CAutoRunCheckerWnd(CWnd* pwndParent)  : TIPCT<CFrameWndEx>(&g_ipClient)
	, m_bCheck{}
{
	m_pwndParent = pwndParent;
	m_eSelectedControl = eC_NONE;
	m_eMouseOver = eC_NONE;
}


CAutoRunCheckerWnd::~CAutoRunCheckerWnd() {
	m_pwndParent = nullptr;
}

IMPLEMENT_DYNCREATE(CAutoRunCheckerWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP(CAutoRunCheckerWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BEGIN_IP_COMMAND_TABLE(CAutoRunCheckerWnd, IIPCommandTarget)
	ON_IP_EXCL("Check", "AutoRun", OnIPAutoRunCheck)
END_IP_COMMAND_TABLE()

BOOL CAutoRunCheckerWnd::PreCreateWindow(CREATESTRUCT& cs) {
	
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}


int CAutoRunCheckerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
try { 
	
	SetWindowText(_T("Auto Run Checker"));
	ShowWindow(SW_HIDE);

	m_pwndProgramChecker = new CProgramConnectionCheckerWnd();
	m_pwndProgramChecker->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(), this);
	m_pwndProgramChecker->ShowWindow(SW_HIDE);

	//<< 창 위치 중앙 세팅
	m_rectShowWindowPos = g_rectArea[eS_FULL_AREA];
	double dW = m_rectShowWindowPos.Width() / 4;
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

void CAutoRunCheckerWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);

	CRect rc = m_rectS[eS_FULL_AREA];
	rc.DeflateRect(0, 10, 0, 0);
	double dW = (rc.Width() - 5) / 7.;
	double dH = (rc.Height() - 5) / 7.;

	m_rectS[eS_RUN_OPTION_AREA				] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 7, rc.top + dH * 1);
	m_rectS[eS_RUN_CHECK_LIST_AREA			] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 1, rc.left + dW * 3, rc.top + dH * 6);
	m_rectS[eS_RUN_CONTROL_PC_RECIPE_AREA	] = CRect(rc.left + 5 + dW * 3, rc.top + 5 + dH * 1, rc.left + dW * 6, rc.top + dH * 6);
	m_rectS[eS_RUN_PROGRAM_CHECK_AREA		] = CRect(rc.left + 5 + dW * 6, rc.top + 5 + dH * 1, rc.left + dW * 7, rc.top + dH * 6);
	m_rectS[eS_RUN_CONTROL_AREA				] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 6, rc.left + dW * 7, rc.top + dH * 7);

	rc = m_rectS[eS_RUN_OPTION_AREA];
	rc.DeflateRect(0, 10, 0, 0);
	dW = (rc.Width() - 5) / 7.;
	dH = (rc.Height() - 5) / 1.;
	m_rectC[eC_SEQUENCE_INITIALIZE		] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 1, rc.top + dH * 1);
	m_rectC[eC_CONVEYOR_INITIALIZE		] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 0, rc.left + dW * 2, rc.top + dH * 1);
	m_rectC[eC_GLASS_TRANSFER_INITIALIZE] = CRect(rc.left + 5 + dW * 2, rc.top + 5 + dH * 0, rc.left + dW * 3, rc.top + dH * 1);

	rc = m_rectS[eS_RUN_CHECK_LIST_AREA];
	rc.DeflateRect(0, 10, 0, 0);
	dW = (rc.Width() - 5) / 1.;
	dH = (rc.Height() - 5) / 14.;

	m_rectC[eC_CHECK_LIST_01] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0 , rc.left + dW * 1, rc.top + dH *  1);
	m_rectC[eC_CHECK_LIST_02] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 1 , rc.left + dW * 1, rc.top + dH *  2);
	m_rectC[eC_CHECK_LIST_03] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 2 , rc.left + dW * 1, rc.top + dH *  3);
	m_rectC[eC_CHECK_LIST_04] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 3 , rc.left + dW * 1, rc.top + dH *  4);
	m_rectC[eC_CHECK_LIST_05] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 4 , rc.left + dW * 1, rc.top + dH *  5);
	m_rectC[eC_CHECK_LIST_06] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 5 , rc.left + dW * 1, rc.top + dH *  6);
	m_rectC[eC_CHECK_LIST_07] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 6 , rc.left + dW * 1, rc.top + dH *  7);
	m_rectC[eC_CHECK_LIST_08] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 7 , rc.left + dW * 1, rc.top + dH *  8);
	m_rectC[eC_CHECK_LIST_09] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 8 , rc.left + dW * 1, rc.top + dH *  9);
	m_rectC[eC_CHECK_LIST_10] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 9 , rc.left + dW * 1, rc.top + dH * 10);
	m_rectC[eC_CHECK_LIST_11] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 10, rc.left + dW * 1, rc.top + dH * 11);
	m_rectC[eC_CHECK_LIST_12] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 11, rc.left + dW * 1, rc.top + dH * 12);
	m_rectC[eC_CHECK_LIST_13] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 12, rc.left + dW * 1, rc.top + dH * 13);
	m_rectC[eC_CHECK_LIST_14] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 13, rc.left + dW * 1, rc.top + dH * 14);
	
	rc = m_rectS[eS_RUN_CONTROL_PC_RECIPE_AREA];
	rc.DeflateRect(0, 10, 0, 0);
	dW = (rc.Width() - 5) / 2.;
	dH = (rc.Height() - 5) / 10.;

	m_rectC[eC_LD_RECIPE_INDEX		] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 1, rc.top + dH *  1);
	m_rectC[eC_SCT_RECIPE_INDEX		] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 1, rc.left + dW * 1, rc.top + dH *  2);
	m_rectC[eC_CCTA_RECIPE_INDEX	] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 2, rc.left + dW * 1, rc.top + dH *  3);
	m_rectC[eC_CCTB_RECIPE_INDEX	] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 3, rc.left + dW * 1, rc.top + dH *  4);
	m_rectC[eC_ULDA_RECIPE_INDEX	] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 4, rc.left + dW * 1, rc.top + dH *  5);
	m_rectC[eC_ULDB_RECIPE_INDEX	] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 5, rc.left + dW * 1, rc.top + dH *  6);

	m_rectC[eC_LD_RECIPE_PPID		] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 0, rc.left + dW * 2, rc.top + dH *  1);
	m_rectC[eC_SCT_RECIPE_PPID		] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 1, rc.left + dW * 2, rc.top + dH *  2);
	m_rectC[eC_CCTA_RECIPE_PPID		] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 2, rc.left + dW * 2, rc.top + dH *  3);
	m_rectC[eC_CCTB_RECIPE_PPID		] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 3, rc.left + dW * 2, rc.top + dH *  4);
	m_rectC[eC_ULDA_RECIPE_PPID		] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 4, rc.left + dW * 2, rc.top + dH *  5);
	m_rectC[eC_ULDB_RECIPE_PPID		] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 5, rc.left + dW * 2, rc.top + dH *  6);

	rc = m_rectS[eS_RUN_CONTROL_AREA];
	dW = (rc.Width() - 5) / 7.;
	dH = (rc.Height() - 5) / 1.;

	m_rectC[eC_RE_CHECK]	= CRect(rc.left + 5 + dW * 4, rc.top + 5 + dH * 0, rc.left + dW * 5, rc.top + dH * 1);
	m_rectC[eC_START]		= CRect(rc.left + 5 + dW * 5, rc.top + 5 + dH * 0, rc.left + dW * 6, rc.top + dH * 1);
	m_rectC[eC_CLOSE]		= CRect(rc.left + 5 + dW * 6, rc.top + 5 + dH * 0, rc.left + dW * 7, rc.top + dH * 1);

	if (m_pwndProgramChecker) {
		CRect rectProgramChecker = m_rectS[eS_RUN_PROGRAM_CHECK_AREA];
		rectProgramChecker.DeflateRect(5, 15, 0, 0);
		m_pwndProgramChecker->MoveWindow(rectProgramChecker, FALSE);
	}
}

void CAutoRunCheckerWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		
		CRect& rc = m_rectShowWindowPos;
		SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);

		fn_ComputeArea();
		m_eSelectedControl	= eC_NONE;
		m_bOptionSequenceInit = FALSE;
		m_bConveyorSequenceInit = FALSE;
		m_bGlassTransferSequenceInit = FALSE;

		m_pwndProgramChecker->ShowWindow(SW_SHOW);

		for (int i = 0; i < nCHECK; ++i)
			m_bCheck[i] = FALSE;

		PostIPCommand("UI", "Check", "AutoRun");
		
		SetTimer(IDC_TIMER_AUTORUN_CHECK, 100, NULL);
	}
	else {
		KillTimer(IDC_TIMER_AUTORUN_CHECK);

		m_pwndProgramChecker->ShowWindow(SW_HIDE);
	}
}

BOOL CAutoRunCheckerWnd::OnEraseBkgnd(CDC* pDC) {
	
	return FALSE;
}

void CAutoRunCheckerWnd::OnPaint() {
	CPaintDC				dc(this);
	CMemDCEx::CMemDC		memDC(&dc);
	memDC.FillSolidRect(m_rectS[eS_FULL_AREA], RgbColor::AliceBlue);

	DrawGroupBox(memDC, m_rectS[eS_RUN_OPTION_AREA				], _T("Option")		, RGB_BLACK, RGB_BLACK, CSize(-1, -1), CPoint(15, -10));
	DrawGroupBox(memDC, m_rectS[eS_RUN_CHECK_LIST_AREA			], _T("Check List")	, RGB_BLACK, RGB_BLACK, CSize(-1, -1), CPoint(15, -5));
	DrawGroupBox(memDC, m_rectS[eS_RUN_CONTROL_PC_RECIPE_AREA	], _T("Recipe List"), RGB_BLACK, RGB_BLACK, CSize(-1, -1), CPoint(15, -5));
	DrawGroupBox(memDC, m_rectS[eS_RUN_PROGRAM_CHECK_AREA		], _T("Process")	, RGB_BLACK, RGB_BLACK, CSize(-1, -1), CPoint(15, -5));
	DrawGroupBox(memDC, m_rectS[eS_RUN_CONTROL_AREA				], _T("Control")	, RGB_BLACK, RGB_BLACK, CSize(-1, -1), CPoint(15, -5));
	
	DrawMDIMenuButtonN(memDC, m_rectC[eC_SEQUENCE_INITIALIZE]		, g_st[IDS_AUTO_RUN_CHECK_WND_SEQUENCE_INIT], m_bOptionSequenceInit || m_eSelectedControl == eC_SEQUENCE_INITIALIZE, m_eMouseOver == eC_SEQUENCE_INITIALIZE);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_CONVEYOR_INITIALIZE]		, g_st[IDS_AUTO_RUN_CHECK_WND_CONVEYOR_SEQUENCE_INIT], m_bConveyorSequenceInit || m_eSelectedControl == eC_CONVEYOR_INITIALIZE, m_eMouseOver == eC_CONVEYOR_INITIALIZE);
    DrawMDIMenuButtonN(memDC, m_rectC[eC_GLASS_TRANSFER_INITIALIZE]	, g_st[IDS_AUTO_RUN_CHECK_WND_GLASS_TRANSFER_SEQUENCE_INIT], m_bGlassTransferSequenceInit || m_eSelectedControl == eC_GLASS_TRANSFER_INITIALIZE, m_eMouseOver == eC_GLASS_TRANSFER_INITIALIZE);

	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_01], lpszCheckList[eCHECK_PROCESS							], m_bCheck[eCHECK_PROCESS							], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_02], lpszCheckList[eCHECK_RECIPE							], m_bCheck[eCHECK_RECIPE							], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_03], lpszCheckList[eCHECK_DRIVE_MODE_AUTO					], m_bCheck[eCHECK_DRIVE_MODE_AUTO					], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_04], lpszCheckList[eCHECK_MC_ON							], m_bCheck[eCHECK_MC_ON 							], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_05], lpszCheckList[eCHECK_DOOR_CLOSE						], m_bCheck[eCHECK_DOOR_CLOSE						], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_06], lpszCheckList[eCHECK_NOT_FOUND_HEAVY_ALARM			], m_bCheck[eCHECK_NOT_FOUND_HEAVY_ALARM			], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_07], lpszCheckList[eCHECK_OTHER_EQP_NOT_FOUND_HEAVY_ALARM	], m_bCheck[eCHECK_OTHER_EQP_NOT_FOUND_HEAVY_ALARM	], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_08], lpszCheckList[eCHECK_MOTOR_SERVO_ON					], m_bCheck[eCHECK_MOTOR_SERVO_ON					], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_09], lpszCheckList[eCHECK_NOT_FOUND_SERVO_ALARM			], m_bCheck[eCHECK_NOT_FOUND_SERVO_ALARM			], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_10], lpszCheckList[eCHECK_SERVO_INIT						], m_bCheck[eCHECK_SERVO_INIT						], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_11], lpszCheckList[eCHECK_CONVEYOR_READY					], m_bCheck[eCHECK_CONVEYOR_READY					], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_12], lpszCheckList[eCHECK_CONTROL_PC_RECIPE				], m_bCheck[eCHECK_CONTROL_PC_RECIPE				], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_13], lpszCheckList[eCHECK_INTERLOCK_MESSAGE				], m_bCheck[eCHECK_INTERLOCK_MESSAGE				], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);
	DrawRoundSensor(memDC, m_rectC[eC_CHECK_LIST_14], lpszCheckList[eCHECK_EMPTY_PPBOX						], m_bCheck[eCHECK_EMPTY_PPBOX						], CPoint(10,10), DT_SENSOR_ON, DT_SENSOR_OFF, DT_FONTSIZE, DT_PEN, DT_TEXT, DT_BK, DT_LEFT | DT_VCENTER);

	int iCurrentRecipeIndex = g_recipes.GetAppliedIndex() + 1;
	DrawSensor(memDC, m_rectC[eC_LD_RECIPE_INDEX]	, L"LD Index : " + ToCString(m_recipeIndex[eMACHINE_LD])	, iCurrentRecipeIndex == m_recipeIndex[eMACHINE_LD]);
	DrawSensor(memDC, m_rectC[eC_SCT_RECIPE_INDEX]	, L"SCT Index : " + ToCString(m_recipeIndex[eMACHINE_SCT])	, iCurrentRecipeIndex == m_recipeIndex[eMACHINE_SCT]);
	DrawSensor(memDC, m_rectC[eC_CCTA_RECIPE_INDEX]	, L"CCTA Index : " + ToCString(m_recipeIndex[eMACHINE_CCTR]), iCurrentRecipeIndex == m_recipeIndex[eMACHINE_CCTR]);
	DrawSensor(memDC, m_rectC[eC_CCTB_RECIPE_INDEX]	, L"CCTB Index : " + ToCString(m_recipeIndex[eMACHINE_CCTL]), iCurrentRecipeIndex == m_recipeIndex[eMACHINE_CCTL]);
	DrawSensor(memDC, m_rectC[eC_ULDA_RECIPE_INDEX]	, L"ULDA Index : " + ToCString(m_recipeIndex[eMACHINE_UNDR]), iCurrentRecipeIndex == m_recipeIndex[eMACHINE_UNDR]);
	DrawSensor(memDC, m_rectC[eC_ULDB_RECIPE_INDEX]	, L"ULDB Index : " + ToCString(m_recipeIndex[eMACHINE_UNDL]), iCurrentRecipeIndex == m_recipeIndex[eMACHINE_UNDL]);

	CString PPID = g_recipes.GetAppliedData() != nullptr ? g_recipes.GetAppliedData()->ppid : L"";
	DrawSensor(memDC, m_rectC[eC_LD_RECIPE_PPID]	, L"LD PPID : " + ToCString(m_recipePPID[eMACHINE_LD])		, !PPID.IsEmpty() && PPID == m_recipePPID[eMACHINE_LD]);
	DrawSensor(memDC, m_rectC[eC_SCT_RECIPE_PPID]	, L"SCT PPID : " + ToCString(m_recipePPID[eMACHINE_SCT])	, !PPID.IsEmpty() && PPID == m_recipePPID[eMACHINE_SCT]);
	DrawSensor(memDC, m_rectC[eC_CCTA_RECIPE_PPID]	, L"CCTA PPID : " + ToCString(m_recipePPID[eMACHINE_CCTR])	, !PPID.IsEmpty() && PPID == m_recipePPID[eMACHINE_CCTR]);
	DrawSensor(memDC, m_rectC[eC_CCTB_RECIPE_PPID]	, L"CCTB PPID : " + ToCString(m_recipePPID[eMACHINE_CCTL])	, !PPID.IsEmpty() && PPID == m_recipePPID[eMACHINE_CCTL]);
	DrawSensor(memDC, m_rectC[eC_ULDA_RECIPE_PPID]	, L"ULDA PPID : " + ToCString(m_recipePPID[eMACHINE_UNDR])	, !PPID.IsEmpty() && PPID == m_recipePPID[eMACHINE_UNDR]);
	DrawSensor(memDC, m_rectC[eC_ULDB_RECIPE_PPID]	, L"ULDB PPID : " + ToCString(m_recipePPID[eMACHINE_UNDL])	, !PPID.IsEmpty() && PPID == m_recipePPID[eMACHINE_UNDL]);

	DrawMDIMenuButtonN(memDC, m_rectC[eC_RE_CHECK]	, g_st[IDS_AUTO_RUN_CHECK_WND_AUTO_RUN_CHECK]   , m_eSelectedControl == eC_RE_CHECK, m_eMouseOver == eC_RE_CHECK);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_START]		, g_st[IDS_AUTO_RUN_CHECK_WND_AUTO_RUN_START]   , m_eSelectedControl == eC_START, m_eMouseOver == eC_START);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_CLOSE]		, g_st[IDS_AUTO_RUN_CHECK_WND_CLOSE]            , m_eSelectedControl == eC_CLOSE, m_eMouseOver == eC_CLOSE);
}


void CAutoRunCheckerWnd::OnMouseMove(UINT nFlags, CPoint point) {
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

void CAutoRunCheckerWnd::OnLButtonDown(UINT nFlags, CPoint point) {
	
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

void CAutoRunCheckerWnd::OnLButtonUp(UINT nFlags, CPoint point)  {
	
	CString strLog;
	auto cleanup = make_scope_exit
	(
		[&]()
		{
			if (!strLog.IsEmpty()) {
				WriteButtonLog(_T("Auto Run Checker"), _T("Auto Run Checker"), _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
			}
			m_eSelectedControl = eC_NONE;

			Invalidate(FALSE);

			CFrameWndEx::OnLButtonUp(nFlags, point);
		}
	);

	//TODO 필요한가?
	if (sm.Option->tKeyPad.fShowKeyPad)
		return;
	
	if (m_eSelectedControl != eC_NONE && m_rectC[m_eSelectedControl].PtInRect(point)) {
        
        CString strYes = g_st[IDS_YES];
        CString strNo = g_st[IDS_NO];
        CString strYesOrNo = strYes + _T(";") + strNo;

		switch (m_eSelectedControl) {	

			case eC_SEQUENCE_INITIALIZE: {
				strLog = _T("시퀀스 초기화");
				if (!m_bOptionSequenceInit) {
					if (strNo == RespMessage(g_st[IDS_AUTO_RUN_CHECK_WND_SEQUENCE_INIT_OPTION_ACTIVATE], strYesOrNo, g_st[IDS_AUTO_RUN_CHECK_WND_SEQUENCE_INIT])) {
						break;
					}
				}

				m_bOptionSequenceInit = !m_bOptionSequenceInit;
				break;
			}
			case eC_CONVEYOR_INITIALIZE: {
				strLog = _T("Conveyor 시퀀스 초기화");
				if (!m_bConveyorSequenceInit) {
					if (strNo == RespMessage(g_st[IDS_AUTO_RUN_CHECK_WND_PAPER_INIT_OPTION_ACTIVATE], strYesOrNo, g_st[IDS_AUTO_RUN_CHECK_WND_SEQUENCE_INIT])) {
						break;
					}
				}

				m_bConveyorSequenceInit = !m_bConveyorSequenceInit;
				break;
			}
			case eC_GLASS_TRANSFER_INITIALIZE: {
				strLog = _T("Glass Transfer 시퀀스 초기화");
				if (!m_bGlassTransferSequenceInit) {
					if (strNo == RespMessage(g_st[IDS_AUTO_RUN_CHECK_WND_GLASS_INIT_OPTION_ACTIVATE], strYesOrNo, g_st[IDS_AUTO_RUN_CHECK_WND_SEQUENCE_INIT])) {
						break;
					}
				}

				m_bGlassTransferSequenceInit = !m_bGlassTransferSequenceInit;
				break;
			}
			case eC_RE_CHECK: {
				strLog = _T("자동운전 체크");
				PostIPCommand("UI", "Check", "AutoRun");
				break;
			}
			case eC_START: {
				strLog = _T("자동운전 시작");
				BOOL bAllCheck = TRUE;
				for (int i = 0; i < nCHECK; ++i) {
					if (!m_bCheck[i])
						bAllCheck = FALSE;
				}

				if (!bAllCheck) {
					IPMessage(g_st[IDS_AUTO_RUN_CHECK_WND_PLEASE_CHECK_AUTO_RUN_CHECKLIST]);
					break;
				}

				//. 조건 다 검사 하고, 사용자에게 질문
				if (strNo == RespMessage(_T("Auto Run ?"), strYesOrNo, _T("AutoRun"))) {
					break;
				}

				//. AUTORUN flag on.
			
				CIPVar varParam;
				varParam.SetChildItem("SequenceInit", m_bOptionSequenceInit);
				varParam.SetChildItem("ConveyorInit", m_bConveyorSequenceInit);
				varParam.SetChildItem("GlassTransferInit", m_bGlassTransferSequenceInit);
				varParam.SetChildItem("AutoRun", TRUE);
				PostIPCommand("UI", "Check", "AutoRun", varParam);

				m_pwndParent->Invalidate(FALSE);
				break;
			}
			case eC_CLOSE: { 
				strLog = _T("Close");
				ShowWindow(SW_HIDE);
				break; 
			}
			default: {
				break;
			}
		}
	}
}



void CAutoRunCheckerWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case IDC_TIMER_AUTORUN_CHECK: {

		Invalidate(FALSE);

		break;
	}
	default: break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}

BOOL CAutoRunCheckerWnd::OnIPAutoRunCheck(CIPCommand& cmd)
{
	const CIPVar&		var				= cmd.m_var;

	enum {
		eS_INIT,
		eS_CHECK_PROCESS,
		eS_CHECK_RECIPE,
		eS_CHECK_DRIVE_MODE,
		eS_CHECK_MC_ON,
		eS_CHECK_DOOR_CLOSE,
		eS_CHECK_HEAVY_ALARM,
		eS_CHECK_OTHER_EQP_HEAVY_ALARM,
		eS_CHECK_MOTOR_SERVO_ON,
		eS_CHECK_MOTOR_SERVO_ALARM,
		eS_CHECK_MOTOR_SERVO_INIT,
		eS_CHECK_CONVEYOR_READY,
		eS_CHECK_CONTROL_PC_RECIPE,
		eS_CHECK_INTERLOCK_MESSAGE,
		eS_CHECK_EMPTY_PPBOX,
		eS_CHECK_AUTO_RUN,
		eS_AUTO_START,
		eS_END,
	};

	BOOL bAutoRun = var("AutoRun", FALSE);

	switch (cmd.GetCurrentStep()) {
		case eS_INIT: {
			for (int i = 0; i < nCHECK; ++i)
				m_bCheck[i] = FALSE;
			cmd.EnqueueNextStep(-1, 100);
			break;
		} 
		case eS_CHECK_PROCESS: {
			m_bCheck[eCHECK_PROCESS] = m_pwndProgramChecker->IsProcessAllConncted();
			cmd.EnqueueNextStep(-1, 100);
			break;
		}
		case eS_CHECK_RECIPE: {
			if (g_pRecipe)
				m_bCheck[eCHECK_RECIPE] = TRUE;
			cmd.EnqueueNextStep(-1, 100);
			break;
		}
		case eS_CHECK_DRIVE_MODE: {
			if (sm.Option->tSequence.eDriveMode == eDRIVE_MODE_AUTO)
				m_bCheck[eCHECK_DRIVE_MODE_AUTO] = TRUE;

			cmd.EnqueueNextStep(-1, 100);
			break;
		}
		case eS_CHECK_MC_ON: {
			BOOL bMC_On = (sm.IO->tIn_CRT.fUPS_MC01A_ON == TRUE) && (sm.IO->tIn_CRT.fUPS_MC01B_ON == TRUE);
			if (bMC_On)
				m_bCheck[eCHECK_MC_ON] = TRUE;
			cmd.EnqueueNextStep(-1, 100);
			break;
		}
		case eS_CHECK_DOOR_CLOSE: {
			if ((CheckAllDoorCloseLD() && CheckAllDoorCloseSCT()))
				m_bCheck[eCHECK_DOOR_CLOSE] = TRUE;
			cmd.EnqueueNextStep(-1, 100);
			break;
		}
		case eS_CHECK_HEAVY_ALARM: {
			BOOL bHeavyAlarm = FALSE;
			for (int i = 0; i < sm.Alarm->nALARM; ++i) {
				if (sm.Alarm->ALARMs[i].bOccurred && sm.Alarm->ALARMs[i].iALCD == 2) {	// 2번이 Heavy
					bHeavyAlarm = TRUE;
					break;
				}
			}

			if (!bHeavyAlarm)
				m_bCheck[eCHECK_NOT_FOUND_HEAVY_ALARM] = TRUE;

			cmd.EnqueueNextStep(-1, 100);
			break;
		}
		case eS_CHECK_OTHER_EQP_HEAVY_ALARM: {
			BOOL bHeavyAlarm = FALSE;

			if (!sm.Sequence->bCIMSkip) {
				if (sm.PCs->ControlPC[eMACHINE_SCT].fIsAlarm)
					bHeavyAlarm = TRUE;
			}

			if (!bHeavyAlarm)
				m_bCheck[eCHECK_OTHER_EQP_NOT_FOUND_HEAVY_ALARM] = TRUE;

			cmd.EnqueueNextStep(-1, 100);
			break;
		}
		case eS_CHECK_MOTOR_SERVO_ON: {
			BOOL bServoOff = FALSE;
			for (int i = 0; i < eMOTOR_END; ++i) {
				if (!sm.MotorStatus[i].tCrt.bServoOn) {
					bServoOff = TRUE;
					break;
				}
			}
			
			if (!bServoOff)
				m_bCheck[eCHECK_MOTOR_SERVO_ON] = TRUE;
			cmd.EnqueueNextStep(-1, 100);
			break;
		}
	
		case eS_CHECK_MOTOR_SERVO_ALARM: {
			BOOL bAlarmAOff = FALSE;
			for (int i = 0; i < eMOTOR_END; ++i) {
				if (sm.MotorStatus[i].tCrt.bAlarm) {
					bAlarmAOff = TRUE;
					break;
				}
			}
			
			if (!bAlarmAOff)
				m_bCheck[eCHECK_NOT_FOUND_SERVO_ALARM] = TRUE;
			cmd.EnqueueNextStep(-1, 100);
			break;
		}

		case eS_CHECK_MOTOR_SERVO_INIT: {
			BOOL bServoNotInit = FALSE;
			//for (int i = 0; i < eMOTOR_END; ++i) {
			//	if (sm.MotorStatus[i].tEtc.dwHomeStatus != eHOME_STATUS_COMPLETED) {
			//		bServoNotInit = TRUE;
			//		break;
			//	}
			//}
			
			if (!bServoNotInit)
				m_bCheck[eCHECK_SERVO_INIT] = TRUE;
			cmd.EnqueueNextStep(-1, 100);
			break;
		}
		case eS_CHECK_CONVEYOR_READY: {
			BOOL bConveyorAlarm = FALSE;
			for (int i = 0; i < nCONVEYOR_LD; i++) {
				if (!Serial::Inverter::GetInverterReady((eMODULEs)(eMODULE_PC1_LOAD_CONVEYOR_1 + i))) {
					bConveyorAlarm = TRUE;
					break;
				}
			}

			if (!bConveyorAlarm) {
				m_bCheck[eCHECK_CONVEYOR_READY] = TRUE;
			}
			cmd.EnqueueNextStep(-1, 100);
            break;
		}
		case eS_CHECK_CONTROL_PC_RECIPE:
		{
			BOOL bRecipe = TRUE;
			auto SetControlPCRecipe = [&](const CStringA& _pcName, int _idx) -> BOOL
			{
				CIPVar varRecipe, varRecipeResult;
				g_ipCIMClient.SendIPCommand(_pcName, "Get", "RecipeInfo", varRecipe, varRecipeResult, 1000);

				BOOL bResult = TRUE;

				m_recipeIndex[_idx] = varRecipeResult.GetChildItem("Index", -1) + 1;
				int iCurrentRecipeIndex = g_recipes.GetAppliedIndex() + 1;
				
				bResult &= iCurrentRecipeIndex == m_recipeIndex[_idx];

				m_recipePPID[_idx] = varRecipeResult.GetChildItem("PPID", L"");
				CString strPPID = g_recipes.GetAppliedData() != nullptr ? g_recipes.GetAppliedData()->ppid : L"";
				
				bResult &= (strPPID.Compare(m_recipePPID[_idx]) == 0);

				return bResult;
			};

			if (!sm.DeviceParameter->tUse.bDryRun && sm.DeviceParameter->tUse.bPCLinkerUse)
			{
				
				bRecipe &= SetControlPCRecipe("CCTR", eMACHINE_CCTR);
				bRecipe &= SetControlPCRecipe("CCTL", eMACHINE_CCTL);
				bRecipe &= SetControlPCRecipe("ULDR", eMACHINE_UNDR);
				bRecipe &= SetControlPCRecipe("ULDL", eMACHINE_UNDL);

				// 2022.02.21 한쪽 라인 문제 생겨도 돌리기 위해 위치 변경
				bRecipe |= SetControlPCRecipe("LD", eMACHINE_LD);
				bRecipe &= SetControlPCRecipe("SCT", eMACHINE_SCT);
			
			}

			m_bCheck[eCHECK_CONTROL_PC_RECIPE] = bRecipe;
		}
		cmd.EnqueueNextStep(-1, 100);
		break;
		case eS_CHECK_INTERLOCK_MESSAGE: {
			if (sm.Sequence->fOnline) {
				const auto [result, msgs] = LoadMSG(eEQP_SCT, eMSG_TYPE_INTERLOCK);
				if (!result || (int)msgs.size())
					m_bCheck[eCHECK_INTERLOCK_MESSAGE] = FALSE;
				else
					m_bCheck[eCHECK_INTERLOCK_MESSAGE] = TRUE;
			}
			else {
				m_bCheck[eCHECK_INTERLOCK_MESSAGE] = TRUE;
			}
			cmd.EnqueueNextStep(-1, 100);
			break;
		}
		case eS_CHECK_EMPTY_PPBOX: {
			//<< 2024.06.01 LBH - Online Run 시 Elevator Conveyor에 Empty PPBox가 있어야 함.
			m_bCheck[eCHECK_EMPTY_PPBOX] = FALSE;
			if (sm.DeviceParameter->tUse.bDryRun) {
				if (sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Glass.bMaterial)
					m_bCheck[eCHECK_EMPTY_PPBOX] = TRUE;
			}
			else {
				// Elevator Conveyor에 빈 PPBox가 있거나
				if (sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Glass.bMaterial || GetPPBoxSensor(eMODULE_PC1_ELEVATOR_CONVEYOR))
					m_bCheck[eCHECK_EMPTY_PPBOX] = TRUE;

				// Conveyor2에 빈 PPBox가 있을 때.
				if ((sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Glass.bMaterial || GetPPBoxSensor(eMODULE_PC1_LOAD_CONVEYOR_2)) && (sm.LoadPPBox->TRAY_INFOs[0].nCell == 0))
					m_bCheck[eCHECK_EMPTY_PPBOX] = TRUE;
			}
			//>>
			m_bCheck[eCHECK_EMPTY_PPBOX] = TRUE;//24.06.05 KGH 임시추가

			cmd.EnqueueNextStep(-1, 100);
			break;
		}
		case eS_CHECK_AUTO_RUN: {
			BOOL bAllCheck = TRUE;
			for (int i = 0; i < nCHECK; ++i) {
				if (!m_bCheck[i])
					bAllCheck = FALSE;
			}

			if (bAutoRun) {
				if (!bAllCheck) {
					IPMessage(g_st[IDS_AUTO_RUN_CHECK_WND_PLEASE_CHECK_AUTO_RUN_CHECKLIST]);
					cmd.EnqueueNextStep(eS_END, 100);
				}
				else
					cmd.EnqueueNextStep(eS_AUTO_START);
			}
			else
				cmd.EnqueueNextStep(eS_END, 100);
			break;
		}
		case eS_AUTO_START: {		
			CIPVar varParam;
			varParam.SetChildItem("SequenceInit", m_bOptionSequenceInit);
			varParam.SetChildItem("ConveyorInit", m_bConveyorSequenceInit);
			varParam.SetChildItem("GlassTransferInit", m_bGlassTransferSequenceInit);
			PostIPCommand("Sequence", "Sequence", "AutoRun", varParam);
			ShowWindow(SW_HIDE);
			cmd.EnqueueNextStep();
			break;
		}
		case eS_END: {
			break;
		}
	}

	return TRUE;
}
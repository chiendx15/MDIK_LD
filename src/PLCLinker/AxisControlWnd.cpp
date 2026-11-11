// AxisControlWnd.cpp: 구현 파일
//

#include "pch.h"
#include "PLCLinker.h"
#include "AxisControlWnd.h"
#include "AxisAlarmWnd.h"
#include "MainFrm.h"
#include "RegistryMapWnd.h"

#define	ID_LIST	1001
#define ID_INTERVAR_UI 1002
#define ID_SLIBER 1003
// CAxisControlWnd

IMPLEMENT_DYNAMIC(CAxisControlWnd, CFrameWnd)
//IMPLEMENT_DYNCREATE(CAxisControlWnd, CFrameWnd)

CAxisControlWnd::CAxisControlWnd() : TIPCT<CFrameWnd>(&g_ipClient)
	, m_bMaximumPage(TRUE)
	, m_iHideCount(0)
	, m_bHide(TRUE)
	, m_iStopButtonClick(0)
	, m_iSelectMove(eC_MOVE_TYPE_JOG)
	, m_eSelectControl(eC_NONE)
	, m_eSelectedMotor(eMOTOR_0)
	, m_iSlider(100)
	, m_iSelectGantry(0)
{
	memset(&m_dTarget, 0x00, sizeof(double)* eMOTOR_END);
	memset(&m_dSpeed, 0x00, sizeof(double)* eMOTOR_END);
}

CAxisControlWnd::~CAxisControlWnd()
{
}


BEGIN_MESSAGE_MAP(CAxisControlWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_NOTIFY(LVN_ITEMCHANGED, ID_LIST, OnItemchangedLst)
	ON_NOTIFY(NM_RCLICK, ID_LIST, OnItemRClick)
	ON_WM_MOUSEMOVE()
	ON_WM_HSCROLL()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAxisControlWnd 메시지 처리기


BEGIN_IP_COMMAND_TABLE(CAxisControlWnd, IIPCommandTarget)
ON_IP("AxisControl", "Show", OnIPShow)
END_IP_COMMAND_TABLE()

int CAxisControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_lstAxis.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, m_rectS[eS_LIST_AREA], this, ID_LIST);
	
	m_lstAxis.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_lstAxis.SetRowHeight(20);
	m_lstAxis.ShowScrollBar(SB_VERT);
	m_lstAxis.ShowScrollBar(SB_HORZ);

	m_lstAxis.InsertColumn(eLI_AXIS_NO,			_T("No"), LVCFMT_CENTER, 30, 1);
	m_lstAxis.InsertColumn(eLI_SERVO_ON,		_T("O"), LVCFMT_CENTER, 25, 1);
	m_lstAxis.InsertColumn(eLI_HOME,			_T("H"),LVCFMT_CENTER, 25, 1);
	m_lstAxis.InsertColumn(eLI_ALARM,			_T("A"), LVCFMT_CENTER, 25, 1);
	m_lstAxis.InsertColumn(eLI_NAME,			_T("Name"), LVCFMT_CENTER, 170, 1);
	m_lstAxis.InsertColumn(eLI_POSITION,		_T("Pos. (mm)"), LVCFMT_CENTER, 70, 1);
	m_lstAxis.InsertColumn(eLI_SPEED,			_T("Spd. (mm/s)"),LVCFMT_CENTER, 80, 1);
	m_lstAxis.InsertColumn(eLI_TORQUE,			_T("TQ(%)"),LVCFMT_CENTER, 50, 1);
	m_lstAxis.InsertColumn(eLI_P_LIMIT,			_T("+"), LVCFMT_CENTER, 25, 1);
	m_lstAxis.InsertColumn(eLI_N_LIMIT,			_T("-"), LVCFMT_CENTER, 25, 1);
	m_lstAxis.InsertColumn(eLI_INPOSTION,		_T("In"), LVCFMT_CENTER, 25, 1);
	m_lstAxis.InsertColumn(eLI_PRE_SET_POS,		_T("PreSet(mm)"), LVCFMT_CENTER, 80,1);
	m_lstAxis.InsertColumn(eLI_CHECK,			_T(""), LVCFMT_CENTER, 20, 1);
	m_lstAxis.SetColumnCheck(eLI_CHECK,			_T(""));
	m_lstAxis.InsertColumn(eLI_GANTRY_CH2,		_T("Ch2(mm)"), LVCFMT_CENTER, 65, 1);
	m_lstAxis.InsertColumn(eLI_TORQUE_CH2,		_T("TQ2(%)"), LVCFMT_CENTER, 55, 1);
	 
	m_lstAxis.SetAlignText(eLI_POSITION,	eLCETA_RIGHT);
	m_lstAxis.SetAlignText(eLI_SPEED,		eLCETA_RIGHT);
	m_lstAxis.SetAlignText(eLI_TORQUE,		eLCETA_RIGHT);
	m_lstAxis.SetAlignText(eLI_TORQUE_CH2,	eLCETA_RIGHT);
	
	m_lstAxis.SetAlignText(eLI_GANTRY_CH2,	eLCETA_RIGHT);
	m_lstAxis.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_lstAxis.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	for (int i = 0; i < eMOTOR_END; i++) {
		int iItem = m_lstAxis.GetItemCount();
		m_lstAxis.InsertItem(iItem, _T(""));

		//. 항목 데이터 설정.
		
		// Axis No
		m_lstAxis.SetItemInt(i, eLI_AXIS_NO, i);

		//. Gantry 축일 경우. ch별로 데이터 표기
		if (sm.MotorParameter[i].tYaskawa.bSetGantry) {

			CString strServoOn;
			COLORREF colorServoOn = RGB_RED;

			//. Servo On 상태 여부
			switch (sm.MotorStatus[i].tCrt.tGantryData.dwChServoOnStatus) {

			case eCH_GANTRY_AXIS_1_STATE: {
				//. Ch1만 켜진거니까, Ch2는 off상태란 말임.
				strServoOn += _T("2");
				break;
			}
			case eCH_GANTRY_AXIS_2_STATE: {
				strServoOn += _T("1");
				break;
			}
			case eCH_GANTRY_AXIS_ALL_STATE: {
				colorServoOn = RGB_GREEN;
				break;
			}
			default:
				//. 둘다 안켜짐
				strServoOn += _T("A");
			}

			m_lstAxis.SetItemText(i, eLI_SERVO_ON, strServoOn);
			m_lstAxis.SetBKColor(i, eLI_SERVO_ON, colorServoOn);

			CString strAlarmOn;
			COLORREF colorAlarmOn = RGB_RED;

			//. Alarm On 상태 여부
			switch (sm.MotorStatus[i].tCrt.tGantryData.dwChAlarmOnStatus) {

			case eCH_GANTRY_AXIS_1_STATE: {
				//. Ch1만 켜진거니까, Ch2는 off상태란 말임.
				strAlarmOn += _T("1");
				break;
			}
			case eCH_GANTRY_AXIS_2_STATE: {
				strAlarmOn += _T("2");
				break;
			}
			case eCH_GANTRY_AXIS_ALL_STATE: {
				strAlarmOn += _T("A");
				break;
			}
			default:
				//. 둘다 안켜짐
				colorAlarmOn = RGB_GREEN;
			}

			m_lstAxis.SetItemText(i, eLI_ALARM, strAlarmOn);
			m_lstAxis.SetBKColor(i, eLI_ALARM, colorAlarmOn);

			CString strHome;
			COLORREF colorHomeState = RGB_RED;

			//. Alarm On 상태 여부
			switch (sm.MotorStatus[i].tCrt.tGantryData.dwChHomeStatus) {

			case eCH_GANTRY_AXIS_1_STATE: {
				//. Ch1만 켜진거니까, Ch2는 off상태란 말임.
				strHome += _T("2");
				break;
			}
			case eCH_GANTRY_AXIS_2_STATE: {
				strHome += _T("1");
				break;
			}
			case eCH_GANTRY_AXIS_ALL_STATE: {
				colorHomeState = RGB_GREEN;
				break;
			}
			default:
				//. 둘다 안켜짐
				strHome += _T("A");
			}

			m_lstAxis.SetItemText(i, eLI_HOME, strHome);
			m_lstAxis.SetBKColor(i, eLI_HOME, colorHomeState);

			CString strPLimit;
			COLORREF colorPLimitState = RGB_RED;

			//. Alarm On 상태 여부
			switch (sm.MotorStatus[i].tCrt.tGantryData.dwChPLimitStatus) {

			case eCH_GANTRY_AXIS_1_STATE: {
				//. Ch1만 리밋센서 감지
				strPLimit += _T("1");
				break;
			}
			case eCH_GANTRY_AXIS_2_STATE: {
				strPLimit += _T("2");
				break;
			}
			case eCH_GANTRY_AXIS_ALL_STATE: {

				strPLimit += _T("A");
				break;
			}
			default:
				//. 둘다 안켜짐. 정상.
				colorPLimitState = RGB_GREEN;
			}

			m_lstAxis.SetItemText(i, eLI_P_LIMIT, strPLimit);
			m_lstAxis.SetBKColor(i, eLI_P_LIMIT, colorPLimitState);

			CString strNLimit;
			COLORREF colorNLimitState = RGB_RED;

			//. Alarm On 상태 여부
			switch (sm.MotorStatus[i].tCrt.tGantryData.dwChNLimitStatus) {

			case eCH_GANTRY_AXIS_1_STATE: {
				//. Ch1만 켜진거니까, Ch2는 off상태란 말임.
				strNLimit += _T("1");
				break;
			}
			case eCH_GANTRY_AXIS_2_STATE: {
				strNLimit += _T("2");
				break;
			}
			case eCH_GANTRY_AXIS_ALL_STATE: {

				strNLimit += _T("A");
				break;
			}
			default:
				//. 둘다 감지 안해야 정상.
				colorNLimitState = RGB_GREEN;
			}

			m_lstAxis.SetItemText(i, eLI_N_LIMIT, strNLimit);
			m_lstAxis.SetBKColor(i, eLI_N_LIMIT, colorNLimitState);

			CString strInPos;
			COLORREF colorInPosState = RGB_RED;

			//. Alarm On 상태 여부
			switch (sm.MotorStatus[i].tCrt.tGantryData.dwChInPosStatus) {

			case eCH_GANTRY_AXIS_1_STATE: {
				//. Ch1만 켜진거니까, Ch2는 off상태란 말임.
				strInPos += _T("2");
				break;
			}
			case eCH_GANTRY_AXIS_2_STATE: {
				strInPos += _T("1");
				break;
			}
			case eCH_GANTRY_AXIS_ALL_STATE: {

				colorInPosState = RGB_GREEN;
				break;
			}
			default:
				//. 둘다 안켜짐
				strInPos += _T("A");
			}

			m_lstAxis.SetItemText(i, eLI_INPOSTION, strInPos);
			m_lstAxis.SetBKColor(i, eLI_INPOSTION, colorInPosState);
			m_lstAxis.SetBKColor(i, eLI_GANTRY_CH2, RGB_WHITE);
		}
		else {

			//. 항목 UI 설정.
			m_lstAxis.SetBKColor(iItem, eLI_SERVO_ON, sm.MotorStatus[i].tCrt.bServoOn ? RGB_GREEN : RGB_RED);
			m_lstAxis.SetBKColor(iItem, eLI_ALARM, sm.MotorStatus[i].tCrt.bAlarm ? RGB_RED : RGB_GREEN);
			m_lstAxis.SetBKColor(iItem, eLI_HOME, sm.MotorStatus[i].tCrt.bHomeSensor ? RGB_GREEN : RGB_RED);
			m_lstAxis.SetBKColor(iItem, eLI_P_LIMIT, sm.MotorStatus[i].tCrt.bPLimit ? RGB_RED : RGB_GREEN);
			m_lstAxis.SetBKColor(iItem, eLI_N_LIMIT, sm.MotorStatus[i].tCrt.bNLimit ? RGB_RED : RGB_GREEN);
			m_lstAxis.SetBKColor(iItem, eLI_INPOSTION, sm.MotorStatus[i].tCrt.bInPosition ? RGB_GREEN : RGB_RED);
			m_lstAxis.SetBKColor(i, eLI_GANTRY_CH2, RGB_BLACK);

		}

		m_lstAxis.SetItemText(iItem, eLI_NAME, g_stMotor[i]);

		m_lstAxis.SetItemDouble(iItem, eLI_POSITION, sm.MotorStatus[i].tCrt.dPos);
		m_lstAxis.SetItemDouble(iItem, eLI_SPEED, sm.MotorStatus[i].tCrt.dSpeed);
		m_lstAxis.SetItemDouble(iItem, eLI_TORQUE, sm.MotorStatus[i].tCrt.dTorque,1);
		m_lstAxis.SetItemDouble(iItem, eLI_PRE_SET_POS, sm.MotorStatus[i].tCrt.dPos - sm.MotorStatus[i].tEtc.dPresetPos);
		m_lstAxis.SetItemDouble(iItem, eLI_GANTRY_CH2, sm.MotorStatus[i].tCrt.tGantryData.dCh2Pos);
		m_lstAxis.SetItemDouble(iItem, eLI_TORQUE_CH2, sm.MotorStatus[i].tCrt.tGantryData.dCh2Torque,1);
		

		//. 기본적으로 홈 동작을 안했을시 RED.
		COLORREF dwCurrentHomeStateColor = RGB_RED;


		switch (sm.MotorStatus[i].tEtc.dwHomeStatus) {
		case eHOME_STATUS_NOT_COMPLETED: {
			//. 그게 아니라면 홈 동작 중 상태도 아니고, 홈 동작 완료도 아니므로
			//. 홈 동작 미완료로 초기값인 RED표시
			dwCurrentHomeStateColor = RGB_RED;
			break;
		}
		case eHOME_STATUS_HOMMING: {
			//. 홈 동작 중인경우 YELLOW
			dwCurrentHomeStateColor = RGB_YELLOW;

			break;
		}
		case eHOME_STATUS_COMPLETED: {
			//. 홈 동작이 아닌경우, 홈 동작을 완료했다면 GREEN. 
			dwCurrentHomeStateColor = RGB_GREEN;

			break;
		}
		default: {
			dwCurrentHomeStateColor = RGB_RED;
			break;
		}
		}
		
		m_lstAxis.SetBKColor(iItem, eLI_NAME, dwCurrentHomeStateColor);

		m_MotorStatus[i] = sm.MotorStatus[i].tCrt;
	}
	//2024.06.05 LBH - Overdrive 설정.
	m_iSlider = sm.Register->Reg_Word_1.dwOverdrive / 100;
	
	m_SliderControl.Create(TBS_NOTICKS | TBS_ENABLESELRANGE | TBS_AUTOTICKS | TBS_BOTH , CRect(), this, ID_SLIBER);
	m_SliderControl.SetRange(1, 100);

	m_SliderControl.SetPos(m_iSlider);
	m_SliderControl.SetTicFreq(10);

	return 0;
}

void CAxisControlWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	if (bShow) {
		int iW = 1920 / 2;
		int iH = 1080 / 6;
		SetWindowPos(&wndTopMost, 100, 100, iW, iH*5, SWP_SHOWWINDOW);
		ComputeArea();
		m_iStopButtonClick = 0;

		SetTimer(ID_INTERVAR_UI, 100, NULL);

		m_lstAxis.ShowWindow(SW_SHOW);
		m_lstAxis.SetItemState(m_eSelectedMotor, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

		//2024.06.05 LBH - Overdrive 설정.
		m_iSlider = sm.Register->Reg_Word_1.dwOverdrive / 100;
		m_SliderControl.SetPos(m_iSlider);

		Invalidate(FALSE);

	}
	else
		KillTimer(ID_INTERVAR_UI);
}


void CAxisControlWnd::OnPaint()
{
	CPaintDC dc(this); 
	CMemDCEx::CMemDC		memDC(&dc);
	CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_DEFAULT]);
	CBrush* pOldBrush = (CBrush*)memDC.SelectStockObject(NULL_BRUSH);
	CPen* pOldPen = (CPen*)memDC.SelectObject(&pen);
	CRect					rect = m_rectClient;
	memDC.SelectObject(&g_brushBackground[eEC_DEFAULT]);

	//. 기본색.
	COLORREF crBackGround = DT_BTN_UP_BK;
	
	//. Gantry인 경우 배경색 변경.
	if (sm.MotorParameter[m_eSelectedMotor].tYaskawa.bSetGantry) {

		switch (m_iSelectGantry) {
		case 1: {
			crBackGround = RGB(36, 23, 62);//RGB_VIOLET_DARK;
			break;
		}
		case 2:
			crBackGround = RGB(21, 66, 46);//RGB_MINT_DARK;
			break;
		}
	}
	
	memDC.FillSolidRect(rect, crBackGround);

	DrawMDIMenuButtonN(memDC, m_rectC[eC_MOVE_TYPE_JOG], _T("Jog"), m_iSelectMove == eC_MOVE_TYPE_JOG);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_MOVE_TYPE_STEP], _T("Step"), m_iSelectMove == eC_MOVE_TYPE_STEP);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_MOVE_TYPE_TORQUE], _T("Torque"), m_iSelectMove == eC_MOVE_TYPE_TORQUE);

	if (m_iSelectMove != eC_MOVE_TYPE_TORQUE) {

		LPCTSTR strMoveN = m_iSelectMove == eC_MOVE_TYPE_JOG ? _T("Jog-") : _T("-");
		LPCTSTR strMoveP = m_iSelectMove == eC_MOVE_TYPE_JOG ? _T("Jog+") : _T("+");

		//Draw3dMoveDirect(memDC, m_rectC[eC_JOG_MOVE_N], (m_eSelectControl == eC_JOG_MOVE_N), eMD_LEFT, strMoveN);
		//Draw3dMoveDirect(memDC, m_rectC[eC_JOG_MOVE_P], (m_eSelectControl == eC_JOG_MOVE_P), eMD_RIGHT, strMoveP);
		DrawMDIMoveDirect(memDC, m_rectC[eC_JOG_MOVE_N], (m_eSelectControl == eC_JOG_MOVE_N), false, eMD_LEFT, strMoveN);
		DrawMDIMoveDirect(memDC, m_rectC[eC_JOG_MOVE_P], (m_eSelectControl == eC_JOG_MOVE_P), false, eMD_RIGHT, strMoveP);
	}

	if (m_iSelectMove == eC_MOVE_TYPE_TORQUE) {
		DrawText(memDC, m_rectS[eS_TARGET], _T("Torque (%)"));
	}
	else{
		DrawText(memDC, m_rectS[eS_TARGET], _T("Target (mm)"));
	}
	SelectedDrawEditDouble(memDC, m_rectC[eC_EDIT_TARGET], m_dTarget[m_eSelectedMotor]);

	DrawText(memDC, m_rectS[eS_SPEED], _T("Speed (mm/s)"));
	SelectedDrawEditDouble(memDC, m_rectC[eC_EDIT_SPEED],	m_dSpeed[m_eSelectedMotor]);

	DrawText(memDC, m_rectS[eS_OVERRIDE], _T("Override (%)"));
	SelectedDrawEditInt(memDC, m_rectC[eC_SLIDE], m_SliderControl.GetPos());
	
	LPCTSTR strMove = m_iSelectMove == eC_MOVE_TYPE_TORQUE ? _T("Torque\nMove") : _T("PTP\nMove");
	DrawMDIMenuButtonN(memDC, m_rectC[eC_MOVE], strMove, m_eSelectControl == eC_MOVE);
	
	DrawMDIMenuButtonN(memDC, m_rectC[eC_MOVE_STOP], _T("Stop Move"), m_eSelectControl == eC_MOVE_STOP);

	if (!m_bHide) {
		DrawMDIMenuButtonN(memDC, m_rectC[eC_LINER_MOVE], _T("L-Move"), m_eSelectControl == eC_LINER_MOVE);
		DrawMDIMenuButtonN(memDC, m_rectC[eC_MULTI_MOVE], _T("M-Move"), m_eSelectControl == eC_MULTI_MOVE);
	}
	//DrawButtonUpDown(memDC, m_rectC[eC_GEAR_MOVE], _T("G-Move"), m_eSelectControl == eC_GEAR_MOVE);


	//<< RightSide UI
	if (m_bMaximumPage)
		DrawMDIMenuButtonN(memDC, m_rectC[eC_MAXIMUM_WINDOW], _T("Maximum"), m_eSelectControl == eC_MAXIMUM_WINDOW);
	else 
		SelectedDrawText(memDC, m_rectC[eC_MAXIMUM_WINDOW], _T("Maximum"), 9, RGB_GREEN, RGB_GREEN, RGB_BLACK);
	
	DrawMDIMenuButtonN(memDC, m_rectC[eC_PRESET], _T("Preset"), m_eSelectControl == eC_PRESET);			
	SelectedDrawEditDouble(memDC, m_rectC[eC_CUR_PRESET], sm.MotorStatus[m_eSelectedMotor].tEtc.dPresetPos);
	
	SelectedDrawDouble(memDC, m_rectS[eS_LOOP], (g_dwTick)/1000.);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_INIT], _T("Init"), m_eSelectControl == eC_INIT);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_INIT_STOP], _T("Init Stop"), m_eSelectControl == eC_INIT_STOP);
	
	DrawMDIMenuButtonN(memDC, m_rectC[eC_ALARM_CLEAR], _T("Alarm Clear"), m_eSelectControl == eC_ALARM_CLEAR);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_SERVO_ON		], _T("Servo On"),	 sm.MotorStatus[m_eSelectedMotor].tCrt.bServoOn);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_SERVO_OFF		], _T("Servo Off"),	 !sm.MotorStatus[m_eSelectedMotor].tCrt.bServoOn);
	
	DrawMDIMenuButtonN(memDC, m_rectC[eC_ALL_ALARM_CLEAR], _T("All Alarm Clear"), m_eSelectControl == eC_ALL_ALARM_CLEAR);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_ALL_SERVO_ON], _T("All Servo On"),	m_eSelectControl == eC_ALL_SERVO_ON);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_ALL_SERVO_OFF], _T("All Servo Off"), m_eSelectControl == eC_ALL_SERVO_OFF);
	
	//<< Gantry
	if (sm.MotorParameter[m_eSelectedMotor].tYaskawa.bSetGantry) {
		DrawGroupBox(memDC, m_rectS[eS_GANTRY], _T("Gantry"), RGB_WHITE, RGB_WHITE);

		BOOL isSelectedChAll = m_iSelectGantry == 0;
		BOOL isSelectedCh1	 = m_iSelectGantry == 1;
		BOOL isSelectedCh2	 = m_iSelectGantry == 2;

		COLORREF crIsSelectedChAll = isSelectedChAll ? DT_BTN_DOWN_BK : DT_BTN_UP_BK;
		COLORREF crIsSelectedCh1 = isSelectedCh1 ? RGB_VIOLET_DARK : DT_BTN_DOWN_BK;
		COLORREF crIsSelectedCh2 = isSelectedCh2 ? RGB_MINT_DARK : DT_BTN_DOWN_BK;
		
		DrawMDIMenuButtonN(memDC, m_rectC[eC_GANTRY_ALL], _T("ALL"), isSelectedChAll);
		DrawMDIMenuButtonN(memDC, m_rectC[eC_GANTRY_CH1], _T("Ch1"), isSelectedCh1);//, crIsSelectedCh1, DT_BTN_UP_BK);
		DrawMDIMenuButtonN(memDC, m_rectC[eC_GANTRY_CH2], _T("Ch2"), isSelectedCh2);//, crIsSelectedCh2, DT_BTN_UP_BK);

		//. 경고 레벨에 따라 색 지정.

		COLORREF crWarningLevel = DT_BTN_UP_BK;

		if (m_MotorStatusETC[m_eSelectedMotor].dwHomeStatus == eHOME_STATUS_COMPLETED) {

			switch (m_MotorStatusETC[m_eSelectedMotor].tGantryData.dwGantryWarningLevel) {
			case eCH_GANTRY_WARNING_LEVEL_WARNING: {
				crWarningLevel = RGB_ORANGE_LIGHT;
				break;
			}
			case eCH_GANTRY_WARNING_LEVEL_ALERT: {
				crWarningLevel = RGB_RED;
				break;
			}
											   //. 중복되긴 하지만 명확하게 목록을 알려주는게 더 낫다.
			case eCH_GANTRY_WARNING_LEVEL_NONE: {
				crWarningLevel = DT_BTN_UP_BK;
			}
			}
		}
		
		DrawMDIMenuButtonN(memDC, m_rectC[eC_GANTRY_OFFSET_NORMALIZE], _T("편차 맞춤"), m_eSelectControl == eC_GANTRY_OFFSET_NORMALIZE);// , DT_BTN_DOWN_BK, crWarningLevel);

		LPCTSTR pszGantryChState = NULL;
		switch (m_iSelectGantry) {
		case 0: {
			pszGantryChState = _T("Gantry Ch 전체 모드입니다.");
			break;
		}
		case 1: {
			pszGantryChState = _T("Gantry Ch 1 모드입니다.");
			break;
		}
		case 2: {
			pszGantryChState = _T("Gantry Ch 2 모드입니다.");
			break;
		}
		}
		DrawText(memDC, m_rectS[eS_GANTRY_CH_STATE], pszGantryChState);
	}
	//>>

	DrawMDIMenuButtonN(memDC, m_rectC[eC_CLOSE	], _T("Close"), m_eSelectControl == eC_CLOSE);
	//>>

	memDC.SelectObject(pOldBrush);
	memDC.SelectObject(pOldPen);	
	
}

void CAxisControlWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_rectC[m_eSelectControl].PtInRect(point)){
		switch( m_eSelectControl){
			case eC_JOG_MOVE_N:
			case eC_JOG_MOVE_P: {
				CIPVar var;
				var.SetChildItem("Axis", m_eSelectedMotor);
				__POST_IP_COMMAND__("Stage", "MoveJogStop", var);
			}
		}
		m_eSelectControl = eC_NONE;
	}

	CFrameWnd::OnMouseMove(nFlags, point);
}

void CAxisControlWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	int iC[] = {
		eC_JOG_MOVE_N,
		eC_JOG_MOVE_P,
	};

	BOOL bNotSafety = FALSE;
	for (int i = 0; i < countof(iC); i++) {
		if (m_rectC[iC[i]].PtInRect(point) && sm.Option->tSequence.eDriveMode != eDRIVE_MODE_MANUAL) {
			bNotSafety = TRUE;
			break;
		}
	}

	if (bNotSafety) {
		IPMessage(g_st[IDS_MS_NOT_DRIVE_MODE_MANUAL]);
		return;
	}

	m_iHideCount = 0;
	for (int i = 0; i < eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			InvalidateRect(m_rectC[i], FALSE);
			switch(m_eSelectControl){
				case eC_JOG_MOVE_N:
				case eC_JOG_MOVE_P: {
					switch (m_iSelectMove) {
					//case eC_PTP:
					case eC_MOVE_TYPE_JOG:{
						switch (m_iSelectGantry) {
							case 0: {sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_ALL; break; }
							case 1: {sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_1; 	break; }
							case 2: {sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_2; 	break; }
						}
							BOOL bDir = m_eSelectControl - eC_JOG_MOVE_N;
							CIPVar var;
							var.SetChildItem("Axis", m_eSelectedMotor);
							var.SetChildItem("Speed", m_dSpeed[m_eSelectedMotor]);
							var.SetChildItem("Dir", bDir);
							var.SetChildItem("Ch", m_iSelectGantry);

							GetCheckAxis(var);

							__POST_IP_COMMAND__("Stage", "MoveJog", var);
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}
	CFrameWnd::OnLButtonDown(nFlags, point);
}


void CAxisControlWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		//<< KES 추가 2024.05.31 - 운전모드가 Manual이 아니면 수동 동작 X
		if (sm.Option->tSequence.eDriveMode != eDRIVE_MODE_MANUAL) {
			if (m_eSelectControl != eC_MOVE_TYPE_STEP && m_eSelectControl != eC_MOVE_TYPE_JOG && m_eSelectControl != eC_MOVE_TYPE_TORQUE && m_eSelectControl != eC_CLOSE) {
				IPMessage(g_st[IDS_MS_NOT_DRIVE_MODE_MANUAL]);
				return;
			}
		}
		//>>

		switch (m_eSelectControl) {
			//case eC_PTP: 
			case eC_MOVE_TYPE_STEP: 
			case eC_MOVE_TYPE_JOG:
			case eC_MOVE_TYPE_TORQUE:
			{
				m_iSelectMove = m_eSelectControl;
				if (m_iSelectMove == eC_MOVE_TYPE_JOG)
					strLog = _T("Jog");
				if (m_iSelectMove == eC_MOVE_TYPE_STEP)
					strLog = _T("Step");
				if (m_iSelectMove == eC_MOVE_TYPE_TORQUE)
					strLog = _T("Torque");
				break;
			}
			case eC_EDIT_SPEED: {
				EditStart(this, m_rectC[m_eSelectControl], m_dSpeed[m_eSelectedMotor], FALSE, FALSE, 0., 100.);
				break;
			}
			case eC_EDIT_TARGET: {
				EditStart(this, m_rectC[m_eSelectControl], m_dTarget[m_eSelectedMotor]);
				break;
			}
			case eC_MOVE: {
				if (m_iSelectMove == eC_MOVE_TYPE_TORQUE)
					strLog = _T("Torque Move");
				else 
					strLog = _T("PTP Move");
				m_iStopButtonClick = 0;
				switch (m_iSelectGantry) {
					case 0: {sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_ALL; break; }
					case 1: {sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_1; 	break; }
					case 2: {sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_2; 	break; }
				}
				if( m_eSelectedMotor < 0){
					return;
				}
				switch( m_iSelectMove){
					default:{
						CStringA strPTP;
						strPTP.Format("PTPMove%d", m_eSelectedMotor);
						CIPVar var;
						var.SetChildItem("Axis", m_eSelectedMotor);
						var.SetChildItem("Speed", m_dSpeed[m_eSelectedMotor]);
						var.SetChildItem("Target", m_dTarget[m_eSelectedMotor]);
						var.SetChildItem("Ch", m_iSelectGantry);
					
						__POST_IP_COMMAND__("Stage", strPTP, var);
						break;
					}
					//case eC_STEP:{
					//	CIPVar var;
					//	var.SetChildItem("Axis", m_eSelectedMotor);
					//	var.SetChildItem("Speed", m_dSpeed[m_eSelectedMotor]);
					//	var.SetChildItem("Distacne", m_dTarget[m_eSelectedMotor]);
					//	var.SetChildItem("Ch", m_iSelectGantry);
					//
					//	__POST_IP_COMMAND__("Stage", "MoveStep", var);
					//	break;
					//}
					case eC_MOVE_TYPE_TORQUE:{
						ys.TorqueMove(m_eSelectedMotor, m_dTarget[m_eSelectedMotor], m_dSpeed[m_eSelectedMotor]);
						//CIPVar var;
						//var.SetChildItem("Axis", m_eSelectedMotor);
						//var.SetChildItem("Speed", m_dSpeed[m_eSelectedMotor]);
						//var.SetChildItem("Target", m_dTarget[m_eSelectedMotor]);
						//__POST_IP_COMMAND__("Stage", "TorqueMove", var);
						break;
					}
				}
				break;
			}


			case eC_JOG_MOVE_N:
			case eC_JOG_MOVE_P: {
				if (m_iSelectMove == eC_MOVE_TYPE_JOG)
					strLog = Format(_T("Jog %s"), m_eSelectControl == eC_JOG_MOVE_N ? _T("-") : _T("+"));
				else
					strLog = Format(_T("%s"), m_eSelectControl == eC_JOG_MOVE_N ? _T("-") : _T("+"));

				m_iStopButtonClick = 0;
				switch (m_iSelectGantry) {
					case 0: {sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_ALL; 	break; }
					case 1: {sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_1; 	break; }
					case 2: {sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_2; 	break; }
				}
				switch (m_iSelectMove) {
				//case eC_PTP: 
				case eC_MOVE_TYPE_JOG: {
					CIPVar var;
					var.SetChildItem("Axis", m_eSelectedMotor);
					var.SetChildItem("Ch", m_iSelectGantry);

					GetCheckAxis(var);
					__POST_IP_COMMAND__("Stage", "MoveJogStop", var);

					break;
				}
				case eC_MOVE_TYPE_STEP: {
					CIPVar var;
					var.SetChildItem("Axis", m_eSelectedMotor);
					var.SetChildItem("Speed", m_dSpeed[m_eSelectedMotor]);
					if (m_eSelectControl - eC_JOG_MOVE_P) {
						var.SetChildItem("Distacne", m_dTarget[m_eSelectedMotor] * -1);
					}
					else {
						var.SetChildItem("Distacne", m_dTarget[m_eSelectedMotor]);
					}
					var.SetChildItem("Ch", m_iSelectGantry);

					__POST_IP_COMMAND__("Stage", "MoveStep", var);

					break;
				}
				}
				break;
			}
			
			case eC_MOVE_STOP:{
				strLog = _T("Stop Move");
				m_iStopButtonClick++;
				if (m_iStopButtonClick > 3) {
					m_iStopButtonClick = 0;
					ys.MotionStop(m_eSelectedMotor);
				}
				else {
				if(m_iSelectMove == eC_MOVE_TYPE_TORQUE){
						ys.MotionStop(m_eSelectedMotor);
					}else{
						CIPVar var;
						var.SetChildItem("Axis", m_eSelectedMotor);
						__POST_IP_COMMAND__("Stage", "MoveStop", var);
					}
				}
				break;
			}
			case eC_LINER_MOVE:{
				strLog = _T("Liner Move");
				if (m_bHide)
					break;
				if( m_eSelectedMotor >= 0){
					CIPVar var;
					GetCheckAxis(var);
					var.SetChildItem("Speed",m_dSpeed[m_eSelectedMotor]);
					__POST_IP_COMMAND__("Stage", "MoveLiner", var);
				}
				break;
			}
			case eC_MULTI_MOVE:{
				strLog = _T("Multi Move");
				if (m_bHide)
					break;
				CIPVar var;
				GetCheckAxis(var);
				__POST_IP_COMMAND__("Stage", "MoveMulti", var);
				break;
			}
			case eC_SLIDE: {
				EditStart(this, m_rectC[m_eSelectControl], m_iSlider, TRUE, FALSE, 1, 100);
				for(int i =0; i < eMOTOR_END; i++){
					sm.MotorStatus[i].tEtc.iOverride = m_iSlider;
				}
				break;
			}
			case eC_CLOSE: {
				strLog = _T("Close");
				ShowWindow(SW_HIDE);
				break;
			}
			case eC_INIT: {
				strLog = _T("Init");
				CIPVar var;
				var.SetChildItem("Axis", m_eSelectedMotor);
				__POST_IP_COMMAND__("Stage", "Homming", var);
		
				break;
			}
			case eC_INIT_STOP: {
				strLog = _T("Init Stop");
				if (!g_ipClient.ResumeIPStepCommandByName("Stage", "HommingTorque", IPPS_RESERVE_ERROR)) {
					TRACE(_T("Init Stop - HommingTorque failed\n"));
				}
				if (!g_ipClient.ResumeIPStepCommandByName("Stage", "HommingHome", IPPS_RESERVE_ERROR)) {
					TRACE(_T("Init Stop - Homming failed\n"));
				}
				
				if (!g_ipClient.ResumeIPStepCommandByName("Stage", "HommingNPSensor", IPPS_RESERVE_ERROR)) {
					TRACE(_T("Init Stop - HommingNPSensor - NP_CENTER failed\n"));
				}

				break;
			}
			case eC_ALARM_CLEAR: {
				strLog = _T("Alarm Clear");
				sm.Register->Out_New.dw1000_AlarmReset = TRUE;
				//ys.ClearServoAlarm(m_eSelectedMotor);
				break;
			}
			case eC_ALL_ALARM_CLEAR: {
				strLog = _T("All Alarm Clear");
				sm.Register->Out_New.dw1000_AlarmReset = TRUE;
				
				//for (int i = 0; i < eMOTOR_END; ++i) {
				//	sm.MotorParameter[i].tYaskawa.iGantryCh = eCH_AXIS_ALL;
				//	ys.ClearServoAlarm(i);
				//}

				break;
			}
			case eC_MAXIMUM_WINDOW: {
				strLog = _T("Maximum Window");
				m_bMaximumPage = !m_bMaximumPage;
				if (!m_bMaximumPage) {
					int iW = 1920 / 2;
					int iH = 1080 / 6;
					SetWindowPos(&wndTopMost, 0, 0, iW, iH * 5, SWP_SHOWWINDOW);
				}
				break;
			}
			case eC_SERVO_ON:{
				strLog = _T("Servo On");
				ServoOnOff(m_eSelectedMotor, TRUE);
				
				break;
			}
			case eC_SERVO_OFF	:{
				strLog = _T("Servo Off");
				ServoOnOff(m_eSelectedMotor, FALSE);
				break;
			}
			case eC_ALL_SERVO_OFF:
			{
				strLog = _T("All Servo Off");
				AllServoOnOff(FALSE);
				//for (int i = 0; i < eMOTOR_END; i++) {
				//	sm.MotorParameter[i].tYaskawa.iGantryCh = eCH_AXIS_ALL;
				//	ServoOnOff(m_eSelectedMotor, FALSE);
				//}
				break;
			}
			case eC_ALL_SERVO_ON: {
				strLog = _T("All Servo On");
				CString strResult = RespMessage(_T("All Servo On 하시겠습니까?"), _T("OK;CANCLE"));
				if (strResult.CompareNoCase(_T("OK")) == 0)
				{
					AllServoOnOff(TRUE);
					//for (int i = 0; i < eMOTOR_END; i++) {
					//	sm.MotorParameter[i].tYaskawa.iGantryCh = eCH_AXIS_ALL;
					//	ServoOnOff(m_eSelectedMotor, TRUE);
					//}
					break;
				}
				else 
					break;
			}
			case eC_GANTRY_ALL : 
			case eC_GANTRY_CH1 : 
			case eC_GANTRY_CH2: {
				m_iSelectGantry = m_eSelectControl - eC_GANTRY_ALL;
				switch (m_iSelectGantry) {
					case 0: {sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_ALL; strLog = _T("GANTRY ALL");	break; }
					case 1: {sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_1; 	strLog = _T("GANTRY CH1");	break; }
					case 2: {sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_2; 	strLog = _T("GANTRY CH2");	break; }
				}
				break;
			}
			case eC_GANTRY_OFFSET_NORMALIZE:
			{
				strLog = _T("GANTRY Offset Normalize");

				if (!sm.MotorParameter[m_eSelectedMotor].tYaskawa.bSetGantry) break;
				if (sm.MotorStatus[m_eSelectedMotor].tEtc.dwHomeStatus != eHOME_STATUS_COMPLETED) break;
				//. 둘 다 켜져있지 않으면 탈출.
				if (sm.MotorStatus[m_eSelectedMotor].tCrt.tGantryData.dwChServoOnStatus != eCH_GANTRY_AXIS_ALL_STATE) break;

				
				CIPVar var;
				var.SetChildItem("Axis", m_eSelectedMotor);
				var.SetChildItem("Speed", 20.);
				//var.SetChildItem("Target", sm.MotorStatus[m_eSelectedMotor].tCrt.tGantryData.dCh1Pos); // 이 값이 아니네..?
				var.SetChildItem("Target", sm.MotorStatus[m_eSelectedMotor].tCrt.tGantryData.dCh1Pos);
				sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_2;
				__POST_IP_COMMAND__("Stage", "PTPMove", var);
			}
			break;

			case eC_PRESET: {
				strLog = _T("Preset");

				sm.MotorStatus[m_eSelectedMotor].tEtc.dPresetPos = GetAxisPos(m_eSelectedMotor);
				m_lstAxis.SetItemDouble(m_eSelectedMotor, eLI_PRE_SET_POS, sm.MotorStatus[m_eSelectedMotor].tCrt.dPos - sm.MotorStatus[m_eSelectedMotor].tEtc.dPresetPos);
				break;
			}
			case eC_CUR_PRESET: {
				EditStart(this, m_rectC[m_eSelectControl], sm.MotorStatus[m_eSelectedMotor].tEtc.dPresetPos);
				break;
			}
			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(_T("PLCLinker"), _T("AxisControl"), g_stMotor[m_eSelectedMotor], strLog, sm.DeviceParameter->szCurUserID);
	}
if (m_eSelectControl != eC_NONE) {

		Invalidate(FALSE);
		//InvalidateRect(m_rectS[eS_RIGHT_SIDE_AREA], FALSE);
		//InvalidateRect(m_rectS[eS_BOTTOM_SIDE_AREA], FALSE);
	}
	
	
	m_eSelectControl = eC_NONE;
	CFrameWnd::OnLButtonUp(nFlags, point);
}
BOOL CAxisControlWnd::GetCheckAxis(CIPVar& AxisData){
	BOOL bReturn = TRUE;
	int iCount =0;
	
	
	for(int i = 0 ; i < eMOTOR_END ; i++){
		if( m_lstAxis.GetCheck(i, eLI_CHECK)){
			CStringA strAxis, strTarget, strSPD;
			strAxis.Format("Axis%d", iCount);
			strSPD.Format("Speed%d", iCount);
			strTarget.Format("Target%d", iCount);
					
			AxisData.SetChildItem(strAxis, i);
			AxisData.SetChildItem(strSPD, m_dSpeed[i]);
			AxisData.SetChildItem(strTarget, m_dTarget[i]);
			
			iCount++;
		}
	}
	AxisData.SetChildItem("Count", iCount);

	return bReturn;
}

void CAxisControlWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
		case ID_INTERVAR_UI: {

			if (sm.MotorStatus[m_eSelectedMotor].tEtc.dwHomeStatus == eHOME_STATUS_COMPLETED) {
				if (m_MotorStatusETC[m_eSelectedMotor].tGantryData.dwGantryWarningLevel != sm.MotorStatus[m_eSelectedMotor].tEtc.tGantryData.dwGantryWarningLevel) {
					InvalidateRect(m_rectC[eC_GANTRY_OFFSET_NORMALIZE]);
				}
			}

			for (int i = eMOTOR_0; i < eMOTOR_END; i++) {
				//. Gantry 축일 경우. ch별로 데이터 표기
				if (sm.MotorParameter[i].tYaskawa.bSetGantry) {

					if (m_MotorStatus[i].tGantryData.dwChServoOnStatus != sm.MotorStatus[i].tCrt.tGantryData.dwChServoOnStatus) {

						CString strServoOn;
						COLORREF colorServoOn = RGB_RED;

						//. Servo On 상태 여부
						switch (sm.MotorStatus[i].tCrt.tGantryData.dwChServoOnStatus) {

						case eCH_GANTRY_AXIS_1_STATE: {
							//. Ch1만 켜진거니까, Ch2는 off상태란 말임.
							strServoOn += _T("2");
							break;
						}
						case eCH_GANTRY_AXIS_2_STATE: {
							strServoOn += _T("1");
							break;
						}
						case eCH_GANTRY_AXIS_ALL_STATE: {
							//. 둘다 servo on이어야 정상.
							colorServoOn = RGB_GREEN;
							break;
						}
						default:
							//. 둘다 안켜짐
							strServoOn += _T("A");
						}
						
						m_lstAxis.SetItemText(i, eLI_SERVO_ON, strServoOn);
						m_lstAxis.SetBKColor(i, eLI_SERVO_ON, colorServoOn);
					}

					if (m_MotorStatus[i].tGantryData.dwChAlarmOnStatus != sm.MotorStatus[i].tCrt.tGantryData.dwChAlarmOnStatus) {

						CString strAlarmOn;
						COLORREF colorAlarmOn = RGB_RED;

						//. Alarm On 상태 여부
						switch (sm.MotorStatus[i].tCrt.tGantryData.dwChAlarmOnStatus) {

						case eCH_GANTRY_AXIS_1_STATE: {
							//. Ch1만 켜진거니까, Ch2는 off상태란 말임.
							strAlarmOn += _T("1");
							break;
						}
						case eCH_GANTRY_AXIS_2_STATE: {
							strAlarmOn += _T("2");
							break;
						}
						case eCH_GANTRY_AXIS_ALL_STATE: {
							strAlarmOn += _T("A");
							break;
						}
						default:
							//. 둘다 안켜짐
							colorAlarmOn = RGB_GREEN;
						}

						m_lstAxis.SetItemText(i, eLI_ALARM, strAlarmOn);
						m_lstAxis.SetBKColor(i, eLI_ALARM, colorAlarmOn);
					}

					if (m_MotorStatus[i].tGantryData.dwChHomeStatus != sm.MotorStatus[i].tCrt.tGantryData.dwChHomeStatus) {

						CString strHome;
						COLORREF colorHomeState = RGB_RED;

						//. Alarm On 상태 여부
						switch (sm.MotorStatus[i].tCrt.tGantryData.dwChHomeStatus) {

						case eCH_GANTRY_AXIS_1_STATE: {
							//. Ch1만 켜진거니까, Ch2는 off상태란 말임.
							strHome += _T("2");
							break;
						}
						case eCH_GANTRY_AXIS_2_STATE: {
							strHome += _T("1");
							break;
						}
						case eCH_GANTRY_AXIS_ALL_STATE: {
							colorHomeState = RGB_GREEN;
							break;
						}
						default:
							//. 둘다 안켜짐
							strHome += _T("A");
						}

						m_lstAxis.SetItemText(i, eLI_HOME, strHome);
						m_lstAxis.SetBKColor(i, eLI_HOME, colorHomeState);
					}
					if (m_MotorStatus[i].tGantryData.dwChPLimitStatus!= sm.MotorStatus[i].tCrt.tGantryData.dwChPLimitStatus) {

						CString strPLimit;
						COLORREF colorPLimitState = RGB_RED;

						//. Alarm On 상태 여부
						switch (sm.MotorStatus[i].tCrt.tGantryData.dwChPLimitStatus) {

						case eCH_GANTRY_AXIS_1_STATE: {
							//. Ch1만 켜진거니까, Ch2는 off상태란 말임.
							strPLimit += _T("1");
							break;
						}
						case eCH_GANTRY_AXIS_2_STATE: {
							strPLimit += _T("2");
							break;
						}
						case eCH_GANTRY_AXIS_ALL_STATE: {
							
							//. 둘 다 들어옴.
							strPLimit += _T("A");
							break;
						}
						default:
							//. 둘다 센서 감지 안됨. 정상.
							colorPLimitState = RGB_GREEN;
						}

						m_lstAxis.SetItemText(i, eLI_P_LIMIT, strPLimit);
						m_lstAxis.SetBKColor(i, eLI_P_LIMIT, colorPLimitState);
					}
					if (m_MotorStatus[i].tGantryData.dwChNLimitStatus != sm.MotorStatus[i].tCrt.tGantryData.dwChNLimitStatus) {

						CString strNLimit;
						COLORREF colorNLimitState = RGB_RED;

						//. Alarm On 상태 여부
						switch (sm.MotorStatus[i].tCrt.tGantryData.dwChNLimitStatus) {

						case eCH_GANTRY_AXIS_1_STATE: {
							//. Ch1만 켜진거니까, Ch2는 off상태란 말임.
							strNLimit += _T("1");
							break;
						}
						case eCH_GANTRY_AXIS_2_STATE: {
							strNLimit += _T("2");
							break;
						}
						case eCH_GANTRY_AXIS_ALL_STATE: {

							//. 둘 다 들어옴.
							strNLimit += _T("A");
							break;
						}
						default:
							//. 둘다 센서 감지 안됨. 정상.
							colorNLimitState = RGB_GREEN;
						}

						m_lstAxis.SetItemText(i, eLI_N_LIMIT, strNLimit);
						m_lstAxis.SetBKColor(i, eLI_N_LIMIT, colorNLimitState);
					}
					if (m_MotorStatus[i].tGantryData.dwChInPosStatus != sm.MotorStatus[i].tCrt.tGantryData.dwChInPosStatus) {

						CString strInPos;
						COLORREF colorInPosState = RGB_RED;

						//. Alarm On 상태 여부
						switch (sm.MotorStatus[i].tCrt.tGantryData.dwChInPosStatus) {

						case eCH_GANTRY_AXIS_1_STATE: {
							//. Ch1만 켜진거니까, Ch2는 off상태란 말임.
							strInPos += _T("2");
							break;
						}
						case eCH_GANTRY_AXIS_2_STATE: {
							strInPos += _T("1");
							break;
						}
						case eCH_GANTRY_AXIS_ALL_STATE: {

							colorInPosState = RGB_GREEN;
							break;
						}
						default:
							//. 둘다 안켜짐
							strInPos += _T("A");
						}

						m_lstAxis.SetItemText(i, eLI_INPOSTION, strInPos);
						m_lstAxis.SetBKColor(i, eLI_INPOSTION, colorInPosState);
					}
				}
				else {

					if (m_MotorStatus[i].bAlarm != sm.MotorStatus[i].tCrt.bAlarm)
					{
						m_lstAxis.SetItemText(i, eLI_ALARM, _T(""));
						m_lstAxis.SetBKColor(i, eLI_ALARM, sm.MotorStatus[i].tCrt.bAlarm ? RGB_RED : RGB_GREEN);
					}


					if (m_MotorStatus[i].bServoOn != sm.MotorStatus[i].tCrt.bServoOn)
					{
						m_lstAxis.SetItemText(i, eLI_SERVO_ON, _T(""));
						m_lstAxis.SetBKColor(i, eLI_SERVO_ON, sm.MotorStatus[i].tCrt.bServoOn ? RGB_GREEN : RGB_RED);
					}

					if (m_MotorStatus[i].bHomeSensor != sm.MotorStatus[i].tCrt.bHomeSensor)
					{
						m_lstAxis.SetItemText(i, eLI_HOME, _T(""));
						m_lstAxis.SetBKColor(i, eLI_HOME, sm.MotorStatus[i].tCrt.bHomeSensor ? RGB_GREEN : RGB_RED);
					}

					if (m_MotorStatus[i].bPLimit != sm.MotorStatus[i].tCrt.bPLimit)
					{
						m_lstAxis.SetItemText(i, eLI_P_LIMIT, _T(" "));
						m_lstAxis.SetBKColor(i, eLI_P_LIMIT, sm.MotorStatus[i].tCrt.bPLimit ? RGB_RED : RGB_GREEN);
					}
					if (m_MotorStatus[i].bNLimit != sm.MotorStatus[i].tCrt.bNLimit)
					{
						m_lstAxis.SetItemText(i, eLI_N_LIMIT, _T(" "));
						m_lstAxis.SetBKColor(i, eLI_N_LIMIT, sm.MotorStatus[i].tCrt.bNLimit ? RGB_RED : RGB_GREEN);
					}
					if (m_MotorStatus[i].bInPosition != sm.MotorStatus[i].tCrt.bInPosition)
					{
						m_lstAxis.SetItemText(i, eLI_INPOSTION, _T(" "));
						m_lstAxis.SetBKColor(i, eLI_INPOSTION, sm.MotorStatus[i].tCrt.bInPosition ? RGB_GREEN : RGB_RED);
					}
				}

				//<< 
				if (m_MotorStatusETC[i].dwHomeStatus != sm.MotorStatus[i].tEtc.dwHomeStatus) {

					switch (sm.MotorStatus[i].tEtc.dwHomeStatus) {
					case eHOME_STATUS_NOT_COMPLETED: {

						
						m_lstAxis.SetBKColor(i, eLI_NAME, RGB_RED);
						m_lstAxis.SetItemText(i, eLI_NAME, g_stMotor[i]);
						break;
					}
					case eHOME_STATUS_HOMMING: {
						
						m_lstAxis.SetBKColor(i, eLI_NAME, RGB_YELLOW);
						m_lstAxis.SetItemText(i, eLI_NAME, g_stMotor[i]);
						break;
					}
					case eHOME_STATUS_COMPLETED: {
						
						m_lstAxis.SetBKColor(i, eLI_NAME, RGB_GREEN);
						m_lstAxis.SetItemText(i, eLI_NAME, g_stMotor[i]);

						break;
					}
					}
				}				//>>

				
				if (fabs(m_MotorStatus[i].dSpeed - sm.MotorStatus[i].tCrt.dSpeed) > 0.001)
				{
					m_lstAxis.SetItemDouble(i, eLI_SPEED, sm.MotorStatus[i].tCrt.dSpeed);
				}
				if (fabs(m_MotorStatus[i].dTorque - sm.MotorStatus[i].tCrt.dTorque) > 0.001)
				{
					m_lstAxis.SetItemDouble(i, eLI_TORQUE, sm.MotorStatus[i].tCrt.dTorque, 1);
				}
				if (fabs(m_MotorStatus[i].dPos - sm.MotorStatus[i].tCrt.dPos) > 0.001)
				{
					m_lstAxis.SetItemDouble(i, eLI_POSITION,	sm.MotorStatus[i].tCrt.dPos);
					m_lstAxis.SetItemDouble(i, eLI_PRE_SET_POS, sm.MotorStatus[i].tCrt.dPos - sm.MotorStatus[i].tEtc.dPresetPos);
				}
				if(sm.MotorParameter[i].tYaskawa.bSetGantry){
					if (fabs(m_MotorStatus[i].tGantryData.dCh2Pos - sm.MotorStatus[i].tCrt.tGantryData.dCh2Pos) > 0.001)
					{
						m_lstAxis.SetItemDouble(i, eLI_GANTRY_CH2, sm.MotorStatus[i].tCrt.tGantryData.dCh2Pos);
					}
				}

				if (m_bIsGantry[i] != sm.MotorParameter[i].tYaskawa.bSetGantry) {
					
					m_lstAxis.SetBKColor(i, eLI_GANTRY_CH2, sm.MotorParameter[i].tYaskawa.bSetGantry ? RGB_WHITE : RGB_BLACK);
				}

				if (fabs(m_MotorStatus[i].tGantryData.dCh2Torque - sm.MotorStatus[i].tCrt.tGantryData.dCh2Torque) > 0.001)
				{
					m_lstAxis.SetItemDouble(i, eLI_TORQUE_CH2, sm.MotorStatus[i].tCrt.tGantryData.dCh2Torque, 1);
				}

				m_MotorStatus[i] = sm.MotorStatus[i].tCrt;
				m_MotorStatusETC[i] = sm.MotorStatus[i].tEtc;
			}
			InvalidateRect(m_rectC[eC_SLIDE], FALSE);
			InvalidateRect(m_rectS[eS_LOOP], FALSE);
			
			InvalidateRect(m_rectC[eC_SERVO_ON], FALSE);
			InvalidateRect(m_rectC[eC_SERVO_OFF], FALSE);
			m_SliderControl.SetPos(m_iSlider);
			
			// 2021.03.22 AutoRun 중에 동작 막음
			if (sm.Sequence->fAutoMode && !sm.Sequence->fCycleStop)
			{
				ShowWindow(SW_HIDE);
			}
		}
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CAxisControlWnd::ComputeArea() {
		
	if (sm.DeviceParameter->eUserGrade < eUSER_ADMIN)
	{
		for (int i = eS_NONE; i < eS_END; i++)
			m_rectS[i] = CRect();
		for (int i = 0; i < eC_END; i++)
			m_rectC[i] = CRect();
	}
	GetClientRect(m_rectClient);
	CRect rc = m_rectClient;
	double dH, dW;
	dW = (rc.Width()-5) / 8.;
	dH = (rc.Height()-5) / 20.;

	m_rectS[eS_LIST_AREA]			= CRect(rc.left+dW*0   +5 ,rc.top+dH*0  +5 , rc.left+dW*6.5 ,rc.top+dH*16);
	m_rectS[eS_RIGHT_SIDE_AREA]		= CRect(rc.left+dW*6.5 +5 ,rc.top+dH*0  +5 , rc.left+dW*8   ,rc.top+dH*20);
	m_rectS[eS_BOTTOM_SIDE_AREA]	= CRect(rc.left+dW*0   +5 ,rc.top+dH*16 +5 , rc.left+dW*6.5 ,rc.top+dH*20);
	
	rc = m_rectS[eS_BOTTOM_SIDE_AREA];
	dW = (rc.Width() -5 ) / 6.;
	dH = (rc.Height() -5) / 4.;

	//m_rectC[eC_PTP]			= CRect(rc.left+dW*0+5 ,rc.top+dH*16+5,rc.left+dW*2 ,rc.top+dH*17);
	m_rectC[eC_MOVE_TYPE_JOG			] = CRect(rc.left+dW*0+5   ,rc.top+dH*0+5,rc.left+dW*2   ,rc.top+dH*1);
	m_rectC[eC_MOVE_TYPE_STEP			] = CRect(rc.left+dW*2+5   ,rc.top+dH*0+5,rc.left+dW*4   ,rc.top+dH*1);
	m_rectC[eC_MOVE_TYPE_TORQUE		] = CRect(rc.left+dW*4+5   ,rc.top+dH*0+5,rc.left+dW*6   ,rc.top+dH*1);
													   							     
	m_rectC[eC_JOG_MOVE_N		] = CRect(rc.left+dW*0+5   ,rc.top+dH*1+5,rc.left+dW*1   ,rc.top+dH*3);
	m_rectC[eC_JOG_MOVE_P		] = CRect(rc.left+dW*1+5   ,rc.top+dH*1+5,rc.left+dW*2   ,rc.top+dH*3);
	m_rectS[eS_GANTRY_CH_STATE	] = CRect(rc.left+dW*0+5   ,rc.top+dH*3+5,rc.left+dW*2   ,rc.top+dH*4);
													   							     
	m_rectS[eS_TARGET			] = CRect(rc.left+dW*2+5   ,rc.top+dH*1+5,rc.left+dW*3   ,rc.top+dH*2);
	m_rectS[eS_SPEED			] = CRect(rc.left+dW*2+5   ,rc.top+dH*2+5,rc.left+dW*3   ,rc.top+dH*3);
													   							     
	m_rectC[eC_EDIT_TARGET	] = CRect(rc.left+dW*3+5   ,rc.top+dH*1+5,rc.left+dW*5   ,rc.top+dH*2);
	m_rectC[eC_EDIT_SPEED	] = CRect(rc.left+dW*3+5   ,rc.top+dH*2+5,rc.left+dW*5   ,rc.top+dH*3);
													   							     
	m_rectC[eC_MOVE			] = CRect(rc.left+dW*5+5   ,rc.top+dH*1+5,rc.left+dW*6   ,rc.top+dH*3);
	m_rectC[eC_MOVE_STOP	] = CRect(rc.left+dW*5+5   ,rc.top+dH*3+5,rc.left+dW*6   ,rc.top+dH*4);

	m_rectS[eS_OVERRIDE		] = CRect(rc.left+dW*2+5   ,rc.top+dH*3+5,rc.left+dW*3   ,rc.top+dH*4);
	m_rectS[eS_SLIDE		] = CRect(rc.left+dW*3+5   ,rc.top+dH*3+5,rc.left+dW*4.5 ,rc.top+dH*4);
	m_rectC[eC_SLIDE		] = CRect(rc.left+dW*4.5+5 ,rc.top+dH*3+5,rc.left+dW*5   ,rc.top+dH*4);
	
	//m_rectC[eC_GEAR_MOVE  ] = CRect(rc.left+dW*5+5 ,rc.top+dH*3+5,rc.left+dW*6 ,rc.top+dH*4);

	rc = m_rectS[eS_RIGHT_SIDE_AREA];
	dW = (rc.Width() - 5) / 1.;
	dH = (rc.Height()-5) / 20.;

	//<< RightSide UI
	m_rectC[eC_MAXIMUM_WINDOW	]	= CRect(rc.left+dW*0+5 ,rc.top+dH*0 + 5   ,rc.left+dW*1 ,rc.top+dH*1);
																	    
	m_rectC[eC_PRESET			]	= CRect(rc.left+dW*0   +5 ,	rc.top+dH*1 + 5   ,rc.left+dW*0.5	,rc.top+dH*2);
	m_rectC[eC_CUR_PRESET		]	= CRect(rc.left+dW*0.5 +5 ,	rc.top+dH*1 + 5   ,rc.left+dW*1		,rc.top+dH*2);
	m_rectS[eS_LOOP				]	= CRect(rc.left+dW*0   +5 , rc.top+dH*2 + 5   ,rc.left+dW*1     ,rc.top+dH*3);

	rc.DeflateRect(0, 5, 0, 0);
	if (sm.DeviceParameter->eUserGrade >= eUSER_ADMIN)
	{
		m_rectC[eC_INIT				]	= CRect(rc.left+dW*0+5 ,rc.top+dH*3 + 5   ,rc.left+dW*1 ,rc.top+dH*4);
		m_rectC[eC_INIT_STOP		]	= CRect(rc.left+dW*0+5 ,rc.top+dH*4 + 5   ,rc.left+dW*1 ,rc.top+dH*5);
	}
	m_rectC[eC_ALARM_CLEAR		]	= CRect(rc.left+dW*0+5 ,rc.top+dH*5 + 5   ,rc.left+dW*1 ,rc.top+dH*6);

	rc.DeflateRect(0, 5, 0, 0);
	m_rectC[eC_SERVO_ON			]	= CRect(rc.left+dW*0+5 ,rc.top+dH*6 + 5   ,rc.left+dW*1 ,rc.top+dH*7);
	m_rectC[eC_SERVO_OFF		]	= CRect(rc.left+dW*0+5 ,rc.top+dH*7 + 5   ,rc.left+dW*1 ,rc.top+dH*8);
																	    
	rc.DeflateRect(0, 5, 0, 0);
	m_rectC[eC_ALL_ALARM_CLEAR	]	= CRect(rc.left+dW*0+5 ,rc.top+dH*8 + 5   ,rc.left+dW*1 ,rc.top+dH*9);
	m_rectC[eC_ALL_SERVO_ON		]	= CRect(rc.left+dW*0+5 ,rc.top+dH*9 + 5   ,rc.left+dW*1 ,rc.top+dH*10);
	m_rectC[eC_ALL_SERVO_OFF	]	= CRect(rc.left+dW*0+5 ,rc.top+dH*10 + 5  ,rc.left+dW*1 ,rc.top+dH*11);

	rc.DeflateRect(0, 10, 0, 0);
	m_rectS[eS_GANTRY			]	= CRect(rc.left+dW*0+5 ,rc.top+dH*12+5, rc.left+dW*1 ,rc.top+dH*16);
	if (sm.DeviceParameter->eUserGrade >= eUSER_ADMIN)
	{
		m_rectC[eC_LINER_MOVE		]	= CRect(rc.left+dW*0+5 ,rc.top+dH*16+5,	rc.left+dW*1 ,rc.top+dH*17);
		m_rectC[eC_MULTI_MOVE		]	= CRect(rc.left+dW*0+5 ,rc.top+dH*17+5,	rc.left+dW*1 ,rc.top+dH*18);
	}
	
	rc.DeflateRect(0, -25, 0, 0);
	m_rectC[eC_CLOSE			]	= CRect(rc.left+dW*0+5 ,rc.top+dH*19+5,rc.left+dW*1 ,rc.top+dH*20);
	//>>

	//<< Gantry Area
	rc = m_rectS[eS_GANTRY];
	rc.DeflateRect(0, 15, 0, 0);
	dW = (rc.Width() - 5) / 1.;
	dH = (rc.Height() - 5) / 4.;
	
	if (sm.DeviceParameter->eUserGrade >= eUSER_ADMIN)
	{
		m_rectC[eC_GANTRY_ALL]				= CRect(rc.left+dW*0+5 ,rc.top+dH*0+5,rc.left+dW*1 ,rc.top+dH*1);
		m_rectC[eC_GANTRY_CH1]				= CRect(rc.left+dW*0+5 ,rc.top+dH*1+5,rc.left+dW*1 ,rc.top+dH*2);
		m_rectC[eC_GANTRY_CH2]				= CRect(rc.left+dW*0+5 ,rc.top+dH*2+5,rc.left+dW*1 ,rc.top+dH*3);
		m_rectC[eC_GANTRY_OFFSET_NORMALIZE] = CRect(rc.left+dW*0+5 ,rc.top+dH*3+5,rc.left+dW*1 ,rc.top+dH*4);
	}
	//>>

	m_lstAxis.MoveWindow(m_rectS[eS_LIST_AREA]);

	if (m_SliderControl) {
		m_SliderControl.MoveWindow(m_rectS[eS_SLIDE]);
		m_SliderControl.ShowWindow(SW_SHOW);
	}
}

void CAxisControlWnd::OnItemchangedLst(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if ((pNMLV->uNewState & LVIS_SELECTED) && !(pNMLV->uOldState & LVIS_SELECTED)) { // Select		
		
		//<< 선택된 모터 업데이트 하기 전에 전처리.
		//. 이전 항목 배경색 지정.
		if (m_eSelectedMotor != pNMLV->iItem) {
			//m_lstAxis.SetBKColor(m_eSelectedMotor, eLI_NAME, RGB_WHITE);
			m_lstAxis.SetBKColor(m_eSelectedMotor, eLI_POSITION, RGB_WHITE);
			m_lstAxis.SetBKColor(m_eSelectedMotor, eLI_SPEED, RGB_WHITE);
			m_lstAxis.SetBKColor(m_eSelectedMotor, eLI_TORQUE, RGB_WHITE);
			m_iSelectGantry = 0;
			//. 사용자 실수 방지. Gantry는 테스트 용도 아니라면 항상 ALL로 설정되어 있어야 한다.
			if (sm.MotorStatus[m_eSelectedMotor].tEtc.dwHomeStatus != eHOME_STATUS_HOMMING) {
				sm.MotorParameter[m_eSelectedMotor].tYaskawa.iGantryCh = eCH_AXIS_ALL;
			}

			if (sm.MotorParameter[m_eSelectedMotor].tYaskawa.bSetGantry) {
				m_lstAxis.SetBKColor(m_eSelectedMotor, eLI_GANTRY_CH2, RGB_WHITE);
			}
			else {
				m_lstAxis.SetBKColor(m_eSelectedMotor, eLI_GANTRY_CH2, RGB_BLACK);
			}
		}
		//>>


		//. 선택된 모터 업데이트.
		m_eSelectedMotor = pNMLV->iItem;
		
		// 2024.4.19 KES 추가 - 축 선택시 Speed 설정값이 0이면 LowSpeed값으로 설정.
		if (fabs(m_dSpeed[m_eSelectedMotor]) <= 0.001) {
			if (sm.MotorParameter[m_eSelectedMotor].dLowSpeed <= 100.)
				m_dSpeed[m_eSelectedMotor] = sm.MotorParameter[m_eSelectedMotor].dLowSpeed;
			else
				m_dSpeed[m_eSelectedMotor] = 100.;
		}

		m_lstAxis.SetItemState(pNMLV->iItem, NULL, LVIS_SELECTED | LVIS_FOCUSED);
		
		//m_lstAxis.SetBKColor(pNMLV->iItem, eLI_NAME, RGB_GRAY_LIGHT);
		COLORREF colorSelected = RGB(150,150,230);
		m_lstAxis.SetBKColor(pNMLV->iItem, eLI_POSITION, colorSelected);
		m_lstAxis.SetBKColor(pNMLV->iItem, eLI_SPEED, colorSelected);
		m_lstAxis.SetBKColor(pNMLV->iItem, eLI_TORQUE, colorSelected);

		Invalidate(FALSE);
	}

	*pResult = 0;
}

void CAxisControlWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_iSlider = m_SliderControl.GetPos();
	sm.Register->Reg_Word_1_NEW.dwOverdrive = m_iSlider*100;
	//for(int i =0; i < eMOTOR_END; i++){
	//	sm.MotorStatus[i].tEtc.iOverride = m_iSlider;
	//}
	CFrameWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CAxisControlWnd::OnIPShow(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	BOOL bShow = var.GetChildItem(_T("Show"), TRUE);

	if (bShow) {
		ShowWindow(SW_SHOW);
		int iW = 1920 / 2;
		int iH = 1080 / 6;
		SetWindowPos(&wndTopMost, iW, 0, iW, iH * 5, SWP_SHOWWINDOW);
		SetForegroundWindow();
	}
	else 
		ShowWindow(SW_HIDE);
	return TRUE;
}
void CAxisControlWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if (m_bMaximumPage && nHitTest == HTCAPTION)
		CFrameWnd::OnNcLButtonDown(nHitTest, point);
}

void CAxisControlWnd::OnItemRClick(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CAxisAlarmWnd* pwnd = (CAxisAlarmWnd*)((CMainFrame*)AfxGetMainWnd())->m_pwndAxisAlarm;
	pwnd->MotorSetting(pNMLV->iItem);
	pwnd->ShowWindow(SW_SHOW);
	pwnd->SetForegroundWindow();
}

void CAxisControlWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	for (int i = 0; i < eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			InvalidateRect(m_rectC[i], FALSE);
		}
	}
	CFrameWnd::OnRButtonDown(nFlags, point);
}


void CAxisControlWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_MAXIMUM_WINDOW: {
				CRegistryMapWnd* pwnd = (CRegistryMapWnd*)((CMainFrame*)AfxGetMainWnd())->m_pwndRegistryMap;
				pwnd->ShowWindow(SW_SHOW);
				break;
			}
			case eC_MOVE: {
				m_iHideCount++;
				if (m_iHideCount > 4) {
					m_iHideCount = 0;
					m_bHide = !m_bHide;
				}
				break;
			}
		}
	}
	m_eSelectControl = eC_NONE;
	Invalidate(FALSE);
	CFrameWnd::OnRButtonUp(nFlags, point);
}


void CAxisControlWnd::OnClose()
{
	ShowWindow(SW_HIDE);
}
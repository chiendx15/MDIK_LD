#include "pch.h"
#include "UI.h"
#include "./MotorParameterWnd.h"

static const CString strParameters[] = {
	// Axis Init
	  _T("Rack No")
	, _T("Slot No")
	, _T("Subslot No(Main)")
	, _T("Motor No")
	, _T("Logical Axis No")
	, _T("Axis Type")
	// Gantry
	, _T("Address")
	, _T("Rack / Slot / Sub Slot")
	, _T("Index 1 Motor / Logical")
	, _T("Index 2 Motor / Logical")
	// Home 
	, _T("Method")
	, _T("Torque ( % )")
	, _T("Direction")
	, _T("Velocity ( mm/s )")
	, _T("Approach Velocity ( mm/s )")
	, _T("Creep Velocity ( mm/s )")
	, _T("Offset ( mm ) ")
	, _T("2ch")
	, _T("Home Serch Timeout ( sec )")
	, _T("Init Position ( mm )")
	, _T("External Home Sensor")
	// Parameter
	, _T("API Timeout ( sec )")
	, _T("Coordinate System")
	, _T("Move Type")
	, _T("Velocity Type")
	, _T("AccDec Type")
	, _T("Filter Type")
	, _T("Data Type")
	, _T("Position Rate")
	, _T("Speed Rate")
	, _T("Move Timeout ( sec )")
	, _T("InPosition ( mm )")
	, _T("Default Speed ( mm/s )")
	, _T("Gantry Offset Limit Warning ( +mm )")
	, _T("Gantry Offset Limit Alert ( +mm )")
	, _T("Acc")
	, _T("Dec")
	, _T("Filter Time ( sec )")
	, _T("Max Velocity ( mm/s )")
	, _T("Low Speed ( mm/s )")
	, _T("Medium Speed ( mm/s )")	// KES 중속 추가 2024.2.15
	, _T("High Speed ( mm/s )")
	, _T("PTP Speed ( mm/s )")
	, _T("Limit Low ( mm )")
	, _T("Limit High ( mm )")
};



//<< ComboBox 관련 데이터 세팅. 
const CMotorParameterWnd::ComboBoxComponent CMotorParameterWnd::m_arrComboBoxComponents[] = {
	{
		_T("InitAxis")
		, 10000
		, eC_YASKAWA_INIT_PARAMETER_AXIS_TYPE
		,
		{
			_T("1 : REAL_AXIS")                      /*  1 : Real servo axis             */
		  , _T("2 : VIRTUAL_AXIS")                   /*  2 : Virtual servo axis          */
		  , _T("3 : EXTERNAL_ENCODER")               /*  3 : External encoder            */
		},
		{ 1, 2, 3 } 
	},
	{
		_T("Home")
		, 10001
		, eC_YASKAWA_HOME_PARAMETER_HOME_TYPE
		,
		{
			_T("0: Torque")                        /* 0 : */
			, _T("1 : Home Sensor")				   /* 1 : */
			, _T("2 : N - P Center")				   /* 2 : */
			, _T("3 : N Sensor")					   /* 3 : */
			, _T("4 : P Sensor")                     /* 4 : */
			//, _T("5 : Default")                     /* 5 : */
												   
		},
		{ 0, 1, 2, 3, 4}//, 5 }
	},
	{
		_T("Direction")
		, 10002
		, eC_YASKAWA_HOME_PARAMETER_DIRECTION
		,
		{
													//. 12.13 변경. API 문서와 반대로 정의했음을 주의하세요.
			_T("0 : DIRECTION_NEGATIVE")            
		  , _T("1 : DIRECTION_POSITIVE")            
		},
		{ 0, 1 }
	},
	{
		_T("CoordinateSystem")
		, 10003
		, eC_PARAMETER_COORDINATE_SYSTEM
		,
		{
		   _T("0 : WORK_SYSTEM")			/*  Workpiece coordinate system     */
		 , _T("1 : MACHINE_SYSTEM")			/*  Machine coordinate system       */
		},
		{ 0 , 1 }
	},
	{
		_T("Move")
		, 10004
		, eC_PARAMETER_MOVE_TYPE
		,
		{
		   _T("0 : MTYPE_RELATIVE")       /*  0: Incremental value specified, common for linear and rotary axes                                         */
		 , _T("1 : MTYPE_ABSOLUTE")       /*  1: Absolute position speciifed, for linear axis                                                           */
		 , _T("2 : MTYPE_R_SHORTEST")     /*  2: Absolute position specified, for rotary axis (Rotates in the direction closer to the target position.) */
		 , _T("3 : MTYPE_R_POSITIVE")     /*  3: Absolute position specified, for rotary axis (forward run)                                             */
		 , _T("4 : MTYPE_R_NEGATIVE")     /*  4: Absolute position specified, for rotary axis (reverse run)                                             */
		},
		{ 0, 1, 2, 3, 4 }
	},
	{
		_T("Velocity")
		, 10005
		, eC_PARAMETER_VELOCITY_TYPE
		,
		{
			  _T("0 : VTYPE_UNIT_PAR") /*  Speed [reference unit/s]              */
			, _T("1 : VTYPE_PERCENT")  /*  Rated speed percentage (%) specified  */
		},
		{ 0, 1 }
	},
	{
		_T("AccDec")
		, 10006
		, eC_PARAMETER_ACCDEC_TYPE
		,
		{
			   _T("0 : ATYPE_UNIT_PAR")    /*  Acceleration [reference unit/s2]  */
			 , _T("1 : ATYPE_TIME")        /*  Time constant [ms]                */
			 , _T("2 : ATYPE_KEEP")        /*  Current setting held              */
		},
		{ 0, 1, 2 }
	},
	{
		_T("Filter")
		, 10007
		, eC_PARAMETER_FILTER_TYPE
		,
		{
			   _T("0 : FTYPE_S_CURVE")  /*  0: Move average filter (simple S-curve)	*/
			 , _T("1 : FTYPE_EXP")      /*  1: Exponential filter					*/
			 , _T("2 : FTYPE_NOTHING")  /*  2: Without filter						*/
			 , _T("3 : FTYPE_KEEP")     /*  3: Current setting held					*/
		},
		{ 0, 1, 2, 3 }
	},
	{
		_T("Data")
		, 10008
		, eC_PARAMETER_DATA_TYPE
		,
		{
			   _T("0 : DTYPE_IMMEDIATE")     /*  Direct designation                      */
			 , _T("1 : DTYPE_INDIRECT")		/*  Indirect designation                    */

			 //. API 문서에 define된 값이 더 있지만, bit연산은 콤보박스에 추가하지 않는다.
		},
		{ 0, 1 }
	}
};
//>>

bool CMotorParameterWnd::isComboBoxControl(const int& i) {

	bool isMatched = false;
	switch (i) {
	case eC_YASKAWA_INIT_PARAMETER_AXIS_TYPE:
	case eC_YASKAWA_HOME_PARAMETER_HOME_TYPE:
	case eC_YASKAWA_HOME_PARAMETER_DIRECTION:
	case eC_PARAMETER_COORDINATE_SYSTEM:
	case eC_PARAMETER_MOVE_TYPE:
	case eC_PARAMETER_VELOCITY_TYPE:
	case eC_PARAMETER_ACCDEC_TYPE:
	case eC_PARAMETER_FILTER_TYPE:
	case eC_PARAMETER_DATA_TYPE:
		isMatched = true;
		break;
	}

	return isMatched;
}

CMotorParameterWnd::CMotorParameterWnd(CWnd* pwndParent)
	: m_pwndParent(pwndParent)
	, m_eMouseOver(eC_NONE)
	, m_eSelectControl(eC_NONE)
	, m_bEdit(FALSE)
	, m_eSelectMotor(0)
	, m_MotorParameter{} //. 초기화 안해주면 ini파일에 쓰레기값 들어감
{
	/*
	* sprintf_s(), strncpy_s(), 등등 문자열 복사 함수의 경우
	* debug 모드에서 복사시, 정상 데이터 경계면(Threshold) 이후로, 0xFE (FREE)를 채워 넣도록 (Fill) 되어 있음.
	* 0 값으로 채워넣도록 하려면 아래의 함수를 호출 후, 0값을 인자로 넘겨주면 된다.
	* 해당 함수는 한 번 세팅하면 전체 프로그램에 flag로서 세팅되므로.. 
	* 해당 클래스 말고 프로그램 초기화 진행하는 부분에서 호출해야할텐데
	* sharedMemory에서 InitShared 할 때 하면 더 낫겠음.
	*/
	//<<
	_CrtSetDebugFillThreshold(0x00);
	//>>
}

CMotorParameterWnd::~CMotorParameterWnd() {
}

#define ID_MOTOR_LIST	10002

IMPLEMENT_DYNCREATE		(CMotorParameterWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CMotorParameterWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
	ON_NOTIFY			(LVN_ITEMCHANGED, ID_MOTOR_LIST, OnItemchangedLst)

	ON_CONTROL_RANGE	(CBN_SELCHANGE, m_arrComboBoxComponents[eCOMBO_YASKAWA_INIT_PARAMETER_AXIS_TYPE].iContextID, m_arrComboBoxComponents[eCOMBO_PARAMETER_DATA_TYPE].iContextID, OnCbnSelchangeComboType)
	
	ON_MESSAGE			(WM_APP_ED_EDIT_FINISHED, OnEditFinished)
END_MESSAGE_MAP()

BOOL CMotorParameterWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CMotorParameterWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {
		m_lstMotor.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, m_rectS[eS_MOTOR_LIST], this, ID_MOTOR_LIST);
		m_lstMotor.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
		m_lstMotor.ModifyStyle(0, LVS_SHOWSELALWAYS);
		m_lstMotor.SetRowHeight(45);
		m_lstMotor.ShowScrollBar(SB_VERT);
		m_lstMotor.ShowScrollBar(SB_HORZ);
		m_lstMotor.InsertColumn(eLI_NONE, _T(""), LVCFMT_LEFT, 0, 1);
		m_lstMotor.InsertColumn(eLI_MOTOR_INDEX, _T("Axis"), LVCFMT_LEFT, 100, 1);
		m_lstMotor.InsertColumn(eLI_MOTOR_NAME, _T("Name"), LVCFMT_LEFT, 200, 1);
		m_lstMotor.InsertColumn(eLI_MOTOR_GANTRY, _T("Gantry"), LVCFMT_LEFT, 100, 1);

		for (int i = eMOTOR_0; i < eMOTOR_END; i++) {
			m_lstMotor.InsertItem(i, _T(""));
			m_lstMotor.SetItemInt(i, eLI_MOTOR_INDEX, i);
			m_lstMotor.SetItemText(i, eLI_MOTOR_NAME, g_stMotor[i]);
			m_lstMotor.SetItemText(i, eLI_MOTOR_GANTRY, _T(""));
			m_lstMotor.SetAlignText(eLI_MOTOR_NAME, eLCETA_LEFT);
			m_lstMotor.SetAlignText(eLI_MOTOR_GANTRY, eLCETA_LEFT);
		}

		//<< Type ComboBox UI Create.
		const int WS = WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST;
		
		for (int iCbo = 0; iCbo < TYPE_PARAMETER_COMBO_BOX_COUNT; ++iCbo) {
			m_cboTypes[iCbo].Create(WS, CRect(), this, m_arrComboBoxComponents[iCbo].iContextID);

			size_t nItemSize = m_arrComboBoxComponents[iCbo].vecComboBoxItems.size();

			//<< Add ComboBox Item.
			for (int iItems = 0; iItems < nItemSize; ++iItems) {

				m_cboTypes[iCbo].AddString(m_arrComboBoxComponents[iCbo].vecComboBoxItems[iItems]);
			}
			//>>

			m_cboTypes[iCbo].SetCurSel(0);
			m_cboTypes[iCbo].ShowWindow(SW_SHOW);
		}
		//>>

		SyncData(FALSE);
	}
	catch (...) {
		return -1;
	}

	return 0;
}

void CMotorParameterWnd::OnTimer(UINT_PTR nIDEvent) {
	
	switch (nIDEvent) {
	case ID_TIMER_HOME_SENSOR: {

		if (m_MotorParameter[m_eSelectMotor].tHome.szExternalHomeSensorAddress[0] == '\0') {
			break;
		}

		if (m_bExternalHomeSensorOnOff != sm.MotorStatus[m_eSelectMotor].tCrt.bHomeSensor) {

			m_bExternalHomeSensorOnOff = sm.MotorStatus[m_eSelectMotor].tCrt.bHomeSensor;
			InvalidateRect(m_rectC[eC_HOME_SENSOR_ON_OFF], FALSE);
		}

		break;
	}
	default: {

		break;
	}
	}

	CFrameWndEx::OnTimer(nIDEvent);
}

void CMotorParameterWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		m_bEdit = FALSE;
		SyncData(FALSE);
		
		m_lstMotor.SetItemState(m_eSelectMotor, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		SetDataFromComboBox();

		SetTimer(ID_TIMER_HOME_SENSOR, nTimerElapsed, nullptr);

		Invalidate();
	}
	else {
		KillTimer(ID_TIMER_HOME_SENSOR);

	}
}

BOOL CMotorParameterWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}

void CMotorParameterWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);

	CRect rect = m_rectS[eS_FULL_AREA];
	DrawLineRect(memDC, rect, RGB_WHITE);
	rect.DeflateRect(1, 1);
	memDC.FillSolidRect(rect, RgbColor::Gainsboro);
	memDC.FillSolidRect(m_rectS[eS_WORK_AREA], RgbColor::AliceBlue);

	//<< 상단 UI
	DrawMDIMenuButton(memDC, m_rectC[eC_EDIT], m_bEdit ? g_st[IDS_PARAMETER_EDIT] : g_st[IDS_PARAMETER_LOCK], m_eSelectControl == eC_EDIT || !m_bEdit, m_eMouseOver == eC_EDIT);
	DrawMDIMenuButton(memDC, m_rectC[eC_SAVE], g_st[IDS_PARAMETER_SAVE], m_eSelectControl == eC_SAVE, m_eMouseOver == eC_SAVE);

	CString strMotorName = m_lstMotor.GetItemText(m_eSelectMotor, eLI_MOTOR_NAME);
	DrawLineRect(memDC, m_rectC[eC_MOTOR_NAME]);
	DrawText(memDC, m_rectC[eC_MOTOR_NAME], strMotorName, DT_FONTSIZE, RGB_BLACK, DT_VCENTER | DT_CENTER);

	DrawMDIMenuButton(memDC, m_rectC[eC_BTN_ALL_SET_INIT], _T("All Set Init"), m_eSelectControl == eC_BTN_ALL_SET_INIT, m_eMouseOver == eC_BTN_ALL_SET_INIT);
	DrawMDIMenuButton(memDC, m_rectC[eC_BTN_ALL_SET_HOME], _T("All Set Home"), m_eSelectControl == eC_BTN_ALL_SET_HOME, m_eMouseOver == eC_BTN_ALL_SET_HOME);
	DrawMDIMenuButton(memDC, m_rectC[eC_BTN_ALL_SET_PARAMETER], _T("All Set Parameter"), m_eSelectControl == eC_BTN_ALL_SET_PARAMETER, m_eMouseOver == eC_BTN_ALL_SET_PARAMETER);
	//>>

	DrawGroupBox(&memDC, m_rectS[eS_YASKAWA_INIT_AREA], _T("Init"), CSize(-1, -1), CPoint(15, -5), 9, RGB_BLACK, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_YASKAWA_INIT_SINGLE_AREA], _T("Single"), CSize(-1, -1), CPoint(15, -5), 9, RGB_BLACK, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_YASKAWA_INIT_GANTRY_AREA], _T("Gantry"), CSize(-1, -1), CPoint(15, -5), 9, RGB_BLACK, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_YASKAWA_HOME_AREA], _T("Home"), CSize(-1, -1), CPoint(15, -5), 9, RGB_BLACK, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_PARAMETER_AREA], _T("Parameter"), CSize(-1, -1), CPoint(15, -5), 9, RGB_BLACK, RGB_BLACK);

	//<< STATIC
	for (int i = 0; i < countof(strParameters); i++) {
		DrawText(memDC, m_rectS[eS_YASKAWA_INIT_PARAMETER_RACK_NO + i], g_st[IDS_SERVO_PARAMETER_INIT_RACK_NO + i], 9, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	}
	//>>

	//<< CONTROL YASKAWA_INIT 
	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_RACK_NO], m_MotorParameter[m_eSelectMotor].tYaskawa.iRackNo, m_bEdit);
	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_SLOT_NO], m_MotorParameter[m_eSelectMotor].tYaskawa.iSlotNo, m_bEdit);
	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_SUBSLOT_NO], m_MotorParameter[m_eSelectMotor].tYaskawa.iSubSlotNo, m_bEdit);

	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_MOTOR_NO], m_MotorParameter[m_eSelectMotor].tYaskawa.iMotorNo, m_bEdit);
	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_LOGICAL_AXIS_NO], m_MotorParameter[m_eSelectMotor].tYaskawa.iLogicalAxisNo, m_bEdit);

	//<< CONTROL YASKAWA_INIT GANTRY
	DrawButtonUpDown(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_USE], m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry ? _T("Use") : _T("No Use"), m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry);

	CString strAddressStop(m_MotorParameter[m_eSelectMotor].tYaskawa.szGantryAddressStop);
	SelectedDrawEditText(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_ADDRESS_STOP], strAddressStop, m_bEdit && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry);

	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_INDEX_1], m_MotorParameter[m_eSelectMotor].tYaskawa.iCh1_Axis, m_bEdit && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry);
	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_INDEX_2], m_MotorParameter[m_eSelectMotor].tYaskawa.iCh2_Axis, m_bEdit && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry);

	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_MOTOR_INDEX_1], m_MotorParameter[m_eSelectMotor].tYaskawa.iCh1_Motor, m_bEdit && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry);
	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_MOTOR_INDEX_2], m_MotorParameter[m_eSelectMotor].tYaskawa.iCh2_Motor, m_bEdit && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry);

	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_RACK_NO], m_MotorParameter[m_eSelectMotor].tYaskawa.iGantryRackNo, m_bEdit && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry);
	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_SLOT_NO], m_MotorParameter[m_eSelectMotor].tYaskawa.iGantrySlotNo, m_bEdit && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry);
	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_SUB_SLOT_NO], m_MotorParameter[m_eSelectMotor].tYaskawa.iGantrySubSlotNo, m_bEdit && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry);
	//>>

	//<< HOME
	CString strExternalHomeSensor(m_MotorParameter[m_eSelectMotor].tHome.szExternalHomeSensorAddress);
	//if (m_MotorParameter[m_eSelectMotor].tHome.iHomeType == eHT_DEFAULT) {
	//	DrawButtonUpDown(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_2CH_USE], m_MotorParameter[m_eSelectMotor].tHome.bGantry2ChUse && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry ? _T("2Ch Use") : _T("2Ch Unused"), FALSE);
	//	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_TORQUE], m_MotorParameter[m_eSelectMotor].tHome.dTorque, 2, FALSE);
	//	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_VELOCITY], m_MotorParameter[m_eSelectMotor].tHome.dVelocity, 3, FALSE);
	//	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_APPROACH_VELOCITY], m_MotorParameter[m_eSelectMotor].tHome.dApproachVelocity, 3, FALSE);
	//	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_CREEP_VELOCITY], m_MotorParameter[m_eSelectMotor].tHome.dCreepVelocity, 3, FALSE);
	//	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_OFFSET], m_MotorParameter[m_eSelectMotor].tHome.dOffset, 3, m_bEdit);
	//	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_OFFSET2], m_MotorParameter[m_eSelectMotor].tHome.dOffset2Ch, 3, m_bEdit);
	//
	//	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_INIT_POSISITION], m_MotorParameter[m_eSelectMotor].tHome.dInitPosisition, 3, FALSE);
	//	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_INIT_POSISITION_GET], m_MotorParameter[m_eSelectMotor].tHome.dInitPosisition, 3, FALSE);
	//	DrawButtonUpDown(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_INIT_POSISITION_GET], _T("Get"), FALSE);
	//
	//	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_HOME_SERCH_TIMEOUT], m_MotorParameter[m_eSelectMotor].tHome.iHomeSerchTimeout, FALSE);
	//
	//	SelectedDrawEditText(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_EXTERNAL_HOME_SENSOR], strExternalHomeSensor, FALSE);
	//}
	//else {
	DrawButtonUpDown(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_2CH_USE], m_MotorParameter[m_eSelectMotor].tHome.bGantry2ChUse && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry ? _T("2Ch Use") : _T("2Ch Unused"), m_bEdit && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry && m_MotorParameter[m_eSelectMotor].tHome.bGantry2ChUse);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_TORQUE], m_MotorParameter[m_eSelectMotor].tHome.dTorque, 2, m_bEdit && (m_MotorParameter[m_eSelectMotor].tHome.iHomeType == eHT_TORQUE || m_MotorParameter[m_eSelectMotor].tHome.iHomeType == eHT_HOME));
	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_VELOCITY], m_MotorParameter[m_eSelectMotor].tHome.dVelocity, 3, m_bEdit && m_MotorParameter[m_eSelectMotor].tHome.iHomeType != 0);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_APPROACH_VELOCITY], m_MotorParameter[m_eSelectMotor].tHome.dApproachVelocity, 3, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_CREEP_VELOCITY], m_MotorParameter[m_eSelectMotor].tHome.dCreepVelocity, 3, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_OFFSET], m_MotorParameter[m_eSelectMotor].tHome.dOffset, 3, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_OFFSET2], m_MotorParameter[m_eSelectMotor].tHome.dOffset2Ch, 3, m_bEdit && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry);

	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_INIT_POSISITION], m_MotorParameter[m_eSelectMotor].tHome.dInitPosisition, 3, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_INIT_POSISITION_GET], m_MotorParameter[m_eSelectMotor].tHome.dInitPosisition, 3, m_bEdit);
	DrawButtonUpDown(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_INIT_POSISITION_GET], _T("Get"), m_eSelectControl == eC_YASKAWA_HOME_PARAMETER_INIT_POSISITION_GET);

	SelectedDrawEditInt(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_HOME_SERCH_TIMEOUT], m_MotorParameter[m_eSelectMotor].tHome.iHomeSerchTimeout, m_bEdit);


	SelectedDrawEditText(&memDC, m_rectC[eC_YASKAWA_HOME_PARAMETER_EXTERNAL_HOME_SENSOR], strExternalHomeSensor, m_bEdit);
	//}

	if (strExternalHomeSensor.IsEmpty()) {
		DrawRoundButtonUpDown(memDC, m_rectC[eC_HOME_SENSOR_ON_OFF], NULL);
	}
	else {
		DrawRoundSensor(memDC, m_rectC[eC_HOME_SENSOR_ON_OFF], NULL, m_bExternalHomeSensorOnOff);
	}

	//>>

	SelectedDrawEditInt(&memDC, m_rectC[eC_PARAMETER_API_TIMEOUT], m_MotorParameter[m_eSelectMotor].iAPITimeout, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_PARAMETER_POSITION_RATE], m_MotorParameter[m_eSelectMotor].dPositionRate, 3, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_PARAMETER_SPEED_RATE], m_MotorParameter[m_eSelectMotor].dSpeedRate, 3, m_bEdit);
	SelectedDrawEditInt(&memDC, m_rectC[eC_PARAMETER_MOVE_TIMEOUT], m_MotorParameter[m_eSelectMotor].iMoveTimeout, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_PARAMETER_INPOSITION], m_MotorParameter[m_eSelectMotor].dInPosition, 3, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_PARAMETER_DEFAULT_SPPED], m_MotorParameter[m_eSelectMotor].dDefaultSpped, 3, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_PARAMETER_GANTRY_OFFSET_LIMIT_WARNING], m_MotorParameter[m_eSelectMotor].dGantryOffsetLimitWarning, 3, m_bEdit && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_PARAMETER_GANTRY_OFFSET_LIMIT_ALERT], m_MotorParameter[m_eSelectMotor].dGantryOffsetLimitAlert, 3, m_bEdit && m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry);

	SelectedDrawEditInt(&memDC, m_rectC[eC_PARAMETER_ACC], m_MotorParameter[m_eSelectMotor].iAcc, m_bEdit);
	SelectedDrawEditInt(&memDC, m_rectC[eC_PARAMETER_DEC], m_MotorParameter[m_eSelectMotor].iDec, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_PARAMETER_FILTER_TIME], m_MotorParameter[m_eSelectMotor].dFilterTime, 3, m_bEdit);
	SelectedDrawEditInt(&memDC, m_rectC[eC_PARAMETER_MAX_VELOCITY], m_MotorParameter[m_eSelectMotor].iMaxVelocity, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_PARAMETER_LOW_SPEED], m_MotorParameter[m_eSelectMotor].dLowSpeed, 3, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_PARAMETER_MEDIUM_SPEED], m_MotorParameter[m_eSelectMotor].dMediumSpeed, 3, m_bEdit);	// KES 중속 추가 2024.2.15
	SelectedDrawEditDouble(&memDC, m_rectC[eC_PARAMETER_HIGH_SPEED], m_MotorParameter[m_eSelectMotor].dHighSpeed, 3, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_PARAMETER_PTP_SPEED], m_MotorParameter[m_eSelectMotor].dPTPSpeed, 3, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_PARAMETER_LIMIT_LOW], m_MotorParameter[m_eSelectMotor].dLimitLow, 3, m_bEdit);
	SelectedDrawEditDouble(&memDC, m_rectC[eC_PARAMETER_LIMIT_HIGH], m_MotorParameter[m_eSelectMotor].dLimitHigh, 3, m_bEdit);

	DrawButtonUpDown(&memDC, m_rectC[eC_PARAMETER_LIMIT_LOW_GET], _T("Get"), m_bEdit ? m_eSelectControl == eC_PARAMETER_LIMIT_LOW_GET : FALSE);
	DrawButtonUpDown(&memDC, m_rectC[eC_PARAMETER_LIMIT_HIGH_GET], _T("Get"), m_bEdit ? m_eSelectControl == eC_PARAMETER_LIMIT_HIGH_GET : FALSE);
	//<< ComboBox Lock일 때 비활성화
	for (int i = 0; i < TYPE_PARAMETER_COMBO_BOX_COUNT; ++i) {
		m_cboTypes[i].EnableWindow(m_bEdit);
		//TODO 콤보박스 값 업데이트 필요.
	}
	//>>
}

BOOL CMotorParameterWnd::PreTranslateMessage(MSG* pMsg)
{
	// 12.13 추가. ComboBox WheelEvent Hooking.
	if (pMsg->message == WM_MOUSEWHEEL) {

		for (int i = 0; i < nCOMBO_BOX_COUNT; ++i) {

			if (pMsg->hwnd == m_cboTypes[i].GetSafeHwnd()) {
				//. dispatch 불가시, 0이 아닌값.
				return 1;
			}
		}
	}

	//. 메세지를 변형시키지 않고, dispatch 할 때 0값 (혹은 상위 함수 호출)
	return CFrameWndEx::PreTranslateMessage(pMsg);
}

void CMotorParameterWnd::OnMouseMove(UINT nFlags, CPoint point) {
	int eMouseOver = m_eMouseOver;
	eMouseOver = eC_NONE;

	if ((m_eSelectControl != eC_NONE) && (!m_rectC[m_eSelectControl].PtInRect(point))) {
		int eSelectControl = m_eSelectControl;
		m_eSelectControl = eC_NONE;
		InvalidateRect(m_rectC[eSelectControl], FALSE);
	}

	for (int i = 0; i < eC_END; i++) {

		//. CoboBox는 InvalidateRect 할 필요 없음.
		//. 안해주면 flicker현상 너무 심함.
		if (isComboBoxControl(i))
			continue;

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



void CMotorParameterWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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



void CMotorParameterWnd::OnLButtonUp(UINT nFlags, CPoint point)  {

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
		if (!m_bEdit && (m_eSelectControl >= eC_YASKAWA_INIT_PARAMETER_RACK_NO && m_eSelectControl <= eC_END))
			return;

		switch (m_eSelectControl) {
			case eC_EDIT: {
				if (m_bEdit || !sm.Sequence->fAutoMode || sm.Sequence->fCycleStop)
					m_bEdit = !m_bEdit;
				else
					IPMessage(g_st[IDS_MOTOR_PARAM_WND_CAN_NOT_CHANGE_WHILE_AUTO_RUN]);
				strLog = _T("잠금/편집");
				break;
			}
			case eC_SAVE: {
				strLog = _T("저장/불러오기");
				if (!m_bEdit)
					break;
				std::vector<CString> ChangeLogAll;
				std::vector<std::pair<CString, COLORREF>> changeDatasAll;
				for (int iMotor = eMOTOR_0; iMotor < eMOTOR_END; iMotor++) {
					T_YASKAWA_AXIS_INIT_PARAM* pNew = (T_YASKAWA_AXIS_INIT_PARAM*)(&m_MotorParameter[iMotor].tYaskawa);
					T_YASKAWA_AXIS_INIT_PARAM* pOrg = (T_YASKAWA_AXIS_INIT_PARAM*)(&sm.MotorParameter[iMotor].tYaskawa);

					std::vector<CString> ChangeLog;
					std::vector<std::pair<CString, COLORREF>> changeDatas;
					CString strMotor = g_stMotor[IDS_MOTOR_NAME_01 + iMotor];

					if (pNew->iRackNo != pOrg->iRackNo) {
						changeDatas.push_back({ Format(_T("Rack No %d -> %d"), pOrg->iRackNo, pNew->iRackNo), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Rack No"), _T("Init"), pOrg->iRackNo, pNew->iRackNo));
					}
					if (pNew->iSlotNo != pOrg->iSlotNo) {
						changeDatas.push_back({ Format(_T("Slot No %d -> %d"), pOrg->iSlotNo, pNew->iSlotNo), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Slot No"), _T("Init"), pOrg->iSlotNo, pNew->iSlotNo));
					}
					if (pNew->iSubSlotNo != pOrg->iSubSlotNo) {
						changeDatas.push_back({ Format(_T("Sub Slot No %d -> %d"), pOrg->iSubSlotNo, pNew->iSubSlotNo), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Sub Slot No"), _T("Init"), pOrg->iSubSlotNo, pNew->iSubSlotNo));
					}
					if (pNew->iMotorNo != pOrg->iMotorNo) {
						changeDatas.push_back({ Format(_T("Motor No %d -> %d"), pOrg->iMotorNo, pNew->iMotorNo), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Motor No"), _T("Init"), pOrg->iMotorNo, pNew->iMotorNo));
					}
					if (pNew->iLogicalAxisNo != pOrg->iLogicalAxisNo) {
						changeDatas.push_back({ Format(_T("Logical Axis No %d -> %d"), pOrg->iLogicalAxisNo, pNew->iLogicalAxisNo), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Logical Axis No"), _T("Init"), pOrg->iLogicalAxisNo, pNew->iLogicalAxisNo));
					}
					if (pNew->iAxisType != pOrg->iAxisType) {
						changeDatas.push_back({ Format(_T("AxisType %d -> %d"), pOrg->iAxisType, pNew->iAxisType), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("AxisType"), _T("Init"), pOrg->iAxisType, pNew->iAxisType));
					}
					if (pNew->bSetGantry != pOrg->bSetGantry) {
						CString strOrg = pOrg->bSetGantry ? _T("Use") : _T("No Use");
						CString strNew = pNew->bSetGantry ? _T("Use") : _T("No Use");
						changeDatas.push_back({ Format(_T("SetGantry %s -> %s"), strOrg, strNew), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%s;%s"), _T("SetGantry"), _T("Init"), strOrg, strNew));
					}
					if (pNew->iCh1_Axis != pOrg->iCh1_Axis) {
						changeDatas.push_back({ Format(_T("Ch 1 Axis %d -> %d"), pOrg->iCh1_Axis, pNew->iCh1_Axis), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Ch 1 Axis"), _T("Init"), pOrg->iCh1_Axis, pNew->iCh1_Axis));
					}
					if (pNew->iCh2_Axis != pOrg->iCh2_Axis) {
						changeDatas.push_back({ Format(_T("Ch 2 Axis %d -> %d"), pOrg->iCh2_Axis, pNew->iCh2_Axis), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Ch 2 Axis"), _T("Init"), pOrg->iCh2_Axis, pNew->iCh2_Axis));
					}
					if (pNew->iCh1_Motor != pOrg->iCh1_Motor) {
						changeDatas.push_back({ Format(_T("Ch 1 Motor %d -> %d"), pOrg->iCh1_Motor, pNew->iCh1_Motor), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Ch 1 Motor"), _T("Init"), pOrg->iCh1_Motor, pNew->iCh1_Motor));
					}
					if (pNew->iCh2_Motor != pOrg->iCh2_Motor) {
						changeDatas.push_back({ Format(_T("Ch 2 Motor %d -> %d"), pOrg->iCh2_Motor, pNew->iCh2_Motor), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Ch 2 Motor"), _T("Init"), pOrg->iCh2_Motor, pNew->iCh2_Motor));
					}
					if ((CString)pNew->szGantryAddressStop	!= (CString)pOrg->szGantryAddressStop		) {
						changeDatas.push_back({ Format(_T("Gantry Address Stop %s -> %s"), (CString)pOrg->szGantryAddressStop, (CString)pNew->szGantryAddressStop), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%s;%s"), _T("Gantry Address Stop"), _T("Init"), (CString)pOrg->szGantryAddressStop, (CString)pNew->szGantryAddressStop));
					}
					if (pNew->iGantryRackNo != pOrg->iGantryRackNo) {
						changeDatas.push_back({ Format(_T("Gantry Rack No %d -> %d"), pOrg->iGantryRackNo, pNew->iGantryRackNo), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Gantry Rack No"), _T("Init"), pOrg->iGantryRackNo, pNew->iGantryRackNo));
					}
					if (pNew->iGantrySlotNo != pOrg->iGantrySlotNo) {
						changeDatas.push_back({ Format(_T("Gantry Slot No %d -> %d"), pOrg->iGantrySlotNo, pNew->iGantrySlotNo), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Gantry Slot No"), _T("Init"), pOrg->iGantrySlotNo, pNew->iGantrySlotNo));
					}
					if (pNew->iGantrySubSlotNo != pOrg->iGantrySubSlotNo) {
						changeDatas.push_back({ Format(_T("Gantry Sub Slot No %d -> %d"), pOrg->iGantrySubSlotNo, pNew->iGantrySubSlotNo), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Gantry Sub Slot No"), _T("Init"), pOrg->iGantrySubSlotNo, pNew->iGantrySubSlotNo));
					}
					//if (pNew->iGantryCh != pOrg->iGantryCh) {
					//	changeDatas.push_back({ Format(_T("Gantry Ch %d -> %d"), pOrg->iGantryCh, pNew->iGantryCh), RGB_WHITE });
					//	ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Gantry Ch"), _T("Init"), pOrg->iGantryCh, pNew->iGantryCh));
					//}
					T_YASKAWA_HOME_PARAM* pNewHome = (T_YASKAWA_HOME_PARAM*)(&m_MotorParameter[iMotor].tHome);
					T_YASKAWA_HOME_PARAM* pOrgHome = (T_YASKAWA_HOME_PARAM*)(&sm.MotorParameter[iMotor].tHome);
					if (pNewHome->iMethod != pOrgHome->iMethod) {
						changeDatas.push_back({ Format(_T("Method %d -> %d"), pOrgHome->iMethod, pNewHome->iMethod), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Method"), _T("Home"), pOrgHome->iMethod, pNewHome->iMethod));
					}
					if (pNewHome->iDirection != pOrgHome->iDirection) {
						changeDatas.push_back({ Format(_T("Direction %d -> %d"), pOrgHome->iDirection, pNewHome->iDirection), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Direction"), _T("Home"), pOrgHome->iDirection, pNewHome->iDirection));
					}
					if (pNewHome->dVelocity != pOrgHome->dVelocity) {
						changeDatas.push_back({ Format(_T("Velocity %.3f -> %.3f"), pOrgHome->dVelocity, pNewHome->dVelocity), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Velocity"), _T("Home"), pOrgHome->dVelocity, pNewHome->dVelocity));
					}
					if (pNewHome->dApproachVelocity != pOrgHome->dApproachVelocity) {
						changeDatas.push_back({ Format(_T("Approach Velocity %.3f -> %.3f"), pOrgHome->dApproachVelocity, pNewHome->dApproachVelocity), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Approach Velocity"), _T("Home"), pOrgHome->dApproachVelocity, pNewHome->dApproachVelocity));
					}
					if (pNewHome->dCreepVelocity != pOrgHome->dCreepVelocity) {
						changeDatas.push_back({ Format(_T("Creep Velocity %.3f -> %.3f"), pOrgHome->dCreepVelocity, pNewHome->dCreepVelocity), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Creep Velocity"), _T("Home"), pOrgHome->dCreepVelocity, pNewHome->dCreepVelocity));
					}
					if (pNewHome->dOffset != pOrgHome->dOffset) {
						changeDatas.push_back({ Format(_T("Offset %.3f -> %.3f"), pOrgHome->dOffset, pNewHome->dOffset), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Offset"), _T("Home"), pOrgHome->dOffset, pNewHome->dOffset));
					}
					if (pNewHome->dOffset2Ch != pOrgHome->dOffset2Ch) {
						changeDatas.push_back({ Format(_T("Offset 2Ch %.3f -> %.3f"), pOrgHome->dOffset2Ch, pNewHome->dOffset2Ch), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Offset 2Ch"), _T("Home"), pOrgHome->dOffset2Ch, pNewHome->dOffset2Ch));
					}
					if (pNewHome->iHomeSerchTimeout != pOrgHome->iHomeSerchTimeout) {
						changeDatas.push_back({ Format(_T("Home Serch Timeout %d -> %d"), pOrgHome->iHomeSerchTimeout, pNewHome->iHomeSerchTimeout), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Home Serch Timeout"), _T("Home"), pOrgHome->iHomeSerchTimeout, pNewHome->iHomeSerchTimeout));
					}
					if (pNewHome->iHomeType != pOrgHome->iHomeType) {
						changeDatas.push_back({ Format(_T("Home Type %d -> %d"), pOrgHome->iHomeType, pNewHome->iHomeType), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Home Type"), _T("Home"), pOrgHome->iHomeType, pNewHome->iHomeType));
					}
					if (pNewHome->dTorque != pOrgHome->dTorque) {
						changeDatas.push_back({ Format(_T("Torque %.3f -> %.3f"), pOrgHome->dTorque, pNewHome->dTorque), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Torque"), _T("Home"), pOrgHome->dTorque, pNewHome->dTorque));
					}
					if (pNewHome->bGantry2ChUse != pOrgHome->bGantry2ChUse) {
						CString strOrg = pOrgHome->bGantry2ChUse ? _T("2Ch Use") : _T("2Ch Unused");
						CString strNew = pNewHome->bGantry2ChUse ? _T("2Ch Use") : _T("2Ch Unused");
						changeDatas.push_back({ Format(_T("Gantry 2Ch Use %s -> %s"), strOrg, strNew), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%s;%s"), _T("Gantry 2Ch Use"), _T("Home"), strOrg, strNew));
					}
					if (pNewHome->dInitPosisition != pOrgHome->dInitPosisition) {
						changeDatas.push_back({ Format(_T("Init Position %.3f -> %.3f"), pOrgHome->dInitPosisition, pNewHome->dInitPosisition), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Init Position"), _T("Home"), pOrgHome->dInitPosisition, pNewHome->dInitPosisition));
					}
					if ((CString)pNewHome->szExternalHomeSensorAddress != (CString)pOrgHome->szExternalHomeSensorAddress) {
						changeDatas.push_back({ Format(_T("Gantry 2Ch Use %s -> %s"), (CString)pOrgHome->szExternalHomeSensorAddress, (CString)pNewHome->szExternalHomeSensorAddress), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%s;%s"), _T("Gantry 2Ch Use"), _T("Home"), (CString)pOrgHome->szExternalHomeSensorAddress, (CString)pNewHome->szExternalHomeSensorAddress));
					}

					T_MOTOR_PARAMETER* pNewParam = (T_MOTOR_PARAMETER*)(&m_MotorParameter[iMotor]);
					T_MOTOR_PARAMETER* pOrgParam = (T_MOTOR_PARAMETER*)(&sm.MotorParameter[iMotor]);
					if (pNewParam->iAPITimeout != pOrgParam->iAPITimeout) {
						changeDatas.push_back({ Format(_T("API Timeout %d -> %d"), pOrgParam->iAPITimeout, pNewParam->iAPITimeout), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("API Timeout"), _T("Parameter"), pOrgParam->iAPITimeout, pNewParam->iAPITimeout));
					}
					if (pNewParam->iCoordinateSystem != pOrgParam->iCoordinateSystem) {
						changeDatas.push_back({ Format(_T("Coordinate System %d -> %d"), pOrgParam->iCoordinateSystem, pNewParam->iCoordinateSystem), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Coordinate System"), _T("Parameter"), pOrgParam->iCoordinateSystem, pNewParam->iCoordinateSystem));
					}
					if (pNewParam->iMoveType != pOrgParam->iMoveType) {
						changeDatas.push_back({ Format(_T("Mode Type %d -> %d"), pOrgParam->iMoveType, pNewParam->iMoveType), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Mode Type"), _T("Parameter"), pOrgParam->iMoveType, pNewParam->iMoveType));
					}
					if (pNewParam->iVelocityType != pOrgParam->iVelocityType) {
						changeDatas.push_back({ Format(_T("Velocity Type %d -> %d"), pOrgParam->iVelocityType, pNewParam->iVelocityType), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Velocity Type"), _T("Parameter"), pOrgParam->iVelocityType, pNewParam->iVelocityType));
					}
					if (pNewParam->iAccDecType != pOrgParam->iAccDecType){
						changeDatas.push_back({ Format(_T("AccDec Type %d -> %d"), pOrgParam->iAccDecType, pNewParam->iAccDecType), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("AccDec Type"), _T("Parameter"), pOrgParam->iAccDecType, pNewParam->iAccDecType));
					}
					if (pNewParam->iFilterType != pOrgParam->iFilterType){
						changeDatas.push_back({ Format(_T("Filter Type %d -> %d"), pOrgParam->iFilterType, pNewParam->iFilterType), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Filter Type"), _T("Parameter"), pOrgParam->iFilterType, pNewParam->iFilterType));
					}
					if (pNewParam->iDataType != pOrgParam->iDataType)	 {
						changeDatas.push_back({ Format(_T("Data Type %d -> %d"), pOrgParam->iDataType, pNewParam->iDataType), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Data Type"), _T("Parameter"), pOrgParam->iDataType, pNewParam->iDataType));
					}
					if (pNewParam->dPositionRate != pOrgParam->dPositionRate) {
						changeDatas.push_back({ Format(_T("Position Rate %.3f -> %.3f"), pOrgParam->dPositionRate, pNewParam->dPositionRate), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Position Rate "), _T("Parameter"), pOrgParam->dPositionRate, pNewParam->dPositionRate));
					}
					if (pNewParam->dSpeedRate != pOrgParam->dSpeedRate) {
						changeDatas.push_back({ Format(_T("Position Rate %.3f -> %.3f"), pOrgParam->dSpeedRate, pNewParam->dSpeedRate), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Position Rate "), _T("Parameter"), pOrgParam->dSpeedRate, pNewParam->dSpeedRate));
					}
					if (pNewParam->iMoveTimeout != pOrgParam->iMoveTimeout) {
						changeDatas.push_back({ Format(_T("Move Timeout %d -> %d"), pOrgParam->iMoveTimeout, pNewParam->iMoveTimeout), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Move Timeout"), _T("Parameter"), pOrgParam->iMoveTimeout, pNewParam->iMoveTimeout));
					}
					if (pNewParam->dInPosition != pOrgParam->dInPosition) {
						changeDatas.push_back({ Format(_T("InPosition %.3f -> %.3f"), pOrgParam->dInPosition, pNewParam->dInPosition), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("InPosition"), _T("Parameter"), pOrgParam->dInPosition, pNewParam->dInPosition));
					}
					if (pNewParam->dDefaultSpped != pOrgParam->dDefaultSpped) {
						changeDatas.push_back({ Format(_T("Default Spped %.3f -> %.3f"), pOrgParam->dDefaultSpped, pNewParam->dDefaultSpped), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Default Spped"), _T("Parameter"), pOrgParam->dDefaultSpped, pNewParam->dDefaultSpped));
					}
					
					// 2021.03.22
					// dGantryOffsetLimitWarning값이 없으면 dGantryOffsetLimitAlert 값의 반을 세팅
					if (pNewParam->dGantryOffsetLimitWarning < 0.001)
					{
						pNewParam->dGantryOffsetLimitWarning = pNewParam->dGantryOffsetLimitAlert * 0.5;
					}

					// 2024.04.15 Gantry offset 변경&사용가능 하도록 주석 제거 <<
					if (pNewParam->dGantryOffsetLimitWarning != pOrgParam->dGantryOffsetLimitWarning){
						changeDatas.push_back({ Format(_T("Gantry Offset Limit Warning %.3f -> %.3f"), pOrgParam->dGantryOffsetLimitWarning, pNewParam->dGantryOffsetLimitWarning), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Gantry Offset Limit Warning"), _T("Parameter"), pOrgParam->dGantryOffsetLimitWarning, pNewParam->dGantryOffsetLimitWarning));
					}
					if (pNewParam->dGantryOffsetLimitAlert != pOrgParam->dGantryOffsetLimitAlert)	 {
						changeDatas.push_back({ Format(_T("Gantry Offset Limit Alert %.3f -> %.3f"), pOrgParam->dGantryOffsetLimitAlert, pNewParam->dGantryOffsetLimitAlert), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Gantry Offset Limit Alert"), _T("Parameter"), pOrgParam->dGantryOffsetLimitAlert, pNewParam->dGantryOffsetLimitAlert));
					}
					// >>
					if (pNewParam->iAcc != pOrgParam->iAcc) {
						changeDatas.push_back({ Format(_T("Acc %d -> %d"), pOrgParam->iAcc, pNewParam->iAcc), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Acc"), _T("Parameter"), pOrgParam->iAcc, pNewParam->iAcc));
					}
					if (pNewParam->iDec != pOrgParam->iDec)	{
						changeDatas.push_back({ Format(_T("Dec %d -> %d"), pOrgParam->iDec, pNewParam->iDec), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Dec"), _T("Parameter"), pOrgParam->iDec, pNewParam->iDec));
					}
					if (pNewParam->dFilterTime != pOrgParam->dFilterTime) {
						changeDatas.push_back({ Format(_T("Filter Time %.3f -> %.3f"), pOrgParam->dFilterTime, pNewParam->dFilterTime), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Filter Time"), _T("Parameter"), pOrgParam->dFilterTime, pNewParam->dFilterTime));
					}
					if (pNewParam->iMaxVelocity != pOrgParam->iMaxVelocity) {
						changeDatas.push_back({ Format(_T("Max Velocity %d -> %d"), pOrgParam->iMaxVelocity, pNewParam->iMaxVelocity), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%d;%d"), _T("Max Velocity"), _T("Parameter"), pOrgParam->iMaxVelocity, pNewParam->iMaxVelocity));
					}
					if (pNewParam->dLowSpeed != pOrgParam->dLowSpeed) {
						changeDatas.push_back({ Format(_T("Low Speed %.3f -> %.3f"), pOrgParam->dLowSpeed, pNewParam->dLowSpeed), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Low Speed"), _T("Parameter"), pOrgParam->dLowSpeed, pNewParam->dLowSpeed));
					}
					if (pNewParam->dMediumSpeed != pOrgParam->dMediumSpeed) {	// KES 중속 추가 2024.2.15
						changeDatas.push_back({ Format(_T("Medium Speed %.3f -> %.3f"), pOrgParam->dMediumSpeed, pNewParam->dMediumSpeed), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Medium Speed"), _T("Parameter"), pOrgParam->dMediumSpeed, pNewParam->dMediumSpeed));
					}
					if (pNewParam->dHighSpeed != pOrgParam->dHighSpeed) {
						changeDatas.push_back({ Format(_T("High Speed %.3f -> %.3f"), pOrgParam->dHighSpeed, pNewParam->dHighSpeed), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("High Speed"), _T("Parameter"), pOrgParam->dHighSpeed, pNewParam->dHighSpeed));
					}
					if (pNewParam->dPTPSpeed != pOrgParam->dPTPSpeed)	{
						changeDatas.push_back({ Format(_T("PTP Speed %.3f -> %.3f"), pOrgParam->dPTPSpeed, pNewParam->dPTPSpeed), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("PTP Speed"), _T("Parameter"), pOrgParam->dPTPSpeed, pNewParam->dPTPSpeed));
					}
					if (pNewParam->dLimitLow != pOrgParam->dLimitLow)	{
						changeDatas.push_back({ Format(_T("Limit Low %.3f -> %.3f"), pOrgParam->dLimitLow, pNewParam->dLimitLow), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Limit Low"), _T("Parameter"), pOrgParam->dLimitLow, pNewParam->dLimitLow));
					}
					if (pNewParam->dLimitHigh != pOrgParam->dLimitHigh)	{
						changeDatas.push_back({ Format(_T("Limit High %.3f -> %.3f"), pOrgParam->dLimitHigh, pNewParam->dLimitHigh), RGB_WHITE });
						ChangeLog.push_back(Format(_T("%s;%s;%.3f;%.3f"), _T("Limit High"), _T("Parameter"), pOrgParam->dLimitHigh, pNewParam->dLimitHigh));
					}
					if (!changeDatas.empty()) {
						changeDatasAll.push_back({ Format(_T("[%s]"), strMotor), RGB_SKY_LIGHT });
						for (int i = 0; i < changeDatas.size(); i++) {
							changeDatasAll.push_back(changeDatas[i]);
						}
					}
					if (!ChangeLog.empty()) {
						for (int i = 0; i < ChangeLog.size(); i++) {
							ChangeLogAll.push_back(Format(_T("[%s] "), strMotor) + ChangeLog[i]);
						}
					}

				}
				if (changeDatasAll.empty() || ChangeLogAll.empty())
					return;
				CChangeParamDlg dlg;
				dlg.SetChangeData(changeDatasAll);
				if (dlg.DoModal() == IDOK)
				{
					WriteParameterLog(g_st[IDS_MOTOR_PARAM_WND_SERVO_PARAM], _T(""), ChangeLogAll, theApp.m_UserManager.GetCurrentUserData().szID);
					SyncData(TRUE);
					m_bEdit = FALSE;
				}
				break;
			}

			case eC_YASKAWA_INIT_PARAMETER_RACK_NO: {
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tYaskawa.iRackNo);
				break;			
			}
			case eC_YASKAWA_INIT_PARAMETER_SLOT_NO: {
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tYaskawa.iSlotNo);
				break;			
			}
			case eC_YASKAWA_INIT_PARAMETER_SUBSLOT_NO: {			
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tYaskawa.iSubSlotNo);
				break;			
			}
			case eC_YASKAWA_INIT_PARAMETER_GANTRY_RACK_NO: {
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tYaskawa.iGantryRackNo);
				break;
			}
			case eC_YASKAWA_INIT_PARAMETER_GANTRY_SLOT_NO: {
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tYaskawa.iGantrySlotNo);
				break;
			}
			case eC_YASKAWA_INIT_PARAMETER_GANTRY_SUB_SLOT_NO: {
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tYaskawa.iGantrySubSlotNo);
				break;
			}

			case eC_YASKAWA_INIT_PARAMETER_MOTOR_NO: {			
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tYaskawa.iMotorNo);
				break;			
			}
			case eC_YASKAWA_INIT_PARAMETER_LOGICAL_AXIS_NO: {	
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tYaskawa.iLogicalAxisNo);
				break;			
			}
			case eC_YASKAWA_INIT_PARAMETER_GANTRY_USE:{
				m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry = !m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry;
				break;
			}
			case eC_YASKAWA_INIT_PARAMETER_GANTRY_ADDRESS_STOP: {

				if (m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry) {
					EditStart(this, m_rectC[m_eSelectControl], CString(m_MotorParameter[m_eSelectMotor].tYaskawa.szGantryAddressStop), TRUE);
				}

				break;
			}
			case eC_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_INDEX_1:{
				if (m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry) {

					SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tYaskawa.iCh1_Axis);
				}
				break;			
			}
			case eC_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_INDEX_2:{

				if (m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry) {

					SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tYaskawa.iCh2_Axis);
				}
				break;
			}
			case eC_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_MOTOR_INDEX_1: {

				if (m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry) {
					SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tYaskawa.iCh1_Motor);
				}

				break;
			}
			case eC_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_MOTOR_INDEX_2: {

				if (m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry) {

					SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tYaskawa.iCh2_Motor);
				}
				break;
			}
			case eC_YASKAWA_HOME_PARAMETER_2CH_USE: {
				if (m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry) {
					m_MotorParameter[m_eSelectMotor].tHome.bGantry2ChUse = !m_MotorParameter[m_eSelectMotor].tHome.bGantry2ChUse;
				}
				break;			
			}
			case eC_YASKAWA_HOME_PARAMETER_TORQUE: {
				if (m_MotorParameter[m_eSelectMotor].tHome.iHomeType == eHT_TORQUE || m_MotorParameter[m_eSelectMotor].tHome.iHomeType == eHT_HOME) {
					EditStart(this, m_rectC[m_eSelectControl], m_MotorParameter[m_eSelectMotor].tHome.dTorque);
				}
				break;			
			}

			case eC_YASKAWA_HOME_PARAMETER_VELOCITY: {
				if (m_MotorParameter[m_eSelectMotor].tHome.iHomeType != eHT_TORQUE ) {//&& m_MotorParameter[m_eSelectMotor].tHome.iHomeType != eHT_DEFAULT) {
					EditStart(this, m_rectC[m_eSelectControl], m_MotorParameter[m_eSelectMotor].tHome.dVelocity);
				}
				break;			
			}
			case eC_YASKAWA_HOME_PARAMETER_APPROACH_VELOCITY: {
				//if (m_MotorParameter[m_eSelectMotor].tHome.iHomeType != eHT_DEFAULT) {
					EditStart(this, m_rectC[m_eSelectControl], m_MotorParameter[m_eSelectMotor].tHome.dApproachVelocity);
				//}
				break;			
			}
			case eC_YASKAWA_HOME_PARAMETER_CREEP_VELOCITY: {
				//if (m_MotorParameter[m_eSelectMotor].tHome.iHomeType != eHT_DEFAULT) {
					EditStart(this, m_rectC[m_eSelectControl], m_MotorParameter[m_eSelectMotor].tHome.dCreepVelocity);
				//}
				break;			
			}
			case eC_YASKAWA_HOME_PARAMETER_OFFSET: {	
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tHome.dOffset);
				
				break;			
			}
			case eC_YASKAWA_HOME_PARAMETER_OFFSET2: {
				if (m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry) {
					SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tHome.dOffset2Ch);
				}
				
				break;
			}
			case eC_YASKAWA_HOME_PARAMETER_HOME_SERCH_TIMEOUT: {
				//if (m_MotorParameter[m_eSelectMotor].tHome.iHomeType != eHT_DEFAULT) {
					SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tHome.iHomeSerchTimeout);
				//}
				break;			
			}
			case eC_YASKAWA_HOME_PARAMETER_INIT_POSISITION: {
				//if (m_MotorParameter[m_eSelectMotor].tHome.iHomeType != eHT_DEFAULT) {
					SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].tHome.dInitPosisition);
				//}
				break;
			}
			case eC_YASKAWA_HOME_PARAMETER_INIT_POSISITION_GET: {
				//if (m_MotorParameter[m_eSelectMotor].tHome.iHomeType != eHT_DEFAULT) {
					m_MotorParameter[m_eSelectMotor].tHome.dInitPosisition = sm.MotorStatus[m_eSelectMotor].tCrt.dPos;
				//}
				break;
			}
			case eC_YASKAWA_HOME_PARAMETER_EXTERNAL_HOME_SENSOR: {
				//if (m_MotorParameter[m_eSelectMotor].tHome.iHomeType != eHT_DEFAULT) {
					EditStart(this, m_rectC[m_eSelectControl], CString(m_MotorParameter[m_eSelectMotor].tHome.szExternalHomeSensorAddress), TRUE);
				//}
				break;
			}
			case eC_PARAMETER_API_TIMEOUT: {		
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].iAPITimeout);
				break;			
			}
			case eC_PARAMETER_POSITION_RATE: {		
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].dPositionRate);
				break;			
			}
			case eC_PARAMETER_SPEED_RATE: {			
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].dSpeedRate);
				break;			
			}
			case eC_PARAMETER_MOVE_TIMEOUT: {		
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].iMoveTimeout);
				break;			
			}
			case eC_PARAMETER_INPOSITION: {			
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].dInPosition);
				break;			
			}
			case eC_PARAMETER_DEFAULT_SPPED: {		
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].dDefaultSpped);
				break;			
			}
			case eC_PARAMETER_GANTRY_OFFSET_LIMIT_WARNING: {
				//. Gantry일 경우만 입력 가능하다.
				if (m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry) {
					SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].dGantryOffsetLimitWarning);
				}
				break;
			}
			case eC_PARAMETER_GANTRY_OFFSET_LIMIT_ALERT: {
				//. Gantry일 경우만 입력 가능하다.
				if (m_MotorParameter[m_eSelectMotor].tYaskawa.bSetGantry) {
					SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].dGantryOffsetLimitAlert);
				}
				break;
			}
			case eC_PARAMETER_ACC: {				
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].iAcc);
				break;			
			}
			case eC_PARAMETER_DEC: {				
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].iDec);
				break;			
			}
			case eC_PARAMETER_FILTER_TIME: {		
				//TODO filter time의 단위는 us지만. UI 표시 상에는 s로 표기 해야함.
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].dFilterTime); 
				break;			
			}
			case eC_PARAMETER_MAX_VELOCITY: {	
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].iMaxVelocity);
				break;			
			}
			case eC_PARAMETER_LOW_SPEED: {			
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].dLowSpeed);
				break;			
			}
			case eC_PARAMETER_MEDIUM_SPEED: {			
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].dMediumSpeed);	// KES 중속 추가 2024.2.15
				break;			
			}
			case eC_PARAMETER_HIGH_SPEED: {			
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].dHighSpeed);
				break;			
			}
			case eC_PARAMETER_PTP_SPEED: {		
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].dPTPSpeed);
				break;			
			}
			case eC_PARAMETER_LIMIT_LOW: {			
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].dLimitLow);
				break;			
			}
			case eC_PARAMETER_LIMIT_HIGH: {		
				SHOW_KEYPAD_EDIT(this, m_eSelectControl, m_MotorParameter[m_eSelectMotor].dLimitHigh);
				break;			
			}
			case eC_PARAMETER_LIMIT_LOW_GET: {
				m_MotorParameter[m_eSelectMotor].dLimitLow = GetPos(m_eSelectMotor);
				break;
			}
			case eC_PARAMETER_LIMIT_HIGH_GET: {
				m_MotorParameter[m_eSelectMotor].dLimitHigh = GetPos(m_eSelectMotor);
				break;
			}
			case eC_BTN_ALL_SET_INIT: {
				//ULD 에서 축정보 다날라가는 경우생겨서 일단 위험해서 주석처리
				//for (int i = 0; i < eMOTOR_END; i++) {
				//	m_MotorParameter[i].tYaskawa = m_MotorParameter[m_eSelectMotor].tYaskawa;
				//}
				strLog = _T("All Set Init");
				break;
			}
			case eC_BTN_ALL_SET_HOME: {
				// 12.18 ULD 데이터 날라감 (위험해서 일단 주석처리)
				//for (int i = 0; i < eMOTOR_END; i++) {
				//	m_MotorParameter[i].tHome = m_MotorParameter[m_eSelectMotor].tHome;
				//}
				strLog = _T("All Set Home");
				break;
			}
			case eC_BTN_ALL_SET_PARAMETER: {
				for (int i = 0; i < eMOTOR_END; i++) {
					m_MotorParameter[i].iAPITimeout					= m_MotorParameter[m_eSelectMotor].iAPITimeout		;
					m_MotorParameter[i].iCoordinateSystem			= m_MotorParameter[m_eSelectMotor].iCoordinateSystem;
					m_MotorParameter[i].iMoveType					= m_MotorParameter[m_eSelectMotor].iMoveType		;
					m_MotorParameter[i].iVelocityType				= m_MotorParameter[m_eSelectMotor].iVelocityType	;
					m_MotorParameter[i].iAccDecType					= m_MotorParameter[m_eSelectMotor].iAccDecType		;
					m_MotorParameter[i].iFilterType					= m_MotorParameter[m_eSelectMotor].iFilterType		;
					m_MotorParameter[i].iDataType					= m_MotorParameter[m_eSelectMotor].iDataType		;
					m_MotorParameter[i].dPositionRate				= m_MotorParameter[m_eSelectMotor].dPositionRate	;	
					m_MotorParameter[i].dSpeedRate					= m_MotorParameter[m_eSelectMotor].dSpeedRate		;	
					m_MotorParameter[i].iMoveTimeout				= m_MotorParameter[m_eSelectMotor].iMoveTimeout		;
					m_MotorParameter[i].dInPosition					= m_MotorParameter[m_eSelectMotor].dInPosition		;
					m_MotorParameter[i].dDefaultSpped				= m_MotorParameter[m_eSelectMotor].dDefaultSpped	;	
					m_MotorParameter[i].iAcc						= m_MotorParameter[m_eSelectMotor].iAcc				;
					m_MotorParameter[i].iDec						= m_MotorParameter[m_eSelectMotor].iDec				;
					m_MotorParameter[i].dFilterTime					= m_MotorParameter[m_eSelectMotor].dFilterTime		;
					m_MotorParameter[i].iMaxVelocity				= m_MotorParameter[m_eSelectMotor].iMaxVelocity		;
					m_MotorParameter[i].dLowSpeed					= m_MotorParameter[m_eSelectMotor].dLowSpeed		;	
					m_MotorParameter[i].dMediumSpeed				= m_MotorParameter[m_eSelectMotor].dMediumSpeed		;	// KES 중속 추가 2024.2.15
					m_MotorParameter[i].dHighSpeed					= m_MotorParameter[m_eSelectMotor].dHighSpeed		;	
					m_MotorParameter[i].dPTPSpeed					= m_MotorParameter[m_eSelectMotor].dPTPSpeed		;	
					m_MotorParameter[i].dLimitLow					= m_MotorParameter[m_eSelectMotor].dLimitLow		;	
					m_MotorParameter[i].dLimitHigh					= m_MotorParameter[m_eSelectMotor].dLimitHigh		;	
					m_MotorParameter[i].dGantryOffsetLimitWarning   = m_MotorParameter[m_eSelectMotor].dGantryOffsetLimitWarning;
					m_MotorParameter[i].dGantryOffsetLimitAlert     = m_MotorParameter[m_eSelectMotor].dGantryOffsetLimitAlert ;

					//<<
					// dGantryLimitOffset은 ALL SET PARAMETER에 넣지 않는다. 축마다 다르게 가져가야 한다.
					// szBreakReleaseCh1 , szBreakReleaseCh2는 ALL SET PARAMETER에 넣지 않는다. 축마다 다르게 가져가야하는 크리티컬한 부분임.
					//>>
				}
				strLog = _T("All Set Parameter");
				break;
			}
			default: break;
		}
	}
}

void  CMotorParameterWnd::OnCbnSelchangeComboType(UINT nID) {
	
	//. 여러군데 쓰일 것 같아서 함수화.
	SetDataFromComboBox();
}

bool CMotorParameterWnd::SetDataFromComboBox() {

	static auto cboToValue = [&](int iControl) -> int {
		
		CStringW strData;
		m_cboTypes[iControl].GetWindowText(strData);

		int iValue = -1;
		try {

			iValue = std::stoi(CStringA(strData).GetBuffer());
				
		}
		catch (std::exception& exp) {

			IPMessage(Format(_T("SetDataFromComboBox() - stoi() Failed. Data : %s, Reason : %s. "), strData, CStringW(exp.what())));
		}
		return iValue;
	};

	//. 개별적으로 case를 나누면 코드 늘어나서 그냥... 모두 업데이트 처리하자
	//<< CoboBox
	m_MotorParameter[m_eSelectMotor].tYaskawa.iAxisType		= cboToValue(eCOMBO_YASKAWA_INIT_PARAMETER_AXIS_TYPE);
	m_MotorParameter[m_eSelectMotor].tHome.iHomeType		= cboToValue(eCOMBO_YASKAWA_HOME_PARAMETER_HOME_TYPE);
	m_MotorParameter[m_eSelectMotor].tHome.iDirection		= cboToValue(eCOMBO_YASKAWA_HOME_PARAMETER_DIRECTION);
	m_MotorParameter[m_eSelectMotor].iCoordinateSystem		= cboToValue(eCOMBO_PARAMETER_COORDINATE_SYSTEM);
	m_MotorParameter[m_eSelectMotor].iMoveType				= cboToValue(eCOMBO_PARAMETER_MOVE_TYPE);
	m_MotorParameter[m_eSelectMotor].iVelocityType			= cboToValue(eCOMBO_PARAMETER_VELOCITY_TYPE);
	m_MotorParameter[m_eSelectMotor].iAccDecType			= cboToValue(eCOMBO_PARAMETER_ACCDEC_TYPE);
	m_MotorParameter[m_eSelectMotor].iFilterType			= cboToValue(eCOMBO_PARAMETER_FILTER_TYPE);
	m_MotorParameter[m_eSelectMotor].iDataType				= cboToValue(eCOMBO_PARAMETER_DATA_TYPE);
	//>>

	Invalidate(FALSE);

	return true;
}

bool CMotorParameterWnd::SetComboBoxFromData() {


	//<<유효성 검사.
	static auto isValidData = [&](ComboBoxName iCbo, int iData) -> bool {

		
		const std::vector<int>& vec = m_arrComboBoxComponents[iCbo].vecComboBoxData;

		auto result = std::find(vec.begin(), vec.end(), iData);
		
		return result != vec.end();
	};

	static auto setComboBox = [&](ComboBoxName iCbo, int& iData) {

		if (isValidData(iCbo, iData)) {

			if (iCbo == eCOMBO_YASKAWA_INIT_PARAMETER_AXIS_TYPE) {
				
				//. 데이터 항목 값이 1부터 시작해서.. SetCurSel 해줄때는 0부터
				m_cboTypes[iCbo].SetCurSel(iData - 1);
			}
			else {

				m_cboTypes[iCbo].SetCurSel(iData);
			}
			
			return;
		}

		m_cboTypes[iCbo].SetCurSel(0);
		//. 값이 유효하지 않으면 0으로 초기화 해줌.
		iData = 0;
	};
	
	//>>

	setComboBox(eCOMBO_YASKAWA_INIT_PARAMETER_AXIS_TYPE, m_MotorParameter[m_eSelectMotor].tYaskawa.iAxisType);
	setComboBox(eCOMBO_YASKAWA_HOME_PARAMETER_HOME_TYPE, m_MotorParameter[m_eSelectMotor].tHome.iHomeType);
	setComboBox(eCOMBO_YASKAWA_HOME_PARAMETER_DIRECTION, m_MotorParameter[m_eSelectMotor].tHome.iDirection);
	setComboBox(eCOMBO_PARAMETER_COORDINATE_SYSTEM, m_MotorParameter[m_eSelectMotor].iCoordinateSystem);
	setComboBox(eCOMBO_PARAMETER_MOVE_TYPE, m_MotorParameter[m_eSelectMotor].iMoveType);
	setComboBox(eCOMBO_PARAMETER_VELOCITY_TYPE, m_MotorParameter[m_eSelectMotor].iVelocityType);
	setComboBox(eCOMBO_PARAMETER_ACCDEC_TYPE, m_MotorParameter[m_eSelectMotor].iAccDecType);
	setComboBox(eCOMBO_PARAMETER_FILTER_TYPE, m_MotorParameter[m_eSelectMotor].iFilterType);
	setComboBox(eCOMBO_PARAMETER_DATA_TYPE, m_MotorParameter[m_eSelectMotor].iDataType);
	
	return true;
}

void CMotorParameterWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect		rect = m_rectS[eS_FULL_AREA];

	double dH = (rect.Height() - 5.) / 16.;
	double dW = (rect.Width() - 5.) / 20.;

	m_rectS[eS_MOTOR_LIST		] = CRect(rect.left + dW*0 +5,	rect.top + dH*0 +5, rect.left + dW*5,	rect.top+dH*16);
	m_rectS[eS_WORK_AREA		] = CRect(rect.left + dW*5 +5,	rect.top + dH*1 +5,	rect.left + dW*20,	rect.top+dH*16);

	m_rectC[eC_MOTOR_NAME			] = CRect(rect.left + dW*5 +5,	rect.top + dH*0 +5, rect.left + dW*7,		rect.top+dH*1);
	m_rectC[eC_BTN_ALL_SET_INIT		] = CRect(rect.left + dW*7 +5,	rect.top + dH*0 +5, rect.left + dW*9,		rect.top+dH*1);
	m_rectC[eC_BTN_ALL_SET_HOME		] = CRect(rect.left + dW*9 +5,	rect.top + dH*0 +5, rect.left + dW*11,		rect.top+dH*1);
	m_rectC[eC_BTN_ALL_SET_PARAMETER] = CRect(rect.left + dW*11 +5,	rect.top + dH*0 +5, rect.left + dW*13,		rect.top+dH*1);	
	m_rectC[eC_EDIT					] = CRect(rect.left + dW*18 +5,	rect.top + dH*0 +5, rect.left + dW*19,		rect.top+dH*1);			
	m_rectC[eC_SAVE					] = CRect(rect.left + dW*19 +5,	rect.top + dH*0 +5, rect.left + dW*20,		rect.top+dH*1);
	
	rect = m_rectS[eS_WORK_AREA];
	dH = (rect.Height() - 5.) / 15.;
	dW = (rect.Width() - 5.) / 12.;

	m_rectS[eS_YASKAWA_INIT_AREA] = CRect(rect.left + dW*0 +5, rect.top + dH*0 +5,	rect.left + dW*4,	rect.top+dH*8);
	m_rectS[eS_YASKAWA_HOME_AREA] = CRect(rect.left + dW*0 +5, rect.top + dH*8 +5,	rect.left + dW*4,	rect.top+dH*15);
	m_rectS[eS_PARAMETER_AREA	] = CRect(rect.left + dW*4 +5, rect.top + dH*0 +5,	rect.left + dW*12,	rect.top+dH*15);

	rect = m_rectS[eS_YASKAWA_INIT_AREA];
	rect.DeflateRect(0, 15, 0, 0);
	dH = (rect.Height() - 5.) / 10.;
	dW = (rect.Width() - 5.) / 4.;

	m_rectS[eS_YASKAWA_INIT_SINGLE_AREA					] = CRect(rect.left + dW*0 +5, rect.top + dH*0 +5, rect.left + dW*4, rect.top+dH*5);
	m_rectS[eS_YASKAWA_INIT_GANTRY_AREA					] = CRect(rect.left + dW*0 +5, rect.top + dH*5 +5, rect.left + dW*4, rect.top+dH*9);
	m_rectS[eS_YASKAWA_INIT_PARAMETER_AXIS_TYPE			] = CRect(rect.left + dW*0 +5, rect.top + dH*9 +5, rect.left + dW*2, rect.top+dH*10);
	m_rectC[eC_YASKAWA_INIT_PARAMETER_AXIS_TYPE			] = CRect(rect.left + dW*2 +5, rect.top + dH*9 +5, rect.left + dW*4, rect.top+dH*10);


	rect = m_rectS[eS_YASKAWA_INIT_SINGLE_AREA];
	dH = (rect.Height() - 5.) / 5.;
	dW = (rect.Width() - 5.) / 4.;

	m_rectS[eS_YASKAWA_INIT_PARAMETER_RACK_NO			] = CRect(rect.left + dW*0 +5, rect.top + dH*0 +5, rect.left + dW*2, rect.top+dH*1);
	m_rectS[eS_YASKAWA_INIT_PARAMETER_SLOT_NO			] = CRect(rect.left + dW*0 +5, rect.top + dH*1 +5, rect.left + dW*2, rect.top+dH*2);
	m_rectS[eS_YASKAWA_INIT_PARAMETER_SUBSLOT_NO		] = CRect(rect.left + dW*0 +5, rect.top + dH*2 +5, rect.left + dW*2, rect.top+dH*3);
	m_rectS[eS_YASKAWA_INIT_PARAMETER_MOTOR_NO			] = CRect(rect.left + dW*0 +5, rect.top + dH*3 +5, rect.left + dW*2, rect.top+dH*4);
	m_rectS[eS_YASKAWA_INIT_PARAMETER_LOGICAL_AXIS_NO	] = CRect(rect.left + dW*0 +5, rect.top + dH*4 +5, rect.left + dW*2, rect.top+dH*5);
	
	m_rectC[eC_YASKAWA_INIT_PARAMETER_RACK_NO			] = CRect(rect.left + dW*2 +5, rect.top + dH*0 +5, rect.left + dW*4, rect.top+dH*1);
	m_rectC[eC_YASKAWA_INIT_PARAMETER_SLOT_NO			] = CRect(rect.left + dW*2 +5, rect.top + dH*1 +5, rect.left + dW*4, rect.top+dH*2);
	m_rectC[eC_YASKAWA_INIT_PARAMETER_SUBSLOT_NO		] = CRect(rect.left + dW*2 +5, rect.top + dH*2 +5, rect.left + dW*4, rect.top+dH*3);
	m_rectC[eC_YASKAWA_INIT_PARAMETER_MOTOR_NO			] = CRect(rect.left + dW*2 +5, rect.top + dH*3 +5, rect.left + dW*4, rect.top+dH*4);
	m_rectC[eC_YASKAWA_INIT_PARAMETER_LOGICAL_AXIS_NO	] = CRect(rect.left + dW*2 +5, rect.top + dH*4 +5, rect.left + dW*4, rect.top+dH*5);
	
	rect = m_rectS[eS_YASKAWA_INIT_GANTRY_AREA];
	dH = (rect.Height() - 5.) / 4.;
	dW = (rect.Width() - 5.) / 12.;

	m_rectS[eS_YASKAWA_INIT_PARAMETER_GANTRY_ADDRESS			] = CRect(rect.left + dW*0 +5, rect.top + dH*0 +5, rect.left + dW*6, rect.top+dH*1);
	m_rectS[eS_YASKAWA_INIT_PARAMETER_GANTRY_SLOT				] = CRect(rect.left + dW*0 +5, rect.top + dH*1 +5, rect.left + dW*6, rect.top+dH*2);
	m_rectS[eS_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_INDEX_1		] = CRect(rect.left + dW*0 +5, rect.top + dH*2 +5, rect.left + dW*6, rect.top+dH*3);
	m_rectS[eS_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_INDEX_2		] = CRect(rect.left + dW*0 +5, rect.top + dH*3 +5, rect.left + dW*6, rect.top+dH*4);

	m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_USE				] = CRect(rect.left + dW*6 +5, rect.top + dH*0 +5, rect.left + dW*8, rect.top+dH*1);
	m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_ADDRESS_STOP		] = CRect(rect.left + dW*10 +5, rect.top + dH*0 +5, rect.left + dW*12, rect.top+dH*1);
	m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_RACK_NO			] = CRect(rect.left + dW*6 +5,	rect.top + dH*1 +5, rect.left + dW*8, rect.top+dH*2);
	m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_SLOT_NO			] = CRect(rect.left + dW*8 +5,	rect.top + dH*1 +5, rect.left + dW*10, rect.top+dH*2);
	m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_SUB_SLOT_NO		] = CRect(rect.left + dW*10 +5, rect.top + dH*1 +5, rect.left + dW*12, rect.top+dH*2);
	m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_MOTOR_INDEX_1	] = CRect(rect.left + dW*6 +5,	rect.top + dH*2 +5, rect.left + dW*9,	rect.top+dH*3);
	m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_INDEX_1		] = CRect(rect.left + dW*9 +5,	rect.top + dH*2 +5, rect.left + dW*12,	rect.top+dH*3);
	m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_MOTOR_INDEX_2	] = CRect(rect.left + dW*6 +5,	rect.top + dH*3 +5, rect.left + dW*9,	rect.top+dH*4);
	m_rectC[eC_YASKAWA_INIT_PARAMETER_GANTRY_AXIS_INDEX_2		] = CRect(rect.left + dW*9 +5,	rect.top + dH*3 +5, rect.left + dW*12,	rect.top+dH*4);


	rect = m_rectS[eS_YASKAWA_HOME_AREA];
	dH = (rect.Height() - 5.) / 10.;
	dW = (rect.Width() - 5.) / 4.;

	m_rectS[eS_YASKAWA_HOME_PARAMETER_METHOD			  ] = CRect(rect.left + dW*0 +5, rect.top + dH*0 +5, rect.left + dW*2, rect.top+dH*1);
	m_rectS[eS_YASKAWA_HOME_PARAMETER_TORQUE			  ] = CRect(rect.left + dW*0 +5, rect.top + dH*1 +5, rect.left + dW*2, rect.top+dH*2);
	m_rectS[eS_YASKAWA_HOME_PARAMETER_DIRECTION			  ] = CRect(rect.left + dW*0 +5, rect.top + dH*2 +5, rect.left + dW*2, rect.top+dH*3);
	m_rectS[eS_YASKAWA_HOME_PARAMETER_VELOCITY			  ] = CRect(rect.left + dW*0 +5, rect.top + dH*3 +5, rect.left + dW*2, rect.top+dH*4);
	m_rectS[eS_YASKAWA_HOME_PARAMETER_APPROACH_VELOCITY	  ] = CRect(rect.left + dW*0 +5, rect.top + dH*4 +5, rect.left + dW*2, rect.top+dH*5);
	m_rectS[eS_YASKAWA_HOME_PARAMETER_CREEP_VELOCITY	  ] = CRect(rect.left + dW*0 +5, rect.top + dH*5 +5, rect.left + dW*2, rect.top+dH*6);
	m_rectS[eS_YASKAWA_HOME_PARAMETER_OFFSET			  ] = CRect(rect.left + dW*0 +5, rect.top + dH*6 +5, rect.left + dW*2, rect.top+dH*7);
	m_rectS[eS_YASKAWA_HOME_PARAMETER_OFFSET2			  ] = CRect(rect.left + dW*3 +5, rect.top + dH*6 +5, rect.left + dW*3.3, rect.top+dH*7);
	m_rectS[eS_YASKAWA_HOME_PARAMETER_HOME_SERCH_TIMEOUT  ] = CRect(rect.left + dW*0 +5, rect.top + dH*7 +5, rect.left + dW*2, rect.top+dH*8);
	m_rectS[eS_YASKAWA_HOME_PARAMETER_INIT_POSISITION	  ] = CRect(rect.left + dW*0 +5, rect.top + dH*8 +5, rect.left + dW*2, rect.top+dH*9);
	m_rectS[eS_YASKAWA_HOME_PARAMETER_EXTERNAL_HOME_SENSOR] = CRect(rect.left + dW*0 +5, rect.top + dH*9 +5, rect.left + dW*2, rect.top+dH*10);

	
	m_rectC[eC_YASKAWA_HOME_PARAMETER_HOME_TYPE			  ] = CRect(rect.left + dW*2 +5, rect.top + dH*0 +5, rect.left + dW*3.3, rect.top+dH*1);
	m_rectC[eC_YASKAWA_HOME_PARAMETER_2CH_USE			  ] = CRect(rect.left + dW*3.3 +5, rect.top + dH*0 +5, rect.left + dW*4, rect.top+dH*1);
	m_rectC[eC_YASKAWA_HOME_PARAMETER_TORQUE			  ] = CRect(rect.left + dW*2 +5, rect.top + dH*1 +5, rect.left + dW*4, rect.top+dH*2);
	m_rectC[eC_YASKAWA_HOME_PARAMETER_DIRECTION			  ] = CRect(rect.left + dW*2 +5, rect.top + dH*2 +5, rect.left + dW*4, rect.top+dH*3);
	m_rectC[eC_YASKAWA_HOME_PARAMETER_VELOCITY			  ] = CRect(rect.left + dW*2 +5, rect.top + dH*3 +5, rect.left + dW*4, rect.top+dH*4);
	m_rectC[eC_YASKAWA_HOME_PARAMETER_APPROACH_VELOCITY	  ] = CRect(rect.left + dW*2 +5, rect.top + dH*4 +5, rect.left + dW*4, rect.top+dH*5);
	m_rectC[eC_YASKAWA_HOME_PARAMETER_CREEP_VELOCITY	  ] = CRect(rect.left + dW*2 +5, rect.top + dH*5 +5, rect.left + dW*4, rect.top+dH*6);
	m_rectC[eC_YASKAWA_HOME_PARAMETER_OFFSET			  ] = CRect(rect.left + dW*2 +5, rect.top + dH*6 +5, rect.left + dW*3, rect.top+dH*7);
	m_rectC[eC_YASKAWA_HOME_PARAMETER_OFFSET2			  ] = CRect(rect.left + dW*3.3 +5, rect.top + dH*6 +5, rect.left + dW*4, rect.top+dH*7);
	m_rectC[eC_YASKAWA_HOME_PARAMETER_HOME_SERCH_TIMEOUT  ] = CRect(rect.left + dW*2 +5, rect.top + dH*7 +5, rect.left + dW*4, rect.top+dH*8);
	m_rectC[eC_YASKAWA_HOME_PARAMETER_INIT_POSISITION     ] = CRect(rect.left + dW*2 +5, rect.top + dH*8 +5, rect.left + dW*3.7, rect.top+dH*9);
	m_rectC[eC_YASKAWA_HOME_PARAMETER_INIT_POSISITION_GET ] = CRect(rect.left + dW*3.7 +5, rect.top + dH*8 +5, rect.left + dW*4, rect.top+dH*9);
	
	m_rectC[eC_YASKAWA_HOME_PARAMETER_EXTERNAL_HOME_SENSOR] = CRect(rect.left + dW*2 +5, rect.top + dH*9 +5, rect.left + dW*3.7, rect.top+dH*10);
	m_rectC[eC_HOME_SENSOR_ON_OFF					      ] = CRect(rect.left + dW*3.7 +5, rect.top + dH*9 +5, rect.left + dW*4, rect.top+dH*10);

	rect = m_rectS[eS_PARAMETER_AREA];
	dH = (rect.Height() - 5.) / 15.;
	dW = (rect.Width() - 5.) / 4.;

	m_rectS[eS_PARAMETER_API_TIMEOUT					] = CRect(rect.left + dW*0 +5, rect.top + dH*0 +5,	rect.left + dW*1, rect.top+dH*1 );
	m_rectS[eS_PARAMETER_COORDINATE_SYSTEM				] = CRect(rect.left + dW*0 +5, rect.top + dH*1 +5,	rect.left + dW*1, rect.top+dH*2 );
	m_rectS[eS_PARAMETER_MOVE_TYPE						] = CRect(rect.left + dW*0 +5, rect.top + dH*2 +5,	rect.left + dW*1, rect.top+dH*3 );
	m_rectS[eS_PARAMETER_VELOCITY_TYPE					] = CRect(rect.left + dW*0 +5, rect.top + dH*3 +5,	rect.left + dW*1, rect.top+dH*4 );
	m_rectS[eS_PARAMETER_ACCDEC_TYPE					] = CRect(rect.left + dW*0 +5, rect.top + dH*4 +5,	rect.left + dW*1, rect.top+dH*5 );
	m_rectS[eS_PARAMETER_FILTER_TYPE					] = CRect(rect.left + dW*0 +5, rect.top + dH*5 +5,  rect.left + dW*1, rect.top+dH*6 );
	m_rectS[eS_PARAMETER_DATA_TYPE						] = CRect(rect.left + dW*0 +5, rect.top + dH*6 +5,  rect.left + dW*1, rect.top+dH*7 );
	m_rectS[eS_PARAMETER_POSITION_RATE					] = CRect(rect.left + dW*0 +5, rect.top + dH*7 +5,  rect.left + dW*1, rect.top+dH*8 );
	m_rectS[eS_PARAMETER_SPEED_RATE						] = CRect(rect.left + dW*0 +5, rect.top + dH*8 +5,  rect.left + dW*1, rect.top+dH*9 );
	m_rectS[eS_PARAMETER_MOVE_TIMEOUT					] = CRect(rect.left + dW*0 +5, rect.top + dH*9 +5,  rect.left + dW*1, rect.top+dH*10);
	m_rectS[eS_PARAMETER_INPOSITION						] = CRect(rect.left + dW*0 +5, rect.top + dH*10 +5, rect.left + dW*1, rect.top+dH*11);
	m_rectS[eS_PARAMETER_DEFAULT_SPPED					] = CRect(rect.left + dW*0 +5, rect.top + dH*11 +5, rect.left + dW*1, rect.top+dH*12);
	m_rectS[eS_PARAMETER_GANTRY_OFFSET_LIMIT_WARNING	] = CRect(rect.left + dW*0 +5, rect.top + dH*12 +5, rect.left + dW*1, rect.top+dH*13);
	m_rectS[eS_PARAMETER_GANTRY_OFFSET_LIMIT_ALERT		] = CRect(rect.left + dW*0 +5, rect.top + dH*13 +5, rect.left + dW*1, rect.top+dH*14);

	m_rectC[eC_PARAMETER_API_TIMEOUT					] = CRect(rect.left + dW*1 +5, rect.top + dH*0 +5,	rect.left + dW*2, rect.top+dH*1 );
	m_rectC[eC_PARAMETER_COORDINATE_SYSTEM				] = CRect(rect.left + dW*1 +5, rect.top + dH*1 +5,	rect.left + dW*2, rect.top+dH*2 );
	m_rectC[eC_PARAMETER_MOVE_TYPE						] = CRect(rect.left + dW*1 +5, rect.top + dH*2 +5,	rect.left + dW*2, rect.top+dH*3 );
	m_rectC[eC_PARAMETER_VELOCITY_TYPE					] = CRect(rect.left + dW*1 +5, rect.top + dH*3 +5,	rect.left + dW*2, rect.top+dH*4 );
	m_rectC[eC_PARAMETER_ACCDEC_TYPE					] = CRect(rect.left + dW*1 +5, rect.top + dH*4 +5,	rect.left + dW*2, rect.top+dH*5 );
	m_rectC[eC_PARAMETER_FILTER_TYPE					] = CRect(rect.left + dW*1 +5, rect.top + dH*5 +5,  rect.left + dW*2, rect.top+dH*6 );
	m_rectC[eC_PARAMETER_DATA_TYPE						] = CRect(rect.left + dW*1 +5, rect.top + dH*6 +5,  rect.left + dW*2, rect.top+dH*7 );
	m_rectC[eC_PARAMETER_POSITION_RATE					] = CRect(rect.left + dW*1 +5, rect.top + dH*7 +5,  rect.left + dW*2, rect.top+dH*8 );
	m_rectC[eC_PARAMETER_SPEED_RATE						] = CRect(rect.left + dW*1 +5, rect.top + dH*8 +5,  rect.left + dW*2, rect.top+dH*9 );
	m_rectC[eC_PARAMETER_MOVE_TIMEOUT					] = CRect(rect.left + dW*1 +5, rect.top + dH*9 +5,  rect.left + dW*2, rect.top+dH*10);
	m_rectC[eC_PARAMETER_INPOSITION						] = CRect(rect.left + dW*1 +5, rect.top + dH*10 +5, rect.left + dW*2, rect.top+dH*11);
	m_rectC[eC_PARAMETER_DEFAULT_SPPED					] = CRect(rect.left + dW*1 +5, rect.top + dH*11 +5, rect.left + dW*2, rect.top+dH*12);
	m_rectC[eC_PARAMETER_GANTRY_OFFSET_LIMIT_WARNING	] = CRect(rect.left + dW*1 +5, rect.top + dH*12 +5, rect.left + dW*2, rect.top+dH*13);
	m_rectC[eC_PARAMETER_GANTRY_OFFSET_LIMIT_ALERT		] = CRect(rect.left + dW*1 +5, rect.top + dH*13 +5, rect.left + dW*2, rect.top+dH*14);

	m_rectS[eS_PARAMETER_ACC							] = CRect(rect.left + dW*2 +5, rect.top + dH*0  +5, rect.left + dW*3, rect.top+dH*1 );
	m_rectS[eS_PARAMETER_DEC							] = CRect(rect.left + dW*2 +5, rect.top + dH*1  +5, rect.left + dW*3, rect.top+dH*2 );
	m_rectS[eS_PARAMETER_FILTER_TIME					] = CRect(rect.left + dW*2 +5, rect.top + dH*2  +5, rect.left + dW*3, rect.top+dH*3 );
	m_rectS[eS_PARAMETER_MAX_VELOCITY					] = CRect(rect.left + dW*2 +5, rect.top + dH*3  +5, rect.left + dW*3, rect.top+dH*4 );
	m_rectS[eS_PARAMETER_LOW_SPEED						] = CRect(rect.left + dW*2 +5, rect.top + dH*4  +5, rect.left + dW*3, rect.top+dH*5 );
	m_rectS[eS_PARAMETER_MEDIUM_SPEED					] = CRect(rect.left + dW*2 +5, rect.top + dH*5  +5, rect.left + dW*3, rect.top+dH*6 );	// KES 중속 추가 2024.2.15
	m_rectS[eS_PARAMETER_HIGH_SPEED						] = CRect(rect.left + dW*2 +5, rect.top + dH*6  +5, rect.left + dW*3, rect.top+dH*7 );
	m_rectS[eS_PARAMETER_PTP_SPEED						] = CRect(rect.left + dW*2 +5, rect.top + dH*7  +5, rect.left + dW*3, rect.top+dH*8 );
	m_rectS[eS_PARAMETER_LIMIT_LOW						] = CRect(rect.left + dW*2 +5, rect.top + dH*8  +5, rect.left + dW*3, rect.top+dH*9 );
	m_rectS[eS_PARAMETER_LIMIT_HIGH						] = CRect(rect.left + dW*2 +5, rect.top + dH*9  +5, rect.left + dW*3, rect.top+dH*10);

	m_rectC[eC_PARAMETER_ACC							] = CRect(rect.left + dW*3 +5, rect.top + dH*0  +5, rect.left + dW*4, rect.top+dH*1 );											 
	m_rectC[eC_PARAMETER_DEC							] = CRect(rect.left + dW*3 +5, rect.top + dH*1  +5, rect.left + dW*4, rect.top+dH*2 );
	m_rectC[eC_PARAMETER_FILTER_TIME					] = CRect(rect.left + dW*3 +5, rect.top + dH*2  +5, rect.left + dW*4, rect.top+dH*3 );
	m_rectC[eC_PARAMETER_MAX_VELOCITY					] = CRect(rect.left + dW*3 +5, rect.top + dH*3  +5, rect.left + dW*4, rect.top+dH*4 );
	m_rectC[eC_PARAMETER_LOW_SPEED						] = CRect(rect.left + dW*3 +5, rect.top + dH*4  +5, rect.left + dW*4, rect.top+dH*5 );
	m_rectC[eC_PARAMETER_MEDIUM_SPEED					] = CRect(rect.left + dW*3 +5, rect.top + dH*5  +5, rect.left + dW*4, rect.top+dH*6 );	// KES 중속 추가 2024.2.15
	m_rectC[eC_PARAMETER_HIGH_SPEED						] = CRect(rect.left + dW*3 +5, rect.top + dH*6  +5, rect.left + dW*4, rect.top+dH*7 );
	m_rectC[eC_PARAMETER_PTP_SPEED						] = CRect(rect.left + dW*3 +5, rect.top + dH*7  +5, rect.left + dW*4, rect.top+dH*8 );

	m_rectC[eC_PARAMETER_LIMIT_LOW						] = CRect(rect.left + dW*3 +5,		rect.top + dH*8  +5, rect.left + dW*3.7,	rect.top+dH*9 );
	m_rectC[eC_PARAMETER_LIMIT_LOW_GET					] = CRect(rect.left + dW*3.7 +5,	rect.top + dH*8  +5, rect.left + dW*4,		rect.top+dH*9 );

	m_rectC[eC_PARAMETER_LIMIT_HIGH						] = CRect(rect.left + dW*3 +5,		rect.top + dH*9  +5, rect.left + dW*3.7,	rect.top+dH*10);
	m_rectC[eC_PARAMETER_LIMIT_HIGH_GET					] = CRect(rect.left + dW*3.7 +5,	rect.top + dH*9  +5, rect.left + dW*4,		rect.top+dH*10);

	m_lstMotor.MoveWindow(m_rectS[eS_MOTOR_LIST]);

	//<< Set ComboBox Positions.
	for (int iCbo = 0; iCbo < TYPE_PARAMETER_COMBO_BOX_COUNT; ++iCbo) {
		rect = m_rectC[m_arrComboBoxComponents[iCbo].iControl];
		m_cboTypes[iCbo].MoveWindow(rect);
		//. UI Height set.
		::SendMessage(m_cboTypes[iCbo].m_hWnd, CB_SETITEMHEIGHT, (WPARAM)-1, rect.Height() - 6);
	}
	//>>
}
void CMotorParameterWnd::SyncData(BOOL bStore) {

	CProfile profile(_T("../cfg/MotorParameter.ini"));
	if (bStore) {
		profile.GetSections().DeleteAll();
	}

	BOOL bCompare = FALSE;
	for (int i = eMOTOR_0; i < eMOTOR_END; i++) {

		CString strMotorIndex = Format(_T("MOTOR_%02d"), i);
		CProfileSection& section = profile.GetSection(strMotorIndex);

		CProfileSection& subSection1 = section.GetSubSection(_T("Yaskawa Axis Init"));

		subSection1.SyncItemValue(bStore, _T("Rack No"),			m_MotorParameter[i].tYaskawa.iRackNo);
		subSection1.SyncItemValue(bStore, _T("Slot No"),			m_MotorParameter[i].tYaskawa.iSlotNo);
		subSection1.SyncItemValue(bStore, _T("Subslot No"),			m_MotorParameter[i].tYaskawa.iSubSlotNo);
		
		subSection1.SyncItemValue(bStore, _T("Motor No"),			m_MotorParameter[i].tYaskawa.iMotorNo);
		subSection1.SyncItemValue(bStore, _T("Logical Axis No"),	m_MotorParameter[i].tYaskawa.iLogicalAxisNo);
		subSection1.SyncItemValue(bStore, _T("Axis Type"),  		m_MotorParameter[i].tYaskawa.iAxisType);

		subSection1.SyncItemValue(bStore, _T("GantryRack No"),		m_MotorParameter[i].tYaskawa.iGantryRackNo);
		subSection1.SyncItemValue(bStore, _T("GantrySlot No"),		m_MotorParameter[i].tYaskawa.iGantrySlotNo);
		subSection1.SyncItemValue(bStore, _T("GantrySubSlot No"),	m_MotorParameter[i].tYaskawa.iGantrySubSlotNo);
		subSection1.SyncItemValue(bStore, _T("SetGantry"),  		m_MotorParameter[i].tYaskawa.bSetGantry);
		subSection1.SyncItemValue(bStore, _T("Ch1_Axis"),  			m_MotorParameter[i].tYaskawa.iCh1_Axis);
		subSection1.SyncItemValue(bStore, _T("Ch2_Axis"),  			m_MotorParameter[i].tYaskawa.iCh2_Axis);
		subSection1.SyncItemValue(bStore, _T("Ch1_Motor"),			m_MotorParameter[i].tYaskawa.iCh1_Motor);
		subSection1.SyncItemValue(bStore, _T("Ch2_Motor"),			m_MotorParameter[i].tYaskawa.iCh2_Motor);

		CString strAddressStop;
		if (bStore) {
			strAddressStop = m_MotorParameter[i].tYaskawa.szGantryAddressStop;
		}
		subSection1.SyncItemValue(bStore, _T("szGantryAddressStop"), strAddressStop);
		sprintf_s(m_MotorParameter[i].tYaskawa.szGantryAddressStop, sizeof(m_MotorParameter[i].tYaskawa.szGantryAddressStop), "%s", CStringA(strAddressStop).GetBuffer());

		CProfileSection& subSection2 = section.GetSubSection(_T("Yaskawa Home"));
		
		subSection2.SyncItemValue(bStore, _T("Home Type"),			m_MotorParameter[i].tHome.iHomeType);
		subSection2.SyncItemValue(bStore, _T("Gantry 2Ch Use"),		m_MotorParameter[i].tHome.bGantry2ChUse);
		subSection2.SyncItemValue(bStore, _T("Method"),				m_MotorParameter[i].tHome.iMethod);
		subSection2.SyncItemValue(bStore, _T("Torque"),				m_MotorParameter[i].tHome.dTorque );
		subSection2.SyncItemValue(bStore, _T("Direction"),			m_MotorParameter[i].tHome.iDirection);
		subSection2.SyncItemValue(bStore, _T("Velocity"),			m_MotorParameter[i].tHome.dVelocity);
		subSection2.SyncItemValue(bStore, _T("Approach Velocity"),	m_MotorParameter[i].tHome.dApproachVelocity);
		subSection2.SyncItemValue(bStore, _T("Creep Velocity"),		m_MotorParameter[i].tHome.dCreepVelocity);
		subSection2.SyncItemValue(bStore, _T("Offset"),				m_MotorParameter[i].tHome.dOffset,			_T("%.3f"));
		subSection2.SyncItemValue(bStore, _T("Offset2Ch"),			m_MotorParameter[i].tHome.dOffset2Ch,		_T("%.3f"));		
		subSection2.SyncItemValue(bStore, _T("Home Serch Timeout"),	m_MotorParameter[i].tHome.iHomeSerchTimeout);
		subSection2.SyncItemValue(bStore, _T("Init Posisition"),	m_MotorParameter[i].tHome.dInitPosisition, _T("%.3f"));

		CString strExtAddress;
		if (bStore) {
			strExtAddress = m_MotorParameter[i].tHome.szExternalHomeSensorAddress;
		}
		subSection2.SyncItemValue(bStore, _T("szExternalHomeSensorAddress"), strExtAddress);
		sprintf_s(m_MotorParameter[i].tHome.szExternalHomeSensorAddress, sizeof(m_MotorParameter[i].tHome.szExternalHomeSensorAddress), "%s", CStringA(strExtAddress).GetBuffer());
		SetOffsetFromAddress(i, strExtAddress);

		CProfileSection& subSection3 = section.GetSubSection(_T("Parameter"));
		subSection3.SyncItemValue(bStore, _T("API Timeout"),					m_MotorParameter[i].iAPITimeout);
		subSection3.SyncItemValue(bStore, _T("Coordinate System"),				m_MotorParameter[i].iCoordinateSystem);
		subSection3.SyncItemValue(bStore, _T("Move Type"),						m_MotorParameter[i].iMoveType);
		subSection3.SyncItemValue(bStore, _T("Velocity Type"),					m_MotorParameter[i].iVelocityType);
		subSection3.SyncItemValue(bStore, _T("AccDec Type"),					m_MotorParameter[i].iAccDecType);
		subSection3.SyncItemValue(bStore, _T("Filter Type"),					m_MotorParameter[i].iFilterType);
		subSection3.SyncItemValue(bStore, _T("Data Type"),						m_MotorParameter[i].iDataType);
		subSection3.SyncItemValue(bStore, _T("Position Rate"),					m_MotorParameter[i].dPositionRate,				_T("%.3f"));
		subSection3.SyncItemValue(bStore, _T("Speed Rate"),						m_MotorParameter[i].dSpeedRate,					_T("%.3f"));
		subSection3.SyncItemValue(bStore, _T("Move Timeout"),					m_MotorParameter[i].iMoveTimeout);
		subSection3.SyncItemValue(bStore, _T("InPosition"),						m_MotorParameter[i].dInPosition,				_T("%.3f"));
		subSection3.SyncItemValue(bStore, _T("Default Spped"),					m_MotorParameter[i].dDefaultSpped,				_T("%.3f"));
		subSection3.SyncItemValue(bStore, _T("Gantry Offset Limit Warning"),	m_MotorParameter[i].dGantryOffsetLimitWarning,	_T("%.3f"));
		subSection3.SyncItemValue(bStore, _T("Gantry Offset Limit Alert"),		m_MotorParameter[i].dGantryOffsetLimitAlert,	_T("%.3f"));
		
		subSection3.SyncItemValue(bStore, _T("Acc"),					m_MotorParameter[i].iAcc);
		subSection3.SyncItemValue(bStore, _T("Dec"),					m_MotorParameter[i].iDec);
		subSection3.SyncItemValue(bStore, _T("Filter Time"),			m_MotorParameter[i].dFilterTime);
		subSection3.SyncItemValue(bStore, _T("Max Velocity"),			m_MotorParameter[i].iMaxVelocity);
		subSection3.SyncItemValue(bStore, _T("Low Speed"),				m_MotorParameter[i].dLowSpeed,			_T("%.3f"));
		subSection3.SyncItemValue(bStore, _T("Medium Speed"),			m_MotorParameter[i].dMediumSpeed,		_T("%.3f"));	// KES 중속 추가 2024.2.15
		subSection3.SyncItemValue(bStore, _T("High Speed"),				m_MotorParameter[i].dHighSpeed,			_T("%.3f"));
		subSection3.SyncItemValue(bStore, _T("PTP Speed"),				m_MotorParameter[i].dPTPSpeed,			_T("%.3f"));
		subSection3.SyncItemValue(bStore, _T("Limit Low"),				m_MotorParameter[i].dLimitLow,			_T("%.3f"));
		subSection3.SyncItemValue(bStore, _T("Limit High"),				m_MotorParameter[i].dLimitHigh,			_T("%.3f"));

		if (bStore) {
			m_MotorParameter[i].tHome.dOffsetCrt = m_MotorParameter[i].tHome.dOffset - sm.MotorParameter[i].tHome.dOffset;
			m_MotorParameter[i].tHome.dOffsetCrt2 = m_MotorParameter[i].tHome.dOffset2Ch - sm.MotorParameter[i].tHome.dOffset2Ch;
		}
		else {
			m_MotorParameter[i].tHome.dOffsetCrt = 0;
			m_MotorParameter[i].tHome.dOffsetCrt2 = 0;
		}

		//TODO 우선 PLCLinker THETC thread에서 계속 검사해서 값을 넣어 주긴 할텐데, 
		//. 내 생각에는 SyncData에서 한번만 세팅하면 값이 변경될 일은 메모리 침범 외에는 없을 것 같다.
		//if (true == DBL_COMPARE(m_MotorParameter[i].dGantryOffsetLimitWarning, 0.0)) {
		//
		//	//. Alert값이 세팅 되어 있을 경우.
		//	//if (false == DBL_COMPARE(sm.MotorParameter[iAxis].dGantryOffsetLimitAlert, 0.0)) {
		//
		//	m_MotorParameter[i].dGantryOffsetLimitWarning = m_MotorParameter[i].dGantryOffsetLimitAlert / 2.;
		//	//}
		//}

		if (FALSE == bStore) {
			SetComboBoxFromData();
			SetDataFromComboBox();
		}

		//<< Lst 중 Gantry 항목에 Gantry 여부 추가.
		m_lstMotor.SetItemText(i, eLI_MOTOR_GANTRY, m_MotorParameter[i].tYaskawa.bSetGantry ? _T("GANTRY") : _T(""));
		//>>

		bCompare |= memcmp(&sm.MotorParameter[i], &m_MotorParameter[i], sizeof(sm.MotorParameter[i]));
		memcpy(&sm.MotorParameter[i], &m_MotorParameter[i], sizeof(sm.MotorParameter[i]));

	}
	if (bStore)
		profile.Save();

	if (bCompare)
		__POST_IP_COMMAND__("PLCLinker", NULL, "Stage", "SetMotorLimit");
}

bool CMotorParameterWnd::SetOffsetFromAddress(int iSelectedMotor, CString strAddress) {

	if (strAddress.IsEmpty()) {

		//. 비어있으면 초기화
		ZeroVar(m_MotorParameter[iSelectedMotor].tHome.szExternalHomeSensorAddress);
		
		return false;
	}

	CString strFind = strAddress;
	strFind.MakeLower();

	//. ib로 시작하는지 검사.
	int iStrOffset = strFind.Find(_T("ib"));

	if (iStrOffset < 0) {
		IPMessage(g_st[IDS_MOTOR_PARAM_WND_ERROR_INSERT_IB]);
		return false;
	}
	//. ib가 있다면 b 뒤의 문자열이 숫자 문자열이 있어야한다.
	iStrOffset = strFind.Find(_T("b")) + 1;

	CString strBitAddress = strFind.Mid(iStrOffset);
	int iBitAddress = 0;
	try {

		//. 16진수 문자열을 int 값으로. 2번째인자는 strBitAddress에서 문자열이 아닌 부분을 넘겨주므로 상관없음.
		iBitAddress = std::stoi(CStringA(strBitAddress).GetBuffer(), nullptr, 16);
	}
	catch (std::exception& exp) {
		IPMessage(Format(_T("%s : %s"), g_st[IDS_MOTOR_PARAM_WND_ERROR_ADRESS_VALUE_INCORRECT], CStringW(exp.what())));
		return false;
	}

	sprintf_s(m_MotorParameter[iSelectedMotor].tHome.szExternalHomeSensorAddress, sizeof(m_MotorParameter[iSelectedMotor].tHome.szExternalHomeSensorAddress), "%s", CStringA(strAddress).GetBuffer());

	return true;
}


BOOL CMotorParameterWnd::ChangeParameter()
{
	CString strChange;
	for (int i = eMOTOR_0; i < eMOTOR_END; i++) {

	}
	return TRUE;
}

void CMotorParameterWnd::OnItemchangedLst(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if ((pNMLV->uNewState & LVIS_SELECTED) && !(pNMLV->uOldState & LVIS_SELECTED)) { // Select 

		if (!sm.Option->tKeyPad.fShowKeyPad) {

			m_lstMotor.SetItemBKColor(m_eSelectMotor, RGB_WHITE);

			m_eSelectMotor = pNMLV->iItem;

			m_lstMotor.SetItemBKColor(pNMLV->iItem, RGB_GRAY_LIGHT);
			
			//<< ComboBox Reset.
			SetComboBoxFromData();
			//>>

			Invalidate(FALSE);
		}
	}
}

 // LJY 추가 2024.06.23
void CMotorParameterWnd::ChangeLanguage()
{
	for (int i = eMOTOR_0; i < eMOTOR_END; i++) {
		m_lstMotor.SetItemText(i, eLI_MOTOR_NAME, g_stMotor[i]);

	}
	Invalidate(FALSE);
}

LRESULT CMotorParameterWnd::OnEditFinished(WPARAM wParam, LPARAM lParam)
{
	if (wParam) {
		
		CString	strNewItem(GetEditString());
		switch (m_eSelectedEdit) {
			case eC_YASKAWA_INIT_PARAMETER_GANTRY_ADDRESS_STOP: {

				//. 값이 비어있으면 사용도 하면 안됨.
				if (strNewItem.IsEmpty()) {
					ZeroVar(m_MotorParameter[m_eSelectMotor].tYaskawa.szGantryAddressStop);
					break;
				}

				if (strNewItem.Find(_T("MB")) >= 0 || strNewItem.Find(_T("mb")) >= 0) {
					sprintf_s(m_MotorParameter[m_eSelectMotor].tYaskawa.szGantryAddressStop, sizeof(m_MotorParameter[m_eSelectMotor].tYaskawa.szGantryAddressStop), "%s", CStringA(strNewItem).GetBuffer());
				}
				else {
					IPMessage(g_st[IDS_MOTOR_PARAM_WND_SZGANTRYADDRESSSTOP_SETTING_FAIL]);
				}
				break;
			}
			case eC_YASKAWA_HOME_PARAMETER_EXTERNAL_HOME_SENSOR: {

				SetOffsetFromAddress(m_eSelectMotor, strNewItem);

				break;
			}
		}

		m_eSelectedEdit = eC_NONE;
		Invalidate(FALSE);
	}
	
	return 0;
}


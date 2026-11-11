#include "pch.h"
#include "../../../UI.h"
#include "./MCMonitorWnd.h"



IMPLEMENT_DYNCREATE		(CMCMonitorWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CMCMonitorWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
	ON_NOTIFY(LVN_ITEMCHANGED, ID_MC_MONITOR, OnItemchangedLst)
END_MESSAGE_MAP			()



CMCMonitorWnd::CMCMonitorWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
	m_eSelectAxis		= eMOTOR_0;
}
CMCMonitorWnd::~CMCMonitorWnd() {
}



BOOL CMCMonitorWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}






int CMCMonitorWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {

		m_listAxis.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, m_rectS[eS_LIST_AREA], this, ID_MC_MONITOR);
		m_listAxis.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
		m_listAxis.ModifyStyle(0, LVS_SHOWSELALWAYS);
		m_listAxis.SetRowHeight(45);
		m_listAxis.ShowScrollBar(SB_VERT);
		m_listAxis.ShowScrollBar(SB_HORZ);
		m_listAxis.InsertColumn(0, _T("Axis"), LVCFMT_LEFT, 170, 1);
		m_listAxis.ShowWindow(SW_SHOW);
		for (int i = 0; i < eMOTOR_END; i++) {
			m_listAxis.InsertItem(i, g_stMotor[i]);
		}
	}
	catch (...) {
		return -1;
	}

	return 0;
}


void CMCMonitorWnd::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case ID_MC_MONITOR:{
			InvalidateRect(m_rectS[eS_STATUS_1_0C		], FALSE);
			InvalidateRect(m_rectS[eS_STATUS_1_09		], FALSE);
			InvalidateRect(m_rectS[eS_STATUS_2			], FALSE);
			InvalidateRect(m_rectS[eS_SERVO_DRIVER_2C	], FALSE);
			InvalidateRect(m_rectS[eS_SERVO_DRIVER_2E	], FALSE);
			InvalidateRect(m_rectS[eS_SERVO_DRIVER_ETC	], FALSE);
			InvalidateRect(m_rectS[eS_SERVO_ALARM_H		], FALSE);
			InvalidateRect(m_rectS[eS_SERVO_ALARM_L		], FALSE);
			InvalidateRect(m_rectS[eS_STATUS_1_00		], FALSE);
			InvalidateRect(m_rectS[eS_STATUS_1_0C		], FALSE);
			InvalidateRect(m_rectS[eS_STATUS_1_09		], FALSE);
			InvalidateRect(m_rectC[eC_GANTRY_CH_1		], FALSE);
			InvalidateRect(m_rectC[eC_GANTRY_CH_2		], FALSE);
			
		}
		default:
			break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CMCMonitorWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		Invalidate();
		SetTimer(ID_MC_MONITOR, 100, nullptr);
	}
	else {
		KillTimer(ID_MC_MONITOR);
	}
}

// LJY 추가 2024.06.23
void CMCMonitorWnd::ChangeLanguage()
{
	for (int i = 0; i < eMOTOR_END; i++) {
		m_listAxis.DeleteItem(i);
		m_listAxis.InsertItem(i, g_stMotor[i]);
	}
	Invalidate(FALSE);

}

BOOL CMCMonitorWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CMCMonitorWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CRect				rect = m_rectS[eS_FULL_AREA];
	memDC.FillSolidRect(rect, g_colorBackground[eEC_DISABLE]);
	rect.DeflateRect(3, 3, 0, 0);
	CDrawSymbol::DrawLineRect(&memDC, rect, g_colorForeground[eEC_DEFAULT]);
	
	DrawGroupBox(&memDC, m_rectS[eS_STATUS_1_00],		_T("00"), DT_PEN, RGB_BK);
	DrawGroupBox(&memDC, m_rectS[eS_STATUS_1_0C],		_T("0C"), DT_PEN, RGB_BK);
	DrawGroupBox(&memDC, m_rectS[eS_STATUS_1_09],		_T("09"), DT_PEN, RGB_BK);
	DrawGroupBox(&memDC, m_rectS[eS_STATUS_2],			_T("Status2"), DT_PEN, RGB_BK);
	DrawGroupBox(&memDC, m_rectS[eS_SERVO_DRIVER_2C],	_T("2C"), DT_PEN, RGB_BK);
	DrawGroupBox(&memDC, m_rectS[eS_SERVO_DRIVER_2E],	_T("2E"), DT_PEN, RGB_BK);
	DrawGroupBox(&memDC, m_rectS[eS_SERVO_DRIVER_ETC],	_T("ETC"), DT_PEN, RGB_BK);
	DrawGroupBox(&memDC, m_rectS[eS_SERVO_ALARM_H],		_T("ALARM_H"), DT_PEN, RGB_BK);
	DrawGroupBox(&memDC, m_rectS[eS_SERVO_ALARM_L],		_T("ALARM_L"), DT_PEN, RGB_BK);

	DrawText(&memDC, m_rectS[eS_STATUS_2_IWxx01], _T("IWxx01"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_STATUS_2_IWxx08], _T("IWxx08"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_STATUS_2_ILxx0E], _T("ILxx0E"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_STATUS_2_ILxx10], _T("ILxx10"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_STATUS_2_ILxx12], _T("ILxx12"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_STATUS_2_ILxx16], _T("ILxx16"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_STATUS_2_ILxx18], _T("ILxx18"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_STATUS_2_ILxx1A], _T("ILxx1A"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_STATUS_2_ILxx1C], _T("ILxx1C"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_STATUS_2_ILxx1E], _T("ILxx1E"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_STATUS_2_IWxx20], _T("IWxx20"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_DRIVER_IWxx2D], _T("IWxx2D"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_DRIVER_IWxx3F], _T("IWxx3F"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_DRIVER_ILxx40], _T("ILxx40"), 10, RGB_BK);
	DrawText(&memDC, m_rectS[eS_DRIVER_ILxx42], _T("ILxx42"), 10, RGB_BK);

	T_MONITORING_PARAM tMonitoring = sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring;
	if (sm.MotorParameter[m_eSelectAxis].tYaskawa.bSetGantry == TRUE && m_bGantry == FALSE)
		tMonitoring = sm.MotorStatus[m_eSelectAxis].tCrt.tSubGantryData;
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx00_0], _T("OperationReady"),	tMonitoring.IWxx00 & BIT(0));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx00_1], _T("SeervoOn"),			tMonitoring.IWxx00 & BIT(1));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx00_2], _T("SystemBusy"),		tMonitoring.IWxx00 & BIT(2));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx00_3], _T("ServoReady"),		tMonitoring.IWxx00 & BIT(3));

	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx09_0], _T("Execution"),				tMonitoring.IWxx09 & BIT(0));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx09_1], _T("Hold"),					tMonitoring.IWxx09 & BIT(1));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx09_3], _T("Error Completed"),		tMonitoring.IWxx09 & BIT(3));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx09_8], _T("Abs Encoder Reset"),		tMonitoring.IWxx09 & BIT(7));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx09_8], _T("CMD execution"),			tMonitoring.IWxx09 & BIT(8));
					
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx0C_0], _T("Dischargin Ok"),			tMonitoring.IWxx0C & BIT(0));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx0C_1], _T("Positioning Ok"),		tMonitoring.IWxx0C & BIT(1));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx0C_2], _T("Latch Ok"),				tMonitoring.IWxx0C & BIT(2));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx0C_3], _T("NEAR Position"),			tMonitoring.IWxx0C & BIT(3));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx0C_4], _T("Zero Point Position"),	tMonitoring.IWxx0C & BIT(4));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx0C_5], _T("Zero Point Set"),		tMonitoring.IWxx0C & BIT(5));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx0C_6], _T("During Machine Lock"),	tMonitoring.IWxx0C & BIT(6));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx0C_8], _T("ABS Rotary Pos"),		tMonitoring.IWxx0C & BIT(8));
	DrawRoundSensor(&memDC, m_rectC[eC_STATUS_1_IWxx0C_9], _T("POSMAX turn preset"),	tMonitoring.IWxx0C & BIT(9));

	SelectedDrawEditText(&memDC, m_rectC[eC_STATUS_2_IWxx01], Format(_T("%04d"), tMonitoring.IWxx01	));
	SelectedDrawEditText(&memDC, m_rectC[eC_STATUS_2_IWxx08], Format(_T("%04d"), tMonitoring.IWxx08	));
	SelectedDrawEditText(&memDC, m_rectC[eC_STATUS_2_ILxx0E], Format(_T("%04d"), tMonitoring.ILxx0E	));
	SelectedDrawEditText(&memDC, m_rectC[eC_STATUS_2_ILxx10], Format(_T("%04d"), tMonitoring.ILxx10	));
	SelectedDrawEditText(&memDC, m_rectC[eC_STATUS_2_ILxx12], Format(_T("%04d"), tMonitoring.ILxx12	));
	SelectedDrawEditText(&memDC, m_rectC[eC_STATUS_2_ILxx16], Format(_T("%04d"), tMonitoring.ILxx16	));
	SelectedDrawEditText(&memDC, m_rectC[eC_STATUS_2_ILxx18], Format(_T("%04d"), tMonitoring.ILxx18	));
	SelectedDrawEditText(&memDC, m_rectC[eC_STATUS_2_ILxx1A], Format(_T("%04d"), tMonitoring.ILxx1A	));
	SelectedDrawEditText(&memDC, m_rectC[eC_STATUS_2_ILxx1C], Format(_T("%04d"), tMonitoring.ILxx1C_));
	SelectedDrawEditText(&memDC, m_rectC[eC_STATUS_2_ILxx1E], Format(_T("%04d"), tMonitoring.ILxx1E	));
	SelectedDrawEditText(&memDC, m_rectC[eC_STATUS_2_ILxx20], Format(_T("%04d"), tMonitoring.ILxx20	));

	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_0], _T("Alarm"),				tMonitoring.IWxx2C & BIT(0));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_1], _T("Warning"),			tMonitoring.IWxx2C & BIT(1));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_2], _T("CMDRDY"),			tMonitoring.IWxx2C & BIT(2));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_3], _T("Servo On"),			tMonitoring.IWxx2C & BIT(3));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_4], _T("Main Power Supply"), tMonitoring.IWxx2C & BIT(4));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_5], _T("Machine Lock"),		tMonitoring.IWxx2C & BIT(5));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_6], _T("Zero Point Pos"),	tMonitoring.IWxx2C & BIT(6));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_7], _T("Positioning Ok"),	tMonitoring.IWxx2C & BIT(7));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_8], _T("Zero Speed"),		tMonitoring.IWxx2C & BIT(8));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_9], _T("Torque Limit"),		tMonitoring.IWxx2C & BIT(9));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_A], _T("Latch OK"),			tMonitoring.IWxx2C & BIT(10));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_B], _T("Speed Limit"),		tMonitoring.IWxx2C & BIT(11));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_C], _T("P_SOT"),				tMonitoring.IWxx2C & BIT(12));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_D], _T("N_SOT"),				tMonitoring.IWxx2C & BIT(13));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_P_OT], _T("P_OT"),			tMonitoring.ILxx28 & BIT(2));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_N_OT], _T("N_OT"),			tMonitoring.ILxx28 & BIT(3));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2C_HOME], _T("HOME"),			tMonitoring.ILxx28 & BIT(1));
	
	
	
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_0], _T("P_LIMIT_SI"),		tMonitoring.IWxx2E_ & BIT(0));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_1], _T("N_LIMIT_SI"),		tMonitoring.IWxx2E_ & BIT(1));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_2], _T("Deceleration Dog"),	tMonitoring.IWxx2E_ & BIT(2));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_3], _T("Encoder Phase_A"),	tMonitoring.IWxx2E_ & BIT(3));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_4], _T("Encoder Phase_B"),	tMonitoring.IWxx2E_ & BIT(4));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_5], _T("Encoder Phase_c"),	tMonitoring.IWxx2E_ & BIT(5));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_6], _T("EXT1"),				tMonitoring.IWxx2E_ & BIT(6));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_7], _T("EXT2"),				tMonitoring.IWxx2E_ & BIT(7));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_8], _T("EXT3"),				tMonitoring.IWxx2E_ & BIT(8));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_9], _T("Break State"),		tMonitoring.IWxx2E_ & BIT(9));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_A], _T("STOP Signal"),		tMonitoring.IWxx2E_ & BIT(10));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_B], _T("2E_B"),				tMonitoring.IWxx2E_ & BIT(11));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_C], _T("2E_C"),				tMonitoring.IWxx2E_ & BIT(12));
	DrawRoundSensor(&memDC, m_rectC[eC_DRIVER_xx2E_D], _T("2E_D"),				tMonitoring.IWxx2E_ & BIT(13));
												   
	SelectedDrawEditText(&memDC, m_rectC[eC_DRIVER_IWxx2D], Format(_T("%04d"),	tMonitoring.IWxx2D));
	SelectedDrawEditText(&memDC, m_rectC[eC_DRIVER_IWxx3F], Format(_T("%04d"),	tMonitoring.IWxx3F));
	SelectedDrawEditText(&memDC, m_rectC[eC_DRIVER_ILxx40], Format(_T("%04d"),	tMonitoring.ILxx40));
	SelectedDrawEditText(&memDC, m_rectC[eC_DRIVER_ILxx42], Format(_T("%04d"),	tMonitoring.ILxx42));
	
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_0],	_T("Servo Driver"),						tMonitoring.ILxx04 & BIT(0));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_1],	_T("Pos"),								tMonitoring.ILxx04 & BIT(1));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_2],	_T("Neg"),								tMonitoring.ILxx04 & BIT(2));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_3],	_T("Pos Sfot"),							tMonitoring.ILxx04 & BIT(3));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_4],	_T("Neg Sfot"),							tMonitoring.ILxx04 & BIT(4));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_5],	_T("Servo Off"),						tMonitoring.ILxx04 & BIT(5));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_6],	_T("Pos Time Over"),					tMonitoring.ILxx04 & BIT(6));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_7],	_T("Excessive Pos Moving"),				tMonitoring.ILxx04 & BIT(7));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_8],	_T("Excessive Speed"),					tMonitoring.ILxx04 & BIT(8));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_9],	_T("Excessive deviation"),				tMonitoring.ILxx04 & BIT(9));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_A],	_T("Filter Type Change"),				tMonitoring.ILxx04 & BIT(10));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_B],	_T("Filter Time Constant Change"),		tMonitoring.ILxx04 & BIT(11));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_D],	_T("Zero Point unsetting"),				tMonitoring.ILxx04 & BIT(13));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_F],	_T("Servo Driver user Constant Set"),	tMonitoring.ILxx04 & BIT(15));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_10],	_T("Servo Driver Synch.Comm"),			tMonitoring.ILxx04 & BIT(16));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_11],	_T("Communication"),					tMonitoring.ILxx04 & BIT(17));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_12],	_T("Command Timeout"),					tMonitoring.ILxx04 & BIT(18));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_13],	_T("ABS encoder Rotations"),			tMonitoring.ILxx04 & BIT(19));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_1D],	_T("Detected Servo Driver Type"),		tMonitoring.ILxx04 & BIT(28));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_1E],	_T("Motor Type Set"),					tMonitoring.ILxx04 & BIT(29)); 
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_1F],	_T("Connected encoder"),				tMonitoring.ILxx04 & BIT(30));
	
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_0], _T("Excessive"),					tMonitoring.ILxx02 & BIT(0));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_1], _T("Setting Param"),				tMonitoring.ILxx02 & BIT(1));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_2], _T("Fixed Param"),				tMonitoring.ILxx02 & BIT(2));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_3], _T("Servo Driver"),				tMonitoring.ILxx02 & BIT(3));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_4], _T("Motion Cmd"),					tMonitoring.ILxx02 & BIT(4));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_6], _T("Positive Direction"),			tMonitoring.ILxx02 & BIT(6));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_7], _T("Negative Direction"),			tMonitoring.ILxx02 & BIT(7));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_8], _T("Servo On"),					tMonitoring.ILxx02 & BIT(8));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_9], _T("Servo Driver communication"),	tMonitoring.ILxx02 & BIT(9));
	DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_A], _T("Servo Driver Stop"),			tMonitoring.ILxx02 & BIT(10));

	if (sm.MotorParameter[m_eSelectAxis].tYaskawa.bSetGantry) 
	{
		DrawMDIMenuButtonN(&memDC, m_rectC[eC_GANTRY_CH_1], _T("Ch1"), m_eSelectControl == eC_GANTRY_CH_1 || m_bGantry);
		DrawMDIMenuButtonN(&memDC, m_rectC[eC_GANTRY_CH_2], _T("Ch2"), m_eSelectControl == eC_GANTRY_CH_2 || !m_bGantry);
	}
}



void CMCMonitorWnd::OnMouseMove(UINT nFlags, CPoint point) {
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



void CMCMonitorWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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



void CMCMonitorWnd::OnLButtonUp(UINT nFlags, CPoint point)  {
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_OK: {				
				break;
			}
			case eC_CANCEL: {
				break;
			}
			case eC_GANTRY_CH_1: {
				if (sm.MotorParameter[m_eSelectAxis].tYaskawa.bSetGantry)
					m_bGantry = TRUE;
				break;
			}
			case eC_GANTRY_CH_2: {
				if (sm.MotorParameter[m_eSelectAxis].tYaskawa.bSetGantry)
					m_bGantry = FALSE;
				break;
			}
			default: break;
		}
	}
	m_eSelectControl	=	eC_NONE;
	Invalidate(FALSE);
	
	//if (!strALogMsg.IsEmpty() && !strALogPos.IsEmpty())
	//	WRITE_LOG(eLOG_BUTTON, TapString(strALogMsg, strALogPos, theApp.m_UserManager.GetCurrentUserData().szID));

	CFrameWndEx::OnLButtonUp(nFlags, point);
}



void CMCMonitorWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
	
	CRect	rc = m_rectS[eS_FULL_AREA];
	double dW = (rc.Width()-5) / 10;
	double dH = (rc.Height()-15) /1;
	m_rectS[eS_LIST_AREA]		= CRect(rc.left+5+dW*0, rc.top+15+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectS[eS_STATUS_1]		= CRect(rc.left+5+dW*1, rc.top+15+dH*0, rc.left+dW*3, rc.top+dH*1);
	m_rectS[eS_STATUS_2]		= CRect(rc.left+5+dW*3, rc.top+30+dH*0, rc.left+dW*5, rc.top+dH*0.7);
	m_rectS[eS_SERVO_DRIVER]	= CRect(rc.left+5+dW*5, rc.top+15+dH*0, rc.left+dW*7, rc.top+dH*1);
	m_rectS[eS_SERVO_ALARM]		= CRect(rc.left+5+dW*7, rc.top+15+dH*0, rc.left+dW*9, rc.top+dH*1);
	
	rc = m_rectS[eS_STATUS_1];
	dW = (rc.Width()-5) / 1;
	dH = (rc.Height()-15) /5;
	m_rectS[eS_STATUS_1_00]		= CRect(rc.left+5+dW*0, rc.top+15+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectS[eS_STATUS_1_0C]		= CRect(rc.left+5+dW*0, rc.top+15+dH*1, rc.left+dW*1, rc.top+dH*2);
	m_rectS[eS_STATUS_1_09]		= CRect(rc.left+5+dW*0, rc.top+15+dH*2, rc.left+dW*1, rc.top+dH*4);

	m_rectC[eC_GANTRY_CH_1]		= CRect(rc.left+5+dW*0,		rc.top+15+dH*4.7, rc.left+dW*0.5,	rc.top+dH*5);
	m_rectC[eC_GANTRY_CH_2]		= CRect(rc.left+5+dW*0.5,	rc.top+15+dH*4.7, rc.left+dW*1,		rc.top+dH*5);
	
	rc = m_rectS[eS_STATUS_1_00];
	rc.InflateRect(0, -15, 0, 0);
	dW = (rc.Width()-5) / 1;
	dH = (rc.Height()-5) /4;

	m_rectC[eC_STATUS_1_IWxx00_0]	= CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectC[eC_STATUS_1_IWxx00_1]	= CRect(rc.left+5+dW*0, rc.top+5+dH*1, rc.left+dW*1, rc.top+dH*2);
	m_rectC[eC_STATUS_1_IWxx00_2]	= CRect(rc.left+5+dW*0, rc.top+5+dH*2, rc.left+dW*1, rc.top+dH*3);
	m_rectC[eC_STATUS_1_IWxx00_3]	= CRect(rc.left+5+dW*0, rc.top+5+dH*3, rc.left+dW*1, rc.top+dH*4);

	rc = m_rectS[eS_STATUS_1_0C];
	rc.InflateRect(0, -15, 0, 0);
	dW = (rc.Width()-5) / 1;
	dH = (rc.Height()-5) /4;

	m_rectC[eC_STATUS_1_IWxx09_0]	= CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectC[eC_STATUS_1_IWxx09_1]	= CRect(rc.left+5+dW*0, rc.top+5+dH*1, rc.left+dW*1, rc.top+dH*2);
	m_rectC[eC_STATUS_1_IWxx09_3]	= CRect(rc.left+5+dW*0, rc.top+5+dH*2, rc.left+dW*1, rc.top+dH*3);
	m_rectC[eC_STATUS_1_IWxx09_8]	= CRect(rc.left+5+dW*0, rc.top+5+dH*3, rc.left+dW*1, rc.top+dH*4);

	rc = m_rectS[eS_STATUS_1_09];
	rc.InflateRect(0, -15, 0, 0);
	dW = (rc.Width()-5) / 1;
	dH = (rc.Height()-5) /9;

	m_rectC[eC_STATUS_1_IWxx0C_0]	= CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectC[eC_STATUS_1_IWxx0C_1]	= CRect(rc.left+5+dW*0, rc.top+5+dH*1, rc.left+dW*1, rc.top+dH*2);
	m_rectC[eC_STATUS_1_IWxx0C_2]	= CRect(rc.left+5+dW*0, rc.top+5+dH*2, rc.left+dW*1, rc.top+dH*3);
	m_rectC[eC_STATUS_1_IWxx0C_3]	= CRect(rc.left+5+dW*0, rc.top+5+dH*3, rc.left+dW*1, rc.top+dH*4);
	m_rectC[eC_STATUS_1_IWxx0C_4]	= CRect(rc.left+5+dW*0, rc.top+5+dH*4, rc.left+dW*1, rc.top+dH*5);
	m_rectC[eC_STATUS_1_IWxx0C_5]	= CRect(rc.left+5+dW*0, rc.top+5+dH*5, rc.left+dW*1, rc.top+dH*6);
	m_rectC[eC_STATUS_1_IWxx0C_6]	= CRect(rc.left+5+dW*0, rc.top+5+dH*6, rc.left+dW*1, rc.top+dH*7);
	m_rectC[eC_STATUS_1_IWxx0C_8]	= CRect(rc.left+5+dW*0, rc.top+5+dH*7, rc.left+dW*1, rc.top+dH*8);
	m_rectC[eC_STATUS_1_IWxx0C_9]	= CRect(rc.left+5+dW*0, rc.top+5+dH*8, rc.left+dW*1, rc.top+dH*9);

	rc = m_rectS[eS_STATUS_2];
	rc.InflateRect(0, -15, 0, 0);
	dW = (rc.Width()-5) / 2;
	dH = (rc.Height()-5) /13;
	m_rectS[eS_STATUS_2_IWxx01]		= CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectS[eS_STATUS_2_IWxx08]		= CRect(rc.left+5+dW*0, rc.top+5+dH*1, rc.left+dW*1, rc.top+dH*2);
	m_rectS[eS_STATUS_2_ILxx0E]		= CRect(rc.left+5+dW*0, rc.top+5+dH*2, rc.left+dW*1, rc.top+dH*3);
	m_rectS[eS_STATUS_2_ILxx10]		= CRect(rc.left+5+dW*0, rc.top+5+dH*3, rc.left+dW*1, rc.top+dH*4);
	m_rectS[eS_STATUS_2_ILxx12]		= CRect(rc.left+5+dW*0, rc.top+5+dH*4, rc.left+dW*1, rc.top+dH*5);
	m_rectS[eS_STATUS_2_ILxx16]		= CRect(rc.left+5+dW*0, rc.top+5+dH*5, rc.left+dW*1, rc.top+dH*6);
	m_rectS[eS_STATUS_2_ILxx18]		= CRect(rc.left+5+dW*0, rc.top+5+dH*6, rc.left+dW*1, rc.top+dH*7);
	m_rectS[eS_STATUS_2_ILxx1A]		= CRect(rc.left+5+dW*0, rc.top+5+dH*7, rc.left+dW*1, rc.top+dH*8);
	m_rectS[eS_STATUS_2_ILxx1C]		= CRect(rc.left+5+dW*0, rc.top+5+dH*8, rc.left+dW*1, rc.top+dH*9);
	m_rectS[eS_STATUS_2_ILxx1E]		= CRect(rc.left+5+dW*0, rc.top+5+dH*9, rc.left+dW*1, rc.top+dH*10);
	m_rectS[eS_STATUS_2_IWxx20]		= CRect(rc.left+5+dW*0, rc.top+5+dH*10, rc.left+dW*1, rc.top+dH*11);
	m_rectC[eC_STATUS_2_IWxx01]		= CRect(rc.left+5+dW*1, rc.top+5+dH*0, rc.left+dW*2, rc.top+dH*1);
	m_rectC[eC_STATUS_2_IWxx08]		= CRect(rc.left+5+dW*1, rc.top+5+dH*1, rc.left+dW*2, rc.top+dH*2);
	m_rectC[eC_STATUS_2_ILxx0E]		= CRect(rc.left+5+dW*1, rc.top+5+dH*2, rc.left+dW*2, rc.top+dH*3);
	m_rectC[eC_STATUS_2_ILxx10]		= CRect(rc.left+5+dW*1, rc.top+5+dH*3, rc.left+dW*2, rc.top+dH*4);
	m_rectC[eC_STATUS_2_ILxx12]		= CRect(rc.left+5+dW*1, rc.top+5+dH*4, rc.left+dW*2, rc.top+dH*5);
	m_rectC[eC_STATUS_2_ILxx16]		= CRect(rc.left+5+dW*1, rc.top+5+dH*5, rc.left+dW*2, rc.top+dH*6);
	m_rectC[eC_STATUS_2_ILxx18]		= CRect(rc.left+5+dW*1, rc.top+5+dH*6, rc.left+dW*2, rc.top+dH*7);
	m_rectC[eC_STATUS_2_ILxx1A]		= CRect(rc.left+5+dW*1, rc.top+5+dH*7, rc.left+dW*2, rc.top+dH*8);
	m_rectC[eC_STATUS_2_ILxx1C]		= CRect(rc.left+5+dW*1, rc.top+5+dH*8, rc.left+dW*2, rc.top+dH*9);
	m_rectC[eC_STATUS_2_ILxx1E]		= CRect(rc.left+5+dW*1, rc.top+5+dH*9, rc.left+dW*2, rc.top+dH*10);
	m_rectC[eC_STATUS_2_ILxx20]		= CRect(rc.left+5+dW*1, rc.top+5+dH*10, rc.left+dW*2, rc.top+dH*11);

	rc = m_rectS[eS_SERVO_DRIVER];
	dW = (rc.Width()-5) / 1;
	dH = (rc.Height()-15) /9;

	m_rectS[eS_SERVO_DRIVER_2C]		= CRect(rc.left+5+dW*0, rc.top+15+dH*0, rc.left+dW*1, rc.top+dH*4);
	m_rectS[eS_SERVO_DRIVER_2E]		= CRect(rc.left+5+dW*0, rc.top+15+dH*4, rc.left+dW*1, rc.top+dH*7);
	m_rectS[eS_SERVO_DRIVER_ETC]	= CRect(rc.left+5+dW*0, rc.top+15+dH*7, rc.left+dW*1, rc.top+dH*9);

	rc = m_rectS[eS_SERVO_DRIVER_2C];
	rc.InflateRect(0, -15, 0, 0);

	dW = (rc.Width()-5) / 2;
	dH = (rc.Height()-5) /9;

	m_rectC[eC_DRIVER_xx2C_0]		= CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectC[eC_DRIVER_xx2C_1]		= CRect(rc.left+5+dW*0, rc.top+5+dH*1, rc.left+dW*1, rc.top+dH*2);
	m_rectC[eC_DRIVER_xx2C_2]		= CRect(rc.left+5+dW*0, rc.top+5+dH*2, rc.left+dW*1, rc.top+dH*3);
	m_rectC[eC_DRIVER_xx2C_3]		= CRect(rc.left+5+dW*0, rc.top+5+dH*3, rc.left+dW*1, rc.top+dH*4);
	m_rectC[eC_DRIVER_xx2C_4]		= CRect(rc.left+5+dW*0, rc.top+5+dH*4, rc.left+dW*1, rc.top+dH*5);
	m_rectC[eC_DRIVER_xx2C_5]		= CRect(rc.left+5+dW*0, rc.top+5+dH*5, rc.left+dW*1, rc.top+dH*6);
	m_rectC[eC_DRIVER_xx2C_6]		= CRect(rc.left+5+dW*0, rc.top+5+dH*6, rc.left+dW*1, rc.top+dH*7);
	m_rectC[eC_DRIVER_xx2C_7]		= CRect(rc.left+5+dW*0, rc.top+5+dH*7, rc.left+dW*1, rc.top+dH*8);
	m_rectC[eC_DRIVER_xx2C_8]		= CRect(rc.left+5+dW*1, rc.top+5+dH*0, rc.left+dW*2, rc.top+dH*1);
	m_rectC[eC_DRIVER_xx2C_9]		= CRect(rc.left+5+dW*1, rc.top+5+dH*1, rc.left+dW*2, rc.top+dH*2);
	m_rectC[eC_DRIVER_xx2C_A]		= CRect(rc.left+5+dW*1, rc.top+5+dH*2, rc.left+dW*2, rc.top+dH*3);
	m_rectC[eC_DRIVER_xx2C_B]		= CRect(rc.left+5+dW*1, rc.top+5+dH*3, rc.left+dW*2, rc.top+dH*4);
	m_rectC[eC_DRIVER_xx2C_C]		= CRect(rc.left+5+dW*1, rc.top+5+dH*4, rc.left+dW*2, rc.top+dH*5);
	m_rectC[eC_DRIVER_xx2C_D]		= CRect(rc.left+5+dW*1, rc.top+5+dH*5, rc.left+dW*2, rc.top+dH*6);
	m_rectC[eC_DRIVER_xx2C_P_OT]	= CRect(rc.left+5+dW*1, rc.top+5+dH*6, rc.left+dW*2, rc.top+dH*7);
	m_rectC[eC_DRIVER_xx2C_N_OT]	= CRect(rc.left+5+dW*1, rc.top+5+dH*7, rc.left+dW*2, rc.top+dH*8);
	m_rectC[eC_DRIVER_xx2C_HOME]	= CRect(rc.left+5+dW*1, rc.top+5+dH*8, rc.left+dW*2, rc.top+dH*9);

	rc = m_rectS[eS_SERVO_DRIVER_2E];
	rc.InflateRect(0, -15, 0, 0);

	dW = (rc.Width()-5) / 2;
	dH = (rc.Height()-5) /8;
	m_rectC[eC_DRIVER_xx2E_0]		= CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectC[eC_DRIVER_xx2E_1]		= CRect(rc.left+5+dW*0, rc.top+5+dH*1, rc.left+dW*1, rc.top+dH*2);
	m_rectC[eC_DRIVER_xx2E_2]		= CRect(rc.left+5+dW*0, rc.top+5+dH*2, rc.left+dW*1, rc.top+dH*3);
	m_rectC[eC_DRIVER_xx2E_3]		= CRect(rc.left+5+dW*0, rc.top+5+dH*3, rc.left+dW*1, rc.top+dH*4);
	m_rectC[eC_DRIVER_xx2E_4]		= CRect(rc.left+5+dW*0, rc.top+5+dH*4, rc.left+dW*1, rc.top+dH*5);
	m_rectC[eC_DRIVER_xx2E_5]		= CRect(rc.left+5+dW*0, rc.top+5+dH*5, rc.left+dW*1, rc.top+dH*6);
	m_rectC[eC_DRIVER_xx2E_6]		= CRect(rc.left+5+dW*0, rc.top+5+dH*6, rc.left+dW*1, rc.top+dH*7);
	m_rectC[eC_DRIVER_xx2E_7]		= CRect(rc.left+5+dW*0, rc.top+5+dH*7, rc.left+dW*1, rc.top+dH*8);
	m_rectC[eC_DRIVER_xx2E_8]		= CRect(rc.left+5+dW*1, rc.top+5+dH*0, rc.left+dW*2, rc.top+dH*1);
	m_rectC[eC_DRIVER_xx2E_9]		= CRect(rc.left+5+dW*1, rc.top+5+dH*1, rc.left+dW*2, rc.top+dH*2);
	m_rectC[eC_DRIVER_xx2E_A]		= CRect(rc.left+5+dW*1, rc.top+5+dH*2, rc.left+dW*2, rc.top+dH*3);
	m_rectC[eC_DRIVER_xx2E_B]		= CRect(rc.left+5+dW*1, rc.top+5+dH*3, rc.left+dW*2, rc.top+dH*4);
	m_rectC[eC_DRIVER_xx2E_C]		= CRect(rc.left+5+dW*1, rc.top+5+dH*4, rc.left+dW*2, rc.top+dH*5);
	m_rectC[eC_DRIVER_xx2E_D]		= CRect(rc.left+5+dW*1, rc.top+5+dH*5, rc.left+dW*2, rc.top+dH*6);

	
	rc = m_rectS[eS_SERVO_DRIVER_ETC];
	rc.InflateRect(0, -15, 0, 0);

	dW = (rc.Width()-5) / 2;
	dH = (rc.Height()-5) /4;
	m_rectS[eS_DRIVER_IWxx2D]		= CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectS[eS_DRIVER_IWxx3F]		= CRect(rc.left+5+dW*0, rc.top+5+dH*1, rc.left+dW*1, rc.top+dH*2);
	m_rectS[eS_DRIVER_ILxx40]		= CRect(rc.left+5+dW*0, rc.top+5+dH*2, rc.left+dW*1, rc.top+dH*3);
	m_rectS[eS_DRIVER_ILxx42]		= CRect(rc.left+5+dW*0, rc.top+5+dH*3, rc.left+dW*1, rc.top+dH*4);
	m_rectC[eC_DRIVER_IWxx2D]		= CRect(rc.left+5+dW*1, rc.top+5+dH*0, rc.left+dW*2, rc.top+dH*1);
	m_rectC[eC_DRIVER_IWxx3F]		= CRect(rc.left+5+dW*1, rc.top+5+dH*1, rc.left+dW*2, rc.top+dH*2);
	m_rectC[eC_DRIVER_ILxx40]		= CRect(rc.left+5+dW*1, rc.top+5+dH*2, rc.left+dW*2, rc.top+dH*3);
	m_rectC[eC_DRIVER_ILxx42]		= CRect(rc.left+5+dW*1, rc.top+5+dH*3, rc.left+dW*2, rc.top+dH*4);

	rc = m_rectS[eS_SERVO_ALARM];
	dW = (rc.Width()-15) / 1;
	dH = (rc.Height()-5) /2;

	m_rectS[eS_SERVO_ALARM_H]		= CRect(rc.left+5+dW*0, rc.top+15+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectS[eS_SERVO_ALARM_L]		= CRect(rc.left+5+dW*0, rc.top+15+dH*1, rc.left+dW*1, rc.top+dH*2);

	rc = m_rectS[eS_SERVO_ALARM_H];
	rc.InflateRect(0, -15, 0, 0);

	dW = (rc.Width()-5) / 2;
	dH = (rc.Height()-5) /11;
	m_rectC[eC_ALARM_xx04_0]		= CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectC[eC_ALARM_xx04_1]		= CRect(rc.left+5+dW*0, rc.top+5+dH*1, rc.left+dW*1, rc.top+dH*2);
	m_rectC[eC_ALARM_xx04_2]		= CRect(rc.left+5+dW*0, rc.top+5+dH*2, rc.left+dW*1, rc.top+dH*3);
	m_rectC[eC_ALARM_xx04_3]		= CRect(rc.left+5+dW*0, rc.top+5+dH*3, rc.left+dW*1, rc.top+dH*4);
	m_rectC[eC_ALARM_xx04_4]		= CRect(rc.left+5+dW*0, rc.top+5+dH*4, rc.left+dW*1, rc.top+dH*5);
	m_rectC[eC_ALARM_xx04_5]		= CRect(rc.left+5+dW*0, rc.top+5+dH*5, rc.left+dW*1, rc.top+dH*6);
	m_rectC[eC_ALARM_xx04_6]		= CRect(rc.left+5+dW*0, rc.top+5+dH*6, rc.left+dW*1, rc.top+dH*7);
	m_rectC[eC_ALARM_xx04_7]		= CRect(rc.left+5+dW*0, rc.top+5+dH*7, rc.left+dW*1, rc.top+dH*8);
	m_rectC[eC_ALARM_xx04_8]		= CRect(rc.left+5+dW*0, rc.top+5+dH*8, rc.left+dW*1, rc.top+dH*9);
	m_rectC[eC_ALARM_xx04_9]		= CRect(rc.left+5+dW*0, rc.top+5+dH*9, rc.left+dW*1, rc.top+dH*10);
	m_rectC[eC_ALARM_xx04_A]		= CRect(rc.left+5+dW*1, rc.top+5+dH*0, rc.left+dW*2, rc.top+dH*1);
	m_rectC[eC_ALARM_xx04_B]		= CRect(rc.left+5+dW*1, rc.top+5+dH*1, rc.left+dW*2, rc.top+dH*2);
	m_rectC[eC_ALARM_xx04_D]		= CRect(rc.left+5+dW*1, rc.top+5+dH*2, rc.left+dW*2, rc.top+dH*3);
	m_rectC[eC_ALARM_xx04_F	]		= CRect(rc.left+5+dW*1, rc.top+5+dH*3, rc.left+dW*2, rc.top+dH*4);
	m_rectC[eC_ALARM_xx04_10]		= CRect(rc.left+5+dW*1, rc.top+5+dH*4, rc.left+dW*2, rc.top+dH*5);
	m_rectC[eC_ALARM_xx04_11]		= CRect(rc.left+5+dW*1, rc.top+5+dH*5, rc.left+dW*2, rc.top+dH*6);
	m_rectC[eC_ALARM_xx04_12]		= CRect(rc.left+5+dW*1, rc.top+5+dH*6, rc.left+dW*2, rc.top+dH*7);
	m_rectC[eC_ALARM_xx04_13]		= CRect(rc.left+5+dW*1, rc.top+5+dH*7, rc.left+dW*2, rc.top+dH*8);
	m_rectC[eC_ALARM_xx04_1D]		= CRect(rc.left+5+dW*1, rc.top+5+dH*8, rc.left+dW*2, rc.top+dH*9);
	m_rectC[eC_ALARM_xx04_1E]		= CRect(rc.left+5+dW*1, rc.top+5+dH*9, rc.left+dW*2, rc.top+dH*10);
	m_rectC[eC_ALARM_xx04_1F]		= CRect(rc.left+5+dW*1, rc.top+5+dH*10, rc.left+dW*2, rc.top+dH*11);
	
	rc = m_rectS[eS_SERVO_ALARM_L];
	rc.InflateRect(0, -15, 0, 0);

	dW = (rc.Width()-5) / 1;
	dH = (rc.Height()-5) /10;
	m_rectC[eC_ALARM_xx02_0]		= CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectC[eC_ALARM_xx02_1]		= CRect(rc.left+5+dW*0, rc.top+5+dH*1, rc.left+dW*1, rc.top+dH*2);
	m_rectC[eC_ALARM_xx02_2]		= CRect(rc.left+5+dW*0, rc.top+5+dH*2, rc.left+dW*1, rc.top+dH*3);
	m_rectC[eC_ALARM_xx02_3]		= CRect(rc.left+5+dW*0, rc.top+5+dH*3, rc.left+dW*1, rc.top+dH*4);
	m_rectC[eC_ALARM_xx02_4]		= CRect(rc.left+5+dW*0, rc.top+5+dH*4, rc.left+dW*1, rc.top+dH*5);
	m_rectC[eC_ALARM_xx02_6]		= CRect(rc.left+5+dW*0, rc.top+5+dH*5, rc.left+dW*1, rc.top+dH*6);
	m_rectC[eC_ALARM_xx02_7]		= CRect(rc.left+5+dW*0, rc.top+5+dH*6, rc.left+dW*1, rc.top+dH*7);
	m_rectC[eC_ALARM_xx02_8]		= CRect(rc.left+5+dW*0, rc.top+5+dH*7, rc.left+dW*1, rc.top+dH*8);
	m_rectC[eC_ALARM_xx02_9]		= CRect(rc.left+5+dW*0, rc.top+5+dH*8, rc.left+dW*1, rc.top+dH*9);
	m_rectC[eC_ALARM_xx02_A]		= CRect(rc.left+5+dW*0, rc.top+5+dH*9, rc.left+dW*1, rc.top+dH*10);
	if (m_listAxis) {
		m_listAxis.MoveWindow(m_rectS[eS_LIST_AREA]);
	}
}

void CMCMonitorWnd::OnItemchangedLst(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if ((pNMLV->uNewState & LVIS_SELECTED) && !(pNMLV->uOldState & LVIS_SELECTED)) { // Select
		m_eSelectAxis = pNMLV->iItem;
	}
}
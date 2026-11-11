// CAxisAlarmWnd.cpp: 구현 파일
//

#include "pch.h"
#include "PLCLinker.h"
#include "AxisAlarmWnd.h"


// CAxisAlarmWnd

IMPLEMENT_DYNAMIC(CAxisAlarmWnd, CFrameWnd)

CAxisAlarmWnd::CAxisAlarmWnd()
{
	m_eSelectControl = eC_NONE;
}

CAxisAlarmWnd::~CAxisAlarmWnd()
{
}


BEGIN_MESSAGE_MAP(CAxisAlarmWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CAxisAlarmWnd 메시지 처리기

void CAxisAlarmWnd::ComputArea(){
	GetClientRect(m_rectClient);
	m_rectS[eS_FULL_AREA] = m_rectClient;
	CRect rc = m_rectS[eS_FULL_AREA];

	double dW = (rc.Width() - 5) / 5;
	double dH = (rc.Height() - 5) / 20;
	m_rectS[eS_MOTOR_NAME	] = CRect(rc.left + 5 + dW * 0 , rc.top + 5 + dH * 0, rc.left + dW * 2, rc.top + dH * 1);
	m_rectC[eC_CLOSE		] = CRect(rc.left + 5 + dW * 4 , rc.top + 5 + dH * 19, rc.left + dW * 5, rc.top + dH * 20);
	m_rectS[eS_SERVO_ALARM	] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 1, rc.left + dW * 5, rc.top + dH * 19);
	rc = m_rectS[eS_SERVO_ALARM];
	dW = (rc.Width() - 5) / 1;
	dH = (rc.Height() - 5) / 2;

	m_rectS[eS_SERVO_ALARM_H] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 1, rc.top + dH * 1);
	m_rectS[eS_SERVO_ALARM_L] = CRect(rc.left + 5 + dW * 0, rc.top + 15 + dH * 1, rc.left + dW * 1, rc.top + dH * 2);

	rc = m_rectS[eS_SERVO_ALARM_H];
	rc.InflateRect(0, -15, 0, 0);

	dW = (rc.Width() - 5) / 2;
	dH = (rc.Height() - 5) / 11;
	m_rectC[eC_ALARM_xx04_0] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 1, rc.top + dH * 1);
	m_rectC[eC_ALARM_xx04_1] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 1, rc.left + dW * 1, rc.top + dH * 2);
	m_rectC[eC_ALARM_xx04_2] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 2, rc.left + dW * 1, rc.top + dH * 3);
	m_rectC[eC_ALARM_xx04_3] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 3, rc.left + dW * 1, rc.top + dH * 4);
	m_rectC[eC_ALARM_xx04_4] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 4, rc.left + dW * 1, rc.top + dH * 5);
	m_rectC[eC_ALARM_xx04_5] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 5, rc.left + dW * 1, rc.top + dH * 6);
	m_rectC[eC_ALARM_xx04_6] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 6, rc.left + dW * 1, rc.top + dH * 7);
	m_rectC[eC_ALARM_xx04_7] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 7, rc.left + dW * 1, rc.top + dH * 8);
	m_rectC[eC_ALARM_xx04_8] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 8, rc.left + dW * 1, rc.top + dH * 9);
	m_rectC[eC_ALARM_xx04_9] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 9, rc.left + dW * 1, rc.top + dH * 10);
	m_rectC[eC_ALARM_xx04_A] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 0, rc.left + dW * 2, rc.top + dH * 1);
	m_rectC[eC_ALARM_xx04_B] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 1, rc.left + dW * 2, rc.top + dH * 2);
	m_rectC[eC_ALARM_xx04_D] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 2, rc.left + dW * 2, rc.top + dH * 3);
	m_rectC[eC_ALARM_xx04_F] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 3, rc.left + dW * 2, rc.top + dH * 4);
	m_rectC[eC_ALARM_xx04_10] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 4, rc.left + dW * 2, rc.top + dH * 5);
	m_rectC[eC_ALARM_xx04_11] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 5, rc.left + dW * 2, rc.top + dH * 6);
	m_rectC[eC_ALARM_xx04_12] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 6, rc.left + dW * 2, rc.top + dH * 7);
	m_rectC[eC_ALARM_xx04_13] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 7, rc.left + dW * 2, rc.top + dH * 8);
	m_rectC[eC_ALARM_xx04_1D] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 8, rc.left + dW * 2, rc.top + dH * 9);
	m_rectC[eC_ALARM_xx04_1E] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 9, rc.left + dW * 2, rc.top + dH * 10);
	m_rectC[eC_ALARM_xx04_1F] = CRect(rc.left + 5 + dW * 1, rc.top + 5 + dH * 10, rc.left + dW * 2, rc.top + dH * 11);

	rc = m_rectS[eS_SERVO_ALARM_L];
	rc.InflateRect(0, -15, 0, 0);

	dW = (rc.Width() - 5) / 1;
	dH = (rc.Height() - 5) / 10;
	m_rectC[eC_ALARM_xx02_0] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 0, rc.left + dW * 1, rc.top + dH * 1);
	m_rectC[eC_ALARM_xx02_1] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 1, rc.left + dW * 1, rc.top + dH * 2);
	m_rectC[eC_ALARM_xx02_2] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 2, rc.left + dW * 1, rc.top + dH * 3);
	m_rectC[eC_ALARM_xx02_3] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 3, rc.left + dW * 1, rc.top + dH * 4);
	m_rectC[eC_ALARM_xx02_4] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 4, rc.left + dW * 1, rc.top + dH * 5);
	m_rectC[eC_ALARM_xx02_6] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 5, rc.left + dW * 1, rc.top + dH * 6);
	m_rectC[eC_ALARM_xx02_7] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 6, rc.left + dW * 1, rc.top + dH * 7);
	m_rectC[eC_ALARM_xx02_8] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 7, rc.left + dW * 1, rc.top + dH * 8);
	m_rectC[eC_ALARM_xx02_9] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 8, rc.left + dW * 1, rc.top + dH * 9);
	m_rectC[eC_ALARM_xx02_A] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 9, rc.left + dW * 1, rc.top + dH * 10);
	m_rectC[eC_ALARM_xx02_A] = CRect(rc.left + 5 + dW * 0, rc.top + 5 + dH * 9, rc.left + dW * 1, rc.top + dH * 10);
	
}
int CAxisAlarmWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}


void CAxisAlarmWnd::OnPaint()
{
	CPaintDC dc(this); 
		
	try {
		CMemDCEx::CMemDC		memDC(&dc);
		CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_NORMAL]);
		CBrush* pOldBrush = (CBrush*)memDC.SelectObject(&g_brushBackground[eEC_NORMAL]);
		CPen* pOldPen = (CPen*)memDC.SelectObject(&pen);
		CRect					rect = m_rectS[eS_FULL_AREA];
		memDC.SelectObject(&g_brushBackground[eEC_NORMAL]);
		memDC.FillSolidRect(rect, g_colorBackground[eEC_DISABLE]);
		rect.DeflateRect(3, 3, 0, 0);
		memDC.SelectObject(&g_brushBackground[eEC_DEFAULT]);
		CDrawSymbol::DrawLineRect(&memDC, rect, g_colorForeground[eEC_DEFAULT]);
		DrawTextB(memDC, m_rectS[eS_MOTOR_NAME], g_stMotor[m_eSelectAxis], 9, FW_HEAVY, RGB_BLACK, DT_LEFT|DT_VCENTER);

		DrawGroupBox(&memDC, m_rectS[eS_SERVO_ALARM_H], _T("ALARM_H"), DT_PEN, RGB_BK);
		DrawGroupBox(&memDC, m_rectS[eS_SERVO_ALARM_L], _T("ALARM_L"), DT_PEN, RGB_BK);


		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_0],	_T("Servo Driver"),						sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(0));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_1],	_T("Pos"),								sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(1));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_2],	_T("Neg"),								sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(2));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_3],	_T("Pos Sfot"),							sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(3));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_4],	_T("Neg Sfot"),							sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(4));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_5],	_T("Servo Off"),						sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(5));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_6],	_T("Pos Time Over"),					sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(6));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_7],	_T("Excessive Pos Moving"),				sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(7));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_8],	_T("Excessive Speed"),					sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(8));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_9],	_T("Excessive deviation"),				sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(9));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_A],	_T("Filter Type "),				sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(10));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_B],	_T("Filter Time Constant "),		sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(11));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_D],	_T("Zero Pos"),				sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(13));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_F],	_T("Driver user Constant Set"),	sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(15));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_10],	_T("Driver Synch.Comm"),			sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(16));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_11],	_T("Communication"),					sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(17));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_12],	_T("CMD Timeout"),					sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(18));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_13],	_T("ABS encoder Rotate"),			sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(19));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_1D],	_T("Detected Driver Type"),		sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(28));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_1E],	_T("Motor Type Set"),					sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(29)); 
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx04_1F],	_T("Connected encoder"),				sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx04 & BIT(30));
		
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_0], _T("Excessive"),					sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx02 & BIT(0));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_1], _T("Setting Param"),				sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx02 & BIT(1));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_2], _T("Fixed Param"),				sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx02 & BIT(2));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_3], _T("Servo Driver"),				sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx02 & BIT(3));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_4], _T("Motion Cmd"),					sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx02 & BIT(4));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_6], _T("Positive Direction"),			sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx02 & BIT(6));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_7], _T("Negative Direction"),			sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx02 & BIT(7));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_8], _T("Servo On"),					sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx02 & BIT(8));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_9], _T("Servo Driver communication"),	sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx02 & BIT(9));
		DrawRoundSensor(&memDC, m_rectC[eC_ALARM_xx02_A], _T("Servo Driver Stop"),			sm.MotorStatus[m_eSelectAxis].tCrt.tMonitoring.ILxx02 & BIT(10));

		DrawButtonUpDown(&memDC, m_rectC[eC_CLOSE	], _T("CLOSE"), m_eSelectControl == eC_CLOSE	);
	
		memDC.SelectObject(pOldBrush);
		memDC.SelectObject(pOldPen);	
	}
	catch (...) {

	}
}


void CAxisAlarmWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	if( bShow){
		SetWindowText(_T("Axis Alarm Register"));
		int iW = 1920 / 5;
		int iH = 800 ;
		SetWindowPos(&wndTopMost, 0, 0, iW, iH, SWP_SHOWWINDOW);
		ComputArea();
		SetTimer(INTERVAL_DATA, 100, nullptr);
	}else{
		KillTimer(INTERVAL_DATA);
	}
}


void CAxisAlarmWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	for(int i = 0; i < eC_END; i++){
		if( m_rectC[i].PtInRect(point)){
			m_eSelectControl = i;
			Invalidate(FALSE);
			break;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CAxisAlarmWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( m_rectC[m_eSelectControl].PtInRect(point)){
		switch(m_eSelectControl){
			case eC_CLOSE:{
				ShowWindow(SW_HIDE);
				break;
			}
		}
	}

	m_eSelectControl = eC_NONE;
	Invalidate(FALSE);
			
	CWnd::OnLButtonUp(nFlags, point);
}


void CAxisAlarmWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	
	CWnd::OnMouseMove(nFlags, point);
}


void CAxisAlarmWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent){
		case INTERVAL_DATA:{
			
			break;
		}
	}
	CWnd::OnTimer(nIDEvent);
}


void CAxisAlarmWnd::MotorSetting(int iSelectMotor) {
	m_eSelectAxis = iSelectMotor;
	Invalidate();
}

void CAxisAlarmWnd::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ShowWindow(SW_HIDE);
}

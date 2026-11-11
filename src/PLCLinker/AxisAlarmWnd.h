#pragma once


// CAxisAlarmWnd
#include "misc/ListCtrlColumnEx.h"
#define ID_LIST_DATA	10000
#define INTERVAL_DATA	9000

class CAxisAlarmWnd : public CFrameWnd
{
	enum eS {
		eS_NONE,
		eS_FULL_AREA,
		eS_SERVO_ALARM,
		eS_SERVO_ALARM_H,
		eS_SERVO_ALARM_L,
		eS_MOTOR_NAME,
		eS_END,
	};
	enum eC {
		eC_NONE,
		eC_ALARM_xx04_0,
		eC_ALARM_xx04_1,
		eC_ALARM_xx04_2,
		eC_ALARM_xx04_3,
		eC_ALARM_xx04_4,
		eC_ALARM_xx04_5,
		eC_ALARM_xx04_6,
		eC_ALARM_xx04_7,
		eC_ALARM_xx04_8,
		eC_ALARM_xx04_9,
		eC_ALARM_xx04_A,
		eC_ALARM_xx04_B,
		eC_ALARM_xx04_D,
		eC_ALARM_xx04_F,
		eC_ALARM_xx04_10,
		eC_ALARM_xx04_11,
		eC_ALARM_xx04_12,
		eC_ALARM_xx04_13,
		eC_ALARM_xx04_1D,
		eC_ALARM_xx04_1E,
		eC_ALARM_xx04_1F,

		eC_ALARM_xx02_0,
		eC_ALARM_xx02_1,
		eC_ALARM_xx02_2,
		eC_ALARM_xx02_3,
		eC_ALARM_xx02_4,
		eC_ALARM_xx02_6,
		eC_ALARM_xx02_7,
		eC_ALARM_xx02_8,
		eC_ALARM_xx02_9,
		eC_ALARM_xx02_A,
		eC_CLOSE,
		eC_END,
	};
	DECLARE_DYNAMIC(CAxisAlarmWnd)

public:
	CAxisAlarmWnd();
	virtual ~CAxisAlarmWnd();
	void MotorSetting(int iSelectMotor = eMOTOR_0);
protected:
	int m_eSelectControl, m_eSelectList;
	CRect m_rectS[eS_END],m_rectC[eC_END], m_rectClient;
	int m_eSelectAxis = eMOTOR_0;
protected:
	void ComputArea();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
};



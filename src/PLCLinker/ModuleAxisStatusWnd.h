#pragma once


// CModuleAxisStatusWnd 프레임
#include "AxisStatusView.h"
#define INTERVAL_MODULE_AXIS_STATUS 9000
class CModuleAxisStatusWnd : public TIPCT<CFrameWnd>
{
	enum{
		eS_NONE = -1,
		eS_AREA_AXIS_STATUS,
		eS_AREA_SELECT_MOTOR_CONTROL,
			eS_SELECT_MOTOR_NAME,
		eS_END,
	};
	enum{
		eC_NONE = -1,

		eC_STEP_OR_JOG,
		eC_SENSOR_MOVE_SPEED,
		eC_EDIT_STEP_DISTANCE,
		eC_BTN_MINUS_MOVE,
		eC_BTN_PLUS_MOVE,
		
		eC_CLOSE,
		eC_END,
	};
	DECLARE_DYNCREATE(CModuleAxisStatusWnd)
public:
	CModuleAxisStatusWnd();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CModuleAxisStatusWnd();

protected:
	void ComputArea();

protected:
	CAxisStatusView m_AxisView;
	CRect	m_rectClient, m_rectS[eS_END], m_rectC[eC_END];
	int		m_eSelectControl = eC_NONE;
	double	m_dStepDistance = 0.;

	eMOTOR_SPEED m_eSpeed = eMOTOR_SPEED_LOW;	// KES 중속 추가 2024.2.15
	bool	m_bStepMove = true;

	BOOL	m_bBeforeAutoRunState = FALSE;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow (BOOL bShow, UINT nStatus);
	afx_msg void OnPaint ();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnTimer (UINT_PTR nIDEvent);
	afx_msg void OnClose();

public:
	DECLARE_IP()
	ip_handler BOOL	OnIP_Show(CIPCommand& cmd);
};



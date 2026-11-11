#pragma once

#include "misc/ListCtrlColumnEx.h"
// CAxisControlWnd 프레임

class CAxisControlWnd : public TIPCT<CFrameWnd>
{
	enum eS {
		eS_NONE, 
		eS_LIST_AREA,
		eS_RIGHT_SIDE_AREA,
		eS_BOTTOM_SIDE_AREA,
		eS_TARGET,
		eS_SPEED,
		eS_SLIDE,
		eS_OVERRIDE,
		eS_LOOP,
		eS_GANTRY,
		eS_GANTRY_CH_STATE,
		eS_END,
	};
	enum eC {
		eC_NONE = -1,
		//<< 기능 삭제.
		//eC_PTP,
		//>>
		eC_MOVE_TYPE_STEP,
		eC_MOVE_TYPE_JOG,
		eC_MOVE_TYPE_TORQUE,
		eC_JOG_MOVE_N,
		eC_JOG_MOVE_P,
		eC_EDIT_TARGET,
		eC_EDIT_SPEED,
		eC_MOVE,
		eC_MOVE_STOP,
		//<< 임시 기능 해제 12.08.
		eC_LINER_MOVE,
		eC_MULTI_MOVE,
		//eC_GEAR_MOVE,
		//>>
		eC_CLEAR,
		eC_SLIDE,
		eC_CLOSE,
		eC_INIT,
		eC_INIT_STOP,
		eC_ALL_ALARM_CLEAR,
		eC_ALARM_CLEAR,
		eC_MAXIMUM_WINDOW,
		eC_SERVO_ON,
		eC_SERVO_OFF,
		eC_ALL_SERVO_ON,
		eC_ALL_SERVO_OFF,
		eC_GANTRY_ALL,
		eC_GANTRY_CH1,
		eC_GANTRY_CH2,
		eC_GANTRY_OFFSET_NORMALIZE,	// 갠트리 Ch1,2 편차를 없애는 동작 (Ch2가 Ch1에 맞춰짐)
		eC_PRESET,
		eC_CUR_PRESET,
		eC_END,
	};
	enum eLI {
		eLI_AXIS_NO,
		eLI_SERVO_ON,
		eLI_HOME,
		eLI_ALARM,
		eLI_NAME,
		eLI_POSITION,
		eLI_SPEED,
		eLI_TORQUE,
		eLI_P_LIMIT,
		eLI_N_LIMIT,
		eLI_INPOSTION,
		eLI_PRE_SET_POS,
		eLI_CHECK,
		eLI_GANTRY_CH2,
		eLI_TORQUE_CH2,
		eLI_END,
	};

	//DECLARE_DYNCREATE(CAxisControlWnd)
	DECLARE_DYNAMIC(CAxisControlWnd)
protected:
	
public : 
	void				ComputeArea();
	CAxisControlWnd();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CAxisControlWnd();
public :
	BOOL				GetCheckAxis(CIPVar& AxisData);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnItemchangedLst(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	DECLARE_IP()
	ip_handler BOOL	OnIPShow(CIPCommand& cmd);

	CRect				m_rectC[eC_END], m_rectS[eS_END], m_rectClient;
	int					m_eSelectControl;
	int					m_eSelectedMotor;
	int					m_iSelectMove;
	CListCtrlColumnEx	m_lstAxis;
	double				m_dTarget[eMOTOR_END];
	double				m_dSpeed[eMOTOR_END];
	T_MOTOR_STATUS_CRT	m_MotorStatus[eMOTOR_END];
	T_MOTOR_STATUS_ETC  m_MotorStatusETC[eMOTOR_END];
	BOOL				m_bIsGantry[eMOTOR_END];

	T_GANTRY_DATA		m_tGantryData[eMOTOR_END];
	CSliderCtrl			m_SliderControl;
	int					m_iSlider;
	BOOL				m_bMaximumPage;					// TRUE 인 경우 이동 가능 / FALSE 인 경우 이동 불가능 
	int					m_iSelectGantry;				// 0 : Gantry All / 1 : CH 1 / 2 : CH 2 
	int					m_iHideCount;					// 5번 클릭 시 M/L-MOVE 버튼 띄움
	BOOL				m_bHide;						// TRUE : M/L-Move 사용 불가능/ FALSE : M/L-Move 사용 가능 
	int					m_iStopButtonClick;				// Move Stop Button 눌러지는 경우 카운트 증가 / 3번 이상 눌리는 경우 0으로 초기화 및 ys.Stop? 호출
};



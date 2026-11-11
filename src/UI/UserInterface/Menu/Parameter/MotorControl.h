#pragma once

#define __MOTOR_STATUS__ 5
#define INTERVAL_UI	9001
class CMotorControl : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,

	eS_TITLE					,
	eS_MOTOR_NAME				,

	eS_ITEM_AREA				,
	eS_SET_AREA					,
	eS_MOVE_AREA				,

	eS_SERVO					,
	eS_ORG						,
	eS_HOME						,
	eS_ALARM					,
	eS_LIMIT_POS				,
	eS_LIMIT_NED				,
	eS_INPOS					,

	eS_POSITION					,
	eS_POSITION_VAL				,
	eS_POSITION_VAL_2			,
	eS_POSITION_UNIT			,

	eS_SET_POSITION				,
	eS_SET_POSITION_UNIT		,

	eS_VELOCITY					,
	eS_VELOCITY_VAL				,
	eS_VELOCITY_UNIT			,

	eS_SET_VELOCITY				,
	eS_SET_VELOCITY_UNIT		,

	eS_TORQUE					,
	eS_TORQUE_VAL				,
	eS_TORQUE_VAL_2				,
	eS_TORQUE_UNIT				,

	eS_SET_TORQUE				,
	eS_SET_TORQUE_UNIT			,
	eS_END						,
};

enum eCOMMAND {
	eC_NONE						= -1,
	eC_EDIT_POSITION			,
	eC_EDIT_VELOCITY			,
	
	eC_ESTOP					,
	eC_ABS_MOVE					,

	eC_REL_NEG					,
	eC_REL_POS					,

	eC_JOG_NEG					,
	eC_JOG_POS					,

	eC_HOME						,
	eC_END						,
};

DECLARE_DYNCREATE(CMotorControl)
public:
	CMotorControl							(CWnd*	pwndParent = nullptr, int iMotorNo = eMOTOR_0);
	virtual				~CMotorControl		();
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct			);
	afx_msg void		OnShowWindow			(BOOL bShow, UINT nStatus				);
	afx_msg void		OnTimer					(UINT_PTR nIDEvent						);
	afx_msg BOOL		OnEraseBkgnd			(CDC* pDC								);
	afx_msg void		OnPaint					();
	afx_msg void		OnMouseMove				(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonDown			(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonUp				(UINT nFlags, CPoint point				);
protected:
	CWnd*				m_pwndParent;
public:
protected:
	CRect				m_rectS			[eS_END], 
						m_rectC			[eC_END];
	int					m_eMouseOver			;
	int					m_eSelectControl		; 
	double				m_dPosition;
	double				m_dSpeed;
	T_POS				m_tPos;
	CString				m_strMotorName;
	BOOL				m_bMotorStatus[__MOTOR_STATUS__] = { FALSE, };	//home, alarm, +limit, -limit, inpos	

public:	

protected:
	void				fn_ComputeArea			();

public:
	void				SetMotorInfo			(T_POS Pos);
	BOOL				Initialize				() { return TRUE; };
	void				UpdataMotorState();
};

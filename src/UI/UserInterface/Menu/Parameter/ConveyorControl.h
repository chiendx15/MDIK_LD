#pragma once

#define __Conveyor_STATUS__ 18
class CConveyorControl : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,

	eS_TITLE					,
	eS_CONVEYOR_NAME			,

	eS_ITEM_AREA				,
	eS_PROGRESS_AREA			,
	eS_CURRENT_SPEED			,
	eS_END						,
};

enum eCOMMAND {
	eC_NONE						= -1,
	eC_CURRENT_SPEED			,

	eC_CONVEYOR_BTN_CONNECT		,
	eC_CONVEYOR_BTN_READY		,
	eC_CONVEYOR_BTN_SPEED_TYPE	,
	eC_CONVEYOR_BTN_MINUS		,
	eC_CONVEYOR_BTN_STOP		,
	eC_CONVEYOR_BTN_PLUS		,

	eC_CONVEYOR_JOG,
	eC_END						,
};

DECLARE_DYNCREATE(CConveyorControl)
public:
	CConveyorControl							(int iConveyorNo = eCONVEYOR_LD_1);
	virtual				~CConveyorControl		();
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
	BOOL				m_bSpeedHigh;	// KES 중속 추가 2024.2.15
	int					m_iConveyor					;
	eMODULEs			m_eModule;
	int					m_iTick					;
	BOOL				m_bTick					;
	ULONGLONG			m_ulStartTime			= 0;
	BOOL				m_bJog					= FALSE;			// TRUE 상태 일때 Jog 로 동작 / FALSE 상태 일때 Stop 하기전까지 계속 동작 
	
protected:
	void				fn_ComputeArea			();

public:
	BOOL				Initialize				() { return TRUE; };
	void				ConveyorMove(BOOL bFwd);
	
};

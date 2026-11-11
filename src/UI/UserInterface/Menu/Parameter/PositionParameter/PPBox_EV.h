#pragma once

class CMotorControl;
class CPPboxWnd : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_PARAMETER_LIST			,
	eS_MOTOR_CONTROL			,
	eS_CONTROL_AREA				,
	eS_ALIGN_AREA				,
	eS_IO_AREA					,
	eS_END						,
};
enum eCOMMAND {
	eC_NONE						= -1,
	eC_LOCK						,
	eC_SAVE						,

	//Align Button
	eC_PPBOX_ALIGN_X_L_FWD		,
	eC_PPBOX_ALIGN_X_L_BWD		,
	eC_PPBOX_ALIGN_X_R_FWD		,
	eC_PPBOX_ALIGN_X_R_BWD		,
	eC_PPBOX_ALIGN_Y_FWD		,
	eC_PPBOX_ALIGN_Y_BWD		,
	eC_PPBOX_ALIGN_ALL_FWD		,
	eC_PPBOX_ALIGN_ALL_BWD		,
	eC_PPBOX_ALIGN_Z_UP			,
	eC_PPBOX_ALIGN_Z_DOWN		,

	eC_ALIGN_IN					,
	eC_ALIGN_OUT				,
	eC_ALIGN_UP					,
	eC_ALIGN_DOWN				,

	//Conveyor Button
	eC_PPBOX_STOPPER_1_UP		,
	eC_PPBOX_STOPPER_1_DOWN		,
	eC_PPBOX_STOPPER_2_UP		,
	eC_PPBOX_STOPPER_2_DOWN		,
	eC_PPBOX_STOPPER_3_UP		,
	eC_PPBOX_STOPPER_3_DOWN		,
	eC_PPBOX_STOPPER_4_UP		,
	eC_PPBOX_STOPPER_4_DOWN		,
	eC_PPBOX_STOPPER_5_UP		,
	eC_PPBOX_STOPPER_5_DOWN		,

	eC_CV4_IN					,
	eC_CV4_OUT					,
	eC_CV4_UP					,
	eC_CV4_DOWN					,
	eC_END,
};
enum eLI_TEACHING_PARAMETER {
	eLI_NONE					,
	eLI_POSITION_NAME			,
	eLI_AXIS					,
	eLI_POSITION				,
	eLI_SPEED					,
	eLI_GET						,
	//eLI_MOVE					,
};
DECLARE_DYNCREATE(CPPboxWnd)
public:
	CPPboxWnd							(CWnd*	pwndParent = nullptr);
	virtual				~CPPboxWnd		();
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
	afx_msg void		OnLvnChange				(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void		OnLvnItemBeginAbleEdit	(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void		OnLvnItemEndAbleEdit	(NMHDR* pNMHDR, LRESULT* pResult);
protected:
	CWnd*				m_pwndParent;
public:
	void				ChangeLanguage();  // LJY 추가 2024.06.23
protected:
	CRect				m_rectS			[eS_END], 
						m_rectC			[eC_END];
	BOOL				m_bLock					;
	int					m_eMouseOver			;
	int					m_eSelectControl		;
	CListCtrlColumnEx	m_lstTeaching			;
	CMotorControl*		m_pMotorControl			;
    T_ELEVATOR_CONVEYOR	m_tPPBoxEV              ;

public:	

protected:
	void				fn_ComputeArea			();
public:
	void				SyncData				(BOOL bStore);
	void				SetListItem				();
};

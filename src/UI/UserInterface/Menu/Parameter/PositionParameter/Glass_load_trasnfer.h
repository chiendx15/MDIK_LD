#pragma once

class CMotorControl;
class CGlassLoadTransferWnd : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_PARAMETER_LIST			,
	eS_MOTOR_CONTROL			,
	eS_STATUS_AREA				,
	eS_IO_AREA					,
	eS_END						,
};
enum eCOMMAND {
	eC_NONE						= -1,
	eC_LOCK						,
	eC_SAVE						,
	eC_PAPER_CYLINDER_A_UP			,
	eC_PAPER_CYLINDER_A_DOWN		,
	eC_PAPER_CYLINDER_B_UP			,
	eC_PAPER_CYLINDER_B_DOWN		,
	eC_PAPER_CYLINDER_C_UP			,
	eC_PAPER_CYLINDER_C_DOWN		,
	eC_PAPER_CYLINDER_D_UP			,
	eC_PAPER_CYLINDER_D_DOWN		,
	eC_PAPER_CYLINDER_ALL_UP		,
	eC_PAPER_CYLINDER_ALL_DOWN		,
	eC_PAPER_CYLINDER_PUSHER_UP		,
	eC_PAPER_CYLINDER_PUSHER_DOWN	,

	eC_GLASS_VACUUM_ON				,
	eC_GLASS_VACUUM_OFF				,
	//eC_GLASS_BLOW_ON				,
	//eC_GLASS_BLOW_OFF				,
	eC_PAPER_VACUUM_ON				,
	eC_PAPER_VACUUM_OFF				,
	//eC_PAPER_BLOW_ON				,
	//eC_PAPER_BLOW_OFF				,
	eC_PAPER_AND_GLASS_BLOW_ON		,
	eC_PAPER_AND_GLASS_BLOW_OFF		,
	eC_PAPER_PUSHER_VACUUM_ON		,
	eC_PAPER_PUSHER_VACUUM_OFF		,

	eC_LOAD_CENTER_POS			,
	eC_UNLOAD_PPBOX_CENTER_POS	,
	eC_UNLOAD_TABLE_CENTER_POS	,
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
DECLARE_DYNCREATE(CGlassLoadTransferWnd)
public:
	CGlassLoadTransferWnd							(CWnd*	pwndParent = nullptr);
	virtual				~CGlassLoadTransferWnd		();
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);
	void ChangeLanguage(); // LJY 추가 2024.06.23
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
protected:
	CRect				m_rectS			[eS_END], 
						m_rectC			[eC_END];
	BOOL				m_bLock					;
	int					m_eMouseOver			;
	int					m_eSelectControl		;
	CListCtrlColumnEx	m_lstTeaching			;
	T_GLASS_TRANSFER	m_tGlassTransfer		;
	CMotorControl*		m_pMotorControl			;

public:	

protected:
	void				fn_ComputeArea			();
public:
	void				SyncData				(BOOL bStore);
	void				SetListItem				();
};

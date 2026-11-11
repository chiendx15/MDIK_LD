#pragma once

class CRunCheck : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_END						,
};

enum eCOMMAND {
	eC_NONE						= -1,
	eC_STEP_1					,	// 사람
	eC_STEP_2					,	// 전원 
	eC_STEP_3					,	// EMO
	eC_STEP_4					,	// Door
	eC_STEP_5					,	// EMO Reset
	eC_STEP_6					,	// EMS Reset
	eC_STEP_7					,	// Vacuum1
	eC_STEP_8					,	// Vacuum2
	eC_STEP_9					,	// Alarm
	eC_STEP_10					,	// Recipe Edit
	eC_STEP_11					,	// Device Parameter
	eC_STEP_12					,	// Position Parameter
	eC_STEP_13					,	// Run Ready
	eC_STEP_14					,	// Axis Position
	eC_STEP_15					,	// init
	eC_STEP_16					,	// Auto Run
	eC_STEP_17					,	// 사람
	// Stop

	eC_CLOSE					,
	eC_END						,
};
DECLARE_DYNCREATE(CRunCheck)
public:
	CRunCheck							(CWnd*	pwndParent = nullptr);
	virtual				~CRunCheck	();
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct			);
	afx_msg void		OnKillFocus				(CWnd* pNewWnd							);
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
	CRect				m_rectS					[eS_END], 
						m_rectC					[eC_END];
	int					m_eSelectControl		;
	int					m_eMouseOver			;
	CString				strALogMsg, strALogPos;
public:	

protected:
	void				fn_ComputeArea			();
public:
	BOOL				Initialize				() { return TRUE; };
};

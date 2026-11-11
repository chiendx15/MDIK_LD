#pragma once

class CConveyorControl;
class CTeachModeConveyorWnd : public CFrameWndEx {
enum eSTATIC {
	eS_FULL_AREA = 0,
	eS_TITLE_AREA,
	eS_CONVEYOR_MODULE_AREA,

	eS_CONVEYOR_MODULE_0,
	eS_CONVEYOR_MODULE_1,
	eS_CONVEYOR_MODULE_2,
	eS_CONVEYOR_MODULE_3,
	eS_CONVEYOR_MODULE_4,
	

	eS_END								,
};

enum eCOMMAND {
	eC_NONE = -1,
	eC_TEST,
	eC_END,
};
DECLARE_DYNCREATE(CTeachModeConveyorWnd)
public:
	CTeachModeConveyorWnd							(CWnd*	pwndParent = nullptr);
	virtual				~CTeachModeConveyorWnd	();
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
	int					m_eSelectControl		= eC_NONE;
	int					m_eMouseOver			= eC_NONE;
public:	
	CConveyorControl*	m_pConveyorControl		[nCONVEYOR_LD];
protected:
	void				fn_ComputeArea			();
public:
	BOOL				Initialize				() { return TRUE; };
};

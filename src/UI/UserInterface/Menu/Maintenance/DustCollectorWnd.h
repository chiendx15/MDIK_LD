#pragma once

class CDustCollectorWnd : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,

	//<< for문 영역. 순서 바꾸면 안됨
	eS_DUST_AREA			        ,

	eS_DUST_CURRENT_PRESS			,
    eS_DUST_CURRENT_RUNNING_TIME	,

	eS_TEXT_DUST_PRESS				,
	eS_TEXT_DUST_LIGHT_ALARM_LIMIT	,
	eS_TEXT_DUST_HEAVY_ALARM_LIMIT	,
	eS_TEXT_DUST_RUNNING_TIME		,
    eS_TEXT_DUST_FILTER_CYCLE_TIME	,
	eS_END						    ,
};

enum eCOMMAND {
	eC_NONE						= -1,

	//<< for문 영역. 순서 바꾸면 안됨
	eC_EDIT_DUST_LIGHT_ALARM_LIMIT	,
	eC_EDIT_DUST_HEAVY_ALARM_LIMIT	,
    eC_EDIT_DUST_FILTER_LIMIT		,
	eC_BTN_DUST_ALARM_LIMIT_SET		,
	eC_BTN_DUST_FILTER_LIMIT_SET	,
    //>>

	eC_END						    ,
};


DECLARE_DYNCREATE(CDustCollectorWnd)
public:
	CDustCollectorWnd	(CWnd*	pwndParent = nullptr);
	virtual				~CDustCollectorWnd	();

public:
	DECLARE_MESSAGE_MAP()
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);
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
	CRect				m_rectS					[eS_END], 
						m_rectC					[eC_END];
	int					m_eSelectControl		= eC_NONE;
	int					m_eMouseOver			= eC_NONE;

	int					m_iDustSetLimit;
	int					m_iDustPressLimit[2] = {};
protected:
	void				fn_ComputeArea			();
};

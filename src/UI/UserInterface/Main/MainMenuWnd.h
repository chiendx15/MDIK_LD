#pragma once



class CMainMenuWnd : public CFrameWndEx {
typedef enum {
	eS_NONE			= -1,
	eS_FULL_AREA	,
	eS_ITEM_AREA	,
	eS_END			,
} eSTATIC;
	DECLARE_DYNCREATE(CMainMenuWnd)
public:
	CMainMenuWnd									(CWnd* pwndParent = nullptr);           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual				~CMainMenuWnd				();
	virtual BOOL		PreCreateWindow				(CREATESTRUCT& cs);
protected:
	afx_msg int			OnCreate					(LPCREATESTRUCT lpCreateStruct			);
	afx_msg void		OnKillFocus					(CWnd* pNewWnd							);
	afx_msg void		OnShowWindow				(BOOL bShow, UINT nStatus				);
	afx_msg void		OnTimer						(UINT_PTR nIDEvent						);
	afx_msg BOOL		OnEraseBkgnd				(CDC* pDC								);
	afx_msg void		OnPaint						();
	afx_msg void		OnMouseMove					(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonDown				(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonUp					(UINT nFlags, CPoint point				);
	DECLARE_MESSAGE_MAP()

protected:
	CWnd*				m_pwndParent				;
	CRect				m_rectS						[eS_END];
	CRect				m_rectC						[eMAIN_MENU_END];
	
	int					m_eMouseOver				;	
	int					m_eSelectControl			;
protected:
	void				fn_ComputeArea				();
	void				fn_UpdateControl			();
public:
	BOOL				Initialize					();	
};



#pragma once




class CExitWnd : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_MESSAGE					,
	eS_END						,
};

enum eCOMMAND {
	eC_NONE						= -1,
	eC_OK						,
	eC_CANCEL					,
	eC_END						,
};
DECLARE_DYNCREATE(CExitWnd)
public:
	CExitWnd									(CWnd*	pwndParent = nullptr);
	virtual				~CExitWnd				();
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
	CRect				m_rectS			[eS_END], 
						m_rectC			[eC_END];
	int					m_eSelectControl		;
	int					m_eMouseOver			;
	CString				strALogMsg, strALogPos;
public:	

protected:
	void				fn_ComputeArea			();
public:
	BOOL				Initialize				() { return TRUE; };
};

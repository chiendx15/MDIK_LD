#pragma once
class CPPBoxWnd : public CFrameWndEx { 
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	//eS_ITEM_AREA				,
	eS_WORK_TITLE,
	eS_WORK_AREA				,
	eS_PPBOX_AREA				,
	eS_GLASS_AREA				,
	eS_COUNT_AREA				,
	eS_ABORT_AREA				,
	eS_DB_AREA,
	eS_GLASS_COUNT				,
	eS_PAPER_COUNT				,
	eS_TOTAL_COUNT				,
	eS_GLASS_ABORT				,
	eS_PAPER_ABORT				,
	eS_CURRENT					,
	eS_PPBOX_TOP_MATERIAL		,

	eS_PPBOX_ID,
	eS_END,
};

enum eCOMMAND {
	eC_NONE						= -1,
	eC_GLASS					,	
	eC_GLASS_END				= eC_GLASS + MAX_CELL,
	eC_SELECT_GLASS				,
	eC_GLASS_CONVEYOR_ABORT		,
	eC_PAPER_CONVEYOR_ABORT		,
	eC_COMPULSION_GLASS_ABORT	,
	eC_COMPULSION_PAPER_ABORT	,
	eC_GLASS_COUNT				,
	eC_PAPER_COUNT				,
	eC_TOTAL_COUNT				,
	eC_TOTAL_COUNT_SET			,
	eC_CURRENT_MATERIAL			,
	eC_CURRENT_COUNT			,
	eC_PPBOX_TOP_MATERIAL		,
	eC_EDIT_PPBOX_ID			,
	eC_LOAD_PPBOX_DATA			,
	eC_END						,
};
DECLARE_DYNCREATE(CPPBoxWnd)
public:
	CPPBoxWnd								(CWnd*	pwndParent = nullptr);
	virtual				~CPPBoxWnd			();
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
	int					m_iSelectGlass			;
	int					m_iTotalCount			;
	CStringA			m_strPPBoxID			;
public:	
	void				fn_ComputeArea			();
public:
	BOOL				Initialize				() { return TRUE; };
};

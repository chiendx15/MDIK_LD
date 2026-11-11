#pragma once

class CDrawControlTestWnd : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_DRAW_1					,
	eS_DRAW_2					,
	eS_DRAW_3					,
	eS_DRAW_4					,
	eS_DRAW_5					,
	eS_DRAW_6					,
	eS_DRAW_7					,
	eS_DRAW_8					,
	eS_DRAW_9					,
	eS_DRAW_10					,
	eS_DRAW_11					,
	eS_DRAW_12					,
	eS_DRAW_13					,
	eS_DRAW_14					,
	eS_DRAW_15					,
	eS_DRAW_16					,
	eS_DRAW_17					,
	eS_DRAW_18					,
	eS_DRAW_19					,
	eS_DRAW_20					,
	eS_DRAW_21					,
	eS_DRAW_22					,
	eS_DRAW_23					,
	eS_DRAW_24					,
	eS_DRAW_25					,
	eS_DRAW_26					,
	eS_DRAW_27					,
	eS_DRAW_28					,
	eS_DRAW_29					,
	eS_DRAW_30					,
	eS_DRAW_31					,
	eS_DRAW_32					,
	eS_DRAW_33					,
	eS_DRAW_34					,
	eS_DRAW_35					,
	eS_DRAW_36					,
	eS_DRAW_37					,
	eS_DRAW_38					,
	eS_DRAW_39					,
	eS_DRAW_40					,
	eS_END						,
};

enum eCOMMAND {
	eC_NONE						= -1,
	eC_DRAW_1					,
	eC_DRAW_2					,
	eC_DRAW_3					,
	eC_DRAW_4					,
	eC_DRAW_5					,
	eC_DRAW_6					,
	eC_DRAW_7					,
	eC_DRAW_8					,
	eC_DRAW_9					,
	eC_DRAW_10					,
	eC_DRAW_11					,
	eC_DRAW_12					,
	eC_DRAW_13					,
	eC_DRAW_14					,
	eC_DRAW_15					,
	eC_DRAW_16					,
	eC_DRAW_17					,
	eC_DRAW_18					,
	eC_DRAW_19					,
	eC_DRAW_20					,
	eC_DRAW_21					,
	eC_DRAW_22					,
	eC_DRAW_23					,
	eC_DRAW_24					,
	eC_DRAW_25					,
	eC_DRAW_26					,
	eC_DRAW_27					,
	eC_DRAW_28					,
	eC_DRAW_29					,
	eC_DRAW_30					,
	eC_DRAW_31					,
	eC_DRAW_32					,
	eC_DRAW_33					,
	eC_DRAW_34					,
	eC_DRAW_35					,
	eC_DRAW_36					,
	eC_DRAW_37					,
	eC_DRAW_38					,
	eC_DRAW_39					,
	eC_DRAW_40					,
	eC_CLOSE					,
	eC_END						,
};
DECLARE_DYNCREATE(CDrawControlTestWnd)
public:
	CDrawControlTestWnd							(CWnd*	pwndParent = nullptr);
	virtual				~CDrawControlTestWnd	();
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

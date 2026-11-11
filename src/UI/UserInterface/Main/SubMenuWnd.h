#pragma once
#define	BLANK_HEIGHT	10

class CSubMenuWnd : public CFrameWndEx {

enum eGROUP {
	eGROUP_1,
	eGROUP_2,
	eGROUP_3,
	eGROUP_4,
	eGROUP_5,
	eGROUP_6,
	eGROUP_7,
	eGROUP_8,
	eGROUP_9,
};
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				,
	eS_ITEM_AREA				,
	eS_END						,
};
//enum eCOMMAND {
//	eC_NONE							= -1,
//	eC_CMD							,
//	eC_END,
//};
	DECLARE_DYNCREATE(CSubMenuWnd)

public:
	CSubMenuWnd(CWnd* pParent=NULL);
	virtual				~CSubMenuWnd				();
	virtual BOOL		PreCreateWindow				(CREATESTRUCT& cs);
protected:
	afx_msg int			OnCreate					(LPCREATESTRUCT lpCreateStruct			);
	afx_msg void		OnShowWindow				(BOOL bShow, UINT nStatus				);
	//afx_msg void		OnTimer						(UINT_PTR nIDEvent						);
	afx_msg BOOL		OnEraseBkgnd				(CDC* pDC								);
	afx_msg void		OnPaint						();
	afx_msg void		OnMouseMove					(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonDown				(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonUp					(UINT nFlags, CPoint point				);

	DECLARE_MESSAGE_MAP()

//==================================================================================================
// Member Veriable
protected:
	CWnd*				m_pwndParent				;
	CRect				m_rectClient				, 
						m_rectS						[eS_END], 
						m_rectC						[eSUB_MENU_END];
	int					m_eSelectControl			;
	int					m_eSelectedItem				;
	int					m_eMouseOver				;

	int					m_eStringTableIndex			;
//================================================
// Member Function
protected:
	void				fn_ComputeArea				();
public:
	BOOL				Initialize					() { return TRUE; };
	void				SetSubMenu					();
	
};

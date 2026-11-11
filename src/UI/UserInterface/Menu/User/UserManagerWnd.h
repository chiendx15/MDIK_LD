#pragma once

#include "UI.h"
#include "misc\ListCtrlEd.h"
#include "UserAddWnd.h"
#include "UserModifyWnd.h"


#define SIZE_USER_MANAGER		CSize(400, 300)
#define	USER_INFO_ID			100000

class CUserManagerWnd;
class CUserManagerWnd : public CWnd
{
	enum eS {
		eS_NONE		= -1, 
		eS_AREA,
		eS_LIST_TITLE,
		eS_LIST_AREA,
		eS_CONTROL_AREA,
		eS_END,
	};
	enum eC {
		eC_NONE		= -1,
		
		eC_BTN_ADD,
		eC_BTN_MODIFY,
		eC_BTN_DELETE,

		eC_END,
	};

	enum LI{
		LI_NONE,
		LI_INDEX,
		LI_USER_ID,
		LI_USER_GRADE,
	};

	DECLARE_DYNAMIC(CUserManagerWnd)
public:
	CUserManagerWnd(CWnd* pwndParent=NULL);
	virtual					~CUserManagerWnd();
		
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int				OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void			OnDestroy();
	afx_msg BOOL			OnEraseBkgnd(CDC* pDC);
	afx_msg void			OnPaint();
	afx_msg void			OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void			OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void			OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void			OnLButtonUp(UINT nFlags, CPoint point);

// Member Veriable
protected:
	CWnd*					m_pwndParent;
	
	CRect					m_rectClient;
	COLORREF				m_colorBackground;
	CBrush					m_brushBackground;
	
	CRect					m_rectS		[eS_END], 
							m_rectC		[eC_END]; 

	int						m_eSelectControl;
// Member Function
protected:

public:
	CListCtrlEd				m_lstUserInfo;
	CUserAddWnd				m_UserAddwnd;
	CUserModifyWnd			m_UserModifyWnd;
	void					ListRenew();
	void	ComputeArea();
};
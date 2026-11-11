#pragma once

#include "UI.h"
#define SIZE_USER_MODIFY		CSize(300, 220)
#define	IDC_EDIT_MODIFY_USER_GRADE	500002

class CUserModifyWnd;
class CUserModifyWnd : public CFrameWnd
{
	enum eS {
		eS_NONE		= -1, 
		eS_TITLE,
		eS_AREA,
		eS_USER_ID,
		eS_CURRENT_PASS,
		eS_CHANGE_PASS,
		eS_PASS_CONFIRM,
		eS_GRADE,
		eS_END,
	};
	enum eC {
		eC_NONE		= -1,
		eC_BTN_CLOSE,
		eC_BTN_MODIFY,

		eC_EDIT_USER_ID,
		eC_EDIT_CURRENT_PASS,
		eC_EDIT_CHANGE_PASS,
		eC_EDIT_PASS_CONFIRM,
		eC_EDIT_GRADE,

		eC_END,
	};

	DECLARE_DYNAMIC(CUserModifyWnd)
public:
	CUserModifyWnd(CWnd* pwndParent=NULL);
	virtual					~CUserModifyWnd();
		
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
	afx_msg LRESULT			OnKeyDownTab(WPARAM wParam, LPARAM lParam);
	afx_msg void			OnSelchangeUserGrade();

// Member Veriable
protected:
	CWnd*					m_pwndParent;
	
	CRect					m_rectClient;
	COLORREF				m_colorBackground;
	CBrush					m_brushBackground;
	
	CRect					m_rectS		[eS_END], 
							m_rectC		[eC_END]; 

	int						m_eSelectControl;
	int						m_iKeyDownTab;
	CString					m_strUserID, m_strCurrentPass, m_strChangePass, m_strConfirm, m_strGrade;
	CComboBox				m_cboGrade;
	BOOL					m_bIDCheck;
// Member Function
protected:
	void					UserModity();

public:
	int						m_iSelectUser;
	virtual BOOL			Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
	void					setData(int iSelectUser);
	void	ComputeArea();
};
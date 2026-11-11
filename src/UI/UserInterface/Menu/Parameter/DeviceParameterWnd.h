#pragma once

class CDeviceParameterDefaultWnd;
class CDeviceParameterWnd : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_DEVICE_LIST_AREA			,
	eS_DEVICE_PARAMETER			,
	eS_TAB						,
	eS_END						,
};

enum eCOMMAND {
	eC_NONE						= -1,
	eC_DEFAULT					,

	eC_END						,
};
DECLARE_DYNCREATE(CDeviceParameterWnd)
public:
	CDeviceParameterWnd							(CWnd*	pwndParent = nullptr);
	virtual				~CDeviceParameterWnd	();
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct			);
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
	void ChangeLanguage(); // LJY 추가 2024.06.23
protected:
	CRect				m_rectS					[eS_END], 
						m_rectC					[eC_END];
	int					m_eSelectControl		;
	int					m_eMouseOver			;
	int					m_eSelectTab			;
	CString				strALogMsg, strALogPos;
	BOOL				m_bEdit;
public:	
	CDeviceParameterDefaultWnd*				m_pwndDefault;
protected:
	void				fn_ComputeArea			();
	void				ShowTabWindow			(int iTab);
public:
};

#pragma once

class CLogInWnd : public CFrameWndEx {
enum eLI_STATIC {
	eS_NONE				= -1,
	eS_FULL_AREA		,
	
    eS_IMAGE_AREA       ,

    eS_LOGIN_AREA		,
	eS_USER_NAME		,
	eS_USER_PASSWORD	,
	
    eS_END				,
};
enum eLI_CONTROL {
	eC_NONE				= -1,
	eC_EDIT_NAME		, 
	eC_EDIT_PASSWORD	,
	eC_RMS_PASSWORD_MODE,
	eC_OK				,
	eC_END              ,
};
	DECLARE_DYNCREATE(CLogInWnd)
public:
	CLogInWnd									(CWnd* pwndParent=nullptr);;
	virtual				~CLogInWnd				();
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);
    virtual BOOL        PreTranslateMessage     (MSG* pMsg);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void		OnShowWindow			(BOOL bShow, UINT nStatus);
	afx_msg BOOL		OnEraseBkgnd			(CDC* pDC);
	afx_msg void		OnPaint					();
	afx_msg void		OnMouseMove				(UINT nFlags, CPoint point);
	afx_msg void		OnLButtonDown			(UINT nFlags, CPoint point);
	afx_msg void		OnLButtonUp				(UINT nFlags, CPoint point);
	afx_msg LRESULT		OnEditTab				(WPARAM wParam, LPARAM lParam);
protected:
	CWnd*				m_pwndParent			;
	CRect				m_rectStatic			[eS_END];
	CRect				m_rectControl			[eC_END];

	int					m_eMouseOver,
						m_eSelectControl;

	int					m_iSelectEdit;
	CRect               m_rectShowWindowPos;
	CString				m_strID, m_strPass;
    Gdiplus::Image*		m_imgLogo = nullptr;
protected:
	void				fn_ComputeArea();
    void				OnOK();
public:
	BOOL				Initialize() { return TRUE; };
	afx_msg void OnClose();
};



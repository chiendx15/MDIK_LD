#pragma once

class CPositionParameterWnd : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_DEVICE_LIST				,
	eS_WORK_AREA				,
	eS_END						,
};

enum eCOMMAND {
	eC_NONE						= -1,
    eC_PPBOX_EV,
	eC_GLASS_LOAD_TRANSFER,
	eC_END,
};
DECLARE_DYNCREATE(CPositionParameterWnd)
public:
	CPositionParameterWnd							(CWnd*	pwndParent = nullptr);
	virtual				~CPositionParameterWnd		();
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
	void				ChagneLanguage(); // LJY 추가 2024.06.23
protected:
	CRect				m_rectS			[eS_END], 
						m_rectC			[eC_END];
	int					m_eMouseOver			;
	int					m_eSelectControl		;
	int					m_eSelectTab			;

	CFrameWndEx*		m_pwndPPBoxEV; 
    CFrameWndEx*        m_pwndGlassLoadTrasnfer;
	HWND							    m_hwndLast;

public:	

protected:
	void				fn_ComputeArea			();
	void				fn_SetView				(int eSelectedTab = eC_GLASS_LOAD_TRANSFER);
public:
	BOOL				Initialize				() { return TRUE; };
};

#pragma once


#define __MAX_IO_ITEM__ 32
#define __MAX_IN_TAB__	544 / __MAX_IO_ITEM__
#define __MAX_OUT_TAB__	544 / __MAX_IO_ITEM__
#define TIMER_REDRAW		10003

class CAGVPIOStatusWnd : public CFrameWndEx {
enum eSTATIC {
	eS_FULL_AREA,
	eS_BF_TITLE,
	eS_EQP_TITLE,
	eS_BF_AREA,
	eS_EQP_AREA,
	eS_END,
};

enum eCOMMAND {
	eC_NONE = -1,
	eC_BF_PIO_1,
	eC_BF_PIO_2,
	eC_BF_PIO_3,
    eC_BF_PIO_4,
	eC_BF_PIO_5,
    eC_BF_PIO_6,
	eC_BF_PIO_7,
    eC_BF_PIO_8,

	eC_EQ_PIO_1,
	eC_EQ_PIO_2,
	eC_EQ_PIO_3,
	eC_EQ_PIO_4,
	eC_EQ_PIO_5,
	eC_EQ_PIO_6,
	eC_EQ_PIO_7,
	eC_EQ_PIO_8,
	eC_END,
};
DECLARE_DYNCREATE(CAGVPIOStatusWnd)
public:
	CAGVPIOStatusWnd (CWnd*	pwndParent = nullptr);
	virtual				~CAGVPIOStatusWnd();
	virtual BOOL		PreCreateWindow(CREATESTRUCT& cs);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int			OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void		OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void		OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL		OnEraseBkgnd(CDC* pDC);
	afx_msg void		OnPaint();
	afx_msg void		OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void		OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void		OnLButtonUp(UINT nFlags, CPoint point);
protected:
	CWnd*				m_pwndParent;
public:
protected:
	CRect				m_rectS					[eS_END], 
						m_rectC					[eC_END];
	int					m_eMouseOver			;
	int					m_eSelectControl;
	
public:	

protected:
	void				fn_ComputeArea			();

};

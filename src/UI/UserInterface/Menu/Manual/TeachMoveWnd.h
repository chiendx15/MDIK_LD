
#pragma once

#define TEACH_MOVE_COUNT 2	// KES 수정 2024.05.23 - PPBOX_EV, GLASS_TRANSFER

class CTeachMoveWnd : public CFrameWndEx
{
	enum eSTATIC {
		eS_NONE = -1,
		eS_FULL_AREA,

		eS_PPBOX_EV,
		eS_GLASS_TRANSFER,

		eS_END,
	};
	
	enum eCOMMAND {
		eC_NONE = -1,
		
		eC_TEST,

		eC_END,
	};

	enum eLIST_NAME {
		eLN_NONE,
		eLN_NAME,
		eLN_MOVE,
	};

	DECLARE_DYNCREATE(CTeachMoveWnd)

public:
	CTeachMoveWnd(CWnd* pwndParent = nullptr);
	virtual ~CTeachMoveWnd();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void ChangeLanguage(); // LJY 추가 2024.06.23
protected:
	CListCtrlColumnEx	m_lst[TEACH_MOVE_COUNT];

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int			OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void		OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void		OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void		OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void		OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT		OnEditFinished(WPARAM wParam, LPARAM lParam);
	afx_msg void		OnPaint();
	afx_msg void		OnTimer(UINT_PTR nIDEvent);
	afx_msg void		OnDestroy();
	afx_msg void		OnLvnEndEditPPBoxEV(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void		OnLvnEndEditGlassTransfer(NMHDR* pNMHDR, LRESULT* pResult);

protected:
	CWnd*				m_pwndParent;	
	CRect				m_rectS[eS_END], 
						m_rectC[eC_END];
	int					m_eSelectControl;
	int					m_eMouseOver;
	int					m_eEndEdit;

protected:
	void				fn_ComputeArea();

};



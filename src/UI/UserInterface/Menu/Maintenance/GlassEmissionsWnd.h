#pragma once


class CGlassEmissionsWnd : public CFrameWndEx
{
	enum eS {
		eS_FULL_AREA,
		
		eS_TITLE,
		eS_CBO_YEAR,
		eS_LIST_MONTH,
		eS_LIST_DAY,
		eS_LIST_TIME,

		eS_YEAR_COUNT,
		eS_INPUT,
		eS_OUTPUT,

		eS_END,
	};
	
	enum eC {
		eC_NONE = -1,

		eC_INPUT,
		eC_OUTPUT,
		eC_LOAD,
		eC_SAVE,

		eC_END,
	};

	enum eCOL_NAME {
		eCN_NONE,
		eCN_DATE,
		eCN_INPUT,
		eCN_OUTPUT,
	};

	enum eLOG_LIST {
		eLL_M,
		eLL_D,
		eLL_H,
		nLIST,
	};

	DECLARE_DYNCREATE(CGlassEmissionsWnd)
public:
	CGlassEmissionsWnd(CWnd* pwndParent = nullptr);
	virtual ~CGlassEmissionsWnd();
	//HWND hMonthCal = NULL; 
	//CMonthCalCtrl* m_MonthCal = NULL;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int				OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void			OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void			OnPaint();
	afx_msg void			OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void			OnLButtonUp(UINT nFlags, CPoint point);

protected:
	CWnd*					m_pwndParent;
	CRect					m_rectS[eS_END],
							m_rectC[eC_END],
							m_rectClient;
	int						m_eSelectControl;

	CListCtrlColumnEx		m_lst[nLIST];

	CComboBox				m_cboYear;
	
protected:
	void					fn_ComputeArea();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};



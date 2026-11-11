#pragma once

class CConveyorParameter : public CFrameWndEx {
	enum eSTATIC {
		eS_NONE = -1,
		eS_FULL_AREA = 0,

		eS_CONVEYOR_LIST,
		eS_WORK_AREA,
		eS_CONVEYOR_PARAMETER_AREA,

		eS_PARAMETER_FORWARD,
		eS_PARAMETER_SPEED_RATE,

		eS_END,
	};

	enum eCOMMAND {
		eC_NONE = -1,
		eC_EDIT,
		eC_SAVE,
		eC_MOTOR_NAME,

		eC_BTN_PARAMETER_FORWARD,
		eC_EDIT_PARAMETER_SPEED_RATE,

		eC_BTN_ALL_SET_PARAMETER,
		eC_END,
	};
	enum eLI_CONVEYOR {
		eLI_NONE,
		eLI_CONVEYOR_INDEX,
		eLI_CONVEYOR_NAME,
	};



	DECLARE_DYNCREATE(CConveyorParameter)
public:
	CConveyorParameter(CWnd* pwndParent = nullptr);
	virtual				~CConveyorParameter();
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
	afx_msg void		OnItemchangedLst(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT		OnEditFinished(WPARAM wParam, LPARAM lParam);
	afx_msg void		OnCbnSelchangeComboType(UINT nID);

protected:
	CWnd* m_pwndParent;
public:
	void ChangeLanguage();
protected:
	CRect					m_rectS[eS_END],
		m_rectC[eC_END];
	int						m_eMouseOver;
	int						m_eSelectControl, m_eSelectedEdit;
	int						m_eSelectTab;
	int						m_eSelectMotor;
	BOOL					m_bEdit;
	T_CONVEYOR_PARAMETER	m_ConveyorParameter[nCONVEYOR_LD];
	CString					m_strChangeLog;
	CListCtrlColumnEx		m_lstMotor;

	BOOL				m_b2ChUse = TRUE;
	CString m_strParameter[4] = {
		// Axis Init
		_T("High Speed"),
		_T("Low Speed"),
		_T("Address"),
		_T("Torque"),
	};

public:

protected:
	void				fn_ComputeArea();
public:
	BOOL				Initialize() { return TRUE; };
	void				SyncData(BOOL bStore = FALSE);
	BOOL				ChangeParameter();
};

#pragma once

class CConveyorMonitorWnd : public CFrameWndEx {
	enum eSTATIC {
		eS_NONE = -1,
		eS_FULL_AREA = 0,

		eS_CONVEYOR_LIST,
		eS_WORK_AREA,
		eS_CONVERYO_NAME,
		eS_CONVEYOR_STATUS_AREA,

		eS_READY,
		eS_RUN_STOP,
		eS_DIRECTION,

		eS_FREQUENCY,
		eS_ACC1,
		eS_ACC2,
		eS_DEC1,
		eS_DEC2,

		eS_TRIP_FACTOR_UPPER_1,
		eS_TRIP_FACTOR_LOWER_1,
		eS_TRIP_FACTOR_UPPER_2,
		eS_TRIP_FACTOR_LOWER_2,
		eS_TRIP_FACTOR_UPPER_3,
		eS_TRIP_FACTOR_LOWER_3,
		eS_TRIP_FACTOR_UPPER_4,
		eS_TRIP_FACTOR_LOWER_4,

		eS_EDIT_READY,
		eS_EDIT_RUN_STOP,
		eS_EDIT_DIRECTION,
		eS_EDIT_FREQUENCY,
		eS_EDIT_ACC1,
		eS_EDIT_ACC2,
		eS_EDIT_DEC1,
		eS_EDIT_DEC2,

		eS_EDIT_TRIP_FACTOR_UPPER_1,
		eS_EDIT_TRIP_FACTOR_UPPER_2,
		eS_EDIT_TRIP_FACTOR_UPPER_3,
		eS_EDIT_TRIP_FACTOR_UPPER_4,

		eS_EDIT_TRIP_FACTOR_LOWER_1,
		eS_EDIT_TRIP_FACTOR_LOWER_2,
		eS_EDIT_TRIP_FACTOR_LOWER_3,
		eS_EDIT_TRIP_FACTOR_LOWER_4,

		eS_END,
	};

	enum eCOMMAND {
		eC_NONE = -1,
		eC_END,
	};

	enum eLI_CONVEYOR {
		eLI_NONE,
		eLI_CONVEYOR_INDEX,
		eLI_CONVEYOR_NAME,
	};



	DECLARE_DYNCREATE(CConveyorMonitorWnd)
public:
	CConveyorMonitorWnd(CWnd* pwndParent = nullptr);
	virtual				~CConveyorMonitorWnd();
	virtual BOOL		PreCreateWindow(CREATESTRUCT& cs);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int			OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void		OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void		OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL		OnEraseBkgnd(CDC* pDC);
	afx_msg void		OnPaint();
	afx_msg void		OnItemchangedLst(NMHDR* pNMHDR, LRESULT* pResult);
protected:
	CWnd* m_pwndParent;
public:
	void ChangeLanguage();
protected:
	CRect					m_rectS[eS_END];
	int						m_eSelectConveyor;
	CListCtrlColumnEx		m_lstConveyor;
public:

protected:
	void				fn_ComputeArea();
};

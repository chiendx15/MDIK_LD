#pragma once


class CParameterChangeLogWnd : public CFrameWndEx
{
	enum eS{
		eS_FULL_AREA,
		
		eS_CONTROL_AREA,
		eS_LOG_AREA,

		eS_START_CAL_TEXT,
		eS_START_CAL_AREA,

		eS_END_CAL_TEXT,
		eS_END_CAL_AREA,

		eS_END,
	};
	enum eC{
		eC_NONE =-1,
		eC_LOGFOLDER_OPEN,
		eC_END,
	};

	enum {
		eLI_COLUMN_NONE,
		eLI_COLUMN1,
		eLI_COLUMN2,
		eLI_COLUMN3,
		eLI_COLUMN4,
		eLI_COLUMN5,
		eLI_COLUMN6,
		eLI_COLUMN7,
		eLI_COLUMN8,
		nLI_COLUMN,
	};



	DECLARE_DYNCREATE(CParameterChangeLogWnd)
public:
	CParameterChangeLogWnd(CWnd* pwndParent = nullptr);
	virtual ~CParameterChangeLogWnd();

	CMonthCalCtrl* m_startMonthCal = nullptr;
	CMonthCalCtrl* m_endMonthCal = nullptr;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int				OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void			OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void			OnPaint();
	afx_msg void			OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void			OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void			OnListSelected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void			OnCalSelected(NMHDR* pNMHDR, LRESULT* pResult);

protected:
	CWnd*					m_pwndParent;
	CRect					m_rectS[eS_END],
							m_rectC[eC_END],
							m_rectClient;
	int						m_eSelectControl;

	CCBSpeedListCtrl		m_listLog;
	BOOL					m_bDirect[nLI_COLUMN] = { FALSE, };			// FALSE : 내림차순 / TRUE : 오름차순
protected:
	void					ComputeArea();
	void					OpenLogFile();
	void					OpenLogFile(CTime _time);
};



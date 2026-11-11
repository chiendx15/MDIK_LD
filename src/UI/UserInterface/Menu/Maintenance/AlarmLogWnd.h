#pragma once


class CAlarmLogWnd : public CFrameWndEx
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
		eLI_COLUMN_TIME,
		eLI_COLUMN_STATUS,
		eLI_COLUMN_INDEX,
		eLI_COLUMN_TYPE,
		eLI_COLUMN_NAME,
		eLI_COLUMN_USER,
		eLI_COLUMN_DESCRIPTION,
		nLI_COLUMN,
	};

	DECLARE_DYNCREATE(CAlarmLogWnd)
public:
	CAlarmLogWnd(CWnd* pwndParent = nullptr);
	virtual ~CAlarmLogWnd();

	CMonthCalCtrl* m_startMonthCal = nullptr;
	CMonthCalCtrl* m_endMonthCal = nullptr;

	void ChangeLanguage();	// KES 추가 2024.07.08
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

	CCBSpeedListCtrl		m_listAlarm;
	BOOL					m_bDirect[nLI_COLUMN] = { FALSE, };			// FALSE : 내림차순 / TRUE : 오름차순
	
protected:
	void					ComputeArea();
	void					OpenLogFile();
	void					OpenLogFile(CTime _time);
};



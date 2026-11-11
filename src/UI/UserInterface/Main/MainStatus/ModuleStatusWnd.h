#pragma once
#define INTERVAL_UI 7000
class CScrapeWnd;
class CModuleStatusWnd : public CWnd
{
	DECLARE_DYNAMIC(CModuleStatusWnd);
	enum eS {
		eS_NONE,
		eS_FULL_AREA,
		eS_TITLE_AREA,
		eS_END,
	};
	enum eC {
		eC_NONE,
		eC_STATUS,
		eC_CUT,
		eC_LD_ULD,
		eC_TACK_TIME,
		eC_END,
	};
public:
	void ComputArea();
    void CloseScrapWnd();
	CString m_strModule;
	eMODULEs m_eModule;
	CScrapeWnd* m_pwndScrape;
public:
	CModuleStatusWnd(CString strModule, eMODULEs eModule);
	  virtual	~CModuleStatusWnd();
protected:
	CRect m_rectS[eS_END], m_rectC[eC_END];
	int m_eSelectControl;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

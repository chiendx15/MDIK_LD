#pragma once


// CScrapeWnd frame
class CInsertMaterialListWnd;
class CScrapeWnd : public TIPCT<CFrameWndEx>
{
	enum eS{
		eS_FULL_AREA,
		eS_TITLE,
		eS_STATUS_TEXT,
		eS_END,
	};
	enum eC {
		eC_NONE = -1,
		eC_INPUT_STOP,
		eC_SCRAPE	,
		eC_CLOSE	,		
		eC_END,
	};
	DECLARE_DYNCREATE(CScrapeWnd)

public:
	CScrapeWnd(CWnd* pwndParent = nullptr);
	virtual ~CScrapeWnd();

protected:
	int		m_eSelectControl;
	CRect	m_rectClient
		, m_rectS[eS_END]
		, m_rectC[eC_END];
	eMODULEs	m_eModule;
	CString		m_strName;
	BOOL		m_bBuffer = FALSE;
protected:
	void ComputArea();
	void RemoveMaterial();
	void InsertMaterial();

public: 
	void SetModule(eMODULEs eModule, CString strName) {
		m_eModule = eModule;
		m_strName = strName;
		ComputArea();
		Invalidate();
	}
	
	CInsertMaterialListWnd* m_pwndInsertMaterialList;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_IP()
};



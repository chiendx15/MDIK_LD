#pragma once

// CPassWordWnd frame
#define INTERVAL_UI 7001
#define IDC_LIST_AXIS 9000
class CPassWordWnd : public CFrameWndEx
{
	enum eS{
		eS_AREA,
		eS_TEXT,
		eS_PASS,
		eS_END,
	};
	enum eC{
		eC_NONE = -1,
		eC_EDIT_PASS,
		eC_BTN_OK,
		eC_END,
	};
	DECLARE_DYNCREATE(CPassWordWnd)
public:
	CPassWordWnd(CWnd* pwndParent = nullptr);           // protected constructor used by dynamic creation
	virtual ~CPassWordWnd();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CWnd* m_pwndParent;
protected:
	int		m_eSelectControl, m_eObjectItem;
	CRect	m_rectClient
		, m_rectS[eS_END]
		, m_rectC[eC_END];
	CString m_strPassword = L"";
	CString m_strInputPassword = L"";
	eDRIVE_MODE	m_eDriveMode;
protected:
	void ComputArea();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT		OnEditFinished(WPARAM wParam, LPARAM lParam);
public:
//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void OnClickOk();
	void SetDriveMode(eDRIVE_MODE eDriveMode);
	BOOL SyncData();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnClose();
};



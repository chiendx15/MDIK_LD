#pragma once

class COPCallMessageWnd : public CFrameWndEx, public IWNDIPCommandTarget {
enum eSTATIC {
	eS_NONE = -1,
	eS_FULL_AREA,
	eS_LIST_TABLE,
	eS_LIST_AREA,
	eS_ETC_AREA,
	eS_TEXT_MSG,
	eS_END,
};

enum eCOMMAND {
	eC_NONE = -1,
	eC_TEMP,
	eC_BTN_CONFIRM,
	eC_EDIT_MSG,
	eC_END,
};

enum eLIST_ITEM {
	eLI_NONE,
	eLI_CONFIRM,
	eLI_OCCURRED,
	eLI_TIME,
	eLI_ID,
	eLI_MODULE,
	eLI_MSG,
	eLI_END,
};



	DECLARE_DYNCREATE(COPCallMessageWnd)
public:
	COPCallMessageWnd();
	virtual				~COPCallMessageWnd			();
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void		OnShowWindow			(BOOL bShow, UINT nStatus);
	afx_msg void		OnPaint					();
	afx_msg void		OnMouseMove				(UINT nFlags, CPoint point);
	afx_msg void		OnLButtonDown			(UINT nFlags, CPoint point);
	afx_msg void		OnLButtonUp				(UINT nFlags, CPoint point);
	afx_msg void		OnSelchanging			(NMHDR* pNMHDR, LRESULT* pResult);

private:
	//DECLARE_IP_AND_INTERFACE()
	//ip_handler BOOL		OnIP_INTERLOCK_MESSAGE_SHOW(CIPCommand& cmd);

protected:
	int					m_eSelectControl		= eC_NONE;
	int					m_eMouseOver			= eC_NONE;
	CRect				m_rectS[eS_END], m_rectC[eC_END];
	//CListCtrlColumnEx	m_lst;
	CCBSpeedListCtrl	m_lst;
	CString				m_strMSG;
	std::vector<T_MESSAGE> m_vecMessage;

protected:
	void 				ComputeArea();

public:
	void UpdateData();
	//BOOL GetInterlock(int iUnit = -1);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


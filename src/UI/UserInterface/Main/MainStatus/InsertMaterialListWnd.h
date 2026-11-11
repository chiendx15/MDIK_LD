#pragma once


// CInsertMaterialListWnd frame

class CInsertMaterialListWnd : public TIPCT<CFrameWndEx>
{
	enum eS
	{
		eS_FULL_AREA,
		eS_TITLE,
		eS_LIST,
		eS_END,
	};
	
	enum eC 
	{
		eC_NONE = -1,
		eC_CLOSE	,		
		eC_APPLY	,		
		eC_END,
	};

	enum
	{
		eLI_MATERIAL_REMOVAL_TIME,
		eLI_MCR_ID,
		eLI_GLASS_ID,
		eLI_LOT_ID,
	};

	DECLARE_DYNCREATE(CInsertMaterialListWnd)

public:
	CInsertMaterialListWnd(CWnd* pwndParent = nullptr);

protected:
	CRect	m_rectClient, m_rectS[eS_END], m_rectC[eC_END];

	int			m_eSelectControl = eC_NONE;
	eMODULEs	m_eModule = eMODULE_NONE;
	WORD		m_wStep = 0;

	CCBSpeedListCtrl	m_listMaterial;
	std::list<T_CELL_INFO>	m_glassInfoList;

protected:
	void ComputArea();
	void GetMaterialList();

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();

public:
	DECLARE_IP()
	ip_handler BOOL OnIPInsertMaterial(CIPCommand& cmd);
};



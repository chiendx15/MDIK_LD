#pragma once


// CRecipeSelectWnd

class CRecipeSelectWnd : public CFrameWndEx
{
	DECLARE_DYNAMIC(CRecipeSelectWnd)

	enum eSTATIC
	{
		eS_NONE = -1,
		eS_TITLE_AREA,
			eS_SEARCH_TEXT,
			eS_APPLY_TEXT,
			eS_APPLY_RECIPE,
		eS_LIST_AREA,
		eS_END,
	};

	enum eCONTROL
	{
		eC_NONE = -1,
		eC_EDIT_SEARCH_ID,
		eC_BTN_SEARCH,
		eC_BTN_CIM_APPLY,
		eC_BTN_CIM_DELETE,
		eC_BTN_ALL_APPLY,
		eC_BTN_LOAD,
		eC_BTN_APPLY,
		//eC_BTN_NEW,	// KES 주석 2024.07.25 - MDI 요청 새로만들기 기능 삭제
		eC_BTN_DELETE,
		eC_BTN_COPY,
		eC_END,
	};

public:
	CRecipeSelectWnd();
	virtual ~CRecipeSelectWnd();
private:
	CListCtrlColumnEx		m_lstRecipe; // 레시피 리스트 컨트롤
	CString					m_searchID;

	CRect					m_rectClient, m_rectS[eS_END], m_rectC[eC_END];
	int						m_iSelectControl = eC_NONE;
	int						m_iMoveControl = eC_NONE;

private:
	void computeArea();

public:
	//<< KES 추가 2024.4.4
	BOOL CreateNewRecipe(BOOL bBtn = FALSE);
	BOOL CopyAppliedRecipe();
	//>>

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnRecipeListChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemBeginAbleRecipeList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemEndAbleRecipeList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
};



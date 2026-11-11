#pragma once


// RegisterViewer
#define ID_REGISTER_LIST_1 9000
#define ID_REGISTER_LIST_2 9001
#define ID_REGISTER_LIST_3 9002
#define ID_REGISTER_LIST_4 9003
#define ID_REGISTER_LIST_5 9004

#define INTERVAL_TIMER	10000
class RegisterViewer : public CFrameWndEx
{
	enum eS{
		eS_INIT,
		eS_AREA_TAB,
		eS_LIST_1_NAME,
		eS_LIST_2_NAME,
		eS_LIST_3_NAME,
		eS_LIST_4_NAME,
		eS_AREA_LIST,
		eS_LIST_1,
		eS_LIST_2,
		eS_LIST_3,
		eS_LIST_4,
		eS_END,
	};
	enum eC{
		eC_NONE,
		eC_LOCK,
		eC_END,
	};
	enum eLIST{
		eLIST_WORD_1,
		eLIST_WORD_2,
		eLIST_BIT_1,
		eLIST_BIT_2,
		eLIST_END,
	};
	enum eLI{
		eLI_INDEX,
		eLI_ADDRESS,
		eLI_NAME,
		eLI_VALUE_CRT,
		eLI_VALUE_INPUT,
		eLI_SET,
	};
	DECLARE_DYNAMIC(RegisterViewer)


public:
	RegisterViewer();
	virtual ~RegisterViewer();
public:
	CRect	m_rectClient, m_rectC[eC_END], m_rectS[eS_END];
	int		m_eSelectControl, m_eSelectTeb;
	CListCtrlColumnEx	m_list[eLIST_END];
	T_REGISTER			m_RegisterMemory;
	BOOL	m_bLock = TRUE;
	void ChangeLanguage();
protected:
	void fn_ComputeArea();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLvnItemEndAbleEdit1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemEndAbleEdit2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemEndAbleEdit3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemEndAbleEdit4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemBeginAbleEdit(NMHDR* pNMHDR, LRESULT* pResult);
};



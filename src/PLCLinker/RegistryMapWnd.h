#pragma once


// CRegistryMapWnd
#include "misc/ListCtrlColumnEx.h"
#define ID_LIST_DATA	10000
#define INTERVAL_DATA	9000
typedef struct T_REG_DATA{
	CString szRegID;
	int iType;
	int iData;
	double dData;
	int iInputData;
	double dInputData;
}T_REG_DATA;
class CRegistryMapWnd : public CFrameWnd
{
	enum eS {
		eS_NONE,
		eS_AREA,
		eS_DATA_AREA,
		eS_CONTROL_AREA,
		eS_END,
	};
	enum eC {
		eC_NONE,
		eC_CLOSE,
		eC_ADD,
		eC_DELETE,
		eC_READ,
		eS_STOP,
		eC_END,
	};
	enum eLI {
		
		eLI_INDEX,
		eLI_REG_ID,
		eLI_CRT,
		eLI_TYPE,
		eLI_INPUT_I,
		eLI_INPUT_D,
		eLI_SET_BTN,
		eLI_END,
	};
	DECLARE_DYNAMIC(CRegistryMapWnd)

public:
	CRegistryMapWnd();
	virtual ~CRegistryMapWnd();

protected:
	int m_eSelectControl, m_eSelectList;
	BOOL m_bLoop;
	CRect m_rectS[eS_END],m_rectC[eC_END], m_rectClient;
	CListCtrlColumnEx	m_lstData;
	std::vector<T_REG_DATA> m_RegDatas;
protected:
	void ComputArea();
	void UpdateList();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void		OnLvnItemEndAbleEdit	(NMHDR* pNMHDR, LRESULT* pResult);
};



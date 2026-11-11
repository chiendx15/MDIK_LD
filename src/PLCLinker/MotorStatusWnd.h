#pragma once
#include "misc/ListCtrlColumnEx.h"

// CMotorStatusWnd 프레임

class CMotorStatusWnd : public CFrameWnd
{
	enum eS {
		eS_LIST,
		eS_OPERATIONREADY,		
		eS_SEERVOON,				
		eS_SYSTEMBUSY,			
		eS_SERVOREADY,			
		eS_SERVOERROR,			
		eS_FOLLOWINGERROR,		
		eS_SPEEDERROR,			
		eS_ZEROPOINTNOTSET,		
		eS_INPOSITION,		
		eS_ZEROPOINTPOSITION,
		eS_ZEROPOINTRETURNSET,
		eS_ALARM,				
		eS_WARNING,			
		eS_CMDREADY,			
		eS_MAINPOWER,			
		eS_TORQUELIMIT,		
		eS_SOFTPLIMIT,		
		eS_SOFTNLIMIT,		
		eS_PLIMIT,			
		eS_NLIMIT,			
		eS_HOMESENSOR,		
		eS_BREAK,				
		eS_END,
	};
	enum eC {
		eC_NONE,
		eC_OPERATIONREADY,
		eC_SEERVOON,
		eC_SYSTEMBUSY,
		eC_SERVOREADY,
		eC_SERVOERROR,
		eC_FOLLOWINGERROR,
		eC_SPEEDERROR,
		eC_ZEROPOINTNOTSET,
		eC_INPOSITION,
		eC_ZEROPOINTPOSITION,
		eC_ZEROPOINTRETURNSET,
		eC_ALARM,
		eC_WARNING,
		eC_CMDREADY,
		eC_MAINPOWER,
		eC_TORQUELIMIT,
		eC_SOFTPLIMIT,
		eC_SOFTNLIMIT,
		eC_PLIMIT,
		eC_NLIMIT,
		eC_HOMESENSOR,
		eC_BREAK,
		eC_IN_0,
		eC_IN_31 = eC_IN_0 + 32,
		eC_OUT_0,
		eC_OUT_31 = eC_OUT_0 + 32,

		eC_END,
	};
	enum eLI {
		eLI_NONE,
		eLI_INDEX,
		eLI_END,
	};
	DECLARE_DYNCREATE(CMotorStatusWnd)
protected:
public : 
	void ComputeArea();
	CMotorStatusWnd();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMotorStatusWnd();
public : 
	CRect m_rectC[eC_END], m_rectS[eS_END], m_rectClient;
	int					m_iAxisSelect;
	int					m_eSelectControl;
	CListCtrlColumnEx	m_lstAxis;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnItemchangedLst(NMHDR* pNMHDR, LRESULT* pResult);
};



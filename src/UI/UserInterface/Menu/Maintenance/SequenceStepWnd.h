#pragma once


// CSequenceStepWnd 프레임

class CSequenceStepWnd : public CFrameWndEx
{
enum eSEQUENCE_STEP_COLUMN
{
	eSSC_TIME		,
	eSSC_CUR_STEP	,
	eSSC_NEXT_STEP	,
	eSSC_NAME		,
	eSSC_DESCRIPTION,
};
enum eSEQUENCE_LIST
{
	eSEQUENCE_LIST_1,
	eSEQUENCE_LIST_2,
	nSEQUENCE_LIST,
};
enum eSTATIC 
{
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_CAL_AREA					, // 달력 공간
	eS_TAB_AREA					, // 탭 공간
	eS_STEP_AREA_1				, // 스탭 정보 공간
	eS_STEP_AREA_2				, // 스탭 정보 공간
	eS_STEP_COMBO_1				,
	eS_STEP_COMBO_2				,
	eS_MODULE_INFO_AREA			, // 모듈 정보 공간

	eS_END						,
};

public:
	CSequenceStepWnd(CWnd* pwndParent = nullptr);           
	virtual ~CSequenceStepWnd();
	CMonthCalCtrl* m_MonthCal = NULL;
protected:
	CWnd*				m_pwndParent;
protected:
	CRect				m_rectS[eS_END]; 
	
// 모듈 스탭 관련
private:
	CCBSpeedListCtrl	m_moduleStepListctrl[nSEQUENCE_LIST];
	CComboBox			m_cboModule[nSEQUENCE_LIST];
private:
	void ComputeArea();
	void UpdateSequenceStep(int iIndex = 0);

// mfc 함수
public:
	DECLARE_DYNCREATE(CSequenceStepWnd)
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnCalSelected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
};



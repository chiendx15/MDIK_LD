#pragma once


// CNumericKeypadWnd frame

class CNumericKeypadWnd : public TIPCT<CFrameWndEx>
{
	enum eS{
		eS_FULL_AREA,
		eS_BEFORE,
		eS_AFTER,
		eS_END,
	};
	enum eC {
		eC_NONE = -1,
		eC_EDIT_BEFORE	,
		eC_EDIT_AFTER	,
		eC_CANCLE		,
		eC_UNDO			,
		eC_DELETE		,
		eC_BACKSPACE	,
		eC_APPLY		,
		eC_NUMBER_0		,
		eC_NUMBER_1		,
		eC_NUMBER_2		,
		eC_NUMBER_3		,
		eC_NUMBER_4		,
		eC_NUMBER_5		,
		eC_NUMBER_6		,
		eC_NUMBER_7		,
		eC_NUMBER_8		,
		eC_NUMBER_9		,
		eC_MINUS		,
		eC_COMMA		,
		eC_CLOSE		,		// Cancel 이 같은 기능인거 같음?
		eC_END,
	};
	DECLARE_DYNCREATE(CNumericKeypadWnd)
public:
	CNumericKeypadWnd(CWnd* pwndParent = nullptr);
	virtual ~CNumericKeypadWnd();
protected:
	int		m_eSelectControl;
	CRect	m_rectClient
		, m_rectS[eS_END]
		, m_rectC[eC_END];

	CString m_strBefore;	// 기존 값
	CString m_strAfter;		// 변경 예정 값
	CString m_strDefault;	// 기본 값

	BOOL	m_bDouble;		// TURE 인 경우 Double / FALSE 인 경우 Int

	//BOOL	m_bApply;		// 적용 여부
	//BOOL	m_bCancel;		// 취소 여부
	double* m_dValue;		// Dobule 형 데이터 
	int*	m_iValue;		// Int 형 데이터 

	std::function<void()> m_funcListUpdate = nullptr;
protected:
	void ComputArea();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_IP()
	//ip_handler BOOL		OnIPShowKeypad(CIPCommand& cmd);

	
	void SetDataValue(double* _v, std::function<void()> _func = nullptr)
	{
		m_dValue = _v;
		m_funcListUpdate = _func;
		m_bDouble = TRUE;
	}
	void SetDataValue(int* _v, std::function<void()> _func = nullptr)
	{
		m_iValue = _v;
		m_funcListUpdate = _func;
		m_bDouble = FALSE;
	}

};



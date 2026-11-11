#pragma once




class CMCCWnd : public CFrameWndEx, public IWNDIPCommandTarget
{
static constexpr int TACT_TIME_COUNT = 30;

enum eSTATIC 
{
	eS_NONE			= -1,
	eS_FULL_AREA	= 0,
	eS_TITLE_AREA	,
	eS_MCC_AREA		,
	eS_END			,
};

enum eLI 
{
	eLI_NONE		,
	eLI_TITLE		,
	eLI_ALL_AVG		,		// 전체 평균
	eLI_AVG			,		// 상하위 10% 로 제외하고 평균
	eLI_TACT_TIME	,
	eLI_END	= eLI_TACT_TIME + TACT_TIME_COUNT,
};

DECLARE_DYNCREATE(CMCCWnd)

public:
	CMCCWnd										(CWnd*	pwndParent = nullptr);
	virtual				~CMCCWnd				();
protected:
	CRect				m_rectS[eS_END];

	// TactTime 정보가 들어가있는 컨테이너
	std::vector<std::list<__uint64>>			m_tactTimes;

	CListCtrlColumnEx	m_lstMCC;

protected:
	void				ComputeArea();

public:
	DECLARE_IP_AND_INTERFACE()
	ip_handler BOOL OnIP_AddTactTime(CIPCommand& cmd);

public:
	DECLARE_MESSAGE_MAP()
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct			);
	afx_msg void		OnShowWindow			(BOOL bShow, UINT nStatus				);
	afx_msg BOOL		OnEraseBkgnd			(CDC* pDC								);
	afx_msg void		OnPaint					();
};

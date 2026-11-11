#pragma once
#define TIMER_LOGOUT 9090
#define TIMER_CHECK_PM	9091
class CPMWnd;
class CIPCommandWnd;
class CCIMIPCommandWnd;
class CUserManagerWnd;
class CMainFrame : public CFrameWndEx {

enum eSTATIC {
	eS_NONE			= -1,
	eS_FULL_AREA	,
	eS_END			,
};
enum eCONTROL {
	eC_NONE			= -1,
	eC_TITLE		,
	eC_END			,
};

	DECLARE_DYNCREATE(CMainFrame)	
public:
	CMainFrame() noexcept;
	virtual					~CMainFrame					();
#ifdef _DEBUG
	virtual void			AssertValid					() const;
	virtual void			Dump						(CDumpContext& dc) const;
#endif
	virtual BOOL			PreCreateWindow				(CREATESTRUCT& cs);
	virtual BOOL			LoadFrame					(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);
protected:
	afx_msg int				OnCreate					(LPCREATESTRUCT lpCreateStruct			);
	afx_msg void			OnTimer						(UINT_PTR nIDEvent						);
	afx_msg void			OnShowWindow				(BOOL bShow, UINT nStatus				);
	afx_msg void			OnKillFocus					(CWnd* pNewWnd							);
	afx_msg void			OnSetFocus					(CWnd *pOldWnd							);	
	afx_msg BOOL			OnEraseBkgnd				(CDC* pDC								);
	afx_msg void			OnPaint						();
	afx_msg void			OnClose						();

	DECLARE_MESSAGE_MAP		()

	BOOL					fn_Initialize();
	void					fn_ComputeArea();

	BOOL					m_bPMMode;

public:
	void					SetView(int eMainMenu = eMAIN_MENU_NONE, int eSubMenu = eSUB_MENU_NONE, eUSER_GRADE eUser = eUSER_SUPER);

protected:
	int						m_eIitializeStep			;

	CRect					m_rectStatic				[eS_END],
							m_rectControl				[eC_END];

	HWND					m_hwndWorkArea				;		
	HWND					m_hwndSubMenu				;
	//. 큰 틀
	CFrameWndEx*			m_pwndTitle					;
	CFrameWndEx*			m_pwndMainMenu				;
	CFrameWndEx*			m_pwndSubMenu				;
	CFrameWndEx*			m_pwndExit					;
	CFrameWndEx*			m_pwndLogIn;

	//. MAIN MENU 1
	CFrameWndEx*			m_pwndMainStatus			;

	//. MAIN MENU 2 수동모드
	CFrameWndEx*			m_pwndTransferThetaMove		;	//. 위치 임시 지정.

	CFrameWndEx*			m_pwndIOControl;
	CFrameWndEx*			m_pwndTeachModeConveyorWnd;
	CFrameWndEx*			m_pwndPPBOXCodeReader;
	CFrameWndEx*			m_pwndTeachMoveWnd;
	CFrameWndEx*			m_pwndAGVPIOStatus;
	CFrameWndEx*			m_pwndConveyorMonitor;
	CFrameWndEx*			m_pwndCIMTester;


	//. MAIN MENU 3
	CFrameWndEx*			m_pwndRecipeSelect			;
	CFrameWndEx*			m_pwndRecipeEdit			;
	CFrameWndEx*			m_pwndRecipeImageSet		;

	//. MAIN MENU 4 
	CFrameWndEx*			m_pwndPPBox;

	//. MAIN MENU 5
	CFrameWndEx*			m_pwndMCMonitor				;
	CFrameWndEx*			m_pwndDustCollector			;
	CFrameWndEx*			m_pwndIOStatus				;
	//CFrameWndEx*			m_pwndAirPressure			;
	CFrameWndEx*			m_pwndMCC					;
	CFrameWndEx*			m_pwndRegisterViewer		;
	CFrameWndEx*			m_pwndGlassEmissions		;

	//. MAIN MENU 6
	CFrameWndEx*			m_pwndDeviceParameter		;
	CFrameWndEx*			m_pwndPositionParameter		;
	CFrameWndEx*			m_pwndMotorParameter		;
	CFrameWndEx*			m_pwndConveyorParameter		;

	//. MAIN MENU 7
	
	CFrameWndEx*			m_pwndAlarmLog				;
	CFrameWndEx*			m_pwndParameterLog			;
	CFrameWndEx*			m_pwndRecipeLog				;
	CFrameWndEx*			m_pwndButtonLog				;
	CFrameWndEx*			m_pwndSequence				;

	//. MAIN MENU 8
	
	//. MAIN MENU 9
	CPMWnd*					m_pwndPMMode				;
	//. MAIN MENU 10
	CUserManagerWnd*		m_pwndUserManager			;
	CFrameWndEx*			m_pwndTerminalMessage		;
	CFrameWndEx*			m_pwndInterlockMessage		;
	CFrameWndEx*			m_pwndOPCallMessage			;
	// CIP Command
	CWnd*			m_pwndIPCommand;
	CWnd*			m_pwndCIMIPCommand;
public:
	CFrameWndEx* getTitleWnd() { return m_pwndTitle; };
	CFrameWndEx* getMainStatusWnd() { return m_pwndMainStatus; };

	void LogOut();
	void LogIn();
    void ClosePopupUI();    // KES 추가 2024.2.14
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ChagneLanguage(); // LJY 추가 2024.06.23
};



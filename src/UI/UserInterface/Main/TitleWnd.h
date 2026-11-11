#pragma once


class CDrawControlTestWnd;
class CDemoWnd;
class CPassWordWnd;
class CAutoRunCheckerWnd;
class CTPMCodeSelectWnd;
class CMessageHistoryWnd;
class CTitleWnd : public CFrameWndEx {
	enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				,
	eS_AREA_EQP_STATE			,
	eS_AREA_DEFAULT_STATUS		,
	eS_AREA_RECIPE_INFO			,
	eS_AREA_OP_DRIVE_MODE_STATUS,
	eS_AREA_CONTROL_BTN_1		,
	eS_AREA_CONTROL_BTN_2		,

	eS_ALARM					,
	
	eS_AVAILABILITY				,
	eS_INTERLOCK				,
	eS_MOVE						,
	eS_RUN						,
	
	eS_AVAILABILITY_UP			,
	eS_AVAILABILITY_DOWN		,

	eS_INTERLOCK_OFF			,
	eS_INTERLOCK_ON				,

	eS_MOVE_RUNNING				,
	eS_MOVE_PAUSE				,

	eS_RUN_RUN					,
	eS_RUN_IDLE					,

	eS_CONNECT_STATUS			,
	eS_TIME						,
	eS_NAME						,

	eS_PPID_1					,
	eS_PPID_2					,
	eS_RECIPEID_1				,
	eS_RECIPEID_2				,
	
	eS_OPID_1					,
	eS_OPID_2					,
	eS_SELECT					,
	eS_END						,
};
enum eCOMMAND {
	eC_NONE						= -1,
	eC_OP_CHANGE				,
	eC_LANGUAGE_CHANGE			,
	eC_CMD						,
	eC_OFF_LINE					,
	eC_SERVO_OFF,
	eC_SERVO_ON,
	eC_RUN,
	eC_STOP,
	eC_IN,
	eC_OUT,
	eC_INIT,
	eC_INLINE,
	eC_PM_MODE,
	eC_BUZZER,
	eC_RESET,
	eC_AGB_USE,
	eC_MESSAGE_HISTORY			,
	eC_SERIAL					,	// KES 추가 2024.3.20
	eC_AXIS						,
	eC_DEMO						,
	eC_LOG_DELETE				,
	eC_KEYPAD					,

	eC_MANUAL_AUTO				,
	eC_TEACH					,
	eC_REVERSE					,
	eC_CIM_STATUS				,
	eC_END						,
};

enum eIMAGE_BUTTON {
	eIMAGE_BUTTON_SERVO_OFF	,
	eIMAGE_BUTTON_SERVO_ON	,
	eIMAGE_BUTTON_RUN		,
	eIMAGE_BUTTON_STOP		,
	eIMAGE_BUTTON_IN		,
	eIMAGE_BUTTON_OUT		,
	eIMAGE_BUTTON_INIT		,
	eIMAGE_BUTTON_BUZZER	,
	eIMAGE_BUTTON_RESET		,
	eIMAGE_BUTTON_END		,
};

// LJY 추가 2024.3.20
enum eFLAG {
	eIMAGE_FLAG_KOREA,
	eIMAGE_FLAG_AMERICA,
	eIMAGE_FLAG_VIETNAM,
	eIMAGE_FLAG_END,
};

	DECLARE_DYNCREATE(CTitleWnd)
public:
	CTitleWnd									(CWnd* pwndParent=nullptr);
	virtual				~CTitleWnd				();
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);
protected:
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct			);
	afx_msg void		OnKillFocus				(CWnd* pNewWnd							);
	afx_msg void		OnShowWindow			(BOOL bShow, UINT nStatus				);
	afx_msg void		OnTimer					(UINT_PTR nIDEvent						);
	afx_msg BOOL		OnEraseBkgnd			(CDC* pDC								);
	afx_msg void		OnPaint					();
	afx_msg void		OnMouseMove				(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonDown			(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonUp				(UINT nFlags, CPoint point				);
	DECLARE_MESSAGE_MAP()

protected:
	CWnd*				m_pwndParent			;
	CRect				m_rectS					[eS_END], 
						m_rectC					[eC_END];
	int					m_eSelectControl		;
	int					m_eSelectedItem			;
	int					m_eMouseOver			;
public:
	CAutoRunCheckerWnd* m_pwndAutoRunChecker	;
	CString				m_strModelName;
	//CImage				m_img					[eIMAGE_END];
	Gdiplus::Image*		m_imgButton[eIMAGE_BUTTON_END] = {nullptr, };
	Gdiplus::Image*		m_imgFlag[eIMAGE_FLAG_END];		// LJY 추가 2024.3.20
	CDrawControlTestWnd* m_pwndDrawControl;
	CDemoWnd*			m_pwndDemo;
	CPassWordWnd*		m_pwndPassword;
	CMessageHistoryWnd* m_pwndMessageHistory;

	CTPMCodeSelectWnd*	m_pwndTPMCodeSelect;

	__uint64			m_clickStartTime;
	__uint64			m_blinkStartTime;
	// Servo On/Off 상태 
	BOOL				m_bServoOn;				// 전부 Servo On 인 경우 TRUE
	BOOL				m_bServoOff;			// 전부 Servo Off 인 경우 TRUE

	// KES 추가 2024.4.9
	BOOL				m_bFirstDoorKeyChange = TRUE;
protected:
	void				fn_ComputeArea			();
public:
	BOOL				Initialize				() { return TRUE; };
	CDemoWnd*			GetDemoWnd()	{ return m_pwndDemo; };
    void                ClosePopupUI(); // KES 추가 2024.2.15
	void				DrawButtonImage(CDC* pDC, CRect rect, Gdiplus::Image* img, BOOL bDown, int iTick = 3);	// LJY 추가 2024.3.20
    void				SetLanguage(eLANGUAGE eLanguage);	 // LJY 추가 2024.3.21
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


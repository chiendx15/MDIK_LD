#pragma once


#define ID_MC_MONITOR 9999

class CMCMonitorWnd : public CFrameWndEx {

enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	
	eS_LIST_AREA,
	eS_STATUS_1,
	eS_STATUS_1_00,
	eS_STATUS_1_0C,
	eS_STATUS_1_09,
	
	eS_STATUS_2,
	eS_SERVO_DRIVER,
	eS_SERVO_DRIVER_2C,
	eS_SERVO_DRIVER_2E,
	eS_SERVO_DRIVER_ETC,
	eS_SERVO_ALARM,
	eS_SERVO_ALARM_H,
	eS_SERVO_ALARM_L,

	eS_STATUS_2_IWxx01,
	eS_STATUS_2_IWxx08,
	eS_STATUS_2_ILxx0E,
	eS_STATUS_2_ILxx10,
	eS_STATUS_2_ILxx12,
	eS_STATUS_2_ILxx16,
	eS_STATUS_2_ILxx18,
	eS_STATUS_2_ILxx1A,
	eS_STATUS_2_ILxx1C,
	eS_STATUS_2_ILxx1E,
	eS_STATUS_2_IWxx20,

	eS_DRIVER_IWxx2D,
	eS_DRIVER_IWxx3F,
	eS_DRIVER_ILxx40,
	eS_DRIVER_ILxx42,
	eS_END						,
};

enum eCOMMAND {
	eC_NONE						= -1,
	eC_OK						,
	eC_CANCEL					,
	eC_STATUS_1_IWxx00_0,
	eC_STATUS_1_IWxx00_1,
	eC_STATUS_1_IWxx00_2,
	eC_STATUS_1_IWxx00_3,

	eC_STATUS_1_IWxx09_0,
	eC_STATUS_1_IWxx09_1,
	eC_STATUS_1_IWxx09_3,
	eC_STATUS_1_IWxx09_8,

	eC_STATUS_1_IWxx0C_0,
	eC_STATUS_1_IWxx0C_1,
	eC_STATUS_1_IWxx0C_2,
	eC_STATUS_1_IWxx0C_3,
	eC_STATUS_1_IWxx0C_4,
	eC_STATUS_1_IWxx0C_5,
	eC_STATUS_1_IWxx0C_6,
	eC_STATUS_1_IWxx0C_8,
	eC_STATUS_1_IWxx0C_9,

	eC_STATUS_2_IWxx01,
	eC_STATUS_2_IWxx08,
	eC_STATUS_2_ILxx0E,
	eC_STATUS_2_ILxx10,
	eC_STATUS_2_ILxx12,
	eC_STATUS_2_ILxx16,
	eC_STATUS_2_ILxx18,
	eC_STATUS_2_ILxx1A,
	eC_STATUS_2_ILxx1C,
	eC_STATUS_2_ILxx1E,
	eC_STATUS_2_ILxx20,

	eC_DRIVER_xx2C_0,
	eC_DRIVER_xx2C_1,
	eC_DRIVER_xx2C_2,
	eC_DRIVER_xx2C_3,
	eC_DRIVER_xx2C_4,
	eC_DRIVER_xx2C_5,
	eC_DRIVER_xx2C_6,
	eC_DRIVER_xx2C_7,
	eC_DRIVER_xx2C_8,
	eC_DRIVER_xx2C_9,
	eC_DRIVER_xx2C_A,
	eC_DRIVER_xx2C_B,
	eC_DRIVER_xx2C_C,
	eC_DRIVER_xx2C_D,
	eC_DRIVER_xx2C_P_OT,
	eC_DRIVER_xx2C_N_OT,
	eC_DRIVER_xx2C_HOME,

	eC_DRIVER_xx2E_0,
	eC_DRIVER_xx2E_1,
	eC_DRIVER_xx2E_2,
	eC_DRIVER_xx2E_3,
	eC_DRIVER_xx2E_4,
	eC_DRIVER_xx2E_5,
	eC_DRIVER_xx2E_6,
	eC_DRIVER_xx2E_7,
	eC_DRIVER_xx2E_8,
	eC_DRIVER_xx2E_9,
	eC_DRIVER_xx2E_A,
	eC_DRIVER_xx2E_B,
	eC_DRIVER_xx2E_C,
	eC_DRIVER_xx2E_D,

	eC_DRIVER_IWxx2D,
	eC_DRIVER_IWxx3F,
	eC_DRIVER_ILxx40,
	eC_DRIVER_ILxx42,

	eC_ALARM_xx04_0,
	eC_ALARM_xx04_1,
	eC_ALARM_xx04_2,
	eC_ALARM_xx04_3,
	eC_ALARM_xx04_4,
	eC_ALARM_xx04_5,
	eC_ALARM_xx04_6,
	eC_ALARM_xx04_7,
	eC_ALARM_xx04_8,
	eC_ALARM_xx04_9,
	eC_ALARM_xx04_A,
	eC_ALARM_xx04_B,
	eC_ALARM_xx04_D,
	eC_ALARM_xx04_F,
	eC_ALARM_xx04_10,
	eC_ALARM_xx04_11,
	eC_ALARM_xx04_12,
	eC_ALARM_xx04_13,
	eC_ALARM_xx04_1D,
	eC_ALARM_xx04_1E,
	eC_ALARM_xx04_1F,

	eC_ALARM_xx02_0,
	eC_ALARM_xx02_1,
	eC_ALARM_xx02_2,
	eC_ALARM_xx02_3,
	eC_ALARM_xx02_4,
	eC_ALARM_xx02_6,
	eC_ALARM_xx02_7,
	eC_ALARM_xx02_8,
	eC_ALARM_xx02_9,
	eC_ALARM_xx02_A,
	eC_GANTRY_CH_1	,
	eC_GANTRY_CH_2	,
	eC_END						,
};
DECLARE_DYNCREATE(CMCMonitorWnd)
public:
	CMCMonitorWnd								(CWnd*	pwndParent = nullptr);
	virtual				~CMCMonitorWnd			();
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct			);
	afx_msg void		OnShowWindow			(BOOL bShow, UINT nStatus				);
	afx_msg void		OnTimer					(UINT_PTR nIDEvent						);
	afx_msg BOOL		OnEraseBkgnd			(CDC* pDC								);
	afx_msg void		OnPaint					();
	afx_msg void		OnMouseMove				(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonDown			(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonUp				(UINT nFlags, CPoint point				);
	afx_msg void		OnItemchangedLst		(NMHDR* pNMHDR, LRESULT* pResult);
protected:
	CWnd*				m_pwndParent;
public:
	void ChangeLanguage(); // LJY 추가 2024.06.23
protected:
	CRect				m_rectS			[eS_END], 
						m_rectC			[eC_END];
	int					m_eSelectControl		;
	int					m_eMouseOver, m_eSelectAxis	;
	CListCtrlColumnEx	m_listAxis;
	BOOL				m_bGantry = TRUE;			// TRUE : Ch1 / FALSE : Ch2
public:	

protected:
	void				fn_ComputeArea			();
public:
	BOOL				Initialize				() { return TRUE; };
};

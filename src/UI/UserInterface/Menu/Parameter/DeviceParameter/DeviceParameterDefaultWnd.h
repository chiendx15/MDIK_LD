#pragma once

class CDeviceParameterDefaultWnd : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_DEVICE_LIST_AREA			,
	eS_DEVICE_DISTACNE_PARAMETER		,	// 거리 / Offset 등 
	eS_DEVICE_TIME_PARAMETER			,	// 시간
	eS_DEVICE_COUNT_PERCENT_PARAMETER	,	// 횟수, 퍼센트 
	eS_DEVICE_USE_PARAMETER				,	// 사용 유무

	// Distance 
	eS_LD_BEFORE_DISTANCE		            ,	
	eS_LD_BEFORE_SPEED		                ,	
	eS_LD_AFTER_DISTANCE		            ,
    eS_LD_AFTER_SPEED                       ,

    eS_ULD_BEFORE_DISTANCE		            ,	
	eS_ULD_BEFORE_SPEED		                ,	
	eS_ULD_AFTER_DISTANCE		            ,
    eS_ULD_AFTER_SPEED                      ,	

    eS_TRANSFET_PAPER_CYLINDER_DOWN_SIZE    ,

	eS_DISTANCE_UNIT_1						,
	eS_DISTANCE_UNIT_2						,
	eS_DISTANCE_UNIT_3						,
	eS_DISTANCE_UNIT_4						,
	eS_DISTANCE_UNIT_5						,
	eS_DISTANCE_UNIT_6						,
	eS_DISTANCE_UNIT_7						,
    eS_DISTANCE_UNIT_8						,
    eS_DISTANCE_UNIT_9						,
	// Time
	
	eS_CONVEYOR_H							,
	eS_CONVEYOR_L							,
	eS_CONVEYOR_STOPER						,
	eS_CURTAIN								,
	eS_CLEANER								,
	eS_AIR_SHOWER_DOOR						,
	eS_CONVEYOR_MOVE						,
    eS_LOGOUT_TIME						    ,   // KES 추가 2024.2.14
	eS_ALIGN_R_DELAY_TIME					,   // LJY 추가 2024.4.29

	eS_TIME_UNIT_1							,
	eS_TIME_UNIT_2							,
	eS_TIME_UNIT_3							,
	eS_TIME_UNIT_4							,
	eS_TIME_UNIT_5							,
	eS_TIME_UNIT_6							,
	eS_TIME_UNIT_7							,
	eS_TIME_UNIT_8							,
	eS_TIME_UNIT_9							,

	// Count 
	eS_GLASS_VACUUM_RETRY					,
	eS_PAPER_VACUUM_RETRY					,
	eS_ALIGN_RETRY_COUNT					,
	
	eS_COUNT_UNIT_1							,
	eS_COUNT_UNIT_2							,
	eS_COUNT_UNIT_3							,
	
	// Use									
	eS_DRY_RUN			,
	eS_SWITCH_USE	,	// LYJ 2024.05.01 - Dry Run중 Switch 미사용 모드 버튼 추가
	eS_GANZI_ALARM_SKIP	,
	eS_PC_LINKER		,
	eS_END				,
};

enum eCOMMAND {
	eC_NONE						= -1,
	eC_EDIT						,
	eC_SAVE						,
	// Distance 
	eC_LD_BEFORE_DISTANCE		,	
	eC_LD_BEFORE_SPEED		    ,	
	eC_LD_AFTER_DISTANCE		,
    eC_LD_AFTER_SPEED           ,

    eC_ULD_BEFORE_DISTANCE		,	
	eC_ULD_BEFORE_SPEED		    ,	
	eC_ULD_AFTER_DISTANCE		,
    eC_ULD_AFTER_SPEED          ,	

    eC_TRANSFET_PAPER_CYLINDER_DOWN_SIZE,

	// Time
	eC_CONVEYOR_H					,
	eC_CONVEYOR_L					,
	eC_CONVEYOR_STOPER				,
	eC_CURTAIN						,
	eC_CLEANER						,
	eC_AIR_SHOWER_DOOR				,		
	eC_CONVEYOR_MOVE				,
    eC_LOGOUT_TIME  				,   // KES 추가 2024.2.14
	eC_ALIGN_R_DELAY_TIME			,	// LJY 추가 2024.4.29

	// Count 
	eC_GLASS_VACUUM_RETRY			,
	eC_PAPER_VACUUM_RETRY			,
	eC_ALIGN_RETRY_COUNT			,

	// Use			
	// 2021.12.06 공운전 기능 삭제 (숨김)
	eC_DRY_RUN						,						
	eC_SWITCH_USE				,	// LYJ 2024.05.01 - Dry Run중 Switch 미사용 모드 버튼 추가
	eC_GANZI_ALARM_SKIP				,
	eC_PC_LINKER					,
	eC_END							,
};
enum eDETAIL {
	eDETAIL_POS				,
	eDETAIL_TIME			,
	eDETAIL_COUNT_PERCENT	,
	eDETAIL_USE				,
	eDETAIL_END				,
};
DECLARE_DYNCREATE(CDeviceParameterDefaultWnd)
public:
	CDeviceParameterDefaultWnd							(CWnd*	pwndParent = nullptr);
	virtual				~CDeviceParameterDefaultWnd	();
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct			);
	afx_msg void		OnKillFocus				(CWnd* pNewWnd							);
	afx_msg void		OnShowWindow			(BOOL bShow, UINT nStatus				);
	afx_msg void		OnTimer					(UINT_PTR nIDEvent						);
	afx_msg BOOL		OnEraseBkgnd			(CDC* pDC								);
	afx_msg void		OnPaint					();
	afx_msg void		OnMouseMove				(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonDown			(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonUp				(UINT nFlags, CPoint point				);
protected:
	CWnd*				m_pwndParent;
public:
protected:
	CRect				m_rectS					[eS_END], 
						m_rectC					[eC_END];
	int					m_eSelectControl		;
	int					m_eMouseOver			;
	CString				strALogMsg, strALogPos;
	BOOL				m_bLock;						// TRUE : 잠금(Lock) ,FASLE : 편집

	CString				m_strUnit[eDETAIL_END] = { _T("mm"), _T("sec"), _T("회"), _T("회") };
	
	// 공유메모리에 Save/Load 시킬 구조체 선언 
	T_DEVICE_PARAMETER_DISTANCE	m_tDistance;
	T_DEVICE_PARAMETER_TIME		m_tTime;
	T_DEVICE_PARAMETER_COUNT	m_tCount;
	T_DEVICE_PARAMETER_USE		m_tUse;
public:	

protected:
	void				fn_ComputeArea			();
public:
	BOOL				Initialize				() { return TRUE; };
	BOOL				SyncData				(BOOL bStore);
	void				SetListItem(eDETAIL eDetail = eDETAIL_POS);
};

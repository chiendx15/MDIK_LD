#pragma once
#include "Simulator/Simulator.h"
class CModuleStatusWnd;
class CMainStatusWnd : public CFrameWndEx { 
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_ITEM_AREA				,
	eS_WORK_AREA				,
	eS_IMG_AREA					,
	eS_MODULE_STATUS_LOAD_GLASS_1_CONVEYOR		,	
	eS_MODULE_STATUS_LOAD_GLASS_2_CONVEYOR		,	
	eS_MODULE_STATUS_ELEVATOR_GLASS_CONVEYOR	,	
	eS_MODULE_STATUS_UNLOAD_GLASS_1_CONVEYOR	,	
	eS_MODULE_STATUS_UNLOAD_GLASS_2_CONVEYOR	,	
	eS_MODULE_STATUS_GLASS_LOAD_TRANSFER		,	
	//eS_GLASS_IMG				,
	eS_END,
};

enum eCOMMAND {
	eC_NONE						= -1,
	eC_GLASS_CONVEYOR_MUTE		,
	eC_PAPER_CONVEYOR_MUTE		,
	eC_END						,
};
DECLARE_DYNCREATE(CMainStatusWnd)
public:
	CMainStatusWnd								(CWnd*	pwndParent = nullptr);
	virtual				~CMainStatusWnd			();
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
	CRect				m_rectS			[eS_END], 
						m_rectC			[eC_END];
	int					m_eSelectControl		;
	int					m_eMouseOver			;
	CString				strALogMsg, strALogPos;
	CImage				m_imgMain;
	CSimulator*			m_pSimulatorView;

	CModuleStatusWnd*	m_ModuleStatus[eMODULE_PC1_END - eMODULE_PC1_BEGIN] ;

public:	
	void				fn_ComputeArea			();
public:
	void				SetSimulatorView		();
	BOOL				Initialize				() { return TRUE; };
    void                CloseAllScrapWnd        (); 
};

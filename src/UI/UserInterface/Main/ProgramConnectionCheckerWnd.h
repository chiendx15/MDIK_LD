#pragma once

class CProgramConnectionCheckerWnd : public CFrameWndEx { 

	enum eSTATIC {
		eS_NONE = -1
		, eS_FULL_AREA
		, eS_CONNECT_AREA
		, eS_END
	};

	enum eCOMMAND {
		eC_NONE	= -1
		, eC_CONNECT_UI
		, eC_CONNECT_PLCLINKER
		, eC_CONNECT_SEQUENCE
		, eC_CONNECT_DATATRANSFER
		, eC_CONNECT_ALARM
		, eC_CONNECT_VISION
		, eC_CONNECT_SERIAL //. 21.01.04. 현재는 비교하지 않는다. 추후에 추가.
		, eC_END						
	};

	enum ePROGRAM_LIST {
		ePROGRAM_UI
		, ePROGRAM_PLCLINKER
		, ePROGRAM_SEQUENCE
		, ePROGRAM_DATATRANSFER
		, ePROGRAM_ALARM
		, ePROGRAM_VISION
		, ePROGRAM_SERIAL		//. 21.01.04. 현재는 비교하지 않는다. 추후에 추가
		, nPROGRAM_LIST
	};

DECLARE_DYNCREATE(CProgramConnectionCheckerWnd)
public:
	CProgramConnectionCheckerWnd								(CWnd*	pwndParent = nullptr);
	virtual				~CProgramConnectionCheckerWnd			();
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);

DECLARE_MESSAGE_MAP()
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct			);
	afx_msg void		OnKillFocus				(CWnd* pNewWnd							);
	afx_msg void		OnShowWindow			(BOOL bShow, UINT nStatus				);
	afx_msg void		OnTimer					(UINT_PTR nIDEvent						);
	afx_msg BOOL		OnEraseBkgnd			(CDC* pDC								);
	afx_msg void		OnPaint					();
	afx_msg void		OnMouseMove				(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonDown			(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonUp				(UINT nFlags, CPoint point				);

	void				fn_ComputeArea();

	CWnd*				m_pwndParent;
	CRect				m_rectS			[eS_END], 
						m_rectC			[eC_END];
	int					m_eSelectControl		;
	int					m_eMouseOver			;

public:	
	BOOL				GetProcessConnected		(ePROGRAM_LIST eProcess);

	/// <summary>
	/// Mocha에 연결된 프로세스 목록을 탐색하여 enum값에 해당하는 process string의 index 를 가져온다.
	/// </summary>
	/// <param name="eProcess">찾을 process enum 값</param>
	/// <returns>목록에 없으면 -1. </returns>
	int					SearchMochaAttachedProcessIndex(ePROGRAM_LIST eProcess);

	/// <summary>
	/// AutoRun Check시 필요.
	/// </summary>
	/// <returns>모든 프로그램이 연결되어있으면 TRUE</returns>
	BOOL				IsProcessAllConncted();
};

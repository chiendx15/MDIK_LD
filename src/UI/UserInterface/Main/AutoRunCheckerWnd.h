#pragma once

class CTitleWnd;
class CProgramConnectionCheckerWnd;
class CAutoRunCheckerWnd : public TIPCT<CFrameWndEx> {

	enum eSTATIC {
		eS_NONE = -1
		, eS_FULL_AREA
		, eS_RUN_OPTION_AREA			// Auto Run 옵션
		, eS_RUN_CHECK_LIST_AREA		// 제어 PC 내부 체크 리스트
		, eS_RUN_CONTROL_PC_RECIPE_AREA	// 모든 제어 PC Recipe 번호 체크
		, eS_RUN_PROGRAM_CHECK_AREA		// 프로세스 연결 상태
		, eS_RUN_CONTROL_AREA			// 컨트롤
		, eS_END
	};
	enum eCOMMAND {
		eC_NONE = -1
		
		, eC_SEQUENCE_INITIALIZE
		, eC_CONVEYOR_INITIALIZE
		, eC_GLASS_TRANSFER_INITIALIZE

		, eC_CHECK_LIST_01
		, eC_CHECK_LIST_02
		, eC_CHECK_LIST_03
		, eC_CHECK_LIST_04
		, eC_CHECK_LIST_05
		, eC_CHECK_LIST_06
		, eC_CHECK_LIST_07
		, eC_CHECK_LIST_08
		, eC_CHECK_LIST_09
		, eC_CHECK_LIST_10
		, eC_CHECK_LIST_11
		, eC_CHECK_LIST_12
		, eC_CHECK_LIST_13
		, eC_CHECK_LIST_14
		
		, eC_LD_RECIPE_INDEX
		, eC_SCT_RECIPE_INDEX
		, eC_CCTA_RECIPE_INDEX
		, eC_CCTB_RECIPE_INDEX
		, eC_ULDA_RECIPE_INDEX
		, eC_ULDB_RECIPE_INDEX

		, eC_LD_RECIPE_PPID
		, eC_SCT_RECIPE_PPID
		, eC_CCTA_RECIPE_PPID
		, eC_CCTB_RECIPE_PPID
		, eC_ULDA_RECIPE_PPID
		, eC_ULDB_RECIPE_PPID

		, eC_RE_CHECK
		, eC_START
		, eC_CLOSE
		, eC_END
	};

	enum eCHECK_LIST {
		eCHECK_PROCESS,
		eCHECK_RECIPE,
		eCHECK_DRIVE_MODE_AUTO,
		eCHECK_MC_ON,
		eCHECK_DOOR_CLOSE,
		eCHECK_NOT_FOUND_HEAVY_ALARM,
		eCHECK_OTHER_EQP_NOT_FOUND_HEAVY_ALARM,
		eCHECK_MOTOR_SERVO_ON,
		eCHECK_NOT_FOUND_SERVO_ALARM,
		eCHECK_SERVO_INIT,
		eCHECK_CONVEYOR_READY,
		eCHECK_CONTROL_PC_RECIPE,
		eCHECK_INTERLOCK_MESSAGE,
		eCHECK_EMPTY_PPBOX, // Elevator에 Empty PPBox가 있어야 함.
		nCHECK,
	};

	DECLARE_DYNCREATE(CAutoRunCheckerWnd)
public:
	CAutoRunCheckerWnd									(CWnd* pwndParent=nullptr);
	virtual				~CAutoRunCheckerWnd				();
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);

	DECLARE_MESSAGE_MAP()
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct			);
	afx_msg void		OnShowWindow			(BOOL bShow, UINT nStatus				);
	afx_msg void		OnTimer					(UINT_PTR nIDEvent						);
	afx_msg BOOL		OnEraseBkgnd			(CDC* pDC								);
	afx_msg void		OnPaint					();
	afx_msg void		OnMouseMove				(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonDown			(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonUp				(UINT nFlags, CPoint point				);

	void				fn_ComputeArea			();

public:
	DECLARE_IP()
	ip_handler BOOL OnIPAutoRunCheck(CIPCommand& cmd);

protected:
	CWnd*				m_pwndParent			;
	CRect				m_rectS					[eS_END], 
						m_rectC					[eC_END];

	CRect               m_rectShowWindowPos;
	int					m_eSelectedControl		;
	int					m_eSelectedItem			;
	int					m_eMouseOver			;

	CProgramConnectionCheckerWnd* m_pwndProgramChecker;
	////. AutoRun 하기 전, 초기화 동작 진행 여부.
	BOOL				m_bOptionSequenceInit		;
	BOOL				m_bConveyorSequenceInit		;
	BOOL				m_bGlassTransferSequenceInit		;

	BOOL				m_bCheck[nCHECK];

	int					m_recipeIndex[nMACHINE_END];
	CString				m_recipePPID[nMACHINE_END];
};


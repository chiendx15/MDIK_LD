#pragma once

#define CHECK_BOX_CODE_BEIGN 101


class CTPMCodeSelectWnd : public CFrameWndEx {

	enum eSTATIC {
		eS_NONE = -1,
		
		eS_FULL_AREA,
		
		eS_CODE_LIST_AREA,
			eS_TITLE_EQP_STATE,
			eS_TITLE_TP_INDEX,
			eS_TITLE_DESCRIPTION,
			eS_TITLE_CODE,

			eS_EQP_STATE_DOWN,

			eS_TP_INDEX_BM,
			eS_TP_INDEX_PM,
			eS_TP_INDEX_CM,
			eS_TP_INDEX_CHANGE_MODEL,
			eS_TP_INDEX_MATERIAL,

			eS_DESCRIPTION_BREAKDOWN_MANUAL,
			eS_DESCRIPTION_REGULAR_PM,
			eS_DESCRIPTION_REGULAR_PM_TITLE,
			eS_DESCRIPTION_CHECK_EQUIPMENT,
			eS_DESCRIPTION_CHECK_QUALITY,

			eS_DESCRIPTION_IMPROVE_EQUIPMENT_EE,
			eS_DESCRIPTION_IMPROVE_PROCESSING_EL,
			eS_DESCRIPTION_SETUP_NEW_PRODUCT,
			eS_DESCRIPTION_CHANGE_SAME_MODEL,
			eS_DESCRIPTION_CHANGE_DIFFERENT_MODEL,
			eS_DESCRIPTION_CHANGE_MATERIAL,
			eS_DESCRIPTION_DOWN_MATERIAL,
			
		eS_END,
	};
	enum eCOMMAND {
		eC_NONE = -1,
		
		//<< 순서 0번째에서 바꾸면 안됨
		eC_CODE_BM,
		eC_PM_REGULAR_PM,
		eC_PM_IRREGULAR_PM_CHECK_EQUIPMENT,
		eC_PM_IRREGULAR_PM_CHECK_QUALITY,
		eC_CM_IMPROVE_EQUIPMENT_EE,
		eC_CM_IMPROVE_PROCESSING_EL,
		eC_CM_SETUP_NEW_PRODUCT,
		eC_CHANGE_MODEL_CHANGE_SAME_MODEL,
		eC_CHANGE_MODEL_CHANGE_DIFFERENT_MODEL,
		eC_MATERIAL_CHANGE_MATERIAL,
		eC_MATERIAL_DOWN_MATERIAL,
		//>>

		eC_SEND,
		eC_CANCEL,

		eC_END,
	};

	DECLARE_DYNCREATE(CTPMCodeSelectWnd)
public:
	CTPMCodeSelectWnd(CWnd* pwndParent = nullptr);
	virtual				~CTPMCodeSelectWnd();

	DECLARE_MESSAGE_MAP()
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct			);
	afx_msg void		OnShowWindow			(BOOL bShow, UINT nStatus				);
	afx_msg void		OnPaint					();
	afx_msg void		OnMouseMove				(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonDown			(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonUp				(UINT nFlags, CPoint point				);

	void				fn_ComputeArea			();

protected:
	CRect				m_rectS					[eS_END], 
						m_rectC					[eC_END];

	CRect               m_rectShowWindowPos;
	int					m_eSelectedControl		;
	int					m_eMouseOver			;
	int					m_iSelectMode = eC_CODE_BM;

	LPCTSTR Description[11] = {
		_T("BREAKDOWN_MANUAL"),
		_T("REGULAR_PM"),
		_T("CHECK_EQUIPMENT"),
		_T("CHECK_QUALITY"),
		_T("IMPROVE_EQUIPMENT_EE"),
		_T("IMPROVE_PROCESSING_EL"),
		_T("SETUP_NEW_PRODUCT"),
		_T("CHANGE_SAME_MODEL"),
		_T("CHANGE_DIFFERENT_MODEL"),
		_T("CHANGE_MATERIAL"),
		_T("DOWN_MATERIAL"),
	};

	LPCTSTR TPMCode[11] = {
		_T("03000"),
		_T("12000"),
		_T("15100"),
		_T("15200"),
		_T("17200"),
		_T("17300"),
		_T("14000"),
		_T("41100"),
		_T("41200"),
		_T("35000"),
		_T("51000"),
	};
};


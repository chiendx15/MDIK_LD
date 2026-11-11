#pragma once

// CRecipeEditWnd
class CRecipeEditWnd : public CFrameWndEx
{
	// 레시피 데이터 종류
	enum eRECIPE_INFO_TAB
	{
		eRIT_PANNEL_DATA,			
		nRIT,
	};
	

	enum eSTATIC
	{
		eS_NONE = -1,
		eS_TITLE_AREA,
			eS_INDEX,
			eS_NAME,
			eS_PPID,
			eS_DESCRIPTION,
		eS_INFO_AREA,
			eS_INFO_TAB_AREA,
			eS_INFO_DATA_AREA,

		eS_INDIVIDUAL_AREA, // 개별로 가져가는 범위
			eS_PPBOX_AREA,
			eS_PPBOX_GLASS_THICKNESS,
			eS_PPBOX_PAPER_THICKNESS,
			eS_PPBOX_GLASS_COUNT,
			//eS_PPBOX_PAPER_COUNT,
			eS_TOP_MATERIAL,
			eS_UNLOAD_OFFSET_X,
			eS_UNLOAD_OFFSET_Y,

            eS_TIME_AREA,
			eS_TIME_DEFAULT_CYLINDER_TIME_OUT,
			eS_TIME_VACUUM_TIME_OUT,
			eS_TIME_BLOW_TIME_OUT,
			eS_TIME_VACUUM_ON_DELAY,
			eS_TIME_BLOW_ON_DELAY,

			eS_PICKER_OFFSET_Z_AREA,
			eS_PICKER_OFFSET_Z_GLASS_PICKER_LOAD,
			eS_PICKER_OFFSET_Z_GLASS_PICKER_UNLOAD,

			eS_CONVEYOR_1_SPEED_AREA,
			eS_CONVEYOR_2_SPEED_AREA,
			eS_CONVEYOR_3_SPEED_AREA,
			eS_CONVEYOR_4_SPEED_AREA,
			eS_CONVEYOR_5_SPEED_AREA,

			eS_CONVEYOR_1_HIGH_SPEED,

			eS_CONVEYOR_1_LOW_SPEED = eS_CONVEYOR_1_HIGH_SPEED + nCONVEYOR_LD,

			eS_CONVEYOR_1_ACC1 = eS_CONVEYOR_1_LOW_SPEED + nCONVEYOR_LD,

			eS_CONVEYOR_1_ACC2 = eS_CONVEYOR_1_ACC1 + nCONVEYOR_LD,

			eS_CONVEYOR_1_DEC1 = eS_CONVEYOR_1_ACC2 + nCONVEYOR_LD,

			eS_CONVEYOR_1_DEC2 = eS_CONVEYOR_1_DEC1 + nCONVEYOR_LD,

		eS_END = eS_CONVEYOR_1_DEC2 + nCONVEYOR_LD,
	};

	enum eCONTROL
	{
		eC_NONE = -1,

		// title area
		eC_EDIT_NAME,
		eC_EDIT_PPID,
		eC_EDIT_DESCRIPTION,
		eC_BTN_AUTO_CALCULATION,
		eC_BTN_EDIT,
		eC_BTN_UNDO,
		eC_BTN_SAVE,

		eC_EDIT_PPBOX_GLASS_THICKNESS,
		eC_EDIT_PPBOX_PAPER_THICKNESS,
		eC_EDIT_PPBOX_GLASS_COUNT,
		eC_BTN_TOP_MATERIAL,
		eC_EDIT_UNLOAD_OFFSET_X,
		eC_EDIT_UNLOAD_OFFSET_Y,

        eC_TIME_DEFAULT_CYLINDER_TIME_OUT,
		eC_TIME_VACUUM_TIME_OUT,
		eC_TIME_BLOW_TIME_OUT,
		eC_TIME_VACUUM_ON_DELAY,
		eC_TIME_BLOW_ON_DELAY,

		eC_PICKER_OFFSET_Z_GLASS_PICKER_LOAD,
		eC_PICKER_OFFSET_Z_GLASS_PICKER_UNLOAD,

		eC_CONVEYOR_1_HIGH_SPEED,
		eC_CONVEYOR_2_HIGH_SPEED,
		eC_CONVEYOR_3_HIGH_SPEED,
		eC_CONVEYOR_4_HIGH_SPEED,
		eC_CONVEYOR_5_HIGH_SPEED,

		eC_CONVEYOR_1_LOW_SPEED,
		eC_CONVEYOR_2_LOW_SPEED,
		eC_CONVEYOR_3_LOW_SPEED,
		eC_CONVEYOR_4_LOW_SPEED,
		eC_CONVEYOR_5_LOW_SPEED,

		eC_CONVEYOR_1_ACC1,
		eC_CONVEYOR_2_ACC1,
		eC_CONVEYOR_3_ACC1,
		eC_CONVEYOR_4_ACC1,
		eC_CONVEYOR_5_ACC1,

		eC_CONVEYOR_1_ACC2,
		eC_CONVEYOR_2_ACC2,
		eC_CONVEYOR_3_ACC2,
		eC_CONVEYOR_4_ACC2,
		eC_CONVEYOR_5_ACC2,

		eC_CONVEYOR_1_DEC1,
		eC_CONVEYOR_2_DEC1,
		eC_CONVEYOR_3_DEC1,
		eC_CONVEYOR_4_DEC1,
		eC_CONVEYOR_5_DEC1,

		eC_CONVEYOR_1_DEC2,
		eC_CONVEYOR_2_DEC2,
		eC_CONVEYOR_3_DEC2,
		eC_CONVEYOR_4_DEC2,
		eC_CONVEYOR_5_DEC2,

		eC_END,
	};

	DECLARE_DYNAMIC(CRecipeEditWnd)

public:
	CRecipeEditWnd();
	virtual ~CRecipeEditWnd();

private:
	int			m_iSelectControl = eC_NONE;
	int			m_iMoveControl = eC_NONE;
	int			m_iSelectEdit = eC_NONE;
	
	int			m_iSelectInfoTab = eRIT_PANNEL_DATA;
	int			m_iSelectScribeTab = 0;
	CRect		m_rectClient, m_rectS[eS_END], m_rectC[eC_END];

	bool		m_editAble = false;
	T_MDI_RECIPE m_recipe;

private:
	// 현재 선택된 레시피 값 로드 (file data)
	void		LoadInitialize();

private:	
	void ChangeEditColor();

	// 선택한 Info 탭 Show
	void ShowRecipeInfoTab();

	// 레시피 정보 업데이트 용 함수 세팅
	void InitFuncPannelData();
	
private:
	// 선택한 Scribe 탭 Show
	void ShowRecipeScribeTab();

private:
	void ComputeArea();
	void SaveRecipe();
	//void WriteRecipeLog(std::vector<CString> vec);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT		OnEditFinished(WPARAM wParam, LPARAM lParam);
};



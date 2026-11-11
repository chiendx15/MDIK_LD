#pragma once

#include "Simulator.h"
// CDemoWnd frame
#define INTERVAL_UI 7003
#define IDC_LIST_AXIS 9000
#define IDC_LIST_SENSOR 9001
class CDemoWnd : public CFrameWndEx
{


	enum eLI{
		eLI_INDEX,
		eLI_AXIS_NO,
		eLI_NAME,
		eLI_MAX,
		eLI_MIN,
		eLI_LEFT,
		eLI_TOP,
		eLI_RIGHT,
		eLI_BOTTOM,
		eLI_TYPE,
		eLI_DIR,
		eLI_LINK_AXIS,
		eLI_LINK_TYPE,
		eLI_LINK_OFFSET_X,
		eLI_LINK_OFFSET_Y,
		eLI_IMG_PATH,
		eLI_TABLE_W,
		eLI_TABLE_H,
		
	};
	enum eLIS{
		eLIS_INDEX,
		eLIS_IO_NO,
		eLIS_IO_NO_SUB,
		eLIS_NAME,
		eLIS_TYPE,
		eLIS_POS_X,
		eLIS_POS_Y,
		eLIS_OFFSET_X,
		eLIS_OFFSET_Y,
		eLIS_ANGLE,
	};
	enum eS{
		eS_AREA,
		eS_AREA_CONTROL,
		eS_AREA_LIST,
		eS_AREA_AXIS_CONTROL,
		eS_AREA_ETC,
		eS_AREA_BOT,
		eS_LIST,
		eS_SPEED,
		eS_TARGET,
		eS_DISTANCE,
		
		eS_END,
	};
	enum eC{
		eC_NONE = -1,
		eC_ADD,
		eC_INSERT,
		eC_DELETE,
		eC_RESET,
		eC_SAVE,
		eC_LOAD,
		eC_CLOSE,

		eC_JOG_N,
		eC_JOG_P,
		
		eC_STEP_N,
		eC_STEP_P,
		
		eC_PTP,
		eC_SIMULATOR,
		eC_SPEED,
		eC_DISTANCE,
		eC_TARGET,
		eC_AUTO,
		eC_TEST,
		eC_GET_RECT,
		eC_GET_RECT_TABLE,
		eC_GET_AXIS_PARAM,
		eC_SYNC_AXIS_PARAM,
		eC_UP,
		eC_DOWN,
		eC_AXIS,
		eC_SENSOR,
		eC_END,
	};
	DECLARE_DYNCREATE(CDemoWnd)
public:
	CDemoWnd(CWnd* pwndParent = nullptr);           // protected constructor used by dynamic creation
	virtual ~CDemoWnd();
protected:
	int		m_eSelectControl, m_eObjectItem;
	int		m_iIndex;
	CRect	m_rectClient
		, m_rcS[eS_END]
		, m_rcC[eC_END];
	double m_dSpeed[eMOTOR_END]		= {0.,};
	double m_dTarget[eMOTOR_END]	= {0.,};
	double m_dDistance[eMOTOR_END]	= {0.,};
	double m_dPos;
	double m_dDir;
	BOOL	m_bType;
	BOOL m_bAuto;
	CSimulator* m_pSimulator;
	CListCtrlColumnEx	m_listControl;
	CListCtrlColumnEx	m_listControlSensor;
	BOOL	m_bSimulator;
	CImage	m_imgBack;
	int		m_iLDModule = 0;
	int		m_iULDModule = 0;
public:
	CSimulator* getSimulator(){ return m_pSimulator;};
protected:
	void ComputArea();
	void ResetItem();
	void ItemUpDown(BOOL bUp);
	void UpdateMCData();
	void GetAxsiParm();
	int	GetAxis2Module(int iAxis);
	void SetLD();
	void SetSCT();
	void SetConveyor(int iModule, eMODULEs eModuleThis, eMODULEs eModuleNext = eMODULE_NONE);
	void LoadListData();
	int	GetMotor2Index(int iMotor);
protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLvnEndlabeleditAxisList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeleditAxisList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnCnangeAxisList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemCnangedAxisList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditSensorList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeleditSensorList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnCnangeSensorList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemCnangedSensorList(NMHDR *pNMHDR, LRESULT *pResult);
};



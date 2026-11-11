// DemoWnd.cpp : implementation file
//

#include "pch.h"
#include "UI.h"
#include "DemoWnd.h"
#include <WinInet.h>
#include "MainFrame.h"
#include "UserInterface/Main/MainStatusWnd.h"
// CDemoWnd

CDemoWnd::CDemoWnd(CWnd* pwndParent)
{
	m_eSelectControl = eC_NONE;
	m_dPos = 0.;
	m_dDir = 2.;
	m_iIndex = 0;
	m_bAuto = FALSE;
	m_bSimulator = FALSE;
	m_bType = FALSE;
	m_pSimulator = nullptr;
}

CDemoWnd::~CDemoWnd()
{
	SAFE_DELETE(m_pSimulator);
}

IMPLEMENT_DYNCREATE(CDemoWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP(CDemoWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_NOTIFY(LVN_ENDLABELEDIT,		IDC_LIST_AXIS,		&CDemoWnd::OnLvnEndlabeleditAxisList)
	ON_NOTIFY(LVN_BEGINLABELEDIT,	IDC_LIST_AXIS,		&CDemoWnd::OnLvnBeginlabeleditAxisList)
	ON_NOTIFY(NM_CLICK,				IDC_LIST_AXIS,		&CDemoWnd::OnLvnCnangeAxisList)
	ON_NOTIFY(LVN_ITEMCHANGED,		IDC_LIST_AXIS,		&CDemoWnd::OnLvnItemCnangedAxisList)
	
	ON_NOTIFY(LVN_ENDLABELEDIT,		IDC_LIST_SENSOR,		&CDemoWnd::OnLvnEndlabeleditSensorList)
	ON_NOTIFY(LVN_BEGINLABELEDIT,	IDC_LIST_SENSOR,		&CDemoWnd::OnLvnBeginlabeleditSensorList)
	ON_NOTIFY(NM_CLICK,				IDC_LIST_SENSOR,		&CDemoWnd::OnLvnCnangeSensorList)
	ON_NOTIFY(LVN_ITEMCHANGED,		IDC_LIST_SENSOR,		&CDemoWnd::OnLvnItemCnangedSensorList)
	
END_MESSAGE_MAP()


// CDemoWnd message handlers


//BOOL CDemoWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, LPCTSTR lpszMenuName, DWORD dwExStyle, CCreateContext* pContext)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
//}

void CDemoWnd::OnLvnEndlabeleditAxisList(NMHDR *pNMHDR, LRESULT *pResult){
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	
	int				iRow = (int)pDispInfo->item.iItem;
	int				iCol = (int)pDispInfo->item.iSubItem;
	if (m_pSimulator->GetModule())
	m_pSimulator->SetSelectModule(iRow);
}
void CDemoWnd::OnLvnBeginlabeleditAxisList(NMHDR *pNMHDR, LRESULT *pResult){
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	int				iItem = (int)pDispInfo->item.iItem;
	int				iSubItem = (int)pDispInfo->item.iSubItem;
	TRACE(_T("BeginLabeled --item:%d  subitem:%d\n"),iItem,iSubItem);
	*pResult = 0;
	if (m_pSimulator->GetModule())
	m_pSimulator->SetSelectModule(iItem);
}
void CDemoWnd::OnLvnCnangeAxisList(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int				iItem = (int)pNMItemActivate->iItem;
	if( iItem < 0)
		return;
	m_iIndex = iItem;
	if (m_pSimulator->GetModule())
	m_pSimulator->SetSelectModule(iItem);
	Invalidate(FALSE);
}
void CDemoWnd::OnLvnItemCnangedAxisList(NMHDR* pNMHDR, LRESULT* pResult) {
		LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int				iItem = (int)pNMItemActivate->iItem;
	if( iItem < 0)
		return;
	m_iIndex = iItem;
	m_pSimulator->SetSelectModule(iItem);
}
void CDemoWnd::OnLvnEndlabeleditSensorList(NMHDR *pNMHDR, LRESULT *pResult){
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	
	int				iRow = (int)pDispInfo->item.iItem;
	int				iCol = (int)pDispInfo->item.iSubItem;
	m_pSimulator->SetSelectSensor(iRow);
}
void CDemoWnd::OnLvnBeginlabeleditSensorList(NMHDR *pNMHDR, LRESULT *pResult){
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	int				iItem = (int)pDispInfo->item.iItem;
	int				iSubItem = (int)pDispInfo->item.iSubItem;
	TRACE(_T("BeginLabeled --item:%d  subitem:%d\n"),iItem,iSubItem);
	*pResult = 0;
	m_pSimulator->SetSelectSensor(iItem);
}
void CDemoWnd::OnLvnCnangeSensorList(NMHDR *pNMHDR, LRESULT *pResult){
		LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int				iItem = (int)pNMItemActivate->iItem;
	if( iItem < 0)
		return;
	m_iIndex = iItem;
	m_pSimulator->SetSelectSensor(iItem);
	
	Invalidate(FALSE);
}
void CDemoWnd::OnLvnItemCnangedSensorList(NMHDR* pNMHDR, LRESULT* pResult) {
		LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int				iItem = (int)pNMItemActivate->iItem;
	if( iItem < 0)
		return;
	m_iIndex = iItem;
	m_pSimulator->SetSelectSensor(iItem);
	
}
int CDemoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_listControl.Create(LVS_REPORT|LVS_SHOWSELALWAYS| LVS_SINGLESEL | WS_VISIBLE|WS_CHILD|WS_BORDER, m_rcS[eS_LIST], this, IDC_LIST_AXIS);
	m_listControl.SetExtendedStyle(LVS_EX_DOUBLEBUFFER|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);		
	m_listControl.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_listControl.SetRowHeight();
	m_listControl.ShowScrollBar(SB_VERT);
	m_listControl.ShowScrollBar(SB_HORZ);
	m_listControl.InsertColumn(eLI_INDEX,	_T("INDEX"),			LVCFMT_CENTER,	50,	1);
	m_listControl.InsertColumn(eLI_AXIS_NO,	_T("No."),			LVCFMT_CENTER,	30,	1);
	m_listControl.InsertColumn(eLI_NAME,	_T("NAME"),				LVCFMT_CENTER,	100,	1);
	m_listControl.InsertColumn(eLI_MAX,		_T("MAX"),				LVCFMT_CENTER,	60,	1);
	m_listControl.InsertColumn(eLI_MIN,		_T("MIN"),				LVCFMT_CENTER,	60,	1);
	m_listControl.InsertColumn(eLI_LEFT,	_T("LEFT"),				LVCFMT_CENTER,	70,	1);
	m_listControl.InsertColumn(eLI_TOP,		_T("TOP"),				LVCFMT_CENTER,	70,	1);
	m_listControl.InsertColumn(eLI_RIGHT,	_T("RIGHT"),			LVCFMT_CENTER,	70,	1);
	m_listControl.InsertColumn(eLI_BOTTOM,	_T("BOTTOM"),			LVCFMT_CENTER,	70,	1);
	m_listControl.InsertColumn(eLI_TYPE,	_T("TYPE"),				LVCFMT_CENTER,	90,	1);
	m_listControl.InsertColumn(eLI_DIR,		_T("DIR"),				LVCFMT_CENTER,	40,	1);
	m_listControl.InsertColumn(eLI_LINK_AXIS,_T("Link_A"),		LVCFMT_CENTER,	50,	1);
	m_listControl.InsertColumn(eLI_LINK_TYPE,_T("Link_T"),		LVCFMT_CENTER,	70,	1);
	m_listControl.InsertColumn(eLI_LINK_OFFSET_X,_T("OffsetX"),		LVCFMT_CENTER,	60,	1);
	m_listControl.InsertColumn(eLI_LINK_OFFSET_Y,_T("OffsetY"),		LVCFMT_CENTER,	60,	1);
	m_listControl.InsertColumn(eLI_IMG_PATH,	_T("PATH"),				LVCFMT_LEFT,	150,1);
	m_listControl.InsertColumn(eLI_TABLE_W,	_T("T-W"),				LVCFMT_LEFT,	50,1);
	m_listControl.InsertColumn(eLI_TABLE_H,	_T("T-H"),				LVCFMT_LEFT,	50,1);
	
	m_listControl.SetColumnEditNumber(eLI_AXIS_NO);
	m_listControl.SetColumnEditDouble(eLI_MAX);
	m_listControl.SetColumnEditDouble(eLI_MIN);
	m_listControl.SetColumnEditNumber(eLI_LEFT);
	m_listControl.SetColumnEditNumber(eLI_TOP);
	m_listControl.SetColumnEditNumber(eLI_RIGHT);
	m_listControl.SetColumnEditNumber(eLI_BOTTOM);
	m_listControl.SetColumnEditNumber(eLI_TABLE_W);
	m_listControl.SetColumnEditNumber(eLI_TABLE_H);
	std::vector<CString> strsType;
	
	strsType.push_back(_T("0: LINE_STAGE"));
	strsType.push_back(_T("1: GANTRY_STAGE"));
	strsType.push_back(_T("2: Z_STAGE"));
	strsType.push_back(_T("3: ROTATE_STAGE"));
	strsType.push_back(_T("4: ROTATE_TABLE"));
	m_listControl.SetColumnCombo(eLI_TYPE, strsType);
	m_listControl.SetColumnEditNumber(eLI_DIR);
	m_listControl.SetColumnEditNumber(eLI_LINK_AXIS);
	
	std::vector<CString> strsLinkType;
	strsLinkType.push_back(_T("0: Center"));
	strsLinkType.push_back(_T("1: Top"));
	strsLinkType.push_back(_T("2: Bot"));
	strsLinkType.push_back(_T("3: Left"));
	strsLinkType.push_back(_T("4: Right"));
	m_listControl.SetColumnCombo(eLI_LINK_TYPE, strsLinkType);
	m_listControl.SetColumnEditNumber(eLI_LINK_OFFSET_X);
	m_listControl.SetColumnEditNumber(eLI_LINK_OFFSET_Y);
	
	m_listControl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	m_listControlSensor.Create(LVS_REPORT|LVS_SHOWSELALWAYS| LVS_SINGLESEL | WS_VISIBLE|WS_CHILD|WS_BORDER, m_rcS[eS_LIST], this, IDC_LIST_SENSOR);
	m_listControlSensor.SetExtendedStyle(LVS_EX_DOUBLEBUFFER|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);		
	m_listControlSensor.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_listControlSensor.SetRowHeight();
	m_listControlSensor.ShowScrollBar(SB_VERT);
	m_listControlSensor.ShowScrollBar(SB_HORZ);
	m_listControlSensor.InsertColumn(eLIS_INDEX,		_T("INDEX"),			LVCFMT_CENTER,	50,	eLIS_INDEX	);
	m_listControlSensor.InsertColumn(eLIS_IO_NO,		_T("No."),				LVCFMT_CENTER,	50,	eLIS_IO_NO	);
	m_listControlSensor.InsertColumn(eLIS_IO_NO_SUB,	_T("Sub-No."),				LVCFMT_CENTER,	50,	eLIS_IO_NO_SUB	);
	
	m_listControlSensor.InsertColumn(eLIS_NAME,			_T("NAME"),				LVCFMT_CENTER,	100,eLIS_NAME	);
	m_listControlSensor.InsertColumn(eLIS_TYPE,			_T("Type"),				LVCFMT_CENTER,	100,	eLIS_TYPE	);
	m_listControlSensor.InsertColumn(eLIS_POS_X,		_T("X"),				LVCFMT_CENTER,	60,	eLIS_POS_X	);
	m_listControlSensor.InsertColumn(eLIS_POS_Y,		_T("Y"),				LVCFMT_CENTER,	60,	eLIS_POS_Y	);
	m_listControlSensor.InsertColumn(eLIS_OFFSET_X,		_T("OX"),				LVCFMT_CENTER,	60,	eLIS_OFFSET_X);
	m_listControlSensor.InsertColumn(eLIS_OFFSET_Y,		_T("OY"),				LVCFMT_CENTER,	60,	eLIS_OFFSET_Y);
	m_listControlSensor.InsertColumn(eLIS_ANGLE,		_T("Angle"),			LVCFMT_CENTER,	60,	eLIS_ANGLE );
	
	m_listControlSensor.SetColumnEditNumber(eLIS_IO_NO);
	m_listControlSensor.SetColumnEditNumber(eLIS_IO_NO_SUB);
	
	m_listControlSensor.SetColumnEditNumber(eLIS_POS_X);
	m_listControlSensor.SetColumnEditNumber(eLIS_POS_Y);
	m_listControlSensor.SetColumnEditNumber(eLIS_OFFSET_X);
	m_listControlSensor.SetColumnEditNumber(eLIS_OFFSET_Y);
	m_listControlSensor.SetColumnEditNumber(eLIS_ANGLE);
	
	std::vector<CString> strsSensorType;
	strsSensorType.push_back(_T("0: Sensor"));
	strsSensorType.push_back(_T("1: Door"));
	strsSensorType.push_back(_T("2: DoorKey"));
	strsSensorType.push_back(_T("3: EMO"));
	strsSensorType.push_back(_T("4: ROTLamp"));
    strsSensorType.push_back(_T("5: LightCurtain"));    // KES 추가 2024.3.21
	m_listControlSensor.SetColumnCombo(eLIS_TYPE, strsSensorType);
	
	
	m_listControlSensor.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_pSimulator = new CSimulator();
	m_pSimulator->Create(NULL, NULL,NULL, CRect(), this, 10000);
	m_pSimulator->ShowWindow(SW_SHOW);
	m_pSimulator->SetLoadBackGroundImg(_T("../img/01. LOADER_01.png"), _T("../img/01. LOADER_01_Rev.png"));
	LoadListData();
	SetTimer(INTERVAL_UI, 100 , NULL);
	return 0;
}


void CDemoWnd::OnPaint()
{
	CPaintDC			dc(this);
	try {
		CRect					rect(m_rectClient);
		CMemDCEx::CMemDC		memDC(&dc);
		CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_DEFAULT]);
		CBrush*					pOldBrush			= (CBrush*)memDC.SelectStockObject(NULL_BRUSH);
		CPen*					pOldPen				= (CPen*)memDC.SelectObject(&pen);

		memDC.SelectObject(g_brushBackground[eEC_DEFAULT]);
		memDC.FillRect(m_rectClient,&g_brushBackground[eEC_DEFAULT]);
		
		DrawLineRect(&memDC, m_rcS[eS_AREA]);
		DrawButtonUpDown(&memDC, m_rcC[eC_CLOSE], g_st[IDS_CLOSE], m_eSelectControl == eC_CLOSE);
		
		DrawButtonUpDown(&memDC, m_rcC[eC_ADD	], _T("Add"),	m_eSelectControl == eC_ADD);
		DrawButtonUpDown(&memDC, m_rcC[eC_INSERT], _T("Insert"),m_eSelectControl == eC_INSERT);
		
		DrawButtonUpDown(&memDC, m_rcC[eC_DELETE], _T("Del"),	m_eSelectControl == eC_DELETE);
		DrawButtonUpDown(&memDC, m_rcC[eC_RESET	], _T("Reset"), m_eSelectControl == eC_RESET);

		DrawButtonUpDown(&memDC, m_rcC[eC_SAVE	], _T("Save"), m_eSelectControl == eC_SAVE);
		DrawButtonUpDown(&memDC, m_rcC[eC_LOAD	], _T("Load"), m_eSelectControl == eC_LOAD);

		
		DrawText(&memDC, m_rcS[eS_SPEED		], _T("Speed"));
		DrawText(&memDC, m_rcS[eS_DISTANCE	], _T("Distance"));
		DrawText(&memDC, m_rcS[eS_TARGET	], _T("Target"));
		
		SelectedDrawEditDouble(&memDC, m_rcC[eC_SPEED		], m_dSpeed[m_iIndex]   );
		SelectedDrawEditDouble(&memDC, m_rcC[eC_DISTANCE	], m_dDistance[m_iIndex]  );
		SelectedDrawEditDouble(&memDC, m_rcC[eC_TARGET		], m_dTarget[m_iIndex]);
		
		DrawMoveDirect(&memDC, m_rcC[eC_JOG_P		], m_eSelectControl == eC_JOG_P, eMD_RIGHT);
		DrawMoveDirect(&memDC, m_rcC[eC_JOG_N		], m_eSelectControl == eC_JOG_N, eMD_LEFT);
		DrawMoveDirect(&memDC, m_rcC[eC_STEP_P		], m_eSelectControl == eC_STEP_P, eMD_RIGHT );
		DrawMoveDirect(&memDC, m_rcC[eC_STEP_N		], m_eSelectControl == eC_STEP_N, eMD_LEFT);
		DrawButtonUpDown(&memDC, m_rcC[eC_PTP		], _T("Move PTP"), m_eSelectControl == eC_PTP);

		//DrawButtonUpDown(&memDC, m_rcC[eC_SIMULATOR	], _T("SIMULATOR"), sm.SystemParameter->bSimulationMode);
		DrawButtonUpDown(&memDC, m_rcC[eC_SIMULATOR], _T("SIMULATOR"), m_bSimulator);
		DrawButtonUpDown(&memDC, m_rcC[eC_AUTO		], _T("AUTO"), m_bAuto);

		
		
		DrawButtonUpDown(&memDC, m_rcC[eC_TEST	], _T("TEST"), m_eSelectControl == eC_TEST);
		DrawButtonUpDown(&memDC, m_rcC[eC_GET_RECT	], _T("GetRect"), m_eSelectControl == eC_GET_RECT);
		DrawButtonUpDown(&memDC, m_rcC[eC_GET_RECT_TABLE], _T("GetRectTable"), m_eSelectControl == eC_GET_RECT_TABLE);

		DrawButtonUpDown(&memDC, m_rcC[eC_GET_AXIS_PARAM], _T("GetAxisParm"), m_eSelectControl == eC_GET_AXIS_PARAM);
		DrawButtonUpDown(&memDC, m_rcC[eC_SYNC_AXIS_PARAM], _T("SyncParam"), m_eSelectControl == eC_SYNC_AXIS_PARAM);
		

		DrawButtonUpDown(&memDC, m_rcC[eC_UP	], _T("UP"), m_eSelectControl == eC_UP	);
		DrawButtonUpDown(&memDC, m_rcC[eC_DOWN	], _T("DOWN	"), m_eSelectControl == eC_DOWN	);
		
		DrawButtonUpDown(&memDC, m_rcC[eC_AXIS],	_T("Axis"),		m_bType);
		DrawButtonUpDown(&memDC, m_rcC[eC_SENSOR],	_T("Sensor"),	!m_bType);
		
		
		
		memDC.SelectObject(pOldBrush);
		memDC.SelectObject(pOldPen);
	}
	catch (...) {
	}
}


void CDemoWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if( bShow){
		//SetWindowText(g_st[IDS_TITLE_APPLYDXFMAP]);
		//int iW = g_rectArea[eAREA_WORK].Width() ;//(g_rectArea[eAREA_WORK].Width() - 5) / 1.5;
		//int iH = g_rectArea[eAREA_WORK].Height();//(g_rectArea[eAREA_WORK].Height() - 5) / 1.5;
		//SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, iW, iH, SWP_SHOWWINDOW);
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left-100, g_rectArea[eAREA_WORK].top-100, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height()+200, SWP_SHOWWINDOW);
		ComputArea();
		m_bType = TRUE;
		if (m_bType)
		{
			m_listControl.ShowWindow(SW_SHOW);
			m_listControlSensor.ShowWindow(SW_HIDE);
		}
		m_pSimulator->SetParent(this);
		//SetTimer(INTERVAL_UI, 100 , NULL);
	}else{

		CMainFrame* pMain =  (CMainFrame*)AfxGetMainWnd();
		CMainStatusWnd* pMainStatus = (CMainStatusWnd*)pMain->getMainStatusWnd();
		if (pMainStatus) {
			pMainStatus->SetSimulatorView();
			//m_pSimulator->MoveWindow(m_rectS[eS_IMG_AREA]);
		}
		//KillTimer(INTERVAL_UI);
	}
}

void CDemoWnd::ComputArea(){
	GetClientRect(m_rectClient);
	m_rcS[eS_AREA] = m_rectClient;
	m_rcS[eS_AREA].InflateRect(-10, -10,-10, -10);
	CRect rc = m_rcS[eS_AREA];
	double dw = (rc.Width()-5) /6;
	double dh = (rc.Height()-5)/15;
	m_rcS[eS_AREA_CONTROL		]	= CRect(rc.left+5+dw*0, rc.top+5+dh*0, rc.left+dw*6, rc.top+dh*10);
	m_rcS[eS_AREA_LIST			]	= CRect(rc.left+5+dw*0, rc.top+5+dh*10, rc.left+dw*3.8, rc.top+dh*14);
	m_rcS[eS_AREA_AXIS_CONTROL	]	= CRect(rc.left+5+dw*4, rc.top+5+dh*12, rc.left+dw*6, rc.top+dh*14);
	m_rcS[eS_AREA_ETC			]	= CRect(rc.left+5+dw*4, rc.top+5+dh*10, rc.left+dw*6, rc.top+dh*12);
	m_rcS[eS_AREA_BOT			]	= CRect(rc.left+5+dw*0, rc.top+5+dh*14, rc.left+dw*6, rc.top+dh*15);
	
	rc = m_rcS[eS_AREA_LIST];
	dw = (rc.Width()-5) /15;
	dh = (rc.Height()-5)/2;
	
	m_rcC[eC_AXIS	] = CRect(rc.left+5+dw*0, rc.top+5+dh*0, rc.left+dw*1, rc.top+dh*1); 
	m_rcC[eC_SENSOR	] = CRect(rc.left+5+dw*0, rc.top+5+dh*1, rc.left+dw*1, rc.top+dh*2); 
	m_rcS[eS_LIST	] = CRect(rc.left+5+dw*1, rc.top+5+dh*0, rc.left+dw*14, rc.top+dh*2); 
	m_rcC[eC_UP		] = CRect(rc.left+5+dw*14, rc.top+5+dh*0, rc.left+dw*15, rc.top+dh*1);
	m_rcC[eC_DOWN	] = CRect(rc.left+5+dw*14, rc.top+5+dh*1, rc.left+dw*15, rc.top+dh*2);

	
	rc = m_rcS[eS_AREA_BOT];
	dw = (rc.Width()-5) /7;
	dh = (rc.Height()-5)/1;
	m_rcC[eC_ADD	] = CRect(rc.left+5+dw*0, rc.top+5+dh*0, rc.left+dw*1, rc.top+dh*1);
	m_rcC[eC_INSERT	] = CRect(rc.left+5+dw*1, rc.top+5+dh*0, rc.left+dw*2, rc.top+dh*1);
	m_rcC[eC_DELETE	] = CRect(rc.left+5+dw*2, rc.top+5+dh*0, rc.left+dw*3, rc.top+dh*1);
	m_rcC[eC_RESET	] = CRect(rc.left+5+dw*3, rc.top+5+dh*0, rc.left+dw*4, rc.top+dh*1);

	m_rcC[eC_SAVE	] = CRect(rc.left+5+dw*4, rc.top+5+dh*0, rc.left+dw*5, rc.top+dh*1);
	m_rcC[eC_LOAD	] = CRect(rc.left+5+dw*5, rc.top+5+dh*0, rc.left+dw*6, rc.top+dh*1);
	m_rcC[eC_CLOSE	] = CRect(rc.left+5+dw*6, rc.top+5+dh*0, rc.left+dw*7, rc.top+dh*1);

	rc = m_rcS[eS_AREA_AXIS_CONTROL];
	dw = (rc.Width()-5) /4;
	dh = (rc.Height()-5)/3;

	m_rcS[eS_SPEED		]	= CRect(rc.left+5+dw*0, rc.top+5+dh*0, rc.left+dw*1, rc.top+dh*1);
	m_rcS[eS_DISTANCE	]	= CRect(rc.left+5+dw*0, rc.top+5+dh*1, rc.left+dw*1, rc.top+dh*2);
	m_rcS[eS_TARGET		]	= CRect(rc.left+5+dw*0, rc.top+5+dh*2, rc.left+dw*1, rc.top+dh*3);
		
	m_rcC[eC_SPEED		]	= CRect(rc.left+5+dw*1, rc.top+5+dh*0, rc.left+dw*2, rc.top+dh*1);
	m_rcC[eC_DISTANCE	]	= CRect(rc.left+5+dw*1, rc.top+5+dh*1, rc.left+dw*2, rc.top+dh*2);
	m_rcC[eC_TARGET		]	= CRect(rc.left+5+dw*1, rc.top+5+dh*2, rc.left+dw*2, rc.top+dh*3);

	m_rcC[eC_JOG_N		]	= CRect(rc.left+5+dw*2, rc.top+5+dh*0, rc.left+dw*3, rc.top+dh*1);
	m_rcC[eC_JOG_P		]	= CRect(rc.left+5+dw*3, rc.top+5+dh*0, rc.left+dw*4, rc.top+dh*1);
	m_rcC[eC_STEP_N		]	= CRect(rc.left+5+dw*2, rc.top+5+dh*1, rc.left+dw*3, rc.top+dh*2);
	m_rcC[eC_STEP_P		]	= CRect(rc.left+5+dw*3, rc.top+5+dh*1, rc.left+dw*4, rc.top+dh*2);
	m_rcC[eC_PTP		]	= CRect(rc.left+5+dw*2, rc.top+5+dh*2, rc.left+dw*3, rc.top+dh*3);
	m_rcC[eC_SIMULATOR	]	= CRect(rc.left+5+dw*3, rc.top+5+dh*2, rc.left+dw*3.5, rc.top+dh*3);
	m_rcC[eC_AUTO		]	= CRect(rc.left+5+dw*3.5, rc.top+5+dh*2, rc.left+dw*4, rc.top+dh*3);

	rc = m_rcS[eS_AREA_ETC];
	dw = (rc.Width()-5) /4;
	dh = (rc.Height()-5)/4;
	m_rcC[eC_TEST			]		= CRect(rc.left+5+dw*0, rc.top+5+dh*0, rc.left+dw*1, rc.top+dh*1);
	m_rcC[eC_GET_RECT		]		= CRect(rc.left+5+dw*0, rc.top+5+dh*1, rc.left+dw*1, rc.top+dh*2);
	m_rcC[eC_GET_RECT_TABLE	]		= CRect(rc.left+5+dw*0, rc.top+5+dh*2, rc.left+dw*1, rc.top+dh*3);
	m_rcC[eC_GET_AXIS_PARAM	]		= CRect(rc.left+5+dw*1, rc.top+5+dh*0, rc.left+dw*2, rc.top+dh*1);
	m_rcC[eC_SYNC_AXIS_PARAM]		= CRect(rc.left+5+dw*1, rc.top+5+dh*1, rc.left+dw*2, rc.top+dh*2);
	
	if (m_pSimulator) 
	{
		m_pSimulator->SetParent(this);
		m_pSimulator->MoveWindow(m_rcS[eS_AREA_CONTROL]);
	}
	if (m_listControl) {
		m_listControl.MoveWindow(m_rcS[eS_LIST]);

	}
	if (m_listControlSensor) {
		m_listControlSensor.MoveWindow(m_rcS[eS_LIST]);
	}
}

void CDemoWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	for( int i=0 ; i < eC_END; i++){
		if( m_rcC[i].PtInRect(point)){
			m_eSelectControl = i;
			//Invalidate(FALSE);
			InvalidateRect(m_rcC[m_eSelectControl]);
			break;
		}
	}

	if(m_eSelectControl >= eC_JOG_N && m_eSelectControl <= eC_JOG_P) {
		CIPVar varJog;
		int iAxis = m_iIndex;
		varJog.AddChildItem(iAxis, "Axis");
		varJog.AddChildItem(m_dSpeed[m_iIndex], "Speed");
		
		int iDirect = ( m_eSelectControl - eC_JOG_N);
		varJog.AddChildItem(iDirect, "Direction");
		//g_theApp.m_ipClient.PostIPCommand("PLCLinker", NULL, "Axis"	,"JogMove", varJog);
	}
	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CDemoWnd::ResetItem(){
	struct T_VAL{
		int iBaseAxis;
		std::vector<int> iLinkAxis;
	};
	std::vector<T_VAL> MultLinkAxis;
	m_pSimulator->DeleteAllModule();
	for(int i =0 ; i < m_listControl.GetItemCount(); i++){
		double dMax = m_listControl.GetItemDouble(i, eLI_MAX	);
		double dMin = m_listControl.GetItemDouble(i, eLI_MIN	);
		int iLeft	= m_listControl.GetItemInt(i, eLI_LEFT	);
		int iTop	= m_listControl.GetItemInt(i, eLI_TOP	);
		int iRight	= m_listControl.GetItemInt(i, eLI_RIGHT	);
		int iBottom = m_listControl.GetItemInt(i, eLI_BOTTOM	);
		int eType	= m_listControl.GetComboCurSel(i, eLI_TYPE	);
		BOOL bDir	= m_listControl.GetItemInt(i, eLI_DIR	);
		int iBaseModule	= m_listControl.GetItemInt(i, eLI_LINK_AXIS	);
		int iOffsetX	= m_listControl.GetItemInt(i, eLI_LINK_OFFSET_X	);
		int iOffsetY	= m_listControl.GetItemInt(i, eLI_LINK_OFFSET_Y	);
		ST_TYPE stType = (ST_TYPE)m_listControl.GetComboCurSel(i, eLI_LINK_TYPE	);
		int iDataTW		= m_listControl.GetItemInt(i, eLI_TABLE_W);
		int iDataTH		= m_listControl.GetItemInt(i, eLI_TABLE_H);
		m_pSimulator->RestItem(i, dMax, dMin, CRect(iLeft, iTop, iRight, iBottom), eType, bDir, iBaseModule);
		m_pSimulator->SetLinkType(i , stType);
		m_pSimulator->SetLinkOffset(i, iOffsetX, iOffsetY);
		m_pSimulator->SetTableSize(i, CSize(iDataTW, iDataTH));
		if( iBaseModule >= 0){
			BOOL bNewModule = TRUE;
			for(int j =0 ; j < MultLinkAxis.size() ; j++){
				if (MultLinkAxis[j].iBaseAxis == iBaseModule){
					MultLinkAxis[j].iLinkAxis.push_back(i);
					bNewModule = FALSE;
				}
			}
			if(bNewModule){
				T_VAL val;
				val.iBaseAxis = iBaseModule;
				val.iLinkAxis.push_back(i);
				MultLinkAxis.push_back(val);
			}
		}
		m_pSimulator->SetShow(i, FALSE);
	}
	m_pSimulator->DeleteAllSensor();
	for (int i = 0; i < m_listControlSensor.GetItemCount(); i++) {
		int iDataIO = m_listControlSensor.GetItemInt(i, eLIS_IO_NO);
		int iDataT = m_listControlSensor.GetItemInt(i, eLIS_TYPE);
		int iDataPX = m_listControlSensor.GetItemInt(i, eLIS_POS_X);
		int iDataPY = m_listControlSensor.GetItemInt(i, eLIS_POS_Y);
		int iDataOX = m_listControlSensor.GetItemInt(i, eLIS_OFFSET_X);
		int iDataOY = m_listControlSensor.GetItemInt(i, eLIS_OFFSET_Y);
		int iDataA = m_listControlSensor.GetItemInt(i, eLIS_ANGLE);
		m_pSimulator->AddSensor(CPoint(iDataPX, iDataPY), CPoint(iDataOX, iDataOY), iDataT, -1);
		m_pSimulator->SetSensorAngle(i, iDataA);
		
	}
	for(int i =0 ; i < MultLinkAxis.size() ; i++){
		if( MultLinkAxis[i].iLinkAxis.size() > 1){
			for(int j =0 ; j < MultLinkAxis[i].iLinkAxis.size() ; j++){
				m_pSimulator->SetGideStage(MultLinkAxis[i].iLinkAxis[j], FALSE);
			}
		}
	}
}
void CDemoWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if( m_rcC[m_eSelectControl].PtInRect(point)){
		switch( m_eSelectControl){
			case eC_ADD:{

				if (m_bType) {
					int iItem = m_listControl.GetItemCount();
					m_listControl.InsertItem(iItem, _T(""));
					m_listControl.SetItemText(iItem, eLI_NAME, Format(_T("Axis%d"), iItem));
					m_listControl.SetItemInt(iItem, eLI_AXIS_NO, iItem);

					m_listControl.SetItemText(iItem, eLI_INDEX, Format(_T("%d"), iItem));
					m_listControl.SetItemDouble(iItem, eLI_MAX, 100);
					m_listControl.SetItemDouble(iItem, eLI_MIN, 0);
					m_listControl.SetItemInt(iItem, eLI_LINK_AXIS, -1);
					m_listControl.SetItemInt(iItem, eLI_LEFT, 100 * iItem);
					m_listControl.SetItemInt(iItem, eLI_TOP, 100 * iItem);
					m_listControl.SetItemInt(iItem, eLI_RIGHT, 100 + 100 * iItem + 50 * iItem);
					m_listControl.SetItemInt(iItem, eLI_BOTTOM, 100 + 100 * iItem + 50 * iItem);
					m_listControl.SetComboCurSel(iItem, eLI_TYPE, 0);
					m_listControl.SetItemInt(iItem, eLI_DIR, iItem % 2);
					m_listControl.SetComboCurSel(iItem, eLI_LINK_TYPE, 0);
					m_listControl.SetItemText(iItem, eLI_IMG_PATH, _T("TEST"));
					m_listControl.SetItemInt(iItem, eLI_TABLE_W, 100);
					m_listControl.SetItemInt(iItem, eLI_TABLE_H, 50);
				}
				else {
					int iItem = m_listControlSensor.GetItemCount();
					m_listControlSensor.InsertItem(iItem, _T(""));
					m_listControlSensor.SetItemText(iItem, eLIS_NAME, Format(_T("Sensor%d"), iItem));
					m_listControlSensor.SetItemInt(iItem, eLIS_POS_X, iItem*7);
					m_listControlSensor.SetItemInt(iItem, eLIS_POS_Y, iItem*7);
					m_listControlSensor.SetItemInt(iItem, eLIS_IO_NO, iItem);
					
					
				}

				
				break;
			}
			case eC_INSERT:{
				if (m_bType) {
					int iItem = m_listControl.GetSelectionMark();

					if (iItem >= 0) {
						m_listControl.InsertItem(iItem, _T(""));
						m_listControl.SetItemText(iItem, eLI_INDEX, Format(_T("%d"), iItem));
						m_listControl.SetItemInt(iItem, eLI_AXIS_NO, iItem);

						m_listControl.SetItemDouble(iItem, eLI_MAX, 100);
						m_listControl.SetItemDouble(iItem, eLI_MIN, 0);
						m_listControl.SetItemInt(iItem, eLI_LINK_AXIS, -1);
						m_listControl.SetItemInt(iItem, eLI_LEFT, 100 * iItem);
						m_listControl.SetItemInt(iItem, eLI_TOP, 100 * iItem);
						m_listControl.SetItemInt(iItem, eLI_RIGHT, 100 + 100 * iItem + 50 * iItem);
						m_listControl.SetItemInt(iItem, eLI_BOTTOM, 100 + 100 * iItem + 50 * iItem);
						m_listControl.SetItemInt(iItem, eLI_TYPE, 0);
						m_listControl.SetComboCurSel(iItem, eLI_TYPE, 0);
						m_listControl.SetComboCurSel(iItem, eLI_LINK_TYPE, 0);
						m_listControl.SetItemText(iItem, eLI_IMG_PATH, _T("TEST"));
						m_listControl.SetItemInt(iItem, eLI_TABLE_W, 100);
						m_listControl.SetItemInt(iItem, eLI_TABLE_H, 50);

					}
					for (int i = 0; i < m_listControl.GetItemCount(); i++) {
						m_listControl.SetItemText(i, eLI_INDEX, Format(_T("%d"), i));
						m_listControl.SetItemInt(i, eLI_AXIS_NO, i);
					}
				}
				break;
			}
			case eC_DELETE:{
				if (m_bType) {
					int iItem = m_listControl.GetSelectionMark();
					if (iItem >= 0) {
						m_listControl.DeleteItem(iItem);
					}
					for (int i = 0; i < m_listControl.GetItemCount(); i++) {
						m_listControl.SetItemText(i, eLI_INDEX, Format(_T("%d"), i));
					}
				}
				else {
					int iItem = m_listControlSensor.GetSelectionMark();
					if (iItem >= 0) {
						m_listControlSensor.DeleteItem(iItem);
					}
					for (int i = 0; i < m_listControlSensor.GetItemCount(); i++) {
						m_listControlSensor.SetItemText(i, eLIS_INDEX, Format(_T("%d"), i));
					}
				}
				break;
			}
			case eC_RESET:{
				ResetItem();
				break;
			}
			case eC_CLOSE:{
				ShowWindow(SW_HIDE);
				break;
			}
			case eC_SAVE:{
				//BROWSEINFO   bi; 
				//LPITEMIDLIST  idl;
				//TCHAR szPathname[MAX_PATH] = {0, };
				//ZeroMemory(&bi,sizeof(BROWSEINFO));
				//bi.hwndOwner   =   this->m_hWnd; 
				//bi.pszDisplayName =  szPathname;  // 선택된 디렉토리명 저장 
				//bi.lpszTitle = _T("폴더를 선택해 주세요"); 
				//bi.ulFlags  =  BIF_EDITBOX 	| 0x0040;            // 새로운 디렉토리를 만들수 있는 버튼 추가 
				//idl = SHBrowseForFolder(&bi);
				//if ( idl )
				//{	
				//	SHGetPathFromIDList(idl, szPathname);
				//	m_listControl.SaveFile(szPathname, _T(","));
				//}

				CString			strInitial;
				CFileDialog		file(FALSE, _T("lst"), NULL, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, 
					_T("lst files(*.lst)|*.lst|All Files(*.*)|*.*||"));
				CreateSubDirectories(_T("..\\Data\\ListData\\"));
				GetFullPathName(_T("..\\Data\\ListData\\"), 1024, strInitial.GetBuffer(1024), NULL);
				strInitial.ReleaseBuffer();
				file.m_ofn.lpstrInitialDir = strInitial;
				
				if (file.DoModal()==IDOK) {
					CString strFullPath, strPath, strTitle, strExt;
					strFullPath = file.GetPathName();
					SplitPath(strFullPath, strPath, strTitle, strExt);
					if (m_bType) {
						m_listControl.SaveFile(strFullPath, _T(","));
					}
					else {
						m_listControlSensor.SaveFile(strPath + strTitle + _T("Sensor") + strExt, _T(","));
					}
				}
				break;
			}
			case eC_LOAD:{
				CString			strInitial;
				CFileDialog		file(TRUE, _T("lst"), NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, 
					_T("lst files(*.lst)|*.lst|All Files(*.*)|*.*||"));
				CreateSubDirectories(_T("..\\Data\\ListData\\"));
				GetFullPathName(_T("..\\Data\\ListData\\"), 1024, strInitial.GetBuffer(1024), NULL);
				strInitial.ReleaseBuffer();
				file.m_ofn.lpstrInitialDir = strInitial;
				
				if (file.DoModal()==IDOK) {
					CString strFullPath, strPath, strTitle, strExt;
					strFullPath = file.GetPathName();
					SplitPath(strFullPath, strPath, strTitle, strExt);
					std::vector<CString> strs,strsSensor;
					m_listControl.LoadFile(strFullPath, _T(","), strs);
					m_listControlSensor.LoadFile(strPath+ strTitle+_T("Sensor")+ strExt, _T(","), strsSensor);
				}
				break;
			}
			case eC_JOG_N:
			case eC_JOG_P: {
				int iAxis = m_iIndex;

				CIPVar varJogStop;
				varJogStop.AddChildItem(iAxis, "Axis");
				//g_theApp.m_ipClient.PostIPCommand("PLCLinker", NULL, "Axis", "MotorStop", varJogStop);
				
				
				break;
			}
			case eC_STEP_N :
			case eC_STEP_P:{
				int iAxis = m_iIndex;

				int iDirect = 1;
				iDirect = ((eC_STEP_P - m_eSelectControl)) ? -1 : 1;
				
				CIPVar varJogStop;
				varJogStop.AddChildItem(iAxis, "Axis");
				varJogStop.AddChildItem(iDirect*m_dDistance[m_iIndex], "Distance");
				varJogStop.AddChildItem(m_dSpeed[m_iIndex], "Speed");
				//g_theApp.m_ipClient.PostIPCommand("PLCLinker", NULL, "Axis", "StepMove", varJogStop);
				break;
			}
			case eC_PTP:{
				int iAxis = m_iIndex;

				CIPVar varPtp;
				varPtp.AddChildItem(iAxis, "Axis");
				varPtp.AddChildItem(m_dSpeed[m_iIndex], "Speed");
				varPtp.AddChildItem(m_dTarget[m_iIndex], "Target");
				
				//g_theApp.m_ipClient.PostIPCommand("PLCLinker", NULL, "Axis"	,Format("PTPMove%d", iAxis), varPtp);
				break;
			}
			case eC_SPEED:{
				EditStart(this, m_rcC[m_eSelectControl], m_dSpeed[m_iIndex]);
				break;
			}
			case eC_DISTANCE:{
				EditStart(this, m_rcC[m_eSelectControl], m_dDistance[m_iIndex]);
				break;
			}
			case eC_TARGET:{
				EditStart(this, m_rcC[m_eSelectControl], m_dTarget[m_iIndex]);
				break;
			}
			case eC_SIMULATOR:{
				//sm.SystemParameter->bSimulationMode = !sm.SystemParameter->bSimulationMode;
				m_bSimulator = !m_bSimulator;
				break;
			}
			case eC_AUTO:{
				m_bAuto =!m_bAuto;
				break;
			}
			case eC_TEST: {
				m_pSimulator->SetMouseEnvet(eME_DRAW_RECT);
				break;
			}
			case eC_GET_RECT: {
				if (m_bType) {
					int iItem = m_listControl.GetSelectionMark();
					if (iItem >= 0) {
						CRect  rect;
						m_pSimulator->GetLastRect(rect);
						m_listControl.SetItemInt(iItem, eLI_LEFT, rect.left);
						m_listControl.SetItemInt(iItem, eLI_RIGHT, rect.right);
						m_listControl.SetItemInt(iItem, eLI_TOP, rect.top);
						m_listControl.SetItemInt(iItem, eLI_BOTTOM, rect.bottom);
					}
				}
				else {
					int iItem = m_listControlSensor.GetSelectionMark();
					if (iItem >= 0) {
						CRect  rect;
						m_pSimulator->GetLastRect(rect);
						m_listControlSensor.SetItemInt(iItem, eLIS_POS_X, rect.CenterPoint().x);
						m_listControlSensor.SetItemInt(iItem, eLIS_POS_Y, rect.CenterPoint().y);
					}
				}
				break;
			}
			case eC_GET_RECT_TABLE: {
				int iItem = m_listControl.GetSelectionMark();
				if(iItem >= 0){
					CRect  rect;
					m_pSimulator->GetLastRect(rect);
					m_listControl.SetItemInt(iItem, eLI_TABLE_W, rect.Width());
					m_listControl.SetItemInt(iItem, eLI_TABLE_H, rect.Height());
					
				}
				break;
			}
			case eC_GET_AXIS_PARAM: {
				m_listControl.DeleteAllItems();
				for (int i = 0; i < eMOTOR_END; i++) {
					m_listControl.InsertItem(i, _T(""));
					m_listControl.SetItemInt(i,	eLI_INDEX,	i);
					m_listControl.SetItemInt(i, eLI_AXIS_NO, i);
					m_listControl.SetItemText(i,	eLI_NAME,	g_stMotor[i]);
					m_listControl.SetItemDouble(i,	eLI_MAX,	sm.MotorParameter[i].dLimitHigh);
					m_listControl.SetItemDouble(i,	eLI_MIN,	sm.MotorParameter[i].dLimitLow);
					m_listControl.SetItemInt(i,	eLI_LEFT,	0);
					m_listControl.SetItemInt(i,	eLI_TOP,	0);
					m_listControl.SetItemInt(i,	eLI_RIGHT,	10);
					m_listControl.SetItemInt(i,	eLI_BOTTOM, 10);
					m_listControl.SetComboCurSel(i,	eLI_TYPE,	CSimulator::eS_LINE_STAGE);
					m_listControl.SetItemInt(i,	eLI_DIR	,	0);
					m_listControl.SetItemInt(i,	eLI_LINK_AXIS,	-1);
					m_listControl.SetComboCurSel(i,	eLI_LINK_TYPE, 0);
					m_listControl.SetItemInt(i, eLI_TABLE_W, 100);
					m_listControl.SetItemInt(i, eLI_TABLE_H, 50);
				
					
				}
				break;
			}
			case eC_UP:
			case eC_DOWN: {
				ItemUpDown( eC_DOWN - m_eSelectControl );
				break;
			}
			case eC_AXIS: {
				m_bType = TRUE;
				m_listControl.ShowWindow(SW_SHOW);
				m_listControlSensor.ShowWindow(SW_HIDE);
				break;
			}
			case eC_SENSOR: {
				m_bType = FALSE;
				m_listControl.ShowWindow(SW_HIDE);
				m_listControlSensor.ShowWindow(SW_SHOW);
				break;
			}
			case eC_SYNC_AXIS_PARAM: {
				GetAxsiParm();
				break;
			}

		}
	}
	InvalidateRect(m_rcC[m_eSelectControl]);
	m_eSelectControl = eC_NONE;
	//Invalidate(FALSE);
	
	CFrameWnd::OnLButtonUp(nFlags, point);
}

void CDemoWnd::ItemUpDown(BOOL bUp) {
	
	int iItem = m_listControl.GetSelectionMark();
	if (bUp && iItem == 0) {
		return;
	}
	if (!bUp && iItem == m_listControl.GetItemCount()-1) {
		return;
	}

	int iAxisNo		= m_listControl.GetItemInt(iItem, eLI_AXIS_NO);
	CString strDatName	= m_listControl.GetItemText(iItem, eLI_NAME);
	double dDataMax		= m_listControl.GetItemDouble(iItem, eLI_MAX);			
	double dDataMin		= m_listControl.GetItemDouble(iItem, eLI_MIN);			
	int iDataLeft		= m_listControl.GetItemInt(iItem, eLI_LEFT);
	int iDataTop		= m_listControl.GetItemInt(iItem, eLI_TOP);
	int iDataRight		= m_listControl.GetItemInt(iItem, eLI_RIGHT);		
	int iDataBottom		= m_listControl.GetItemInt(iItem, eLI_BOTTOM);
	int iDataType		= m_listControl.GetComboCurSel(iItem, eLI_TYPE);
	int iDataDir		= m_listControl.GetItemInt(iItem, eLI_DIR);
	int iDataLinkAxis	= m_listControl.GetItemInt(iItem, eLI_LINK_AXIS);
	int iDataLinkType	= m_listControl.GetComboCurSel(iItem, eLI_LINK_TYPE);
	int iDataOffsetX	= m_listControl.GetItemInt(iItem, eLI_LINK_OFFSET_X);
	int iDataOffsetY	= m_listControl.GetItemInt(iItem, eLI_LINK_OFFSET_Y);
	CString strImgPath	= m_listControl.GetItemText(iItem, eLI_IMG_PATH);
	int iDataTW		= m_listControl.GetItemInt(iItem, eLI_TABLE_W);
	int iDataTH		= m_listControl.GetItemInt(iItem, eLI_TABLE_H);
	
	if (iItem >= 0) {
		m_listControl.DeleteItem(iItem);
		int iIndexPos = iItem+1;
		if (bUp) {
			iIndexPos = iItem-1;
		}
		m_listControl.InsertItem(iIndexPos,_T(""));

		
		m_listControl.SetItemInt(iIndexPos, eLI_AXIS_NO			, iAxisNo		);
		m_listControl.SetItemText(iIndexPos, eLI_NAME			, strDatName	);
		m_listControl.SetItemDouble(iIndexPos, eLI_MAX			, dDataMax		);		
		m_listControl.SetItemDouble(iIndexPos, eLI_MIN			, dDataMin		);		
		m_listControl.SetItemInt(iIndexPos, eLI_LEFT			, iDataLeft		);
		m_listControl.SetItemInt(iIndexPos, eLI_TOP				, iDataTop		);
		m_listControl.SetItemInt(iIndexPos, eLI_RIGHT			, iDataRight		);		
		m_listControl.SetItemInt(iIndexPos, eLI_BOTTOM			, iDataBottom		);
		m_listControl.SetComboCurSel(iIndexPos, eLI_TYPE		, iDataType		);
		m_listControl.SetItemInt(iIndexPos, eLI_DIR				, iDataDir		);
		m_listControl.SetItemInt(iIndexPos, eLI_LINK_AXIS		, iDataLinkAxis	);
		m_listControl.SetComboCurSel(iIndexPos, eLI_LINK_TYPE	, iDataLinkType	);
		m_listControl.SetItemInt(iIndexPos, eLI_LINK_OFFSET_X	, iDataOffsetX	);
		m_listControl.SetItemInt(iIndexPos, eLI_LINK_OFFSET_Y	, iDataOffsetY	);
		m_listControl.SetItemText(iIndexPos, eLI_IMG_PATH		, strImgPath	);
		m_listControl.SetItemInt(iIndexPos, eLI_TABLE_W			, iDataTW);
		m_listControl.SetItemInt(iIndexPos, eLI_TABLE_H			, iDataTH);
		
		for (int i = 0; i < eMOTOR_END; i++) {
			m_listControl.SetItemInt(i, eLI_INDEX, i);
		}
		m_listControl.SetItemState(iIndexPos, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_listControl.SetSelectionMark(iIndexPos);
	}
				
}
void CDemoWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent){
		case INTERVAL_UI:{
			KillTimer(INTERVAL_UI);
			int iCount = m_listControl.GetItemCount();
			for( int i =0 ; i < iCount ; i++){
				int iAxisNo = m_listControl.GetItemInt(i, eLI_AXIS_NO);
				if (iAxisNo < eMOTOR_END) {
					m_pSimulator->SetPos(i, sm.MotorStatus[iAxisNo].tCrt.dPos);
				}
				else {
					//m_pSimulator->SetPos(i, 0);
				}
			}
			UpdateMCData();
			SetTimer(INTERVAL_UI, 100 , NULL);
			break;
		}
	}
	CFrameWnd::OnTimer(nIDEvent);
}

int	CDemoWnd::GetMotor2Index(int iMotor) {
	int iResult = -1;
	int iCount = m_listControl.GetItemCount();
	for (int i = 0; i < iCount; i++) {
		int iAxisNo = m_listControl.GetItemInt(i, eLI_AXIS_NO);
		if (iMotor == iAxisNo) {
			iResult = i;
			break;
		}
	}
	return iResult ;
}

void CDemoWnd::SetConveyor(int iModule, eMODULEs eModuleThis, eMODULEs eModuleNext) {
	return;
	
	BOOL bCheck[2] = { TRUE, };
	BOOL bRun[2] = { TRUE, };
	BOOL bSpeedH[2] = { TRUE, };
	//BOOL bRevers[2] = { TRUE, };	// KES bRevers 변수 중복으로 주석 2024.3.27
	int iConveyor[2] = {0, };
	std::fill_n(bCheck, countof(bCheck), TRUE);
	std::fill_n(bRun, countof(bRun), TRUE);
	std::fill_n(bSpeedH, countof(bSpeedH), TRUE);
	//std::fill_n(bRevers, countof(bRevers), TRUE);	// KES bRevers 변수 중복으로 주석 2024.3.27
	std::fill_n(iConveyor, countof(iConveyor), 0);
	
	double dPosL = 0;
	double dPosH = 0;
	double dSpd = 0;
	BOOL bRevers = FALSE;
	iConveyor[0] = eModuleThis - eMODULE_PC1_LOAD_CONVEYOR_1;
	iConveyor[1] = eModuleNext - eMODULE_PC1_LOAD_CONVEYOR_1;

	bCheck[0] &= GetPPBoxSensor(eModuleThis, TRUE) && sm.Module[eModuleThis].Glass.bMaterial;
	bCheck[1] &= GetPPBoxSensor(eModuleNext, TRUE) && sm.Module[eModuleNext].Glass.bMaterial;

	bRun[0] &= Serial::Inverter::GetInverterRun(eModuleThis);
	bRun[1] &= Serial::Inverter::GetInverterRun(eModuleNext);

	bSpeedH[0] &= Serial::Inverter::GetInverterSpeedCompare(eModuleThis, g_pRecipe->conveyorSpeed[iConveyor[0]].dHighSpeed);
	bSpeedH[1] &= Serial::Inverter::GetInverterSpeedCompare(eModuleNext, g_pRecipe->conveyorSpeed[iConveyor[1]].dHighSpeed);

	
    int eArea = 0;
	BOOL bMove = FALSE;

	dPosL = eArea * 20;
	dPosH = (eArea + 1) * 20;

	if (bRun[0]) {
		m_pSimulator->SetPos(iModule, (dPosH + dPosL)/2);
		m_pSimulator->SetAutoMove(iModule, bMove, -dSpd, dPosH, dPosL);
	}
	else {
		dSpd = (!bSpeedH[0]) ? 10 : 30;
	
		if (bRevers) {
			m_pSimulator->SetAutoMove(iModule, bMove, -dSpd, dPosH, dPosL);
		}
		else {
			m_pSimulator->SetAutoMove(iModule, bMove, dSpd, dPosL, dPosH);
		}
	}
}
	
	
void CDemoWnd::UpdateMCData() {
	
	eMODULEs eModuleLD_CV1 = eMODULE_PC1_LOAD_CONVEYOR_1;
	eMODULEs eModuleLD_CV2 = eMODULE_PC1_LOAD_CONVEYOR_2;
	eMODULEs eModuleELEV_CV = eMODULE_PC1_ELEVATOR_CONVEYOR;
	eMODULEs eModuleULD_CV1 = eMODULE_PC1_UNLOAD_CONVEYOR_1;
	eMODULEs eModuleULD_CV2 = eMODULE_PC1_UNLOAD_CONVEYOR_2;
	eMODULEs eModuleGLASS_TR = eMODULE_PC1_GLASS_TRANSFER;
	
	auto& ModuleLD_CV1		= sm.Module[eModuleLD_CV1];
	auto& ModuleLD_CV2		= sm.Module[eModuleLD_CV2];
	auto& ModuleELEV_CV		= sm.Module[eModuleELEV_CV];
	auto& ModuleULD_CV1		= sm.Module[eModuleULD_CV1]; 
	auto& ModuleULD_CV2		= sm.Module[eModuleULD_CV2]; 
	auto& ModuleGLASS_TR	= sm.Module[eModuleGLASS_TR]; 
	
	int iCount = m_listControl.GetItemCount();
	
	for( int i =0 ; i < iCount ; i++){
		int iAxisNo = m_listControl.GetItemInt(i, eLI_AXIS_NO);
		switch (iAxisNo) {
			case eMOTOR_GLASS_TRANSFER_Y: {
				m_pSimulator->SetOnMaterial(i, ModuleGLASS_TR.Glass.bMaterial);
				break;
			}
			case 100: {	// LD Conveyor 1
				SetConveyor(i, eModuleLD_CV1, eModuleLD_CV2);
				m_pSimulator->SetOnMaterial(i, ModuleLD_CV1.Glass.bMaterial);
				m_pSimulator->SetShow(i, FALSE, ModuleLD_CV1.Glass.bMaterial);
				break;
			}
			case 101: {	// LD Conveyor 2
				SetConveyor(i, eModuleLD_CV2, eModuleELEV_CV);
				m_pSimulator->SetOnMaterial(i, ModuleLD_CV2.Glass.bMaterial);
				m_pSimulator->SetShow(i, FALSE, ModuleLD_CV2.Glass.bMaterial);
				break;
			}
			case eMOTOR_GLASS_BOX_EV_Z: {
				SetConveyor(i, eModuleELEV_CV, eModuleULD_CV1);
				m_pSimulator->SetOnMaterial(i, ModuleELEV_CV.Glass.bMaterial);
				m_pSimulator->SetShow(i, FALSE, ModuleELEV_CV.Glass.bMaterial);
				break;
			}
			case 102: {	// ULD Conveyor 1
				SetConveyor(i, eModuleULD_CV1, eModuleULD_CV2);
				m_pSimulator->SetOnMaterial(i, ModuleULD_CV1.Glass.bMaterial);
				m_pSimulator->SetShow(i, FALSE, ModuleULD_CV1.Glass.bMaterial);
				break;
			}
			case 103: {	// ULD Conveyor 2
				SetConveyor(i, eModuleULD_CV2);
				m_pSimulator->SetOnMaterial(i, ModuleULD_CV2.Glass.bMaterial);
				m_pSimulator->SetShow(i, FALSE, ModuleULD_CV2.Glass.bMaterial);
				break;
			}
			default:
				break;
							
		}
	}

	iCount = m_listControlSensor.GetItemCount();
	DWORD* dwIO = (DWORD*)&sm.IO->tIn_CRT;
	int iIOMax = (sizeof(sm.IO->tIn_CRT) / sizeof(DWORD)) -1;
	for (int i = 0; i < iCount; i++) {
		int iMain = m_listControlSensor.GetItemInt(i, eLIS_IO_NO);
		int iSub = m_listControlSensor.GetItemInt(i, eLIS_IO_NO_SUB);
		if (iIOMax > iMain 
			&& iIOMax > iSub
			&& iMain >=0
			&& iSub >=0) {
			m_pSimulator->SetSensor(i, dwIO[iMain], dwIO[iSub]);
		}
	}
}

void CDemoWnd::GetAxsiParm() {
	int iCount = m_listControl.GetItemCount();
	for (int i = 0; i < iCount; i++) {
		int iAxisNo = m_listControl.GetItemInt(i, eLI_AXIS_NO);
		if (iAxisNo < eMOTOR_END) {
			m_listControl.SetItemDouble(i, eLI_MAX, sm.MotorParameter[iAxisNo].dLimitLow);
			m_listControl.SetItemDouble(i, eLI_MIN, sm.MotorParameter[iAxisNo].dLimitHigh);
		}
	}
}


int	CDemoWnd::GetAxis2Module(int iAxis) {
	int iModule = -1;
	int iCount = m_listControl.GetItemCount();
	for (int i = 0; i < iCount; i++) {
		int iAxisNo = m_listControl.GetItemInt(i, eLI_AXIS_NO);
		if (iAxisNo == iAxis) {
			iModule = i;
			break;
		}
	}

	return iModule;
}
void CDemoWnd::LoadListData() {
	
	CString strFullPath, strPath, strTitle, strExt;
	strFullPath = _T("..\\Data\\ListData\\LD.lst");
	SplitPath(strFullPath, strPath, strTitle, strExt);
	std::vector<CString> strs,strsSensor;
	m_listControl.LoadFile(strFullPath, _T(","), strs);
	m_listControlSensor.LoadFile(strPath+ strTitle+_T("Sensor")+ strExt, _T(","), strsSensor);
	
	ResetItem();
	//SetTimer(INTERVAL_UI, 100 , NULL);
}
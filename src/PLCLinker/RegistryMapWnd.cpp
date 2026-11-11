// CRegistryMapWnd.cpp: 구현 파일
//

#include "pch.h"
#include "PLCLinker.h"
#include "RegistryMapWnd.h"


// CRegistryMapWnd

IMPLEMENT_DYNAMIC(CRegistryMapWnd, CFrameWnd)

CRegistryMapWnd::CRegistryMapWnd()
{
	m_bLoop = FALSE;
	m_eSelectControl = eC_NONE;
}

CRegistryMapWnd::~CRegistryMapWnd()
{
}


BEGIN_MESSAGE_MAP(CRegistryMapWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_NOTIFY(LVN_ENDLABELEDIT,		ID_LIST_DATA, OnLvnItemEndAbleEdit)
END_MESSAGE_MAP()



// CRegistryMapWnd 메시지 처리기



void CRegistryMapWnd::ComputArea(){
	GetClientRect(m_rectClient);
	CRect rc = m_rectClient;
	m_rectC[eC_CLOSE	]	= CRect(rc.right-105, rc.bottom-35, rc.right-5, rc.bottom-5);
	rc.InflateRect(0,0,0,-45);

	double dW = (rc.Width() -5) /1.;
	double dH = (rc.Height() -5) / 10.;
	
	
	m_rectS[eS_DATA_AREA	]	= CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*1, rc.top+dH*9);
	m_rectS[eS_CONTROL_AREA	]	= CRect(rc.left+5+dW*0, rc.top+5+dH*9, rc.left+dW*1, rc.top+dH*10);
	
	
	rc	=m_rectS[eS_CONTROL_AREA	];
	dW = (rc.Width() -5) /4.;
	dH = (rc.Height() -5) / 1.;

	m_rectC[eC_ADD			]	= CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectC[eC_DELETE		]	= CRect(rc.left+5+dW*1, rc.top+5+dH*0, rc.left+dW*2, rc.top+dH*1);
	m_rectC[eC_READ			]	= CRect(rc.left+5+dW*2, rc.top+5+dH*0, rc.left+dW*3, rc.top+dH*1);
	m_rectC[eS_STOP			]	= CRect(rc.left+5+dW*3, rc.top+5+dH*0, rc.left+dW*4, rc.top+dH*1);
	if( m_lstData)
		m_lstData.MoveWindow(m_rectS[eS_DATA_AREA]);
}
int CRegistryMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_lstData.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, m_rectS[eS_DATA_AREA], this, ID_LIST_DATA);
	m_lstData.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_lstData.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_lstData.SetRowHeight(40);
	m_lstData.ShowScrollBar(SB_VERT);
	m_lstData.ShowScrollBar(SB_HORZ);
	//m_lstAxis.InsertColumn(eLI_NONE, _T(""), LVCFMT_CENTER, 0, 1);
	m_lstData.InsertColumn(eLI_INDEX,		_T("No."), LVCFMT_CENTER, 40, 1);
	m_lstData.InsertColumn(eLI_REG_ID,		_T("RegID."), LVCFMT_CENTER, 100, 1);
	m_lstData.InsertColumn(eLI_CRT,			_T("Data."), LVCFMT_CENTER, 100, 1);
	m_lstData.InsertColumn(eLI_TYPE,		_T("Type."), LVCFMT_CENTER, 100, 1);
	
	m_lstData.InsertColumn(eLI_INPUT_I,		_T("Int"), LVCFMT_CENTER, 100, 1);
	m_lstData.InsertColumn(eLI_INPUT_D,		_T("Double"), LVCFMT_CENTER, 100, 1);
	m_lstData.InsertColumn(eLI_SET_BTN,		_T("Set"), LVCFMT_CENTER, 100, 1);
	m_lstData.SetColumnButton(eLI_SET_BTN, _T("SET"));
	m_lstData.SetColumnEdit(eLI_REG_ID);
	m_lstData.SetColumnEditNumber(eLI_TYPE);
	m_lstData.SetColumnEditNumber(eLI_INPUT_I);
	m_lstData.SetColumnEditDouble(eLI_INPUT_D);
	
	return 0;
}


void CRegistryMapWnd::OnPaint()
{
	CPaintDC dc(this); 
	try {
		CMemDCEx::CMemDC		memDC(&dc);
		CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_DEFAULT]);
		CBrush* pOldBrush = (CBrush*)memDC.SelectStockObject(NULL_BRUSH);
		CPen* pOldPen = (CPen*)memDC.SelectObject(&pen);
		CRect					rect = m_rectClient;
		memDC.SelectObject(&g_brushBackground[eEC_DEFAULT]);
		DrawButtonUpDown(&memDC, m_rectC[eC_CLOSE	], _T("CLOSE"), m_eSelectControl == eC_CLOSE	);
		DrawButtonUpDown(&memDC, m_rectC[eC_ADD		], _T("ADD"),	m_eSelectControl == eC_ADD		);
		DrawButtonUpDown(&memDC, m_rectC[eC_DELETE	], _T("DELETE"),m_eSelectControl == eC_DELETE	);
		DrawButtonUpDown(&memDC, m_rectC[eC_READ	], _T("READ"),	m_eSelectControl == eC_READ	);
		DrawButtonUpDown(&memDC, m_rectC[eS_STOP	], _T("STOP"),	m_eSelectControl == eS_STOP	);
		
		memDC.SelectObject(pOldBrush);
		memDC.SelectObject(pOldPen);	
	}
	catch (...) {

	}
}


void CRegistryMapWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	if( bShow){
		int iW = 1920 / 3;
		int iH = 1080 / 2;
		SetWindowPos(&wndTopMost, iW, 0, iW, iH, SWP_SHOWWINDOW);
		ComputArea();
		SetTimer(INTERVAL_DATA, 100, nullptr);
	}else{
		KillTimer(INTERVAL_DATA);
	}
}


void CRegistryMapWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	for(int i = 0; i < eC_END; i++){
		if( m_rectC[i].PtInRect(point)){
			m_eSelectControl = i;
			Invalidate(FALSE);
			break;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CRegistryMapWnd::UpdateList(){
	m_lstData.DeleteAllItems();

	for( int i =0 ; i < m_RegDatas.size() ;i++){
		m_lstData.InsertItem(i, _T(""));
		m_lstData.SetItemInt(i, eLI_INDEX,	i+1);
		m_lstData.SetItemText(i, eLI_REG_ID,	CString(m_RegDatas[i].szRegID));
		
		
		CString str;
		if(m_RegDatas[i].iType == 0){
			m_lstData.SetItemInt(i, eLI_CRT, m_RegDatas[i].iData);
		}else{
			m_lstData.SetItemDouble(i, eLI_CRT, m_RegDatas[i].dData);
		}
		m_lstData.SetItemText(i, eLI_CRT,	str);
		if(m_RegDatas[i].iType == 0){
			m_lstData.SetItemInt(i, eLI_INPUT_I, m_RegDatas[i].iInputData);
		}else{
			m_lstData.SetItemDouble(i, eLI_INPUT_D, m_RegDatas[i].dInputData);
		}
	}
}
void CRegistryMapWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( m_rectC[m_eSelectControl].PtInRect(point)){
		switch(m_eSelectControl){
			case eC_CLOSE:{
				//ShowWindow(SW_HIDE);
				break;
			}
			case eC_ADD:{
				m_RegDatas.push_back(T_REG_DATA());
				UpdateList();
				break;
			}
			case eC_DELETE:{
				m_RegDatas.erase(m_RegDatas.begin()+(m_eSelectList));
				UpdateList();
				break;
			}
			case eC_READ:{

				for( int i = 0 ; i < m_RegDatas.size() ;i++){
					m_RegDatas[i].szRegID = m_lstData.GetItemText(i, eLI_REG_ID);
					m_RegDatas[i].iType = m_lstData.GetItemInt(i, eLI_TYPE);
				}
				
				m_bLoop = TRUE;
				break;
			}
			case eS_STOP:{
				m_bLoop = FALSE;
				break;
			}
		
		}
	}

	m_eSelectControl = eC_NONE;
	Invalidate(FALSE);
			
	CWnd::OnLButtonUp(nFlags, point);
}


void CRegistryMapWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);
}


void CRegistryMapWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent){
		case INTERVAL_DATA:{
			if( m_bLoop){
				for( int i = 0 ; i < m_RegDatas.size() ;i++){
					
					if(m_RegDatas[i].iType == 0){
						m_RegDatas[i].iData = ys.GetRegDataInt(m_RegDatas[i].szRegID);
						m_lstData.SetItemInt(i, eLI_CRT, m_RegDatas[i].iData);
					}else{
						m_RegDatas[i].dData = ys.GetRegDataDouble(m_RegDatas[i].szRegID);
						m_lstData.SetItemDouble(i, eLI_CRT, m_RegDatas[i].dData);
					}
				}
			}
			break;
		}
	}
	CWnd::OnTimer(nIDEvent);
}

void CRegistryMapWnd::OnLvnItemEndAbleEdit(NMHDR* pNMHDR, LRESULT* pResult) {
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;

	int iItem = pDispInfo->item.iItem;
	int iSubItem = pDispInfo->item.iSubItem;

	switch (iSubItem) {
		case eLI_SET_BTN: {
			m_RegDatas[iItem].iInputData = m_lstData.GetItemInt(iItem, eLI_INPUT_I);
			m_RegDatas[iItem].dInputData = m_lstData.GetItemDouble(iItem, eLI_INPUT_D);
			if( m_RegDatas[iItem].iType ==0){
				ys.SetRegData(m_RegDatas[iItem].szRegID, m_RegDatas[iItem].iInputData);
			}else{
				ys.SetRegData(m_RegDatas[iItem].szRegID, m_RegDatas[iItem].dInputData);
			}
			break;
		}
	}
}

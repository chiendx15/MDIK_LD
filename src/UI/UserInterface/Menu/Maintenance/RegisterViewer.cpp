// RegisterViewer.cpp: 구현 파일
//

#include "pch.h"
#include "UI.h"
#include "RegisterViewer.h"


// RegisterViewer


IMPLEMENT_DYNAMIC(RegisterViewer, CFrameWndEx)

RegisterViewer::RegisterViewer()
{

}

RegisterViewer::~RegisterViewer()
{
}


BEGIN_MESSAGE_MAP(RegisterViewer, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_NOTIFY(LVN_ENDLABELEDIT,		ID_REGISTER_LIST_1, OnLvnItemEndAbleEdit1)
	ON_NOTIFY(LVN_ENDLABELEDIT,		ID_REGISTER_LIST_2, OnLvnItemEndAbleEdit2)
	ON_NOTIFY(LVN_ENDLABELEDIT,		ID_REGISTER_LIST_3, OnLvnItemEndAbleEdit3)
	ON_NOTIFY(LVN_ENDLABELEDIT,		ID_REGISTER_LIST_4,	OnLvnItemEndAbleEdit4)
	ON_NOTIFY(LVN_BEGINLABELEDIT,	ID_REGISTER_LIST_1, OnLvnItemBeginAbleEdit)
	ON_NOTIFY(LVN_BEGINLABELEDIT,	ID_REGISTER_LIST_2, OnLvnItemBeginAbleEdit)
	ON_NOTIFY(LVN_BEGINLABELEDIT,	ID_REGISTER_LIST_3, OnLvnItemBeginAbleEdit)
	ON_NOTIFY(LVN_BEGINLABELEDIT,	ID_REGISTER_LIST_4,	OnLvnItemBeginAbleEdit)
END_MESSAGE_MAP()					



// RegisterViewer 메시지 처리기


void RegisterViewer::fn_ComputeArea(){
	GetClientRect(m_rectClient);
	CRect rc = m_rectClient;
	double dw = (rc.Width()-5)/10;
	double dh = (rc.Height()-5)/10;
	//eS_INIT,
	//eS_END,
	m_rectC[eC_LOCK		]	= CRect(rc.left+5+dw*9.5, rc.top+5, rc.left+dw*10, rc.top+30);
	m_rectS[eS_AREA_TAB	]	= CRect(rc.left+5+dw*0, rc.top+35+dh*0, rc.left+dw*10, rc.top+dh*1+5);
	m_rectS[eS_AREA_LIST]	= CRect(rc.left+5+dw*0, rc.top+5+dh*1, rc.left+dw*10, rc.top+dh*10);
	rc = m_rectS[eS_AREA_TAB	];
	dw = (rc.Width()-5)/4;
	dh = (rc.Height()-5)/1;
	m_rectS[eS_LIST_1_NAME]	= CRect(rc.left+5+dw*0, rc.top+5+dh*0, rc.left+dw*1, rc.top+dh*1);
	m_rectS[eS_LIST_2_NAME]	= CRect(rc.left+5+dw*1, rc.top+5+dh*0, rc.left+dw*2, rc.top+dh*1);
	m_rectS[eS_LIST_3_NAME]	= CRect(rc.left+5+dw*2, rc.top+5+dh*0, rc.left+dw*3, rc.top+dh*1);
	m_rectS[eS_LIST_4_NAME]	= CRect(rc.left+5+dw*3, rc.top+5+dh*0, rc.left+dw*4, rc.top+dh*1);
	rc = m_rectS[eS_AREA_LIST	];
	dw = (rc.Width()-5)/4;
	dh = (rc.Height()-5)/1;
	m_rectS[eS_LIST_1	]	= CRect(rc.left+5+dw*0, rc.top+5+dh*0, rc.left+dw*1, rc.top+dh*1);
	m_rectS[eS_LIST_2	]	= CRect(rc.left+5+dw*1, rc.top+5+dh*0, rc.left+dw*2, rc.top+dh*1);
	m_rectS[eS_LIST_3	]	= CRect(rc.left+5+dw*2, rc.top+5+dh*0, rc.left+dw*3, rc.top+dh*1);
	m_rectS[eS_LIST_4	]	= CRect(rc.left+5+dw*3, rc.top+5+dh*0, rc.left+dw*4, rc.top+dh*1);

	if(m_list[eLIST_WORD_1])
		m_list[eLIST_WORD_1].MoveWindow(m_rectS[eS_LIST_1], FALSE);
	if(m_list[eLIST_WORD_2])
		m_list[eLIST_WORD_2].MoveWindow(m_rectS[eS_LIST_2], FALSE);
	if(m_list[eLIST_BIT_1 ])
		m_list[eLIST_BIT_1 ].MoveWindow(m_rectS[eS_LIST_3], FALSE);
	if(m_list[eLIST_BIT_2 ])
		m_list[eLIST_BIT_2 ].MoveWindow(m_rectS[eS_LIST_4], FALSE);
				
}

int RegisterViewer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	for(int i =0 ; i < eLIST_END; i++){
		m_list[i].Create(LVS_REPORT|LVS_SHOWSELALWAYS| LVS_SINGLESEL | WS_VISIBLE|WS_CHILD|WS_BORDER, m_rectS[eS_LIST_1+i], this, ID_REGISTER_LIST_1+i);
		m_list[i].SetExtendedStyle(LVS_EX_DOUBLEBUFFER|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);		
		m_list[i].ModifyStyle(0, LVS_SHOWSELALWAYS);
		m_list[i].SetRowHeight();
		m_list[i].ShowScrollBar(SB_VERT);
		m_list[i].ShowScrollBar(SB_HORZ);
		m_list[i].InsertColumn(eLI_INDEX,		_T("INDEX"),		LVCFMT_CENTER,	45,	1);
		m_list[i].InsertColumn(eLI_ADDRESS,		_T("Address"),		LVCFMT_CENTER,	45,	1);
		m_list[i].InsertColumn(eLI_NAME,		_T("Name"),			LVCFMT_CENTER,	200,	1);
		m_list[i].InsertColumn(eLI_VALUE_CRT,	_T("Crt"),			LVCFMT_CENTER,	40,	1);
		m_list[i].InsertColumn(eLI_VALUE_INPUT,	_T("Input"),		LVCFMT_CENTER,	40,	1);
		m_list[i].InsertColumn(eLI_SET,			_T("SET"),			LVCFMT_CENTER,	40,	1);
		m_list[ i].SetColumnButton(eLI_SET,		_T("SET"));
		if (i >= eLIST_BIT_1)
			m_list[i].SetColumnEditNumber(eLI_VALUE_INPUT, 0, 1);
		else 
			m_list[i].SetColumnEditNumber(eLI_VALUE_INPUT);
	}
	int iItemCount = 0;
	iItemCount = sizeof(sm.Register->Reg_Word_1)/sizeof(DWORD);
	for (int i = 0; i < iItemCount; i++) {
		m_list[eLIST_WORD_1].InsertItem(i, _T(""));
		m_list[eLIST_WORD_1].SetItemInt(i, eLI_INDEX, i+1);
		m_list[eLIST_WORD_1].SetItemInt(i, eLI_ADDRESS, 1000+i*2);
		m_list[eLIST_WORD_1].SetItemText(i, eLI_NAME, g_st[IDS_REGISTER_WORD_1000+i]);

	}
	
	iItemCount = sizeof(sm.Register->Reg_Word_2)/sizeof(DWORD);
	for (int i = 0; i < iItemCount; i++) {
		m_list[eLIST_WORD_2].InsertItem(i, _T(""));
		m_list[eLIST_WORD_2].SetItemInt(i, eLI_INDEX, i+1);
		m_list[eLIST_WORD_2].SetItemInt(i, eLI_ADDRESS, 2000+i*2);
		m_list[eLIST_WORD_2].SetItemText(i, eLI_NAME, g_st[IDS_REGISTER_WORD_2000+i]);
	}
	
	iItemCount = sizeof(sm.Register->Out)/sizeof(DWORD);
	for (int i = 0; i < iItemCount; i++) {
		m_list[eLIST_BIT_1].InsertItem(i, _T(""));
		m_list[eLIST_BIT_1].SetItemInt(i, eLI_INDEX, i+1);
		m_list[eLIST_BIT_1].SetItemText(i, eLI_ADDRESS, Format(_T("%d%X"),100+((i/16)), i%16));
		m_list[eLIST_BIT_1].SetItemText(i, eLI_NAME, g_st[IDS_REGISTER_BIT_1000+i]);
	}

	iItemCount = sizeof(sm.Register->In)/sizeof(DWORD);
	for (int i = 0; i < iItemCount; i++) {
		m_list[eLIST_BIT_2].InsertItem(i, _T(""));
		m_list[eLIST_BIT_2].SetItemInt(i, eLI_INDEX, i+1);
		m_list[eLIST_BIT_2].SetItemText(i, eLI_ADDRESS, Format(_T("%d%X"),200+((i/16)), i%16));
		m_list[eLIST_BIT_2].SetItemText(i, eLI_NAME, g_st[IDS_REGISTER_BIT_2000+i]);
	}

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

void RegisterViewer::ChangeLanguage()
{
	int iItemCount = 0;
	iItemCount = sizeof(sm.Register->Reg_Word_1) / sizeof(DWORD);
	for (int i = 0; i < iItemCount; i++) {
		m_list[eLIST_WORD_1].SetItemText(i, eLI_NAME, g_st[IDS_REGISTER_WORD_1000 + i]);
	}

	iItemCount = sizeof(sm.Register->Reg_Word_2) / sizeof(DWORD);
	for (int i = 0; i < iItemCount; i++) {
		m_list[eLIST_WORD_2].SetItemText(i, eLI_NAME, g_st[IDS_REGISTER_WORD_2000 + i]);
	}

	iItemCount = sizeof(sm.Register->Out) / sizeof(DWORD);
	for (int i = 0; i < iItemCount; i++) {
		m_list[eLIST_BIT_1].SetItemText(i, eLI_NAME, g_st[IDS_REGISTER_BIT_1000 + i]);
	}

	iItemCount = sizeof(sm.Register->In) / sizeof(DWORD);
	for (int i = 0; i < iItemCount; i++) { 
		m_list[eLIST_BIT_2].SetItemText(i, eLI_NAME, g_st[IDS_REGISTER_BIT_2000 + i]);
	}
	Invalidate(FALSE);
}

void RegisterViewer::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	if( bShow){
		CRect rect = g_rectArea[eAREA_WORK];
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		m_bLock = TRUE;
		SetTimer(INTERVAL_TIMER, 100, nullptr);
	}else{
		KillTimer(INTERVAL_TIMER);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void RegisterViewer::OnPaint()
{
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	memDC.FillSolidRect(m_rectClient, RgbColor::Gainsboro);
	
	DrawLineRect(&memDC, m_rectClient);
	DrawMDIMenuButtonN(memDC, m_rectC[eC_LOCK], m_bLock ? g_st[IDS_PARAMETER_LOCK] : g_st[IDS_PARAMETER_EDIT], m_bLock || m_eSelectControl == eC_LOCK);
		
	SelectedDrawText(memDC, m_rectS[eS_LIST_1_NAME], _T("MP -> PC Word"), 15, RGB_WHITE, RGB_GRAY, RGB_WHITE, DT_CENTER|DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_LIST_2_NAME], _T("PC -> MP Word"), 15, RGB_WHITE, RGB_GRAY, RGB_WHITE, DT_CENTER|DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_LIST_3_NAME], _T("PC -> MP Bit"),  15, RGB_WHITE, RGB_GRAY, RGB_WHITE, DT_CENTER|DT_VCENTER);
	SelectedDrawText(memDC, m_rectS[eS_LIST_4_NAME], _T("MP -> PC Bit"),  15, RGB_WHITE, RGB_GRAY, RGB_WHITE, DT_CENTER|DT_VCENTER);
}


void RegisterViewer::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	for(int i =0 ; i < eC_END; i++){
		if( m_rectC[i].PtInRect(point)){
			InvalidateRect(m_rectC[i], FALSE);
			m_eSelectControl = i;
			break;
		}
	}
	CFrameWndEx::OnLButtonDown(nFlags, point);
}


void RegisterViewer::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( m_rectC[m_eSelectControl].PtInRect(point)){
		switch( m_eSelectControl){
			case eC_LOCK: {
				m_bLock = !m_bLock;
				break;
			}
		}
	}
	m_eSelectControl = eC_NONE;
	//Invalidate(FALSE);
	CFrameWndEx::OnLButtonUp(nFlags, point);
}


void RegisterViewer::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent){
		case INTERVAL_TIMER:{
			InvalidateRect(m_rectC[eC_LOCK], FALSE);
			int iItemCount = 0;
			DWORD* pDw =  nullptr;
			DWORD* pDwOld =  nullptr;

				iItemCount = sizeof(sm.Register->Reg_Word_1)/sizeof(DWORD);
				pDw =  (DWORD*)&sm.Register->Reg_Word_1;
				pDwOld = (DWORD*)&m_RegisterMemory.Reg_Word_1;
				for (int i = 0; i < iItemCount; i++) {
					if(pDwOld[i] != pDw[i]){
						m_list[eLIST_WORD_1].SetItemInt(i, eLI_VALUE_CRT, pDw[i]);
					}
				}
				m_RegisterMemory.Reg_Word_1 = sm.Register->Reg_Word_1;

				iItemCount = sizeof(sm.Register->Reg_Word_2)/sizeof(DWORD);
				pDw =  (DWORD*)&sm.Register->Reg_Word_2;
				pDwOld = (DWORD*)&m_RegisterMemory.Reg_Word_2;
				for (int i = 0; i < iItemCount; i++) {
					if(pDwOld[i] != pDw[i]){
						m_list[eLIST_WORD_2].SetItemInt(i, eLI_VALUE_CRT, pDw[i]);
					}
				}
				m_RegisterMemory.Reg_Word_2 = sm.Register->Reg_Word_2;

				iItemCount = sizeof(sm.Register->Out)/sizeof(DWORD);
				pDw = (DWORD*)&sm.Register->Out;
				pDwOld = (DWORD*)&m_RegisterMemory.Out;
				for (int i = 0; i < iItemCount; i++) {
					if(pDwOld[i] != pDw[i]){
						m_list[eLIST_BIT_1].SetItemInt(i, eLI_VALUE_CRT, pDw[i]);
					}
				}
				m_RegisterMemory.Out = sm.Register->Out;

				iItemCount = sizeof(sm.Register->In)/sizeof(DWORD);
				pDw = (DWORD*)&sm.Register->In;
				pDwOld = (DWORD*)&m_RegisterMemory.In;
				for (int i = 0; i < iItemCount; i++) {
					if(pDwOld[i] != pDw[i]){
						m_list[eLIST_BIT_2].SetItemInt(i, eLI_VALUE_CRT, pDw[i]);
					}
				}
				m_RegisterMemory.In = sm.Register->In;
			}
			break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}


void RegisterViewer::OnLvnItemEndAbleEdit1(NMHDR* pNMHDR, LRESULT* pResult) {
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;

	if (m_bLock) {
		*pResult = -1;
		return;
	}
	int iItem = pDispInfo->item.iItem;
	int iSubItem = pDispInfo->item.iSubItem;
	
	switch (iSubItem) {
		case eLI_SET: {
			// 팬던트 연결 IO 가 정상적으로 들어오지 않았을 때에도 어떤 이유인지 확인 할 수 있기 위해서
			if (sm.IO->tIn_CRT.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION) {
				IPMessage(g_st[IDS_MS_CAN_NOT_CTRL_PENDANT_CONNECTED]);
				break;
			}
			int iValue = m_list[eLIST_WORD_1].GetItemInt(iItem, eLI_VALUE_INPUT);		
			DWORD *pdw = (DWORD*)&sm.Register->Reg_Word_1_NEW;
			pdw[iItem] = iValue;
			break;
		}
	}
}

void RegisterViewer::OnLvnItemEndAbleEdit2(NMHDR* pNMHDR, LRESULT* pResult) {
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;

	if (m_bLock) {
		*pResult = -1;
		return;
	}
	int iItem = pDispInfo->item.iItem;
	int iSubItem = pDispInfo->item.iSubItem;
	
	switch (iSubItem) {
		case eLI_SET: {
			if (sm.IO->tIn_CRT.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION) {
				IPMessage(g_st[IDS_MS_CAN_NOT_CTRL_PENDANT_CONNECTED]);
				break;
			}
			int iValue = m_list[eLIST_WORD_2].GetItemInt(iItem, eLI_VALUE_INPUT);		
			DWORD *pdw = (DWORD*)&sm.Register->Reg_Word_2_NEW;
			pdw[iItem] = iValue;
			break;
		}
	}
}

void RegisterViewer::OnLvnItemEndAbleEdit3(NMHDR* pNMHDR, LRESULT* pResult) {
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;

	if (m_bLock) {
		*pResult = -1;
		return;
	}
	int iItem = pDispInfo->item.iItem;
	int iSubItem = pDispInfo->item.iSubItem;

	switch (iSubItem) {
		case eLI_SET: {
			if (sm.IO->tIn_CRT.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION) {
				IPMessage(g_st[IDS_REGISTER_VIEWER_CAN_NOT_CTRL_PENDANT_CONNECTED]);
				break;
			}
			int iValue = m_list[eLIST_BIT_1].GetItemInt(iItem, eLI_VALUE_INPUT);		
			DWORD *pdw = (DWORD*)&sm.Register->Out_New;
			pdw[iItem] = iValue;
			break;
		}
	}
}

void RegisterViewer::OnLvnItemEndAbleEdit4(NMHDR* pNMHDR, LRESULT* pResult) {
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;
	if (m_bLock) {
		*pResult = -1;
		return;
	}
	int iItem = pDispInfo->item.iItem;
	int iSubItem = pDispInfo->item.iSubItem;
	
	switch (iSubItem) {
		case eLI_SET: {
			if (sm.IO->tIn_CRT.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION) {
				IPMessage(g_st[IDS_MS_CAN_NOT_CTRL_PENDANT_CONNECTED]);
				break;
			}
			int iValue = m_list[eLIST_BIT_2].GetItemInt(iItem, eLI_VALUE_INPUT);		
			//. In New가 없음.
			//DWORD *pdw = (DWORD*)&sm.Register->Reg_IO_2_NEW;
			//pdw[iItem] = !pdw[iItem];
			break;
		}
	}
}


void RegisterViewer::OnLvnItemBeginAbleEdit(NMHDR* pNMHDR, LRESULT* pResult) {
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;

	if (m_bLock) {
		*pResult = -1;
		return;
	}
}
	

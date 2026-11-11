// IOControl.cpp: 구현 파일
//

#include "pch.h"
#include "UI.h"
#include "IOControl.h"


// CIOControl

IMPLEMENT_DYNCREATE(CIOControl, CFrameWndEx)

CIOControl::CIOControl(CWnd* pParent)
{
	m_eSelectControl = eC_NONE;
}

CIOControl::~CIOControl()
{
}


BEGIN_MESSAGE_MAP(CIOControl, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CIOControl 메시지 처리기

void CIOControl::fn_ComputeArea() {
	GetClientRect(m_rectClient);
	CRect rc = m_rectClient;
	double dw = (rc.Width() - 5.) / 5.;
	double dh = (rc.Height() - 10.) / 5.;

	m_rectS[eS_AREA_PPBOX_ALIGN]	= CRect(rc.left+5+dw*0, rc.top+10+dh*0, rc.left+dw*5, rc.top+dh*1);
	m_rectS[eS_AREA_CV_PPBOX]		= CRect(rc.left+5+dw*0, rc.top+10+dh*1.5, rc.left+dw*3, rc.top+dh*3.5);
	m_rectS[eS_AREA_TR_GLASS]		= CRect(rc.left+5+dw*0, rc.top+10+dh*4, rc.left+dw*1, rc.top+dh*5);
	m_rectS[eS_AREA_TR_PAPER]		= CRect(rc.left+5+dw*1, rc.top+10+dh*4, rc.left+dw*3, rc.top+dh*5);
	m_rectS[eS_AREA_BLOW]			= CRect(rc.left+5+dw*3, rc.top+10+dh*4, rc.left+dw*4, rc.top+dh*5);
	m_rectS[eS_AREA_MUTE]			= CRect(rc.left+5+dw*4, rc.top+10+dh*4, rc.left+dw*5, rc.top+dh*5);
	
	rc = m_rectS[eS_AREA_PPBOX_ALIGN];
	rc.InflateRect(0, -15, 0, 0);
	dw = (rc.Width() -5.) / 5.;
	dh = (rc.Height() -5.) / 2.;
	
	m_rectC[eC_PPBOX_ALIGN_X_L_FWD	]	= CRect(rc.left + 5 + dw * 0, rc.top + 5 + dh * 0, rc.left + dw * 1, rc.top + dh * 1);
	m_rectC[eC_PPBOX_ALIGN_X_L_BWD	]	= CRect(rc.left + 5 + dw * 0, rc.top + 5 + dh * 1, rc.left + dw * 1, rc.top + dh * 2);
	m_rectC[eC_PPBOX_ALIGN_X_R_FWD	]	= CRect(rc.left + 5 + dw * 1, rc.top + 5 + dh * 0, rc.left + dw * 2, rc.top + dh * 1);
	m_rectC[eC_PPBOX_ALIGN_X_R_BWD	]	= CRect(rc.left + 5 + dw * 1, rc.top + 5 + dh * 1, rc.left + dw * 2, rc.top + dh * 2);
	m_rectC[eC_PPBOX_ALIGN_Y_FWD	]	= CRect(rc.left + 5 + dw * 2, rc.top + 5 + dh * 0, rc.left + dw * 3, rc.top + dh * 1);
	m_rectC[eC_PPBOX_ALIGN_Y_BWD	]	= CRect(rc.left + 5 + dw * 2, rc.top + 5 + dh * 1, rc.left + dw * 3, rc.top + dh * 2);
	m_rectC[eC_PPBOX_ALIGN_Z_UP		]	= CRect(rc.left + 5 + dw * 3, rc.top + 5 + dh * 0, rc.left + dw * 4, rc.top + dh * 1);
	m_rectC[eC_PPBOX_ALIGN_Z_DOWN	]	= CRect(rc.left + 5 + dw * 3, rc.top + 5 + dh * 1, rc.left + dw * 4, rc.top + dh * 2);
	m_rectC[eC_PPBOX_ALIGN_ALL_FWD	]	= CRect(rc.left + 5 + dw * 4, rc.top + 5 + dh * 0, rc.left + dw * 5, rc.top + dh * 1);
	m_rectC[eC_PPBOX_ALIGN_ALL_BWD	]	= CRect(rc.left + 5 + dw * 4, rc.top + 5 + dh * 1, rc.left + dw * 5, rc.top + dh * 2);
	

	rc = m_rectS[eS_AREA_CV_PPBOX];
	rc.InflateRect(0, -15, 0, 0);
	dw = (rc.Width() - 5.) / 3.;
	dh = (rc.Height() - 5.) / 10.;

	m_rectC[eC_PPBOX_STOPPER_1_UP	]	= CRect(rc.left + 5 + dw * 0, rc.top + 5 + dh * 0, rc.left + dw * 1, rc.top + dh * 2);
	m_rectC[eC_PPBOX_STOPPER_1_DOWN	]	= CRect(rc.left + 5 + dw * 0, rc.top + 5 + dh * 2, rc.left + dw * 1, rc.top + dh * 4);
	m_rectC[eC_PPBOX_STOPPER_2_UP	]	= CRect(rc.left + 5 + dw * 1, rc.top + 5 + dh * 0, rc.left + dw * 2, rc.top + dh * 2);
	m_rectC[eC_PPBOX_STOPPER_2_DOWN	]	= CRect(rc.left + 5 + dw * 1, rc.top + 5 + dh * 2, rc.left + dw * 2, rc.top + dh * 4);

	m_rectC[eC_PPBOX_STOPPER_3_UP	]	= CRect(rc.left + 5 + dw * 2, rc.top + 5 + dh * 3, rc.left + dw * 3, rc.top + dh * 5);
	m_rectC[eC_PPBOX_STOPPER_3_DOWN	]	= CRect(rc.left + 5 + dw * 2, rc.top + 5 + dh * 5, rc.left + dw * 3, rc.top + dh * 7);

	m_rectC[eC_PPBOX_STOPPER_4_UP	]	= CRect(rc.left + 5 + dw * 0, rc.top + 5 + dh * 6, rc.left + dw * 1, rc.top + dh * 8);
	m_rectC[eC_PPBOX_STOPPER_4_DOWN	]	= CRect(rc.left + 5 + dw * 0, rc.top + 5 + dh * 8, rc.left + dw * 1, rc.top + dh * 10);
	m_rectC[eC_PPBOX_STOPPER_5_UP	]	= CRect(rc.left + 5 + dw * 1, rc.top + 5 + dh * 6, rc.left + dw * 2, rc.top + dh * 8);
	m_rectC[eC_PPBOX_STOPPER_5_DOWN	]	= CRect(rc.left + 5 + dw * 1, rc.top + 5 + dh * 8, rc.left + dw * 2, rc.top + dh * 10);

	//vacuum/
	rc = m_rectS[eS_AREA_TR_GLASS];
	rc.InflateRect(0, -15, 0, 0);
	dw = (rc.Width() - 5.) / 1.;
	dh = (rc.Height() - 5.) / 2.;
	//m_rectC[eC_GLASS_BLOW_ON]			= CRect(rc.left+5+dw*0, rc.top+5+dh*0, rc.left+dw*1, rc.top+dh*1);
	//m_rectC[eC_GLASS_BLOW_OFF]		= CRect(rc.left+5+dw*0, rc.top+5+dh*1, rc.left+dw*1, rc.top+dh*2);
	m_rectC[eC_GLASS_VACUUM_1_ON]		= CRect(rc.left+5+dw*0, rc.top+5+dh*0, rc.left+dw*1, rc.top+dh*1);
	m_rectC[eC_GLASS_VACUUM_1_OFF]		= CRect(rc.left+5+dw*0, rc.top+5+dh*1, rc.left+dw*1, rc.top+dh*2);
	//m_rectC[eC_GLASS_VACUUM_2_ON]		= CRect(rc.left+5+dw*2, rc.top+5+dh*0, rc.left+dw*3, rc.top+dh*1);
	//m_rectC[eC_GLASS_VACUUM_2_OFF]	= CRect(rc.left+5+dw*2, rc.top+5+dh*1, rc.left+dw*3, rc.top+dh*2);
	
	rc = m_rectS[eS_AREA_TR_PAPER];
	rc.InflateRect(0, -15, 0, 0);
	dw = (rc.Width() - 5.) / 2.;
	dh = (rc.Height() - 5.) / 2.;
	//m_rectC[eC_PAPER_BLOW_ON]			= CRect(rc.left+5+dw*0, rc.top+5+dh*0, rc.left+dw*1, rc.top+dh*1);
	//m_rectC[eC_PAPER_BLOW_OFF]		= CRect(rc.left+5+dw*0, rc.top+5+dh*1, rc.left+dw*1, rc.top+dh*2);
	m_rectC[eC_PAPER_VACUUM_ON]			= CRect(rc.left+5+dw*0, rc.top+5+dh*0, rc.left+dw*1, rc.top+dh*1);
	m_rectC[eC_PAPER_VACUUM_OFF]		= CRect(rc.left+5+dw*0, rc.top+5+dh*1, rc.left+dw*1, rc.top+dh*2);
	m_rectC[eC_LD_TR_PAPER_PUSHER_ON]	= CRect(rc.left+5+dw*1, rc.top+5+dh*0, rc.left+dw*2, rc.top+dh*1);
	m_rectC[eC_LD_TR_PAPER_PUSHER_OFF]	= CRect(rc.left+5+dw*1, rc.top+5+dh*1, rc.left+dw*2, rc.top+dh*2);

	rc = m_rectS[eS_AREA_BLOW];
	rc.InflateRect(0, -15, 0, 0);
	dw = (rc.Width() - 5.) / 1.;
	dh = (rc.Height() - 5.) / 2.;
	m_rectC[eC_PAPER_AND_GLASS_BLOW_ON]		= CRect(rc.left+5+dw*0, rc.top+5+dh*0, rc.left+dw*1, rc.top+dh*1);
	m_rectC[eC_PAPER_AND_GLASS_BLOW_OFF]	= CRect(rc.left+5+dw*0, rc.top+5+dh*1, rc.left+dw*1, rc.top+dh*2);

	rc = m_rectS[eS_AREA_MUTE];
	rc.InflateRect(0, -15, 0, 0);
	dw = (rc.Width() - 5.) / 1.;
	dh = (rc.Height() - 5.) / 2.;
	m_rectC[eC_MUTE_ON]						= CRect(rc.left+5+dw*0, rc.top+5+dh*0, rc.left+dw*1, rc.top+dh*1);
	m_rectC[eC_MUTE_OFF]					= CRect(rc.left+5+dw*0, rc.top+5+dh*1, rc.left+dw*1, rc.top+dh*2);
}
int CIOControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void CIOControl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		SetTimer(INTERVAL_IOCONTROWND, 100, nullptr);
	}
	else {
		KillTimer(INTERVAL_IOCONTROWND);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CIOControl::OnPaint()
{
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);

	memDC.FillRect(m_rectClient,&g_brushBackground[eEC_DEFAULT]);
	DrawGroupBox(&memDC, m_rectS[eS_AREA_PPBOX_ALIGN],	g_st[IDS_MANUAL_AREA_PPBOX_ALIGN]		,	RGB_BLACK, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_AREA_CV_PPBOX],		g_st[IDS_MANUAL_AREA_CV_PPBOX]			,	RGB_BLACK, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_AREA_TR_GLASS],		g_st[IDS_MANUAL_AREA_TR_GLASS_VAC]		,	RGB_BLACK, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_AREA_TR_PAPER],		g_st[IDS_MANUAL_AREA_TR_PAPER_VAC]		,	RGB_BLACK, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_AREA_BLOW],			g_st[IDS_MANUAL_AREA_GLASS_PAPER_BLOW]	,	RGB_BLACK, RGB_BLACK);
	DrawGroupBox(&memDC, m_rectS[eS_AREA_MUTE],			_T("MUTE ON/OFF")						,	RGB_BLACK, RGB_BLACK); // LJY 추가 2024.06.25

	auto IOControl = [&](const int _enum, const CString& _text, bool _onOff, bool _isWrite)
	{
		DrawMDIIOButton(memDC, m_rectC[_enum], _text, m_eSelectControl == _enum, m_eMouseHover == _enum, _onOff, _isWrite);
	};
	
	IOControl(eC_PPBOX_ALIGN_X_L_FWD	, _T("PPBox Aligner X_L FWD")		, GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_X_L),	GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_X_L));
	IOControl(eC_PPBOX_ALIGN_X_L_BWD	, _T("PPBox Aligner X_L BWD")		, GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_X_L),	GetPPBoxAlignerXY(TRUE,  TRUE,  ePPBOX_ALIGNER_X_L));
	IOControl(eC_PPBOX_ALIGN_X_R_FWD	, _T("PPBox Aligner X_R FWD")		, GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_X_R),	GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_X_R));
	IOControl(eC_PPBOX_ALIGN_X_R_BWD	, _T("PPBox Aligner X_R BWD")		, GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_X_R),	GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_X_R));
	IOControl(eC_PPBOX_ALIGN_Y_FWD		, _T("PPBox Aligner Y FWD")		    , GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_Y),		GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_Y));
	IOControl(eC_PPBOX_ALIGN_Y_BWD		, _T("PPBox Aligner Y BWD")		    , GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_Y),		GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_Y));
	IOControl(eC_PPBOX_ALIGN_ALL_FWD	, _T("Aligner All FWD")	            , GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_ALL),	GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_ALL));
	IOControl(eC_PPBOX_ALIGN_ALL_BWD	, _T("Aligner All BWD")	            , GetPPBoxAlignerXY(FALSE, TRUE,	ePPBOX_ALIGNER_ALL),	GetPPBoxAlignerXY(TRUE,  TRUE,	ePPBOX_ALIGNER_ALL));

	IOControl(eC_PPBOX_ALIGN_Z_UP		, _T("PPBox Aligner Y Up")		    , GetPPBoxAlignerZ(TRUE,  TRUE),							GetPPBoxAlignerZ(FALSE, TRUE));
	IOControl(eC_PPBOX_ALIGN_Z_DOWN		, _T("PPBox Aligner Y Down")		, GetPPBoxAlignerZ(FALSE, TRUE),							GetPPBoxAlignerZ(TRUE,  TRUE));
	
	IOControl(eC_PPBOX_STOPPER_1_UP		, _T("CV1 Stopper Up")	            , GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1,	TRUE,  TRUE),	GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1,		FALSE, TRUE));
	IOControl(eC_PPBOX_STOPPER_1_DOWN	, _T("CV1 Stopper Down")	        , GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1,	FALSE, TRUE),	GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1,		TRUE,  TRUE));
	IOControl(eC_PPBOX_STOPPER_2_UP		, _T("CV2 Stopper Up")	            , GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_2,	TRUE,  TRUE),	GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_2,		FALSE, TRUE));
	IOControl(eC_PPBOX_STOPPER_2_DOWN	, _T("CV2 Stopper Down")	        , GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_2,	FALSE, TRUE),	GetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_2,		TRUE,  TRUE));
	IOControl(eC_PPBOX_STOPPER_3_UP		, _T("CV3 Stopper Up")	            , GetConveyor_Stoper(eMODULE_PC1_ELEVATOR_CONVEYOR, TRUE,  TRUE),	GetConveyor_Stoper(eMODULE_PC1_ELEVATOR_CONVEYOR,	FALSE, TRUE));
	IOControl(eC_PPBOX_STOPPER_3_DOWN	, _T("CV3 Stopper Down")	        , GetConveyor_Stoper(eMODULE_PC1_ELEVATOR_CONVEYOR, FALSE, TRUE),	GetConveyor_Stoper(eMODULE_PC1_ELEVATOR_CONVEYOR,	TRUE,  TRUE));
	IOControl(eC_PPBOX_STOPPER_4_UP		, _T("CV4 Stopper Up")	            , GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1, TRUE,  TRUE),	GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1,	FALSE, TRUE));
	IOControl(eC_PPBOX_STOPPER_4_DOWN	, _T("CV4 Stopper Down")	        , GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1, FALSE, TRUE),   GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1,	TRUE,  TRUE));
	IOControl(eC_PPBOX_STOPPER_5_UP		, _T("CV5 Stopper Up")	            , GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_2, TRUE,  TRUE),	GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_2,	FALSE, TRUE));
	IOControl(eC_PPBOX_STOPPER_5_DOWN	, _T("CV5 Stopper Down")	        , GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_2, FALSE, TRUE),	GetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_2,	TRUE,  TRUE));

	IOControl(eC_GLASS_VACUUM_1_ON,			g_st[IDS_MANUAL_GLASS_VACUUM_ON	],	GetGlassTransferVacuum(eMT_ORG_GLASS, TRUE, TRUE)   ,GetGlassTransferVacuum(eMT_ORG_GLASS, TRUE));
	IOControl(eC_GLASS_VACUUM_1_OFF,		g_st[IDS_MANUAL_GLASS_VACUUM_OFF],	GetGlassTransferVacuum(eMT_ORG_GLASS, FALSE, TRUE)  ,GetGlassTransferVacuum(eMT_ORG_GLASS, FALSE)); 

	IOControl(eC_PAPER_VACUUM_ON,			g_st[IDS_MANUAL_PAPER_VACUUM_ON	],	GetGlassTransferVacuum(eMT_PAPER, TRUE, TRUE)	    ,GetGlassTransferVacuum(eMT_PAPER, TRUE));
	IOControl(eC_PAPER_VACUUM_OFF,			g_st[IDS_MANUAL_PAPER_VACUUM_OFF],	GetGlassTransferVacuum(eMT_PAPER, FALSE, TRUE)	    ,GetGlassTransferVacuum(eMT_PAPER, FALSE));
    
    IOControl(eC_LD_TR_PAPER_PUSHER_ON,		g_st[IDS_MANUAL_PUSHER_UP],		    GetGlassTransferCylinderPusher(TRUE, TRUE)			, GetGlassTransferCylinderPusher(TRUE));
	IOControl(eC_LD_TR_PAPER_PUSHER_OFF,	g_st[IDS_MANUAL_PUSHER_DOWN	],		GetGlassTransferCylinderPusher(FALSE, TRUE)			, GetGlassTransferCylinderPusher(FALSE));

	IOControl(eC_PAPER_AND_GLASS_BLOW_ON,	g_st[IDS_MANUAL_BLOW_ON],			GetGlassTransferBlow(TRUE, TRUE)					, GetGlassTransferBlow(TRUE));
	IOControl(eC_PAPER_AND_GLASS_BLOW_OFF,	g_st[IDS_MANUAL_BLOW_OFF],			GetGlassTransferBlow(FALSE, TRUE)					, GetGlassTransferBlow(FALSE));

	// LJY 추가 2024.06.25
	IOControl(eC_MUTE_ON,					_T("Mute On"),						GetMute(TRUE)										, GetMute(TRUE));
	IOControl(eC_MUTE_OFF,					_T("Mute Off"),						GetMute(FALSE)										, GetMute(FALSE));
}


void CIOControl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent) {
		case INTERVAL_IOCONTROWND: {
			InvalidateRect(m_rectS[eS_AREA_PPBOX_ALIGN], FALSE);
			InvalidateRect(m_rectS[eS_AREA_CV_PPBOX], FALSE);
			InvalidateRect(m_rectS[eS_AREA_TR_GLASS], FALSE);
			InvalidateRect(m_rectS[eS_AREA_TR_PAPER], FALSE);
			InvalidateRect(m_rectS[eS_AREA_BLOW], FALSE);
		}
	}
	CFrameWndEx::OnTimer(nIDEvent);
}


void CIOControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	for (int i = 0; i < eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			Invalidate(FALSE);
			break;
		}
	}
	CFrameWndEx::OnLButtonDown(nFlags, point);
}


void CIOControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	T_SEQUENCE_MODULE_INFO& pThis = sm.Module[eMODULE_PC1_UNLOAD_CONVEYOR_2];
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		//<< KES 추가 2024.4.9 - 운전모드가 Manual이 아니면 수동 동작 X
		if (sm.Option->tSequence.eDriveMode != eDRIVE_MODE_MANUAL) {
			IPMessage(g_st[IDS_MS_NOT_DRIVE_MODE_MANUAL]);
			return;
		}
		//>>	

		switch (m_eSelectControl) {
		    case eC_PPBOX_ALIGN_X_L_FWD:
		    case eC_PPBOX_ALIGN_X_L_BWD: {
		    	SetPPBoxAlignerXY(m_eSelectControl == eC_PPBOX_ALIGN_X_L_FWD, ePPBOX_ALIGNER_X_L);
		    	strLog = Format(_T("PPBOX Align X_L%s"), m_eSelectControl == eC_PPBOX_ALIGN_X_L_FWD ? _T("FWD") : _T("BWD"));
		    	break;
		    }
		    case eC_PPBOX_ALIGN_X_R_FWD:
		    case eC_PPBOX_ALIGN_X_R_BWD: {
				SetPPBoxAlignerXY(m_eSelectControl == eC_PPBOX_ALIGN_X_R_FWD, ePPBOX_ALIGNER_X_R);
		    	strLog = Format(_T("PPBOX Align X_R%s"), m_eSelectControl == eC_PPBOX_ALIGN_X_R_FWD ? _T("FWD") : _T("BWD"));
		    	break;
		    }
		    case eC_PPBOX_ALIGN_Y_FWD:
		    case eC_PPBOX_ALIGN_Y_BWD: {
		    	SetPPBoxAlignerXY(m_eSelectControl == eC_PPBOX_ALIGN_Y_FWD, ePPBOX_ALIGNER_Y);
		    	strLog = Format(_T("PPBOX Align Y%s"), m_eSelectControl == eC_PPBOX_ALIGN_Y_FWD ? _T("FWD") : _T("BWD"));
		    	break;
		    }
		    case eC_PPBOX_ALIGN_ALL_FWD:
		    case eC_PPBOX_ALIGN_ALL_BWD: {
		    	SetPPBoxAlignerXY(m_eSelectControl == eC_PPBOX_ALIGN_ALL_FWD, ePPBOX_ALIGNER_ALL);
		    	strLog = Format(_T("PPBOX Align %s"), m_eSelectControl == eC_PPBOX_ALIGN_ALL_FWD ? _T("FWD") : _T("BWD"));
		    	break;
		    }
			case eC_PPBOX_ALIGN_Z_UP: {
				if (!GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_Y)) {
					IPMessage(g_st[IDS_MANUAL_DO_NOT_UP_PLEASE_FWD]);
				}
				else {
					SetPPBoxAlignerZ(TRUE);
					strLog = _T("PPBOX Align Y UP");
				}
				break;

			}
		    case eC_PPBOX_ALIGN_Z_DOWN: {
				if (!GetPPBoxAlignerXY(FALSE, TRUE, ePPBOX_ALIGNER_Y)) {
					IPMessage(g_st[IDS_MANUAL_DO_NOT_DOWN_PLEASE_FWD]);
				}
				else {
		    		SetPPBoxAlignerZ(FALSE);
					strLog = _T("PPBOX Align Y DOWN");
				}
		    	break;
		    }
		    case eC_PPBOX_STOPPER_1_UP:
		    case eC_PPBOX_STOPPER_1_DOWN: {
		    	SetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_1, m_eSelectControl == eC_PPBOX_STOPPER_1_UP);
		    	strLog = Format(_T("PPBOX Stopper %s"), m_eSelectControl == eC_PPBOX_STOPPER_1_UP ? _T("Up") : _T("Down"));
		    	break;
		    }
		    case eC_PPBOX_STOPPER_2_UP:
		    case eC_PPBOX_STOPPER_2_DOWN: {
		    	SetConveyor_Stoper(eMODULE_PC1_LOAD_CONVEYOR_2, m_eSelectControl == eC_PPBOX_STOPPER_2_UP);
		    	strLog = Format(_T("PPBOX Stopper %s"), m_eSelectControl == eC_PPBOX_STOPPER_2_UP ? _T("Up") : _T("Down"));
		    	break;
		    }
		    case eC_PPBOX_STOPPER_3_UP:
		    case eC_PPBOX_STOPPER_3_DOWN: {
		    	SetConveyor_Stoper(eMODULE_PC1_ELEVATOR_CONVEYOR, m_eSelectControl == eC_PPBOX_STOPPER_3_UP);
		    	strLog = Format(_T("PPBOX Stopper %s"), m_eSelectControl == eC_PPBOX_STOPPER_3_UP ? _T("Up") : _T("Down"));
		    	break;
		    }
		    case eC_PPBOX_STOPPER_4_UP:
		    case eC_PPBOX_STOPPER_4_DOWN: {
		    	SetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_1, m_eSelectControl == eC_PPBOX_STOPPER_4_UP);
		    	strLog = Format(_T("PPBOX Stopper %s"), m_eSelectControl == eC_PPBOX_STOPPER_4_UP ? _T("Up") : _T("Down"));
		    	break;
		    }
		    case eC_PPBOX_STOPPER_5_UP:
		    case eC_PPBOX_STOPPER_5_DOWN: {
		    	SetConveyor_Stoper(eMODULE_PC1_UNLOAD_CONVEYOR_2, m_eSelectControl == eC_PPBOX_STOPPER_5_UP);
		    	strLog = Format(_T("PPBOX Stopper %s"), m_eSelectControl == eC_PPBOX_STOPPER_5_UP ? _T("Up") : _T("Down"));
		    	break;
			}
			case eC_GLASS_VACUUM_1_ON: {
				SetGlassTransferVacuum(eMT_ORG_GLASS, TRUE);
				strLog = _T("Transfer Glass Vacuum On");
				break;
			}
			case eC_GLASS_VACUUM_1_OFF: {
				SetGlassTransferVacuum(eMT_ORG_GLASS, FALSE);
				strLog = _T("Transfer Glass Vacuum Off");
				break;
			}

			case eC_PAPER_VACUUM_ON:
			case eC_PAPER_VACUUM_OFF: {
				SetGlassTransferVacuum(eMT_PAPER, m_eSelectControl == eC_PAPER_VACUUM_ON);
				strLog = Format(_T("Transfer Paper Vacuum %s"), m_eSelectControl == eC_PAPER_VACUUM_ON ? _T("On") : _T("Off"));
				break;
			}
			case eC_PAPER_PUSHER_VACUUM_ON:
			case eC_PAPER_PUSHER_VACUUM_OFF: {
				//SetGlassTransferPaperPusherVacuum(m_eSelectControl == eC_PAPER_PUSHER_VACUUM_ON);
				strLog = Format(_T("Transfer Paper Pusher Vacuum %s"), m_eSelectControl == eC_PAPER_PUSHER_VACUUM_ON ? _T("On") : _T("Off"));
				break;
			}
			case eC_PAPER_AND_GLASS_BLOW_ON:
			case eC_PAPER_AND_GLASS_BLOW_OFF: {
				SetGlassTransferBlow(m_eSelectControl == eC_PAPER_AND_GLASS_BLOW_ON);
				strLog = Format(_T("Transfer Blow %s"), m_eSelectControl == eC_PAPER_AND_GLASS_BLOW_ON ? _T("On") : _T("Off"));
				break;
			}
            case eC_LD_TR_PAPER_PUSHER_ON:
            case eC_LD_TR_PAPER_PUSHER_OFF: {
                SetGlassTransferCylinderPusher(m_eSelectControl == eC_LD_TR_PAPER_PUSHER_ON);
                strLog = Format(_T("Transfer Cylinder Pusher %s"), m_eSelectControl == eC_LD_TR_PAPER_PUSHER_ON ? _T("Up") : _T("Down"));
                break;
            }
			// LJY 추가 2024.06.25
			case eC_MUTE_ON:
			case eC_MUTE_OFF: {
				SetMute(pThis.eModule, m_eSelectControl == eC_MUTE_ON);
				strLog = Format(_T("Muting %s"), m_eSelectControl == eC_MUTE_ON ? _T("On") : _T("Off"));
				break;
			}
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}

	m_eSelectControl = eC_NONE;
	Invalidate(FALSE);
	CFrameWndEx::OnLButtonUp(nFlags, point);
}

void CIOControl::OnMouseMove(UINT nFlags, CPoint point)
{
	// 이전 위치 저장
	int oldControl = m_eMouseHover;
	m_eMouseHover = eC_NONE;

	// 현재 위치 찾기 및 새로 그리기
	for (int idx = 0; idx < eC_END; idx++)
	{
		if (m_rectC[idx].PtInRect(point))
		{
			m_eMouseHover = idx;
			InvalidateRect(m_rectC[idx], FALSE);
			break;;
		}
	}

	// 이전 위치와 현재 위치가 다른 경우 이전 위치 다시 그리기
	// 마우스 위치가 이전 위치와 다른 경우 선택 위치도 초기화
	if (oldControl != eC_NONE && oldControl != m_eMouseHover)
	{
		m_eSelectControl = eC_NONE;
		InvalidateRect(m_rectC[oldControl], FALSE);
	}

	CFrameWndEx::OnMouseMove(nFlags, point);
}

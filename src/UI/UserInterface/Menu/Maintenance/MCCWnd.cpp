#include "pch.h"
#include "UI.h"
#include "MCCWnd.h"

CMCCWnd::CMCCWnd(CWnd* pwndParent) 
	: IWNDIPCommandTarget(m_hWnd, &g_ipClient)
{}

CMCCWnd::~CMCCWnd() {}


void CMCCWnd::ComputeArea() 
{
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect rc = m_rectS[eS_FULL_AREA];
	rc.DeflateRect(1, 1);
	double dH = (rc.Height() - 5) / 15.;
	double dW = (rc.Width() - 5) / 2.;

	m_rectS[eS_TITLE_AREA	]	= CRect(rc.left + 5 + dW * 0, rc.top + 5 +	dH * 0, rc.left + dW * 2, rc.top + dH * 1);
	m_rectS[eS_MCC_AREA		]	= CRect(rc.left + 5 + dW * 0, rc.top + 10 + dH * 1, rc.left + dW * 2, rc.top + dH * 15);

	m_lstMCC.MoveWindow(m_rectS[eS_MCC_AREA]);
	m_lstMCC.ShowWindow(SW_SHOW);
}

BEGIN_IP_COMMAND_TABLE(CMCCWnd, IIPCommandTarget)
	ON_IP("Add", "TactTime", OnIP_AddTactTime)
END_IP_COMMAND_TABLE()

BOOL CMCCWnd::OnIP_AddTactTime(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;

	// 각 제어 PC별로 다르게
	int Module = var.GetChildItem("Module", eMODULE_NONE) - eMODULE_PC1_BEGIN;
	__uint64 TactTime = var.GetChildItem("TactTime", 0ull);

	auto& Times = m_tactTimes[Module];

	Times.push_front(TactTime);
	if (Times.size() > TACT_TIME_COUNT)
	{
		Times.pop_back();
	}

	// 현재 텍 타임 데이터 표시
	std::list<__uint64> temps(Times.begin(), Times.end());
	int iPos = 0;
	for (auto& data : temps)
	{
		m_lstMCC.SetItemText(Module, eLI_TACT_TIME + iPos++, Format(L"%.3f", data * 0.001));
	}

	// 전체 평균 구하기
	__uint64 average = 0;
	for (auto& data : temps)
	{
		average += data;
	}
	average /= temps.size();
	m_lstMCC.SetItemText(Module, eLI_ALL_AVG, Format(L"%.3f", average * 0.001));

	// 상위/하위 10% 제외평균 구하기
	__uint64 excludeCount = temps.size() / 10;
	for (__uint64 idx = 0; idx < excludeCount; ++idx)
	{
		auto maxIter = std::max_element(temps.begin(), temps.end());
		auto minIter = std::min_element(temps.begin(), temps.end());

		temps.erase(maxIter);
		temps.erase(minIter);
	}
	average = 0;
	for (auto& data : temps)
	{
		average += data;
	}
	average /= temps.size();
	m_lstMCC.SetItemText(Module, eLI_AVG, Format(L"%.3f", average * 0.001));

	return TRUE;
}

IMPLEMENT_DYNCREATE		(CMCCWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CMCCWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
END_MESSAGE_MAP			()

BOOL CMCCWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

int CMCCWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_lstMCC.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, m_rectS[eS_MCC_AREA], this, 10001);
	m_lstMCC.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_lstMCC.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_lstMCC.SetRowHeight(45);
	m_lstMCC.ShowScrollBar(SB_VERT);
	m_lstMCC.ShowScrollBar(SB_HORZ);
	m_lstMCC.InsertColumn(eLI_NONE			, _T(""), LVCFMT_LEFT, 0, 1);
	m_lstMCC.InsertColumn(eLI_TITLE			, _T(""), LVCFMT_LEFT, 350, 1);
	m_lstMCC.InsertColumn(eLI_ALL_AVG		, _T("ALL AVERAGE"), LVCFMT_CENTER, 100, 1);
	m_lstMCC.InsertColumn(eLI_AVG			, _T("AVERAGE"), LVCFMT_CENTER, 65, 1);
	m_lstMCC.InsertColumn(eLI_TACT_TIME		, _T("TACT TIME"), LVCFMT_CENTER, 75, 1);
	
	for (int j = 1; j < TACT_TIME_COUNT; ++j)
	{
		m_lstMCC.InsertColumn(eLI_TACT_TIME + j, Format(_T("%s %d"), _T("BEFORE"), j), LVCFMT_CENTER, 75, 1);
	}

	// 각 제어 PC별로 다르게
	const int nModule = eMODULE_PC1_END - eMODULE_PC1_BEGIN;

	for (int iModule = 0; iModule < nModule; ++iModule)
	{
		m_lstMCC.InsertItem(iModule, _T(""));
		m_lstMCC.SetItemText(iModule, eLI_TITLE, moduleNameStep[iModule]);
		m_lstMCC.SetSubItemBKColor(eLI_TACT_TIME, RGB_GREEN_LIGHT);
		for (int j = 1; j < TACT_TIME_COUNT; ++j) 
		{
			m_lstMCC.SetSubItemBKColor(eLI_TACT_TIME+j, RGB_GRAY_LIGHT);
		}
	}

	m_tactTimes.resize(nModule);
	return 0;
}


void CMCCWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) 
	{
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		ComputeArea();
		Invalidate(FALSE);
	}
}


BOOL CMCCWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}


void CMCCWnd::OnPaint() 
{
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CRect rect			= m_rectS[eS_FULL_AREA];
	memDC.FillSolidRect(rect, g_colorBackground[eEC_DISABLE]);
	rect.DeflateRect(3, 3, 0, 0);
	CDrawSymbol::DrawLineRect(&memDC, rect, g_colorForeground[eEC_DEFAULT]);
	SelectedDrawText(&memDC, m_rectS[eS_TITLE_AREA], _T("M C C"), 15, RGB_GRAY, RGB_GRAY, RGB_WHITE);
}

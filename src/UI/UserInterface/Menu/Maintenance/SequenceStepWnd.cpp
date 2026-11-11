#include "pch.h"
#include "UI.h"
#include "SequenceStepWnd.h"


// CSequenceStepWnd

IMPLEMENT_DYNCREATE(CSequenceStepWnd, CFrameWndEx)

CSequenceStepWnd::CSequenceStepWnd(CWnd* pwndParent)
	: m_pwndParent(pwndParent)
{}

CSequenceStepWnd::~CSequenceStepWnd()
{}

// CSequenceStepWnd 메시지 처리기


void CSequenceStepWnd::ComputeArea()
{
	GetClientRect(m_rectS[eS_FULL_AREA]);

	CRect	rect = m_rectS[eS_FULL_AREA];
	rect.DeflateRect(1, 1);
	double	dW = (rect.Width() -5. )/ 7.;
	double	dH = (rect.Height() -5.)/ 20.;

	// 공간별 분할
	m_rectS[eS_CAL_AREA]			= CRect(rect.left + dW * 0 + 5, rect.top + dH * 0 + 5, rect.left + dW * 1, rect.top + dH * 7);

	m_rectS[eS_TAB_AREA]			= CRect(rect.left + dW * 1 + 5, rect.top + dH * 0 + 5, rect.left + dW * 7	, rect.top + dH * 1);
	m_rectS[eS_MODULE_INFO_AREA]	= CRect(rect.left + dW * 1 + 5, rect.top + dH * 1 + 5, rect.left + dW * 7	, rect.top + dH * 20);

	m_rectS[eS_STEP_AREA_1]			= CRect(rect.left + dW * 1 + 5, rect.top + dH * 1 + 5, rect.left + dW * 4	, rect.top + dH * 20);
	m_rectS[eS_STEP_AREA_2]			= CRect(rect.left + dW * 4 + 5, rect.top + dH * 1 + 5, rect.left + dW * 7	, rect.top + dH * 20);


	rect = m_rectS[eS_TAB_AREA];
	dW = (rect.Width() - 5.) / 12.;
	dH = (rect.Height() - 5.) / 1.;
	m_rectS[eS_STEP_COMBO_1		] = CRect(rect.left + dW * 0 + 5, rect.top + dH * 0 + 5, rect.left + dW * 2, rect.top + dH * 1);
	m_rectS[eS_STEP_COMBO_2		] = CRect(rect.left + dW * 6 + 5, rect.top + dH * 0 + 5, rect.left + dW * 8, rect.top + dH * 1);

	for (int i = 0; i < nSEQUENCE_LIST; i++) {
		if (m_moduleStepListctrl[i].m_hWnd) {
			m_moduleStepListctrl[i].MoveWindow(m_rectS[eS_STEP_AREA_1 + i]);
		}

		if (m_cboModule[i]) {
			CRect rc = m_rectS[eS_STEP_COMBO_1 + i];
			m_cboModule[i].MoveWindow(rc);
			::SendMessage(m_cboModule[i].m_hWnd, CB_SETITEMHEIGHT, (WPARAM)-1, rc.Height() - 6);
			m_cboModule[i].ShowWindow(SW_SHOW);
		}
	}
	if (m_MonthCal != NULL)
		m_MonthCal->MoveWindow(m_rectS[eS_CAL_AREA]);
}

void CSequenceStepWnd::UpdateSequenceStep(int iIndex)
{
	auto moduleListFunc = [&](eMODULEs _eModule)
	{
		if (m_moduleStepListctrl[iIndex].GetItemCount()) 
		{
			m_moduleStepListctrl[iIndex].DeleteAllItems();
		}
		
		CTime cTime;
		m_MonthCal->GetCurSel(cTime);
		CString strPath = GetSeqLogFilePath(_eModule, cTime);

		CFile file;
		if (FALSE == file.Open(strPath, CFile::modeRead | CFile::shareDenyNone))
		{
			return;
		}
	
		CString strLog;
		int itemCount = 0;
		CArchiveU	arU(&file, CArchive::load);
		arU.ReadStringU(strLog); // 첫번째 줄은 필요 없는 내용
		while (arU.ReadStringU(strLog) != FALSE) 
		{
			CStrings contents;
			SplitString(strLog, _T(";"), contents);

			m_moduleStepListctrl[iIndex].AddItem(_T(""));
			for (int iColumn = 0; iColumn < contents.size(); ++iColumn)
			{
				m_moduleStepListctrl[iIndex].SetItemText(itemCount, iColumn, contents[iColumn]);
			}
			itemCount++;
		}
		m_moduleStepListctrl[iIndex].Complete();
		arU.Close();
		file.Close();
	};
	// LD 
	if (m_cboModule[iIndex].GetCurSel() != -1)
	{
		moduleListFunc(eMODULEs(m_cboModule[iIndex].GetCurSel() + eMODULE_PC1_BEGIN));
	}
}

#define IDC_SEQUENCE_STEP_LIST WM_APP + 5305
#define IDC_SEQUENCE_STEP_COMBO WM_APP + 5310
#define IDD_LOG_CAL	6223
BEGIN_MESSAGE_MAP(CSequenceStepWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_NOTIFY(MCN_SELECT, IDD_LOG_CAL, OnCalSelected)
	ON_LBN_SELCHANGE(IDC_SEQUENCE_STEP_COMBO,	OnSelchangeCombo1)
	ON_LBN_SELCHANGE(IDC_SEQUENCE_STEP_COMBO+1, OnSelchangeCombo2)
END_MESSAGE_MAP()

BOOL CSequenceStepWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
}

int CSequenceStepWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_MonthCal == NULL)
	{
		m_MonthCal = new CMonthCalCtrl();
		m_MonthCal->Create(WS_BORDER | WS_CHILD | WS_VISIBLE | MCS_NOTODAYCIRCLE, CPoint2d(0, 0), this, IDD_LOG_CAL);

		CFont* font = new CFont();
		font->CreateFontW(20, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, DT_FONT);

		m_MonthCal->SetFont(font);
	}

	for (int i = 0; i < nSEQUENCE_LIST; i++) {
		m_moduleStepListctrl[i].Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_OWNERDATA, CRect(), this, IDC_SEQUENCE_STEP_LIST + i);
		m_moduleStepListctrl[i].SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

		m_moduleStepListctrl[i].InsertColumn(eSSC_TIME			,	_T("TIME")					, LVCFMT_CENTER	, 100	);
		m_moduleStepListctrl[i].InsertColumn(eSSC_CUR_STEP		,	_T("CURRENT STEP")			, LVCFMT_CENTER	, 100	);
		m_moduleStepListctrl[i].InsertColumn(eSSC_NEXT_STEP		,	_T("NEXT STEP")				, LVCFMT_CENTER	, 90	);
		m_moduleStepListctrl[i].InsertColumn(eSSC_NAME			,	_T("CURRENT STEP NAME")		, LVCFMT_CENTER	, 250	);
		m_moduleStepListctrl[i].InsertColumn(eSSC_DESCRIPTION	,	_T("DESCRIPTION")			, LVCFMT_LEFT	, 400	);

		m_cboModule[i].Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | CBS_DROPDOWNLIST, CRect(), this, IDC_SEQUENCE_STEP_COMBO + i);
		m_cboModule[i].SetCurSel(-1);
		for (int iModule = 0; iModule < countof(moduleNameStep); iModule++) {
			m_cboModule[i].AddString(moduleNameStep[iModule]);
		}		
	}
	m_MonthCal->SetCurSel(CTime::GetCurrentTime());

	return 0;
}


void CSequenceStepWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		ComputeArea();
		Invalidate();
	}
}


void CSequenceStepWnd::OnPaint()
{
	CPaintDC dc(this);
	CMemDCEx::CMemDC memDC(&dc);
	memDC.FillSolidRect(m_rectS[eS_FULL_AREA], g_colorBackground[eEC_DEFAULT]);

	for (int idx = eS_TAB_AREA; idx < eS_END; ++idx)
	{
		DrawLineRect(memDC, m_rectS[idx]);
	}
}

void CSequenceStepWnd::OnCalSelected(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateSequenceStep(eSEQUENCE_LIST_1);
	UpdateSequenceStep(eSEQUENCE_LIST_2);
}

void CSequenceStepWnd::OnSelchangeCombo1()
{
	UpdateSequenceStep(eSEQUENCE_LIST_1);
}

void CSequenceStepWnd::OnSelchangeCombo2()
{
	UpdateSequenceStep(eSEQUENCE_LIST_2);
}
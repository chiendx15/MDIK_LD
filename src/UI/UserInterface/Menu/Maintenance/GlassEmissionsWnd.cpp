#include "pch.h"
#include "UI.h"
#include "GlassEmissionsWnd.h"


IMPLEMENT_DYNCREATE(CGlassEmissionsWnd, CFrameWndEx)

#define IDD_LOG_LIST_MONTH	61231
#define IDD_LOG_LIST_DAY	61241
#define IDD_LOG_LIST_TIME	61251
#define IDD_LOG_CBO_YEAR	61261
#define TIMER_LIST_UPDATE	8912

CGlassEmissionsWnd::CGlassEmissionsWnd(CWnd* pwndParent)
{
	m_pwndParent	= pwndParent;
	m_eSelectControl= eC_NONE;
}

CGlassEmissionsWnd::~CGlassEmissionsWnd()
{
}

BEGIN_MESSAGE_MAP(CGlassEmissionsWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_INITMENU()
	ON_WM_TIMER()
END_MESSAGE_MAP()


int CGlassEmissionsWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_lst[eLL_M].Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, CRect(), this, IDD_LOG_LIST_MONTH);
	m_lst[eLL_M].SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_lst[eLL_M].ModifyStyle(0, LVS_SHOWSELALWAYS);

	m_lst[eLL_D].Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, CRect(), this, IDD_LOG_LIST_DAY);
	m_lst[eLL_D].SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_lst[eLL_D].ModifyStyle(0, LVS_SHOWSELALWAYS);

	m_lst[eLL_H].Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER, CRect(), this, IDD_LOG_LIST_TIME);
	m_lst[eLL_H].SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_lst[eLL_H].ModifyStyle(0, LVS_SHOWSELALWAYS);

	LPCTSTR lpName[] = {
		_T(""),
		_T("MONTH")			,
        _T("DAY")			,
        _T("TIME")			,
        _T("INPUT")			,
        _T("OUTPUT")		,
        _T("OK")			,
        _T("NG")			,
        _T("YIELD(%)")		,
	};

	for (int i = 0; i < nLIST; ++i) {
		m_lst[i].InsertColumn(eCN_NONE,		lpName[0],		LVCFMT_CENTER, 0,	1);
		m_lst[i].InsertColumn(eCN_DATE,		lpName[1+i],	LVCFMT_CENTER, 120,	1);
		m_lst[i].InsertColumn(eCN_INPUT,	lpName[4],		LVCFMT_CENTER, 150,	1);
		m_lst[i].InsertColumn(eCN_OUTPUT,	lpName[5],		LVCFMT_CENTER, 150,	1);
	}

	for (int i = 0; i < 12; ++i) {
		m_lst[eLL_M].InsertItem(i, _T(""));
		m_lst[eLL_M].SetItemText(i, eCN_DATE, Format(_T("%d"), i+1));
	}

	for (int i = 0; i < 31; ++i) {
		m_lst[eLL_D].InsertItem(i, _T(""));
		m_lst[eLL_D].SetItemText(i, eCN_DATE, Format(_T("%d"), i+1));
	}

	for (int i = 0; i < 24; ++i) {
		m_lst[eLL_H].InsertItem(i, _T(""));
		m_lst[eLL_H].SetItemText(i, eCN_DATE, Format(_T("%d - %d"), i, i+1));
	}

	CTime time = CTime::GetCurrentTime();
	m_lst[eLL_M].SetMark(time.GetMonth()-1);
	m_lst[eLL_D].SetMark(time.GetDay()-1);
	m_lst[eLL_H].SetMark(time.GetHour());

	m_cboYear.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | CBS_DROPDOWNLIST, CRect(), this, IDD_LOG_CBO_YEAR);
	m_cboYear.Clear();

	int iSelCbo = 0;
	for (int i = 0; i < GLASS_COUNT_YEAR; ++i) {
		if (time.GetYear() == STANDARD_YEAR+i)
			iSelCbo = i;

		m_cboYear.AddString(Format(_T("%d"), STANDARD_YEAR+i));
	}

	m_cboYear.SetCurSel(iSelCbo);
	return 0;
}


void CGlassEmissionsWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left,g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl = eC_NONE;

		int iM = m_lst[eLL_M].GetSelectionMark();
		int iD = m_lst[eLL_D].GetSelectionMark();
		int iH = m_lst[eLL_H].GetSelectionMark();

		if (iM >= 0 && iM < m_lst[eLL_M].GetItemCount())
			m_lst[eLL_M].SetMark(iM);
		if (iD >= 0 && iD < m_lst[eLL_D].GetItemCount())
			m_lst[eLL_D].SetMark(iD);
		if (iH >= 0 && iH < m_lst[eLL_H].GetItemCount())
			m_lst[eLL_H].SetMark(iH);

		SetTimer(TIMER_LIST_UPDATE, 100, nullptr);
	}
	else {
		KillTimer(TIMER_LIST_UPDATE);
	}
}

void CGlassEmissionsWnd::fn_ComputeArea() {
	GetClientRect(m_rectClient);

	m_rectS[eS_FULL_AREA] = m_rectClient;
	CRect rc = m_rectS[eS_FULL_AREA];
	double dW = (rc.Width() - 20.) / 20;
	double dH = (rc.Height() - 20.) / 12;

	m_rectS[eS_TITLE]		= CRect(rc.left+20+dW*0,	rc.top+20+dH*0,		rc.left+dW*4,	rc.top+dH*1);
	m_rectS[eS_CBO_YEAR]	= CRect(rc.left+20+dW*4,	rc.top+20+dH*0,		rc.left+dW*8,	rc.top+dH*1);
	m_rectS[eS_YEAR_COUNT]	= CRect(rc.left+10+dW*8,	rc.top+20+dH*0,		rc.left+dW*14,	rc.top+dH*1.2);

	m_rectC[eC_LOAD]		= CRect(rc.left+dW*18,	rc.top+20+dH*0,		rc.left+dW*19,	rc.top+dH*1);
	m_rectC[eC_SAVE]		= CRect(rc.left+dW*19,	rc.top+20+dH*0,		rc.left+dW*20,	rc.top+dH*1);

	dW = (rc.Width() - 20.) / 3;
	m_rectS[eS_LIST_MONTH]	= CRect(rc.left+20+dW*0,	rc.top+20+dH*1.2,	rc.left+dW*1,	rc.top+dH*12);
	m_rectS[eS_LIST_DAY	]	= CRect(rc.left+20+dW*1,	rc.top+20+dH*1.2,	rc.left+dW*2,	rc.top+dH*12);
	m_rectS[eS_LIST_TIME]	= CRect(rc.left+20+dW*2,	rc.top+20+dH*1.2,	rc.left+dW*3,	rc.top+dH*12);

	rc = m_rectS[eS_YEAR_COUNT];
	dW = (rc.Width() - 5) / 5.;
	dH = (rc.Height() - 5) / 5.;
	m_rectS[eS_INPUT]		= CRect(rc.left+dW*0,		rc.top+dH*0,		rc.left+1+dW*1,	rc.top+1+dH*2);
	m_rectS[eS_OUTPUT]		= CRect(rc.left+dW*1,		rc.top+dH*0,		rc.left+1+dW*2,	rc.top+1+dH*2);

	m_rectC[eC_INPUT]		= CRect(rc.left+dW*0,		rc.top+dH*2,		rc.left+1+dW*1,	rc.top+dH*5);
	m_rectC[eC_OUTPUT]		= CRect(rc.left+dW*1,		rc.top+dH*2,		rc.left+1+dW*2,	rc.top+dH*5);

	for (int i = 0; i < nLIST; ++i) {
		if (m_lst[i].m_hWnd) {
			m_lst[i].MoveWindow(m_rectS[eS_LIST_MONTH+i]);
			m_lst[i].ShowWindow(SW_SHOW);
		}
	}
	
	if (m_cboYear) {
		m_cboYear.MoveWindow(m_rectS[eS_CBO_YEAR]);
		m_cboYear.ShowWindow(SW_SHOW);
	}
}

void CGlassEmissionsWnd::OnPaint()
{
	CPaintDC dc(this); 
	CMemDCEx::CMemDC		memDC(&dc);
	
	CDrawSymbol::DrawLineRect(&memDC, m_rectS[eS_FULL_AREA], RGB_GRAY);
	CRect rect =m_rectS[eS_FULL_AREA];
	rect.DeflateRect(1, 1);
	memDC.FillSolidRect(rect, g_colorBackground[eEC_DEFAULT]);
	SelectedDrawTextB(&memDC, m_rectS[eS_TITLE],g_st[IDS_GLASS_EMISSIONS_WND_GLASS_OUTPUT_RATE],    FW_HEAVY, 12, RGB_GRAY, RGB_GRAY, RGB_WHITE, DT_CENTER|DT_VCENTER);

	SelectedDrawTextB(&memDC, m_rectS[eS_INPUT],	g_st[IDS_GLASS_EMISSIONS_WND_INPUT_VALUE],		FW_HEAVY, 9, RGB_GRAY_DARK, RGB_GRAY, RGB_WHITE, DT_CENTER|DT_VCENTER);
	SelectedDrawTextB(&memDC, m_rectS[eS_OUTPUT],	g_st[IDS_GLASS_EMISSIONS_WND_OUTPUT_VALUE],		FW_HEAVY, 9, RGB_GRAY_DARK, RGB_GRAY, RGB_WHITE, DT_CENTER|DT_VCENTER);
	//SelectedDrawTextB(&memDC, m_rectS[eS_OK],		_T("OK"),			FW_HEAVY, 9, RGB_GRAY_DARK, RGB_GRAY, RGB_WHITE, DT_CENTER|DT_VCENTER);
	//SelectedDrawTextB(&memDC, m_rectS[eS_NG],		_T("NG"),			FW_HEAVY, 9, RGB_GRAY_DARK, RGB_GRAY, RGB_WHITE, DT_CENTER|DT_VCENTER);
	//SelectedDrawTextB(&memDC, m_rectS[eS_RATE],	_T("양품율"),		FW_HEAVY, 9, RGB_GRAY_DARK, RGB_GRAY, RGB_WHITE, DT_CENTER|DT_VCENTER);

	int iY = m_cboYear.GetCurSel();
	auto GetValueI = [](const int value) {
		return value > 0 ? Format(_T("%d"), value) : _T("");
	};
	auto GetValueD = [](const double value) {
		return value > 0 ? Format(_T("%.3f"), value) : _T("");
	};

	SelectedDrawTextB(&memDC, m_rectC[eC_INPUT],	GetValueI(sm.GlassEmissions->y[iY].count.nIn),		FW_HEAVY, 9, RGB_GRAY_DARK, RGB_GRAY_LIGHT, RGB_BLACK, DT_CENTER|DT_VCENTER);
	SelectedDrawTextB(&memDC, m_rectC[eC_OUTPUT],	GetValueI(sm.GlassEmissions->y[iY].count.nOut),		FW_HEAVY, 9, RGB_GRAY_DARK, RGB_GRAY_LIGHT, RGB_BLACK, DT_CENTER|DT_VCENTER);
	//SelectedDrawTextB(&memDC, m_rectC[eC_OK],		GetValueI(sm.GlassEmissions->y[iY].count.nOK),		FW_HEAVY, 9, RGB_GRAY_DARK, RGB_GRAY_LIGHT, RGB_BLACK, DT_CENTER|DT_VCENTER);
	//SelectedDrawTextB(&memDC, m_rectC[eC_NG],		GetValueI(sm.GlassEmissions->y[iY].count.nNG),		FW_HEAVY, 9, RGB_GRAY_DARK, RGB_GRAY_LIGHT, RGB_BLACK, DT_CENTER|DT_VCENTER);
	//SelectedDrawTextB(&memDC, m_rectC[eC_RATE],	GetValueD(sm.GlassEmissions->y[iY].count.dRate),	FW_HEAVY, 9, RGB_GRAY_DARK, RGB_GRAY_LIGHT, RGB_BLACK, DT_CENTER|DT_VCENTER);

	DrawMDIMenuButtonN(&memDC, m_rectC[eC_LOAD], g_st[IDS_RELOAD],	m_eSelectControl == eC_LOAD);
	DrawMDIMenuButtonN(&memDC, m_rectC[eC_SAVE], g_st[IDS_SAVE],	m_eSelectControl == eC_SAVE);
}

void CGlassEmissionsWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	for (int i=0; i<eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl			= i;
			InvalidateRect(m_rectC[i],FALSE);
			break;
		}
	}

	CFrameWndEx::OnLButtonDown(nFlags, point);
}

void CGlassEmissionsWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_LOAD: {
				CString			strInitial;
				CFileDialog		file(TRUE, _T("dmp"), NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
					_T("dmp files(*.dmp)|*.dmp|All Files(*.*)|*.*||"));
				GetFullPathName(_T("D:\\MDIK\\"), 1024, strInitial.GetBuffer(1024), NULL);
				strInitial.ReleaseBuffer();
				file.m_ofn.lpstrInitialDir = strInitial;
				if (file.DoModal() == IDOK) {
					CString strFullPath, strPath, strTitle, strExt;
					strFullPath = file.GetPathName();

					CFile f;
					CFileException e;
					f.Open(strFullPath, CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeRead, &e);
					if (e.m_cause != CFileException::none)
					{
						TRACE(_T("%s File Exception Error : %d\n"), strFullPath, e.m_cause);
						return;
					}
					CArchive ar(&f, CArchive::load);
					std::vector<BYTE> buf;
					buf.resize(sizeof(T_GLASS_EMISSIONS));
					ar >> buf;
					ar.Close();
					f.Close();
					memcpy(sm.GlassEmissions, buf.data(), sizeof(T_GLASS_EMISSIONS));
				}
				break;
			}
			case eC_SAVE: {
				CString			strInitial;
				CFileDialog		file(FALSE, _T("dmp"), NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
					_T("dmp files(*.dmp)|*.dmp|All Files(*.*)|*.*||"));
				GetFullPathName(_T("D:\\MDIK\\"), 1024, strInitial.GetBuffer(1024), NULL);
				strInitial.ReleaseBuffer();
				file.m_ofn.lpstrInitialDir = strInitial;

				if (file.DoModal() == IDOK) {
					CString strFullPath, strPath, strTitle, strExt;
					strFullPath = file.GetPathName();

					CFile f;
					CFileException e;
					f.Open(strFullPath, CFile::shareDenyWrite | CFile::modeNoTruncate | CFile::modeWrite | CFile::modeCreate, &e);
					if (e.m_cause != CFileException::none)
					{
						TRACE(_T("%s File Exception Error : %d\n"), strFullPath, e.m_cause);
						return;
					}
					CArchive ar(&f, CArchive::store);
					std::vector<BYTE> buf;
					buf.resize(sizeof(T_GLASS_EMISSIONS));
					memcpy(buf.data(), sm.GlassEmissions, sizeof(T_GLASS_EMISSIONS));
					ar << buf;
					ar.Close();
					f.Close();
				}

				break;
			}
		}
	}
	m_eSelectControl		= eC_NONE;
	Invalidate(FALSE);
	CFrameWndEx::OnLButtonUp(nFlags, point);
}

void CGlassEmissionsWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
		case TIMER_LIST_UPDATE: {
			int iY = m_cboYear.GetCurSel();
			int iM = m_lst[eLL_M].GetSelectionMark();
			int iD = m_lst[eLL_D].GetSelectionMark();

			auto GetValueI = [](const int value) {
				return value > 0 ? Format(_T("%d"), value) : _T("");
			};
			auto GetValueD = [](const double value) {
				return value > 0 ? Format(_T("%.3f"), value) : _T("");
			};

			for (int i = 0; i < 12; ++i) {
				m_lst[eLL_M].SetItemText(i, eCN_INPUT,	GetValueI(sm.GlassEmissions->y[iY].m[i].count.nIn));
				m_lst[eLL_M].SetItemText(i, eCN_OUTPUT,	GetValueI(sm.GlassEmissions->y[iY].m[i].count.nOut));
				//m_lst[eLL_M].SetItemText(i, eCN_OK,		GetValueI(sm.GlassEmissions->y[iY].m[i].count.nOK));
				//m_lst[eLL_M].SetItemText(i, eCN_NG,		GetValueI(sm.GlassEmissions->y[iY].m[i].count.nNG));
				//m_lst[eLL_M].SetItemText(i, eCN_RATE,	GetValueD(sm.GlassEmissions->y[iY].m[i].count.dRate));
			}

			for (int i = 0; i < 31; ++i) {
				m_lst[eLL_D].SetItemText(i, eCN_INPUT,	GetValueI(sm.GlassEmissions->y[iY].m[iM].d[i].count.nIn));
				m_lst[eLL_D].SetItemText(i, eCN_OUTPUT,	GetValueI(sm.GlassEmissions->y[iY].m[iM].d[i].count.nOut));
				//m_lst[eLL_D].SetItemText(i, eCN_OK,		GetValueI(sm.GlassEmissions->y[iY].m[iM].d[i].count.nOK));
				//m_lst[eLL_D].SetItemText(i, eCN_NG,		GetValueI(sm.GlassEmissions->y[iY].m[iM].d[i].count.nNG));
				//m_lst[eLL_D].SetItemText(i, eCN_RATE,	GetValueD(sm.GlassEmissions->y[iY].m[iM].d[i].count.dRate));
			}

			for (int i = 0; i < 24; ++i) {
				m_lst[eLL_H].SetItemText(i, eCN_INPUT,	GetValueI(sm.GlassEmissions->y[iY].m[iM].d[iD].h[i].count.nIn));
				m_lst[eLL_H].SetItemText(i, eCN_OUTPUT,	GetValueI(sm.GlassEmissions->y[iY].m[iM].d[iD].h[i].count.nOut));
				//m_lst[eLL_H].SetItemText(i, eCN_OK,		GetValueI(sm.GlassEmissions->y[iY].m[iM].d[iD].h[i].count.nOK));
				//m_lst[eLL_H].SetItemText(i, eCN_NG,		GetValueI(sm.GlassEmissions->y[iY].m[iM].d[iD].h[i].count.nNG));
				///m_lst[eLL_H].SetItemText(i, eCN_RATE,	GetValueD(sm.GlassEmissions->y[iY].m[iM].d[iD].h[i].count.dRate));
			}

			InvalidateRect(m_rectS[eS_YEAR_COUNT], FALSE);
			break;
		}
	}
	CFrameWndEx::OnTimer(nIDEvent);
}

#include "pch.h"
#include "UI.h"
#include "AlarmLogWnd.h"


IMPLEMENT_DYNCREATE(CAlarmLogWnd, CFrameWndEx)
#define IDD_LOG_LIST 6123
#define IDD_LOG_CAL	6223


CAlarmLogWnd::CAlarmLogWnd(CWnd* pwndParent)
{
	m_pwndParent	= pwndParent;
	m_eSelectControl= eC_NONE;
}

CAlarmLogWnd::~CAlarmLogWnd()
{
}

BEGIN_MESSAGE_MAP(CAlarmLogWnd, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_INITMENU()
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnListSelected)
	ON_NOTIFY(MCN_SELECT, IDD_LOG_CAL, OnCalSelected)
	ON_NOTIFY(MCN_SELECT, IDD_LOG_CAL + 1, OnCalSelected)
END_MESSAGE_MAP()


int CAlarmLogWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	if (m_startMonthCal == nullptr)
	{
		m_startMonthCal = new CMonthCalCtrl();
		m_startMonthCal->Create(WS_BORDER | WS_CHILD | WS_VISIBLE | MCS_NOTODAYCIRCLE, CPoint2d(0, 0), this, IDD_LOG_CAL);

		CFont* font = new CFont();
		font->CreateFontW(20, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, DT_FONT);

		m_startMonthCal->SetFont(font);
	}

	if (m_endMonthCal == nullptr)
	{
		m_endMonthCal = new CMonthCalCtrl();
		m_endMonthCal->Create(WS_BORDER | WS_CHILD | WS_VISIBLE | MCS_NOTODAYCIRCLE, CPoint2d(0, 0), this, IDD_LOG_CAL + 1);

		CFont* font = new CFont();
		font->CreateFontW(20, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, DT_FONT);

		m_endMonthCal->SetFont(font);
	}

	ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);
    ModifyStyleEx(this->m_hWnd, WS_EX_CLIENTEDGE, 0, 0);
    
	m_listAlarm.Create(LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_OWNERDATA, CRect(), this, IDD_LOG_LIST);
	m_listAlarm.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	std::pair<CString, int> columnData[nLI_COLUMN]
	{
		{_T("")		        , 0},
		{_T("TIME")			, 200},
        {_T("SITUATION")	, 85},
        {_T("NUMBER")		, 70},
        {_T("TYPE")			, 50},
        {_T("ALARM NAME")	, 300},
        {_T("USER")			, 100},
        {_T("EXPLANATION")	, 600},
	};

	for (int iColumn = eLI_COLUMN_NONE; iColumn < nLI_COLUMN; ++iColumn)
	{
		m_listAlarm.InsertColumn(iColumn, columnData[iColumn].first, LVCFMT_CENTER, columnData[iColumn].second);
	}
	m_startMonthCal->SetCurSel(CTime::GetCurrentTime());
	m_endMonthCal->SetCurSel(CTime::GetCurrentTime());

	return 0;
}


void CAlarmLogWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left,g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		ComputeArea();
		m_eSelectControl	= eC_NONE;
		OpenLogFile();
	}
}

void CAlarmLogWnd::ComputeArea() 
{
	GetClientRect(m_rectClient);

	m_rectS[eS_FULL_AREA] = m_rectClient;
	CRect rc = m_rectS[eS_FULL_AREA];
	double dW = rc.Width() / 30.;
	double dH = rc.Height() / 18.;

	m_rectS[eS_CONTROL_AREA] = CRect(rc.left + dW * 0, rc.top, rc.left + dW * 6, rc.bottom);
	m_rectS[eS_LOG_AREA] = CRect(rc.left + dW * 6, rc.top, rc.right, rc.bottom);

	rc = m_rectS[eS_CONTROL_AREA];
	rc.DeflateRect(10, 0, 10, 0);
	dW = rc.Width() / 6.;
	dH = rc.Height() / 18.;

	m_rectS[eS_START_CAL_TEXT] = CRect(rc.left + dW * 0, rc.top + dH * 0, rc.left + dW * 6, rc.top + dH * 1);
	m_rectS[eS_START_CAL_AREA] = CRect(rc.left + dW * 0, rc.top + dH * 1, rc.left + dW * 6, rc.top + dH * 5);
	m_rectS[eS_END_CAL_TEXT] = CRect(rc.left + dW * 0, rc.top + dH * 6, rc.left + dW * 6, rc.top + dH * 7);
	m_rectS[eS_END_CAL_AREA] = CRect(rc.left + dW * 0, rc.top + dH * 7, rc.left + dW * 6, rc.top + dH * 11);

	m_rectC[eC_LOGFOLDER_OPEN] = CRect(rc.left + dW * 0, rc.top + dH * 12, rc.left + dW * 6, rc.top + dH * 16);

	if (m_listAlarm.m_hWnd)
	{
		m_rectS[eS_LOG_AREA].DeflateRect(5, 5);
		m_listAlarm.MoveWindow(m_rectS[eS_LOG_AREA]);
	}

	if (m_startMonthCal != nullptr && m_startMonthCal->m_hWnd)
	{
		m_startMonthCal->MoveWindow(m_rectS[eS_START_CAL_AREA]);
	}
	if (m_endMonthCal != nullptr && m_endMonthCal->m_hWnd)
	{
		m_endMonthCal->MoveWindow(m_rectS[eS_END_CAL_AREA]);
	}
}

void CAlarmLogWnd::OnPaint()
{
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);

	CDrawSymbol::DrawLineRect(&memDC, m_rectS[eS_FULL_AREA], RGB_GRAY);
	CRect rect = m_rectS[eS_FULL_AREA];
	rect.DeflateRect(1, 1);
	memDC.FillSolidRect(rect, g_colorBackground[eEC_DEFAULT]);

	CTime startTime, endTime;
	m_startMonthCal->GetCurSel(startTime);
	m_endMonthCal->GetCurSel(endTime);
	DrawText(&memDC, m_rectS[eS_START_CAL_TEXT], g_st[IDS_LOG_START_TIME] + Format(L" : %04d-%02d-%02d", startTime.GetYear(), startTime.GetMonth(), startTime.GetDay()), 9, RGB_BLACK);
	DrawText(&memDC, m_rectS[eS_END_CAL_TEXT], g_st[IDS_LOG_END_TIME] + Format(L" : %04d-%02d-%02d", endTime.GetYear(), endTime.GetMonth(), endTime.GetDay()), 9, RGB_BLACK);

	DrawMDIMenuButton(memDC, m_rectC[eC_LOGFOLDER_OPEN], g_st[IDS_LOG_FOLDER_OPEN], m_eSelectControl == eC_LOGFOLDER_OPEN, FALSE, 13);
}

void CAlarmLogWnd::OnLButtonDown(UINT nFlags, CPoint point)
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

void CAlarmLogWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
		case eC_LOGFOLDER_OPEN:
		{
			strLog = _T("Log Folder Open");
			ShellExecute(NULL, _T("open"), _T("explorer.exe"), _T("D:\\MDIK\\Alarm Log\\"), NULL, SW_SHOW);
			break;
		}
		}
	}

	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl		= eC_NONE;
	Invalidate(FALSE);
	CFrameWndEx::OnLButtonUp(nFlags, point);
}

void CAlarmLogWnd::OpenLogFile()
{
	// 새로운 데이터 불러오기 전 기존 데이터 삭제
	if (m_listAlarm.GetItemCount())
	{
		m_listAlarm.DeleteAllItems();
	}

	CTime startTime, endTime;
	m_startMonthCal->GetCurSel(startTime);
	m_endMonthCal->GetCurSel(endTime);

	while (endTime >= startTime)
	{
		OpenLogFile(startTime);
		startTime += CTimeSpan(1, 0, 0, 0);
	}
}

void CAlarmLogWnd::OpenLogFile(CTime _time)
{
	CString strPath = Format(_T("D:\\MDIK\\Alarm Log\\%04d\\%02d\\%02d"), _time.GetYear(), _time.GetMonth(), _time.GetDay());
	CString strFile = Format(_T("%s.csv"), strPath);

	CFile				file;
	CFileException		e;
	if (file.Open(strFile, CFile::modeRead | CFile::shareDenyNone) == FALSE)
	{
		return;
	}

	CString logData;
	CArchiveU arU(&file, CArchive::load);
	if (arU.ReadStringU(logData) == FALSE)
	{
		arU.Close();
		return;
	}

	int count = m_listAlarm.GetItemCount();
	while (arU.ReadStringU(logData) != FALSE)
	{
		CStrings contents;
		SplitString(logData, _T(";"), contents);

		m_listAlarm.AddItem(_T(""));
		int nLogItem = contents.size();
		if (nLogItem > nLI_COLUMN) {	// 한글, 영어, 베트남 3가지 언어로 로그 남은 경우
			m_listAlarm.SetItemText(count, eLI_COLUMN_TIME, Format(L"%04d-%02d-%02d ", _time.GetYear(), _time.GetMonth(), _time.GetDay()) + contents[0]);
			m_listAlarm.SetItemText(count, eLI_COLUMN_STATUS, contents[1]);
			m_listAlarm.SetItemText(count, eLI_COLUMN_INDEX, contents[2]);
			m_listAlarm.SetItemText(count, eLI_COLUMN_TYPE, contents[3]);
			m_listAlarm.SetItemText(count, eLI_COLUMN_NAME, contents[4 + sm.DeviceParameter->System.eLanguage]);
			m_listAlarm.SetItemText(count, eLI_COLUMN_USER, contents[7]);
			m_listAlarm.SetItemText(count, eLI_COLUMN_DESCRIPTION, contents[8 + sm.DeviceParameter->System.eLanguage]);
		}
		else {	// 한글로만 로그 남은 경우 (옛날버전)
			for (int iColumn = 0; iColumn < contents.size(); ++iColumn)
			{
				if (iColumn == 0)
				{
					m_listAlarm.SetItemText(count, eLI_COLUMN_TIME, Format(L"%04d-%02d-%02d ", _time.GetYear(), _time.GetMonth(), _time.GetDay()) + contents[iColumn]);
				}
				else
				{
					m_listAlarm.SetItemText(count, eLI_COLUMN_TIME + iColumn, contents[iColumn]);
				}
			}
		}
		++count;
	}
	m_listAlarm.Complete();
	arU.Close();
	file.Close();
}

void CAlarmLogWnd::OnCalSelected(NMHDR* pNMHDR, LRESULT* pResult) 
{
	InvalidateRect(m_rectS[eS_START_CAL_TEXT], FALSE);
	InvalidateRect(m_rectS[eS_END_CAL_TEXT], FALSE);
	OpenLogFile();
}

void CAlarmLogWnd:: OnListSelected(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	
	std::vector<std::pair<CString, CStringsv>> vec;
	vec.resize(m_listAlarm.GetItemCount());

	int selectColumn = phdr->iItem;
	for (int iItem = 0; iItem < m_listAlarm.GetItemCount(); ++iItem)
	{
		CString strSel;
		CStringsv strAll;
		for (int iColumn = 0; iColumn < nLI_COLUMN; ++iColumn)
		{
			if (iColumn == selectColumn)
			{
				vec[iItem].first = m_listAlarm.GetItemText(iItem, iColumn);
			}
			vec[iItem].second.push_back(m_listAlarm.GetItemText(iItem, iColumn));
		}
	}
	m_listAlarm.DeleteAllItems();

	std::sort
	(
		vec.begin(), vec.end(),
		[&](const std::pair<CString, CStringsv>& _item1, const std::pair<CString, CStringsv>& _item2) -> bool
		{
			return m_bDirect[selectColumn] ? _item1.first > _item2.first : _item1.first < _item2.first;
		}
	);

	for (int iItem = 0; iItem < vec.size(); ++iItem)
	{
		m_listAlarm.AddItem(_T(""));
		for (int iColumn = 0; iColumn < nLI_COLUMN; ++iColumn)
		{
			m_listAlarm.SetItemText(iItem, iColumn, vec[iItem].second[iColumn]);
		}
	}
	m_listAlarm.Complete();
	m_bDirect[selectColumn] = !m_bDirect[selectColumn];
}

void CAlarmLogWnd::ChangeLanguage()
{
	Invalidate(FALSE);
	OpenLogFile();
}
// CAutoDeleteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoDelete.h"
#include "AutoDeleteDlg.h"
#include "afxdialogex.h"

// CAutoDeleteDlg dialog

IMPLEMENT_DYNAMIC(CAutoDeleteDlg, CDialogEx)

CAutoDeleteDlg::CAutoDeleteDlg(CWnd* pParent /*=NULL*/)
: TIPCommandTarget<CDialogEx>(&theApp.m_ipClient) {
	//: CDialogEx(CAutoDeleteDlg::IDD, pParent)

}

CAutoDeleteDlg::~CAutoDeleteDlg()
{
	if (m_rthreadAutoDelete)
		m_rthreadAutoDelete->join();
	m_rthreadAutoDelete.reset();
}

void CAutoDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_lst);
}


BEGIN_MESSAGE_MAP(CAutoDeleteDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SAVE, &CAutoDeleteDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CAutoDeleteDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_ADD, &CAutoDeleteDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CAutoDeleteDlg::OnBnClickedBtnDelete)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

BEGIN_IP_COMMAND_TABLE(CAutoDeleteDlg, IIPCommandTarget)
	ON_IP("AutoDelete", "Show",	OnIPShow)
END_IP_COMMAND_TABLE()

// CAutoDeleteDlg message handlers
BOOL CAutoDeleteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_lst.InsertColumn(eLI_NONE,	_T(""),			LVCFMT_CENTER, 0);
	m_lst.InsertColumn(eLI_NO,		_T("No."),		LVCFMT_CENTER, 70);
	m_lst.InsertColumn(eLI_USE,		_T("Use"),		LVCFMT_CENTER, 100);
	m_lst.InsertColumn(eLI_DAY,		_T("Day"),		LVCFMT_CENTER, 100);
	m_lst.InsertColumn(eLI_PATH,	_T("Path"),		LVCFMT_CENTER, 265);
	m_lst.SetColumnCheck(eLI_USE, _T("Use"), _T("UnUse"));
	m_lst.GetHeaderCtrl()->EnableWindow(FALSE);
	m_lst.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lst.SetColumnEditNumber(eLI_DAY);

	SyncData();
	SetProcessAffinityMask(GetCurrentProcess(), 1);
	m_rthreadAutoDelete.reset(new thread([&]() { AutoDelete(); }));
	return TRUE;
}


BOOL CAutoDeleteDlg::SyncData(BOOL bStore)
{
	CProfile profile(_T("..\\cfg\\AutoDelete.ini"));

	if (bStore) {
		//profile.GetSections().DeleteAll();
		profile.GetSection(_T("Item")).GetSubSections().DeleteAll();
		SyncLst(bStore);
	}
	else
		m_tData.DeleteAll();


	CProfileSection& section = profile.GetSection(_T("Item"));
	int nItem = bStore? m_tData.N() : section.GetSubSections().N();
	for (int i = 0; i < nItem; i++) {
		CProfileSection &sectionItem = section.GetSubSection(Format(_T("Item_%d"), i + 1));

		T_AUTO_DELETE_DATA tData;
		if (bStore)
			tData = m_tData[i];


		sectionItem.SyncItemValueBoolean(bStore, _T("Use"), tData.bUse);
		sectionItem.SyncItemValue(bStore, _T("Day"), tData.iDay);
		sectionItem.SyncItemValue(bStore, _T("Path"), tData.strPath);

		if (!bStore)
			m_tData.Attach(new T_AUTO_DELETE_DATA(tData));
	}

	if (!bStore)
		SyncLst();
	else
		profile.Save();

	return TRUE;
}

BOOL CAutoDeleteDlg::SyncLst(BOOL bStore) {
	if (bStore) {
		m_tData.DeleteAll();

		for (int i = 0; i < m_lst.GetItemCount(); i++) {
			T_AUTO_DELETE_DATA tData;
			tData.bUse = m_lst.GetCheck(i, eLI_USE);
			tData.iDay = m_lst.GetItemInt(i, eLI_DAY);
			tData.strPath = CString(m_lst.GetItemText(i, eLI_PATH));

			m_tData.Attach(new T_AUTO_DELETE_DATA(tData));
		}
	}
	else {
		m_lst.DeleteAllItems();

		for (int i = 0; i < m_tData.N(); i++) {
			int iItem = m_lst.GetItemCount();
			m_lst.InsertItem(iItem, _T(""));
			m_lst.SetItemText(iItem, eLI_NO, Format(_T("%d"), i+1));
			m_lst.SetCheck(iItem, eLI_USE, m_tData[i].bUse);
			m_lst.SetItemInt(iItem, eLI_DAY, m_tData[i].iDay);
			m_lst.SetItemText(iItem, eLI_PATH, m_tData[i].strPath);
		}
	}
	return TRUE;
}

void CAutoDeleteDlg::OnClose()
{
	//CDialogEx::OnClose();
	ShowWindow(SW_HIDE);
}


void CAutoDeleteDlg::OnBnClickedBtnSave()
{
	SyncData(TRUE);
}

void CAutoDeleteDlg::OnBnClickedBtnLoad()
{
	SyncData();
}


void CAutoDeleteDlg::OnBnClickedBtnAdd()
{
	CString strPath(SelectFolder());
	BOOL bAdd = TRUE;
	if (!strPath.IsEmpty()) {
		for (int i = 0; i < m_tData.N(); i++) {
			if (strPath.Compare(m_tData[i].strPath) == 0) {
				bAdd = FALSE;
				break;
			}
		}

		if (!InterlockPath(strPath)) {
			bAdd = FALSE;
		}

		if (bAdd) {
			// Add List
			int iItem = m_lst.GetItemCount();
			m_lst.InsertItem(iItem, _T(""));
			m_lst.SetCheck(iItem, eLI_USE);
			m_lst.SetItemText(iItem, eLI_NO, Format(_T("%d"), iItem+1));
			m_lst.SetItemInt(iItem, eLI_DAY, 30);
			m_lst.SetItemText(iItem, eLI_PATH, strPath);
		}
	}
}


CString CAutoDeleteDlg::SelectFolder() {
	BROWSEINFO bInfo;
	memset(&bInfo, 0, sizeof(bInfo));

	bInfo.hwndOwner = GetSafeHwnd();
	bInfo.pidlRoot = NULL;
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	CString strInitPath = _T("D:\\Log");
    bInfo.lParam = (LPARAM)strInitPath.GetBuffer();

	LPITEMIDLIST path = NULL;
	path = SHBrowseForFolder(&bInfo);
	TCHAR pszPath[MAX_PATH];

	if (path != NULL) {
		SHGetPathFromIDList(path, pszPath);
		return pszPath;
	}

	return _T("");
}

BOOL CAutoDeleteDlg::RemoveDirectoryFileTemp(LPCTSTR szFolderPath, int iDay) {
	if (szFolderPath == NULL)
		return FALSE;

	BOOL bRet = FALSE;
	CString szRoot;
	CString strFolder;

	szRoot.Format(_T("%s\\*.*"), szFolderPath);
	CFileFind find;
	bRet = find.FindFile(szRoot);

	if (!bRet)
		return FALSE;

	while(bRet) {
		bRet = find.FindNextFile();
		if (!find.IsDots()) {
			if (find.IsDirectory()) {
				strFolder = find.GetFilePath();
				
				RemoveDirectoryFileTemp(find.GetFilePath(), iDay);
			}
			else {
				CString strFile1 = find.GetFilePath();
				if (IsFileDateOver(find, iDay)) {
					CString strFile = find.GetFilePath();
					bRet = DeleteFile(find.GetFilePath());
				}
			}
		}
	}

	find.Close();
	//ShellExecute(NULL, _T("open"), _T("rmdir"), Format(_T("/s /q \"%s\""), szFolderPath), NULL, SW_HIDE);
	bRet = RemoveDirectory(szFolderPath); //Directory 내부에 File이 남아있을경우 지워지지 않음.

	return bRet;
}

BOOL CAutoDeleteDlg::IsFileDateOver(CFileFind&find, int iDay) {
	CTime filetime;
	find.GetCreationTime(filetime);

	COleDateTime CurrentTime = COleDateTime::GetCurrentTime();

	COleDateTimeSpan Prevtime(-iDay, 0, 0, 0);
	CurrentTime += Prevtime;
	CString strCurTime = Format(_T("%d;%d;%d;%d;%d;%d"), CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute(), CurrentTime.GetSecond());
	CString strFileTime = Format(_T("%d;%d;%d;%d;%d;%d"), filetime.GetYear(), filetime.GetMonth(), filetime.GetDay(), filetime.GetHour(), filetime.GetMinute(), filetime.GetSecond());

	CTime timeCalc(CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute(), CurrentTime.GetSecond());

	return (filetime <= timeCalc);
}

BOOL CAutoDeleteDlg::DeleteFolder(const CString &strFolder) {
	SHFILEOPSTRUCT FileOp = {0};
	char szTemp;
	szTemp = NULL;
	CString strTemp;
	strTemp.Format(_T("%s%c"), strFolder, szTemp);
	
	FileOp.hwnd = NULL;
	FileOp.wFunc = FO_DELETE;
	FileOp.pFrom = NULL;
	FileOp.pTo = NULL;
	FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;
	FileOp.fAnyOperationsAborted = FALSE;
	FileOp.hNameMappings = NULL;
	FileOp.lpszProgressTitle = NULL;
	
	FileOp.pFrom = strTemp;
	SHFileOperation(&FileOp);
	return TRUE;
}

BOOL CAutoDeleteDlg::OnIPShow(CIPCommand &cmd) {
	// KES 수정 2024.2.15
	const CIPVar& var = cmd.m_var;

	BOOL bShow = var.GetChildItem("Show", TRUE);
	switch(cmd.GetCurrentStep()) {
		case 0 : {
			CWnd* pWnd = AfxGetMainWnd();
			if (pWnd) {
				pWnd->ShowWindow(SW_HIDE);
				pWnd->SetForegroundWindow();
				pWnd->SetFocus();
			}
			ShowWindow(SW_HIDE);

			if (!bShow) {
				return FALSE;
			}

			cmd.EnqueueNextStep(-1, 100);
			break;
		}
		case 1 : {
			CWnd* pWnd = AfxGetMainWnd();
			if (pWnd) {
				pWnd->SetForegroundWindow();
				pWnd->SetFocus();
			}
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
			SetFocus();
			break;
		}
	}
	
	return TRUE;
}

void CAutoDeleteDlg::OnBnClickedBtnDelete()
{
	int iItem = m_lst.GetNextItem(-1, LVNI_SELECTED);
	m_lst.DeleteItem(iItem);
}


BOOL CAutoDeleteDlg::InterlockPath(LPCTSTR szFolderPath) {
	LPCTSTR lpInterlockCompareFolder[] = {
		_T("D:\\"),
		_T("d:\\"),
	};

	LPCTSTR lpInterlockFindFolder[] = {
		_T("C:\\"),
		_T("c:\\"),
		_T("C\\"),
		_T("c\\"),
		_T("BackUp"),
		_T("backup"),
		_T("CIM_DB"),
		_T("SetUp"),
		_T("setup"),
		_T("QRS"),
		_T("qrs"),
		_T("MOCHA"),
		_T("Mocha"),
		_T("mocha"),
	};

	BOOL bInterlock = TRUE;

	CStrings strs;
	SplitString(szFolderPath, _T("\\"), strs);
	if (strs.N()) {
		bInterlock &= (strs[strs.N()-1].Compare(_T("C")));
		bInterlock &= (strs[strs.N()-1].Compare(_T("c")));
		bInterlock &= (strs[strs.N()-1].Compare(_T("D")));
		bInterlock &= (strs[strs.N()-1].Compare(_T("d")));
	}


	for (int i = 0; i < countof(lpInterlockCompareFolder); i++) {
		if (CString(szFolderPath).CompareNoCase(lpInterlockCompareFolder[i]) == 0) {
		//if (CString(szFolderPath).Compare(lpInterlockFindFolder[i]) == 0) {
			bInterlock = FALSE;
			break;
		}
	}

	for (int i = 0; i < countof(lpInterlockFindFolder); i++) {
		if (CString(szFolderPath).Find(lpInterlockFindFolder[i]) >= 0) {
			bInterlock = FALSE;
			break;
		}
	}

	return bInterlock;

}

void CAutoDeleteDlg::AutoDelete() {
	while (true) {
		for (int i = 0; i < m_tData.N(); i++) {
			if (m_tData[i].bUse) {
				CString strPath(m_tData[i].strPath);
				if (InterlockPath(strPath)) { // Folder 예외처리.
 					RemoveDirectoryFileTemp(strPath, m_tData[i].iDay);
				}
			}
		}

		Sleep(1000);
	}
}

void CAutoDeleteDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SyncData(FALSE);
	}
}

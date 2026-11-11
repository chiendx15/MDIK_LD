#pragma once
#include "afxcmn.h"
#include "afxeditbrowsectrl.h"

typedef struct T_AUTO_DELETE_DATA {
	BOOL bUse = TRUE;
	CString strPath;
	int iDay = 30;
} T_AUTO_DELETE_DATA;

#define TIEMR_AUTO_DELETE	9090
// CAutoDeleteDlg dialog
//class CAutoDeleteDlg : public CDialogEx
class CAutoDeleteDlg : public TIPCT<CDialogEx>
{
	DECLARE_DYNAMIC(CAutoDeleteDlg)

public:
	CAutoDeleteDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAutoDeleteDlg();

// Dialog Data
	enum { IDD = IDD_DLG_AUTO_DELETE };

	enum eLI{
		eLI_NONE,
		eLI_NO,
		eLI_USE,
		eLI_DAY,
		eLI_PATH,
		eLI_END,
	};

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK() {};
	virtual void OnCancel() {};

	DECLARE_MESSAGE_MAP()
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnAdd();
public:
	DECLARE_IP()
	ip_handler BOOL	OnIPShow(CIPCommand& cmd);
public:
	CListCtrlColumnEx m_lst;
	TList<T_AUTO_DELETE_DATA> m_tData;
	BOOL SyncData(BOOL bStore = FALSE);
	BOOL SyncLst(BOOL bStore = FALSE);
	CString SelectFolder();
	

	BOOL RemoveDirectoryFileTemp(LPCTSTR szFolderPath, int iDay);
	BOOL IsFileDateOver(CFileFind& find, int iDay);
	BOOL DeleteFolder(const CString &strFolder);

	// 지우면 안될때 return FALSE;
	BOOL InterlockPath(LPCTSTR szFolderPath);

	unique_ptr<thread>	m_rthreadAutoDelete;
	void				AutoDelete();
	
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

#include "pch.h"
#include "UI.h"
#include "ChangeParamDlg.h"
#include "afxdialogex.h"


// CChangeParamDlg 대화 상자

IMPLEMENT_DYNAMIC(CChangeParamDlg, CDialogEx)

CChangeParamDlg::CChangeParamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHANGE_PARAM_DATA, pParent)
{}

CChangeParamDlg::~CChangeParamDlg()
{}

void CChangeParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PARAM_LIST, m_paramList);
	m_paramList.InsertColumn(eLC_NONE, _T(""), eLCETA_LEFT, 0, eLC_NONE);
	m_paramList.InsertColumn(eLC_TEXT, g_st[IDS_CHANGE_PARAM_DLG_CHANGE_ITEM], eLCETA_LEFT, 0, eLC_TEXT);
	m_paramList.SetColumnWidth(eLC_TEXT, LVSCW_AUTOSIZE_USEHEADER);
	m_paramList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_paramList.SetAlignText(eLC_TEXT, eLCETA_LEFT);
	m_paramList.SetRowHeight(40);

	m_paramList.DeleteAllItems();
	for (int idx = 0; idx < m_params.size(); ++idx)
	{
		m_paramList.InsertItem(idx, _T(""));
		m_paramList.SetItemText(idx, eLC_TEXT, m_params[idx].first);
		m_paramList.SetItemBKColor(idx, m_params[idx].second);
	}
}

void CChangeParamDlg::SetChangeData(const std::vector<std::pair<CString, COLORREF>>& _datas)
{
	m_params.assign(_datas.begin(), _datas.end());
}

BEGIN_MESSAGE_MAP(CChangeParamDlg, CDialogEx)
	ON_BN_CLICKED(ID_PARAM_SAVE, &CChangeParamDlg::OnBnClickedSave)
	ON_BN_CLICKED(ID_PARAM_CANCEL, &CChangeParamDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CChangeParamDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CChangeParamDlg::OnBnClickedSave()
{
	CDialogEx::OnOK();
}

void CChangeParamDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

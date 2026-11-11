#pragma once


// CChangeParamDlg 대화 상자

class CChangeParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeParamDlg)

	enum
	{
		eLC_NONE,
		eLC_TEXT,
	};

public:
	CChangeParamDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CChangeParamDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANGE_PARAM_DATA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	std::vector<std::pair<CString, COLORREF>> m_params;
	CListCtrlColumnEx m_paramList;

public:
	void SetChangeData(const std::vector<std::pair<CString, COLORREF>>& _datas);


public:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();
};

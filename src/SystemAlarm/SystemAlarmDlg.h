#pragma once


// CSystemAlarmDlg 대화 상자

class CSystemAlarmDlg : public CDialogEx, public IWNDIPCommandTarget
{

enum eALARM_COLUMNS 
{
	eAC_NONE,
	eAC_INDEX,
	eAC_ID,
	eAC_TIME,
	eAC_TYPE,
	eAC_AREA,
	eAC_NAME,
};

	DECLARE_DYNAMIC(CSystemAlarmDlg)
public:
	CSystemAlarmDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSystemAlarmDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SYSTEM_ALARM_WND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnBnClickedAlarmclear();
	afx_msg void OnBnClickedBuzzerstop();
	afx_msg void OnItemchangedListAlarmList(NMHDR *pNMHDR, LRESULT *pResult);

private:
	CRect		m_minRect;
	//CListCtrlEd	m_alarmListctrl;
	CListCtrlColumnEx m_alarmListctrl;
	CEdit		m_alarmMsgEdit;
	CEdit		m_helpMsgEdit;
	CEdit		m_StepNameMsgEdit;
	std::map<int, std::pair<CTime, T_SYSTEM_ALARM_ITEM*>> m_Alarms;

	CString		m_logPath = _T("D:/MDIK/Alarm Log/");
private:
	void		WriteAlarmLog(const T_SYSTEM_ALARM_ITEM& _alarm, const bool _bOccurred);
	void		SetClipBoardText(CStringA strText);	// KES 추가 2024.3.26
	void		SetAlarmDescription(int _iCode);	// KES 추가 2024.3.26

private:
	DECLARE_IP_AND_INTERFACE()
	ip_handler BOOL OnIP_AlarmClear(CIPCommand& cmd);
	ip_handler BOOL OnIP_SetLanguage(CIPCommand& cmd);	// KES 추가 2024.3.26 
};

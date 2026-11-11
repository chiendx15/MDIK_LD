// CSystemAlarmDlg.cpp: 구현 파일
//

#include "pch.h"
#include "SystemAlarm.h"
#include "SystemAlarmDlg.h"
#include "afxdialogex.h"

#include "DrawControl.h"


// CSystemAlarmDlg 대화 상자

IMPLEMENT_DYNAMIC(CSystemAlarmDlg, CDialogEx)

CSystemAlarmDlg::CSystemAlarmDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SYSTEM_ALARM_WND, pParent)
	, IWNDIPCommandTarget(m_hWnd, &g_ipClient)
{}

CSystemAlarmDlg::~CSystemAlarmDlg()
{
}

void CSystemAlarmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALARM_LIST, m_alarmListctrl);
	DDX_Control(pDX, IDC_ALARM_MSG_EDIT, m_alarmMsgEdit);
	DDX_Control(pDX, IDC_HELP_MSG_EDIT, m_helpMsgEdit);
	DDX_Control(pDX, IDC_STEP_NAME, m_StepNameMsgEdit);

	// 리스트 컬럼 생성
	m_alarmListctrl.InsertColumn(eAC_NONE, _T(""), LVCFMT_LEFT, 0, eAC_NONE);
	m_alarmListctrl.InsertColumn(eAC_INDEX, _T("No"), LVCFMT_CENTER, 40, eAC_INDEX);
	m_alarmListctrl.InsertColumn(eAC_ID,	_T("ALID"), LVCFMT_CENTER, 40, eAC_ID);
	m_alarmListctrl.InsertColumn(eAC_TIME, g_st[IDS_SYSTEM_ALARM_DLG_ALARM_TIME], LVCFMT_CENTER, 70, eAC_TIME);
	m_alarmListctrl.InsertColumn(eAC_TYPE, g_st[IDS_SYSTEM_ALARM_DLG_TYPE], LVCFMT_CENTER, 60, eAC_TYPE);
	m_alarmListctrl.InsertColumn(eAC_AREA, g_st[IDS_SYSTEM_ALARM_DLG_ALARM_LOCATE], LVCFMT_CENTER, 90, eAC_AREA);
	m_alarmListctrl.InsertColumn(eAC_NAME, g_st[IDS_SYSTEM_ALARM_DLG_NAME], LVCFMT_LEFT, 350, eAC_NAME);	// KES 수정 2024.3.26
	m_alarmListctrl.SetColumnWidth(eAC_NAME, LVSCW_AUTOSIZE_USEHEADER);
	m_alarmListctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_alarmListctrl.SetRowHeight(40);

	//m_alarmListctrl.SetEditable(FALSE);
}

BOOL CSystemAlarmDlg::PreTranslateMessage(MSG* pMsg)
{
	// KES 추가 2024.1.11
	if (pMsg->message == WM_LBUTTONUP) {
		CRect rect;
		m_StepNameMsgEdit.GetWindowRect(rect);
		ScreenToClient(rect);

		POINT pt;
		pt.x = GET_X_LPARAM(pMsg->lParam);
		pt.y = GET_Y_LPARAM(pMsg->lParam);

		if (rect.PtInRect(pt))
		{
			CString str;
			m_StepNameMsgEdit.GetWindowTextW(str);
			SetClipBoardText((CStringA)str);
		}
	}

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

#define INTERVAL_ALARM_DISPLAY 4101
// CSystemAlarmDlg 메시지 처리기
BEGIN_MESSAGE_MAP(CSystemAlarmDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDALARMCLEAR, &CSystemAlarmDlg::OnBnClickedAlarmclear)
	ON_BN_CLICKED(IDBUZZERSTOP, &CSystemAlarmDlg::OnBnClickedBuzzerstop)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ALARM_LIST, &CSystemAlarmDlg::OnItemchangedListAlarmList)
END_MESSAGE_MAP()

int CSystemAlarmDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetWindowRect(m_minRect);
	LoadAlarm(_T("LOADER_AlarmCode.csv"));

	// KES 주석처리 2024.3.26
	//// 2021.05.31 - 100번 부터 있는 Motor 관련 내용
	//for (int iMotor = 0; iMotor < eMOTOR_END; ++iMotor)
	//{
	//	auto func = [](int idx, const CString& text)
	//	{
	//		auto& alarm = sm.Alarm->ALARMs[idx];
	//		wcscpy_s(alarm.szKText, _countof(alarm.szKText), text);
	//	};
	//
	//	
	//	int AlarmIdx = eAI_MOTOR_ALARM_START;
	//	// 축 초기화
	//	func(AlarmIdx + iMotor, Format(L"%s%s", g_stMotor[iMotor], L" 축 초기화 필요"));
	//	AlarmIdx += eMOTOR_END;
	//	// - 리밋
	//	func(AlarmIdx + iMotor, Format(L"%s%s", g_stMotor[iMotor], L" -리밋 센서 감지"));
	//	AlarmIdx += eMOTOR_END;
	//	// + 리밋
	//	func(AlarmIdx + iMotor, Format(L"%s%s", g_stMotor[iMotor], L" +리밋 센서 감지"));
	//	AlarmIdx += eMOTOR_END;
	//	// 모터 알람
	//	func(AlarmIdx + iMotor, Format(L"%s%s", g_stMotor[iMotor], L" 축 알람"));
	//	AlarmIdx += eMOTOR_END;
	//	// Servo Off
	//	func(AlarmIdx + iMotor, Format(L"%s%s", g_stMotor[iMotor], L" Servo Off"));
	//	AlarmIdx += eMOTOR_END;
	//	// Gantry Offset Limit warning
	//	func(AlarmIdx + iMotor, Format(L"%s%s", g_stMotor[iMotor], L" Gantry Offset Limit Warning"));
	//	AlarmIdx += eMOTOR_END;
	//	// Gantry Offset Limit Alert
	//	func(AlarmIdx + iMotor, Format(L"%s%s", g_stMotor[iMotor], L" Gantry Offset Limit Alert"));
	//	AlarmIdx += eMOTOR_END;
	//}

	SetTimer(INTERVAL_ALARM_DISPLAY, 100, nullptr);

	return 0;
}

void CSystemAlarmDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = m_minRect.Width();
	lpMMI->ptMinTrackSize.y = m_minRect.Height();
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CSystemAlarmDlg::OnTimer(UINT_PTR nIDEvent)
{
	auto cleanup = make_scope_exit
	(
		[&]() { CDialogEx::OnTimer(nIDEvent); }
	);

	if (nIDEvent != INTERVAL_ALARM_DISPLAY)
		return;

	// alarm check
	int iLastAlarm = sm.Alarm->iCurAlarmIndex; // 2022.05.16 이전 마지막 알람 저장
	bool isAlarm = false; // 2022.04.27
	for (int idx = 0; idx < sm.Alarm->nALARM; ++idx)
	{
		auto& alarm = sm.Alarm->ALARMs[idx];
		if (!alarm.bOccurred) continue;

		isAlarm = true;
		const auto [iter, result] = m_Alarms.insert({ alarm.iIndex, std::make_pair(CTime::GetCurrentTime(), &alarm) });
		if (result) // 새로 발생한 알람
		{

			//2024.06.05 LBH
			sm.Option->tSequence.bBuzzerStop = FALSE;

			//-->24.05.02 KGH EDIT : 멜로디 정상화
			sm.IO->tOut_NEW.fLD_MELODY_2 = TRUE;
			//sm.IO->tOut_NEW.fLD_MELODY_1 = TRUE;
			//<--24.05.02 KGH EDIT : 멜로디 정상화
			int pos = m_alarmListctrl.GetItemCount();
			auto& time = iter->second.first;
			m_alarmListctrl.InsertItem(0, _T(""));
			m_alarmListctrl.SetItemText(0, eAC_INDEX,	Format(_T("%d"), alarm.iIndex));
			m_alarmListctrl.SetItemText(0, eAC_ID,		Format(_T("%d"), alarm.iALID));
			m_alarmListctrl.SetItemText(0, eAC_TIME,	Format(_T("%02d:%02d:%02d"), time.GetHour(), time.GetMinute(), time.GetSecond()));
			m_alarmListctrl.SetItemText(0, eAC_TYPE,	Format(_T("%s"), alarm.iALCD == 2 ? _T("Heavy") : _T("Light")));
			m_alarmListctrl.SetItemText(0, eAC_AREA,	Format(_T("%s"), alarm.AlarmData[sm.DeviceParameter->System.eLanguage].szArea));	// KES 수정 2024.1.9
			m_alarmListctrl.SetItemText(0, eAC_NAME,	Format(_T("%s"), alarm.AlarmData[sm.DeviceParameter->System.eLanguage].szText));	// KES 수정 2024.1.9
			m_alarmListctrl.SetItemBKColor(0, alarm.iALCD == 2 ? RGB_RED : RGB_RED_LIGHT);
			m_alarmListctrl.SetSelectionMark(0);	// 최신 알람 자동 선택
			// MP에 가장 최근에 올라온 Alarm 을 ASCII Code 로 써줌
			memcpy(&sm.Register->Reg_Word_1_NEW.dwAlarmCode1, alarm.szALTX, sizeof(sm.Register->Reg_Word_1_NEW.dwAlarmCode1) * 20);
			// 가장 최근에 올라온 Alarm ID 를 저장
			sm.Alarm->iCurAlarmIndex = alarm.iIndex;
			WriteAlarmLog(alarm, true);

			////<< 2024.04.27 LBH - Alarm 모두 보고.
			//if (sm.Sequence->bCIMReport)
			//{
			//	// 온라인 상태에서 Alarm 발생 CIM 보고
			//	if (alarm.bReport == false)
			//	{
			//		CIPVar varAlarm;
			//		varAlarm.SetChildItemT<T_SYSTEM_ALARM_ITEM>("T_SYSTEM_ALARM_ITEM", alarm);
			//		if (g_ipClient.SendIPCommand("UI", "SendMessage", "S5F1_ALARM_REPORT_SEND", varAlarm)) {
			//			alarm.bReport = true;
			//		}
			//	}
			//}
			////>>
		}

		if (alarm.iALCD == 2)
		{
			sm.Sequence->fIsAlarm = TRUE; // 알람 발생 상태로 변경
		}

		if (!IsWindowVisible())
		{
			
			CenterWindow();
			SetWindowPos(&wndTopMost, 0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
			//m_alarmListctrl.SetSelectionMark(0);
			m_alarmListctrl.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
		}
	}

	//<< 2024.04.27 LBH - 마지막 Alarm 하나만 보고
	// 2022.05.16 온라인 상태 + 이전 마지막 알람과 다른 경우 alarm 보고
	if (!sm.Sequence->bCIMSkip && sm.Alarm->iCurAlarmIndex != iLastAlarm && sm.Alarm->iCurAlarmIndex != -1)
	{
		// 온라인 상태에서 Alarm 발생 CIM 보고
		auto iter = m_Alarms.find(sm.Alarm->iCurAlarmIndex);
		if (iter != m_Alarms.end())
		{
			auto& alarm = *iter->second.second;
			if (alarm.bReport == false)
			{
				CIPVar varAlarm;
				varAlarm.SetChildItemT<T_SYSTEM_ALARM_ITEM>("T_SYSTEM_ALARM_ITEM", alarm);
				//g_ipClient.PostIPCommand("UI", "SendMessage", "S5F1_ALARM_REPORT_SEND", varAlarm);
				//alarm.bReport = true;

				// 2024.04.11 LBH 이게 맞지 않을까? (UI나 CIM 이 꺼져 있으면?) Send로 했을 때 문제 없는지 확인 해야됨.
				if (g_ipClient.SendIPCommand("UI", "SendMessage", "S5F1_ALARM_REPORT_SEND", varAlarm)) {
					alarm.bReport = true;
				}
			}
		}
	}
	//>>
	
	// 2022.04.27
	if (!isAlarm)
	{
		sm.Alarm->iCurAlarmIndex = -1;
	}
}

void CSystemAlarmDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (!m_alarmListctrl.m_hWnd) return;

	// 다이얼로그 사이즈 따라서 설명 컬럼 크기 변경
	int addLength = cx - m_minRect.Width();
	m_alarmListctrl.SetColumnWidth(eAC_NAME, 380 + addLength);
}

void CSystemAlarmDlg::OnClose()
{
	ShowWindow(SW_HIDE);
	//CDialogEx::OnClose();
}

void CSystemAlarmDlg::OnBnClickedAlarmclear()
{
	// 2021.05.02 알람 클리어할때 모터 알람도 클리어
	sm.Register->Out_New.dw1000_AlarmReset = TRUE;

	ClearAlarm();
}


void CSystemAlarmDlg::OnBnClickedBuzzerstop()
{
	// 2024.06.05 LBH
	sm.Option->tSequence.bBuzzerStop = TRUE;
	//-->24.05.02 KGH EDIT : 멜로디 정상화
	sm.IO->tOut_NEW.fLD_MELODY_2 = FALSE;
	//sm.IO->tOut_NEW.fLD_MELODY_1 = FALSE;
	//<--24.05.02 KGH EDIT : 멜로디 정상화
}

void CSystemAlarmDlg::OnItemchangedListAlarmList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->uNewState & LVIS_SELECTED) 
	{
		int pos = pNMLV->iItem;
		int code = _ttof(m_alarmListctrl.GetItemText(pos, eAC_INDEX));
		SetAlarmDescription(code);	// KES 수정 2024.1.12
	}

	*pResult = 0;
}

void CSystemAlarmDlg::WriteAlarmLog(const T_SYSTEM_ALARM_ITEM& _alarm, const bool _bOccurred)
{
	// 시간, 번호, 아이디, 로그인한 유저, 설명
	CTime cTime = CTime::GetCurrentTime(); 
	CString path = m_logPath + Format(_T("%04d/%02d/"), cTime.GetYear(), cTime.GetMonth());
	CreateSubDirectories(path);
	CString filePath = path + Format(_T("%02d.csv"), cTime.GetDay());
	
	// 파일 열기
	CFile file;
	CFileException e;
	file.Open(filePath, CFile::shareDenyWrite | CFile::modeNoTruncate | CFile::modeWrite | CFile::modeCreate, &e);
	if (e.m_cause != CFileException::none)
	{
		// to do message
		TRACE(_T("%s File Exception Error : %d\n"), filePath, e.m_cause);
		return;
	}

	// log 파일 쓰기
	CArchiveU arU(&file, CArchiveU::store);
	if (file.SeekToEnd() == 0)
	{ // 파일 처음 생성시 항목을 적어주고 시작한다
		arU.WriteStringU(_T("시간;상황;알람번호;타입;알람명(한글);알람명(영어);알람명(베트남);유저;추가설명(한글);추가설명(영어);추가설명(베트남);\r\n"));
	}

	arU.WriteStringU(GetTimeStringHMS(true) + _T(";"));
	arU.WriteStringU(CString(_bOccurred ? _T("Occurred") : _T("Clear")) + _T(";"));
	arU.WriteStringU(ToCString((int)_alarm.iALID) + _T(";"));
	arU.WriteStringU(CString(_alarm.iALCD == 1 ? _T("Light") : _T("Heavy")) + _T(";"));
	arU.WriteStringU(CString(_alarm.AlarmData[eL_KOREAN].szText) + _T(";"));
	arU.WriteStringU(CString(_alarm.AlarmData[eL_ENGLISH].szText) + _T(";"));
	arU.WriteStringU(CString(_alarm.AlarmData[eL_VIETNAM].szText) + _T(";"));
	arU.WriteStringU(CString(sm.DeviceParameter->szCurUserID) + _T(";")); // to do change
	arU.WriteStringU(g_stDescription.GetCString(_alarm.iAlarmDesc, eL_KOREAN) + _T(";"));
	arU.WriteStringU(g_stDescription.GetCString(_alarm.iAlarmDesc, eL_ENGLISH) + _T(";"));
	arU.WriteStringU(g_stDescription.GetCString(_alarm.iAlarmDesc, eL_VIETNAM) + _T(";"));
	arU.WriteStringU(_T("\r\n"));

	arU.Close();
	file.Close();
}

BEGIN_IP_COMMAND_TABLE(CSystemAlarmDlg, IIPCommandTarget)
	ON_IP_EXCL("Alarm", "Clear", OnIP_AlarmClear)
	ON_IP_EXCL("Alarm", "SetLanguage", OnIP_SetLanguage)	// KES 추가 2024.1.12 
END_IP_COMMAND_TABLE()

BOOL CSystemAlarmDlg::OnIP_AlarmClear(CIPCommand& cmd)
{
	const CIPVar&	var			=	cmd.m_var;
	CIPVar&			varResult	=	cmd.m_varResult;
	CIPVar&			varCookie	=	cmd.m_varCookie;

	for (int iAlarm = 0; iAlarm < sm.Alarm->nALARM; ++iAlarm)
	{
		auto& alarm = sm.Alarm->ALARMs[iAlarm];

		// 발생 상태이고 클리어 가능한 알람만 클리어 가능
		if (!alarm.bOccurred || !alarm.bReset) continue;

		BOOL bOldOccurred = alarm.bOccurred;
		BOOL bOldReset = alarm.bReset;
		// 알람 상태 초기화
		alarm.bOccurred = FALSE;
		alarm.bReset = FALSE;


		// CIM에 보고한 알람은 클리어 보고 (CIM)
		if (alarm.bReport == TRUE) {
			//if (!sm.Sequence->bCIMSkip) {
				CIPVar varAlarm;
				varAlarm.SetChildItemT<T_SYSTEM_ALARM_ITEM>("T_SYSTEM_ALARM_ITEM", alarm);
				if (g_ipClient.SendIPCommand("UI", "SendMessage", "S5F1_ALARM_REPORT_SEND", varAlarm, 500)) {
					alarm.bReport = FALSE;
				}
				else {
					alarm.bOccurred = bOldOccurred;
					alarm.bReset = bOldReset;
				}
			//}
			//else {
			//	alarm.bOccurred = bOldOccurred;
			//	alarm.bReset = bOldReset;
			//}

		}
	
		// map에서 삭제
		if (alarm.bOccurred == FALSE) {
			m_Alarms.erase(alarm.iIndex); // mfc 메세지는 single thread여서 lock 안걸었음 (추후 필요하면 lock 추가할 것)

			// list에서 삭제
			for (int idx = 0; idx < m_alarmListctrl.GetItemCount(); ++idx)
			{
				//m_alarmListctrl.DeleteItem(idx); break;

				if (alarm.iIndex == _ttoi(m_alarmListctrl.GetItemText(idx, eAC_INDEX)))
				{
					m_alarmListctrl.DeleteItem(idx); break;
				}
			}
			
			// 클리어 로그
			WriteAlarmLog(alarm, false); 
			// to do 클리어 보고 (CIM)
		}
	}
	
	// 현재 발생한 알람이 없는 경우
	if (m_Alarms.empty())
	{
		//<< MCC
		sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI01].A[eMCC_ACTION_LD_LI_01_ALARM] = 0;
		sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI02].A[eMCC_ACTION_LD_LI_02_ALARM] = 0;
		sm.MCC_EQP->Layer[eMCC_LAYER_LD_LE01].A[eMCC_ACTION_LD_LE_01_ALARM] = 0;
		sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO01].A[eMCC_ACTION_LD_LO_01_ALARM] = 0;
		sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO02].A[eMCC_ACTION_LD_LO_02_ALARM] = 0;
		sm.MCC_EQP->Layer[eMCC_LAYER_LD_TR01].A[eMCC_ACTION_LD_TR_01_ALARM] = 0;
		//>>

		sm.Sequence->fIsAlarm = FALSE; // 알람 Off 발생 상태로 변경
		//-->24.05.02 KGH EDIT : 멜로디 정상화
		sm.IO->tOut_NEW.fLD_MELODY_2 = FALSE;
		//sm.IO->tOut_NEW.fLD_MELODY_1 = FALSE;
		//<--24.05.02 KGH EDIT : 멜로디 정상화
		// 알람 코드 Register 초기화
		memset(&sm.Register->Reg_Word_1_NEW.dwAlarmCode1, 0x00,sizeof(sm.Register->Reg_Word_1_NEW.dwAlarmCode1) * 20);
		// Title 에 표시할 알람 초기화
		sm.Alarm->iCurAlarmIndex = -1;
		ShowWindow(SW_HIDE);
		return TRUE;
	}

	bool bHeavy = false;
	for (auto& [idx, alarm] : m_Alarms)
	{
		if (sm.Alarm->ALARMs[idx].iALCD == 2)
		{
			bHeavy = true;
		}
	}

	if (!bHeavy)
	{
		//<< MCC
		sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI01].A[eMCC_ACTION_LD_LI_01_ALARM] = 0;
		sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI02].A[eMCC_ACTION_LD_LI_02_ALARM] = 0;
		sm.MCC_EQP->Layer[eMCC_LAYER_LD_LE01].A[eMCC_ACTION_LD_LE_01_ALARM] = 0;
		sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO01].A[eMCC_ACTION_LD_LO_01_ALARM] = 0;
		sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO02].A[eMCC_ACTION_LD_LO_02_ALARM] = 0;
		sm.MCC_EQP->Layer[eMCC_LAYER_LD_TR01].A[eMCC_ACTION_LD_TR_01_ALARM] = 0;
		//>>

		sm.Sequence->fIsAlarm = FALSE; // 알람 Off 발생 상태로 변경
	}
	else
	{
		sm.Sequence->fIsAlarm = TRUE; // 
	}

	return TRUE;
}

// KES 추가 2024.1.12 
BOOL CSystemAlarmDlg::OnIP_SetLanguage(CIPCommand& cmd)
{
	if (!IsWindowVisible()) {
		return TRUE;
	}
	
	//// 리스트 항목 지우고 Timer에서 다시 쓴다
	//m_alarmListctrl.DeleteAllItems();
	//m_Alarms.clear();
	//
	//int iCode = _ttof(m_alarmListctrl.GetItemText(m_alarmListctrl.GetSelectionMark(), eAC_INDEX));
	//SetAlarmDescription(iCode);
	int nItem = m_alarmListctrl.GetItemCount();
	if (nItem != 0) {
		for (int i = 0; i < nItem; i++) {
			int iIndex = m_alarmListctrl.GetItemInt(i, eAC_INDEX);
			auto iter = m_Alarms.find(iIndex);
			if (iter != m_Alarms.end()) {
				const auto& alarm = iter->second.second;

				m_alarmListctrl.SetItemText(i, eAC_AREA, Format(_T("%s"), alarm->AlarmData[sm.DeviceParameter->System.eLanguage].szArea));	// KES 수정 2024.1.9
				m_alarmListctrl.SetItemText(i, eAC_NAME, Format(_T("%s"), alarm->AlarmData[sm.DeviceParameter->System.eLanguage].szText));	// KES 수정 2024.1.9
			}

		}
	}
	
	int iCode = _ttof(m_alarmListctrl.GetItemText(m_alarmListctrl.GetSelectionMark(), eAC_INDEX));
	SetAlarmDescription(iCode);
	return TRUE;
}

// KES 추가 2024.1.11
void CSystemAlarmDlg::SetClipBoardText(CStringA strText) {
	if (strText.IsEmpty()) {
		return;
	}

	int nLen = strText.GetLength() + 1;
	HANDLE h_data = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, nLen);

	char* p_data = (char*)::GlobalLock(h_data);
	if (p_data != nullptr) {
		memcpy(p_data, strText, nLen);
		::GlobalUnlock(h_data);

		if (::OpenClipboard(m_hWnd)) {
			::EmptyClipboard();
			::SetClipboardData(CF_TEXT, h_data);
			::CloseClipboard();
		}
	}
}

// KES 추가 2024.1.12
void CSystemAlarmDlg::SetAlarmDescription(int _iCode) {
	if (auto iter = m_Alarms.find(_iCode); iter != m_Alarms.end())
	{
		auto& item = *(iter->second.second);
		auto& occurredTime = iter->second.first;
		m_alarmMsgEdit.SetWindowTextW(Format(_T("[%02d:%02d:%02d %d, %s] : %s")
			, occurredTime.GetHour(), occurredTime.GetMinute(), occurredTime.GetSecond()
			, item.iALID, item.iALCD == 2 ? _T("Heavy") : _T("Light"), item.AlarmData[sm.DeviceParameter->System.eLanguage].szText));	// KES 수정 2024.1.9
		
		// KES 작업 2024.1.10 - 알람 UI 팝업창 내용 수정 
		CString strHelp(_T(""));
		if (item.iAlarmDesc != IDS_DES_NOT_USE)	
			strHelp += _T("- ") + g_stDescription.GetCString(item.iAlarmDesc, sm.DeviceParameter->System.eLanguage) + _T("\r\n");
		if (item.iMotorDesc != -1 && item.iMotorDesc != IDS_DES_NOT_USE) {	
			strHelp += _T("- ") + g_stDescription.GetCString(item.iMotorDesc, sm.DeviceParameter->System.eLanguage) + _T("\r\n");
			strHelp += _T("- ") + (CString)item.szTargetPos;
		}
		m_helpMsgEdit.SetWindowTextW(strHelp);	

		CString strStepName(item.szSeqStep);
		m_StepNameMsgEdit.SetWindowTextW(strStepName);
	}
}


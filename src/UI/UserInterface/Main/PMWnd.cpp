

#include "pch.h"
#include "UI.h"
#include "MainFrame.h"
#include "PMWnd.h"
#include "PopupMapWnd.h"    // KES 추가 2024.3.21

#define PM_WND_REFRESH 6969
IMPLEMENT_DYNCREATE(CPMWnd, CFrameWnd)

CPMWnd::CPMWnd(CWnd* pwndParent) :TIPCT<CFrameWnd>(&g_ipClient)
{
	m_pwndParent		= pwndParent;
	m_eSelectControl = m_eEditEnd = eC_NONE;
	m_strEditPassword	= _T("");
	m_strPassword		= _T("");
}

CPMWnd::~CPMWnd()
{
    SAFE_DELETE(m_pWndPopupMap);   // KES 추가 2024.3.21
}

BEGIN_MESSAGE_MAP(CPMWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_APP_ED_EDIT_FINISHED,		OnEditFinished)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BEGIN_IP_COMMAND_TABLE(CPMWnd, IIPCommandTarget)
ON_IP("PM", "Show", OnIPShow)
ON_IP("PM", "ModeChange", OnIPModeChange)
ON_IP("SET", "DoorRelease", OnIPDoorRelease)
END_IP_COMMAND_TABLE()

int CPMWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {
		// 2021.06.10
		sm.Sequence->fPMMode = FALSE;

		SyncData();
		ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);					// Remove Title Bar
		ModifyStyleEx(this->m_hWnd, WS_EX_CLIENTEDGE, 0, 0);

		LPCTSTR szName = _T("../img/pm_korea.png");
		

		HRESULT		hResult;
		hResult = m_img.Load(szName);

		if (FAILED(hResult)) {
		CString strTmp = _T("ERROR: Failed to load an PM image (Check an Icon in PMWnd)");
		AfxMessageBox(strTmp);
	}
		
}
catch (...) {
	return -1;
}

	// KES 추가 2024.3.21
    m_pWndPopupMap = new CPopupMapWnd();
	m_pWndPopupMap->Create(nullptr, nullptr, NULL, CRect(), this);
	m_pWndPopupMap->ShowWindow(SW_HIDE);
	
	return 0;
}


void CPMWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	if (bShow) {
		//SetWindowPos(&wndTopMost, 0,0, 1920,1080, SWP_SHOWWINDOW);
		SetWindowPos(&wndTop, 0, 0, 1920, 1080, SWP_SHOWWINDOW);
		m_strEditPassword =_T("");
		fn_ComputeArea();
		Invalidate(FALSE);
		//AfxGetMainWnd()->ShowWindow(SW_HIDE);
		//((CMainFrame*)AfxGetMainWnd())->ClosePopupUI();
		SetForegroundWindow();
		SetFocus();
		SetTimer(PM_WND_REFRESH, 100, NULL);
	}
	else {
		KillTimer(PM_WND_REFRESH);
	}
}


void CPMWnd::OnPaint()
{
	CPaintDC dc(this); 
	CMemDCEx::CMemDC memDC(&dc);
	CDrawSymbol::DrawLineRect(&memDC, m_rectS[eS_FULL_AREA], g_colorForeground[eEC_DEFAULT]);
	
	CRect rect = m_rectS[eS_FULL_AREA];
	rect.DeflateRect(1,1);
	memDC.FillSolidRect(rect, g_colorBackground[eEC_DEFAULT]);

	CDrawSymbol::DrawText(memDC, m_rectS[eS_PASSWORD], RGB_WHITE, DT_RIGHT, 15, _T("Password : "));

	CString strTemp;
	for (int i = 0; i < m_strEditPassword.GetLength(); i++)
		strTemp += _T("●");
	CDrawSymbol::SelectedDrawText(memDC, m_rectC[eC_PASSWORD], RGB_BLACK, RGB_WHITE, RGB_BLACK, DT_CENTER, 13, strTemp);

	DrawMDIMenuButtonN(memDC, m_rectC[eC_OK], g_st[IDS_OK], m_eSelectControl == eC_OK);
	if (m_img)
		m_img.Draw(memDC, m_rectS[eS_IMAGE_AREA]);

	DrawMDIMenuButtonN(memDC, m_rectC[eC_POPUP_MAP], _T("View Map"), m_eSelectControl == eC_POPUP_MAP); // KES 추가 2024.3.21

	// Door 의 상태 표시는 첫번째 Door 만 확인 해서 진행 해도 됨 
	DrawMDIMenuButtonN(memDC, m_rectC[eC_DOOR_LOCK], _T("Door Lock"), sm.IO->tOut_CRT.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST || m_eSelectControl == eC_DOOR_LOCK);
}


void CPMWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	for (int i=0; i<eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			InvalidateRect(m_rectC[i],FALSE);
			break;
		}
	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}


void CPMWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString strLog;
	if ((m_eSelectControl != eC_NONE) && m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_PASSWORD : {
				EditStart(this, m_rectC[eC_PASSWORD], m_strEditPassword, TRUE, FALSE, DT_CENTER | ES_PASSWORD | ES_AUTOHSCROLL, 9);
				break;
			}
			case eC_OK : {
				OnOK();
				m_strEditPassword = _T("");
				strLog = _T("Ok");
				break;
			}
			case eC_MODE_CHANGE: {
				break;
			}
            case eC_POPUP_MAP: {    // KES 추가 2024.3.21
			    m_pWndPopupMap->ShowWindow(SW_SHOW);
				break;
			}
			case eC_DOOR_LOCK: {
				strLog = _T("Lock");
				if (m_strPassword.CompareNoCase(m_strEditPassword) == 0) {
					if (sm.IO->tOut_CRT.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST)
						break;

					CString str;
					if (CheckAllDoorCloseLD() && CheckAllDoorCloseSCT()) {
						sm.IO->tOut_NEW.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST		= TRUE;
						sm.IO->tOut_NEW.fSCT_DUMMY_DOOR_LOCK_RELEASE_REQUEST	= TRUE;
						sm.Option->tSequence.bKeepOnDoorOpenMelody = FALSE;
						//-->24.05.02 KGH EDIT : 멜로디 정상화
						sm.IO->tOut_NEW.fLD_MELODY_1 = FALSE;
						//sm.IO->tOut_NEW.fLD_MELODY_6 = FALSE;
						//<--24.05.02 KGH EDIT : 멜로디 정상화

						//램프 다 꺼줌
						sm.IO->tOut_NEW.fLD_DOOR01_RELEASE_SW = FALSE;
						sm.IO->tOut_NEW.fLD_DOOR02_RELEASE_SW = FALSE;
						sm.IO->tOut_NEW.fLD_DOOR03_RELEASE_SW = FALSE;
						sm.IO->tOut_NEW.fLD_DOOR04_RELEASE_SW = FALSE;
						sm.IO->tOut_NEW.fLD_DOOR05_RELEASE_SW = FALSE;
						sm.IO->tOut_NEW.fLD_DOOR06_RELEASE_SW = FALSE;
						
						//SCT
						//IP함수 호출하여 Door Release Sw 들 OFF 시켜 주도록 할것
						CIPVar var;
						var.SetChildItem("SET", FALSE);
						g_ipClient.PostIPCommand("UI", "SET", "DoorRelease", var);

						str += g_st[IDS_PM_WND_DOOR_LOCK_SUCCESS];
					}
					else
					{
						str += _T("%s\r\n\r\n",g_st[IDS_PM_WND_DOOR_LOCK_FAIL]);
						vector<int> vecDoorNum;
						if (!CheckAllDoorCloseSCT())
						{
							str += _T("%s\r\n",g_st[IDS_PM_WND_DOOR_NOT_CLOSED_OR_KEY_INTERNAL_KEY_BOX_SCT]);
							str += _T("%s : ",g_st[IDS_PM_WND_RELATED_DOOR]);
							GetOpenDoorNumberSCT(vecDoorNum);
							for (int i = 0; i < vecDoorNum.size(); i++)
							{
								if (vecDoorNum.at(i) > 10)
									str += Format(L"Dummy Number: %d", vecDoorNum.at(i) - 10);
								else
									str += Format(L"Number: %d", vecDoorNum.at(i));

								if (i == vecDoorNum.size() - 1)
									str += "\r\n";
								else
									str += ", ";
							}
						}
						vecDoorNum.clear();
						if (!CheckAllDoorCloseLD())
						{
							str += _T("%s\r\n",g_st[IDS_PM_WND_DOOR_NOT_CLOSED_OR_KEY_INTERNAL_KEY_BOX_LD]);
							str += _T("%s:",g_st[IDS_PM_WND_RELATED_DOOR]);
							GetOpenDoorNumberLD(vecDoorNum);
							for (int i = 0; i < vecDoorNum.size(); i++)
							{
								if (vecDoorNum.at(i) > 10)
									str += Format(L"Dummy Number: %d", vecDoorNum.at(i) - 10);
								else
									str += Format(L"Number: %d", vecDoorNum.at(i));

								if (i == vecDoorNum.size() - 1)
									str += "\r\n";
								else
									str += ", ";
							}
						}
					}
					IPMessage(str, 10 * 1000);
				}
				else
					IPMessage(_T("Password Fail"));
				m_strEditPassword = _T("");
				break;
			}
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(_T("PM"), _T("PM"), _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eEditEnd = m_eSelectControl;
	m_eSelectControl = eC_NONE;
	Invalidate(FALSE);

	CFrameWnd::OnLButtonUp(nFlags, point);
}


void CPMWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	int eMouseOver = eC_NONE;
	if ((m_eSelectControl != eC_NONE) && (!m_rectC[m_eSelectControl].PtInRect(point))) {
		int eSelectControl = m_eSelectControl;
		m_eSelectControl = eC_NONE;
		InvalidateRect(m_rectC[eSelectControl], FALSE);
	}

	for (int i = 0; i < eC_END; i++) {
		if (eMouseOver != i) {
			if (m_rectC[i].PtInRect(point)) {
				InvalidateRect(m_rectC[i], FALSE);
				eMouseOver = i;
				
				break;
			}
		}
	}
	if (eMouseOver != m_eMouseOver) {
		
		if (m_eMouseOver != eC_NONE)
			InvalidateRect(m_rectC[m_eMouseOver], FALSE);
		m_eMouseOver = eMouseOver;
	}

	CFrameWnd::OnMouseMove(nFlags, point);
}

void CPMWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect rect = m_rectS[eS_FULL_AREA];

	int iW = (rect.Width() - 5) / 10;
	int iH = (rect.Height() - 5) / 12;

	m_rectS[eS_IMAGE_AREA]		= CRect(rect.left+5+iW*2,	rect.top+5+iH*0+20,		rect.left+iW*8,		rect.top+iH*10);

	m_rectS[eS_PASSWORD]		= CRect(rect.left+5+iW*2,	rect.top+5+iH*11,		rect.left+iW*4,		rect.top+iH*11.5);
	m_rectC[eC_PASSWORD]		= CRect(rect.left+5+iW*4,	rect.top+5+iH*11,		rect.left+iW*6,		rect.top+iH*11.5);
	m_rectC[eC_OK]				= CRect(rect.left+5+iW*6,	rect.top+5+iH*11,		rect.left+iW*7,		rect.top+iH*11.5);

	m_rectC[eC_POPUP_MAP    ]	= CRect(rect.left+5+iW*8,	rect.top+5+iH*10.5,		rect.left+iW*9,		rect.top+iH*11);  // KES 추가 2024.3.21
	m_rectC[eC_DOOR_LOCK	]	= CRect(rect.left+5+iW*8,	rect.top+5+iH*11,		rect.left+iW*9,		rect.top+iH*11.5);
}

void CPMWnd::OnOK() {
	CString strResult;

	BOOL bCheckCondiPW = FALSE;
	BOOL bCheckCondiMC = FALSE;
	BOOL bCheckCondiDoor = FALSE;
	bCheckCondiPW = m_strPassword.CompareNoCase(m_strEditPassword) == 0;
	//bCheckCondiMC = sm.IO->tIn_CRT.fSCT_MSB_MC11_ON;	//MC ON 상태 체크   // KES 주석 R32 Version에서 어떤게 MC인지 모르겠음. 설비 확인 필요.
	bCheckCondiDoor = !CheckDoorOpen_SCT_LD();	//열린문 체크(하나만 열려도 TRUE)

	// 2022.05.11 Door Close일 때만 PM창 닫을 수 있도록 수정
	if (bCheckCondiPW && (bCheckCondiMC || bCheckCondiDoor)) 
	{
		if (bCheckCondiPW && bCheckCondiDoor) {
			//sm.Option->tSequence.bPMShow = FALSE;
			ShowWindow(SW_HIDE);
			m_pWndPopupMap->ShowWindow(SW_HIDE);    // KES 추가 2024.3.21
		}
	}
	else {
		if (!bCheckCondiPW)
		{
			IPMessage(g_st[IDS_PM_WND_PASSWORD_MISMATCH]);
		}
		else
		{
			// 2022.05.11 Door Close일 때만 PM창 닫을 수 있도록 수정
			//IPMessage(_T("PM 화면 해제 조건 중 일치하는 조건이 없습니다.\r\n1.해당 MC 신호가 On.\r\n2.모든 문이 닫힘."));
			IPMessage(g_st[IDS_PM_WND_ALL_DOOR_CLOSE_CHECK]);
		}
	}
		//ShowWindow(SW_HIDE);
}

BOOL CPMWnd::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			OnOK();
		}
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

LRESULT CPMWnd::OnEditFinished(WPARAM wParam, LPARAM lParam) {

	if (wParam) {
		CString str = GetEditString();
		switch (m_eEditEnd) {
			case eC_PASSWORD : {
				m_strEditPassword = str;
				break;
			}
			
		}
	}

	m_eEditEnd = eC_NONE;
	return 0L;
}



BOOL CPMWnd::OnIPShow(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	if (!sm.Sequence->fAutoMode || (sm.Sequence->fAutoMode && sm.Sequence->fCycleStop))
	{
		ShowWindow(SW_SHOW);
	}
	else {
		IPMessage(g_st[IDS_PM_WND_CAN_NOT_ACTIVATED_PM_MODE]);
	}

	return TRUE;
}
BOOL CPMWnd::SyncData() {
	CProfile profile(_T("../cfg/PM.ini"));
	profile.SyncProfileValue(FALSE, _T("Password"), _T("Password"), m_strPassword);
	return TRUE;
}
BOOL CPMWnd::OnIPModeChange(CIPCommand& cmd) {


	return TRUE;
}

BOOL CPMWnd::OnIPDoorRelease(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varResult = cmd.m_varResult;

	g_ipCIMClient.PostIPCommand("SCT", "SET", "DoorRelease", var);

	// 2022.04.27 Door Open 시점에 Servo Off
	AllServoOnOff(FALSE);

	return TRUE;
}

void CPMWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case PM_WND_REFRESH:{
			Invalidate(FALSE);
			break;
		}
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CPMWnd::OnClose()
{
	ShowWindow(SW_HIDE);
}
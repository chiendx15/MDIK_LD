#include "pch.h"
#include "UI.h"
#include "IOStatusWnd.h"



IMPLEMENT_DYNCREATE		(CIOStatusWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CIOStatusWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
END_MESSAGE_MAP			()



CIOStatusWnd::CIOStatusWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
	m_pIn_CRT			= nullptr;
	m_pOut_CRT			= nullptr;
	m_pOut_NEW			= nullptr;
	m_pOut_Rel			= nullptr;
	
	m_bToggle			= TRUE;
}
CIOStatusWnd::~CIOStatusWnd() {
}



BOOL CIOStatusWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}






int CIOStatusWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
    try {
    	m_pIn_CRT	= (DWORD*)&sm.IO->tIn_CRT;
    	m_pOut_CRT	= (DWORD*)&sm.IO->tOut_CRT;
    	m_pOut_NEW	= (DWORD*)&sm.IO->tOut_NEW;
    	SyncData(FALSE);		// 처음 로딩 시 공유메모리에 저장
    	m_pOut_Rel	= (DWORD*)&sm.IO->tOut_Release;
    } 
    catch (...) {
    	return -1;
    }
    
    fn_MakeIOIndexMap();
	return 0;
}



void CIOStatusWnd::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case INTERVAL_IO: {
			Invalidate(FALSE);
			break;
		}
		default:
			break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CIOStatusWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width(), g_rectArea[eAREA_WORK].Height(), SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		m_eSelectTabIn	= 0;
		m_eSelectTabOut = 0;
		Invalidate();
		SetTimer(INTERVAL_IO, 100, NULL);
		m_bToggle = TRUE;
	}
	else {
		KillTimer(INTERVAL_IO);
	}
}


BOOL CIOStatusWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}	

void CIOStatusWnd::OnPaint() {
	CPaintDC			dc(this);
	CMemDCEx::CMemDC	memDC(&dc);
	CRect				rect = m_rectS[eS_FULL_AREA];
	memDC.FillSolidRect(rect, g_colorBackground[eEC_DISABLE]);
	rect.DeflateRect(3, 3, 0, 0);
	CDrawSymbol::DrawLineRect(&memDC, rect, g_colorForeground[eEC_DEFAULT]);

	if (!m_bToggle) {
		DrawButtonUpDown(&memDC, m_rectC[eC_NUMBER_TOGGLE], _T("Number"), m_eSelectControl == eC_NUMBER_TOGGLE);
	}

    int iIn = m_mapIOIndexIn[m_eSelectTabIn] * __MAX_IO_ITEM__; 
    if (m_mapIOIndexIn[m_eSelectTabIn] > 32) {  //113FO ~ 113FF IO 없음
        iIn -= 16;
    }
	int iOut = m_mapIOIndexOut[m_eSelectTabOut] * __MAX_IO_ITEM__;
    if (m_mapIOIndexOut[m_eSelectTabOut] > 32) {  //113FO ~ 113FF IO 없음
        iOut -= 16;
    }
	for (int i = 0; i < (--m_mapIOIndexIn.end())->first + 1; i++) 
	{
		DrawMDIMenuButton(&memDC, m_rectC[eC_TAB_IN + i], Format(_T("In %d"), i + 1), m_eSelectTabIn == i);
	}

	for (int i = 0; i < __MAX_IO_ITEM__; i++) {
        // map data error
        if (m_mapIOIndexIn.find(m_eSelectTabIn) == m_mapIOIndexIn.end()) {
            break;
        }
        // 113FO ~ 113FF IO 없음
        if ((m_mapIOIndexIn[m_eSelectTabIn] == 32) && i >= 16) {
            break;
        }

		CString strIO;
		CString strAddr;
        CString strIn;
		if (m_mapIOIndexIn[m_eSelectTabIn] == 0) {
            if (m_bToggle) {
			    strAddr = Format(_T("ib%05X"), 0x10000 + i + m_mapIOIndexIn[m_eSelectTabIn] * __MAX_IO_ITEM__);
            }
            else {
                strAddr = Format(_T("ib%03d"), (__MAX_IO_ITEM__ * m_mapIOIndexIn[m_eSelectTabIn])+i );
            } 
            strIn = g_stIO[IDS_IO_IN_LD_IB10000 + i + m_mapIOIndexIn[m_eSelectTabIn] * __MAX_IO_ITEM__];
        }
        else { // 0x11000 ~ 0x113EF
            if (m_bToggle) {
                strAddr = Format(_T("ib%05X"), 0x11000 + i + (m_mapIOIndexIn[m_eSelectTabIn]-1) * __MAX_IO_ITEM__);
            }
            else {
                strAddr = Format(_T("ib%03d"), (__MAX_IO_ITEM__ * m_mapIOIndexIn[m_eSelectTabIn])+i );
            }
            strIn = g_stIO[IDS_IO_IN_LD_IB11000 + i + (m_mapIOIndexIn[m_eSelectTabIn]-1) * __MAX_IO_ITEM__];
        }    
		strIO = Format(_T("[%s]  %s"), strAddr, strIn);
		DrawEllipseSensor(&memDC, m_rectC[eC_IN_0 + i], strIO,
			m_pIn_CRT[iIn + i], CPoint(10, 10), DT_SENSOR_ON, DT_SENSOR_OFF, 8, DT_PEN, RGB_BLACK, i % 2 ? RGB_GRAY : RGB_GRAY_LIGHT, DT_LEFT | DT_VCENTER);
	}

	for (int i = 0; i < (--m_mapIOIndexOut.end())->first + 1; i++) 
	{
		DrawMDIMenuButton(&memDC, m_rectC[eC_TAB_OUT + i], Format(_T("Out %d"), i + 1), m_eSelectTabOut == i);
	}
		
	for (int i = 0; i < __MAX_IO_ITEM__; i++) {
        // map data error
        if (m_mapIOIndexOut.find(m_eSelectTabOut) == m_mapIOIndexOut.end()) {
            break;
        }
        // 113FO ~ 113FF IO 없음
        if ((m_mapIOIndexOut[m_eSelectTabOut] == 32) && i >= 16) {
            break;
        }

		CString strIO;
		CString strAddr;
        CString strOut;
		if (m_mapIOIndexOut[m_eSelectTabOut] == 0) {
            if (m_bToggle) {
			    strAddr = Format(_T("ob%05X"), 0x10000 + i + m_mapIOIndexOut[m_eSelectTabOut] * __MAX_IO_ITEM__);
            }
            else {
                strAddr = Format(_T("ob%03d"), (__MAX_IO_ITEM__ * m_mapIOIndexOut[m_eSelectTabOut]) + i);
            }
            strOut = g_stIO[IDS_IO_OUT_LD_OB10000 + i + m_mapIOIndexOut[m_eSelectTabOut] * __MAX_IO_ITEM__];
        }
        else { // 0x11000 ~ 0x113EF
            if (m_bToggle) {
                strAddr = Format(_T("ob%05X"), 0x11000 + i + (m_mapIOIndexOut[m_eSelectTabOut]-1) * __MAX_IO_ITEM__);
            }
            else {
                strAddr = Format(_T("ob%03d"), (__MAX_IO_ITEM__ * m_mapIOIndexOut[m_eSelectTabOut]) + i);
            }
            strOut = g_stIO[IDS_IO_OUT_LD_OB11000 + i + (m_mapIOIndexOut[m_eSelectTabOut]-1) * __MAX_IO_ITEM__];
        }
		strIO = Format(_T("[%s]  %s"), strAddr, strOut);
		DrawEllipseSensor(&memDC, m_rectC[eC_OUT_0 + i], strIO,
			m_pOut_CRT[iOut + i], CPoint(10, 10), DT_SENSOR_ON, DT_SENSOR_OFF, 8, DT_PEN, RGB_BLACK, i % 2 ? RGB_GRAY : RGB_GRAY_LIGHT, DT_LEFT | DT_VCENTER);

		CString strLock(_T("Unused"));
		COLORREF colBack	= DT_BTN_UP_BK;
		COLORREF colText	= DT_BTN_UP_TEXT;
		switch (m_pOut_Rel[iOut + i]) {
			case eIO_LOCK_UI: {				// UI Lock
				strLock = _T("UI\nLock");
				colBack = RGB_RED;
				colText = RGB_BLACK;
				break;
			}
			case eIO_LOCK_CONTROL: {				// Control Lock
				strLock = _T("Control\nLock");
				colBack = RGB_YELLOW;
				colText = RGB_BLACK;
				break;
			}
		}
		if (m_eSelectControl != eC_OUT_RELEASE + i) {
			SelectedDrawText(&memDC, m_rectC[eC_OUT_RELEASE + i], strLock, 7, DT_BTN_UP_PEN, colBack, colText);
		}
		else {
			DrawButtonUpDown(&memDC, m_rectC[eC_OUT_RELEASE + i], strLock, m_eSelectControl == eC_OUT_RELEASE + i, DT_BTN_DOWN_BK, DT_BTN_UP_BK, 7);
		}
	}
}



void CIOStatusWnd::OnMouseMove(UINT nFlags, CPoint point) {
	int eMouseOver = m_eMouseOver;
	eMouseOver = eC_NONE;

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
	CFrameWndEx::OnMouseMove(nFlags, point);
}



void CIOStatusWnd::OnLButtonDown(UINT nFlags, CPoint point) {
	SetFocus();
	m_eSelectControl = eC_NONE;
	for (int i = 0; i<eC_END; i++) {
		if (m_rectC[i].PtInRect(point)) {
			m_eSelectControl = i;
			InvalidateRect(m_rectC[i],FALSE);
			break;
		}
	}

	CFrameWndEx::OnLButtonDown(nFlags, point);
}



void CIOStatusWnd::OnLButtonUp(UINT nFlags, CPoint point)  {
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		if (m_eSelectControl >= eC_TAB_IN && m_eSelectControl <= eC_TAB_IN_END) {
			m_eSelectTabIn = m_eSelectControl - eC_TAB_IN;
			return;
		}
		if (m_eSelectControl >= eC_TAB_OUT && m_eSelectControl <= eC_TAB_OUT_END) {
			m_eSelectTabOut = m_eSelectControl - eC_TAB_OUT;
			return;
		}

		//<< KES 추가 2024.4.9 - 운전모드가 Manual이 아니면 수동 동작 X
		if (sm.Option->tSequence.eDriveMode != eDRIVE_MODE_MANUAL) {
			IPMessage(g_st[IDS_MS_NOT_DRIVE_MODE_MANUAL]);
			return;
		}
		//>>	

		// 해당 신호가 들어와잇는 경우 IO 제어 안되도록 
		CString strAddr;
        if (m_mapIOIndexOut[m_eSelectTabOut] == 0) {
			strAddr = Format(_T("ob%05X"), 0x10000 + m_eSelectControl - eC_OUT_0 + m_mapIOIndexOut[m_eSelectTabOut] * __MAX_IO_ITEM__);
        }
        else {  // 0x11000 ~ 0x113EF
			strAddr = Format(_T("ob%05X"), 0x11000 + m_eSelectControl - eC_OUT_0 + (m_mapIOIndexOut[m_eSelectTabOut] - 1) * __MAX_IO_ITEM__);
        }

		if (!sm.IO->tIn_CRT.fLD_PDT_ADAPTOR_SELECT_KEY_SWITCH_AUTO_POSITION) {
			if (m_eSelectControl >= eC_OUT_0 && m_eSelectControl <= eC_OUT_END) {
				int iIndex = m_mapIOIndexOut[m_eSelectTabOut] * __MAX_IO_ITEM__ + m_eSelectControl - eC_OUT_0;
				if (m_pOut_Rel[iIndex] == eIO_LOCK_UNUSED)
					m_pOut_NEW[iIndex] = !m_pOut_NEW[iIndex];
				strLog = Format(_T("[%s] %s"), strAddr, g_stIO[IDS_IO_OUT_LD_OB10000 + iIndex]);
			}

			if (m_eSelectControl >= eC_OUT_RELEASE && m_eSelectControl <= eC_OUT_RELEASE_END) {
				int iIndex = m_mapIOIndexOut[m_eSelectTabOut] * __MAX_IO_ITEM__ + m_eSelectControl - eC_OUT_RELEASE;
                CString strTemp("qrs");// = InPutMessage(g_st[IDS_IO_STATUS_WND_INSERT_PASSWORD], TRUE, _T("Password"));
				if (strTemp.Compare(_T("qrs")) != 0)
					return;
				else {
					CString strResp = RespMessage(g_st[IDS_IO_STATUS_WND_SELECT_LOCK_TYPE], _T("UI;Control;Unused"));
					if (strResp.Compare(_T("UI")) == 0) {				
						m_pOut_Rel[iIndex] = eIO_LOCK_UI;					
					}
					else if (strResp.Compare(_T("Control")) == 0) {
						m_pOut_Rel[iIndex] = eIO_LOCK_CONTROL;					
					}
					else if (strResp.Compare(_T("Unused"))==0) {
						//CString strResp = InPutMessage(g_st[IDS_IO_STATUS_WND_UNSUED_ONLY], TRUE, _T("Password"));
						//CTime time = CTime::GetCurrentTime();
						//CString strHor = time.GetHour() > 12 ? _T("PM") : _T("AM");
						//int iHor = time.GetHour() > 12 ? time.GetHour() - 12 : time.GetHour();
						//CString strPass= Format(_T("%04d%02d%02d%s%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), strHor,iHor,time.GetMinute());

						//if (strResp.CompareNoCase(strPass) == 0) {
							m_pOut_Rel[iIndex] = eIO_LOCK_UNUSED;					
						//}
					}
					SyncData(TRUE);
				}
			}
		}
		else {
			// 팬던트 연결 IO 가 정상적으로 들어오지 않았을 때에도 어떤 이유인지 확인 할 수 있기 위해서
			IPMessage(g_st[IDS_IO_STATUS_WND_CAN_NOT_CTRL_PENDANT_CONNECTED]);
		}
		if (m_eSelectControl == eC_NUMBER_TOGGLE) {
			m_bToggle = !m_bToggle;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(g_st[IDS_MAIN_MENU_0 + g_eSelectedMainMenu], g_st[IDS_SM_1_1 + ((g_eSelectedMainMenu - eMAIN_MENU_1) * eSUB_MENU_END) + g_eSelectedSubMenu], _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl	=	eC_NONE;
	fn_ComputeArea();
	Invalidate(FALSE);

	CFrameWndEx::OnLButtonUp(nFlags, point);
}



void CIOStatusWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect	rc = m_rectS[eS_FULL_AREA];
	double	dW = (rc.Width()-5) / 2;
	double	dH = (rc.Height()-5) /10;

	m_rectS[eS_TAB_IN_AREA]		=  CRect(rc.left+5+dW*0, rc.top+5+dH*0,		rc.left+dW*1, rc.top+dH*1+15);
	m_rectS[eS_TAB_IN_IO_AREA]	=  CRect(rc.left+5+dW*0, rc.top+15+dH*1,	rc.left+dW*1, rc.top+dH*10);
	m_rectS[eS_TAB_OUT_AREA]	=  CRect(rc.left+5+dW*1, rc.top+5+dH*0,		rc.left+dW*2, rc.top+dH*1+15);
	m_rectS[eS_TAB_OUT_IO_AREA]	=  CRect(rc.left+5+dW*1, rc.top+15+dH*1,	rc.left+dW*2, rc.top+dH*10);

	int iLoop = (--m_mapIOIndexIn.end())->first + 1;
    iLoop += 2; // KES 2024.2.26 - eC_NUMBER_TOGGLE 버튼 공간 때문에 하드코딩
	rc = m_rectS[eS_TAB_IN_AREA];
	dW = (rc.Width()-5) / ceil(iLoop /2.);
	dH = (rc.Height()-5) / 2;

	for (int i = 0; i < iLoop; i++) {
        if (i >= (--m_mapIOIndexIn.end())->first + 1) continue;    // KES 2024.2.26 - eC_NUMBER_TOGGLE 버튼 공간 때문에 하드코딩
		int iTemp = ceil(iLoop / 2.);
		int ix = i % iTemp;
		int iy = i / iTemp;
		m_rectC[eC_TAB_IN+i]		=  CRect(rc.left+dW*ix, rc.top+5+dH*iy, rc.left+dW*(1 + ix), rc.top+5+dH*(1+iy));
	}
	rc = m_rectS[eS_TAB_IN_IO_AREA];
	dW = (rc.Width()-5) / 2;
	dH = (rc.Height()-5) /16;
	for (int i = 0; i < __MAX_IO_ITEM__; i++) {
		int ix = i /16;
		int iy = i %16;
		m_rectC[eC_IN_0+i]			= CRect(rc.left+5+dW*ix, rc.top+5+dH*iy, rc.left+dW*(1+ix), rc.top+dH*(1+iy));
	}
	
	iLoop = (--m_mapIOIndexOut.end())->first + 1;
    iLoop += 2; // KES 2024.2.26 - eC_NUMBER_TOGGLE 버튼 공간 때문에 하드코딩
    rc = m_rectS[eS_TAB_OUT_AREA];
	dW = (rc.Width()-5) / ceil(iLoop/2.);
	dH = (rc.Height()-5) /2;

	for (int i = 0; i < iLoop; i++) {
        if (i >= (--m_mapIOIndexOut.end())->first + 1) continue;    // KES 2024.2.26 - eC_NUMBER_TOGGLE 버튼 공간 때문에 하드코딩
		int iTemp = ceil(iLoop / 2.);
		int ix = i % iTemp;
		int iy = i / iTemp;
		m_rectC[eC_TAB_OUT+i]		=  CRect(rc.left+dW*ix, rc.top+5+dH*iy, rc.left+dW*(1+ ix), rc.top+5+dH*(1+iy));
	}
	int iTemp = ceil(iLoop/2.);
	m_rectC[eC_NUMBER_TOGGLE]		=  CRect(rc.left+dW*(iTemp-1), rc.top+5+dH*1, rc.left+dW*iTemp, rc.top+5+dH*2);

	rc = m_rectS[eS_TAB_OUT_IO_AREA];
	dW = (rc.Width()-5) / 2;
	dH = (rc.Height()-5) /16;
	for (int i = 0; i < __MAX_IO_ITEM__; i++) {
		int ix = i /16;
		int iy = i %16;
		m_rectC[eC_OUT_0+i]			= CRect(rc.left+5+dW*ix, rc.top+5+dH*iy, rc.left+dW*(1+ix), rc.top+dH*(1+iy));
		m_rectC[eC_OUT_RELEASE + i] = CRect(rc.left+dW*(1+ix)-35, rc.top+5+dH*iy, rc.left+dW*(1+ix), rc.top+dH*(1+iy));
	}
}
void CIOStatusWnd::SyncData(BOOL bStore) {
	CProfile profile(_T("../cfg/IO_Release.ini"));
	//CProfileSection& section = profile.GetSection(_T("IO"));

	BOOL* bIO = (BOOL*)&sm.IO->tOut_Release;
	if (bStore) 
		profile.GetSections().DeleteAll();

	for (int i = 0; i < sizeof(sm.IO->tOut_Release) / sizeof(DWORD); i++)
		profile.SyncProfileValue(bStore, _T("IO"), Format(_T("%03d"), i), bIO[i]);
	
	if (bStore) {
		profile.Save();
	}
}



// KES 추가 2024.2.22 - UI에서 Spare IO만 32개 있는 Tap을 Blocking 하기 위한 Index를 만드는 함수
void CIOStatusWnd::fn_MakeIOIndexMap() {
    m_mapIOIndexIn.clear();
    m_mapIOIndexOut.clear();

    // In
    int iIOTapIndex = 0;
    for (int iTap = 0; iTap < __MAX_IN_TAB__; iTap++) {
        for (int iIO = 0; iIO < __MAX_IO_ITEM__; iIO++) {
            if (iTap == 32 && iIO >= 16) {  // 113FO ~ 113FF IO 없음
                break;
            }
            CString str;
            if (iTap > 32) {
                str = g_stIO[IDS_IO_IN_LD_IB10000 + iIO + iTap * __MAX_IO_ITEM__ - 16];
            }
            else {
                str = g_stIO[IDS_IO_IN_LD_IB10000 + iIO + iTap * __MAX_IO_ITEM__];
            } 
            if (str != _T("")) {
                m_mapIOIndexIn.insert(std::make_pair(iIOTapIndex, iTap));
                iIOTapIndex++;
                break;
            }
        }
    }

    // Out
    iIOTapIndex = 0;
    for (int iTap = 0; iTap < __MAX_OUT_TAB__; iTap++) {
        for (int iIO = 0; iIO < __MAX_IO_ITEM__; iIO++) {
            if (iTap == 32 && iIO >= 16) {  // 113FO ~ 113FF IO 없음
                break;
            }
            CString str;
            if (iTap > 32) {
                str = g_stIO[IDS_IO_OUT_LD_OB10000 + iIO + iTap * __MAX_IO_ITEM__ - 16];
            }
            else {
                str = g_stIO[IDS_IO_OUT_LD_OB10000 + iIO + iTap * __MAX_IO_ITEM__];
            } 
            if (str != _T("")) {
                m_mapIOIndexOut.insert(std::make_pair(iIOTapIndex, iTap));
                iIOTapIndex++;
                break;
            }
        }
    }
}
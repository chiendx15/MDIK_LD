// AxisStatusView.cpp: 구현 파일
//

#include "pch.h"
#include "PLCLinker.h"
#include "AxisStatusView.h"


// CAxisStatusView

IMPLEMENT_DYNAMIC(CAxisStatusView, CWnd)

CAxisStatusView::CAxisStatusView()
{
	m_iScrollPos = 0;
}

CAxisStatusView::~CAxisStatusView()
{
}

void CAxisStatusView::ComputArea() 
{
	if (m_tStatus.size())
	{
		for (auto& item : m_tStatus)
		{
			item.bSelect = FALSE;
		}
		m_tStatus[0].bSelect = TRUE;
	}

	GetClientRect(m_rectClient);
	CRect rc = m_rectClient;
	
	m_rectS[eS_AREA_MOTOR_STATUS]			= CRect(rc.left, rc.top, rc.right - 30, rc.bottom);

	// Motor Status Area
	rc = m_rectS[eS_AREA_MOTOR_STATUS];
	double dW = rc.Width() / 2.;
	int nCurHeight = 0;
	bool bLeft = true;
	for (size_t iMotor = 0; iMotor < m_tStatus.size(); ++iMotor)
	{
		//m_tStatus[iMotor].rect = CRect(rc.left, rc.top + (m_iHeight * iMotor), rc.right, rc.top + (m_iHeight * (iMotor + 1)));
		int iStatusHeight = m_tStatus[iMotor].bSetSub ? 2 : 1;
		if (bLeft)
		{
			m_tStatus[iMotor].rect = CRect(rc.left, rc.top + (m_iHeight * nCurHeight), rc.left + dW, rc.top + (m_iHeight * (nCurHeight + iStatusHeight)));
		}
		else
		{
			m_tStatus[iMotor].rect = CRect(rc.left + dW, rc.top + (m_iHeight * nCurHeight), rc.right, rc.top + (m_iHeight * (nCurHeight + iStatusHeight)));
		}
		m_tStatus[iMotor].rect.DeflateRect(3, 3);
		nCurHeight += iStatusHeight;

		if (bLeft && iMotor + 1 >= m_tStatus.size() / 2 + m_tStatus.size() % 2)
		{
			bLeft = false;
			nCurHeight = 0;
		}
	}
	
	if (m_vScrollbar.m_hWnd) 
	{
		m_vScrollbar.MoveWindow(CRect(rc.right + 10, rc.top, m_rectClient.right, rc.bottom));

		int iRange = (m_iHeight * m_tStatus.size()) - m_rectClient.Height();
		if (iRange < 0) 
		{
			iRange = 0;
		}

		m_vScrollbar.SetScrollRange(0, iRange);
		m_vScrollbar.SetScrollPos(0);
	}
}

void CAxisStatusView::DrawAxisStatus(CDC& pDC, T_AXIS_STATUS_VIEW& tStatus) 
{
enum 
{
	eVIEW_TEXT,				// 4
	eVIEW_POS,				// 2
	eVIEW_TORQUE,			// 2

	eVIEW_SUB_POS,
	eVIEW_SUB_TORQUE,
	eVIEW_END,
};

	CRect rect = tStatus.rect;
	rect.OffsetRect(0, -m_iScrollPos);

	CRect rc[eVIEW_END];
	double dW = rect.Width() / 6.;
	double dH = rect.Height() / 2.;

	if (!tStatus.bSetSub)
	{
		rc[eVIEW_TEXT]					= CRect(rect.left + dW * 0		, rect.top, rect.left + dW * 3.5	, rect.bottom);
		rc[eVIEW_POS]					= CRect(rect.left + dW * 3.5 + 3, rect.top + 3, rect.left + dW * 5 - 3, rect.bottom - 3);
		rc[eVIEW_TORQUE]				= CRect(rect.left + dW * 5	 + 3, rect.top + 3, rect.left + dW * 6 - 3, rect.bottom - 3);
	}
	else
	{
		rc[eVIEW_TEXT]					= CRect(rect.left + dW * 0	, rect.top, rect.left + dW * 3.5	, rect.bottom);
		rc[eVIEW_POS]					= CRect(rect.left + dW * 3.5 + 3, rect.top + 3, rect.left + dW * 5 - 3, rect.top + dH * 1 - 3);
		rc[eVIEW_TORQUE]				= CRect(rect.left + dW * 5	 + 3, rect.top + 3, rect.left + dW * 6 - 3, rect.top + dH * 1 - 3);
		rc[eVIEW_SUB_POS]				= CRect(rect.left + dW * 3.5 + 3, rect.top + 3 + dH * 1, rect.left + dW * 5 - 3, rect.bottom - 3);
		rc[eVIEW_SUB_TORQUE]			= CRect(rect.left + dW * 5	 + 3, rect.top + 3 + dH * 1, rect.left + dW * 6 - 3, rect.bottom - 3);
	}
	
	pDC.FillSolidRect(rect, tStatus.bSelect ? RGB_GREEN_LIGHT : RGB_GRAY_LIGHT);
	//DrawLineRect(&pDC, rc[eVIEW_TEXT], RGB_BLUE);
	SelectedDrawText(&pDC, rc[eVIEW_POS], L"", DT_FONTSIZE, RGB_BLUE, RGB_WHITE);
	SelectedDrawText(&pDC, rc[eVIEW_SUB_POS], L"", DT_FONTSIZE, RGB_BLUE, RGB_WHITE);
	SelectedDrawText(&pDC, rc[eVIEW_TORQUE], L"", DT_FONTSIZE, RGB_BLUE, RGB_WHITE);
	SelectedDrawText(&pDC, rc[eVIEW_SUB_TORQUE], L"", DT_FONTSIZE, RGB_BLUE, RGB_WHITE);

	DrawTextB(&pDC, rc[eVIEW_TEXT], tStatus.strText, 12, FW_MEDIUM, RGB_BLACK, DT_CENTER | DT_VCENTER);
	
	auto DrawTextData = [&](int _enum, const CString& _text)
	{
		DrawTextB(&pDC, rc[_enum], _text, 11, FW_MEDIUM, RGB_BLACK, DT_RIGHT | DT_VCENTER);
	};

	auto DrawPosition = [&](BOOL _onOff, int _enum, const CString& _text)
	{
		pDC.FillSolidRect(rc[_enum], tStatus.bHome ? RGB_GREEN_LIGHT : RGB_WHITE);
		DrawLineRect(&pDC, rc[_enum], RGB_BLUE);
		DrawTextB(&pDC, rc[_enum], _text, 11, FW_MEDIUM, _onOff ? RGB_BLACK : RGB_RED, DT_RIGHT | DT_VCENTER);
	};

	DrawPosition(tStatus.bInpos, eVIEW_POS, Format(_T("%.3f %s"), tStatus.dPos, tStatus.bmm ? _T("mm ") : _T("deg ")));
	DrawTextData(eVIEW_TORQUE, Format(_T("%.1f %% "), tStatus.dTorque));
	
	if (tStatus.bSetSub) 
	{
		DrawPosition(tStatus.bInpos_Sub, eVIEW_SUB_POS, Format(_T("%.3f %s"), tStatus.dPos_Sub, tStatus.bmm ? _T("mm ") : _T("deg ")));
		DrawTextData(eVIEW_SUB_TORQUE, Format(_T("%.1f %% "), tStatus.dTorque_Sub));
	}

	COLORREF rgvSelect = RGB_GRAY_LIGHT;
	int iLine = 1;
	
	//if (tStatus.bSelect) 
	//{
	//	rgvSelect = RGB_YELLOW;
	//	iLine = 5;
	//	rect.DeflateRect(2, 2, 2, 2);
	//}

	DrawLineRect(&pDC, rect, rgvSelect, iLine);
	DrawLineRect(&pDC, rect, RGB_BLUE);
}

void CAxisStatusView::SetSelectIndex(int iIndex) 
{
	for (auto& data : m_tStatus) 
	{
		data.bSelect = FALSE;
	}

	// 마지막에서 더 내려가면 처음으로 돌아감
	if ((int)m_tStatus.size() <= iIndex)
	{
		iIndex = 0;
	}
	// 처음에서 더 올라가면 마지막으로 감
	else if (iIndex < 0)
	{
		iIndex = (int)(m_tStatus.size() - 1);
	}

	if (iIndex >= 0)
	{
		m_tStatus[iIndex].bSelect = TRUE;
	}
}

int CAxisStatusView::GetSelectIndex()
{
	for (size_t idx = 0; idx < m_tStatus.size(); ++idx)
	{
		if (m_tStatus[idx].bSelect == TRUE)
		{
			return (int)idx;
		}
	}
	return -1;
}

void CAxisStatusView::ClearData()
{
	m_tStatus.clear();
}

void CAxisStatusView::InsertAxisItem(T_AXIS_STATUS_VIEW tData) 
{
	m_tStatus.push_back(tData);
	int iRange = (m_iHeight * m_tStatus.size()) - m_rectS[eS_AREA_MOTOR_STATUS].Height();
	if (iRange < 0) 
	{
		iRange = 0;
	}
	m_vScrollbar.SetScrollRange(0, iRange);
}

void CAxisStatusView::SetAxisData(int iItem, double dSpeed, double dPos, double dTorque) 
{
	for (auto& data : m_tStatus)
	{
		if (data.iAxis == iItem)
		{
			data.dSpeed = dSpeed;
			data.dPos = dPos;
			data.dTorque = dTorque;
			break;
		}
	}
}

void CAxisStatusView::SetAxisSensor(int iItem, BOOL bOn, BOOL bOrg, BOOL bHomming, BOOL bAlarm, BOOL bPLimit, BOOL bNLimit, BOOL bInPos, BOOL bHome) 
{
	for (auto& data : m_tStatus)
	{
		if (data.iAxis == iItem)
		{
			data.bPLimit = bPLimit;
			data.bNLimit = bNLimit;
			data.bHome = bHome;
			data.bOrg = bOrg;
			data.bServoOn = bOn;
			data.bHomming = bHomming;
			data.bAlarm = bAlarm;
			data.bInpos = bInPos;
			break;
		}
	}
}

void CAxisStatusView::SetSubAxisData(int iItem, double dSpeed, double dPos, double dTorque) 
{
	for (auto& data : m_tStatus)
	{
		if (data.iAxis == iItem)
		{
			data.dSpeed_Sub = dSpeed;
			data.dPos_Sub = dPos;
			data.dTorque_Sub = dTorque;
			break;
		}
	}
}

void CAxisStatusView::SetSubAxisSensor(int iItem, BOOL bOn, BOOL bAlarm, BOOL bPLimit, BOOL bNLimit, BOOL bInPos, BOOL bHome) 
{
	for (auto& data : m_tStatus)
	{
		if (data.iAxis == iItem)
		{
			data.bPLimit_Sub = bPLimit;
			data.bNLimit_Sub = bNLimit;
			data.bHome_Sub = bHome;
			data.bServoOn_Sub = bOn;
			data.bAlarm_Sub = bAlarm;
			data.bInpos_Sub = bInPos;
			break;
		}
	}
}


BEGIN_MESSAGE_MAP(CAxisStatusView, CWnd)
	ON_WM_CREATE ()
	ON_WM_SHOWWINDOW ()
	ON_WM_PAINT ()
	ON_WM_LBUTTONDOWN ()
	ON_WM_LBUTTONUP ()
	ON_WM_TIMER ()
	ON_WM_VSCROLL ()
	ON_WM_MOUSEWHEEL ()
	ON_WM_VSCROLLCLIPBOARD ()
	ON_WM_RBUTTONUP ()
	ON_WM_RBUTTONDOWN ()
END_MESSAGE_MAP()

int CAxisStatusView::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate (lpCreateStruct) == -1)
		return -1;

	m_vScrollbar.Create(SBS_VERT | SBS_LEFTALIGN | WS_CHILD | WS_VISIBLE, CRect(), this, IDC_SCROLLBAR_AXIS_STATUS_VIEW);
	m_vScrollbar.ShowScrollBar();
	m_vScrollbar.EnableScrollBar(ESB_ENABLE_BOTH);

	return 0;
}

void CAxisStatusView::OnShowWindow (BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow (bShow, nStatus);

	if (bShow)
	{		
		SetTimer(INVERVAL_AXIS_STATUS_VIEW, 100, nullptr);
		ComputArea();
	}
	else
	{
		KillTimer(INVERVAL_AXIS_STATUS_VIEW);
	}
}

void CAxisStatusView::OnPaint ()
{
	CPaintDC dc(this); 
	CMemDCEx::CMemDC		memDC(&dc);
	memDC.FillSolidRect(m_rectClient, RGB_GRAY_LIGHT);
	
	for(int iMotor = 0; iMotor < m_tStatus.size(); ++iMotor)
	{
		DrawAxisStatus(memDC, m_tStatus[iMotor]);
	}
}


void CAxisStatusView::OnLButtonDown (UINT nFlags, CPoint point)
{
	SetFocus();
	for(size_t iMotor = 0; iMotor < m_tStatus.size(); ++iMotor)
	{
		m_tStatus[iMotor].bSelect = FALSE;
	}
	CWnd::OnLButtonDown (nFlags, point);
}


void CAxisStatusView::OnLButtonUp (UINT nFlags, CPoint point)
{
	CRect rect;
	for(size_t iMotor = 0; iMotor < m_tStatus.size(); ++iMotor)
	{
		rect = m_tStatus[iMotor].rect;
		rect.OffsetRect(0, -m_iScrollPos);
		
		if (rect.PtInRect(point))
		{
			m_tStatus[iMotor].bSelect = TRUE;
			GetParent()->Invalidate(FALSE);
		}
	}
	CWnd::OnLButtonUp (nFlags, point);
}


void CAxisStatusView::OnTimer (UINT_PTR nIDEvent)
{
	if (nIDEvent == INVERVAL_AXIS_STATUS_VIEW)
	{
		InvalidateRect(m_rectS[eS_AREA_MOTOR_STATUS], FALSE);
	}

	CWnd::OnTimer (nIDEvent);
}

void CAxisStatusView::OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int iMin, iMax;
	m_vScrollbar.GetScrollRange(&iMin, &iMax);
	switch(nSBCode){
		case SB_THUMBTRACK:{
			m_iScrollPos = nPos;
			m_vScrollbar.SetScrollPos(m_iScrollPos);

			break;
		}
		case SB_PAGEDOWN:{
			m_iScrollPos += (m_iHeight*3);
			if( m_iScrollPos > iMax){
				m_iScrollPos = iMax;
			}
			m_vScrollbar.SetScrollPos(m_iScrollPos);
			break;
		}
		case SB_PAGEUP:{
			m_iScrollPos -= (m_iHeight*3);
			if( m_iScrollPos < iMin){
				m_iScrollPos = iMin;
			}
			
			m_vScrollbar.SetScrollPos(m_iScrollPos);
			break;
		}
	}
	CWnd::OnVScroll (nSBCode, nPos, pScrollBar);
}

BOOL CAxisStatusView::OnMouseWheel (UINT nFlags, short zDelta, CPoint pt)
{
	int iPL = 0, iNL = 0;
	m_vScrollbar.GetScrollRange(&iNL, &iPL);
	m_iScrollPos -= zDelta / 10;
	
	if (m_iScrollPos < iNL) 
	{
		m_iScrollPos = iNL;
	}
	
	if (m_iScrollPos > iPL) 
	{
		m_iScrollPos = iPL;
	}

	m_vScrollbar.SetScrollPos(m_iScrollPos);
	return CWnd::OnMouseWheel (nFlags, zDelta, pt);
}


void CAxisStatusView::OnVScrollClipboard (CWnd* pClipAppWnd, UINT nSBCode, UINT nPos)
{
	m_iScrollPos = nPos;
	m_vScrollbar.SetScrollPos(m_iScrollPos);
	
	CWnd::OnVScrollClipboard (pClipAppWnd, nSBCode, nPos);
}

void CAxisStatusView::OnRButtonDown (UINT nFlags, CPoint point)
{
	for (auto& data : m_tStatus)
	{
		data.bSelect = FALSE;
	}

	CWnd::OnRButtonDown (nFlags, point);
}

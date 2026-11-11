#include "pch.h"
#include "UI.h"
#include "./TransferThetaMoveWnd.h"


#define ID_INTERVAL_UI 40001
IMPLEMENT_DYNCREATE		(CTransferThetaMoveWnd, CFrameWndEx)
BEGIN_MESSAGE_MAP		(CTransferThetaMoveWnd, CFrameWndEx)
	ON_WM_CREATE		()
	ON_WM_SHOWWINDOW	()
	ON_WM_TIMER			()
	ON_WM_ERASEBKGND	()
	ON_WM_PAINT			()
	ON_WM_MOUSEMOVE		()
	ON_WM_LBUTTONDOWN	()
	ON_WM_LBUTTONUP		()
	ON_WM_CLOSE()
END_MESSAGE_MAP			()



CTransferThetaMoveWnd::CTransferThetaMoveWnd(CWnd* pwndParent) {
	m_pwndParent		= pwndParent;
	m_eSelectControl	= eC_NONE;
	m_eMouseOver		= eC_NONE;
	m_dPitch			= 0.;
	m_dTheta			= 0.;
	m_dSpeed			= 0.;
	m_dPitchSpeed		= 0.;
	m_dCurrentX			= 0.;
	m_dCurrentY			= 0.;
	m_dCurrentZ			= 0.;
	m_dCurrentT			= 0.;
}

CTransferThetaMoveWnd::~CTransferThetaMoveWnd() {
}



BOOL CTransferThetaMoveWnd::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}






int CTransferThetaMoveWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	try {
		
	}
	catch (...) {
		return -1;
	}

	return 0;
}


void CTransferThetaMoveWnd::OnKillFocus(CWnd* pNewWnd) {
	CFrameWndEx::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
	Invalidate();
}



void CTransferThetaMoveWnd::OnTimer(UINT_PTR nIDEvent) {
	switch (nIDEvent) {
		case ID_INTERVAL_UI : {
			InvalidateRect(m_rectC[eC_POSITION_X]);
			InvalidateRect(m_rectC[eC_POSITION_Y]);
			InvalidateRect(m_rectC[eC_POSITION_Z]);
			InvalidateRect(m_rectC[eC_POSITION_T]);
			break;
		}
		default:
			break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}



void CTransferThetaMoveWnd::OnShowWindow(BOOL bShow, UINT nStatus) {
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	if (bShow) {
		SetWindowText(_T("Transfer Theta Move"));
		SetWindowPos(&wndTop, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, g_rectArea[eAREA_WORK].Width()/3., g_rectArea[eAREA_WORK].Height()/2., SWP_SHOWWINDOW);
		fn_ComputeArea();
		m_eSelectControl	= eC_NONE;
		Invalidate();
		SetTimer(ID_INTERVAL_UI, 100, nullptr);
	}
	else {
		KillTimer(ID_INTERVAL_UI);
	}
}


BOOL CTransferThetaMoveWnd::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}



void CTransferThetaMoveWnd::OnPaint() {
	CPaintDC			dc(this);
try {
	CMemDCEx::CMemDC		memDC(&dc);
	CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_NORMAL]);
	CBrush* pOldBrush = (CBrush*)memDC.SelectObject(&g_brushBackground[eEC_NORMAL]);
	CPen* pOldPen = (CPen*)memDC.SelectObject(&pen);
	CRect					rect = m_rectS[eS_FULL_AREA];
	memDC.SelectObject(&g_brushForeground[eEC_DEFAULT]);
	memDC.FillSolidRect(rect, g_colorBackground[eEC_DEFAULT]);
	rect.DeflateRect(3, 3, 3, 3);
	memDC.SelectObject(&g_brushForeground[eEC_DEFAULT]);
	CDrawSymbol::DrawLineRect(&memDC, rect, g_colorForeground[eEC_DEFAULT]);

	
	DrawButtonUpDown(&memDC, m_rectC[eC_CLOSE], g_st[IDS_CLOSE], m_eSelectControl == eC_CLOSE);
	DrawText(memDC, m_rectS[eS_POSITION_X], g_st[IDS_MANUAL_TRANSFER_THETA_MOVE_CURRENT_X_POS], 9, RGB_BLACK, DT_RIGHT|DT_VCENTER);
	DrawText(memDC, m_rectS[eS_POSITION_Y], g_st[IDS_MANUAL_TRANSFER_THETA_MOVE_CURRENT_Y_POS], 9, RGB_BLACK, DT_RIGHT|DT_VCENTER);
	DrawText(memDC, m_rectS[eS_POSITION_T], g_st[IDS_MANUAL_TRANSFER_THETA_MOVE_CURRENT_Z_POS], 9, RGB_BLACK, DT_RIGHT|DT_VCENTER);
	DrawText(memDC, m_rectS[eS_POSITION_Z], g_st[IDS_MANUAL_TRANSFER_THETA_MOVE_CURRENT_T_POS], 9, RGB_BLACK, DT_RIGHT|DT_VCENTER);

	DrawText(memDC, m_rectS[eS_SPEED		], g_st[IDS_MANUAL_TRANSFER_THETA_MOVE_THETA], 9, RGB_BLACK, DT_RIGHT|DT_VCENTER);
	DrawText(memDC, m_rectS[eS_THETA		], g_st[IDS_MANUAL_TRANSFER_THETA_MOVE_SPEED], 9, RGB_BLACK, DT_RIGHT|DT_VCENTER);
	DrawText(memDC, m_rectS[eS_PITCH		], g_st[IDS_MANUAL_TRANSFER_THETA_MOVE_PITCH], 9, RGB_BLACK, DT_RIGHT|DT_VCENTER);
	DrawText(memDC, m_rectS[eS_PITCH_SPEED	], g_st[IDS_MANUAL_TRANSFER_THETA_MOVE_PITCH_SPEED], 9, RGB_BLACK, DT_RIGHT|DT_VCENTER);
	
	SelectedDrawDouble(memDC, m_rectC[eC_POSITION_Y], sm.MotorStatus[eMOTOR_GLASS_TRANSFER_Y].tCrt.dPos, 3, 9, RGB_WHITE, RGB_WHITE, RGB_BLACK);
	SelectedDrawDouble(memDC, m_rectC[eC_POSITION_Z], sm.MotorStatus[eMOTOR_GLASS_TRANSFER_Z].tCrt.dPos, 3, 9, RGB_WHITE, RGB_WHITE, RGB_BLACK);

	SelectedDrawEditDouble( memDC, m_rectC[eC_SPEED			], m_dSpeed , 3, TRUE, 9, DT_CENTER|DT_VCENTER);
	SelectedDrawEditDouble( memDC, m_rectC[eC_THETA			], m_dTheta , 3, TRUE, 9, DT_CENTER|DT_VCENTER);
	SelectedDrawEditDouble( memDC, m_rectC[eC_PITCH			], m_dPitch , 3, TRUE, 9, DT_CENTER|DT_VCENTER);	
	SelectedDrawEditDouble( memDC, m_rectC[eC_PITCH_SPEED	], m_dPitchSpeed , 3, TRUE, 9, DT_CENTER|DT_VCENTER);

	DrawButtonUpDown(memDC, m_rectC[eC_MOVE], g_st[IDS_MOVE], m_eSelectControl == eC_MOVE);
	DrawMoveDirect(memDC, m_rectC[eC_P_MOVE], m_eSelectControl == eC_P_MOVE, eMD_RIGHT, _T("+"));
	DrawMoveDirect(memDC, m_rectC[eC_N_MOVE], m_eSelectControl == eC_N_MOVE, eMD_LEFT, _T("-"));


	memDC.SelectObject(pOldBrush);
	memDC.SelectObject(pOldPen);
}
catch (...) {
}
}



void CTransferThetaMoveWnd::OnMouseMove(UINT nFlags, CPoint point) {
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



void CTransferThetaMoveWnd::OnLButtonDown(UINT nFlags, CPoint point) {
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



void CTransferThetaMoveWnd::OnLButtonUp(UINT nFlags, CPoint point)  {	
	CString strLog;
	if (m_rectC[m_eSelectControl].PtInRect(point)) {
		switch (m_eSelectControl) {
			case eC_CLOSE: {
				ShowWindow(SW_HIDE);
				strLog = _T("Close");
				break;
			}
			case eC_SPEED: {
				EditStart(this, m_rectC[m_eSelectControl], m_dSpeed);
				break;
			}
			case eC_THETA: {
				EditStart(this, m_rectC[m_eSelectControl], m_dTheta);
				break;
			}
			case eC_PITCH: {
				EditStart(this, m_rectC[m_eSelectControl], m_dPitch);
				break;
			}
			case eC_PITCH_SPEED: {
				EditStart(this, m_rectC[m_eSelectControl], m_dPitchSpeed);
				break;
			}
			//case eC_P_MOVE: 
			//case eC_N_MOVE: {
			//	double dPitch = m_dPitch;
			//	if (m_eSelectControl == eC_N_MOVE) dPitch *= (-1);
			//	MGTransferThetaMove(dPitch, m_dPitchSpeed);
			//	strLog = m_eSelectControl == eC_N_MOVE ? _T("- 이동") : _T("+ 이동");
			//	break;
			//}
			//case eC_MOVE: {
			//	// 입력된 Theta 로 이동
			//	PTPMove(eMOTOR_MG_TRANSFER_T, m_dTheta, m_dSpeed);
			//	strLog = _T("Move");
			//	break;
			//}

			default: break;
		}
	}
	if (!strLog.IsEmpty()) {
		WriteButtonLog(_T("TransferThetaMove"), _T("TransferThetaMove"), _T(""), strLog, theApp.m_UserManager.GetCurrentUserData().szID);
	}
	m_eSelectControl	=	eC_NONE;
	Invalidate(FALSE);
	
	CFrameWndEx::OnLButtonUp(nFlags, point);
}



void CTransferThetaMoveWnd::fn_ComputeArea() {
	GetClientRect(m_rectS[eS_FULL_AREA]);
		
	CRect	rc = m_rectS[eS_FULL_AREA];
	double dH = (rc.Height() - 5.) / 6.;
	double dW = (rc.Width() - 5.) / 6.;
	m_rectS[eS_POSITION_X] = CRect(rc.left+dW*0+5, rc.top+dH*0+5, rc.left+dW*1, rc.top+dH*1);
	m_rectS[eS_POSITION_Y] = CRect(rc.left+dW*2+5, rc.top+dH*0+5, rc.left+dW*3, rc.top+dH*1);
	m_rectS[eS_POSITION_Z] = CRect(rc.left+dW*0+5, rc.top+dH*1+5, rc.left+dW*1, rc.top+dH*2);
	m_rectS[eS_POSITION_T] = CRect(rc.left+dW*2+5, rc.top+dH*1+5, rc.left+dW*3, rc.top+dH*2);
	
	m_rectC[eC_POSITION_X] = CRect(rc.left+dW*1+5, rc.top+dH*0+5, rc.left+dW*2, rc.top+dH*1);				
	m_rectC[eC_POSITION_Y] = CRect(rc.left+dW*3+5, rc.top+dH*0+5, rc.left+dW*4, rc.top+dH*1);				
	m_rectC[eC_POSITION_Z] = CRect(rc.left+dW*1+5, rc.top+dH*1+5, rc.left+dW*2, rc.top+dH*2);				
	m_rectC[eC_POSITION_T] = CRect(rc.left+dW*3+5, rc.top+dH*1+5, rc.left+dW*4, rc.top+dH*2);				

	m_rectS[eS_THETA	] = CRect(rc.left+dW*0+5, rc.top+dH*2+5, rc.left+dW*1, rc.top+dH*3);				
	m_rectC[eC_THETA	] = CRect(rc.left+dW*1+5, rc.top+dH*2+5, rc.left+dW*2, rc.top+dH*3);									
	
	m_rectS[eS_SPEED	] = CRect(rc.left+dW*2+5, rc.top+dH*2+5, rc.left+dW*3, rc.top+dH*3);				
	m_rectC[eC_SPEED	] = CRect(rc.left+dW*3+5, rc.top+dH*2+5, rc.left+dW*4, rc.top+dH*3);									
	m_rectC[eC_MOVE		] = CRect(rc.left+dW*4+5, rc.top+dH*2+5, rc.left+dW*5, rc.top+dH*3);									

	m_rectS[eS_PITCH		] = CRect(rc.left+dW*0+5, rc.top+dH*3+5, rc.left+dW*1, rc.top+dH*4);				
	m_rectC[eC_PITCH		] = CRect(rc.left+dW*1+5, rc.top+dH*3+5, rc.left+dW*2, rc.top+dH*4);									
	m_rectS[eS_PITCH_SPEED	] = CRect(rc.left+dW*2+5, rc.top+dH*3+5, rc.left+dW*3, rc.top+dH*4);				
	m_rectC[eC_PITCH_SPEED	] = CRect(rc.left+dW*3+5, rc.top+dH*3+5, rc.left+dW*4, rc.top+dH*4);									

	m_rectC[eC_N_MOVE	] = CRect(rc.left+dW*0+5, rc.top+dH*4+5, rc.left+dW*1, rc.top+dH*5);								
	m_rectC[eC_P_MOVE	] = CRect(rc.left+dW*1+5, rc.top+dH*4+5, rc.left+dW*2, rc.top+dH*5);								

	m_rectC[eC_CLOSE	] = CRect(rc.left+dW*5+5, rc.top+dH*5+5, rc.left+dW*6, rc.top+dH*6);									
}



void CTransferThetaMoveWnd::ThetaMove(double dTheta, double dPich, double dPitchSpeed) {
	// Pitch 로 계산값을 구한 후 
	// Pitch Speed 로 해당 위치로 이동 
	//double dRTheta = deg2rad(90 - dTheta);	// Radian 값을 변환 
	//double dY = dPich * cos(dRTheta);	// Y 이동 거리 계산 
	//double dZ = dPich * sin(dRTheta);	// Z 이동 거리 계산 
	//if (fabs(dY) < 0.0001)				
	//	dY = 0.;
	//if (fabs(dZ) < 0.0001)
	//	dZ = 0.;
	//dY *= (-1);			
	//
	//std::vector<T_POS> vec;
	//T_POS posY;
	//posY.iAxis = eMOTOR_MG_TRANSFER_Y;
	//posY.dTarget = GetAxisPos(eMOTOR_MG_TRANSFER_Y) + dY;
	//posY.dSpeed = dPitchSpeed;
	//
	//T_POS posZ;
	//posZ.iAxis = eMOTOR_MG_TRANSFER_Z;
	//posZ.dTarget = GetAxisPos(eMOTOR_MG_TRANSFER_Z) + dZ;
	//posZ.dSpeed = dPitchSpeed;
	//
	//vec.push_back(posY);
	//vec.push_back(posZ);
	//LinerMove(vec, dPitchSpeed);
	//
	////StepMove(eMOTOR_MG_TRANSFER_Y, dY, dPitchSpeed);
	////StepMove(eMOTOR_MG_TRANSFER_Z, dZ, dPitchSpeed);
	////PTPMove(eMOTOR_MG_TRANSFER_Y, dY, m_dPitchSpeed, nullptr);
	////PTPMove(eMOTOR_MG_TRANSFER_Z, dZ, m_dPitchSpeed, nullptr);
	//double dTemp = dY;
}

void CTransferThetaMoveWnd::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ShowWindow(SW_HIDE);
}

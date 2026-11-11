// NumericKeypadWnd.cpp : implementation file
//

#include "pch.h"
#include "UI.h"
#include "NumericKeypadWnd.h"

// CNumericKeypadWnd
IMPLEMENT_DYNCREATE(CNumericKeypadWnd, CFrameWndEx)

CNumericKeypadWnd::CNumericKeypadWnd(CWnd* pwndParent) : TIPCT<CFrameWndEx>(&g_ipClient)
{
	m_eSelectControl	= eC_NONE;
	m_strBefore			= _T("0.000");
	m_strAfter			= _T("0.000");
	m_strDefault		= _T("0.000");
}

CNumericKeypadWnd::~CNumericKeypadWnd()
{
}


BEGIN_MESSAGE_MAP(CNumericKeypadWnd, CFrameWndEx)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


BEGIN_IP_COMMAND_TABLE(CNumericKeypadWnd, IIPCommandTarget)
//ON_IP_EXCL("Keypad", "ShowKeypad", OnIPShowKeypad)
END_IP_COMMAND_TABLE()
// CNumericKeypadWnd message handlers


void CNumericKeypadWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	if( bShow){
		SetWindowText(g_st[IDS_NUM_KETPAD_WND_TILE]);
		int iW = (g_rectArea[eAREA_WORK].Width() - 5) / 4;
		int iH = (g_rectArea[eAREA_WORK].Height() - 5) / 1.5 ;
		SetWindowPos(&wndTopMost, g_rectArea[eAREA_WORK].left, g_rectArea[eAREA_WORK].top, iW, iH, SWP_SHOWWINDOW);
		//m_bApply = FALSE;
		//m_bCancel = FALSE;
		sm.Option->tKeyPad.fShowKeyPad = TRUE;
		if (m_bDouble) {
			m_strBefore = Format(_T("%.3f"), *m_dValue);
			m_strAfter = Format(_T("%.3f"), *m_dValue);
			m_strDefault = _T("0.000");
		}
		else {
			m_strBefore = Format(_T("%d"), *m_iValue);
			m_strAfter = Format(_T("%d"), *m_iValue);
			m_strDefault = _T("0");
		}

		//m_strBefore = Format(_T("%d"), *m_iValue);
		//m_strAfter = Format(_T("%d"), *m_iValue);
		//m_strDefault = _T("0");
		//m_bDouble = FALSE;
		
		ComputArea();
	}else{
		sm.Option->tKeyPad.fShowKeyPad = FALSE;
	}
	// TODO: Add your message handler code here
}

void CNumericKeypadWnd::ComputArea(){
	GetClientRect(m_rectClient);
	
	m_rectS[eS_FULL_AREA] = m_rectClient;
	CRect rc = m_rectS[eS_FULL_AREA];
	//rc.InflateRect(0,0,0,0);
	
	double dW = (rc.Width()-5.)/4.;
	double dH = (rc.Height()-5.)/6.;

	m_rectS[eS_BEFORE		] = CRect(rc.left+5+dW*0, rc.top+5+dH*0, rc.left+dW*1, rc.top+dH*1);
	m_rectS[eS_AFTER		] = CRect(rc.left+5+dW*0, rc.top+5+dH*1, rc.left+dW*1, rc.top+dH*2);

	m_rectC[eC_EDIT_BEFORE	] = CRect(rc.left+5+dW*1, rc.top+5+dH*0, rc.left+dW*3, rc.top+dH*1);
	m_rectC[eC_EDIT_AFTER	] = CRect(rc.left+5+dW*1, rc.top+5+dH*1, rc.left+dW*3, rc.top+dH*2);

	m_rectC[eC_CANCLE		] = CRect(rc.left+5+dW*3, rc.top+5+dH*0, rc.left+dW*4, rc.top+dH*1);
	m_rectC[eC_UNDO			] = CRect(rc.left+5+dW*3, rc.top+5+dH*1, rc.left+dW*4, rc.top+dH*2);

	m_rectC[eC_NUMBER_7		] = CRect(rc.left+5+dW*0, rc.top+5+dH*2, rc.left+dW*1, rc.top+dH*3);
	m_rectC[eC_NUMBER_8		] = CRect(rc.left+5+dW*1, rc.top+5+dH*2, rc.left+dW*2, rc.top+dH*3);
	m_rectC[eC_NUMBER_9		] = CRect(rc.left+5+dW*2, rc.top+5+dH*2, rc.left+dW*3, rc.top+dH*3);
	m_rectC[eC_DELETE		] = CRect(rc.left+5+dW*3, rc.top+5+dH*2, rc.left+dW*4, rc.top+dH*3);	

	m_rectC[eC_NUMBER_4		] = CRect(rc.left+5+dW*0, rc.top+5+dH*3, rc.left+dW*1, rc.top+dH*4);
	m_rectC[eC_NUMBER_5		] = CRect(rc.left+5+dW*1, rc.top+5+dH*3, rc.left+dW*2, rc.top+dH*4);
	m_rectC[eC_NUMBER_6		] = CRect(rc.left+5+dW*2, rc.top+5+dH*3, rc.left+dW*3, rc.top+dH*4);
	m_rectC[eC_BACKSPACE	] = CRect(rc.left+5+dW*3, rc.top+5+dH*3, rc.left+dW*4, rc.top+dH*4);

	m_rectC[eC_NUMBER_1		] = CRect(rc.left+5+dW*0, rc.top+5+dH*4, rc.left+dW*1, rc.top+dH*5);
	m_rectC[eC_NUMBER_2		] = CRect(rc.left+5+dW*1, rc.top+5+dH*4, rc.left+dW*2, rc.top+dH*5);
	m_rectC[eC_NUMBER_3		] = CRect(rc.left+5+dW*2, rc.top+5+dH*4, rc.left+dW*3, rc.top+dH*5);
	
	m_rectC[eC_NUMBER_0		] = CRect(rc.left+5+dW*0, rc.top+5+dH*5, rc.left+dW*1, rc.top+dH*6);
	m_rectC[eC_COMMA		] = CRect(rc.left+5+dW*1, rc.top+5+dH*5, rc.left+dW*2, rc.top+dH*6);
	m_rectC[eC_MINUS		] = CRect(rc.left+5+dW*2, rc.top+5+dH*5, rc.left+dW*3, rc.top+dH*6);

	m_rectC[eC_APPLY		] = CRect(rc.left+5+dW*3, rc.top+5+dH*4, rc.left+dW*4, rc.top+dH*6);
	//eC_CLOSE		

	//m_rcC[eC_CLOSE		]		= CRect(rc.left+5+dW*4,	rc.top+5+dH*3, rc.left+dW*5, rc.top+dH*4);
}

void CNumericKeypadWnd::OnPaint()
{
	CPaintDC			dc(this);
	try {
		CMemDCEx::CMemDC		memDC(&dc);
		CPen					pen(PS_SOLID, 1, g_colorForeground[eEC_NORMAL]);
		CBrush* pOldBrush = (CBrush*)memDC.SelectObject(&g_brushBackground[eEC_NORMAL]);
		CPen* pOldPen = (CPen*)memDC.SelectObject(&pen);
		CRect					rect = m_rectS[eS_FULL_AREA];
		memDC.SelectObject(&g_brushBackground[eEC_NORMAL]);
		memDC.FillSolidRect(rect, g_colorBackground[eEC_DISABLE]);
		rect.DeflateRect(3, 3, 3, 3);
		memDC.SelectObject(&g_brushForeground[eEC_DEFAULT]);
		CDrawSymbol::DrawLineRect(&memDC, rect, g_colorForeground[eEC_DEFAULT]);
		
		DrawLineRect(&memDC, m_rectS[eS_FULL_AREA]);

		DrawText(&memDC, m_rectS[eS_BEFORE		], g_st[IDS_NUM_KETPAD_WND_BEFORE], 15, RGB_BLACK);
		DrawText(&memDC, m_rectS[eS_AFTER		], g_st[IDS_NUM_KETPAD_WND_AFTER], 15, RGB_BLACK);
		
		SelectedDrawEditText(&memDC, m_rectC[eC_EDIT_BEFORE	], m_strBefore,	FALSE,	15, DT_VCENTER|DT_CENTER);
		SelectedDrawEditText(&memDC, m_rectC[eC_EDIT_AFTER	], m_strAfter,	TRUE,	15, DT_VCENTER|DT_CENTER); 
		
		DrawButtonUpDown(&memDC, m_rectC[eC_CANCLE		], g_st[IDS_CANCEL],	            m_eSelectControl == eC_CANCLE,		15);
		DrawButtonUpDown(&memDC, m_rectC[eC_UNDO		], g_st[IDS_NUM_KETPAD_WND_UNDO],	m_eSelectControl == eC_UNDO,		15);
		DrawButtonUpDown(&memDC, m_rectC[eC_NUMBER_7	], _T("7"),		                    m_eSelectControl == eC_NUMBER_7,	15);
		DrawButtonUpDown(&memDC, m_rectC[eC_NUMBER_8	], _T("8"),		                    m_eSelectControl == eC_NUMBER_8,	15);
		DrawButtonUpDown(&memDC, m_rectC[eC_NUMBER_9	], _T("9"),		                    m_eSelectControl == eC_NUMBER_9,	15);
		DrawButtonUpDown(&memDC, m_rectC[eC_DELETE		], g_st[IDS_DELETE],	            m_eSelectControl == eC_DELETE,		15);
		DrawButtonUpDown(&memDC, m_rectC[eC_NUMBER_4	], _T("4"),		                    m_eSelectControl == eC_NUMBER_4,	15);
		DrawButtonUpDown(&memDC, m_rectC[eC_NUMBER_5	], _T("5"),		                    m_eSelectControl == eC_NUMBER_5,	15);
		DrawButtonUpDown(&memDC, m_rectC[eC_NUMBER_6	], _T("6"),		                    m_eSelectControl == eC_NUMBER_6,	15);
		DrawButtonUpDown(&memDC, m_rectC[eC_BACKSPACE	], _T("←"),	                    m_eSelectControl == eC_BACKSPACE,	15);
		DrawButtonUpDown(&memDC, m_rectC[eC_NUMBER_1	], _T("1"),		                    m_eSelectControl == eC_NUMBER_1,	15);
		DrawButtonUpDown(&memDC, m_rectC[eC_NUMBER_2	], _T("2"),		                    m_eSelectControl == eC_NUMBER_2,	15);
		DrawButtonUpDown(&memDC, m_rectC[eC_NUMBER_3	], _T("3"),		                    m_eSelectControl == eC_NUMBER_3,	15);
		DrawButtonUpDown(&memDC, m_rectC[eC_NUMBER_0	], _T("0"),		                    m_eSelectControl == eC_NUMBER_0,	15);
		DrawButtonUpDown(&memDC, m_rectC[eC_MINUS		], _T("-"),		                    m_eSelectControl == eC_MINUS,		15);
		if (m_bDouble)
			DrawButtonUpDown(&memDC, m_rectC[eC_COMMA		], _T("."),		m_eSelectControl == eC_COMMA,		15);
		else 
			DrawButtonUpDown(&memDC, m_rectC[eC_COMMA], _T("."), FALSE, 15);
		DrawButtonUpDown(&memDC, m_rectC[eC_APPLY		], g_st[IDS_NUM_KETPAD_WND_CHECK],	m_eSelectControl == eC_APPLY,		15);

		memDC.SelectObject(pOldBrush);
		memDC.SelectObject(pOldPen);
	}
	catch (...) {
	}
}

int CNumericKeypadWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	return 0;
}


void CNumericKeypadWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	for(int i =0 ; i < eC_END; i++){
		if( m_rectC[i].PtInRect(point)){
			m_eSelectControl = i;
			Invalidate(FALSE);
			break;
		}
	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}


void CNumericKeypadWnd::OnLButtonUp(UINT nFlags, CPoint point){
	
	if( m_rectC[m_eSelectControl].PtInRect(point)){
		switch (m_eSelectControl)
		{
			case eC_CANCLE:{
				//m_bCancel = TRUE;
				sm.Option->tKeyPad.fShowKeyPad = FALSE;
				ShowWindow(SW_HIDE);
				break;
			}
			case eC_EDIT_AFTER: {
				EditStart(this, m_rectC[m_eSelectControl], m_strAfter);
				break;
			}
			case eC_DELETE: {
				m_strAfter = m_strDefault;
				break;
			}
			case eC_UNDO: {
				m_strAfter = m_strBefore;
				break;
			}
			case eC_BACKSPACE: {
				m_strAfter.Delete(m_strAfter.GetLength() - 1);
				break;
			}
			case eC_APPLY: {
				//m_bApply = TRUE;
				if (m_bDouble)
					*m_dValue = _ttof(m_strAfter);
				else 
					*m_iValue = _ttoi(m_strAfter);

				sm.Option->tKeyPad.fShowKeyPad = FALSE;
				if (m_funcListUpdate != nullptr)
					m_funcListUpdate();
				ShowWindow(SW_HIDE);
				break;
			}
			case eC_NUMBER_0: 
			case eC_NUMBER_1: 
			case eC_NUMBER_2: 
			case eC_NUMBER_3: 
			case eC_NUMBER_4: 
			case eC_NUMBER_5: 
			case eC_NUMBER_6: 
			case eC_NUMBER_7: 
			case eC_NUMBER_8: 
			case eC_NUMBER_9: {
				if (m_strAfter.Compare(_T("0")) == 0 || m_strAfter.Compare(_T("0.000"))==0)
					m_strAfter = Format(_T("%d"), m_eSelectControl - eC_NUMBER_0);
				else 
					m_strAfter += Format(_T("%d"), m_eSelectControl - eC_NUMBER_0);
				break;
			}	
			case eC_MINUS: {
				if (!(m_strAfter.Find(_T("-"), 0) > -1))
					m_strAfter = _T("-") + m_strAfter;
				break;
			}		
			case eC_COMMA: {
				if (m_bDouble && !(m_strAfter.Find(_T("."), 0) > -1))
					m_strAfter += _T(".");
				break;
			}		
			default:
				break;
		}
		 
	}
	m_eSelectControl = eC_NONE;
		Invalidate(FALSE);
		
	CFrameWnd::OnLButtonUp(nFlags, point);
}

void CNumericKeypadWnd::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
	return;

	CFrameWnd::OnClose();
}


void CNumericKeypadWnd::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	ComputArea();
	// TODO: Add your message handler code here
}
void CNumericKeypadWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent){
	}
	CFrameWnd::OnTimer(nIDEvent);
}

//BOOL CNumericKeypadWnd::OnIPShowKeypad(CIPCommand& cmd) {
//	const CIPVar& var = cmd.m_var;
//	CIPVar& varCookie = cmd.m_varCookie;
//	CIPVar& varResult = cmd.m_varResult;
//
//	enum {
//		S_INIT,
//		S_PAUSE,
//		S_RETURN,
//		S_END,
//	};
//
//	if (!cmd.IsChildOK() || !cmd.IsOK()) {
//		return FALSE;
//	}
//
//	switch (cmd.GetCurrentStep()) {
//		case S_INIT: {
//			ShowWindow(SW_SHOW);
//			sm.Option->fKeyPad = TRUE;
//			m_bDouble = var.GetChildItem("Check", FALSE);
//			if (m_bDouble) {
//				double dData = var.GetChildItem("Value", 0.);
//				m_strBefore = Format(_T("%.3f"),	dData);
//				m_strAfter = Format(_T("%.3f"),		dData);
//				m_strDefault = _T("0.000");
//			}
//			else {
//				int iData = var.GetChildItem("Value", 0);
//				m_strBefore = Format(_T("%d"),	iData);
//				m_strAfter = Format(_T("%d"),	iData);
//				m_strDefault = _T("0");
//			}
//			cmd.EnqueueNextStep();
//			break;
//		}
//		case S_PAUSE: {
//			//if (m_bApply) 
//			//	cmd.EnqueueNextStep(S_RETURN, 100);
//			//else if (m_bCancel) 
//			//	cmd.EnqueueNextStep(S_END, 100);
//			//else
//			//	cmd.EnqueueNextStepCurrent();
//			cmd.EnqueueNextStep(S_END, 100);
//			break;
//		}
//		case S_RETURN: {
//			if (m_bDouble)
//				varResult.SetChildItem("Value", _ttof(m_strAfter));
//			else 
//				varResult.SetChildItem("Value", atoi((CStringA)m_strAfter));
//			cmd.EnqueueNextStep();
//			break;
//		}
//		case S_END: {
//			cmd.EnqueueNextStep();
//			break;
//		}
//	}
//	return TRUE;
//}

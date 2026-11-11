#include "pch.h"
#include "DrawControl.h"


void DrawLineRect(CDC* pDC, LPRECT rect, COLORREF crPen, int iTick) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	if (iTick > 5 ||  iTick < 1)
		iTick = 1;

	CPen	pen(PS_SOLID, iTick, crPen);
	CPen*	pOldPen	= pDC->SelectObject(&pen);

	if (CRect(rect).IsRectEmpty())
		return;
	CRect rectTemp(rect);
	rectTemp.InflateRect(0, 0, -1, -1);

	pDC->MoveTo(rectTemp.TopLeft());
	pDC->LineTo(rectTemp.right, rectTemp.top);
	pDC->LineTo(rectTemp.BottomRight());
	pDC->LineTo(rectTemp.left, rectTemp.bottom);
	pDC->LineTo(rectTemp.TopLeft());

	pDC->SelectObject( pOldPen );
	pen.DeleteObject();
}

void DrawFitRect(CDC* pDC, LPRECT rect, COLORREF crPen) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CPen	pen(PS_SOLID, 1, crPen);
	CPen*	pOldPen	= pDC->SelectObject(&pen);

	if (CRect(rect).IsRectEmpty())
		return;

	pDC->Rectangle(rect);
	pDC->SelectObject( pOldPen );
	pen.DeleteObject();
}

void DrawRoundRect(CDC* pDC, LPRECT rect, COLORREF crPen, COLORREF crBack, CPoint pt) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CPen	pen(PS_SOLID, 1, crPen);
	CPen*	pOldPen	= pDC->SelectObject(&pen);

	CBrush	brush(crBack);
	CBrush*	pOldBrush = pDC->SelectObject(&brush);

	if (CRect(rect).IsRectEmpty())
		return;

	pDC->RoundRect(rect, pt);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pen.DeleteObject();
	brush.DeleteObject();
}

void DrawLineRoundRect(CDC* pDC, LPRECT rect, COLORREF crPen, CPoint pt)
{
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CPen	pen(PS_SOLID, 1, crPen);
	CPen*	pOldPen = pDC->SelectObject(&pen);

	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	if (CRect(rect).IsRectEmpty())
		return;

	pDC->RoundRect(rect, pt);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	pen.DeleteObject();
}

void DrawLine(CDC* pDC, CPoint pt1, CPoint pt2, COLORREF crPen, int iWidth) {
	if (iWidth > 5)
		iWidth = 5;
	if (iWidth < 1)
		iWidth = 1;

	CPen	pen(PS_SOLID, iWidth, crPen);
	CPen*	pOldPen	= pDC->SelectObject(&pen);

	pDC->MoveTo(pt1);
	pDC->LineTo(pt2);
	pDC->SelectObject( pOldPen );

	pen.DeleteObject();
}

void DrawLine(CDC* pDC, CPoint pt, int iX, int iY, COLORREF crPen, int iWidth) {
	if (iWidth > 5)
		iWidth = 5;
	if (iWidth < 1)
		iWidth = 1;

	CPen	pen(PS_SOLID, iWidth, crPen);
	CPen*	pOldPen	= pDC->SelectObject(&pen);

	pDC->MoveTo(pt);
	pDC->LineTo(CPoint(pt.x+iX, pt.y+iY));
	pDC->SelectObject( pOldPen );
	
	pen.DeleteObject();
}
void DrawGroupBox(CDC* pDC, LPRECT rect, LPCTSTR szTitle , COLORREF crPen , COLORREF crText , CSize szText , CPoint ptS , int iFontSize , DWORD dwAlign , LPCTSTR szFont ) {
	DrawGroupBox(pDC, rect, szTitle, szText, ptS, iFontSize, crPen, crText, dwAlign, szFont);
}
void DrawGroupBox(CDC* pDC, LPRECT rect, LPCTSTR szTitle, CSize szText, CPoint ptS, int iFontSize, COLORREF crPen, COLORREF crText, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CPen	pen(PS_SOLID, 1, crPen);
	CPen*	pOldPen = pDC->SelectObject(&pen);

	if (CRect(rect).IsRectEmpty())
		return;

	if (szText == CSize(-1, -1)) {
		CString strText(szTitle);
		CFont	font;
		LOGFONT lf;
		TEXTMETRIC tm;
		memset(&lf, 0x00, sizeof(lf));

		if ((iFontSize <= 0) || (iFontSize >= 500))
			iFontSize = DT_FONTSIZE;
		//if ((iWeight < 400) || (iWeight > 2000))
		//	iWeight = DT_FONTWEIGHT;

		lf.lfWeight = 400;
		lf.lfHeight = iFontSize * 10;
		wcscpy_s(lf.lfFaceName, szFont);
		font.CreatePointFontIndirect(&lf, pDC);
		font.GetLogFont(&lf);

		CFont*		pOld = pDC->SelectObject(&font);
		int			iBkMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF	color = pDC->SetTextColor(crText);
		pDC->GetTextMetrics(&tm);
		int iText = 1;
		
		for (int i = 0; i < CString(szTitle).GetLength(); i++) {
			if ('A' < szTitle[i] && szTitle[i] < 'z')
				iText += 2;
			else
				iText += 3;
		}
		szText = CSize(iText * tm.tmAveCharWidth, tm.tmHeight + tm.tmInternalLeading + tm.tmDescent);

		pDC->SelectObject(pOld);
		font.DeleteObject();
	}

	CRect rectText(CRect(rect).left + ptS.x, CRect(rect).top + ptS.y, CRect(rect).left + ptS.x + szText.cx, CRect(rect).top + ptS.y + szText.cy);

	if (ptS == CPoint()) {
		CRect rectLT, rectRT, rectLB, rectRB;

		rectLT = CRect(rect->left, rect->top, rect->left + 20, rect->top + 20);
		rectRT = CRect(rect->right - 20, rect->top, rect->right, rect->top + 20);
		rectRB = CRect(rect->right - 20, rect->bottom - 20, rect->right, rect->bottom);
		rectLB = CRect(rect->left, rect->bottom - 20, rect->left + 20, rect->bottom);

		pDC->Arc(rectLT, CPoint(rectLT.right - 10, rectLT.top), CPoint(rectLT.left, rectLT.bottom - 9));
		pDC->Arc(rectRT, CPoint(rectRT.right, rectRT.bottom - 10), CPoint(rectRT.left + 10, rectRT.top));
		pDC->Arc(rectRB, CPoint(rectRB.left + 10, rectRB.bottom), CPoint(rectRB.right, rectRB.top + 10));
		pDC->Arc(rectLB, CPoint(rectLB.left, rectLB.bottom - 10), CPoint(rectLB.right - 9, rectLB.bottom));

		DrawLine(pDC, CPoint(rect->left + 10, rect->top), CPoint(rectText.left, rect->top), crPen);
		DrawLine(pDC, CPoint(rectText.right, rect->top), CPoint(rect->right - 10, rect->top), crPen);
		DrawLine(pDC, CPoint(rect->right, rect->top + 10), CPoint(rect->right, rect->bottom - 10), crPen);
		DrawLine(pDC, CPoint(rect->right - 11, rect->bottom), CPoint(rect->left + 10, rect->bottom), crPen);
		DrawLine(pDC, CPoint(rect->left, rect->bottom - 10), CPoint(rect->left, rect->top + 10), crPen);
	}
	else {
		DrawLine(pDC, CPoint(rect->left, rect->top), CPoint(rectText.left, rect->top), crPen);
		DrawLine(pDC, CPoint(rectText.right, rect->top), CPoint(rect->right, rect->top), crPen);
		DrawLine(pDC, CPoint(rect->right, rect->top), CPoint(rect->right, rect->bottom), crPen);
		DrawLine(pDC, CPoint(rect->right, rect->bottom), CPoint(rect->left, rect->bottom), crPen);
		DrawLine(pDC, CPoint(rect->left, rect->bottom), CPoint(rect->left, rect->top), crPen);
	}

	if ((szText != CSize()) && (szTitle != NULL))
		DrawText(pDC, rectText, szTitle, iFontSize, crText, dwAlign, szFont);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

void DrawRoundGroupBox(CDC* pDC, LPRECT rect, LPCTSTR szTitle, CSize szText, CPoint ptS, CPoint pt, int iFontSize, COLORREF crPen, COLORREF crText, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	DrawGroupBox(pDC, rect, szTitle, szText, ptS, iFontSize, crPen, crText, dwAlign, szFont);
}

void DrawText(CDC* pDC, LPRECT rect, LPCTSTR szText, int iFontSize, COLORREF crText, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CString strText(szText);
	CFont	font;
	LOGFONT lf;
	TEXTMETRIC tm;
	memset(&lf, 0x00, sizeof(lf));

	if ((iFontSize <= 0) || (iFontSize >= 500))
		iFontSize = DT_FONTSIZE;
	//if ((iWeight < 400) || (iWeight > 2000))
	//	iWeight = DT_FONTWEIGHT;

	lf.lfWeight = 400;
	lf.lfHeight = iFontSize * 10;
	wcscpy_s(lf.lfFaceName, szFont);
	font.CreatePointFontIndirect(&lf, pDC);
	font.GetLogFont(&lf);

	CFont*		pOld = pDC->SelectObject(&font);
	int			iBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF	color = pDC->SetTextColor(crText);
	pDC->GetTextMetrics(&tm);

	//Text VCenter
	CRect rectT(rect);
	if (!(dwAlign & DT_SINGLELINE) && (dwAlign & DT_VCENTER)) {
		CString strTemp(szText);
		rectT.DeflateRect(2, 2);

		int iVCount = 1;
		int iFCount = 0;
		while (1) {
			iFCount = strTemp.Find(_T("\n"), iFCount);

			if (iFCount >= 0) {
				iFCount++;
				iVCount++;
			}
			else
				break;
		}

		if (iVCount > 1) {
			double dH = tm.tmHeight + tm.tmExternalLeading;
			double dT = (rectT.Height() - (dH *iVCount)) / 2.;
			rectT = CRect(rectT.left, rectT.top + (int)dT, rectT.right, rectT.bottom);
		}
		else {
			dwAlign |= DT_SINGLELINE;
		}
	}


	if (dwAlign & DT_LEFT)
		strText.Format(_T(" %s"), szText);
	else if (dwAlign & DT_RIGHT)
		strText.Format(_T("%s "), szText);

	pDC->DrawText(szText, rectT, dwAlign);

	pDC->SetTextColor(color);
	pDC->SetBkMode(iBkMode);
	pDC->SelectObject(pOld);
	font.DeleteObject();
}

void SelectedDrawText(CDC* pDC, LPRECT rect, LPCTSTR szText, int iFontSize, COLORREF crPen, COLORREF crBack, COLORREF crText, DWORD dwAlign, LPCTSTR szFont) {
	CRect rectT(rect);
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CPen  pen(PS_SOLID, 1, crPen);
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->FillSolidRect(rectT, crBack);
	DrawLineRect(pDC, rectT, crPen);

	if (dwAlign == DT_LEFT) {
		CString strText;
		strText.Format(_T(" %s"), szText);
		DrawText(pDC, rect, strText, iFontSize, crText, dwAlign, szFont);
	}
	else
		DrawText(pDC, rect, szText, iFontSize, crText, dwAlign, szFont);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

void SelectedDrawInt(CDC* pDC, LPRECT rect, int iValue, int iFontSize, COLORREF crPen, COLORREF crBack, COLORREF crText, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CString str;
	str.Format(_T("%d"), iValue);
	SelectedDrawText(pDC, rect, str, iFontSize, crPen, crBack, crText, dwAlign, szFont);
}

void SelectedDrawDouble(CDC* pDC, LPRECT rect, double dValue, int iPoint, int iFontSize, COLORREF crPen, COLORREF crBack, COLORREF crText, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	
	CString strFormat, str;

	if (iPoint > 20) iPoint = 20;
	
	if (fabs(dValue) > 1e20)
		strFormat.Format(_T("%%.%dg"), iPoint);
	else
		strFormat.Format(_T("%%.%df"), iPoint);
	
	str.Format(strFormat, dValue);
	SelectedDrawText(pDC, rect, str, iFontSize, crPen, crBack, crText, dwAlign, szFont);
}

void SelectedDrawEditText(CDC* pDC, LPRECT rect, LPCTSTR szText, BOOL bEdit, int iFontSize, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	
	CString str;
	COLORREF crPen, crBack, crText;

	if (bEdit) {
		crPen	= DT_EDIT_ENABLE_PEN;
		crBack	= DT_EDIT_ENABLE_BK;
		crText	= DT_EDIT_ENABLE_TEXT;
	}
	else {
		crPen	= DT_EDIT_DISABLE_PEN;
		crBack	= DT_EDIT_DISABLE_BK;
		crText	= DT_EDIT_DISABLE_TEXT;
	}

	SelectedDrawText(pDC, rect, szText, iFontSize, crPen, crBack, crText, dwAlign, szFont);
}

void DrawEditBrowse(CDC* pDC, LPRECT rect, LPCTSTR szText, int iFontSize, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	
	CString str;
	COLORREF crPen, crBack, crText;

	crPen	= DT_EDIT_ENABLE_PEN;
	crBack	= DT_EDIT_ENABLE_BK;
	crText	= DT_EDIT_ENABLE_TEXT;

	CRect rectText, rectImg;
	CRect rectT(rect);
	rectText = CRect(rectT.left, rectT.top, rectT.right-rectT.Height(), rectT.bottom);
	rectImg = CRect(rectT.right-rectT.Height(), rectT.top, rectT.right, rectT.bottom);

	rectImg.InflateRect(-2, -2);

	CPen	pen(PS_SOLID, 1, RGB_GRAY);
	CBrush	brush;
	brush.CreateSolidBrush(RGB_RGB_FOLDER);
	CPen*	pOldPen		= pDC->SelectObject(&pen);
	CBrush* pOldBrush	= pDC->SelectObject(&brush);

	SelectedDrawText(pDC, rect, _T(""), iFontSize, crPen, crBack, crText, dwAlign, szFont);
	DrawText(pDC, rect, szText, iFontSize, crText, dwAlign, szFont);

	POINT pt[6];
	
	pt[0] = CPoint(rectImg.left+5,	rectImg.top+2);
	pt[1] = CPoint(rectImg.right-4, rectImg.top+1);
	pt[2] = CPoint(rectImg.right-4, rectImg.top+rectImg.Height()/2.-2);
	pt[3] = CPoint(rectImg.right-2,	rectImg.top+rectImg.Height()/2.-2);
	pt[4] = CPoint(rectImg.right-2,	rectImg.bottom-5);
	pt[5] = CPoint(rectImg.left+5,	rectImg.bottom-5);
	pDC->Polygon(pt, 6);
	
	SelectedDrawText(pDC, CRect(rectImg.left+5, rectImg.top+3, rectImg.right-4, rectImg.bottom-5), _T(""), iFontSize, RGB_GRAY, RGB_WHITE, crText, dwAlign, szFont);
	
	pt[0] = CPoint(rectImg.left+5,	rectImg.top+2);
	pt[1] = CPoint(rectImg.right-7,	rectImg.top+5);
	pt[2] = CPoint(rectImg.right-7,	rectImg.bottom-2);
	pt[3] = CPoint(rectImg.left+5,	rectImg.bottom-5);
	pDC->Polygon(pt, 4);

	pDC->SelectObject(pen);
	pDC->SelectObject(pOldBrush);

	pen.DeleteObject();
	brush.DeleteObject();
}

void SelectedDrawEditInt(CDC* pDC, LPRECT rect, int iValue, BOOL bEdit, int iFontSize, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	
	CString strFormat, str;
	COLORREF crPen, crBack, crText;

	if (bEdit) {
		crPen	= DT_EDIT_ENABLE_PEN;
		crBack	= DT_EDIT_ENABLE_BK;
		crText	= DT_EDIT_ENABLE_TEXT;
	}
	else {
		crPen	= DT_EDIT_DISABLE_PEN;
		crBack	= DT_EDIT_DISABLE_BK;
		crText	= DT_EDIT_DISABLE_TEXT;
	}

	SelectedDrawInt(pDC, rect, iValue, iFontSize, crPen, crBack, crText, dwAlign, szFont);
}

void SelectedDrawEditDouble(CDC* pDC, LPRECT rect, double dValue, int iPoint, BOOL bEdit, int iFontSize, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	
	CString strFormat, str;
	COLORREF crPen, crBack, crText;

	if (bEdit) {
		crPen	= DT_EDIT_ENABLE_PEN;
		crBack	= DT_EDIT_ENABLE_BK;
		crText	= DT_EDIT_ENABLE_TEXT;
	}
	else {
		crPen	= DT_EDIT_DISABLE_PEN;
		crBack	= DT_EDIT_DISABLE_BK;
		crText	= DT_EDIT_DISABLE_TEXT;
	}

	SelectedDrawDouble(pDC, rect, dValue, iPoint, iFontSize, crPen, crBack, crText, dwAlign, szFont);
}

void DrawButtonUpDown(CDC* pDC, LPRECT rect, LPCTSTR szText, BOOL bBtnDown, COLORREF crBackDown, COLORREF crBackUp, int iFontSize, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;


	COLORREF crPen,crBack, crText;
	if (bBtnDown) {
		crPen	= DT_BTN_DOWN_PEN;
		crBack	= crBackDown;
		crText	= DT_BTN_DOWN_TEXT;
	}
	else {
		crPen	= DT_BTN_UP_PEN;
		crBack	= crBackUp;
		crText	= DT_BTN_UP_TEXT;
	}

	SelectedDrawText(pDC, rect, szText, iFontSize, crPen, crBack, crText, dwAlign, szFont);
}

void DrawToggleButton(CDC* pDC, LPRECT rect, BOOL bToggleOn, LPCTSTR szTextOn, LPCTSTR szTextOff, int iFontSize, COLORREF crBackOn, COLORREF crBackOff, COLORREF crPenOn, COLORREF crPenOff, COLORREF crTextOn, COLORREF crTextOff, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	if (bToggleOn)
		SelectedDrawText(pDC, rect, szTextOn, iFontSize, crPenOn, crBackOn, crTextOn, dwAlign, szFont);
	else
		SelectedDrawText(pDC, rect, szTextOff, iFontSize, crPenOff, crBackOff, crTextOff, dwAlign, szFont);
}

void DrawRoundButtonUpDown(CDC* pDC, LPRECT rect, LPCTSTR szText, BOOL bBtnDown, CPoint pt, int iFontSize, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	COLORREF crPen,crBack, crText;
	if (bBtnDown) {
		crPen	= DT_BTN_DOWN_PEN;
		crBack	= DT_BTN_DOWN_BK;
		crText	= DT_BTN_DOWN_TEXT;
	}
	else {
		crPen	= DT_BTN_UP_PEN;
		crBack	= DT_BTN_UP_BK;
		crText	= DT_BTN_UP_TEXT;
	}

	CPen	pen(PS_SOLID, 1, crPen);
	CBrush	brush;
	brush.CreateSolidBrush(crBack);
	CPen*	pOldPen		= pDC->SelectObject(&pen);
	CBrush* pOldBrush	= pDC->SelectObject(&brush);

	if (CRect(rect).IsRectEmpty())
		return;
	
	pDC->RoundRect(rect, pt);
	pDC->FillPath();
	pDC->SelectObject( pOldPen );
	pDC->SelectObject( pOldBrush );
	
	DrawText(pDC, rect, szText, iFontSize, crText, dwAlign, szFont);

	pen.DeleteObject();
	brush.DeleteObject();
}

void Draw3dButton(CDC* pDC, LPRECT rect, LPCTSTR szText, BOOL bBtnDown, int iTick, COLORREF crText, COLORREF crBK, int iFontSize, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CRect rectT(rect);

	if (iTick < 1) iTick = 1;
	else if (iTick > 3) iTick = 3;

	//rectT.InflateRect(-1, -1);
	if (bBtnDown) {
		SelectedDrawText(pDC, rectT, szText, iFontSize, RGB_WHITE, RGB_GRAY, crText, dwAlign, szFont);
		
		// Left Top
	//	DrawLine(pDC, CPoint(rect->left, rect->bottom), CPoint(rect->left, rect->top), RGB_BLACK, iTick);
	//	DrawLine(pDC, CPoint(rect->left, rect->top), CPoint(rect->right, rect->top), RGB_BLACK, iTick);
	//
	//	// Right Bottom
	//	DrawLine(pDC, CPoint(rect->right, rect->top), CPoint(rect->right, rect->bottom), RGB_WHITE, iTick);
	//	DrawLine(pDC, CPoint(rect->right, rect->bottom), CPoint(rect->left, rect->bottom), RGB_WHITE, iTick);

		//pDC->Draw3dRect(rect, RGB_BLACK, RGB_WHITE);
	}
	else {
		SelectedDrawText(pDC, rectT, szText, iFontSize, RGB_WHITE, crBK, crText, dwAlign, szFont);

		// Left Top
	//	DrawLine(pDC, CPoint(rect->left, rect->top), CPoint(rect->right, rect->top), RGB_WHITE, iTick);
	//	DrawLine(pDC, CPoint(rect->left, rect->bottom), CPoint(rect->left, rect->top), RGB_WHITE, iTick);
	//
	//	// Right Bottom
	//	DrawLine(pDC, CPoint(rect->right, rect->top), CPoint(rect->right, rect->bottom), RGB_BLACK, iTick);
	//	DrawLine(pDC, CPoint(rect->right, rect->bottom), CPoint(rect->left, rect->bottom), RGB_BLACK, iTick);

		//pDC->Draw3dRect(rect, RGB_WHITE, RGB_BLACK);
	}
}


void DrawSensor(CDC* pDC, LPRECT rect, LPCTSTR szText, BOOL bSensor, COLORREF crSensorOn, COLORREF crSensorOff, int iFontSize, COLORREF crPen, COLORREF crText, COLORREF crBack, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CRect rectTemp, rectSensor, rectText;
	rectTemp = rect;
	COLORREF crSensor;

	if (rectTemp.Width() > rectTemp.Height()) {//W
		rectSensor.SetRect(rectTemp.left + 5, rectTemp.top + 5, rectTemp.left+rectTemp.Height()-5, rectTemp.bottom-5);
		rectText.SetRect(rectSensor.right, rectTemp.top, rectTemp.right, rectTemp.bottom);
	}
	else {
		rectSensor.SetRect(rectTemp.left + 5, rectTemp.top + 5, rectTemp.right-5, rectTemp.top + rectTemp.Width()-5);
		rectText.SetRect(rectTemp.left + 5, rectSensor.bottom, rectTemp.right-5, rectTemp.bottom);
	}
	crSensor = bSensor? crSensorOn : crSensorOff;

	if (CRect(rect).IsRectEmpty())
		return;
	SelectedDrawText(pDC, rect, _T(""), iFontSize, crPen, crBack);
	SelectedDrawText(pDC, rectSensor, _T(""), DT_FONTSIZE, DT_PEN, crSensor);
	DrawText(pDC, rectText, szText, iFontSize, crText, dwAlign, szFont);
}

void DrawRoundSensor(CDC* pDC, LPRECT rect, LPCTSTR szText, BOOL bSensor, CPoint pt, COLORREF crSensorOn, COLORREF crSensorOff, int iFontSize, COLORREF crPen, COLORREF crText, COLORREF crBack, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CRect rectTemp, rectSensor, rectText;
	rectTemp = rect;

	if (rectTemp.Width() > rectTemp.Height()) {
		rectSensor.SetRect(rectTemp.left + 5, rectTemp.top + 5, rectTemp.left+rectTemp.Height()-5, rectTemp.bottom-5);
		rectText.SetRect(rectSensor.right, rectTemp.top, rectTemp.right, rectTemp.bottom);
	}
	else {
		rectSensor.SetRect(rectTemp.left + 5, rectTemp.top + 5, rectTemp.right-5, rectTemp.top + rectTemp.Width()-5);
		rectText.SetRect(rectTemp.left + 5, rectSensor.bottom, rectTemp.right-5, rectTemp.bottom);
	}

	COLORREF crSensor = bSensor? crSensorOn : crSensorOff;
	CPen	pen(PS_SOLID, 1, crPen);
	CPen*	pOldPen		= pDC->SelectObject(&pen);
	CBrush	brush;
	brush.CreateSolidBrush(crBack);
	CBrush* pOldBrush	= pDC->SelectObject(&brush);

	pDC->RoundRect(rect, pt);
	pDC->FillPath();

	if (CRect(rect).IsRectEmpty())
		return;

	brush.DeleteObject();
	brush.CreateSolidBrush(crSensor);
	pDC->SelectObject(&brush);

	pDC->RoundRect(rectSensor, pt);
	pDC->FillPath();

	pDC->SelectObject( pOldPen );
	pDC->SelectObject( pOldBrush );

	DrawText(pDC, rectText, szText, iFontSize, crText, dwAlign, szFont);

	pen.DeleteObject();
	brush.DeleteObject();
}

void DrawEllipseSensor(CDC* pDC, LPRECT rect, LPCTSTR szText, BOOL bSensor, CPoint pt, COLORREF crSensorOn, COLORREF crSensorOff, int iFontSize, COLORREF crPen, COLORREF crText, COLORREF crBack, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CRect rectTemp, rectSensor, rectText;
	rectTemp = rect;
	double dGab = _min(rectTemp.Width(), rectTemp.Height())*0.15;
	if (rectTemp.Width() > rectTemp.Height()) {
		rectSensor.SetRect(rectTemp.left + dGab, rectTemp.top + dGab, rectTemp.left+rectTemp.Height()-dGab, rectTemp.bottom-dGab);
		rectText.SetRect(rectSensor.right + 5, rectTemp.top, rectTemp.right, rectTemp.bottom);
	}
	else {
		rectSensor.SetRect(rectTemp.left + dGab, rectTemp.top + dGab, rectTemp.right-dGab, rectTemp.top + rectTemp.Width()-dGab);
		rectText.SetRect(rectTemp.left + 5, rectSensor.bottom, rectTemp.right-5, rectTemp.bottom);
	}


	CPen	pen(PS_SOLID, 1, crPen);
	CPen*	pOldPen		= pDC->SelectObject(&pen);
	CBrush	brush;
	brush.CreateSolidBrush(crBack);
	CBrush* pOldBrush	= pDC->SelectObject(&brush);

	pDC->RoundRect(rect, pt);
	pDC->FillPath();

	if (CRect(rect).IsRectEmpty())
		return;

	// 2021.09.01 타원 찌그러짐 때문에 CDC -> Gdiplus로 수정
	COLORREF crSensor = bSensor? crSensorOn : crSensorOff;
	brush.DeleteObject();
	brush.CreateSolidBrush(crSensor);
	pDC->SelectObject(&brush);
	pDC->Ellipse(rectSensor);//pDC->RoundRect(rectSensor, pt);
	pDC->FillPath();
	//Gdiplus::Graphics g(pDC->m_hDC);
	//Gdiplus::SolidBrush gbrush(Gdiplus::Color::Red);
	//g.FillEllipse(&gbrush, rectSensor.left, rectSensor.top, rectSensor.Width(), rectSensor.Height());

	pDC->SelectObject( pOldPen );
	pDC->SelectObject( pOldBrush );

	DrawText(pDC, rectText, szText, iFontSize, crText, dwAlign, szFont);

	pen.DeleteObject();
	brush.DeleteObject();
}

void DrawMoveDirect(CDC* pDC, CRect rect, BOOL bClick, eMOVE_DIRECT eDirect, LPCTSTR szText, BOOL bDrawRect) {
	CRect rectMove(rect);
	double dW, dH;

	dW = (rect.Width() / 10.);
	dH = (rect.Height() / 10.);
	int iL, iT, iR, iB;

	switch (eDirect) {
	case eMD_UP: iL = -dW; iT = -dH*1.5; iR = -dW; iB = -dH*2; break;
	case eMD_LEFT: iL = -dW * 2; iT = -dH; iR = -dW*1.5; iB = -dH; break;
	case eMD_RIGHT:	iL = -dW*1.5; iT = -dH; iR = -dW * 2; iB = -dH; break;
	case eMD_DOWN: iL = -dW; iT = -dH * 2; iR = -dW; iB = -dH*1.5; break;
	}

	rectMove.InflateRect(iL, iT, iR, iB);

	CPoint ptLT(rectMove.TopLeft());
	CPoint ptLC(rectMove.left, rectMove.CenterPoint().y);
	CPoint ptLB(rectMove.left, rectMove.bottom);

	CPoint ptRT(rectMove.right, rectMove.top);
	CPoint ptRC(rectMove.right, rectMove.CenterPoint().y);
	CPoint ptRB(rectMove.right, rectMove.bottom);

	CPoint ptCT(rectMove.CenterPoint().x, rectMove.top);
	CPoint ptCB(rectMove.CenterPoint().x, rectMove.bottom);

	CBrush brush;

	POINT pt[3];

	switch (eDirect) {
		case eMD_UP :		pt[0] = ptLB; pt[1] = ptCT; pt[2] = ptRB; break;
		case eMD_LEFT :		pt[0] = ptLC; pt[1] = ptRT; pt[2] = ptRB; break;
		case eMD_RIGHT :	pt[0] = ptLT; pt[1] = ptRC; pt[2] = ptLB; break;
		case eMD_DOWN :		pt[0] = ptLT; pt[1] = ptRT; pt[2] = ptCB; break;
	}
	
	COLORREF crBack, crRect, crText;

	if (bClick) {
		crRect = RGB_WHITE;
		crBack = RGB_BLACK;
		crText = RGB_BLACK;
	}
	else {
		crRect = RGB_BLACK;
		crBack = DT_BK;
		crText = RGB_WHITE;
	}

	if (bDrawRect) {
		DrawLineRect(pDC, rect);
		SelectedDrawText(pDC, rect, _T(""), DT_FONTSIZE, crText, crBack, crText);
	}
	
	brush.CreateSolidBrush(crRect);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->Polygon(pt, 3);

	CRect rectText(rectMove);
	switch (eDirect) {
		case eMD_UP :		DrawText(pDC, rectText, szText, DT_FONTSIZE, crText, DT_VCENTER | DT_CENTER );	break;
		case eMD_LEFT :		DrawText(pDC, rectText, szText, DT_FONTSIZE, crText, DT_VCENTER | DT_RIGHT);	break;
		case eMD_RIGHT :	DrawText(pDC, rectText, szText, DT_FONTSIZE, crText, DT_VCENTER | DT_LEFT);	break;
		case eMD_DOWN :		DrawText(pDC, rectText, szText, DT_FONTSIZE, crText, DT_VCENTER | DT_CENTER );		break;
	}
	
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
}


void Draw3dMoveDirect(CDC* pDC, CRect rect, BOOL bClick, eMOVE_DIRECT eDirect, LPCTSTR szText, BOOL bDrawRect) {
	CRect rectMove(rect);
	double dW, dH;

	dW = (rect.Width() / 10.);
	dH = (rect.Height() / 10.);
	int iL, iT, iR, iB;

	if (bClick) {
		switch (eDirect) {
			case eMD_UP: iL = -dW; iT = -dH*1.5; iR = -dW; iB = -dH*2; break;
			case eMD_LEFT: iL = -dW * 2; iT = -dH; iR = -dW*1.5; iB = -dH; break;
			case eMD_RIGHT:	iL = -dW*1.5; iT = -dH; iR = -dW * 2; iB = -dH; break;
			case eMD_DOWN: iL = -dW; iT = -dH * 2; iR = -dW; iB = -dH*1.5; break;
		}
		iL -= 1;
		iT -= 1;
		iR += 1;
		iB += 1;
	}
	else {
		switch (eDirect) {
			case eMD_UP: iL = -dW; iT = -dH*1.5; iR = -dW; iB = -dH*2; break;
			case eMD_LEFT: iL = -dW * 2; iT = -dH; iR = -dW*1.5; iB = -dH; break;
			case eMD_RIGHT:	iL = -dW*1.5; iT = -dH; iR = -dW * 2; iB = -dH; break;
			case eMD_DOWN: iL = -dW; iT = -dH * 2; iR = -dW; iB = -dH*1.5; break;
		}
	}

	rectMove.InflateRect(iL, iT, iR, iB);

	CPoint ptLT(rectMove.TopLeft());
	CPoint ptLC(rectMove.left, rectMove.CenterPoint().y);
	CPoint ptLB(rectMove.left, rectMove.bottom);

	CPoint ptRT(rectMove.right, rectMove.top);
	CPoint ptRC(rectMove.right, rectMove.CenterPoint().y);
	CPoint ptRB(rectMove.right, rectMove.bottom);

	CPoint ptCT(rectMove.CenterPoint().x, rectMove.top);
	CPoint ptCB(rectMove.CenterPoint().x, rectMove.bottom);

	CBrush brush;

	POINT pt[3];

	switch (eDirect) {
		case eMD_UP :		pt[0] = ptLB; pt[1] = ptCT; pt[2] = ptRB; break;
		case eMD_LEFT :		pt[0] = ptLC; pt[1] = ptRT; pt[2] = ptRB; break;
		case eMD_RIGHT :	pt[0] = ptLT; pt[1] = ptRC; pt[2] = ptLB; break;
		case eMD_DOWN :		pt[0] = ptLT; pt[1] = ptRT; pt[2] = ptCB; break;
	}
	
	COLORREF crBack, crRect, crText;

	crRect = RGB_BLACK;
	crBack = DT_BK;
	crText = RGB_WHITE;

	if (bDrawRect) {
		Draw3dButton(pDC,rect,_T(""),bClick,2, crText,crBack,DT_FONTSIZE);
	}
	else{
	crRect = RGB_GRAY_DARK;
		if (bClick) {
			switch (eDirect)
			{
			case eMD_UP:
			case eMD_LEFT:
				DrawLine(pDC, pt[0], pt[2], RGB_WHITE, 5);
				DrawLine(pDC, pt[1], pt[2], RGB_WHITE, 5);
				DrawLine(pDC, pt[0], pt[1], RGB_BLACK, 5);
				break;
			case eMD_RIGHT:
			case eMD_DOWN:
				DrawLine(pDC, pt[1], pt[2], RGB_WHITE, 5);
				DrawLine(pDC, pt[0], pt[2], RGB_WHITE, 5);
				DrawLine(pDC, pt[0], pt[1], RGB_BLACK, 5);
				break;
			}
		}
		else {
			switch (eDirect)
			{
			case eMD_UP:
			case eMD_LEFT:
				DrawLine(pDC, pt[0], pt[1], RGB_WHITE, 5);
				DrawLine(pDC, pt[1], pt[2], RGB_BLACK, 5);
				DrawLine(pDC, pt[0], pt[2], RGB_BLACK, 5);
				break;
			case eMD_RIGHT:
			case eMD_DOWN:
				DrawLine(pDC, pt[0], pt[1], RGB_WHITE, 5);
				DrawLine(pDC, pt[0], pt[2], RGB_BLACK, 5);
				DrawLine(pDC, pt[1], pt[2], RGB_BLACK, 5);
				break;
			}
		}
	}
	
	brush.CreateSolidBrush(crRect);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->SelectStockObject(NULL_PEN);
	pDC->Polygon(pt, 3);

	CRect rectText(rectMove);
	switch (eDirect) {
		case eMD_UP :		DrawText(pDC, rectText, szText, DT_FONTSIZE, crText, DT_VCENTER | DT_CENTER );	break;
		case eMD_LEFT :		DrawText(pDC, rectText, szText, DT_FONTSIZE, crText, DT_VCENTER | DT_RIGHT);	break;
		case eMD_RIGHT :	DrawText(pDC, rectText, szText, DT_FONTSIZE, crText, DT_VCENTER | DT_LEFT);	break;
		case eMD_DOWN :		DrawText(pDC, rectText, szText, DT_FONTSIZE, crText, DT_VCENTER | DT_CENTER );		break;
	}
	
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
}

void Draw3dGradationButton(CDC* pDC, LPRECT rect, LPCTSTR szText, BOOL bBtnDown, int iFontSize, int iGradation, COLORREF crText, COLORREF crStartBK,COLORREF crEndBK, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	TRIVERTEX vert[2];
    GRADIENT_RECT rectG;
	CRect rectT(rect);
	CRect rectB(rect);

	vert[0].x      = rectT.left;
    vert[0].y      = rectT.top;
	
	vert[1].x      = rectT.right;
    vert[1].y      = rectT.bottom;

	vert[0].Red		= GetRValue(crStartBK) << 8;
	vert[0].Green	= GetGValue(crStartBK) << 8;
	vert[0].Blue	= GetBValue(crStartBK) << 8;
	vert[0].Alpha	= 0x0000;

	vert[1].Red		= GetRValue(crEndBK) << 8;
	vert[1].Green	= GetGValue(crEndBK) << 8;
	vert[1].Blue	= GetBValue(crEndBK) << 8;
	vert[1].Alpha	= 0x0000;
	
    rectG.UpperLeft  = 0;
    rectG.LowerRight = 1;

	
	if (iGradation < eGO_H || iGradation > eGO_M)
		iGradation = eGO_V;

	GradientFill(pDC->m_hDC, vert, 2, &rectG, 1, iGradation);

	if (bBtnDown) {
		rectB.left += 2;
		rectB.top += 2;
		rectB.right += 2;
		rectB.bottom += 2;

		rectT.left += 1;
		rectT.top += 1;
		rectT.right += 1;
		rectT.bottom += 1;

		DrawText(pDC, rectB, szText, iFontSize, RGB_BLACK, dwAlign, szFont);
		DrawText(pDC, rectT, szText, iFontSize, crText, dwAlign, szFont);
		
		// Left Top
		DrawLine(pDC, CPoint(rect->left, rect->bottom), CPoint(rect->left, rect->top), crStartBK);
		DrawLine(pDC, CPoint(rect->left, rect->top), CPoint(rect->right, rect->top), crStartBK);
	
		// Right Bottom
		DrawLine(pDC, CPoint(rect->right, rect->top), CPoint(rect->right, rect->bottom), crStartBK);
		DrawLine(pDC, CPoint(rect->right, rect->bottom), CPoint(rect->left, rect->bottom), crStartBK);

		//pDC->Draw3dRect(rect, RGB_BLACK, RGB_WHITE);
	}
	else {
		rectB.left += 1;
		rectB.top += 1;
		rectB.right += 1;
		rectB.bottom += 1;

		DrawText(pDC, rectB, szText, iFontSize, RGB_BLACK, dwAlign, szFont);
		DrawText(pDC, rectT, szText, iFontSize, crText, dwAlign, szFont);

		// Left Top
		DrawLine(pDC, CPoint(rect->left, rect->top), CPoint(rect->right, rect->top), RGB_BLACK);
		DrawLine(pDC, CPoint(rect->left, rect->bottom), CPoint(rect->left, rect->top), RGB_BLACK);
	
		// Right Bottom
		DrawLine(pDC, CPoint(rect->right, rect->top), CPoint(rect->right, rect->bottom), RGB_BLACK);
		DrawLine(pDC, CPoint(rect->right, rect->bottom), CPoint(rect->left, rect->bottom), RGB_BLACK);

		//pDC->Draw3dRect(rect, RGB_WHITE, RGB_BLACK);
	}
}

void DrawButtonUpDownMouseMove(CDC * pDC, LPRECT rect, LPCTSTR szText, int iSelect, int iMouseover, int iEnum, int iFontSize, DWORD dwAlign, LPCTSTR szFont, COLORREF crSelColor, COLORREF crMoveColor, COLORREF crDefalut, COLORREF crDefFont,COLORREF crSelFont, COLORREF crSelPen, COLORREF crDefPen)
{
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	
	if (iSelect == iEnum) {
		SelectedDrawText(pDC,rect,	szText, iFontSize, crSelPen, crSelColor,crSelFont,dwAlign);	
	}
	else if (iMouseover == iEnum) {
		SelectedDrawText(pDC,rect,	szText, iFontSize, crSelPen, crMoveColor,crSelFont,dwAlign);
	}
	else {
		SelectedDrawText(pDC,rect,	szText, iFontSize, crDefalut, crDefalut,crDefFont,dwAlign);
	}
}

void Draw3dGradationMouseMoveButton(CDC * pDC, LPRECT rect, LPCTSTR szText, int iSelect, int iMouseover, int iEnum, int iFontSize, int iGradation, COLORREF crText, COLORREF crSelStartBK, COLORREF crSelEndBK, COLORREF crMoveStartBK, COLORREF crMoveEndBK, COLORREF crDefStartBK, COLORREF crDefEndBK, DWORD dwAlign, LPCTSTR szFont)
{
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	
	if (iSelect == iEnum) {
			Draw3dGradationButton(pDC, rect,	szText,	true,	iFontSize,	iGradation,	crText,crSelStartBK,crSelEndBK);
	}
	else if (iMouseover == iEnum) {
			Draw3dGradationButton(pDC, rect,	szText,	false,	iFontSize,	iGradation,	crText,crMoveStartBK,crMoveEndBK);
	}
	else {
			Draw3dGradationButton(pDC, rect,	szText,	false,	iFontSize,	iGradation,	crText,crDefStartBK,crDefEndBK);
	}
}

void DrawRoundButtonUpDownSensor(CDC* pDC, LPRECT rect, LPCTSTR szText, BOOL bBtnDown, BOOL bSensor, CPoint pt, int iFontSize, int iBoder, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	COLORREF crPen, crBack, crText;
	if (bBtnDown) {
		crPen = DT_BTN_DOWN_PEN;
		crBack = DT_BTN_DOWN_BK;
		crText = DT_BTN_DOWN_TEXT;
	}
	else {
		if (bSensor) {
			crPen = DT_BTN_UP_PEN;
			crBack = RGB_GREEN;
			crText = RGB_BLACK;
		}
		else {
			crPen = DT_BTN_UP_PEN;
			crBack = DT_BTN_UP_BK;
			crText = DT_BTN_UP_TEXT;
		}
	}

	CPen	pen(PS_SOLID, 1, crPen);
	CBrush	brush;
	brush.CreateSolidBrush(crBack);
	CPen*	pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	if (CRect(rect).IsRectEmpty())
		return;

	pDC->RoundRect(rect, pt);
	pDC->FillPath();
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	DrawText(pDC, rect, szText, iFontSize, crText, dwAlign, szFont);

	pen.DeleteObject();
	brush.DeleteObject();
}
void DrawTextB(CDC* pDC, LPRECT rect, LPCTSTR szText, int iFontSize, int iWeight, COLORREF crText, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CString strText(szText);
	CFont	font;
	LOGFONT lf;
	TEXTMETRIC tm;
	memset(&lf, 0x00, sizeof(lf));
	
	if ((iFontSize <= 0) || (iFontSize >= 200))
		iFontSize = DT_FONTSIZE;

	//font.CreatePointFont(iFontSize*10, szFont, pDC);
	
	lf.lfWeight = iWeight;
	lf.lfHeight =iFontSize*10;
	wcscpy_s(lf.lfFaceName, szFont);
	font.CreatePointFontIndirect(&lf, pDC);
	CFont*		pOld	= pDC->SelectObject(&font);
	int			iBkMode	= pDC->SetBkMode(TRANSPARENT);
	COLORREF	color	= pDC->SetTextColor(crText);

	pDC->GetTextMetrics(&tm);

	//Text VCenter
	CRect rectT(rect);
	if (!(dwAlign & DT_SINGLELINE) && (dwAlign & DT_VCENTER)) {
		CString strTemp(szText);
		rectT.DeflateRect(2, 2);

		int iVCount = 1;
		int iFCount = 0;
		while (1) {
			iFCount = strTemp.Find(_T("\n"), iFCount);

			if (iFCount >= 0) {
				iFCount++;
				iVCount++;
			}
			else
				break;
		}

		if (iVCount > 1) {
			double dH = tm.tmHeight + tm.tmExternalLeading;
			double dT = (rectT.Height() - (dH *iVCount)) / 2.;
			rectT = CRect(rectT.left, rectT.top + (int)dT, rectT.right, rectT.bottom);
		}
		else {
			dwAlign |= DT_SINGLELINE;
		}
	}

	if (dwAlign & DT_LEFT)
		strText.Format(_T(" %s"), szText);
	else if (dwAlign & DT_RIGHT)
		strText.Format(_T("%s "), szText);

	pDC->DrawText(szText, rectT, dwAlign);

	pDC->SetTextColor(color);
	pDC->SetBkMode(iBkMode);
	pDC->SelectObject(pOld);
	font.DeleteObject();
}

void SelectedDrawTextB(CDC* pDC, LPRECT rect, LPCTSTR szText,int iBoder, int iFontSize, COLORREF crPen, COLORREF crBack, COLORREF crText, DWORD dwAlign, LPCTSTR szFont) {
	CRect rectT(rect);
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CPen  pen(PS_SOLID, 1, crPen);
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->FillSolidRect(rectT, crBack);
	DrawLineRect(pDC, rectT, crPen);

	if (dwAlign == DT_LEFT) {
		CString strText;
		strText.Format(_T(" %s"), szText);
		DrawTextB(pDC, rect, strText,iFontSize, iBoder, crText, dwAlign, szFont);
	}
	else
		DrawTextB(pDC, rect, szText,iFontSize, iBoder, crText, dwAlign, szFont);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}
// Timer 버튼??
void DrawTimerButtonUpDown(CDC* pDC, LPRECT rect, LPCTSTR szText, BOOL bBtnDown, double dSec, int iFontSize, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	COLORREF crPen, crBack, crText;
	if (bBtnDown) {
		crPen = DT_BTN_DOWN_PEN;
		crBack = RGB_GREEN;
		crText = DT_BTN_DOWN_TEXT;
		CRect rectParcent(rect);
		double dWidth	= rectParcent.Width()/3.;
		double dHeight	= rectParcent.Height();
		double dRight = rect->left + dWidth * dSec; 
		if (fabs(rect->right - dRight) < 0.001)
			dRight = rect->right;
		rectParcent = CRect(rect->left, rect->top + 10 + dHeight/2., dRight, rect->bottom);
		SelectedDrawText(pDC, rect, szText, iFontSize, DT_BTN_UP_PEN, DT_BTN_UP_BK, DT_BTN_UP_TEXT, dwAlign, szFont);
		SelectedDrawText(pDC, rectParcent, _T(""), iFontSize, crPen, crBack, crText, dwAlign, szFont);
	}
	else {
		crPen = DT_BTN_UP_PEN;
		crBack = DT_BTN_UP_BK;
		crText = DT_BTN_UP_TEXT;
		SelectedDrawText(pDC, rect, szText, iFontSize, crPen, crBack, crText, dwAlign, szFont);
	}
}


void Draw3dGradationButtonB(CDC* pDC, LPRECT rect, LPCTSTR szText, BOOL bBtnDown, int iFontSize, int iBoder, int iGradation, COLORREF crText, COLORREF crStartBK, COLORREF crEndBK, DWORD dwAlign, LPCTSTR szFont) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	TRIVERTEX vert[2];
	GRADIENT_RECT rectG;
	CRect rectT(rect);
	CRect rectB(rect);

	vert[0].x = rectT.left;
	vert[0].y = rectT.top;

	vert[1].x = rectT.right;
	vert[1].y = rectT.bottom;

	vert[0].Red = GetRValue(crStartBK) << 8;
	vert[0].Green = GetGValue(crStartBK) << 8;
	vert[0].Blue = GetBValue(crStartBK) << 8;
	vert[0].Alpha = 0x0000;

	vert[1].Red = GetRValue(crEndBK) << 8;
	vert[1].Green = GetGValue(crEndBK) << 8;
	vert[1].Blue = GetBValue(crEndBK) << 8;
	vert[1].Alpha = 0x0000;

	rectG.UpperLeft = 0;
	rectG.LowerRight = 1;


	if (iGradation < eGO_H || iGradation > eGO_M)
		iGradation = eGO_V;

	GradientFill(pDC->m_hDC, vert, 2, &rectG, 1, iGradation);

	if (bBtnDown) {
		rectB.left += 2;
		rectB.top += 2;
		rectB.right += 2;
		rectB.bottom += 2;

		rectT.left += 1;
		rectT.top += 1;
		rectT.right += 1;
		rectT.bottom += 1;

		DrawTextB(pDC, rectB, szText, iFontSize, iBoder, RGB_BLACK, dwAlign, szFont);
		DrawTextB(pDC, rectT, szText, iFontSize, iBoder, crText, dwAlign, szFont);

		// Left Top
		DrawLine(pDC, CPoint(rect->left, rect->bottom), CPoint(rect->left, rect->top), crStartBK);
		DrawLine(pDC, CPoint(rect->left, rect->top), CPoint(rect->right, rect->top), crStartBK);

		// Right Bottom
		DrawLine(pDC, CPoint(rect->right, rect->top), CPoint(rect->right, rect->bottom), crStartBK);
		DrawLine(pDC, CPoint(rect->right, rect->bottom), CPoint(rect->left, rect->bottom), crStartBK);

		//pDC->Draw3dRect(rect, RGB_BLACK, RGB_WHITE);
	}
	else {
		rectB.left += 1;
		rectB.top += 1;
		rectB.right += 1;
		rectB.bottom += 1;

		DrawTextB(pDC, rectB, szText, iFontSize, iBoder, RGB_BLACK, dwAlign, szFont);
		DrawTextB(pDC, rectT, szText, iFontSize, iBoder, crText, dwAlign, szFont);

		// Left Top
		DrawLine(pDC, CPoint(rect->left, rect->top), CPoint(rect->right, rect->top), RGB_GRAY_DARK);
		DrawLine(pDC, CPoint(rect->left, rect->bottom), CPoint(rect->left, rect->top), RGB_GRAY_DARK);

		// Right Bottom
		DrawLine(pDC, CPoint(rect->right, rect->top), CPoint(rect->right, rect->bottom), RGB_GRAY_DARK);
		DrawLine(pDC, CPoint(rect->right, rect->bottom), CPoint(rect->left, rect->bottom), RGB_GRAY_DARK);

		//pDC->Draw3dRect(rect, RGB_WHITE, RGB_BLACK);
	}
}

void Draw3dGradationMouseMoveButtonB(CDC* pDC, LPRECT rect, LPCTSTR szText, int iSelect, int iMouseover, int iEnum, int iFontSize, int iBoder, int iGradation, COLORREF crText, COLORREF crSelStartBK, COLORREF crSelEndBK, COLORREF crMoveStartBK, COLORREF crMoveEndBK, COLORREF crDefStartBK, COLORREF crDefEndBK, DWORD dwAlign, LPCTSTR szFont)
{
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	if (iSelect == iEnum) {
		Draw3dGradationButtonB(pDC, rect, szText, true, iFontSize, iBoder, iGradation, crText, crSelStartBK, crSelEndBK);
	}
	else if (iMouseover == iEnum) {
		Draw3dGradationButtonB(pDC, rect, szText, false, iFontSize, iBoder, iGradation, crText, crMoveStartBK, crMoveEndBK);
	}
	else {
		Draw3dGradationButtonB(pDC, rect, szText, false, iFontSize, iBoder, iGradation, crText, crDefStartBK, crDefEndBK);
	}
}
void DrawImageButtonUpDownMouseMoveB(CDC* pDC, LPRECT rect, LPCTSTR szText, int iSelect, int iMouseover, int iEnum, int iFontSize, int iBoder, CImage &img, COLORREF crPen, COLORREF crText, COLORREF crBack)
{
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	CRect rectImg(rect);
	CRect rectText(rect);

	CPoint2d ptC = rectImg.CenterPoint();
	int iImgW = img.GetWidth() ;
	int iImgH = img.GetHeight();
	rectImg.left = ptC.x - iImgW/2. ;
	rectImg.top = ptC.y - iImgH/2.	-10;
	rectImg.right =  rectImg.left +  iImgW;
	rectImg.bottom = rectImg.top + iImgH -10;
	if (iSelect == iEnum)
		SelectedDrawText(pDC, rect, _T(""), iFontSize, crPen, RGB_GRAY_DARK, crText);
	else if (iMouseover == iEnum)
		SelectedDrawText(pDC, rect, _T(""), iFontSize, RGB_BLUE, crBack, crText);
	else 
		SelectedDrawText(pDC, rect, _T(""), iFontSize, crPen, crBack, crText);
	if (img)
		img.Draw(pDC->m_hDC, rectImg);
	
	rectText.top = rectImg.bottom + 2;
	DrawTextB(pDC, rectText, szText, iFontSize, iBoder, crText, DT_CENTER);
}

void DrawTimerImageButtonUpDownMouseMoveB(CDC* pDC, LPRECT rect, LPCTSTR szText, double dSec, int iSelect, int iMouseover, int iEnum, int iFontSize, int iBoder, CImage& img, COLORREF crPen, COLORREF crText, COLORREF crBack)
{
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	CRect rectImg(rect);
	CRect rectText(rect);
	CRect rectParcent(rect);

	// 이미지 영역 계산
	CPoint2d ptC = rectImg.CenterPoint();
	int iImgW = img.GetWidth();
	int iImgH = img.GetHeight();
	rectImg.left = ptC.x - iImgW / 2.;
	rectImg.top = ptC.y - iImgH / 2. - 10;
	rectImg.right = rectImg.left + iImgW;
	rectImg.bottom = rectImg.top + iImgH - 10;

	// 배경 영역 계산 
	double dW = rectParcent.Width() / 3.;
	double dRight = rectParcent.left + dW * dSec;
	if (fabs(rectParcent.right - dRight) < 0.001)
		dRight = rectParcent.right;
	
	rectParcent.right = dRight;

	// 배경 출력
	

	if (iSelect == iEnum) {							// 버튼 눌렸을때
	SelectedDrawText(pDC, rect, _T(""), iFontSize, RGB_WHITE, crBack, crText);
	SelectedDrawText(pDC, rectParcent, _T(""), iFontSize, RGB_WHITE, RGB_GREEN, crText);
	}
	else if (iMouseover == iEnum) {					// 마우스 가져다가 댔을 때 
		SelectedDrawText(pDC, rect, _T(""), iFontSize, RGB_BLUE, crBack, crText);
	}
	else 
		SelectedDrawText(pDC, rect, _T(""), iFontSize, crPen, crBack, crText);
	// 이미지 출력
	if (img)
		img.Draw(pDC->m_hDC, rectImg);

	// TEXT 출력
	rectText.top = rectImg.bottom + 2;
	DrawTextB(pDC, rectText, szText, iFontSize, iBoder, crText, DT_CENTER);
}
void DrawGradationTimerImageButtonUpDownMouseMoveB(CDC* pDC, LPRECT rect, LPCTSTR szText, double dSec, int iSelect, int iMouseover, int iEnum, int iFontSize, int iBoder, CImage& img, COLORREF crPen, COLORREF crText, COLORREF crBack, COLORREF crSelStart, COLORREF crSelEnd, COLORREF crDefStart, COLORREF crDefEnd)
{
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	CRect rectImg(rect);
	CRect rectText(rect);
	CRect rectParcent(rect);
	
	Draw3dGradationButton(pDC, rect, _T(""), false, iFontSize, eGO_H, crText, crSelStart, crSelEnd);
	// 이미지 영역 계산
	CPoint2d ptC = rectImg.CenterPoint();
	int iImgW = img.GetWidth();
	int iImgH = img.GetHeight();
	rectImg.left = ptC.x - iImgW / 2.;
	rectImg.top = ptC.y - iImgH / 2. - 10;
	rectImg.right = rectImg.left + iImgW;
	rectImg.bottom = rectImg.top + iImgH - 10;

	// 배경 영역 계산 
	double dW = rectParcent.Width() / 3.;
	double dLeft = rectParcent.left + (dW * dSec);
	if (rectParcent.right - dLeft < 0.001)
		dLeft = rectParcent.right;
	rectParcent.left = dLeft;
	// Gradaation 방식 

	// 배경 출력

	// 버튼 눌렸을때 기본색의 영역을 점점 작게 만들면서 색상이 바뀌는 효과를 만듬 
	if (iSelect == iEnum) {							
		Draw3dGradationButton(pDC, rectParcent, _T(""), false, iFontSize, eGO_H, crText, crDefStart, crDefEnd);
	}
	// 마우스 가져다가 댔을 때 
	else if (iMouseover == iEnum) {					
		Draw3dGradationButton(pDC, rect, _T(""), false, iFontSize, eGO_H, crText, RGB_SKY, RGB_GRAY_DARK);
	}
	// 기본 Select / Mouse Over 가 아닌 경우 기본 색으로 뒤덮어줌
	else 
		Draw3dGradationButton(pDC, rect, _T(""), false, iFontSize, eGO_H, crText, crDefStart, crDefEnd);

	// 이미지 출력
	if (img)
		img.Draw(pDC->m_hDC, rectImg);

	// TEXT 출력
	rectText.top = rectImg.bottom + 2;
	CRect rectB(rectText);
	rectB.DeflateRect(1, 1, -1, -1);
	DrawTextB(pDC, rectB, szText, iFontSize, iBoder, RGB_BLACK, DT_CENTER);
	DrawTextB(pDC, rectText, szText, iFontSize, iBoder, crText, DT_CENTER);
}

void DrawGradationTimerImageLightButtonUpDownMouseMoveB(CDC* pDC, LPRECT rect, LPCTSTR szText, BOOL bCheck, double dSec, int iSelect, int iMouseover, int iEnum, int iFontSize, int iBoder,
	CImage& img, COLORREF crPen, COLORREF crText, COLORREF crBack, COLORREF crSelStart, COLORREF crSelEnd, COLORREF crDefStart, COLORREF crDefEnd, BOOL bTick)
{
	if (false == bCheck) {
		DrawGradationTimerImageButtonUpDownMouseMoveB(pDC, rect, szText, dSec, iSelect, iMouseover, iEnum, iFontSize, iBoder, img, crPen, crText, crBack, crSelStart, crSelEnd, crDefStart, crDefEnd);
		return;
	}

	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	CRect rectImg(rect);
	CRect rectText(rect);
	CRect rectParcent(rect);

	// 이미지 영역 계산
	CPoint2d ptC = rectImg.CenterPoint();
	int iImgW = img.GetWidth();
	int iImgH = img.GetHeight();
	rectImg.left = ptC.x - iImgW / 2.;
	rectImg.top = ptC.y - iImgH / 2. - 10;
	rectImg.right = rectImg.left + iImgW;
	rectImg.bottom = rectImg.top + iImgH - 10;


	// 마우스 가져다가 댔을 때 
	//if (iMouseover == iEnum) {
	//	Draw3dGradationButton(pDC, rect, _T(""), false, iFontSize, eGO_H, crText, RGB_SKY, RGB_GRAY_DARK);
	//}
	//// 시간에 따라서 점등
	//else {
	// 점등 아닐때 색상 RGB_GRAY_DARK
	// 점등 일때 색상 RGB_GREEN
	if (bTick) {
		Draw3dGradationButton(pDC, rect, _T(""), false, iFontSize, eGO_H, crText, RGB_GREEN, RGB_WHITE);
	}
	else {
		Draw3dGradationButton(pDC, rect, _T(""), false, iFontSize, eGO_H, crText, crDefStart, crDefEnd);
	}
	//}

	// 이미지 출력
	if (img)
		img.Draw(pDC->m_hDC, rectImg);

	// TEXT 출력
	rectText.top = rectImg.bottom + 2;
	CRect rectB(rectText);
	rectB.DeflateRect(1, 1, -1, -1);
	DrawTextB(pDC, rectB, szText, iFontSize, iBoder, RGB_BLACK, DT_CENTER);
	DrawTextB(pDC, rectText, szText, iFontSize, iBoder, crText, DT_CENTER);
}


void DrawInOutButton(CDC* pDC, LPRECT rect, LPCTSTR szText, int iFontSize, BOOL bIn, BOOL bOut) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;

	COLORREF crPen, crBack, crText;
	crPen = RGB_BLACK;
	crText = RGB_BLACK;
	crBack = RGB_GRAY_LIGHT;

	if (bIn && bOut)
		crBack = RGB_GREEN;
	else if (bIn)
		crBack = RGB_YELLOW;
	else if (bOut)
		crBack = RGB_ORANGE_LIGHT;

	SelectedDrawText(pDC, rect, szText, iFontSize, crPen, crBack, crText, DT_DT, DT_FONT);
}


void DrawGradationTimerButtonUpDownMouseMoveB(CDC* pDC, LPRECT rect, LPCTSTR szText, double dSec, int iSelect, int iMouseover, int iEnum, int iFontSize, int iBoder, COLORREF crPen, COLORREF crText, COLORREF crBack, COLORREF crSelStart, COLORREF crSelEnd, COLORREF crDefStart, COLORREF crDefEnd)
{
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	CRect rectImg(rect);
	CRect rectText(rect);
	CRect rectParcent(rect);

	TRIVERTEX vert[2];
	GRADIENT_RECT rectG;
	CRect rectT(rect);

	vert[0].x = rectT.left;
	vert[0].y = rectT.top;

	vert[1].x = rectT.right;
	vert[1].y = rectT.bottom;

	vert[0].Red = GetRValue(crSelStart) << 8;
	vert[0].Green = GetGValue(crSelStart) << 8;
	vert[0].Blue = GetBValue(crSelStart) << 8;
	vert[0].Alpha = 0x0000;

	vert[1].Red = GetRValue(crSelEnd) << 8;
	vert[1].Green = GetGValue(crSelEnd) << 8;
	vert[1].Blue = GetBValue(crSelEnd) << 8;
	vert[1].Alpha = 0x0000;

	rectG.UpperLeft = 0;
	rectG.LowerRight = 1;

	int iGradation = eGO_H;

	// 해당 버튼의 경우 선택한 색상을 가장 뒷부분에 깔아둠 
	GradientFill(pDC->m_hDC, vert, 2, &rectG, 1, iGradation);

	// 배경 영역 계산 
	double dW = rectParcent.Width() / 3.;
	double dLeft = rectParcent.left + dW * dSec;
	if (rectParcent.right - dLeft < 0.001)
		dLeft = rectParcent.right;
	rectParcent.left = dLeft;
	// Gradaation 방식 

	// 배경 출력

	// 버튼 눌렸을때 기본색의 영역을 점점 작게 만들면서 색상이 바뀌는 효과를 만듬 
	if (iSelect == iEnum) {
		Draw3dGradationButton(pDC, rectParcent, _T(""), true, iFontSize, eGO_H, crText, crDefStart, crDefEnd);
	}
	// 마우스 가져다가 댔을 때 
	else if (iMouseover == iEnum) {
		Draw3dGradationButton(pDC, rect, _T(""), false, iFontSize, eGO_H, crText, RGB_SKY, RGB_GRAY_DARK);
	}
	// 기본 Select / Mouse Over 가 아닌 경우 기본 색으로 뒤덮어줌
	else
		Draw3dGradationButton(pDC, rect, _T(""), false, iFontSize, eGO_H, crText, crDefStart, crDefEnd);

	// TEXT 출력
	CRect rectB(rectText);
	rectB.DeflateRect(1, 1, -1, -1);
	DrawTextB(pDC, rectB, szText, iFontSize, iBoder, RGB_BLACK, DT_CENTER|DT_VCENTER);
	DrawTextB(pDC, rectText, szText, iFontSize, iBoder, crText, DT_CENTER|DT_VCENTER);
}

void DrawProgressBar(CDC* pDC, LPRECT rect, int iDir, DWORD dwStart, int iMax, int iSpeed, COLORREF crPen, COLORREF crBack, COLORREF crPar) {
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	CRect rectAll(rect);
	CRect rectParcent(rect);

	CPen  pen(PS_SOLID, 1, crPen);
	CPen* pOldPen = pDC->SelectObject(&pen);
	if (dwStart) {
		
		DWORD dwTick = GetTickCount() - dwStart;
		DWORD dMaxTime = double(iMax / iSpeed)*1000;
		dwTick %=  (dMaxTime*2);
		double dParcent = ((double)dwTick / (double)dMaxTime)*100;
		double dParcent2 = dParcent;
		
		if (dParcent2 > 100) {
			dParcent2 -= 100;
		}
		double dValue = (double)rectParcent.Width() / ((double)iMax*0.5);
		
		
		if (iDir == 0)	// Left -> Right
		{
			rectParcent.right = rectParcent.left + (dParcent2* dValue);
		}
		else if (iDir == 1) // Right -> Left
		{
			rectParcent.left = rectParcent.right - (dParcent2* dValue);
		}
		if (dParcent >= 100) {
			pDC->FillSolidRect(rectAll, crBack);
			pDC->FillSolidRect(rectParcent, crPar);
		}
		else {
			pDC->FillSolidRect(rectAll, crPar);
			pDC->FillSolidRect(rectParcent, crBack);
		}
	}
	else 
		pDC->FillSolidRect(rectParcent, crBack);
	
	DrawLineRect(pDC, rectAll, crPen);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

void DrawGradationFlashTimerButton(CDC* pDC, LPRECT rect, LPCTSTR szText , DWORD dwStart, BOOL bClick, int iFontSize , int iBoder , CImage& img , COLORREF crPen , COLORREF crText , COLORREF crSelStart , COLORREF crSelEnd , COLORREF crDefStart , COLORREF crDefEnd, BOOL bSensor )
{
	if ((rect->left == rect->right) || (rect->top == rect->bottom))
		return;
	CRect rectImg(rect);
	CRect rectText(rect);
	CRect rectParcent(rect);

	// 배경 색상 출력 
	Draw3dGradationButton(pDC, rect, _T(""), FALSE, iFontSize, eGO_H, crText, crSelStart, crSelEnd);

	// 이미지 영역 계산
	CPoint2d ptC = rectImg.CenterPoint();
	int iImgW = img.GetWidth();
	int iImgH = img.GetHeight();
	rectImg.left = ptC.x - iImgW / 2.;
	rectImg.top = ptC.y - iImgH / 2. - 10;
	rectImg.right = rectImg.left + iImgW;
	rectImg.bottom = rectImg.top + iImgH - 10;

	// 배경 영역 계산 
	double dW = rectParcent.Width() / 3.;
	double dSec = (GetTickCount() - dwStart) / 1000.;
	if (dSec > 3) dSec = 3;
	double dLeft = rectParcent.left + (dW * dSec);
	if (fabs(rectParcent.right - dLeft) < 0.001)
		dLeft = rectParcent.right;
	rectParcent.left = dLeft;
	// Gradaation 방식 

	// 2차 배경 출력
	if (bSensor) {
		int iSec = ((GetTickCount() - dwStart) / 500.);
		if (iSec % 2) {
			if (bClick) {
				Draw3dGradationButton(pDC, rectParcent, _T(""), FALSE, iFontSize, eGO_H, crText, crDefStart, crDefEnd);
			}
			else {
				Draw3dGradationButton(pDC, rect, _T(""), FALSE, iFontSize, eGO_H, crText, RGB_GREEN, RGB_WHITE);
			}
		}
		else {
			if (bClick) {
				Draw3dGradationButton(pDC, rectParcent, _T(""), FALSE, iFontSize, eGO_H, crText, RGB_GREEN, RGB_WHITE);
			}
			else {
				Draw3dGradationButton(pDC, rect, _T(""), FALSE, iFontSize, eGO_H, crText, crDefStart, crDefEnd);
			}
		}
	}

	// 버튼 눌렸을때 기본색의 영역을 점점 작게 만들면서 색상이 바뀌는 효과를 만듬 
	else {
		if (bClick) {								
			Draw3dGradationButton(pDC, rectParcent, _T(""), FALSE, iFontSize, eGO_H, crText, crDefStart, crDefEnd);
		}
		else {
			Draw3dGradationButton(pDC, rect, _T(""), false, iFontSize, eGO_H, crText, crDefStart, crDefEnd);
		}
	}

	// 이미지 출력
	if (img)
		img.Draw(pDC->m_hDC, rectImg);

	// TEXT 출력
	rectText.top = rectImg.bottom + 2;
	CRect rectB(rectText);
	rectB.DeflateRect(1, 1, -1, -1);
	DrawTextB(pDC, rectB, szText, iFontSize, iBoder, RGB_BLACK, DT_CENTER);
	DrawTextB(pDC, rectText, szText, iFontSize, iBoder, crText, DT_CENTER);

}

void DrawImage(CDC* pDC, CRect rect, Gdiplus::Image* img) {
	if (img != nullptr) {
		Gdiplus::Graphics g(pDC->m_hDC);
		g.DrawImage(img, rect.left, rect.top, rect.Width(), rect.Height());
	}
}

void DrawMenuButton(CDC* pDC, CRect rect, LPCTSTR szText, int iSelect, int iMouseOver, int iEnum, int iFontSize, COLORREF crBkDefault, COLORREF crBkSelect, COLORREF crBkMouseOver, COLORREF crText, COLORREF crPen, DWORD dwAlign, LPCTSTR szFont)
{
	COLORREF bkColor = crBkDefault;
	if (iEnum == iSelect)
	{
		bkColor = crBkSelect;
	}
	else if (iEnum == iMouseOver)
	{
		bkColor = crBkMouseOver;
	}

	pDC->FillSolidRect(rect, bkColor);
	DrawLineRect(pDC, rect, crPen);
	// 3D 효과 - 삼성 요청
	Draw3DRectEffect(pDC, rect, iEnum == iSelect);
	
	DrawTextB(pDC, rect, szText, iFontSize, FW_HEAVY, crText, dwAlign, szFont);
}

void DrawMainToggleButton(CDC* pDC, CRect rect, bool bOn, LPCTSTR szOnText, LPCTSTR szOffText, int iSelect, int iMouseOver, int iEnum, int iFontSize, COLORREF crBkDefault, COLORREF crBkSelect, COLORREF crBkMouseOver, COLORREF crText, COLORREF crPen, DWORD dwAlign, LPCTSTR szFont)
{
	if (!bOn)
	{
		DrawMenuButton(pDC, rect, szOnText, iSelect, iMouseOver, iEnum, iFontSize, crBkDefault, crBkSelect, crBkMouseOver, crText, crPen, dwAlign, szFont);
	}
	else
	{
		DrawMenuButton(pDC, rect, szOffText, iSelect, iMouseOver, iSelect, iFontSize, crBkDefault, crBkSelect, crBkMouseOver, crText, crPen, dwAlign, szFont);
	}
}

void DrawTitleButton(CDC* pDC, CRect rect, LPCTSTR szText, CImage& img, int iSelect, int iMouseOver, int iEnum, int iFontSize, COLORREF crBkDefault, COLORREF crBkSelect, COLORREF crBkMouseOver, COLORREF crText, COLORREF crPen, DWORD dwAlign, LPCTSTR szFont)
{
	if (iEnum == iSelect)
	{
		pDC->FillSolidRect(rect, crBkSelect);
	}
	else if (iEnum == iMouseOver)
	{
		pDC->FillSolidRect(rect, crBkMouseOver);
	}
	else
	{
		pDC->FillSolidRect(rect, crBkDefault);
	}

	DrawLineRect(pDC, rect, crPen);
	// 3D 효과 - 삼성 요청
	Draw3DRectEffect(pDC, rect, iEnum == iSelect);

	// 이미지 그리기
	CRect rectImg(rect);
	CRect rectText(rect);
	if (img)
	{
		CPoint2d ptC = rectImg.CenterPoint();
		int iImgW = img.GetWidth();
		int iImgH = img.GetHeight();
		rectImg.left = ptC.x - iImgW / 2.;
		rectImg.top = ptC.y - iImgH / 2. - 10;
		rectImg.right = rectImg.left + iImgW;
		rectImg.bottom = rectImg.top + iImgH - 10;
		img.Draw(pDC->m_hDC, rectImg);

		rectText.top = rectImg.bottom + 2;
	}

	DrawTextB(pDC, rectText, szText, iFontSize, FW_HEAVY, crText, dwAlign, szFont);
}

void DrawTimeFillTitleButton(CDC* pDC, CRect rect, LPCTSTR szText, CImage& img, int iSelect, int iMouseOver, int iEnum, __uint64 currentTime, __uint64 completeTime, int iFontSize, COLORREF crBkDefault, COLORREF crBkSelectDefault, COLORREF crBkMouseOver, COLORREF crText, COLORREF crPen, DWORD dwAlign, LPCTSTR szFont)
{
	if (iEnum == iSelect)
	{
		pDC->FillSolidRect(rect, crBkDefault);

		// 누른 시간에 따라 커지는 Rect
		CRect rectTime(rect);
		if (currentTime <= completeTime)
		{
			double dSecondForPixel = rectTime.Width() / (double)completeTime; // 시간당 픽셀
			double dCurrentPixel = dSecondForPixel * currentTime;
			rectTime.right = rectTime.left + dCurrentPixel;
		}
		pDC->FillSolidRect(rectTime, crBkSelectDefault);
	}
	else if (iEnum == iMouseOver)
	{
		pDC->FillSolidRect(rect, crBkMouseOver);
	}
	else
	{
		pDC->FillSolidRect(rect, crBkDefault);
	}

	DrawLineRect(pDC, rect, crPen);

	// 3D 효과 - 삼성 요청
	Draw3DRectEffect(pDC, rect, iEnum == iSelect);

	// 이미지 그리기
	CRect rectImg(rect);
	CRect rectText(rect);
	if (img)
	{
		CPoint2d ptC = rectImg.CenterPoint();
		int iImgW = img.GetWidth();
		int iImgH = img.GetHeight();
		rectImg.left = ptC.x - iImgW / 2.;
		rectImg.top = ptC.y - iImgH / 2. - 10;
		rectImg.right = rectImg.left + iImgW;
		rectImg.bottom = rectImg.top + iImgH - 10;
		img.Draw(pDC->m_hDC, rectImg);
		
		rectText.top = rectImg.bottom + 2;
	}

	DrawTextB(pDC, rectText, szText, iFontSize, FW_HEAVY, crText, dwAlign, szFont);
}

void DrawBlinkTilteButton(CDC* pDC, CRect rect, LPCTSTR szText, CImage& img, __uint64 startTime, __uint64 blinkTime, int iFontSize, COLORREF crBkDefault, COLORREF crBkBlink, COLORREF crText, COLORREF crPen, DWORD dwAlign, LPCTSTR szFont)
{
	bool bOnOff = ((GetTickCount64() - startTime) / blinkTime) % 2 == 0;

	if (bOnOff)
	{
		pDC->FillSolidRect(rect, crBkBlink);
	}
	else
	{
		pDC->FillSolidRect(rect, crBkDefault);
	}

	DrawLineRect(pDC, rect, crPen);
	// 3D 효과 - 삼성 요청
	Draw3DRectEffect(pDC, rect, true);

	// 이미지 그리기
	CRect rectImg(rect);
	CRect rectText(rect);
	if (img)
	{
		CPoint2d ptC = rectImg.CenterPoint();
		int iImgW = img.GetWidth();
		int iImgH = img.GetHeight();
		rectImg.left = ptC.x - iImgW / 2.;
		rectImg.top = ptC.y - iImgH / 2. - 10;
		rectImg.right = rectImg.left + iImgW;
		rectImg.bottom = rectImg.top + iImgH - 10;
		img.Draw(pDC->m_hDC, rectImg);

		rectText.top = rectImg.bottom + 2;
	}

	DrawTextB(pDC, rectText, szText, iFontSize, FW_HEAVY, crText, dwAlign, szFont);
}

// 완벽히 작동 중 - 점등
// 비정상 작동 중 - 점멸
// 비동작 - 기본
// 어떠한 상태에서든 시간 경과에 따른 그라데이션 동작이 되어야 함
// ex) Servo On 점멸 상태에서 Servo On 동작까지 그라데이션 효과 이후에 점등해야함
//		작동상태, 점멸 시간, 마우스 버튼 누른 시간, 버튼 완료까지 시간, 점등 배경색, 그라데이션 효과색 2개
// 
// ex) 투입정지 점등 상태에서 투입정지 해제까지 그라데이션 효과 이후 기본상태로 바뀌어야함
void DrawBlinkTimeFillTilteButton(CDC* pDC, CRect rect, LPCTSTR szText, CImage& img, int iSelect, int iMouseOver, int iEnum, int iFontSize, bool bOn, bool bOnOff, __uint64 currentTime, __uint64 completeTime, __uint64 startTime, __uint64 blinkTime, COLORREF crBkDefault, COLORREF crBkSelectDefault, COLORREF crBkMouseOver, COLORREF crText, COLORREF crPen, DWORD dwAlign, LPCTSTR szFont)
{
	if (bOn && iSelect != iEnum)
	{
		DrawTitleButton(pDC, rect, szText, img, 0, iMouseOver, 0, iFontSize, crBkSelectDefault, crBkSelectDefault, crBkSelectDefault, crText, crPen, dwAlign, szFont);
	}
	else if (bOnOff && iSelect != iEnum)
	{
		DrawBlinkTilteButton(pDC, rect, szText, img, startTime, blinkTime, iFontSize, crBkDefault, crBkSelectDefault, crText, crPen, dwAlign, szFont);
	}
	else
	{
		DrawTimeFillTitleButton(pDC, rect, szText, img, iSelect, iMouseOver, iEnum, currentTime, completeTime, iFontSize, crBkDefault, crBkSelectDefault, crBkMouseOver, crText, crPen, dwAlign, szFont);
	}
}

void Draw3DRectEffect(CDC* pDC, CRect& rect, bool bSelect)
{
	constexpr unsigned int EffectSize = 2;
	rect.DeflateRect(EffectSize, EffectSize);

	// 버튼 다운 상태
	if (bSelect)
	{
		// Left Top
		DrawLine(pDC, CPoint(rect.left, rect.bottom), CPoint(rect.left, rect.top), RGB_BLACK, EffectSize);
		DrawLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.right, rect.top), RGB_BLACK, EffectSize);
		
		// Right Bottom
		DrawLine(pDC, CPoint(rect.right, rect.top), CPoint(rect.right, rect.bottom), RGB_WHITE, EffectSize);
		DrawLine(pDC, CPoint(rect.right, rect.bottom), CPoint(rect.left, rect.bottom), RGB_WHITE, EffectSize);
	}
	// 버튼 업 상태
	else
	{
		//Left Top
		DrawLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.right, rect.top), RGB_WHITE, EffectSize);
		DrawLine(pDC, CPoint(rect.left, rect.bottom), CPoint(rect.left, rect.top), RGB_WHITE, EffectSize);
		// Right Bottom
		DrawLine(pDC, CPoint(rect.right, rect.top), CPoint(rect.right, rect.bottom), RGB_BLACK, EffectSize);
		DrawLine(pDC, CPoint(rect.right, rect.bottom), CPoint(rect.left, rect.bottom), RGB_BLACK, EffectSize);
	}
}

void DrawMDIMenuButton(CDC* pDC, CRect rect, LPCTSTR szText, bool bSelected, bool bMouseOver, int iFontSize, COLORREF crBkDefault, COLORREF crBkSelect, int iBoder, COLORREF crText, DWORD dwAlign, LPCTSTR szFont)
{
	COLORREF borderColor = bSelected || bMouseOver ? RGB_GRAY_LIGHT : RGB_GRAY;

	// 기본 Rect 색상 및 테두리
	pDC->FillSolidRect(rect, borderColor);
	DrawLineRect(pDC, rect, bMouseOver ? RGB_BLUE : RGB_WHITE);
	
	// 내부 Round Rect
	CRect roundRect = rect;
	roundRect.DeflateRect(2, 2);

	COLORREF selectColor = bSelected ? crBkSelect : crBkDefault;
	// Rect 내부 그라데이션
	{
		TRIVERTEX vert[2];
		GRADIENT_RECT rectG;
		CRect rectT(roundRect);

		vert[0].x = rectT.left;
		vert[0].y = rectT.top;

		vert[1].x = rectT.right;
		vert[1].y = rectT.bottom;
		auto brightnessColor = [](int color) -> int
		{
			int newColor = color + color / 3.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[0].Red = brightnessColor(GetRValue(selectColor)) << 8;
		vert[0].Green = brightnessColor(GetGValue(selectColor)) << 8;
		vert[0].Blue = brightnessColor(GetBValue(selectColor)) << 8;
		vert[0].Alpha = 0x0000;

		auto darknessColor = [](int color) -> int
		{
			int newColor = color - color / 7.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[1].Red = darknessColor(GetRValue(selectColor)) << 8;
		vert[1].Green = darknessColor(GetGValue(selectColor)) << 8;
		vert[1].Blue = darknessColor(GetBValue(selectColor)) << 8;
		vert[1].Alpha = 0x0000;

		rectG.UpperLeft = 0;
		rectG.LowerRight = 1;
		GradientFill(pDC->m_hDC, vert, 2, &rectG, 1, eGO_V);
	}

	// 라운드 테두리
	{
		CPen pen(PS_SOLID, 1, bSelected ? RGB_WHITE : RGB_GRAY);
		CBrush* pOldBrush;
		CPen* pOldPen;
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&pen);
		pDC->RoundRect(roundRect, {10, 10});
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

		pen.DeleteObject();
	}
	// 외부 그라데이션 삭제
	{
		CPen pen(PS_SOLID, 1, borderColor);
		CBrush* pOldBrush;
		CPen* pOldPen;
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&pen);
	
		roundRect.InflateRect(1, 1);
		pDC->RoundRect(roundRect, { 10, 10 });

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

		pen.DeleteObject();
	}

	if (iBoder == FW_HEAVY)
	{
		DrawTextB(pDC, roundRect, szText, iFontSize, FW_HEAVY, crText, dwAlign, szFont);
	}
	else
	{
		DrawText(pDC, roundRect, szText, iFontSize, crText, dwAlign, szFont);
	}
}

void DrawMDIMenuButtonN(CDC* pDC, CRect rect, LPCTSTR szText, bool bSelected, bool bMouseOver, int iFontSize, COLORREF crBkDefault, COLORREF crBkSelect, int iBoder, COLORREF crText, DWORD dwAlign, LPCTSTR szFont)
{
	DrawMDIMenuButton(pDC, rect, szText, bSelected, bMouseOver, iFontSize, crBkDefault, crBkSelect, iBoder, crText, dwAlign, szFont);
}

void DrawMDIImageButton(CDC* pDC, CRect rect, LPCTSTR szText, Gdiplus::Image* img, bool bSelected, bool bMouseOver, int iFontSize, COLORREF crBkDefault, COLORREF crBkSelect, COLORREF crText, DWORD dwAlign, LPCTSTR szFont)
{
	COLORREF borderColor = bSelected || bMouseOver ? RGB_GRAY_LIGHT : RGB_GRAY;

	// 기본 Rect 색상 및 테두리
	pDC->FillSolidRect(rect, borderColor);
	DrawLineRect(pDC, rect, bMouseOver ? RGB_BLUE : RGB_WHITE);

	// 내부 Round Rect
	CRect roundRect = rect;
	roundRect.DeflateRect(2, 2);

	COLORREF selectColor = bSelected ? crBkSelect : crBkDefault;
	// Rect 내부 그라데이션
	{
		TRIVERTEX vert[2];
		GRADIENT_RECT rectG;
		CRect rectT(roundRect);

		vert[0].x = rectT.left;
		vert[0].y = rectT.top;

		vert[1].x = rectT.right;
		vert[1].y = rectT.bottom;
		auto brightnessColor = [](int color) -> int
		{
			int newColor = color + color / 3.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[0].Red = brightnessColor(GetRValue(selectColor)) << 8;
		vert[0].Green = brightnessColor(GetGValue(selectColor)) << 8;
		vert[0].Blue = brightnessColor(GetBValue(selectColor)) << 8;
		vert[0].Alpha = 0x0000;

		auto darknessColor = [](int color) -> int
		{
			int newColor = color - color / 7.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[1].Red = darknessColor(GetRValue(selectColor)) << 8;
		vert[1].Green = darknessColor(GetGValue(selectColor)) << 8;
		vert[1].Blue = darknessColor(GetBValue(selectColor)) << 8;
		vert[1].Alpha = 0x0000;

		rectG.UpperLeft = 0;
		rectG.LowerRight = 1;
		GradientFill(pDC->m_hDC, vert, 2, &rectG, 1, eGO_V);
	}

	// 라운드 테두리
	{
		CPen pen(PS_SOLID, 1, bSelected ? RGB_WHITE : RGB_GRAY);
		CBrush* pOldBrush;
		CPen* pOldPen;
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&pen);
		pDC->RoundRect(roundRect, { 10, 10 });
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

		pen.DeleteObject();
	}
	// 외부 그라데이션 삭제
	{
		CPen pen(PS_SOLID, 1, borderColor);
		CBrush* pOldBrush;
		CPen* pOldPen;
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&pen);

		roundRect.InflateRect(1, 1);
		pDC->RoundRect(roundRect, { 10, 10 });

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

		pen.DeleteObject();
	}


	// 이미지 그리기
	CRect rectImg(roundRect);
	CRect rectText(roundRect);
	if (img != nullptr)
	{
		CPoint2d ptC = rectImg.CenterPoint();
		int iImgW = img->GetWidth();
		int iImgH = img->GetHeight();
		rectImg.left = ptC.x - iImgW / 2.;
		rectImg.top = ptC.y - iImgH / 2. - 10;
		rectImg.right = rectImg.left + iImgW;
		rectImg.bottom = rectImg.top + iImgH - 10;
		Gdiplus::Graphics g(pDC->m_hDC);
		g.DrawImage(img, rectImg.left, rectImg.top, rectImg.Width(), rectImg.Height());

		rectText.top = rectImg.bottom + 2;
	}

	//DrawText(pDC, rectText, szText, iFontSize, crText, dwAlign, szFont);
	DrawTextB(pDC, rectText, szText, iFontSize, FW_HEAVY, crText, dwAlign, szFont);
	//DrawText(pDC, roundRect, szText, iFontSize, crText, dwAlign, szFont);
}

void DrawMDIImageTimeFillButton(CDC* pDC, CRect rect, LPCTSTR szText, Gdiplus::Image* img, bool bOn, bool bSelected, bool bMouseOver, __uint64 currentTime, __uint64 completeTime, int iFontSize, COLORREF crBkDefault, COLORREF crBkSelect, COLORREF crText, DWORD dwAlign, LPCTSTR szFont)
{
	COLORREF borderColor = bSelected || bMouseOver ? RGB_GRAY_LIGHT : RGB_GRAY;

	// 기본 Rect 색상 및 테두리
	pDC->FillSolidRect(rect, borderColor);
	DrawLineRect(pDC, rect, bMouseOver ? RGB_BLUE : RGB_WHITE);

	// 내부 Round Rect
	CRect roundRect = rect;
	roundRect.DeflateRect(2, 2);

	COLORREF selectColor = bOn ? crBkSelect : crBkDefault;
	// bOn && !bSelected = select
	// !bOn && bSelected = select
	// !bOn && !bSelected = default
	// bOn && bSelected = default

	// Rect 내부 그라데이션
	{
		TRIVERTEX vert[2];
		GRADIENT_RECT rectG;
		CRect rectT(roundRect);

		vert[0].x = rectT.left;
		vert[0].y = rectT.top;

		vert[1].x = rectT.right;
		vert[1].y = rectT.bottom;
		auto brightnessColor = [](int color) -> int
		{
			int newColor = color + color / 3.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[0].Red = brightnessColor(GetRValue(selectColor)) << 8;
		vert[0].Green = brightnessColor(GetGValue(selectColor)) << 8;
		vert[0].Blue = brightnessColor(GetBValue(selectColor)) << 8;
		vert[0].Alpha = 0x0000;

		auto darknessColor = [](int color) -> int
		{
			int newColor = color - color / 7.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[1].Red = darknessColor(GetRValue(selectColor)) << 8;
		vert[1].Green = darknessColor(GetGValue(selectColor)) << 8;
		vert[1].Blue = darknessColor(GetBValue(selectColor)) << 8;
		vert[1].Alpha = 0x0000;

		rectG.UpperLeft = 0;
		rectG.LowerRight = 1;
		GradientFill(pDC->m_hDC, vert, 2, &rectG, 1, eGO_V);
	}

	// 차오르는 그라데이션 표현
	COLORREF timeFillColor = selectColor == crBkSelect ? crBkDefault : crBkSelect;
	if (bSelected)
	{
		TRIVERTEX vert[2];
		GRADIENT_RECT rectG;

		CRect rectT(roundRect);

		// 누른 시간에 따라 커지는 Rect
		if (currentTime <= completeTime)
		{
			double dSecondForPixel = rectT.Width() / (double)completeTime; // 시간당 픽셀
			double dCurrentPixel = dSecondForPixel * currentTime;
			rectT.right = rectT.left + dCurrentPixel;
		}

		vert[0].x = rectT.left;
		vert[0].y = rectT.top;

		vert[1].x = rectT.right;
		vert[1].y = rectT.bottom;
		auto brightnessColor = [](int color) -> int
		{
			int newColor = color + color / 3.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[0].Red = brightnessColor(GetRValue(timeFillColor)) << 8;
		vert[0].Green = brightnessColor(GetGValue(timeFillColor)) << 8;
		vert[0].Blue = brightnessColor(GetBValue(timeFillColor)) << 8;
		vert[0].Alpha = 0x0000;

		auto darknessColor = [](int color) -> int
		{
			int newColor = color - color / 7.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[1].Red = darknessColor(GetRValue(timeFillColor)) << 8;
		vert[1].Green = darknessColor(GetGValue(timeFillColor)) << 8;
		vert[1].Blue = darknessColor(GetBValue(timeFillColor)) << 8;
		vert[1].Alpha = 0x0000;

		rectG.UpperLeft = 0;
		rectG.LowerRight = 1;
		GradientFill(pDC->m_hDC, vert, 2, &rectG, 1, eGO_V);
	}	

	// 라운드 테두리
	{
		CPen pen(PS_SOLID, 1, bSelected ? RGB_WHITE : RGB_GRAY);
		CBrush* pOldBrush;
		CPen* pOldPen;
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&pen);
		pDC->RoundRect(roundRect, { 10, 10 });
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}
	// 외부 그라데이션 삭제
	{
		CPen pen(PS_SOLID, 1, borderColor);
		CBrush* pOldBrush;
		CPen* pOldPen;
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&pen);

		roundRect.InflateRect(1, 1);
		pDC->RoundRect(roundRect, { 10, 10 });

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}


	// 이미지 그리기
	CRect rectImg(roundRect);
	CRect rectText(roundRect);
	if (img != nullptr)
	{
		CPoint2d ptC = rectImg.CenterPoint();
		int iImgW = img->GetWidth();
		int iImgH = img->GetHeight();
		rectImg.left = ptC.x - iImgW / 2.;
		rectImg.top = ptC.y - iImgH / 2. - 10;
		rectImg.right = rectImg.left + iImgW;
		rectImg.bottom = rectImg.top + iImgH - 10;

		Gdiplus::Graphics g(pDC->m_hDC);
		g.DrawImage(img, rectImg.left, rectImg.top, rectImg.Width(), rectImg.Height());

		rectText.top = rectImg.bottom + 2;
	}

	DrawTextB(pDC, rectText, szText, iFontSize, FW_HEAVY, crText, dwAlign, szFont);
}

void DrawMDIImageBlinkButton(CDC* pDC, CRect rect, LPCTSTR szText, Gdiplus::Image* img, bool bSelected, bool bMouseOver, __uint64 startTime, __uint64 blinkTime, int iFontSize, COLORREF crBkDefault, COLORREF crBkSelect, COLORREF crText, DWORD dwAlign, LPCTSTR szFont)
{
	COLORREF borderColor = bSelected || bMouseOver ? RGB_GRAY_LIGHT : RGB_GRAY;

	// 기본 Rect 색상 및 테두리
	pDC->FillSolidRect(rect, borderColor);
	DrawLineRect(pDC, rect, bMouseOver ? RGB_BLUE : RGB_WHITE);

	// 내부 Round Rect
	CRect roundRect = rect;
	roundRect.DeflateRect(2, 2);

	bool bOnOff = ((GetTickCount64() - startTime) / blinkTime) % 2 == 0;

	COLORREF selectColor = bOnOff ? crBkSelect : crBkDefault;
	// Rect 내부 그라데이션
	{
		TRIVERTEX vert[2];
		GRADIENT_RECT rectG;
		CRect rectT(roundRect);

		vert[0].x = rectT.left;
		vert[0].y = rectT.top;

		vert[1].x = rectT.right;
		vert[1].y = rectT.bottom;
		auto brightnessColor = [](int color) -> int
		{
			int newColor = color + color / 3.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[0].Red = brightnessColor(GetRValue(selectColor)) << 8;
		vert[0].Green = brightnessColor(GetGValue(selectColor)) << 8;
		vert[0].Blue = brightnessColor(GetBValue(selectColor)) << 8;
		vert[0].Alpha = 0x0000;

		auto darknessColor = [](int color) -> int
		{
			int newColor = color - color / 7.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[1].Red = darknessColor(GetRValue(selectColor)) << 8;
		vert[1].Green = darknessColor(GetGValue(selectColor)) << 8;
		vert[1].Blue = darknessColor(GetBValue(selectColor)) << 8;
		vert[1].Alpha = 0x0000;

		rectG.UpperLeft = 0;
		rectG.LowerRight = 1;
		GradientFill(pDC->m_hDC, vert, 2, &rectG, 1, eGO_V);
	}

	// 라운드 테두리
	{
		CPen pen(PS_SOLID, 1, bSelected ? RGB_WHITE : RGB_GRAY);
		CBrush* pOldBrush;
		CPen* pOldPen;
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&pen);
		pDC->RoundRect(roundRect, { 10, 10 });
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}
	// 외부 그라데이션 삭제
	{
		CPen pen(PS_SOLID, 1, borderColor);
		CBrush* pOldBrush;
		CPen* pOldPen;
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&pen);

		roundRect.InflateRect(1, 1);
		pDC->RoundRect(roundRect, { 10, 10 });

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}


	// 이미지 그리기
	CRect rectImg(roundRect);
	CRect rectText(roundRect);
	if (img != nullptr)
	{
		CPoint2d ptC = rectImg.CenterPoint();
		int iImgW = img->GetWidth();
		int iImgH = img->GetHeight();
		rectImg.left = ptC.x - iImgW / 2.;
		rectImg.top = ptC.y - iImgH / 2. - 10;
		rectImg.right = rectImg.left + iImgW;
		rectImg.bottom = rectImg.top + iImgH - 10;
		
		Gdiplus::Graphics g(pDC->m_hDC);
		g.DrawImage(img, rectImg.left, rectImg.top, rectImg.Width(), rectImg.Height());

		rectText.top = rectImg.bottom + 2;
	}

	DrawTextB(pDC, rectText, szText, iFontSize, FW_HEAVY, crText, dwAlign, szFont);
}

void DrawMDIImageBlinkFillButton(CDC* pDC, CRect rect, LPCTSTR szText, Gdiplus::Image* img, bool bOn, bool bOnOff, bool bSelected, bool bMouseOver, __uint64 currentTime, __uint64 completeTime, __uint64 startTime, __uint64 blinkTime, int iFontSize, COLORREF crBkDefault, COLORREF crBkSelect, COLORREF crText, DWORD dwAlign, LPCTSTR szFont)
{
	if (bSelected)
	{
		DrawMDIImageTimeFillButton(pDC, rect, szText, img, bOn, bSelected, bMouseOver, currentTime, completeTime, iFontSize, crBkDefault, crBkSelect, crText, dwAlign, szFont);
	}
	else if (bOn)
	{
		DrawMDIImageButton(pDC, rect, szText, img, true, bMouseOver, iFontSize, crBkDefault, crBkSelect, crText, dwAlign, szFont);
	}
	else if (bOnOff)
	{
		DrawMDIImageBlinkButton(pDC, rect, szText, img, false, bMouseOver, startTime, blinkTime, iFontSize, crBkDefault, crBkSelect, crText, dwAlign, szFont);
	}
	else
	{
		DrawMDIImageButton(pDC, rect, szText, img, false, bMouseOver, iFontSize, crBkDefault, crBkSelect, crText, dwAlign, szFont);
	}
}

void DrawMDIIOButton(CDC* pDC, CRect rect, LPCTSTR szText, bool bSelected, bool bMouseHover, bool bOn, bool bWrite)
{
	COLORREF borderColor = bSelected || bMouseHover ? RGB_GRAY_LIGHT : RGB_GRAY;

	// 기본 Rect 색상 및 테두리
	pDC->FillSolidRect(rect, borderColor);
	DrawLineRect(pDC, rect, bMouseHover ? RGB_BLUE : RGB_WHITE);

	// 내부 Round Rect
	CRect roundRect = rect;
	roundRect.DeflateRect(2, 2);

	COLORREF backColor = RGB_GRAY_DARK, textColor = RGB_WHITE;
	if (bSelected)
	{
		backColor = RGB_GRAY_LIGHT;
		textColor = RGB_BLACK;
	}
	else if (bOn)
	{
		backColor = RGB_GREEN_DARK;
	}
	else if (bWrite)
	{
		backColor = RGB_RED_DARK;
	}

	// Rect 내부 그라데이션
	{
		TRIVERTEX vert[2];
		GRADIENT_RECT rectG;
		CRect rectT(roundRect);

		vert[0].x = rectT.left;
		vert[0].y = rectT.top;

		vert[1].x = rectT.right;
		vert[1].y = rectT.bottom;
		auto brightnessColor = [](int color) -> int
		{
			int newColor = color + color / 3.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[0].Red = brightnessColor(GetRValue(backColor)) << 8;
		vert[0].Green = brightnessColor(GetGValue(backColor)) << 8;
		vert[0].Blue = brightnessColor(GetBValue(backColor)) << 8;
		vert[0].Alpha = 0x0000;

		auto darknessColor = [](int color) -> int
		{
			int newColor = color - color / 7.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[1].Red = darknessColor(GetRValue(backColor)) << 8;
		vert[1].Green = darknessColor(GetGValue(backColor)) << 8;
		vert[1].Blue = darknessColor(GetBValue(backColor)) << 8;
		vert[1].Alpha = 0x0000;

		rectG.UpperLeft = 0;
		rectG.LowerRight = 1;
		GradientFill(pDC->m_hDC, vert, 2, &rectG, 1, eGO_V);
	}

	// 라운드 테두리
	{
		CPen pen(PS_SOLID, 1, bSelected ? RGB_WHITE : RGB_GRAY);
		CBrush* pOldBrush;
		CPen* pOldPen;
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&pen);
		pDC->RoundRect(roundRect, { 10, 10 });
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}
	// 외부 그라데이션 삭제
	{
		CPen pen(PS_SOLID, 1, borderColor);
		CBrush* pOldBrush;
		CPen* pOldPen;
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&pen);

		roundRect.InflateRect(1, 1);
		pDC->RoundRect(roundRect, { 10, 10 });

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}
	
	DrawText(pDC, roundRect, szText, DT_FONTSIZE, textColor, DT_VCENTER | DT_CENTER);
}

void DrawMDIMoveDirect(CDC* pDC, CRect rect, bool bSelected, bool bMouseHover, eMOVE_DIRECT eDirect, LPCTSTR szText)
{
	COLORREF borderColor = bSelected || bMouseHover ? RGB_GRAY_LIGHT : RGB_GRAY;

	// 기본 Rect 색상 및 테두리
	pDC->FillSolidRect(rect, borderColor);
	DrawLineRect(pDC, rect, bMouseHover ? RGB_BLUE : RGB_WHITE);

	// 내부 Round Rect
	CRect roundRect = rect;
	roundRect.DeflateRect(2, 2);

	COLORREF backColor = bSelected ? RGB_ORANGE_LIGHT : RGB_GRAY_LIGHT, textColor = RGB_BLACK;

	// Rect 내부 그라데이션
	{
		TRIVERTEX vert[2];
		GRADIENT_RECT rectG;
		CRect rectT(roundRect);

		vert[0].x = rectT.left;
		vert[0].y = rectT.top;

		vert[1].x = rectT.right;
		vert[1].y = rectT.bottom;
		auto brightnessColor = [](int color) -> int
		{
			int newColor = color + color / 3.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[0].Red = brightnessColor(GetRValue(backColor)) << 8;
		vert[0].Green = brightnessColor(GetGValue(backColor)) << 8;
		vert[0].Blue = brightnessColor(GetBValue(backColor)) << 8;
		vert[0].Alpha = 0x0000;

		auto darknessColor = [](int color) -> int
		{
			int newColor = color - color / 7.;
			if (newColor >= 255) newColor = 255;
			if (newColor < 0) newColor = 0;
			return newColor;
		};

		vert[1].Red = darknessColor(GetRValue(backColor)) << 8;
		vert[1].Green = darknessColor(GetGValue(backColor)) << 8;
		vert[1].Blue = darknessColor(GetBValue(backColor)) << 8;
		vert[1].Alpha = 0x0000;

		rectG.UpperLeft = 0;
		rectG.LowerRight = 1;
		GradientFill(pDC->m_hDC, vert, 2, &rectG, 1, eGO_V);
	}

	// 라운드 테두리
	{
		CPen pen(PS_SOLID, 1, bSelected ? RGB_WHITE : RGB_GRAY);
		CBrush* pOldBrush;
		CPen* pOldPen;
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&pen);
		pDC->RoundRect(roundRect, { 10, 10 });
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}
	// 외부 그라데이션 삭제
	{
		CPen pen(PS_SOLID, 1, borderColor);
		CBrush* pOldBrush;
		CPen* pOldPen;
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&pen);

		roundRect.InflateRect(1, 1);
		pDC->RoundRect(roundRect, { 10, 10 });

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}

	CRect rectMove(rect);
	double dW, dH;

	dW = (rect.Width() / 10.);
	dH = (rect.Height() / 10.);
	int iL, iT, iR, iB;

	switch (eDirect) {
	case eMD_UP: iL = -dW; iT = -dH * 1.5; iR = -dW; iB = -dH * 2; break;
	case eMD_LEFT: iL = -dW * 2; iT = -dH; iR = -dW * 1.5; iB = -dH; break;
	case eMD_RIGHT:	iL = -dW * 1.5; iT = -dH; iR = -dW * 2; iB = -dH; break;
	case eMD_DOWN: iL = -dW; iT = -dH * 2; iR = -dW; iB = -dH * 1.5; break;
	}

	rectMove.InflateRect(iL, iT, iR, iB);

	CPoint ptLT(rectMove.TopLeft());
	CPoint ptLC(rectMove.left, rectMove.CenterPoint().y);
	CPoint ptLB(rectMove.left, rectMove.bottom);

	CPoint ptRT(rectMove.right, rectMove.top);
	CPoint ptRC(rectMove.right, rectMove.CenterPoint().y);
	CPoint ptRB(rectMove.right, rectMove.bottom);

	CPoint ptCT(rectMove.CenterPoint().x, rectMove.top);
	CPoint ptCB(rectMove.CenterPoint().x, rectMove.bottom);

	POINT pt[3];

	switch (eDirect) {
	case eMD_UP:		pt[0] = ptLB; pt[1] = ptCT; pt[2] = ptRB; break;
	case eMD_LEFT:		pt[0] = ptLC; pt[1] = ptRT; pt[2] = ptRB; break;
	case eMD_RIGHT:		pt[0] = ptLT; pt[1] = ptRC; pt[2] = ptLB; break;
	case eMD_DOWN:		pt[0] = ptLT; pt[1] = ptRT; pt[2] = ptCB; break;
	}

	CPen* pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);;
	CBrush brush(RGB(0, 191, 255));
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->Polygon(pt, 3);

	CRect rectText(rectMove);
	switch (eDirect) {
		case eMD_UP :		DrawText(pDC, rectText, szText, DT_FONTSIZE, textColor, DT_VCENTER | DT_CENTER );	break;
		case eMD_LEFT :		DrawText(pDC, rectText, szText, DT_FONTSIZE, textColor, DT_VCENTER | DT_RIGHT);		break;
		case eMD_RIGHT :	DrawText(pDC, rectText, szText, DT_FONTSIZE, textColor, DT_VCENTER | DT_LEFT);		break;
		case eMD_DOWN :		DrawText(pDC, rectText, szText, DT_FONTSIZE, textColor, DT_VCENTER | DT_CENTER );	break;
	}
	
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	brush.DeleteObject();
}

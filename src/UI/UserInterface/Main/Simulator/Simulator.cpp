// Simulator.cpp : implementation file
//

#include "pch.h"
#include "UI.h"
#include "Simulator.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;
// CSimulator

IMPLEMENT_DYNAMIC(CSimulator, CWnd)
CSimulator::CSimulator()
{
	memset(&m_tMouseEvent , 0x00, sizeof(m_tMouseEvent));
	Gdiplus::GdiplusStartupInput gdiInput;
	Gdiplus::GdiplusStartup(&m_GdiplusToken, &gdiInput, NULL);
	
}

CSimulator::~CSimulator()
{
	Gdiplus::GdiplusShutdown(m_GdiplusToken);
}


BEGIN_MESSAGE_MAP(CSimulator, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



// CSimulator message handlers




int CSimulator::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_tLine.iWidth = 30;
	m_tLine.iHeight = 30;
	m_tGantry.iSize = 40;

	CRect rcArea = CRect(0, 0, 1423, 367);
	m_ct.SetTransformMatrixT(1., M_PI, rcArea.CenterPoint().x, rcArea.CenterPoint().y, rcArea.CenterPoint().x, rcArea.CenterPoint().y);

	return 0;
}
BOOL CSimulator::DeleteAllModule(){
	m_vctModule.clear();
	m_iVector = m_vctModule.size();
	return m_vctModule.size() == 0;
}
BOOL CSimulator::DeleteModule(int iModule){
	BOOL bReturn = FALSE;

	if( m_vctModule.size() > iModule){
		m_vctModule.erase(m_vctModule.begin()+iModule);
		bReturn = TRUE;
	}
	m_iVector = m_vctModule.size();
	return bReturn;
}
BOOL CSimulator::SetSelectModule(int iModule){

	BOOL bReturn = FALSE;
	
	

	if( m_vctModule.size() > iModule){
		if(m_iOldSelect >= 0){
			m_vctModule[m_iOldSelect].bSelect = FALSE;
			
		}
		m_vctModule[iModule].bSelect = TRUE;
		bReturn = TRUE; 
		m_iOldSelect = iModule;
	}
	if( iModule  < 0){
		m_iOldSelect = -1;
		for(int i =0 ; i < m_vctModule.size() ; i++){
			m_vctModule[i].bSelect = FALSE;
		}
	}
	
	return bReturn;

}

void CSimulator::SetMouseEnvet(eMOUSE_EVENT eMouseEvnet, int iAxis) { 
	m_tMouseEvent.eME = eMouseEvnet;
	m_tMouseEvent.iAxisIndex = iAxis;
}
int CSimulator::GetModule(){
	return m_iVector;
}
int CSimulator::AddModule(double dMax, double dMin, CRect Rect, int eType, BOOL bVet){
	T_SIMULATOR module;
	
	module.dMax = dMax;
	module.dMin = dMin;
	module.dPos = 0.;
	module.rectBound = Rect;
	module.eType = eType;
	module.bVet = bVet;
	module.iBaseModule	= -1;
	module.bOn	= FALSE;
	module.sizeTable = CSize();
	module.stType = ST_C;
	
	module.bSelect = FALSE;
	module.bDrawGideStage = TRUE;
	module.stLinkType = ST_C;
	module.sizeRect = CSize(Rect.Width(), Rect.Height());
	
	m_vctModule.push_back(module);
	m_iVector = m_vctModule.size();
	m_vctModule[m_iVector - 1].iViewIndex = m_iVector - 1;
	return m_vctModule.size();
}
void CSimulator::SetinitModule(T_SIMULATOR& Module){
	Module.dMax = 0;
	Module.dMin = 0;
	Module.dPos = 0.;
	Module.rectBound = CRect();
	Module.sizeRect = CSize();
	Module.eType = eS_LINE_STAGE;
	Module.bVet = FALSE;
	Module.bSelect = FALSE;
	Module.iBaseModule	= -1;
	Module.bOn	= FALSE;
	Module.sizeTable = CSize();
	Module.stType = ST_C;
	Module.stLinkType = ST_C;
	Module.bDrawGideStage = TRUE;
	Module.bDrawTable = TRUE;
	Module.Matrix	= CSize(1,1);
	Module.btType	= eBT_MATRIX;
	Module.bAutoUse			= FALSE;
	Module.dAutoPosStart	=0.;
	Module.dAutoPosEnd		=0.;
	Module.dAutoSPD			=0.;
	Module.iViewIndex		= -1;
	Module.colorBk			= RGB(200,200,200);
	Module.colorCell		= RGB(0,255,0);
	Module.iAlpha			= 210;
	Module.colorText		= RGB(200,200,200);
}

BOOL CSimulator::SetShow(int iModule, BOOL bBase, BOOL bTable) {
	BOOL bReturn = FALSE;
	if (m_vctModule.size() > iModule) {
		m_vctModule[iModule].bDrawTable = bTable;
		m_vctModule[iModule].bDrawGideStage = bBase;
		bReturn = TRUE;
	}

	return bReturn;
}
BOOL CSimulator::SetTableItem(int iModule, int iTableItem){
	BOOL bReturn = FALSE;

	m_vctModule[iModule].TableItems.clear();
	if( m_vctModule.size() > iModule){
		for(int i =0 ; i < iTableItem ; i++){
			BOOL bItem = FALSE;
			m_vctModule[iModule].TableItems.push_back(bItem);
		}
		bReturn = TRUE;
	}
	return bReturn;

}
BOOL CSimulator::SetLinkType(int iModule, ST_TYPE type){
	BOOL bReturn = FALSE;
	if( m_vctModule.size() > iModule){
		BOOL bItem = FALSE;
		m_vctModule[iModule].stLinkType = type;
		bReturn = TRUE;
	}

	return bReturn;
}
BOOL CSimulator::RestItem(int iModule, double dMax, double dMin, CRect Rect, int eType, BOOL bVet, int iBaseAxis){
	BOOL bReturn = FALSE;
	T_SIMULATOR module;
	
	SetinitModule(module);
		
	module.dMax = dMax;
	module.dMin = dMin;
	module.rectBound = Rect;
	module.sizeRect = CSize(Rect.Width(), Rect.Height());
	module.eType = eType;
	module.bVet = bVet;
	module.iBaseModule = iBaseAxis != iModule ? iBaseAxis : -1;
	module.bDrawGideStage = TRUE;
	BOOL bTable = FALSE;
	//for(int i =0 ; i < 8 ; i++){
	//	module.TableItems.push_back(bTable);
	//}
	m_vctModule.push_back(module);
	bReturn = TRUE;
	m_iVector = m_vctModule.size();
	m_vctModule[m_iVector - 1].iViewIndex = m_iVector - 1;
	return bReturn;
}
BOOL CSimulator::SetPos( int iModule, double dPos){
	BOOL bReturn = FALSE;

	if( m_vctModule.size() > iModule){
		if(m_vctModule[iModule].dMax > m_vctModule[iModule].dMin){
			if( m_vctModule[iModule].dMax < dPos ){
				m_vctModule[iModule].dPos = m_vctModule[iModule].dMax;
			}else if( m_vctModule[iModule].dMin > dPos ){
				m_vctModule[iModule].dPos = m_vctModule[iModule].dMin;
			}
		}else {
			if( m_vctModule[iModule].dMax > dPos ){
				m_vctModule[iModule].dPos = m_vctModule[iModule].dMax;
			}else if( m_vctModule[iModule].dMin < dPos ){
				m_vctModule[iModule].dPos = m_vctModule[iModule].dMin;
			}
		}
		{
			m_vctModule[iModule].dPos = dPos;
		}
		bReturn = TRUE;
	}
	return bReturn;
}
BOOL CSimulator::SetLinkModule(int iBaseModule, int iSlaveModule){
	BOOL bReturn = FALSE;

	if (m_vctModule.size() >= iSlaveModule) {
		if (m_vctModule.size() >= iBaseModule
			&& iBaseModule != iSlaveModule) {
			m_vctModule[iSlaveModule].iBaseModule = iBaseModule;

			bReturn = TRUE;
		}
		else {
			m_vctModule[iSlaveModule].iBaseModule = -1;
		}
	}
	return bReturn;
}
BOOL CSimulator::SetOnMaterial(int iModule, BOOL bOn){
	BOOL bReturn = FALSE;
	if( m_vctModule.size() > iModule){
		m_vctModule[iModule].bOn = bOn;
		bReturn = TRUE;
	}
	return bReturn;
}

BOOL CSimulator::SetSubTableOnMaterial(int iModule, int iTable, BOOL bOn){
	BOOL bReturn = FALSE;
	BOOL OnMaterial = FALSE;
	if( m_vctModule.size() > iModule){
		if(m_vctModule[iModule].TableItems.size() > iTable){

			m_vctModule[iModule].TableItems[iTable] = bOn;
			for(int i =0; i < m_vctModule[iModule].TableItems.size(); i++){
				if( m_vctModule[iModule].TableItems[iTable]){
					OnMaterial = TRUE;
					break;
				}
			}
			bReturn = TRUE;
		}
		
	}
	//SetOnMaterial(iModule, OnMaterial);
	return bReturn;
}


BOOL CSimulator::SetLinkOffset(int iModule, CPoint pt){

	BOOL bReturn = FALSE;
	if( m_vctModule.size() > iModule){
		m_vctModule[iModule].ptLinkOffset = pt;
		bReturn = TRUE;
	}

	return bReturn;
}
BOOL CSimulator::SetLinkOffset(int iModule, int ix, int iy){
	return SetLinkOffset(iModule, CPoint(ix, iy));
}
BOOL CSimulator::SetGideStage(int iModule, BOOL bDraw){
	BOOL bReturn = FALSE;
	if( m_vctModule.size() > iModule){
		m_vctModule[iModule].bDrawGideStage = bDraw;
		bReturn = TRUE;
	}

	return bReturn;
}
CPoint CSimulator::GetStageCenter(CRect rect, CSize sizeTable, BOOL bVet, int eType, double dPercentPos){
	CPoint ptC = rect.CenterPoint();
	CPoint ptPos;
	double dDistance;
	
	int iMinWidth	= sizeTable.cx/2;
	int iMinHeight  = sizeTable.cy/2;
	int iMinData = 0;
	switch( eType){ 
		case eS_LINE_STAGE:{
			
			iMinData = iMinWidth;
			
			if (bVet){
				iMinData = iMinHeight;
			}
			break;
		}
		case eS_GANTRY_STAGE:{
			iMinData = iMinWidth;
			
			if (bVet){
				iMinData = iMinHeight;
			}
			break;
		}
		case eS_ROTATE_STAGE:{
			
			break;
		}
	}
	if( eType != eS_ROTATE_STAGE){
		if( bVet){
			ptPos = CPoint(ptC.x, rect.top + iMinData);
			dDistance = fabs((rect.bottom - iMinData) - (rect.top + iMinData));
			ptPos.y+= (dDistance* dPercentPos);

		}else{
			ptPos = CPoint(rect.left + iMinData, ptC.y);
			dDistance = fabs((rect.left + iMinData) - (rect.right - iMinData));
			ptPos.x+= (dDistance* dPercentPos);

		}
	}
	return ptPos;
}
int CSimulator::GetDepth(int iBaseAxis, int iDepth){
	if( m_vctModule[iBaseAxis].iBaseModule >= 0){
		GetDepth(m_vctModule[iBaseAxis].iBaseModule, ++iDepth);
	}
	return iDepth;
}
CRect CSimulator::GetBaseRect(int iCrtAxis, CRect rectModule, int iDepth){
	CRect rectBase;
	
	CPoint ptCenter = CPoint(0,0);
	int iBaseModule = m_vctModule[iCrtAxis].iBaseModule;
	if (iDepth > m_vctModule.size()){
		rectModule = CRect(0, 0, 100, 100); 
		return rectModule;
	}
	if(iDepth == 0 ){
		rectModule -= rectModule.CenterPoint();
	}
	if( iBaseModule >= 0){
		double dBasePercent  =  GetPercent(iBaseModule);
		ptCenter = GetStageCenter(m_vctModule[iBaseModule].rectBound, m_vctModule[iBaseModule].sizeTable, m_vctModule[iBaseModule].bVet, m_vctModule[iBaseModule].eType, dBasePercent);

		CPoint ptOffset = GetLinkPos(iCrtAxis, m_vctModule[iBaseModule].sizeTable, m_vctModule[iCrtAxis].sizeTable);
		rectModule.OffsetRect(-ptOffset);
		if(iDepth > 0 ){
			CPoint ptOffset = m_vctModule[iCrtAxis].rectBound.CenterPoint() - ptCenter;
			rectModule.OffsetRect(-ptOffset);
		}else{
			rectModule.OffsetRect(ptCenter); 
		}
		if(m_vctModule[iBaseModule].iBaseModule >= 0){
			rectModule = GetBaseRect(iBaseModule, rectModule, ++iDepth); 
		}
		rectModule.OffsetRect(m_vctModule[iCrtAxis].ptLinkOffset); 

	}
	/*

	CRect rectBase;
	double dBaserPercent = 0;
	dBaserPercent  =  GetPercent(iBaseAxis);
	rectBase =  m_vctModule[iBaseAxis].rectBound;
	
	CPoint ptCenter = GetStageCenter(rectBase, rectModule, m_vctModule[iBaseAxis].bVet, m_vctModule[iBaseAxis].eType, dBaserPercent);
	rectModule.MoveToXY(ptCenter);
	if( m_vctModule[iBaseAxis].iBaseModule >= 0){
		rectBase.OffsetRect(-ptCenter);
		CRect rectBaseModule = GetBaseRect(m_vctModule[iBaseAxis].iBaseModule, rectBase, rectAxis);
		rectModule.OffsetRect(CSize(rectAxis.Width()/2, rectAxis.Height()/2));
	}else{
		
	}
	*/
	return rectModule;
}
double CSimulator::GetPercent(int iModule){
	double dPercent = ((m_vctModule[iModule].dMin - m_vctModule[iModule].dPos) /((m_vctModule[iModule].dMin - m_vctModule[iModule].dMax) /100.))/100.;
	if( fabs(m_vctModule[iModule].dMax -	m_vctModule[iModule].dMin) < 0.001){
		dPercent = 0.;
	}
	return dPercent;
}
CPoint CSimulator::GetLinkPos(int iCrtAxis, CSize sizeBaseTable, CSize sizeCrtTable){
	
	CPoint pt = CPoint(0,0);
	int iBaseAxis = m_vctModule[iCrtAxis].iBaseModule;
	ST_TYPE stLinkType = m_vctModule[iCrtAxis].stLinkType;
	if( iBaseAxis < 0){
		return pt;
	}
	
	int iBaseHalfX = sizeBaseTable.cx / 2;
	int iBaseHalfY = sizeBaseTable.cy / 2;
	int iCrtHalfX = sizeCrtTable.cx/2;
	int iCrtHalfY = sizeCrtTable.cy/2;
	if (m_vctModule[iCrtAxis].eType == eS_Z_STAGE) {
		iCrtHalfX = Z_SIZE/2;
		iCrtHalfY = Z_SIZE/2;
	}
	switch(stLinkType){
		case ST_T:{
			pt.y = (iBaseHalfY + iCrtHalfY);
			break;
		}
		case ST_B:{
			pt.y = -(iBaseHalfY + iCrtHalfY);
			break;
		}
		case ST_L:{
			pt.x = (iBaseHalfX + iCrtHalfX);
			break;
		}
		case ST_R:{
			pt.x = -(iBaseHalfX + iCrtHalfX);
			break;
		}
	}
			
	return pt;
}

void CSimulator::DrawMouseEvnet(CDC *pDC) {
	
	switch (m_tMouseEvent.eME) {
		case eME_DRAW_RECT: {
			DrawDashRectCross(pDC, m_tMouseEvent.rectLast);
			break;
		}
	}
	if (m_tMouseEvent.bSetLastRect == TRUE) {
		DrawDashRectCross(pDC, m_tMouseEvent.rectLast);
	}
}

BOOL CSimulator::GetLastRect(CRect& rect) {
	BOOL bReturn = TRUE;
	rect = m_tMouseEvent.rectLast;
	m_tMouseEvent.rectLast = CRect();
	m_tMouseEvent.bSetLastRect = FALSE;
	Invalidate(FALSE);
	return bReturn;
}
BOOL CSimulator::SetLoadBackGroundImg(CString strPath, CString strPathRev) {
	HRESULT		hResult;
	hResult = m_imgBackGround.Load(strPath);
	if (FAILED(hResult)) {
		CString strTmp = _T("ERROR: Failed to load ");
		return FALSE;
	}
	hResult = m_imgBackGroundRev.Load(strPathRev);
	if (FAILED(hResult)) {
		CString strTmp = _T("ERROR: Failed to load ");
		return FALSE;
	}
	return TRUE;
}
void CSimulator::OnPaint()
{
	CPaintDC dc(this); 
	CBitmap BitMap;
	CDC memDC;
	GetClientRect(m_rectClient);
	BitMap.CreateCompatibleBitmap(&dc, m_rectClient.Width(), m_rectClient.Height());

	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&BitMap);

	CBrush brush;
	brush.CreateSolidBrush(RGB(100,100,100));
	memDC.FillRect(m_rectClient, &brush);
	memDC.SetStretchBltMode(HALFTONE);
	CRect rcArea = CRect(0, 0, 1423, 367);
	if (m_bRotateDir) {
		if (m_imgBackGround) {
			m_imgBackGround.Draw(memDC, rcArea);
		}
	}
	else {
		if (m_imgBackGroundRev) {
			m_imgBackGroundRev.Draw(memDC, rcArea);
		}
	}
	
	
	if (m_vctSensor.size()) {
		for (int i = 0; i < m_vctSensor.size(); i++) {
			
			if (m_vctSensor[i].iLinkAxis < 0) {
				CPoint ptSensor = m_vctSensor[i].ptPosition;
				if (!m_bRotateDir)
					ptSensor = m_ct.Trans(ptSensor);

				switch (m_vctSensor[i].eSensorType) {
					case eST_SENSOR: {
						DrawSensor(&memDC, ptSensor, m_vctSensor[i].bOn, m_vctSensor[i].bSelected);		
						break;
					}
					case eST_DOOR :
					case eST_DOOR_KEY:{
						double dDoorAngle = 0;
						if (m_vctSensor[i].bOn != m_vctSensor[i].bOnOld) {
							m_vctSensor[i].dwTick = GetTickCount();
							m_vctSensor[i].bOnOld = m_vctSensor[i].bOn;
						}
						if (eST_DOOR == m_vctSensor[i].eSensorType) {
							m_vctSensor[i].bKey = -1;
						}
						DWORD dwTime = GetTickCount() - m_vctSensor[i].dwTick;
						dwTime /= 100;
						if ( dwTime < 10) {
							if (m_vctSensor[i].bOn) {
								dDoorAngle += dwTime*4.6;
							}
							else {
								dDoorAngle = 45 - (dwTime*4.5);
							}

						}
						else {
							if (m_vctSensor[i].bOn) {
								dDoorAngle = 45;
							}
						}
						DrawDoor(&memDC, ptSensor, m_vctSensor[i].bOn, m_vctSensor[i].bKey, -dDoorAngle, m_vctSensor[i].iAngle, m_vctSensor[i].bSelected);
						break;
					}
					case eST_EMO: {
						BOOL bOn = FALSE;
						if (!m_vctSensor[i].bOn) {
							if (GetTickCount() - m_vctSensor[i].dwTick < 500) {
								bOn = TRUE;
							}
							else if (GetTickCount() - m_vctSensor[i].dwTick > 1000){
								m_vctSensor[i].dwTick = GetTickCount();
							}
						}
						else {
							m_vctSensor[i].dwTick = GetTickCount();
							bOn = FALSE;
						}
						DrawEMO(&memDC, ptSensor, bOn, m_vctSensor[i].bSelected);
						break;
					}
					case eST_ROT_LEMP: {
						double dAngle = 0;
						if (m_vctSensor[i].bOn) {
							DWORD dwTick = GetTickCount() - m_vctSensor[i].dwTick;
							if (dwTick > 1000){
								m_vctSensor[i].dwTick = GetTickCount();
							}
							double dScaleAngle = 360./ 1000.;
							dAngle = (dwTick) * dScaleAngle;
						}
						DrawRotateLamp(&memDC, ptSensor, m_vctSensor[i].bOn, dAngle, m_vctSensor[i].bSelected);
                        break;  // KES 추가 2024.3.21
					}
                    // KES 추가 2024.3.21
					case eST_LIGHT_CURTAIN: {
                        // m_vctSensor[i].bOn -> ossd1, m_vctSensor[i].bKey -> ossd2 IO로 Mapping해서 사용
						BOOL bOn = FALSE;
                        BOOL bKey = FALSE;
						if (!m_vctSensor[i].bOn || !m_vctSensor[i].bKey) {
							if (GetTickCount() - m_vctSensor[i].dwTick < 500) {
								bOn = TRUE;
                                bKey = TRUE;
							}
							else if (GetTickCount() - m_vctSensor[i].dwTick > 1000){
								m_vctSensor[i].dwTick = GetTickCount();
							}
						}
						else {
							m_vctSensor[i].dwTick = GetTickCount();
							bOn = FALSE;
                            bKey = FALSE;
						}
						DrawLightCurtain(&memDC, ptSensor, !m_vctSensor[i].bOn, m_vctSensor[i].bKey, m_vctSensor[i].bSelected);
                        break;
					}
				}	
			}			
		}
	}
	if( m_vctModule.size()){
		double dPercent;
		CRect rect = CRect();
		int iViewIndex = -1;
		for(int i = 0 ; i < m_vctModule.size() ; i++){
			

			iViewIndex  = m_vctModule[i].iViewIndex;
			dPercent  =  GetPercent(iViewIndex);
			rect = m_vctModule[iViewIndex].rectBound;
			if (m_vctModule[iViewIndex].iBaseModule >=0 
				&&  m_vctModule[iViewIndex].iBaseModule < m_iVector
				&& m_vctModule[iViewIndex].iBaseModule != iViewIndex){
				rect = CRect(0,0, m_vctModule[iViewIndex].rectBound.Width(), m_vctModule[iViewIndex].rectBound.Height());
				rect = GetBaseRect(iViewIndex, rect);
			}
			
			switch( m_vctModule[iViewIndex].eType){
				case eS_LINE_STAGE:{
					DrawLineStage(&memDC, iViewIndex, rect, dPercent);
					break;
				}
				case eS_GANTRY_STAGE:{
					DrawGantryStage(&memDC, iViewIndex, rect, m_vctModule[i].sizeTable, m_vctModule[i].bVet, dPercent, m_vctModule[i].bSelect, m_vctModule[i].bDrawGideStage, m_vctModule[i].bDrawTable);
					break;
				}
				case eS_Z_STAGE:{

					DrawZStage(&memDC, iViewIndex, rect, m_vctModule[i].sizeTable, m_vctModule[i].bVet, dPercent, m_vctModule[i].bSelect, m_vctModule[i].bDrawTable);
					break;
				}
				case eS_ROTATE_STAGE:{
					DrawRotateStage(&memDC, iViewIndex, rect, dPercent, m_vctModule[i].TableItems, m_vctModule[i].bSelect, m_vctModule[i].bDrawGideStage);
					break;
				}
				case eS_ROTATE_TABLE:{
					//DrawRotateTable(&memDC, i, rect, dPercent, m_vctModule[i].bOn, m_vctModule[i].bSelect);
					DrawRotateTable(&memDC, iViewIndex, rect, dPercent);
					break;
				}
			}
		}
	}
	
	DrawMouseEvnet(&memDC);
	
	dc.BitBlt(m_rectClient.left, m_rectClient.top, m_rectClient.Width(), m_rectClient.Height(), &memDC, 0,0, SRCCOPY);
	
	// 2023.08.03 ...
	brush.DeleteObject();
	memDC.DeleteDC();
	BitMap.DeleteObject();
}
void CSimulator::DrawRect(CDC *pDC, CRect rect, COLORREF rgbBK, COLORREF rgbLine, int iAlpha){
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, rgbLine);
	CBrush Brush;
	Brush.CreateStockObject(NULL_BRUSH);
	Gdiplus::Graphics gr(pDC->GetSafeHdc());
	int iR = GetRValue(rgbBK);
	int iG = GetGValue(rgbBK);
	int iB = GetBValue(rgbBK);
	
	Gdiplus::SolidBrush br(Color(iAlpha, iR, iG, iB));
	gr.SetSmoothingMode(SmoothingModeHighQuality); 
	pDC->SelectObject(&Brush);
	//pDC->FillRect(rect, &Brush);
	pDC->SelectObject(&pen);
	gr.FillRectangle(&br, rect.left, rect.top, rect.Width(), rect.Height());
	pDC->Rectangle(rect);
	
	pen.DeleteObject();
	Brush.DeleteObject();
}
void CSimulator::DrawRect(CDC *pDC, CRect rect, BOOL bColor){
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(50,50,50));
	CBrush Brush;
	Brush.CreateSolidBrush(RGB(70,70,70));
	pDC->FillRect(rect, &Brush);
	pDC->SelectObject(&pen);
	pDC->Rectangle(rect);
	
	pen.DeleteObject();
	Brush.DeleteObject();
}
void CSimulator::DrawDashRectCross(CDC* pDC, CRect rect) {
	CPen pen;
	pen.CreatePen(PS_DASH, 1, RGB(255,0,0));
	pDC->SelectObject(&pen);
	CPoint pt[5];
	pt[0] = CPoint(rect.left, rect.top);
	pt[1] = CPoint(rect.left, rect.bottom);
	pt[2] = CPoint(rect.right, rect.bottom);
	pt[3] = CPoint(rect.right, rect.top);
	pt[4] = CPoint(rect.left, rect.top);
	pDC->Polyline(pt ,5);
	// H
	int iH = (rect.left + rect.right) / 2;
	int iV = (rect.top + rect.bottom) / 2;
	pDC->MoveTo(CPoint(iH, rect.top		));
	pDC->LineTo(CPoint(iH, rect.bottom	));
	// V
	pDC->MoveTo(CPoint(rect.left	, iV));
	pDC->LineTo(CPoint(rect.right	, iV));

	pen.DeleteObject();
}
CPoint CSimulator::GetRotatePoint(CPoint ptCenter, double dAngle){
	CPoint ptReturn;
	CPoint2d pt;
	//if( dAngle > 360.){
	//	dAngle = (int)dAngle % 360;
	//}
	double dTheta = M_PI/180*dAngle;
	pt.x = ptCenter.x*cos(dTheta) + ptCenter.y*sin(dTheta);
	pt.y = -ptCenter.x*sin(dTheta) + ptCenter.y*cos(dTheta);
	ptReturn.x = pt.x+0.5;
	ptReturn.y = pt.y+0.5;
	return ptReturn;
}
void CSimulator::DrawLineRect(CDC *pDC, const std::vector<CPoint> &pts, COLORREF rgbBK, COLORREF rgbLine, int iAlpha, int iSelectLine){
	
	Gdiplus::Point pt[255];
	int iSize = pts.size();
	for(int i =0 ; i < iSize ; i++){
		pt[i].X = pts[i].x;
		pt[i].Y = pts[i].y;
	}
	pt[iSize].X = pts[0].x;
	pt[iSize].Y = pts[0].y;
	//pDC->Polygon(pt, pts.size());
	
	Gdiplus::Graphics gr(pDC->GetSafeHdc());
	int iR = GetRValue(rgbBK);
	int iG = GetGValue(rgbBK);
	int iB = GetBValue(rgbBK);
	int iPR = GetRValue(rgbLine);
	int iPG = GetGValue(rgbLine);
	int iPB = GetBValue(rgbLine);
	
	Gdiplus::SolidBrush br(Color(iAlpha, iR, iG, iB));
	gr.SetSmoothingMode(SmoothingModeHighQuality); 
	Gdiplus::Pen gdipen(Color(255, iPR, iPG, iPB), iSelectLine); 
	gr.FillPolygon(&br, pt, iSize );
	gr.DrawLines(&gdipen, pt, iSize+1 );

}
void CSimulator::DrawLineRect(CDC *pDC, CRect rect){
	CPen pen;
	pen.CreatePen(PS_DASHDOT, 1, RGB(50,50,50));
	CBrush Brush;
	pDC->SelectObject(&pen);
	CPoint pt[5];
	pt[0] = CPoint(rect.left, rect.top);
	pt[1] = CPoint(rect.right, rect.top);
	pt[2] = CPoint(rect.right, rect.bottom);
	pt[3] = CPoint(rect.left, rect.bottom);
	pt[4] = CPoint(rect.left, rect.top);
	pDC->Polyline(pt, 5);

	pen.DeleteObject();
}
void CSimulator::DrawCircle(CDC *pDC, CRect rect, COLORREF rgb){

	double dRedius = min(rect.Width(), rect.Height())/2.;
	CRect rectCircle; 
	rectCircle = CRect(rect.CenterPoint().x-dRedius, rect.CenterPoint().y-dRedius, rect.CenterPoint().x+dRedius, rect.CenterPoint().y+dRedius);
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, rgb);
	CBrush Brush;
	Brush.CreateSolidBrush(RGB(70,70,70));
	pDC->SelectObject(&pen);
	pDC->SelectObject(&Brush);
	pDC->Ellipse(rectCircle);
	
	pen.DeleteObject();
	Brush.DeleteObject();
}

void CSimulator::DrawLineStage(CDC *pDC, int iModule, CRect rect, double dPercentPos){

	
	CSize sizeTable		= m_vctModule[iModule].sizeTable;
	BOOL bVet			= m_vctModule[iModule].bVet;
	BOOL bOn			= m_vctModule[iModule].bOn;
	BOOL bSelect		= m_vctModule[iModule].bSelect;
	BOOL bStage			= m_vctModule[iModule].bDrawGideStage;
	BOOL bTable			= m_vctModule[iModule].bDrawTable;
	COLORREF rgbStage	= m_vctModule[iModule].colorBk;
	COLORREF rgbOn		= m_vctModule[iModule].colorCell;
	COLORREF rgb = RGB_DEFAULT;
	int iLineWidth = 1;
	if(bSelect){
		rgb = RGB_ON_SELECT;
		iLineWidth = 2;
	}
	
	CBrush Brush;
	Brush.CreateSolidBrush(RGB(70,70,70));
	CRect rectDefaultLine;
	CPoint ptC = rect.CenterPoint();
	CPoint ptPos;
	double dDistance;
	int iMinWidth	= m_tLine.iWidth/2;
	int iMinHeight  = m_tLine.iHeight/2;
	int iHalfX = sizeTable.cx/2;
	int iHalfY = sizeTable.cy/2;
	if( bVet){
		rectDefaultLine = CRect( ptC.x-iHalfX, rect.top, ptC.x+iHalfX, rect.bottom);
		ptPos = CPoint(ptC.x, rect.top + iHalfY);
		
		dDistance = fabs((rect.bottom - iHalfY) - (rect.top + iHalfY));
		ptPos.y+= (dDistance* dPercentPos);
	}else{
		rectDefaultLine = CRect( rect.left, ptC.y-iHalfY, rect.right, ptC.y+iHalfY);
		ptPos = CPoint(rect.left + iHalfX, ptC.y);
		
		dDistance = fabs((rect.left + iHalfX) - (rect.right - iHalfX));
		ptPos.x+= (dDistance* dPercentPos);
	}
	if(bStage){
		DrawRect(pDC, rectDefaultLine, RGB_OFF_MATERIAL, RGB_DEFAULT);
	}
	CRect rectStage = CRect(-iHalfX ,-iHalfY ,iHalfX ,iHalfY );
	
	rectStage.OffsetRect(ptPos);
	
	if (!m_bRotateDir) {
		CPoint2d ptS, ptE;
		ptS = rectStage.TopLeft();
		ptE = rectStage.BottomRight();
		ptS = m_ct.Trans(ptS);
		ptE = m_ct.Trans(ptE);
		rectStage = CRect(ptE, ptS);
	}
	if (bTable) {
		DrawRect(pDC, rectStage, rgbStage, rgb);
	}

	if ( bOn ){
		rectStage.DeflateRect(4,4,4,4);
		double dW = (rectStage.Width()	- 2)/m_vctModule[iModule].Matrix.cx;
		double dH = (rectStage.Height()	- 2)/m_vctModule[iModule].Matrix.cy;
		int iIndex = 0;
		if ( m_vctModule[iModule].TableItems.size() ==0) {
			DrawRect(pDC, rectStage, rgbOn, rgb);
		}
		else {
			for (int iX = 0; iX < m_vctModule[iModule].Matrix.cx; iX++) {
				for (int iY = 0; iY < m_vctModule[iModule].Matrix.cy; iY++) {
					if (m_vctModule[iModule].TableItems.size() > 0) {
						if (m_vctModule[iModule].TableItems[iIndex]) {
							DrawRect(pDC, CRect(rectStage.left + 2 + dW * iX, rectStage.top + 2 + dH * iY, rectStage.left + dW * (iX + 1), rectStage.top + dH * (iY + 1)), rgbOn, rgb);
						}
						iIndex++;
					}
				}
			}
		}
	}
	if (!m_vctModule[iModule].strText.IsEmpty()) {
		DrawText(pDC, rectStage, m_vctModule[iModule].strText, 9, m_vctModule[iModule].colorText, m_vctModule[iModule].dwTextAlign);
	}

	Brush.DeleteObject();
}

void CSimulator::DrawGantryStage(CDC *pDC, int iModule, CRect rect, CSize sizeTable, BOOL bVet, double dPercentPos, BOOL bSelect, BOOL bStage, BOOL bTable){
	COLORREF rgb = RGB_DEFAULT;
	if(bSelect){
		rgb = RGB_ON_SELECT;
	}
	
	CBrush Brush;
	Brush.CreateSolidBrush(RGB(70,70,70));
	CRect rectDefaultLine1;
	CRect rectDefaultLine2;
	CPoint ptC = rect.CenterPoint();
	CPoint ptPos;
	double dDistance;
	CRect rectStage;
	int iMinWidth  = m_tGantry.iSize/2;
	if( bVet){
		rectDefaultLine1 = CRect( rect.left,			rect.top,  rect.left+LINE_SIZE,	rect.bottom);
		rectDefaultLine2 = CRect( rect.right-LINE_SIZE,	rect.top,  rect.right,			rect.bottom);
		ptPos = CPoint(ptC.x, rect.top + iMinWidth);
		
		rectStage = CRect(-rect.Width()/2 ,-iMinWidth ,rect.Width()/2 ,iMinWidth );

		dDistance = fabs((rect.bottom - iMinWidth) - (rect.top + iMinWidth));
		ptPos.y+= (dDistance* dPercentPos);

	}else{
		rectDefaultLine1 = CRect( rect.left, rect.top,				rect.right,	rect.top+LINE_SIZE);
		rectDefaultLine2 = CRect( rect.left, rect.bottom-LINE_SIZE,	rect.right,	rect.bottom);
		ptPos = CPoint(rect.left + iMinWidth, ptC.y);
		
		
		rectStage = CRect(-iMinWidth ,-rect.Height()/2 ,iMinWidth ,rect.Height()/2);

		dDistance = fabs((rect.left + iMinWidth) - (rect.right - iMinWidth));
		ptPos.x+= (dDistance* dPercentPos);
	}
	if(bStage){
		DrawRect(pDC, rectDefaultLine1);
		DrawRect(pDC, rectDefaultLine2);
	}
	
	rectStage.OffsetRect(ptPos);
	if (bTable) {
		DrawRect(pDC, rectStage, RGB_OFF_MATERIAL, rgb);
	}
	Brush.DeleteObject();
}
void CSimulator::DrawZStage(CDC *pDC, int iModule, CRect rect, CSize sizeTable, BOOL bVet, double dPercentPos, BOOL bSelect, BOOL bTable){
	COLORREF rgb = RGB_DEFAULT;
	if(bSelect){
		rgb = RGB_ON_SELECT;
	}
	
	CBrush Brush;
	Brush.CreateSolidBrush(RGB(70,70,70));
	CRect rectDefaultLine;
	CPoint ptC = rect.CenterPoint();
	CPoint ptPos;
	double dDistance;
	CRect rectStage = CRect();
	
	if( bVet){
		rectDefaultLine = CRect( ptC.x-Z_SIZE/2, rect.top, ptC.x+Z_SIZE/2, rect.bottom);
		
		ptPos = CPoint(ptC.x, rect.top);
		
		dDistance = fabs((rect.bottom ) - (rect.top));
		ptPos.y+= (dDistance* dPercentPos);
		ptPos.x+= Z_SIZE/2;
		rectStage = CRect(rectDefaultLine.left, rectDefaultLine.top, ptPos.x, ptPos.y);
		if (rectStage.Height() > 0) {
			rectStage.DeflateRect(1, 1, 1, 1);
		}
	}else{
		rectDefaultLine = CRect( rect.left, ptC.y-Z_SIZE/2, rect.right, ptC.y+Z_SIZE/2);
		
		ptPos = CPoint(rect.left, ptC.y);
		dDistance = fabs((rect.left) - (rect.right));
		ptPos.x+= (dDistance* dPercentPos);
		ptPos.y+= Z_SIZE/2;
		rectStage = CRect(rectDefaultLine.left, rectDefaultLine.top, ptPos.x, ptPos.y);
		if (rectStage.Width() > 0) {
			rectStage.DeflateRect(1, 1, 1, 1);
		}
	}
	if (!m_bRotateDir) {
		CPoint2d ptS, ptE;
		ptS = rectStage.TopLeft();
		ptE = rectStage.BottomRight();
		ptS = m_ct.Trans(ptS);
		ptE = m_ct.Trans(ptE);
		rectStage = CRect(ptE, ptS);

		ptS = rectDefaultLine.TopLeft();
		ptE = rectDefaultLine.BottomRight();
		ptS = m_ct.Trans(ptS);
		ptE = m_ct.Trans(ptE);
		rectDefaultLine = CRect(ptE, ptS);
	}

	DrawRect(pDC, rectDefaultLine, RGB_DEFAULT, rgb, 255);
	
	DrawRect(pDC, rectStage, RGB_OFF_MATERIAL, RGB_OFF_MATERIAL, 255);
	Brush.DeleteObject();
}
void CSimulator::DrawRotateStage(CDC *pDC, int iModule, CRect rect, double dPercentPos, std::vector<BOOL> &SubTable, BOOL bSelect, BOOL bStage, BOOL bTable){
	COLORREF rgb = RGB_DEFAULT;
	if(bSelect){
		rgb = RGB_ON_SELECT;
	}
	CBrush Brush;
	Brush.CreateSolidBrush(RGB(70,70,70));

	if(bStage){
		DrawCircle(pDC, rect , rgb);
	}
	double dMinData = min( rect.Width(), rect.Height())/2;
	double dWidth = dMinData * 0.4;
	if( dWidth< 3.){
		dWidth = 3.;
	}
	double dWidthHalf = dWidth/2;
	CPoint ptC = rect.CenterPoint();
	CPoint ptDef;//= dMinData * 0.8;
	ptDef.x = (min( rect.Width(), rect.Height() )/2.)*0.7;
	ptDef.y = 0;

	double dDefTheta =  360 / SubTable.size();
	CString str;
	CRect rectStage = CRect(-dWidthHalf, -dWidthHalf, dWidthHalf, dWidthHalf);
	if (bTable) {
		for (int i = 0; i < SubTable.size(); i++) {
			rectStage = CRect(-dWidthHalf, -dWidthHalf, dWidthHalf, dWidthHalf);
			std::vector<CPoint> pts;

			pts.push_back(CPoint(-dWidthHalf, -dWidthHalf));
			pts.push_back(CPoint(dWidthHalf, -dWidthHalf));
			pts.push_back(CPoint(dWidthHalf, dWidthHalf));
			pts.push_back(CPoint(-dWidthHalf, dWidthHalf));
			CPoint ptCenter = GetRotatePoint(ptDef, (i * dDefTheta) + (360 * dPercentPos));
			for (int j = 0; j < pts.size(); j++) {
				pts[j] = GetRotatePoint(pts[j], (i * dDefTheta) + (360 * dPercentPos));
				pts[j] += ptCenter;
				pts[j] += ptC;

			}
			COLORREF rgbStage = RGB_OFF_MATERIAL;
			if (SubTable[i]) {
				rgbStage = RGB_ON_MATERIAL;
			}
			DrawLineRect(pDC, pts, rgbStage, rgb);
			rectStage += (ptCenter + ptC);

			//str.Format(_T("%d"), i);

			DWORD dwAlign = DT_SINGLELINE | DT_CENTER | DT_VCENTER;
			pDC->DrawText(str, rectStage, dwAlign);
		}
	}
	Brush.DeleteObject();
}

void CSimulator::DrawRotateTable(CDC *pDC, int iModule, CRect rect, double dPercentPos){
	
	BOOL bOn		= m_vctModule[iModule].bOn;
	BOOL bSelect	= m_vctModule[iModule].bSelect;
	BOOL bTable		= m_vctModule[iModule].bDrawTable;
	COLORREF rgbStage	= m_vctModule[iModule].colorBk;
	COLORREF rgbOn		= m_vctModule[iModule].colorCell;
	int iAlpah			= m_vctModule[iModule].iAlpha;
	COLORREF rgb = RGB_DEFAULT;
	if(bSelect){
		rgb = RGB_ON_SELECT;
	}
	CBrush Brush;
	Brush.CreateSolidBrush(RGB(70,70,70));

	
	double dMinData = min( rect.Width(), rect.Height())/2;
	double dWidth = dMinData * 0.4;
	if( dWidth< 3.){
		dWidth = 3.;
	}
	std::vector<CPoint> pts;
	double dWidthHalf = dWidth/2;
	CPoint ptC = rect.CenterPoint();
	rect.OffsetRect(-ptC);
	
	pts.push_back( CPoint(rect.left, rect.top));
	pts.push_back( CPoint(rect.right, rect.top));
	pts.push_back( CPoint(rect.right, rect.bottom));
	pts.push_back( CPoint(rect.left, rect.bottom));

	for(int i =0; i < pts.size() ;i++){
		pts[i] = GetRotatePoint( pts[i], (360*dPercentPos));
		pts[i] += ptC;
		if (!m_bRotateDir)
			pts[i] = m_ct(pts[i]);
	}
	
	
	if (bTable) {
		DrawLineRect(pDC, pts, rgbStage, rgb, iAlpah);
	}
	if ( bOn ){
		std::vector<CPoint> ptsOnCell;
		rect.DeflateRect(4,4,4,4);
	
		ptsOnCell.push_back( CPoint(rect.left, rect.top));
		ptsOnCell.push_back( CPoint(rect.right, rect.top));
		ptsOnCell.push_back( CPoint(rect.right, rect.bottom));
		ptsOnCell.push_back( CPoint(rect.left, rect.bottom));

		for(int i =0; i < ptsOnCell.size() ;i++){
			ptsOnCell[i] = GetRotatePoint( ptsOnCell[i], (360*dPercentPos));
			ptsOnCell[i] += ptC;
			if (!m_bRotateDir)
				ptsOnCell[i] = m_ct(ptsOnCell[i]);
		}
		DrawLineRect(pDC, ptsOnCell, rgbOn, rgb);

	}
	Brush.DeleteObject();
}

void CSimulator::DrawSensor(CDC* pDC, CPoint pt, BOOL bOn, BOOL bSelect) {
	//pt
	CRect rect = CRect(pt.x-8, pt.y-8, pt.x+8, pt.y+8);
	COLORREF color;
	Gdiplus::SolidBrush brR(Color(255, 255, 0, 0));    // Alpha, Red, Blue, Green
	Gdiplus::SolidBrush brG(Color(255, 0, 255, 0));    // Alpha, Red, Blue, Green
	Gdiplus::SolidBrush brGray(Color(255, 100, 100, 100));    // Alpha, Red, Blue, Green
	Gdiplus::Pen grPen(Color(255, 0, 0, 0));    // Alpha, Red, Blue, Green
	Gdiplus::Pen grPenSelect(Color(255, 255, 0, 0), 2);    // Alpha, Red, Blue, Green
	Gdiplus::Graphics gr(pDC->GetSafeHdc());
	Gdiplus::Rect grRectbase, grRect;
	
	grRectbase.X		= rect.left;
	grRectbase.Y		= rect.top;
	grRectbase.Width	= rect.Width();
	grRectbase.Height	= rect.Height();
	grRect.X		= rect.left+3;
	grRect.Y		= rect.top+3;
	grRect.Width	= rect.Width()-6;
	grRect.Height	= rect.Height()-6;
	gr.SetSmoothingMode(SmoothingModeHighQuality); 
	gr.FillEllipse(&brGray, grRectbase);
	gr.DrawEllipse(&grPen, grRectbase);
	if (bOn) {
		gr.FillEllipse(&brG, grRect);
	}else{
		gr.FillEllipse(&brR, grRect);
	}
	if (bSelect) {
		gr.DrawEllipse(&grPenSelect, grRectbase);
	}
	
}
void CSimulator::DrawDoor(CDC* pDC, CPoint pt, BOOL bOpen, BOOL bKey,double dDoorAngle, double dAngle, BOOL bSelect) {
	
	
	CSize sizeDoorBase	= CSize(30,12);
	CSize sizeDoor		= CSize(30,6);
	CPoint ptKey = CPoint(20,0);
	int dHalfBase = (sizeDoor.cy/2);
	CRect rectDoorBase;
	if (!m_bRotateDir) {
		dAngle += 180;
	}
	std::vector<CPoint> ptsBase;
	std::vector<CPoint> pts;
	ptsBase.push_back(CPoint(-sizeDoorBase.cx, -sizeDoorBase.cy));
	ptsBase.push_back(CPoint(sizeDoorBase.cx, -sizeDoorBase.cy));
	ptsBase.push_back(CPoint(sizeDoorBase.cx, sizeDoorBase.cy));
	ptsBase.push_back(CPoint(-sizeDoorBase.cx, sizeDoorBase.cy));
	
	pts.push_back(CPoint(-sizeDoor.cx, -sizeDoor.cy));
	pts.push_back(CPoint(sizeDoor.cx, -sizeDoor.cy));
	pts.push_back(CPoint(sizeDoor.cx, sizeDoor.cy));
	pts.push_back(CPoint(-sizeDoor.cx, sizeDoor.cy));
	CPoint ptDoorRotateOffset;
	CRect rectBase(INT_MAX/2, INT_MAX/2, -INT_MAX/2, -INT_MAX/2);
	for (int i = 0; i < 4; i++) {
		
		ptsBase[i] = GetRotatePoint(ptsBase[i], dAngle);
		if(rectBase.left	> ptsBase[i].x)  rectBase.left   = 	ptsBase[i].x;
		if(rectBase.right	< ptsBase[i].x)  rectBase.right  = 	ptsBase[i].x;
		if(rectBase.top		> ptsBase[i].y)  rectBase.top    = 	ptsBase[i].y;
		if(rectBase.bottom	< ptsBase[i].y)  rectBase.bottom = 	ptsBase[i].y;
		
		ptsBase[i] += pt;
		pts[i] = GetRotatePoint(pts[i], dAngle+dDoorAngle);
		pts[i] += pt;
		
		
	}
	rectBase.OffsetRect(pt);
	ptDoorRotateOffset = ptsBase[3] - pts[0] ;
	for (int i = 0; i < 4; i++) {
		pts[i] += ptDoorRotateOffset;
	}
	COLORREF rgbBase = RGB_OFF_MATERIAL;
	COLORREF rgbDoor = RGB_OFF_MATERIAL;
	if (bOpen) {
		rgbDoor = RGB(255, 0, 0);
	}
	int iSelectLine = 1;
	COLORREF colorSelect = RGB(100, 100, 100);
	if (bSelect) {
		colorSelect = RGB_ON_SELECT;
		iSelectLine = 2;
	}
	DrawLineRect(pDC, ptsBase, rgbBase, colorSelect, 255, iSelectLine );
	DrawLineRect(pDC, pts, rgbDoor, colorSelect, 255, iSelectLine  );
	if (bKey > -1) {
		ptKey = GetRotatePoint(ptKey, dAngle);
		ptKey += pt;
		DrawSensor(pDC, ptKey, bKey);
	}
	
	CFont font_Rotate;
	memset(&font_Rotate,  0x00, sizeof( font_Rotate));
	font_Rotate.CreateFontW(sizeDoorBase.cx-6, sizeDoorBase.cy-3, dAngle*10, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH| FF_SWISS, _T("console"));
	CFont *font_Old = pDC->SelectObject(&font_Rotate);
	int iOld_Mode = pDC->SetBkMode(TRANSPARENT);
	if (bOpen) {
		pDC->TextOutW(ptsBase[0].x, ptsBase[0].y, g_st[IDS_SIMULATOR_WND_OPEN]);
	}
	else {
		pDC->TextOutW(ptsBase[0].x, ptsBase[0].y,g_st[IDS_SIMULATOR_WND_CLOSE]);
	}
	pDC->SelectObject(font_Old);
	pDC->SetBkMode(iOld_Mode);
	
	font_Rotate.DeleteObject();
}
void CSimulator::DrawEMO(CDC* pDC, CPoint pt, BOOL bOn, BOOL bSelect) {
	CRect rect = CRect(pt.x-10, pt.y-10, pt.x+10, pt.y+10);
	
	Gdiplus::SolidBrush brR(Color(255, 255, 0, 0));    
	Gdiplus::SolidBrush brY(Color(255, 255, 255, 0));   
	Gdiplus::SolidBrush brGray(Color(255, 100, 100, 100));
	Gdiplus::Pen grPen(Color(255, 0, 0, 0));
	Gdiplus::Pen grPenSelect(Color(255, 255, 0, 0), 2);
	Gdiplus::Graphics gr(pDC->GetSafeHdc());
	Gdiplus::Rect grRectbase, grRect;
	
	grRectbase.X		= rect.left;
	grRectbase.Y		= rect.top;
	grRectbase.Width	= rect.Width();
	grRectbase.Height	= rect.Height();
	grRect.X		= rect.left+4;
	grRect.Y		= rect.top+4;
	grRect.Width	= rect.Width()-8;
	grRect.Height	= rect.Height()-8;
	gr.SetSmoothingMode(SmoothingModeHighQuality); 
	gr.FillEllipse(&brGray, grRectbase);
	gr.DrawEllipse(&grPen, grRectbase);
	if (bOn) {
		gr.FillEllipse(&brR, grRectbase);
		gr.FillEllipse(&brY, grRect);
		
	}else{
		gr.FillEllipse(&brY, grRectbase);
		gr.FillEllipse(&brR, grRect);
		
	}
	if (bSelect) {
		gr.DrawEllipse(&grPenSelect, grRectbase);
	}
}
void CSimulator::DrawRotateLamp(CDC* pDC, CPoint pt, BOOL bOn, double dAngle, BOOL bSelect) {
	CRect rect = CRect(pt.x-12, pt.y-12, pt.x+12, pt.y+12);
	
	Gdiplus::SolidBrush brR(Color(255, 255, 0, 0));    
	Gdiplus::SolidBrush brO(Color(255, 255, 255, 0));   
	Gdiplus::SolidBrush brGray(Color(100, 255, 127, 0));
	Gdiplus::Pen grPenO(Color(255, 255, 100, 0),1);
	Gdiplus::Pen grPen(Color(255, 255, 127, 0));
	Gdiplus::Pen grPenSelect(Color(255, 255, 0, 0), 2);
	Gdiplus::Graphics gr(pDC->GetSafeHdc());
	Gdiplus::Rect grRectbase, grRect;
	Gdiplus::Point gdipt[5];
	grRectbase.X		= rect.left;
	grRectbase.Y		= rect.top;
	grRectbase.Width	= rect.Width();
	grRectbase.Height	= rect.Height();
	grRect.X		= rect.left+8;
	grRect.Y		= rect.top+8;
	grRect.Width	= rect.Width()-16;
	grRect.Height	= rect.Height()-16;
	gr.SetSmoothingMode(SmoothingModeHighQuality); 
	rect.DeflateRect(2,2,2,2);
	CPoint cpt[4];
	cpt[0] = CPoint(-9,-7);
	cpt[1] = CPoint(9,	7);
	cpt[2] = CPoint(9,	-7);
	cpt[3] = CPoint(-9, 7);
	for (int i = 0; i < countof(cpt); i++) {
		CPoint 	ptTemp = GetRotatePoint(cpt[i], dAngle);
		gdipt[i].X = ptTemp.x + pt.x;
		gdipt[i].Y = ptTemp.y + pt.y;
		
	}
	gdipt[4] = gdipt[0];
	gr.FillEllipse(&brGray, grRectbase);
	gr.FillPolygon(&brO, gdipt, 5);
	gr.DrawPolygon(&grPenO, gdipt, 5);
	gr.DrawEllipse(&grPen, grRectbase);
	
	gr.FillEllipse(&brO, grRect);
	
	if (bSelect) {
		gr.DrawEllipse(&grPenSelect, grRectbase);
	}
}
// KES 추가 2024.3.21
void CSimulator::DrawLightCurtain(CDC* pDC, CPoint pt, BOOL bOn, BOOL bKey, BOOL bSelect) {
	CRect rect = CRect(pt.x-10, pt.y-35, pt.x+10, pt.y+35);
	
	Gdiplus::SolidBrush brR(Color(255, 255, 0, 0));    
	Gdiplus::SolidBrush brY(Color(255, 255, 255, 0));   
	Gdiplus::SolidBrush brGray(Color(255, 100, 100, 100));
	Gdiplus::Pen grPen(Color(255, 0, 0, 0));
	Gdiplus::Pen grPenSelect(Color(255, 255, 0, 0), 2);
	Gdiplus::Graphics gr(pDC->GetSafeHdc());
	Gdiplus::Rect grRectbase, grRect;
	
	grRectbase.X		= rect.left;
	grRectbase.Y		= rect.top;
	grRectbase.Width	= rect.Width();
	grRectbase.Height	= rect.Height();
	grRect.X		= rect.left+5;
	grRect.Y		= rect.top+5;
	grRect.Width	= rect.Width()-10;
	grRect.Height	= rect.Height()-10;
	gr.SetSmoothingMode(SmoothingModeHighQuality); 
	gr.FillRectangle(&brGray, grRectbase);
	gr.DrawRectangle(&grPen, grRectbase);
	
	// 감지
	if (bOn) {
		gr.FillRectangle(&brR, grRectbase);
		gr.FillRectangle(&brR, grRect);
	} // Muting
	else if (bKey) {
		gr.FillRectangle(&brY, grRectbase);
		gr.FillRectangle(&brY, grRect);
	}
	else {
		gr.FillRectangle(&brGray, grRectbase);
		gr.FillRectangle(&brGray, grRect);
	}

	if (bSelect) {
		gr.DrawRectangle(&grPenSelect, grRectbase);
	}
}
BOOL CSimulator::SetTableSize(int iModule, CSize Size) {
	BOOL bReturn = FALSE;
	if (m_vctModule.size() > iModule) {
		m_vctModule[iModule].sizeTable = Size;
		bReturn = TRUE;
	}
	return bReturn;
}
BOOL CSimulator::SetRect(int iModule, CRect rect) {
	BOOL bReturn = FALSE;
	if (m_vctModule.size() > iModule) {
		m_vctModule[iModule].rectBound = rect;
		bReturn = TRUE;
	}

	return bReturn;
}
void CSimulator::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch( nIDEvent){
		case INTERVAL_UI:{
			for (int i = 0; i < m_iVector; i++) {
				CalcAutoPos(i);
			}
			Invalidate(FALSE);
			
			break;
		}
	}
	CWnd::OnTimer(nIDEvent);
}


void CSimulator::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	if( bShow){
		GetClientRect(m_rectClient);
		SetTimer(INTERVAL_UI, 100, NULL);
	}else{
		KillTimer(INTERVAL_UI);
	}
	// TODO: Add your message handler code here
}


void CSimulator::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (m_tMouseEvent.eME) {
		case eME_DRAW_RECT:{
			if (m_tMouseEvent.bOn == TRUE) {
				m_tMouseEvent.ptEnd = point;
				
				m_tMouseEvent.rectLast.left		=	min(m_tMouseEvent.ptStart.x, m_tMouseEvent.ptEnd.x);
				m_tMouseEvent.rectLast.right	=	max(m_tMouseEvent.ptStart.x, m_tMouseEvent.ptEnd.x);
				m_tMouseEvent.rectLast.top		=	min(m_tMouseEvent.ptStart.y, m_tMouseEvent.ptEnd.y);
				m_tMouseEvent.rectLast.bottom	=	max(m_tMouseEvent.ptStart.y, m_tMouseEvent.ptEnd.y);
				Invalidate(FALSE);
			}
			break;
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}


void CSimulator::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch (m_tMouseEvent.eME) {
		case eME_DRAW_RECT: {
			
			break;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void CSimulator::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (m_tMouseEvent.eME) {
	
		
		case eME_DRAW_RECT:{
			if (m_tMouseEvent.bOn == FALSE) {
				Invalidate(FALSE);
				m_tMouseEvent.bOn			= TRUE;
				m_tMouseEvent.ptStart		= point;
				m_tMouseEvent.rectLast.left = point.x;
				m_tMouseEvent.rectLast.top	= point.y;
				m_tMouseEvent.rectLast.right = point.x;
				m_tMouseEvent.rectLast.bottom	= point.y;
				
			}
			else {
				m_tMouseEvent.bOn			= FALSE;
				m_tMouseEvent.eME = eME_NONE;
				m_tMouseEvent.rectLast.left = min(m_tMouseEvent.ptStart.x, m_tMouseEvent.ptEnd.x);
				m_tMouseEvent.rectLast.right = max(m_tMouseEvent.ptStart.x, m_tMouseEvent.ptEnd.x);
				m_tMouseEvent.rectLast.top = min(m_tMouseEvent.ptStart.y, m_tMouseEvent.ptEnd.y);
				m_tMouseEvent.rectLast.bottom = max(m_tMouseEvent.ptStart.y, m_tMouseEvent.ptEnd.y);
				m_tMouseEvent.bSetLastRect = TRUE;
			}
			break;
		}
	}
	m_tMouseEvent.nRClick = 0;

	CWnd::OnLButtonUp(nFlags, point);
}


void CSimulator::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnRButtonDown(nFlags, point);
}


void CSimulator::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (m_tMouseEvent.eME) {
		
		case eME_DRAW_RECT:{
			m_tMouseEvent.bOn	= FALSE;
			m_tMouseEvent.eME	= eME_NONE;
			m_tMouseEvent.rectLast = CRect();
			m_tMouseEvent.bSetLastRect = FALSE;	
			Invalidate(FALSE);
			break;
		}
	}
	BOOL bCheck = FALSE;
	if (GetTickCount() - m_dwRButtonUp < 500) {
		bCheck = TRUE;
	}
	else {
		m_dwRButtonUp = GetTickCount();
	}
	if (m_tMouseEvent.eME == eME_NONE) {
		if (bCheck) {
			
			m_tMouseEvent.eME = eME_DRAW_RECT;
			m_tMouseEvent.nRClick = 0;
			m_tMouseEvent.bSetLastRect = FALSE;
			m_tMouseEvent.bOn = TRUE;
			m_tMouseEvent.ptStart = point;
			m_tMouseEvent.rectLast.left = point.x;
			m_tMouseEvent.rectLast.top = point.y;
			m_tMouseEvent.rectLast.right = point.x;
			m_tMouseEvent.rectLast.bottom = point.y;
			Invalidate(FALSE);
			
		}
	}
	CWnd::OnRButtonUp(nFlags, point);
}

BOOL CSimulator::DeleteAllSensor() {
	m_vctSensor.clear();
	
	return m_vctSensor.size() == 0;
}
int	CSimulator::AddSensor(CPoint ptPos, CPoint ptOffset, int iType, int iLinkAxis) {
	T_SENSOR tSensor;
	tSensor.bOn = FALSE;
	tSensor.bKey	= FALSE;
	tSensor.ptPosition = ptPos;
	tSensor.ptOffset = ptOffset; 
	tSensor.iLinkAxis = iLinkAxis;
	tSensor.iAngle	= 0;
	tSensor.bOnOld = -1;
	tSensor.bSelected = FALSE;
	tSensor.eSensorType = (eSENSOR_TYPE)iType; 
	m_vctSensor.push_back(tSensor);
	return m_vctSensor.size();
}
BOOL CSimulator::SetMatrix(int iModule, CSize sizeMatrix) {
	BOOL bReturn = FALSE;
	if (m_vctModule.size() > iModule) {
		if (sizeMatrix.cx < 1) {
			sizeMatrix.cx = 1; 
		}
		if (sizeMatrix.cy < 1) {
			sizeMatrix.cy = 1; 
		}
		if (m_vctModule[iModule].TableItems.size() != sizeMatrix.cx * sizeMatrix.cy) {
			m_vctModule[iModule].TableItems.clear();
			m_vctModule[iModule].Matrix = sizeMatrix;
			m_vctModule[iModule].TableItems.assign(sizeMatrix.cx * sizeMatrix.cy, 0);
		}
		bReturn = TRUE;
	}
	return bReturn;
}
BOOL CSimulator::SetSensorAngle(int iModule, int iAngle) {
	BOOL bReturn = FALSE;
	if (m_vctSensor.size() > iModule) {
		m_vctSensor[iModule].iAngle = iAngle;
		bReturn = TRUE;
	}
	return bReturn;
}
BOOL CSimulator::SetSensor(int iModule, BOOL bOn, BOOL bKey) {
	BOOL bReturn = FALSE;
	if (m_vctSensor.size() > iModule) {
		m_vctSensor[iModule].bOn = bOn;
		bReturn = TRUE;
        // KES 수정 2024.3.21
		if (m_vctSensor[iModule].eSensorType == eST_DOOR_KEY || m_vctSensor[iModule].eSensorType == eST_LIGHT_CURTAIN){
			m_vctSensor[iModule].bKey = bKey;
		}
		else {
			bReturn = FALSE;
		}
	}
	return bReturn;
}
BOOL CSimulator::SetAutoMove(int iModule, BOOL bUse, double dSpd, double dStart, double dEnd) {
	BOOL bReturn = FALSE;
	if (m_vctModule.size() > iModule) {
		if (m_vctModule[iModule].bAutoUse == FALSE) {
			m_vctModule[iModule].dwTick = GetTickCount();
		}
		m_vctModule[iModule].bAutoUse = bUse;
		m_vctModule[iModule].dAutoSPD = dSpd;
		m_vctModule[iModule].dAutoPosStart = dStart;
		m_vctModule[iModule].dAutoPosEnd = dEnd;

		bReturn = TRUE;
	}
	return bReturn;
}

void CSimulator::CalcAutoPos(int iModule) {

	if (m_vctModule[iModule].bAutoUse) {

		double dTime = (GetTickCount() - m_vctModule[iModule].dwTick)/1000.;
		if (fabs(m_vctModule[iModule].dAutoSPD) > 0.001) {
			m_vctModule[iModule].dPos = m_vctModule[iModule].dAutoPosStart + (dTime * m_vctModule[iModule].dAutoSPD);
		}
		if (m_vctModule[iModule].dAutoSPD > 0 ? m_vctModule[iModule].dPos >= m_vctModule[iModule].dAutoPosEnd : m_vctModule[iModule].dPos <= m_vctModule[iModule].dAutoPosEnd) {
			m_vctModule[iModule].dwTick = GetTickCount();
			m_vctModule[iModule].dPos = m_vctModule[iModule].dAutoPosStart;
		}

	}
}
BOOL CSimulator::SetSelectSensor(int iModule) {
	BOOL bReturn = FALSE;
	for (int i = 0; i < m_vctSensor.size(); i++) {
		m_vctSensor[i].bSelected = FALSE;
	}
	if (m_vctSensor.size() > iModule) {
		m_vctSensor[iModule].bSelected = TRUE;
		bReturn = TRUE;
	}
	return bReturn;
}

BOOL CSimulator::SetViewSwap(int iModule1, int iModule2, BOOL bSwap){
	BOOL bReturn = FALSE;
	
	if (m_vctModule.size() > iModule1 && m_vctModule.size() > iModule2) {
		
		if (bSwap) {
			m_vctModule[iModule1].iViewIndex = iModule2;
			m_vctModule[iModule2].iViewIndex = iModule1;
		}
		else {
			m_vctModule[iModule1].iViewIndex = iModule1;
			m_vctModule[iModule2].iViewIndex = iModule2;
		}
		bReturn = TRUE;
	}
	return bReturn;
}
BOOL CSimulator::SetColorBK(int iModule, COLORREF rgbTable) {
	BOOL bReturn = FALSE;
	
	if (m_vctModule.size() > iModule) {
		m_vctModule[iModule].colorBk = rgbTable;
		bReturn = TRUE;
	}
	return bReturn;
}
BOOL CSimulator::SetColorCell(int iModule, COLORREF rgbCell) {
	BOOL bReturn = FALSE;
	
	if (m_vctModule.size() > iModule) {
		m_vctModule[iModule].colorCell = rgbCell;
		bReturn = TRUE;
	}
	return bReturn;
}

BOOL CSimulator::SetAlphaTable(int iModule, int iAlpha) {
	BOOL bReturn = FALSE;
	
	if (m_vctModule.size() > iModule) {
		m_vctModule[iModule].iAlpha = iAlpha;
		bReturn = TRUE;
	}
	return bReturn;
}
BOOL CSimulator::SetString(int iModule, CString strText, COLORREF colorText, DWORD dwAlign ) {
	BOOL bReturn = FALSE;
	
	if (m_vctModule.size() > iModule) {
		m_vctModule[iModule].strText = strText;
		m_vctModule[iModule].colorText = colorText;
		m_vctModule[iModule].dwTextAlign = dwAlign;
		bReturn = TRUE;
	}
	return bReturn;
}
BOOL CSimulator::SetDirection(BOOL bDir) {

	m_bRotateDir = bDir;

	return TRUE;
}
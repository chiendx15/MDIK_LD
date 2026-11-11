#pragma once


// CSimulator
#define INTERVAL_UI 7004

#define		Z_SIZE		10
#define		LINE_SIZE	20
#define		GANTRY_SIZE	40
#define		RGB_ON_SELECT RGB(255,10,10)
#define		RGB_ON_MATERIAL RGB(0,255,0)
#define		RGB_OFF_MATERIAL RGB(200,200,200)
#define		RGB_DEFAULT RGB(0,0,0)
#ifndef GetRValue							
#define GetRValue(rgb)		( (BYTE)(rgb) )	
#endif
#ifndef GetGValue
#define GetGValue(rgb)		( (BYTE)(((WORD)(rgb))>>8) )
#endif
#ifndef GetBValue
#define GetBValue(rgb)		( (BYTE)((rgb)>>16) )
#endif
typedef enum eBOX_TYPE {
	eBT_STACK,
	eBT_MATRIX,
}eBOX_TYPE;
typedef enum eSENSOR_TYPE {
	eST_SENSOR,
	eST_DOOR,
	eST_DOOR_KEY,
	eST_EMO,
	eST_ROT_LEMP,
    eST_LIGHT_CURTAIN,  // KES 추가 2024.3.21
}eSENSOR_TYPE;
typedef enum ST_TYPE{
	ST_Center,
	ST_Top,
	ST_Bot,
	ST_Left,
	ST_Right,

	ST_C = ST_Center,
	ST_T = ST_Top,
	ST_B = ST_Bot,
	ST_L = ST_Left,
	ST_R = ST_Right,
}ST_TYPE;
typedef enum eMOUSE_EVENT{
	eME_NONE,
	eME_DRAW_RECT,
	eME_END,
}eMOUSE_EVENT;
typedef struct T_MOUSE_EVENT {
	eMOUSE_EVENT	eME;
	BOOL	bOn;
	CPoint	ptStart;
	CPoint	ptEnd;
	CRect	rectLast;
	BOOL	bSetLastRect;
	int		nRClick;
	int		iAxisIndex;
}T_MOUSE_DATA;
typedef struct T_LINER_SIZE {
	int	iWidth;
	int	iHeight;
}T_LINER_SIZE;
typedef struct T_GANTRY_SIZE {
	int	iSize;
	int	iWidth;
	int	iHeight;
}T_GANTRY_SIZE;

typedef struct T_SIMULATOR{
	double	dMax;
	double	dMin;
	double	dPos;
	CRect	rectBound;
	CSize	sizeRect;
	int		eType;
	BOOL	bVet;
	int		iBaseModule;
	BOOL	bReverse;
	BOOL	bOn;
	BOOL	bSelect;
	BOOL	bDrawGideStage;
	BOOL	bDrawTable;
	CSize	sizeTable;
	CSize	Matrix;
	CPoint	ptLinkOffset;
	ST_TYPE stType;
	ST_TYPE stLinkType;
	eBOX_TYPE btType;
	std::vector<BOOL> TableItems;
	CString strPath;
	CImage	img;
	DWORD	dwTick;
	BOOL	bAutoUse;
	double  dAutoPosStart;
	double  dAutoPosEnd;
	double  dAutoSPD;
	int		iViewIndex; // 그려지는 순서
	COLORREF	colorBk;
	COLORREF	colorCell;
	int			iAlpha;
	CString		strText;
	COLORREF	colorText;
	DWORD		dwTextAlign;
}T_SIMULATOR;
typedef struct T_SENSOR {
	CPoint ptPosition;
	CPoint ptOffset;
	int	iLinkAxis;
	BOOL bOn;
	BOOL bOnOld;
	BOOL bKey;
	int iAngle;
	DWORD	dwTick;
	BOOL bSelected;
	eSENSOR_TYPE	eSensorType;
}T_SENSOR;
class CSimulator : public CWnd
{
public:
	enum eTYPE{
		eS_LINE_STAGE,
		eS_GANTRY_STAGE,
		eS_Z_STAGE,
		eS_ROTATE_STAGE,
		eS_ROTATE_TABLE,
	};
	DECLARE_DYNAMIC(CSimulator)

public:
	CSimulator();
	virtual ~CSimulator();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	int AddModule(double dMax, double dMin, CRect Rect, int eType, BOOL bVet);
	BOOL DeleteAllModule();
	BOOL DeleteModule(int iModule);
	BOOL DeleteAllSensor();
	int	AddSensor(CPoint ptPos, CPoint ptOffset, int iType, int iLinkAxis);
	BOOL RestItem(int iModule, double dMax, double dMin, CRect Rect, int eType, BOOL bVet, int iBaseAxis);
	BOOL SetPos( int iModule, double dPos);
	BOOL SetLinkModule(int iModule, int iLinkModule);
	BOOL SetLinkType(int iModule, ST_TYPE type);
	BOOL SetOnMaterial(int iModule, BOOL bOn = FALSE);
	BOOL SetSubTableOnMaterial(int iModule, int iTable, BOOL bOn = FALSE);
	BOOL SetTableItem(int iModule, int iTableItem);
	BOOL SetSelectModule(int iModule);
	BOOL SetLinkOffset(int iModule, CPoint pt);
	BOOL SetLinkOffset(int iModule, int ix, int iy);
	BOOL SetGideStage(int iModule, BOOL bDraw);
	
	int GetModule();
	void SetMouseEnvet(eMOUSE_EVENT eMouseEvnet, int iAxis = -1);
	BOOL SetTableSize(int iModule, CSize Size);
	BOOL SetRect(int iModule, CRect rect);
	BOOL GetLastRect(CRect& rect);
	BOOL SetLoadBackGroundImg(CString strPath, CString strPathRev);
	BOOL SetShow(int iModule, BOOL bBase, BOOL bTable = TRUE);
//	BOOL SetBOX_TYPE(eBOX_TYPE eBT = eBT_MATRIX);
	BOOL SetMatrix(int iModule, CSize sizeMatrix);
	BOOL SetSensorAngle(int iModule, int iAngle);
	BOOL SetSensor(int iModule, BOOL bOn, BOOL bKey);
	BOOL SetAutoMove(int iModule, BOOL bUse, double dSpd = 0, double dStart = 0, double dEnd = 0);
	BOOL SetSelectSensor(int iModule);
	BOOL SetViewSwap(int iModule1, int iModule2, BOOL bSwap = TRUE);
	BOOL SetColorBK(int iModule, COLORREF rgbTable = RGB(200, 200, 200));
	BOOL SetColorCell(int iModule, COLORREF rgbCell = RGB(0, 255, 0));
	BOOL SetAlphaTable(int iModule, int iAlpha = 210);
	BOOL SetString(int iModule, CString strText, COLORREF colorText = RGB(255,255,255), DWORD dwAlign = DT_LEFT|DT_TOP);
	// 21.02.03 
	BOOL SetDirection(BOOL bDir);
protected:
	int		m_iVector;
	BOOL	m_iOldSelect;
	DWORD	m_dwRButtonUp;
protected:
	CRect	m_rectClient;
	std::vector<T_SIMULATOR> m_vctModule;
	std::vector<T_SENSOR>	m_vctSensor;
	CCoordTrans m_ct;
	T_LINER_SIZE	m_tLine;
	T_GANTRY_SIZE	m_tGantry;
	T_MOUSE_EVENT	m_tMouseEvent;
	CImage			m_imgBackGround;
	CImage			m_imgBackGroundRev;
	ULONG_PTR		m_GdiplusToken;
	// 21.02.03 
	BOOL			m_bRotateDir;	// 회전 방향 TRUE : 물류진행방향 기준 우측 / FALSE : 물류진행방향 기준 좌측
	CPoint GetStageCenter(CRect rect, CSize sizeTable, BOOL bVet, int eType, double dPercentPos);
	CRect GetBaseRect(int iCrtAxis, CRect rectModule = CRect(), int iDepth = 0);
	CPoint GetLinkPos(int iCrtAxis, CSize sizeBaseTable, CSize sizeCrtTable);
	double GetPercent(int iModule);
	//void DrawLineStage(CDC *pDC, int iModule, CRect rect, CSize sizeTable, BOOL bVet, double dPercentPos, BOOL bOn = FALSE, BOOL bSelect = FALSE, BOOL bStage = FALSE, BOOL bTable = TRUE);
	void DrawLineStage(CDC *pDC, int iModule, CRect rect, double dPercentPos);
	void DrawGantryStage(CDC *pDC, int iModule, CRect rect, CSize sizeTable, BOOL bVet, double dPercentPos, BOOL bSelect = FALSE, BOOL bStage = FALSE, BOOL bTable = TRUE);
	void DrawRotateStage(CDC *pDC, int iModule, CRect rect, double dPercentPos, std::vector<BOOL> &SubTable, BOOL bSelect = FALSE, BOOL bStage = FALSE, BOOL bTable = TRUE);
	//void DrawRotateTable(CDC *pDC, int iModule, CRect rect, double dPercentPos, BOOL bOn = FALSE, BOOL bSelect = FALSE, BOOL bTable = TRUE);
	void DrawRotateTable(CDC *pDC, int iModule, CRect rect, double dPercentPos);
	void DrawZStage(CDC *pDC, int iModule, CRect rect, CSize sizeTable, BOOL bVet, double dPercentPos, BOOL bSelect = FALSE, BOOL bTable = TRUE);
	void DrawConveyor(CDC *pDC, CRect rect, double dPercentPos, BOOL bOn = FALSE, BOOL bReverse = FALSE, BOOL bVet = FALSE, BOOL bSelect = FALSE);
	void DrawRect(CDC *pDC, CRect rect, COLORREF rgbBK = RGB_OFF_MATERIAL, COLORREF rgbLine = RGB_DEFAULT, int iAlpha = 210);
	void DrawRect(CDC *pDC, CRect rect, BOOL bColor);
	void DrawDashRectCross(CDC *pDC, CRect rect);
	void DrawSensor( CDC *pDC, CPoint pt, BOOL bOn, BOOL bSelect = FALSE);
	void DrawDoor( CDC *pDC, CPoint pt, BOOL bOpen, BOOL bKey,double dDoorAngle, double dAngle = 0., BOOL bSelect = FALSE);
	void DrawEMO( CDC *pDC, CPoint pt, BOOL bOn, BOOL bSelect = FALSE);
	void DrawRotateLamp(CDC *pDC, CPoint pt, BOOL bOn, double dAngle, BOOL bSelect = FALSE);
    void DrawLightCurtain(CDC *pDC, CPoint pt, BOOL bOn, BOOL bKey, BOOL bSelect = FALSE);  // KES 추가 2024.3.21
	CPoint GetRotatePoint(CPoint ptCenter, double dTheta);
	void DrawLineRect(CDC *pDC, CRect rect);
	void DrawLineRect(CDC *pDC,  const std::vector<CPoint> &pt, COLORREF rgbBK = RGB_OFF_MATERIAL, COLORREF rgbLine = RGB_DEFAULT, int iAlpha= 210, int iLineWidth =1);
	void DrawCircle(CDC *pDC, CRect rect, COLORREF rgb);
	void DrawMouseEvnet(CDC *pDC);
	void SetinitModule(T_SIMULATOR& Module);
	void CalcAutoPos(int iModule);
	
	int CSimulator::GetDepth(int iBaseAxis, int iDepth = 0);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};



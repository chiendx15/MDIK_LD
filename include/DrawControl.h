#pragma once
#include "default.defines.h"
#pragma warning (disable : 4005)
#pragma warning (disable : 4244)

#include <gdiplus.h>
#pragma comment(lib, "gdiplus")

#define __min(a, b) (((a) < (b)) ? (a) : (b))

#define	RGB_WHITE				RGB( 255,	255,	255)
#define RGB_WHITE_DARK			RGB( 250,	250,	250)

#define RGB_BLACK_LIGHT			RGB( 20,	20,		20)
#define	RGB_BLACK				RGB( 0,		0,		0)

#define	RGB_GRAY				RGB( 125,	125,	125)
#define	RGB_GRAY_DARK			RGB( 75,	75,		75)
#define	RGB_GRAY_LIGHT			RGB( 200,	200,	200)

#define	RGB_RED					RGB( 255,	0,		0)
#define	RGB_RED_LIGHT			RGB( 235,	130,	130)
#define	RGB_RED_DARK			RGB( 170,	10,		10)

#define	RGB_GREEN_LIGHT			RGB( 170,	255,	170)
#define	RGB_GREEN				RGB( 0,		255,	0)
#define	RGB_GREEN_DARK			RGB( 21,	111,	48)

#define	RGB_BLUE				RGB( 0,		0,		255)
#define RGB_BLUE_LIGHT			RGB( 210,	210,	255)
#define RGB_BLUE_DARK			RGB( 30,	30,		110)
#define RGB_STILL_BLUE			RGB( 77,	96,		130)
#define	RGB_SKY					RGB( 120,	120,	255)
#define RGB_SKY_LIGHT			RGB( 170,	170,	255)

#define	RGB_YELLOW				RGB( 255,	255,	0)
#define RGB_DARK_GOLD			RGB( 184,	134,	11)
#define RGB_GOLDENROD			RGB( 218,	165,	32)
#define	RGB_ORANGE				RGB( 255,	50,		0)
#define	RGB_ORANGE_LIGHT		RGB( 255,	170,	60)
#define RGB_ORANGE_DARK			RGB( 230,	130,	0)

#define RGB_CYAN				RGB( 0,		255,	255)
#define RGB_VIOLET				RGB( 180,	0,		180)
#define RGB_BK					RGB(  50,	 50,	 50)
#define RGB_MINT_DARK			RGB( 135,	 215,	180)
#define RGB_PINK				RGB(251,	127,	159)
#define RGB_VIOLET_DARK			RGB(160,	135,	212)

#define RGB_RED_LIGHT			RGB(250, 230, 230)
#define RGB_BLUE_LIGHT			RGB(230, 230, 250)
#define RGB_POP_BK				RGB(  90,	 90,	 90)
#define RGB_MENU_BK				RGB(  50,	 90,	 120)
#define RGB_RGB_FOLDER			RGB( 251,	 235,	 152)

//------ User Edit (DT Setting) ------------------------------------------------------
#define DT_DT					DT_CENTER | DT_VCENTER | DT_WORD_ELLIPSIS
#define DT_FONT					_T("Segoe UI")
#define DT_FONTSIZE				9

#define DT_PEN					RGB_GRAY//_LIGHT//RGB_BLACK
#define DT_BK					RGB_BK
#define DT_TEXT					RGB_WHITE//RGB_BLACK

 //Edit Color
#define DT_EDIT_ENABLE_PEN		RGB_GRAY_LIGHT
#define DT_EDIT_DISABLE_PEN		RGB_GRAY_LIGHT

#define DT_EDIT_ENABLE_BK		RGB_WHITE
#define DT_EDIT_DISABLE_BK		RGB_GRAY_DARK

#define DT_EDIT_ENABLE_TEXT		RGB_BLACK
#define DT_EDIT_DISABLE_TEXT	RGB_WHITE

 // Button
#define DT_BTN_UP_PEN			DT_PEN
#define DT_BTN_DOWN_PEN			RGB_WHITE

#define DT_BTN_UP_BK			RGB(55, 55, 55)
#define DT_BTN_DOWN_BK			RGB_GRAY

#define DT_BTN_UP_TEXT			RGB_WHITE
#define DT_BTN_DOWN_TEXT		RGB_BLACK

#define DT_SENSOR_ON			RGB_GREEN
#define DT_SENSOR_OFF			RGB_RED

#define DT_TOGGLE_ON_BK			RGB_GREEN
#define DT_TOGGLE_OFF_BK		DT_BK

#define DT_TOGGLE_ON_PEN		DT_PEN
#define DT_TOGGLE_OFF_PEN		RGB_WHITE

#define DT_TOGGLE_ON_TEXT		RGB_BLACK
#define DT_TOGGLE_OFF_TEXT		DT_TEXT
//-----------------------------------------------------------------------------------------

typedef enum eGRADATION_OPTION{
	eGO_H = 0,
	eGO_V,
	eGO_M,
}eGRADATION_OPTION;

void DrawLineRect(CDC* pDC, LPRECT rect, COLORREF crPen = DT_PEN, int iTick = 1);
void DrawGroupBox(CDC* pDC, LPRECT rect, LPCTSTR szTitle = NULL, CSize szText = CSize(-1, -1), CPoint ptS = CPoint(15, -12), int iFontSize = DT_FONTSIZE, COLORREF crPen = DT_PEN, COLORREF crText = DT_TEXT, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);
void DrawGroupBox(CDC* pDC, LPRECT rect, LPCTSTR szTitle = NULL, COLORREF crPen = DT_PEN, COLORREF crText = DT_TEXT, CSize szText = CSize(-1, -1), CPoint ptS = CPoint(15, -12), int iFontSize = DT_FONTSIZE, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);
void DrawRoundGroupBox(CDC* pDC, LPRECT rect, LPCTSTR szTitle = NULL, CSize szText = CSize(100, 25), CPoint ptS = CPoint(15, -12), CPoint pt = CPoint(10, 10), int iFontSize = DT_FONTSIZE, COLORREF crPen = DT_PEN, COLORREF crText = DT_TEXT, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);
void DrawFitRect(CDC* pDC, LPRECT rect, COLORREF crPen = DT_PEN);
void DrawRoundRect(CDC* pDC, LPRECT rect, COLORREF crPen = DT_PEN, COLORREF crBack = RGB_WHITE, CPoint pt = CPoint(10, 10));
void DrawLineRoundRect(CDC* pDC, LPRECT rect, COLORREF crPen = DT_PEN, CPoint pt = CPoint(10, 10));
void DrawLine(CDC* pDC, CPoint pt1, CPoint pt2, COLORREF crPen = DT_PEN, int iWidth = 1);
void DrawLine(CDC* pDC, CPoint pt, int iX = 0, int iY = 0, COLORREF crPen = DT_PEN, int iWidth = 1);

void DrawToggleButton(CDC* pDC, LPRECT rect, BOOL bToggleOn = TRUE, LPCTSTR szTextOn = NULL, LPCTSTR szTextOff = NULL, int iFontSize = DT_FONTSIZE, COLORREF crBackOn = DT_TOGGLE_ON_BK, COLORREF crBackOff = DT_TOGGLE_OFF_BK, COLORREF crPenOn = DT_TOGGLE_ON_PEN, COLORREF crPenOff = DT_TOGGLE_OFF_PEN, COLORREF crTextOn = DT_TOGGLE_ON_TEXT, COLORREF crTextOff = DT_TOGGLE_OFF_TEXT, DWORD dwAlign = DT_DT, LPCTSTR szFont = DT_FONT);
void DrawText(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, int iFontSize = DT_FONTSIZE, COLORREF crText = DT_TEXT, DWORD dwAlign = DT_LEFT | DT_VCENTER, LPCTSTR szFont = DT_FONT);
void SelectedDrawText(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, int iFontSize = DT_FONTSIZE, COLORREF crPen = DT_PEN, COLORREF crBack = DT_BK, COLORREF crText = DT_TEXT, DWORD dwAlign = DT_DT, LPCTSTR szFont = DT_FONT);
void SelectedDrawInt(CDC* pDC, LPRECT rect, int iValue = 0, int iFontSize = DT_FONTSIZE, COLORREF crPen = DT_PEN, COLORREF crBack = DT_BK, COLORREF crText = DT_TEXT, DWORD dwAlign = DT_DT, LPCTSTR szFont = DT_FONT);
void SelectedDrawDouble(CDC* pDC, LPRECT rect, double dValue = 0., int iPoint = 3, int iFontSize = DT_FONTSIZE, COLORREF crPen = DT_PEN, COLORREF crBack = DT_BK, COLORREF crText = DT_TEXT, DWORD dwAlign = DT_DT, LPCTSTR szFont = DT_FONT);

void SelectedDrawEditText(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, BOOL bEdit = TRUE, int iFontSize = DT_FONTSIZE, DWORD dwAlign = DT_LEFT | DT_VCENTER, LPCTSTR szFont = DT_FONT);
void SelectedDrawEditInt(CDC* pDC, LPRECT rect, int iValue = 0, BOOL bEdit = TRUE, int iFontSize = DT_FONTSIZE, DWORD dwAlign = DT_RIGHT | DT_VCENTER, LPCTSTR szFont = DT_FONT);
void SelectedDrawEditDouble(CDC* pDC, LPRECT rect, double dValue = 0., int iPoint = 3, BOOL bEdit = TRUE, int iFontSize = DT_FONTSIZE, DWORD dwAlign = DT_RIGHT | DT_VCENTER, LPCTSTR szFont = DT_FONT);
void DrawEditBrowse(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, int iFontSize = DT_FONTSIZE, DWORD dwAlign = DT_LEFT | DT_VCENTER, LPCTSTR szFont = DT_FONT);

void DrawButtonUpDown(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, BOOL bBtnDown = FALSE, COLORREF crBackDown = DT_BTN_DOWN_BK, COLORREF crBackUp = DT_BTN_UP_BK, int iFontSize = DT_FONTSIZE, DWORD dwAlign = DT_DT, LPCTSTR szFont = DT_FONT);
void DrawRoundButtonUpDown(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, BOOL bBtnDown = FALSE, CPoint pt = CPoint(10, 10), int iFontSize = DT_FONTSIZE, DWORD dwAlign = DT_DT, LPCTSTR szFont = DT_FONT);
void Draw3dButton(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, BOOL bBtnDown = FALSE, int iTick = 2, COLORREF crText = DT_TEXT, COLORREF crBK = DT_BK, int iFontSize = DT_FONTSIZE, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);

void DrawSensor(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, BOOL bSensor = FALSE, COLORREF crSensorOn = DT_SENSOR_ON, COLORREF crSensorOff = DT_SENSOR_OFF, int iFontSize = DT_FONTSIZE, COLORREF crPen = DT_PEN, COLORREF crText = DT_TEXT, COLORREF crBack = DT_BK, DWORD dwAlign = DT_DT, LPCTSTR szFont = DT_FONT);
void DrawRoundSensor(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, BOOL bSensor = FALSE, CPoint pt = CPoint(10, 10), COLORREF crSensorOn = DT_SENSOR_ON, COLORREF crSensorOff = DT_SENSOR_OFF, int iFontSize = DT_FONTSIZE, COLORREF crPen = DT_PEN, COLORREF crText = DT_TEXT, COLORREF crBack = DT_BK, DWORD dwAlign = DT_DT, LPCTSTR szFont = DT_FONT);
void DrawEllipseSensor(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, BOOL bSensor = FALSE, CPoint pt = CPoint(10, 10), COLORREF crSensorOn = DT_SENSOR_ON, COLORREF crSensorOff = DT_SENSOR_OFF, int iFontSize = DT_FONTSIZE, COLORREF crPen = DT_PEN, COLORREF crText = DT_TEXT, COLORREF crBack = DT_BK, DWORD dwAlign = DT_DT, LPCTSTR szFont = DT_FONT);

enum eMOVE_DIRECT { eMD_UP, eMD_LEFT, eMD_RIGHT, eMD_DOWN, };
void DrawMoveDirect(CDC* pDC, CRect rect, BOOL bClick, eMOVE_DIRECT eDirect, LPCTSTR szText = NULL, BOOL bDrawRect = FALSE);
void Draw3dMoveDirect(CDC* pDC, CRect rect, BOOL bClick, eMOVE_DIRECT eDirect, LPCTSTR szText = NULL, BOOL bDrawRect = TRUE);
void Draw3dGradationButton(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, BOOL bBtnDown = FALSE, int iFontSize = DT_FONTSIZE, int iGradation = eGO_H, COLORREF crText = DT_TEXT,  COLORREF crStartBK= RGB_GRAY_DARK,COLORREF crEndBK= RGB_GRAY_LIGHT, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);
void DrawButtonUpDownMouseMove(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL,int iSelect = -1,int iMouseover = -1, int iEnum = -1, int iFontSize = DT_FONTSIZE, DWORD dwAlign = DT_DT, LPCTSTR szFont = DT_FONT, COLORREF crSelColor = DT_BTN_DOWN_BK, COLORREF crMoveColor = DT_BTN_UP_BK, COLORREF crDefalut = DT_BTN_UP_BK, COLORREF crDefFont = DT_PEN, COLORREF crSelFont = RGB_WHITE, COLORREF crSelPen = DT_PEN, COLORREF crDefPen = DT_PEN);
void Draw3dGradationMouseMoveButton(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, int iSelect = -1, int iMouseover = -1, int iEnum = -1, int iFontSize = DT_FONTSIZE, int iGradation = eGO_H, COLORREF crText = DT_TEXT,  COLORREF crSelStartBK= RGB_VIOLET,COLORREF crSelEndBK= RGB_GRAY_DARK,  COLORREF crMoveStartBK= RGB_SKY,COLORREF crMoveEndBK= RGB_GRAY_DARK,  COLORREF crDefStartBK= RGB_GRAY_DARK,COLORREF crDefEndBK= RGB_GRAY, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);
void DrawRoundButtonUpDownSensor(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, BOOL bBtnDown = FALSE, BOOL bSensor = FALSE, CPoint pt = CPoint(10, 10), int iFontSize = DT_FONTSIZE, int iBoder = FW_HEAVY, DWORD dwAlign = DT_DT, LPCTSTR szFont = DT_FONT);
void DrawTextB(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, int iFontSize = DT_FONTSIZE, int iWeight = FW_HEAVY, COLORREF crText = DT_TEXT, DWORD dwAlign = DT_LEFT | DT_VCENTER, LPCTSTR szFont = DT_FONT);
void SelectedDrawTextB(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL,int iBoder = FW_HEAVY, int iFontSize = DT_FONTSIZE, COLORREF crPen = DT_PEN, COLORREF crBack = DT_BK, COLORREF crText = DT_TEXT, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);

void DrawTimerButtonUpDown(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, BOOL bBtnDown = FALSE, double dSec = 0., int iFontSize = DT_FONTSIZE, DWORD dwAlign = DT_DT, LPCTSTR szFont = DT_FONT);

void Draw3dGradationButtonB(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, BOOL bBtnDown = FALSE, int iFontSize = DT_FONTSIZE, int iBoder = FW_HEAVY, int iGradation = eGO_H, COLORREF crText = DT_TEXT,  COLORREF crStartBK= RGB_GRAY_DARK,COLORREF crEndBK= RGB_GRAY_LIGHT, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);
void Draw3dGradationMouseMoveButtonB(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, int iSelect = -1, int iMouseover = -1, int iEnum = -1, int iFontSize = DT_FONTSIZE, int iBoder = FW_HEAVY, int iGradation = eGO_H, COLORREF crText = DT_TEXT, COLORREF crSelStartBK = RGB_VIOLET, COLORREF crSelEndBK = RGB_GRAY_DARK, COLORREF crMoveStartBK = RGB_SKY, COLORREF crMoveEndBK = RGB_GRAY_DARK, COLORREF crDefStartBK = RGB_GRAY_DARK, COLORREF crDefEndBK = RGB_GRAY, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);
	 
void DrawImageButtonUpDownMouseMoveB(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, int iSelect = -1, int iMouseover = -1, int iEnum = -1, int iFontSize = DT_FONTSIZE, int iBoder = FW_HEAVY, CImage& img = CImage(), COLORREF crPen = DT_PEN, COLORREF crText = DT_TEXT, COLORREF crBack = DT_BK);
void DrawTimerImageButtonUpDownMouseMoveB(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, double dSec = 0., int iSelect = -1, int iMouseover = -1, int iEnum = -1, int iFontSize = DT_FONTSIZE, int iBoder = FW_HEAVY, CImage& img = CImage(), COLORREF crPen = DT_PEN, COLORREF crText = DT_TEXT, COLORREF crBack = DT_BK);
void DrawGradationTimerImageButtonUpDownMouseMoveB(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, double dSec = 0., int iSelect = -1, int iMouseover = -1, int iEnum = -1, int iFontSize = DT_FONTSIZE, int iBoder = FW_HEAVY, CImage& img = CImage(), COLORREF crPen = RGB_WHITE, COLORREF crText = RGB_BLACK, COLORREF crBack = RGB_WHITE, COLORREF crSelStart = RGB_WHITE, COLORREF crSelEnd = RGB_GREEN, COLORREF crDefStart = RGB_WHITE, COLORREF crDefEnd = RGB_GREEN);

void DrawGradationTimerImageLightButtonUpDownMouseMoveB(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, BOOL bCheck = FALSE,double dSec = 0., int iSelect = -1, int iMouseover = -1, int iEnum = -1, int iFontSize = DT_FONTSIZE, int iBoder = FW_HEAVY, CImage& img = CImage(), COLORREF crPen = RGB_WHITE, COLORREF crText = RGB_BLACK, COLORREF crBack = RGB_WHITE, COLORREF crSelStart = RGB_WHITE, COLORREF crSelEnd = RGB_GREEN, COLORREF crDefStart = RGB_WHITE, COLORREF crDefEnd = RGB_GREEN, BOOL bTick = TRUE);

void DrawInOutButton(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, int iFontSize = DT_FONTSIZE, BOOL bIn = FALSE, BOOL bOut = FALSE);

void DrawGradationTimerButtonUpDownMouseMoveB(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, double dSec = 0., int iSelect = -1, int iMouseover = -1, int iEnum = -1, int iFontSize = DT_FONTSIZE, int iBoder = FW_HEAVY, COLORREF crPen = RGB_WHITE, COLORREF crText = RGB_BLACK, COLORREF crBack = RGB_WHITE, COLORREF crSelStart = RGB_WHITE, COLORREF crSelEnd = RGB_GREEN, COLORREF crDefStart = RGB_WHITE, COLORREF crDefEnd = RGB_GREEN);

void DrawProgressBar(CDC* pDC, LPRECT rect, int iDir, DWORD dwStart, int iMax, int iSpeed, COLORREF crPen, COLORREF crBack, COLORREF crPar);


void DrawGradationFlashTimerButton(CDC* pDC, LPRECT rect, LPCTSTR szText = NULL, DWORD dwStart = 0, BOOL bClick = FALSE, int iFontSize = DT_FONTSIZE, int iBoder = FW_HEAVY, CImage& img = CImage(), COLORREF crPen = RGB_WHITE, COLORREF crText = RGB_WHITE, COLORREF crSelStart = RGB_WHITE, COLORREF crSelEnd = RGB_GREEN, COLORREF crDefStart = RGB_WHITE, COLORREF crDefEnd = RGB_GREEN, BOOL bSensor = FALSE);

void DrawImage(CDC* pDC, CRect rect, Gdiplus::Image* img);

// 2021.07.05 MDI 버튼 추가 (3D 효과 추가함)
// 메뉴 버튼 (사각형 윤곽선 + 텍스트) -> 클릭 혹은 마우스 이동시 색상 변경
void DrawMenuButton(CDC* pDC, CRect rect, LPCTSTR szText = nullptr, int iSelect = -1, int iMouseOver = -1, int iEnum = -1, int iFontSize = DT_FONTSIZE, COLORREF crBkDefault = RGB_GRAY_LIGHT, COLORREF crBkSelect = RGB_ORANGE_DARK/*RGB_DARK_GOLD*/, COLORREF crBkMouseOver = RGB_WHITE_DARK, COLORREF crText = RGB_BLACK, COLORREF crPen = RGB_BLACK, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);

// 메인창 토글 버튼
void DrawMainToggleButton(CDC* pDC, CRect rect, bool bOn = false, LPCTSTR szOnText = nullptr, LPCTSTR szOffText = nullptr, int iSelect = -1, int iMouseOver = -1, int iEnum = -1, int iFontSize = DT_FONTSIZE, COLORREF crBkDefault = RGB_GRAY_LIGHT, COLORREF crBkSelect = RGB_ORANGE_DARK/*RGB_SKY*/, COLORREF crBkMouseOver = RGB_WHITE_DARK, COLORREF crText = RGB_BLACK, COLORREF crPen = RGB_BLACK, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);

// 타이틀 버튼 (메인 버튼 + 이미지) -> 클릭 혹은 마우스 이동시 색상 변경
void DrawTitleButton(CDC* pDC, CRect rect, LPCTSTR szText = nullptr, CImage& img = CImage(), int iSelect = -1, int iMouseOver = -1, int iEnum = -1, int iFontSize = DT_FONTSIZE, COLORREF crBkDefault = RGB_GRAY_LIGHT, COLORREF crBkSelect = RGB_ORANGE_DARK, COLORREF crBkMouseOver = RGB_WHITE_DARK, COLORREF crText = RGB_BLACK, COLORREF crPen = RGB_BLACK, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);

// 시간따라서 채워지는 타이틀 버튼 (메인 버튼 + 이미지 + 시간 경과 그라데이션) -> 클릭 혹은 마우스 이동시 색상 변경
void DrawTimeFillTitleButton(CDC* pDC, CRect rect, LPCTSTR szText = nullptr, CImage& img = CImage(), int iSelect = -1, int iMouseOver = -1, int iEnum = -1, __uint64 currentTime = 0, __uint64 completeTime = 3000, int iFontSize = DT_FONTSIZE, COLORREF crBkDefault = RGB_GRAY_LIGHT, COLORREF crBkSelectDefault = RGB_ORANGE_DARK, COLORREF crBkMouseOver = RGB_WHITE_DARK, COLORREF crText = RGB_BLACK, COLORREF crPen = RGB_BLACK, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);

// 깜빡이는 타이틀 버튼(메인 버튼 + 이미지 + 깜빡이는 효과) -> 클릭, 마우스 이동에 변화 없음
void DrawBlinkTilteButton(CDC* pDC, CRect rect, LPCTSTR szText = nullptr, CImage& img = CImage(), __uint64 startTime = 0, __uint64 blinkTime = 500, int iFontSize = DT_FONTSIZE, COLORREF crBkDefault = RGB_GRAY_LIGHT, COLORREF crBkBlink = RGB_ORANGE_DARK, COLORREF crText = RGB_BLACK, COLORREF crPen = RGB_BLACK, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);

// 선택한 enum, 이동 중 enum, 버튼 enum, 점등 상태, 점멸 상태, 마우스 버튼 누른 시간, 버튼 완료까지 시간, 점멸 상태 체크 시작 시간, 점멸 시간, 기본 배경색, 점등 색, 마우스 오버 색
// 점멸, 점등 중에는 이동 중에 따른 색 변경 없음 (기본 상태에서만 변경)
void DrawBlinkTimeFillTilteButton(CDC* pDC, CRect rect, LPCTSTR szText = nullptr, CImage& img = CImage(), int iSelect = -1, int iMouseOver = -1, int iEnum = -1, int iFontSize = DT_FONTSIZE, bool bOn = false, bool bOnOff = false, __uint64 currentTime = 0, __uint64 completeTime = 3000, __uint64 startTime = 0, __uint64 blinkTime = 500, COLORREF crBkDefault = RGB_GRAY_LIGHT, COLORREF crBkSelectTime = RGB_ORANGE_DARK, COLORREF crBkMouseOver = RGB_WHITE_DARK, COLORREF crText = RGB_BLACK, COLORREF crPen = RGB_BLACK, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);


void Draw3DRectEffect(CDC* pDC, CRect& rect, bool bSelect);

// 2021.07.20 - 기본 버튼
// 기본 상태 - 기본 Rect 테두리, 기본 테두리 안 색상, 기본 Rect보다 작은 라운드 테두리, 라운드 테두리 내부 색상 그라데이션
// 마우스 오버 상태 - 기본 상태에서 테두리 색상과 기본 테두리 안 색상 밝게 변경
// 선택 상태 - 마우스 오버 상태에서 라운드 테두리 삭제 및 라운드 테두리 내부색상 그라데이션으로 변경
void DrawMDIMenuButton(CDC* pDC, CRect rect, LPCTSTR szText = nullptr, bool bSelected = false, bool bMouseOver = false, int iFontSize = DT_FONTSIZE, COLORREF crBkDefault = RGB_GRAY_LIGHT, COLORREF crBkSelect = RGB_ORANGE_LIGHT, int iBoder = FW_HEAVY, COLORREF crText = RGB_BLACK, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);
void DrawMDIMenuButtonN(CDC* pDC, CRect rect, LPCTSTR szText = nullptr, bool bSelected = false, bool bMouseOver = false, int iFontSize = DT_FONTSIZE, COLORREF crBkDefault = RGB_GRAY_LIGHT, COLORREF crBkSelect = RGB_ORANGE_LIGHT, int iBoder = FW_NORMAL, COLORREF crText = RGB_BLACK, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);

// 2021.07.20 - 이미지 버튼
// 기본 버튼 + 이미지
void DrawMDIImageButton(CDC* pDC, CRect rect, LPCTSTR szText = nullptr, Gdiplus::Image* img = nullptr, bool bSelected = false, bool bMouseOver = false, int iFontSize = DT_FONTSIZE, COLORREF crBkDefault = RGB_GRAY_LIGHT, COLORREF crBkSelect = RGB_ORANGE_LIGHT, COLORREF crText = RGB_BLACK, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);
// 기본 버튼 + 이미지 + 차오르는 그라데이션
void DrawMDIImageTimeFillButton(CDC* pDC, CRect rect, LPCTSTR szText = nullptr, Gdiplus::Image* img = nullptr, bool bOn = false, bool bSelected = false, bool bMouseOver = false, __uint64 currentTime = 0, __uint64 completeTime = 3000, int iFontSize = DT_FONTSIZE, COLORREF crBkDefault = RGB_GRAY_LIGHT, COLORREF crBkSelect = RGB_ORANGE_LIGHT, COLORREF crText = RGB_BLACK, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);
// 기본 버튼 + 이미지 + 점멸
void DrawMDIImageBlinkButton(CDC* pDC, CRect rect, LPCTSTR szText = nullptr, Gdiplus::Image* img = nullptr, bool bSelected = false, bool bMouseOver = false, __uint64 startTime = 0, __uint64 blinkTime = 500, int iFontSize = DT_FONTSIZE, COLORREF crBkDefault = RGB_GRAY_LIGHT, COLORREF crBkSelect = RGB_ORANGE_LIGHT, COLORREF crText = RGB_BLACK, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);

// 우선순위
// 1. select -> gradation
// 2. bOn = true 점등
// 3. bOnOff = true 점멸
// 4. bOn = false, bOnOff = false 기본
void DrawMDIImageBlinkFillButton(CDC* pDC, CRect rect, LPCTSTR szText = nullptr, Gdiplus::Image* img = nullptr, bool bOn = false, bool bOnOff = false, bool bSelected = false, bool bMouseOver = false, __uint64 currentTime = 0, __uint64 completeTime = 3000, __uint64 startTime = 0, __uint64 blinkTime = 500, int iFontSize = DT_FONTSIZE, COLORREF crBkDefault = RGB_GRAY_LIGHT, COLORREF crBkSelect = RGB_ORANGE_LIGHT, COLORREF crText = RGB_BLACK, DWORD dwAlign = DT_CENTER | DT_VCENTER, LPCTSTR szFont = DT_FONT);


void DrawMDIIOButton(CDC* pDC, CRect rect, LPCTSTR szText, bool bSelected, bool bMouseHover, bool bOn, bool bWrite);


void DrawMDIMoveDirect(CDC* pDC, CRect rect, bool bSelected, bool bMouseHover, eMOVE_DIRECT eDirect, LPCTSTR szText = NULL);
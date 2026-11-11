
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif
#include "Resource.h"       // 주 기호입니다.

#include "DrawControl.h"
#include "RGBColor.h"

#include "misc/Ado2.h"
#include "UserInterface/Main/MainStatus/NumericKeypadWnd.h"
#include "ChangeParamDlg.h"
#include "UserInterface\Menu\User\UserManager.h"


//#include "misc/DrawSymbol.h"
//#include "misc/MemDC.h"

#define	__USER_GRADE__			sm.DeviceParameter->eUserGrade
#define __SEND_IP_COMMAND__		g_ipClient.SendIPCommand
#define __POST_IP_COMMAND__		g_ipClient.PostIPCommand

//>> KES 추가 2024.05.23 - 티칭 포지션 갯수 바뀌면 업데이트 해줘야 함
#define MAX_TEACH_POS_PPBOX_EV			2
#define MAX_TEACH_POS_GLASS_LOAD_TR		8
//<<

#define SHOW_KEYPAD_EDIT(Wnd, eSelectControl, Value)\
	if (sm.Option->tKeyPad.fKeyPadUseMode && !sm.Option->tKeyPad.fShowKeyPad) {\
		g_pwndNumber->SetDataValue(&Value,\
			[Wnd]()\
			{\
				Invalidate();\
			});\
		g_pwndNumber->ShowWindow(SW_SHOW);\
	}\
	else if (!sm.Option->tKeyPad.fKeyPadUseMode)\
		EditStart(Wnd, m_rectC[eSelectControl], Value);\

typedef enum {
	eMAIN_MENU_NONE					= -1,
	eMAIN_MENU_1					,
	eMAIN_MENU_2					,
	eMAIN_MENU_3					,
	eMAIN_MENU_4					,
	eMAIN_MENU_5					,
	eMAIN_MENU_6					,
	eMAIN_MENU_7					,
	eMAIN_MENU_8					,
	eMAIN_MENU_9					,
	eMAIN_MENU_10					,
	eMAIN_MENU_11					,
	eMAIN_MENU_12					,
	eMAIN_MENU_13					,
	eMAIN_MENU_14					,
	eMAIN_MENU_END					,
// Menu가 달라질 경우 아래에서 매칭시켜주세요.
	eMM_AUTO						= eMAIN_MENU_1	,	
	eMM_MANUAL						= eMAIN_MENU_2	,
	eMM_RECIPE_EDIT					= eMAIN_MENU_3	,
	eMM_RECIPE_SELECT				= eMAIN_MENU_4	,
	eMM_PPBOX						= eMAIN_MENU_5	,
	eMM_MONITOR					    = eMAIN_MENU_6	,
	eMM_PARAMETER					= eMAIN_MENU_7	,
    eMM_LOG	        				= eMAIN_MENU_8	,
    eMM_PM_MODE					    = eMAIN_MENU_9	,
    eMM_USER    					= eMAIN_MENU_10	,
	eMM_END							= eMAIN_MENU_END,
} eMAIN_MENU;

typedef enum {
	eSUB_MENU_NONE = -1,
	eSUB_MENU_1,
	eSUB_MENU_2,
	eSUB_MENU_3,
	eSUB_MENU_4,
	eSUB_MENU_5,
	eSUB_MENU_6,
	eSUB_MENU_7,
	eSUB_MENU_8,
	eSUB_MENU_9,
	eSUB_MENU_10,
	eSUB_MENU_11,
	eSUB_MENU_12,
	eSUB_MENU_13,
	eSUB_MENU_END,
} eSUB_MENU;

enum eIDD  {
	IDD_MAIN_TITLE					= 10000,
	IDD_MAIN_TIME					,
	IDD_MAIN_MENU					,
	IDD_MAIN_SUB_MENU				,
	IDD_MAIN_EXIT					,
	IDD_MAIN_STATUS					,
	IDD_MAIN_RECIPE					,
	IDD_MAIN_MANAGEMENT				,
	IDD_MAIN_PROCESS_DATA			,
	IDD_MAIN_TRANSFER_SIGNAL		,
	IDD_MAIN_PARAMETER				,
	IDD_MAIN_MEASUREMENT			,
	IDD_SUB_SYSTEM_STATUS			,

	IDD_MAINTENANCE_IO_FRONT		,
	IDD_MAINTENANCE_IO_REAR			,
	IDD_MAINTENANCE_MOTOR_STATUS	,

	IDD_MOTOR_STATUS				,
	IDD_STATUS_WORKINFO				,
	IDC_ALARM						,
	IDC_IO_CONTROL					,
	IDC_DXF_MOTION					,
	IDC_RECIPE_DXF					,
	ID_CBO_LAYER_ALIGNMENT			,
	ID_CBO_LAYER_PROCESS			,
	IDC_MOTOR_CONTROL				,
	IDD_END							,
};
typedef enum {
	INTERVAL_START_PROCESS			= 30000,
	INTERVAL_ALARM					,
	INTERVAL_MAIN_MENU				,
	INTERVAL_TITLE					,
	INTERVAL_MAIN_STATUS			,
	INTERVAL_SUB_STATUS				,
	INTERVAL_STATUS					,
	INTERVAL_RECIPE					,
	INTERVAL_IO_STATUS				,
	INTERVAL_LASER_STATUS			,
	INTERVAL_INITIALIZE				,
} eINTERVAL;

//>> KES 추가 2024.05.23
typedef enum {
	eMB_NONE,

	eMB_PPBOX_EV_TEACH_POS,
	eMB_PPBOX_EV_TEACH_POS_END = eMB_PPBOX_EV_TEACH_POS + MAX_TEACH_POS_PPBOX_EV - 1,

	eMB_GLASS_LD_TR_TEACH_POS,
	eMB_GLASS_LD_TR_TEACH_POS_END = eMB_GLASS_LD_TR_TEACH_POS + MAX_TEACH_POS_GLASS_LOAD_TR - 1,
	
	nMB,
} eMOVE_BUTTON;
//<<

class CUIApp : public CWinAppEx {
public:
	CUIApp() noexcept;
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();
	virtual void	PreLoadState();
	virtual void	LoadCustomState();
	virtual void	SaveCustomState();

// 구현입니다.
	BOOL			m_bHiColorIcons;

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
protected:
	BOOL							m_bCreatedWindows;														// All Windows Create Check
	std::unique_ptr<std::thread>	m_cimIPLoopThread = nullptr;
public:
	BOOL							GetCreatedWindows() { return m_bCreatedWindows; }
	CUserManager					m_UserManager;
};

extern CUIApp				theApp;
extern int					g_eSelectedMainMenu ;
extern int					g_eSelectedSubMenu	;
extern DStringTable			g_stIO;
// KES 2024.05.23 g_stMotor, g_stDecription 선언위치 SharedFunction으로 이동
extern CRecipe<T_MDI_RECIPE> g_recipes;
extern bool					g_closeIPLoop;
extern ULONGLONG			g_iLogout_time;

extern void					DrawButton			(CDC* pDC, LPRECT lrect, LPCTSTR szText=NULL	,eEVENT_COLOR event=eEC_DEFAULT);
extern void					SendRecipe			();
extern CNumericKeypadWnd*	g_pwndNumber;

extern BOOL					g_bUIRotateDirection;		// UI 회전 방향 : 물류 진행 방향 기준 TRUE Right 에서 볼때 / FALSE Left 에서 볼때
extern std::unique_ptr<CADODatabase> g_db;
extern BOOL OpenDB();
extern int g_CimReport;
extern int g_PC;
extern LPCTSTR moduleNameStep[20];

extern CMoveButtonStatus g_MoveButtonStatus;	// KES 추가 2024.05.23

extern T_POS_PARAM PPBoxEV[2];
extern T_POS_PARAM GlassLoadTrasnfer[8];

// 2021.12.10 UpdateECID, GetRecipeParam 위치 변경

typedef struct T_PPID_PARAM {
	int iParam;
	CString Name;
	CString Value;
} T_PPID_PARAM;

extern std::vector<std::pair<CString, CString>> GetRecipeParam(T_MDI_RECIPE* recipe);
extern std::vector<T_PPID_PARAM> GetRecipeParam(int iRecipe);
extern BOOL SetRecipeData(int iRecipe);
extern void Update_ECID();


// PPID
extern BOOL IsPPID(LPCTSTR lpPPID, int iPPID_NUMBER, const eEQP_TYPE _type);
extern BOOL IsPPID(int iPPID_NUMBER);
extern BOOL IsPPID(LPCTSTR lpPPID);
extern BOOL IsPPID_REPORT(int iPPID);
extern void Update_PPID_PARAM(int iPPID);

//<< 2024.04.12 LBH - DB
// Message
//extern std::pair<bool, std::vector<T_TERMINAL_MSG>> LoadTerminalMSG(const eEQP_TYPE _type);
extern std::pair<bool, std::vector<T_MESSAGE>> LoadMSG(const eEQP_TYPE _type, const eMSG_TYPE _eMSG_TYPE, BOOL _bConfirmCheck = TRUE);	// KES 수정 2024.07.21 - History 조회 기능을 위해 _bConfirmCheck 인자 추가

// Lot
extern std::pair<bool, T_LOT_INFO> LoadLotInfo(const eEQP_TYPE _type, CString strCSTID);
extern void DeleteLotInfo(const eEQP_TYPE _type, CString strCstID);
extern void DeleteTrayInfo(const eEQP_TYPE _type, CString strTrayID);
extern void DeleteCellInfo(const eEQP_TYPE _type, CString strCellID);
//>>

// 2024.07.06 LBH - CIM Report 부분. {{session, Command}, seqstatus}
extern std::map<std::pair<CStringA, CStringA>, eSEQUENCE_STATE> g_mapSEQ_CIM;
extern std::map<std::pair<CStringA, int>, eSEQUENCE_STATE> g_mapAlarmReport;
extern void	ManualStop();
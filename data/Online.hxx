#pragma once
#include "Mocha.type.hxx"
#include "Online.hpp"

#pragma pack(push, 1)
// EFID Equipment's Function

typedef struct T_FUNCTION {
	char szEFID[3];
	char szEFST[11];
	// UI화 해야함.
	// 보고 때문에 Current, New 해야 할 수도 있음.
	// char szEFST_Current[10];
	// char szEFST_New[10]; // Change 보고 완료 후 Ack 받으면 DB 업데이트 하는 방식으로 사용.
} T_FUNCTION;

typedef struct T_EQP_FUNCTION {
	T_FUNCTION Item[nEFID];
} T_EQP_FUNCTION;

// RMS PPID OFFSET
typedef struct T_PPID_OFFSET {
	double dPPID_OFFSET[MAX_PPID_OFFSET];
} T_PPID_OFFSET;

// OPCall, EQP Interlock, Unit Interlcok
typedef struct T_MESSAGE {
	BOOL bConfirm;
	char szOCCURRED[10]; // 발생 범주 (Opacll [6], Interlock[9])
	char szMODULEID[41];
	char szID[41];
	char szMSG[121];
	char szTIME[51];
} T_MESSAGE;

// ----------------------------- Cassette -----------------------------
// ----------------------------- Cassette -----------------------------
// ----------------------------- Cassette -----------------------------
typedef struct {
	char szID[41];
	char szTYPE[5];
	char szPPID[41];
	char szPRODUCT[41];
	char szSTEPID[41];
	char szS_COUNT[5];
	char szC_COUNT[5];
	char szPORTNO[5]; // 필요 있을까?

	char szREPLYCODE[11];
	char szREPLYTEXT[121];
} T_CASSETTE_INFO;

typedef struct T_SPECIFIC_VALIDATION_ITEM {
	char szITEM_NAME[41];
	char szITEM_VALUE[41];
} T_SPECIFIC_VALIDATION_ITEM;


//<< 2024.04.15  - LBH 메모리로 따로 가지고 있어야 할것 같음.
// 설비에 특정 물류나 용기가 투입되어 정합성 확인이 필요한 시점에 보고.
// S3F103, S3F109 내부 내용 몇개 데이터 트렌스퍼로 주고 받도록 처리해야 할듯.
typedef struct T_SPECIFIC_VALIDATION_DATA {
	// 혹시 몰라서 추가.
	char szCARRIERID[41];	// LOTID
	// 
	char szUNIQUEID[201];	// CSTID
	char szUNIQUETYPE[21];
	char szPRODUCTID[41];
	char szPRODUCTSPEC[41];
	char szPRODUCT_TYPE[5];	//EE/PP/RR/NR/DM
	char szPRODUCT_KIND[5];	//TFT/CF/MOD
	char szPPID[41];
	char szSTEPID[41];
	char szCELL_SIZE[17];
	char szCELL_THICKNESS[17];
	char szCELLINFORESULT[3];
	char szINS_COUNT[11];
	char szCOMMENT[41];

	int nITEM;
	T_SPECIFIC_VALIDATION_ITEM ITEMs[MAX_SPECIFIC_VALIDATION_ITEM];
	
	char szREPLYSTATUS[5];
	char szREPLYTEXT[121];
} T_SPECIFIC_VALIDATION_DATA;

typedef struct T_CELL_ITEM {
	char szITEM_NAME[41];
	char szITEM_VALUE[81];
} T_CELL_ITEM;

typedef struct T_ATTR_ITEM {
	char szATTRID[41];
	char szATTRDATA[41];
} T_ATTR_ITEM;

typedef struct T_EQP_ATTR_DATA {
	// PRODUCTID, STEPID, TRAYQTY, CELLQTY 정보 내려옴.
	int nATTR;
	T_ATTR_ITEM ITEMs[MAX_ATTR];
} T_EQP_ATTR_DATA;

// ----------------------------- PORT -----------------------------
// ----------------------------- PORT -----------------------------
// ----------------------------- PORT -----------------------------
typedef struct {
	//<< LBH
	char szNO[5];							// ID로 사용. ex) US01...
	eAVAILABILITY_STATE eAvailabilityState;	// char szAVAILABLESTATE[2];
	eACCESS_MODE eAccessMode;				// char szACCESSMODE[2];
	eTRANSFER_STATE eTransfer;				// char szTRANSFERSTATE[2];
	ePROCESSING_STATE eProcessingState;		// char szPROCESSINGSTATE[2];
	char szJOBID[41];						// "PRELOAD", "", Cover Tray ID, Cassette ID.
	char szJOBTYPE[21];						// "EMPTYTRAY", "BATCHTRAY"
	char szREASONCODE[21];					// 사유 코드
	char szDESCRIPTION[41];					// State에 관련 된 설명
	 //T_CASSETTE_INFO	Cassette;			// 필요 있을지?
	//>>
} T_PORT_INFO;


// ----------------------------- CIM Layer -----------------------------
// ----------------------------- CIM Layer -----------------------------
// ----------------------------- CIM Layer -----------------------------
typedef struct T_MODULE_STATE
{
	eAVAILABILITY_STATE	eAvailabilityState;
	eINTERLOCK_STATE	eInterlockState;
	eMOVE_STATE			eMoveState;
	eRUN_STATE			eRunState;
	char				szREASONCODE[21];
	char				szDESCRIPTION[41];
#ifndef _IP_DATA_INTERNAL_
	T_MODULE_STATE()
	{
		eAvailabilityState	= eAS_UP;
		eInterlockState		= eIS_OFF;
		eMoveState			= eMS_RUNNING;
		eRunState			= eRS_IDLE;
	}
#endif
} T_MODULE_STATE;

typedef struct T_CIM_MODULE_INFO {
	char				szId[41];	
	T_MODULE_STATE		tState;
	T_MODULE_STATE		tState_Old;
	int					eByWho						;
	char				szOperatorId				[16];
} T_CIM_MODULE_INFO;

typedef struct {
	T_CIM_MODULE_INFO		Module				;
} T_MODULE_LAYER_2;

typedef struct {
	T_CIM_MODULE_INFO		Module				;
	int						nLAYER_2			;
	T_MODULE_LAYER_2		LAYER_2				[MAX_MODULE_LAYER_2];
	//int						nCASSETTE			;
	//T_CASSETTE_INFO			CASSETTEs			[MAX_PORT];

	int						nPORT;
	T_PORT_INFO				PORTs[MAX_PORT];
} T_MODULE_LAYER_1;

// 2024.04.09 LBH - EQP 수정.
// LAYER_1 : EQP
// LAYER_2 : UNIT
typedef struct {
	char					szVersion			[7];	// soft version
	char					szOperator			[17];	// 사용자
	
	// Machine Connection Mode 
	// offline = 1, online local, online remote
	eMCMD_CODE				eMachineConnectionMode		;
	eMCMD_CODE				eMachineConnectionMode_Old	;

	// 내부 모듈 레이어
	int						nLAYER_1			;
	T_MODULE_LAYER_1		LAYER_1				[MAX_MODULE_LAYER_1];	

	//T_EQP_FUNCTION			EQPFunction;
	T_PPID_OFFSET			PPID_Offset[MAX_PPID];
} T_HSMS_SYSTEM;

typedef struct{
	wchar_t		szArea[50];		// Alarm 발생 지역
	wchar_t		szText[256];	// Alarm 이름
} T_SYSTEM_ALARM_DATA;

typedef struct {
	BOOL	bOccurred	;	// Alarm Occurred (발생)
	BOOL	bReport		;	// Alarm Report (CIM 보고 여부)
	int		iUnit;	// Unit Alarm 인지 default : -1, CCT_A1, CCT_A2, CCT_B3, CCT_B4

	// Alarm Reset 
	// true = UI에서 알람 리셋해서 알람 클리어 가능한 상태
	// false = 알람 클리어 상황이 성립되기 전까지 클리어가 불가능한 상태
	BOOL		bReset	;	

	UINT		iIndex;			// enum과 동일한 값
	UINT		iALID	;		// Alarm ID : ID + Start Index
	UINT		iALCD	;		// Alarm Code (Light = 1, Heavy = 2)
	char		szALTX	[120];	// Alarm Text
//-- 위는 CIM 보고 Data, 밑에는 추가 데이터
	char		szSeqStep[128];		// 발생위치 (시퀀스 스텝)
	char		szTargetPos[128];	// 모터가 이동하려고 했던 위치
	T_SYSTEM_ALARM_DATA	AlarmData[nLANGUAGE]; // 한글, 영어, 베트남	// KES 추가 2024.3.26

	// string table index
	int			iAlarmDesc; // Alarm 설명 (StringTable에서 가져옴 eLANGUAGE 참조)
	int			iMotorDesc;	// 모터 동작 실패 원인 (StringTable에서 가져옴 eLANGUAGE 참조)
} T_SYSTEM_ALARM_ITEM;

typedef struct {
	int				iCurAlarmIndex;
	int				nALARM;
	T_SYSTEM_ALARM_ITEM		ALARMs[MAX_SYSTEM_ALARM];
} T_SYSTEM_ALARM;

//typedef struct {
//	char				szPPID					[32];
//	int					ePPIDType				;
//	BOOL				bGlassLoaded			;
//	char				szLotID					[32];
//	char				szModelName				[8];
//} T_LOT_PROCESS;

typedef struct {
	char				szDATA_ITEM				[40];
	char				szDATA_VALUE			[40];
} T_DATA_INFO;

typedef struct {
	char				szFLOW_PATH				[40];
	char				szFLOW_MODULEID			[40];
} T_FLOW_INFO;

typedef struct {
	char				szLOT_ID				[21];	
	char				szCELL_ID				[21];
	POINT2D				pt;
	char				szCOORD_X				[21];
	char				szCOORD_Y				[21];
	char				szDEFECT_NAME			[21];
} T_DEFECT_INFO;

// 일단 문서 내용 전부 넣는데 추후 실제 문서와 시나리오에서 사용 안하는 데이터는 삭제 예정
// 추가로 시간 체크용, 지나간 모듈 정보, 발생했던 알람 등 넣어서 사용하세요
// CIM 시나리오에서 보고할 때 공백 없이 사용하는 변수와 공백이 있어도 되는 변수 구분되면 접두어 수정하세요 (공백 가능 sz, 공백 불가능 혹은 공백 없이 보고하는 경우가 있으면 c)

typedef struct T_CELL_INFO {
	char szCST_ID[41];

	// CEID 421 - Track In (Glass, Stick)
	char szCELLID[41];			// MCR Reading (16자리)
	char szPAIRCELLID[41];		// 12자리 + 1(Checksum)
	char szCELLTYPE[41];		// GLASS, STICK, CELL, ....
	char szINDEX[41];			// SCT, ULD Track In/Out 시의 Index
	char szOPTIONINFO[41];
	char szPPID[41];
	char szPRODUCTID[41];
	char szSTEPID[41];
	char szDESCRIPTION[41];

	// CEID 422 - Track Out
	//char szCELLID[41];
	//char szPAIRCELLID[41];
	//char szCELLTYPE[41];
	//char szINDEX[41];
	//char szOPTIONINFO[41];
	//char szPPID[41];
	//char szPRODUCTID[41];
	//char szSTEPID[41];
	//char szDESCRIPTION[4];

	char szOPERATORID[21];
	char szJUDGE[2];	// G, O
	char szREASONCODE[21];
	//char szDESCRIPTION[41];

	// S3F115
	//char szCELLID[41];
	char szLOCATIONNO[5];
	//char szJUDGE[2];
	//char szREASONCOED[21];

	// S3F117
	//char szCELLID[41];
	//char szLOCATIONNO[5];
	//char szJUDGE[2];
	//char szREASONCOED[21];
	char szGRADE[11];
	
	// S3F103, S3F109 는 T_CELL_INFO 가 아닌 다른 메모리로 가지고 있으면 될듯. (첫 Cell 때만 받음)
	
	// S2F43 RCMD : 21, 22
	char szJOBID[41]; // (TrayID, CassetteID....)
	//char szCELLID[41];
	//char szPRODUCTID[41];
	//char szSTEPID[41];
	char szACTIONTYPE[21];	// 별도의 제어가 필요 없을 경우 "NORMAL" 사용 (자동화 사양서)

	// Sequence에서 사용하는 정보
	//>> Glass 에서 사용하던 정보.
	char szEQP_CELLID[41];	// Make한 Cell ID

	int	nStickCount; 	// 스틱 개수
	int	nCellCount;		// 셀 개수

	int iGlassIndex;
	int iGlassMaxCount;

	int nPortType; // 2024.04.17 LBH - A/B Line 구분 용으로 쓰는것 같음.
	//<<
	BOOL				bMaterial;
	char				szSTART_TIME			[20];	
	char				szEND_TIME				[20];
	char				szTACT_TIME				[20];
	char				szCELL_ID_BCR			[41];
	eCELL_INSP_RESULT	eCell_INSP_Result;	// 검사 결과
	eCELL_MCR_RESULT	eCell_MCR_Result;	// 리딩 결과
	eCELL_ID_RESULT		eCell_ID_Result;	// 매칭 결과
	eTRAY_POCKET_RESULT eTray_Pocket_Result;
	int					nDefect;		// 현재 unload picker에서 cim보고 했는지 판단여부로 사용하고 있음. 추후에 변경해야 할듯
	BOOL				bFirstMaterial; // 첫번째 자재 체크
	int					ProcessLine;

	char				szTimeOfRemoval[30]; // 2021.06.14 스크랩 시간 저장

	// 2024.03.19 kms - 셀 결과 (어떤포트로 갈지)
	eJUDGE				eCellJudgeType;
	// 2024.03.21 kms - 프로세스 상태 (필링했는지 검사했는지 가공했는지 등등, 재시작 할경우 해당 프로세스 넘어가기 위함)
	int					eProcessStatus; // eCELL_PROCESS_STATUS를 참조 (장치마다 내용 다르게 써도 됨)
	// 2024.03.21 kms - 셀 택타임
	__uint64			runningStartTime;
	__uint64			runningTime;	

	//<< 2024.06.27 LBH - DCol 관련 데이터.
	POINT2D				ptAlignCuttingOffset;
	double				dAlignScore[2];
	//>>
} T_CELL_INFO;

typedef struct T_TRAY_INFO {
	char				szTRAY_ID				[41];
	char				szCELLQTY				[5];	// CELLQTY

	// Sequence
	char				szSTART_TIME			[20];	
	char				szEND_TIME				[20];
	char				szTACT_TIME				[20];

	int					nCell = 0;
	T_CELL_INFO			CELL_INFOs				[MAX_CELL];
} T_TRAY_INFO;

typedef struct T_LOT_INFO {
	// HSMS 보고
	T_CASSETTE_INFO		CassetteInfo;

	// 
	char				szSTART_TIME			[20];	
	char				szEND_TIME				[20];
	char				szTACT_TIME				[20];

	ePORT_STATE			eState;
	eTRAY_POCKET_RESULT	ePocketInspResult;		// TrayPocket 결과
	int					nAbort;					// 강제배출 예약개수

	int					nTray = 0;
	T_TRAY_INFO			TRAY_INFOs				[MAX_TRAY];
} T_LOT_INFO;

typedef struct T_DCOL_ITEM {
	char szDVNAME[41]; 
	char szValue[41];
} T_DCOL_ITEM;

typedef struct T_DCOL_CELL_INFO {
	T_CELL_INFO cell;
	int nDCOL;
	T_DCOL_ITEM DCOL[nDCOL_ITEM_CCT]; // CCT 보고 개수가 많아서 CCT 껄로
} T_DCOL_CELL_INFO;

typedef struct T_TERMINAL_MSG {
	char szMSG_TYPE[30];
	int iTID;
	char szTIME[50];
	char szMSG[120];
} T_TERMINAL_MSG;

#pragma pack(pop)

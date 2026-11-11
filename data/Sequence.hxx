#pragma once

#include "Inline.hxx"
#include "Online.hxx"
#include "MotorInfos.hxx"
#include "MotorPosition_4.hxx"

#define MAX_MODULE_CELL 1
#define POST_MESSAGE_TIME_OUT 5000
#define MAX_PPBOX_GLASS 50
#pragma pack(push, 1)

/*  Module Communication
//========================================================
	UPPER : table
	THIS : picker
	LOWER : table
	
	//<< UPPER
	S_RECV_ABLE
	S_SEND_ABLE_CHECK
	S_SEND_START_CHECK
	S_RECV_START
	S_RECV_COMPLETE
	S_SEND_COMPLETE_CHECK
	S_RECV_INIT
	S_SEND_INIT_CHECK
	//>>
	
	//<< LOWER
	S_SEND_ABLE
	S_RECV_ABLE_CHECK
	S_RECV_START_CHECK
	S_SEND_START
	S_SEND_COMPLETE
	S_RECV_COMPLETE_CHECK
	S_SEND_INIT
	S_RECV_INIT_CHECK
	//>>
	
	
//========================================================
	UPPER : picker
	THIS : table
	LOWER : picker
	
	//<< UPPER
	S_RECV_ABLE
	S_SEND_ABLE_CHECK
	S_RECV_START
	S_SEND_START_CHECK
	S_SEND_COMPLETE_CHECK
	S_RECV_COMPLETE
	S_SEND_INIT_CHECK
	S_RECV_INIT
	//>>
	
	//<< LOWER
	S_SEND_ABLE
	S_RECV_ABLE_CHECK
	S_SEND_START
	S_RECV_START_CHECK
	S_RECV_COMPLETE_CHECK
	S_SEND_COMPLETE
	S_RECV_INIT_CHECK
	S_SEND_INIT
	//>>
*/

typedef enum {
	eSEQ_IDLE,
	eSEQ_READY,
	eSEQ_START,
	eSEQ_RUN,
	eSEQ_COMPLETED,
	eSEQ_ALARM,
	nSEQ,
} eSEQUENCE_STATE;

typedef enum {
	eMODULE_NONE = 0,
	eMODULE_PC1_BEGIN,
	eMODULE_PC1_LOAD_CONVEYOR_1 = eMODULE_PC1_BEGIN,
	eMODULE_PC1_LOAD_CONVEYOR_2, // Glass Input
	eMODULE_PC1_ELEVATOR_CONVEYOR,
	eMODULE_PC1_UNLOAD_CONVEYOR_1,
	eMODULE_PC1_UNLOAD_CONVEYOR_2,
	eMODULE_PC1_GLASS_TRANSFER, // MG Transfer
	eMODULE_PC1_END,

	// pc 2
	eMODULE_PC2_BEGIN = eMODULE_PC1_END,
	eMODULE_PC2_GLASS_SHIFT_TABLE = eMODULE_PC2_BEGIN,
	eMODULE_PC2_GLASS_PICKER,
	eMODULE_PC2_GLASS_MAIN_TABLE,
	eMODULE_PC2_GLASS_CHUCK,
	eMODULE_PC2_STICK_CUT_TABLE,
	eMODULE_PC2_STICK_BREAK_TABLE,
	eMODULE_PC2_STICK_TRANSFER_1,
	eMODULE_PC2_STICK_TRANSFER_2,
	eMODULE_PC2_END,

	// pc 3
	eMODULE_PC3_BEGIN = eMODULE_PC2_END,
	eMODULE_PC3_STICK_LINESCAN_TABLE = eMODULE_PC3_BEGIN,
	eMODULE_PC3_STICK_PICKER,
	eMODULE_PC3_STICK_MAIN_TABLE_1,
	eMODULE_PC3_STICK_MAIN_TABLE_2,
	eMODULE_PC3_STICK_CHUCK_1,
	eMODULE_PC3_STICK_CHUCK_2,
	eMODULE_PC3_CELL_CUT_TABLE_1,
	eMODULE_PC3_CELL_CUT_TABLE_2,
	eMODULE_PC3_CELL_BREAK_TABLE_1,
	eMODULE_PC3_CELL_BREAK_TABLE_2,
	eMODULE_PC3_END,

	// PC 4
	eMODULE_PC4_BEGIN = eMODULE_PC3_END,
	eMODULE_PC4_STICK_LINESCAN_TABLE = eMODULE_PC4_BEGIN,
	eMODULE_PC4_STICK_PICKER,
	eMODULE_PC4_STICK_MAIN_TABLE_1,
	eMODULE_PC4_STICK_MAIN_TABLE_2,
	eMODULE_PC4_STICK_CHUCK_1,
	eMODULE_PC4_STICK_CHUCK_2,
	eMODULE_PC4_CELL_CUT_TABLE_1,
	eMODULE_PC4_CELL_CUT_TABLE_2,
	eMODULE_PC4_CELL_BREAK_TABLE_1,
	eMODULE_PC4_CELL_BREAK_TABLE_2,
	eMODULE_PC4_END,
	
	// PC 5
	eMODULE_PC5_BEGIN = eMODULE_PC4_END,
	eMODULE_PC5_CELL_TRANSFER_1 = eMODULE_PC5_BEGIN,
	eMODULE_PC5_CELL_TRANSFER_2,

	eMODULE_PC5_PEELING_TABLE_1,
	eMODULE_PC5_PEELING_TABLE_2,
	eMODULE_PC5_PEELING_TABLE_3,
	eMODULE_PC5_PEELING_TABLE_4,

	eMODULE_PC5_CELL_SHIFT_PICKER,

	eMODULE_PC5_LINE_SCAN_TABLE_MAIN_1,
	eMODULE_PC5_LINE_SCAN_TABLE_MAIN_2,

	eMODULE_PC5_CELL_LINE_SCAN_PICKER_1,
	eMODULE_PC5_CELL_LINE_SCAN_PICKER_2,

	eMODULE_PC5_LINE_SCAN_TABLE_SUB_1,
	eMODULE_PC5_LINE_SCAN_TABLE_SUB_2,

	eMODULE_PC5_CELL_BUFFER_PICKER_1,
	eMODULE_PC5_CELL_BUFFER_PICKER_2,

	eMODULE_PC5_CELL_BUFFER_TABLE_1,
	eMODULE_PC5_CELL_BUFFER_TABLE_2,

	eMODULE_PC5_CELL_UNLOAD_PICKER_1,
	eMODULE_PC5_CELL_UNLOAD_PICKER_2,
	eMODULE_PC5_CELL_UNLOAD_PICKER_3,

	eMODULE_PC5_TRAY_PICKER_1,
	eMODULE_PC5_TRAY_PICKER_2,

	eMODULE_PC5_CELL_LIFT_1,
	eMODULE_PC5_CELL_LIFT_2,
	eMODULE_PC5_CELL_LIFT_3,
	eMODULE_PC5_CELL_LIFT_4,
	eMODULE_PC5_CELL_LIFT_5,

	eMODULE_PC5_TRAY_LIFT_1,
	eMODULE_PC5_TRAY_LIFT_2,
	eMODULE_PC5_TRAY_LIFT_3,
	eMODULE_PC5_TRAY_LIFT_4,
	eMODULE_PC5_TRAY_LIFT_5,

	eMODULE_PC5_CONVEYOR_1,
	eMODULE_PC5_CONVEYOR_2,
	eMODULE_PC5_CONVEYOR_3,
	eMODULE_PC5_CONVEYOR_4,
	eMODULE_PC5_CONVEYOR_5,

	eMODULE_PC5_PEELING_TABLE_PROCESS_1,
	eMODULE_PC5_PEELING_TABLE_PROCESS_2,
	eMODULE_PC5_PEELING_TABLE_PROCESS_3,
	eMODULE_PC5_PEELING_TABLE_PROCESS_4,
	eMODULE_PC5_PEELING_1,
	eMODULE_PC5_PEELING_2,

	// 2024.03.21 kms - 안쓰는 모듈임
	/*eMODULE_PC5_LINE_SCAN_TABLE_MAIN_PROCESS_1,
	eMODULE_PC5_LINE_SCAN_TABLE_MAIN_PROCESS_2,
	eMODULE_PC5_LINE_SCAN_MAIN,

	eMODULE_PC5_LINE_SCAN_TABLE_SUB_PROCESS_1,
	eMODULE_PC5_LINE_SCAN_TABLE_SUB_PROCESS_2,
	eMODULE_PC5_LINE_SCAN_SUB,*/

	eMODULE_PC5_END,
	// PC 6
	eMODULE_PC6_BEGIN = eMODULE_PC5_END,
	eMODULE_PC6_CELL_TRANSFER_1 = eMODULE_PC6_BEGIN,
	eMODULE_PC6_CELL_TRANSFER_2,

	eMODULE_PC6_PEELING_TABLE_1,
	eMODULE_PC6_PEELING_TABLE_2,
	eMODULE_PC6_PEELING_TABLE_3,
	eMODULE_PC6_PEELING_TABLE_4,

	eMODULE_PC6_CELL_SHIFT_PICKER,

	eMODULE_PC6_LINE_SCAN_TABLE_MAIN_1,
	eMODULE_PC6_LINE_SCAN_TABLE_MAIN_2,

	eMODULE_PC6_CELL_LINE_SCAN_PICKER_1,
	eMODULE_PC6_CELL_LINE_SCAN_PICKER_2,

	eMODULE_PC6_LINE_SCAN_TABLE_SUB_1,
	eMODULE_PC6_LINE_SCAN_TABLE_SUB_2,

	eMODULE_PC6_CELL_BUFFER_PICKER_1,
	eMODULE_PC6_CELL_BUFFER_PICKER_2,

	eMODULE_PC6_CELL_BUFFER_TABLE_1,
	eMODULE_PC6_CELL_BUFFER_TABLE_2,

	eMODULE_PC6_CELL_UNLOAD_PICKER_1,
	eMODULE_PC6_CELL_UNLOAD_PICKER_2,
	eMODULE_PC6_CELL_UNLOAD_PICKER_3,

	eMODULE_PC6_TRAY_PICKER_1,
	eMODULE_PC6_TRAY_PICKER_2,

	eMODULE_PC6_CELL_LIFT_1,
	eMODULE_PC6_CELL_LIFT_2,
	eMODULE_PC6_CELL_LIFT_3,
	eMODULE_PC6_CELL_LIFT_4,
	eMODULE_PC6_CELL_LIFT_5,

	eMODULE_PC6_TRAY_LIFT_1,
	eMODULE_PC6_TRAY_LIFT_2,
	eMODULE_PC6_TRAY_LIFT_3,
	eMODULE_PC6_TRAY_LIFT_4,
	eMODULE_PC6_TRAY_LIFT_5,

	eMODULE_PC6_CONVEYOR_1,
	eMODULE_PC6_CONVEYOR_2,
	eMODULE_PC6_CONVEYOR_3,
	eMODULE_PC6_CONVEYOR_4,
	eMODULE_PC6_CONVEYOR_5,

	eMODULE_PC6_PEELING_TABLE_PROCESS_1,
	eMODULE_PC6_PEELING_TABLE_PROCESS_2,
	eMODULE_PC6_PEELING_TABLE_PROCESS_3,
	eMODULE_PC6_PEELING_TABLE_PROCESS_4,
	eMODULE_PC6_PEELING_1,
	eMODULE_PC6_PEELING_2,

	// 2024.03.21 kms - 안쓰는 모듈임
	/*eMODULE_PC6_LINE_SCAN_TABLE_MAIN_PROCESS_1,
	eMODULE_PC6_LINE_SCAN_TABLE_MAIN_PROCESS_2,
	eMODULE_PC6_LINE_SCAN_MAIN,

	eMODULE_PC6_LINE_SCAN_TABLE_SUB_PROCESS_1,
	eMODULE_PC6_LINE_SCAN_TABLE_SUB_PROCESS_2,
	eMODULE_PC6_LINE_SCAN_SUB,*/

	eMODULE_PC6_END,
	nMODULEs				= eMODULE_PC6_END,
} eMODULEs;

typedef struct 
{
	// 장비 동작 시작 시간
	DWORD						fStartYear;
	DWORD						fStartMonth;
	DWORD						fStartDay;
	DWORD						fStartHour;
	DWORD						fStartMinute;
	DWORD						fStartSec;
	// 동작한 시간 (초기화 시점은 사용자 요청에 따라서 할 것 + UI에는 일,시,분,초로 표시할 것)
	__uint64					fWorkingTime;
} T_PROCESS_TIME;


//<< 2024.04.16 LBH -
typedef struct T_CIM_DATA_CASSTTE_INFO { // PPBox, US Port - S3F115
	BOOL bDataSet; // Default : FALSE (함수 호출 시 FALSE), 응답 데이터 셋팅 후 TRUE;
	char szREPLYCODE[11]; // PASS/FAIL
	char szCSTID[41];
} T_CIM_DATA_CASSTTE_INFO; // SCT 1, CCT 4

// Track In시
// Cell Type : GLASS, STICK
// Stick 의 경우 g_line
// 해당 번호 초기화 및 Cell ID 미리 넣어놓기) - S2F43에서 데이터 넣어줄 때, Cell ID 검색 후 넣어주도록.
typedef struct T_CIM_DATA_JOB_PROCESS { // SCT : GLASS 1, CCT : STICK 2 - S2F43 : RCMD 21/22
	BOOL bDataSet; // Default : FALSE (함수 호출 시 FALSE), 응답 데이터 셋팅 후 TRUE;
	int iRCMD; // eRCMD_CELL_JOB_PROCESS_START(21), eRCMD_CELL_JOB_PROCESS_CANCEL(22)
	char szJOBID[41];
	char szCELLID[41];
	char szPRODUCTID[41];
	char szSTEPID[41];
	char szACTIONTYPE[21];
} T_CIM_DATA_JOB_PROCESS; // 3개 (SCT 1, CCT2)

// S6F203 -> S3F103 : 
typedef struct T_CIM_DATA_SPECIFIC_VALIDATION {
	BOOL bDataSet; // Default : FALSE (함수 호출 시 FALSE), 응답 데이터 셋팅 후 TRUE;
	char szUNIQUEID[201];
	char szSTEPID[41];
	char szREPLYSTATUS[5];// PASS/FAIL
} T_CIM_DATA_SPECIFIC_VALIDATION; // 1개

// S6F205 -> S3F109
typedef struct T_CIM_DATA_CELL_LOT_INFO {
	BOOL bDataSet; // Default : FALSE (함수 호출 시 FALSE), 응답 데이터 셋팅 후 TRUE;
	char szCELLID[41];
	char szCASSETTEID[41];
	char szBATCHLOT[21];
	char szPRODUCTID[41];
	char szPRODUCT_TYPE[5];
	char szPRODUCT_KIND[5];
	char szPRODUCTSPEC[41];
	char szSTEPID[41];
	char szPPID[21];
	char szCELL_SIZE[17];
	char szCELL_THICKNESS[17];
	char szCOMMENT[41];

	int nITEM;
	T_CELL_ITEM ITEMs[MAX_CELL_ITEM];
} T_CIM_DATA_CELL_LOT_INFO; // 1개

typedef struct T_CIM_DATA_GETATTR {
	BOOL bDataSet; // Default : FALSE (함수 호출 시 FALSE), 응답 데이터 셋팅 후 TRUE;
	char szPRODUCTID[41];
	char szSTEPID[41];
	char szTRAYQTY[41];
	char szCELLQTY[41];

	//2024.06.21 LBH
	char szREPLYCOED[11];
	char szREPLYTEXT[121];
} T_CIM_DATA_GETATTR; // 1개

typedef struct T_CIM_DATA_LABLE {
	BOOL bDataSet; // Default : FALSE (함수 호출 시 FALSE), 응답 데이터 셋팅 후 TRUE;
	//char szCELLID[41];	//	CSTID
	//char szPRODUCTID[41];
	//char szLABELID[201];
	//char szREPLYSTATUS[5];
	//char szREPLYCODE[11];
	//char szREPLYTEXT[121];
} T_CIM_DATA_LABLE;

typedef struct T_CIM_DATA_TRANSFER_SCT {
	T_CIM_DATA_CASSTTE_INFO			PPBox;
	T_CIM_DATA_JOB_PROCESS			JobProcess; // sct : Glass

} T_CIM_DATA_TRANSFER_SCT;

typedef struct T_CIM_DATA_TRANSFER_CCT {
	T_CIM_DATA_JOB_PROCESS			JobProcess; //cct : Stick Picker
	T_CIM_DATA_SPECIFIC_VALIDATION	SpecificValidation[3];	// 0 : Buffer Picker, 1 : ePORT_UI_01, 2 : ePORT_UI_02
	T_CIM_DATA_CELL_LOT_INFO		CellLotInfo;
	T_CIM_DATA_GETATTR				GetAttr;

	T_CIM_DATA_CASSTTE_INFO			Port[ePORT_END]; // Load Port US 01~03
	T_CIM_DATA_CASSTTE_INFO			Assign[ePORT_END];
	T_CIM_DATA_LABLE				Lable[ePORT_END];
} T_CIM_DATA_TRANSFER_CCT;

typedef struct eEQP_MODULE_STATUS {
	eAVAILABILITY_STATE	eAvailabilityState;
	eINTERLOCK_STATE	eInterlockState;
	eMOVE_STATE			eMoveState;
	eRUN_STATE			eRunState;
} eEQP_MODULE_STATUS;

typedef struct eEQP_STATUS {
	eEQP_MODULE_STATUS EQP;
	eEQP_MODULE_STATUS UNIT[2];
} eEQP_STATUS;

typedef struct T_CIM_DATA_TRANSFER {
	T_CIM_DATA_TRANSFER_SCT SCT;
	T_CIM_DATA_TRANSFER_CCT CCT_A;
	T_CIM_DATA_TRANSFER_CCT CCT_B;

	T_EQP_FUNCTION			EQPFunction[nEQP];

	// 2024.07.10 LBH
	eEQP_STATUS				EQPStatus[nEQP];
} T_CIM_DATA_TRANSFER;
//>>
//
//typedef struct T_ONLINE_SEQUENCE_DATA {
//	BOOL bS6F11_CEID262_Reply;
//	BOOL bS6F11_CEID257_Reply;
//} T_ONLINE_SEQUENCE_DATA;

typedef struct 
{
	DWORD						fSimulationMode		;
	DWORD						fAutoMode			;
	DWORD						fCycleStop			;	
	DWORD						fInline				;
	DWORD						fOnline				;
	DWORD						fIsAlarm			;
	DWORD						fPMMode				;

	DWORD						fCIM_StepStop;
	DWORD						fCIM_LoadingStop;	// Interlock 

	DWORD						fCIM_UNIT_LoadingStop[2];	// CCT전용

	__uint64					iTotalProcessCellCount;
	T_PROCESS_TIME				tProcessTime;

	BOOL						bOmronCameraConnect[2];
	char						szCELL_ID[21];

	BOOL						bHomming;
	BOOL						bHomeComplete;

	// 2024.03.26 Add By LBH (Conveyor 2 After CIM Respons Off)
	BOOL						bConveyor1_BoxInStop;
	//<< 2024.4.3 KES 추가 - Online모드 UI에서 삭제. 메모리 위치 DeviceParameter -> Sequence로 변경
	BOOL						bCIMSkip;					// TRUE : Off Line / FALSE : On Line
	//>>

	// 2024.04.16  LBH
	T_CIM_DATA_TRANSFER			CIMData;

	// 2024.06. 20 LBH
	double dGlassTransferOffsetZ;

} T_SEQUENCE;

//typedef struct 
//{
//	BOOL				bHomingLoader;
//	BOOL				bHomingUnLoader;
//	BOOL				bHomingProcess;
//	BOOL				bAlarmOccurred;
//	BOOL				bAlarmHeavy;
//	BOOL				bBuzzerStop;
//	BOOL				bPMMode;
//} T_LOT_PROCESS;	


// ----------------------------- Sequence Module -----------------------------
// ----------------------------- Sequence Module -----------------------------
// ----------------------------- Sequence Module -----------------------------

// 모듈 현재 상태
typedef enum 
{
	// Normal
	eMODULE_STATUS_INITIAL,			// 초기화 완료 상태
	eMODULE_STATUS_INITIALIZING,		// 초기화 중
	eMODULE_STATUS_PREPARING,		// 동작 준비 중
	eMODULE_STATUS_READY,			// 동작 준비 완료
	eMODULE_STATUS_RUN,				// 동작 중
	eMODULE_STATUS_COMPLETED,		// 완료

	// Abnormal
	eMODULE_STATUS_DOWN,			// 알람 상태
	eMODULE_STATUS_STOP,			// 정지
	eMODULE_STATUS_PAUSE,			// 일시 정지
	eMODULE_STATUS_PM,			// PM모드
	eMODULE_STATUS_OR,
	eMODULE_STATUS_GATE,
} eMODULE_STATUS;

typedef enum
{
	eCIM_IDLE,
	eCIM_READY,			// IPMessage(Post) 보내기 전
	eCIM_START,			// IPMessage(Post) 보낸 후
	eCIM_COMPLETE,		// CIM 에서 응답 IPMessage가 온 후
	eCIM_ERROR,
} eMODULE_CIM_REPORT;

typedef enum
{
	eMESSAGE_NONE,
	eMESSAGE_REQUEST,
	eMESSAGE_RESPONSE,
} eMESSAGE_STATUS;

typedef enum 
{
	eBCR_MESSAGE_RETRY,
	eBCR_MESSAGE_INPUT,
	eBCR_MESSAGE_DUPLICATION, // 중복
} eBCR_MESSAGE_RESULT;

// 자재 타입 (원장, 스틱, 셀 등)
typedef enum {
	eMT_ORG_GLASS,
	eMT_STICK,
	eMT_CELL,
	eMT_PAPER,
	eMT_NONE,//24.06.14 KGH EDIT : 초기화 상태 필요하여 추가
}eMATERIAL_TYPE;

// 모듈 상태
typedef struct T_MODULE_STATUS
{
	eMODULE_STATUS	eStatus;		// 현재 모듈 상태
	eMODULE_CIM_REPORT eReportStatus; // CIM 보고 상태
	eMESSAGE_STATUS	eMessageStatus; // 메세지 상태
	WORD			wMessageResult;	// 메세지 처리 결과
	__uint64		MotorTimeout;	// 모터 이동거리 및 속도에 따라 바뀌는 타임 아웃 값 (단위 msec)
	__uint64		stepStartTime;
	__uint64		stepTime;		// 구분 동작 시작 시간 (스탭간 timeout에 사용)
	__uint64		materialStartTime;
	__uint64		materialTime;	// 새로운 자재 작업 시작 시간 (택 타임 체크용)
	__uint64		runningStartTime;
	__uint64		runningTime;	// 모듈 동작 시작 시간 (모듈 동작 시간 체크용)
	WORD			wMaterialType;	// 모듈에서 작업하는 자재 타입 (원장, 셀, 글래스, 트레이 등)
	WORD			wSeqStep;		// 현재 작업 스탭
	WORD			wSeqPrevStep;	// 이전 작업 스탭
	WORD			wRetryCount;	// 현재 재시도 횟수
	int				iLastAlarm;		// 마지막 발생한 알람
	WORD			wMode;			// 정상 모듈 상태 이외의 동작이 필요한 경우 사용할 변수
	WORD			wRepeatCount;	// 현재 반복 동작 횟수 (모듈 내에서 반복하는 동작이 있는 경우 사용)
	WORD			wTargetIndex;	// 현재 모듈이 가르키는 목표 인덱스(ex 로봇 핸드, 트레이 포트 등)
	WORD			wTargetRequest;
	WORD			wTargetRun;
	WORD			nMaterial;			// 현재 모듈이 가지고 있는 자재 갯수
	WORD			nCrtCount;			// 스크라이브 하는 위치
	WORD			eTargetPosition;		// 모듈이 가려는 구간 위치
	WORD			eCurrentPosition;		// 모듈의 현재 구간 위치
	BOOL			bSafety;			// 안전 상태 체크 (현재 모듈을 지나가도 되는지 체크)
	BOOL			bLastMaterial;	
	// 프로세스 상태는 현재 없음. 필요하면 추가하세요
	BOOL			bInputStop; // 투입정지
	BOOL			bOutputStop; // 배출정지
	BOOL			bPause;		// 일시정지
	int				iReserve1;	// 임시
	int				iReserve2;	// 임
	int				iReserve3;
	__uint64		iTimeOut_CIM; // CIM 자재 요청 후 응답 안왓을때 Timeout 확인용 (10초)
} T_MODULE_STATUS;

typedef struct T_MODULE_ETC {
	//<< Align
	POINT2D					Align[2];			// 찾은 얼라인 포인트
	POINT3D					Offset;				// XYT offset 값(Z가 T로 사용)
	//>>

	// << Scribe Offset (SCT, CCT)
	POINT3D					markOffset;
	// >>
	
	T_POS					UnloadX;			// 얼라인 후 계산된 언로드 최종 위치
	T_POS					UnloadY;			// 얼라인 후 계산된 언로드 최종 위치
	T_POS					UnloadT;			// 얼라인 후 계산된 언로드 최종 위치
	T_POS					UnloadZ;			// 얼라인 후 계산된 언로드 최종 위치

	T_POS					CameraX1;			// 얼라인 후 계산된 카메라 확인용 위치
	T_POS					CameraX2;			// 얼라인 후 계산된 카메라 확인용 위치
	T_POS					CameraY;			// 얼라인 후 계산된 카메라 확인용 위치
	T_POS					CameraT;			// 얼라인 후 계산된 카메라 확인용 위치

	T_POS					TargetX;			// 아무 곳에 사용
	T_POS					TargetY;			// 아무 곳에 사용
	T_POS					TargetZ;			// 아무 곳에 사용
	T_POS					TargetT;			// 아무 곳에 사용

	double					dDegree;			// 얼라인 후 계산된 세타(Degree)
	double					dStickOut;			// Cell 튀어나온 노출량
	
	//<< for Unloader
	WORD					wTargetIndex;			// Status구조에 있는 것 외에 추가로 필요한 경우 사용
	//>>
} T_MODULE_ETC;

// 시퀀스에서 사용하는 모듈 정보
typedef struct T_SEQUENCE_MODULE_INFO
{
	eMODULEs				eModule;		// 모듈 번호 (eMODULEs를 쓸까..)
	char					szId[20];		// 아이디
	T_MODULE_STATUS			Status;			// 상태
	T_HS_TO_UPPER_EQUIPMENT	ToUpper;		// 기존 inline.hxx 데이터 사용
	T_HS_TO_LOWER_EQUIPMENT	ToLower;		// 기존 inline.hxx 데이터 사용
	T_CELL_INFO				Glass;			// 원장 정보
	T_CELL_INFO				Cell[MAX_MODULE_CELL];	// 셀 정보
	T_MODULE_ETC			etc;
} T_SEQUENCE_MODULE_INFO;

// Unloader 전용 
typedef struct T_TRAY_PORT {
	T_LOT_INFO				tLotInfo[nTRAY_PORT];
	T_LOT_INFO				tBufferInfo[nTRAY_PORT];
} T_TRAY_PORT;

typedef struct T_CELL_DEFECT_INFO {
	T_DEFECT_INFO		tDefect[4][100];
} T_CELL_DEFECT_INFO;


#pragma pack(pop)




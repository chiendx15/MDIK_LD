// Port 정보는 장비에 따라 변경될 수 있음에 유의할것..
#define MAX_MODULE_LAYER_1	1	// EQP (SCT, CCT)
#define MAX_MODULE_LAYER_2	4	// SCT의 경우 0, CCT의 경우 4. (Unit)
#define MAX_PORT			20		// 상위 모듈 중 PORT 갯수가 가장 많은 곳의 갯수
#define MAX_SYSTEM_ALARM 	2000
#define MAX_TRAY			51  // 실물 50개 뚜겅 1개
#define MAX_CELL			15	// 4 (Load PPBox 때문에 더 많이 잡아야함.)
//#define MAX_DEFECT		5
#define MAX_DATA			20
#define MAX_LAYERS			50
#define MAX_TACT_TIME		10
#define MAX_SLOT			30
#define MAX_PROGRAM			50
#define MAX_PARAM			1000
#define	MAX_PPID			99
#define	MAX_PPID_OFFSET		10
#define MAX_MSG				50
#define MAX_SPECIFIC_VALIDATION_ITEM 30 // ?? 몇개인지 모름.
#define	MAX_ATTR			10// 몇개 인지 모름 일단 4?
#define MAX_CELL_ITEM		10
#define	MAX_TRAY_CELL		4

/*
#define __HSMS__				L"HSMS"
#define __FTP_INFO__			L"FTP_INFO"
#define __MCC_INFO__			L"MCC_INFO"
#define __MODULE_INFO__			L"MODULE_INFO"
#define __EO_ID__				L"EO_ID"
#define __CELL_DEFECT_PATH__	L"CELL_DEFECT_PATH"

#define __HSMS_HOST__			L"Host"
#define __HSMS_ACTIVE__			L"Active"
#define __HSMS_PASSIVE__		L"Passive"
#define __HSMS_TEMPLATE_PATH__	L"TemplatePath"
#define __HSMS_T3_TIMEOUT__		L"T3_Timeout"

#define __HSMS_IP__				L"IP"
#define __HSMS_PORT__			L"Port"
#define __HSMS_USER_NAME__		L"USER_NAME"
#define __HSMS_USER_PASSWORD__	L"USER_PASSWORD"
#define __HSMS_SERVER_PATH__	L"SERVER_PATH"
#define __HSMS_LOCAL_PATH__		L"LOCAL_PATH"

#define MAX_TRID							100
#define MAX_LOADING_PORT					2
#define MAX_UNLOADING_PORT					2
#define MAX_LOADING_UNLOADING_PORT			3

#define PPID_TYPE_EQP													1
#define PPID_TYPE_HOST													2

*/

typedef enum eCELL_PROCESS_STATUS {
	eCPS_NONE,
	eCPS_PEELING,
	eCPS_INSPECTION_REQ,
	eCPS_INSPECTION_RESULT,
} eCELL_PROCESS_STATUS;


typedef enum {
	eMSG_TYPE_OPCALL,
	eMSG_TYPE_INTERLOCK,
	//eMSG_TYPE_UNIT_INTERLOCK,
	nMSG_TYPE,
} eMSG_TYPE;
// EQP Type (SCT - Stick Cut, CCT - Cell Cut)
typedef enum  {
	eEQP_NONE = -1,
	eEQP_SCT,
	eEQP_CCT_A,
	eEQP_CCT_B,
	nEQP,
} eEQP_TYPE;

//<< Judge 
typedef enum {
	eCELL_INSP_NORMAL,
	eCELL_INSP_OK,
	eCELL_INSP_NG,
	eCELL_INSP_DF,
} eCELL_INSP_RESULT;

typedef enum {
	eCELL_MCR_NORMAL,
	eCELL_MCR_OK,
	eCELL_MCR_NG,
} eCELL_MCR_RESULT;

typedef enum {
	eCELL_ID_NORMAL,
	eCELL_ID_OK,
	eCELL_ID_NG,
} eCELL_ID_RESULT;

//<< 2024.06.14 kms - 수정함
typedef enum eTRAY_POCKET_RESULT {
	eTRAY_POCKET_NORMAL,
	eTRAY_POCKET_OK,
	eTRAY_POCKET_NG,
}eTRAY_POCKET_RESULT;
//>>
//>>


// SFCD = State Form Code
typedef enum eSFCD {
	eSFCD_EQUIPMENT_STATE = 1,	// EQP 설비 상태에 정보
	eSFCD_UNIT_STATE = 2,	// UNIT 상태에 대한 정보
	eSFCD_MATERIAL_STATE = 3,	// 자재 상태에 대한 정보
	eSFCD_PORT_STATE = 4,	// 포트 상태에 대한 정보
	eSFCD_FUNCTION_STATE = 5,	// 기능 상태에 대한 정보
} eSFCD;

// RCMD = Remote Control Command
typedef enum eRCMD {
	eRCMD_OPCALL_SEND = 1,						// HOST에서 설비로 Opcall Command Send
	eRCMD_INTERLOCK_SEND = 2,					// HOST에서 설비로 Interlock Command Send
	eRCMD_JOB_PROCESS_START = 4,				// HOST에서 Process 시작 Command Send
	eRCMD_JOB_PROCESS_CANCEL = 8,				// HOST에서 Process 취소 Command Send
	eRCMD_FUNCTION_CHANGE = 10,					// HOST에서 설비로 기능 변경에 대한 Command Send
	eRCMD_TRANSFER_STOP = 11,					// HOST에서 설비로 Interlock Command Send(동작 구분 제어)
	eRCMD_LOADING_STOP = 12,					// HOST에서 설비로 Interlock Command Send(동작 구분 제어)
	eRCMD_STEP_STOP = 13,						// HOST에서 설비로 Interlock Command Send(동작 구분 제어)
	eRCMD_OWN_STOP = 14,						// HOST에서 설비로 Interlock Command Send(동작 구분 제어)
	eRCMD_CONTROL_INFORMATION = 15,				// HOST에서 설비의 동작을 제어하기 위한 Command Send
	eRCMD_CELL_JOB_PROCESS_START = 21,			// Cell Process Start Command
	eRCMD_CELL_JOB_PROCESS_CANCEL = 22,			// Cell Process Cancel Command
	eRCMD_FORCE_TRANSFER_STOP = 41,				// HOST가 필요 시 특정 설비(EQPID & Module ID)에 Interlock 명령을 전송한다
	eRCMD_FORCE_LOADING_STOP = 42,				// HOST가 필요 시 특정 설비(EQPID & Module ID)에 Interlock 명령을 전송한다.
	eRCMD_FORCE_STEP_STOP = 43,					// HOST가 필요 시 특정 설비(EQPID & Module ID)에 Interlock 명령을 전송한다
	eRCMD_FORCE_OWN_STOP = 44,					// HOST가 필요 시 특정 설비(EQPID & Module ID)에 Interlock 명령을 전송한다
	eRCMD_FORCE_INRWELOXK_RELEASE_STOP = 45,	// HOST가 필요 시 특정 설비(EQPID & Module ID)에 Interlock 명령을 전송한다.
} eRCMD;

// CEID = Collected Event Id
typedef enum eCEID {
	eCEID_NONE = 0,

	// RELATED EQUIPMENT
	eCEID_EQUIPMENT_STATUS_CHANGE = 101,
	eCEID_UNIT_STATUS_CHANGE = 102,
	eCEID_CONTROL_STATE_CHANGE_OFF_LINE = 104,	// (OFF_-_LINE_or_INITIALIZED)
	eCEID_CONTROL_STATE_CHANGE_ON_LINE_LOCAL = 105,	// (ON_-_LINE_LOCAL)
	eCEID_CONTROL_STATE_CHANGE_ON_LINE_REMOTE = 106,	// (ON_-_LINE_REMOTE)
	eCEID_PPID_CHANGE = 107,
	eCEID_EQUIPMENT_CONSTANT_PARAMETER_CHANGE = 110,
	eCEID_EQUIPMENT_FUNCTION_CHANGE = 111,

	//_RELATEDIQC_&_TRS_&_EQP_PORT
	eCEID_CARRIER_RELEASE_REQUEST = 256,
	eCEID_CARRIER_RELEASE_COMPLETE = 257,
	eCEID_CARRIER_PROCESS_START = 258,
	eCEID_CARRIER_PROCESS_END = 259,
	eCEID_CARRIER_ASSIGN_REQUEST = 260,
	eCEID_CARRIER_ASSIGN_COMPLETE = 261,
	eCEID_CARRIER_INFORMATION_REQUEST = 262,
	eCEID_CARRIER_INFORMATION_REQUEST_CELL_LINE = 263,

	eCEID_CASSETTE_LOAD_REQUEST = 350,
	eCEID_CASSETTE_LOAD_COMPLETE = 351,
	eCEID_CASSETTE_UNLOAD_REQUEST = 352,
	eCEID_CASSETTE_UNLOAD_COMPLETE = 353,
	eCEID_CASSETTE_INFORMATION_REQUEST = 354,
	eCEID_CASSETTE_MOVE_IN = 355,
	eCEID_CASSETTE_MOVE_OUT = 356,

	//_RELATED_JOB_PROCESS
	eCEID_JOB_PROCESS_START = 301,
	eCEID_JOB_PROCESS_COMPLETE = 302,

	//_RELATED_CELL_PROCESS
	eCEID_CELL_PROCESS_START_FOR_MODULE = 401,
	eCEID_CELL_PROCESS_COMPLETE_FOR_MODULE = 402,
	eCEID_CELL_PROCESS_END_FOR_MODULE = 406,
	eCEID_PAIR_CELL_PROCESS_START = 411,
	eCEID_PAIR_CELL_PROCESS_COMPLETE = 412,
	eCEID_TRACK_IN = 421,
	eCEID_TRACK_OUT = 422,

	//_RELATED_EQUIPMENT_SPECIFIC_PROCESS
	eCEID_OPCALL_CONFIRM = 501,
	eCEID_INTERLOCK_CONFIRM = 502,
	eCEID_UNIT_INTERLOCK_CONFIRM = 514,
	eCEID_EQUIPMENT_LOSS_CODE_REPORT_V110 = 606,
	eCEID_EQUIPMENT_LOSS_CODE_REPORT_V116B = 616,
} eCEID;

// EFID Equipment's Function
//#define MAX_EFID 16
typedef enum eEFID {
	eEFID_NONE = 0,
	eEFID_CELL_TRACKING,				// ON, OFF, TRACE, NOTHING
	eEFID_TRACKING_CONTROL,				// TKIN, TKOUT, BOTH, NOTHING
	eEFID_MATERIAL_TRACKING,			// ON, OFF, NOTHING
	eEFID_CELL_MCR_MODE,				// USE, UNUSE, NOTHING
	eEFID_MATERIAL_MCR_MODE,			// USE, UNUSE, NOTHING
	eEFID_LOT_ASSIGN_INFORMATION,		// AUTO, MANU, NOTHING
	eEFID_AGV_ACCESS_MODE,				// AUTO, MANU, NOTHING
	eEFID_AREA_SENSOR_MODE,				// USE, UNUSE, NOTHING
	eEFID_SORT_MODE,					// USE, UNUSE, NOTHING
	eEFID_INTERLOCK_CONTROL,			// TRANSFER, LOADING, STEP, OWN
	eEFID_LOADER_LOAD_PORT_MCR_MODE,	// ON, OFF, NOTHING
	eEFID_LOADER_USE_PORT_MCR_MODE,		// ON, OFF, NOTHING
	eEFID_UNLOADER_USE_PORT_MCR_MODE,	// ON, OFF, NOTHING
	eEFID_APC_MODE,						// AUTO, MENU
	eEFID_MULTI_PASS_MODE,				// 0 ~ 100
	nEFID,
} eEFID;

// Equipment's Function Status
typedef enum eEFST {
	eEFST_ON = 0,
	eEFST_OFF,
	eEFST_TRACE,
	eEFST_NOTHING,
	eEFST_TKIN,
	eEFST_TKOUT,
	eEFST_BOTH,
	eEFST_USE,
	eEFST_UNUSE,
	eEFST_AUTO,
	eEFST_MANU,
	eEFST_TRANSFER,
	eEFST_LOADING,
	eEFST_STEP,
	eEFST_OWN,
	nEFST,
} eEFST;

typedef enum eCASSETTE_TYPE {
	eCST_FULL			= 1,
	eCST_QUARTER		,
	eCST_CELL			,
	eCST_MASK			,
	eCST_MASK_CASE		,
	eCST_WIRE			,
	//eCST_BOX			,
	//eCST_TRAY			,
	//eCST_SMALL_BOX		,
	//eCST_LARGE_BOX		,
	//eCST_FILM_ROLL		,
	//eCST_FILM_CASSETTE	,
	//eCST_PAPER			,
	//eCST_OCTA			,
} eCASSETTE_TYPE;

typedef enum eBATCH_ORDER {
	eBATCH_START		= 1,
	eBATCH_NORMAL		,
	eBATCH_END			,
} eBATCH_ORDER;

// 포트 상태
typedef enum ePORT_STATE {
	ePORT_STATE_EMPTY					= 0,
	ePORT_STATE_IDLE					,
	ePORT_STATE_WAITING					,
	ePORT_STATE_RESERVED				,
	ePORT_STATE_BUSY					,
	ePORT_STATE_COMPLETED				,
	ePORT_STATE_ABORTED					,
	ePORT_STATE_CANCELD					,
	ePORT_STATE_PAUSE					,	
	ePORT_STATE_DISABLE					,
} ePORT_STATE;

// 포트 타입
typedef enum ePORT_HSMS_TYPE {
	ePORT_HSMS_TYPE_BOTH						= 0,
	ePORT_HSMS_TYPE_IN						,
	ePORT_HSMS_TYPE_OUT						,
	//ePORT_TYPE_BUFFER					,
	//ePORT_TYPE_TEACHING					,
} ePORT_HSMS_TYPE;

// 분류 유형
typedef enum {
	eSORT_DUMP_FILL						= 0,
	eSORT_DUMP							,
	eSORT_LOTID							,
	eSORT_BATCHID						,
	eSORT_JOBID							,
	eSORT_PRODUCT_KIND					,
	eSORT_PRERUN_FLAG					,
	eSORT_WORK_STATE					,
	eSORT_WORK_STATE_FLIP_STATE			,
	eSORT_PRODUCTID_PRODUCT_TYPE		,
	//eSORT_PRODUCTID						,
	//eSORT_PRODUCT_TYPE					,
	//eSORT_PRODUCT_TYPE_KIND				,
	//eSORT_PRODUCT_ID_TYPE_RUNSPEC		,
	//eSORT_PRODUCT_ID_TYPE_RUNSPEC_STEP	,
} eSORT_TYPE;

typedef enum eGLASS_STATE {
	eGLASS_STATE_IDLE = 1,
	eGLASS_STATE_SELECTED_TO_PROCESS,
	eGLASS_STATE_PROCESSING,
	eGLASS_STATE_DONE,
	eGLASS_STATE_ABORTED,
	eGLASS_STATE_CANCELED,
} eGLASS_STATE;

// Machine Connection Mode
typedef enum eMCMD_CODE {
	eMCMD_OFFLINE	= 0, 
	eMCMD_ONLINE_REMOTE,
	eMCMD_ONLINE_LOCAL, 
} eMCMD_CODE;

// AVAILABILITY State (EQP State)
typedef enum eAVAILABILITY_STATE {
	eAS_DOWN = 1,	// 설비에 문제 발생으로 운영이 불가능한 상태
	eAS_UP,			// 설비 운영이 가능한 상태
} eAVAILABILITY_STATE;

typedef enum eINTERLOCK_STATE {
	eIS_ON = 1,	// 설비에 Interlock 발생 된 상태
	eIS_OFF,	// 설비에 Interlock 발생 되지 않은 상태
} eINTERLOCK_STATE;

typedef enum eMOVE_STATE {
	eMS_PAUSE = 1,	// 설비가 외부요인으로 일시 정지 중인 상태
	eMS_RUNNING,	// 설비가 정상 동작인 상태
} eMOVE_STATE;

typedef enum eRUN_STATE {
	eRS_IDLE = 1,	// 설비 내 Cell이 없는 상태
	eRS_RUN,		// 설비 내 Cell이 있는 상태
} eRUN_STATE;

typedef enum eACCESS_MODE {
	eAM_MANUAL = 0,	// 물류 수동 공급 배출
	eAM_AUTO,		// 물류 자동 공급 배출
} eACCESS_MODE;

typedef enum eTRANSFER_STATE {
	eTS_EMPTY = 0,			// 설비에서 Carrier Unloading 완료 된 상태
	eTS_READY_TO_LOAD,		// 설비에서 Port가 Loading 요구한 상태
	eTS_LOADED,				// 설비 Port에 Carrier 적재 된 시작 시점에서 완료 시점까지의 상태
	eTS_READY_TO_UNLOAD,	// Carrier 진행이 완료 되어 Carrier Unload 요구한 상태
}eTRANSFER_STATE;

typedef enum ePROCESSING_STATE {
	ePS_NONE = 0,			// None 설비의 Port에 진행 중 이거나 대기 중인 Carrier가 없음
	ePS_READY_TO_PROCESS,	// ReadyToProcess 설비의 Port에 Carrier가 적재 완료된 경우
	ePS_READY_TO_START,		// ReadyToStart 설비가 Host의 Start 명령에 OK하여 Carrier에 대한 Start 대기중인 경우
	ePS_CANCELED,			// Canceled Carrier 진행을 시작하기 전에 Host 또는 사용자의 Carrier 진행 취소 명령을 받아서 Carrier 진행을 취소한 경우
	ePS_PROCESSING,			// Processing 설비가 Host 또는 사용자의 Carrier 진행 시작 명령을 받아서 Carrier 진행을 시작하여 진행중인 경우
	ePS_PAUSED,				// Paused 설비가 Carrier 진행을 시작한 후에 Host 또는 사용자의 Carrier 진행 일시정지 명령을 받아서 Carrier 진행을 일시 정지한 경우
	ePS_PROCESSED,			// Processed 설비에서 Carrier 진행을 완료한 경우
	ePS_ABORTED,			// Aborted 설비가 Carrier 진행을 시작한 후에 Host 또는 사용자의 Carrier 진행 중지명령을 받아서 Tray투입을 중지한 경우
	ePS_ALL,				// *			 
} ePROCESSING_STATE;

//typedef enum eWARNING_STATE {
//	eWS_WARNING = 1,	// 배터리 충전이 필요한 상태
//	eWS_NORMAL,			// 배터리 정상 상태
//} eWARNING_STATE;
//
//typedef enum eFRONT_STATE {
//	eFS_DOWN = 1,	// 상류 설비 물류 상태가 물류 진행이 불가능한 상태
//	eFS_UP,			// 상류 설비 물류 상태가 물류 진행이 가능한 상태
//} eFRONT_STATE;
//
//typedef enum eREAR_STATE {
//	eRS_DOWN = 1,	// 하류 설비 물류 상태가 물류 진행이 불가능한 상태
//	eRS_UP,			// 하류 설비 물류 상태가 물류 진행이 가능한 상태
//} eREAR_STATE;

typedef enum eCODE_TYPE {
	eCT_REWORK = 0,
	eCT_SCRAP,
	eCT_MAINT,
	eCT_NG,
	nCT,
} eCODE_TYPE;

typedef enum eREADER_ID {
	eRI_NO_HW,			// 공백, MCR 설치
	eRI_FIRST = 1,		// 장비 내 First REader
	eRI_SECOND,			// 장비 내 Second Reader, n개의 ReaderID에 대해서 정의
	eRI_THIRD,			// 장비 내 Third Reader, n개의 ReaderID에 대해서 정의
	eRI_EQ_TO_EQ = 9,	// 상류 설비 혹은 하류 설비로부터 데이터 받은 값
} eREADER_ID;

typedef enum eREADER_RESULT_CODE {
	eRRC_OK = 0,			// Reader 정상으로 값을 읽은 값
	eRRC_ERROR,				// Reader 비정상으로 값을 읽은 값
	eRRC_MCR_READ_OFF,		// H/W 기능 미사용
	eRRC_VALID_FUNC_OFF,	// Tracking Validation 기능 미사용
} eREADER_RESULT_CODE;


typedef enum eJUDGE {
	// G, R, O, L 임의 취출.
	eJUDGE_NONE = -1,
	eJUDGE_GOOD = 0,	// 정상 진행 완료 상태
	eJUDGE_NO_GOOD,		// 진행 완료이지만 제품이 불량인 상태
	eJUDGE_SCRAP,		// 진행 중 문제 발생으로 정상치 못한 상태
	eJUDGE_LOT_LOSS,	// 진행 완료이지만 불량인 상태
	eJUDGE_RETEST,		// 진행 완료이지만 재확인 필요한 상태
	eJUDGE_OUT,			// 공정 진행 중 제품이 설비 외부로 배출 된 상태
	nJUDGE,

	// 2024.04.22 LBH - Sequence 내부에서 사용
	eCELL_JUDGE_NONE = eJUDGE_NONE,
	eCELL_JUDGE_OK = eJUDGE_GOOD,
	eCELL_JUDGE_NG = eJUDGE_NO_GOOD,
} eJUDGE;


// By Who (Who triggers the event) - 이벤트 트리거를 발생시는 사람
typedef enum eBY_WHO {
	eBY_HOST		= 1,
	eBY_OPERATOR	,
	eBY_EQUIPMENT	, // 장비 스스로
} eBY_WHO;

// 포트 갯수 변경시 여기서 설정할 것.
typedef enum ePORT {
	ePORT_IP01 = 0,	// 로딩부 카셋트를 처리하는 포트
	ePORT_OP02,		// 언로딩부 셀을 받는 카셋트에 넣음.
	ePORT_OP03,		// 언로딩부 셀을 받는 카셋트에 넣음.
	ePORT_OP04,		// 언로딩부 셀을 받는 카셋트에 넣음.
	ePORT_IP05,		// 로딩부 카셋트를 처리하는 포트

	// 외부
	ePORT_UI_01 = ePORT_IP01,
	ePORT_UO_01,
	ePORT_UO_03,
	ePORT_UO_02,
	ePORT_UI_02,

	// 내부
	ePORT_US_01,
	ePORT_UH_01,
	ePORT_UH_03,
	ePORT_UH_02,
	ePORT_US_02,
	ePORT_END,
} ePORT;

typedef enum eLD_PORT {
	ePPBOX_LI_01 = ePORT_IP01, // Load CV1
	ePPBOX_LS_01, // Load CV2
	ePPBOX_LO_01, // Unlaod CV2
	nLD_PORT,
} eLD_PORT;

// KES 추가 2024.3.26
typedef enum eLANGUAGE {
	eL_KOREAN,
	eL_ENGLISH,
	eL_VIETNAM,
	nLANGUAGE,
} eLANGUAGE;

typedef enum ePPID_MODE {
	ePPID_MODE_NONE
	, ePPID_MODE_CREATION_OF_PPID = 1
	, ePPID_MODE_DELETION_OF_PPID = 2
	, ePPID_MODE_MODIFICATION_OF_PPID = 3,
} ePPID_MODE;

typedef enum ePPID_OFFSET {
	ePPID_OFFSET_CREATE_N, // 값 초기화 후 생성
	ePPID_OFFSET_CREATE_O, // 기존 값 참조 생성
	ePPID_OFFSET_DELETE_D, // 값 초기화 후 삭제
	ePPID_OFFSET_DELETE_G, // 값 유지 후 삭제
	ePPID_OFFSET_MODIFY_C, // 값 유지 후 삭제
	nPPID_OFFSET,
} ePPID_OFFSET_TEACHING;

// Index로만 하용하는것 같음.
typedef enum ePPID_TYPE {
	ePPID_TYPE_EQ = 1,		// EQ PPIO Body
	ePPID_TYPE_HOST,	// Host PPID, EQ PPID Matching Table.
	ePPID_TYPE_FLOWID,	// FLOWID For Produce Control Module & Flow Controller
	// PPID TYPE 이 3번인 경우에는 PPID 자리에 FLOW RECIPE를 넣고, 그 외 PPID TYPE이 1번 혹은 2버인 경우 설비의 PPID를 전송함.
} ePPID_TYPE;

typedef enum eTID
{
	//org - CIM SPEC org는 이렇지만 SDC와 상의하여 맞출수 있다고함
	//0 = All
	//1 = CIM PC
	//2 = EQ UI
	//3 = LOADER UI
	eTID_ALL_PC	= 0
	, eTID_CIM_PC
	, eTID_LD_PC
	, eTID_SCT_PC
	, eTID_CCTL_PC
	, eTID_CCTR_PC
	, eTID_ULDL_PC
	, eTID_ULDR_PC,
	nTID,
} eTID;

typedef enum eTPM_LOSS_CODE {
	eTLC_BM_BREAKDOWN_MANUAL,
	eTLC_PM_REGULAR_PM,
	eTLC_PM_IRREGULAR_PM_CHECK_EQUIPMENT,
	eTLC_PM_IRREGULAR_PM_CHECK_QUALITY,
	eTLC_CM_IMPROVE_EQUIPMENT,
	eTLC_CM_IMPROVE_PROCESSING_EI,
	eTLC_CM_SETUP_NE_PRODUCT,
	eTLC_CHANGE_MODEL_CHANGE_SAME_MODEL,
	eTLC_CHANGE_MODEL_CHANGE_DIFFERENT_MODEL,
	eTLC_MATERIAL_CHANGE_MATERIAL,
	eTLC_MATERIAL_DOWN_MATERIAL,
	nTLC,
} eTPM_LOSS_CODE;

typedef enum eDCOL_ITEM {
	// SCT
	eDI_ALIGN_CORRECTION_X = 0,
	eDI_ALIGN_CORRECTION_Y,
	eDI_ALIGN_SCORE_1,
	eDI_ALIGN_SCORE_2,
	nDCOL_ITEM_SCT,

	// CCT
	eDI_CS01_ALIGN_CORRECTION_X = 0,
	eDI_CS01_ALIGN_CORRECTION_Y,
	eDI_CS01_ALIGN_SCORE_1,
	eDI_CS01_ALIGN_SCORE_2,
	eDI_T_CHIP_JUDGE,
	eDI_T_CHIP_SPEC_X,
	eDI_T_CHIP_LOCATION_X,
	eDI_T_CHIP_LOCATION_Y,
	eDI_T_CHIP_HEIGHT,
	eDI_T_CHIP_WIDTH,
	eDI_T_BURR_JUDGE,
	eDI_T_BURR_SPEC_X,
	eDI_T_BURR_LOCATION_X,
	eDI_T_BURR_LOCATION_Y,
	eDI_T_BURR_HEIGHT,
	eDI_T_BURR_WIDTH,
	eDI_B_CHIP_JUDGE,
	eDI_B_CHIP_SPEC_X,
	eDI_B_CHIP_LOCATION_X,
	eDI_B_CHIP_LOCATION_Y,
	eDI_B_CHIP_HEIGHT,
	eDI_B_CHIP_WIDTH,
	eDI_B_BURR_JUDGE,
	eDI_B_BURR_SPEC_X,
	eDI_B_BURR_LOCATION_X,
	eDI_B_BURR_LOCATION_Y,
	eDI_B_BURR_HEIGHT,
	eDI_B_BURR_WIDTH,
	eDI_L_CHIP_JUDGE,
	eDI_L_CHIP_SPEC_X,
	eDI_L_CHIP_LOCATION_X,
	eDI_L_CHIP_LOCATION_Y,
	eDI_L_CHIP_HEIGHT,
	eDI_L_CHIP_WIDTH,
	eDI_L_BURR_JUDGE,
	eDI_L_BURR_SPEC_X,
	eDI_L_BURR_LOCATION_X,
	eDI_L_BURR_LOCATION_Y,
	eDI_L_BURR_HEIGHT,
	eDI_L_BURR_WIDTH,
	eDI_R_CHIP_JUDGE,
	eDI_R_CHIP_SPEC_X,
	eDI_R_CHIP_LOCATION_X,
	eDI_R_CHIP_LOCATION_Y,
	eDI_R_CHIP_HEIGHT,
	eDI_R_CHIP_WIDTH,
	eDI_R_BURR_JUDGE,
	eDI_R_BURR_SPEC_X,
	eDI_R_BURR_LOCATION_X,
	eDI_R_BURR_LOCATION_Y,
	eDI_R_BURR_HEIGHT,
	eDI_R_BURR_WIDTH,
	eDI_SURFACE_JUDGE,
	eDI_SURFACE_SPEC,
	eDI_SURFACE_LOCATION_X,
	eDI_SURFACE_LOCATION_Y,
	eDI_SURFACE_LENGTH,
	eDI_FILM_ATTACH_JUDGE,
	eDI_SIZE1_JUDGE,
	eDI_SIZE1_LSL,
	eDI_SIZE1_TARGET,
	eDI_SIZE1_USL,
	eDI_SIZE1_VALUE,
	eDI_SIZE2_JUDGE,
	eDI_SIZE2_LSL,
	eDI_SIZE2_TARGET,
	eDI_SIZE2_USL,
	eDI_SIZE2_VALUE,
	eDI_SIZE3_JUDGE,
	eDI_SIZE3_LSL,
	eDI_SIZE3_TARGET,
	eDI_SIZE3_USL,
	eDI_SIZE3_VALUE,
	eDI_SIZE4_JUDGE,
	eDI_SIZE4_LSL,
	eDI_SIZE4_TARGET,
	eDI_SIZE4_USL,
	eDI_SIZE4_VALUE,
	eDI_SIZE5_JUDGE,
	eDI_SIZE5_LSL,
	eDI_SIZE5_TARGET,
	eDI_SIZE5_USL,
	eDI_SIZE5_VALUE,
	eDI_SIZE6_JUDGE,
	eDI_SIZE6_LSL,
	eDI_SIZE6_TARGET,
	eDI_SIZE6_USL,
	eDI_SIZE6_VALUE,
	eDI_SIZE7_JUDGE,
	eDI_SIZE7_LSL,
	eDI_SIZE7_TARGET,
	eDI_SIZE7_USL,
	eDI_SIZE7_VALUE,
	eDI_SIZE8_JUDGE,
	eDI_SIZE8_LSL,
	eDI_SIZE8_TARGET,
	eDI_SIZE8_USL,
	eDI_SIZE8_VALUE,
	eDI_T_CHIP_SPEC_Y,
	eDI_T_BURR_SPEC_Y,
	eDI_B_CHIP_SPEC_Y,
	eDI_B_BURR_SPEC_Y,
	eDI_L_CHIP_SPEC_Y,
	eDI_L_BURR_SPEC_Y,
	eDI_R_CHIP_SPEC_Y,
	eDI_R_BURR_SPEC_Y,
	nDCOL_ITEM_CCT,
} eDCOL_ITEM;

//HCACK: Host Command Parameter Acknowledge Code[Unsigned 1 Byte]
//0 = Acknowledge is OK
//1 = Command does not exist
//2 = Cannot perform now(Hardware Problem)
//* If equipment reply HCACK = 2, then reason should be report as Alarm(S5F1)
//3 = At least one parameter invalid
//4 = Acknowledge, command will be performed with completion signalled later by an event
//5 = Rejected, Already in desired Condition
//6 = No such object exists
//7 = Control State is Online Local.Equipment rejects command.
//8 = Cannot find raw data
//9 = Cannot read raw data
//10 = PPID is not exist.
//12 = Rejected, The other Job is Reserved
//13 = Rejected, Eqp doesn't receive CASSETTE INFO
//14 = Specific Virtual Port is already full
//15 = PPID is not availlable
//16 = Rejected, Eqp doesn't receive Material(Frame or Stick Mask) INFO
//CPACK : Command Parameter Acknowledge Code, 1Byte[Unsigned 1 Byte]
//0 = No Error
//1 = Parameter Name(CPNAME) does not exist
//2 = Illegal Value specified for CPVAL
//3 = Illegal Format specified for CPVAL
//4 = No cassette(input port)
//5 = No cassette(output port)
//6 = Cassette ID not match(input port)
//7 = Cassette ID not match(output port)
//8 = Port state error(input port)
//9 = Port state error(output port)
//10 = Too many slots are selected, empty slots are short. (SLOT(QTY) > Empty)
//11 = No Glass Error
//12 = Glass already exist, cannot replace
//13 = Glass State error
//14 = Denied, Module State Error
//15 = Denied, Tool State Error
//16 = Denied, Unit State Error
//21 = Glass_Size Mismatch(input = source port) * for sorter
//22 = STEPID(OPERATIONID) (Mismatch(output = target port)) * for sorter
//23 = PORTID Mismatch(input = Source port) * for sorter
//24 = Car_Size Mismatch(input≠output) * for sorter
//25 = Output Slot Occupied(output = target port) * for sorter
//26 = Input Slot Empty(input = Source port) * for sorter
//27 = Reserved
//(If No item matched, use this item for other cases.But if possible, choose other one& find similar one.)
//28 = Output is NG Cassette
//29 = Cassette_Demand is not matched
//30 = Module Name has a Problem
//31 = Glassid has a Problem
//32 = Lotid has a Problem
//33 = Product Type has a Problem
//34 = PPID has a Problem
//* These HCACK, CPACK can be expanded the ack type by the discussion afterward

#ifndef _IP_DATA_INTERNAL_
static LPSTR lpEQP[nEQP] = {
	"SCT",
	"CCT_A",
	"CCT_B",
};

static LPSTR lpEFID[nEFID] = {
	"NONE",
	"CELL_TRACKING",
	"TRACKING_CONTROL",
	"MATERIAL_TRACKING",
	"CELL_MCR_MODE",
	"MATERIAL_MCR_MODE",
	"LOT_ASSIGN_INFORMATION",
	"AGV_ACCESS_MODE",
	"AREA_SENSOR_MODE",
	"SORT_MODE",
	"INTERLOCK_CONTROL",
	"LOADER_LOAD_PORT_MCR_MODE",
	"LOADER_USE_PORT_MCR_MODE",
	"UNLOADER_USE_PORT_MCR_MODE",
	"APC_MODE",
	"MULTI_PASS_MODE",
};

static LPSTR lpEFST[nEFST] = {
	"ON",
	"OFF",
	"TRACE",
	"NOTHING",
	"TKIN",
	"TKOUT",
	"BOTH",
	"USE",
	"UNUSE",
	"AUTO",
	"MANU",
	"TRANSFER",
	"LOADING",
	"STEP",
	"OWN",
};

static LPSTR lpCODE_TYPE[nCT] = {
	"R",
	"S",
	"M",
	"N",
};

static LPSTR lpJUDGE[nJUDGE] = {
	// G, R, O, L 임의 취출.
	"G",
	"N",	// 미사용
	"S",	// 미사용
	"L",
	"R",
	"O",
};

static LPSTR lpPPID_OFFST[nPPID_OFFSET] = {
	"N",
	"O",
	"D",
	"G",
	"C",
};

static LPSTR lpPC[nTID] = {
	"ALL_PC",
	"CIMTransfer",
	"LD",
	"SCT",
	"CCTL",
	"CCTR",
	"ULDL",
	"ULDR",
};

static LPSTR lpMSG_TYPE[nMSG_TYPE] = {
	"OPCALL_MSG",
	"INTERLOCK_MSG",
	//"UNIT_INTERLOCK_MSG",
};

static LPSTR lpTPM_LossCode[nTLC] = {
	"03000",
	"12000",
	"15100",
	"15200",
	"17200",
	"17300",
	"14000",
	"41100",
	"41200",
	"35000",
	"51000",
};


static LPSTR lpDCOL_SCT[nDCOL_ITEM_SCT] {
	// SCT
	"ALIGN_CORRECTION_X",
	"ALIGN_CORRECTION_Y",
	"ALIGN_SCORE_1",
	"ALIGN_SCORE_2",
};


static LPSTR lpDCOL_CCT[nDCOL_ITEM_CCT]{
	"CS01_ALIGN_CORRECTION_X",
	"CS01_ALIGN_CORRECTION_Y",
	"CS01_ALIGN_SCORE_1",
	"CS01_ALIGN_SCORE_2",
	"T_CHIP_JUDGE",
	"T_CHIP_SPEC_X",
	"T_CHIP_LOCATION_X",
	"T_CHIP_LOCATION_Y",
	"T_CHIP_HEIGHT",
	"T_CHIP_WIDTH",
	"T_BURR_JUDGE",
	"T_BURR_SPEC_X",
	"T_BURR_LOCATION_X",
	"T_BURR_LOCATION_Y",
	"T_BURR_HEIGHT",
	"T_BURR_WIDTH",
	"B_CHIP_JUDGE",
	"B_CHIP_SPEC_X",
	"B_CHIP_LOCATION_X",
	"B_CHIP_LOCATION_Y",
	"B_CHIP_HEIGHT",
	"B_CHIP_WIDTH",
	"B_BURR_JUDGE",
	"B_BURR_SPEC_X",
	"B_BURR_LOCATION_X",
	"B_BURR_LOCATION_Y",
	"B_BURR_HEIGHT",
	"B_BURR_WIDTH",
	"L_CHIP_JUDGE",
	"L_CHIP_SPEC_X",
	"L_CHIP_LOCATION_X",
	"L_CHIP_LOCATION_Y",
	"L_CHIP_HEIGHT",
	"L_CHIP_WIDTH",
	"L_BURR_JUDGE",
	"L_BURR_SPEC_X",
	"L_BURR_LOCATION_X",
	"L_BURR_LOCATION_Y",
	"L_BURR_HEIGHT",
	"L_BURR_WIDTH",
	"R_CHIP_JUDGE",
	"R_CHIP_SPEC_X",
	"R_CHIP_LOCATION_X",
	"R_CHIP_LOCATION_Y",
	"R_CHIP_HEIGHT",
	"R_CHIP_WIDTH",
	"R_BURR_JUDGE",
	"R_BURR_SPEC_X",
	"R_BURR_LOCATION_X",
	"R_BURR_LOCATION_Y",
	"R_BURR_HEIGHT",
	"R_BURR_WIDTH",
	"SURFACE_JUDGE",
	"SURFACE_SPEC",
	"SURFACE_LOCATION_X",
	"SURFACE_LOCATION_Y",
	"SURFACE_LENGTH",
	"FILM_ATTACH_JUDGE",
	"SIZE1_JUDGE",
	"SIZE1_LSL",
	"SIZE1_TARGET",
	"SIZE1_USL",
	"SIZE1_VALUE",
	"SIZE2_JUDGE",
	"SIZE2_LSL",
	"SIZE2_TARGET",
	"SIZE2_USL",
	"SIZE2_VALUE",
	"SIZE3_JUDGE",
	"SIZE3_LSL",
	"SIZE3_TARGET",
	"SIZE3_USL",
	"SIZE3_VALUE",
	"SIZE4_JUDGE",
	"SIZE4_LSL",
	"SIZE4_TARGET",
	"SIZE4_USL",
	"SIZE4_VALUE",
	"SIZE5_JUDGE",
	"SIZE5_LSL",
	"SIZE5_TARGET",
	"SIZE5_USL",
	"SIZE5_VALUE",
	"SIZE6_JUDGE",
	"SIZE6_LSL",
	"SIZE6_TARGET",
	"SIZE6_USL",
	"SIZE6_VALUE",
	"SIZE7_JUDGE",
	"SIZE7_LSL",
	"SIZE7_TARGET",
	"SIZE7_USL",
	"SIZE7_VALUE",
	"SIZE8_JUDGE",
	"SIZE8_LSL",
	"SIZE8_TARGET",
	"SIZE8_USL",
	"SIZE8_VALUE",
	"T_CHIP_SPEC_Y",
	"T_BURR_SPEC_Y",
	"B_CHIP_SPEC_Y",
	"B_BURR_SPEC_Y",
	"L_CHIP_SPEC_Y",
	"L_BURR_SPEC_Y",
	"R_CHIP_SPEC_Y",
	"R_BURR_SPEC_Y",
};



#endif
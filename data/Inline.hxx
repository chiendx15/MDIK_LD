#pragma once
#include "Mocha.type.hxx"

#pragma pack(push, 1)

//==================================================================================================
// Port Status
#define __PORT_STATUS_IDLE__			"ID"		// Idle
#define __PORT_STATUS_WAIT__			"WT"		// Wait
#define __PORT_STATUS_EMPTY__			"EM"		// Empty
#define __PORT_STATUS_RESERVED__		"RS"		// Reserved
#define __PORT_STATUS_PAUSE__			"PS"		// Pause
#define __PORT_STATUS_BUSY__			"BS"		// Busy
#define __PORT_STATUS_ABORTED__			"AB"		// Aborted
#define __PORT_STATUS_CANCELED__		"CN"		// Canceled
#define __PORT_STATUS_COMPLETED__		"CM"		// Completed
#define __PORT_STATUS_ERROR__			"ER"		// Error
#define __PORT_STATUS_NOT_USED_PORT__	"NU"		// Not-Used Port

//==================================================================================================
// 0. Contact Point State (EQ To EQ)
typedef enum {
	// Bit 0 - 7
	eCP_ABNORMAL									,
	eCP_TYPE_OF_ARM									,
	eCP_TYPE_OF_STAGE_CONVEYOR						,
	eCP_ARM_STRETCH_UP_MOVING						,
	eCP_ARM_STRETCH_UP_COMPLETE						,
	eCP_ARM_STRETCH_DOWN_MOVING						,
	eCP_ARM_STRETCH_DOWN_COMPLETE					,
	eCP_ARM_STRETCHING								,
	// Bit 8 - 15
	eCP_ARM_STRETCH_COMPLETE						,
	eCP_ARM_FOLDING									,
	eCP_ARM_FOLD_COMPLETE							,
	eCP_RESERVED_11									,
	eCP_RESERVED_12									,
	eCP_ARM_1_FOLDED								,
	eCP_ARM_2_FOLDED								,
	eCP_ARM_1_GLASS_DETECT							,
	// Bit 16 - 23
	eCP_ARM_2_GLASS_DETECT							,
	eCP_ARM_1_GLASS_VACUUM							,
	eCP_ARM_2_GLASS_VACUUM							,
	eCP_ROBOT_DIRECTION								,
	eCP_MANUAL_OPERATION							,
	eCP_PIN_UP										,
	eCP_PIN_DOWN									,
	eCP_DOOR_OPEN									,
	// Bit 24 - 31
	eCP_DOOR_CLOSE									,
	eCP_GLASS_DETECT								,
	eCP_BODY_MOVING									,
	eCP_BODY_ORIGIN_POSITION						,
	eCP_EMERGENCY									,
	eCP_VERTICAL									,
	eCP_HORIZONTAL									,
	eCP_RESERVED_31									,
} eCONTACT_POINT_STATE;
typedef struct {
	union {
		struct {
			// Bit 0 - 7
			DWORD	fAbnormal						: 1;
			DWORD	fType_Of_Arm					: 1;
			DWORD	fType_Of_Stage_Conveyor			: 1;
			DWORD	fArm_Stretch_Up_Moving			: 1;
			DWORD	fArm_Stretch_Up_Complete		: 1;
			DWORD	fArm_Stretch_Down_Moving		: 1;
			DWORD	fArm_Stretch_Down_Complete		: 1;
			DWORD	fArm_Stretching					: 1;
			// Bit 8 - 15
			DWORD	fArm_Stretch_Complete			: 1;
			DWORD	fArm_Folding					: 1;
			DWORD	fArm_Fold_Complete				: 1;
			DWORD	fReserved_11					: 1;
			DWORD	fReserved_12					: 1;
			DWORD	fArm_1_Folded					: 1;
			DWORD	fArm_2_Folded					: 1;
			DWORD	fArm_1_Glass_Detect				: 1;
			// Bit 16 - 23
			DWORD	fArm_2_Glass_Detect				: 1;
			DWORD	fArm_1_Glass_Vacuum				: 1;
			DWORD	fArm_2_Glass_Vacuum				: 1;
			DWORD	frRobot_Direction				: 1;
			DWORD	fManual_Operation				: 1;
			DWORD	fPin_Up							: 1;
			DWORD	fPin_Down						: 1;
			DWORD	fDoor_Open						: 1;
			// Bit 24 - 31
			DWORD	fDoor_Close						: 1;
			DWORD	fGlass_Detect					: 1;
			DWORD	fBody_Moving					: 1;
			DWORD	fBody_Origin_Position			: 1;
			DWORD	fEmergency						: 1;
			DWORD	fVertical						: 1;
			DWORD	fHorizontal						: 1;
			DWORD	fReserved_31					: 1;
		};
		DWORD dwValue;
	} ;
} T_CONTACT_POINT_STATE_BIT;
// 1. Component Hand Shake Of "To Upper Equipment"
typedef enum {
		// Bit 0 - 7
		eHS_TO_UPPER_HEART_BEAT						, 
		eHS_TO_UPPER_MACHINE_PAUSE					, 
		eHS_TO_UPPER_MACHINE_DOWN					, 
		eHS_TO_UPPER_MACHINE_ALARM					, 
		eHS_TO_UPPER_RECEIVE_ABLE					,
		eHS_TO_UPPER_RECEIVE_START					, 
		eHS_TO_UPPER_RECEIVE_COMPLETE				, 
		eHS_TO_UPPER_EXCHANGE_FLAG					,
		// Bit 8 - 15
		eHS_TO_UPPER_RETURN_SEND_START				, 
		eHS_TO_UPPER_RETURN_SEND_COMPLETE			, 
		eHS_TO_UPPER_IMMEDIATELY_PAUSE_REQUEST		,
		eHS_TO_UPPER_IMMEDIATELY_STOP_REQUEST		,
		eHS_TO_UPPER_RECEIVE_ABLE_REMAINED_STEP_1	,
		eHS_TO_UPPER_RECEIVE_ABLE_REMAINED_STEP_2	,
		eHS_TO_UPPER_RECEIVE_ABLE_REMAINED_STEP_3	,
		eHS_TO_UPPER_RECEIVE_ABLE_REMAINED_STEP_4	,
		// Bit 16 - 23
		eHS_TO_UPPER_GLASS_ID_READ_COMPLETE			,
		eHS_TO_UPPER_LOADING_STOP					, 
		eHS_TO_UPPER_TRANSFER_STOP					,
		eHS_TO_UPPER_EXCHANGE_FAIL_FLAG				,
		eHS_TO_UPPER_PROCESS_TIME_UP				, 
		eHS_TO_UPPER_RESERVED_21					, 
		eHS_TO_UPPER_RESERVED_22					, 
		eHS_TO_UPPER_RECEIVE_ABLE_RESERVE_REQUEST	,
		// Bit 24 - 31
		eHS_TO_UPPER_HS_CANCEL_REQUEST				, 
		eHS_TO_UPPER_HS_ABORT_REQUEST				, 
		eHS_TO_UPPER_HS_RESUME_REQUEST				, 
		eHS_TO_UPPER_HS_RECOVERY_REPLY_ACK			, 
		eHS_TO_UPPER_HS_RECOVERY_REPLY_NAK			, 
		eHS_TO_UPPER_SEND_JOB_READY					,
		eHS_TO_UPPER_SEND_ACTION_MOVE				,
		eHS_TO_UPPER_SEND_ACTION_REMOVE				,
} eHS_TO_UPPER_EQUIPMENT;
typedef struct {
	union {
		struct { 
			// Bit 0 - 7
			DWORD fHeartBeat						: 1;
			DWORD fMachine_Pause					: 1;
			DWORD fMachine_Down						: 1;
			DWORD fMachine_Alarm					: 1;
			DWORD fReceive_Able						: 1;
			DWORD fReceive_Start					: 1;
			DWORD fReceive_Complete					: 1;
			DWORD fExchange_Flag					: 1;
			// Bit 8 - 15
			DWORD fReturn_Send_Start				: 1;
			DWORD fReturn_Send_Complete				: 1;
			DWORD fImm_Pause_Request				: 1;
			DWORD fImm_Stop_Request					: 1;			
			DWORD fReceive_Able_Remained_Step_1		: 1;
			DWORD fReceive_Able_Remained_Step_2		: 1;
			DWORD fReceive_Able_Remained_Step_3		: 1;
			DWORD fReceive_Able_Remained_Step_4		: 1;			
			// Bit 16 - 23
			DWORD fLoading_Id_Read_Complete			: 1;
			DWORD fLoading_Stop						: 1;
			DWORD fTransfer_Stop					: 1;
			DWORD fExchange_Fail_Flag				: 1;
			DWORD fProcess_Time_Up					: 1;
			DWORD fReserved_21						: 1;
			DWORD fReserved_22						: 1;
			DWORD fReceive_Able_Reserve_Request		: 1;
			// Bit 24 - 31
			DWORD fHS_Cancel_Request				: 1;
			DWORD fHS_Abort_Request					: 1;
			DWORD fHS_Resume_Request				: 1;
			DWORD fHS_Recovery_Reply_Ack			: 1;
			DWORD fHS_Recovery_Reply_Nak			: 1;
			DWORD fReceive_Job_Ready				: 1;
			DWORD fReceive_Action_Move				: 1;
			DWORD fReceive_Action_Remove			: 1;
		};
		DWORD dwToUpper_HS;
	} ;
	T_CONTACT_POINT_STATE_BIT	ContactPointState	;
	DWORD						dwReserved_2		;
	DWORD						dwReserved_3		;
} T_HS_TO_UPPER_EQUIPMENT;							// 128 bit (4 words)
// 2. Component Hand Shake Of "To Lower Equipment"
typedef enum {
	// Bit 0 - 7
	eHS_TO_LOWER_HEART_BEAT							, 
	eHS_TO_LOWER_MACHINE_PAUSE						, 
	eHS_TO_LOWER_MACHINE_DOWN						, 
	eHS_TO_LOWER_MACHINE_ALARM						, 
	eHS_TO_LOWER_SEND_ABLE							,
	eHS_TO_LOWER_SEND_START							, 
	eHS_TO_LOWER_SEND_COMPLETE						, 
	eHS_TO_LOWER_EXCHANGE_FLAG						,
	// Bit 8 - 15
	eHS_TO_LOWER_RETURN_RECEIVE_START				, 
	eHS_TO_LOWER_RETURN_RECEIVE_COMPLETE			, 
	eHS_TO_LOWER_IMMEDIATELY_PAUSE_REQUEST			,
	eHS_TO_LOWER_IMMEDIATELY_STOP_REQUEST			,
	eHS_TO_LOWER_SEND_ABLE_REMAINED_STEP_1			,
	eHS_TO_LOWER_SEND_ABLE_REMAINED_STEP_2			,
	eHS_TO_LOWER_SEND_ABLE_REMAINED_STEP_3			,
	eHS_TO_LOWER_SEND_ABLE_REMAINED_STEP_4			,
	// Bit 16 - 23
	eHS_TO_LOWER_WORK_START							, 
	eHS_TO_LOWER_WORK_CANCEL						,
	eHS_TO_LOWER_WORK_SKIP							,
	eHS_TO_LOWER_JOB_START							,
	eHS_TO_LOWER_JOB_END							,
	eHS_TO_LOWER_HOT_FLOW							,
	eHS_TO_LOWER_RESERVED_22						, 
	eHS_TO_LOWER_SEND_ABLE_RESERVE_REQUEST			,
	// Bit 24 - 31
	eHS_TO_LOWER_HS_CANCEL_REQUEST					, 
	eHS_TO_LOWER_HS_ABORT_REQUEST					, 
	eHS_TO_LOWER_HS_RESUME_REQUEST					, 
	eHS_TO_LOWER_HS_RECOVERY_REPLY_ACK				, 
	eHS_TO_LOWER_HS_RECOVERY_REPLY_NAK				, 
	eHS_TO_LOWER_SEND_JOB_READY						,
	eHS_TO_LOWER_SEND_ACTION_MOVE					,
	eHS_TO_LOWER_SEND_ACTION_REMOVE					,
} eHS_TO_LOWER_EQUIPMENT_BIT;
typedef struct {
	union {
		struct {
			// Bit 0 - 7
			DWORD fHeartBeat						: 1;
			DWORD fMachine_Pause					: 1;
			DWORD fMachine_Down						: 1;
			DWORD fMachine_Alarm					: 1;
			DWORD fSend_Able						: 1;
			DWORD fSend_Start						: 1;
			DWORD fSend_Complete					: 1;
			DWORD fExchange_Flag					: 1;
			// Bit 8 - 15
			DWORD fReturn_Receive_Start				: 1;
			DWORD fReturn_Receive_Complete			: 1;
			DWORD fImmediately_Pause_Request		: 1;
			DWORD fImmediately_Stop_Request			: 1;			
			DWORD fSend_Able_Remained_Step_1		: 1;
			DWORD fSend_Able_Remained_Step_2		: 1;
			DWORD fSend_Able_Remained_Step_3		: 1;
			DWORD fSend_Able_Remained_Step_4		: 1;			
			// Bit 16 - 23
			DWORD fWork_Start						: 1;
			DWORD fWork_Cancel						: 1;
			DWORD fWork_Skip						: 1;
			DWORD fJob_Start						: 1;
			DWORD fJob_End							: 1;
			DWORD fHot_Flow							: 1;
			DWORD fReserved_22						: 1;
			DWORD fSend_Able_Reserve_Request		: 1;
			// Bit 24 - 31
			DWORD fHS_Cancel_Request				: 1;
			DWORD fHS_Abort_Request					: 1;
			DWORD fHS_Resume_Request				: 1;
			DWORD fHS_Recovery_Reply_Ack			: 1;
			DWORD fHS_Recovery_Reply_Nak			: 1;
			DWORD fSend_Job_Ready					: 1;
			DWORD fSend_Action_Move					: 1;
			DWORD fSend_Action_Remove				: 1;
		};
		DWORD dwToLower_HS;
	};
	T_CONTACT_POINT_STATE_BIT	ContactPointState	;
	DWORD						dwReserved_2		;
	DWORD						dwReserved_3		;
} T_HS_TO_LOWER_EQUIPMENT;							// 128 bit (4 words)
// 3. Pre-View Of "Equipment Stayte"
typedef struct {
	WORD						wStatus							;		// 0
	WORD						wProcess_State					;		// 1
	char						szPP_Id							[20];	// 3
	WORD						wGlass_Size						[2 ];	// 13
	WORD						wGlass_Thickness				;		// 15
	WORD						wTact_Setted					;		// 16 
	WORD						wTact_Current					;		// 17
	WORD						wOperation_Mode					;		// 18
	WORD						wRequestCountOfDummyGlass		;
	WORD						wRecipe_Change_Mode				;		// 19
	WORD						wGlass_Mechanical_Change_Mode	;		// 20
	WORD						wMaterial_1_Type				[6 ];	// 21
	WORD						wMaterial_1_Remain_Count		;		// 27
	WORD						wMaterial_2_Type				[6 ];	// 28
	WORD						wMaterial_2_Remain_Count		;		// 34
	WORD						wMaterial_3_Type				[6 ];	// 35
	WORD						wMaterial_3_Remain_Count		;		// 41
	WORD						wIonizerState					;		// 42
	WORD						wReserved						[5 ];	// 43
} T_EQP_STATUS;															// 48 words
// 4. Prev_View Of "Glass Summary"
typedef struct {
	WORD						wTotal_Glass_Count				;		// 0
	WORD						wLTPS_Glass_Count				;		// 1
	WORD						wEVEN_Glass_Count				;		// 2
	WORD						wPMDT_Glass_Count				;		// 3
	WORD						wIDLE_Glass_Count				;		// 4
	WORD						wBusy_Glass_Count				;		// 5
	WORD						wFinish_Glass_Count				;		// 6
	WORD						wBF_Glass_Count					;		// 7
	WORD						wReserved_8						[6 ];	// 8
	char						szProcess_Id_1					[20];	// 14
	char						szProduct_Id_1					[20];	// 24
	char						szBatchId_1						[12];	// 34 
	char						szStepId_1						[12];	// 40
	WORD						wGlass_Count_1					;		// 46
	WORD						wReserved_47					;		// 47	
	char						szProcess_Id_2					[20];	// 48
	char						szProduct_Id_2					[20];	// 58
	char						szBatchId_2						[12];	// 68 
	char						szStepId_2						[12];	// 74
	WORD						wGlass_Count_2					;		// 80
	WORD						wReserved_81					;		// 81	
	char						szProcess_Id_3					[20];	// 82
	char						szProduct_Id_3					[20];	// 92
	char						szBatchId_3						[12];	// 102 
	char						szStepId_3						[12];	// 108
	WORD						wGlass_Count_3					;		// 114
	WORD						wReserved_115					;		// 115
	WORD						wDBR_Recipe_1					[2   ];	// 116		
	WORD						wDBR_Glass_Count_1				;		// 118
	WORD						wReserved_119					;		// 119
	WORD						wDBR_Recipe_2					[2   ];	// 120		
	WORD						wDBR_Glass_Count_2				;		// 122
	WORD						wReserved_123					;		// 123
	WORD						wDBR_Recipe_3					[2   ];	// 124		
	WORD						wDBR_Glass_Count_3				;		// 126
	WORD						wReserved_127					;		// 127
	WORD						wReserved_128					[22];	// 128
} T_GLASS_SUMMARY;														// 150 words
// 5. Prev_View Of "To Upper Equipment"
typedef struct {
	char						szReading_Glass_Id				[12];	// 0
	WORD						wReading_Glass_Id_type			;		// 6
	WORD						wGlass_Type						;		// 7
	WORD						wLot_Process_Type				;		// 8
	WORD						wReserved						;		// 7
} T_REQUEST_GLSSS_TYPE;													// 10 words
typedef struct {
	WORD						wCurrent_Slot_Information		[2];	// 0
	WORD						wAccess_Slot					[2];	// 2
	WORD						wAccess_Possible_Slot			[2];	// 4
} T_CONTACT_POINT_STATE;												// 6 words
// 6. Pre-View Of "To Lower Equipment"
typedef struct {
	char						szGlassId_H						[16];
	char						szGlassId_E						[16];
	char						szLotId							[16];
	char						szBatchId						[16];
	char						szJobId							[16];
	char						szPortId						[4 ];
	char						szSlotId						[2 ];
	char						szProductType					[4 ];
	char						szProductkind					[4 ];
	char						szProductId						[16];
	char						szRunSpecId						[16];
	char						szLayerId						[8 ];
	char						szStepId						[8 ];	
	char						szPPId							[20];
	char						szFlowId						[20];
	WORD						sGlassSize						[2 ];
	WORD						sGlassThickness					;
	WORD						sGlassState						;
	char						szGlassOrder					[4 ];
	char						szComment						[16];
	char						szUseCount						[4 ];
	char						szJudgement						[4 ];
	char						szReasonCode					[4 ];
	char						szINSFlag						[2 ];
	char						szENCFlag						[2 ];
	char						szPreRunFlag					[2 ];
	char						szTurnDir						[2 ];
	char						szFlipState						[2 ];
	char						szWorkState						[4 ];
	char						szMultiUse						[16];
	char						szPairGlassId					[16];
	char						szPaitPPId						[20];
	char						szOptionName_1					[40];
	char						szOptionValue_1					[40];
	char						szOptionName_2					[40];
	char						szOptionValue_2					[40];
	char						szOptionName_3					[40];
	char						szOptionValue_3					[40];
	char						szOptionName_4					[40];
	char						szOptionValue_4					[40];
	char						szOptionName_5					[40];
	char						szOptionValue_5					[40];
	WORD						sCurrentSlotInformation			[2 ];
	WORD						sAccessSlot						[2 ];
	WORD						sAccessPossibleSlot				[2 ];
	WORD						sUniqueId						[2 ];
	WORD						sBitSignal						[2 ];
	WORD						sReserved						[6 ];
} T_GLASS_DATA;

typedef struct {
	T_GLASS_DATA				GlassData						;
} T_TO_LOWER_EQUIPMENT;
typedef struct {
	T_REQUEST_GLSSS_TYPE		RequestGlassType				;
	T_CONTACT_POINT_STATE		ContactPointState				;
} T_TO_UPPER_EQUIPMENT;

#define INLINE_MAX_COMMUNICATION	4
typedef struct {
	T_HS_TO_UPPER_EQUIPMENT		ToUpperHS[4]	;			// 0000
	T_HS_TO_LOWER_EQUIPMENT		ToLowerHS[4]	;			// 0500
	//T_TO_UPPER_EQUIPMENT		ToUpperEquipment;			// 0000
	T_TO_LOWER_EQUIPMENT		ToLowerEqpipment[4];		// 00A0
	T_EQP_STATUS				EqpStateOwn		;			// 0EB0
	T_EQP_STATUS				EqpStateRelated	;			// 0FE0
	T_GLASS_SUMMARY				GlassSummary	;			// 0F10
} T_TO_LOWER;

typedef struct {
	T_HS_TO_UPPER_EQUIPMENT		ToUpperHS[4]	;			// 0A00
	T_HS_TO_LOWER_EQUIPMENT		ToLowerHS[4]	;			// 사용안함

	T_TO_LOWER_EQUIPMENT		ToLowerEquipment[4];		// 14A0 : 원래는 Upper을 사용해야하나 A3에서 바뀜.
	T_EQP_STATUS				EqpStateOwn		;			// 22B0
	T_EQP_STATUS				EqpStateRelated	;			// 22E0
	T_GLASS_SUMMARY				GlassSummary	;			// 2310
} T_TO_UPPER;

typedef struct {
	T_TO_LOWER	ToLower_L1;
	T_TO_UPPER	ToUpper_L2;
} T_ZVARIABLES;

typedef struct {
	DWORD	ToUpperEquipment_1		[4];	// LB 0000 : 128 bit
	DWORD	ToUpperEquipment_2		[4];	// LB 0080 : 128 bit
	DWORD	ToUpperEquipment_3		[4];	// LB 0100 : 128 bit
	DWORD	ToUpperEquipment_4		[4];	// LB 0180 : 128 bit
	//DWORD	ToUpperEquipment_5		[4];	// LB 0200 : 128 bit
	//DWORD	ToUpperEquipment_6		[4];	// LB 0280 : 128 bit
	//DWORD	ToUpperEquipment_7		[4];	// LB 0300 : 128 bit
	//DWORD	ToUpperEquipment_8		[4];	// LB 0380 : 128 bit
	//DWORD	ToUpperEquipment_9		[4];	// LB 0400 : 128 bit
	//DWORD	ToUpperEquipment_10		[4];	// LB 0480 : 128 bit
	DWORD	ToLowerEquipment_1		[4];	// LB 0500 : 128 bit
	DWORD	ToLowerEquipment_2		[4];	// LB 0580 : 128 bit
	DWORD	ToLowerEquipment_3		[4];	// LB 0600 : 128 bit
	DWORD	ToLowerEquipment_4		[4];	// LB 0680 : 128 bit
	//DWORD	ToLowerEquipment_5		[4];	// LB 0700 : 128 bit
	//DWORD	ToLowerEquipment_6		[4];	// LB 0780 : 128 bit
	//DWORD	ToLowerEquipment_7		[4];	// LB 0800 : 128 bit
	//DWORD	ToLowerEquipment_8		[4];	// LB 0880 : 128 bit
	//DWORD	ToLowerEquipment_9		[4];	// LB 0900 : 128 bit
	//DWORD	ToLowerEquipment_10		[4];	// LB 0980 : 128 bit
} T_BIT_L1;

typedef struct {
	DWORD	ToUpperEquipment_1		[4];	// LB 0A00 : 128 bit
	DWORD	ToUpperEquipment_2		[4];	// LB 0A80 : 128 bit
	DWORD	ToUpperEquipment_3		[4];	// LB 0B00 : 128 bit
	DWORD	ToUpperEquipment_4		[4];	// LB 0B80 : 128 bit
	//DWORD	ToUpperEquipment_5		[4];	// LB 0C00 : 128 bit
	//DWORD	ToUpperEquipment_6		[4];	// LB 0C80 : 128 bit
	//DWORD	ToUpperEquipment_7		[4];	// LB 0D00 : 128 bit
	//DWORD	ToUpperEquipment_8		[4];	// LB 0D80 : 128 bit
	//DWORD	ToUpperEquipment_9		[4];	// LB 0E00 : 128 bit
	//DWORD	ToUpperEquipment_10		[4];	// LB 0E80 : 128 bit
	DWORD	ToLowerEquipment_1		[4];	// LB 0F00 : 128 bit
	DWORD	ToLowerEquipment_2		[4];	// LB 0F80 : 128 bit
	DWORD	ToLowerEquipment_3		[4];	// LB 1000 : 128 bit
	DWORD	ToLowerEquipment_4		[4];	// LB 1080 : 128 bit
	//DWORD	ToLowerEquipment_5		[4];	// LB 1100 : 128 bit
	//DWORD	ToLowerEquipment_6		[4];	// LB 1180 : 128 bit
	//DWORD	ToLowerEquipment_7		[4];	// LB 1200 : 128 bit
	//DWORD	ToLowerEquipment_8		[4];	// LB 1280 : 128 bit
	//DWORD	ToLowerEquipment_9		[4];	// LB 1300 : 128 bit
	//DWORD	ToLowerEquipment_10		[4];	// LB 1380 : 128 bit
} T_BIT_L2;

typedef struct {
	//WORD	ToUpperEquipment_1		[16];
	//WORD	ToUpperEquipment_2		[16];
	//WORD	ToUpperEquipment_3		[16];
	//WORD	ToUpperEquipment_4		[16];
	//WORD	ToUpperEquipment_5		[16];
	//WORD	ToUpperEquipment_6		[16];
	//WORD	ToUpperEquipment_7		[16];
	//WORD	ToUpperEquipment_8		[16];
	//WORD	ToUpperEquipment_9		[16];
	//WORD	ToUpperEquipment_10		[16];
	WORD	ToLowerEquipment_1		[360];
	WORD	ToLowerEquipment_2		[360];
	WORD	ToLowerEquipment_3		[360];
	WORD	ToLowerEquipment_4		[360];
	//WORD	ToLowerEquipment_5		[360];
	//WORD	ToLowerEquipment_6		[360];
	//WORD	ToLowerEquipment_7		[360];
	//WORD	ToLowerEquipment_8		[360];
	//WORD	ToLowerEquipment_9		[360];
	//WORD	ToLowerEquipment_10		[360];
	WORD	EquipmentState_Own		[48 ];
	WORD	EquipmentState_Related	[48 ];
	WORD	GlassSummary			[150];
} T_WORD_L1;

typedef struct {
// 사양은 위의 것이지만 A3 현장에서 바꾸어 사용하함.
	// 2015.09.24. From Lotze
	//WORD	ToUpperEquipment_1		[16];	
	//WORD	ToUpperEquipment_2		[16];
	//WORD	ToUpperEquipment_3		[16];
	//WORD	ToUpperEquipment_4		[16];
	//WORD	ToUpperEquipment_5		[16];
	//WORD	ToUpperEquipment_6		[16];
	//WORD	ToUpperEquipment_7		[16];
	//WORD	ToUpperEquipment_8		[16];
	//WORD	ToUpperEquipment_9		[16];
	//WORD	ToUpperEquipment_10		[16];
	WORD	ToLowerEquipment_1		[360];
	WORD	ToLowerEquipment_2		[360];
	WORD	ToLowerEquipment_3		[360];
	WORD	ToLowerEquipment_4		[360];
	//WORD	ToLowerEquipment_5		[360];
	//WORD	ToLowerEquipment_6		[360];
	//WORD	ToLowerEquipment_7		[360];
	//WORD	ToLowerEquipment_8		[360];
	//WORD	ToLowerEquipment_9		[360];
	//WORD	ToLowerEquipment_10		[360];
	WORD	EquipmentState_Own		[48 ];
	WORD	EquipmentState_Related	[48 ];
	WORD	GlassSummary			[150];	
} T_WORD_L2;

typedef struct {
	T_BIT_L1	BitData;
	T_WORD_L1	WordData;
} T_SERVER_DATA;

typedef struct {
	T_BIT_L2	BitData;
	T_WORD_L2	WordData;
} T_EQUIPMENT_DATA;


#pragma pack(pop)

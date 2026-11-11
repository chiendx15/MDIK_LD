
#pragma once
#include "Mocha.type.hxx"
#pragma pack(push, 4)

typedef enum eMOTOR{
	eMOTOR_0,
	eMOTOR_1,
	eMOTOR_2,
	eMOTOR_3,
	eMOTOR_4,
	eMOTOR_5,
	eMOTOR_6,
	eMOTOR_7,
	eMOTOR_8,
	eMOTOR_9,
	eMOTOR_10,
	eMOTOR_11,
	eMOTOR_12,
	eMOTOR_13,
	eMOTOR_14,
	eMOTOR_15,
	eMOTOR_16,
	eMOTOR_17,
	eMOTOR_18,
	eMOTOR_19,
	eMOTOR_20,
	eMOTOR_21,
	eMOTOR_22,
	eMOTOR_23,
	eMOTOR_24,
	eMOTOR_25,
	eMOTOR_26,
	eMOTOR_27,
	eMOTOR_28,
	eMOTOR_29,
	eMOTOR_30,
	eMOTOR_31,

	eMOTOR_GLASS_BOX_EV_Z = eMOTOR_0, // Gantry : Motor 1
	eMOTOR_GLASS_TRANSFER_Y, // Motor 3
	eMOTOR_GLASS_TRANSFER_Z, // Motor 4
	eMOTOR_END,
}eMOTOR;
typedef eMOTOR    eAxis;
typedef enum eCH {
	eCH_0,
	eCH_1,
	eCH_2,
	eCH_3,
	eCH_4,
	eCH_5,
	eCH_6,
	eCH_7,
	eCH_8,
	eCH_9,
	eCH_10,
	eCH_11,
	eCH_12,
	eCH_13,
	eCH_14,
	eCH_15,
	eCH_ALL,
	eCH_A = 0,
	eCH_B,
	eCH_C,
	eCH_D,
	eCH_L,
	eCH_R,
	eCH_END,
	eCH_LINE_1 = 0,
	eCH_LINE_2,
	eCH_LINE_END,
}eCH;

typedef enum eCH_AXIS {
	eCH_AXIS_1 = 0,
	eCH_AXIS_2,
	eCH_AXIS_ALL,
	eCH_AXIS_END = eCH_AXIS_ALL,

}eCH_AXIS;

//각 채널별로 상태값을 알고 싶을때 FLAG
typedef enum eCH_GANTRY_AXIS_STATE {
	eCH_GANTRY_AXIS_1_STATE		= 0x01,
	eCH_GANTRY_AXIS_2_STATE		= 0x10,
	eCH_GANTRY_AXIS_ALL_STATE	= 0x11,

}eCH_GANTRY_AXIS_STATE;

typedef enum eCH_GANTRY_WARNING_LEVEL {

	eCH_GANTRY_WARNING_LEVEL_NONE	= 0,
	eCH_GANTRY_WARNING_LEVEL_WARNING,	// 경계 레벨. AxisControl UI의 편차 맞춤 버튼에만 표시.
	eCH_GANTRY_WARNING_LEVEL_ALERT,			// 위험 레벨. ServoOff 및 알람 표시.

}eCH_GANTRY_WARNING_LEVEL;

typedef enum eHOME_TYPE {
	eHT_TORQUE,
	eHT_HOME,
	eHT_NP_CENTER,
	eHT_N_SENSOR,
	eHT_P_SENSOR,
	eHT_DEFAULT,
	eHT_END,

}eHOME_TYPE;

typedef enum eHOME_SENSOR_TYPE {
	eHST_NP_CENTER,
	eHST_N_SENSOR,
	eHST_P_SENSOR,

}eHOME_SENSOR_TYPE;

typedef enum eHOME_STATUS {
	eHOME_STATUS_NOT_COMPLETED,
	eHOME_STATUS_HOMMING,
	eHOME_STATUS_COMPLETED,

}eHOME_STATUS;

// KES 중속 추가 2024.2.15
typedef enum eMOTOR_SPEED {
	eMOTOR_SPEED_LOW,
	eMOTOR_SPEED_MEDIUM,	
	eMOTOR_SPEED_HIGH,
	nMOTOR_SPEED,
} eMOTOR_SPEED;

typedef struct {
	int					iRackNo;
	int					iSlotNo;
	int					iSubSlotNo;
	int					iMotorNo; // 실제 모션 드라이버에서 잡혀있는 번호
	int					iLogicalAxisNo;
	int					iAxisType; // REAL_AXIS :1, VIRTUAL_AXIS :2, EXTERNAL_ENCODER :3
	BOOL				bSetGantry;
	int					iCh1_Axis;
	int					iCh2_Axis;
	int					iCh1_Motor;
	int					iCh2_Motor;
	char				szGantryAddressStop[100];
	int					iGantryRackNo;
	int					iGantrySlotNo;
	int					iGantrySubSlotNo;
	int					iGantryCh;
}T_YASKAWA_AXIS_INIT_PARAM;

typedef struct {
	int					iMethod;
	int					iDirection;
	double				dVelocity;
	double				dApproachVelocity;
	double				dCreepVelocity;
	double				dOffset;
	double				dOffset2Ch;
	double				dOffsetCrt; // Not UI 
	double				dOffsetCrt2; // Not UI 
	int					iHomeSerchTimeout;
	int					iHomeType;
	double				dTorque;
	BOOL				bGantry2ChUse;
	double				dInitPosisition;					// 홈서칭 이후 기본 위치 이동
	char				szExternalHomeSensorAddress[10];	//. Home 센서의 레지스터 신호를 IO의 IN신호로 대체할 경우, ini 세팅 주소값. 

}T_YASKAWA_HOME_PARAM;

typedef struct {
	T_YASKAWA_AXIS_INIT_PARAM	tYaskawa;
	T_YASKAWA_HOME_PARAM		tHome;
	int					iAPITimeout;
	int					iCoordinateSystem;
	int					iMoveType;
	int					iVelocityType;
	int					iAccDecType;
	int					iFilterType;
	int					iDataType;
	double				dPositionRate;		// 위치 포지션 채배
	double				dSpeedRate;			// 속도 채배
	int					iMoveTimeout;		// 위치결정 타임 아웃 // 속도에 대한 이동거리 환산시각 + iMoveTimeout;
	double				dInPosition;		// 위치결정 확인 수치
	double				dDefaultSpped;		// 속도를 입력하지 않은 기본 속도
	double				dGantryOffsetLimitWarning;	// 주의 단계. Gantry일 경우, 축 위치와, ch2 위치의 차이값이 해당 값 보다 크면 heavy alarm. 
	double				dGantryOffsetLimitAlert;	// 심각 단계. Gantry일 경우, 축 위치와, ch2 위치의 차이값이 해당 값 보다 크면 heavy alarm. 

	int					iAcc;				// 기본 가속도 
	int					iDec;				// 기본 감속도
	double				dFilterTime;		// modified by HGH. YASKAWA에 실제 세팅할 때는 us 단위지만, UI 표기상에는 s.
	int					iMaxVelocity;
	double				dLowSpeed;			// 기본 조그 속도 
	double				dMediumSpeed;		// KES 중속 추가 2024.2.15
	double				dHighSpeed;			// 기본 스텝 속도
	double				dPTPSpeed;			// 기본 ptp 속도
	double				dLimitLow;			// 모터 최소 이동위치
	double				dLimitHigh;			// 모터 최대 이동위치
	int					iGantryAxis[eCH_AXIS_END];
} T_MOTOR_PARAMETER;

typedef struct{
	double dCh1Pos;
	double dCh2Pos;
	double dCh1Torque;
	double dCh2Torque;

	DWORD	dwChServoOnStatus;			//. ServoOn 상태eCH_GANTRY_AXIS_STATE값 사용.
	DWORD	dwChAlarmOnStatus;			//. AlarmOn 상태 eCH_GANTRY_AXIS_STATE값 사용.
	DWORD	dwChPLimitStatus;			//. PLimit 상태 eCH_GANTRY_AXIS_STATE값 사용.
	DWORD	dwChNLimitStatus;			//. NLimit 상태 eCH_GANTRY_AXIS_STATE값 사용.
	DWORD	dwChHomeStatus;				//. HomeSensor 상태 eCH_GANTRY_AXIS_STATE값 사용.
	DWORD	dwChInPosStatus;			//. Inpos 상태 eCH_GANTRY_AXIS_STATE값 사용.
	DWORD   dwGantryWarningLevel;		//. MotorParameter dGantryLimitOffset 설정값에 대한 경고 감지 상태. eCH_GANTRY_WARNING_LEVEL 값 사용

	BOOL	bSetData;
}T_GANTRY_DATA;

typedef struct {
	BOOL	bInit;
	BOOL	bCmdJogStop;
	BOOL	bJogStop;
	BOOL	bCmdMoveStop;
	BOOL	bMoveStop;
	double	dLastStartPos;
	double	dLastTargetPos;
	double	dLastTargetSpeed;
	ULONGLONG ullJogStartTick;
	ULONGLONG ullJogEndTick;
	ULONGLONG ullStartTick;		// 시작시간
	ULONGLONG ullEndTick;		// 끝난시간
	ULONGLONG ullTactTime;		// 경과시간
	ULONGLONG ullEstimated_time;	// 예상시간
	BOOL	bIPRun;
	DWORD   dwHomeStatus;		// Home 동작 Status. eHOME_STATUS의 값 참조.
	BOOL	bGantryOn;
	T_GANTRY_DATA tGantryData;
	int iOverride;
	double dPresetPos;
	int iLoadCell;
	ULONGLONG ullPosErrorTick;
	int iMotorDesc;	// KES 추가 2024.3.26 - 모터 동작 실패 원인 (StringTable에서 가져옴 eLANGUAGE 참조)
}T_MOTOR_STATUS_ETC;

typedef struct T_MONITORING_PARAM{
	WORD IWxx00;	//Drive status
	WORD IWxx01;	//Over range parameter number
	DWORD ILxx02;	//Warning
	DWORD ILxx04;	//Alarm
	WORD wSpace06;	//Space
	WORD wSpace07;	//Space
	WORD IWxx08;	//Servo command type response
	WORD IWxx09;	//Servo module command status
	WORD IWxx0A;	//Motion subcommand response code
	WORD IWxx0B;	//Motion subcommand status
	WORD IWxx0C;	//Position management status
	WORD wSpace0D;	//Space
	DWORD ILxx0E;	//Machine coordinate target position(TPOS)
	DWORD ILxx10;	//Target position(CPOS)
	DWORD ILxx12;	//Machine coordinate system position(MPOS)
	DWORD ILxx14;	//32 - bit coordinate system position(DPOS)
	DWORD ILxx16;	//Machine coordinate feedback position(APOS)
	DWORD ILxx18;	//Machine coordinate latch position(LPOS)
	DWORD ILxx1A;	//Position error(PERR)
	DWORD ILxx1C_;	//Target position difference monitor // Not use
	DWORD ILxx1E;	//POSMAX number of turns
	DWORD ILxx20;	//Speed reference output monitor
	WORD wSpace22;	//Space
	WORD wSpace23;	//Space
	DWORD ILxx24_;	//Integral output monitor			// Not use
	DWORD ILxx26_;	//Primary delay monitor				// Not use
	DWORD ILxx28;	//(SVC)M - III Servo Command Input Signal Monitor(SVC)Position loop output monitor
	DWORD ILxx2A;	//M - III Servo Command Status
	WORD IWxx2C;	//(SVC)M - III Command Status(SVB)Servo Driver Status
	WORD IWxx2D;	//Servo alarm code
	WORD IWxx2E_;	//Network servo I / O monitor			// Not use
	WORD IWxx2F;	//Network servo user monitor information
	DWORD ILxx30;	//Servo user monitor 2
	DWORD ILxx32;	//Servo user monitor 3
	DWORD ILxx34;	//Servo user monitor 4
	WORD IWxx36;	//Servo constant number
	WORD IWxx37;	//Auxiliary servo user constant number
	DWORD ILxx38;	//Servo user constant
	DWORD ILxx3A;	//Auxiliary servo user constant
	WORD wSpace3C;	//Space
	WORD wSpace3D;	//Space
	WORD wSpace3E;	//Space
	WORD IWxx3F;	//Motor type
	DWORD ILxx40;	//Feedback speed
	DWORD ILxx42;	//Torque(thrust) reference monitor
	WORD IWxx44;	//Latch Completion Sequence Number	// Not use
	WORD IWxx45;	//Number of Continuous Latch Sequence Completion Cycles	// Not use
	WORD wSpace46;	//Space
	WORD wSpace47;	//Space
	WORD wSpace48;	//Space
	WORD wSpace49;	//Space
	DWORD ILxx4A;	//The number of accumulated rotations of absolute value encoder // Not use
	DWORD ILxx4C;	//The number of initial incremental pulses	// Not use
	WORD wSpace4E;	//Space
	WORD wSpace4F;	//Space
	WORD wSpace50;	//Space
	WORD wSpace51;	//Space
	WORD wSpace52;	//Space
	WORD wSpace53;	//Space
	WORD wSpace54;	//Space
	WORD wSpace55;	//Space
	DWORD ILxx56;	//Fixed parameter monitor
	WORD IWxx58_;	//General - purpose DI monitor					// Not use
	WORD IWxx59_;	//General - purpose AI monitor 1				// Not use
	WORD IWxx5A_;	//General - purpose AI monitor 2				// Not use
	WORD IWxx5B;	//Device Information Monitor Code
	WORD wSpace5C;	//Space
	WORD wSpace5D;	//Space
	DWORD ILxx5E;	//Absolute position at power OFF(low value)
	DWORD ILxx60;	//Absolute position at power OFF(high value)
	DWORD ILxx62;	//Modularized position at power OFF(low value)
	DWORD ILxx64;	//Modularized position at power OFF(high value)
	DWORD ILxx66_;	//Monitor data status						// Not use
	DWORD ILxx68;	//(SVA)Monitor data(SVC)Command buffer for transparent command mode
	DWORD ILxx6A;	//Command buffer for transparent command mode
	DWORD ILxx6C;	//Command buffer for transparent command mode
	DWORD ILxx6E;	//Command buffer for transparent command mode
}T_MONITORING_PARAM;

typedef struct {
	double	dPos;
	double	dSpeed;
	double	dTorque;
	BOOL	bSetGantry;
	T_GANTRY_DATA tGantryData;
	T_MONITORING_PARAM tSubGantryData;
	int		iOverride;
	//IWxx00 
	BOOL	bOperationReady;		//Bit0
	BOOL	bServoOn;			//Bit1
	BOOL	bSystemBusy;			//Bit2
	BOOL	bServoReady;			//Bit3
	//
	//end IWxx00
	//ILxx04
	BOOL	bServoError;			//Bit0
	BOOL	bFollowingError;		//Bit9
	BOOL	bSpeedError;			//Bit8
	BOOL	bZeroPointNotSet;		//Bit13 Origin??;
	//BOOL	bServoPackMotorError;	//Bit20 // 번호가 확실지 않음 
	//BOOL	bServoPackEncoderError;	//Bit21 // 번호가 확실지 않음 
	//end ILxx04
	//IWxx0C
	BOOL	bInPosition;		//Bit1
	BOOL	bNEARPosition;
	BOOL	bZeroPointPosition;
	BOOL	bZeroPointReturnSet;	//Bit5
	//end IWxx0C
	//IWxx2C
	BOOL	bAlarm;				//Bit0
	BOOL	bWarning;			//Bit1
	BOOL	bCmdReady;			//Bit2
	BOOL	bMainPower;			//Bit4
	BOOL	bZeroSpeed;
	BOOL	bTorqueLimit;		//Bit9
	BOOL	bSoftPLimit;		//Bit12
	BOOL	bSoftNLimit;		//Bit13
	//End IWxx0C
	//IWxx2E
	BOOL	bPLimit;			//BIT0
	BOOL	bNLimit;			//BIT1
	BOOL	bHomeSensor;		//BIT6 // 제 1외부 래치 입력(EXT1)을 나타냅니다.
	BOOL	bBreak;				//BIT9
	//End IWxx2E
	T_MONITORING_PARAM tMonitoring;

}T_MOTOR_STATUS_CRT;

typedef struct {
	T_MOTOR_STATUS_CRT tCrt; // 실시간 갱신값.
	T_MOTOR_STATUS_ETC tEtc; // 임시 저장소.
}T_MOTOR_STATUS;

typedef struct T_POS {
	int iAxis;
	double dTarget;
	double dSpeed;
#ifndef _IP_DATA_INTERNAL_
public:
	T_POS() {
		iAxis = -1;
		dTarget = 0.;
		dSpeed = 0.;
	}

	FEM_BEGIN_TBL(T_POS)
		FEM_ADD_MEMBER(iAxis)
		FEM_ADD_MEMBER(dTarget)
		FEM_ADD_MEMBER(dSpeed)
	FEM_END_TBL()
	FEM_SERIALIZE(T_POS)
#endif
}T_POS;

typedef struct T_POS_PARAM {
	wchar_t	strName[100];
	eMOTOR	eAxis;
	COLORREF cr;
}T_POS_PARAM;

typedef struct {
	int iAxis;
	double dTarget;
}T_MOTOR_LINER;

typedef struct{
	int iBoardIndex;
	char	strINStartAddress[20];
	char	strOUTStartAddress[20];
	int iIOCount;
}T_IO_BOARD_PARAMETER;

typedef enum neBOARD{
	eIO_BOARD_1,
	eIO_BOARD_END,
} neBOARD;


typedef struct{
	DWORD	dwAlarmCode;				
	DWORD	dwOverdrive;				
	DWORD	dwABS_Status_Tolerance;
	DWORD	dw1006;
	DWORD	dw1008;
	DWORD	dw1010;
	DWORD	dw1012;				
	DWORD	dw1014;				
	DWORD	dw1016;				
	DWORD	dw1018;				
	union {
		struct {
			DWORD dwGlassBoxMaterial0 : 1;
			DWORD dwGlassBoxMaterial1 : 1;
			DWORD dwGlassBoxMaterial2 : 1;
			DWORD dwGlassBoxMaterial3 : 1;
			DWORD dwGlassBoxMaterial4 : 1;
			DWORD dwGlassBoxMaterial5 : 1;
			DWORD dwGlassBoxMaterial6 : 1;
			DWORD dwGlassBoxMaterial7 : 1;
			DWORD dwGlassBoxMaterial8 : 1;
			DWORD dw9 : 1;
			DWORD dwA : 1;
			DWORD dwB : 1;
			DWORD dwC : 1;
			DWORD dwD : 1;
			DWORD dwE : 1;
			DWORD dwF : 1;
		};
		DWORD dwGlassBoxMaterial;
	};
	union {
		struct {
			DWORD dwPaperBoxMaterial0 : 1;
			DWORD dwPaperBoxMaterial1 : 1;
			DWORD dwPaperBoxMaterial2 : 1;
			DWORD dwPaperBoxMaterial3 : 1;
			DWORD dwPaperBoxMaterial4 : 1;
			DWORD dwPaperBoxMaterial5 : 1;
			DWORD dwPaperBoxMaterial6 : 1;
			DWORD dwPaperBoxMaterial7 : 1;
			DWORD dwPaperBoxMaterial8 : 1;
			DWORD dw9 : 1;
			DWORD dwA : 1;
			DWORD dwB : 1;
			DWORD dwC : 1;
			DWORD dwD : 1;
			DWORD dwE : 1;
			DWORD dwF : 1;
		};
		DWORD dwPaperBoxMaterial;
	};
	//DWORD	dwGlassBoxMaterial;		// Glass Box Conveyor Material 정보 (0~8 BIT 연산)
	//DWORD	dwPaperBoxMaterial;		// Paper Box Conveyor Material 정보 (0~8 BIT 연산)
	//DWORD	dwMachineStatus;		// 0 : 서보온 / 1 : 운전개시 / 2 : 알람 발생 / 3 : 워닝 발생
	union {
		struct {
			DWORD dwServoOn: 1;
			DWORD dwAutoRun : 1;
			DWORD dwHeavyAlarm : 1;
			DWORD dwLightAlarm : 1;
			DWORD dwMuteAlarmClear : 1;
			DWORD dw5 : 1;
			DWORD dw6 : 1;
			DWORD dw7 : 1;
			DWORD dw8 : 1;
			DWORD dw9 : 1;
			DWORD dwA : 1;
			DWORD dwB : 1;
			DWORD dwC : 1;
			DWORD dwD : 1;
			DWORD dwE : 1;
			DWORD dwF : 1;
		};
		DWORD dwMachineStatus;
	};
	DWORD	dw1026;
	DWORD	dw1028;	
	DWORD	dw1030;
	DWORD	dw1032;
	DWORD	dw1034;
	DWORD	dw1036;
	DWORD	dw1038;
	DWORD	dwAlarmCode1;				
	DWORD	dwAlarmCode2;				
	DWORD	dwAlarmCode3;				
	DWORD	dwAlarmCode4;				
	DWORD	dwAlarmCode5;				
	DWORD	dwAlarmCode6;				
	DWORD	dwAlarmCode7;				
	DWORD	dwAlarmCode8;				
	DWORD	dwAlarmCode9;				
	DWORD	dwAlarmCode10;				
	DWORD	dwAlarmCode11;	
	DWORD	dwAlarmCode12;
	DWORD	dwAlarmCode13;				
	DWORD	dwAlarmCode14;				
	DWORD	dwAlarmCode15;		
	DWORD	dwAlarmCode16;	
	DWORD	dwAlarmCode17;
	DWORD	dwAlarmCode18;		
	DWORD	dwAlarmCode19;
	DWORD	dwAlarmCode20;	
	DWORD	dw1080;				
	DWORD	dw1082;				
	DWORD	dw1084;				
	DWORD	dw1086;				
	DWORD	dw1088;				
	DWORD	dw1090;				
	DWORD	dw1092;				
	DWORD	dw1094;				
	DWORD	dw1096;				
	DWORD	dw1098;				
	DWORD	dwBreakTable_1_0_Position;		// 12.28 Tilt Offset -> 0도 티칭 값으로 수정
	DWORD	dwBreakTable_1_Speed;
	DWORD	dwBreakTable_1_AccSpeed;
	DWORD	dwBreakTable_1_DecSpeed;
	DWORD	dwBreakTable_1_Tilt_T;
	DWORD	dwBreakTable_1_Offset_Z;
	DWORD	dwBreakTable_1_Offset_Y;
	DWORD	dwBreakTable_1_StickOut_Distance;
	DWORD	dwBreakTable_1_Glass_Thickness;
	DWORD	dwBreakTable_1_Type;	// 0 : Normal(테이블 엣지 기준 틸트), 1 : Data 적용(Glass 기준 틸트), 2 : Data + 반송(Glass 기준 틸트 후 Offset Y 반송)
	DWORD	dwBreakTable_2_0_Position;		// 12.28 Tilt Offset -> 0도 티칭 값으로 수정				
	DWORD	dwBreakTable_2_Speed;
	DWORD	dwBreakTable_2_AccSpeed;
	DWORD	dwBreakTable_2_DecSpeed;
	DWORD	dwBreakTable_2_Tilt_T;
	DWORD	dwBreakTable_2_Offset_Z;
	DWORD	dwBreakTable_2_Offset_Y;
	DWORD	dwBreakTable_2_StickOut_Distance;
	DWORD	dwBreakTable_2_Glass_Thickness;
	DWORD	dwBreakTable_2_Type;	// 0 : Normal(테이블 엣지 기준 틸트), 1 : Data 적용(Glass 기준 틸트), 2 : Data + 반송(Glass 기준 틸트 후 Offset Y 반송)				
	DWORD	dwMVS3_MaxLimitSpeed;				
	DWORD	dwMVS3_MoveSpeed;					
	DWORD	dwMVS3_Acc;							
	DWORD	dwMVS3_Dec;							
	DWORD	dwMVS3_1AxisMoveDistacne;			
	DWORD	dwMVS3_2AxisMoveDistacne;			
	DWORD	dwMVS3_3AxisMoveDistacne;			
	DWORD	dw1154;				
	DWORD	dw1156;				
	DWORD	dw1158;				
	DWORD	dwMVS4_MaxLimitSpeed;					
	DWORD	dwMVS4_MoveSpeed;						
	DWORD	dwMVS4_Acc;								
	DWORD	dwMVS4_Dec;								
	DWORD	dwMVS4_1AxisMoveDistacne;				
	DWORD	dwMVS4_2AxisMoveDistacne;				
	DWORD	dwMVS4_3AxisMoveDistacne;				
	DWORD	dw1174;				
	DWORD	dw1176;				
	DWORD	dw1178;				
	DWORD	dwMVS5_MaxLimitSpeed;				
	DWORD	dwMVS5_MoveSpeed;					
	DWORD	dwMVS5_Acc;							
	DWORD	dwMVS5_Dec;							
	DWORD	dwMVS5_1AxisMoveDistacne;			
	DWORD	dwMVS5_2AxisMoveDistacne;			
	DWORD	dwMVS5_3AxisMoveDistacne;			
	DWORD	dw1194;				
	DWORD	dw1196;				
	DWORD	dw1198;
} T_REGISTER_WORD_1;

typedef struct{
	DWORD	dwLINE_1_SCRIBE_X_START_POS;						// um
	DWORD	dwLINE_1_SCRIBE_SHIFT_START_POS;					// um
	DWORD	dwLINE_1_SCRIBE_X_END_POS;							// um
	DWORD	dwLINE_1_SCRIBE_SHIFT_END_POS;						// um
	DWORD	dwLINE_1_SCRIBE_Z_AXIS_GLASS_SURFACE_POS;			// um
	DWORD	dwLINE_1_SCRIBE_DEPTH_OF_CUT;		
	DWORD	dwLINE_1_SCRIBE_SPEED;								// um/sec
	DWORD	dwLINE_1_SCRIBE_WEIGHT;								// 토크 % 값인데 소수점 두자리여서 100곱해서 적용 (0~10000)
	DWORD	dwLINE_1_1_INTERVAL_SCRIBE_SPEED;					// um/sec
	DWORD	dwLINE_1_1_INTERVAL_SCRIBE_WEIGHT;					// 토크 % 값인데 소수점 두자리여서 100곱해서 적용 (0~10000)
	DWORD	dwLINE_1_1_INTERVAL_SCRIBE_DISTANCE;				// um
	DWORD	dwLINE_1_3_INTERVAL_SCRIBE_SPEED;					// um/sec
	DWORD	dwLINE_1_3_INTERVAL_SCRIBE_WEIGHT;					// 토크 % 값인데 소수점 두자리여서 100곱해서 적용 (0~10000)
	DWORD	dwLINE_1_3_INTERVAL_SCRIBE_DISTANCE;				// um
	DWORD	dw2028;		
	DWORD	dwLINE_1_ACCELATION_SPEED;							// um/sec
	DWORD	dwLINE_1_DECELATION_SPEED;							// um/sec
	DWORD	dwLINE_1_1_DEPTH_OF_CUT;		// 1구간 절입량
	DWORD	dwLINE_1_3_DEPTH_OF_CUT;		// 3구간 절입량
	DWORD	dw2038;		
	DWORD	dwLINE_1_TILT_AFTER_OFFSET_Y;						// 12.28 틸트 후 반송량 추가
	DWORD	dw2042;		
	DWORD	dw2044;		
	DWORD	dw2046;		
	DWORD	dwGLASS_THICKNESS;		
	DWORD	dwLINE_2_SCRIBE_X_START_POS;
	DWORD	dwLINE_2_SCRIBE_SHIFT_START_POS;
	DWORD	dwLINE_2_SCRIBE_X_END_POS;
	DWORD	dwLINE_2_SCRIBE_SHIFT_END_POS;
	DWORD	dwLINE_2_SCRIBE_Z_AXIS_GLASS_SURFACE_POS;
	DWORD	dwLINE_2_SCRIBE_DEPTH_OF_CUT;
	DWORD	dwLINE_2_SCRIBE_SPEED;
	DWORD	dwLINE_2_SCRIBE_WEIGHT;			
	DWORD	dwLINE_2_1_INTERVAL_SCRIBE_SPEED;		
	DWORD	dwLINE_2_1_INTERVAL_SCRIBE_WEIGHT;			
	DWORD	dwLINE_2_1_INTERVAL_SCRIBE_DISTANCE;	
	DWORD	dwLINE_2_3_INTERVAL_SCRIBE_SPEED;		
	DWORD	dwLINE_2_3_INTERVAL_SCRIBE_WEIGHT;			
	DWORD	dwLINE_2_3_INTERVAL_SCRIBE_DISTANCE;
	DWORD	dw2078;		
	DWORD	dwLINE_2_ACCELATION_SPEED;		
	DWORD	dwLINE_2_DECELATION_SPEED;		
	DWORD	dwLINE_2_1_DEPTH_OF_CUT;		// 1구간 절입량		
	DWORD	dwLINE_2_3_DEPTH_OF_CUT;		// 3구간 절입량		
	DWORD	dw2088;		
	DWORD	dwLINE_2_TILT_AFTER_OFFSET_Y;						// 12.28 틸트 후 반송량 추가
	DWORD	dw2092;		
	DWORD	dw2094;		
	DWORD	dw2096;		
	DWORD	dw2098;		
	DWORD	dwLINE_1_CUTTERT_ZERO_DETECT_MEASURE_POS	;		
	DWORD	dwLINE_1_ZERO_DETECT_STANDARD_TORQUE;		
	DWORD	dwLINE_1_ZERO_DETECT_STANDARD_SPEED;		
	DWORD	dwLINE_1_CUTTER_ZERO_DETECT_RANGE_UP_POS;		
	DWORD	dwLINE_1_CUTTER_ZERO_DETECT_RANGE_DOWN_POS;		
	DWORD	dw2110;		
	DWORD	dw2112;		
	DWORD	dw2114;		
	DWORD	dw2116;		
	DWORD	dw2118;		
	DWORD	dwLINE_2_CUTTERT_ZERO_DETECT_MEASURE_POS		;		
	DWORD	dwLINE_2_ZERO_DETECT_STANDARD_TORQUE;		
	DWORD	dwLINE_2_ZERO_DETECT_STANDARD_SPEED;		
	DWORD	dwLINE_2_CUTTER_ZERO_DETECT_RANGE_UP_POS		;		
	DWORD	dwLINE_2_CUTTER_ZERO_DETECT_RANGE_DOWN_POS		;		
	DWORD	dw2130;		
	DWORD	dw2132;		
	DWORD	dw2134;		
	DWORD	dw2136;		
	DWORD	dw2138;		
	DWORD	dw2140;		
	DWORD	dw2142;		
	DWORD	dw2144;		
	DWORD	dw2146;		
	DWORD	dw2148;		
	DWORD	dw2150;		
	DWORD	dwSC_CHUCK1_ZERO_DETECT_MEASURE_POS;		
	DWORD	dwSC_CHUCK1_ZERO_DETECT_STANDARD_TORQUE;		
	DWORD	dwSC_CHUCK1_ZERO_DETECT_STANDARD_SPEED	;		
	DWORD	dwSC_CHUCK1_ZERO_DETECT_RANGE_UP_POS;		
	DWORD	dwSC_CHUCK1_ZERO_DETECT_RANGE_DOWN_POS	;		
	DWORD	dw2162;		
	DWORD	dw2164;		
	DWORD	dw2166;		
	DWORD	dw2168;		
	DWORD	dw2170;		
	DWORD	dwSC_CHUCK2_ZERO_DETECT_MEASURE_POS;		
	DWORD	dwSC_CHUCK2_ZERO_DETECT_STANDARD_TORQUE;		
	DWORD	dwSC_CHUCK2_ZERO_DETECT_STANDARD_SPEED;		
	DWORD	dwSC_CHUCK2_ZERO_DETECT_RANGE_UP_POS;		
	DWORD	dwSC_CHUCK2_ZERO_DETECT_RANGE_DOWN_POS	;		
	DWORD	dw2182;		
	DWORD	dw2184;		
	DWORD	dw2186;		
	DWORD	dw2188;		
	DWORD	dwSC_CHUCK3_ZERO_DETECT_MEASURE_POS;		
	DWORD	dwSC_CHUCK3_ZERO_DETECT_STANDARD_TORQUE;		
	DWORD	dwSC_CHUCK3_ZERO_DETECT_STANDARD_SPEED;		
	DWORD	dwSC_CHUCK3_ZERO_DETECT_RANGE_UP_POS;		
	DWORD	dwSC_CHUCK3_ZERO_DETECT_RANGE_DOWN_POS;		
	DWORD	dw2200;	
	DWORD	dw2202;	
	DWORD	dw2204;	
	DWORD	dw2206;	
	DWORD	dw2208;	
	DWORD	dwSC_CHUCK4_ZERO_DETECT_MEASURE_POS;	
	DWORD	dwSC_CHUCK4_ZERO_DETECT_STANDARD_TORQUE;	
	DWORD	dwSC_CHUCK4_ZERO_DETECT_STANDARD_SPEED;	
	DWORD	dwSC_CHUCK4_ZERO_DETECT_RANGE_UP_POS;	
	DWORD	dwSC_CHUCK4_ZERO_DETECT_RANGE_DOWN_POS;	
	DWORD	dw2220;	
	DWORD	dw2222;	
	DWORD	dw2224;	
	DWORD	dw2226;	
	DWORD	dwSC_CHUCK5_ZERO_DETECT_MEASURE_POS;	
	DWORD	dwSC_CHUCK5_ZERO_DETECT_STANDARD_TORQUE;	
	DWORD	dwSC_CHUCK5_ZERO_DETECT_STANDARD_SPEED;	
	DWORD	dwSC_CHUCK5_ZERO_DETECT_RANGE_UP_POS;	
	DWORD	dwSC_CHUCK5_ZERO_DETECT_RANGE_DOWN_POS;	
	DWORD	dw2238;	
	DWORD	dw2240;	
	DWORD	dw2242;	
	DWORD	dw2244;	
	DWORD	dw2246;	
	DWORD	dw2248;	
	DWORD	dwSC_CHUCK6_ZERO_DETECT_MEASURE_POS;	
	DWORD	dwSC_CHUCK6_ZERO_DETECT_STANDARD_TORQUE;	
	DWORD	dwSC_CHUCK6_ZERO_DETECT_STANDARD_SPEED;	
	DWORD	dwSC_CHUCK6_ZERO_DETECT_RANGE_UP_POS;	
	DWORD	dwSC_CHUCK6_ZERO_DETECT_RANGE_DOWN_POS;	
	DWORD	dw2260;	
	DWORD	dw2262;	
	DWORD	dw2264;	
	DWORD	dw2266;	
	DWORD	dw2268;	
	DWORD	dw2270;	
	DWORD	dw2272;	
	DWORD	dw2274;	
	DWORD	dw2276;	
	DWORD	dw2278;	
	DWORD	dw2280;	
	DWORD	dw2282;	
	DWORD	dw2284;	
	DWORD	dw2286;	
	DWORD	dw2288;	
	DWORD	dw2290;	
	DWORD	dw2292;	
	DWORD	dw2294;	
	DWORD	dw2296;	
	DWORD	dw2298;	
	DWORD	dw2300;	
	DWORD	dw2302;	
	DWORD	dw2304;	
	DWORD	dw2306;	
	DWORD	dw2308;	
	DWORD	dw2310;	
	DWORD	dw2312;	
	DWORD	dw2314;	
	DWORD	dw2316;	
	DWORD	dw2318;	
	DWORD	dw2320;	
	DWORD	dw2322;	
	DWORD	dw2324;	
	DWORD	dw2326;	
	DWORD	dw2328;	
	DWORD	dw2330;	
	DWORD	dw2332;	
	DWORD	dw2334;	
	DWORD	dw2336;	
	DWORD	dw2338;	
	DWORD	dw2340;	
	DWORD	dw2342;	
	DWORD	dw2344;	
	DWORD	dw2346;	
	DWORD	dw2348;	
	DWORD	dw2350;	
	DWORD	dw2352;	
	DWORD	dw2354;	
	DWORD	dw2356;	
	DWORD	dw2358;	
	DWORD	dw2360;	
	DWORD	dw2362;	
	DWORD	dw2364;	
	DWORD	dw2366;	
	DWORD	dw2368;	
	DWORD	dw2370;	
	DWORD	dw2372;	
	DWORD	dw2374;	
	DWORD	dw2376;	
	DWORD	dw2378;	
	DWORD	dw2380;	
	DWORD	dw2382;	
	DWORD	dw2384;	
	DWORD	dw2386;	
	DWORD	dw2388;	
	DWORD	dw2390;	
	DWORD	dw2392;	
	DWORD	dw2394;	
	DWORD	dw2396;	
	DWORD	dw2398;	
	DWORD	dw2400;	
	DWORD	dw2402;	
	DWORD	dw2404;	
	DWORD	dw2406;	
	DWORD	dw2408;	
	DWORD	dw2410;	
	DWORD	dw2412;	
	DWORD	dw2414;	
	DWORD	dw2416;	
	DWORD	dw2418;	
	DWORD	dw2420;	
	DWORD	dw2422;	
	DWORD	dw2424;	
	DWORD	dw2426;	
	DWORD	dw2428;	
	DWORD	dw2430;	
	DWORD	dw2432;	
	DWORD	dw2434;	
	DWORD	dw2436;	
	DWORD	dw2438;	
	DWORD	dw2440;	
	DWORD	dw2442;	
	DWORD	dw2444;	
	DWORD	dw2446;	
	DWORD	dw2448;	
	DWORD	dw2450;	
	DWORD	dw2452;	
	DWORD	dw2454;	
	DWORD	dw2456;	
	DWORD	dw2458;	
	DWORD	dw2460;	
	DWORD	dw2462;	
	DWORD	dw2464;	
	DWORD	dw2466;	
	DWORD	dw2468;	
	DWORD	dw2470;	
	DWORD	dw2472;	
	DWORD	dw2474;	
	DWORD	dw2476;	
	DWORD	dw2478;	
	DWORD	dw2480;	
	DWORD	dw2482;	
	DWORD	dw2484;	
	DWORD	dw2486;	
	DWORD	dw2488;	
	DWORD	dw2490;	
	DWORD	dw2492;	
	DWORD	dw2494;	
	DWORD	dw2496;	
	DWORD	dw2498;	
} T_REGISTER_WORD_2;
typedef struct {
	DWORD	dwBarCodeInfo1;
	DWORD	dwBarCodeInfo2;
	DWORD	dwBarCodeInfo3;
	DWORD	dwBarCodeInfo4;
	DWORD	dwBarCodeInfo5;
	DWORD	dwBarCodeInfo6;
	DWORD	dwBarCodeInfo7;
	DWORD	dwBarCodeInfo8;
	DWORD	dwBarCodeInfo9;
	DWORD	dwBarCodeInfo10;
	DWORD	dw5020;
	DWORD	dw5022;
	DWORD	dw5024;
	DWORD	dw5026;
	DWORD	dw5028;
	DWORD	dw5030;
	DWORD	dw5032;
	DWORD	dw5034;
	DWORD	dw5036;
	DWORD	dw5038;
	DWORD	dw5040;
	DWORD	dw5042;
	DWORD	dw5044;
	DWORD	dw5046;
	DWORD	dw5048;
	DWORD	dw5050;
	DWORD	dw5052;
	DWORD	dw5054;
	DWORD	dw5056;
	DWORD	dw5058;
	DWORD	dw1LineAbsStatusCheck;
	DWORD	dw2LineAbsStatusCheck;
	DWORD	dw5LineAbsStatusCheck;
	DWORD	dw6LineAbsStatusCheck;
	DWORD	dw7LineAbsStatusCheck;
	DWORD	dw8LineAbsStatusCheck;
	DWORD	dw9LineAbsStatusCheck;
	DWORD	dw5074;
	DWORD	dw5076;
	DWORD	dw5078;
	DWORD	dw5080;
	DWORD	dw5082;
	DWORD	dw5084;
	DWORD	dw5086;
	DWORD	dw5088;
	DWORD	dw5090;
	DWORD	dw5092;
	DWORD	dw5094;
	DWORD	dw5096;
	DWORD	dw5098;
	DWORD	dw5100;
	DWORD	dw5102;
	DWORD	dw5104;
	DWORD	dw5106;
	DWORD	dw5108;
	DWORD	dw5110;
	DWORD	dw5112;
	DWORD	dw5114;
	DWORD	dw5116;
	DWORD	dw5118;
	DWORD	dw5120;
	DWORD	dw5122;
	DWORD	dw5124;
	DWORD	dw5126;
	DWORD	dw5128;
	DWORD	dw5130;
	DWORD	dw5132;
	DWORD	dw5134;
	DWORD	dw5136;
	DWORD	dw5138;
	DWORD	dw5140;
	DWORD	dw5142;
	DWORD	dw5144;
	DWORD	dw5146;
	DWORD	dw5148;
	DWORD	dw5150;
	DWORD	dw5152;
	DWORD	dw5154;
	DWORD	dw5156;
	DWORD	dw5158;
	DWORD	dw5160;
	DWORD	dw5162;
	DWORD	dw5164;
	DWORD	dw5166;
	DWORD	dw5168;
	DWORD	dw5170;
	DWORD	dw5172;
	DWORD	dw5174;
	DWORD	dw5176;
	DWORD	dw5178;
	DWORD	dw5180;
	DWORD	dw5182;
	DWORD	dw5184;
	DWORD	dw5186;
	DWORD	dw5188;
	DWORD	dw5190;
	DWORD	dw5192;
	DWORD	dw5194;
	DWORD	dw5196;
	DWORD	dw5198;
	DWORD	dwAixsCurrentPosSave[112];		// 축 현재 좌표 저장하는
	DWORD	dw5424;
	DWORD	dw5426;
	DWORD	dw5428;
	DWORD	dw5430;
	DWORD	dw5432;
	DWORD	dw5434;
	DWORD	dw5436;
	DWORD	dw5438;
} T_REGISTER_WORD_3;
typedef struct{
	DWORD	dw1000_AlarmReset;
	DWORD	dwMuteAlarm;				// Mute Alarm 발생
	DWORD	dw1002_PDT_TEACH_MODE;
	DWORD	dw1003;
	DWORD	dw1004;
	DWORD	dw1005;
	DWORD	dw1006;
	DWORD	dw1007;
	DWORD	dw1008;
	DWORD	dw1009;
	DWORD	dw100A;
	DWORD	dw100B;
	DWORD	dw100C;
	DWORD	dw100D;
	DWORD	dw100E;
	DWORD	dw100F;
	
	DWORD	dw1010_LINE_1_SCRIBE_START_ABLE;
	DWORD	dw1011_LINE_1_SCRIBE_START_REQUEST;
	DWORD	dw1012_LINE_1_SCRIBE_START_COMPLETE_RESP;
	DWORD	dw1013_LINE_1_SCRIBE_STOP;
	DWORD	dw1014_LINE_2_SCRIBE_START_ABLE;
	DWORD	dw1015_LINE_2_SCRIBE_START_REQUEST;
	DWORD	dw1016_LINE_2_SCRIBE_START_COMPLETE_RESP;
	DWORD	dw1017_LINE_2_SCRIBE_STOP;
	DWORD	dw1018;
	DWORD	dw1019;
	DWORD	dw101A_LINE_1_SCRIBE_RETRY;
	DWORD	dw101B_LINE_2_SCRIBE_RETRY;
	DWORD	dw101C_LINE_1_SKIP_DEPTH_OF_CUT_ALARM;
	DWORD	dw101D_LINE_2_SKIP_DEPTH_OF_CUT_ALARM;
	DWORD	dw101E_LINE_1_CUT_DEPTH_OFF;
	DWORD	dw101F_LINE_2_CUT_DEPTH_OFF;

	DWORD	dw1020_LINE_1_ZERO_START_ABLE;
	DWORD	dw1021_LINE_1_ZERO_START_REQU;	
	DWORD	dw1022_LINE_1_ZERO_COMPLETE_RESP;	
	DWORD	dw1023;	
	DWORD	dw1024_LINE_2_ZERO_START_ABLE;	
	DWORD	dw1025_LINE_2_ZERO_START_REQU;	
	DWORD	dw1026_LINE_2_ZERO_COMPLETE_RESP;	
	DWORD	dw1027;	
	DWORD	dw1028;	
	DWORD	dw1029;	
	DWORD	dw102A;	
	DWORD	dw102B;	
	DWORD	dw102C;	
	DWORD	dw102D;	
	DWORD	dw102E;	
	DWORD	dw102F;	

	DWORD	dw1030_LINE_1_CHUCK1_ZERO_DETECT_START_ABLE;	
	DWORD	dw1031_LINE_1_CHUCK1_ZERO_DETECT_START_REQU;	
	DWORD	dw1032_LINE_1_CHUCK1_ZERO_DETECT_COMPLETE_RESP;
	DWORD	dw1033_LINE_1_CHUCK2_ZERO_DETECT_START_ABLE;	
	DWORD	dw1034_LINE_1_CHUCK2_ZERO_DETECT_START_REQU;	
	DWORD	dw1035_LINE_1_CHUCK2_ZERO_DETECT_COMPLETE_RESP;
	DWORD	dw1036_LINE_1_CHUCK3_ZERO_DETECT_START_ABLE;		
	DWORD	dw1037_LINE_1_CHUCK3_ZERO_DETECT_START_REQU;	
	DWORD	dw1038_LINE_1_CHUCK3_ZERO_DETECT_COMPLETE_RESP;
	DWORD	dw1039;	
	DWORD	dw103A;	
	DWORD	dw103B;	
	DWORD	dw103C;	
	DWORD	dw103D;	
	DWORD	dw103E;	
	DWORD	dw103F;	

	DWORD	dw1040_LINE_2_CHUCK1_ZERO_DETECT_START_ABLE;	
	DWORD	dw1041_LINE_2_CHUCK1_ZERO_DETECT_START_REQU;	
	DWORD	dw1042_LINE_2_CHUCK1_ZERO_DETECT_COMPLETE_RESP;	
	DWORD	dw1043_LINE_2_CHUCK2_ZERO_DETECT_START_ABLE;	
	DWORD	dw1044_LINE_2_CHUCK2_ZERO_DETECT_START_REQU;	
	DWORD	dw1045_LINE_2_CHUCK2_ZERO_DETECT_COMPLETE_RESP;	
	DWORD	dw1046_LINE_2_CHUCK3_ZERO_DETECT_START_ABLE;	
	DWORD	dw1047_LINE_2_CHUCK3_ZERO_DETECT_START_REQU;	
	DWORD	dw1048_LINE_2_CHUCK3_ZERO_DETECT_COMPLETE_RESP;	
	DWORD	dw1049;	
	DWORD	dw104A;	
	DWORD	dw104B;	
	DWORD	dw104C;	
	DWORD	dw104D;	
	DWORD	dw104E;	
	DWORD	dw104F;	

	DWORD	dw1050_LINE_1_BREAK_TABLE_START_ABLE;	
	DWORD	dw1051_LINE_1_BREAK_TABLE_START_REQU;	
	DWORD	dw1052_LINE_1_BREAK_TABLE_COMPLETE_RESP;	
	DWORD	dw1053_LINE_2_BREAK_TABLE_START_ABLE;
	DWORD	dw1054_LINE_2_BREAK_TABLE_START_REQU;	
	DWORD	dw1055_LINE_2_BREAK_TABLE_COMPLETE_RESP;	
	DWORD	dw1056_MVS3_START_ABLE;	
	DWORD	dw1057_MVS3_START_REQU;	
	DWORD	dw1058_MVS3_COMPLETE_RESP;	
	DWORD	dw1059_MVS4_START_ABLE;	
	DWORD	dw105A_MVS4_START_REQU;	
	DWORD	dw105B_MVS4_COMPLETE_RESP;
	DWORD	dw105C_MVS5_START_ABLE;	
	DWORD	dw105D_MVS5_START_REQU;	
	DWORD	dw105E_MVS5_COMPLETE_RESP;
	DWORD	dw105F;	

	DWORD	dw1060_LINE_1_BREAK_TABLE_STOP_REQUEST;
	DWORD	dw1061_LINE_2_BREAK_TABLE_STOP_REQUEST;
	DWORD	dw1062;
	DWORD	dw1063;
	DWORD	dw1064;
	DWORD	dw1065;
	DWORD	dw1066;
	DWORD	dw1067;
	DWORD	dw1068;
	DWORD	dw1069;
	DWORD	dw106A_LINE1_TILT_RETRY;
	DWORD	dw106B_LINE2_TILT_RETRY;
	DWORD	dw106C;
	DWORD	dw106D;
	DWORD	dw106E;
	DWORD	dw106F;	

	DWORD	dw107[16];
	DWORD	dw108[16];
	DWORD	dw109[16];
	
	DWORD	dw110[16];
	DWORD	dw111[16];
	DWORD	dw112[16];
}T_REGISTER_PC_OUT;

typedef struct{
	DWORD	dw2000_AlarmResetCheck;
	DWORD	dw2001;
	DWORD	dw2002;
	DWORD	dw2003;
	DWORD	dw2004;
	DWORD	dw2005;
	DWORD	dw2006;
	DWORD	dw2007;
	DWORD	dw2008;
	DWORD	dw2009;
	DWORD	dw200A;
	DWORD	dw200B;
	DWORD	dw200C;
	DWORD	dw200D;
	DWORD	dw200E;
	DWORD	dw200F;

	DWORD	dw2010_LINE_1_SCRIBE_START_ABLE;
	DWORD	dw2011_LINE_1_SCRIBE_START_REQUEST;
	DWORD	dw2012_LINE_1_SCRIBE_START_COMPLETE_RESP;
	DWORD	dw2013_LINE_1_SCRIBE_STOP_RESP;
	DWORD	dw2014_LINE_2_SCRIBE_START_ABLE;
	DWORD	dw2015_LINE_2_SCRIBE_START_REQUEST;
	DWORD	dw2016_LINE_2_SCRIBE_START_COMPLETE_RESP;
	DWORD	dw2017_LINE_2_SCRIBE_STOP_RESP;
	DWORD	dw2018;
	DWORD	dw2019;
	DWORD	dw201A_LINE_1_SCRIBE_ALARM;
	DWORD	dw201B_LINE_2_SCRIBE_ALARM;
	DWORD	dw201C_LINE_1_DEPTH_OF_CUT_ALARM;
	DWORD	dw201D_LINE_2_DEPTH_OF_CUT_ALARM;
	DWORD	dw201E;
	DWORD	dw201F;

	DWORD	dw2020_LINE_1_ZERO_START_ABLE;
	DWORD	dw2021_LINE_1_ZERO_START_REQU;	
	DWORD	dw2022_LINE_1_ZERO_COMPLETE_RESP;	
	DWORD	dw2023;	
	DWORD	dw2024_LINE_2_ZERO_START_ABLE;	
	DWORD	dw2025_LINE_2_ZERO_START_REQU;	
	DWORD	dw2026_LINE_2_ZERO_COMPLETE_RESP;	
	DWORD	dw2027;
	DWORD	dw2028;
	DWORD	dw2029;
	DWORD	dw202A;
	DWORD	dw202B;
	DWORD	dw202C;
	DWORD	dw202D;
	DWORD	dw202E;
	DWORD	dw202F;

	DWORD	dw2030_LINE_1_CHUCK1_ZERO_DETECT_START_ABLE;
	DWORD	dw2031_LINE_1_CHUCK1_ZERO_DETECT_PROCESS;
	DWORD	dw2032_LINE_1_CHUCK1_ZERO_DETECT_COMPLETE;
	DWORD	dw2033_LINE_1_CHUCK2_ZERO_DETECT_START_ABLE;
	DWORD	dw2034_LINE_1_CHUCK2_ZERO_DETECT_PROCESS;
	DWORD	dw2035_LINE_1_CHUCK2_ZERO_DETECT_COMPLETE;
	DWORD	dw2036_LINE_1_CHUCK3_ZERO_DETECT_START_ABLE;
	DWORD	dw2037_LINE_1_CHUCK3_ZERO_DETECT_PROCESS;
	DWORD	dw2038_LINE_1_CHUCK3_ZERO_DETECT_COMPLETE;
	DWORD	dw2039;
	DWORD	dw203A;
	DWORD	dw203B;
	DWORD	dw203C;
	DWORD	dw203D;
	DWORD	dw203E;
	DWORD	dw203F;

	DWORD	dw2040_LINE_2_CHUCK1_ZERO_DETECT_START_ABLE;
	DWORD	dw2041_LINE_2_CHUCK1_ZERO_DETECT_PROCESS;
	DWORD	dw2042_LINE_2_CHUCK1_ZERO_DETECT_COMPLETE;
	DWORD	dw2043_LINE_2_CHUCK2_ZERO_DETECT_START_ABLE;
	DWORD	dw2044_LINE_2_CHUCK2_ZERO_DETECT_PROCESS;
	DWORD	dw2045_LINE_2_CHUCK2_ZERO_DETECT_COMPLETE;
	DWORD	dw2046_LINE_2_CHUCK3_ZERO_DETECT_START_ABLE;
	DWORD	dw2047_LINE_2_CHUCK3_ZERO_DETECT_PROCESS;
	DWORD	dw2048_LINE_2_CHUCK3_ZERO_DETECT_COMPLETE;
	DWORD	dw2049;
	DWORD	dw204A;
	DWORD	dw204B;
	DWORD	dw204C;
	DWORD	dw204D;
	DWORD	dw204E;
	DWORD	dw204F;

	DWORD	dw2050_LINE_1_BREAK_TABLE_START_ABLE;	
	DWORD	dw2051_LINE_1_BREAK_TABLE_START_REQU;	
	DWORD	dw2052_LINE_1_BREAK_TABLE_COMPLETE_RESP;	
	DWORD	dw2053_LINE_2_BREAK_TABLE_START_ABLE;
	DWORD	dw2054_LINE_2_BREAK_TABLE_START_REQU;	
	DWORD	dw2055_LINE_2_BREAK_TABLE_COMPLETE_RESP;	
	DWORD	dw2056_MVS3_START_ABLE;
	DWORD	dw2057_MVS3_PROCESS;
	DWORD	dw2058_MVS3_COMPLETE;
	DWORD	dw2059_MVS4_START_ABLE;
	DWORD	dw205A_MVS4_PROCESS;
	DWORD	dw205B_MVS4_COMPLETE;
	DWORD	dw205C_MVS5_START_ABLE;
	DWORD	dw205D_MVS5_PROCESS;
	DWORD	dw205E_MVS5_COMPLETE;
	DWORD	dw205F;

	DWORD	dw2060_LINE_1_BREAK_TABLE_STOP_RESP;
	DWORD	dw2061_LINE_2_BREAK_TABLE_STOP_RESP;
	DWORD	dw2062;
	DWORD	dw2063;
	DWORD	dw2064;
	DWORD	dw2065;
	DWORD	dw2066;
	DWORD	dw2067;
	DWORD	dw2068;
	DWORD	dw2069;
	DWORD	dw206A_LINE1_TILT_ALARM;
	DWORD	dw206B_LINE2_TILT_ALARM;
	DWORD	dw206C;
	DWORD	dw206D;
	DWORD	dw206E;
	DWORD	dw206F;

	DWORD	dw207[16];
	DWORD	dw208[16];
	DWORD	dw209[16];

	DWORD	dw210[16];
	DWORD	dw211[16];
	DWORD	dw212[16];
}T_REGISTER_MP_IN;

#define ID_VALUE_AXIS_BIT_REG 150
typedef struct {
	DWORD	dwAxis[ID_VALUE_AXIS_BIT_REG];
}T_AXIS_BIT_REG;

typedef struct{
	T_REGISTER_WORD_1	Reg_Word_1;
	T_REGISTER_WORD_2	Reg_Word_2;
	T_REGISTER_WORD_3	Reg_Word_3;		// 읽기 전용 메모리

	T_REGISTER_WORD_1	Reg_Word_1_NEW;
	T_REGISTER_WORD_2	Reg_Word_2_NEW;

	T_REGISTER_MP_IN	In;
	T_REGISTER_PC_OUT	Out;
	T_REGISTER_PC_OUT	Out_New;

	T_AXIS_BIT_REG	Axis_On;
	T_AXIS_BIT_REG	Axis_Off;
	T_AXIS_BIT_REG	Axis_ORG_PC;
	T_AXIS_BIT_REG	Axis_ORG_MP;
	T_AXIS_BIT_REG	Axis_ABS_RESET_PC;

	T_AXIS_BIT_REG	Axis_On_NEW;
	T_AXIS_BIT_REG	Axis_Off_NEW;
	T_AXIS_BIT_REG	Axis_ORG_PC_NEW;
	T_AXIS_BIT_REG	Axis_ABS_RESET_PC_NEW;
}T_REGISTER;
#pragma pack(pop)

//<< 2024.05.30 kms
#pragma pack(push, 1)
typedef struct T_TORQUE {
	BOOL bGantry;
	double dTorque[2];
} T_TORQUE;

typedef struct T_MOTOR_TORQUE {
	T_TORQUE torque[eMOTOR_END];
} T_MOTOR_TORQUE;
//>>
#pragma pack(pop)

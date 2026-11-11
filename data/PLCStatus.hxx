// UMAC을 사용하는 경우		: #define __UMAC__			1
// DPRam을 사용하는 경우	: #define __UMAC_DPR__		1
// 1. IO 관련 설정
//      - M-Veriable(최대 8192까지 사용 가능)
//      - P-Variable(최대 8192까지 사용 가능)
//      - DPRAM 
//        DPRAM을 사용하는 경우 Memory size만큼의 Struct를 설정후, Address Mapping을 한다.
//      - String Table
//		- Alarm은 현재의 상태이고
//        AlarmLast는 이전 상태임.
//        기존 알람의 경우에는 PLC의알람만 처리하나 신규에는 알람의 우선순위로 처리하도록 함.
//        상위알람/하위알람의 경우 상위 알람부터 표시하도록 함.
//      - UMAC에서는 Hartbeat와 Emergency-Stop만을 메인으로 처리하고,
//        PC와 연결이 끊어진 경우 Lamp/Buzzer/Reset도 처리하도록 함.
// 2. StatusT 쓰레드
//      - DPRAM을 읽어들이며 PLC와 기본적인 HandShaking을 함.
// 3. Motor Status
//      - Posotion / Speed Rate는 MotorStatus에 정의한다.
// 4. Memory Map 파일은 PLCLinker.cpp에서 읽어들임.
// 5. PC Control Mode 추가
//		- Toggle	: Hartbeat Check용
//                    Auto Mode 진행 중 PC와 연결이 끊어진 경우 Alarm 처리함.
//      - Auto Mode	: 
// 6. Motor Status에서 I-Variable를 Motor Parameter를 통해 UMAC로부터 읽어들임.
// 7. T_COMMAND_TIMEOUT
//      - Command Request 명령 처리 대기 시간임
#pragma once
#include "Mocha.type.hxx"
#include "IOInfo.hxx"
#include "MotorInfos.hxx"

#pragma pack(push, 1)

#define SENSOR_ON				1
#define SENSOR_OFF				0

#define SIGNAL_OFF				0
#define	SIGNAL_ON				1
#define	SIGNAL_ACK				2
#define SIGNAL_NAK				3

#define __UMAC__				1				// UMAC을 사용하는 경우

typedef enum {
// 프로세스
	eA_CONNECTION_UMAC									,	
	eA_CONNECTION_MELSEC_NET							,	
	eA_CONNECTION_LOCAL_PLC								,	
	eA_CONNECTION_LASER									,	
	eA_CONNECTION_MVSOL									,	
	eA_CONNECTION_LIGHT									,	
	eA_CONNECTION_ATTENUATOR							,	
	eA_CONNECTION_LDS									,	
	eA_CONNECTION_POWER_METER_1							,	
	eA_CONNECTION_POWER_METER_2							,	
// 쓰레드/연결상태
	eA_PROCESS_UMAC_STATUS_T							,
	eA_PROCESS_UMAC_MAIN_T								,	  
	eA_PROCESS_UMAC_MOTOR_T								,
	eA_PROCESS_UMAC_ALARM_T								,
	eA_LASER_FAULT										,
	eA_TRIGGER_T										,	// 트리거 보드 쓰레드	(MVSol)
	eA_TRIGGER_CONNECTED								,	// 트리거 보드 연결	(MVSol)
	eA_LIGHT_CONTROL_PORT_OPEN							,
	eA_LDS_PORT_OPEN									,
	eA_ATTENUATOR_T										,	// 쓰레드 동작
	eA_ATTENUATOR_PORT_OPEN								,	// Port  Open 
	eA_POWER_METER_1_T									,
	eA_POWER_METER_2_T									,
	eA_POWER_METER_1_PORT_OPEN							,
	eA_POWER_METER_2_PORT_OPEN							,
// IO	  
	eA_MAIN_CP_TRIP										,			
	eA_ETC_CP_TRIP										,		
	eA_MAIN_MC_TRIP										,		
	eA_MOTOR_1_MC_TRIP									,		
	eA_MOTOR_2_MC_TRIP									,		
	eA_MOTOR_3_MC_TRIP									,		
	eA_CONTROLLER_MC_TRIP								,			
	eA_PC_RACK_CP_TRIP									,		
	eA_MAIN_CDA_CHECK_SENSOR_1							,		
	eA_MAIN_CDA_CHECK_SENSOR_2							,		
	eA_WORKTABLE_AIR_BEARING_CHECK_SENSOR				,			
	eA_M4011											,			
	eA_WORKTABLE_GROOVE_ON_SENSOR						,			
	eA_WORKTABLE_GLASS_VACUUM_1							,		
	eA_WORKTABLE_GLASS_VACUUM_2							,		
	eA_M4015											,
	eA_WORKTABLE_GLASS_DETECT							,
	eA_LASER_WATER_LEAK_CHECK_SENSOR					,
	eA_CLEANING_UINT_CYLINDER_UP_CHECK_SENSOR			,
	eA_CLEANING_UINT_CYLINDER_DOWN_CHECK_SENSOR			,
	eA_POWER_METER_SOL_1_FORWARD						,
	eA_POWER_METER_SOL_1_BACKWARD						,
	eA_POWER_METER_SOL_2_FORWARD						,
	eA_POWER_METER_SOL_2_BACKWARD						,
	eA_CLEAN_ROLLER_CHECK_SENSOR						,
	eA_M4025											,
	eA_M4026											,
	eA_IONIZER_1_ERROR									,
	eA_IONIZER_2_ERROR									,
	eA_M4029											,
	eA_HEAD_1_MECHANICAL_SHUTTER_CLOSE_CHECK_SENSOR		,
	eA_HEAD_2_MECHANICAL_SHUTTER_CLOSE_CHECK_SENSOR		,
	eA_LASER_IS_ON										,
	eA_EXT_ACTIVATION_ACTIVE							,
	eA_SIK_OPEN											,
	eA_POWER_LIMIT										,
	eA_LASER_ASSIGNED									,
	eA_PROGRAM_COMPLETED								,
	eA_FAULT_LASER										,
	eA_LASER_READY										,
	eA_PROG_ACTIVE										,
	eA_MONITORING										,
	eA_PROG_CANCELLED									,
	eA_M4043											,
	eA_M4044											,
	eA_M4045											,
	eA_M4046											,
	eA_M4047											,
	eA_M4048											,
	eA_M4049											,
	eA_M4050											,
	eA_M4051											,
	eA_M4052											,
	eA_M4053											,
	eA_M4054											,
	eA_M4055											,
	eA_M4056											,
	eA_M4057											,
	eA_M4058											,
	eA_M4059											,
	eA_M4060											,
	eA_M4061											,
	eA_M4062											,
	eA_M4063											,

	eA_MOTOR_1_ENABLE			,
	eA_MOTOR_2_ENABLE			,
	eA_MOTOR_3_ENABLE			,
	eA_MOTOR_4_ENABLE			,
	eA_MOTOR_5_ENABLE			,
	eA_MOTOR_6_ENABLE			,
	
	eA_MOTOR_1_HOME_COMPLETE	,
	eA_MOTOR_2_HOME_COMPLETE	,
	eA_MOTOR_3_HOME_COMPLETE	,
	eA_MOTOR_4_HOME_COMPLETE	,
	eA_MOTOR_5_HOME_COMPLETE	,
	eA_MOTOR_6_HOME_COMPLETE	,

	eA_MOTOR_1_AMP_FAULT		,
	eA_MOTOR_2_AMP_FAULT		,
	eA_MOTOR_3_AMP_FAULT		,
	eA_MOTOR_4_AMP_FAULT		,
	eA_MOTOR_5_AMP_FAULT		,
	eA_MOTOR_6_AMP_FAULT		,
	
	eA_MOTOR_1_FOLLOWING		,
	eA_MOTOR_2_FOLLOWING		,
	eA_MOTOR_3_FOLLOWING		,
	eA_MOTOR_4_FOLLOWING		,
	eA_MOTOR_5_FOLLOWING		,
	eA_MOTOR_6_FOLLOWING		,

	eA_MOTOR_1_POSITIVE_LIMIT	,
	eA_MOTOR_2_POSITIVE_LIMIT	,
	eA_MOTOR_3_POSITIVE_LIMIT	,
	eA_MOTOR_4_POSITIVE_LIMIT	,
	eA_MOTOR_5_POSITIVE_LIMIT	,
	eA_MOTOR_6_POSITIVE_LIMIT	,

	eA_MOTOR_1_NEGATIVE_LIMIT	,
	eA_MOTOR_2_NEGATIVE_LIMIT	,
	eA_MOTOR_3_NEGATIVE_LIMIT	,
	eA_MOTOR_4_NEGATIVE_LIMIT	,
	eA_MOTOR_5_NEGATIVE_LIMIT	,
	eA_MOTOR_6_NEGATIVE_LIMIT	,

	eA_MOTOR_1_ACTUAL_LOAD		,
	eA_MOTOR_2_ACTUAL_LOAD		,
	eA_MOTOR_3_ACTUAL_LOAD		,
	eA_MOTOR_4_ACTUAL_LOAD		,
	eA_MOTOR_5_ACTUAL_LOAD		,
	eA_MOTOR_6_ACTUAL_LOAD		,

	eA_MOTOR_1_ENCODER			,
	eA_MOTOR_2_ENCODER			,
	eA_MOTOR_3_ENCODER			,
	eA_MOTOR_4_ENCODER			,
	eA_MOTOR_5_ENCODER			,
	eA_MOTOR_6_ENCODER			,
	
	eA_FAIL_TO_INITIALIZE		,
	eA_MAIN_MC_POWER_ON			,

	eA_HOME_START_Z1_Z2_CMD		,
	eA_HOME_START_X1_X2_CMD		,
	eA_HOME_START_Y1_CMD		,
	eA_HOME_START_T1_CMD		,

	eA_FAIL_TO_ALIGNMENT		,
	eA_FAIL_TO_FIND_MARK		,

	eA_POWER_METER_SOL_FORWARD	,
	eA_POWER_METER_SOL_BACKWARD	,
	eA_TABLE_GLASS_VACUUM_ON	,
	eA_TABLE_GLASS_VACUUM_OFF	,
	eA_TABLE_GLASS_BLOW_ON		,
	eA_TABLE_GLASS_BLOW_OFF		,

	eA_MOVE_FAIL_X1				,
	eA_MOVE_FAIL_X2				,
	eA_MOVE_FAIL_Y1				,
	eA_MOVE_FAIL_T1				,
	eA_MOVE_FAIL_Z1				,
	eA_MOVE_FAIL_Z2				,

	eA_TABLE_GROOVE_VACUUM_ON	,
	eA_TABLE_GROOVE_VACUUM_OFF	,

	eA_MOTION_CREATE			,
	eA_MOTION_DOWNLOADED		,

	eA_END						,
} eALARM;

typedef struct {
	int		eAlarmNo;
	int		eHeavy;				// 1 : Light, 2 : Heavy
	char	szName				[3][256];
	char	szHelp				[3][256];
	wchar_t	szOccurredArea		[3][256];
	BOOL	bReport				;
	BOOL	bOccurred			;
	BOOL	bReset				;
} T_ALARM;

#pragma pack(pop)
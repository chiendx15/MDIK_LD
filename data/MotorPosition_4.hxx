
#pragma once
#include "Mocha.type.hxx"
#include "MotorInfos.hxx"
#include "Online.hxx"
#pragma pack(push, 4)

typedef enum eCELL_TRANSFER {
	eCELL_TRANSFER_NONE = -1,
	eCELL_TRANSFER_1,
	eCELL_TRANSFER_2,
	nCELL_TRANSFER,
} eCELL_TRANSFER;

typedef enum ePEELING_LINE {
	ePEELING_LINE_NONE = -1,
	ePEELING_LINE_1,
	ePEELING_LINE_2,
	nPEELING_LINE,
} ePEELING_LINE;

typedef enum eTABLE {
	eTABLE_NONE = -1,
	eTABLE_1,
	eTABLE_2,
	nTABLE,
} eTABLE;

typedef enum eLINE_SCAN_PICKER {
	eLINE_SCAN_PICKER_NONE = -1,
	eLINE_SCAN_PICKER_1,
	eLINE_SCAN_PICKER_2,
	nLINE_SCAN_PICKER,
} eLINE_SCAN_PICKER;

typedef enum eBUFFER_TABLE {
	eBUFFER_TABLE_NONE = -1,
	eBUFFER_TABLE_1,
	eBUFFER_TABLE_2,
	nBUFFER_TABLE,
} eBUFFER_TABLE;

typedef enum eBUFFER_PICKER {
	eBUFFER_PICKER_NONE = -1,
	eBUFFER_PICKER_1,
	eBUFFER_PICKER_2,
	nBUFFER_PICKER,
} eBUFFER_PICKER;

typedef enum eUNLOAD_PICKER {
	eUNLOAD_PICKER_NONE = -1,
	eUNLOAD_PICKER_1,
	eUNLOAD_PICKER_2,
	eUNLOAD_PICKER_3,
	nUNLOAD_PICKER,
} eUNLOAD_PICKER;

typedef enum eTRAY_PICKER {
	eTRAY_PICKER_NONE = -1,
	eTRAY_PICKER_1,
	eTRAY_PICKER_2,
	nTRAY_PICKER,
} eTRAY_PICKER;

typedef enum eTRAY_PORT {
	eTRAY_PORT_NONE = -1,
	eTRAY_PORT_1,
	eTRAY_PORT_2,
	eTRAY_PORT_3,
	eTRAY_PORT_4,
	eTRAY_PORT_5,
	nTRAY_PORT,
} eTRAY_PORT;

// Tray 1 -> 1, 2, 3, 4, 5 순서
// Tray 2 -> 5, 4, 3, 2, 1 순서
typedef enum eMODULE_POS_INDEX {
	eMODULE_POS_PORT_1,
	eMODULE_POS_PORT_2,
	eMODULE_POS_PORT_3,
	eMODULE_POS_PORT_4,
	eMODULE_POS_PORT_5,
	eMODULE_POS_TABLE_BUFFER,
} eMODULE_POS_INDEX;

typedef struct T_CELL_TRANSFER_POS {
	T_POS		Brush_Z;			// 브러쉬 닿는 높이

	T_POS		Load_Y;
	T_POS		Load_T;
	T_POS		Load_Z;

	T_POS		AlignCamera_X1;			// 카메라 X1
	T_POS		AlignCamera_X2;			// 카메라 X2
	T_POS		AlignTransfer_Y;		// 트랜스퍼 Y
	T_POS		AlignTransfer_T;		// 트랜스퍼 T
	T_POS		AlignTransfer_Z;		// 트랜스퍼 Z

	T_POS		HyperCleaner_Start_Y;	// T축은 이 위치에서만 움직인다
	T_POS		HyperCleaner_End_Y;		// T축은 이 위치에서만 움직인다

	T_POS		Unload_Y;				// 1번 테이블에 티칭한다
	T_POS		Unload_T;				// 1번 테이블에 티칭한다
	T_POS		Unload_Z[nTABLE];

	T_POS		x1;						// 첫번째 포지션
	T_POS		y1;						// 첫번째 포지션
	T_POS		x2;						// 두번째 포지션
	T_POS		y2;						// 두번째 포지션
	T_POS		x3;						// 세번째 포지션
	T_POS		y3;						// 세번째 포지션
	T_POS		CenterX;				// 결과 센터포지션
	T_POS		CenterY;				// 결과 센터포지션
} T_CELL_TRANSFER_POS;

typedef struct T_PEELING_PROCESS_POS {
	T_POS		Pin_Y;
	T_POS		Pin_Up_Z;					// Z축 업위치
	T_POS		Pin_Down_Z;					// Recipe 에 있던거 이동해옴 해드핀만 잘움직이면 되서 고속하강은 고정 티칭으로 
	T_POS		Pin_Head_UP_Z;				// Z축 헤드 업위치

	T_POS		Gripper_Y;
	T_POS		Gripper_Up_Z;				// Z축 업위치
	T_POS		Gripper_Down_Z;
	T_POS		Gripper_Detach_X;			// 더미 뜯어내는 위치
	T_POS		Gripper_Detach_Z;			// 더미 뜯어내는 위치
	T_POS		Gripper_Dummy_X;			// 더미 버리는 위치 (대기 위치로도 사용함)
	T_POS		Gripper_Dummy_Y;
	T_POS		Gripper_Dummy_Z;

	T_POS		GripperOpen;
	T_POS		GripperClose;
	// Load Cell 위치 
	T_POS		LoadCell_Wait_Z;
	T_POS		LoadCell_Wait_HeadZ;
	T_POS		LoadCell_Y;
	T_POS		LoadCell_HeadX;
	T_POS		LoadCell_Z;
	T_POS		LoadCell_HeadZ;

	//<< 기준 얼라인 위치
	T_POS		Align_X1;
	T_POS		Align_Y1[nTABLE];			// 테이블 2번은 1번이랑 똑같은 값으로 쓴다
	T_POS		Align_Z1;

	T_POS		Align_X2;
	T_POS		Align_Y2[nTABLE];			// 테이블 2번은 1번이랑 똑같은 값으로 쓴다
	T_POS		Align_Z2;
	//>>

	//<< 핀과 그리퍼의 안전거리 x옵셋을 구한다
	T_POS		Offset_Peeling_X;			// 임의의 특정 포인트
	T_POS		Offset_Gripper_X;			// Offset_Pin_X과 최대한 붙여서 티칭
	//>>

	//<< Vision to Peeling ( 필링pin X축과, 1번 테이블Y축으로 움직인다 )
	T_POS		Offset_Pin_X;				// 임의의 특정 포인트
	T_POS		Offset_Pin_Y;				// 임의의 특정 포인트
	T_POS		Offset_Camera_X;			// 임의의 특정 포인트
	T_POS		Offset_Camera_Y;			// 임의의 특정 포인트
	//>>
} T_PEELING_PROCESS_POS;

typedef struct T_PEELING_TABLE_POS {
	T_POS		Load_Y;
	T_POS		Safe_Y;
	T_POS		Unload_Y;
	T_POS		UP_Z;
	T_POS		Down_Z;
} T_PEELING_TABLE_POS;

typedef struct T_CELL_SHIFT_PICKER_POS {
	T_POS		UP_Z;

	struct T_LOAD {
		T_POS		X;
		T_POS		Z[nTABLE];
		T_POS		T;
	} Load[nPEELING_LINE];

	T_POS		Unload_X;
	T_POS		Unload_Z[nTABLE];
	T_POS		Unload_T;
} T_CELL_SHIFT_PICKER_POS;

typedef struct T_LINE_SCAN_TABLE_POS {
	T_POS		Load_Y;
	T_POS		Safe_Y;

	T_POS		Process_Start_Y;
	T_POS		Process_Z;
	T_POS		Process_End_Y;

	T_POS		Unload_Y;

	T_POS		UP_Z;
	T_POS		Down_Z;
} T_LINE_SCAN_TABLE_POS;

typedef struct T_CELL_LINE_SCAN_PICKER_POS {
	T_POS		UP_Z;					// 두 피커 다 같은 위치

	T_POS		Load_Y;
	T_POS		Load_T;
	T_POS		Load_Z[nTABLE];

	T_POS		Safety_Z;			// 아래에서 Y축 이동할때 세팅될 Z축 (두 피커 다 같은 위치)

	T_POS		Unload_Y;
	T_POS		Unload_T;
	T_POS		Unload_Z[nTABLE];
} T_CELL_LINE_SCAN_PICKER_POS;

typedef struct T_CELL_BUFFER_PICKER_POS {
	T_POS		UP_Z;					// 두 피커 다 같은 위치

	T_POS		Load_Y;
	T_POS		Load_T;
	T_POS		Load_Z[nTABLE];


	T_POS		AlignCamera_X1;			// 카메라 X1
	T_POS		AlignCamera_X2;			// 카메라 X2
	T_POS		AlignTransfer_Y;		// 트랜스퍼 Y
	T_POS		AlignTransfer_T;		// 트랜스퍼 T
	T_POS		AlignTransfer_Z;		// 트랜스퍼 Z

	T_POS		Brush_Wait_Y;
	T_POS		Brush_Z;

	T_POS		Unload_Y;
	T_POS		Unload_T;
	T_POS		Unload_Z[nBUFFER_TABLE]; // 2024.04.19 kms - nBUFFER_TABLE이 맞다

	//<< Align to MCR Offset
	T_POS		Offset_Align_X;			// 임의의 특정 포인트
	T_POS		Offset_Align_Y;			// 임의의 특정 포인트
	T_POS		Offset_MCR_X;			// 임의의 특정 포인트
	T_POS		Offset_MCR_Y;			// 임의의 특정 포인트
	//>>

	T_POS		x1;						// 첫번째 포지션
	T_POS		y1;						// 첫번째 포지션
	T_POS		x2;						// 두번째 포지션
	T_POS		y2;						// 두번째 포지션
	T_POS		x3;						// 세번째 포지션
	T_POS		y3;						// 세번째 포지션
	T_POS		CenterX;				// 결과 센터포지션
	T_POS		CenterY;				// 결과 센터포지션
} T_CELL_BUFFER_PICKER_POS;

typedef struct T_BUFFER_TABLE_POS {
	T_POS		Load_X;
	T_POS		Load_Y;

	struct T_UNLOAD {
		T_POS		X;
		T_POS		Y;
	} Unload[nUNLOAD_PICKER];
} T_BUFFER_TABLE_POS;

typedef struct T_CELL_UNLOAD_PICKER_POS {
	T_POS		UP_Z;

	T_POS		Load_X;
	T_POS		Load_Y;
	T_POS		Load_Z;

	T_POS		Wait_Y;

	T_POS		Unload_X;
	T_POS		Unload_Y;
	T_POS		Unload_Z;
} T_CELL_UNLOAD_PICKER_POS;

typedef struct T_TRAY_LIFT_POS {
	T_POS		TrayPicker1_Up_Z;
	T_POS		TrayPicker1_X[nTRAY_PORT];		// 5번 사용 안함
	T_POS		TrayPicker1_Down_Z[nTRAY_PORT];	// 5번 사용 안함

	T_POS		TrayPicker2_Up_Z;
	T_POS		TrayPicker2_X[nTRAY_PORT];		// 1번 사용 안함
	T_POS		TrayPicker2_Down_Z[nTRAY_PORT];	// 1번 사용 안함

	T_POS		Lift_Up_Z[nTRAY_PORT];		// 전부 사용
	T_POS		Lift_Down_Z[nTRAY_PORT];	// 전부 사용
} T_TRAY_LIFT_POS;

typedef struct T_MOTOR_POS_4 {
	T_CELL_TRANSFER_POS				CellTransfer[nCELL_TRANSFER];
	T_PEELING_PROCESS_POS			PeelingProcess[nPEELING_LINE];
	T_PEELING_TABLE_POS				PeelingTable[nPEELING_LINE][nTABLE];
	T_CELL_SHIFT_PICKER_POS			CellShiftPicker;
	T_LINE_SCAN_TABLE_POS			LineScanTableMain[nTABLE];
	T_LINE_SCAN_TABLE_POS			LineScanTableSub[nTABLE];
	T_CELL_LINE_SCAN_PICKER_POS		CellLinescanPicker[nLINE_SCAN_PICKER];
	T_CELL_BUFFER_PICKER_POS		CellBufferPicker[nBUFFER_PICKER];
	T_BUFFER_TABLE_POS				BufferTable[nBUFFER_TABLE];
	T_CELL_UNLOAD_PICKER_POS		CellUnloadPicker[nUNLOAD_PICKER];
	T_TRAY_LIFT_POS					TrayLift;
}T_MOTOR_POS_4;
#pragma pack(pop)


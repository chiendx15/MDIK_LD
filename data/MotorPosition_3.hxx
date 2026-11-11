#pragma once
#include "Mocha.type.hxx"
#include "MotorInfos.hxx"
#pragma pack(push, 4)

// 스틱 라인스캔 테이블
typedef struct T_STICK_LINESCAN_TABLE {
	T_POS		Load_Y;
	T_POS		Unload_Y;
} T_STICK_LINESCAN_TABLE;

// 스틱 피커
typedef struct T_STICK_PICKER {
	T_POS		Wait_NotMaterial_Z;
	T_POS		Wait_IsMaterial_Z;

	// 브러쉬 동작 12.06 추가 - UI 수정 필요
	T_POS		Brush_Z;
	T_POS		Brush_Before_X;
	T_POS		Brush_Before_Y;
	T_POS		Brush_Before_T;
	T_POS		Brush_After_X;
	T_POS		Brush_After_Y;
	T_POS		Brush_After_T;

	// 받는 위치
	T_POS		Load_X;
	T_POS		Load_Y;
	T_POS		Load_T;
	T_POS		Load_Before_Z;
	T_POS		Load_Z;
	T_POS		Load_After_Z;

	// 얼라인 카메라 기준위치 맞출 때 피커의 위치
	T_POS		Default_Align_X;
	T_POS		Default_Align_Y;
	T_POS		Default_Align_T;
	T_POS		Default_Align_Focus_Z;
	T_POS		Default_Cam1X;	// Cam1 기준 위치 (티칭 자재 마크)
	T_POS		Default_Cam2X;	// Cam2 기준 위치 (티칭 자재 마크)


	// MCR 카메라 기준위치 맞출 때 피커의 위치 (T는 얼라인에서 사용하는 T값 사용)
	T_POS		Offset_Align_X;
	T_POS		Offset_Align_Y;
	T_POS		Offset_MCR_X;
	T_POS		Offset_MCR_Y;
	T_POS		Default_MCR_Focus_Z;
	
	// Unload 1번 위치
	T_POS		Unload1_X;
	T_POS		Unload1_Y;
	T_POS		Unload1_T;
	T_POS		Unload1_Before_Z;
	T_POS		Unload1_Z;
	T_POS		Unload1_After_Z;

	// Unload 2번 위치
	T_POS		Unload2_X;
	T_POS		Unload2_Y;
	T_POS		Unload2_T;
	T_POS		Unload2_Before_Z;
	T_POS		Unload2_Z;
	T_POS		Unload2_After_Z;

	// 회전 중심 구하기
	T_POS		Rotate1_Cam1X;
	T_POS		Rotate1_Picker_Y;
	T_POS		Rotate2_Cam1X;
	T_POS		Rotate2_Picker_Y;
	T_POS		Rotate3_Cam1X;
	T_POS		Rotate3_Picker_Y;
	T_POS		RotateCenter_Cam1X;
	T_POS		RotateCenter_Picker_Y;
} T_STICK_PICKER;

// 척
typedef struct T_STICK_CHUCK
{
	T_POS		Wait_Y;
	T_POS		Wait_Z1;
	T_POS		Wait_Z2;
	T_POS		Wait_Z3;

	// 척이 스크라이브 테이블에 닿는 위치 // 21.03.03
	T_POS		ScribeTable_Y;
	// 척이 메인(플로팅) 테이블을 지난 위치
	T_POS		OutMainTable_Y;
	// 척이 헤드와 간섭이 생기는 위치
	T_POS		HeadCrush_Y;
	// 기본 자재 시작 위치
	T_POS		Default_Start_Y;
	// 척이 카메라 원점에서 보이는 위치
	T_POS		Camera_Center_Y;

	// 기본 자재 집는 값
	T_POS		Default_X1;
	T_POS		Default_X2;

	// 자재 잡기 전 빠르게 Z축이 내려가도 되는 구간 티칭
	T_POS		Before_Z1;
	T_POS		Before_Z2;
	T_POS		Before_Z3;

	// 커팅 테이블에 닿는 Z축 위치 (자재 두께만큼 올라가서 사용)
	T_POS		CutTable_Z1;
	T_POS		CutTable_Z2;
	T_POS		CutTable_Z3;

	//// 자재 놓고 올라갈 때 천천히 올라갈 구간 티칭
	//T_POS		After_Z1;
	//T_POS		After_Z2;
	//T_POS		After_Z3;

} T_STICK_CHUCK;

// 커팅 헤드 관련 축
typedef struct T_CELL_CUT_TABLE 
{
	T_POS		Wait_Head1X;
	T_POS		Wait_Head2X;
	T_POS		Wait_CutY;
	T_POS		Wait_CutZ;

	T_POS		Focus_Cam1Z;	// 기준자재 포커스 위치
	T_POS		Focus_Cam2Z;	// 기준자재 포커스 위치
	T_POS		CutTable_CutZ;			// 기준자재에 커터가 닿는 위치

	// Load Cell (커터 토크측정하는 위치)
	T_POS		LoadCell_Head1X;
	T_POS		LoadCell_CutY;
	T_POS		LoadCell_CutZ;

	// 휠 클리닝 위치
	T_POS		WheelCleanStart_HeadX;
	T_POS		WheelCleanEnd_HeadX;
	T_POS		WheelClean_CutZ;
	T_POS		WheelChange_Head1X;
	T_POS		WheelChange_Head2X;

	// 카메라1,2 커터 옵셋
	T_POS		DefaultCut_Head1X;		// 커터가 찍은 위치 (기준 좌표) 
	T_POS		DefaultCut_CutY;		// 커터가 찍은 위치 (기준 좌표)
	T_POS		CutToCam1_Head1X;		// 커터가 찍은 곳을 바라 볼때 카메라1번 기준 좌표
	T_POS		CutToCam1_Cam1Y;		// 커터가 찍은 곳을 바라 볼때 카메라1번 기준 좌표
	T_POS		CutToCam2_Head2X;		// 커터가 찍은 곳을 바라 볼때 카메라2번 기준 좌표
	T_POS		CutToCam2_Cam2Y;		// 커터가 찍은 곳을 바라 볼때 카메라2번 기준 좌표

	// 카메라 기준 자재 엣지 티칭 (무조건 Y는 0위치에서 엣지가 센터여야함)
	T_POS		Default_LeftEdge_Head1X;
	T_POS		Default_RightEdge_Head2X;

	// 카메라 1번 Y 원점 기준 커팅 테이블과 틸트 테이블 엣지가 중앙에 오는 값
	T_POS		Default_CutTableY;
	T_POS		Default_TiltTableY;
	
	// 커팅 테이블, 틸트 테이블과의 브레이크 옵셋
	// Default_CutTableY, Default_TiltTableY에서 티칭함
	T_POS		CutTableToBreaker_PusherY;
	T_POS		TiltTableToBreaker_PusherY;

	// 브레이크 대기, 다운 위치
	T_POS		Wait_PusherY;
	T_POS		Wait_PusherZ;
	//T_POS		Before_PusherZ;
	T_POS		Table_PusherZ;

	// 커팅 테이블 청소 위치	// KES 추가 2024.4.2
	T_POS		CutTable_BrushY;
} T_CELL_CUT_TABLE;


// 틸트 테이블
typedef struct T_CELL_BREAK_TABLE
{
	T_POS		Wait_Y;
	T_POS		Wait_T;
	T_POS		Wait_Z;

	T_POS		Brush_Before_Y;
	T_POS		Brush_After_Y;
	T_POS		Brush_T;
	T_POS		Brush_Z;

	T_POS		Load_T;
	T_POS		Load_Z;
	
	T_POS		HyperClean_Before_Y;
	T_POS		HyperClean_After_Y;
	T_POS		HyperClean_Z;
	T_POS		HyperClean_T;

	T_POS		Unload_Y;
	T_POS		Unload_T;
	T_POS		Unload_Z;

	// 2023.01.25 평탄도 위치 추가
	T_POS		Flatness_Y;
} T_CELL_BREAK_TABLE;

typedef struct T_MOTOR_POS_3
{
	T_STICK_LINESCAN_TABLE	Linescan;		// Tab 1 
	T_STICK_PICKER			StickPicker;	// Tab 2

	T_STICK_CHUCK			Chuck[2];		// Tab 3~4
	T_CELL_CUT_TABLE		CutTable[2];		// Tab 5~6
	T_CELL_BREAK_TABLE		BreakTable[2];	// Tab 7~8
} T_MOTOR_POS_3;
#pragma pack(pop)


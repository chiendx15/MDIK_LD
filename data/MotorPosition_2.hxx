

#pragma once
#include "Mocha.type.hxx"
#include "MotorInfos.hxx"
#pragma pack(push, 4)

// 글래스 시프트 테이블
typedef struct T_GLASS_SHIFT_TABLE 
{
	T_POS		Brush_Start_Hyper_End_Y;
	T_POS		Brush_End_Hyper_Start_Y;
	T_POS		Load_Wait_Y;
	T_POS		Load_Y;
	T_POS		Unload_Y;
} T_GLASS_SHIFT_TABLE;

// 글래스 피커
typedef struct T_GLASS_PICKER 
{
	T_POS		Wait_NotMaterial_Z;
	T_POS		Wait_IsMaterial_Z;
	T_POS		Wait_Y;

	// 브러쉬 동작 12.11 추가 - UI 수정 필요
	//T_POS		Brush_T; -> Load T 사용
	// X -> Load X 사용
	T_POS		Brush_Hyper_Z;
	T_POS		Brush_Start_Hyper_End_Y;
	T_POS		Brush_End_Hyper_Start_Y;

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
	T_POS		Default_MCR_X;
	T_POS		Default_MCR_Y;
	T_POS		Default_MCR_Focus_Z;
	T_POS		Default_MCRCamX;	// MCR 기준 위치 (티칭 자재 MCR) to do change function

	// Unload 위치
	T_POS		Unload_X;
	T_POS		Unload_Y;
	T_POS		Unload_T;
	T_POS		Unload_Before_Z;
	T_POS		Unload_Z;
	T_POS		Unload_After_Z;

	// 회전 중심 구하기
	T_POS		Rotate1_Cam1X;
	T_POS		Rotate1_Y;
	T_POS		Rotate2_Cam1X;
	T_POS		Rotate2_Y;
	T_POS		Rotate3_Cam1X;
	T_POS		Rotate3_Y;
	T_POS		RotateCenterX;
	T_POS		RotateCenterY;
} T_GLASS_PICKER;

// 척
typedef struct T_GLASS_CHUCK 
{
	T_POS		Wait_Y;
	T_POS		Wait_Z1;
	T_POS		Wait_Z2;
	T_POS		Wait_Z3;
	T_POS		Wait_Z4;
	T_POS		Wait_Z5;
	T_POS		Wait_Z6;

	// 척이 스크라이브 테이블에 닿는 위치 // 21.03.02
	T_POS		ScribeTable_Y;

	// 척이 메인테이블에 간섭이 없어지는 위치
	T_POS		OutMainTable_Y;
	// 척이 헤드와 간섭이 생기는 위치
	T_POS		HeadCrush_Y;
	// 기본 자재 시작 위치
	T_POS		Default_Start_Y;

	// 기본 자재 시작 위치에서 글래스 사이즈를 사용해서 센터 구할거라 삭제함
	//// 기본 자재 종료 위치 
	//T_POS		Default_End_Y;

	// 척이 카메라 원점에서 보이는 위치
	T_POS		Camera_Center_Y;

	// 자재 잡기 전 빠르게 Z축이 내려가도 되는 구간 티칭
	T_POS		Before_Z1;
	T_POS		Before_Z2;
	T_POS		Before_Z3;
	T_POS		Before_Z4;
	T_POS		Before_Z5;
	T_POS		Before_Z6;

	// 커팅 테이블에 닿는 Z축 위치 (자재 두께만큼 올라가서 사용)
	T_POS		CutTable_Z1;
	T_POS		CutTable_Z2;
	T_POS		CutTable_Z3;
	T_POS		CutTable_Z4;
	T_POS		CutTable_Z5;
	T_POS		CutTable_Z6;

	//// 자재 놓고 올라갈 때 천천히 올라갈 구간 티칭
	//T_POS		After_Z1;
	//T_POS		After_Z2;
	//T_POS		After_Z3;
	//T_POS		After_Z4;
	//T_POS		After_Z5;
	//T_POS		After_Z6;
} T_GLASS_CHUCK;

// 커팅 헤드 관련 축
typedef struct T_STICK_CUT_TABLE 
{
	T_POS		Wait_Head1X;
	T_POS		Wait_Head2X;
	T_POS		Wait_CutY;
	T_POS		Wait_CutZ;

	T_POS		Focus_Cam1Z;
	T_POS		Focus_Cam2Z;
	T_POS		CutTable_CutZ;

	// Load Cell (커터 토크측정하는 위치)
	T_POS		LoadCell_Head1X;
	T_POS		LoadCell_CutY;
	T_POS		LoadCell_CutZ;

	// 휠 클리닝 위치
	T_POS		WheelCleanStart_HeadX;
	T_POS		WheelCleanEnd_HeadX;
	T_POS		WheelClean_CutZ;
	T_POS		WheelChange_Head1X;		// 2021.07.05 추가 (휠 교체 Head X)
	T_POS		WheelChange_Head2X;		// 2021.07.05 추가 (휠 교체 Head X)

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
	T_POS		TiltTableToBreaker_PusherY;		// <- 이제 필요 없음 삭제할 것 (12.28)

	// 브레이크 대기, 다운 위치
	T_POS		Wait_PusherY;
	T_POS		Wait_PusherZ;
	//T_POS		Before_PusherZ;
	T_POS		Table_PusherZ;

	// 커팅 테이블 청소 위치	// KES 추가 2024.3.28
	T_POS		CutTable_BrushY;
} T_STICK_CUT_TABLE;

// 브레이크 테이블(틸트)
typedef struct T_STICK_BREAK_TABLE
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
	T_POS		HyperClean_T;
	T_POS		HyperClean_Z;

	T_POS		Unload_Y;
	T_POS		Unload_T;
	T_POS		Unload_Z;

	T_POS		Flatness_Y; // 2023.01.25 평탄도 Y 위치 추가
} T_STICK_BREAK_TABLE;

typedef struct T_STICK_TRANSFER 
{
	T_POS		Wait_X;
	T_POS		Wait_NotMaterial_Z;
	T_POS		Wait_IsMaterial_Z;

	T_POS		Load_X;
	T_POS		Load_Before_Z;
	T_POS		Load_Z;
	T_POS		Load_After_Z;

	T_POS		Brush_Wait_X;	// Stick Transfer와 연결된 브러시 Down후 Unload 위치로 이동해도 안전한 위치
	T_POS		Unload_X;
	T_POS		Unload_Before_Z;
	T_POS		Unload_Z;
	T_POS		Unload_After_Z;
} T_STICK_TRANSFER;

typedef struct T_MOTOR_POS_2 
{
	T_GLASS_SHIFT_TABLE		ShiftTable;
	T_GLASS_PICKER			Picker;
	T_GLASS_CHUCK			Chuck;
	T_STICK_CUT_TABLE		CutTable;
	T_STICK_BREAK_TABLE		BreakTable;
	T_STICK_TRANSFER		Transfer[2];
} T_MOTOR_POS_2;

#pragma pack(pop)


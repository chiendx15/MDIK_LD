
#pragma once
#include "Mocha.type.hxx"
#include "MotorInfos.hxx"
#pragma pack(push, 4)


typedef enum eCONVEYOR_LD {
	eCONVEYOR_LD_NONE = -1,
	eCONVEYOR_LD_1,
	eCONVEYOR_LD_2,
	eCONVEYOR_EL,
	eCONVEYOR_ULD_1,
	eCONVEYOR_ULD_2,
	nCONVEYOR_LD,
} eCONVEYOR_LD;

typedef struct T_ELEVATOR_CONVEYOR {
	T_POS		Load_Z;
	T_POS		Unload_Z;
} T_ELEVATOR_CONVEYOR;

typedef struct T_GLASS_TRANSFER {
	// 대기 위치 Load Wait, Glass Unload Wait, Paper Unload Wait 가 따로 있어야 할지...
	T_POS		Wait_Y;
	T_POS		Wait_Z;

	// LDCV2 (Load)
	T_POS		Load_Y;
	T_POS		Load_Z; // 빈 PPBox 바닥면 기준.

	// SCT Table (Glass Unload)
	T_POS		Unload_Glass_Y;
	T_POS		Unload_Glass_Z;

	// ELCV (Paper Unload)
	T_POS		Unload_Paper_Y;
	T_POS		Unload_Paper_Z;
}T_GLASS_TRANSFER;

typedef struct T_MOTOR_POS_1{
	T_ELEVATOR_CONVEYOR		EL_Convryor;
	T_GLASS_TRANSFER		Glass_TR;
}T_MOTOR_POS_1;

// LIH
#pragma pack(pop)


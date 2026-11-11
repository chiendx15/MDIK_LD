#pragma once

#include "Mocha.type.hxx"

#pragma pack(push, 1)

enum {
	eDUST_COLLECTOR_1,
	nDUST_COLLECTOR,
};

typedef struct T_DUST_COLLECTOR_DATA {
	// 압력계
	int iPressure;

	// 기판
	int iPressureDifference;	// 차압 mmAq
	int iInverterOutput;		// 인버터출력(%)
	int iAlarmHistory[10];		// 알람이력 10개
	int iRunningStatus;			// 운전상태
	int iAlarmCode;				// 알람발생코드(1: 모터과전류, 2: 필터과차압, 3: 필터교체, 4: 전류불평형, 5: 모터과부하)
	int iRunningTime;			// 운전시간
} T_DUST_COLLECTOR_DATA;

typedef struct T_DUST_COLLECTOR {
	T_DUST_COLLECTOR_DATA	data[nDUST_COLLECTOR];
} T_DUST_COLLECTOR;

typedef struct T_STEP_CELL {
	int iSTEP_ID;
	char szCELL_ID[41];
} T_STEP_CELL;

typedef struct T_STEP_CELL_ALIGN {
	T_STEP_CELL StepCell;
	double dAlignScore[2];
} T_STEP_CELL_ALIGN;

typedef struct T_FDC {
	T_STEP_CELL Conveyor[5];
	T_STEP_CELL GlassTransfer; // LD01_TRO1
} T_FDC;

#pragma pack(pop)
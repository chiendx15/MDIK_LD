#pragma once

#include "IOInfo_1_LD.hxx"
#include "IOInfo_2_SCT.hxx"
#include "IOInfo_3_CCT_L.hxx"
#include "IOInfo_3_CCT_R.hxx"
#include "IOInfo_4_UND_L.hxx"
#include "IOInfo_4_UND_R.hxx"
typedef enum eMACHINE{
	eMACHINE_LD,
	eMACHINE_SCT,
	eMACHINE_CCTL,
	eMACHINE_CCTR,
	eMACHINE_UNDL,
	eMACHINE_UNDR,
	nMACHINE_END,
	// CCT Unit 아래 내용 추가해서 따로 관리 해야 할것 같음.
	eMACHINE_CCTL_1 = nMACHINE_END,
	eMACHINE_CCTL_2,
	eMACHINE_CCTR_3,
	eMACHINE_CCTR_4,
	nMACHINE_UNIT_END,
} eMACHINE;

typedef enum eIO_LOCK {
	eIO_LOCK_UNUSED,
	eIO_LOCK_UI,
	eIO_LOCK_CONTROL,
}eIO_LOCK;
typedef struct
{
	struct T_IO_LD
	{
		T_IO_IN_1_LD In;
		T_IO_OUT_1_LD Out;
	} LD;

	struct T_IO_SCT
	{
		T_IO_IN_2_SCT In;
		T_IO_OUT_2_SCT Out;
	} SCT;

	struct T_IO_CCT_L
	{
		T_IO_IN_3_CCT_L In;
		T_IO_OUT_3_CCT_L Out;
	} CCTL;

	struct T_IO_CCT_R
	{
		T_IO_IN_3_CCT_R In;
		T_IO_OUT_3_CCT_R Out;
	} CCTR;

	struct T_IO_UND_L
	{
		T_IO_IN_4_UND_L In;
		T_IO_OUT_4_UND_L Out;
	} UNDL;

	struct T_IO_UND_R
	{
		T_IO_IN_4_UND_R In;
		T_IO_OUT_4_UND_R Out;
	} UNDR;
} T_ALL_IO;


// PC 상태 통신용 - 필요시 더 추가할 것 (모든 PC에 동일하게)
typedef struct
{
	struct T_CONTROL_PC_STATUS
	{
		DWORD		fAutoMode;
		DWORD		fCycleStop;
		DWORD		fIsAlarm;
		DWORD		fPMMode;
		DWORD		fIsMaterial;
		DWORD		fOnlineMode;

		//<< 2024.06.14 LBH - EQP Status (Interlock)
		DWORD		fEQP_StepStop;		// Interlock 
		DWORD		fEQP_LoadingStop;		// Interlock 
		DWORD		fUNIT_LoadingStop[2];	// CCT전용
		DWORD		fIsUnitMaterial[2];		// CCT전용

		//>>
	} ControlPC[nMACHINE_END];
} T_ALL_PC_STATUS;
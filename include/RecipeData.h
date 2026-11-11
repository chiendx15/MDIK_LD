#pragma once
#include "for_each_member.h"
#include "MotorInfos.hxx"

#define CIM_RECIPE_COUNT 90				
#define PREFIX_TEST_RECIPE _T("TT")

#pragma pack(push, 8)
WARNING_PUSH_AND_DISABLE(4267)

enum eRECIPE_COLUMN
{
	eRC_NONE,
	eRC_INDEX,
	eRC_PPID,
	eRC_RECIPE_ID,
	eRC_PANNEL_SIZE,
	eRC_MODIFIED_COUNT,
	eRC_LASTUPDATE_TIME,
	eRC_DESCRIPTION,
};

using std::vector;

typedef struct T_IMAGE_DATA_RECIPE
{
	CString imgName = _T("");
	double dPassRate = 0.;
	CPoint2d ptCenterOffset = {0, };
	CString imgPath = _T("");
} T_IMAGE_DATA_RECIPE;

class T_RECIPE 
{
public:
	T_RECIPE& operator=(const T_RECIPE& B) {
		if (this == &B)
			return *this;

		applied = false;
		cimapplied = false;
		recipeName = B.recipeName;
		// 2021.07.05 ppid는 복사 안되도록 수정
		ppid.Empty();
		//ppid = B.ppid;
		pannelSize = B.pannelSize;
		modifiedCount = B.modifiedCount;
		lastupdateTime = B.lastupdateTime;
		description = B.description;
		return *this;
	}

	FEM_BEGIN_TBL(T_RECIPE)
		//FEM_ADD_MEMBER(applied) // apply 상태는 ini 파일에만 저장
		FEM_ADD_MEMBER(recipeName)
		FEM_ADD_MEMBER(ppid)
		FEM_ADD_MEMBER(pannelSize)
		FEM_ADD_MEMBER(modifiedCount)
		FEM_ADD_MEMBER(lastupdateTime)
		FEM_ADD_MEMBER(description)
	FEM_END_TBL()
	FEM_SERIALIZE(T_RECIPE)

public:
	bool				applied = false;
	bool				cimapplied = false;
	CString				recipeName = _T("");
	CString				ppid = _T("");
	CSize2d				pannelSize = { 0, 0 };
	size_t				modifiedCount = 0;
	CString				lastupdateTime = _T("");
	CString				description = _T("");
};

class T_PANNEL_RECIPE_DATA
{
	// 패널 가로,세로 사이즈는 select화면에 표시해야해서 T_RECIPE에 있음
public:
	double dThickness = 0.;

	double alignMarkPosA = 0.;
	double alignMarkPosB = 0.;
	double alignMarkPosC = 0.;
	double alignMarkPosD = 0.;

	double scribeMarkPosA = 0.;
	double scribeMarkPosB = 0.;
	double scribeMarkPosC = 0.;
	double scribeMarkPosD = 0.;

	double MCRPosA = 0.;
	double MCRPosB = 0.;

public:
	FEM_BEGIN_TBL(T_PANNEL_RECIPE_DATA)
		FEM_ADD_MEMBER(dThickness)

		FEM_ADD_MEMBER(alignMarkPosA)
		FEM_ADD_MEMBER(alignMarkPosB)
		FEM_ADD_MEMBER(alignMarkPosC)
		FEM_ADD_MEMBER(alignMarkPosD)

		FEM_ADD_MEMBER(scribeMarkPosA)
		FEM_ADD_MEMBER(scribeMarkPosB)
		FEM_ADD_MEMBER(scribeMarkPosC)
		FEM_ADD_MEMBER(scribeMarkPosD)

		FEM_ADD_MEMBER(MCRPosA)
		FEM_ADD_MEMBER(MCRPosB)
	FEM_END_TBL()
	FEM_SERIALIZE(T_PANNEL_RECIPE_DATA)
};
class T_PPBOX_RECIPE_DATA {
public:
	double dGlassThickness = 0.;
	double dPaperThickness = 0.;
	int	iGlassCount = 0;
	int	iPaperCount = 0;
	BOOL bTopMaterialType = TRUE;		// Glass 가 먼저인지 Paper 가 먼저인지 (TURE : Glass 가 먼저 / FALSE : Paper 가 먼저)
	double dUnloadTableOffsetX = 0.;
	double dUnloadTableOffsetY = 0.;

public:
	FEM_BEGIN_TBL(T_PPBOX_RECIPE_DATA)
		FEM_ADD_MEMBER(dGlassThickness)
		FEM_ADD_MEMBER(dPaperThickness)
		FEM_ADD_MEMBER(iGlassCount)
		FEM_ADD_MEMBER(iPaperCount)
		FEM_ADD_MEMBER(bTopMaterialType)
		FEM_ADD_MEMBER(dUnloadTableOffsetX)
		FEM_ADD_MEMBER(dUnloadTableOffsetY)
	FEM_END_TBL()
	FEM_SERIALIZE(T_PPBOX_RECIPE_DATA)
};

// KES 추가 2024.2.19
class T_VACUUM_BLOW_TIME_RECIPE {
public:
	double dDefaultCylinder = 5.;		// Time Out 시간들
	double dVacuum = 10.;				// Time Out 시간들
	double dBlow = 15.;					// Time Out 시간들
	double dVacuumOnDelay = /*3.*/ 2.5;			// Vacuum On 하고 Check 하기 전까지 대기 시간
	double dBlowOnWait = /*3.*/ 2.5;			// Blow On 하고 동작 대기 시간

public:
	FEM_BEGIN_TBL(T_VACUUM_BLOW_TIME_RECIPE)
		FEM_ADD_MEMBER(dDefaultCylinder)
		FEM_ADD_MEMBER(dVacuum)
		FEM_ADD_MEMBER(dBlow)
		FEM_ADD_MEMBER(dVacuumOnDelay)
		FEM_ADD_MEMBER(dBlowOnWait)
	FEM_END_TBL()
	FEM_SERIALIZE(T_VACUUM_BLOW_TIME_RECIPE)
};

// KES 추가 2024.2.15 - Picker 받는위치, 주는위치 Z축 Offset 
class T_PICKER_OFFSET_Z_RECIPE
{
public:
	double dGlassPickerLoad = 0.;
	double dGlassPickerUnload = 0.;	// Glass 언로딩일 경우만 적용

public:
	FEM_BEGIN_TBL(T_PICKER_OFFSET_Z_RECIPE)
		FEM_ADD_MEMBER(dGlassPickerLoad)
		FEM_ADD_MEMBER(dGlassPickerUnload)
		FEM_END_TBL()
		FEM_SERIALIZE(T_PICKER_OFFSET_Z_RECIPE)
};

//<< KES 추가 2024.06.25 - ConveyorParameter 에 있던 속도 관련 항목 레시피로 변경
class T_CONVEYOR_SPEED_RECIPE {
public:
	double	dHighSpeed = 3000.;
	double	dLowSpeed = 1500.;
	int		iACC1 = 1000;
	int		iACC2 = 1000;
	int		iDEC1 = 500;
	int		iDEC2 = 1000;

public:
	FEM_BEGIN_TBL(T_CONVEYOR_SPEED_RECIPE)
		FEM_ADD_MEMBER(dHighSpeed)
		FEM_ADD_MEMBER(dLowSpeed)
		FEM_ADD_MEMBER(iACC1)
		FEM_ADD_MEMBER(iACC2)
		FEM_ADD_MEMBER(iDEC1)
		FEM_ADD_MEMBER(iDEC2)
		FEM_END_TBL()
		FEM_SERIALIZE(T_CONVEYOR_SPEED_RECIPE)
};
//>>

// 변수 이름이 변경되는 경우 그 데이터는 로딩이 되지 않음. (Json -> 변수명에 값이 들어간 형태임)
// 기존 데이터를 유지할 필요가 있으면 변수 이름을 변경하지 마세요.
class T_MDI_RECIPE : public T_RECIPE
{ 
public:
	T_PPBOX_RECIPE_DATA					ppbox;
	T_VACUUM_BLOW_TIME_RECIPE			vacuumBlowTime;	// KES 추가 2024.2.19
	T_PICKER_OFFSET_Z_RECIPE			pickerOffsetZ;
	T_CONVEYOR_SPEED_RECIPE				conveyorSpeed[5];

public:
	FEM_BEGIN_TBL_C(T_MDI_RECIPE, T_RECIPE)
		FEM_ADD_MEMBER(ppbox)
		FEM_ADD_MEMBER(vacuumBlowTime)
		FEM_ADD_MEMBER(pickerOffsetZ)
		FEM_ADD_MEMBER(conveyorSpeed[0])
		FEM_ADD_MEMBER(conveyorSpeed[1])
		FEM_ADD_MEMBER(conveyorSpeed[2])
		FEM_ADD_MEMBER(conveyorSpeed[3])
		FEM_ADD_MEMBER(conveyorSpeed[4])
	FEM_END_TBL()
	FEM_SERIALIZE(T_MDI_RECIPE)
};

WARNING_POP()
#pragma pack(pop)
#pragma once

#include "Mocha.type.hxx"

#define MAX_MCC_LAYER	40
#define MAX_MCC_TYPE_A	60
#define MAX_MCC_TYPE_S	60



#pragma pack(push, 1)

typedef struct {
	BOOL		bUploadUse;
	int			iUploadIntervalTime; // 분

	wchar_t		szIPAdress[32];
	int			iPortNo;
	wchar_t		szUserName[32];
	wchar_t		szUserPass[32];
	wchar_t		szServerPathSCT[64];
	wchar_t		szServerPathCCT[64];
	wchar_t		szLocalPath[64];
} T_MCC_PARAMETER;

typedef struct {
	wchar_t	szModuleName[64];
	wchar_t	szType[3];
	wchar_t	szParamName[128];
	wchar_t	szFrom[16];
	wchar_t	szTo[16];
} T_MCC_T;

typedef struct {
	char	szStepID[41];
	char	szCELL_ID[41];
	char	szLOT_ID[41];

	WORD	A[MAX_MCC_TYPE_A];
	WORD	S[MAX_MCC_TYPE_S];
} T_MCC_LAYER;

typedef struct {
	char			szPPID[41];
	T_MCC_LAYER		Layer[MAX_MCC_LAYER];
} T_MCC_EQP;

typedef struct {
	T_MCC_EQP	PCs[6];
} T_MCC;

#pragma pack(pop)
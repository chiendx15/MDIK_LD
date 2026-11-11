#pragma once

#include "Mocha.type.hxx"

#pragma pack(push, 8)

#define MOCHA_MAX_PROCESS 100

typedef struct {
	char		szVersion[128];
	BOOL		bInited;
	int			nProcess;
} T_MOCHA_INFO;

typedef struct {
	char		szProcessName[128];
	char		szProcessPath[512];
	char		szCmdLine[1024];
	BOOL		bConnected;
	double		tConnected;		// Time
	double		tDisconnected;	// Time
	BOOL		bStartedByFramework;
} T_MOCHA_PROCESS_INFO;

#pragma pack(pop)

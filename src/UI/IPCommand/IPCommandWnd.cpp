// C:\MDIK_HMBMB1500w_LD#\src\UI\IPCommand\IPCommandWnd.cpp: 구현 파일
//

#include "pch.h"
#include "UI.h"
#include "IPCommandWnd.h"


// CIPCommandWnd
IMPLEMENT_DYNAMIC(CIPCommandWnd, CWnd)

CIPCommandWnd::CIPCommandWnd() : IWNDIPCommandTarget(m_hWnd, &g_ipClient)
{
}

CIPCommandWnd::~CIPCommandWnd()
{
}

// 임시로 Process 명칭 여기다가 배열로 선언 
CString strProcess[] = {
	_T("UI"),
	_T("PLCLinker"),
	_T("DataTransfer"),
	_T("Serial"),
	_T("SystemAlarm"),
	_T("Sequence"),
	_T("Vision"),
};


BEGIN_MESSAGE_MAP(CIPCommandWnd, CWnd)
END_MESSAGE_MAP()


BEGIN_IP_COMMAND_TABLE(CIPCommandWnd, IIPCommandTarget)
ON_IP_EXCL("SCTIO", "OnOff",	OnIP_SCTIOOnOff)
ON_IP_EXCL("Recv", "Recipe",	OnIP_RecvRecipe)
ON_IP("HSMS", "Transfer",		OnIP_HSMSTransfer)
ON_IP("DeviceParam", "CimReportChange", OnIP_CimReportChange)
ON_IP("ShiftTable", "Vacuum", OnIP_ShiftTableVacuum)	// KES 추가 2024.4.7


//ON_IP("SendMessage", "S5F1_ALARM_REPORT_SEND", OnIP_S5F1_ALARM_REPORT_SEND)
//<<
// SEND_MESSAGE/POST_MESSAGE 사용시 session, Command 가 같아야 함.
//ON_IP("SendMessage", "S5F1_ALARM_REPORT_SEND", OnIP_SEND_MESSAGE)

ON_IP("SendMessage", "S6F11_INTERLOCK_CONFIRM", OnIP_POST_MESSAGE)
ON_IP("SendMessage", "S6F11_OPCALL_CONFIRM", OnIP_POST_MESSAGE)

ON_IP("SendMessage", "S6F11_CASSETTE_LOAD_REQUEST",				OnIP_POST_MESSAGE)
ON_IP("SendMessage", "S6F11_CASSETTE_LOAD_COMPLETE",			OnIP_POST_MESSAGE)
ON_IP("SendMessage", "S6F11_CARRIER_RELEASE_COMPLETE",			OnIP_POST_MESSAGE)

ON_IP("SendMessage", "S6F11_CARRIER_INFORMATION_REQUEST",		OnIP_POST_MESSAGE)
ON_IP("SendMessage", "S6F11_CASSETTE_UNLOAD_REQUEST",			OnIP_POST_MESSAGE)
ON_IP("SendMessage", "S6F11_CASSETTE_UNLOAD_COMPLETE",			OnIP_POST_MESSAGE)

ON_IP("Sequence", "RECV_PPBOX_LOT_DATA",						OnIP_READ_PPBOX_LOT_DATA)
ON_IP("Sequence", "DELETE_PPBOX_LOT_DATA",						OnIP_DELETE_PPBOX_LOT_DATA)
//>>


//-------- Reset
ON_IP("SendMessage", "S6F11_CARRIER_INFORMATION_REQUEST_RESET", OnIP_POST_MESSAGE)
ON_IP("SendMessage", "S6F11_CARRIER_RELEASE_COMPLETE_RESET",	OnIP_POST_MESSAGE)
//>>

//<< MCC 통신용 IPCommand
ON_IP_EXCL("MCC", "GTComponentIn", OnIP_MCCGTComponentIn)
ON_IP_EXCL("MCC", "GTVacOn", OnIP_MCCGTVacOn)
//>>


//<< 2024.07.02 LBH - EQP Status 관련
ON_IP("Scenario", "AUTO_RUN", OnIP_AUTO_RUN)
//>>

ON_IP("RECIPE", "SAVE_AND_APPLY", OnIP_SAVE_AND_APPLY_RECIPE)

ON_IP("SendMessage", "S5F1_ALARM_REPORT_SEND", OnIP_ALARM_REPORT_SEND)
END_IP_COMMAND_TABLE()

// CIPCommandWnd 메시지 처리기

BOOL CIPCommandWnd::OnIP_SEND_MESSAGE(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	enum {
		S_INIT,
		S_SEND,
		S_CHECK,
		S_END,
	};

	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		return FALSE;
	}

	auto& SEQ_STATUS = g_mapSEQ_CIM[{cmd.GetSession(), cmd.GetCommand()}];
    // SendIPCommand로 했을 때 문제 없는지 확인 해야됨.
	
	ULONGLONG ulTick = cmd.m_varCookie.ChildLValue<ULONGLONG>("Tick", GetTickCount64());
	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			SEQ_STATUS = eSEQ_READY;
			cmd.EnqueueNextStep();
			break;
		}
		case S_SEND: {
			ulTick = GetTickCount64();
			g_ipCIMClient.SendIPCommand(cmd.GetSession(), cmd.GetCommand(), var);
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK: {
			if (SEQ_STATUS == eSEQ_COMPLETED) {
				cmd.EnqueueNextStep();
			}
			else {
				if (SEQ_STATUS == eSEQ_ALARM || (GetTickCount64() - ulTick) > 30 * 1000) {
					return FALSE;
				}
				cmd.EnqueueNextStepCurrent();
			}
			break;
		}
		case S_END: {
			break;
		}
	}

	return TRUE;
}

BOOL CIPCommandWnd::OnIP_POST_MESSAGE(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	return g_ipCIMClient.PostIPCommand(cmd.GetSession(), cmd.GetCommand(), var);
}

BOOL CIPCommandWnd::OnIP_S6F11_INTERLOCK_CONFIRM(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	return g_ipCIMClient.PostIPCommand(cmd.GetSession(), cmd.GetCommand(), var);
	//return TRUE;
}

BOOL CIPCommandWnd::OnIP_S6F11_OPCALL_CONFIRM(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	return g_ipCIMClient.PostIPCommand(cmd.GetSession(), cmd.GetCommand(), var);
	//return TRUE;
}


BOOL CIPCommandWnd::OnIP_READ_PPBOX_LOT_DATA(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CString strCSTID = var("CSTID", _T(""));
	int _eModule = var("eMODULEs", eMODULE_NONE);

	auto& eStatus = sm.Module[_eModule].Status.eReportStatus;
	if (strCSTID.IsEmpty()) {
		eStatus = eCIM_ERROR;
		return FALSE;
	}

	eStatus = eCIM_START;
	auto& item = LoadLotInfo(eEQP_SCT, strCSTID);

	if (!item.first) {
		eStatus = eCIM_ERROR;
		return FALSE;
	}

	memcpy(sm.LoadPPBox, &item.second, sizeof(*sm.LoadPPBox));

	// 2024.06.21 LBH - Lot Data Glass 에 Copy
	for (int iGlass = 0; iGlass < sm.LoadPPBox->TRAY_INFOs[0].nCell; iGlass++) {
		auto& Glass = sm.LoadPPBox->TRAY_INFOs[0].CELL_INFOs[iGlass];

		strncpy_s(Glass.szCST_ID,		countof(Glass.szCST_ID),	sm.LoadPPBox->CassetteInfo.szID,		countof(Glass.szCST_ID));
		strncpy_s(Glass.szPPID,			countof(Glass.szPPID),		sm.LoadPPBox->CassetteInfo.szPPID,		countof(Glass.szPPID));
		strncpy_s(Glass.szPRODUCTID,	countof(Glass.szPRODUCTID), sm.LoadPPBox->CassetteInfo.szPRODUCT,	countof(Glass.szPRODUCTID));
		strncpy_s(Glass.szSTEPID,		countof(Glass.szSTEPID),	sm.LoadPPBox->CassetteInfo.szSTEPID,	countof(Glass.szSTEPID));
	}

	eStatus = eCIM_COMPLETE;
	return TRUE;
}



BOOL CIPCommandWnd::OnIP_DELETE_PPBOX_LOT_DATA(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CString strCSTID = var("CSTID", _T(""));
	int _eModule = var("eMODULEs", eMODULE_NONE);

	auto& eStatus = sm.Module[_eModule].Status.eReportStatus;
	if (strCSTID.IsEmpty()) {
		if (!sm.DeviceParameter->tUse.bDryRun) {
			//24.07.20 KGH EDIT : BCR ID 이상시에도 그냥 진행되도록 수정
			eStatus = eCIM_COMPLETE;
			//eStatus = eCIM_ERROR;
			return FALSE;
		}
		else {
			eStatus = eCIM_COMPLETE;
			return FALSE;
		}
	}

	eStatus = eCIM_START;
	DeleteLotInfo(eEQP_SCT, strCSTID);


	eStatus = eCIM_COMPLETE;
	return TRUE;
}

BOOL CIPCommandWnd::OnIP_RecvRecipe(CIPCommand& cmd)
{
    SendRecipe();
    return TRUE;
}

BOOL CIPCommandWnd::OnIP_SCTIOOnOff(CIPCommand& cmd)
{
    const CIPVar& var = cmd.m_var;

    CStringA session = var.GetChildItem("Session", "");
    CStringA command = var.GetChildItem("Command", "");
    g_ipCIMClient.PostIPCommand("SCT", session, command, var);

    return TRUE;
}


BOOL CIPCommandWnd::OnIP_HSMSTransfer(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;
	CIPVar& varResult = cmd.m_varResult;

	CStringA session = var.GetChildItem("TargetSession", "");
	CStringA command = var.GetChildItem("TargetCommand", "");

	if (session.IsEmpty() || command.IsEmpty())
	{
		return FALSE;
	}

	g_ipCIMClient.PostIPCommand(session, command, var);
	return TRUE;
}


BOOL CIPCommandWnd::OnIP_CimReportChange(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;

	if (!g_ipCIMClient.SendIPCommand(cmd.GetSession(), cmd.GetCommand(), var, 5*1000))
		return FALSE;
	return TRUE;
}

//<< KES 추가 2024.4.7
BOOL CIPCommandWnd::OnIP_ShiftTableVacuum(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;

	if (!cmd.IsOK() || !cmd.IsChildOK())
	{
		return FALSE;
	}

	g_ipCIMClient.PostIPCommand("SCT", NULL, "ShiftTable", "Vacuum", var);

	return TRUE;
}
//>>

//<< KES 2024.06.07 - 모듈간 MCC 통신용 IPCommand
BOOL CIPCommandWnd::OnIP_MCCGTComponentIn(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;
	g_ipCIMClient.PostIPCommand("SCT", "MCC", "SetGTComponentIn", var);
	return TRUE;
}


BOOL CIPCommandWnd::OnIP_MCCGTVacOn(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;
	g_ipCIMClient.PostIPCommand("SCT", "MCC", "SetGTVacOn", var);
	return TRUE;
}
//>>


BOOL CIPCommandWnd::OnIP_AUTO_RUN(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		return FALSE;
	}

	enum {
		S_INIT,
		S_SEND,
		S_CHECK,
		S_END,
	};

	ULONGLONG ulTick = cmd.m_varCookie.ChildLValue<ULONGLONG>("Tick", GetTickCount64());
	auto& SEQ_STATUS = g_mapSEQ_CIM[{cmd.GetSession(), cmd.GetCommand()}];

	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			SEQ_STATUS = eSEQ_READY;
			cmd.EnqueueNextStep();
			break;
		}
		case S_SEND: {
			ulTick = GetTickCount64();
			g_ipCIMClient.PostIPCommand(cmd.GetSession(), cmd.GetCommand(), var);
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK: {
			if (SEQ_STATUS == eSEQ_COMPLETED) {
				cmd.EnqueueNextStep();
			}
			else {
				if (SEQ_STATUS == eSEQ_ALARM || (GetTickCount64() - ulTick) > 30 * 1000) {
					return FALSE;
				}
				cmd.EnqueueNextStepCurrent();
			}
			break;
		}
		case S_END: {
			break;
		}
	}

	return TRUE;
}

//<< 2024.07.03 LBH - RECIPE 관련
BOOL CIPCommandWnd::OnIP_SAVE_AND_APPLY_RECIPE(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		return FALSE;
	}

	enum {
		S_INIT,
		S_SEND,
		S_CHECK,
		S_END,
	};

	ULONGLONG ulTick = cmd.m_varCookie.ChildLValue<ULONGLONG>("Tick", GetTickCount64());
	auto& SEQ_STATUS = g_mapSEQ_CIM[{cmd.GetSession(), cmd.GetCommand()}];

	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			SEQ_STATUS = eSEQ_READY;
			cmd.EnqueueNextStep();
			break;
		}
		case S_SEND: {
			ulTick = GetTickCount64();
			g_ipCIMClient.PostIPCommand(cmd.GetSession(), cmd.GetCommand(), var);
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK: {
			if (SEQ_STATUS == eSEQ_COMPLETED) {
				cmd.EnqueueNextStep();
			}
			else {
				if (SEQ_STATUS == eSEQ_ALARM || (GetTickCount64() - ulTick) > 30 * 1000) {
					return FALSE;
				}
				if (SEQ_STATUS == eSEQ_READY && (GetTickCount64() - ulTick) > 3 * 1000) {
					return FALSE;
				}
				cmd.EnqueueNextStepCurrent();
			}
			break;
		}
		case S_END: {
			break;
		}
	}

	return TRUE;
}
//>>

BOOL CIPCommandWnd::OnIP_ALARM_REPORT_SEND(CIPCommand& cmd) {
    const CIPVar& var = cmd.m_var;
	enum {
		S_INIT,
		S_SEND,
		S_CHECK,
		S_END,
	};

	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		return FALSE;
	}

	T_SYSTEM_ALARM_ITEM alarm;
	var.GetChildItemT<T_SYSTEM_ALARM_ITEM>("T_SYSTEM_ALARM_ITEM", alarm);
	auto& AlarmReport = g_mapAlarmReport[{ cmd.GetCommand(), alarm.iALID}];
    // SendIPCommand로 했을 때 문제 없는지 확인 해야됨.
	
	ULONGLONG ulTick = cmd.m_varCookie.ChildLValue<ULONGLONG>("Tick", GetTickCount64());
	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			AlarmReport = eSEQ_READY;
			cmd.EnqueueNextStep();
			break;
		}
		case S_SEND: {
			ulTick = GetTickCount64();
			g_ipCIMClient.SendIPCommand(cmd.GetSession(), cmd.GetCommand(), var);
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK: {
			if (AlarmReport == eSEQ_COMPLETED) {
				cmd.EnqueueNextStep();
			}
			else {
				if (AlarmReport == eSEQ_ALARM || (GetTickCount64() - ulTick) > 30 * 1000) {
					return FALSE;
				}
				cmd.EnqueueNextStepCurrent();
			}
			break;
		}
		case S_END: {
			break;
		}
	}

	return TRUE;
}
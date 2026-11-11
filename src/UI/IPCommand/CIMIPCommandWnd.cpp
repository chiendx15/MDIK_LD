#include "pch.h"
#include "UI.h"
#include "CIMIPCommandWnd.h"
// CCIMIPCommandWnd

IMPLEMENT_DYNAMIC(CCIMIPCommandWnd, CWnd)

CCIMIPCommandWnd::CCIMIPCommandWnd() : IWNDIPCommandTarget(m_hWnd, &g_ipCIMClient) {

}

CCIMIPCommandWnd::~CCIMIPCommandWnd()
{
}


BEGIN_MESSAGE_MAP(CCIMIPCommandWnd, CWnd)
END_MESSAGE_MAP()


BEGIN_IP_COMMAND_TABLE(CCIMIPCommandWnd, IIPCommandTarget)
	ON_IP("Get", "RecipeInfo", OnIP_GetRecipeInfo)

	ON_IP_EXCL("PM", "ModeChange",	OnIPPMModeChange)
	ON_IP_EXCL("PM", "DoorLock",	OnIPPMDoorLock)
	ON_IP_EXCL("SET", "DoorRelease", OnIP_SetDoorRelease)
	ON_IP_EXCL("Show", "PM", OnIPShowPM)

	ON_IP_EXCL("RecvMessage", "S2F31_DATE_TIME_SET", OnIP_S2F31_DATE_TIME_SET)
	ON_IP_EXCL("RecvMessage", "S10F3_TERMINAL_DISPLAY_SINGLE", OnIP_S10F3_TERMINAL_DISPLAY_SINGLE)

	//<< 2024.05.28 LBH
	ON_IP("RecvMessage", "INTERLOCK",			OnIP_RECV_INTERLOCK)
	ON_IP("RecvMessage", "INTERLOCK_CONFIRM",	OnIP_RECV_INTERLOCK_CONFIRM)
	//>>
	ON_IP("DeviceParam", "CimReportChange", OnIP_CimReportChange)
	// MCR Data Get 
	ON_IP("Recv", "GlassID", OnIP_RecvGlassID)

	ON_IP_EXCL("Recipe", "Change", OnIP_RecipeChange)

	//<< 2024.04.11 LBH - 
	ON_IP("SendMessage", "S6F11_INTERLOCK_CONFIRM",					OnIP_CIM_POST_IP_COMMADN)
	ON_IP("SendMessage", "S6F11_OPCALL_CONFIRM",					OnIP_CIM_POST_IP_COMMADN)

	//ON_IP("SendMessage", "S5F1_ALARM_REPORT_SEND",					OnIP_CIM_SEND_IP_STEP_COMMAND)

	ON_IP("SendMessage", "S6F11_CASSETTE_LOAD_REQUEST",				OnIP_CIM_SEND_IP_STEP_COMMAND)
	ON_IP("SendMessage", "S6F11_CASSETTE_LOAD_COMPLETE",			OnIP_CIM_SEND_IP_STEP_COMMAND)
	ON_IP("SendMessage", "S6F11_CARRIER_RELEASE_COMPLETE",			OnIP_CIM_SEND_IP_STEP_COMMAND)

	ON_IP("SendMessage", "S6F11_CARRIER_INFORMATION_REQUEST",		OnIP_CIM_SEND_IP_STEP_COMMAND)
	ON_IP("SendMessage", "S6F11_CASSETTE_UNLOAD_REQUEST",			OnIP_CIM_SEND_IP_STEP_COMMAND)
	ON_IP("SendMessage", "S6F11_CASSETTE_UNLOAD_COMPLETE",			OnIP_CIM_SEND_IP_STEP_COMMAND)

	//-------- Reset
	ON_IP("SendMessage", "S6F11_CARRIER_INFORMATION_REQUEST_RESET",	OnIP_CIM_SEND_IP_STEP_COMMAND)
	ON_IP("SendMessage", "S6F11_CARRIER_RELEASE_COMPLETE_RESET",	OnIP_CIM_SEND_IP_STEP_COMMAND)
	//>>

	//<< 2024.07.02 LBH - EQP Status 관련
	ON_IP("Scenario", "MANUAL_STOP",		OnIP_MANUAL_STOP)
	ON_IP("Scenario", "CYCLE_STOP",			OnIP_CYCLE_STOP)
	ON_IP("Scenario", "INTERLOCK_RELEASE",	OnIP_INTERLOCK_RELEASE)

	ON_IP("Scenario", "AUTO_RUN",			OnIP_AUTO_RUN)
	//>>

	ON_IP("CIM", "Disconnect", OnIP_CIM_Disconnect)

	ON_IP("RECIPE", "SAVE_AND_APPLY",	OnIP_SAVE_AND_APPLY_RECIPE)
	ON_IP("RECIPE", "GET_RECIPE_PARAM", OnIP_GET_RECIPE_PARAM)

	ON_IP("SendMessage", "S5F1_ALARM_REPORT_SEND", OnIP_ALARM_REPORT_SEND)
	ON_IP_EXCL("RECIPE", "CIM_CONTROL", OnIP_RECIPE_CIM_CONTROL)
END_IP_COMMAND_TABLE()

void IPFuncTemp() {}



BOOL CCIMIPCommandWnd::OnIP_GetRecipeInfo(CIPCommand& cmd)
{
	CIPVar& varResult = cmd.m_varResult;

	varResult.SetChildItem("Index", g_recipes.GetAppliedIndex());

	T_MDI_RECIPE* appliedRecipe = g_recipes.GetAppliedData();
	if (appliedRecipe != nullptr)
	{
		varResult.SetChildItem("PPID", appliedRecipe->ppid);
	}
	
	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIPPMModeChange(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;
	CIPVar& varResult = cmd.m_varResult;
	CIPVar& varCookie = cmd.m_varCookie;
	BOOL bAuto = var.GetChildItem("Auto", TRUE);
	enum {
		S_INIT,
		S_IO_CHANGE,
		S_IO_CHECK,
		S_END
	};
	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			cmd.EnqueueNextStep();
			break;
		}
		case S_IO_CHANGE: {
			
			varCookie.SetChildItem("Time",GetTickCount());
			cmd.EnqueueNextStep();
			break;
		}
		case S_IO_CHECK: {

			//if (sm.IO->tOut_CRT.ob60003)
				cmd.EnqueueNextStep();
			//else {
			//	if (GetTickCount() - varCookie.GetChildItem("Time",0) > 3 * 1000)
			//		return FALSE;
			//	else 
			//		cmd.EnqueueNextStepCurrent(100);
			//}
			break;
		}
		case S_END: {
			break;
		}
	}
	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIPPMDoorLock(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;
	CIPVar& varResult = cmd.m_varResult;
	CStringA strTask = cmd.GetTaskSender();
	BOOL bLock = var.GetChildItem(_T("Lock"), TRUE);

	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIP_SetDoorRelease(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;
	CIPVar& varResult = cmd.m_varResult;

	BOOL bSet = var.GetChildItem(_T("SET"), TRUE);
	
	sm.IO->tOut_NEW.fLD_SCT_DOOR_LOCK_RELEASE_REQUEST = !bSet;	//검수시 주석풀것
	sm.IO->tOut_NEW.fSCT_DUMMY_DOOR_LOCK_RELEASE_REQUEST = !bSet;

	sm.Option->tSequence.bKeepOnDoorOpenMelody = bSet;	//Melody 관련 공유메모리 ON
	sm.Option->tSequence.bDoorOpenLampOn = bSet;
	//24.06.06 KGH DEL : DOOR OPEN시에만 MELODY ON 되도록 수정
	//sm.IO->tOut_NEW.fLD_MELODY_1 = bSet;

	sm.IO->tOut_NEW.fLD_DOOR01_RELEASE_SW = bSet;
	sm.IO->tOut_NEW.fLD_DOOR02_RELEASE_SW = bSet;
	sm.IO->tOut_NEW.fLD_DOOR03_RELEASE_SW = bSet;
	sm.IO->tOut_NEW.fLD_DOOR04_RELEASE_SW = bSet;
	sm.IO->tOut_NEW.fLD_DOOR05_RELEASE_SW = bSet;
	sm.IO->tOut_NEW.fLD_DOOR06_RELEASE_SW = bSet;
	
	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIPShowPM(CIPCommand& cmd) {
	CIPVar var; 
	g_ipClient.PostIPCommand("PM", "Show", var);
	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIP_S2F31_DATE_TIME_SET(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;

	CStringA curTime = var.GetChildItem("TIME", "");

	SYSTEMTIME st;	ZeroVar(st);
	st.wYear	= atoi(curTime.Mid(0, 4));
	st.wMonth	= atoi(curTime.Mid(4, 2));
	st.wDay		= atoi(curTime.Mid(6, 2));
	st.wHour	= atoi(curTime.Mid(8, 2));
	st.wMinute	= atoi(curTime.Mid(10, 2));
	st.wSecond	= atoi(curTime.Mid(12, 2));
	SetLocalTime(&st);

	return TRUE;
}

// CCIMIPCommandWnd 메시지 처리기
BOOL CCIMIPCommandWnd::OnIP_S10F3_TERMINAL_DISPLAY_SINGLE(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;
	CIPVar& varResult = cmd.m_varResult;

	g_ipClient.PostIPCommand(cmd.GetSession(), cmd.GetCommand(), var);

	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIP_RECV_INTERLOCK(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	CStringA curTime = var.GetChildItem("TIME", "");

	CStringA RCMD = var("RCMD", "");
	int iUnit = var("UNIT", -1);

	// 추후 처리.
	if (RCMD.IsEmpty()) {
		return FALSE;
	}

	if (atoi(RCMD) == eRCMD_LOADING_STOP) {
		if (iUnit == -1) {
			sm.Sequence->fCIM_LoadingStop = TRUE;
		}
		else {
			//sm.Sequence->fUNIT_LoadingStop[iUnit];
		}
		//SetAlarm(eAI_INTERLOCK_LOADING_STOP, IDS_DES_NOT_USE);
		// Light Alarm
		// Load Stop이후 자재 배출 후 정지.
		// Auto Run 진입 시 Material이 없을 경우에 초기화 가능. (Box, Tray 상관없음. Glass, Sitck, Cell만 보면 됨)
	}
	// 2024.06.14 LBH - 수정.
	else if (atoi(RCMD) == eRCMD_INTERLOCK_SEND || atoi(RCMD) == eRCMD_STEP_STOP) {
		sm.Sequence->fCIM_StepStop = FALSE;
		sm.Sequence->fAutoMode = FALSE;
	}

	return TRUE;
}


BOOL CCIMIPCommandWnd::OnIP_RECV_INTERLOCK_CONFIRM(CIPCommand& cmd) {
	g_ipClient.PostIPCommand("UI", "", "INTERLOCK", "SHOW");

	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIP_CimReportChange(CIPCommand& cmd)
{
	enum {
		eS_CHECK_AUTORUN,
		eS_CHECK_MATERIAL,
		eS_CHECK_PPID,
		eS_SET_PARAM,
	};
	const CIPVar& var = cmd.m_var;
	CIPVar& varResult = cmd.m_varResult;
	CIPVar& varCookie = cmd.m_varCookie;

	bool bParam = var.GetChildItem("CimReport", false);

	if (!cmd.IsOK() || !cmd.IsChildOK())
	{
		CString strErrorMsg = varCookie.GetChildItem("ERRORMSG", L"");
		strErrorMsg += Format(_T("\r\n%s %s", bParam == true ? L"Online": L"Offline",g_st[IDS_CIM_IP_COMMAND_WND_MODE_CHANGE_FAIL]));
		IPMessage(strErrorMsg);
		return FALSE;
	}

	CStringA PCs[] = { "LD", "SCT", "CCTL", "CCTR", "ULDL", "ULDR" };
	BOOL bCheck[] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE };
	//CStringA PCs[] = { "LD", "SCT", "CCTR", "ULDR" };
	//BOOL bCheck[] = { FALSE, FALSE, FALSE, FALSE };
	CString strErrMsg;


	switch (cmd.GetCurrentStep())
	{
	case eS_CHECK_AUTORUN:
	{
		if (!sm.Sequence->fAutoMode || (sm.Sequence->fAutoMode && sm.Sequence->fCycleStop))
		{
			cmd.EnqueueNextStep();
		}
		else
		{
			strErrMsg = g_st[IDS_CIM_IP_COMMAND_WND_MODE_CHANGE_NOT_POSSIBLE];
			varCookie.SetChildItem("ERRORMSG", strErrMsg);
			cmd.ReserveErrorStep();
		}
		break;
	}
	case eS_CHECK_MATERIAL:
	{
		if (bParam)
		{
			bool bCheckMaterial = false;
			int idx = -1;
			for (int i = 0; i < countof(PCs); i++)
			{
				bCheck[i] = sm.PCs->ControlPC[i].fIsMaterial;	
				bCheckMaterial |= bCheck[i];
			}

			if (bCheckMaterial)
			{
				for (int i = 0; i < countof(bCheck); i++)
				{
					if (bCheck[i])					{
						strErrMsg += (CString(PCs[i]) + L", ");
					}
				}
				strErrMsg += _T("\r\n%s",g_st[IDS_CIM_IP_COMMAND_WND_CELL_INFO_EXIST]);
				varCookie.SetChildItem("ERRORMSG", strErrMsg);
				cmd.ReserveErrorStep();
			}
			else
			{
				cmd.EnqueueNextStep();
			}
		}
		//Online -> Offline 일때는 자재정보 체크하지 않음
		else
		{
			cmd.EnqueueNextStep();
		}
		break;
	}
	case eS_CHECK_PPID:
	{
		if (bParam)
		{
			int idx = -1;
			bool bCheckPPID = false;
			for (int i = 1; i < countof(PCs); i++)
			{
				CIPVar varPPID, varPPIDresult;
				varPPID.SetChildItem("GetPPID", TRUE);
				if (g_ipCIMClient.SendIPCommand(PCs[i], cmd.GetSession(), cmd.GetCommand(), varPPID, varPPIDresult, 5*1000))
				{
					CString strLD_PPID = g_pRecipe->ppid;
					CString strGetPPID = varPPIDresult.GetChildItem("PPID", L"");
					if (strLD_PPID.CompareNoCase(strGetPPID) != 0)
					{
						bCheck[i] = true;
						bCheckPPID |= true;
					}
				}
				else
				{
					strErrMsg.Format(_T("%s %s", CString(PCs[i]), g_st[IDS_CIM_IP_COMMAND_WND_APPLIED_RECIPE_INFO_LOAD_FAIL]));
					varCookie.SetChildItem("ERRORMSG", strErrMsg);
					cmd.ReserveErrorStep();
					return TRUE;
				}
			}

			if (bCheckPPID)
			{
				for (int i = 0; i < countof(bCheck); i++)
				{
					if (bCheck[i])
					{
						strErrMsg += (CString(PCs[i]) + L", ");
					}
				}
				strErrMsg += _T("\r\n%s",g_st[IDS_CIM_IP_COMMAND_WND_APPLED_RECIPE_PPID_IS_NOT_SAME]);
				varCookie.SetChildItem("ERRORMSG", strErrMsg);
				cmd.ReserveErrorStep();
			}
			else
			{
				cmd.EnqueueNextStep();
			}
		}
		//Online -> Offline 일때는 PPID 여부 체크하지 않음
		else
		{
			cmd.EnqueueNextStep();
		}
		break;
	}
	case eS_SET_PARAM:
	{
		for (int i = 1; i < countof(PCs); i++)
		{
			CIPVar varPPID, varPPIDresult;
			varPPID.SetChildItem("SetParam", bParam);
			if (!g_ipCIMClient.SendIPCommand(PCs[i], cmd.GetSession(), cmd.GetCommand(), varPPID, varPPIDresult, 5*1000))
			{
				strErrMsg.Format(_T("%s %s: %s: Mode %s", CString(PCs[i]), g_st[IDS_CIM_IP_COMMAND_WND_EQUIPMENT],g_st[IDS_CIM_IP_COMMAND_WND_MODE_PARAM_CHANGE_FAIL],bParam == true ? L"Online" : L"Offline"));
				varCookie.SetChildItem("ERRORMSG", strErrMsg);
				cmd.ReserveErrorStep();
				return TRUE;
			}
		}
		cmd.EnqueueNextStep();
		break;
	}
	}
	return TRUE;
}
BOOL CCIMIPCommandWnd::OnIP_RecvGlassID(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varResult = cmd.m_varResult;
	CIPVar& varCookie = cmd.m_varCookie;

	int iGlassIndex = var.GetChildItem("Index", 0);
	CStringA strGlassID = var.GetChildItem("GlassID", "");

	//sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Glass.iGlassIndex = iGlassIndex;			// 여기서 들어가는 Index 로 판단해서 마지막 Glass MCR 넘어오면 CEID 4 진행
	//sprintf_s(sm.LoadPPBox[4].tGlass[iGlassIndex].szEGlassID, sizeof(sm.LoadPPBox[4].tGlass[iGlassIndex].szEGlassID), "%s", strGlassID.GetBuffer());

	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIP_RecipeChange(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;
	CIPVar& varResult = cmd.m_varResult;

	// 유저가 보는 화면이 파라미터, 레시피 편집이면 수정 실패
	if (g_eSelectedMainMenu == eMAIN_MENU_7)
	{
		varResult.SetChildItem("Success", FALSE);
		varResult.SetChildItem("Description", g_st[IDS_CIM_IP_COMMAND_WND_PARAM_EDIT_SCREEN]);
		return TRUE;
	}

	if (g_eSelectedMainMenu == eMAIN_MENU_3)
	{
		varResult.SetChildItem("Success", FALSE);
		varResult.SetChildItem("Description", g_st[IDS_CIM_IP_COMMAND_WND_RECIPE_EDIT_SCREEN]);
		return TRUE;
	}

	if (g_eSelectedMainMenu == eMAIN_MENU_2)
	{
		varResult.SetChildItem("Success", FALSE);
		varResult.SetChildItem("Description", g_st[IDS_CIM_IP_COMMAND_WND_MANUAL_MODE_SCREEN]);
		return TRUE;
	}

	int recipeIndex = var.GetChildItem("Index", -1);
	CString ppid = var.GetChildItem("PPID", L"");

	if (recipeIndex == -1)
	{
		varResult.SetChildItem("Success", FALSE);
		varResult.SetChildItem("Description", g_st[IDS_CIM_IP_COMMAND_WND_RECIPE_NUMVER_SEND_FAIL]);
		return TRUE;
	}

	if (g_recipes[recipeIndex] == nullptr)
	{
		varResult.SetChildItem("Success", FALSE);
		varResult.SetChildItem("Description", g_st[IDS_CIM_IP_COMMAND_WND_NONE_RECIPE_NUMBER]);
		return TRUE;
	}

	if (g_recipes[recipeIndex]->ppid.Compare(ppid) != 0)
	{
		varResult.SetChildItem("Success", FALSE);
		varResult.SetChildItem("Description", g_st[IDS_CIM_IP_COMMAND_WND_PPID_IS_DIFFRENT]);
		return TRUE;
	}


	if (g_recipes[recipeIndex] != nullptr && g_db.get() != nullptr && g_db->IsOpen() && g_recipes.GetAppliedIndex() != recipeIndex)
	{
		Update_ECID();

		auto ppid = g_recipes[recipeIndex]->ppid;

		int CIM = g_CimReport;	//LD 혹은 SCT = 1 / CCT 혹은 ULD = 2
		int CCODE = g_PC;		//SCT 보고시 LD = 1, SCT = 2, CCT 보고시 CCTL = 1 CCTR = 2 ULDL = 3 ULDR = 4
		int iCNT = -1;
		ePPID_MODE MODE = ePPID_MODE_NONE;
		CString strQuery;

		CADORecordset rs(g_db.get());
		strQuery.Format(L"SELECT count(*) as CNT FROM PPID_%s WHERE PPID = '%s'", CIM == 1 ? L"SCT" : L"CCT", ppid);
		if (rs.Open(strQuery, CADORecordset::openQuery))
		{
			rs.MoveFirst();
			while (!rs.IsEOF())
			{
				rs.GetFieldValue(_T("CNT"), iCNT);
				rs.MoveNext();
			}
		}
		rs.Close();

		auto InsertParamItem = [&](CString _paramName, CString _paramValue)
		{
			strQuery.Format(L"Insert into PPID_%s(PPID, CCODE, P_PARM_NAME, P_PARM) values('%s', %d, '%s', '%s')", CIM == 1 ? L"SCT" : L"CCT", ppid, CCODE, _paramName, _paramValue);
			g_db->Execute(strQuery);
		};

		if (iCNT > 0)
		{
			iCNT = -1;
			MODE = ePPID_MODE_MODIFICATION_OF_PPID;
			strQuery.Format(L"SELECT count(*) as CNT FROM PPID_%s WHERE PPID = '%s' and CCODE = %d", CIM == 1 ? L"SCT" : L"CCT", ppid, CCODE);
			if (rs.Open(strQuery, CADORecordset::openQuery))
			{
				rs.MoveFirst();
				while (!rs.IsEOF())
				{
					rs.GetFieldValue(_T("CNT"), iCNT);
					rs.MoveNext();
				}
			}
			rs.Close();


			if (iCNT > 0)
			{
				strQuery.Format(L"Delete From PPID_%s WHERE PPID = '%s' and CCODE = %d", CIM == 1 ? L"SCT" : L"CCT", ppid, CCODE);
				g_db->Execute(strQuery);

				auto vec = GetRecipeParam(g_recipes[recipeIndex]);
				for (int i = 0; i < vec.size(); i++)
					InsertParamItem(vec[i].first, vec[i].second);

			}
			else if (iCNT == 0)
			{
				auto vec = GetRecipeParam(g_recipes[recipeIndex]);
				for (int i = 0; i < vec.size(); i++)
					InsertParamItem(vec[i].first, vec[i].second);
			}
			else
			{
				varResult.SetChildItem("Success", FALSE);
				varResult.SetChildItem("Description", g_st[IDS_CIM_IP_COMMAND_WND_DB_QUERY_CCODE_SEARCH_FAIL]);
				return TRUE;
			}
		}
		else if (iCNT == 0)
		{
			MODE = ePPID_MODE_CREATION_OF_PPID;
			auto vec = GetRecipeParam(g_recipes[recipeIndex]);
			for (int i = 0; i < vec.size(); i++)
				InsertParamItem(vec[i].first, vec[i].second);
		}
		else
		{
			varResult.SetChildItem("Success", FALSE);
			varResult.SetChildItem("Description", g_st[IDS_CIM_IP_COMMAND_WND_DB_QUERY_PPID_SEARCH_FAIL]);
			return TRUE;
		}

		CIPVar varCim, varCimR;
		varCim.SetChildItem("MODE", MODE);
		varCim.SetChildItem("PPID", ppid);
		if (g_ipCIMClient.SendIPCommand("CIMTransfer", CIM == 1 ? "SCT" : "CCT", "SendMessage", "S7F107_PPID_SETTING", varCim, 3 * 1000))
		{
			g_recipes[recipeIndex]->cimapplied = true;
		}
		else
		{
			varResult.SetChildItem("Success", FALSE);
			varResult.SetChildItem("Description", g_st[IDS_CIM_IP_COMMAND_WND_CIM_REPORT_FAIL]);
			return TRUE;
		}
		g_recipes.setListData(recipeIndex);
		//g_recipes.saveRecipe(selectIndex);
	}
	/*
	* recipe 정보를 이용하여 DB 에 Insert 혹은 delete 후 Insert
	* ppid 로 조회해서 없을경우 mode 1 셋팅하고 레시피 데이터 insert
	* ppid 로 조회해서 있을경우 mode 3 셋팅
	* CCODE 로 조회해서 있을경우 PPID와 CCODE 가 동일한 데이터 모두 삭제후 다시 Insert
	* CCODE 로 조회해서 없을경우 Insert
	* 그후 CIM 보고 실행 "CIMTransfer", "SendMessage", "S7F101_PPID_SETTING"
	* "MODE" = 1 CIM APPLY 를 처음 할때
	*		 = 3 CIM APPLY 를 이미 했던경우
	* "PPID" = 현재 선택된 레시피의 PPID
	*/
	g_recipes.ApplyRecipe();
	SendRecipe();
	varResult.SetChildItem("Success", TRUE);

	return TRUE;
}

// CCIMIPCommandWnd 메시지 처리기
BOOL CCIMIPCommandWnd::OnIP_CIM_POST_IP_COMMADN(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	CStringA strCommand(cmd.GetCommand());

	if (strCommand.Compare("S6F11_INTERLOCK_CONFIRM") == 0) {
		CIM::Message::S6F11_INTERLOCK_CONFIRM(eEQP_SCT);
	}
	else if (strCommand.Compare("S6F11_OPCALL_CONFIRM") == 0) {
		CIM::Message::S6F11_OPCALL_CONFIRM(eEQP_SCT);
	}

	// 각 PC로 Confirm Message 전달.

	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIP_CIM_SEND_IP_STEP_COMMAND(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	enum {
		S_INIT,
		S_SEND,
		S_END,
	};

	int _eModule = var("eMODULEs", eMODULE_NONE);
	auto& SEQ_STATUS = g_mapSEQ_CIM[{cmd.GetSession(), cmd.GetCommand()}];
	auto& eStatus = sm.Module[_eModule].Status.eReportStatus;
	if (!cmd.IsChildOK() || !cmd.IsOK()) {
		eStatus = eCIM_ERROR;
		SEQ_STATUS = eSEQ_ALARM;
		return FALSE;
	}
	
	CStringA strCommand(cmd.GetCommand());
	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			eStatus = eCIM_START;
			SEQ_STATUS = eSEQ_START;
			cmd.EnqueueNextStep();
			break;
		}
		case S_SEND: {
			if (strCommand.Compare("S5F1_ALARM_REPORT_SEND") == 0) {
				T_SYSTEM_ALARM_ITEM alarm;
				var.GetChildItemT<T_SYSTEM_ALARM_ITEM>("T_SYSTEM_ALARM_ITEM", alarm);
				CIM::ALARM::ALARM_REPORT(eEQP_SCT, alarm, &cmd); // 보내고 완료 확인을 위해 SendIPStepCommand로 
			}
			else if (strCommand.Compare("S6F11_CASSETTE_LOAD_REQUEST") == 0) {
				int iPort = var("PORT", -1);
				CIM::Sequence::Load::S6F11_CASSETTE_STATUS_CHANGE_CEID_350_CASSETTE_LOAD_REQUEST(eEQP_SCT, iPort, &cmd);
			}
			else if (strCommand.Compare("S6F11_CASSETTE_LOAD_COMPLETE") == 0) {
				int iPort = var("PORT", -1);
				BOOL bUseAGV = var("USE_AGV", FALSE);
				CIM::Sequence::Load::S6F11_CASSETTE_STATUS_CHANGE_CEID_351_CASSETTE_LOAD_COMPLETE(eEQP_SCT, iPort, bUseAGV, &cmd);
			}
			else if (strCommand.Compare("S6F11_CARRIER_RELEASE_COMPLETE") == 0) {
				int iPort = var("PORT", -1);
				CStringA szCSTID(var("CSTID", ""));
				CIM::Sequence::Load::S6F11_CARRIER_PROCESS_CHANGE_CEID_257_CARRIER_RELEASE_COMPLETE(eEQP_SCT, iPort, szCSTID, &cmd);
			}

			else if (strCommand.Compare("S6F11_CARRIER_INFORMATION_REQUEST") == 0) {
				int iPort = var("PORT", -1);
				CStringA szCSTID(var("CSTID", ""));
				CIM::Sequence::Load::S6F11_CARRIER_PROCESS_CHANGE_CEID_262_CARRIER_INFORMATION_REQUEST(eEQP_SCT, iPort, szCSTID, &cmd);
			}
			else if (strCommand.Compare("S6F11_CASSETTE_UNLOAD_REQUEST") == 0) {
				int iPort = var("PORT", -1);
				CIM::Sequence::Unload::S6F11_CASSETTE_STATUS_CHANGE_CEID_352_CASSETTE_UNLOAD_REQUEST(eEQP_SCT, iPort, &cmd);
			}
			else if (strCommand.Compare("S6F11_CASSETTE_UNLOAD_COMPLETE") == 0) {
				int iPort = var("PORT", -1);
				CIM::Sequence::Unload::S6F11_CASSETTE_STATUS_CHANGE_CEID_353_CASSETTE_UNLOAD_COMPLETE(eEQP_SCT, iPort, &cmd);
			}

			// Reset


			else if (strCommand.Compare("S6F11_CARRIER_INFORMATION_REQUEST_RESET") == 0) {
				int iPort = var("PORT", -1);
				CStringA szCSTID(var("CSTID", ""));
				CIM::Sequence::Load::S6F11_CARRIER_PROCESS_CHANGE_CEID_262_CARRIER_INFORMATION_REQUEST_RESET(eEQP_SCT, iPort, &cmd);
			}
			else if (strCommand.Compare("S6F11_CARRIER_RELEASE_COMPLETE_RESET") == 0) {
				int iPort = var("PORT", -1);
				CStringA szCSTID(var("CSTID", ""));
				CIM::Sequence::Load::S6F11_CARRIER_PROCESS_CHANGE_CEID_257_CARRIER_RELEASE_COMPLETE_RESET(eEQP_SCT, iPort, &cmd);
			}

			cmd.EnqueueNextStep();
			break;
		}
		case S_END: {
			eStatus = eCIM_COMPLETE;
			SEQ_STATUS = eSEQ_COMPLETED;
			break;
		}
	}
	
	return TRUE;
}


BOOL CCIMIPCommandWnd::OnIP_CIM_Disconnect(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	g_ipCIMClient.StopIPCommandLoop();
	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIP_MANUAL_STOP(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	sm.Sequence->fAutoMode = FALSE;
	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIP_CYCLE_STOP(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	if (sm.Sequence->fAutoMode == TRUE)
		sm.Sequence->fCycleStop = TRUE;
	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIP_INTERLOCK_RELEASE(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	BOOL bLOADING_STOP = var("LOADING_STOP", FALSE);

	if (bLOADING_STOP)
		sm.Sequence->fCIM_LoadingStop = FALSE;

	// CCT의 경우 자체 판단하에 Unit Interlock Stop를 FALSE로 만든다 (Line의 자재 유무)

	// Step Stop 는 무조건 초기화 한다.
	sm.Sequence->fCIM_StepStop = FALSE;
	return TRUE;
}

// EQP -> CIM
BOOL CCIMIPCommandWnd::OnIP_AUTO_RUN(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	enum {
		S_SEND,
		S_END,
	};

	auto& SEQ_STATUS = g_mapSEQ_CIM[{cmd.GetSession(), cmd.GetCommand()}];

	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		SEQ_STATUS = eSEQ_ALARM;
		return FALSE;
	}

	switch (cmd.GetCurrentStep()) {
		case S_SEND: {
			SEQ_STATUS = eSEQ_START;
			SendIPStepCommand("CIMTransfer", lpEQP[eEQP_SCT], cmd.GetSession(), cmd.GetCommand(), var);
			cmd.EnqueueNextStep();
			break;
		}
		case S_END: {
			SEQ_STATUS = eSEQ_COMPLETED;
			break;
		}
	}

	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIP_SAVE_AND_APPLY_RECIPE(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	enum {
		S_GET_RECIPE_PARAM,
		S_SEND,
		S_END,
	};

	auto& SEQ_STATUS = g_mapSEQ_CIM[{cmd.GetSession(), cmd.GetCommand()}];

	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		SEQ_STATUS = eSEQ_ALARM;
		IPMessage(_T("SCT Recipe Check."));
		return FALSE;
	}

	int iPPID = var("PPID_NO", -1);
	CString strPPID = var("PPID", _T(""));
	BOOL bAPPLY = var("APPLY", FALSE);

	if (iPPID < 0) {
		cmd.ReserveErrorStep();
		return TRUE;
	}

	switch (cmd.GetCurrentStep()) {
		case S_GET_RECIPE_PARAM: {
			SEQ_STATUS = eSEQ_START;
			SendIPStepCommand("SCT", NULL, "RECIPE", "GET_RECIPE_PARAM", var);
			cmd.EnqueueNextStep();
			break;
		}
		case S_SEND: {
			const CIPVar& varResult = cmd.GetChildResult("SCT", NULL, "RECIPE", "GET_RECIPE_PARAM");
			CString strRecipeName = varResult("RecipeName", _T(""));	// KES 추가 2024.07.25
			int nCount = varResult("nPARAM", 0);

			//<< KES 추가 2024.07.25
			if (strPPID.Compare(strRecipeName) != 0) {
				SEQ_STATUS = eSEQ_ALARM;
				IPMessage(_T("ULD Recipe Check. RecipeName Missmatch."));
				return FALSE;
			}
			//>>

			std::vector<T_PPID_PARAM> vecParamLD = GetRecipeParam(iPPID);

			for (int i = 0; i < nCount; i++) {
				T_PPID_PARAM Param;
				Param.iParam = varResult.GetChildItem(Format("PARAM_%d", i), -1);
				Param.Name = varResult.GetChildItem(Format("NAME_%d", i), _T(""));
				Param.Value = varResult.GetChildItem(Format("VALUE_%d", i), _T(""));

				if (Param.iParam != -1 && !Param.Name.IsEmpty() && !Param.Value.IsEmpty())
					vecParamLD.push_back(Param);
			}

			std::sort
			(
				vecParamLD.begin(), vecParamLD.end(),
				[&](const T_PPID_PARAM& _item1, const T_PPID_PARAM& _item2) -> bool
				{
					return  _item1.iParam < _item2.iParam;
				}
			);

			CIPVar varSend;
			varSend.SetChildItem("PPID_NO", iPPID + 1);
			varSend.SetChildItem("PPID", strPPID);
			varSend.SetChildItem("nPARAM", vecParamLD.size());
			varSend.SetChildItem("APPLY", bAPPLY);

			for (int i = 0; i < vecParamLD.size(); i++) {
				varSend.SetChildItem(Format("PARAM_%d", i), vecParamLD[i].iParam);
				varSend.SetChildItem(Format("NAME_%d", i), vecParamLD[i].Name);
				varSend.SetChildItem(Format("VALUE_%d", i), vecParamLD[i].Value);
			}

			SendIPStepCommand("CIMTransfer", lpEQP[eEQP_SCT], cmd.GetSession(), cmd.GetCommand(), varSend);
			break;
		}
		case S_END: {
			SEQ_STATUS = eSEQ_COMPLETED;
		}
	}

	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIP_GET_RECIPE_PARAM(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varResult = cmd.m_varResult;

	int iPPID = var("PPID_NO", -1);

	if (iPPID < 0)
		return FALSE;

	auto vecParamLD = GetRecipeParam(iPPID);
	if (vecParamLD.size() == 0)
		return FALSE;

	varResult.SetChildItem("RecipeName", CStringA(g_recipes[iPPID]->recipeName));	// KES 추가 2024.07.25 - 레시피명도 같이 보냄
	varResult.SetChildItemT("PARAMs", vecParamLD);
	for (int i = 0; i < vecParamLD.size(); i++) {
		varResult.SetChildItem(Format("PARAM_%d", i), vecParamLD[i].iParam);
		varResult.SetChildItem(Format("NAME_%d", i), vecParamLD[i].Name);
		varResult.SetChildItem(Format("VALUE_%d", i), vecParamLD[i].Value);
	}

	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIP_ALARM_REPORT_SEND(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;

	enum {
		S_INIT,
		S_SEND,
		S_END,
	};

	T_SYSTEM_ALARM_ITEM alarm;
	var.GetChildItemT<T_SYSTEM_ALARM_ITEM>("T_SYSTEM_ALARM_ITEM", alarm);
	auto& AlarmReport = g_mapAlarmReport[{ cmd.GetCommand(), alarm.iALID}];

	if (!cmd.IsChildOK() || !cmd.IsOK()) {
		AlarmReport = eSEQ_ALARM;

		return FALSE;
	}
	
	CStringA strCommand(cmd.GetCommand());
	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			AlarmReport = eSEQ_START;
			cmd.EnqueueNextStep();
			break;
		}
		case S_SEND: {
			eEQP_TYPE eEQP = eEQP_SCT;
			CIM::ALARM::ALARM_REPORT(eEQP, alarm, &cmd); // 보내고 완료 확인을 위해 SendIPStepCommand로 

			cmd.EnqueueNextStep();
			break;
		}
		case S_END: {
			AlarmReport = eSEQ_COMPLETED;
			break;
		}
	}
	
	return TRUE;
}

BOOL CCIMIPCommandWnd::OnIP_RECIPE_CIM_CONTROL(CIPCommand& cmd) {
	const CIPVar& var = cmd.m_var;
	CIPVar& varResult = cmd.m_varResult;

	enum {
		S_INIT,
		S_CONTROL,
		S_END,
	};

	int iPPID = var("PPID_NO", -1);
	CStringA strPPID = var("PPID", "");
	int iCODE  = var("CODE", -1);

	if (iPPID < 0 || iCODE < 0 || strPPID.IsEmpty())
		return FALSE;

	switch (cmd.GetCurrentStep()) {
		case S_INIT: {
			cmd.EnqueueNextStep();
			break;
		}
		case S_CONTROL: {
			switch (iCODE) {
				case ePPID_MODE_CREATION_OF_PPID :
				case ePPID_MODE_MODIFICATION_OF_PPID: {
					if (iCODE == ePPID_MODE_CREATION_OF_PPID) {
						if (g_recipes.GetRecipeData(iPPID - 1) == nullptr)
							g_recipes.CreateRecipe(iPPID - 1);
					}

					SetRecipeData(iPPID - 1);
					break;
				}
				case ePPID_MODE_DELETION_OF_PPID: {
					g_recipes.deleteRecipe(iPPID - 1);
					break;
				}
			}
			cmd.EnqueueNextStep();
			break;
		}
		case S_END: {
			break;
		}
	}

	return TRUE;
}
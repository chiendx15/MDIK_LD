#pragma once


// CCIMIPCommandWnd

class CCIMIPCommandWnd : public CWnd, public IWNDIPCommandTarget
{
	DECLARE_DYNAMIC(CCIMIPCommandWnd)

public:
	CCIMIPCommandWnd();
	virtual ~CCIMIPCommandWnd();

protected:
	DECLARE_MESSAGE_MAP()

private:
	DECLARE_IP_AND_INTERFACE()
	//<< 2024.04.11 LBH - 
	// SendMessage
	ip_handler BOOL OnIP_CIM_POST_IP_COMMADN(CIPCommand& cmd);
	ip_handler BOOL OnIP_CIM_SEND_IP_STEP_COMMAND(CIPCommand& cmd);

	// RecvMessage
	ip_handler BOOL OnIP_S2F31_DATE_TIME_SET(CIPCommand& cmd);	// 시간 동기화
	ip_handler BOOL OnIP_S10F3_TERMINAL_DISPLAY_SINGLE(CIPCommand& cmd);
	ip_handler BOOL OnIP_RECV_INTERLOCK(CIPCommand& cmd); // RCMD 12, 13 : Loading Stop, StepStop
	ip_handler BOOL OnIP_RECV_INTERLOCK_CONFIRM(CIPCommand& cmd); // RCMD 12, 13 : Loading Stop, StepStop
	//>>

	// 2024.06.28 LBH
	ip_handler BOOL OnIP_CIM_Disconnect(CIPCommand& cmd); // RCMD 12, 13 : Loading Stop, StepStop
	//

	// 2021.03.29 AutoRun 시작할 때 제어 PC <Recipe Index, PPID> 체크하기 위한 함수
	ip_handler BOOL OnIP_GetRecipeInfo(CIPCommand& cmd);

	ip_handler BOOL OnIPPMModeChange(CIPCommand& cmd);
	ip_handler BOOL OnIPPMDoorLock(CIPCommand& cmd);

	ip_handler BOOL OnIP_SetDoorRelease(CIPCommand& cmd);
	ip_handler BOOL OnIPShowPM(CIPCommand& cmd);

	ip_handler BOOL OnIP_CimReportChange(CIPCommand& cmd);
	ip_handler BOOL OnIP_RecvGlassID(CIPCommand& cmd);

	// 2021.12.10 레시피 변경
	ip_handler BOOL OnIP_RecipeChange(CIPCommand& cmd);

	//<< 2024.07.02 LBH - EQP Status 관련
	ip_handler BOOL OnIP_MANUAL_STOP(CIPCommand& cmd);
	ip_handler BOOL OnIP_CYCLE_STOP(CIPCommand& cmd);
	ip_handler BOOL OnIP_INTERLOCK_RELEASE(CIPCommand& cmd);
	
	ip_handler BOOL OnIP_AUTO_RUN(CIPCommand& cmd);
	//>>

	//<< 2024.07.03 LBH - RECIPE 관련
	ip_handler BOOL OnIP_SAVE_AND_APPLY_RECIPE(CIPCommand& cmd);
	ip_handler BOOL OnIP_GET_RECIPE_PARAM(CIPCommand& cmd);
	ip_handler BOOL OnIP_RECIPE_CIM_CONTROL(CIPCommand& cmd);
	//>>

	ip_handler BOOL OnIP_ALARM_REPORT_SEND(CIPCommand& cmd);
};



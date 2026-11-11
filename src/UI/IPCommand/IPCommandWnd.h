#pragma once


// CIPCommandWnd

class CIPCommandWnd : public CWnd, public IWNDIPCommandTarget
{
	DECLARE_DYNAMIC(CIPCommandWnd)
public:
	CIPCommandWnd();
	virtual ~CIPCommandWnd();

protected:
	DECLARE_MESSAGE_MAP()

private:
	DECLARE_IP_AND_INTERFACE()
	//<< 2024.04.11 LBH - 
	ip_handler BOOL OnIP_SEND_MESSAGE(CIPCommand& cmd);
	ip_handler BOOL OnIP_POST_MESSAGE(CIPCommand& cmd);

	ip_handler BOOL OnIP_S6F11_INTERLOCK_CONFIRM(CIPCommand& cmd);
	ip_handler BOOL OnIP_S6F11_OPCALL_CONFIRM(CIPCommand& cmd);
	ip_handler BOOL OnIP_READ_PPBOX_LOT_DATA(CIPCommand& cmd);
	ip_handler BOOL OnIP_DELETE_PPBOX_LOT_DATA(CIPCommand& cmd);
	//>>


	ip_handler BOOL OnIP_RecvRecipe(CIPCommand& cmd);
	ip_handler BOOL OnIP_SCTIOOnOff(CIPCommand& cmd);
	ip_handler BOOL OnIP_HSMSTransfer(CIPCommand& cmd);
	// Systemalarm -> CIMTransfer (Cell Data Request)
	ip_handler BOOL OnIP_CimReportChange(CIPCommand& cmd);

	// KES 추가 2024.4.7
	ip_handler BOOL OnIP_ShiftTableVacuum(CIPCommand& cmd);

	//<< MCC 통신용 IPCommand
	ip_handler BOOL OnIP_MCCGTComponentIn(CIPCommand& cmd);
	ip_handler BOOL OnIP_MCCGTVacOn(CIPCommand& cmd);
	//>>


	//<< 2024.07.02 LBH - EQP Status 관련
	ip_handler BOOL OnIP_AUTO_RUN(CIPCommand& cmd);
	//>>

	//<< 2024.07.03 LBH - RECIPE 관련
	ip_handler BOOL OnIP_SAVE_AND_APPLY_RECIPE(CIPCommand& cmd);
	//>>

	ip_handler BOOL OnIP_ALARM_REPORT_SEND(CIPCommand& cmd);
};




#include "pch.h"
#include "TorqueClient.h"

void CTorqueClient::Connect(const CString& _ip, const unsigned short _port, int pc_no)
{
	m_pc_no = pc_no;

	// ¿¬°á
	auto func = [this](CAsioCommand& _cmd)
	{
		return Run(_cmd);
	};
	m_client.Connect(std::string(CStringA(_ip).GetBuffer()), _port, std::make_shared<spt::param>(func));
	m_lastSendMessageTime = GetTickCount64();
}

spt::result CTorqueClient::Run(CAsioCommand& _cmd)
{
	auto result = spt::success;
	auto pService = _cmd.GetService();

	result = pService->Write(&m_pc_no, sizeof(m_pc_no));
	if (result != spt::success)
		return spt::aborted;


	//<<
	result = pService->Write(&sm.MotorTorque->torque, sizeof(sm.MotorTorque->torque));
	if (result != spt::success)
		return spt::aborted;
	//>>

	BOOL bReadComplete = FALSE;
	result = pService->Read(&bReadComplete, sizeof(bReadComplete));
	if (result != spt::success)
		return spt::aborted;

	std::this_thread::sleep_for(10ms);
	m_lastSendMessageTime = GetTickCount64();
	return result;
}
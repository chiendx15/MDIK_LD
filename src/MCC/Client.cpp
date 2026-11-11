
#include "pch.h"
#include "Client.h"

void CClient::Connect()
{

	// 포트 오픈
	auto func = [this](CAsioCommand& _cmd)
	{
		return Run(_cmd);
	};

	m_client.Connect(_T("192.168.0.1"), 21000, std::make_shared<spt::param>(func));
	m_lastSendMessageTime = GetTickCount64();
}

spt::result CClient::Run(CAsioCommand& cmd)
{
	auto pService = cmd.GetService();
	auto id = cmd.GetID();

	int pc_index = eMACHINE_LD;
	auto result = pService->AsyncWrite(&pc_index, sizeof(pc_index));
	if (result->get() != spt::success)
		return spt::aborted;

	if (pc_index == -1)
		return spt::aborted;


	result = pService->AsyncWrite(sm.MCC_EQP->szPPID, sizeof(sm.MCC_EQP->szPPID));
	if (result->get() != spt::success)
		return spt::aborted;

	//<< 용량이 크므로 8000바이트 넘지 않게 나눠서 쓴다
	result = pService->AsyncWrite(&sm.MCC_EQP->Layer[0], sizeof(T_MCC_LAYER) * 20);
	if (result->get() != spt::success)
		return spt::aborted;

	result = pService->AsyncWrite(&sm.MCC_EQP->Layer[20], sizeof(T_MCC_LAYER) * 20);
	if (result->get() != spt::success)
		return spt::aborted;
	//>>



	BOOL bReadComplete = FALSE;
	result = pService->AsyncRead(&bReadComplete, sizeof(bReadComplete));
	if (result->get() != spt::success)
		return spt::aborted;


	m_lastSendMessageTime = GetTickCount64();
	std::this_thread::sleep_for(10ms);
	return spt::success;
}
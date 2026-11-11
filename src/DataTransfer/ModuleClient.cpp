#include "pch.h"
#include "ModuleClient.h"

void ModuleClient::Connect(const CString& _ip, const unsigned short _port)
{
	// 연결
	auto func = [this](CAsioCommand& _cmd)
	{
		return Run(_cmd);
	};
	m_client.Connect(std::string(CStringA(_ip).GetBuffer()), _port, std::make_shared<spt::param>(func));
	m_lastSendMessageTime = GetTickCount64();
}


spt::result ModuleClient::Run(CAsioCommand& _cmd)
{
	auto pService = _cmd.GetService();

	DWORD isMaterial = CheckIsMaterialInEQP();
	DWORD isMaterialUnit1 = CheckIsMaterialInUNIT(0);
	DWORD isMaterialUnit2 = CheckIsMaterialInUNIT(1);

	// PC 상태 보내기
	// PC Number, 보내는 모듈 개수, 모듈 사이즈, In 신호 사이즈, Out 신호 사이즈, 오토 모드, 일시 정지, 알람 상태, 자재유무, 온라인
	std::array<DWORD, DATA_PACKET_COUNT> status;
	
	//<< 2024.06.14 3개 추가됨
	if (m_pcNumber < 6)
	{
		status = { m_pcNumber, (DWORD)m_sendModules.size(), (DWORD)(sizeof(T_SEQUENCE_MODULE_INFO)) , (DWORD)m_In.second, (DWORD)m_Out.second
			, *m_Status[0], *m_Status[1], *m_Status[2], *m_Status[3], *m_Status[4],* m_Status[5], *m_Status[6], *m_Status[7]
			, isMaterial, (DWORD)(!sm.Sequence->bCIMSkip), isMaterialUnit1, isMaterialUnit2 };
						// SCT, CCT
						//, (DWORD)sm.DeviceParameter->System.Mode.bCIMReport };
						// ULD
						//, (DWORD)sm.DeviceParameter->Use.bOnlineUse };
	}
	else
	{
		status = { m_pcNumber, 0, (DWORD)(sizeof(T_SEQUENCE_MODULE_INFO)), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	}
	//>>

	auto result = pService->AsyncWrite(status.data(), sizeof(DWORD) * status.size());
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	// 혹시 세팅 안된 상태로 가면 위험해서
	for (size_t idx = 0; idx < nMODULEs; ++idx)
	{
		sm.Module[idx].eModule = (eMODULEs)idx; 
	}

	if (m_pcNumber != 6) // CIM PC
	{
		// 모듈 데이터 보내기 (반복)
		for (size_t iModule = 0; iModule < m_sendModules.size(); ++iModule)
		{
			result = pService->AsyncWrite(m_sendModules[iModule], sizeof(*m_sendModules[iModule]));
			if (result->get() != spt::success)
			{
				return spt::aborted;
			}
		}

		// IO 보내기
		result = pService->AsyncWrite(m_In.first, m_In.second);
		if (result->get() != spt::success)
		{
			return spt::aborted;
		}
		result = pService->AsyncWrite(m_Out.first, m_Out.second);
		if (result->get() != spt::success)
		{
			return spt::aborted;
		}
	}

	//<< 2024.04.19 kms - cim data 보내기 (cim pc에서만 데이터를 보낸다)
	if (m_pcNumber == nMACHINE_END) { // nMACHINE_END이 cim pc번호다
		result = pService->AsyncWrite(&sm.Sequence->CIMData, sizeof(sm.Sequence->CIMData));
		if (result->get() != spt::success)
		{
			return spt::aborted;
		}
	}
	//>>

//---------------------------------------------------- 위는 보내기 아래는 받기
	// IO 데이터 받기

	result = pService->AsyncRead(&sm.IOs->LD.In, sizeof(sm.IOs->LD.In));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncRead(&sm.IOs->LD.Out, sizeof(sm.IOs->LD.Out));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncRead(&sm.IOs->SCT.In, sizeof(sm.IOs->SCT.In));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncRead(&sm.IOs->SCT.Out, sizeof(sm.IOs->SCT.Out));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncRead(&sm.IOs->CCTL.In, sizeof(sm.IOs->CCTL.In));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncRead(&sm.IOs->CCTL.Out, sizeof(sm.IOs->CCTL.Out));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncRead(&sm.IOs->CCTR.In, sizeof(sm.IOs->CCTR.In));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncRead(&sm.IOs->CCTR.Out, sizeof(sm.IOs->CCTR.Out));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncRead(&sm.IOs->UNDL.In, sizeof(sm.IOs->UNDL.In));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncRead(&sm.IOs->UNDL.Out, sizeof(sm.IOs->UNDL.Out));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncRead(&sm.IOs->UNDR.In, sizeof(sm.IOs->UNDR.In));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncRead(&sm.IOs->UNDR.Out, sizeof(sm.IOs->UNDR.Out));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}
	

	// PC 상태 받기
	result = pService->AsyncRead(sm.PCs, sizeof(*sm.PCs));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	// 받을 모듈 개수
	int nModule = 0;
	result = pService->AsyncRead(&nModule, sizeof(nModule));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}
	
	if (m_recvModules.size() != nModule)
	{
		m_recvModules.resize(nModule);
	}

	// 모듈 데이터 받기
	for (int i = 0; i < nModule; ++i) {
		result = pService->AsyncRead(&m_recvModules[i], sizeof(T_SEQUENCE_MODULE_INFO));
		if (result->get() != spt::success)
		{
			return spt::aborted;
		}
	}

	// 받은 데이터에서 현재 보낸 모듈 데이터는 제외하고 적용
	for (size_t iModule = 0; iModule < m_recvModules.size(); ++iModule)
	{
		const auto& module = m_recvModules[iModule];
		
		if (m_beginModule > module.eModule || m_endModule <= module.eModule)
		{
			memcpy(&sm.Module[module.eModule], &module, sizeof(T_SEQUENCE_MODULE_INFO));
		}
	}


	// 2024.04.19 kms - cim data받기 (cim pc가 아닌 곳만 데이터를 받는다)
	if (m_pcNumber != nMACHINE_END) { // nMACHINE_END이 cim pc번호다

		result = pService->AsyncRead(&sm.Sequence->CIMData, sizeof(sm.Sequence->CIMData));
		if (result->get() != spt::success)
		{
			return spt::aborted;
		}
	}

	m_lastSendMessageTime = GetTickCount64();
	std::this_thread::sleep_for(100ms);
	return spt::success;
}
void ModuleClient::SetModuleAddress(eMODULEs _beginModule, eMODULEs _endModule, const std::vector<T_SEQUENCE_MODULE_INFO*>& _modules)
{
	m_beginModule = _beginModule;
	m_endModule = _endModule;

	m_sendModules.clear();
	m_sendModules = _modules;
}

void ModuleClient::SetPCStatus(DWORD _pcIndex, DWORD* _AutoMode, DWORD* _CycleStop, DWORD* _IsAlarm, DWORD* _PMMode, DWORD* _CIM_StepStop, DWORD* _CIM_LoadingStop, DWORD* _CIM_UNIT_LoadingStop1, DWORD* _CIM_UNIT_LoadingStop2)
{
	// PC 상태 세팅
	m_pcNumber = _pcIndex;
	assert(m_pcNumber <= 6);

	m_Status.clear();
	m_Status.push_back(_AutoMode);
	m_Status.push_back(_CycleStop);
	m_Status.push_back(_IsAlarm);
	m_Status.push_back(_PMMode);

	//<< 2024.06.14 추가됨
	m_Status.push_back(_CIM_StepStop);
	m_Status.push_back(_CIM_LoadingStop);
	m_Status.push_back(_CIM_UNIT_LoadingStop1);
	m_Status.push_back(_CIM_UNIT_LoadingStop2);
	//>>
}

void ModuleClient::SetIOAddress(DWORD* _In, size_t _InSize, DWORD* _Out, size_t _OutSize)
{
	// In 신호 세팅
	m_In.first = _In;
	m_In.second = _InSize;

	// Out 신호 세팅
	m_Out.first = _Out;
	m_Out.second = _OutSize;
}


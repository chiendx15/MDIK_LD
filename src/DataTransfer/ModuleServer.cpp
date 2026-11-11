#include "pch.h"
#include "ModuleServer.h"

void ModuleServer::Open(const unsigned short _port, const short _max_connector)
{
	// 클라이언트가 보내주는 모듈 데이터 임시 저장 공간
	// 함수 내부 선언시 사이즈가 커서 스택 넘침
	m_data.reset(new T_SEQUENCE_MODULE_INFO[_max_connector] {});
	
	// 포트 오픈
	auto func = [this](CAsioCommand& _cmd)
	{
		return Run(_cmd);
	};
	m_server.Open(_port, _max_connector, _max_connector, std::make_shared<spt::param>(func));

	// LD
	m_recvModules.insert({ eMODULE_PC1_GLASS_TRANSFER, sm.Module[eMODULE_PC1_GLASS_TRANSFER] });

	// SCT
	m_recvModules.insert({ eMODULE_PC2_GLASS_SHIFT_TABLE, sm.Module[eMODULE_PC2_GLASS_SHIFT_TABLE] });
	m_recvModules.insert({ eMODULE_PC2_STICK_TRANSFER_1, sm.Module[eMODULE_PC2_STICK_TRANSFER_1] });
	m_recvModules.insert({ eMODULE_PC2_STICK_TRANSFER_2, sm.Module[eMODULE_PC2_STICK_TRANSFER_2] });

	// CCT L/R
	m_recvModules.insert({ eMODULE_PC3_STICK_LINESCAN_TABLE, sm.Module[eMODULE_PC3_STICK_LINESCAN_TABLE] });
	m_recvModules.insert({ eMODULE_PC3_CELL_BREAK_TABLE_1, sm.Module[eMODULE_PC3_CELL_BREAK_TABLE_1] });
	m_recvModules.insert({ eMODULE_PC3_CELL_BREAK_TABLE_2, sm.Module[eMODULE_PC3_CELL_BREAK_TABLE_2] });
	m_recvModules.insert({ eMODULE_PC4_STICK_LINESCAN_TABLE, sm.Module[eMODULE_PC4_STICK_LINESCAN_TABLE] });
	m_recvModules.insert({ eMODULE_PC4_CELL_BREAK_TABLE_1, sm.Module[eMODULE_PC4_CELL_BREAK_TABLE_1] });
	m_recvModules.insert({ eMODULE_PC4_CELL_BREAK_TABLE_2, sm.Module[eMODULE_PC4_CELL_BREAK_TABLE_2] });

	// ULD L/R
	m_recvModules.insert({ eMODULE_PC5_CELL_TRANSFER_1, sm.Module[eMODULE_PC5_CELL_TRANSFER_1] });
	m_recvModules.insert({ eMODULE_PC5_CELL_TRANSFER_2, sm.Module[eMODULE_PC5_CELL_TRANSFER_2] });
	m_recvModules.insert({ eMODULE_PC6_CELL_TRANSFER_1, sm.Module[eMODULE_PC6_CELL_TRANSFER_1] });
	m_recvModules.insert({ eMODULE_PC6_CELL_TRANSFER_2, sm.Module[eMODULE_PC6_CELL_TRANSFER_2] });
}

size_t ModuleServer::GetClientCount() const
{
	return m_server.GetServices().size();
}

std::list<CString> ModuleServer::GetClientIPs() const
{
	const auto& services = m_server.GetServices();
	std::list<CString> IPs;
	for (auto service : services)
	{
		CString ip;
		service->GetIPAddress(ip);
		IPs.push_back(ip);
	}
	return IPs;
}

spt::result ModuleServer::Run(CAsioCommand& _cmd)
{
	auto pService = _cmd.GetService();

	// PC 상태 받기
	// PC Number, 보내는 모듈 개수, 모듈 사이즈, In 신호 사이즈, Out 신호 사이즈, 오토 모드, 일시 정지, 알람 상태, 자재유무, 온라인
	std::array<DWORD, DATA_PACKET_COUNT> status = { 0, };
	auto result = pService->AsyncRead(status.data(), sizeof(DWORD) * status.size());
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	DWORD pcIndex = status[0];
	// 데이터가 안맞으면 실패시킴
	assert(pcIndex <= 6); // PC number error

	// 모듈 데이터 받기 (개 수 만큼)
	size_t module_count = status[1];
	auto& data = m_data.get()[_cmd.GetID()];
	
	if (sizeof(T_SEQUENCE_MODULE_INFO) != status[2]) // 모듈 구조체 사이즈 체크
	{
		return spt::aborted;
	}

	for (size_t idx = 0; idx < module_count; ++idx)
	{
		result = pService->AsyncRead(&data, sizeof(T_SEQUENCE_MODULE_INFO));
		if (result->get() != spt::success)
		{
			return spt::aborted;
		}

		auto iter = m_recvModules.find(data.eModule);
		if (iter == m_recvModules.end())
		{
			return spt::aborted;
		}
		memcpy(&(iter->second), &data, sizeof(iter->second));
	}
	
	// IO 받기
	auto InSize = status[3];
	switch (pcIndex)
	{
		case eMACHINE_LD:
		{
			assert(InSize == sizeof(sm.IOs->LD.In));
			result = pService->AsyncRead(&(sm.IOs->LD.In), sizeof(sm.IOs->LD.In));
		}
		break;

		case eMACHINE_SCT:
		{
			assert(InSize == sizeof(sm.IOs->SCT.In));
			result = pService->AsyncRead(&(sm.IOs->SCT.In), sizeof(sm.IOs->SCT.In));
		}
		break;

		case eMACHINE_CCTL:
		{
			assert(InSize == sizeof(sm.IOs->CCTL.In));
			result = pService->AsyncRead(&(sm.IOs->CCTL.In), sizeof(sm.IOs->CCTL.In));
		}
		break;

		case eMACHINE_CCTR:
		{
			assert(InSize == sizeof(sm.IOs->CCTR.In));
			result = pService->AsyncRead(&(sm.IOs->CCTR.In), sizeof(sm.IOs->CCTR.In));
		}
		break;

		case eMACHINE_UNDL:
		{
			assert(InSize == sizeof(sm.IOs->UNDL.In));
			result = pService->AsyncRead(&(sm.IOs->UNDL.In), sizeof(sm.IOs->UNDL.In));
		}
		break;

		case eMACHINE_UNDR:
		{
			assert(InSize == sizeof(sm.IOs->UNDR.In));
			result = pService->AsyncRead(&(sm.IOs->UNDR.In), sizeof(sm.IOs->UNDR.In));
		}
		break;
	}

	if (pcIndex != 6 && result->get() != spt::success)
	{
		return spt::aborted;
	}

	auto OutSize = status[4];
	switch (pcIndex)
	{
		case eMACHINE_LD:
		{
			assert(OutSize == sizeof(sm.IOs->LD.Out));
			result = pService->AsyncRead(&(sm.IOs->LD.Out), sizeof(sm.IOs->LD.Out));
		}
		break;

		case eMACHINE_SCT:
		{
			assert(OutSize == sizeof(sm.IOs->SCT.Out));
			result = pService->AsyncRead(&(sm.IOs->SCT.Out), sizeof(sm.IOs->SCT.Out));
		}
		break;

		case eMACHINE_CCTL:
		{
			assert(OutSize == sizeof(sm.IOs->CCTL.Out));
			result = pService->AsyncRead(&(sm.IOs->CCTL.Out), sizeof(sm.IOs->CCTL.Out));
		}
		break;

		case eMACHINE_CCTR:
		{
			assert(OutSize == sizeof(sm.IOs->CCTR.Out));
			result = pService->AsyncRead(&(sm.IOs->CCTR.Out), sizeof(sm.IOs->CCTR.Out));
		}
		break;

		case eMACHINE_UNDL:
		{
			assert(OutSize == sizeof(sm.IOs->UNDL.Out));
			result = pService->AsyncRead(&(sm.IOs->UNDL.Out), sizeof(sm.IOs->UNDL.Out));
		}
		break;

		case eMACHINE_UNDR:
		{
			assert(OutSize == sizeof(sm.IOs->UNDR.Out));
			result = pService->AsyncRead(&(sm.IOs->UNDR.Out), sizeof(sm.IOs->UNDR.Out));
		}
		break;
	}

	if (pcIndex != 6 && result->get() != spt::success)
	{
		return spt::aborted;
	}

	// PC 상태 세팅
	if (pcIndex != 6) // CIM PC는 세팅 안함
	{
		sm.PCs->ControlPC[pcIndex].fAutoMode = status[5];
		sm.PCs->ControlPC[pcIndex].fCycleStop = status[6];
		sm.PCs->ControlPC[pcIndex].fIsAlarm = status[7];
		sm.PCs->ControlPC[pcIndex].fPMMode = status[8];
		sm.PCs->ControlPC[pcIndex].fEQP_StepStop = status[9];
		sm.PCs->ControlPC[pcIndex].fEQP_LoadingStop = status[10];
		sm.PCs->ControlPC[pcIndex].fUNIT_LoadingStop[0] = status[11];
		sm.PCs->ControlPC[pcIndex].fUNIT_LoadingStop[1] = status[12];
		sm.PCs->ControlPC[pcIndex].fIsMaterial = status[13];
		sm.PCs->ControlPC[pcIndex].fOnlineMode = status[14];
		sm.PCs->ControlPC[pcIndex].fIsUnitMaterial[0] = status[15];
		sm.PCs->ControlPC[pcIndex].fIsUnitMaterial[1] = status[16];
		
	}

	//<< 2024.04.19 kms - cim data 받기 (cim pc에서 보내준 것만 읽어 들인다)
	if (pcIndex == nMACHINE_END) { // nMACHINE_END이 cim pc번호다
		result = pService->AsyncRead(&sm.Sequence->CIMData, sizeof(sm.Sequence->CIMData));
		if (result->get() != spt::success)
		{
			return spt::aborted;
		}
	}
	//>>

	

//---------------------------------------------------- 위는 받기 아래는 보내기

	// IO 데이터 보내기
	result = pService->AsyncWrite(&sm.IOs->LD.In, sizeof(sm.IOs->LD.In));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncWrite(&sm.IOs->LD.Out, sizeof(sm.IOs->LD.Out));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncWrite(&sm.IOs->SCT.In, sizeof(sm.IOs->SCT.In));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncWrite(&sm.IOs->SCT.Out, sizeof(sm.IOs->SCT.Out));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncWrite(&sm.IOs->CCTL.In, sizeof(sm.IOs->CCTL.In));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncWrite(&sm.IOs->CCTL.Out, sizeof(sm.IOs->CCTL.Out));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncWrite(&sm.IOs->CCTR.In, sizeof(sm.IOs->CCTR.In));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncWrite(&sm.IOs->CCTR.Out, sizeof(sm.IOs->CCTR.Out));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncWrite(&sm.IOs->UNDL.In, sizeof(sm.IOs->UNDL.In));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncWrite(&sm.IOs->UNDL.Out, sizeof(sm.IOs->UNDL.Out));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncWrite(&sm.IOs->UNDR.In, sizeof(sm.IOs->UNDR.In));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	result = pService->AsyncWrite(&sm.IOs->UNDR.Out, sizeof(sm.IOs->UNDR.Out));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	// PC 상태 보내기
	result = pService->AsyncWrite(sm.PCs, sizeof(*sm.PCs));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	// 모든 모듈 데이터 전달
	int nModule = (int)m_recvModules.size();
	result = pService->AsyncWrite(&nModule, sizeof(nModule));
	if (result->get() != spt::success)
	{
		return spt::aborted;
	}

	for (auto& [eModule, item] : m_recvModules)
	{
		result = pService->AsyncWrite(&item, sizeof(item));
		if (result->get() != spt::success)
		{
			return spt::aborted;
		}
	}

	// 2024.04.19 kms - cim data보내기 (cim pc빼고 다 보낸다)
	if (pcIndex != nMACHINE_END) { // nMACHINE_END이 cim pc번호다
		result = pService->AsyncWrite(&sm.Sequence->CIMData, sizeof(sm.Sequence->CIMData));
		if (result->get() != spt::success)
		{
			return spt::aborted;
		}
	}

	// 네트워크 사용량 감소를 위해
	std::this_thread::sleep_for(100ms);
	return spt::success;
}




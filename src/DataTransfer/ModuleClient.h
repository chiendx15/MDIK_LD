#pragma once

#include "misc/asio.h"

class ModuleClient
{
public:
	void Connect(const CString& _ip, const unsigned short _port);
	
	// 연결이 끊긴지 확인 (끊겼으면 Connect로 재연결할 것)
	bool IsConnected() const
	{
		return m_client.IsConnected();
	}

	void Close()
	{
		m_client.Close();
	}

	size_t GetLastMessagTime() const
	{
		return m_lastSendMessageTime;
	}

private:
	spt::result Run(CAsioCommand& _cmd);
	
private:
	CAsioClient m_client;

	DWORD m_pcNumber = 999;
	std::vector<DWORD*> m_Status;
	std::pair<DWORD*, size_t> m_In;
	std::pair<DWORD*, size_t> m_Out;

	eMODULEs m_beginModule	= eMODULE_NONE;
	eMODULEs m_endModule	= eMODULE_NONE;
	
	std::vector<T_SEQUENCE_MODULE_INFO*> m_sendModules;
	std::vector<T_SEQUENCE_MODULE_INFO> m_recvModules;

	size_t m_lastSendMessageTime;
public:
	void SetModuleAddress(eMODULEs _beginModule, eMODULEs _endModule, const std::vector<T_SEQUENCE_MODULE_INFO*>& _modules);
	//<< 2024.06.14 인자 3개 추가됨
	void SetPCStatus(DWORD _pcIndex, DWORD* _AutoMode, DWORD* _CycleStop, DWORD* _IsAlarm, DWORD* _PMMode, DWORD* _CIM_StepStop, DWORD* _CIM_LoadingStop, DWORD* _CIM_UNIT_LoadingStop1, DWORD* _CIM_UNIT_LoadingStop2);
	//>>
	void SetIOAddress(DWORD* _In, size_t _InSize, DWORD* _Out, size_t _OutSize);
};


#pragma once
#include "misc/asio.h"

class ModuleServer
{
public:
	void Open(const unsigned short _port, const short _max_connector = 10);
	size_t GetClientCount() const;
	std::list<CString> GetClientIPs() const;
	std::map<eMODULEs, T_SEQUENCE_MODULE_INFO> m_recvModules;
	
private:
	spt::result Run(CAsioCommand& _cmd);

private:
	CAsioServer m_server;
	std::unique_ptr<T_SEQUENCE_MODULE_INFO[]> m_data = nullptr;
};


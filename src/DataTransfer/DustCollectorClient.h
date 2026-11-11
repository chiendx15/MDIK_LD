
#pragma once
#include "misc/asio.h"



class CDustCollectorClient
{
public:
	bool IsConnected() const
	{
		return m_client.IsConnected();
	}

	void Close()
	{
		m_client.Close();
	}

	void Connect(const CString& _ip, const unsigned short _port, int pc_no);

	size_t GetLastMessagTime() const
	{
		return m_lastSendMessageTime;
	}

protected:
	spt::result Run(CAsioCommand& _cmd);

protected:
	CAsioClient	m_client;
	int m_pc_no = eMACHINE_UNDL;

	size_t m_lastSendMessageTime;
};


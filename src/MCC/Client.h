
#pragma once
#include "misc/asio.h"

class CClient {
	
public:
	void Connect();
	BOOL IsConnected() {
		return m_client.IsConnected();
	}

	void Close()
	{
		m_client.Close();
	}

	ULONGLONG GetLastMessagTime() const
	{
		return m_lastSendMessageTime;
	}

private:
	spt::result Run(CAsioCommand& cmd);

private:
	CAsioClient		m_client;

	ULONGLONG		m_lastSendMessageTime;

};
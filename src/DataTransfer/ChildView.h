
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once

#include "ModuleClient.h"
#include "ModuleServer.h"
#include "TorqueClient.h"
#include "DustCollectorClient.h"
#include "FDCClient.h"

// CChildView 창

class CChildView : public CWnd
{
public:
	CChildView();
	virtual ~CChildView();

private:
	ModuleClient m_client;
	ModuleServer m_server;

	CTorqueClient			m_torque_client;
	CDustCollectorClient	m_dust_collector_client;
	CFDCClient				m_fdc_client;

private:
	void ClientConnect();
	void TorqueClientConnect();
	void DustCollectorClientConnect();
	void FDCClientConnect();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


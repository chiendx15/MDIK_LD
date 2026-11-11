
// Serial.h : main header file for the Serial application
//
#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "MIP/MIP.h"       


// CSerialApp:
// See Serial.cpp for the implementation of this class
//

class CSerialApp : public CWinApp
{
public:
	CSerialApp() noexcept;


	// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSerialApp theApp;
extern CIPClient g_ipClient;

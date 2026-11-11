
// AutoDelete.h : main header file for the AutoDelete application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Include_Default.hpp"

// CAutoDeleteApp:
// See AutoDelete.cpp for the implementation of this class
//

class CAutoDeleteApp : public CWinApp
{
public:
	CAutoDeleteApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	CIPClient m_ipClient;
};

extern CAutoDeleteApp theApp;


#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

class CMCCApp : public CWinApp
{
public:
	CMCCApp() noexcept;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	DECLARE_MESSAGE_MAP()
};

extern CMCCApp theApp;

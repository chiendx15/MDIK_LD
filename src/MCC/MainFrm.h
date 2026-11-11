

#pragma once
#include "Client.h"

class CMainFrame : public TIPCT<CFrameWndEx>
{
	
public:
	CMainFrame() noexcept;
	virtual ~CMainFrame();

protected: 
	DECLARE_DYNAMIC(CMainFrame)

protected:
	CClient		m_client;

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

	DECLARE_IP()

};



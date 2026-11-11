#pragma once
#include "ChildView.h"

class CInverter;
class CPPBoxCodeReaderWnd;
class CDustCollectorPressure;
class CDustCollectorControlPanel;

class CMainFrame : public TIPCT<CFrameWnd>
{
	enum eAREA {
		eWORK
		, eMENU
		, eA_LIST
	};

	enum eSTATIC {

		eS_SEM
		, eS_LIST
	};

	enum eCONTROL {
		eC_NONE = -1
		, eC_INVERTER
		, eC_SEM_START
		, eC_SEM_STOP

		, eC_PPBOX_CODE_READER
		, eC_MCR

		, eC_LABEL_PRINTER_OPEN

		, eC_TRAY_POCEKT_COMM
		, eC_PRESSURE_SENSOR_READER
		
		, eC_CLOSE

		, eC_LIST
	};
public:

	CMainFrame();
	virtual ~CMainFrame();

	// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);
	
	DECLARE_DYNAMIC(CMainFrame)

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	
	DECLARE_IP()
	ip_handler BOOL	OnIPShow(CIPCommand& cmd);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void ComputeArea();

public:

	CRect m_rectA[eA_LIST];
	CRect m_rectS[eS_LIST];
	CRect m_rectC[eC_LIST];

	int m_iSelectedControl;

	enum eUSE_SERIAL_LIST {
		eUSE_INVERTER_1
		, eUSE_INVERTER_2
		, eUSE_INVERTER_3
		, eUSE_INVERTER_4
		, eUSE_INVERTER_5
		, eUSE_PPBOX_CODE_READER
		//eUSE_SEM
		//, eUSE_MCR
		//, eUSE_LABEL_PRINTER
		//, eUSE_TRAY_POCKET
		//, eUSE_PRESSURE_SENSOR_BOARD
		, eUSE_DUST_COLLECTOR_PRESSURE
		, eUSE_DUST_COLLECTOR_CONTROL_PANEL
		, nUSE_SERIAL_LIST
	};

	/// <summary>
	/// SerialUseOption.ini 파일로 프로그램 내에서 사용할지 여부를 결정
	/// 위험할 것 같으면 코드 날리고 PC별로 따로 빌드 해야할수도 있음
	/// </summary>
	BOOL m_bSerialUse[nUSE_SERIAL_LIST];

	//. UI
	//CSEMWnd* m_pWndSEM;
	//CLablePrinterWnd* m_pWndLabelPrinter;
	CInverter* m_pInverter[nCONVEYOR_LD];
	CPPBoxCodeReaderWnd* m_pPPBoxCodeReader;  

	CDustCollectorPressure* m_pDustCollectorPressure[nDUST_COLLECTOR];
	CDustCollectorControlPanel* m_pDustCollectorControlPanel[nDUST_COLLECTOR];
	//CPressureSensorReaderSMCWnd* m_pWndPressure;
	//CTrayPocketWnd* m_pWndTrayPocket;
	//

	//. DATA
	//CSEMReader* m_SEMReader;
	//
	//CTrayPocketServer* m_pTrayPocketServer;
	//
	//PressureSensorReaderSMC* m_pPressureReader;
	//

	
};


// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once
#include "ChildView.h"
class CAxisControlWnd;
class CMotorStatusWnd;
class CRegistryMapWnd;
class CAxisAlarmWnd;
class CModuleAxisStatusWnd;
class CMainFrame : public TIPCT<CFrameWnd>
{
	
public:
	CMainFrame() ;
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CChildView    m_wndView;
	CAxisControlWnd* m_pwndAxisControl;
	CMotorStatusWnd* m_pwndMotorStatus;
	CRegistryMapWnd* m_pwndRegistryMap;
	CAxisAlarmWnd*	 m_pwndAxisAlarm;
	CModuleAxisStatusWnd* m_pwndModuleAxis;
// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

	DECLARE_IP()
	ip_handler BOOL	OnIPIOOnOff(CIPCommand& cmd);
	ip_handler BOOL	OnIPMoveJog(CIPCommand& cmd);
	ip_handler BOOL	OnIPMoveJogStop(CIPCommand& cmd);
	ip_handler BOOL	OnIpMoveStep(CIPCommand& cmd);
	ip_handler BOOL	OnIPMoveStop(CIPCommand& cmd);
	ip_handler BOOL	OnIPHomming(CIPCommand& cmd);
	ip_handler BOOL	OnIPHommingHome(CIPCommand& cmd);
	ip_handler BOOL	OnIPHommingNPSensor(CIPCommand& cmd);
	ip_handler BOOL	OnIPHommingTorque(CIPCommand& cmd);

	ip_handler BOOL	OnIPServoOnOff(CIPCommand& cmd);
	ip_handler BOOL	OnIPAlarmClear(CIPCommand& cmd);
	ip_handler BOOL	OnIPServoAllOnOff(CIPCommand& cmd);
	ip_handler BOOL	OnIPPTPMove(CIPCommand& cmd);
	ip_handler BOOL	OnIPYaskawaInit(CIPCommand& cmd);
	ip_handler BOOL	OnIPSetMotorLimit(CIPCommand& cmd);
	ip_handler BOOL OnIPMoveLiner(CIPCommand& cmd);
	ip_handler BOOL OnIPMoveMulti(CIPCommand& cmd);
	ip_handler BOOL OnIPTorqueMove(CIPCommand& cmd);
	ip_handler BOOL OnIPAverageTorqueMove(CIPCommand& cmd);
	
	ip_handler BOOL OnIPVisionMove(CIPCommand& cmd);
	ip_handler BOOL OnIPPitchMove(CIPCommand& cmd);
};



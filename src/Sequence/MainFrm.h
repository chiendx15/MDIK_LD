
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once
#include "ChildView.h"

class CMainFrame : public CFrameWnd, public IWNDIPCommandTarget
{
	
public:
	CMainFrame() noexcept;
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

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar      m_wndToolBar;
	CChildView    m_wndView;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

private:
	DECLARE_IP_AND_INTERFACE()
	ip_handler BOOL OnIP_ApplyRecipe(CIPCommand& cmd);
	ip_handler BOOL OnIP_SequenceAutoRun(CIPCommand& cmd);
	ip_handler BOOL OnIP_MuteOnOff(CIPCommand& cmd);
	ip_handler BOOL OnIPSequenceBarCodeMessage(CIPCommand& cmd);

	//ip_handler BOOL OnIPManualStop(CIPCommand& cmd);
	//ip_handler BOOL OnIPManualGlassTRLoading(CIPCommand& cmd);
	//ip_handler BOOL OnIPManualGlassTRUnloadingPPBox(CIPCommand& cmd);
	//ip_handler BOOL OnIPManualGlassTRUnloadingTable(CIPCommand& cmd);
	//ip_handler BOOL OnIPManualCoverPickerLoading(CIPCommand& cmd);
	//ip_handler BOOL OnIPManualCoverPickerUnloading(CIPCommand& cmd);

	//ip_handler BOOL OnIPRepeatXMove(CIPCommand& cmd);
	//ip_handler BOOL OnIPRepeatXMoveStop(CIPCommand& cmd);

	//ip_handler BOOL OnIPReturnOrigin(CIPCommand& cmd);		// Main UI Title 원점복귀 버튼

	//ip_handler BOOL OnIPGlassConveyorLoad(CIPCommand& cmd);		// Glass Conveyor Load 동작 
	//ip_handler BOOL OnIPGlassConveyorUnload(CIPCommand& cmd);	// Glass Conveyor Unload 동작
	//ip_handler BOOL OnIPPaperConveyorLoad(CIPCommand& cmd);		// Paper Conveyor Load 동작 
	//ip_handler BOOL OnIPPaperConveyorUnload(CIPCommand& cmd);	// Paper Conveyor Unload 동작  

	//ip_handler BOOL OnIPSequenceBarCodeMessage(CIPCommand& cmd);	// 시퀀스 도중 Barcode 가 비어있는 경우 Message 처리하기 위한 IP 함수 

	//ip_handler BOOL OnIP_ReturnZeroPoint(CIPCommand& cmd);
};



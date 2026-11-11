
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "Sequence.h"
#include "LoaderSeq.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
	: IWNDIPCommandTarget(m_hWnd, &g_ipClient)
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("뷰 창을 만들지 못했습니다.\n");
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 그렇지 않으면 기본 처리합니다.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BEGIN_IP_COMMAND_TABLE(CMainFrame, IIPCommandTarget)
	ON_IP_EXCL("Apply",			"Recipe",				OnIP_ApplyRecipe)
	ON_IP_EXCL("Sequence",		"AutoRun",				OnIP_SequenceAutoRun)
	ON_IP_EXCL("SubSequence",	"Mute",					OnIP_MuteOnOff)
	ON_IP_EXCL("Sequence",		"BarCodeMessage",		OnIPSequenceBarCodeMessage)

	//ON_IP_EXCL("Manual",	"Stop",						OnIPManualStop)
	//ON_IP_EXCL("Manual",	"GlassTRLoading",			OnIPManualGlassTRLoading)
	//ON_IP_EXCL("Manual",	"GlassTRUnloadingPPBox",	OnIPManualGlassTRUnloadingPPBox)
	//ON_IP_EXCL("Manual",	"GlassTRUnloadingTable",	OnIPManualGlassTRUnloadingTable)
	//ON_IP_EXCL("Manual",	"CoverPickerLoading",		OnIPManualCoverPickerLoading)
	//ON_IP_EXCL("Manual",	"CoverPickerUnloading",		OnIPManualCoverPickerUnloading)
	//ON_IP_EXCL("Stage",		"RepeatXMove",				OnIPRepeatXMove)
	//ON_IP_EXCL("Stage",		"RepeatXMoveStop",			OnIPRepeatXMoveStop)
	//ON_IP_EXCL("Manual",	"ReturnOrigin",				OnIPReturnOrigin)		// Main UI Title 원점복귀 버튼
	//ON_IP_EXCL("Manual",	"GlassConveyorLoad",		OnIPGlassConveyorLoad)	// Glass Conveyor Loading 동작 
	//ON_IP_EXCL("Manual",	"GlassConveyorUnload",		OnIPGlassConveyorUnload)
	//ON_IP_EXCL("Manual",	"PaperConveyorLoad",		OnIPPaperConveyorLoad)	
	//ON_IP_EXCL("Manual",	"PaperConveyorUnload",		OnIPPaperConveyorUnload)


	//// 21.03.10 원점복귀
	//ON_IP_EXCL("Return", "ZeroPoint", OnIP_ReturnZeroPoint)

END_IP_COMMAND_TABLE()

// 첫 번째 함수 빨간줄 방지.
void Dummy() {}

ip_handler BOOL CMainFrame::OnIP_ApplyRecipe(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;
	CIPVar& varResult = cmd.m_varResult;
	CIPVar& varCookie = cmd.m_varCookie;
	const CIPVar* pVar = var.FindChild("Recipe");
	if (!cmd.IsOK() || !cmd.IsChildOK()) 
	{
		return FALSE;
	}
	std::vector<BYTE> buf;
	buf.assign((size_t)pVar->GetCount() * (size_t)pVar->GetItemSize(), 0);
	if (buf.size() == 0)
	{
		return FALSE;
	}
	pVar->GetRawData(buf.data(), (UINT)(buf.size()));
	CMemFile f(buf.data(), (UINT)(buf.size()));
	CArchive ar(&f, CArchive::load);
	
	if (g_pRecipe == nullptr)
	{
		g_pRecipe = new T_MDI_RECIPE;
	}
	ar >> *g_pRecipe;
	ar.Close();
	f.Close();
	varResult.SetChildItem("Ack", TRUE);

	strncpy_s(sm.MCC_EQP->szPPID, countof(sm.MCC_EQP->szPPID), CStringA(g_pRecipe->recipeName), countof(sm.MCC_EQP->szPPID) - 1);

	return TRUE;
}
//
//BOOL CMainFrame::OnIPManualGlassTRLoading(CIPCommand& cmd) {
//	const CIPVar& var = cmd.m_var;
//	CIPVar& varResult = cmd.m_varResult;
//	CIPVar& varCookie = cmd.m_varCookie;
//	if (!cmd.IsOK() || !cmd.IsChildOK()) 
//	{
//		return FALSE;
//	}
//	enum {
//		S_INIT,
//		S_INIT_POS,
//		S_MANUAL,
//		S_END,
//	};
//	// S_GUL_SEND_MODULE_SET_GLASS_TRANSFER Lift 시작 지점 
//	// S_GUL_SEND_GLASS_TRANSFER_IO_OFF Lift 종료 시점
//	//BOOL bLinkCheck		= var.GetChildItem("LinkCheck", TRUE);		// bLinkCheck 가 TRUE 인 경우 Upper or Lower 의 Able/Complete/Start 신호를 Check / FALSE 인 경우 Check 이전에 강제로 값을 바꿈 
//	//WORD& wStepTransfer = varCookie.ChildLValue("StepTransfer", WORD(0));
//	//WORD& wStepLift		= varCookie.ChildLValue("StepLift", WORD(0));	
//	WORD& wStepTransfer = sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep;
//	WORD& wStepLift		= sm.Module[eMODULE_GLASS_LIFT_TABLE].Status.wSeqStep;
//	switch (cmd.GetCurrentStep()) {
//		case S_INIT: {
//		SetToLowerInit(eMODULE_GLASS_LIFT_TABLE);
//		SetToUpperInit(eMODULE_PC1_GLASS_TRANSFER);
//		wStepTransfer= S_GT_INIT;
//		cmd.EnqueueNextStep();
//		break;
//		}
//		case S_INIT_POS: {
//			if ((wStepTransfer >= S_GT_INIT) && (wStepTransfer<= S_GT_INIT_X_CHECK))
//			{
//				Glass_Transfer(eMODULE_PC1_GLASS_TRANSFER);
//				cmd.EnqueueNextStepCurrent();
//			}
//			else
//			{
//				wStepTransfer = S_GT_RECV_ABLE;
//				wStepLift = S_GUL_SEND_MODULE_SET_GLASS_TRANSFER;
//				cmd.EnqueueNextStep();
//			}
//			break;
//		}
//		case S_MANUAL: {
//			BOOL bTransfer	= (wStepTransfer >= S_GT_RECV_ABLE) && (wStepTransfer <= S_GT_RECV_SEND_IO_OFF_CHECK);
//			BOOL bLift = (wStepLift >= S_GUL_SEND_MODULE_SET_GLASS_TRANSFER) && (wStepLift <= S_GUL_SEND_GLASS_TRANSFER_IO_OFF);
//			if (bTransfer)		// Send Able 살리고 nMaterial++ 하는 부분까지 단동에 포함
//			{	
//				Glass_Transfer(eMODULE_PC1_GLASS_TRANSFER);
//			}
//			if (bLift)
//			{
//				Conveyor_Glass_Unload_Lift(eMODULE_GLASS_LIFT_TABLE);
//			}
//
//			if (!bLift && !bTransfer)
//			{
//				cmd.EnqueueNextStep();
//			}
//			else {
//				cmd.EnqueueNextStepCurrent();
//			}
//			break;
//		}
//		case S_END: {
//			IPMessage(_T("Glass TR Loading Sequence End"));
//			break;
//		}
//	}
//	return TRUE;
//}
//BOOL CMainFrame::OnIPManualGlassTRUnloadingPPBox(CIPCommand& cmd) {
//	const CIPVar& var = cmd.m_var;
//	CIPVar& varResult = cmd.m_varResult;
//	CIPVar& varCookie = cmd.m_varCookie;
//	if (!cmd.IsOK() || !cmd.IsChildOK())
//	{
//		return FALSE;
//	}
//	enum {
//		S_INIT,
//		//S_INIT_POS,
//		//S_LOAD_PPBOX,
//		S_LOOP,
//		S_END,
//	};
//
//	//WORD& wStepTransfer		= varCookie.ChildLValue("StepTransfer", WORD(0));
//	//WORD& wStepLift			= varCookie.ChildLValue("StepLift", WORD(0));
//	WORD& wStepTransfer		= sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep;
//	WORD& wStepLift			= sm.Module[eMODULE_PPBOX_LIFT_TABLE].Status.wSeqStep;
//	BOOL bLinkCheck = var.GetChildItem("LinkCheck", TRUE);		// bLinkCheck 가 TRUE 인 경우 Upper or Lower 의 Able/Complete/Start 신호를 Check / FALSE 인 경우 Check 이전에 강제로 값을 바꿈 
//	switch (cmd.GetCurrentStep()) {
//		case S_INIT: {
//
//			SetToUpperInit(eMODULE_PPBOX_LIFT_TABLE);
//			SetToLowerInit(eMODULE_PC1_GLASS_TRANSFER);
//			wStepTransfer = S_GT_SEND_ABLE;
//			wStepLift = S_PL_RECV_ABLE_GLASS_TRANSFER;
//			sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wMaterialType = eMT_PAPER;
//			cmd.EnqueueNextStep();
//			break;
//		}
//		case S_LOOP: {
//			BOOL bTransfer = ((wStepTransfer >= S_GT_SEND_ABLE) && (wStepTransfer <= S_GT_SEND_RECV_IO_OFF_PPBOX_CHECK));
//			BOOL bLift = ((wStepLift >= S_PL_RECV_ABLE_GLASS_TRANSFER) && (wStepLift <= S_PL_RECV_GLASS_TRANSFER_IO_OFF));
//			if (bTransfer) {
//				Glass_Transfer(eMODULE_PC1_GLASS_TRANSFER);
//			}
//			if (bLift) {
//				Conveyor_Paper_load_Lift(eMODULE_PPBOX_LIFT_TABLE);
//			}
//		
//			if (!bLift && !bTransfer)
//			{
//				cmd.EnqueueNextStep();
//			}
//			else {
//				cmd.EnqueueNextStepCurrent();
//			}
//			break;
//		}
//		case S_END: {
//			IPMessage(_T("Glass TR Loading -> Unloading PPBox Sequence End"));
//			break;
//		}
//	}
//	return TRUE;
//}
//
//BOOL CMainFrame::OnIPManualGlassTRUnloadingTable(CIPCommand& cmd) {
//	const CIPVar& var = cmd.m_var;
//	CIPVar& varResult = cmd.m_varResult;
//	CIPVar& varCookie = cmd.m_varCookie;
//	if (!cmd.IsOK() || !cmd.IsChildOK())
//	{
//		return FALSE;
//	}
//	enum {
//		S_INIT,
//		//S_INIT_POS,
//		S_MANUAL,
//		S_END,
//	};
//
//	WORD& wStep = sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep;
//	BOOL bLinkCheck = var.GetChildItem("LinkCheck", TRUE);		// TRUE 인 경우 PC 간 통신을 통해서 동작을 진행 / FALSE 인 경우 PC 간 통신에 상관없이 동작만 진행 ( ex)Transfer <-> PC2 Shift Table )
//	
//	switch (cmd.GetCurrentStep()) {
//	case S_INIT: {
//		SetToLowerInit(eMODULE_PC1_GLASS_TRANSFER);
//		wStep = bLinkCheck ? S_GT_SEND_ABLE : S_GT_SEND_START_TABLE;
//		sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wMaterialType = eMT_ORG_GLASS;	// 해당 셋팅을 해주지 않으면 이전에 남아있던 값이 그대로 넘어가서 정상적인 동작을 하지 않음
//		cmd.EnqueueNextStep();
//		break;
//	}
//	case S_MANUAL: {
//		
//		if (!bLinkCheck) {	
//			if (wStep == S_GT_SEND_TABLE_COMPLETE)
//				wStep = S_GT_SEND_Z_UP_TABLE;
//		}
//		
//		if ((wStep >= S_GT_SEND_ABLE) && (wStep <= S_GT_SEND_Z_UP_TABLE_CHECK))
//		{
//			Glass_Transfer(eMODULE_PC1_GLASS_TRANSFER);
//			cmd.EnqueueNextStepCurrent();
//		}
//		else
//		{
//			cmd.EnqueueNextStep();
//		}
//		break;
//	}
//	case S_END: {
//		SetToLowerInit(eMODULE_PC1_GLASS_TRANSFER);
//		SetToUpperInit(eMODULE_PC1_GLASS_TRANSFER);
//		IPMessage(_T("Glass TR Unloading Table Sequence End"));
//		break;
//	}
//	}
//	return TRUE;
//}
//
//
//
//BOOL CMainFrame::OnIPManualCoverPickerLoading(CIPCommand& cmd) {
//	const CIPVar& var = cmd.m_var;
//	CIPVar& varResult = cmd.m_varResult;
//	CIPVar& varCookie = cmd.m_varCookie;
//	if (!cmd.IsOK() || !cmd.IsChildOK())
//	{
//		return FALSE;
//	}
//	enum {
//		S_INIT,
//		S_MANUAL,
//		S_END,
//	};
//	WORD& wStepCover = sm.Module[eMODULE_LOAD_GLASS_4_CONVEYOR].Status.wSeqStep;
//	WORD& wStepPicker = sm.Module[eMODULE_PPBOX_COVER_PICKER].Status.wSeqStep;
//	// 여기서 다음 Conveyor 에서 받는 부분까지 처리해줘야할까?
//	switch (cmd.GetCurrentStep()) {
//		case S_INIT: {
//			SetToLowerInit(eMODULE_LOAD_GLASS_4_CONVEYOR);
//			SetToUpperInit(eMODULE_PPBOX_COVER_PICKER);
//			wStepCover = S_CVL_4_COVER_TARGET_SET;
//			wStepPicker = S_CP_INIT;
//			cmd.EnqueueNextStep();
//			break;
//		}
//		case S_MANUAL: {
//			BOOL bCover = (wStepCover >= S_CVL_4_COVER_TARGET_SET) && (wStepCover <= S_CVL_4_COVER_SEND_IO_OFF);
//			BOOL bPicker = (wStepPicker >= S_CP_INIT) && (wStepPicker <= S_CP_RECV_SEND_IO_OFF_CHECK);
//			if (bCover)
//			{
//				Conveyor_PPBOX_COVER(eMODULE_LOAD_GLASS_4_CONVEYOR);
//			}
//			if (bPicker) {
//				Conveyor_PPBOX_PICKER(eMODULE_PPBOX_COVER_PICKER);
//			}
//			if (!bCover && !bPicker)
//			{
//				cmd.EnqueueNextStep();
//			}
//			else 
//				cmd.EnqueueNextStepCurrent();
//			break;
//		}
//		case S_END: {
//			IPMessage(_T("Cover Picker Loading Complete"));
//			break;
//		}
//	}
//	return TRUE;
//}
//
//BOOL CMainFrame::OnIPManualCoverPickerUnloading(CIPCommand& cmd) {
//	const CIPVar& var = cmd.m_var;
//	CIPVar& varResult = cmd.m_varResult;
//	CIPVar& varCookie = cmd.m_varCookie;
//	if (!cmd.IsOK() || !cmd.IsChildOK())
//	{
//		return FALSE;
//	}
//	enum {
//		S_INIT,
//		S_MANUAL,
//		S_END,
//	};
//
//	WORD& wStepPicker = sm.Module[eMODULE_PPBOX_COVER_PICKER].Status.wSeqStep;
//	WORD& wStepLift = sm.Module[eMODULE_GLASS_LIFT_TABLE].Status.wSeqStep;
//	
//	switch (cmd.GetCurrentStep()) {
//		case S_INIT: {
//			SetToLowerInit(eMODULE_PPBOX_COVER_PICKER);
//			SetToUpperInit(eMODULE_GLASS_LIFT_TABLE);
//			wStepPicker = S_CP_SEND_ABLE;
//			wStepLift = S_GUL_RECV_SEND_ABLE_COVER;
//			cmd.EnqueueNextStep();
//			break;
//		}
//		case S_MANUAL: {
//			BOOL bPicker = (wStepPicker >= S_CP_SEND_ABLE) && (wStepPicker <= S_CP_SEND_RECV_IO_OFF_CHECK);
//			BOOL bLift = (wStepLift>= S_GUL_RECV_SEND_ABLE_COVER) && (wStepLift <= S_GUL_RECV_SEND_IO_OFF_COVER);
//			if (bLift)
//			{
//				Conveyor_Glass_Unload_Lift(eMODULE_GLASS_LIFT_TABLE);
//			}
//			if (bPicker) {
//				Conveyor_PPBOX_PICKER(eMODULE_PPBOX_COVER_PICKER);
//			}
//			if (!bLift && !bPicker)
//			{
//				cmd.EnqueueNextStep();
//			}
//			else
//				cmd.EnqueueNextStepCurrent();
//			break;
//		}
//		case S_END: {
//			IPMessage(_T("Cover Picker Unloading Complete"));
//			break;
//		}
//	}
//	return TRUE;
//}
//
//BOOL CMainFrame::OnIPManualStop(CIPCommand& cmd) {
//	HIPCOMMAND hcmd1 = FindPendingIPStepCommand("Manual", "GlassTRLoading");
//	if (hcmd1) {
//		hcmd1->ReserveErrorStep();
//	}
//	HIPCOMMAND hcmd2 = FindPendingIPStepCommand("Manual", "GlassTRUnloadingPPBox");
//	if (hcmd2) {
//		hcmd2->ReserveErrorStep();
//	}
//	HIPCOMMAND hcmd3 = FindPendingIPStepCommand("Manual", "GlassTRUnloadingTable");
//	if (hcmd3) {
//		hcmd3->ReserveErrorStep();
//	}
//	HIPCOMMAND hcmd4 = FindPendingIPStepCommand("Manual", "CoverPickerLoading");
//	if (hcmd4) {
//		hcmd4->ReserveErrorStep();
//	}
//	HIPCOMMAND hcmd5 = FindPendingIPStepCommand("Manual", "CoverPickerUnloading");
//	if (hcmd5) {
//		hcmd5->ReserveErrorStep();
//	}
//	HIPCOMMAND hcmd6 = FindPendingIPStepCommand("Manual", "PaperConveyorLoad");
//	if (hcmd6) {
//		hcmd6->ReserveErrorStep();
//	}
//	HIPCOMMAND hcmd7 = FindPendingIPStepCommand("Manual", "PaperConveyorUnload");
//	if (hcmd7) {
//		hcmd7->ReserveErrorStep();
//	}
//	HIPCOMMAND hcmd8 = FindPendingIPStepCommand("Manual", "GlassConveyorLoad");
//	if (hcmd8) {
//		hcmd8->ReserveErrorStep();
//	}
//	HIPCOMMAND hcmd9 = FindPendingIPStepCommand("Manual", "GlassConveyorUnload");
//	if (hcmd9) {
//		hcmd9->ReserveErrorStep();
//	}
//	HIPCOMMAND hcmd10 = FindPendingIPStepCommand("Return", "ZeroPoint");
//	if (hcmd10) {
//		hcmd10->ReserveErrorStep();
//	}
//	return TRUE;
//}
//
//
//BOOL	CMainFrame::OnIPRepeatXMove(CIPCommand& cmd) {
//	const CIPVar& var = cmd.m_var;
//	CIPVar& varCookie = cmd.m_varCookie;
//
//	enum {
//		S_INIT_ZT,
//		S_INIT_ZT_CHECK,
//		S_INIT_Y,
//		S_INIT_Y_CHECK,
//		S_WAIT_X,
//		S_WAIT_X_CHECK,
//		S_UNLOAD_X,
//		S_UNLOAD_X_CHECK,
//		S_END,
//	};
//	if (!cmd.IsChildOK() || !cmd.IsOK()) {
//		PTPStop((eAxis)sm.MotorPos_1->Glass_TR.Wait_X.iAxis);
//		return FALSE;
//	}
//	//if (sm.MotorStatus[eMOTOR_PAPER_BOX_EV_Z].tCrt.bServoOn == FALSE)
//	//	return FALSE;
//	switch (cmd.GetCurrentStep()) {
//		case S_INIT_ZT: {
//			T_POS tPosZ = sm.MotorPos_1->Glass_TR.Wait_NotMaterial_Z;
//			T_POS tPosT = sm.MotorPos_1->Glass_TR.Wait_T;
//			std::vector<T_POS> Multi;
//			Multi.push_back(tPosZ);
//			Multi.push_back(tPosT);
//			MultMove(Multi, &cmd);
//			cmd.EnqueueNextStep();
//			break;
//		}
//		case S_INIT_ZT_CHECK:{
//			BOOL bCheck = TRUE;
//			bCheck &= CheckInPos(sm.MotorPos_1->Glass_TR.Wait_NotMaterial_Z);
//			bCheck &= CheckInPos(sm.MotorPos_1->Glass_TR.Wait_T);
//			if (bCheck)
//				cmd.EnqueueNextStep();
//			else 
//				cmd.EnqueueNextStepCurrent(100);
//			break;
//		}
//		case S_INIT_Y: {
//			T_POS tPosY = sm.MotorPos_1->Glass_TR.Wait_Y;
//			PTPMove(tPosY, &cmd);
//			cmd.EnqueueNextStep();
//			break;
//		}
//		case S_INIT_Y_CHECK: {
//			BOOL bCheck = TRUE;
//			bCheck &= CheckInPos(sm.MotorPos_1->Glass_TR.Wait_Y);
//			if (bCheck)
//				cmd.EnqueueNextStep();
//			else 
//				cmd.EnqueueNextStepCurrent(100);
//			break;
//		}
//		case S_WAIT_X: {
//			T_POS tPosX = sm.MotorPos_1->Glass_TR.Wait_X;
//			//T_POS tPosX = sm.MotorPos_1->ULD_Table.Wait_Z;
//			PTPMove(tPosX, &cmd);
//			cmd.EnqueueNextStep(S_WAIT_X_CHECK);
//			break;
//		}
//		case S_WAIT_X_CHECK: {
//			BOOL bCheck = TRUE;
//			bCheck &= CheckInPos(sm.MotorPos_1->Glass_TR.Wait_X);
//			//bCheck &= CheckInPos(sm.MotorPos_1->ULD_Table.Wait_Z);
//			if (bCheck)
//				cmd.EnqueueNextStep(S_UNLOAD_X);
//			else 
//				cmd.EnqueueNextStepCurrent(100);
//			break;
//		}
//		case S_UNLOAD_X:{
//			T_POS tPosX = sm.MotorPos_1->Glass_TR.Unload_PPBox_Left_X;
//			//T_POS tPosX = sm.MotorPos_1->ULD_Table.PPBoxUnLoad_Z;
//			PTPMove(tPosX, &cmd);
//			cmd.EnqueueNextStep(S_UNLOAD_X_CHECK);
//			break;
//		}
//		case S_UNLOAD_X_CHECK: {
//			BOOL bCheck = TRUE;
//			bCheck &= CheckInPos(sm.MotorPos_1->Glass_TR.Unload_PPBox_Left_X);
//			//bCheck &= CheckInPos(sm.MotorPos_1->ULD_Table.PPBoxUnLoad_Z);
//			if (bCheck)
//				cmd.EnqueueNextStep(S_WAIT_X);
//			else 
//				cmd.EnqueueNextStepCurrent(100);
//			break;
//		}
//		case S_END: {
//			break;
//		}
//	}
//	return TRUE;
//}
//	
//
//BOOL CMainFrame::OnIPRepeatXMoveStop(CIPCommand& cmd) {
//
//	HIPCOMMAND hcmd = FindPendingIPStepCommand("Stage", "RepeatXMove");
//	if (hcmd) {
//		hcmd->ReserveErrorStep();
//	}
//	return TRUE;
//}
//
BOOL CMainFrame::OnIP_SequenceAutoRun(CIPCommand& cmd)
{
    const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;
	CIPVar& varResult = cmd.m_varResult;

	enum {
		S_CONVEYOR_PARAM_SET,
		S_CHECK,
		S_START,
		S_CIM_AUTO_RUN,
	};

	BOOL bCheck = TRUE;

	if (!cmd.IsChildOK() || !cmd.IsOK()) {
		if ((cmd.GetCurrentStep() == S_CONVEYOR_PARAM_SET) || (cmd.GetCurrentStep() == S_CHECK)) {
			IPMessage(_T("Conveyor Parameter Setting Fail.\nConnect Status Check."), _T("Auto Run"), 10 * 1000);
		}
		else if ((cmd.GetCurrentStep() >= S_CIM_AUTO_RUN)) {
			IPMessage(_T("Other EQP Heavy Alarm Or CIM Program Check."), _T("Auto Run"), 10 * 1000);
		}

		return FALSE;
	}
	BOOL bSequenceInit			= var.GetChildItem("SequenceInit", FALSE);
	BOOL bConveyorInit			= var.GetChildItem("ConveyorInit", FALSE);
	BOOL bGlassTransferInit		= var.GetChildItem("GlassTransferInit", FALSE);

	switch (cmd.GetCurrentStep()) {
		case S_CONVEYOR_PARAM_SET: {
			for (int i = 0; i < nCONVEYOR_LD; i++) {
				eMODULEs eModule = (eMODULEs)(eMODULE_PC1_LOAD_CONVEYOR_1 + i);
				auto& Param = sm.Conveyor_Parameter[i];
				Serial::Inverter::SetInverterSetting(eModule, g_pRecipe->conveyorSpeed[i].iACC1, g_pRecipe->conveyorSpeed[i].iACC2, g_pRecipe->conveyorSpeed[i].iDEC1, g_pRecipe->conveyorSpeed[i].iDEC2, &cmd);
			}
			cmd.EnqueueNextStep();
			break;
		}
		case S_CHECK: {
			if (g_pRecipe == nullptr)
			{
				IPMessage(g_st[IDS_MAIN_FRAME_RECIPE_NEED_TO_BE_APPLIED]);
				return FALSE;
			}

			// eModule Setting
			for (int iModule = eMODULE_PC1_BEGIN; iModule < eMODULE_PC1_END; ++iModule) {
				sm.Module[iModule].eModule = (eMODULEs)iModule;
			}

			// 전체 초기화
			if (bSequenceInit) {
				for (int iModule = eMODULE_PC1_BEGIN; iModule < eMODULE_PC1_END; ++iModule) {
					SetToLowerInit((eMODULEs)iModule);
					SetToUpperInit((eMODULEs)iModule);
					sm.Module[iModule].Status.wSeqStep = 0;
					sm.Module[iModule].Status.nCrtCount = 0;
					sm.Module[iModule].Status.bSafety = FALSE;
					sm.Module[iModule].Status.wMode = 0;

					sm.Module[iModule].Glass.bMaterial = FALSE;

					//<< KES 2024.07.13 - 시퀀스 초기화시 FDC 모듈별 Step, CellID 초기화
					for (int i = 0; i < 5; i++) {
						SET_STEP_CELL(sm.FDC->Conveyor[i], 0, "");
					}
					SET_STEP_CELL(sm.FDC->GlassTransfer, 0, "");
					//>>
				}
			}
			else {
				// Conveyor 초기화
				if (bConveyorInit) {
					for (int iModule = eMODULE_PC1_LOAD_CONVEYOR_1; iModule <= eMODULE_PC1_GLASS_TRANSFER; iModule++) {
						// Glass Transfer도 연결 되어 있기 때문에 신호 확인 부분 초기화 해줘야 함.

						BOOL bInitCheck = TRUE;
						if (iModule == eMODULE_PC1_GLASS_TRANSFER) {
							bInitCheck = !(sm.Module[iModule].Status.wSeqStep >= S_GLASS_TR_SEQ_GLASS_UNLOAD_START && sm.Module[iModule].Status.wSeqStep <= S_GLASS_TR_SEQ_GLASS_UNLOAD_END);
						}

						if (bInitCheck) {
							SetToLowerInit((eMODULEs)iModule);
							SetToUpperInit((eMODULEs)iModule);
							sm.Module[iModule].Status.wSeqStep = 0;

							if (iModule != eMODULE_PC1_GLASS_TRANSFER) {
								//<< KES 2024.07.13 - 시퀀스 초기화시 FDC 모듈별 Step, CellID 초기화
								int iIndex = iModule - eMODULE_PC1_LOAD_CONVEYOR_1;
								SET_STEP_CELL(sm.FDC->Conveyor[iIndex], 0, "");
								//>>

								sm.Module[iModule].Status.nCrtCount = 0;
								sm.Module[iModule].Status.bSafety = FALSE;
								sm.Module[iModule].Status.wMode = 0;
								sm.Module[iModule].Glass.bMaterial = FALSE;
							}
							else {
								//<< KES 2024.07.13 - 시퀀스 초기화시 FDC 모듈별 Step, CellID 초기화
								SET_STEP_CELL(sm.FDC->GlassTransfer, 0, "");
								//>>
							}
						}
					}
				}

				// Glass Transfer 초기화
				if (bGlassTransferInit) {
					// Load Conveyor 2
					{
						BOOL bInitCheck = (sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep >= S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_START && sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep <= S_LDCV2_SEQ_GLASS_PAPER_UNLOAD_END);
						if (bInitCheck) {
							SetToLowerInit((eMODULEs)eMODULE_PC1_LOAD_CONVEYOR_2);
							SetToUpperInit((eMODULEs)eMODULE_PC1_LOAD_CONVEYOR_2);
							sm.Module[eMODULE_PC1_LOAD_CONVEYOR_2].Status.wSeqStep = 0;
							//<< KES 2024.07.13 - 시퀀스 초기화시 FDC 모듈별 Step, CellID 초기화
							SET_STEP_CELL(sm.FDC->Conveyor[1], 0, "");
							//>>
						}						
					}
					// Elevator Conveyor
					{
						BOOL bInitCheck = (sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Status.wSeqStep >= S_ELCV_SEQ_PAPER_LOAD_START && sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Status.wSeqStep <= S_ELCV_SEQ_PAPER_LOAD_END);
						if (bInitCheck) {
							SetToLowerInit((eMODULEs)eMODULE_PC1_ELEVATOR_CONVEYOR);
							SetToUpperInit((eMODULEs)eMODULE_PC1_ELEVATOR_CONVEYOR);
							sm.Module[eMODULE_PC1_ELEVATOR_CONVEYOR].Status.wSeqStep = 0;
							//<< KES 2024.07.13 - 시퀀스 초기화시 FDC 모듈별 Step, CellID 초기화
							SET_STEP_CELL(sm.FDC->Conveyor[2], 0, "");
							//>>
						}
					}
					// Glass Transfer
					{
						SetToLowerInit((eMODULEs)eMODULE_PC1_GLASS_TRANSFER);
						SetToUpperInit((eMODULEs)eMODULE_PC1_GLASS_TRANSFER);
						sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wSeqStep = 0;
						sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.nCrtCount = 0;
						sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.bSafety = FALSE;
						sm.Module[eMODULE_PC1_GLASS_TRANSFER].Status.wMode = 0;
						sm.Module[eMODULE_PC1_GLASS_TRANSFER].Glass.bMaterial = FALSE;
						//<< KES 2024.07.13 - 시퀀스 초기화시 FDC 모듈별 Step, CellID 초기화
						SET_STEP_CELL(sm.FDC->GlassTransfer, 0, "");
						//>>
					}
				}
			}

			cmd.EnqueueNextStep();
			break;
		}
		case S_START: {
			if (!sm.Sequence->bCIMSkip)
			if (!sm.Sequence->bCIMSkip)
				SendIPStepCommand("UI", NULL, "Scenario", "AUTO_RUN");

			cmd.EnqueueNextStep();
			break;
		}
		case S_CIM_AUTO_RUN: {
			//<< MCC
			sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI01].A[eMCC_ACTION_LD_LI_01_MANUAL_STOP] = 0;
			sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI02].A[eMCC_ACTION_LD_LI_02_MANUAL_STOP] = 0;
			sm.MCC_EQP->Layer[eMCC_LAYER_LD_LE01].A[eMCC_ACTION_LD_LE_01_MANUAL_STOP] = 0;
			sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO01].A[eMCC_ACTION_LD_LO_01_MANUAL_STOP] = 0;
			sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO02].A[eMCC_ACTION_LD_LO_02_MANUAL_STOP] = 0;
			sm.MCC_EQP->Layer[eMCC_LAYER_LD_TR01].A[eMCC_ACTION_LD_TR_01_MANUAL_STOP] = 0;
			//>>

			sm.Sequence->fAutoMode = TRUE;
			sm.Sequence->fCycleStop = FALSE;
			break;
		}
	}
	return TRUE;
}

BOOL CMainFrame::OnIP_MuteOnOff(CIPCommand& cmd)
{
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	enum {
		eS_INIT,
		eS_MUTE_OFF,
		eS_MUTE_OFF_CHECK,
		eS_MUTE_A,
		eS_MUTE_A_CHECK,
		eS_MUTE_B,
		eS_MUTE_B_CHECK,
		eS_MUTE_CHECK,
		eS_END,
	};

	eMODULEs eModule = (eMODULEs)var("Module", eMODULE_NONE);
	BOOL bOnOff = var("OnOff", FALSE);
	
	if (!cmd.IsOK() || !cmd.IsChildOK()) {
		sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_A_ON = FALSE;
		sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_B_ON = FALSE;
		sm.Module[eModule].Status.eStatus = eMODULE_STATUS_DOWN;
		return FALSE;
	}
	
	const int time_out = 5000;
	switch (cmd.GetCurrentStep()) {
		case eS_INIT: {
			sm.Module[eModule].Status.eStatus = eMODULE_STATUS_RUN;
			cmd.EnqueueNextStep();
			break;
		}
		case eS_MUTE_OFF: {
			sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_A_ON = FALSE;
			sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_B_ON = FALSE;
			cmd.m_dwCookie = GetTickCount64();
			cmd.EnqueueNextStep();
			break;
		}
		case eS_MUTE_OFF_CHECK: {
			BOOL bCheck = sm.IO->tOut_CRT.fLD_LIGHT_CURTAIN_MUTE_A_ON;
			bCheck |= sm.IO->tOut_CRT.fLD_LIGHT_CURTAIN_MUTE_B_ON;

			if (bCheck) {
				if (GetTickCount64() - cmd.m_dwCookie >= time_out) // 일정시간 변화가 없으면 에러처리
					cmd.ReserveErrorStep();
				else
					cmd.EnqueueNextStepCurrent();
			}
			else {
				if (bOnOff)
					cmd.EnqueueNextStep();
				else
					cmd.EnqueueNextStep(eS_MUTE_CHECK);
			}
			break;
		}
		case eS_MUTE_A: {
			sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_A_ON = TRUE;
			cmd.m_dwCookie = GetTickCount64();
			cmd.EnqueueNextStep();
			break;
		}
		case eS_MUTE_A_CHECK: {
		   if (sm.IO->tOut_CRT.fLD_LIGHT_CURTAIN_MUTE_A_ON) {
		      cmd.EnqueueNextStep(-1, 1000); // 1초뒤에 뮤트 상태를 체크한다
		   }
		   else {
		      if (GetTickCount64() - cmd.m_dwCookie >= time_out) // 일정시간 변화가 없으면 에러처리
		         cmd.ReserveErrorStep();
		      else
		         cmd.EnqueueNextStepCurrent();
		   }
		   break;
		}
		case eS_MUTE_B: {
			sm.IO->tOut_NEW.fLD_LIGHT_CURTAIN_MUTE_B_ON = TRUE;
			cmd.m_dwCookie = GetTickCount64();
			cmd.EnqueueNextStep();
			break;
		}
		case eS_MUTE_B_CHECK: {
			if (sm.IO->tOut_CRT.fLD_LIGHT_CURTAIN_MUTE_B_ON) {
				cmd.EnqueueNextStep(); // 1초뒤에 뮤트 상태를 체크한다
			}
			else {
				if (GetTickCount64() - cmd.m_dwCookie >= time_out) // 일정시간 변화가 없으면 에러처리
					cmd.ReserveErrorStep();
				else
					cmd.EnqueueNextStepCurrent();
			}
			break;
		}
		case eS_MUTE_CHECK: {
			if (GetMute(bOnOff)) {
			   cmd.EnqueueNextStep();
			}
			else {
			   if (GetTickCount64() - cmd.m_dwCookie >= time_out) // 일정시간 변화가 없으면 에러처리
			      cmd.ReserveErrorStep();
			   else
			      cmd.EnqueueNextStepCurrent();
			}
			break;
		}
		case eS_END: {
			sm.Module[eModule].Status.eStatus = eMODULE_STATUS_COMPLETED;
			break;
		}
	}
	
	return TRUE;
}


BOOL CMainFrame::OnIPSequenceBarCodeMessage(CIPCommand& cmd) {
	// 21.03.18
	enum {
		S_MESSAGE,
		S_MESSAGE_RESULT,
		S_INPUT,
		S_END,
	};
	const CIPVar& var = cmd.m_var;
	CIPVar& varCookie = cmd.m_varCookie;

	int iModule = var.GetChildItem("Module", eMODULE_NONE);
	CString strMessage = var.GetChildItem("Message", g_st[IDS_MAIN_FRAME_BARCODE_NOT_NORMAL]);
	CString strButton[4];
	for (int i = 0; i < countof(strButton); i++) {
		strButton[i] = var.GetChildItem(FormatA("Button%d", i + 1), _T(""));
	}

	switch (cmd.GetCurrentStep()) {
		case S_MESSAGE: {
			if (sm.Module[iModule].Status.eMessageStatus != eMESSAGE_REQUEST) {
				sm.Module[iModule].Status.eMessageStatus = eMESSAGE_NONE;
				return FALSE;
			}

			CString strButtons;
			for (int i = 0; i < countof(strButton); i++) {
				if (!strButton[i].IsEmpty()) {
					strButtons += strButton[i] + _T(";");
				}
			}
			if (strButtons.Find(_T(";"), strButtons.GetLength() - 1)) {
				strButtons = strButtons.Mid(0, strButtons.GetLength() - 1);
			}

			RespMessageCmd(&cmd, strMessage, strButtons);
			break;
		}
		case S_MESSAGE_RESULT: {
			const auto& varResult = cmd.GetChildResult("Message", nullptr, "Message", "Response");
			CString MessageResult = varResult.GetChildItem("ButtonResult", _T(""));

			for (int idx = 0; idx < countof(strButton); ++idx) {
				if (MessageResult.Compare(strButton[idx]) == 0) {
					sm.Module[iModule].Status.wMessageResult = idx;
				}
			}

			if (sm.Module[iModule].Status.wMessageResult == eBCR_MESSAGE_INPUT) {
				cmd.EnqueueNextStep(S_INPUT);
			}
			else {
				cmd.EnqueueNextStep(S_END);
			}
			break;
		}
		case S_INPUT: {
			CString strInput = InPutMessage(g_st[IDS_MAIN_FRAME_MANUAL_INSERT_BARCODE_VALUE]);
			strncpy_s(sm.Module[iModule].Glass.szCELL_ID_BCR, sizeof(sm.Module[iModule].Glass.szCELL_ID_BCR), CStringA(strInput).MakeUpper(), sizeof(sm.Module[iModule].Glass.szCELL_ID_BCR) - 1);
			cmd.EnqueueNextStep();
			break;
		}
		case S_END: {
			sm.Module[iModule].Status.eMessageStatus = eMESSAGE_RESPONSE;
		}
	}
	return TRUE;
}

// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "DataTransfer.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{}

CChildView::~CChildView()
{}

void CChildView::ClientConnect()
{
	m_client.Close();

	CProfile profile(_T("../cfg/AddressInfo.ini"));

	CProfileSection section = profile.GetSection(_T("MODULE_SERVER_INFO"));
	// 서버 IP LOADER로 변경됨 (192.168.0.10)
	CString ip = section.GetItemValue(_T("IP"), _T("127.0.0.1"));
	auto port = (unsigned short)section.GetItemValue(_T("PORT"), 7000);

	//<<<<< 이 부분은 PC 별 따로 세팅
	// IO, PC 번호/상태
	m_client.SetIOAddress((DWORD*)(&sm.IO->tIn_CRT), sizeof(sm.IO->tIn_CRT), (DWORD*)(&sm.IO->tOut_CRT), sizeof(sm.IO->tOut_CRT));
	m_client.SetPCStatus(eMACHINE_LD, &(sm.Sequence->fAutoMode), &(sm.Sequence->fCycleStop), &(sm.Sequence->fIsAlarm), &(sm.Sequence->fPMMode)
		//<< 2024.06.14 추가됨
		, &(sm.Sequence->fCIM_StepStop), &(sm.Sequence->fCIM_LoadingStop), &(sm.Sequence->fCIM_UNIT_LoadingStop[0]), &(sm.Sequence->fCIM_UNIT_LoadingStop[1]));
	//>>
// 통신할 모듈 세팅
	std::vector<T_SEQUENCE_MODULE_INFO*> sendModules;
	sendModules.push_back(&sm.Module[eMODULE_PC1_GLASS_TRANSFER]);
	m_client.SetModuleAddress(eMODULE_PC1_BEGIN, eMODULE_PC1_END, sendModules);

	// CIM PC
	//// IO, PC 번호/상태
	//m_client.SetIOAddress(nullptr, 0, nullptr, 0);
	//m_client.SetPCStatus(nMACHINE_END, nullptr, nullptr, nullptr, nullptr);
	//// 통신할 모듈 세팅
	//m_client.SetModuleAddress(eMODULE_NONE, eMODULE_NONE, {});
	////>>>>>> 

	m_client.Connect(ip, port);
}

void CChildView::TorqueClientConnect()
{
	m_torque_client.Connect(_T("192.168.0.1"), 7002, eMACHINE_LD);
}

void CChildView::DustCollectorClientConnect()
{
	m_dust_collector_client.Connect(_T("192.168.0.1"), 7003, eMACHINE_LD);
}

void CChildView::FDCClientConnect()
{
	m_fdc_client.Connect(_T("192.168.0.1"), 7004, eMACHINE_LD);
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

// CChildView 메시지 처리기
BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

#define CLIENT_CONNECT_CHECK 9904
#define ONLINE_STATUS_CHECK 9905
int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 서버 - CIM PC가 과부하 걸리면 다른 PC도 괜찮음
	{
		CProfile profile(_T("../cfg/AddressInfo.ini"));
		CProfileSection section = profile.GetSection(_T("MODULE_SERVER_INFO"));
		auto port = (unsigned short)section.GetItemValue(_T("PORT"), 7000);
		m_server.Open(port);
		std::this_thread::sleep_for(1s);
	}

	// 클라이언트가 서버로 연결 요청한다
	{
		ClientConnect();
		TorqueClientConnect();
		DustCollectorClientConnect();
		FDCClientConnect();
		SetTimer(CLIENT_CONNECT_CHECK, 3000, nullptr);
	}

	{
		SetTimer(ONLINE_STATUS_CHECK, 100, nullptr);
	}
	return 0;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// Client
	if (CLIENT_CONNECT_CHECK == nIDEvent)
	{
		KillTimer(CLIENT_CONNECT_CHECK);
		if (!m_client.IsConnected())
		{
			ClientConnect();
		}
		else if (GetTickCount64() - m_client.GetLastMessagTime() > 3'000)
		{
			m_client.Close();
		}
		//>>

		//<< Torque
		if (!m_torque_client.IsConnected())
		{
			TorqueClientConnect();
		}
		else if (GetTickCount64() - m_torque_client.GetLastMessagTime() > 3'000)
		{
			m_torque_client.Close();
		}
		//>>

		//<< DustCollector
		if (!m_dust_collector_client.IsConnected())
		{
			DustCollectorClientConnect();
		}
		else if (GetTickCount64() - m_dust_collector_client.GetLastMessagTime() > 3'000)
		{
			m_dust_collector_client.Close();
		}
		//>>

		//<< FDC
		if (!m_fdc_client.IsConnected())
		{
			FDCClientConnect();
		}
		else if (GetTickCount64() - m_fdc_client.GetLastMessagTime() > 3'000)
		{
			m_fdc_client.Close();
		}
		//>>

		SetTimer(CLIENT_CONNECT_CHECK, 100, nullptr);
	}
	else if (ONLINE_STATUS_CHECK == nIDEvent)
	{
		// 21.03.10 온라인 상태 체크 후 알람
		for (int idx = eMACHINE_SCT; idx < nMACHINE_END; ++idx)
		{
			if (sm.PCs->ControlPC[eMACHINE_LD].fOnlineMode != sm.PCs->ControlPC[idx].fOnlineMode)
				SetAlarm(eAI_ONLINE_STATUS_SCT_MATCHING_FAIL + idx - eMACHINE_SCT, IDS_DES_NOT_USE);
		}
	}
	CWnd::OnTimer(nIDEvent);
}

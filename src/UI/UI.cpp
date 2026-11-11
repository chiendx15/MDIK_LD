#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "UI.h"
#include "MainFrame.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
//--------------------------------------------------------------------------------------------------
class CAboutDlg : public CDialogEx {
public:
	CAboutDlg() noexcept;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnCancelMode();
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX) {
	EnableActiveAccessibility();
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------------------------

// CUIApp
BEGIN_MESSAGE_MAP	(CUIApp, CWinAppEx)
	ON_COMMAND		(ID_APP_ABOUT	,&CUIApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
//	ON_COMMAND		(ID_FILE_NEW	,&CWinAppEx::OnFileNew)
//	ON_COMMAND		(ID_FILE_OPEN	,&CWinAppEx::OnFileOpen)
END_MESSAGE_MAP		()


// CUIApp 생성

CUIApp::CUIApp() noexcept {
	m_bHiColorIcons = TRUE;

	//// 다시 시작 관리자 지원
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 애플리케이션을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 애플리케이션 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("UI.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CUIApp 개체입니다.

CUIApp					theApp;
std::unique_ptr<CADODatabase> g_db = nullptr;
int g_CimReport = -1;
int g_PC = -1;
int						g_eSelectedMainMenu = eMAIN_MENU_NONE;
int						g_eSelectedSubMenu	= eSUB_MENU_NONE;
DStringTable			g_stIO;
// KES 2024.05.23 g_stMotor, g_stDecription 선언위치 SharedFunction으로 이동
CRecipe<T_MDI_RECIPE>	g_recipes;
bool					g_closeIPLoop;
ULONGLONG				g_iLogout_time = 0;
// CUIApp 초기화
BOOL					g_bUIRotateDirection = TRUE;

CMoveButtonStatus g_MoveButtonStatus;	// KES 추가 2024.05.23

std::map<std::pair<CStringA, CStringA>, eSEQUENCE_STATE> g_mapSEQ_CIM;
std::map<std::pair<CStringA, int>, eSEQUENCE_STATE> g_mapAlarmReport;

BOOL CUIApp::InitInstance() {
	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	//if (!AfxSocketInit()) {
	//	AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
	//	return FALSE;
	//}
	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit()) {
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	//<< 프로그램 중복실행 방지
	TList<DWORD> lst;
	CString strAppName(AfxGetAppName());
	int iSize = FindApp(strAppName + _T(".exe"), lst, true);
	if (iSize > 1) {
		AfxMessageBox(Format(_T("%s %s"), strAppName, g_st[IDS_UI_PROGRAM_ALREADY_RUN]));
		return FALSE;
	}
	//>>

	AfxEnableControlContainer	();
	EnableTaskbarInteraction	(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	 AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey			(g_st[IDS_UI_REGISTRY_KEY]);

	g_st.LoadStringTable(_T("../data/StringTable/StringTable.csv"));
	g_stIO.LoadStringTable(_T("../data/StringTable/StringTable_IO_1_LD.csv"));
	g_stMotor.LoadStringTable(_T("../data/StringTable/StringTable_Motor.csv"));
	g_stDescription.LoadStringTable(_T("../data/StringTable/StringTable_Description.csv"));	// KES 추가 2024.05.23	

	CDrawSymbol::InitInstance();
	// 공유메모리 초기화
	if (!OpenDB())
	{
		AfxMessageBox(_T("Can't Connect DB"));
	}

	if (!sm.InitSharedMemory()) {
		AfxMessageBox(_T("Can't Initialize SharedMemory"));
		return FALSE;
	}
	if (!g_ipClient.Init() || !g_ipClient.StartIPCommandLoop())
	{
		AfxMessageBox(_T("UI IPClient connect fail..."));
		return FALSE;
	}

	m_UserManager.Initialize();

	CProfile profile(_T("../cfg/CIMMocha.ini"));
    sm.Sequence->bCIMSkip = profile.GetProfileValue(_T("CIMSKIP"), _T("Use"), FALSE);

	// CIM Mocha 접속
	m_cimIPLoopThread.reset(new std::thread
	(
		[&]()
		{
			CString Task(_T("LD"));
			if (!g_ipCIMClient.Init(Task, _T("../cfg/CIMMocha.ini")))
			{
				return;
			}
				
			g_closeIPLoop = false;
			do
			{
				if (!g_ipCIMClient.IsIPCommandLoopStarted() || !g_ipCIMClient.IsServerConnected())
				{
					g_ipCIMClient.StartIPCommandLoop();
				}
				std::this_thread::sleep_for(300ms);
			} while (!g_closeIPLoop);
			g_ipCIMClient.StopIPCommandLoop();
		}
	));


	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	pFrame->LoadFrame	(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);
	pFrame->ShowWindow	(SW_SHOWMAXIMIZED);
	pFrame->UpdateWindow();

	m_bCreatedWindows = TRUE;

	return TRUE;
}

int CUIApp::ExitInstance() {
	
	AfxOleTerm(FALSE);
	g_closeIPLoop = true;	// cim에 붙은 thread 종료
	if (m_cimIPLoopThread != nullptr)
	{
		m_cimIPLoopThread->join();
	}

	g_ipClient.QuitOnDisconnected();

	return CWinAppEx::ExitInstance();
}

// CUIApp 메시지 처리기


// CUIApp 사용자 지정 로드/저장 방법

void CUIApp::PreLoadState() {
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);	
}

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CUIApp::OnAppAbout() {
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
void CUIApp::LoadCustomState() {
}

void CUIApp::SaveCustomState() {
}


// CUIApp 메시지 처리기
 void		DrawButton(CDC* pDC, LPRECT lrect, LPCTSTR szText, eEVENT_COLOR event) {
	if (event== eEC_MOUSE_UP)			CDrawSymbol::SelectedDrawText	(pDC	,lrect	,RGB_BLUE								,RGB_BLUE								,RGB_WHITE								,DT_CENTER	,9		,szText);
	else if (event== eEC_MOUSE_MOVE)	CDrawSymbol::SelectedDrawText	(pDC	,lrect	,g_colorForeground[eEC_MOUSE_MOVE	]	,g_colorForeground[eEC_DEFAULT		]	,g_colorForeground[eEC_MOUSE_DOWN	]	,DT_CENTER	,9		,szText);
	else								CDrawSymbol::SelectedDrawText	(pDC	,lrect	,g_colorForeground[eEC_NORMAL		]	,g_colorBackground[eEC_DEFAULT		]	,g_colorForeground[eEC_NORMAL		]	,DT_CENTER	,9		,szText);
}
 void		DrawButton(CDC* pDC, LPRECT lrect, int iSize, DWORD dwAlign	,LPCTSTR szText, eEVENT_COLOR event) {
	if (event== eEC_MOUSE_UP)			CDrawSymbol::SelectedDrawText	(pDC	,lrect	,RGB_GREEN								,g_colorForeground[eEC_DEFAULT		]	,g_colorForeground[eEC_MOUSE_DOWN	]	,dwAlign	,iSize	,szText);
	else if (event== eEC_MOUSE_MOVE)	CDrawSymbol::SelectedDrawText	(pDC	,lrect	,g_colorForeground[eEC_MOUSE_MOVE	]	,g_colorForeground[eEC_DEFAULT		]	,g_colorForeground[eEC_MOUSE_DOWN	]	,dwAlign	,iSize	,szText);
	else								CDrawSymbol::SelectedDrawText	(pDC	,lrect	,g_colorForeground[eEC_NORMAL		]	,g_colorBackground[eEC_DEFAULT		]	,g_colorForeground[eEC_NORMAL		]	,dwAlign	,iSize	,szText);
}

void SendRecipe()
{
    T_MDI_RECIPE& recipe = *(g_recipes.GetAppliedData());
	g_pRecipe = &recipe;
	CMemFile f;
	CArchive ar(&f, CArchive::store);
	
	if (ar.IsStoring())
	{
		ar << recipe;
		ar.Close();
	}
	
	auto length = f.GetLength();
	auto ptr = f.Detach();
	
	CIPVar varRecipe;
	varRecipe.AddChildItemEmpty("Recipe").SetRawData(ptr, length);
	BOOL bResult = g_ipClient.SendIPCommand("Sequence", "Apply", "Recipe", varRecipe);
	bResult &= varRecipe.GetChildItem("Ack", FALSE);
	if (bResult)
	{
		// to do message
	}
	
	free(ptr);
	f.Close();
}

BOOL OpenDB()
{
	CProfile profile(_T("../cfg/DB.ini"));

	CProfileSection section		= profile.GetSection(_T("DB_INFO"));
	CString			IP			= section.GetItemValue(_T("IP")			, _T("localhost"));
	CString 		Port		= section.GetItemValue(_T("PORT")		, _T("1433"));
	CString			ID			= section.GetItemValue(_T("ID")			, _T("sa"));
	CString			Password	= section.GetItemValue(_T("PASSWORD")	, _T("qrsystem"));
	CString			DBName		= section.GetItemValue(_T("DB_NAME")	, _T("HSMS"));

	section						= profile.GetSection(_T("CIM"));
	g_CimReport					= section.GetItemValue(_T("CIMREPORT")	, 0);
	g_PC						= section.GetItemValue(_T("PC")			, 0);

	g_db.reset(new CADODatabase());
	return DBConnection(g_db.get(), IP, DBName, ID, Password, 5);
}


std::vector<std::pair<CString, CString>> GetRecipeParam(T_MDI_RECIPE* recipe)
{
	std::vector<std::pair<CString, CString>> vecParam;
	std::pair<CString, CString> Param;

	auto ParamItem = [&](CString _paramName, auto _paramValue)
	{
		Param.first = Format(L"%d_%s", g_PC, _paramName);
		Param.second = ToCString(_paramValue);
		vecParam.push_back(Param);
	};

	//ParamItem(L"NAME",					recipe->recipeName);

	ParamItem(L"GLASS_THINKNESS",		recipe->ppbox.dGlassThickness);
	ParamItem(L"PAPER_THINKNESS",		recipe->ppbox.dPaperThickness);
	ParamItem(L"GLASS_COUNT",			recipe->ppbox.iGlassCount);
	ParamItem(L"TOP_MATERIAL_TYPE",		recipe->ppbox.bTopMaterialType);
	ParamItem(L"UNLOAD_TABLE_OFFSET_X", recipe->ppbox.dUnloadTableOffsetX);
	ParamItem(L"UNLOAD_TABLE_OFFSET_Y", recipe->ppbox.dUnloadTableOffsetY);
	return vecParam;
}


std::vector<T_PPID_PARAM> GetRecipeParam(int iRecipe) {
	std::vector<T_PPID_PARAM> vecParam;
	T_PPID_PARAM Param;

	if (g_recipes.GetRecipeData(iRecipe) == nullptr)
	{
		return vecParam;
	}

	const T_MDI_RECIPE& recipe = *(g_recipes.GetRecipeData(iRecipe));

	auto ParamItem = [&](int iPARAM, CString _paramName, auto _paramValue)
	{
		Param.iParam = iPARAM;
		Param.Name = Format(L"%s", _paramName); ;
		Param.Value = ToCString(_paramValue);
		vecParam.push_back(Param);
	};

	ParamItem(1, L"GLASS_THICKNESS", recipe.ppbox.dGlassThickness);
	ParamItem(2, L"PAPER_THICKNESS", recipe.ppbox.dPaperThickness);

	ParamItem(258, L"LD01_CV01_SPEED", recipe.conveyorSpeed[eCONVEYOR_LD_1].dHighSpeed);
	ParamItem(259, L"LD01_CV02_SPEED", recipe.conveyorSpeed[eCONVEYOR_LD_2].dHighSpeed);
	ParamItem(260, L"LD01_CV03_SPEED", recipe.conveyorSpeed[eCONVEYOR_EL].dHighSpeed);
	ParamItem(261, L"LD01_CV04_SPEED", recipe.conveyorSpeed[eCONVEYOR_ULD_1].dHighSpeed);
	ParamItem(262, L"LD01_CV05_SPEED", recipe.conveyorSpeed[eCONVEYOR_ULD_2].dHighSpeed);

	return vecParam;
}

BOOL SetRecipeData(int iRecipe) {
	if (g_db == nullptr)
		return FALSE;

	int iDB_PPID = iRecipe + 1;


	auto GetPPIDName = [&](int iPPID)->std::pair<BOOL, CString> {
		std::vector<int> vecECID;
		CADORecordset rs(g_db.get());
		if (!rs.Open(Format(L"SELECT * FROM PPID_%s WHERE PPID_NUMBER = %d", CONV_T(lpEQP[eEQP_SCT]), iPPID), CADORecordset::openQuery)) {
			rs.Close();
			return { FALSE, CString() };
		}

		if (rs.GetRecordCount() <= 0) {
			rs.Close();
			return { FALSE, CString() };
		}

		CString strPPID;
		rs.MoveFirst();
		while (!rs.IsEOF()) {
			rs.GetFieldValue(_T("PPID"), strPPID);
			strPPID.Trim();
			rs.MoveNext();
			break;
		}
		rs.Close();

		return { TRUE, strPPID };
	};

	T_MDI_RECIPE* recipe = (g_recipes.GetRecipeData(iRecipe));
	auto RecipeName = GetPPIDName(iDB_PPID);
	if (!RecipeName.first)
		return FALSE;

	recipe->recipeName = RecipeName.second;
	recipe->ppid = RecipeName.second;
	recipe->cimapplied = true;
	
	auto GetParamValue = [&](int iPARAM, CString _paramName) {
		CString strQuery = Format(_T("SELECT * FROM PPID_PARAM_%s WHERE PPID_NUMBER = %d AND PPID_PARAM_NO = %d AND P_PARAM_NAME = '%s'"),
			CONV_T(lpEQP[eEQP_SCT]), iDB_PPID, iPARAM, _paramName);

		CADORecordset rs(g_db.get());
		if (!rs.Open(strQuery, CADORecordset::openQuery)) {
			rs.Close();
			return CString();
		}

		if (rs.GetRecordCount() <= 0) {
			rs.Close();
			return CString();
		}

		CString strValue;
		rs.MoveFirst();
		while (!rs.IsEOF()) {
			rs.GetFieldValue(_T("P_PARAM_VALUE"), strValue);
			strValue.Trim();
			rs.MoveNext();
			break;
		}
		rs.Close();

		return strValue;
	};

	recipe->ppbox.dGlassThickness = _ttof(GetParamValue(1, L"GLASS_THICKNESS"));
	recipe->ppbox.dPaperThickness = _ttof(GetParamValue(2, L"PAPER_THICKNESS"));

	recipe->conveyorSpeed[eCONVEYOR_LD_1].dHighSpeed = _ttof(GetParamValue(258, L"LD01_CV01_SPEED"));
	recipe->conveyorSpeed[eCONVEYOR_LD_2].dHighSpeed = _ttof(GetParamValue(259, L"LD01_CV02_SPEED"));
	recipe->conveyorSpeed[eCONVEYOR_EL].dHighSpeed = _ttof(GetParamValue(260, L"LD01_CV03_SPEED"));
	recipe->conveyorSpeed[eCONVEYOR_ULD_1].dHighSpeed = _ttof(GetParamValue(261, L"LD01_CV04_SPEED"));
	recipe->conveyorSpeed[eCONVEYOR_ULD_2].dHighSpeed = _ttof(GetParamValue(262, L"LD01_CV05_SPEED"));


	//g_recipes.setRecipeData(iRecipe, recipe);
	g_recipes.saveRecipe(iRecipe);
	return TRUE;
}


void Update_ECID()
{
	if (g_db == nullptr)
		return;

	auto IsECID = [&](int iECID) {
		CADORecordset rs(g_db.get());
		if (!rs.Open(Format(L"SELECT * FROM ECID_%s WHERE ECID = %d", CONV_T(lpEQP[eEQP_SCT]), iECID), CADORecordset::openQuery)) {
			rs.Close();
			return FALSE;
		}

		if (rs.GetRecordCount() <= 0) {
			rs.Close();
			return FALSE;
		}
		
		rs.Close();
		return TRUE;
	};

	auto IsECID_NAME = [&](const CString& _name)->std::pair<BOOL, std::vector<int>> {
		std::vector<int> vecECID;
		CADORecordset rs(g_db.get());
		if (!rs.Open(Format(L"SELECT * FROM ECID_%s WHERE ECNAME = '%s'", CONV_T(lpEQP[eEQP_SCT]), _name), CADORecordset::openQuery)) {
			rs.Close();
			return {FALSE, vecECID };
		}

		if (rs.GetRecordCount() <= 0) {
			rs.Close();
			return { FALSE, vecECID};
		}

		rs.MoveFirst();
		while (!rs.IsEOF()) {
			int iECID = 0;
			rs.GetFieldValue(_T("ECID"), iECID);
			vecECID.push_back(iECID);
			rs.MoveNext();
		}

		rs.Close();
		return { TRUE, vecECID };
	};

	auto DeleteECID = [&](int iECID, const CString& _name) {
		CString query;
		query = Format(_T("DELETE FROM ECID_%s WHERE ECID = %d AND ECNAME = '%s'")
			, CString(lpEQP[eEQP_SCT]), iECID, _name, _name);

		return g_db->Execute(query) == TRUE;
	};

	auto UpdateQuery = [&](int iECID, const CString& _name, auto _val, auto _sll, auto _sul, auto _wll, auto _wul) -> bool
	{
		// Select 해서 있으면 Update, 없으면 Insert
		auto a = IsECID_NAME(_name);

		if (a.first) {
			for (int i = 0; i < a.second.size(); i++) {
				if (iECID != a.second[i]) {
					DeleteECID(a.second[i], _name);
				}
			}
		}
		BOOL bExist = IsECID(iECID);

		CString query;
		if (bExist) {
			query = Format(_T("UPDATE ECID_%s SET ECNAME = '%s', ECDEF = '%s', ECSLL = '%s', ECSUL = '%s', ECWLL = '%s', ECWUL = '%s' WHERE ECID LIKE %d")
				, CString(lpEQP[eEQP_SCT]), _name, ToCString(_val), ToCString(_sll), ToCString(_sul), ToCString(_wll), ToCString(_wul), iECID);
		}
		else {
			query = Format(_T("INSERT INTO ECID_%s VALUES ( %d, '%s', '%s', '%s', '%s', '%s', '%s' )")
				, CString(lpEQP[eEQP_SCT]), iECID, _name, ToCString(_val), ToCString(_sll), ToCString(_sul), ToCString(_wll), ToCString(_wul));
		}

		return g_db->Execute(query) == TRUE;
	};

	auto UpdateQuery_auto = [&](int iECID, const CString& _name, auto _val) -> bool
	{
		return UpdateQuery(iECID, _name, _val, _val, _val, _val, _val);
	};


	// 2024.06.25 LBH - 추후에 ECSLL....등등 확정되면 바꿔야함.
	UpdateQuery_auto(1000, L"LD01_EV01_RECV_Z_POSITION",	sm.MotorPos_1->EL_Convryor.Load_Z.dTarget);
	UpdateQuery_auto(1001, L"LD01_EV01_RECV_Z_SPEED",		sm.MotorPos_1->EL_Convryor.Load_Z.dSpeed);
	UpdateQuery_auto(1002, L"LD01_EV01_SEND_Z_POSITION",	sm.MotorPos_1->EL_Convryor.Unload_Z.dTarget);
	UpdateQuery_auto(1003, L"LD01_EV01_SEND_Z_SPEED",		sm.MotorPos_1->EL_Convryor.Unload_Z.dSpeed);

	UpdateQuery_auto(1020, L"LD01_TR01_WAIT_Y_POSITION",	sm.MotorPos_1->Glass_TR.Wait_Y.dTarget);
	UpdateQuery_auto(1021, L"LD01_TR01_WAIT_Y_SPEED",		sm.MotorPos_1->Glass_TR.Wait_Y.dSpeed);
	UpdateQuery_auto(1022, L"LD01_TR01_WAIT_Z_POSITION",	sm.MotorPos_1->Glass_TR.Wait_Z.dTarget);
	UpdateQuery_auto(1023, L"LD01_TR01_WAIT_Z_SPEED",		sm.MotorPos_1->Glass_TR.Wait_Z.dSpeed);
	UpdateQuery_auto(1024, L"LD01_TR01_RECV_Y_POSITION",	sm.MotorPos_1->Glass_TR.Load_Y.dTarget);
	UpdateQuery_auto(1025, L"LD01_TR01_RECV_Y_SPEED",		sm.MotorPos_1->Glass_TR.Load_Y.dSpeed);
	UpdateQuery_auto(1026, L"LD01_TR01_RECV_Z_POSITION",	sm.MotorPos_1->Glass_TR.Load_Z.dTarget);
	//UpdateQuery_auto(1026, L"LD01_TR01_RECV_Z_POSITION",	104.2);
	UpdateQuery_auto(1027, L"LD01_TR01_RECV_Z_SPEED",		sm.MotorPos_1->Glass_TR.Load_Z.dSpeed);
	UpdateQuery_auto(1028, L"LD01_TR01_SEND_G_Y_POSITION",	sm.MotorPos_1->Glass_TR.Unload_Glass_Y.dTarget);
	UpdateQuery_auto(1029, L"LD01_TR01_SEND_G_Y_SPEED",		sm.MotorPos_1->Glass_TR.Unload_Glass_Y.dSpeed);
	UpdateQuery_auto(1030, L"LD01_TR01_SEND_G_Z_POSITION",	sm.MotorPos_1->Glass_TR.Unload_Glass_Z.dTarget);
	UpdateQuery_auto(1031, L"LD01_TR01_SEND_G_Z_SPEED",		sm.MotorPos_1->Glass_TR.Unload_Glass_Z.dSpeed);
	UpdateQuery_auto(1032, L"LD01_TR01_SEND_P_Y_POSITION",	sm.MotorPos_1->Glass_TR.Unload_Paper_Y.dTarget);
	UpdateQuery_auto(1033, L"LD01_TR01_SEND_P_Y_SPEED",		sm.MotorPos_1->Glass_TR.Unload_Paper_Y.dSpeed);
	UpdateQuery_auto(1034, L"LD01_TR01_SEND_P_Z_POSITION",	sm.MotorPos_1->Glass_TR.Unload_Paper_Z.dTarget);
	UpdateQuery_auto(1035, L"LD01_TR01_SEND_P_Z_SPEED",		sm.MotorPos_1->Glass_TR.Unload_Paper_Z.dSpeed);
	
	UpdateQuery_auto(1550, L"BCR_ON_OFF",					1);
	//UpdateQuery_auto(2000, L"BCR_ON_OFF_DSDFD", 1);

	g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[eEQP_SCT], "ECID", "UPDATE");
}


BOOL Is_PPID(int iPPID, CStringA strPPID) {
	if (g_db == nullptr)
		return FALSE;


	return TRUE;
}

BOOL Is_PPID_REPORT(int iPPID, CStringA strPPID) {
	if (g_db == nullptr)
		return FALSE;

	return TRUE;
}

BOOL IsPPID_REPORT(int iPPID) {
	if (!g_db) return FALSE;

	eEQP_TYPE _type = eEQP_SCT;

	CADORecordset rs(g_db.get());
	CString query = Format(_T("SELECT * FROM PPID_%s WHERE PPID_NUMBER = %d"), CONV_T(lpEQP[_type]), iPPID);

	if (!rs.Open(query, CADORecordset::openQuery)) {
		return FALSE;
	}

	if (rs.GetRecordCount() == 0) {
		return FALSE;
	}

	rs.MoveFirst();
	int iReport = 0;
	while (!rs.IsEOF()) {
		rs.GetFieldValue(_T("REPORT"), iReport);
		rs.MoveNext();
	}

	return iReport != 0;
}

BOOL IsPPID(LPCTSTR lpPPID, int iPPID_NUMBER) {
	if (!g_db) return FALSE;
	
	eEQP_TYPE _type = eEQP_SCT;


	CADORecordset rs(g_db.get());
	CString query = Format(_T("SELECT * FROM PPID_%s WHERE PPID = '%s' AND PPID_NUMBER = %d"), CONV_T(lpEQP[_type]), lpPPID, iPPID_NUMBER);

	if (!rs.Open(query, CADORecordset::openQuery)) {
		return FALSE;
	}

	if (rs.GetRecordCount() == 0) {
		return FALSE;
	}

	rs.Close();

	return TRUE;
}

BOOL IsPPID(LPCTSTR lpPPID) {
	if (!g_db) return FALSE;

	eEQP_TYPE _type = eEQP_SCT;

	CADORecordset rs(g_db.get());
	CString query = Format(_T("SELECT * FROM PPID_%s WHERE PPID = '%s'"), CONV_T(lpEQP[_type]), lpPPID);

	if (!rs.Open(query, CADORecordset::openQuery)) {
		return FALSE;
	}

	if (rs.GetRecordCount() == 0) {
		return FALSE;
	}

	rs.Close();

	return TRUE;
}

BOOL IsPPID(int iPPID_NUMBER) {
	if (!g_db) return FALSE;

	eEQP_TYPE _type = eEQP_SCT;

	CADORecordset rs(g_db.get());
	CString query = Format(_T("SELECT * FROM PPID_%s WHERE PPID_NUMBER = %d"), CONV_T(lpEQP[_type]), iPPID_NUMBER);

	if (!rs.Open(query, CADORecordset::openQuery)) {
		return FALSE;
	}

	if (rs.GetRecordCount() == 0) {
		return FALSE;
	}

	rs.Close();

	return TRUE;
}

void Update_PPID_PARAM(int iPPID)
{
	if (g_db == nullptr)
		return;

    const T_MDI_RECIPE& recipe = *(g_recipes.GetRecipeData(iPPID));

	auto isPPID = [&](int iPPID) {
		CADORecordset rs(g_db.get());
		if (!rs.Open(Format(L"SELECT * FROM PPID_%s WHERE PPID_NUMBER = %d", CONV_T(lpEQP[eEQP_SCT]), iPPID), CADORecordset::openQuery)) {
			rs.Close();
			return FALSE;
		}

		if (rs.GetRecordCount() <= 0) {
			rs.Close();
			return FALSE;
		}
		
		rs.Close();
		return TRUE;
	};

	auto IsPPID_PARAM = [&](int iPPID, int iPPID_PARAM) {
		CADORecordset rs(g_db.get());
		if (!rs.Open(Format(L"SELECT * FROM PPID_PARAM_%s WHERE PPID_NUMBER = %d AND PPID_PARAM_NO = %d", CONV_T(lpEQP[eEQP_SCT]), iPPID, iPPID_PARAM), CADORecordset::openQuery)) {
			rs.Close();
			return FALSE;
		}

		if (rs.GetRecordCount() <= 0) {
			rs.Close();
			return FALSE;
		}

		rs.Close();

		return TRUE;
	};

	auto UpdateQuery = [&](int iPPID, CString strPPID, int iPPID_PARAM, const CString& _name, auto _val) -> bool
	{
		CString query;
		//<< 2024.06.25 LBH - PPID Param 생성/수정.
		BOOL bPPID_PARAM_Exist = IsPPID_PARAM(iPPID, iPPID_PARAM);
		if (bPPID_PARAM_Exist) {
			query = Format(_T("UPDATE PPID_PARAM_%s SET PPID = '%s', P_PARAM_NAME = '%s', P_PARAM_VALUE = '%s' WHERE PPID_NUMBER = %d AND PPID_PARAM_NO = %d")
				, CString(lpEQP[eEQP_SCT]), strPPID, _name, ToCString(_val), iPPID, iPPID_PARAM);
		}
		else {
			query = Format(_T("INSERT INTO PPID_PARAM_%s VALUES ( %d, '%s', %d, '%s', '%s')")
				, CString(lpEQP[eEQP_SCT]), iPPID, strPPID, iPPID_PARAM, _name, ToCString(_val));
		}

		return g_db->Execute(query) == TRUE;
		//>>
	};

	{
		BOOL bPPID_Exist = isPPID(iPPID);

		CString query;

		//<< 2024.06.25 LBH - PPID 생성/수정.
		if (bPPID_Exist) {
			query = Format(_T("UPDATE PPID_%s SET PPID = '%s' WHERE PPID_NUMBER LIKE %d")
				, CString(lpEQP[eEQP_SCT]), recipe.recipeName, iPPID);
		}
		else {
			query = Format(_T("INSERT INTO PPID_%s VALUES (%d, '%s', %d)")
				, CString(lpEQP[eEQP_SCT]), iPPID, recipe.recipeName, 0);
		}

		g_db->Execute(query);
	}

	UpdateQuery(iPPID, recipe.recipeName, 1, L"GLASS_THICKNESS", 							recipe.ppbox.dGlassThickness);
	UpdateQuery(iPPID, recipe.recipeName, 2, L"PAPER_THICKNESS", 							recipe.ppbox.dPaperThickness);

	UpdateQuery(iPPID, recipe.recipeName, 258, L"LD01_CV01_SPEED", 							recipe.conveyorSpeed[eCONVEYOR_LD_1].dHighSpeed);
	UpdateQuery(iPPID, recipe.recipeName, 259, L"LD01_CV02_SPEED", 							recipe.conveyorSpeed[eCONVEYOR_LD_2].dHighSpeed);
	UpdateQuery(iPPID, recipe.recipeName, 260, L"LD01_CV03_SPEED", 							recipe.conveyorSpeed[eCONVEYOR_EL].dHighSpeed);
	UpdateQuery(iPPID, recipe.recipeName, 261, L"LD01_CV04_SPEED", 							recipe.conveyorSpeed[eCONVEYOR_ULD_1].dHighSpeed);
	UpdateQuery(iPPID, recipe.recipeName, 262, L"LD01_CV05_SPEED", 							recipe.conveyorSpeed[eCONVEYOR_ULD_2].dHighSpeed);

	CIPVar var;
	var.SetChildItem("PPID", recipe.recipeName);
	var.SetChildItem("PPID_NUMBER", iPPID);
	g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[eEQP_SCT], "PPID", "UPDATE", var);
}

// LD
LPCTSTR moduleNameStep[20] =
{
	_T("LD Conveyor 1"),
    _T("LD Conveyor 2"),
    _T("LD Conveyor 3 E/V"),
    _T("LD Conveyor 4"),
    _T("LD Conveyor 5"),
    _T("LD Transfer"),
    
};

T_POS_PARAM PPBoxEV[] = {
	{_T("Load Z"),						eMOTOR_GLASS_BOX_EV_Z, RGB_SKY_LIGHT }	,
	{_T("Unload Z"),					eMOTOR_GLASS_BOX_EV_Z, RGB_SKY_LIGHT }	,
};

T_POS_PARAM GlassLoadTrasnfer[] = {

	{_T("Wait Y"),					eMOTOR_GLASS_TRANSFER_Y, RGB_SKY_LIGHT }	,
	{_T("Wait Z"),		            eMOTOR_GLASS_TRANSFER_Z, RGB_SKY_LIGHT }	,

	{_T("Load Y"),					eMOTOR_GLASS_TRANSFER_Y, RGB_MINT_DARK }	,
	{_T("Load Z"),		            eMOTOR_GLASS_TRANSFER_Z, RGB_MINT_DARK }	,

    {_T("Unload Glass Y"),			eMOTOR_GLASS_TRANSFER_Y, RGB_SKY_LIGHT }	,
    {_T("Unload Glass Z"),		    eMOTOR_GLASS_TRANSFER_Z, RGB_SKY_LIGHT }	,

    {_T("Unload Paper Y"),			eMOTOR_GLASS_TRANSFER_Y, RGB_MINT_DARK }	,
    {_T("Unload Paper Z"),		    eMOTOR_GLASS_TRANSFER_Z, RGB_MINT_DARK }	,
};

// DB
// Message

std::pair<bool, std::vector<T_MESSAGE>> LoadMSG(const eEQP_TYPE _type, const eMSG_TYPE _eMSG_TYPE, BOOL _bConfirmCheck) {
	std::vector<T_MESSAGE> msgs;

	if (!g_db) return { false, msgs };

	CADORecordset rs(g_db.get());
	if (!rs.Open(Format(L"SELECT * FROM %s_%s ORDER BY TIME DESC", CONV_T(lpMSG_TYPE[_eMSG_TYPE]), CONV_T(lpEQP[_type])), CADORecordset::openQuery)) {
		//g_logEQP.WriteErrorLog(_T("%s Section Recordset Open Error"), Table);
		return { false, msgs };
	}

	int nMsg = rs.GetRecordCount();
	if (nMsg == 0) {
		return { false, msgs };
	}

	rs.MoveFirst();
	while (!rs.IsEOF()) {
		T_MESSAGE msg;

		int temp = 0;
		rs.GetFieldValue(L"CONFIRM", temp);	msg.bConfirm = temp;

		// UI에서는 Confirm 안된 것들만 보여주면 됨.
		if (!_bConfirmCheck || !msg.bConfirm) {	// KES 수정 2024.07.21 - _bConfirmCheck 체크 추가
			GetDBCharString(rs, L"OCCURRED", msg.szOCCURRED, sizeof(msg.szOCCURRED));
			GetDBCharString(rs, L"MODULEID", msg.szMODULEID, sizeof(msg.szMODULEID));
			GetDBCharString(rs, L"ID", msg.szID, sizeof(msg.szID));
			GetDBCharString(rs, L"MSG", msg.szMSG, sizeof(msg.szMSG));
			GetDBCharString(rs, L"TIME", msg.szTIME, sizeof(msg.szTIME));

			msgs.push_back(msg);
		}
		rs.MoveNext();
	}
	rs.Close();

	return { true, msgs };
}
// Lot
std::pair<bool, T_LOT_INFO> LoadLotInfo(const eEQP_TYPE _type, CString strCSTID) {
	T_LOT_INFO _LotInfo;
	memset(&_LotInfo, 0x00, sizeof(_LotInfo));

	if (!g_db) return { false, _LotInfo };

	CADORecordset rs(g_db.get());

	{
		if (!rs.Open(Format(L"SELECT * FROM LOT_INFO_%s WHERE CSTID = '%s'", CONV_T(lpEQP[_type]), strCSTID), CADORecordset::openQuery)) {
			//g_logEQP.WriteErrorLog(_T("%s Section Recordset Open Error"), Table);
			return { false, _LotInfo };
		}

		int nLot = rs.GetRecordCount();
		if (nLot == 0) {
			return { false, _LotInfo };
		}

		// Cassette Info
		rs.MoveFirst();
		//CSTID, TYPE, PPID, PRODUCT, STEPID, S_COUNT, C_COUNT, PORTNO, REPLYCODE, REPLYTEXT
		auto& CstInfo = _LotInfo.CassetteInfo;

		GetDBCharString(rs, L"CSTID", CstInfo.szID, sizeof(CstInfo.szID));
		GetDBCharString(rs, L"TYPE", CstInfo.szTYPE, sizeof(CstInfo.szTYPE));
		GetDBCharString(rs, L"PPID", CstInfo.szPPID, sizeof(CstInfo.szPPID));
		GetDBCharString(rs, L"PRODUCT", CstInfo.szPRODUCT, sizeof(CstInfo.szPRODUCT));
		GetDBCharString(rs, L"STEPID", CstInfo.szSTEPID, sizeof(CstInfo.szSTEPID));
		GetDBCharString(rs, L"S_COUNT", CstInfo.szS_COUNT, sizeof(CstInfo.szS_COUNT));
		GetDBCharString(rs, L"C_COUNT", CstInfo.szC_COUNT, sizeof(CstInfo.szC_COUNT));
		GetDBCharString(rs, L"PORTNO", CstInfo.szPORTNO, sizeof(CstInfo.szPORTNO));
		GetDBCharString(rs, L"REPLYCODE", CstInfo.szREPLYCODE, sizeof(CstInfo.szREPLYCODE));
		GetDBCharString(rs, L"REPLYTEXT", CstInfo.szREPLYTEXT, sizeof(CstInfo.szREPLYTEXT));
	}
	rs.Close();

	{ // Tray Info
		//TRAYNO, CSTID, TRAYID, CELLQTY
		if (!rs.Open(Format(L"SELECT * FROM TRAY_INFO_%s WHERE CSTID = '%s' ORDER BY TRAYNO", CONV_T(lpEQP[_type]), CString(_LotInfo.CassetteInfo.szID)), CADORecordset::openQuery)) {
			//g_logEQP.WriteErrorLog(_T("%s Section Recordset Open Error"), Table);
			return { false, _LotInfo };
		}

		_LotInfo.nTray = _min(rs.GetRecordCount(), MAX_TRAY);
		if (_LotInfo.nTray == 0) {
			return { false, _LotInfo };
		}

		rs.MoveFirst();
		int iTray = 0;
		while (!rs.IsEOF()) {
			auto& Tray = _LotInfo.TRAY_INFOs[iTray++];

			GetDBCharString(rs, L"TRAYID", Tray.szTRAY_ID, sizeof(Tray.szTRAY_ID));

			GetDBCharString(rs, L"CELLQTY", Tray.szCELLQTY, sizeof(Tray.szCELLQTY));
			rs.MoveNext();

			if (iTray >= _LotInfo.nTray)
				break;
		}
		rs.Close();
	}

	{ // Cell Info
		for (int iTray = 0; iTray < _LotInfo.nTray; iTray++) {
			auto& Tray = _LotInfo.TRAY_INFOs[iTray];
			//CELLNO, CSTID, TRAYID, CELLID, LOCATIONNO, JUDGE, REASONCODE
			if (!rs.Open(Format(L"SELECT * FROM CELL_INFO_%s WHERE CSTID = '%s'and  TRAYID = '%s'ORDER BY CELLNO", CONV_T(lpEQP[_type]), CString(_LotInfo.CassetteInfo.szID), CString(Tray.szTRAY_ID)), CADORecordset::openQuery)) {
				//g_logEQP.WriteErrorLog(_T("%s Section Recordset Open Error"), Table);
				Tray.nCell = 0; // Empty Tray
				return { true, _LotInfo };
			}

			Tray.nCell = _min(rs.GetRecordCount(), MAX_CELL);
			// 언로더 빈 Tray의 경우 Cell이 없을수 있음.
			if (Tray.nCell == 0) {
				return { true, _LotInfo };
			}

			rs.MoveFirst();
			int iCell = 0;
			while (!rs.IsEOF()) {
				auto& Cell = Tray.CELL_INFOs[iCell++];
				
				GetDBCharString(rs, L"CSTID",		Cell.szCST_ID,		sizeof(Cell.szCST_ID));
				GetDBCharString(rs, L"CELLID",		Cell.szCELLID,		sizeof(Cell.szCELLID));
				GetDBCharString(rs, L"LOCATIONNO",	Cell.szLOCATIONNO,	sizeof(Cell.szLOCATIONNO));
				GetDBCharString(rs, L"JUDGE",		Cell.szJUDGE,		sizeof(Cell.szJUDGE));
				GetDBCharString(rs, L"REASONCODE",	Cell.szREASONCODE,	sizeof(Cell.szREASONCODE));
				Cell.bMaterial = TRUE;
				rs.MoveNext();

				if (iCell >= Tray.nCell)
					break;
			}
			rs.Close();
		}
	}

	return { true, _LotInfo };
}

void DeleteLotInfo(const eEQP_TYPE _type, CString strCstID) {
	if (!g_db) return;

	CString query = Format(L"DELETE FROM LOT_INFO_%s WHERE CSTID = '%s'", CONV_T(lpEQP[_type]), strCstID);
	g_db->Execute(query);

	query = Format(L"DELETE FROM TRAY_INFO_%s WHERE CSTID = '%s'", CONV_T(lpEQP[_type]), strCstID);
	g_db->Execute(query);

	query = Format(L"DELETE FROM CELL_INFO_%s WHERE CSTID = '%s'", CONV_T(lpEQP[_type]), strCstID);
	g_db->Execute(query);
}

void DeleteTrayInfo(const eEQP_TYPE _type, CString strTrayID) {
	if (!g_db) return;

	CString query = Format(L"DELETE FROM TRAY_INFO_%s WHERE TRAYID = '%s'", CONV_T(lpEQP[_type]), strTrayID);
	g_db->Execute(query);

	query = Format(L"DELETE FROM CELL_INFO_%s WHERE TRAYID = '%s'", CONV_T(lpEQP[_type]), strTrayID);
	g_db->Execute(query);
}

void DeleteCellInfo(const eEQP_TYPE _type, CString strCellID) {
	if (!g_db) return;

	CString query = Format(L"DELETE FROM CELL_INFO_%s WHERE CELLID = '%s'", CONV_T(lpEQP[_type]), strCellID);
	g_db->Execute(query);
}
//>>

void ManualStop() {
	//<< MCC
	sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI01].A[eMCC_ACTION_LD_LI_01_MANUAL_STOP] = 1;
	sm.MCC_EQP->Layer[eMCC_LAYER_LD_LI02].A[eMCC_ACTION_LD_LI_02_MANUAL_STOP] = 1;
	sm.MCC_EQP->Layer[eMCC_LAYER_LD_LE01].A[eMCC_ACTION_LD_LE_01_MANUAL_STOP] = 1;
	sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO01].A[eMCC_ACTION_LD_LO_01_MANUAL_STOP] = 1;
	sm.MCC_EQP->Layer[eMCC_LAYER_LD_LO02].A[eMCC_ACTION_LD_LO_02_MANUAL_STOP] = 1;
	sm.MCC_EQP->Layer[eMCC_LAYER_LD_TR01].A[eMCC_ACTION_LD_TR_01_MANUAL_STOP] = 1;
	//>>

	SetAutoStop();
}
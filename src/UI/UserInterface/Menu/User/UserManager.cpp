#include "pch.h"
#include "UserManager.h"
#include "UI.h"
// User Grade 
const static char keys[] = {0x3f, 0x8a, 0x13, 0xb6, 0x83 } ;

LPTSTR lpUserGrade []= {
	_T("None"),
	_T("Operator"),
	_T("Equipment"),
	_T("Administrator"),
};

CUserManager::CUserManager(void) {
}

CUserManager::~CUserManager(void) {
}

BOOL CUserManager::Initialize ( void ) {
	m_tUserData.nUserData = 0;
	sm.DeviceParameter->eUserGrade = eUSER_NONE;
	m_Profile.Load( _T("../cfg/USER_DATA.ini") );
	LoadUserData();
	return TRUE;
}

BOOL CUserManager::LoadUserData ( void ){
	m_tUserData.nUserData = 0;
	memset(m_tUserData.UserItem, 0x00, sizeof(m_tUserData.UserItem[0])*MAX_USER_DATA );

	CProfileSection&	section	= m_Profile.GetSection(_T("UserData"));
	TList<CProfileSection>&	subSections		= section.GetSubSections();
	for ( int iIndex = 0 ; iIndex < subSections.N(); ++iIndex ){
		CStringA strPass;
		CString	strUserID	=	subSections[iIndex].GetItemValue(_T("UserID"),_T(""));
		CString strUserPS	=	subSections[iIndex].GetItemValue(_T("UserPass"), _T(""));
		Decrypt(CStringA(strUserPS), strPass);
		USER_DATA userData;

		_tcscpy (m_tUserData.UserItem[iIndex].szID, strUserID );
		_tcscpy (m_tUserData.UserItem[iIndex].szPass, CString(strPass));
		m_tUserData.UserItem[iIndex].uGrade	=	(eUSER_GRADE)subSections[iIndex].GetItemValue(_T("UserGrade"), -1);
		
		m_tUserData.nUserData++;
	}
	return TRUE;
}
BOOL CUserManager::AddUserData ( LPCTSTR pszID, LPCTSTR pszPass, int iUserGrade ){
	if ( !IDCheck( pszID ) || m_tUserData.nUserData >= MAX_USER_DATA ) {
		// pszID 
		return FALSE;
	}

	if (sm.DeviceParameter->eUserGrade < iUserGrade) {
		IPMessage(g_st[IDS_USER_MANAGER_ADD_USER_HIGHER_THAN_LOGIN_USER]);
		return FALSE;
	}

	_tcscpy (m_tUserData.UserItem[m_tUserData.nUserData].szID, pszID );
	_tcscpy (m_tUserData.UserItem[m_tUserData.nUserData].szPass, pszPass );
	m_tUserData.UserItem[m_tUserData.nUserData].uGrade	=	(eUSER_GRADE)iUserGrade;
	
	
	m_tUserData.nUserData++;

	SaveUserData();
	return TRUE;
}

BOOL CUserManager::ModifyUserData(int iSelectUser, LPCTSTR pszPass, int iUserGrade) {
	if ((iSelectUser < 0 ) || (iSelectUser >= m_tUserData.nUserData))
		return FALSE;

	if (sm.DeviceParameter->eUserGrade < iUserGrade) {
		IPMessage(g_st[IDS_USER_MANAGER_EDIT_USER_HIGHER_THAN_LOGIN_USER]);
		return FALSE;
	}

	if ((m_tUserData.nUserData == 1) && (iUserGrade != eUSER_ADMIN)) {
		IPMessage(g_st[IDS_USER_MANAGER_USER_ADMIN]);
		return FALSE;
	}

	_tcscpy ( m_tUserData.UserItem[iSelectUser].szPass, pszPass );
	m_tUserData.UserItem[iSelectUser].uGrade = (eUSER_GRADE)iUserGrade;
	
	SaveUserData();
	
	return TRUE;
}

BOOL CUserManager::DeleteUserData ( int iSelectUser ){
	BOOL bResult = FALSE;
	if (iSelectUser >= 0 && iSelectUser < m_tUserData.nUserData) {
		if (sm.DeviceParameter->eUserGrade < m_tUserData.UserItem[iSelectUser].uGrade) {
			IPMessage(g_st[IDS_USER_MANAGER_DELETE_USER_HIGHER_THAN_LOGIN_USER]);
			return FALSE;
		}

		for (int i = 0; i < m_tUserData.nUserData; i++) {
			if (i <= iSelectUser)
				continue;

			m_tUserData.UserItem[i-1] = m_tUserData.UserItem[i];
		}
		
		memset(&m_tUserData.UserItem[m_tUserData.nUserData], 0x00, sizeof(m_tUserData.UserItem[0]));
		m_tUserData.nUserData--;

		SaveUserData();
	}
	

	return bResult;
}


BOOL CUserManager::SaveUserData ( void ){
	CProfileSection&	section	= m_Profile.GetSection(_T("UserData"));
	TList<CProfileSection>&		subSections		= section.GetSubSections();
	subSections.DeleteAll();

	for ( int iIndex = 0 ; iIndex < m_tUserData.nUserData; iIndex++ ){
		CStringA strPass;
		Encrypt(CStringA(m_tUserData.UserItem[iIndex].szPass), strPass);
		CProfileSection&		subSection			= section.GetSubSection(Format(_T("User %d"),iIndex+1));
		subSection.SetItemValue( _T("UserID"),			m_tUserData.UserItem[iIndex].szID );
		subSection.SetItemValue( _T("UserPass"),		CString(strPass));
		subSection.SetItemValue( _T("UserGrade"),		m_tUserData.UserItem[iIndex].uGrade);
	}
	LoadUserData();
	m_Profile.Save();
	return TRUE;
}

BOOL CUserManager::IDCheck ( LPCTSTR pszWantID ){
	BOOL bResult = TRUE;
	for ( int iIndex = 0 ; iIndex < m_tUserData.nUserData; iIndex++ ){
		if (CString(pszWantID).Compare( m_tUserData.UserItem[iIndex].szID ) == 0){
			bResult = FALSE;
			break;
		}
	}
	return bResult;
}

BOOL CUserManager::IDPassCheck(LPCTSTR pszID, LPCTSTR pszPass ) {
	BOOL bResult = FALSE;
	for (int iIndex = 0 ;iIndex < m_tUserData.nUserData; iIndex++ ) {
		if ((CString(pszID).Compare( m_tUserData.UserItem[iIndex].szID ) == 0) &&
			((CString(pszPass).Compare( m_tUserData.UserItem[iIndex].szPass ) == 0))) {
			bResult = TRUE;
			break;
		}
	}

	return bResult;
}

BOOL CUserManager::AdminPassCheck(LPCTSTR pszPass) {
	BOOL bResult = FALSE;

	if (CString(pszPass).CompareNoCase(SUPPER_USER_PASS) == 0) {
		return TRUE;
	}

	for (int iIndex = 0 ;iIndex < m_tUserData.nUserData; iIndex++ ) {
		if (m_tUserData.UserItem[iIndex].uGrade >= eUSER_ADMIN) {
			if ((CString(pszPass).Compare( m_tUserData.UserItem[iIndex].szPass) == 0)) {
				bResult = TRUE;
				break;
			}
		}
	}
	return bResult;
}

BOOL CUserManager::UserLogin( LPCTSTR pszID, LPCTSTR pszPass ){
	BOOL bResult = FALSE;
	for ( int iIndex = 0 ;iIndex < m_tUserData.nUserData; iIndex++ ){
		if ((CString(pszID).Compare( m_tUserData.UserItem[iIndex].szID ) == 0) &&
			((CString(pszPass).Compare( m_tUserData.UserItem[iIndex].szPass ) == 0))) {
			sm.DeviceParameter->eUserGrade = m_tUserData.UserItem[iIndex].uGrade;
			m_CurrentUser = m_tUserData.UserItem[iIndex];

			_tcscpy_s(sm.DeviceParameter->szCurUserID, m_CurrentUser.szID);
			bResult = TRUE;
			break;
		}
	}
	if (!bResult) {
		if ((CString(SUPPER_USER_PASS).CompareNoCase(pszPass) == 0)
			|| (CString(SUPPER_USER_PASS).CompareNoCase(pszID) == 0)) {
			_tcscpy_s(m_CurrentUser.szID, _T("QRS"));
			sm.DeviceParameter->eUserGrade = eUSER_SUPER;			
			_tcscpy_s(sm.DeviceParameter->szCurUserID, _T("QRS"));
			m_CurrentUser.uGrade = eUSER_SUPER;
			return TRUE;
		}
	}
	return bResult;
}

void CUserManager::Encrypt(LPCSTR lpstr, CStringA& strResult) {
	CStringA str(lpstr);
	if (str.IsEmpty())
		return;
	char* pass = new char [str.GetLength()+1];
	memset(pass, 0, str.GetLength()+1);
	//const static char keys[] = {0x3f, 0x8a, 0x13, 0xb6, 0x83 } ;

	strcpy(pass, (const char*)(LPCSTR)str);
	
	for(int i = 0; i < str.GetLength(); i++ ) {
		pass[i] ^= keys[i % countof(keys)];
		
		if (i)
			strResult += "-";
		strResult += FormatA("%02x", (unsigned char)pass[i]);
	}

	delete [] pass;
}

void CUserManager::Decrypt(LPCSTR lpstr, CStringA& strResult) {
	CString str(lpstr);
	if (str.IsEmpty())
		return;
	
	char* pass = new char [str.GetLength()+1];
	memset(pass, 0, str.GetLength()+1);
	//const static char keys[] = {0x3f, 0x8a, 0x13, 0xb6, 0x83 } ;

	CStrings strs;
	SplitString(str, CString(_T("-")), strs);

	for (int i = 0; i < strs.N(); i++ ) {
		int iTemp = _tcstoul(strs[i], NULL, 16);
		iTemp ^= keys[i % countof(keys)];
		unsigned char szTemp = iTemp;
		strResult += szTemp;
	}

	delete [] pass;
}

CString CUserManager:: GetUserGradeText(int iGrade) {
	if (iGrade == eUSER_SUPER)
		return _T("SuperUser");

	if (iGrade < 0 || iGrade > countof(lpUserGrade))
		iGrade = 0;

	return lpUserGrade[iGrade];
}

BOOL CUserManager::CheckAdminUser(CString strPass) {
	BOOL bResult = FALSE;
	for (int iIndex = 0; iIndex < m_tUserData.nUserData; iIndex++) {
		if (((strPass.Compare(m_tUserData.UserItem[iIndex].szPass) == 0))) {
			if (m_tUserData.UserItem[iIndex].uGrade >= eUSER_ADMIN) {
				bResult = TRUE;
				break;
			}
		}
	}
	if (!bResult) {
		if (CString(SUPPER_USER_PASS).CompareNoCase(strPass) == 0) {
			bResult =  TRUE;
		}
	}

	return bResult;
}
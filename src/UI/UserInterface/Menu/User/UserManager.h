#pragma once
#include "misc\Profile.h"

#pragma warning (disable : 4309)
#pragma warning(disable : 4996)

#define MAX_USER_DATA		100
#define	SUPPER_USER_PASS	_T("QRS")


typedef struct _USER_ITEM {
	wchar_t			szID[256];
	wchar_t			szPass[256];
	eUSER_GRADE		uGrade;

	_USER_ITEM() {
		memset(szID, 0x00, sizeof(szID));
		memset(szPass, 0x00, sizeof(szPass));
		uGrade = eUSER_NONE;
	}

} USER_ITEM;

typedef struct _USER_DATA {
	USER_ITEM	UserItem[MAX_USER_DATA];
	int			nUserData;
} USER_DATA;

class CUserManager {
public:
	CUserManager(void);
	~CUserManager(void);

private :
	CProfile						m_Profile;
	USER_ITEM						m_CurrentUser;
	USER_DATA						m_tUserData;
	
	BOOL							LoadUserData			( void );
	BOOL							SaveUserData			( void );
	
public: 
	BOOL							Initialize				( void );
	BOOL							IDCheck					( LPCTSTR pszWantID );
	BOOL							UserLogin				( LPCTSTR pszID, LPCTSTR pszPass );
	BOOL							IDPassCheck				( LPCTSTR pszID, LPCTSTR pszPass );
	BOOL							AdminPassCheck			( LPCTSTR pszPass );
	void							UserLogOut				( void ) {
		memset(m_CurrentUser.szID, 0x00, sizeof(m_CurrentUser.szID)); 
		memset(m_CurrentUser.szPass, 0x00, sizeof(m_CurrentUser.szPass)); 
		m_CurrentUser.uGrade = eUSER_NONE; 
	};
	BOOL							AddUserData				( LPCTSTR pszID, LPCTSTR pszPass, int iUserGrade );
	BOOL							ModifyUserData			( int iSelectUser, LPCTSTR pszPass, int iUserGrade );
	BOOL							DeleteUserData			( int iSelectUser );
	int								GetUserGrade			( void ) {	return m_CurrentUser.uGrade;}
	USER_ITEM&						GetCurrentUserData		( void ) {	return m_CurrentUser;		}
	const USER_DATA					GetUserData				( void ) {	return m_tUserData;			}
	void							Encrypt					(LPCSTR lpstr = "", CStringA& strResult = CStringA(""));
	void							Decrypt					(LPCSTR lpstr = "", CStringA& strResult = CStringA(""));
	CString							GetUserGradeText		(int iGrade);
	CString							GetSuperPass			() { return SUPPER_USER_PASS; }
	BOOL							CheckAdminUser(CString strPass);
};

#pragma once

class CPopupMapWnd; // KES 추가 2024.3.21

class CPMWnd : public TIPCT<CFrameWnd>
{
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_IMAGE_AREA				,
	eS_PASSWORD					,
	eS_END						,
};

enum eCOMMAND {
	eC_NONE						= -1,
	eC_PASSWORD					,
	eC_OK						,
	eC_MODE_CHANGE				,
    eC_POPUP_MAP				,   // KES 추가 2024.3.21
	eC_DOOR_LOCK				,
	eC_DOOR_UNLOCK				,
	eC_END						,
};
	DECLARE_DYNCREATE(CPMWnd)
public:
	CPMWnd(CWnd* pwndParent = nullptr);
	virtual ~CPMWnd();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int			OnCreate						(LPCREATESTRUCT lpCreateStruct);
	afx_msg void		OnShowWindow					(BOOL bShow, UINT nStatus);
	afx_msg void		OnPaint							();
	afx_msg void		OnLButtonDown					(UINT nFlags, CPoint point);
	afx_msg void		OnLButtonUp						(UINT nFlags, CPoint point);
	afx_msg void		OnMouseMove						(UINT nFlags, CPoint point);
	afx_msg LRESULT		OnEditFinished					(WPARAM wParam, LPARAM lParam);
protected:
	CWnd*				m_pwndParent;
	CRect				m_rectS[eS_END],
						m_rectC[eC_END];

	int					m_eSelectControl;
	int					m_eEditEnd;
	int					m_eMouseOver;

	CImage				m_img;
	CString				m_strPassword;
	CString				m_strEditPassword;
private:
	CPopupMapWnd*       m_pWndPopupMap = nullptr;   // KES 추가 2024.3.21
protected:
	void				fn_ComputeArea();
	void				OnOK();
	BOOL				SyncData();

	DECLARE_IP()
	ip_handler BOOL	OnIPShow(CIPCommand& cmd);
	ip_handler BOOL	OnIPModeChange(CIPCommand& cmd);
	ip_handler BOOL	OnIPDoorRelease(CIPCommand& cmd);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
};



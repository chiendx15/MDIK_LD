#pragma once

class CTransferThetaMoveWnd : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_POSITION_X				,
	eS_POSITION_Y				,
	eS_POSITION_Z				,
	eS_POSITION_T				,
	eS_SPEED					,
	eS_THETA					,
	eS_PITCH					,
	eS_PITCH_SPEED				,
	eS_END						,
};

enum eCOMMAND {
	eC_NONE						= -1,
	eC_POSITION_X				,
	eC_POSITION_Y				,
	eC_POSITION_Z				,
	eC_POSITION_T				,
	eC_SPEED					,
	eC_THETA					,
	eC_PITCH					,
	eC_PITCH_SPEED				,
	eC_P_MOVE					,
	eC_N_MOVE					,
	eC_MOVE						,
	eC_CLOSE					,
	eC_END						,
};
DECLARE_DYNCREATE(CTransferThetaMoveWnd)
public:
	CTransferThetaMoveWnd							(CWnd*	pwndParent = nullptr);
	virtual				~CTransferThetaMoveWnd	();
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct			);
	afx_msg void		OnKillFocus				(CWnd* pNewWnd							);
	afx_msg void		OnShowWindow			(BOOL bShow, UINT nStatus				);
	afx_msg void		OnTimer					(UINT_PTR nIDEvent						);
	afx_msg BOOL		OnEraseBkgnd			(CDC* pDC								);
	afx_msg void		OnPaint					();
	afx_msg void		OnMouseMove				(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonDown			(UINT nFlags, CPoint point				);
	afx_msg void		OnLButtonUp				(UINT nFlags, CPoint point				);
protected:
	CWnd*				m_pwndParent;
public:
protected:
	CRect				m_rectS					[eS_END], 
						m_rectC					[eC_END];
	int					m_eSelectControl		;
	int					m_eMouseOver			;
	CString				strALogMsg, strALogPos;
	double				m_dSpeed;
	double				m_dTheta;
	double				m_dPitch;
	double				m_dPitchSpeed;

	double				m_dCurrentX;
	double				m_dCurrentY;
	double				m_dCurrentZ;
	double				m_dCurrentT;
public:	

protected:
	void				fn_ComputeArea			();
	void				ThetaMove(double dTheta = 0.,double dPitch = 0., double dPitchSpeed = 1.);
public:
	BOOL				Initialize				() { return TRUE; };
	afx_msg void OnClose();
};

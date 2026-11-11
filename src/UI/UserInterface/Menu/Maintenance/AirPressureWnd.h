#pragma once

#define INTERVAL_AIR 10000
#define MAX_COUNT 8
class CVacuumView : public CWnd {
	enum eS{
		eS_NONE,
		eS_FULL_AREA,
		eS_TITLE,
		eS_VAC_BLOW,
		eS_END = eS_VAC_BLOW + MAX_COUNT,
	};
	enum eC {
		eC_NONE,
		eC_VAC_BLOW,
		eC_END = eC_VAC_BLOW + MAX_COUNT,
	};
public:
	CVacuumView	(int iVac = 2, int iBlow = 1, CString strTitle = _T("Title"));
	virtual	~CVacuumView		();
protected:
	
	double	m_dVac1, m_dVac2, m_dBlow;	// 갯수를 받아와야 하기 때문에 이런식으로 처리하면 안될꺼 같음 
	CPoint	m_ptStart;					// 시작 지점
	Size	m_ptSize;					// 사이즈 
	CString m_strTitle;					// Title
	int		m_iVac, m_iBlow;			// Vac/Blow 갯수 (생성 시 받아와야함 )

	std::vector<double>	m_vecValue;
	CRect m_rectS[eS_END], m_rectC[eC_END];
public:
	void ComputArea();
	void SetSize(Size& size) {
		m_ptSize = size;
		ComputArea();
	}
	void SetStart(CPoint pt) {
		m_ptStart = pt;
		m_rectS[eS_FULL_AREA] = CRect(m_ptStart.x ,m_ptStart.y, m_ptStart.x + m_ptSize.width, m_ptStart.y+ m_ptSize.height);
		ComputArea();
	}
	// Vacuum/Blow Setting
	void SetVacBlow(int iVac, int iBlow) {
		m_iVac = iVac;
		m_iBlow = iBlow;
	}
	// Data Setting 
	void SetValue(int iIndex, double dValue) {
		m_vecValue[iIndex] = dValue;
	}
	// Title Setting 
	void SetTitle(CString strTitle) {
		m_strTitle = strTitle;
	}
	void SetVac1(double dVac) {
		m_dVac1 = dVac;
		
	}
	void SetVac2(double dVac) {
		m_dVac2 = dVac;
		
	}
	void SetBlow1(double dBlow) {
		m_dBlow = dBlow;
		
	}
	void SetAll(double dVac1, double dVac2, double dBlow) {
		SetVac1(dVac1);
		SetVac2(dVac2);
		SetBlow1(dBlow);
	}
	void Draw(CDC* pDC);
};

class CAirPressureWnd : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_TITLE_AREA				,
	eS_WORK_AREA				,
	eS_IMG_1					,
	eS_IMG_2					,
	eS_IMG_3					,
	eS_IMG_4					,
	eS_END						,
};

enum eCOMMAND {
	eC_NONE						= -1,
	eC_OK						,
	eC_CANCEL					,
	eC_END						,
};
enum eAIR_MONITOR {
	eAM_1,
	eAM_2,
	eAM_3,
	eAM_4,
	eAM_5,
	eAM_6,
	eAM_7,
	eAM_END,
};
DECLARE_DYNCREATE(CAirPressureWnd)
public:
	CAirPressureWnd								(CWnd*	pwndParent = nullptr);
	virtual				~CAirPressureWnd		();
	virtual BOOL		PreCreateWindow			(CREATESTRUCT& cs);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int			OnCreate				(LPCREATESTRUCT lpCreateStruct			);	
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
	int					m_eMouseOver			;
	int					m_eSelectControl		;

	CVacuumView			m_AirMonitor[eAM_END];
public:	

protected:
	void				fn_ComputeArea			();
public:
	BOOL				Initialize				() { return TRUE; };
};

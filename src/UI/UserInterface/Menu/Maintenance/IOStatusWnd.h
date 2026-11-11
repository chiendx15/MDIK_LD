#pragma once


#define __MAX_IO_ITEM__ 32
#define __MAX_IN_TAB__	1040 / __MAX_IO_ITEM__   
#define __MAX_OUT_TAB__	1040 / __MAX_IO_ITEM__   
#define INTERVAL_IO		10003

class CIOStatusWnd : public CFrameWndEx {
enum eSTATIC {
	eS_NONE						= -1,
	eS_FULL_AREA				= 0,
	eS_TAB_IN_AREA,
	eS_TAB_IN_IO_AREA,
	eS_TAB_OUT_AREA,
	eS_TAB_OUT_IO_AREA,
	eS_END						,
};

enum eCOMMAND {
	eC_NONE = -1,
	eC_OUT_RELEASE,
	eC_OUT_RELEASE_END = eC_OUT_RELEASE + __MAX_IO_ITEM__,
	eC_OK,
	eC_CANCEL, 
	eC_TAB_IN,
	eC_TAB_IN_END = eC_TAB_IN + __MAX_IN_TAB__,
	eC_IN_0,
	eC_IN_END = eC_IN_0 + __MAX_IO_ITEM__,
	eC_TAB_OUT,
	eC_TAB_OUT_END = eC_TAB_OUT + __MAX_OUT_TAB__,
	eC_OUT_0,
	eC_OUT_END = eC_OUT_0 + __MAX_IO_ITEM__,
	eC_NUMBER_TOGGLE			,
	eC_END						,
};
DECLARE_DYNCREATE(CIOStatusWnd)
public:
	CIOStatusWnd								(CWnd*	pwndParent = nullptr);
	virtual				~CIOStatusWnd			();
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
	int					m_eSelectControl, m_eSelectTabIn, m_eSelectTabOut;
	
	DWORD*				m_pIn_CRT;			//I/O In Struct 포인터 위치
	DWORD*				m_pOut_CRT;			//I/O Out Struct 포인터 위치
	DWORD*				m_pOut_NEW;			//I/O Out를 쓰기 위한 포인터 위치
	DWORD*				m_pOut_Rel;			//I/O Break Release 전용 포인터 위치

	BOOL				m_bToggle;			// TRUE 인 경우 정상적인 IO 표시 / FALSE 인 경우 IO 명칭을 10진수 번호로만 표시

    // KES 추가 2024.2.22 - UI에서 Spare IO만 32개 있는 Tap을 Blocking 하기 위한 Index
    // key: UI에 표시되는 Tap Index, value: IO.hxx와 매칭되는 Index 
    map<int, int>       m_mapIOIndexIn; 
    map<int, int>       m_mapIOIndexOut;
public:	

protected:
	void				fn_ComputeArea			();
    // KES 추가 2024.2.22 - UI에서 Spare IO만 32개 있는 Tap을 Blocking 하기 위한 Index를 만드는 함수
    void                fn_MakeIOIndexMap       (); 
public:
	BOOL				Initialize				() { return TRUE; };
	void				SyncData				(BOOL bStore);

};

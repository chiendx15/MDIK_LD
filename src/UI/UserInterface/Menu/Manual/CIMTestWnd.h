#pragma once


// CCIMTestWnd 프레임
class CCIMTestWnd : public CFrameWndEx
{
	enum eS {
		eS_NONE,
		eS_AREA,
		eS_END,
	};
	enum eC {
		eC_NONE,
		eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_350_CASSETTE_LOAD_REQUEST,
		eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_351_CASSETTE_LOAD_COMPLETE,
		eC_S6F11_CARRIER_PROCESS_CHANGE_CEID_262_CARRIER_INFORMATION_REQUEST,
		eC_S3F115_CARRIER_INFORMATION_DATA_RECV_WAIT,
		eC_S6F11_S6F11_CARRIER_PROCESS_CHANGE_CEID_257_CARRIER_RELEASE_COMPLETE,

		eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_352_CASSETTE_UNLOAD_REQUEST,
		eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_353_CASSETTE_UNLOAD_COMPLETE,

		//eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_351_CASSETTE_LOAD_COMPLETE,
		//eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_351_CASSETTE_LOAD_COMPLETE,
		//eC_S6F11_CASSETTE_STATUS_CHANGE_CEID_351_CASSETTE_LOAD_COMPLETE,
		eC_END,
	};
	DECLARE_DYNCREATE(CCIMTestWnd)
public:
	CCIMTestWnd(CWnd* pParent = nullptr);           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CCIMTestWnd();
	
protected:

	
	int m_eSelectControl;
	CRect m_rectS[eS_END], m_rectC[eC_END], m_rectClient;
	BOOL	m_bLinkCheck;						// TRUE 인 경우 PC 간 통신을 통해서 동작을 진행 / FALSE 인 경우 PC 간 통신에 상관없이 동작만 진행 ( ex)Transfer <-> PC2 Shift Table )

	void fn_ComputeArea();


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};



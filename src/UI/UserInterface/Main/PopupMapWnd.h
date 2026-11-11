#pragma once
#include <afxwin.h>

#include "Simulator/Simulator.h"

class CPopupMapWnd : public CFrameWnd
{
    enum eS{
        eS_NONE = -1,

		eS_MAP_AREA,
		
		eS_END,
	};

    enum eC{
        eC_NONE = -1,

		eC_REVERSE,
		
		eC_END,
	};

protected:
    CRect m_rectClient, m_rectS[eS_END], m_rectC[eC_END];
    int m_eSelectControl = eC_NONE;
    int m_eMouseOver;
    CSimulator* m_pSimulatorView;
    CImage m_imgMain;
    CImage m_imgMainRev;

    void ComputeArea();
    void SetSimulatorView();

public:
    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnClose();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnPaint();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


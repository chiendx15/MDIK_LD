#pragma once

#define INVERVAL_AXIS_STATUS_VIEW 9000
#define IDC_SCROLLBAR_AXIS_STATUS_VIEW 9001
// CAxisStatusView
typedef struct T_AXIS_STATUS_VIEW{
	CString strText;
	int iAxis = 0;
	double dSpeed = 0.;
	double dPos = 0.;
	double dTorque = 0.;
	BOOL bPLimit = FALSE;
	BOOL bNLimit = FALSE;
	BOOL bHome = FALSE;
	BOOL bOrg = FALSE;
	BOOL bHomming = FALSE;
	BOOL bServoOn = FALSE;
	BOOL bAlarm = FALSE;
	BOOL bInpos = FALSE;

	int iAxis_Sub = 0;
	double dSpeed_Sub = 0.;
	double dPos_Sub = 0.;
	double dTorque_Sub = 0.;
	BOOL bPLimit_Sub = FALSE;
	BOOL bNLimit_Sub = FALSE;
	BOOL bHome_Sub = FALSE;
	BOOL bOrg_Sub = FALSE;
	BOOL bServoOn_Sub = FALSE;
	BOOL bAlarm_Sub = FALSE;
	BOOL bInpos_Sub = FALSE;

	BOOL bSetSub = FALSE;
	BOOL bmm = FALSE; // TRUE : mm ; FALSE : deg
	CRect rect;
	BOOL bSelect = FALSE;
	DWORD dwTick = 0;
	int iFrontSize_Name = 0;
	int iFrontSize_Pos = 0;
	int iFrontSize_Spd = 0;
	int iFrontSize_Torque = 0;
}T_AXIS_STATUS_VIEW;

class CAxisStatusView : public CWnd
{
	enum eSTATIC
	{
		eS_NONE = -1,
		eS_AREA_MOTOR_STATUS,
		
		eS_END,
	};
	DECLARE_DYNAMIC(CAxisStatusView)

public:
	CAxisStatusView();
	virtual ~CAxisStatusView();

protected:
	const int m_iHeight = 50;
	int m_iScrollPos = 0;

	CRect m_rectClient, m_rectS[eS_END];
	std::vector<T_AXIS_STATUS_VIEW> m_tStatus;
	CScrollBar m_vScrollbar;

public:
	void ComputArea();
	const T_AXIS_STATUS_VIEW* GetSelectAxisData() const
	{
		for (auto& motor : m_tStatus)
		{
			if (motor.bSelect)
			{
				return &motor;
			}
		}
		return nullptr;
	}

protected:
	void DrawAxisStatus(CDC& pDC, T_AXIS_STATUS_VIEW& tStatus);
	void SetSelectIndex(int iIndex);
	int GetSelectIndex();

public:
	void ClearData();
	void InsertAxisItem(T_AXIS_STATUS_VIEW tData);
	void SetAxisData(int iIndex, double dSpeed, double dPos, double dTorque);
	void SetAxisSensor(int iItem, BOOL bOn, BOOL bOrg, BOOL bHomming, BOOL bAlarm, BOOL bPLimit, BOOL bNLimit, BOOL bInPos, BOOL bHome );
	void SetSubAxisData(int iIndex, double dSpeed, double dPos, double dTorque);
	void SetSubAxisSensor(int iItem, BOOL bOn, BOOL bAlarm, BOOL bPLimit, BOOL bNLimit, BOOL bInPos, BOOL bHome );

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow (BOOL bShow, UINT nStatus);
	afx_msg void OnPaint ();
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnTimer (UINT_PTR nIDEvent);
	afx_msg void OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel (UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScrollClipboard (CWnd* pClipAppWnd, UINT nSBCode, UINT nPos);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
};



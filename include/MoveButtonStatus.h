#pragma once

// Made By KES 2024.1.26

class CMoveButtonStatus {
private:
	enum eSTATUS {
		eSTATUS_READY,
		eSTATUS_RUN,
		eSTATUS_ALARM,
		eSTATUS_INTERLOCK,
		eSTATUS_COMPLETED,
	};

	typedef struct {
		eSTATUS eStatus; 
		int iStatusSetTime;
		CString strMsg;
		COLORREF crBK;
		COLORREF crText;
		vector<const T_POS*> vecPos;
	} T_STATUS;

	std::map<int, T_STATUS> Status;	// key: eCommand 번호

public:
	BOOL IsInterlock(const int& eCommand, CString strTitle = _T("")) {
		if (Status.find(eCommand) == Status.end()) {
			IPMessage(g_st[IDS_MOVE_BUTTON_STATUS_INCORRECT_INDEX_VALUE], strTitle, INFINITE);
			return TRUE;
		}

		UpdateMoveButtonStatus(eCommand, eCommand);
		if (Status[eCommand].eStatus == eSTATUS_READY) {
			return FALSE;
		}
		else {
			if (!Status[eCommand].strMsg.IsEmpty()) {
				IPMessage(Status[eCommand].strMsg, strTitle, INFINITE);	
			}
			return TRUE;
		}
		
		return TRUE;
	}
	
public:
	void Add(const int& eCommand, const T_POS* pos1);
	void SetStatusReady(const int& eCommand);
	void SetStatusRun(const int& eCommand);
	void SetStatusCompleted(const int& eCommand);
	COLORREF GetStatusColorBK(const int& eCommand);
	COLORREF GetStatusColorText(const int& eCommand);
	CString GetStatusMsg(const int& eCommand);
	void UpdateMoveButtonStatus(const int& eCommandStart, const int& eCommandEnd);
};
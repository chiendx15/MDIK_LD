#pragma once
#include "pch.h"
#include "MoveButtonStatus.h"

// Made By KES 2024.1.26

void CMoveButtonStatus::Add(const int& eCommand, const T_POS* pos) {
	Status[eCommand].eStatus = eSTATUS_READY;
	Status[eCommand].iStatusSetTime = 0;
	Status[eCommand].strMsg = _T("");
	Status[eCommand].crBK = RGB_GRAY_LIGHT;	
	Status[eCommand].crText = RGB_BLACK;	
	if (pos != nullptr) {
		Status[eCommand].vecPos.push_back(pos);
	}
}

void CMoveButtonStatus::SetStatusReady(const int& eCommand) {
	// key 가 존재하지 않는다
	if (Status.find(eCommand) == Status.end()) {
		return;
	}

	Status[eCommand].eStatus = eSTATUS_READY;
}

void CMoveButtonStatus::SetStatusRun(const int& eCommand) {
	// key 가 존재하지 않는다
	if (Status.find(eCommand) == Status.end()) {
		return;
	}

	Status[eCommand].eStatus = eSTATUS_RUN;
}

void CMoveButtonStatus::SetStatusCompleted(const int& eCommand) {
	// key 가 존재하지 않는다
	if (Status.find(eCommand) == Status.end()) {
		return;
	}

	Status[eCommand].eStatus = eSTATUS_COMPLETED;
}

COLORREF CMoveButtonStatus::GetStatusColorBK(const int& eCommand) {
	// key 가 존재하지 않는다
	if (Status.find(eCommand) == Status.end()) {
		return RGB_GRAY_LIGHT;
	}

	return Status[eCommand].crBK;
}

COLORREF CMoveButtonStatus::GetStatusColorText(const int& eCommand) {
	// key 가 존재하지 않는다
	if (Status.find(eCommand) == Status.end()) {
		return RGB_BLACK;
	}

	return Status[eCommand].crText;
}

CString CMoveButtonStatus::GetStatusMsg(const int& eCommand) {
	// key 가 존재하지 않는다
	if (Status.find(eCommand) == Status.end()) {
		return _T("");
	}

	return Status[eCommand].strMsg;
}

void CMoveButtonStatus::UpdateMoveButtonStatus(const int& eCommandStart, const int& eCommandEnd) {
	for (int iCommand = eCommandStart; iCommand <= eCommandEnd; iCommand++) {
		// key 가 존재하지 않는다
		if (Status.find(iCommand) == Status.end()) {
			continue;
		}

		// RUN 중일 경우 Yellow
		if (Status[iCommand].eStatus == eSTATUS_RUN) {
			Status[iCommand].crBK = RGB_YELLOW;
			Status[iCommand].crText = RGB_BLACK;	
			continue;
		}

		// Completed 일 경우 Green, 타이머 동작
		if (Status[iCommand].eStatus == eSTATUS_COMPLETED) {
			if (Status[iCommand].iStatusSetTime == 0) {	// 타이머 세팅
				Status[iCommand].iStatusSetTime = GetTickCount64(); 
			}
			if (Status[iCommand].iStatusSetTime - GetTickCount64() >= 5000) {	// Completed 5초 후 Ready 상태로 변경
				Status[iCommand].eStatus = eSTATUS_READY;
				Status[iCommand].iStatusSetTime = 0;
			}
			Status[iCommand].crBK = RGB_GREEN;
			Status[iCommand].crText = RGB_BLACK;	
			continue;
		}

		Status[iCommand].strMsg = _T("");
		BOOL bAlarm = FALSE;
		for (auto& pos : Status[iCommand].vecPos) {
			if (pos == nullptr) {
				continue;
			}

			if (pos->iAxis != -1) {
				// 알람 상태 여부
				if (sm.MotorStatus[pos->iAxis].tCrt.bAlarm == TRUE) {
					if (!Status[iCommand].strMsg.IsEmpty())
						Status[iCommand].strMsg += _T("\n");

					Status[iCommand].strMsg += g_st[IDS_MOVE_BUTTON_STATUS_MOTOR_ALARM_STATE] + Format(_T("(Axis: %d)"), pos->iAxis);
					bAlarm = TRUE;
				}
				if (pos->dTarget > sm.MotorParameter[pos->iAxis].dLimitHigh) {
					if (!Status[iCommand].strMsg.IsEmpty())
						Status[iCommand].strMsg += _T("\n");

					Status[iCommand].strMsg += g_st[IDS_MOVE_BUTTON_STATUS_MOTOR_PLUS_SW_LIMIT] + Format(_T("(Axis: %d Target: %.3f)"), pos->iAxis, pos->dTarget);
					bAlarm = TRUE;
				}
				if (pos->dTarget < sm.MotorParameter[pos->iAxis].dLimitLow) {
					if (!Status[iCommand].strMsg.IsEmpty())
						Status[iCommand].strMsg += _T("\n");

					Status[iCommand].strMsg += g_st[IDS_MOVE_BUTTON_STATUS_MOTOR_MINUS_SW_LIMIT] + Format(_T("(Axis: %d Target: %.3f)"), pos->iAxis, pos->dTarget);
					bAlarm = TRUE;
				}
				if (GetMotorPosition((eAxis)pos->iAxis) < pos->dTarget) {
					if (sm.MotorStatus[pos->iAxis].tCrt.bPLimit) {
						if (!Status[iCommand].strMsg.IsEmpty())
							Status[iCommand].strMsg += _T("\n");

						Status[iCommand].strMsg += g_st[IDS_MOVE_BUTTON_STATUS_CURRENT_PLUS_LIMIT] + Format(_T("(Axis: %d)"), pos->iAxis);
						bAlarm = TRUE;
					}
				}
				else {
					if (sm.MotorStatus[pos->iAxis].tCrt.bNLimit) {
						if (!Status[iCommand].strMsg.IsEmpty())
							Status[iCommand].strMsg += _T("\n");

						Status[iCommand].strMsg += g_st[IDS_MOVE_BUTTON_STATUS_CURRENT_MINUS_LIMIT] + Format(_T("(Axis: %d)"), pos->iAxis);
						bAlarm = TRUE;
					}
				}

				if (bAlarm == TRUE) {
					continue;	// 알람 상태 우선. bAlarm == TRUE 일 경우 인터락 체크 패스
				}

				// 인터락 여부 + 메세지
				auto [bInterlock, msg] = CheckMotorInterLockMsg(*pos);
				if (bInterlock) {
					if (!Status[iCommand].strMsg.IsEmpty())
						Status[iCommand].strMsg += _T("\n");

					Status[iCommand].strMsg += msg;
				}
			}
		}

		if (bAlarm == TRUE) {
			Status[iCommand].eStatus = eSTATUS_ALARM;
			Status[iCommand].crBK = RGB_RED;
			Status[iCommand].crText = RGB_WHITE;	
			continue;
		}
		else if (!Status[iCommand].strMsg.IsEmpty()) {
			Status[iCommand].eStatus = eSTATUS_INTERLOCK;
			Status[iCommand].crBK = RGB_BLACK;
			Status[iCommand].crText = RGB_WHITE;	
		}
		else {	
			Status[iCommand].eStatus = eSTATUS_READY;
			Status[iCommand].crBK = RGB_GRAY_LIGHT;	
			Status[iCommand].crText = RGB_BLACK;	
		}
	}
}

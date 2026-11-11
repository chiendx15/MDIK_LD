#pragma once
#include "pch.h"
BOOL GetJogInterLock(int iAxis, BOOL bDir, double dSpeed, double dFutureSec = 0.1); // TRUE is Fault
BOOL GetPTPInterLcok(int iAxis, double dTarget, BOOL bDir); // TRUE is Fault
BOOL GetInPosition(int iAxis, double dPos);
BOOL GetInPosition(const T_POS& data);
BOOL GetInPosition(const T_MOTOR_LINER& data);
BOOL GetInPosition(const std::vector<T_MOTOR_LINER>& data);
BOOL GetInPosition(const std::vector<T_POS>& data);
BOOL GetTimeout(int iAxis, ULONG64 ullDistanceInTime);
BOOL GetTimeout(int iAxis);
ULONG64 GetTimeDistance(const double& dDistance, const double& dSpeed) ;
double GetPosition(int iAxis);
double GetAxisC2T(T_POS Data);
double GetAxisC2T(T_MOTOR_LINER Data);
double GetAxisC2T(int iAxis, double dTarget);
void SetTargetPosSet(int iAxis, double dTarget, double dSpeed);
void SetLastPosSet(int iAxis);
void SetMoveTimeout(int iAxis, ULONG64 ullDistanceInTime);

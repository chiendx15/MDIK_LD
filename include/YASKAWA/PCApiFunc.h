/************************************************************************

		PCApiFunc.h

		COPYRIGHT (C) YASKAWA ELECTRIC CORPORATION

*************************************************************************/


#ifndef _PCAPI_FUNC_H
#define _PCAPI_FUNC_H

/************************************************************************/
/*																		*/
/*				<< Declaration of PCAPI function >>						*/
/*																		*/
/*		File name	:  PCApiFunc.h										*/
/*																		*/
/*																		*/
/************************************************************************/


EXPORT_FUNC DWORD	WINAPI ymcGetLastError( void);

EXPORT_FUNC	DWORD	WINAPI	ymcDeclareAxis(
	WORD					RackNo,
	WORD					SlotNo,
	WORD					SubslotNo,
	WORD					AxisNo,
	WORD					LogicalAxisNo,
	WORD					AxisType,
	LPBYTE					pAxisName,
	HAXIS*					phAxis
);

EXPORT_FUNC	DWORD	WINAPI	ymcClearAxis(
	HAXIS					hAxis
);

EXPORT_FUNC	DWORD	WINAPI	ymcClearAllAxes( void);

EXPORT_FUNC	DWORD	WINAPI	ymcGetAxisHandle(
	WORD					SpecifyType,
	WORD					RackNo,
	WORD					SlotNo,
	WORD					SubslotNo,
	WORD					AxisNo,
	WORD					LogicalAxisNo,
	LPBYTE					pAxisName,
	HAXIS*					phAxis
);

EXPORT_FUNC	DWORD	WINAPI	ymcDeclareDevice(
	WORD					AxisNum,
	HAXIS*					pAxis,
	HDEVICE*				phDevice
);

EXPORT_FUNC	DWORD	WINAPI	ymcClearDevice(
	HDEVICE					hDevice
);

EXPORT_FUNC	DWORD	WINAPI	ymcConvertFloat2Fix(
	HAXIS					hAxis,
	DOUBLE					FloatValue,
	long*					pFixValue
);

EXPORT_FUNC	DWORD	WINAPI	ymcConvertFix2Float(
	HAXIS					hAxis,
	long					FixValue,
	LPDOUBLE				pFloatValue
);

EXPORT_FUNC	DWORD	WINAPI	ymcSetMotionParameterValue(
	HAXIS					hAxis,
	WORD					ParameterType,
	DWORD					ParameterOffset,
	DWORD					Value
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetMotionParameterValue(
	HAXIS					hAxis,
	WORD					ParameterType,
	DWORD					ParameterOffset,
	LPDWORD					pStoredValue
);

EXPORT_FUNC	DWORD	WINAPI	ymcDefinePosition(
	HDEVICE					hDevice,
	LPPOSITION_DATA			lpPos
);

EXPORT_FUNC	DWORD	WINAPI	ymcMovePositioning(
	HDEVICE					hDevice,
	LPMOTION_DATA			lpMotionData,
	LPPOSITION_DATA			lpPos,
	DWORD					hMoveIO,
	LPBYTE					pObjectName,
	WORD					WaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcMoveExternalPositioning(
	HDEVICE					hDevice,
	LPMOTION_DATA			lpMotionData,
	LPPOSITION_DATA			lpPos,
	LPPOSITION_DATA			lpMaxLatchPos,
	LPPOSITION_DATA			lpMinLatchPos,
	LPPOSITION_DATA			lpDistance,
	DWORD					hMoveIO,
	LPBYTE					pObjectName,
	LPWORD					pWaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcMoveIntimePositioning(
	HDEVICE					hDevice,
	LPMOTION_DATA			lpMotionData,
	LPPOSITION_DATA			lpPos,
	LPDWORD					PositioningTime,
	DWORD					hMoveIO,
	LPBYTE					pObjectName,
	WORD					WaitForCompletion,
	DWORD					SystemOption
);


EXPORT_FUNC	DWORD	WINAPI	ymcMoveHomePosition(
	HDEVICE					hDevice,
	LPMOTION_DATA			lpMotionData,
	LPPOSITION_DATA			lpOffsetPosition,
	LPWORD					pHomingMethod,
	LPWORD					pDirection,
	DWORD					hMoveIO,
	LPBYTE					pObjectName,
	LPWORD					pWaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcMoveJOG(
	HDEVICE					hDevice,
	LPMOTION_DATA			lpMotionData,
	LPWORD					pDirection,
	LPWORD					pTimeout,
	DWORD					hMoveIO,
	LPBYTE					pObjectName,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcStopJOG(
	HDEVICE					hDevice,
	DWORD					hMoveIO,
	LPBYTE					pObjectName,
	LPWORD					pWaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcMoveDriverPositioning(
	HDEVICE					hDevice,
	LPMOTION_DATA			lpMotionData,
	LPPOSITION_DATA			lpPos,
	DWORD					hMoveIO,
	LPBYTE					pObjectName,
	LPWORD					pWaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcStopMotion(
	HDEVICE					hDevice,
	LPMOTION_DATA			lpMotionData,
	LPBYTE					pObjectName,
	LPWORD					pWaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcMoveLinear(
	HDEVICE					hDevice,
	LPMOTION_DATA			lpMotionData,
	LPPOSITION_DATA			lpPos,
	DWORD					hMoveIO,
	LPBYTE					pObjectName,
	WORD					WaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcMoveCircularRadius(
	HDEVICE					hDevice,
	LPMOTION_DATA			lpMotionData,
	LPPOSITION_DATA			lpEndPoint,
	LPDWORD_DATA			lpRadius,
	LPDWORD_DATA			lpTurnNumber,
	WORD					Direction,
	WORD					AngleType,
	DWORD					hMoveIO,
	LPBYTE					pObjectName,
	WORD					WaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcMoveCircularCenter(
	HDEVICE					hDevice,
	LPMOTION_DATA			lpMotionData,
	LPPOSITION_DATA			lpEndPoint,
	LPPOSITION_DATA			lpCenter,
	LPDWORD_DATA			lpTurnNumber,
	WORD					Direction,
	DWORD					hMoveIO,
	LPBYTE					pObjectName,
	WORD					WaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcMoveHelicalRadius(
	HDEVICE					hDevice,
	LPMOTION_DATA			lpMotionData,
	LPPOSITION_DATA			lpEndPoint,
	LPDWORD_DATA			lpRadius,
	LPDWORD_DATA			lpTurnNumber,
	WORD					Direction,
	WORD					AngleType,
	DWORD					hMoveIO,
	LPBYTE					pObjectName,
	WORD					WaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcMoveHelicalCenter(
	HDEVICE					hDevice,
	LPMOTION_DATA			lpMotionData,
	LPPOSITION_DATA			lpEndPoint,
	LPPOSITION_DATA			lpCenter,
	LPDWORD_DATA			lpTurnNumber,
	WORD					Direction,
	DWORD					hMoveIO,
	LPBYTE					pObjectName,
	WORD					WaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcMoveTorque(
	HDEVICE					hDevice,
	LPPOSITION_DATA			lpSpeedLimit,
	LPPOSITION_DATA			lpTorqueData,
	DWORD					hMoveIO,
	LPBYTE					pObjectName,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcEnableGear(
	HAXIS					hAxis,
	HDEVICE					hDevice,
	DWORD					MasterType,
	LPSYNC_DISTANCE			pSyncDistance,
	LPDWORD					pStatus,
	LPBYTE					pObjectName,
	LPWORD					pWaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcDisableGear(
	HAXIS					hAxis,
	HDEVICE					hDevice,
	DWORD					MasterType,
	LPSYNC_DISTANCE			pSyncDistance,
	LPBYTE					pObjectName,
	LPWORD					pWaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcSetGearRatio(
	HDEVICE					hDevice,
	LPGEAR_RATIO_DATA		lpGearRatioData,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcPositionOffset(
	HDEVICE					hDevice,
	LPPOSITION_OFFSET_DATA	lpPositionOffsetData,
	LPBYTE					pObjectName,
	LPWORD					pWaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcChangeDynamics(
	HDEVICE					hDevice,
	LPMOTION_DATA			lpMotionData,
	LPPOSITION_DATA			lpPos,
	DWORD					Subject,
	LPBYTE					pObjectName,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcServoControl(
	HDEVICE					hDevice,
	WORD					ControlType,
	WORD					TimeOut
);

EXPORT_FUNC	DWORD	WINAPI	ymcEnableLatch(
	HDEVICE					hDevice,
	LPBYTE					pObjectName,
	LPWORD					pWaitForCompletion,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcDisableLatch(
	HDEVICE					hDevice,
	LPBYTE					pObjectName,
	DWORD					SystemOption
);

EXPORT_FUNC	DWORD	WINAPI	ymcSetIoDataBit(
	LPIO_DATA				lpIoData,
	DWORD					BitValue
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetIoDataBit(
	LPIO_DATA				pIoData,
	LPDWORD					pStoredBitValue
);
EXPORT_FUNC	DWORD	WINAPI	ymcSetIoDataValue(
	LPIO_DATA				lpIoData,
	DWORD					Value
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetIoDataValue(
	LPIO_DATA				lpIoData,
	LPDWORD					pStoredValue
);

EXPORT_FUNC	DWORD	WINAPI	ymcSetRegisterData(
	HREGISTERDATA			hRegisterData,
	DWORD					RegisterDataNumber,
	LPVOID					pRegisterData
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetRegisterData(
	HREGISTERDATA			hRegisterData,
	DWORD					RegisterDataNumber,
	LPVOID					pRegisterData,
	LPDWORD					pReadDataNumber
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetRegisterDataHandle(
	LPBYTE					pRegisterName,
	HREGISTERDATA*			phRegisterData
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetAlarm(
	DWORD					Number,
	HALARM*					pAlarm,
	LPALARM_INFO			lpAlarmInfo,
	LPDWORD					pAlarmNumber
);

EXPORT_FUNC	DWORD	WINAPI	ymcClearAlarm(
	HALARM					hAlarm
);

EXPORT_FUNC	DWORD	WINAPI	ymcClearServoAlarm(
	HAXIS					hAxis
);

EXPORT_FUNC	DWORD	WINAPI	ymcOpenController(
	LPCOM_DEVICE			lpComDevice,
	HCONTROLLER*			phController
);

EXPORT_FUNC	DWORD	WINAPI	ymcCloseController(
	HCONTROLLER				hController
);

EXPORT_FUNC	DWORD	WINAPI ymcSetController(
	HCONTROLLER				hController
);

EXPORT_FUNC	DWORD	WINAPI ymcGetController(
	HCONTROLLER*			phController
);

EXPORT_FUNC	DWORD	WINAPI ymcResetController(
	HCONTROLLER				hController
);

EXPORT_FUNC	DWORD	WINAPI	ymcSetCalendar(
	LPCALENDAR_DATA			lpCalendarData
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetCalendar(
	LPCALENDAR_DATA			lpCalendarData
);

EXPORT_FUNC	DWORD	WINAPI	ymcSetAPITimeoutValue(
	LONG					TimeoutValue
);

EXPORT_FUNC	DWORD	WINAPI	ymcWaitTime(
	DWORD					WaitTime,
	LPBYTE					pObjectName
);

EXPORT_FUNC	DWORD	WINAPI	ymcSetGroupRegisterData(
	DWORD			GroupNumber,
	LPREGISTER_INFO	pRegisterInfo
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetGroupRegisterData(
	DWORD			GroupNumber,
	LPREGISTER_INFO	pRegisterInfo
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetBusIFData(
	DWORD			Offset,
	DWORD			Size,
	LPWORD			pBusIFData
);

EXPORT_FUNC	DWORD	WINAPI	ymcSetBusIFData(
	DWORD			Offset,
	DWORD			Size,
	LPWORD			pBusIFData
);


EXPORT_FUNC	DWORD	WINAPI	ymcGetBusIFInfo(
	LPBUSIF_INFO	pBusIFInfo
);

EXPORT_FUNC	DWORD	WINAPI	ymcWriteDpram(
	DWORD			Offset,				// offset of Address
	DWORD			Size,				// size of BusIFData(WORD Size)
	LPWORD			pData				// pointer of BusIFData
);
EXPORT_FUNC	DWORD	WINAPI	ymcReadDpram_DMA(
	DWORD			Offset,				// offset of Address
	DWORD			Size,				// size of BusIFData(WORD Size)
	LPWORD			pData				// pointer of BusIFData
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetLoggingFile(
	UCHAR			*MPLogPathName,		// log path neme
	UCHAR			*WriteFileName		// Status
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetLoggingPath(
	UCHAR			*LogPathName,		// log path neme
	DWORD			*MPStatus			// Status
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetRegisterDataHandleEx(
	LPBYTE			pRegisterName,		// pointer of Register Name
	LPQWORD			hRegisterData		// handle to Register Data
);


EXPORT_FUNC	DWORD	WINAPI	ymcSetRegisterDataEx(
	QWORD		hRegisterData,			// handle to RegisterData
	DWORD		RegisterDataNumber,		// Number of RegisterData
	LPVOID		pRegisterData			// pointer of Stored RegisterData
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetRegisterDataEx(
	QWORD		hRegisterData,		// handle to RegisterData
	DWORD		RegisterDataNumber,	// Number of RegisterData
	LPVOID		pRegisterData,		// pointer of Stored RegisterData
	LPDWORD		pReadDataNumber		// pointer of Stored RegisterData Number
);

EXPORT_FUNC	DWORD	WINAPI	ymcSetGroupRegisterDataEx(
	DWORD			GroupNumber,	// Number of RegisterData Group
	LPREGISTER_INFO_EX	pRegisterInfo	// pointer of REGISTER INFO
);

EXPORT_FUNC	DWORD	WINAPI	ymcGetGroupRegisterDataEx(
	DWORD			GroupNumber,	// Number of RegisterData Group
	LPREGISTER_INFO_EX	pRegisterInfo	// pointer of REGISTER INFO
);

#endif /* #ifndef _PCAPI_FUNC_H */

#pragma once

#include "MIP/IPData.h"

// String Table
//#include "StringTable_UI.h"
//#include "StringTable_PLCLinker.h"

// Default Include
#include <map>
#include <array>
#include <memory>
#include <mutex>
#include <functional>

#include "Coordinate.hpp"
#include "xMathUtil/MatView.h"
#include "xMathUtil/CoordTrans.h"
#include "xMathUtil/xMathUtil.h"
#include "xMathUtil/MatHelper.h"

#include "misc/Ado2.h" // db
#include "misc/Profile.h"
#include "misc/TMDialog.h"
#include "misc/MFCEditBrowseCtrl2.h"
#include "misc/MFCPropertyGridCtrl2.h"
#include "misc/ListCtrlEd.h"
#include "misc/ListCtrlColumnEx.h"
#include "misc/ListCtrlEx.h"
#include "misc/SplashWnd.h"
#include "misc/Socket2.h"
#include "misc/DrawSymbol.h"
#include "misc/MemDC.h"
#include "misc/EditBox.h"
#include "MIP/MIP.h"
#include "Shape/Shape.h"
#include "Shape/SCanvasMatCtrl.h"
#include "Shape/DXFLoader.h"
#include "FrameGrabber/FrameGrabber.h"
#include "misc\StringTable.h"
#include "xMathUtil\MeshTable.h"

#include "Mocha.hxx"
#include "Sequence.hxx"
#include "inline.hxx"
#include "Online.hxx"
#include "MotorInfos.hxx"
#include "ConveyorInfos.hxx"
#include "MotorPosition_1.hxx"
#include "MotorPosition_2.hxx"
#include "MotorPosition_3.hxx"
#include "MotorPosition_4.hxx"
#include "DeviceParameter.hxx"
#include "Option.hxx"
#include "FDC.hxx"
#include "IOInfo.hxx"
#include "Serial.hxx"
#include "GlassEmissions.hxx"
#include "MCC.hxx"

//CBSpeedListCtrl
#include "CBSpeedListCtrl.h"
// Shared Memory
using namespace std;
using namespace cv;

class CSharedMemory {
public:
	T_MOCHA_PROCESS_INFO*		Process				= nullptr; // Process Connect Status Check

	// Sequence
	T_SEQUENCE*					Sequence			= nullptr;
	T_LOT_INFO*					LoadPPBox			= nullptr;
	T_SEQUENCE_MODULE_INFO*		Module				= nullptr;

	// MotorInfos
	T_MOTOR_TORQUE*				MotorTorque		= nullptr;
	T_MOTOR_STATUS*				MotorStatus			= nullptr;
	T_MOTOR_PARAMETER*			MotorParameter		= nullptr;
	T_CONVEYOR_PARAMETER*		Conveyor_Parameter	= nullptr;
	T_IO_BOARD_PARAMETER*		IOBoardParameter	= nullptr;

	// DeviceParameter
	T_DEVICE_PARAMETER*			DeviceParameter		= nullptr;
	
	// MotorPosition
	T_MOTOR_POS_1*				MotorPos_1			= nullptr;
	T_MOTOR_POS_2*				MotorPos_2			= nullptr;
	T_MOTOR_POS_3*				MotorPos_3			= nullptr;
	T_MOTOR_POS_4*				MotorPos_4			= nullptr;

	// IO
	T_IO_MC_1_LD *					IO			= nullptr;
	//T_IO_MC_2_SCT*				IO			= nullptr;
	//T_IO_MC_3_CCT_L*				IO			= nullptr;
	//T_IO_MC_3_CCT_R*				IO			= nullptr;
	//T_IO_MC_4_UND_L*				IO			= nullptr;
	//T_IO_MC_4_UND_R*				IO			= nullptr;
	T_ALL_IO*						IOs			= nullptr;
	T_ALL_PC_STATUS*				PCs			= nullptr;

	// Online
	T_SYSTEM_ALARM*				Alarm				= nullptr;
	T_REGISTER*					Register			= nullptr;
	T_OPTION*					Option				= nullptr;
	//T_ALARM*					ALARMs				= nullptr;
	//T_UMAC_MOTOR_PARAMETER*		MOTOR_PARAMETERs	= nullptr;
	//T_UMAC_MOTOR_STATUS*		MOTOR_STATUSs		= nullptr;
	//T_LOT_PROCESS*				LotProcess			= nullptr;
	//T_INLINE*					Inline				= nullptr;
	//T_MODULE_INFO*				MODULEs				= nullptr;
	//T_SERIAL_DATA*				Serial				= nullptr;

	T_SERIAL_CONNECTION_INFO*	SerialConnection	= nullptr;
	T_INVERTER_DATA*			InverterData		= nullptr;

	T_GLASS_EMISSIONS*			GlassEmissions		= nullptr;
	T_MCC_EQP*					MCC_EQP				= nullptr;

	T_FDC*						FDC					= nullptr;
	T_DUST_COLLECTOR*			DustCollector		= nullptr;

private:
	CIPData						m_ipMocha			;
	CIPData						m_ipSequence		;
	CIPData						m_ipOnline			;
	CIPData						m_ipPLCStatus		;
	CIPData						m_ipSerial			;
	CIPData						m_ipMotorInfo		;
	CIPData						m_ipConveyorInfo	;
	CIPData						m_ipMotorPosition	;
	CIPData						m_ipDeviceParameter	;
	CIPData						m_ipIOInfo			;
	CIPData						m_ipOption			;
	CIPData						m_ipGlassEmissions	;
	CIPData						m_ipMCC;
	CIPData						m_ipFDC;

	BOOL						m_bInitialized;
	BOOL						m_bSimulation;
public:
	BOOL IsInitialized() const { return m_bInitialized; }
	void SetSimultation(BOOL bSet) { m_bSimulation = bSet; }
	BOOL IsSimultation() const { return m_bSimulation; }
	BOOL InitSharedMemory(void) {
		BOOL bResult = TRUE;
		if (!m_ipMocha.Open("Mocha"))
			return FALSE;
		if (!m_ipMocha.GetAddress("MochaProcessInfo", &Process))
			return FALSE;

		if (!m_ipSequence.Open("Sequence")) 
			return FALSE;
		if (	!m_ipSequence.GetAddress("Sequence"	, &Sequence)
			|| !m_ipSequence.GetAddress("Module", &Module)
			|| !m_ipSequence.GetAddress("LoadPPBox", &LoadPPBox)
			//|| !m_ipSequence.GetAddress("Option", &Option)
			)
			return FALSE;

		if (!m_ipOnline.Open("Online"))
			return FALSE;
		if (!m_ipOnline.GetAddress("Alarm", &Alarm))
			return FALSE;

		// Motor Info
		if (!m_ipMotorInfo.Open("MotorInfos"))
			return FALSE;
		if (!m_ipMotorInfo.GetAddress("MotorStatus", &MotorStatus)
			|| !m_ipMotorInfo.GetAddress("MotorParameter", &MotorParameter)
			|| !m_ipMotorInfo.GetAddress("IOBoardParameter", &IOBoardParameter)
			//|| !m_ipMotorInfo.GetAddress("IO", &IO)
			|| !m_ipMotorInfo.GetAddress("Register", &Register)
			|| !m_ipMotorInfo.GetAddress("MotorTorque", &MotorTorque)
			)
			return FALSE;

		//Conveyor Info 
		if (!m_ipConveyorInfo.Open("ConveyorInfos"))
			return FALSE;
		if ( !m_ipConveyorInfo.GetAddress("Conveyor_Parameter", &Conveyor_Parameter))
			return FALSE;

		// Motor Position
		if (!m_ipMotorPosition.Open("MotorPosition"))
			return FALSE;
		if (!m_ipMotorPosition.GetAddress("MotorPos_1", &MotorPos_1))
			return FALSE;

		// Device Parameter
		if (!m_ipDeviceParameter.Open("DeviceParameter"))
			return FALSE;
		if (!m_ipDeviceParameter.GetAddress("DeviceParameter", &DeviceParameter))
			return FALSE;

		// IO Info
		if (!m_ipIOInfo.Open("IOInfo"))
			return FALSE;
		if (
			!m_ipIOInfo.GetAddress("IO", &IO)
			|| !m_ipIOInfo.GetAddress("IOs", &IOs)
			|| !m_ipIOInfo.GetAddress("PCs", &PCs)
			)
			return FALSE;

		if (!m_ipOption.Open("Option"))
			return FALSE;
		if (!m_ipOption.GetAddress("Option", &Option))
			return FALSE;

		// Serial
		if (!m_ipSerial.Open("Serial")) {
			return FALSE;
		}
		if (!m_ipSerial.GetAddress("SerialConnection", &SerialConnection)) {
			return FALSE;
		}
		if (!m_ipSerial.GetAddress("InverterData", &InverterData)) {
			return FALSE;
		}

		if (!m_ipGlassEmissions.Open("GlassEmissions")) {
			return FALSE;
		}
		if (!m_ipGlassEmissions.GetAddress("GlassEmissions", &GlassEmissions)) {
			return FALSE;
		}

		//<< 2024.04.11 kms
		if (!m_ipMCC.Open("MCC")) {
			assert(FALSE);
			return FALSE;
		}
		if (!m_ipMCC.GetAddress("MCC_EQP", &MCC_EQP)) {
			assert(MCC_EQP == nullptr);
			return FALSE;
		}
		//>>

		//<< 2024.06.24 kms
		if (!m_ipFDC.Open("FDC")) {
			assert(FALSE);
			return FALSE;
		}
		if (!m_ipFDC.GetAddress("FDC", &FDC)) {
			assert(FDC == nullptr);
			return FALSE;
		}
		if (!m_ipFDC.GetAddress("DustCollector", &DustCollector)) {
			assert(DustCollector == nullptr);
			return FALSE;
		}
		//>>

		m_bInitialized = TRUE;
		return TRUE;
	}
};

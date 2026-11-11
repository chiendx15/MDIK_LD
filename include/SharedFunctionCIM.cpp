#include "pch.h"
#include "SharedFunctionCIM.h"
namespace CIM {
	// 3.RMS
	namespace RMS {
		// Recipe 관련.
		void S7F217_PPID_CREATE_DELETE_MODIFY_REPORT(const eEQP_TYPE _eEQP, CString strPPID, CString strPPID_NUM, ePPID_MODE _ePPID_MODE, PIPCOMMAND _pCmd) {
			CIPVar var;
			var.SetChildItem("MODE", _ePPID_MODE);
			var.SetChildItem("PPID", strPPID);
			// PPID별 고유 Key 값이 있어야 함. 1 ~ 99 (Recipe Index와 동일 하게 사용 하면 될듯)
			var.SetChildItem("PPID_NUMBER", strPPID_NUM);
			//var.SetChildItem("UNIT", strUnit);

			if (_pCmd)
				g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "SendMessage", "S7F217_PPID_CREATE_DELETE_MODIFY_REPORT", var);
			else
				g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "SendMessage", "S7F217_PPID_CREATE_DELETE_MODIFY_REPORT", var);
		}

		void S6F11_EQUIPMENT_PPID_CHANGE_CEID_107_RUNNING_PPID_CHANGE(const eEQP_TYPE _eEQP, CString strPPID, PIPCOMMAND _pCmd) {
			CIPVar var;
			var.SetChildItem("PPID", strPPID);

			if (_pCmd)
				g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_EQUIPMENT_PPID_CHANGE", var);
			else
				g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_EQUIPMENT_PPID_CHANGE", var);
		}
	}
	// 4.ECM
	namespace ECID {
		void S6F11_EQUIPMENT_CONSTANT_PARAMETER_CHANGE(const eEQP_TYPE _eEQP, PIPCOMMAND _pCmd) {
			CIPVar var;
			if (_pCmd)
				g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_EQUIPMENT_CONSTANT_PARAMETER_CHANGE", var);
			else
				g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_EQUIPMENT_CONSTANT_PARAMETER_CHANGE", var);
		}
	}
	// 6.ALARM
	namespace ALARM {
		void ALARM_REPORT(const eEQP_TYPE _eEQP, const T_SYSTEM_ALARM_ITEM& _alarm, PIPCOMMAND _pCmd) {
			CIPVar var;
			var.SetChildItemT<T_SYSTEM_ALARM_ITEM>("Alarm", _alarm);

			if (_alarm.iUnit == -1) {
				if (_pCmd)
					g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "SendMessage", "S5F1_ALARM_REPORT_SEND", var);
				else
					g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "SendMessage", "S5F1_ALARM_REPORT_SEND", var);
			}
			else {
				if (_pCmd)
					g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "SendMessage", "S5F11_ALARM_REPORT_SEND", var);
				else
					g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "SendMessage", "S5F11_ALARM_REPORT_SEND", var);
			}
		}
	}
	// 7.EQP State
	namespace Status {
		void MANUAL_STOP(const eEQP_TYPE _eEQP, const eMACHINE _eMMACHINE, PIPCOMMAND _pCmd) {
			CIPVar var;
			var.SetChildItem("eMACHINE", _eMMACHINE); // LD, SCT, CCT_L, CCT_B, ULD_L, ULD_B
			if (_pCmd)
				g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "Scenario", "MANUAL_STOP", var);
			else
				g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "Scenario", "MANUAL_STOP", var);
		}
	}
	// 8. Cell Tracking
	namespace CellTracking {
		//void S6F11_CELL_PROCESS_START_CEID_401(const eEQP_TYPE _eEQP, PIPCOMMAND _pCmd) {

		//}

		//void S6F11_PAIR_CELL_PROCESS_START_CEID_411(const eEQP_TYPE _eEQP, PIPCOMMAND _pCmd) {

		//}
	}
	// 11,12. Message
	namespace Message {
		void S6F11_INTERLOCK_CONFIRM(const eEQP_TYPE _eEQP, PIPCOMMAND _pCmd) {
			CIPVar var;
			if (_pCmd)
				g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "INTERLOCK", "CONFIRM", var);
			else
				g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "INTERLOCK", "CONFIRM", var);
		}
		void S6F11_OPCALL_CONFIRM(const eEQP_TYPE _eEQP, PIPCOMMAND _pCmd) {
			CIPVar var;
			if (_pCmd)
				g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "OPCALL", "CONFIRM", var);
			else
				g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "OPCALL", "CONFIRM", var);
		}
	}
	
	// 13. Carrier Process
	namespace Sequence {
		namespace Load {
			void S6F11_CASSETTE_STATUS_CHANGE_CEID_350_CASSETTE_LOAD_REQUEST(const eEQP_TYPE _eEQP, int _ePort, PIPCOMMAND _pCmd) {
				T_PORT_INFO PortInfo;
				memset(&PortInfo, 0x00, sizeof(PortInfo));

				PortInfo.eAvailabilityState = eAS_UP;
				PortInfo.eAccessMode = eAM_AUTO;
				PortInfo.eTransfer = eTS_READY_TO_LOAD;
				PortInfo.eProcessingState = ePS_NONE;

				strncpy_s(PortInfo.szJOBID, sizeof(PortInfo.szJOBID), "", sizeof(PortInfo.szJOBID) - 1);
				strncpy_s(PortInfo.szJOBTYPE, sizeof(PortInfo.szJOBTYPE), "EMPTYTRAY", sizeof(PortInfo.szJOBTYPE) - 1);

				//- PORTID : UI01 
				//- PORTAVAILSTATE : UP(2)
				//- PORTACCESSMODE : AUTO(1)
				//- PORTTRANSFERSTATE : READY TO LOAD(1)
				//- PORTPROCESSINGSTATE : NONE(0)
				//- JOBID :
				//- JOB TYPE : EMPTYTRAY

				CIPVar var;
				var.SetChildItem("eCEID", eCEID_CASSETTE_LOAD_REQUEST);
				//var.SetChildItem("ePORT", _ePort); // Loader PPBox도 Port 만들어줘야됨.
				//var.SetChildItem("JOBID", ""); // Load Request 일때 공백

				var.SetChildItemT<T_PORT_INFO>("T_PORT_INFO", PortInfo);

				if (_pCmd)
					g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CASSETTE_STATUS_CHANGE", var);
				else
					g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CASSETTE_STATUS_CHANGE", var);
			}

			// 수동 투입 시에도 Load Completed 보고 해야 됨.
			void S6F11_CASSETTE_STATUS_CHANGE_CEID_351_CASSETTE_LOAD_COMPLETE(const eEQP_TYPE _eEQP, int _ePort, BOOL bUseAGV, PIPCOMMAND _pCmd) {
				T_PORT_INFO PortInfo;
				memset(&PortInfo, 0x00, sizeof(PortInfo));

				PortInfo.eAvailabilityState = eAS_UP;
				PortInfo.eAccessMode = bUseAGV ? eAM_AUTO : eAM_MANUAL; // eAM_MANUAL
				PortInfo.eTransfer = eTS_LOADED;
				PortInfo.eProcessingState = ePS_READY_TO_PROCESS;

				strncpy_s(PortInfo.szJOBID, sizeof(PortInfo.szJOBID), "PRELOAD", sizeof(PortInfo.szJOBID) - 1);
				strncpy_s(PortInfo.szJOBTYPE, sizeof(PortInfo.szJOBTYPE), "EMPTYTRAY", sizeof(PortInfo.szJOBTYPE) - 1);

				//- PORTID : UI01
				//- PORTAVAILSTATE : UP(2)
				//- PORTACCESSMODE : AUTO(1) - MANUAL(0) 수동일 경우
				//- PORTTRANSFERSTATE : LOADED(2)
				//- PORTPROCESSINGSTATE : READY TO PROCESS(1)
				//- JOBID : PRELOAD
				//- JOB TYPE : EMPTYTRAY

				CIPVar var;
				var.SetChildItem("eCEID", eCEID_CASSETTE_LOAD_COMPLETE);
				var.SetChildItem("ePORT", _ePort); // Loader PPBox도 Port 만들어줘야됨.
				//var.SetChildItem("JOBID", "PRELOAD"); // Load Completed 일때 PRELOAD

				var.SetChildItemT<T_PORT_INFO>("T_PORT_INFO", PortInfo);

				if (_pCmd)
					g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CASSETTE_STATUS_CHANGE", var);
				else
					g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CASSETTE_STATUS_CHANGE", var);
			}

			void S6F11_CARRIER_PROCESS_CHANGE_CEID_257_CARRIER_RELEASE_COMPLETE(const eEQP_TYPE _eEQP, int _ePort, CStringA _szCSTID, PIPCOMMAND _pCmd) {
				T_LOT_INFO LotInfo;
				memset(&LotInfo, 0x00, sizeof(LotInfo));

				auto& CstInfo = LotInfo.CassetteInfo;

				//strncpy_s(CstInfo.szPORTNO, sizeof(CstInfo.szPORTNO), sm.HSMSSystem[_eEQP].LAYER_1[0].PORTs[_ePort].szNO, sizeof(CstInfo.szPORTNO) - 1);
				strncpy_s(CstInfo.szID, sizeof(CstInfo.szID), _szCSTID, sizeof(CstInfo.szID) - 1);
				strncpy_s(CstInfo.szID, sizeof(CstInfo.szID), _szCSTID, sizeof(CstInfo.szID) - 1);

				CStringA szCarrierType = Format("%s", _eEQP == eEQP_SCT ? "91" : "11"); // 91 : PPBOX
				strncpy_s(CstInfo.szTYPE, sizeof(CstInfo.szTYPE), szCarrierType, sizeof(CstInfo.szTYPE) - 1);

				LotInfo.nTray = 1;
				strncpy_s(LotInfo.TRAY_INFOs[0].szTRAY_ID, sizeof(LotInfo.TRAY_INFOs[0].szTRAY_ID), _szCSTID, sizeof(LotInfo.TRAY_INFOs[0].szTRAY_ID) - 1);

				CIPVar var;
				var.SetChildItem("eCEID", eCEID_CARRIER_RELEASE_COMPLETE);
				var.SetChildItem("ePORT", _ePort); // Loader PPBox도 Port 만들어줘야됨.
				var.SetChildItemT<T_LOT_INFO>("T_LOT_INFO", LotInfo);

				//- CARRIERID : 뚜껑 TRAYID
				//- SUBCARRIERID : PPBOX ID
				//- CARRIERTYPE = 11
				//- PORTNO = US01

				if (_pCmd)
					g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CARRIER_PROCESS_CHANGE", var);
				else
					g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CARRIER_PROCESS_CHANGE", var);
			}

			// PPBox Bcr Reading 이후 발생.
			void S6F11_CARRIER_PROCESS_CHANGE_CEID_262_CARRIER_INFORMATION_REQUEST(const eEQP_TYPE _eEQP, int _ePort, CStringA _szCSTID, PIPCOMMAND _pCmd) {
				T_LOT_INFO LotInfo;
				memset(&LotInfo, 0x00, sizeof(LotInfo));

				auto& CstInfo = LotInfo.CassetteInfo;

				//strncpy_s(CstInfo.szPORTNO, sizeof(CstInfo.szPORTNO), sm.HSMSSystem[_eEQP].LAYER_1[0].PORTs[_ePort].szNO, sizeof(CstInfo.szPORTNO) - 1);
				strncpy_s(CstInfo.szID, sizeof(CstInfo.szID), _szCSTID, sizeof(CstInfo.szID) - 1);
				strncpy_s(CstInfo.szID, sizeof(CstInfo.szID), _szCSTID, sizeof(CstInfo.szID) - 1);

				CStringA szCarrierType("91"); // 91 : PPBOX
				strncpy_s(CstInfo.szTYPE, sizeof(CstInfo.szTYPE), szCarrierType, sizeof(CstInfo.szTYPE) - 1);

				LotInfo.nTray = 1;
				strncpy_s(LotInfo.TRAY_INFOs[0].szTRAY_ID, sizeof(LotInfo.TRAY_INFOs[0].szTRAY_ID), _szCSTID, sizeof(LotInfo.TRAY_INFOs[0].szTRAY_ID) - 1);

				CIPVar var;
				var.SetChildItem("eCEID", eCEID_CARRIER_INFORMATION_REQUEST);
				var.SetChildItem("ePORT", _ePort); // Loader PPBox도 Port 만들어줘야됨.
				var.SetChildItemT<T_LOT_INFO>("T_LOT_INFO", LotInfo);

				//- CARRIERID : PPBOX ID
				//- SUBCARRIERID : PPBOX ID (정보를 받기 전이라 Sub Carrier 가 있을 수 없음)
				//- CARRIERTYPE = 91
				//- PORTNO = LS01

				if (_pCmd)
					g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CARRIER_PROCESS_CHANGE", var);
				else
					g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CARRIER_PROCESS_CHANGE", var);
			}


			// Reset
			void S6F11_CARRIER_PROCESS_CHANGE_CEID_262_CARRIER_INFORMATION_REQUEST_RESET(const eEQP_TYPE _eEQP, int _ePort, PIPCOMMAND _pCmd) {
				CIPVar var;
				var.SetChildItem("eCEID", eCEID_CARRIER_INFORMATION_REQUEST);
				var.SetChildItem("ePORT", _ePort); // Loader PPBox도 Port 만들어줘야됨.

				if (_pCmd)
					g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CARRIER_PROCESS_CHANGE_RESET", var);
				else
					g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CARRIER_PROCESS_CHANGE_RESET", var);
			}

			void S6F11_CARRIER_PROCESS_CHANGE_CEID_257_CARRIER_RELEASE_COMPLETE_RESET(const eEQP_TYPE _eEQP, int _ePort, PIPCOMMAND _pCmd) {
				CIPVar var;
				var.SetChildItem("eCEID", eCEID_CARRIER_RELEASE_COMPLETE);
				var.SetChildItem("ePORT", _ePort); // Loader PPBox도 Port 만들어줘야됨.

				if (_pCmd)
					g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CARRIER_PROCESS_CHANGE_RESET", var);
				else
					g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CARRIER_PROCESS_CHANGE_RESET", var);
			}
		}

		namespace Unload {
			void S6F11_CASSETTE_STATUS_CHANGE_CEID_352_CASSETTE_UNLOAD_REQUEST(const eEQP_TYPE _eEQP, int _ePort, PIPCOMMAND _pCmd) {
				T_PORT_INFO PortInfo;
				memset(&PortInfo, 0x00, sizeof(PortInfo));

				PortInfo.eAvailabilityState = eAS_UP;
				//PortInfo.eAccessMode = bUseAGV ? eAM_AUTO : eAM_MANUAL; // eAM_MANUAL
				PortInfo.eTransfer = eTS_READY_TO_UNLOAD;
				PortInfo.eProcessingState = ePS_PROCESSED;

				strncpy_s(PortInfo.szJOBID, sizeof(PortInfo.szJOBID), "", sizeof(PortInfo.szJOBID) - 1);
				strncpy_s(PortInfo.szJOBTYPE, sizeof(PortInfo.szJOBTYPE), "", sizeof(PortInfo.szJOBTYPE) - 1);

				//- PORTID : UI01
				//- PORTAVAILSTATE : UP(2)
				//- PORTACCESSMODE : AUTO(1) - MANUAL(0) 수동일 경우
				//- PORTTRANSFERSTATE : LOADED(2)
				//- PORTPROCESSINGSTATE : READY TO PROCESS(1)
				//- JOBID : PRELOAD
				//- JOB TYPE : EMPTYTRAY

				CIPVar var;
				var.SetChildItem("eCEID", eCEID_CASSETTE_UNLOAD_REQUEST);
				var.SetChildItem("ePORT", _ePort); // Loader PPBox도 Port 만들어줘야됨.
				//var.SetChildItem("JOBID", "PRELOAD"); // Load Completed 일때 PRELOAD

				var.SetChildItemT<T_PORT_INFO>("T_PORT_INFO", PortInfo);

				if (_pCmd)
					g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CASSETTE_STATUS_CHANGE", var);
				else
					g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CASSETTE_STATUS_CHANGE", var);
			}

			void S6F11_CASSETTE_STATUS_CHANGE_CEID_353_CASSETTE_UNLOAD_COMPLETE(const eEQP_TYPE _eEQP, int _ePort, PIPCOMMAND _pCmd) {
				T_PORT_INFO PortInfo;
				memset(&PortInfo, 0x00, sizeof(PortInfo));

				PortInfo.eAvailabilityState = eAS_UP;
				//PortInfo.eAccessMode = bUseAGV ? eAM_AUTO : eAM_MANUAL; // eAM_MANUAL
				PortInfo.eTransfer = eTS_READY_TO_LOAD; // 설비에서 받을꺼니까?
				PortInfo.eProcessingState = ePS_NONE;

				strncpy_s(PortInfo.szJOBID, sizeof(PortInfo.szJOBID), "", sizeof(PortInfo.szJOBID) - 1);
				strncpy_s(PortInfo.szJOBTYPE, sizeof(PortInfo.szJOBTYPE), "", sizeof(PortInfo.szJOBTYPE) - 1);

				//- PORTID : UI01
				//- PORTAVAILSTATE : UP(2)
				//- PORTACCESSMODE : AUTO(1) - MANUAL(0) 수동일 경우
				//- PORTTRANSFERSTATE : LOADED(2)
				//- PORTPROCESSINGSTATE : READY TO PROCESS(1)
				//- JOBID : PRELOAD
				//- JOB TYPE : EMPTYTRAY

				CIPVar var;
				var.SetChildItem("eCEID", eCEID_CASSETTE_UNLOAD_COMPLETE);
				var.SetChildItem("ePORT", _ePort); // Loader PPBox도 Port 만들어줘야됨.
				//var.SetChildItem("JOBID", "PRELOAD"); // Load Completed 일때 PRELOAD

				var.SetChildItemT<T_PORT_INFO>("T_PORT_INFO", PortInfo);

				if (_pCmd)
					g_ipCIMClient.SendIPStepCommand(_pCmd, "CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CASSETTE_STATUS_CHANGE", var);
				else
					g_ipCIMClient.PostIPCommand("CIMTransfer", lpEQP[_eEQP], "SendMessage", "S6F11_CASSETTE_STATUS_CHANGE", var);
			}
		}
	}
}

void _STRNCPY_S(char* pszTarget, int iSize, LPCSTR lpSrc) {
	strncpy_s(pszTarget, iSize, lpSrc, iSize - 1);
};
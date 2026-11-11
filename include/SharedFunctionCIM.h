#pragma once
#include "SharedFunction.h"

namespace CIM {
	// 3.RMS
	namespace RMS {
		void S7F217_PPID_CREATE_DELETE_MODIFY_REPORT(const eEQP_TYPE _eEQP, CString strPPID, CString strPPID_NUM, ePPID_MODE _ePPID_MODE, PIPCOMMAND _pCmd = nullptr);
		void S6F11_EQUIPMENT_PPID_CHANGE_CEID_107_RUNNING_PPID_CHANGE(const eEQP_TYPE _eEQP, CString strPPID, PIPCOMMAND _pCmd = nullptr);
	}
	// 4.ECM
	namespace ECID {
		void S6F11_EQUIPMENT_CONSTANT_PARAMETER_CHANGE(const eEQP_TYPE _eEQP, PIPCOMMAND _pCmd = nullptr);
	}
	// 6.ALARM
	namespace ALARM {
		void ALARM_REPORT(const eEQP_TYPE _eEQP, const T_SYSTEM_ALARM_ITEM& _alarm, PIPCOMMAND _pCmd = nullptr);
	}
	// 7.EQP State
	namespace Status {
		// CIMTransfer.cpp로 옮김.
		void MANUAL_STOP(const eEQP_TYPE _eEQP, const eMACHINE _eMMACHINE ,PIPCOMMAND _pCmd = nullptr);
	}
	// 8. Cell Tracking
	namespace CellTracking {
	//	void S6F11_CELL_PROCESS_START_CEID_401(const eEQP_TYPE _eEQP, PIPCOMMAND _pCmd = nullptr);
	//	void S6F11_PAIR_CELL_PROCESS_START_CEID_411(const eEQP_TYPE _eEQP, PIPCOMMAND _pCmd = nullptr);
	}

	// 11,12. Message
	namespace Message {
		void S6F11_INTERLOCK_CONFIRM(const eEQP_TYPE _eEQP, PIPCOMMAND _pCmd = nullptr);
		void S6F11_OPCALL_CONFIRM(const eEQP_TYPE _eEQP, PIPCOMMAND _pCmd = nullptr);
	}

	// 13. Carrier Process
	namespace Sequence {
		namespace Load {
			// PPBox Seq : LI01 요청 (350) -> AGV 투입 완료 -> LI01 완료(351) -> LS01이동 -> 정보 요청 (262) -> 해제 (257) -> S3F115 정보 받음.
			// Cassette Seq : UI01 요청 (350) -> AGV 투입 완료 -> UI01 완료(351) -> US01 이동 -> Move In (355) -> 정보 요청 (263) -> S3F117로 정보 받음
			void S6F11_CASSETTE_STATUS_CHANGE_CEID_350_CASSETTE_LOAD_REQUEST(const eEQP_TYPE _eEQP, int _ePort, PIPCOMMAND _pCmd = nullptr);
			void S6F11_CASSETTE_STATUS_CHANGE_CEID_351_CASSETTE_LOAD_COMPLETE(const eEQP_TYPE _eEQP, int _ePort, BOOL bUseAGV = TRUE, PIPCOMMAND _pCmd = nullptr);
			void S6F11_CARRIER_PROCESS_CHANGE_CEID_257_CARRIER_RELEASE_COMPLETE(const eEQP_TYPE _eEQP, int _ePort, CStringA _szCSTID, PIPCOMMAND _pCmd = nullptr);
			void S6F11_CARRIER_PROCESS_CHANGE_CEID_262_CARRIER_INFORMATION_REQUEST(const eEQP_TYPE _eEQP, int _ePort, CStringA _szCSTID, PIPCOMMAND _pCmd = nullptr);

			// Reset

			void S6F11_CARRIER_PROCESS_CHANGE_CEID_262_CARRIER_INFORMATION_REQUEST_RESET(const eEQP_TYPE _eEQP, int _ePort, PIPCOMMAND _pCmd = nullptr);
			void S6F11_CARRIER_PROCESS_CHANGE_CEID_257_CARRIER_RELEASE_COMPLETE_RESET(const eEQP_TYPE _eEQP, int _ePort, PIPCOMMAND _pCmd = nullptr);
		}
		namespace Unload {
			void S6F11_CASSETTE_STATUS_CHANGE_CEID_352_CASSETTE_UNLOAD_REQUEST(const eEQP_TYPE _eEQP, int _ePort, PIPCOMMAND _pCmd = nullptr);
			void S6F11_CASSETTE_STATUS_CHANGE_CEID_353_CASSETTE_UNLOAD_COMPLETE(const eEQP_TYPE _eEQP, int _ePort, PIPCOMMAND _pCmd = nullptr);
		}
	}
}

void _STRNCPY_S(char* pszTarget, int iSize, LPCSTR lpSrc);
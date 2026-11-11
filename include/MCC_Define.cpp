#include "pch.h"
#include "MCC_Define.h"

std::vector<CMCCLayer> g_mcc_layer_A[] = {
	// LD
	{
		{ eMCC_LAYER_LD_LI01, _T("LI01.csv") },
		{ eMCC_LAYER_LD_LI02, _T("LI02.csv") },
		{ eMCC_LAYER_LD_LE01, _T("LE01.csv") },
		{ eMCC_LAYER_LD_LO01, _T("LO01.csv") },
		{ eMCC_LAYER_LD_LO02, _T("LO02.csv") },
		{ eMCC_LAYER_LD_TR01, _T("TR01.csv") },
	},

	// SCT
	{
		{ eMCC_LAYER_SCT_GT01, _T("GT01.csv") },
		{ eMCC_LAYER_SCT_TR02, _T("TR02.csv") },
		{ eMCC_LAYER_SCT_FT01, _T("FT01.csv") },
		{ eMCC_LAYER_SCT_CK01, _T("CK01.csv") },
		{ eMCC_LAYER_SCT_CT01, _T("CT01.csv") },
		{ eMCC_LAYER_SCT_HB01, _T("HB01.csv") },
		{ eMCC_LAYER_SCT_PB01, _T("PB01.csv") },
		{ eMCC_LAYER_SCT_BT01, _T("BT01.csv") },
		{ eMCC_LAYER_SCT_TR04, _T("TR04.csv") },
		{ eMCC_LAYER_SCT_TR03, _T("TR03.csv") },
	},

	// CCTL
	{
		{ eMCC_LAYER_CCT_DT02_DT01, _T("DT02.csv") },
		{ eMCC_LAYER_CCT_TR06_TR05, _T("TR06.csv") },
		{ eMCC_LAYER_CCT_FT05_FT03, _T("FT05.csv") },
		{ eMCC_LAYER_CCT_FT04_FT02, _T("FT04.csv") },
		{ eMCC_LAYER_CCT_CK05_CK03, _T("CK05.csv") },
		{ eMCC_LAYER_CCT_CK04_CK02, _T("CK04.csv") },
		{ eMCC_LAYER_CCT_CT05_CT03, _T("CT05.csv") },
		{ eMCC_LAYER_CCT_CT04_CT02, _T("CT04.csv") },
		{ eMCC_LAYER_CCT_HB05_HB03, _T("HB05.csv") },
		{ eMCC_LAYER_CCT_HB04_HB02, _T("HB04.csv") },
		{ eMCC_LAYER_CCT_PB05_PB03, _T("PB05.csv") },
		{ eMCC_LAYER_CCT_PB04_PB02, _T("PB04.csv") },
		{ eMCC_LAYER_CCT_BT05_BT03, _T("BT05.csv") },
		{ eMCC_LAYER_CCT_BT04_BT02, _T("BT04.csv") },
	},

	// CCTR
	{
		{ eMCC_LAYER_CCT_DT02_DT01, _T("DT01.csv") },
		{ eMCC_LAYER_CCT_TR06_TR05, _T("TR05.csv") },
		{ eMCC_LAYER_CCT_FT05_FT03, _T("FT03.csv") },
		{ eMCC_LAYER_CCT_FT04_FT02, _T("FT02.csv") },
		{ eMCC_LAYER_CCT_CK05_CK03, _T("CK03.csv") },
		{ eMCC_LAYER_CCT_CK04_CK02, _T("CK02.csv") },
		{ eMCC_LAYER_CCT_CT05_CT03, _T("CT03.csv") },
		{ eMCC_LAYER_CCT_CT04_CT02, _T("CT02.csv") },
		{ eMCC_LAYER_CCT_HB05_HB03, _T("HB03.csv") },
		{ eMCC_LAYER_CCT_HB04_HB02, _T("HB02.csv") },
		{ eMCC_LAYER_CCT_PB05_PB03, _T("PB03.csv") },
		{ eMCC_LAYER_CCT_PB04_PB02, _T("PB02.csv") },
		{ eMCC_LAYER_CCT_BT05_BT03, _T("BT03.csv") },
		{ eMCC_LAYER_CCT_BT04_BT02, _T("BT02.csv") },
	},

	// ULDL
	{
		{ eMCC_LAYER_ULD_TR14_TR12, _T("TR14.csv") },
		{ eMCC_LAYER_ULD_TR13_TR11, _T("TR13.csv") },
		{ eMCC_LAYER_ULD_PT08_PT04, _T("PT08.csv") },
		{ eMCC_LAYER_ULD_PT07_PT03, _T("PT07.csv") },
		{ eMCC_LAYER_ULD_PT06_PT02, _T("PT06.csv") },
		{ eMCC_LAYER_ULD_PT05_PT01, _T("PT05.csv") },
		{ eMCC_LAYER_ULD_PH04_PH02, _T("PH04.csv") },
		{ eMCC_LAYER_ULD_PH03_PH01, _T("PH03.csv") },
		{ eMCC_LAYER_ULD_TR22_TR21, _T("TR22.csv") },
		{ eMCC_LAYER_ULD_ST04_ST02, _T("ST04.csv") },
		{ eMCC_LAYER_ULD_ST03_ST01, _T("ST03.csv") },
		{ eMCC_LAYER_ULD_TR34_TR32, _T("TR34.csv") },
		{ eMCC_LAYER_ULD_TR33_TR31, _T("TR33.csv") },
		{ eMCC_LAYER_ULD_ST08_ST06, _T("ST08.csv") },
		{ eMCC_LAYER_ULD_ST07_ST05, _T("ST07.csv") },
		{ eMCC_LAYER_ULD_TR44_TR42, _T("TR44.csv") },
		{ eMCC_LAYER_ULD_TR43_TR41, _T("TR43.csv") },
		{ eMCC_LAYER_ULD_BF04_BF02, _T("BF04.csv") },
		{ eMCC_LAYER_ULD_BF03_BF01, _T("BF03.csv") },
		{ eMCC_LAYER_ULD_TR56_TR53, _T("TR56.csv") },
		{ eMCC_LAYER_ULD_TR55_TR52, _T("TR55.csv") },
		{ eMCC_LAYER_ULD_TR54_TR51, _T("TR54.csv") },
		{ eMCC_LAYER_ULD_TT04_TT02, _T("TT04.csv") },
		{ eMCC_LAYER_ULD_TT03_TT01, _T("TT03.csv") },
		{ eMCC_LAYER_ULD_TL10_TL05, _T("TL10.csv") },
		{ eMCC_LAYER_ULD_TL09_TL04, _T("TL09.csv") },
		{ eMCC_LAYER_ULD_TL08_TL03, _T("TL08.csv") },
		{ eMCC_LAYER_ULD_TL07_TL02, _T("TL07.csv") },
		{ eMCC_LAYER_ULD_TL06_TL01, _T("TL06.csv") },
		{ eMCC_LAYER_ULD_TB09_TB04, _T("TB09.csv") },
		{ eMCC_LAYER_ULD_TB08_TB03, _T("TB08.csv") },
		{ eMCC_LAYER_ULD_TB07_TB02, _T("TB07.csv") },
		{ eMCC_LAYER_ULD_UI04_UI02, _T("UI04.csv") },
		{ eMCC_LAYER_ULD_UO04_UO02, _T("UO04.csv") },
		{ eMCC_LAYER_ULD_UO06_UO05, _T("UO06.csv") },
		{ eMCC_LAYER_ULD_UO03_UO01, _T("UO03.csv") },
		{ eMCC_LAYER_ULD_UI03_UI01, _T("UI03.csv") },
	},

	// ULDR
	{
		{ eMCC_LAYER_ULD_TR14_TR12, _T("TR12.csv") },
		{ eMCC_LAYER_ULD_TR13_TR11, _T("TR11.csv") },
		{ eMCC_LAYER_ULD_PT08_PT04, _T("PT04.csv") },
		{ eMCC_LAYER_ULD_PT07_PT03, _T("PT03.csv") },
		{ eMCC_LAYER_ULD_PT06_PT02, _T("PT02.csv") },
		{ eMCC_LAYER_ULD_PT05_PT01, _T("PT01.csv") },
		{ eMCC_LAYER_ULD_PH04_PH02, _T("PH02.csv") },
		{ eMCC_LAYER_ULD_PH03_PH01, _T("PH01.csv") },
		{ eMCC_LAYER_ULD_TR22_TR21, _T("TR21.csv") },
		{ eMCC_LAYER_ULD_ST04_ST02, _T("ST02.csv") },
		{ eMCC_LAYER_ULD_ST03_ST01, _T("ST01.csv") },
		{ eMCC_LAYER_ULD_TR34_TR32, _T("TR32.csv") },
		{ eMCC_LAYER_ULD_TR33_TR31, _T("TR31.csv") },
		{ eMCC_LAYER_ULD_ST08_ST06, _T("ST06.csv") },
		{ eMCC_LAYER_ULD_ST07_ST05, _T("ST05.csv") },
		{ eMCC_LAYER_ULD_TR44_TR42, _T("TR42.csv") },
		{ eMCC_LAYER_ULD_TR43_TR41, _T("TR41.csv") },
		{ eMCC_LAYER_ULD_BF04_BF02, _T("BF02.csv") },
		{ eMCC_LAYER_ULD_BF03_BF01, _T("BF01.csv") },
		{ eMCC_LAYER_ULD_TR56_TR53, _T("TR53.csv") },
		{ eMCC_LAYER_ULD_TR55_TR52, _T("TR52.csv") },
		{ eMCC_LAYER_ULD_TR54_TR51, _T("TR51.csv") },
		{ eMCC_LAYER_ULD_TT04_TT02, _T("TT02.csv") },
		{ eMCC_LAYER_ULD_TT03_TT01, _T("TT01.csv") },
		{ eMCC_LAYER_ULD_TL10_TL05, _T("TL05.csv") },
		{ eMCC_LAYER_ULD_TL09_TL04, _T("TL04.csv") },
		{ eMCC_LAYER_ULD_TL08_TL03, _T("TL03.csv") },
		{ eMCC_LAYER_ULD_TL07_TL02, _T("TL02.csv") },
		{ eMCC_LAYER_ULD_TL06_TL01, _T("TL01.csv") },
		{ eMCC_LAYER_ULD_TB09_TB04, _T("TB04.csv") },
		{ eMCC_LAYER_ULD_TB08_TB03, _T("TB03.csv") },
		{ eMCC_LAYER_ULD_TB07_TB02, _T("TB02.csv") },
		{ eMCC_LAYER_ULD_UI04_UI02, _T("UI02.csv") },
		{ eMCC_LAYER_ULD_UO04_UO02, _T("UO02.csv") },
		{ eMCC_LAYER_ULD_UO06_UO05, _T("UO05.csv") },
		{ eMCC_LAYER_ULD_UO03_UO01, _T("UO01.csv") },
		{ eMCC_LAYER_ULD_UI03_UI01, _T("UI01.csv") },
	},
};

std::vector<CString> g_mcc_layer_S[] = {
	// LD
	{
		_T("LD_S.csv")
	},

	// SCT
	{

	},

	// CCTL
	{

	},

	// CCTR
	{

	},

	// ULDL
	{
		_T("ULDL_S.csv")
	},

	// ULDR
	{
		_T("ULDR_S.csv")
	},
};
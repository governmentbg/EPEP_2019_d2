#include "cac.h"

// ----- KIND_ ---------------------------------------------------------------
#if COURT_TYPE == COURT_AREAL
constant
	KIND_EXCHANGE_LAWS	= "ZT",
	KIND_EXCHANGE_INDOCS	= "w�" "��";	// 2011:199 LRQ: +�y㔄
#endif  // AREAL

#if COURT_TYPE == COURT_MILITARY
// 2017:117 citizen only, may be incorrect
constant
	KIND_EXCHANGE_LAWS	= "Z",
	KIND_EXCHANGE_INDOCS	= "w�" "�";
#endif  // MILITARY

#if COURT_TYPE == COURT_REGIONAL
constant
	KIND_EXCHANGE_LAWS	= "cm",
	KIND_EXCHANGE_INDOCS	= "MC";
#endif  // REGIONAL

#if COURT_TYPE == COURT_APPEAL
constant
	// 2015:154 FIX: +!�; 2015:188 +[]; 2016:054 +��;
	// 2016:110 IRQ: -�� +�
	KIND_VZ_LAWSUITS	= "WHU" "ZN" "!�" "Tn" "[]" "�",
	KIND_KS_LAWSUITS	= "��" "ͽ";
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN
constant
	KIND_VZ_LAWSUITS	= "",
	KIND_KS_LAWSUITS	= "�M�E" "nz";
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL
constant
	KIND_VZ_LAWSUITS	= "WCH�" "N]" "n}",
	KIND_KS_LAWSUITS	= "U" "��";
#endif  // AREAL

#if COURT_TYPE == COURT_MILITARY
constant
	KIND_VZ_LAWSUITS	= "WCH�" "N]",
	KIND_KS_LAWSUITS	= "U";
#endif  // MILITARY

constant CONNECT_TYPE_PRELIMITARY_ACTS = "FIGKQEDTVW";

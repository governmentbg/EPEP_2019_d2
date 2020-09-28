#include "cac.h"

// ----- KIND_ ---------------------------------------------------------------
// ----- RESULT_ -------------------------------------------------------------

constant EMPTY_STRING = "" ;
constant STAR_STRING = "*" ;
constant Q3_STRING = "???" ;
constant DASH_STRING = "-" ;

constant JUDGE_STRING = "����" ;
constant SECRETAR_STRING = "��" ;
constant PROSECUTOR_STRING = "�" ;
constant JUROR_STRING = "�" ;
constant REJUROR_STRING = "�" ;
constant JUDGEX_STRING = "�" ;
constant REJUDGEX_STRING = "�" ;
constant JURORX_STRING = "�" ;

// 2015:120 +D where h

# if INSTANCE
#   if APPEAL  // 25.jul.2000
constant RESULT_UNMOVEABLES =
  "hD"
   ;  //
constant RESULT_STOPPEDS =
  "S"
   ;  // Do not forget RESULT_LEFTS_OR_STOPPEDS !!!
constant RESULT_CANCELEDS =
  "O"
  "P"
  "c"
  "�"  // 14.jun.2000  ��� ������������
   ;  // Do not forget RESULT_CANCELEDS_OR_RETURNEDS
	// and RESULT_WITH_DECS_OR_CANCELEDS !!!
constant RESULT_WITH_DECISIONS =
  "W"
  "Z"
  "d"
  "j"
  "p" // ������� �� ��������
  "y"
  "T"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "z"		// 2011:173
  //"�"	// 2011:173 duplicate, comout
  "v"		// 2011:173
  //"�"	// 2011:173 duplicate, comout
  "C"
  "E"
  "G"
  "H"
	"0"
	"�"  // � ����������� �� ��.37 ���
  "�"  // 2015:080
  "�"  // 2015:080
   ; // Do not forget RESULT_WITH_DECS_OR_CANCELEDS !!!
constant RESULT_RETURNEDS =
//  "r"  // 09.jan.2001
  "B"
   ;  // Do not forget RESULT_CANCELEDS_OR_RETURNEDS !!!
constant RESULT_CANCELEDS_OR_RETURNEDS =
  "O"
  "P"
  "c"
  "B"
  "�"  // 14.jun.2000  ��� ������������
   ;
constant RESULT_M_COL_FINISHEDS =
  "A"
  "Z"
  "d"  // � �����������, ����������� ������  // 01.sep.2000
  "P"
  "a"
  "c"
  "p"
  "y"
   ;
constant RESULT_M_COL_NOT_FINISHEDS =
  "1"
  "2"
  "3"
  "4"
  "5"
  "6"
  "I"
  "M"
  "N"
  "m"
  "l"
  "o"
  "L"
  "S"
  "hD"
  "s"
  "7"
  "8"
  "9"
  "�"
  "�"  // 09.feb.2000
  "�"  // 25.jul.2000
  "K"  // �������� ������ ��������� �����������  // 25.jul.2000
  "Q"  // �������� �� ���. ��.107, ��.2 ���  // 25.jul.2000
  "V"  // �������� � ����. ��.65 ���  // 25.jul.2000
  "F"  // �������� �� ���� �������������  // 27.jul.2000
  "�"  // �������� � �������� �� �����  // 25.may.2004
  "�"  // � ����������� �� �����
  "�"  // � ������������ �� �����
   ;   // *  RESULT_LEFTS + RESULT_STOPPEDS + "hs"
constant RESULT_LEFT_RECONCILES =
  "M"
  "N"
  "m" ;
constant RESULT_WITH_DECS_OR_CANCELEDS =
  "W"
  "Z"
  "d"
  "j"
  "p"
  "y"
  "T"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "z"		// 2011:173
  //"�"	// 2011:173 duplicate, comout
  "v"		// 2011:173
  //"�"	// 2011:173 duplicate, comout
  "C"
  "E"
  "G"
  "H"
  "O"
  "P"
  "c"
  "0"
  "�"
  "�"  // 2015:080
  "�"  // 2015:080
   ;
constant RESULT_DECISIONS_CLSESBK =
  "Z"
   ;
constant RESULT_DEFINITIONS_CLSESBK =
  "J"
  "S"
   ;
constant RESULT_DEFIN_2ND_TERMIN_CLSESBK =
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "C"
  "E"
  "G"
  "H"
  "z"		// 2011:173
  //"�"	// 2011:173 duplicate, comout
  "v"		// 2011:173
  //"�"	// 2011:173 duplicate, comout
   ;
constant RESULT_TO_UP_LOOK_INTO_RETURNEDS =  // 16.jun.2000
  "B"  // ���������� - �� �������������
   ;
constant RESULT_CONFIRMATIONS =
  //"�"  // �����������          // RESULT_CONFIRMATION; 2011:173 duplicate, comout
  "v"  // �����������          // RESULT_CONFIRMATION_1
  "�"  // �������              // RESULT_GRANT
  "q"  // �������              // RESULT_GRANT_1
   ;
constant RESULT_CHANGINGS =
  "�"  // ������               // RESULT_CHANGE
  "w"  // ������               // RESULT_CHANGE_1
   ;
constant RESULT_REJECTS =
  //"�"  // ������               // RESULT_ABOLISH; 2011:173 duplicate, comout
  "z"  // ������               // RESULT_ABOLISH_1
  "�"  // ��� ��������         // RESULT_NOT_GRANT
  "u"  // ������ ��� ��������  // RESULT_NOT_GRANT_1
   ;
constant RESULT_SLOWDOWN_CONFIRMATIONS =
  "1"  //  ������
  "2"  //  ���������
   ;
constant RESULT_SLOWDOWN_REJECTS =
  "u"  // ������ ��� ��������
  "�"  // ����������
  "�"  // ������ ��� �����������	// 2016:025
   ;
constant RESULT_SLOWDOWN_REJECT_NOT_GR =   // 20060417
  "u"  // ������ ��� ��������
  ;
constant RESULT_SLOWDOWN_REJECT_CEASED =   // 20060417
  "�"  // ����������
  "�"  // ������ ��� �����������	// 2016:025
   ;
constant RESULT_IRE_UNMOVEABLES =  // 27.apr.2001
  "�"  //  � ���� - ��� ��������
   ;
constant RESULT_IRE_SENDS =  // 03.may.2001
  "�"  // ��������
  "�"  // � ���� - ��������� �������
   ;
constant RESULT_IRE_RETURNEDS =  // 03.may.2001
  "�"  // ������
  "�"  // ������ ���� ���������
  "�"  // ������ ���� ���������
  "�"  // ������ ���� ����������
  "�"  // ������ - ��������
  "�"  // ������
   ;

#   else  // of APPEAL  // 25.jul.2000
constant RESULT_UNMOVEABLES =
  "hD"
   ;
constant RESULT_STOPPEDS =
  "S"
   ;  // *  Do not forget RESULT_LEFTS_OR_STOPPEDS !!!
constant RESULT_CANCELEDS =
  "O"
  "P"
  "c"
  "�"  // 14.jun.2000  ��� ������������
	"A"  // ���������� ������������ �� �����������  // 22.jan.2001
	"�"  // ������������ ��� ����� ���� 2015:104
	 ;   // *  Do not forget RESULT_CANCELEDS_OR_RETURNEDS and
			 // RESULT_WITH_DECS_OR_CANCELEDS !!!
constant RESULT_LEFTS_1ST_SESS =  // 24.sep.2001  // AREAL COURT
	"L"  // �������� � I-�� ���������
	 ;
constant RESULT_WITH_DECISIONS =
// =========== DO NOT INCLUDE '9' IN THIS SET !!! ====================
  "W"
  "Z"
  "d"
  "j"
  "p"
  "y"
  "T"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "z"		// 2011:173
  //"�"	// 2011:173 duplicate, comout
  "v"		// 2011:173
  //"�"	// 2011:173 duplicate, comout
  "C"
  "E"
  "F"
  "G"
  "H"
  "K"
  "0"  // � ��������
  "4"  // � ������������, ����������� ������  // 24.oct.2000
  "5"  // � ������ ������������  // 24.oct.2000
  "6"  // � ����� ��������  // 24.oct.2000
  "�"  // � ������� � ������������  // 25.jul.2001
  "�"  // � ����������� �� ���������  // 08.nov.2004
//"�"  // 2010:350 LRQ; 2013:072 TRQ/LRQ: comout
  "9"  // 2011:020 IRQ
  "Q"  // 2011:020 IRQ
  "�"  // 2013:336 IRQ
  "�"  // 2015:080
  "�"  // 2015:080
   ;
  // Do not forget RESULT_WITH_DECS_OR_CANCELEDS !!!
constant RESULT_RETURNEDS =
  "r"  // 09.jan.2001 commented, 28.feb.2001 back again
  "B"
   ;  // Do not forget RESULT_CANCELEDS_OR_RETURNEDS !!!
constant RESULT_CANCELEDS_OR_RETURNEDS =
  "O"
  "P"
  "c"
  "r"  // 09.jan.2001 commented, 28.feb.2001 back again
  "B"
  "�"  // 14.jun.2000  ��� ������������
  "A"  // ���������� ������������ �� �����������  // 22.jan.2001
  "�"  // ������������ ��� ����� ���� 2015:104
   ;
constant RESULT_M_COL_FINISHEDS =
  "A"
  "Z"
  "d"  // � �����������, ����������� ������  // 01.sep.2000
  "P"
  "a"
  "c"
  "p"
  "y"
  "�"  // ������������ ��� ����� ���� 2015:104
   ;
constant RESULT_M_COL_NOT_FINISHEDS =
  "1"
  "2"
  "I"
  "M"
	"N"
  "m"
  "l"
  "o"
  "L"
  "S"
  "hD"
  "s"
//                     "78"  // 26.jul.2001
  "�"
	"�"
  "�"  // �������� � ����?��.65 ���  // 20.apr.2001 (23.may.2001)
  "�"  // �������� � �������� �� �����  // 09.jun.2003
  "�"  // � ����������� �� �����
  "�"  // � ������������ �� �����
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
  //"J"  // 2010:089 LRQ; 2010:089 SRQ: +AREAL; 2011:332 LRQ: comout
#endif  // ADMIN
   ;  // *  RESULT_LEFTS + RESULT_STOPPEDS + "hs"

constant RESULT_LEFT_RECONCILES =
  "M"
  "N"
	"m"
	 ;  // *
constant RESULT_WITH_DECS_OR_CANCELEDS =
  "W"
  "Z"
  "d"
  "j"
  "p"
  "y"
  "T"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "z"		// 2011:173
  //"�"	// 2011:173 duplicate, comout
  "v"		// 2011:173
  //"�"	// 2011:173 duplicate, comout
  "C"
  "E"
  "F"
  "G"
  "H"
  "K"
  "O"
  "P"
  "c"
  "0"
  "�"  // 04.feb.2000
//  "3"  // ���������� �� ������������� �� ��.241 // 20.oct.2000,
		  // 28.feb.2001 commented (joined with 'r')
  "4"  // � ������������, ����������� ������  // 24.oct.2000
  "5"  // � ������ ������������  // 24.oct.2000
  "6"  // � ����� ��������  // 24.oct.2000
  "A"  // ���������� ������������ �� �����������  // 22.jan.2001
  "�"  // � ������� � ������������  // 25.jul.2001
  "�"  // 2015:080
  "�"  // 2015:080
  "�"  // ������������ ��� ����� ���� 2015:104
   ;
constant RESULT_DECISIONS_CLSESBK =
  "Z"
   ;
constant RESULT_DEFINITIONS_CLSESBK =
  "J"
  "S"
   ;
constant RESULT_DEFIN_2ND_TERMIN_CLSESBK =
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "�"
  "C"
  "E"
  "F"
  "G"
  "H"
  "K"
  "z"		// 2011:173
  //"�"	// 2011:173 duplicate, comout
  "v"		// 2011:173
  //"�"	// 2011:173 duplicate, comout
   ;
constant RESULT_FIRM_CANCELEDS =
  "�"  // ������������ �� ������� �� ����
  "�"  // ������������ ������ ����� �� ����������
  "�"  // ������������ ������ ������������� � ���� ���
  "�"  // ������������ ������ ��������� �� ���������
  "�"  // ������������ ������ ������� � ����� ���������
  "�"  // ������������ ������ ������� � ����� ���������
  "1"  // ������������ ������ ����������� �� �����������
//  "2"  // ������������ �� ������� �� ������ ��������  // 20.sep.2001
//  "�"  // ����������� �� �����������  // 03.aug.2000
  "3"  // ������� �� ����������  //20.apr.2001
   ;

constant RESULT_FIRM_DECISION_CHANGES =  // 03.aug.2000
  "�"  // ������� � ���������������
  "�"  // ����������� �� �����������
  "�"  // ����� �� ������� � ���������������
  "�"  // ����������
  "�"  // ������������ ������.�� ���������� � ��������.�� ���������
  "�"  // ������������ ������.�� ���������� ������ ����.������.������.
  "2"  // ����������� ���� �����  // 17.sep.2001
  "�"  // RESULT_FIRM_DEFINITION_FOR_TR // � �����������, ���������
						    // �� �������� � ��  21.feb.2005
  "�"  // RESULT_FIRM_DECREE_FOR_TR  // � ������������, ���������
						    // �� �������� � ��  21.feb.2005

  //it's not a result of decision "a"  // ��������� �� ���������� ���� �����������  // 20060321
  "�"  // ��������� �� ���������� ���� �����������  // 20060414  - �������� �� ����� ��������
   ;
constant RESULT_FIRM_REFUSED_CHANGES = // 03.apr.2001
  "�"  // ����� �� ������� � ���������������
   ;
constant RESULT_FIRM_REGISTRATIONS =  // 12.oct.2000
  "�"  // RESULT_FIRM_REGISTRATION
  "�"  // RESULT_BRANCH_REGISTRATION
  "�"  // RESULT_REGISTRATION_DENIAL
   ;
constant RESULT_FIRM_LEGAL_FINALIZEDS =  // 04.jan.2001
//  "c"  // RESULT_CANCELED  // 26.jan.2001 removed
  "s"  // RESULT_SEND
  "A"  // ���������� ������������ �� �����������  // 22.jan.2001
   ;
constant RESULT_FIRM_LEGAL_CANCELEDS =  // 24.jan.2001
//  "c"  // RESULT_CANCELED  // 26.jan.2001 removed
  "s"  // RESULT_SEND
  "A"  // ���������� ������������ �� �����������
   ;
constant RESULT_FIRM_CL_S_BK_EXCLUDEDS =  // 22.jan.2001
  "V"  // ���������� ������������ �� �������
  "a"  // ��������� �� ���������� ���� �����������  // 2006XXXX
   ;
constant RESULT_FIRM_CLSBK_MUST_INCLUDE =  // 22.jan.2001
  "c"  // RESULT_CANCELED                            // don't change
  "s"  // RESULT_SEND
  "A"  // ���������� ������������ �� �����������
  "�"  // ������������ ��� ����� ���� 2015:104
//20050322  - in comment ->
//  "�"  // RESULT_FIRM_DEFINITION_FOR_TR // � �����������, ���������
//                                        // �� �������� � ��  21.feb.2005
//  "�"  // RESULT_FIRM_DECREE_FOR_TR  // � ������������, ���������
//                                        // �� �������� � ��  21.feb.2005
   ;
constant RESULT_FIRM_FINISHEDS = "��2" "������1��" "�" "�"
   ;
constant RESULT_TO_UP_LOOK_INTO_RETURNEDS =  // 16.jun.2000
  "r"  // ������� - �� �������������  // 09.jan.2001 commented,
						  // 28.feb.2001 back again
  "B"  // ���������� - �� �������������
//  "3"  // ���������� �� ������������� �� ��.241 // 20.oct.2000,
		  // 28.feb.2001 commented (joined with 'r')
   ;
constant RESULT_CONFIRMATIONS =
  //"�"  // �����������          // RESULT_CONFIRMATION; 2011:173 duplicate, comout
  "v"  // �����������          // RESULT_CONFIRMATION_1
  "�"  // �������              // RESULT_GRANT
  "q"  // �������              // RESULT_GRANT_1
   ;
constant RESULT_CHANGINGS =
#   if PUNISHM_2ND_REPORT_12_SEP_2001
#   else  // of PUNISHM_2ND_REPORT_12_SEP_2001
  "�"  // ������               // RESULT_CHANGE
#   endif  // of PUNISHM_2ND_REPORT_12_SEP_2001
  "w"  // ������               // RESULT_CHANGE_1
   ;
constant RESULT_REJECTS =
  //"�"  // ������               // RESULT_ABOLISH; 2011:173 duplicate, comout
  "z"  // ������               // RESULT_ABOLISH_1
  "�"  // ��� ��������         // RESULT_NOT_GRANT
  "u"  // ������ ��� ��������  // RESULT_NOT_GRANT_1
   ;
constant RESULT_SLOWDOWN_CONFIRMATIONS =
  "7"  // ������
  "8"  // ���������
   ;
constant RESULT_SLOWDOWN_REJECTS =
  "u"  // ������ ��� ��������
   ;
constant RESULT_IRE_UNMOVEABLES =  // 27.apr.2001
  "�"  //  � ���� - ��� ��������
   ;
constant RESULT_IRE_SENDS =  // 03.may.2001
  "�"  // ��������
  "�"  // � ���� - ��������� �������
   ;
constant RESULT_IRE_RETURNEDS =  // 03.may.2001
  "�"  // ������
  "�"  // ������ ���� ���������
  "�"  // ������ ���� ���������
  "�"  // ������ ���� ����������
  "�"  // ������ - ��������
  "�"  // ������
   ;
#   endif  // of APPEAL  // 25.jul.2000
# else  // of INSTANCE

					 // REGIONAL court only ===================
constant RESULT_UNMOVEABLES =
  "hD"
   ;  // Do not forget RESULT_LEFT_REPORT_DESIGNATION !!!
constant RESULT_STOPPEDS =
  "S"
   ;  //  Do not forget RESULT_LEFTS_OR_STOPPEDS !!!
constant RESULT_CANCELEDS =
  "O"
  "P"
  "c"
  "�"  // 14.jun.2000  ��� ������������
   ;  // Do not forget RESULT_CANCELEDS_OR_RETURNEDS
	// and RESULT_WITH_DECS_OR_CANCELEDS !!!
constant RESULT_WITH_DECISIONS =
  "W"
  "Z"
  "d"
  "j"
  "p"
  "y"
  "T"
  "A"
  "0"
  "�"  // � ������� � ������������  // 14.sep.2001
  "�"  // � ������������, ����������� ������    //20080314
  "F"  // 2010:064 LRQ: FIX: was missing
  "7"  // 2015:048 TRQ: FIX: was missing
  "�"  // 2015:080
  "�"  // 2015:080
   ;   // Do not forget RESULT_WITH_DECS_OR_CANCELEDS !!!
constant RESULT_RETURNEDS =
//  "r"  // 09.jan.2001
  "B"
   ;  // Do not forget RESULT_CANCELEDS_OR_RETURNEDS !!!
constant RESULT_CANCELEDS_OR_RETURNEDS =
  "O"
  "P"
  "c"
  "B"
  "�"  // 14.jun.2000  ��� ������������
   ;
constant RESULT_M_COL_FINISHEDS =
  "A"
  "Z"
  "d"  // � �����������, ����������� ������  // 01.sep.2000
  "P"
  "a"
  "c"
  "p"
  "y"
   ;
constant RESULT_M_COL_NOT_FINISHEDS =
  "1"
  "2"
  "3"
  "4"
  "5"
  "6"
  "I"
  "M"
  "N"
  "m"
  "l"
  "o"
  "L"
  "S"
  "hD"
  "s"
  "7"
  "8"
  "9"
  "e"
  "k"
  "�"
  "�"  // �������� � �������� �� �����  // 25.may.2004
  "�"  // � ����������� �� �����
  "�"  // � ������������ �� �����

  "E"  // �������� � ����������� �����  // 20060310

   ;  // RESULT_LEFTS + RESULT_STOPPEDS + "hs"
constant RESULT_LEFT_RECONCILES =
  "M"
  "N"
  "m"
   ;
constant RESULT_WITH_DECS_OR_CANCELEDS =
  "W"
  "Z"
  "d"
  "j"
  "p"
  "y"
  "T"
  "A"
  "O"
  "P"
  "c"
  "0"
  "�"
  "�"  // � ������� � ������������  // 14.sep.2001
  "F"		// 2010:064 LRQ: FIX: was missing
  "�"  // 2015:080
  "�"  // 2015:080
   ;
constant RESULT_DECISIONS_CLSESBK =
  "Z"
   ;
constant RESULT_DEFINITIONS_CLSESBK =
  "hD" // ��� ��������
  "S"  // ������
  "g"  // � ������� ��� �� ��������
  "J"  // � �����������
   ;
constant RESULT_DEFIN_TERMIN_CLSESBK =
  "d"
   ;
constant RESULT_KEEPING_METHOD_CONFIRMEDS =
  "v"  // �����������
  "u"  // ������ ��� ��������
   ;
constant RESULT_KEEPING_METHOD_CHANGEDS =
  "w"  // ������
  "q"  // �������
   ;
constant RESULT_TO_UP_LOOK_INTO_RETURNEDS =  // 16.jun.2000
  "B"  // ���������� - �� �������������
   ;
constant RESULT_IRE_UNMOVEABLES =  // 27.apr.2001
  "�"  //  � ���� - ��� ��������
   ;
constant RESULT_IRE_SENDS =  // 03.may.2001
  "�"  // ��������
  "�"  // � ���� - ��������� �������
   ;
constant RESULT_IRE_RETURNEDS =  // 03.may.2001
  "�"  // ������
  "�"  // ������ ���� ���������
  "�"  // ������ ���� ���������
  "�"  // ������ ���� ����������
  "�"  // ������ - ��������
  "�"  // ������
   ;
constant RESULT_PENALTIES =  // 25.may.2001
  "e"  // �������� - ��������. ���������� �� �.�. - ��.71
  "k"  // �������� - ��������. �������� ���. ���-�� - ��.65
   ;
constant RESULT_SPLITS =  // 25.may.2001
  "a"  // � ������� �� ��������� �� �����
  "A"  // � ������� �� ���������� �� ���� �� �����
   ;
                               // REGIONAL court only

constant RESULT_LEFT_REPORT_DESIGNATION =  // 20060627  // ����� ��, 16087, �������� ����, ����� ����� �����.
  "1"    		         // �������� I-�� ����
  "2"          		   // �������� II-�� ����
  "3"       		     // �������� I-�� ��������� I-�� ����
  "4"                // �������� I-�� ��������� II-�� ����
  "5"		             // �������� II-�� ��������� I-�� ����
  "6"          		   // ��������  II-�� ��������� II-�� ����
  "8"          		   // �������� III-�� ����
  "9"          		   // �������� II-�� ��������� III-�� ����
  "I"    		         // �������� ��� II-�� ���������
  "M"         	     //	�������� � �����. ���. ��� ������� ����
  "N"          	     //	�������� �� ���� ����������� ���������
//   L                 	�������� � I-�� ��������� -
  "m"          	     //	�������� � �����. ���. ������� ����
  "l"         	     //	��������
  "e"          	     //	�������� - ��������. ���������� �� �.�. - ��.71
  "k"           		 // �������� - ��������. �������� ���. ���-�� - ��.65
  "�"    		         // �������� � �������� �� ������������
  "�"          	     //	�������� � �������� �� �����
  "E"          		   // �������� � ����������� �����

  "�"         	     //	�������� ��� ���� ����������
  "hD"                // RESULT_UNMOVEABLES
  "�"  // � ����������� �� �����
  "�"  // � ������������ �� �����
   ;
constant RESULT_LEFT_1_REPORT_DESIGNATION =  // 20060627  // ����� ��, 16094, �������� ����, � ����� �����.
  "L"                //	�������� � I-�� ���������
   ;

constant RESULT_DECIDED_REPORT_DESIGNATION =  // 20060627 // ����� ��, ��� 15, �������� ����/� �������
  "p"          		   // ������� �� ��������
  "y"          		   // � ������ �������
  //"D"          		   // � ������ �������; 2011:173 duplicate, comout
  "A"          		   // � ������� �� ���������� �� ���� �� �����
   ;

constant RESULT_ESS_CS_REPORT_DESIGNATION =    // 20060627 // ����� ��, ��� 19, 16090, �������� ����/ � ��� �� �������� � �.�.
  "y"          		   // � ������ �������
  //"D"          		   // � ������ �������; 2011:173 duplicate, comout
  "A"          		   // � ������� �� ���������� �� ���� �� �����
  "d"                // � �����������, ����������� ������
   ;

constant KIND_PRIVATE_COMPL_REPORT_DESIGNATION =    // 20060627 // ����� ��, ��� 32, 16091,  ��������� ����
  "�"                // �������� �����		      i
  "�"                // �������� ������ �����	  i
  "�"                // ���������� �����		    i
  "�"                // ���������� ������ �����	i
  "�"                // ����� �� ��.126� ���		i
  "�"			// 2011:074 LRQ: �������
  "�"			// 2011:074 LRQ: ������ �������
  ;

# endif  // of INSTANCE                 // REGIONAL COURT only <--

const char NON_SESSION_DATED_CONT_RESULT = RESULT_FAKE ;  // 25.mar.2002

constant SUMMONS_STANDARD_RESULTS =  // 18.may.2001
  "f"  // SUMMONS_FIXED
  "u"  // SUMMONS_UNMOVEABLE
  "s"  // SUMMONS_STOPPED
  "c"  // SUMMONS_CANCELED
  "o"  // SUMMONS_ANY_OTHER_RESULT  // Why is it here (08.jun.2004)?
   ;

		    // �� ���� ������� �� ��������� �� �� �������������� - T_LINK_WIND, F_PROXY_*
constant SUMMONS_FOR_SIDE_INDIRECT_INVS =
	"B"  // ������� ������������
	"D"  // ������ ������������
	"A"  // ����.��������
	"L"  // �������
	"H"  // �����������
	"J"  // �������������
	"K"  // ���� �� ������� �����
	"P"  // ����������� ������������
	"U"  // ���������  // 21.jul.2000
	"Y"  // ���������  // 21.jul.2000
	"S"  // ������  // 21.jul.2000
	"�"  // ����������  // 31.jan.2001
	"�"  // ������������ // 20050602
	"R"  // �������   // 20051103
	"�"  // ��������  // 20070130
	"�"  // �������� ��������  // 20110018
	"�"  // ��������� // 20070130
	"�"  // �������� ������ // 20120405
	"�"  // ����������� �������� // 20130259
	"�"  // ������� ������� // 20140143
	"�"  // ����������� // 20160927	
;

constant SUMMONS_FOR_SIDE_LEGAL_SPOK_INVS =
	"B"  // ������� ������������
	"U"  // ���������  // 21.jul.2000
	"Y"  // ���������  // 21.jul.2000
	"R"  // �������
	"�"  // ������� ������� // 20140143
;

constant SUMMONS_FOR_SIDE_WITH_INVS =
	"L"  // �������
	"A"  // �������� ��������
	"�"  // �������� ��������
	"�"  // ��������
	"H"  // �����������	// 2014:302
	"�"  // ���������		// 2015:075
	"�"  // �����������	// 2016:271
;

constant PERSLST_BONEVA_0_INVS =  // 22.jan.2001
	"L"  // �������
	"H"  // �����������
	"K"  // ���� �� ������� �����
	"P"  // ����������� ������������
	"A"  // �������� ��������
	"�"  // ����������� ��������
	"�"  // �����������
;

constant INVOLV_TIME_BK_SUIT_TO_DEFENDS = "ra" ;

# if INSTANCE
#   if APPEAL
constant INVOLV_2ND_T_BK_SUIT_TO_DEFENDS = "ra""m""u"
  "i"  // �������
  "�"  // ������������-��������  // 22.nov.2000
  "�"  // ������������-��.����  // 22.nov.2000
  "�"  // ������������-�.���������  // 22.nov.2000
  "�"  // ������������-��������  // 22.nov.2000
   ;
#   else  // APPEAL
constant INVOLV_2ND_T_BK_SUIT_TO_DEFENDS = "ra""m""u"
  "i"  // �������
   ;
#   endif  // APPEAL
# else
constant INVOLV_DEFENDS_TO_EXCLUDE_P_ADM = "p" ;
# endif

//# if INSTANCE
//#   if APPEAL  // 04.may.2000                  // COURT_APPEAL  -->

#if COURT_TYPE == COURT_APPEAL
// constant KIND_COMPLAINTS  // 24.oct.2000 removed

constant KIND_EXACT_COMPLAINTS =       // ������ ����� - ��;
								 // ����� ��, ������������� -��� 10452 - ��
								 // ... ��
    "1"  // ����� (����)
    "3"  // ����� (���)
    "2"  // ����� (����/���)  // 1.3240  20070416
//  "Q"  // ����� � �������  // "� �������" - let it be  // 10.oct.2000
    "7"  // �����-�����. ����
//  "�"  // �����-�����. ����-��  // 18.dec.2003 died out
//  "j"  // ����� �� �������-��.217  // 24.oct.2000
    "l"  // �����
    "�"  // �����-����. ����  // 28.jun.2004
    "�"  // �����-����. ���� (���������������)  // 20110902
    "P"  // ���������� ����� (���)  // 20080315
    "�"  // ���������� ����� (��)   // 20080315
    "g"  // �����-�����. ���� (���������������)  // 20120120
  ;
constant KIND_PRIVATE_COMPLAINTS =
  "4"  // ������ ����� (���)
  "9"  // ������ ����� (����)  // 1.3240  20070416
  "�"  // ������ ����� (����)
  "v"  // ������ �����

  "0"  // �. �����-�����.����
  "h"  //�. �����-�����.���� (���������������)  // 20120120
  "M"  // ����� �� �����������  // KIND_ENSURE_REQUEST_R  // 21.nov.2000
  "m"  // ����� �� �����������  // KIND_ENSURE_REQUEST
  "�"  // ���� �� ������������ �� �����-��.14 ��.2  // KIND_JUDGE_ELIMINATION
  "f"  // �������� �� ����������  // KIND_JURISD_QUARREL  // 21.nov.2000
	"8"  // ����� �� �����.�� ���� �� ���.-��.37  // 19.jun.2002
  "�"  // 2011:231 TRQ/LRP: ���������� ������ ����� (���)
  "o"  // �. �����-�� - ��.274, ��.2 ���

  // Trade - begin  // 28.jun.2004
  "�"  // �. �����-����.����      // KIND_PRV_COMPL_TRA
  "�"  // �. �����-����.���� (���������������)  // 20110902
  "�"  // ����� �� �����������    // KIND_TRADE_ENSURE_REQUEST_R
  "�"  // ����� �� �����������    // KIND_TRADE_ENSURE_REQUEST
  "y"  // ���� �� ������������ �� �����-��.14 ��.2//KIND_TRADE_JUDGE_ELIMINATION
  "�"  // �������� �� ����������  // KIND_TRADE_JURISD_QUARREL
  "w"  // ����� �� �����.�� ���� �� ���.-��.37 ���
  "�"  // 2011:231 TRQ/LRP: ���������� ������ ����� (��)
  "p"  // �. �����-�� - ��.274, ��.2 ���
  // Trade - end
	;
constant KIND_SLOWDOWN_COMPLAINTS =  // 24.oct.2000
  "j"  // ����� �� �������-��.217
  "�"  // ����� �� �������-��.217  // Trade  // 28.jun.2004
	 ;
constant KIND_PETITIONS = // 11.jun.2002
	"8"  // ����� �� �����.�� ���� �� ���.-��.37
	"m"  // ����� �� �����������
	"M"  // ����� �� �����������

  // Trade - begin  // 28.jun.2004
  "w"  // ����� �� �����.�� ���� �� ���.-��.37 ���
  "�"  // ����� �� �����������    // KIND_TRADE_ENSURE_REQUEST_R
  "�"  // ����� �� �����������    // KIND_TRADE_ENSURE_REQUEST
  // Trade - end
	 ;

constant KIND_EXACT_OBJECTIONS =
	"5"  // ������� (����)
  "�"  // ������� (���)
  "L"  // �������
  "Q"  // ������� � �����  // 10.oct.2000
  "q"  // ������� � �����  // 12.sep.2002
  "X"  // ������� (����)   // 20070926
  ;
constant KIND_PRIVATE_OBJECTIONS =
  "6"  // ������ ������� (���)
  "V"  // ������ �������
  "F"  // ������ ������� � ������ �����
  "Y"  // ������ ������� (����)  // 20070926
  ;
constant KIND_PRIVATE_PUN_C_O_INDOCS =
  "3"  // ����� (���)
  "�"  // ������� (���)
  ;
constant KIND_INREG_EXACT_COMPLAINTS =
  "�"  // ���������� �����
   ;
constant KIND_INREG_PRIVATE_COMPLAINTS =
  "�"  // ���������� ������ �����
   ;
constant KIND_INREG_EXACT_OBJECTIONS =
  "�"  // ���������� �������
   ;
constant KIND_INREG_PRIVATE_OBJECTIONS =
  "�"  // ���������� ������ �������
   ;
constant KIND_INREG_REJ_RETR_COMPLAINTS =
  "�"  // ����� �� ������
   ;

constant KIND_DEMAND_INDOCS = "" ;

#endif  // APPEAL <--

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY

#if COURT_TYPE == COURT_AREAL
	constant KIND_EXACT_COMPLAINTS =
	"l"  // �����
	"1"  // ����� (����)
	"�"  // ���������� ����� (����)
	"�"  // �����-����. ����
//      "9"  // �����-�����. ����
//      "P"  // �����-�����. ����  // 20.sep.1999 - begin
//  "�"  // ������ �����-�����. ����-��  // 07.jun.2001  Why here?
//  "�"  // ������ �����-�����. ����-��  // 07.jun.2001  Why here?
	"�"  // ���������� �����
	"�"  // ���������� �����  // 20.sep.1999 - end
//  "�"  // �����-�����. ����-��  // 18.jul.2000  // 18.dec.2003 died out
//  "�"  // �����-����. ����-��  // 18.jul.2000  // 18.dec.2003 died out
	"�"  // ����� �-� �������� �� �����-����������  // 08.jan.2004
	"�"  // ������� � �����  // 2010:025
	"&"  // ���������� ������� (����)  // 2011:315
	 ;

constant KIND_EXACT_OBJECTIONS =
  "5"  // ������� (����)
  "L"  // �������
  ;
#endif  // AREAL

#if COURT_TYPE == COURT_MILITARY
	constant KIND_EXACT_COMPLAINTS =
	"l"  // �����
	"1"  // ����� (����)
	"2"  // ����� (����)
	"3"  // ����� (���)
	"�"  // ���������� ����� (���)
	"7"  // �����-�����. ����
	"�"  // ����� (���)  // 20060718
	 ;

constant KIND_EXACT_OBJECTIONS =
  "5L""�"
  "�"  // ������� (���)              // 20060718
  ;
#endif  // MILITARY

#if COURT_TYPE == COURT_ADMIN
	constant KIND_EXACT_COMPLAINTS =
	// 2008:308 LRQ: -9P�e
	//"9"  //              200    ����� - ��������� �� �����.���
	//"P"  //              200    ����� - ��������� �� �����.���
	//"�"  //              202    ����� �-� �������� �� ������ �� ������.
	//"e"  //              202    ����� �-� �������� �� ������ �� ������.
	"�"  //              222    ���������� �����
      "�"  //              224    ���������� ����� (���)-�����.����
      "�"  //              222    ���������� �����
	"�"  //              224    ���������� ����� (���)-���.����
      "g"  //              300    ���������� ����� (���)-�����.����
      "�"  //              300    ���������� ����� (���)-�����.����
      "�"  //                5    ���������� �����
       ;

constant KIND_EXACT_OBJECTIONS =
      "�"  //                7    ���������� �������
  ;

#endif  // ADMIN


constant KIND_EXACT_COMPLAINTS_CONCERN =  // 16.jan.2001
  "�"  // ���������� ����� (����)
  "�"  // ���������� �����
  "�"  // ���������� �����
  "&"  // ���������� ������� (����)  // 2011:315
   ;

#if COURT_TYPE == COURT_ADMIN
	constant KIND_PRIVATE_COMPLAINTS =
	"b"  //              214    ������ �����
	"�"  //              214    ������ �����
	"�"  //              226    ������ ���������� �����
	"�"  //              226    ������ ���������� �����
	"�"  //                6    ���������� ������ �����
	// 2008:308 LRQ: +��
	"�"  // ������ ���������� ����� (���)-�����.����
	"�"  // ������ ���������� ����� (���)-�����.����
	// 2012:079 LRQ: +��X�
	"X"  // ������ ���������� ����� (���)-�����.����
	"�"  // ������ ���������� ����� (���)-�����.����
	"�"  // �������� �� ����������
	"�"  // �������� �� ����������
	 ;

	constant KIND_PRIVATE_OBJECTIONS =
	"�"  //                8    ���������� ������ �������
	 ;
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL
	constant KIND_PRIVATE_COMPLAINTS =
	  "4"  // ������ ����� (���)
	  "v"  // ������ �����
	  "�"  // �. �����-����. ����
	  "�"  // ������ ����� - �����. ���� // 10.oct.2000
	  "�"  // ������ ����� - �����. ���� // 10.oct.2000
	  "�"  // ������ ���������� �����    // 10.oct.2000
	  "�"  // ������ ���������� �����        // 10.oct.2000
	  "g"  // ������ ���������� ����� (����) // 25.jan.2001
	  "�"  // ������ ����� (����)            // 04.may.2001
	  "�"  // �������� �� ����������     // 17.sep.2001
	  "�"  // ������ ����� (����)        // 20060718 added
	  // 2009:182 LRQ: moved here from EXACT
	  "�"  // ����� �-� �������� �� ����� �� �������.  // 2008:255
	  "�"  // ������ ����� (����)	// 2009:182
	  "�"  // ������ ������� � ������ �����  // 2010:025
	  "�"  // ���������� �� ��.423 ��� // 2010:025
	  "�"  // ���� �� �������.�� ����� ��.23, ��.3 ��� // 2015:091
	 ;

	constant KIND_PRIVATE_OBJECTIONS =
	  "6"  // ������ ������� (���)
	  "V"  // ������ �������
	  "�"  // ������ ������� (����)       // 04.may.2001
	  "�"  // ������ ������� (����)  // 2011:320 FIX
	 ;
#endif  // AREAL

#if COURT_TYPE == COURT_MILITARY
	constant KIND_PRIVATE_COMPLAINTS =
	  "4"  // ������ ����� (���)
	  "v"  // ������ �����
	  "0"  // �. �����-�����.����
	  "g"  // ������ ���������� ����� (���) // 25.jan.2001
	  "�"  // ������ ����� (����)            // 04.may.2001
	  "�"  // �������� �� ����������     // 17.sep.2001
	  "�"  // ������ ����� (���)        // 20060718 added
	 ;

	constant KIND_PRIVATE_OBJECTIONS =
	  "6"  // ������ ������� (���)
	  "V"  // ������ �������
	  "�"  // ������ ������� (����)       // 04.may.2001
	 ;
#endif  // MILITARY

constant KIND_PRIVATE_COMPLAINTS_CONCERN =  // 16.jan.2001
  "�"  // ������ ���������� �����
  "�"  // ������ ���������� �����
  "g"  // ������ ���������� ����� (����)  // 25.jan.2001
//  "�"  // ������ ���������� �����-��  // 07.jun.2001  // 18.dec.2003 died out
//  "�"  // ������ ���������� �����-��  // 07.jun.2001  // 18.dec.2003 died out
   ;
constant KIND_SLOWDOWN_COMPLAINTS =  // 24.oct.2000
  "�"  // ����� �� ������� // 20060208    '�', 0407, '����� �� �������'
   ;
//constant KIND_OBJECTIONS  // 24.oct.2000 removed

constant KIND_INREG_EXACT_COMPLAINTS =
  "�"  // �������� �����
  "�"  // ���������� �����
   ;
constant KIND_INREG_PRIVATE_COMPLAINTS =
  "�"  // �������� ������ �����
  "�"  // ���������� ������ �����
   ;
constant KIND_INREG_EXACT_OBJECTIONS =
  "�"  // �������
  "�"  // ���������� �������
   ;
constant KIND_INREG_PRIVATE_OBJECTIONS =
  "�"  // ������ �������
  "�"  // ���������� ������ �������
   ;
constant KIND_INREG_REJ_RETR_COMPLAINTS =
  "�"  // ����� �� ������
  "�"  // ����� �� �������
   ;

constant KIND_TRADE_BOZA_INDOCS =  // 08.may.2002
  "�"  // ����������
  "�"  // ������ ����� ����� �������� �� ������
   ;

constant KIND_DEMAND_INDOCS = "" ;

#endif  // AREAL || ADMIN || MILITARY

#if COURT_TYPE == COURT_APPEAL
constant KIND_PSEUDO_SESSIONS =
  "�"  // ����� ���������
   ;
constant KIND_PSEUDO_OR_CLOSED_SESSIONS =
  "�"  // ����� ���������
  "�"
  "I"  // 12.jan.2001
  "J"  // 12.jan.2001
   ;
constant KIND_JUDGEMENTS_BY_NAME =
  "�"  // �������
   ;
constant KIND_DECISIONS_BY_NAME =
  "�"  // �������
   ;
constant KIND_DEFINITIONS_BY_NAME =
  "�"  // �����������
   ;
constant KIND_DECREES_BY_NAME =
  "�"  // ������������
   ;
constant KIND_PUN_GENERAL_LAWS = // 08.apr.2002
  "W"  // �����
	 ;
constant KIND_PUN_PRIVATE_LAWS = // 08.apr.2002
	"H"  // ����
	 ;
constant KIND_KEEP_METHOD_CHG_COMPLAINTS = "x" ;
constant KIND_ACCUSE_ACTS = "Au" ;

#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
constant KIND_PSEUDO_SESSIONS =
  "�"  // ����� ���������
#if COURT_TYPE != COURT_MILITARY
  "�"  // �������� �� �����������
  "�"  // ������������ ����� ���������
#endif  // !MILITARY
   ;
constant KIND_PSEUDO_OR_CLOSED_SESSIONS =
  "�"  // ����� ���������
#if COURT_TYPE != COURT_MILITARY
  "�"  // �������� �� �����������
  "�"  // ������������ ����� ���������
#endif  // !MILITARY
  "�"  // �������/�������������� ���������
  "F"  // �������/�������������� ��������� 2  // 12.jan.2001
  "G"  // �������/�������������� ��������� 3  // 12.jan.2001
  "�"  // �������/�������������� ��������� 4  // 21.sep.2001
  "�"  // �������/�������������� ��������� 5
  "�"  // �������/�������������� ��������� 6
  "�"  // �������/�������������� ��������� 7
  "�"  // �������/�������������� ��������� 8
  "�"  // �������/�������������� ��������� 9
	"�"  // �������/�������������� ���������10
#if COURT_TYPE == COURT_ADMIN
  "�"  // �������/�������������� ���������11
  "�"  // �������/�������������� ���������12
  "�"  // �������/�������������� ���������13
  "�"  // �������/�������������� ���������14
  "�"  // �������/�������������� ���������15
  "�"  // �������/�������������� ���������16
  "�"  // �������/�������������� ���������17
  "�"  // �������/�������������� ���������18
  "�"  // �������/�������������� ���������19
  "�"  // �������/�������������� ���������20
  "�"  // �������/�������������� ���������21
  "�"  // �������/�������������� ���������22
  "�"  // �������/�������������� ���������23
  "�"  // �������/�������������� ���������24
  "�"  // �������/�������������� ���������25
#endif  // COURT_ADMIN
   ;
constant KIND_JUDGEMENTS_BY_NAME =
  "�"  // �������
   ;
constant KIND_DECISIONS_BY_NAME =
  "�"  // �������
  "R"  // ������� 2
   ;
constant KIND_DEFINITIONS_BY_NAME =
  "�"  // �����������
   ;
constant KIND_DECREES_BY_NAME =
  "�"  // ������������
   ;
constant KIND_PUN_GENERAL_LAWS = // 08.apr.2002
  "W"  // �����
  "O"  // ����
   ;
constant KIND_PUN_PRIVATE_LAWS = // 08.apr.2002
	"C"  // �����
	"H"  // ����
	"I"  // ���
	 ;

constant KIND_KEEP_METHOD_CHG_COMPLAINTS = "x" ;
constant KIND_ACCUSE_ACTS = "Au" ;

#endif  // ADMIN || AREAL || MILITARY


#if COURT_TYPE == COURT_REGIONAL

constant KIND_TEMP_MEASURES_SESSIONS =    // 20060310
  "�"   // ��������� �� ����������� �����
   ;

constant KIND_SESS_COMPLE_REPORT_DESIGNATION =    // 20060627 // RC, Report, 16089, �������� ����/� �������
  "�"   // ��������� �� ����������� �����
  "4"   // ����������� ���������
  "5"   // ����� ���������
  "6"   // ����� ���������
   ;

constant KIND_PSEUDO_SESSIONS =
	"9"   // ����� ���������
	"d"   // ��������� ����� ���������
	"L"   // ���������� �� ��. 126 "�" ���
   ;
constant KIND_PSEUDO_OR_CLOSED_SESSIONS =
  "9"
  "d"
  "L"  // 04.may.2000
  "l"
  "V"  // 12.jan.2001
  "W"  // 12.jan.2001
   ;

constant KIND_JUDGEMENTS_BY_NAME =
  "G"
//  "�"  // 01.aug.2000
    ;
constant KIND_DEFINITIONS_BY_NAME =
							    "S" ;
constant KIND_DECISIONS_BY_NAME =
                                            "E" ;
constant KIND_KEEP_METHOD_CHG_COMPLAINTS = "x" ;
constant KIND_ACCUSE_ACTS = "AQ" ;

constant KIND_INREG_EXACT_COMPLAINTS =
  "�"  // �������� �����
  "�"  // ���������� �����
   ;
constant KIND_INREG_PRIVATE_COMPLAINTS =
  "�"  // �������� ������ �����
  "�"  // ���������� ������ �����
   ;
constant KIND_INREG_EXACT_OBJECTIONS =
  "�"  // �������
   ;
constant KIND_INREG_PRIVATE_OBJECTIONS =
  "�"  // ������ �������
   ;
constant KIND_INREG_REJ_RETR_COMPLAINTS =
  "�"  // ����� �� ������
  "�"  // ����� �� �������
   ;
constant KIND_PUN_GENERAL_LAWS = // 08.apr.2002
  "O"  // ����
   ;
constant KIND_PUN_PRIVATE_LAWS = // 08.apr.2002
  "I"  // ���
   ;
#endif  // REGIONAL

#if INSTANCE
constant JUDGED_JUDGEDS = "ij"
						"p"  // 13.jun.2000
  "o"  // �������� �� ������������� ��. ���������  // 25.jul.2001
  "r"  // �������� ������������ �������  // 25.jul.2001
  "7"  // ��������.�� �����.�����.-��.78� ��  // 25.jul.2001
  "6"  // ��������.�� �����.�����.-��.61 ��� 64 ��  // 25.jul.2001
						   ;
constant JUDGED_CONVICTEDS =
  "j"
  "p"
  "r"  // �������� ������������ �������  // 25.jul.2001
  "7"  // ��������.�� �����.�����.-��.78� ��  // 25.jul.2001
  "6"  // ��������.�� �����.�����.-��.61 ��� 64 ��  // 25.jul.2001
   ;
constant JUDGED_CONVICTED_BULWARKS = "p" ;  // 13.jun.2000
constant JUDGED_ACQUITS =
  "i"
  "o"  // �������� �� ������������� ��. ���������  // 25.jul.2001
   ;
constant JUDGED_CANCELEDS =
//  "c"  // 10.sep.2001
  "s"  // ������������ �� ��.21 ��.1 �.1,2,3,4,5,6  // 10.sep.2001
   ;
constant JUDGED_FREEDS =
  "7"  // ��������.�� �����.�����.-��.78� ��  // 25.jul.2001
   ;
#else  // of INSTANCE
constant JUDGED_JUDGEDS = "ij"
						"p"  // 13.jun.2000
  "o"  // �������� �� ������������� ��. ���������  // 25.jul.2001
  "r"  // �������� ������������ �������  // 25.jul.2001
  "7"  // ��������.�� �����.�����.-��.78� ��  // 25.jul.2001
  "6"  // ��������.�� �����.�����.-��.61 ��� 64 ��  // 25.jul.2001
						   ;
constant JUDGED_CONVICTEDS =
  "j"
  "p"
  "r"  // �������� ������������ �������  // 25.jul.2001
  "7"  // ��������.�� �����.�����.-��.78� ��  // 25.jul.2001
  "6"  // ��������.�� �����.�����.-��.61 ��� 64 ��  // 25.jul.2001
   ;
constant JUDGED_CONVICTED_BULWARKS = "p" ;  // 13.jun.2000
constant JUDGED_ACQUITS =
  "i"
  "o"  // �������� �� ������������� ��. ���������  // 25.jul.2001
   ;
constant JUDGED_CANCELEDS =
//  "c"
  "s"  // ������������ �� ��.21 ��.1 �.1,2,3,4,5,6  // 10.sep.2001
   ;
constant JUDGED_FREEDS =
  "7"  // ��������.�� �����.�����.-��.78� ��  // 25.jul.2001
   ;
#endif  // of INSTANCE

#if INSTANCE
#else
constant PS2_KIND_INVESTIGATIONS = "G";// "p" ;
#endif  // 11.jan.2000 - end


const char

# if INSTANCE
#   if APPEAL
  * const PUNISHMENT_UP_TO_3_YEARS_S = "NC",
  * const PUNISHMENT_UP_TO_3_YEARS_CONDS = "C",
  * const PUNISHMENT_UP_TO_3_YEARS_EFFS = "N",
  * const PUNISHMENT_OTHERS_S = "o",
//  * const PUNISHMENT_COMPLEMENTS_S = "x",  16.may.2000

  * const PUNISHMENT_3_TO_10_YEARS_S = "3",
  * const PUNISHMENT_10_TO_30_YEARS_S = "t",
  * const PUNISHMENT_LIFE_IMPRISONMENTS = "w",
  * const PUNISHMENT_LIFE_IMPRISONMENT_NCS = "s"
#   else  // of APPEAL
	    // COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_ADMIN
  * const PUNISHMENT_FINE_ARL = "M",   // �����     // 20070611 - �� "�����������" �����

  * const PUNISHMENT_UP_TO_3_YEARS_S =
    "N"  // �� �� 3 �.-���������
    "C"  // �� �� 3 �.-�������
    ,
  * const PUNISHMENT_UP_TO_3_YEARS_CONDS =
    "C"
    ,
  * const PUNISHMENT_UP_TO_3_YEARS_EFFS =
    "N"
    ,
  * const PUNISHMENT_OTHERS_S =
    "o"  // ����� ���������
    "W"  // ������������ ����       // 26.jul.2001
    "m"  // ������������ ���������  // 26.jul.2001
    "M"  // �����                   // 26.jul.2001
    "P"  // ���������� ���������    // 26.jul.2001
    "d"  // �������� �� �����       // 26.jul.2001
    "p"  // ��������                // 02.jun.2004
    ,

  * const PUNISHMENT_OTHERS_S_CORR =
    "o"  // ����� ���������
    "W"  // ������������ ����       // 26.jul.2001
    "m"  // ������������ ���������  // 26.jul.2001
    // "M"  // �����                   // 26.jul.2001
    "P"  // ���������� ���������    // 26.jul.2001
    "d"  // �������� �� �����       // 26.jul.2001
    //"p"  // ��������                // 02.jun.2004
    ,

  * const PUNISHMENT_3_TO_10_YEARS_S =
    "3"  // �� 3-5�.
    "5"  // �� 5-10�.  // 26.jul.2001
    ,
  * const PUNISHMENT_10_TO_30_YEARS_S =
    "t"  // �� 10-15�.
    "f"  // �� 15-20�.  // 26.jul.2001
    "e"  // �� 20-30�.  // 26.jul.2001
    "T"  // �� ��� 30�.  // 26.jul.2001
    ,
  * const PUNISHMENT_LIFE_IMPRISONMENTS =
    "w"  // ��������� ������
    ,
  * const PUNISHMENT_LIFE_IMPRISONMENT_NCS =
    "s"  // ��������� ������ ��� ������
#   endif  // of APPEAL
# else  // of INSTANCE
  * const PUNISHMENT_UP_TO_3_YEARS_S =
	"N"  // �� �� 3 �-���������
	"C"  // �� �� 3 �-�������
	"k"  // �� �� 1 �-���������  // 10.jan.2001
	"u"  // �� �� 1 �-�������  // 10.jan.2001
	,
  * const PUNISHMENT_UP_TO_3_YEARS_CONDS =
	"C"  // �� �� 3 �-�������
	"u"  // �� �� 1 �-�������  // 10.jan.2001
	,
  * const PUNISHMENT_UP_TO_3_YEARS_EFFS =
	"N"  // �� �� 3 �-���������
	"k"  // �� �� 1 �-���������  // 10.jan.2001
	,
  * const PUNISHMENT_3_YEARS_EFFS =  // 01.mar.2001
	"N"  // �� �� 3 �-���������
	,
  * const PUNISHMENT_UP_TO_1_YEAR_S =  // 10.jan.2001
	"k"  // �� �� 1 �-���������
	"u"  // �� �� 1 �-�������
	,
  * const PUNISHMENT_UP_TO_1_YEAR_EFFS =  // 01.mar.2001
	"k"  // �� �� 1 �-���������
	,
  * const PUNISHMENT_OTHERS_S =
	"o"
	"s"  // 28.jul.1999
	"O"  // ����� ��������� - �������  // 20.apr.2001
	"m"  // ������������ ���������  // 26.jul.2001
	"P"  // ���������� ���������    // 26.jul.2001
	"d"  // �������� �� �����       // 26.jul.2001
    //"p"  // ��������                // 20050512  // 20050601 in comment
	"w"  // 20050601 added  ������������ ����
	"a"  // ��������� � ��������� �� ��� // 20050512
	"l"  // �� �� ������� // 2014:006
	,
//  * const PUNISHMENT_COMPLEMENTS_S = "x",  // 16.may.2000

  * const PUNISHMENT_OTHER_CONDIT_S =  // 20.apr.2001
	"O"  // ����� ��������� - �������
	,
  * const PUNISHMENT_3_TO_15_YEARS_S =
	"3"  // �� 3-5�.
	"5"  // �� 5-10�.  // 26.jul.2001
	"T"  // �� 10-15�.  // 26.jul.2001
	,
  * const PUNISHMENT_PENALTIES = "t",     //
  * const PUNISHMENT_WORK_EDUCATE_SCHOOL_S = "s"
# endif  // of INSTANCE
 ;

constant INVOLVEMENT_COMPLAINTLODGERS =  // 22.jan.2001
  "a"  // ������������
  "�"  // ������������- ��������
  "�"  // ������������- ��.����
  "�"  // ������������- �.���������
  "�"  // ������������- ��������
   ;

# if INSTANCE
#   if APPEAL
constant TYPE_TRADE_OR_CITIZEN_LAWS = "tc" ;
#   else
constant TYPE_TRADE_OR_CITIZEN_LAWS = "tc" ;
#   endif
# else
constant TYPE_TRADE_OR_CITIZEN_LAWS = "c" ;
# endif

#if COURT_TYPE == COURT_AREAL
constant
	ACCOMPLY_TL_630S = "0EF",	// former '0'
	ACCOMPLY_TL_632S = "2H";	// former '2'
#endif  // AREAL

const char
  * const CANNOT_OPEN_THE_TEMPORARY_FILE =
    "�� ���� �� ������ ��������� ����",
  * const UNEXPECTED_EVENT =
    "���������� (�� ����������) �������������",
  * const CANNOT_LOAD_ANY_COLLEGES =
    "��������� ��������� �� �������" ,
  * const CANNOT_LOAD_ANY_COMPOSITIONS =
    "��������� ��������� �� �������",
//  * const BUFF_TOO_SMALL = "������ ����� �����"
  * const OMISSIONS_WILL_BE_OCCURED =
    "�� ����� �������� � ����������� ��������",
  * const CANNOT_LOAD_ANY_LAW_TYPES =
    "��������� ��������� �� ������ ����",
  * const MORE_THAN_ONE_L_JUDGEMENT =
    "������ �� ���� ������ �������/�������/�����������"
//    " �� ���� �� ���������",
    " �� ���� �� ����",
  * const MORE_THAN_ONE_DES_G_JUDG =
    "������ �� ���� �������/�������/����������� �� ���������",
  * const CANNOT_LOAD_THE_FORMAT_INFO =
    "�� ���� �� ������ ������������ �� ������� �� ���������",
  * const BAD_FORMAT_INFO =
    "�������� ��� ���������� ���������� �� ������� �� ���������",
  * const CANNOT_READ_FROM_TMP_FILE =  // 22.jun.2001
    "������ ��� ������ �� ��������� ����",
  * const CANNOT_RW_TEMP_FILE =
    "������ ��� ����������� (������/�����) �� ��������� ����",
  * const CANNOT_WRITE_TO_TEMP_FILE =
    "������ ��� ����� ��� ��������� ����",
  * const TYPES_BUFF_TOO_SHORT =
    "������ ��� ����� �� �������/������ ����",
  * const UNABLE_TO_DEL_INS_STR =
    "Unable to delete/insert string to list",
  * const NO_TYPE_CHOOSEN =
    "���� ������ ��� ����",
  * const CANNOT_UNIFY_PUNISHMENT =
    "������������� ���� �� ����� �� �� ���������� � ������� ������.",
  * const FLYING_INREG_DOCS_STRING =
    "���������� � ���� ���������",
  * const WRONG_COMPOSITION_INDEXES = "Wrong composition indexes",
  * const WRONG_HALL_INDEXES = "Wrong hall indexes",
  * const WRONG_ARGS_PASSED_TO = "Incorrect argument(s) passed to",
  * const IMPROPER_ABSTRACT_METHOD_USE =
	  "Improper use of abstract method",
  * const IMPROPER_USE_OF = "Improper use of",
  * const IS_RETURNING_NULL = "is returning NULL",
  * const UNDAJUSTED_ARRAYS_PASSED_TO = "Unadjusted arrays passed to",
  * const NO_OUTPUT_CHOSEN = "�� � ������ �����",
  * const UNSUCCESSFUL_DOC_TEXT_GENERATING =
    "��������� ���������� �� ������ �� ���������",
  * const WRONG_ARR_DIMENSION = "wrong array dimension",
  * const UNSUCCESSFUL_OPERATION = "unsuccessful operation",
  * const UNSUCCESSFUL_ADD_TO_ARRAY = "unsuccessful add to array", //03.nov.2003
  * const UNSUCCESSFUL_DETECTING_OF = "��������� ���������� ��", // 24.aug.2004
  * const CANNOT_PERFORM_OPERATION = "cannot perform the operation",
  * const IS_NOT_IMPLEMENTED = "is not implemented",
  * const NULL_POINTER_IN_ARRAY = "NULL pointer in array",
  * const DISTRIB_HISTORY_CHECKUP = "������� �� ����������� �������������",
  * const NO_CHOICES =
    "���� ����������� �� ����� �� ���� ���� �� ���� ��������",
  * const NO_DATA_STR = "������� �����",
  * const WANT_HISTORY_CHECKUP_QUESTION =
//    "������� �� ������� �� ����������� �������������?",
    "������� �� ������� �� ���������� ��������� �������������?",
  * const RETURNING_EMPTY_OBJ_INSTEAD =
    "Returning empty object instead.",
  * const INCORRECT_PERIOD_GIVEN = "���������� ������� ������",
  * const UNSPECIFIED_EXCEPTION_CAUGHT = "���������� � ��������� ����������",
  * const BADLY_FILLED_OBJECT = "the object is not filled correctly",
  * const WANT_CONTINUE_QUESTION = "������������ ��?",
  * const ALL_COMPOSITIONS = "������",
  * const RESULT_WILL_BE_DELAYED =
    "���� �� ������ �� �������� �� ���������",
  * const LINK_COULD_NOT_BE_ADDED =
    "��������� �������� �� ������������� ������ ��� �����",
  * const NEWLY_ADDED_LINK_NOT_FOUND =
    "��������� ������������ ��� ����� ������",
  * const ABOLISHED_MOVE_STR =  // 29.may.2001
    "������ �� ���� �� �������� - ",
  * const ARG_IS_NULL_OR_EMPTY =  // 17.oct.2001
    " argument is NULL or empty",
  * const EMPTY_FMT = "text\\empty.fmt",  // 17.oct.2001
  * const NO_LAW_TYPES_TO_START_REGIME =
    "���� ������ ����, �� ����� �� ���� ��������� �������.", // 02.nov.2001
  * const INSUFFICIENT_RIGHTS_TO_START =
    "��� ������ ���������� �����, �� �� ���������� ������.", // 02.nov.2001
  * const SQL_SELECT = "SELECT ",  // 19.mar.2002
  * const SQL_WHERE = " WHERE ",  // 19.mar.2002
  * const SQL_ORDER_BY = " ORDER BY ",  // 19.mar.2002
  * const PLEASE_CHECK_ANY_INST =
# if INSTANCE
#   if APPEAL
    " "
#   else  // of APPEAL
    "����, �������� ������� ��� ���� �� ���������"
#   endif  // of APPEAL
# else  // of INSTANCE
    " "
# endif  // of INSTANCE
  ,
  * const FIRM_LAWS_ARE_1ST_INSTANCE =
# if INSTANCE
#   if APPEAL
    " "
#   else  // of APPEAL
    "��������� ���� �� ���� �����������������"
#   endif  // of APPEAL
# else  // of INSTANCE
    " "
# endif  // of INSTANCE
  ,
  * const BUFFER_TOO_SHORT = "Buffer is too short." ;
   ;

constant PLEASE_WAIT = "����, ��������� ..." ;
constant PLEASE_WAIT_EXTRACTING =
  "���� ���������, �������� ����� ..." ;
constant PLEASE_WAIT_GENERATING =
	"���� ���������, ��������� �������� �������� ..." ;
constant SAVING = "�����..." ;  // 28.feb.2003
constant LOADING = "���������...";  // 28.feb.2003
constant EXTRACTING_DATA = "��������� �� �����" ;
constant MAKING_GENERAL_ROW = "���������� �� ������� ���" ;
constant CREATING_OUT_DOC = "��������� �� �������� ��������" ;
constant STEP = "������" ;  // 11.feb.2003
constant PASSEDS = "����������" ;  // 11.feb.2003
constant PLEASE_WAIT_CHECKING_FINISHEDS =
	"����� �������� �� ���������� ����. ����, ��������� ..." ;
constant PLEASE_WAIT_CHECKING_SESSIONS =
	"����� �������� �� �����������. ����, ��������� ..." ;
constant PLEASE_WAIT_PROCESS_FINISHEDS =
	"���� ���������, ���������� ���������� ���� ..." ;
constant PLEASE_WAIT_PROCESS_NON_FINISHEDS =
	"���� ���������, ���� ���������� ���������� ���� ..." ;
constant PLEASE_WAIT_PROCESS_SESSIONS =
	"���� ���������, ���������� ����������� ..." ;
constant PLEASE_WAIT_LOOKING_UNWRITTENS =
  "���� ���������, �������� �� ���������� ������ ..." ;
constant PLEASE_WAIT_PROCESS_OTHERS =
  "���� ���������, ���������� ������� ������� ������ ..." ;
constant PLEASE_WAIT_PROCESS_FOR_EXAM =
  "���� ���������, ���������� ������ �� ����������� ..." ;
constant PLEASE_WAIT_PROCESS_REQUESTS =
  "���� ���������, ���������� ���������� ..." ;
constant PLEASE_WAIT_PROCESS_NEW_LAWS =
  "���� ���������, ���������� ��������������� ���� ..." ;
constant PLEASE_WAIT_PROCESS_DECISIONS =
  "���� ���������, ���������� ��������� ..." ;
constant ALMOST_DONE = "����� ������ ..." ;  // 06.feb.2002
constant CHECKUPS = "�������" ;  // 14.feb.2002
constant SUMMARY = "�������������" ;  // 14.feb.2002
constant SUMMARY_DATA = "����� �� �������������" ; // 21.jun.2002
constant FINISHED_LAWS = "�������� ����" ;  // 21.jun.2002
constant SURROUND_KIND_OMITTED =
	"The current surround kind is not fully supported yet." ;
constant DEFINITION_OR_DECREE =
	"�����������/������������" ;

constant CONDENSED_PRINT_S = "\xf" ;
const char* const NORMAL_PRINT_S     = "\x12" ;

constant CONTINUE_QUEST = "������������ ��?" ;
constant WRONG_DATED_SIDE_TYPE =
  "Unexpected kind of the dated side" ;
constant WRONG_DATED_SIDE_STATE =
  "Unexpected state of the dated side" ;
constant WRONG_DATED_CONTAINER_TYPE =
  "Unexpected kind of the dated container" ;

# if INSTANCE
#   if APPEAL
constant SUPREME_CONCERN_COURT = "���" ;
#   else  // of APPEAL
constant SUPREME_CONCERN_COURT = "���/����" ;
#   endif  // of APPEAL
# else  // of INSTANCE
constant SUPREME_CONCERN_COURT = "���/����" ;
constant PUNISHM_ADMIN_REPORT_FILE_NAME = "text\\reportpa.fmt" ;
# endif  // of INSTANCE

const char* const HTML_CODE_ON = "(_!H!T!M!L!_!C!O!D!E!_!O!N!_)";
const char* const HTML_CODE_OFF = "(_!H!T!M!L!_!C!O!D!E!_!O!F!F!_)";

constant BLOCK_NO = "��.N " ;

constant AppNameMinistry = "������������ �� ������������" ;
constant AppNameMinistryShort = "��" ;  // ��->����->��-> ...
constant AppNameTheMinistry = "�������������� �� ������������" ;

constant KEEPING_METHOD_PRT_STRING = "����� �� ������������" ;

#if COURT_TYPE == COURT_APPEAL
   constant DEFAULT_FORMAT_FILE_EXTENSION = ".fmp" ;
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
   constant DEFAULT_FORMAT_FILE_EXTENSION = ".fmd" ;
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
   constant DEFAULT_FORMAT_FILE_EXTENSION = ".fma" ;
#endif  // AREAL
#if COURT_TYPE == COURT_REGIONAL
   constant DEFAULT_FORMAT_FILE_EXTENSION = ".fmt" ;
#endif  // REGIONAL

constant UNKNOWN_CRIMINAL = "���������� ����������" ;

#if COURT_TYPE == COURT_APPEAL
constant HIGHER_INST_1 = "��-������ ���������" ;
constant HIGHER_INST_2 = "��-������ ��������� II" ;
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN
constant HIGHER_INST_1 = "��-������ ���������" ;
constant HIGHER_INST_2 = "��-������ ��������� II" ;
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL
constant HIGHER_INST_1 = "���������� ���" ;
constant HIGHER_INST_2 = "���������� ��� II" ;
#endif  // AREAL

#if COURT_TYPE == COURT_MILITARY
constant HIGHER_INST_1 = "��-������ ���������" ;
constant HIGHER_INST_2 = "��-������ ��������� II" ;
#endif  // MILITARY

#if COURT_TYPE == COURT_REGIONAL
constant HIGHER_INST_1 = "������� ���" ;
constant HIGHER_INST_2 = "������� ��� II" ;
#endif  // REGIONAL

constant AGE_NONES =
  "0"
  ;
constant AGE_UP_TO_ONES =  // 10.feb.2005
  "1"  // �� 1 �����
  ;
constant AGE_UP_TO_THREES =
  "1"  // �� 1 �����
  "2"  // �� 1 �� 3 ������
  "3"  // �� 3 ������
  ;

constant  AGE_MORE_THAN_THREES_ALL =  //20050318
  "6"  // �� 3 �� 6 ������
  "7"  // �� 6 �� 12 ������
  "9"  // ��� 3 ������
  "8"  // ��� 12 ������
  ;

constant AGE_UP_TO_SIXES =  // 10.feb.2005
  "1"  // �� 1 �����
  "2"  // �� 1 �� 3 ������
  "3"  // �� 3 ������
  "6"  // �� 3 �� 6 ������
  ;
constant AGE_UP_TO_TWELVES =  // 10.feb.2005
  "1"  // �� 1 �����
  "2"  // �� 1 �� 3 ������
  "3"  // �� 3 ������
  "6"  // �� 3 �� 6 ������
  "7"  // �� 6 �� 12 ������
  ;
constant AGE_MORE_THAN_THREES =
  "9"  // ��� 3 ������
  ;
constant AGE_MORE_THAN_TWELVES =  // 10.feb.2005
  "8"  // ��� 12 ������
  ;

#     if PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
const CDate PATCH_04_AUG_2000_RDT_DATE (1, 1, 2001) ;  // 28.sep.2000
#     endif  // PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
const CDate VARNA_FIRM_LAWS_NOT_FIN_DATE (1, 1, 1998) ;  // 05.oct.2000

constant GRANT_REJECTEDS =
  "r"  // ���������
  "z"  // ������� �� �������
  ;  // 20.apr.2001

constant DEFAULT_BRIEF_SESSION_FIELDS =
  "F_TYPE, F_YEAR, F_NO, F_DATE, F_KIND, F_RESULT, F_RETURNED, "
  "F_JUDGE, F_JUDGE_X, F_REJUDGE_X, F_PRESIDENT, F_TIME" ;  // 05.jun.2001


# if INSTANCE    // 20060331
#    if APPEAL   // APPEAL
  constant TB_SUPPRESS_SESS_RESULTS = " ";
#    else        // AREAL
  constant TB_SUPPRESS_SESS_RESULTS =
    "B" // ���������� �� ������������� �� ��.287
    "P"	// ���������� �� ��������
    "s" // ��������� �� ����������
    "c" // ����������
     ;
#    endif
# else           // REGIONAL
  constant TB_SUPPRESS_SESS_RESULTS =
    "B" // ������� �� �������������
    "P"	// ���������� �� ��������
    "s" // ��������� �� ����������
    "c" // ����������
     ;
# endif

//

const int MAX_JUDGEMENT_FLAGS = 7 ;  // 2011:080 6 -> 7		
const int MAX_USER_FLAGS = 6 ;  // 20.nov.2002
const int MAX_QUICK_FLAGS = 6 ;  // 23.sep.2003
const int MAX_LAWSUIT_FLAGS = 6 ;  // 01.mar.2004
//
// 6 flags: the maximum possible length of the part an SQL criteria is
// 9*2 + 64*3 = 18 + 192 = 210 (+ 10) characters
// in the following format: 1,2,....,62,63
// Of cource, don't forget the "in ()" expression --> tis is "+10" for.
//


extern const CDate VARNA_FIRM_LAWS_NOT_FIN_DATE ;

const size_t WRITE_STRING_LEN_AS_IS = 255 ;  // 29.oct.2001
const int MIN_PAGE_LINES = 30 ;
const int A4_PORTRAIT_CONDENSED_SYMBOLS = 137 ;  // 22.feb.2001
const int A4_MAX_PORTRAIT_CONDENS_SYMBOLS = 160 ;  // 22.feb.2001
const int ANNOUNCE_ANNOUNCE_NO_INDEX = 5 ;  // 03.may.2001
const size_t MIN_FORMAT_NAME_BUFF_SIZE = 8+3+1+8+3+1;  // 10.apr.2002

const int  // Moved from _data_.h on 13.feb.2003
  SIZE_OF_NAME_FOR_BOOK = 63 + 1,
  SIZE_OF_ADDRESS_FOR_BOOK = 63 + 1,
  SIZE_OF_REG_MAIL_TEXT = 101,  // 28.jul.2003
  SIZE_OF_SUBP_RCV_STAT_LAW_KINDS = 64 + 1,  // 29.nov.2003
  SIZE_OF_SUBP_RCV_STAT_SUBP_KINDS = 128 + 1,  // 10.oct.2003
  SIZE_OF_HISTORY_RET_LAW_TEXT = 100 + 1,	// 2011:109
  SIZE_OF_SUBJECT_RANGE_NAME = 120 + 1	// 2016:061 former SUBJECT_NAME
  ;

const char STUB_CHAR = '\x1B' ;

const char WLLW_CONVERTED = 'y';  // 09.dec.2003

# if INSTANCE

#   if APPEAL        // APPEAL -->
const long int SUBJECT_9000 = 9000 ;
const long int SUBJECT_9050 = 9050 ; // Vesi
const long int SUBJECT_9150 = 9150 ; // Vesi
// --- Thereby yet... --------------
//const long int SUBJECT_9000 = 9000 ; //20070503//9200; //20060718//9000;  // Anelia; 2016:060 obsolete
const long int SUBJECT_9001 = 9001 ; //20070503//9201; //20060718//9001;  // Anelia
const long int SUBJECT_9002 = 9002 ; //20070503//9202; //20060718//9002;  // Anelia
const long int SUBJECT_9003 = 9003 ; //20070503//9203; //20060718//9003;  // Anelia
const long int SUBJECT_9004 = 9004 ; //20070503//9204; //20060718//9004;  // Anelia
			   // APPEAL <--

#   else             // AREAL  -->
// --- Thereby yet... --------------
//const long int SUBJECT_9000 = 9200 ; // 20060718 // 9000 ;  // Anelia; 2016:060 obsolete
const long int SUBJECT_9001 = 9201 ; // 20060718 // 9001 ;  // Anelia
const long int SUBJECT_9002 = 9202 ; // 20060718 // 9002 ;  // Anelia
const long int SUBJECT_9003 = 9203 ; // 20060718 // 9003 ;  // Anelia
const long int SUBJECT_9004 = 9204 ; // 20060718 // 9004 ;  // Anelia

//const long int SUBJECT_1905 = 1905 ;  // Anelia  // 14.dec.2000; 2016:060 obsolete
const long int SUBJECT_9005 = 9205 ;  // 20060718 // 9005 ;  // Lucy  // 11.dec.2003
#   endif            // AREAL  <--

# endif  // of INSTANCE

// 2016:060 LPR: for law_is_237_enactment
#if COURT_TYPE == COURT_REGIONAL
const
	SUBJECT_CANCEL_ENACTMENT = 304000,
	OLD_SUBJECT_CANCEL_ENACTMENT = 1488,
	OLD_SUBJECT_P_ADM_LOW_SUBJ_RNG = 2000,
	OLD_SUBJECT_P_ADM_HIGH_SUBJ_RNG = 2099,
	// 2017:046 LPR: now for exclusion
	//SUBJECT_P_ADM_LOW_SUBJ_RNG = 400000,
	//SUBJECT_P_ADM_HIGH_SUBJ_RNG = 499999,
	SUBJECT_P_ADM_LOW_SUBJ_RNG = 100,
	SUBJECT_P_ADM_HIGH_SUBJ_RNG = 199;
#endif

// 27.oct.2000 - begin
//const long int SUBJECT_1609 = 1609 ;  // Anelia
//const long int SUBJECT_1610 = 1610 ;  // Anelia
//const long int SUBJECT_1620 = 1620 ;  // Anelia
//const long int SUBJECT_1621 = 1621 ;  // Anelia
//const long int SUBJECT_1632 = 1632 ;  // Anelia
const long int SUBJECT_1910 = 1910 ;  // Anelia
const long int SUBJECT_1920 = 1920 ;  // Anelia
const long int SUBJECT_1921 = 1921 ;  // Anelia
const long int SUBJECT_1932 = 1932 ;  // Anelia

// 27.oct.2000 - end   // 20070118 Прекодиране
//const long int SUBJECT_1485 = 1485 ;  // Lucy   --> 1482
//const long int SUBJECT_1490 = 1490 ;  // Lucy   --> 1481
//const long int SUBJECT_1500 = 1500 ;  // Lucy   --> 1486
//const long int SUBJECT_1501 = 1501 ;  // Lucy   --> 1487

const long int SUBJECT_1482 = 1482 ;  // 20070118 Прекодиране
const long int SUBJECT_1481 = 1481 ;  // 20070118 Прекодиране
const long int SUBJECT_1486 = 1486 ;  // 20070118 Прекодиране
const long int SUBJECT_1487 = 1487 ;  // 20070118 Прекодиране

const long int SUBJECT_MAX = 9999;  // 11.mar.2003

# if APPEAL
# else  // of APPEAL
#   if INSTANCE
const long int SUBJECT_PPL_PRELIMINARY_BEG = 1910 ; // 26.oct.2000 // The
// correct value is most likely 1905, but no one has complained yet.
// 06.dec.2000
const long int SUBJECT_PPL_PRELIMINARY_END = 1989 ; // 1699,// 18.oct.2000
                                                    // 1689 // 26.oct.2000
const long int SUBJECT_2ND_PRELIMINARY_BEG = 9201 ; // 9001 ;  // 20060718
const long int SUBJECT_2ND_PRELIMINARY_END = 9204 ; // 9004 ;  // 20060718
#   else  // of INSTANCE
const long int SUBJECT_PPL_PRELIMINARY_BEG = 1480 ;
const long int SUBJECT_PPL_PRELIMINARY_END = 1489 ; // = 1560 ; //20070118
#   endif // of INSTANCE
# endif  // of APPEAL
// ---------------------------------

const int
//  SUMMONS_TEXT_SIZE = (2 * 55 + 1),  // 19.nov.2001
  MAKE_COLUMNS_BUFF_SIZE = SIZE_OF_DECISION_TEXT + 10,
  NFB_DLG_STATIC1_SIZE = 10 + 10 + 10,
  NFB_DLG_STATIC_2_3_4_SIZE = 81,
  ED_NFB_DLG_COURT_STATIC_SIZE = SIZE_OF_NAME + SIZE_OF_UCN + 10,
  SIZE_OF_CITIZEN_INITIALS = 5,  // 03.jun.2002
  FMT_SECTION_NAME_SIZE = 81 ;

const int
  RIGHT_ALIGN = 0,
  LEFT_ALIGN = 1,
  CENTER_ALIGN = 2,
  JUSTIFY_ALIGN = 3 ;  // 23.nov.2001

const char BONEVA_SIDES_DELIMITER = (char) 160 ;

extern constant EMPTY_STRING ;
extern constant STAR_STRING ;
extern constant Q3_STRING ;
extern constant DASH_STRING ;

extern constant JUDGE_STRING ;
extern constant SECRETAR_STRING ;
extern constant PROSECUTOR_STRING ;
extern constant JUROR_STRING ;
extern constant REJUROR_STRING ;
extern constant JUDGEX_STRING ;
extern constant REJUDGEX_STRING ;
extern constant JURORX_STRING ;

extern constant BLOCK_NO ;
extern constant UNKNOWN_CRIMINAL ;
extern constant WANT_CONTINUE_QUESTION ;
extern constant WRONG_DATED_SIDE_TYPE ;
extern constant WRONG_DATED_SIDE_STATE ;
extern constant WRONG_DATED_CONTAINER_TYPE ;
extern constant IMPROPER_ABSTRACT_METHOD_USE ;
extern constant IMPROPER_USE_OF ;
extern constant IS_RETURNING_NULL ;
extern constant UNDAJUSTED_ARRAYS_PASSED_TO ;
extern constant NO_OUTPUT_CHOSEN ;

extern constant SUPREME_CONCERN_COURT ;

extern const char* const HTML_CODE_ON;  // 20.nov.2002
extern const char* const HTML_CODE_OFF;  // 20.nov.2002

const char
  SUMMONS_FIXED = 'f',
  SUMMONS_UNMOVEABLE = 'u',
  SUMMONS_STOPPED = 's',
  SUMMONS_CANCELED = 'c',
  SUMMONS_ANY_OTHER_RESULT = 'o' ;

extern constant SUMMONS_STANDARD_RESULTS ;  // 18.may.2001
// This string must include all SUMMONS_XXX characters, listed above

const char  // Subpoena kinds 14.oct.2003
  SK_WILL_BE_SUMMONED = 'u',
  SK_SUBPOENA_ANN_254 = 'A',
  SK_PREVIOUSLY_NOTIFIED = 'i',
  SK_NOTIFIED_AT_CURRENT_EVENT = 'c',
  SK_NULL = '\0';

const char  // 07.oct.2003
SUBPOENA_OFFICIALLY_NOTIFIED = 'y',
SUBPOENA_OFFICIALLY_NONNOTIFIED = 'n',
SUBPOENA_OFFICIALLY_NULL = '\0';

const char  // 26.jan.2004 Subpoen kind - show subpoena date
SKSSD_SHOW = 'y',
SKSSD_DONTSHOW = 'n',
SKSSD_NULL = '\0';

const char  // 20070803 T_SUBPOENA_KIND  F_SUBPOENA_KIND
SK_SK_SRG = 'k',
SK_SK_SRG_VAS = 'm';

const char
  SUMMONS_FOR_EXPERT = 'e',
  SUMMONS_FOR_SIDE = 's',
  SUMMONS_FOR_WITNESS = 'w',
  SUMMONS_FOR_ACCUSED = 'a' ;

const char
  SIGN_EXTERNAL_USE = 'e',
  SIGN_INTERNAL_USE = 'i',
  SIGN_FOR_BOSS = 'b',
  SIGN_FOR_CORRUPT = 'r',
  SIGN_INTERNAL_USE_COMBINED = 'c',
  SIGN_FOR_VSS = 'v' ;

const char
  TYPESIGN_GENERAL_REPORT = 'g' ;  

const char
  ATTRIB_BUILD_IN = 'b',

# if COURT_TYPE == COURT_ADMIN
  ATTRIB_VSS = 'v',        // T_SUBJECT_RANGE   F_ATTRIB = 'v' => отчет за ВСС
# endif

  ATTRIB_DO_NOT_COUNT_DEFENDANTS = 'u' ;

const int
  SPARE_PAGE_LINES = 5 ;

// ----------------------------------------------------------------------

const char ACCOMPLY_ACCOMPANY_LETTER = 'z' ;  // 27.nov.2001 Придружително писмо
const char ACCOMPLY_ACCOMPANY_LETTER_EXTRA = 'y' ;  // 15.feb.2002 Придружително писмо за суми по сметка на ОС
const char ACCOMPLY_CONVICTION_BULLETIN = 'B' ;  // 16.jun.2003 Бюлетин за съдимост
const char ACCOMPLY_CONVICTION_BURO_LETTER = 'j' ;  // 19.jun.2003 // Писмо до Бюро съдимост
const char ACCOMPLY_PROSECUTOR_OFFICE_LET = 'w' ;  // 19.jun.2003 // Писмо до Окръжна прокуратура към препис от присъда/споразумение

#if COURT_TYPE == COURT_AREAL
const char  // 11.jan.2002
  ACCOMPLY_TL_631 = '6',  // чл.631 от ТЗ - отхвърляне на молбата
  ACCOMPLY_TL_692 = '9',  // чл.692, ал.1 от ТЗ - одобряване списъка на приети вземания
  ACCOMPLY_TL_707 = '7',  // чл.707 от ТЗ - утвърждаване на оздравителен план
  ACCOMPLY_TL_711 = '1',  // чл.711 от ТЗ - обявяване в несъстоятелност
  ACCOMPLY_TL_729 = 'A',  // чл.729 от ТЗ - одобряване сметка за разпределение
  ACCOMPLY_TL_735 = '3',  // чл.735 от ТЗ - прекратяване поради приключване
  ACCOMPLY_TL_740 = '8',  // чл.740 от ТЗ - извънсъдебно споразумение с прекратяване
  ACCOMPLY_TL_744 = '4',  // чл.744 от ТЗ - възобновени производства
  ACCOMPLY_TL_755 = '5'   // чл.755 от ТЗ - възстановяване правата на длъжника
   ;

// 2011:013 LRQ: single -> sets
extern constant
	ACCOMPLY_TL_630S,
	ACCOMPLY_TL_632S;
#endif  // AREAL

extern const char
  * const CANNOT_OPEN_THE_TEMPORARY_FILE,
  * const UNEXPECTED_EVENT,
  * const CANNOT_LOAD_ANY_COLLEGES,
  * const CANNOT_LOAD_ANY_COMPOSITIONS,
  * const OMISSIONS_WILL_BE_OCCURED,
  * const ABOLISHED_MOVE_STR,  // 29.may.2001
  * const ARG_IS_NULL_OR_EMPTY,  // 17.oct.2001
  * const EMPTY_FMT,  // 17.oct.2001
  * const NO_LAW_TYPES_TO_START_REGIME,  // 02.nov.2001
  * const INSUFFICIENT_RIGHTS_TO_START,  // 02.nov.2001
  * const SQL_SELECT,  // 19.mar.2002
  * const SQL_WHERE,  // 19.mar.2002
  * const SQL_ORDER_BY,  // 19.mar.2002
  * const PLEASE_CHECK_ANY_INST,  // 27.sep.2001
  * const FIRM_LAWS_ARE_1ST_INSTANCE,  // 21.nov.2001
  * const DEFINITION_OR_DECREE,  // 08.oct.2001
  * const LINK_COULD_NOT_BE_ADDED,  // 27.jul.2001
  * const NEWLY_ADDED_LINK_NOT_FOUND,  // 27.jul.2001
  * const CANNOT_LOAD_ANY_LAW_TYPES,
  * const MORE_THAN_ONE_L_JUDGEMENT,
  * const MORE_THAN_ONE_DES_G_JUDG,
  * const CANNOT_LOAD_THE_FORMAT_INFO,
  * const BAD_FORMAT_INFO,
  * const CANNOT_READ_FROM_TMP_FILE,  // 22.jun.2001
  * const CANNOT_RW_TEMP_FILE,
  * const CANNOT_WRITE_TO_TEMP_FILE,
  * const TYPES_BUFF_TOO_SHORT,
  * const BUFFER_TOO_SHORT,  // 21.jan.2005
  * const UNABLE_TO_DEL_INS_STR,
  * const NO_TYPE_CHOOSEN,
  * const CANNOT_UNIFY_PUNISHMENT,
  * const FLYING_INREG_DOCS_STRING,
  * const WRONG_COMPOSITION_INDEXES,
  * const HIGHER_INST_1,
  * const HIGHER_INST_2,
  * const CONTINUE_QUEST,
  * const ALL_COMPOSITIONS,
  * const RESULT_WILL_BE_DELAYED,
  * const WRONG_HALL_INDEXES,
  * const WRONG_ARGS_PASSED_TO,
  * const UNSUCCESSFUL_DOC_TEXT_GENERATING,
  * const UNSUCCESSFUL_OPERATION,  // 18.feb.2003
  * const UNSUCCESSFUL_ADD_TO_ARRAY,  // 03.nov.2003
  * const UNSUCCESSFUL_DETECTING_OF,  // 24.aug.2004
  * const CANNOT_PERFORM_OPERATION,  // 07.oct.2003
  * const IS_NOT_IMPLEMENTED,  // 28.nov.2003
  * const NULL_POINTER_IN_ARRAY,  // 04.nov.2003
  * const NO_CHOICES,  // 21.mar.2003
  * const DISTRIB_HISTORY_CHECKUP,  // 29.may.2003
  * const NO_DATA_STR,  // 29.may.2003
  * const WANT_HISTORY_CHECKUP_QUESTION,  // 30.may.2002
  * const RETURNING_EMPTY_OBJ_INSTEAD,  // 12.mar.2003
  * const WRONG_ARR_DIMENSION,  // 11.mar.2003
  * const INCORRECT_PERIOD_GIVEN,
  * const UNSPECIFIED_EXCEPTION_CAUGHT,  // 07.feb.2003
  * const BADLY_FILLED_OBJECT  // 07.feb.2003
   ;

extern constant BULGARIAN_VOWELS ;  // 27.mar.2001

extern const char
  * const PLEASE_WAIT,
  * const PLEASE_WAIT_EXTRACTING,
  * const PLEASE_WAIT_GENERATING,
  * const SAVING,  // 28.feb.2003
  * const LOADING,  // 28.feb.2003
	* const EXTRACTING_DATA,  // 04.jun.2002
	* const MAKING_GENERAL_ROW,  // 04.jun.2002
	* const CREATING_OUT_DOC,  // 04.jun.2002
  * const STEP,  // 11.feb.2003
  * const PASSEDS,  // 11.feb.2003
	* const PLEASE_WAIT_CHECKING_FINISHEDS,
  * const PLEASE_WAIT_CHECKING_SESSIONS,
  * const PLEASE_WAIT_PROCESS_FINISHEDS,
  * const PLEASE_WAIT_PROCESS_NON_FINISHEDS,
  * const PLEASE_WAIT_PROCESS_SESSIONS,
  * const PLEASE_WAIT_LOOKING_UNWRITTENS,
  * const PLEASE_WAIT_PROCESS_OTHERS,
  * const PLEASE_WAIT_PROCESS_FOR_EXAM,
  * const PLEASE_WAIT_PROCESS_NEW_LAWS,
  * const PLEASE_WAIT_PROCESS_DECISIONS,
  * const ALMOST_DONE,  // 06.feb.2002
  * const CHECKUPS,  // 14.feb.2002
  * const SUMMARY,  // 14.feb.2002
	* const SUMMARY_DATA,  // 21.jun.2002
	* const FINISHED_LAWS,  // 21.jun.2002
  * const SURROUND_KIND_OMITTED,
  * const PLEASE_WAIT_PROCESS_REQUESTS ;

// ----------------------------------
const int
  MISC_OUTPUT_EXTRA_TXT = 257,
  MISC_OUTPUT_GIVEN_NUM = 250,
  MISC_OUTPUT_STRING_UNCHANGED = 248,
  MISC_OUTPUT_STRING_AS_COLUMN = 247,
  MISC_OUTPUT_STRING_AS_COLUMN_BR = 246,
  MISC_OUTPUT_STRING_TO_HTML_SWPBR = 245,  // 04.mar.2003
  MISC_SMALLEST_SPECIAL_CIPHER = 245 // must be the smallest
   ;
// ----------------------------------

extern const char
  * const CONDENSED_PRINT_S,
  * const NORMAL_PRINT_S
    ;

const char
  SIDE_MARK_NO_INDIRECT = '0',
  SIDE_MARK_LIKE_LAWYER = '1',
  SIDE_MARK_INDIRECT_AS_ANY = '2',
  SIDE_MARK_CONSENT_OF_INDIRECT = '3',
  SIDE_MARK_INDIRECT_AS_ANY_1 = '4',    // 20070730 new link (new button in TSidesRelationDialog)
  SIDE_MARK_INDIRECT_AS_ANY_2 = '5',    // 20080301 new link (new button in TSidesRelationDialog)
  SIDE_MARK_INDIRECT_AS_ANY_3 = '6',     // 20080301 new link (new button in TSidesRelationDialog)
  SIDE_MARK_AS_WITH_INVOLVEMENT = '7'	// 2014:206 +related
   ;

const char  // 08.sep.2003
  REGISTERED_MAIL_PREPARED = 'p',
  REGISTERED_MAIL_SENT = 's'
  ;

const unsigned int
  OFFSET_MISC_CIPHER = 0,
  OFFSET_LAWSUIT_CIPHER = 1000,
  OFFSET_SESSION_CIPHER = 2000,
  OFFSET_SIDE_CIPHER = 3000,
  OFFSET_CITIZEN_CIPHER = 4000,
  OFFSET_FIRM_LIKE_CITIZEN_CIPHER = OFFSET_CITIZEN_CIPHER,
  OFFSET_FIRM_CIPHER = 6000,
  OFFSET_BEGN_ENDN_YEAR_CIPHER = 7000,
  OFFSET_C_C_BD_ED_BN_CIPHER = 8000,
  OFFSET_MONTH_CIPHER = 9000,
  OFFSET_TIME_SUMMARY_CIPHER = 10000,
  OFFSET_SUMMONS_CIPHER = 11000,
  OFFSET_ABC_BOOK_CIPHER = 12000,
  OFFSET_TIME_BOOK_CIPHER = 13000,
  OFFSET_PERSON_LIST_CIPHER = 14000,
  OFFSET_DESK_BOOK_CIPHER = 15000,
  OFFSET_REPORT_BOOK_CIPHER = 16000,
  OFFSET_BMY_EMY_T_C_IU_CIPHER = 17000,
  OFFSET_CLOSED_SESS_BOOK_CIPHER = 18000,
  OFFSET_COL_BD_ED_BN_CIPHER = 19000,
  OFFSET_REGIONAL_AREAL_OUTPUT = 20000,
  OFFSET_COLLEGE_DATE_CIPHER = 21000,
  OFFSET_SESSION_LIST_CIPHER = 22000,
  OFFSET_REPORT_BOOK_CIPHER_2 = 23000,
  OFFSET_SIDE_LIST_CIPHER = 24000,
  OFFSET_JUDGE_CHECKUP_CIPHER = 25000,
  OFFSET_SUMMARY_DATA_CIPHER = 26000,
  OFFSET_ADDRESS_CIPHER = 27000,
  OFFSET_WALN_CIPHER = 28000,
  OFFSET_DAMN_LIKE_CITIZEN_CIPHER = OFFSET_CITIZEN_CIPHER,
  OFFSET_DAMN_CIPHER = 30000,
  OFFSET_FRGN_ADR_LIKE_ADDR_CIPHER = OFFSET_ADDRESS_CIPHER,
  OFFSET_INPUT_DIARY_CIPHER = 32000,
  OFFSET_SESS_LST_FOR_PER_CIPHER = 33000,
  OFFSET_ANNOUNCE_CIPHER = 34000,
  OFFSET_APPL_LAWS_IN_REG_CIPHER = 35000,
  OFFSET_APPL_LAWS_OUT_REG_CIPHER = 36000,
  OFFSET_RESULTS_SET_CIPHER = 37000,
  OFFSET_BEGN_ENDN_YEAR_AC_CIPHER = OFFSET_BEGN_ENDN_YEAR_CIPHER,
  OFFSET_EXECUTION_BOOK_CIPHER = 38000,
  OFFSET_TRADE_634_DIALOG_CIPHER = 39000,
  OFFSET_TRADE_634_BOOK_CIPHER = 40000,
	OFFSET_DECISION_CIPHER = 41000,
  OFFSET_WEB_CHECKUP_DIALOG_CIPHER = 42000,
  OFFSET_WEB_CHECKUP_CIPHER = 43000,
  OFFSET_INREG_CIPHER = 44000,
  OFFSET_INREG_EVENT_CIPHER = 45000,
  OFFSET_INREG_EVENT_ANNNC_CIPHER = 46000,
  OFFSET_OUTREG_CIPHER = 47000,
  OFFSET_OUTREG_LETTER_CIPHER = 48000,
  OFFSET_SLOWDOWN_BOOK_CIPHER = 49000,
  OFFSET_REQ_RESOLUTION_ANNOUNCE = 50000,
  OFFSET_STRING_ARRAY_CIPHER = 51000,
  OFFSET_REQUEST_CIPHER = 52000,
  OFFSET_REQ_RESOLUTION_CIPHER = 53000,
  OFFSET_ACT_EXECUTION_BOOK = 54000,
  OFFSET_JUDGEMENT_OUTPUT = 55000,
  OFFSET_BANKRUPTCY_LEFT = 56000,
  OFFSET_BANKRUPTCY_MIDDLE = 57000,
  OFFSET_BANKRUPTCY_RIGHT = 58000,
  OFFSET_BANKRUPTCY_DLG_DATA = 59000,
  OFFSET_ARCH_BK_DLG_DATA = 60000,
  OFFSET_ARCHIVE_BOOK = 61000,
  OFFSET_PROVE_BOOK = 62000,
	OFFSET_APL_LAWS_BY_JUDGES_CIPHER = 63000,  // Not in use yet
	OFFSET_SESS_DISTRIBUTION_CIPHER = 64000,
  OFFSET_DISTRIBUTION_HISTORY = 65000,
  OFFSET_FIRM_DISTRIBUTION_HISTORY = 66000,
  OFFSET_PENALTY_BOOK = 67000,
  OFFSET_PENALTY_BK_DLG_DATA = 68000,
  OFFSET_EXPENSE_CIPHER = 69000,
  OFFSET_MAIL_LIST = 70000,
  OFFSET_RCDKEY_CONTAINER_OUTPUT = 71000,
  OFFSET_SUBPOENA_LIST = 72000,
  OFFSET_SUBPOENA_CIPHER = 73000,
  OFFSET_LINK_WIND_CIPHER = 74000,
  OFFSET_SURROUND_CIPHER = 75000,
  OFFSET_RCDKEY_CONTAINER_CIPHER = 76000,
  OFFSET_KNOWN_PERSON_LIST = 77000,
  OFFSET_WHOLE_INCOMING_FLOW = 78000,
  OFFSET_BOOK_ZZDN_CIPHER = 79000,            // 20051010
  OFFSET_BOOK_HERITAGE_CIPHER = 80000         // 20060201
  ;

const long int
  VARNA_UCLP = 10135,
  PLOVDIV_UCLP = 56784
   ;
// ====================== INSTANCE dependencies =========================
// ============================== begin =================================
// ------------------- chars and integers - begin -----------------------
const char
  RESULT_SESS_WITH_MANY_END_DOCS = (char) 1,  // 08.feb.2001
# if INSTANCE
// 08.jun.2000 - begin
#   if APPEAL                  // -- APPEAL COURT ONLY
  RESULT_DECREE = 'З',  //  С разпореждане  // 22.jun.2000
  RESULT_DECREE_1 = 'ы',  // 'С разпореждане'  // 09.apr.2002
  RESULT_CIT_205 = 'a',  // С определение по чл.205, ал.1 ГПК
  RESULT_CIT_192_4 = 'A',  // С определение по чл.192, ал.4 ГПК
  RESULT_CIT_192_5 = 'о', // С определение по чл.192, ал.5 ГПК//07.feb.2001
  RESULT_CIT_37 = 'п',  // С определение по чл.37 ГПК  // 07.feb.2001
  RESULT_CIT_DECIS_194_1 = 'р',//С решение по чл.194, ал.1 ГПК//07.feb.2001
  RESULT_DEFINITION_326 = '3',  // С определение по чл.326
  RESULT_CIT_192_193 = 'м',  // С решение по чл.192 и 193  // 22.nov.2000
  RESULT_APPEAL_LEFT_NEW_EVIDENCES = 'F',//Отложено за нови доказателства
  RESULT_APPEAL_LEFT_INVALID_CALL = 'K',//Отложено поради нередовно призоваване
  RESULT_APPEAL_LEFT_107 = 'Q',  // Отложено на осн. чл.107, ал.2 ГПК
  RESULT_APPEAL_LEFT_65 = 'V',  // Отложено с прил. чл.65 ГПК
  RESULT_APPEAL_LEFT_157 = 'л', //Отложено за несп.срок по чл.157 ГПК в.л.

#   else  // of APPEAL         // -- AREAL COURT ONLY
  RESULT_DECREE = 'З',  // С разпореждане
  RESULT_DECREE_1 = 'ы',  // 'С разпореждане'  // 09.apr.2002
  RESULT_FIRM_ERASING = 'ъ',
  RESULT_FIRM_REGISTRATION = 'я',  // 'б',  // 18.feb.2000
  RESULT_BRANCH_REGISTRATION = 'г',
  RESULT_CIRCUMSTANCES_CHANGE = 'ж',
  RESULT_FIRM_MOVE = 'ш',  // Прехвърляне на предприятие  // 03.aug.2000
  RESULT_FIRM_DEFINITION_FOR_TR = 'Ё',  // С определение, подлежащо
                                        // на вписване в ТР  21.feb.2005
  RESULT_FIRM_DECREE_FOR_TR = 'ё',  // С разпореждане, подлежащо
                                        // на вписване в ТР  21.feb.2005
  RESULT_REGISTRATION_DENIAL = 'ь',
//  RESULT_CANCEL_241 = '3',//Прекр. за доразследване по чл.241
                            //08.jan.2001, 28.feb.2001 commented
  RESULT_CANCEL_241 = 'r',//Прекр. за доразследване по чл.241//28.feb.2001
//RESULT_RETURNED='r',//Върнато-за доразсл.//08.jan.2001,09.jan.2001comment
  RESULT_WITH_JUDGEMENT_BULWARK = 'Љ',  // 17.sep.2000
#   endif  // of APPEAL
// 08.jun.2000 - end
                           // -- APPEAL AND AREAL COURTS

  RESULT_WITH_DECISION_FIRM_LAW = 'F',  // 20051208
  RESULT_SEND_BY_JURIDICTION = 's',     // 20051208
  RESULT_CANCEL_BY_AGREEMENT = 'P',
  RESULT_CANCEL_EXACT_OTH_CAUSES = 'O',
  RESULT_CANCEL_FOR_MORE_LOOK_INTO = 'B',
  RESULT_DEFINITION_TERMINATING = 'd',
  RESULT_DECISION_ALLOW_SPLIT = 'a',

  RESULT_JUDGEM_VALIDITY_CONFIRMED = 'Г',  // Пoтвърждава присъдата
  RESULT_COND_CONVICTING_APPLIED = 'Д',  // Изменя присъдата-прилага условно осъждане
  RESULT_COND_CONVICTING_ABOLISHED = 'И',  // Изменя присъдата-отменя условното осъждане
  RESULT_PUNISHMENT_DECREASED = 'Й',  // Изменя присъдата-намалява наказанието
  RESULT_PUNISHMENT_INCREASED = 'Л',  // Изменя присъдата-увеличава наказанието
  RESULT_JUDG_CHG_PUN_PART = 'П',  // Изменя присъдата в наказателната част
  RESULT_JUDG_CHG_CIT_PART = 'У',  // Изменя присъдата в гражданската част
  RESULT_JUDGEM_REPEAL_PARTILALLY = 'Ф',  // Отменя присъдата отчасти-връща за ново разглеждане
  RESULT_JUDG_REPEAL_FULLY_RETURN = 'Ц',  // Отменя присъдата изцяло-връща за ново разглеждане
  RESULT_JUDG_REPEAL_FULLY_NEW_J = 'Ч',  // Отменя присъдата изцяло-произнася нова присъда

  //RESULT_CONFIRMATION = 'Ъ',  // Потвърждава; 2011:173 duplicate, comout
  RESULT_CONFIRMATION = 'v',    // Потвърждава; 2011:173 was RESULT_CONFIRMATION_1

  //RESULT_ABOLISH = 'Щ',  // Отменя; 2011:173 duplicate, comout
  RESULT_ABOLISH = 'z',    // Отменя; 2011:173 was RESULT_ABOLISH_1


#   if PUNISHM_2ND_REPORT_12_SEP_2001
#   else  // of PUNISHM_2ND_REPORT_12_SEP_2001
  RESULT_CHANGE = 'Ь',     // Изменя
#   endif  // of PUNISHM_2ND_REPORT_12_SEP_2001
  RESULT_CHANGE_1 = 'w',   // Изменя

  RESULT_GRANT = 'С',      // Уважава
  RESULT_GRANT_1 = 'q',    // Уважава

  RESULT_NOT_GRANT = 'Т',     // Без уважение
  RESULT_NOT_GRANT_1 = 'u',   // Оставя без уважение

  RESULT_DECIS_VALIDITY_CONFIRMED = 'C',  // Оставя в сила решението
  RESULT_NOT_EXAMINED = 'К',  // Оставя без разглеждане  // 12.sep.2001
  RESULT_OTHER_ATTITUDE = 'љ',  // Друго  // 12.sep.2001
  RESULT_DEC_REPEAL_PART_2_INST_D = 'E',  // Изменя отчасти решението
  RESULT_DEC_REPEAL_FULLY_RETURN = 'G',   // Отменя изцяло решението
  RESULT_DEC_REPEAL = 'М',  // Отменя решението // 15.feb.2001
  RESULT_DEC_REPEAL_RECONSIDER = 'f',  // Отменя решението и връща делото за ново разглеждане  // 18.FEB.2005
  RESULT_DEC_RPL_FULL_NEW_D_NO_MV = 'H',
  RESULT_DEC_CHANGE = 'Ґ',	// 2008:301

  RESULT_DEFINITION = 'J',

  RESULT_DEC_CHANGED_PARTIALLY = 'E',
  RESULT_DEC_CHG_FULLY_NEW_DEC = 'G',
  RESULT_DECISION_ANNULED = 'H',

  RESULT_SPLIT_PROTOCOL = '7',
  RESULT_LEFT_3_PHASE = '8',
  RESULT_LEFT_1_PHASE_DECISION = 'В',
  RESULT_DECISION_READY = 'y',
  RESULT_DEFINITION_READY = 'T',  // С готово определение
  //RESULT_DEFINITION_READY_1 = 'U',  // С готово определение; 2011:172 duplicate, comout
  RESULT_WITH_PROTOCOL = '0',
  RESULT_DECREE_TERMINATING = '4',  // С разпореждане, приключващо делото
  RESULT_ANNOUNCE_TO_BE_DECIDED = 'p'  // 04.dec.2000
   ;
# else  // of INSTANCE
                           // -- REGIONAL COURT ONLY

  RESULT_DECREE = 'З',  //  С разпореждане  // 12.jan.2001
  RESULT_DECREE_1 = 'ы',  // 'С разпореждане'  // 09.apr.2002
  RESULT_CANCEL_BY_AGREEMENT = 'P',
  RESULT_CANCEL_EXACT_OTH_CAUSES = 'O',
  RESULT_CANCEL_FOR_MORE_LOOK_INTO = 'B',
  RESULT_DEFINITION_TERMINATING = 'd',
  RESULT_DECISION_ALLOW_SPLIT = 'a',
  RESULT_PUNISHM_DECREE_CONFIRMED = 't',
  RESULT_PUNISHM_DECREE_CHANGED = 'x',
  RESULT_PUNISHM_DECREE_ABOLISHED = 'X',

  RESULT_DEFINITION = 'J',

  RESULT_SPLIT_PROTOCOL = '7',
  RESULT_LEFT_3_PHASE = '8',
  RESULT_WITH_PROTOCOL = '0',
  RESULT_ANNOUNCE_TO_BE_DECIDED = 'p',  // 26.jul.2000

  RESULT_SESS_LEFT_TEMP_MEASURES = 'E', // 20060310 'Отложено с привременни мерки'
  RESULT_SESS_DEC_TEMP_MEASURES  = 'C', // 20060310 'С определение по привременни мерки'

  RESULT_BOZA_LIKE_0 = 'V', // 'С определение по мярка за неотклонение'
				    // 'С допълнително определение' 20070208

  // 2010:196 FIX: '4' -> 'Ґ'
  RESULT_DECREE_TERMINATING = 'Ґ'  // С разпореждане, приключващо делото
# endif    // of INSTANCE
   ;

  extern const char NON_SESSION_DATED_CONT_RESULT ;  // 25.mar.2002

# if INSTANCE
#   if APPEAL
  const char
    RESOLUTION_RET_FOR_COMPLETE = 'V',  // Връща за комплектоване
    RESOLUTION_RET_NOT_ALLOWED = 'L', // Връща като недопустим/недопустима
    RESOLUTION_RET_FOR_320_322_PROC = 'G'  // Връща за процедура
                                           // по чл.320/322 НПК
      ;
#   else  // of APPEAL
  const char
    RESOLUTION_RET_FOR_COMPLETE = 'm',
    RESOLUTION_RET_NOT_ALLOWED = 'P'  // Връща като недопустим/недопустима
      ;
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE
                           // -- ALL COURTS
  const char
    RESOLUTION_ACC_ARREST = 'Б'  // Наложена мярка "задържане под стража"
      ;

// ----- KIND_ ---------------------------------------------------------------
const char
# if INSTANCE
#   if APPEAL  // 15.may.2000       // -- A P P E A L ONLY
  KIND_PUNI_GENERAL_CHAR_LAW_2A = 'W',// ВНОХД
  KIND_2ND_COMMON_PUNISHMENT_LAW = 'W',
  KIND_2ND_PRIVATE_PUNISHMENT_LAW = 'H',
  // 2015:188 TRQ/LPR: for _abcbook
  KIND_1ST_RENEW_COMMON_PUNISHMENT_LAW = '[',
  KIND_1ST_RENEW_PRIVATE_PUNISHMENT_LAW = ']',
  KIND_2ND_PUNISHMENT_PRIVCHAR_LAW = '!',
  KIND_2ND_RENEW_PRIVCHAR_LAW = '«',	// 2016:054
  //KIND_2ND_PUNISHMENT_ADMCHAR_LAW = 'П',
  KIND_2ND_PUNISHMENT_VNAHD_LAW = 'П',	// 2017:325 sync w/ non-appeal
  KIND_2ND_RENEW_ADMCHAR_LAW = '»',		// 2016:054

  KIND_PRIVATE_OBJECTION_PPL = '6',   // 22.jun.2000
  KIND_CIT_2ND_LAW = 'Z',  // ВГр.Д
  KIND_CIT_PRV_LAW = 'N',  // ЧГД
  KIND_TRA_2ND_LAW = 'T',  // ВТ.Д  // KIND_2ND_TRADE_LAW		= 'T',
  // 20070927 ->
  KIND_PRIVATE_CASS_CITIZEN_LAW = 'Я',
  KIND_PRIVATE_CASS_TRADE_LAW = 'Ѕ',
  // 20070927 <-
  KIND_COMPLAINT_R = 'l',  // Жалба  // 18.oct.2000
  KIND_PRIVATE_COMPLAINT_R = 'v',  // Частна жалба  // 18.oct.2000
  KIND_ENSURE_REQUEST_R = 'M',  // Молба за обезпечение  // 18.oct.2000
  KIND_TRADE_ENSURE_REQUEST_R = 'Ф',  // Молба за обезпечение  // 28.jun.2004
  KIND_PRV_COMPL_CIT = '0',  // Ч. жалба-гражд.дело
  KIND_PRV_COMPL_TRA = 'Ч',  // Ч. жалба-търг.дело             // 28.jun.2004
  KIND_PRV_CT274_CIT = 'o',  // Ч. жалба-ГД - чл.274, ал.2 ГПК // 2015:245
  KIND_PRV_CT274_TRA = 'p',  // Ч. жалба-ТД - чл.274, ал.2 ГПК // 2015:245 
  KIND_COMPL_SLOWLY_217 = 'j',  // Жалба за бавност-чл.217
  KIND_TRADE_COMPL_SLOWLY_217 = 'Э',  // Жалба за бавност-чл.217  // 28.jun.2004
  KIND_ENSURE_REQUEST = 'm',  // Молба за обезпечение
  KIND_TRADE_ENSURE_REQUEST = 'Ґ',  // Молба за обезпечение  // 28.jun.2004
  KIND_JUDGE_ELIMINATION = 'Г', //Дело по отстраняване на съдии-чл.14 ал.2
  KIND_TRADE_JUDGE_ELIMINATION = 'y', //Дело по отстраняване на съдии-чл.14 ал.2
                                      // 28.jun.2004
  KIND_JURISD_QUARREL = 'f',  // Препирня за подсъдност  // 28.jun.2004
  KIND_TRADE_JURISD_QUARREL = 'ґ',  // Препирня за подсъдност
	KIND_PRV_COMPL = 'v',  // Частна жалба
	KIND_PRV_RESTORE_APPEAL_TERM = '8',//Молба за възст.на срок за обж.-чл.37
	KIND_PRV_RESTORE_APPEAL_TERM_TRA = 'w',//Молба за възст.на срок за обж.-чл.37
                                         // 28.jun.2004

  KIND_PUN_GEN_CH_COMPL       = '1',  // Жалба (НОХД)
  KIND_PUN_GEN_CH_COMPL_NAHD  = '2',  // Жалба (НАХД)      // 20070129

  KIND_PUN_GEN_CH_OBJECTION = '5',  // Протест (НОХД)
  KIND_PUN_COMPL_OBJECTION = 'Q',  // Жалба и протест

  KIND_ENACTMENT_237 = KIND_CANCEL_ENACTMENT,  // 25.sep.2000

  KIND_OTHER_SESSION = 'У',

  KIND_INREG_2ND_COMPLAINT = STUB_CHAR,  // Stub! of "Въззивна жалба"
  KIND_INREG_OBJECTION = STUB_CHAR,  // Sub! of "Протест"
	KIND_INREG_CONCERN_COMPLAINT = 'д', // Stub! of "Касационна жалба"

  KIND_CONCERN_COMPLAINT_PUN_INDOC_NAHD = '9', // Частна жалба НАХД  // 20070129
  KIND_TRA_PRV_LAW = 'n'

#   else  // 15.may.2000            // --  A R E A L  ONLY
  KIND_PUNISHMENT_GENERAL_CHAR_LAW = 'O',  // НОХД
  KIND_PUNISHMENT_PRIVATE_CHAR_LAW = 'R',  // НЧХД
  KIND_PUNISHMENT_NAHD_LAW = 'D',          // НАХД    20050315
  KIND_2ND_PUNISHMENT_VNCHHD_LAW = 'C',    // ВНЧХД   20050315
  KIND_2ND_PUNISHMENT_KNAHD_LAW  = 'U',    // КНАХД   20050315
  KIND_2ND_PRIVATE_PUNISHMENT_LAW  = 'H',
  KIND_2ND_PUNISHMENT_VNAHD_LAW = 'П',     // ВНАХД   20060717

  KIND_2ND_PR_PUNISH_COMBINE_LAW = 'p',    // Комбин. 20050315


//  KIND_RECONCILIATION_SESSION = 'П',  // 18.dec.2003
  KIND_OTHER_SESSION = 'У',
  KIND_FIRST_SESSION = 'Ъ',
  KIND_SECOND_SESSION = 'Д',

  KIND_ENACTMENT_237 = 'К',  // 01.aug.2000
  KIND_DEMAND = 'ь',  // 04.aug.2000
  KIND_PRIVATE_COMPLAINT_CIT_INDOC = 'Ч'//,
//#   if APPEAL  // 15.may.2000
//#   else  // 15.may.2000
                                        ,
  KIND_CONCERN_COMPLAINT_ADM_INDOC = 'б',
  KIND_CONCERN_COMPLAINT_PUN_INDOC = 'ц', // Частна жалба НОХД
  KIND_PRV_CONCERN_COMPL_PUN_INDOC = 'g', // Частна касационна жалба (НАХД)

  KIND_INREG_2ND_COMPLAINT = 'Х',  // Въззивна жалба
  KIND_INREG_OBJECTION = 'а',  // Протест
  KIND_INREG_CONCERN_COMPLAINT = 'д',  // Касационна жалба

  KIND_ADMIN_DEMAND_12 = 'ї',  //  Искане по чл.12 ДПК

	KIND_SUPREME_RENEW = 'є', // 25.jul.2001

# 	if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
		KIND_2ND_COMMON_PUNISHMENT_LAW= 'W', // !!ADM: stub
#	endif

	KIND_OUTREG_LETTER = 'О'   //  Писмо
#   endif  // 15.may.2000

//  KIND_DEFINITION = 'л'
# else                              // -- R E G I O N A L   ONLY
  KIND_PUNISHMENT_GENERAL_CHAR_LAW = 'O',   // НОХД
  KIND_PUNISHMENT_PRIVATE_CHAR_LAW = 'R',   // НЧХД

  //KIND_PRIVATE_PUNISHMENT_LAW = 'I',  // 29.dec.2004 -- declared in const.h

  KIND_RECONCILIATION_SESSION = '4',
  KIND_OTHER_SESSION = '9',
  KIND_FIRST_SESSION = '5',
  KIND_SECOND_SESSION = '6',

  KIND_DEMAND = 'X',  // 04.aug.2000
// 12.jan.2000 - begin
  KIND_LISTENING_SESSION = 'L',
// 12.jan.2000 - end

  KIND_INTERIM_SESSION = 'Ј',  // 20060627  Заседание по привременни мерки

//  KIND_DEFINITION = 'S' ;
  KIND_INREG_2ND_COMPLAINT = 'Х',  // Въззивна жалба
  KIND_INREG_OBJECTION = 'а',  // Протест
	KIND_INREG_CONCERN_COMPLAINT = 'д',  // Касационна жалба

	KIND_OUTREG_LETTER = 'О'   //  Писмо

# endif  // of INSTANCE
   ;

const char
# if INSTANCE                       // -- APPEAL and AREAL
  RECIDIVE_GENERAL = 'c' ,
  RECIDIVE_SPECIAL = 's' ,
  RECIDIVE_DANGEROUS = 'd' ;
# else                              // -- REGIONAL
  RECIDIVE_GENERAL = 'c' ,
  RECIDIVE_SPECIAL = 's' ,
  RECIDIVE_DANGEROUS = 'd' ;
# endif

const char
  INVOLVEMENT_LEGAL_SPOKESMAN = 'B',    // 24.nov.1999
  INVOLVEMENT_GUARDIAN = 'U',           // 21.jul.2000
  INVOLVEMENT_TRUSTEE = 'Y',            // 21.jul.2000
  INVOLVEMENT_LAWYER = 'L',             // 19.jan.2000
  INVOLVEMENT_AUTHORIZED = 'H',         // 19.jan.2000
  INVOLVEMENT_SPOKESMAN = 'J',          // 19.jan.2000
  INVOLVEMENT_VIOLATOR = 'k',           // 01.mar.2001  // Нарушител
  INVOLVEMENT_ADMIN_PUN_AGENCY = 'p',   // 01.mar.2001 //Админ.наказ.орган
  INVOLVEMENT_VICTIM_254 = 'O',         // 22.apr.2002  // Пострадал
  INVOLVEMENT_VICTIM_FOR_SUMMONS = 'А'; // 22.apr.2002  // Пострадал

# if INSTANCE                       // -- APPEAL and AREAL
const char
  INVOLVEMENT_FIRM_OWNER = 'w',         // 18.nov.1999
  INVOLVEMENT_COMPLAINTLODGER = 'a',    // 02.mar.2001  // Жалбоподател
  INVOLVEMENT_DEBTOR = 'y';             // 07.jan.2001  // Длъжник

#    if APPEAL                      // -- APPEAL ONLY
const char
  INVOLVEMENT_COMPLAINT_CLAIMANT = 'п',  //Жалбоподател-гр.ищец     // 20050831
  INVOLVEMENT_COMPLAINT_ACCUSER = 'ф',   //Жалбоподател-ч.обвинител  // 20050831
  INVOLVEMENT_COMPLAINT_DEFENDANT = 'ц', //Жалбоподател-ответник   // 20050831
  INVOLVEMENT_COMPLAINT_ACCUSED = 'л';    //Жалбоподател-подсъдим     // 20050831
#    endif
# else                              // -- REGIONAL
const char
  INVOLVEMENT_COMPLAINTLODGER = 'a';    // 02.mar.2001  // Жалбоподател
# endif

# if INSTANCE
const char COMPANY_TYPE_SHOPKEEPER = 'e' ;
# endif

const char GRANT_NO_EFFECT = 'z' ;  // Обявява за нищожно
extern constant
  GRANT_REJECTEDS ;  // 20.apr.2001

// -------------------- chars and integers - end ------------------------
// ------------------------- strings - begin ----------------------------
extern constant AppNameMinistry ;  // 04.may.2000
extern constant AppNameMinistryShort ;  // 05.may.2000
extern constant AppNameTheMinistry ;  // 16.jun.2000
extern constant KEEPING_METHOD_PRT_STRING ;  // 26.may.2000

extern const char
  * const RESULT_UNMOVEABLES,
  * const RESULT_STOPPEDS,
  * const RESULT_CANCELEDS,
  * const RESULT_WITH_DECISIONS,
  * const RESULT_RETURNEDS,
  * const RESULT_CANCELEDS_OR_RETURNEDS,
//  * const RESULT_FOR_TIME_BOOK_S,
  * const RESULT_M_COL_FINISHEDS,
  * const RESULT_M_COL_NOT_FINISHEDS,
  * const RESULT_LEFT_RECONCILES,
//  * const RESULT_LEFT_1ST_SESS,
//  * const RESULT_LEFT_2ST_SESS,
  * const RESULT_WITH_DECS_OR_CANCELEDS,
  * const RESULT_TO_UP_LOOK_INTO_RETURNEDS,     // 16.jun.2000

# if INSTANCE                               // APPEAL and AREAL
  * const RESULT_DEFIN_2ND_TERMIN_CLSESBK,
  * const RESULT_CONFIRMATIONS,                 // 26.oct.2000
  * const RESULT_CHANGINGS,                     // 26.oct.2000
  * const RESULT_REJECTS,                       // 26.oct.2000
  * const RESULT_SLOWDOWN_CONFIRMATIONS,        // 26.oct.2000
  * const RESULT_SLOWDOWN_REJECTS,              // 26.oct.2000

#   if APPEAL                               // APPEAL only
  * const RESULT_SLOWDOWN_REJECT_NOT_GR,        // 20060417
  * const RESULT_SLOWDOWN_REJECT_CEASED,        // 20060417

#   else  // of APPEAL                      // AREAL only
  * const RESULT_FIRM_CANCELEDS,
  * const RESULT_FIRM_DECISION_CHANGES,         // 03.aug.2000
  * const RESULT_FIRM_REFUSED_CHANGES,          // 03.apr.2001
  * const RESULT_FIRM_REGISTRATIONS,            // 12.oct.2000
  * const RESULT_FIRM_LEGAL_FINALIZEDS,         // 04.jan.2001
  * const RESULT_FIRM_LEGAL_CANCELEDS,          // 24.jan.2001
  * const RESULT_FIRM_CL_S_BK_EXCLUDEDS,        // 22.jan.2001
  * const RESULT_FIRM_CLSBK_MUST_INCLUDE,       // 22.jan.2001

  * const RESULT_LEFTS_1ST_SESS,                // 24.sep.2001
  * const RESULT_FIRM_FINISHEDS,
#   endif  // of APPEAL

# else  // of INSTANCE                      // REGIONAL only
  * const RESULT_DEFIN_TERMIN_CLSESBK,
  * const RESULT_KEEPING_METHOD_CONFIRMEDS,
  * const RESULT_KEEPING_METHOD_CHANGEDS,
  * const RESULT_PENALTIES,                     // 25.may.2001
  * const RESULT_SPLITS,                        // 25.may.2001

  * const RESULT_LEFT_REPORT_DESIGNATION,       // 20060627
  * const RESULT_LEFT_1_REPORT_DESIGNATION,     // 20060627
  * const RESULT_DECIDED_REPORT_DESIGNATION,    // 20060627
  * const RESULT_ESS_CS_REPORT_DESIGNATION,     // 20060627
  * const KIND_PRIVATE_COMPL_REPORT_DESIGNATION,// 20060627
# endif  // of INSTANCE


  * const RESULT_DECISIONS_CLSESBK,
  * const RESULT_DEFINITIONS_CLSESBK,
  * const RESULT_IRE_UNMOVEABLES,  // 27.apr.2001]
  * const RESULT_IRE_SENDS,  // 03.may.2001
  * const RESULT_IRE_RETURNEDS  // 03.may.2001
   ;

extern const char
  * const SUMMONS_FOR_SIDE_LEGAL_SPOK_INVS,  // 21.jul.2000
  * const SUMMONS_FOR_SIDE_INDIRECT_INVS,   // 24.nov.1999
  * const SUMMONS_FOR_SIDE_WITH_INVS;	// 2014:206

extern const char
  * const PERSLST_BONEVA_0_INVS  // 22.jan.2001
    ;

extern const char
  * const INVOLVEMENT_COMPLAINTLODGERS  // 22.jan.2001
    ;

extern constant INVOLV_TIME_BK_SUIT_TO_DEFENDS ;
# if INSTANCE
extern constant INVOLV_2ND_T_BK_SUIT_TO_DEFENDS ;
# else
extern constant INVOLV_DEFENDS_TO_EXCLUDE_P_ADM ;
# endif

extern const char
# if INSTANCE                         // APPEAL  and  AREAL
//  * const KIND_COMPLAINTS,  // 24.oct.2000 removed
//  * const KIND_OBJECTIONS,  // 24.oct.2000 removed
  * const KIND_EXACT_COMPLAINTS,
  * const KIND_EXACT_OBJECTIONS,
  * const KIND_PRIVATE_COMPLAINTS,
  * const KIND_PRIVATE_OBJECTIONS,
#   if APPEAL                         // APPEAL ONLY
  * const KIND_PRIVATE_PUN_C_O_INDOCS,
#   endif  // of APPEAL
  * const KIND_DEMAND_INDOCS,
  * const KIND_SLOWDOWN_COMPLAINTS,   // 24.oct.2000
#   if APPEAL                         // APPEAL ONLY
  * const KIND_PETITIONS,             // 11.jun.2002
#   else  // of APPEAL                // AREAL ONLY
  * const KIND_EXACT_COMPLAINTS_CONCERN,        // 16.jan.2001
  * const KIND_PRIVATE_COMPLAINTS_CONCERN,      // 16.jan.2001
  * const KIND_TRADE_BOZA_INDOCS,               // 08.may.2002
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE                     // ALL COURTS

//  * const KIND_FAST_PROCEDURE_INDOCS,     // 24.sep.2003
  * const KIND_ACCUSE_ACTS,                 // 28.feb.2000

  * const KIND_KEEP_METHOD_CHG_COMPLAINTS,  // 01.dec.1999
  * const KIND_PSEUDO_SESSIONS,
  * const KIND_PSEUDO_OR_CLOSED_SESSIONS,

  * const KIND_JUDGEMENTS_BY_NAME,
  * const KIND_DECISIONS_BY_NAME,
  * const KIND_DEFINITIONS_BY_NAME,
# if INSTANCE
  * const KIND_DECREES_BY_NAME,
# else  // of INSTANCE
# endif  // of INSTANCE
  * const KIND_PUN_GENERAL_LAWS,  // 08.apr.2002
  * const KIND_PUN_PRIVATE_LAWS,  // 08.apr.2002

  * const KIND_TEMP_MEASURES_SESSIONS,       // 20060310
  * const KIND_SESS_COMPLE_REPORT_DESIGNATION,  // 20060627

  * const KIND_INREG_EXACT_COMPLAINTS,
  * const KIND_INREG_PRIVATE_COMPLAINTS,
  * const KIND_INREG_EXACT_OBJECTIONS,
  * const KIND_INREG_PRIVATE_OBJECTIONS,
  * const KIND_INREG_REJ_RETR_COMPLAINTS
   ;

extern const char
  * const JUDGED_JUDGEDS,
  * const JUDGED_CONVICTEDS,
  * const JUDGED_CONVICTED_BULWARKS,  // 13.jun.2000
  * const JUDGED_ACQUITS,
  * const JUDGED_FREEDS,  // 26.jul.2001
  * const JUDGED_CANCELEDS ;

extern const char
  * const PUNISHMENT_UP_TO_3_YEARS_S,
  * const PUNISHMENT_UP_TO_3_YEARS_CONDS,
  * const PUNISHMENT_UP_TO_3_YEARS_EFFS,
  * const PUNISHMENT_OTHERS_S,
//  * const PUNISHMENT_COMPLEMENTS_S,  // 16.may.2000
# if INSTANCE
#   if APPEAL
#   else  // COURT_TYPE == COURT_AREAL  ||  COURT_TYPE == COURT_AREAL
  * const PUNISHMENT_OTHERS_S_CORR,   // 20070612  - Корупционен отчет
#   endif  // of APPEAL
  * const PUNISHMENT_3_TO_10_YEARS_S,
  * const PUNISHMENT_10_TO_30_YEARS_S,
  * const PUNISHMENT_LIFE_IMPRISONMENTS,
  * const PUNISHMENT_LIFE_IMPRISONMENT_NCS,
  * const PUNISHMENT_FINE_ARL  // 20070611
# else
  * const PUNISHMENT_UP_TO_1_YEAR_S,  // 10.jan.2001
  * const PUNISHMENT_UP_TO_1_YEAR_EFFS,  // 01.mar.2001
  * const PUNISHMENT_3_YEARS_EFFS,  // 01.mar.2001
  * const PUNISHMENT_OTHER_CONDIT_S,  // 20.apr.2001
  * const PUNISHMENT_3_TO_15_YEARS_S,
  * const PUNISHMENT_PENALTIES,
  * const PUNISHMENT_WORK_EDUCATE_SCHOOL_S
# endif
    ;

extern constant TYPE_TRADE_OR_CITIZEN_LAWS ;  // 14.nov.2000

extern constant CLOSED_SESS_BK_COLLEGES ;  // Not used,
                         // definition is "commented"

# if INSTANCE
# else
extern constant PUNISHM_ADMIN_REPORT_FILE_NAME ;
# endif

extern const char
  * const DEFAULT_FORMAT_FILE_EXTENSION ;

extern const char
  * const AGE_NONES,

  * const AGE_UP_TO_ONES,  // 10.feb.2005
  * const AGE_UP_TO_THREES,
  * const AGE_UP_TO_SIXES,  // 10.feb.2005
  * const AGE_UP_TO_TWELVES,  // 10.feb.2005

  * const AGE_MORE_THAN_THREES,
  * const AGE_MORE_THAN_THREES_ALL, //20050318
  * const AGE_MORE_THAN_TWELVES   // 10.feb.2005
   ;

# if INSTANCE
# else
extern constant PS2_KIND_INVESTIGATIONS ;
# endif  // 11.jan.2000 - end

// -------------------------- strings - end -----------------------------
// ====================== INSTANCE dependencies =========================
// =============================== end ==================================

extern constant DEFAULT_BRIEF_SESSION_FIELDS ;

const char LINK_PARENT = 'R' ;  // 20051121

// 2008:059 LPR: == PENALITY_TYPE_BOOK (иначе ми хареса :-)
//const PENALITY_BOZA	  = 0x02 ;  // 20051214

# if ! INSTANCE
const char KIND_REQUEST_ZZDN = 'w';  // Mолба (искане) по ЗЗДН
# endif

// 20060210 prove actions
const char PROVE_RESOLUTION_SEND = 'Й';        // 'Изпращане'
const char PROVE_RESOLUTION_RETURN  = 'И';     // 'Връщане'
const char PROVE_RESOLUTION_RECEIVE = 'й';     // 'Получаване'
const char PROVE_RESOLUTION_DESTROY = 'и';     // 'Унищожаване'

# if INSTANCE
const char RESULT_SESS_FIRM_JURIDICTION = 'a'; // Изпратено по подсъдност след регистрация  // 20060322
# endif

extern constant TB_SUPPRESS_SESS_RESULTS ;  // 20060331

// 20070807
#if COURT_TYPE == COURT_ADMIN
const char KIND_LOWER_LAWS_PUNISHMENT	= 'п';
#endif	// ADMIN

const char OLD_NOT_MAJOR = 'y' ;

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
const char KIND_PRIVATE_ADMIN_LAW = '?';
#endif  // ADMIN || MILITARY

#if COURT_TYPE == COURT_COURT_AREAL
const char KIND_PRIVATE_ADMIN_LAW = 'Н';
#endif  // AREAL

#if COURT_TYPE == COURT_REGIONAL
const char KIND_PRIVATE_ADMIN_LAW = 'r';
#endif  // REGIONAL

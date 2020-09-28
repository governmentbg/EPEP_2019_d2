// ----- PRINT_ --------------------------------------------------------------
extern long WINDOWS_PRINT;	// 2009:267
extern long WINDOWS_A3;		// 2010:111
// 2011:326 public
extern long PRINT_PORT;
extern long PRINT_CP1251;
extern long PRINT_CHECK;
extern long PRINT_PSEUDO;
extern long PRINT_SETUPEX;
extern long PRINT_JUSTIFY;
extern long PRINT_LINE_WIDTH;
extern long PRINT_BARCODES;
extern long BARCODE_ASTERISK;
extern char BARCODE_FONT[SIZE_OF_FONT_NAME];
extern long BARCODE_FONT_SIZE;
extern char BARDIGIT_FONT[SIZE_OF_FONT_NAME];
extern long BARDIGIT_FONT_SIZE;
extern char SERVICE_PHONE[SIZE_OF_PHONE];
extern char SERVICE_FAX[SIZE_OF_FAX];
extern long PRINT_JUDGE_TYPE;
extern char PRINT_COURT_ADDRESS_1[SIZE_OF_PRINT_ADDRESS_LINE];
extern char PRINT_COURT_ADDRESS_2[SIZE_OF_PRINT_ADDRESS_LINE];
extern long PRINT_COURT_UPPERCASE;

// ----- ADDRESS -------------------------------------------------------------
extern long LOCK_UCLP;
extern long LOCK_STREET;

// ----- SERIAL --------------------------------------------------------------
extern int
	*_CONFIG_MAX,
	*_COURT_UCLP,
	*_CourtType,
	*_COURT_AREA,
	*_COURT_CODE,
	*_COURT_FLAT,	// 2008:254 +related: flat compositions
	*_COURT_WRIT,	// 2010:047 deliverer functions
	*_COURT_N10K,	// 2010:047 separated from FLAT
	*_COURT_MINI,	// 2010:117 mini-terminal mode
	*_COURT_LOCK,	// 2010:267 lock some changes
	*_COURT_SIME,	// 2015:155 subpoena import/export
	*_COURT_CSVE,	// 2015:222 admin csv export
	*_COURT_EXAR;	// 2017:159 external court archiving

extern long COURT_EISPP;
extern char COURT_TLACR[SIZE_OF_ALIAS];
long _DEFAULT_UCLP();
#define DEFAULT_UCLP _DEFAULT_UCLP()

#define CONFIG_MAX (*_CONFIG_MAX)
#define COURT_UCLP (*_COURT_UCLP)
#define CourtType (*_CourtType)
#define COURT_AREA (*_COURT_AREA)
#define COURT_CODE (*_COURT_CODE)
#define COURT_FLAT (*_COURT_FLAT)
#define COURT_WRIT (*_COURT_WRIT)
#define COURT_N10K (*_COURT_N10K)
#define COURT_MINI (*_COURT_MINI)
#define COURT_LOCK (*_COURT_LOCK)
#define COURT_SIME (*_COURT_SIME)
#define COURT_CSVE (*_COURT_CSVE)
#define COURT_EXAR (*_COURT_EXAR)

extern char SERIAL_NO[SIZE_OF_SERIAL_NO];
extern long LOGIN_LIMIT;
extern long LOG_QUERIES;
extern CDate FULL_KIND_DATE;
extern long EXEC_LIST_NUMBERING;
extern long EXEC_LIST_PRIV_SUMS;
extern long LEGAL_REQUEST_ID;
extern char DOMAIN_NAME[SIZE_OF_PATH];
extern long NIA_PROT_NUMBERING;
extern long NIA_PROT_OUT_LETTER;
#if COURT_TYPE == COURT_ADMIN
extern char EXPORT_CSV_PATH[SIZE_OF_DIR];
#endif  // ADMIN
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
#define DEFAULT_LAW_AGE "1"
#endif  // APPEAL || AREAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
#define DEFAULT_LAW_AGE "0"
#endif  // ADMIN || MILITARY || REGIONAL
extern long COMPUTE_LAW_AGE;
extern long DEPARTMENT;
#if TESTVER
extern CDate XFER_BANK_DATE;
#endif  // TESTVER

bool snd_check(const char *s);
extern int SERIAL_ERROR;
inline bool _SERIAL_VALID() { return !SERIAL_ERROR; }
#define SERIAL_VALID _SERIAL_VALID()

// ----- REPEAT --------------------------------------------------------------
extern long REPEAT_REQUESTS;
extern long REPEAT_LAWSUITS;
extern long REPEAT_FIXES;
extern long REPEAT_RESULTS;
extern long REPEAT_MDRS;
extern long REPEAT_MOVEMENTS;
extern long REPEAT_SURROUNDS;
extern long REPEAT_LAWSUITS1;
extern long REPEAT_SURROUNDS1;
extern long REPEAT_P24A1S;
extern long REPEAT_MESSAGES;
extern long REPEAT_ARCHIVES;		// 2006:019
extern long REPEAT_ANNOUNCES;		// 2009:356
extern long REPEAT_PAYMENTS;		// 2013:099
extern long REPEAT_DESTROYS;		// 2017:097
extern long REPEAT_BARCODES;		// 2017:321 JRQ/URQ
extern long REPEAT_BOOKS_A;		// 2018-05-28
extern long REPEAT_BOOKS_B;		// 2018-05-29

// ----- AUTORUN -------------------------------------------------------------
extern long AUTORUN_BASICS;
extern long AUTORUN_FIXES;
extern long AUTORUN_WALNS;
extern long AUTORUN_MDRS;
extern long AUTORUN_ENDOCS;
extern long AUTORUN_MSRS;

// ----- EXTERNAL ------------------------------------------------------------
extern long SAVE_SUBPOENAS;
extern long WORD_OPEN_7A;
extern char EXTERNAL_SERVER[SIZE_OF_PATH];
extern long WAIT_FOR_ATTACH;
extern char SIGN_TEST[SIZE_OF_PATH];
extern long SIGN_PDFS;
extern long HIDE_ALL_ON_SAVE;

// ----- HYPER ---------------------------------------------------------------
extern long WEB_SCHEME;
extern long WEB_ATTACH;
extern long HTML_OPEN_DELTA;	// 2012:052
extern char HTML_OVER_PATH[SIZE_OF_DIR];
// 2016:321
extern long HTML_MAPS_FORMAT;
extern long HTML_MAPS_ZOOM;
extern char HTML_LOGO_NAME[SIZE_OF_HTEMPLATE_NAME];
extern long HTML_LOGO_WIDTH;
extern long HTML_LOGO_HEIGHT;
extern long HTML_LOGO_SCALE;

// ----- PRINT ---------------------------------------------------------------
// 2010:151 LPR: DEFLOAT_ -> DEFAULT_
extern long DEFAULT_PAGE_LEN;			// 2010:151 LPR: real value (60)
extern long DEFAULT_ALIGNMENT;

// ----- APPLICATION ---------------------------------------------------------
extern long PRINT_COMPOSITIONS;
extern long PRINT_JUDGEMENTS;
extern long PROPAGANDATE_JUDGE;
extern long SHOWUP_IMPORTANT;
extern long SHOWUP_REGISTER_ACT;
extern long AREA_SPLIT_DESC_BOOK;
extern long WRITINGS_ACTIVE;
extern long PRINT_UCN;				// 2009:069
extern long LOCK_RETURN;			// 2006:018
extern long KILL_FORWARD_SIDES;		// 2006:039 BLOAT
extern long PRINT_COLLEGE;
extern long RKO_ADD_DATA;
extern long DEC_REG_NUM_ALL;			// 2008:113 URQ: plovdiv
extern char OUT_PATH[SIZE_OF_DIR];		// 2009:138 former TEXT_PATH
extern long PING_DATABASE;
extern CDate WEB_FIXED_START_DATE;
extern CDate WEB_FIXED_FINAL_DATE;
extern CDate WEB_QUERY_START_DATE;
extern CDate WEB_QUERY_FINAL_DATE;
extern long WEB_QUERY_USE_STYLES;
extern long PRINT_EXEC_LIST_NO;
extern long PRINT_EXEC_LIST_SUMS;
extern long LOCK_RECEIVE;
extern long EXTERNAL_LAWSUIT_NO;		// 2015:278 +related
extern long USER_INTERSECT;			// 2015:300 +related
// 2010:111 LPR: from _
extern long CENTER_PROGRESS_BAR;
extern long PRINT_PAGE_BREAK;
extern long ADMIN_SUMMONS_GPK;
extern long SIGN_SUMMONS;
extern long ALT_PATCH_COLUMNS;
extern long LEFT_MEETING;
extern long LOCK_REGISTER;			// 2016:133
#if COURT_TYPE == COURT_MILITARY
extern long REPORT_NEW_LAWSUITS;
#endif  // MILITARY
extern long LINK_NOTIFICATION;		// 2018-03-27 +related

// ----- SCHED ---------------------------------------------------------------
extern long SCHED_ACTIVE;
extern long SCHED_SHOWUP;
extern long SCHED_DELTA_X;
extern long SCHED_DELTA_Y;
extern long SCHED_DELTA_Z;

// ----- SMS -----------------------------------------------------------------
extern long SMS_ACTIVE;
extern char SMS_PATH[SIZE_OF_DIR];

// ----- CHROME --------------------------------------------------------------
extern long CHROME_TEXT_MAXIMIZED;
extern long CHROME_HTML_MAXIMIZED;
extern long CHROME_FONT_SCALE;
extern long CHROME_PUSH_IMAGE;
extern long CHROME_AUTO_COMPLETE;
extern long CHROME_KEYBOARD_LAYOUT;
extern long CHROME_AUTO_REMEMBER;
extern long CHROME_EFILE_MENUS;
extern long CHROME_THEME;
extern long CHROME_LOAD_KNOWNS;
extern long CHROME_WAIT_FOR_IDLE;
//extern long CHROME_BARCODE_ENTER;  // 2017:097 unused, comout +related
extern long CHROME_LUCIDACONSOLE;
extern long CHROME_PERCENT_SCALE;	// 2017:202 LPR
extern long CHROME_UNICODE_CONVERT;	// 2017:299 LPR

enum
{
	SIDE_SEARCH_ALPHA	= 0,
	SIDE_SEARCH_1NAME	= 1,
	SIDE_SEARCH_NAMES	= 2,
	SIDE_SEARCH_QUICK	= 3
};

extern long CHROME_SIDE_SEARCH;
extern long CHROME_AUTO_SELECT;		// 2014:003
extern long CHROME_CENTER_OPEN_SAVE;	// 2014:026

// ----- DISTRIB -------------------------------------------------------------
extern long DISTRIB_COUNT_REJUDGE;
extern long DISTRIB_SESSION_LENGTH;
extern long DISTRIB_SESSION_PAUSE;
extern long DISTRIB_SESSION_TIMES;	// 2012:248
extern long DISTRIB_SESSION_TIMES_FORCE;
extern long DISTRIB_RESERVE_JURORS;	// 2012:269
extern long DISTRIB_JURORS_LOAD;	// 2013:210
// 2016:280
extern long DISTRIB_SESSION_7302;
extern long DISTRIB_SESSION_MIN20L;
extern long DISTRIB_SESSION_MAX8H;

inline bool can_session_times() { return DISTRIB_SESSION_TIMES && !COURT_LOCK; }

// ----- UPDATE --------------------------------------------------------------
extern long UPDATE_ACTIVE;				// 2007:198 explicit disabling
extern char UPDATE_DIRECTORY[SIZE_OF_DIR];	// 2007:155
extern long UPDATE_WAIT;				// 2007:205 process syncing
extern long UPDATE_VERBOSE;				// 2008:078 diagnostic messages
extern long UPDATE_POWERSHELL;			// 2013:260
extern long UPDATE_LOCAL_COPY;			// 2014:041

// ----- FILTER --------------------------------------------------------------
extern long FILTER_DECISIONS;
extern long FILTER_SESSIONS;
extern long FILTER_OTHERS;
extern char *const FILTER_PATH;	// == Electrics[TYPE_HTML].docketPath
extern long FILTER_TYPE;
extern long FILTER_NUMBER;
extern long FILTER_OLE_CASE;
extern long FILTER_JUDICIAL;
extern long FILTER_HIDE_SPELL;
extern long FILTER_FIND_UCLP;
extern long FILTER_FIND_STREET;
extern long FILTER_RUN_MACRO;

// ----- XFER ----------------------------------------------------------------
extern long XFER_ACTIVE;
extern char XFER_PATH[SIZE_OF_DIR];
extern int XFER_LEN;			// == strlen(XFER_PATH)
extern char XFER_CCEXP[SIZE_OF_PATH];
extern long XFER_EMAIL;
extern char XFER_BANKRUPT_PATH[SIZE_OF_DIR];
extern long XFER_LEGAL_AID;
extern char XFER_LEGAL_AID_PATH[SIZE_OF_DIR];
extern long XFER_EPORTAL;
extern long XFER_LINK_APPEALS;

// ----- EISPP ---------------------------------------------------------------
extern long EISPP_ACTIVE;
extern char EISPP_OUTPATH[SIZE_OF_DIR];
extern char EISPP_VZLPLC[SIZE_OF_EISPP_STRING];
extern char EISPP_VZLIZP[SIZE_OF_EISPP_STRING];
extern long EISPP_STRUCTURA_PLC;
extern long EISPP_STRUCTURA_IZP;
extern char EISPP_KST_USRSID[SIZE_OF_EISPP_STRING];
extern char EISPP_KST_USRREJ[SIZE_OF_EISPP_STRING];
extern char EISPP_KST_USRRAB[SIZE_OF_EISPP_STRING];
extern char EISPP_KST_ARMSID[SIZE_OF_EISPP_STRING];
extern char *const EISPP_XMLPATH;	// == Electrics[TYPE_XML].docketPath
// 2012:333
extern char EISPP_KST_OKASID[SIZE_OF_EISPP_STRING];
extern char EISPP_KST_SBEVID[SIZE_OF_EISPP_STRING];	// if !sbe->vid
extern char EISPP_KST_ARMSTRSID[SIZE_OF_EISPP_STRING];
extern char EISPP_KST_SESSID[SIZE_OF_EISPP_STRING];
extern char EISPP_KST_LGTSID[SIZE_OF_EISPP_STRING];
extern long EISPP_QUERY_TIMEOUT;	// 2013:154
extern char EISPP_EIQ_USRSID[SIZE_OF_EISPP_STRING];
extern char EISPP_EIQ_USRREJ[SIZE_OF_EISPP_STRING];
extern long EISPP_DIAG_NAMES;

// ----- POSTERM -------------------------------------------------------------
extern long POSTERM_ACTIVE[POSTERM_COUNT];
extern long POSTERM_ECR;
extern char POSTERM_PORT_NAME[POSTERM_COUNT][SIZE_OF_PORT_NAME];
extern long POSTERM_OPERATOR;
extern long POSTERM_CURRENCY;
extern long POSTERM_REINITIALIZE;

// ----- functions -----------------------------------------------------------
void initialize_config();
void shutdown_config();

class TPrinterGroup : public TDummyGroup
{
public:
	TPrinterGroup();

	long port;
	long cp1251;
	long check;
	long pseudo;
	long windows;
	long setupex;
	long justify;
	long pagelen;
	long alignto;
	long linewidth;
	long windowsA3;			// 2007:081

	void Load();
	void Save();
};

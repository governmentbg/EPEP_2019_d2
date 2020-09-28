#include "cac.h"
#include <lmcons.h>

// ----- PRINT ---------------------------------------------------------------
long WINDOWS_PRINT = 1;
long WINDOWS_A3 = 0;
// 2010:112
long PRINT_PORT = 0;
long PRINT_CP1251 = 0;
long PRINT_CHECK = 0;
long PRINT_PSEUDO = 0;
long PRINT_SETUPEX = 0;
long PRINT_JUSTIFY = 240;
long PRINT_LINE_WIDTH = 0;
long PRINT_BARCODES = 0;
long BARCODE_ASTERISK = 0;
char BARCODE_FONT[SIZE_OF_FONT_NAME];
long BARCODE_FONT_SIZE;
char BARDIGIT_FONT[SIZE_OF_FONT_NAME];
long BARDIGIT_FONT_SIZE;
char SERVICE_PHONE[SIZE_OF_PHONE];
char SERVICE_FAX[SIZE_OF_FAX];
long PRINT_JUDGE_TYPE;
char PRINT_COURT_ADDRESS_1[SIZE_OF_PRINT_ADDRESS_LINE];
char PRINT_COURT_ADDRESS_2[SIZE_OF_PRINT_ADDRESS_LINE];
long PRINT_COURT_UPPERCASE = 0;

// ----- ADDRESS -------------------------------------------------------------
long LOCK_UCLP = 0;
long LOCK_STREET = 1;

// ----- SERIAL --------------------------------------------------------------
int
	CONFIG_MAX	= NULL,
	COURT_UCLP	= NULL,
	CourtType	= NULL,
	COURT_AREA	= NULL,
	COURT_CODE	= NULL,
	COURT_FLAT	= NULL,
	COURT_WRIT	= NULL,
	COURT_N10K	= NULL,
	COURT_MINI	= NULL,
	COURT_LOCK	= NULL,
	COURT_SIME	= NULL,
	COURT_CSVE	= NULL,
	COURT_EXAR	= NULL;

long COURT_EISPP;
char COURT_TLACR[SIZE_OF_ALIAS];

long _DEFAULT_UCLP()
{
	return _COURT_UCLP ? COURT_UCLP : BUILDIN_UCLP;
}

#include "Aes.c"
#include "sn_cls.cpp"
static const sn_cls::VK vk =
	{ 0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

char SERIAL_NO[SIZE_OF_SERIAL_NO];
int SERIAL_ERROR = 0;
long LOGIN_LIMIT = 0;		// 2013:064
long LOG_QUERIES = 0;
CDate FULL_KIND_DATE;
long EXEC_LIST_NUMBERING = 0;
long EXEC_LIST_PRIV_SUMS = 1;
long LEGAL_REQUEST_ID = 0;
char DOMAIN_NAME[SIZE_OF_PATH];
long NIA_PROT_NUMBERING = 0;
long NIA_PROT_OUT_LETTER = 0;
char EXPORT_CSV_PATH[SIZE_OF_DIR];
long COMPUTE_LAW_AGE = 0;
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
long DEPARTMENT = 1;
#endif // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
long DEPARTMENT = 0;
#endif  // REGIONAL
#if TESTVER
CDate XFER_BANK_DATE;
#endif  // TESTVER

// ----- REPEAT --------------------------------------------------------------
long REPEAT_REQUESTS = 0;
long REPEAT_LAWSUITS = 0;
long REPEAT_FIXES = 0;
long REPEAT_RESULTS = 0;
long REPEAT_MDRS = 0;
long REPEAT_MOVEMENTS = 1;
long REPEAT_SURROUNDS = 0;
long REPEAT_LAWSUITS1 = 0;
long REPEAT_SURROUNDS1 = 0;
long REPEAT_P24A1S = 0;
long REPEAT_MESSAGES = 0;
long REPEAT_ARCHIVES = 1;
long REPEAT_ANNOUNCES = 1;
long REPEAT_PAYMENTS = 1;
long REPEAT_DESTROYS = 0;
long REPEAT_BARCODES = 1;
long REPEAT_BOOKS_A = 0;
long REPEAT_BOOKS_B = 0;

// ----- AUTORUN -------------------------------------------------------------
long AUTORUN_BASICS = 1;
long AUTORUN_FIXES = 1;
long AUTORUN_WALNS = 1;
long AUTORUN_MDRS = 0;
long AUTORUN_ENDOCS = 1;
long AUTORUN_MSRS = 0;

// ----- EXTERNAL ------------------------------------------------------------
char *const EISPP_XMLPATH = Electrics[TYPE_EISPP].docketPath;
long SAVE_SUBPOENAS = 0;
long SHOWUP_REGISTER_ACT = 0;
long WORD_OPEN_7A = 0;
char EXTERNAL_SERVER[SIZE_OF_PATH] = "";
long WAIT_FOR_ATTACH = 0;
char SIGN_TEST[SIZE_OF_PATH] = "..\SignTest";
long SIGN_PDFS = 0;
long HIDE_ALL_ON_SAVE = 0;

// ----- HYPER ---------------------------------------------------------------
char WEB_PATH[SIZE_OF_DIR];
long WEB_SCHEME = 0;
long WEB_ATTACH = 0;
long HTML_OPEN_DELTA = 5;
char HTML_OVER_PATH[SIZE_OF_DIR];
long HTML_MAPS_FORMAT = 0;
long HTML_MAPS_ZOOM = 18;
char HTML_LOGO_NAME[SIZE_OF_HTEMPLATE_NAME];
long HTML_LOGO_WIDTH = 119;
long HTML_LOGO_HEIGHT = 0;
long HTML_LOGO_SCALE = 94;

// ----- PRINT ---------------------------------------------------------------
long DEFAULT_PAGE_LEN	= 60;
// 2007:337 FIX: backward compatibility
long DEFAULT_ALIGNMENT	= FLOAT_ALIGNMENT;

// ----- APPLICATION ---------------------------------------------------------
long PRINT_COMPOSITIONS = 0;
long PRINT_JUDGEMENTS = 0;
long PROPAGANDATE_JUDGE = 0;
long SHOWUP_IMPORTANT = 0;
long AREA_SPLIT_DESC_BOOK = 1;
long PRINT_UCN = 0;
long LOCK_RETURN = 0;
long KILL_FORWARD_SIDES = 0;
long WRITINGS_ACTIVE = 0;
long PRINT_COLLEGE = 1;
long RKO_ADD_DATA = 1;
long DEC_REG_NUM_ALL = 0;
char OUT_PATH[SIZE_OF_DIR];
long PING_DATABASE = 0;
CDate WEB_FIXED_START_DATE(0, 0, 0);
CDate WEB_FIXED_FINAL_DATE(0, 0, 0);
CDate WEB_QUERY_START_DATE(0, 0, 0);
CDate WEB_QUERY_FINAL_DATE(0, 0, 0);
long WEB_QUERY_USE_STYLES = 1;
long PRINT_EXEC_LIST_NO = 0;
long PRINT_EXEC_LIST_SUMS = 1;
long LOCK_RECEIVE = 0;
long EXTERNAL_LAWSUIT_NO = 0;
long USER_INTERSECT = 0;
long CENTER_PROGRESS_BAR = 0;
long PRINT_PAGE_BREAK = 1;
long SIGN_SUMMONS = 0;
long ALT_PATCH_COLUMNS = 0;
long LEFT_MEETING = 0;
long LOCK_REGISTER = 0;
#if COURT_TYPE == COURT_MILITARY
long REPORT_NEW_LAWSUITS = 0;
#endif  // MILITARY
long LINK_NOTIFICATION = 0;

// ----- SCHED ---------------------------------------------------------------
long SCHED_ACTIVE = 0;
long SCHED_SHOWUP = 0;
long SCHED_DELTA_X = 1;
long SCHED_DELTA_Y = -1;
long SCHED_DELTA_Z = -3;

// ----- SMS -----------------------------------------------------------------
long SMS_ACTIVE = 0;
char SMS_PATH[SIZE_OF_DIR];

// ----- CHROME --------------------------------------------------------------
long CHROME_TEXT_MAXIMIZED = 0;
long CHROME_HTML_MAXIMIZED = 2;
long CHROME_FONT_SCALE = 1;
long CHROME_PUSH_IMAGE = 1;
long CHROME_AUTO_COMPLETE = 1;
long CHROME_KEYBOARD_LAYOUT = 0;
long CHROME_AUTO_REMEMBER = 3;
long CHROME_EFILE_MENUS = 0;
long CHROME_THEME = 0;
long CHROME_SIDE_SEARCH = 0;
long CHROME_AUTO_SELECT = 1;
long CHROME_CENTER_OPEN_SAVE = 0;
long CHROME_LOAD_KNOWNS = 1;
long CHROME_WAIT_FOR_IDLE = 5;
//long CHROME_BARCODE_ENTER = 0;
long CHROME_LUCIDACONSOLE = 0;	// low, Andale Mono too
long CHROME_PERCENT_SCALE = 75;
#if TESTVER
long CHROME_UNICODE_CONVERT = 1;	// 2018-10-30: LPR: was 4; 2018-11-07: was 3
#else  // TESTVER
long CHROME_UNICODE_CONVERT = 0;
#endif  // TESTVER

// ----- DISTRIB -------------------------------------------------------------
long DISTRIB_COUNT_REJUDGE = 1;
long DISTRIB_SESSION_LENGTH = LENGTH_OF_STANDARD_SESSION;
long DISTRIB_SESSION_PAUSE = LENGTH_OF_SESSION_PAUSE;
long DISTRIB_SESSION_TIMES = 0;
long DISTRIB_SESSION_TIMES_FORCE = 0;
long DISTRIB_RESERVE_JURORS = 0;
long DISTRIB_JURORS_LOAD = 0;
long DISTRIB_SESSION_7302 = 1;
long DISTRIB_SESSION_MIN20L = 1;
long DISTRIB_SESSION_MAX8H = 1;

// ----- UPDATE --------------------------------------------------------------
long UPDATE_ACTIVE = 1;
char UPDATE_DIRECTORY[SIZE_OF_DIR] = "";
long UPDATE_WAIT = 3;
long UPDATE_VERBOSE = 0;
long UPDATE_POWERSHELL = 0;
long UPDATE_LOCAL_COPY = 0;

// ----- FILTER --------------------------------------------------------------
long FILTER_DECISIONS = 1;
long FILTER_SESSIONS = 1;
long FILTER_OTHERS = 1;
char *const FILTER_PATH = Electrics[TYPE_HTML].docketPath;
long FILTER_TYPE = 0;
long FILTER_NUMBER = 0;
long FILTER_OLE_CASE = 1;
long FILTER_JUDICIAL = 0;	// 2009:288 LRQ: +related: 1 -> 0
long FILTER_HIDE_SPELL = 0;	// 2010:118 LPR
long FILTER_FIND_UCLP = 0;
long FILTER_FIND_STREET = 0;
long FILTER_RUN_MACRO = 0;	// 2018-05-28: IRQ: +related

// ----- XFER ----------------------------------------------------------------
long XFER_ACTIVE = 0;
char XFER_PATH[SIZE_OF_DIR];
int XFER_LEN = 0;
char XFER_CCEXP[SIZE_OF_PATH];
long XFER_EMAIL = 0;
char XFER_BANKRUPT_PATH[SIZE_OF_DIR];
long XFER_LEGAL_AID = 0;
char XFER_LEGAL_AID_PATH[SIZE_OF_DIR];
long XFER_EPORTAL = 0;
long XFER_LINK_APPEALS = 0;

// ----- EISPP ---------------------------------------------------------------
long EISPP_ACTIVE = 0;
char EISPP_OUTPATH[SIZE_OF_DIR];
char EISPP_VZLPLC[SIZE_OF_EISPP_STRING];
char EISPP_VZLIZP[SIZE_OF_EISPP_STRING];
long EISPP_STRUCTURA_PLC = 0;
long EISPP_STRUCTURA_IZP = 0;
char EISPP_KST_USRSID[SIZE_OF_EISPP_STRING];
char EISPP_KST_USRREJ[SIZE_OF_EISPP_STRING];
char EISPP_KST_USRRAB[SIZE_OF_EISPP_STRING];
char EISPP_KST_ARMSID[SIZE_OF_EISPP_STRING];
// 2012:333
char EISPP_KST_OKASID[SIZE_OF_EISPP_STRING];
char EISPP_KST_SBEVID[SIZE_OF_EISPP_STRING];
char EISPP_KST_ARMSTRSID[SIZE_OF_EISPP_STRING];
char EISPP_KST_SESSID[SIZE_OF_EISPP_STRING];
char EISPP_KST_LGTSID[SIZE_OF_EISPP_STRING];
long EISPP_QUERY_TIMEOUT = 120;
char EISPP_EIQ_USRSID[SIZE_OF_EISPP_STRING];
char EISPP_EIQ_USRREJ[SIZE_OF_EISPP_STRING];
long EISPP_DIAG_NAMES = 0;

// ----- POSTERM -------------------------------------------------------------
long POSTERM_ACTIVE[POSTERM_COUNT] = { 0, 0 };
long POSTERM_ECR = 0;
char POSTERM_PORT_NAME[POSTERM_COUNT][SIZE_OF_PORT_NAME];
long POSTERM_OPERATOR = 0;
long POSTERM_CURRENCY = 0;
long POSTERM_REINITIALIZE = 0;

// ----- functions -----------------------------------------------------------
#include "FontTestDialog.h"
#include "FontTestDialog.cpp"

#define DUMPFONT 0

static void initialize_fonts()
{
	TScreenDC screenDC;
	int fixedScale = FONT_SCALE == 1 ? 0 : FONT_SCALE;
	int screenWidth = screenDC.GetDeviceCaps(HORZRES);
	int screenHeight = screenDC.GetDeviceCaps(VERTRES);
	int validScale = 0;	// zero just in case we never init it below

	// 2007:248 LPR: should be zeroed on reloading to init them below
	FONT_MONO.fsOrig = FONT_NORMAL.fsOrig = 0;

#if DUMPFONT
	TUserFile uf("служебен");
	FILE *f = uf.Open("dumpfont.txt", "w");
	uf.Assert();
	fputs("size   normal width   normal height   normal orig   mono width   mono height   mono orig\n\n", f);
#endif  // DUMPFONT

	for (FONT_SCALE = 8; FONT_SCALE <= (fixedScale ? fixedScale : 24); FONT_SCALE++)
	{
		TFontTestDialog mono(NULL, IDD_TEST_MONO);
		mono.Create();
		TRect rectMono = mono.GetWindowRect();
		TFontTestDialog normal(NULL, IDD_TEST_NORMAL);
		normal.Create();
		TRect rectNormal = normal.GetWindowRect();
		bool fit =
			FONT_SCALE == 8 || fixedScale ||
			(rectMono.Width() <= screenWidth &&
			rectMono.Height() <= screenHeight &&
			rectNormal.Width() <= screenWidth &&
			rectNormal.Height() <= screenHeight);
		TFontScale fontMono = mono.GetFontScale();
		TFontScale fontNormal = normal.GetFontScale();

#if DUMPFONT
		fprintf(f, "%4d   %12d   %13d   %11d   %10d   %11d   %9d\n",
			FONT_SCALE, rectNormal.Width(), rectNormal.Height(), fontNormal.fsOrig, rectMono.Width(),
			rectMono.Height(), fontMono.fsOrig);
		fit = true;
#endif  // DUMPFONT
		if (fit && fontMono.fsOrig != FONT_MONO.fsOrig && fontNormal.fsOrig != FONT_NORMAL.fsOrig)
		{
			FONT_MONO = fontMono;
			FONT_NORMAL = fontNormal;
			validScale = FONT_SCALE;
		}

		if (!fit)
			break;
	}

#if DUMPFONT
	uf.Close();
	fatal("DUMPFONT");
#endif  // DUMPFONT

	FONT_SCALE = validScale > 8 || fixedScale ? validScale : 0;
}

static LPMALLOC ppMalloc = NULL;

#include "TestListWindow.h"
#include "TestListWindow.cpp"

//{{TDialog = TListTestDialog}}
class TListTestDialog : public TFontTestDialog
{
public:
	TListTestDialog();
};    //{{TListTestDialog}}

//{{TTListTestDialog Implementation}}

TListTestDialog::TListTestDialog()
	: TFontTestDialog(NULL, IDD_TEST_NORMAL)
{
	new TTestListWindow(this, IDC_TEST_FONT_LIST_HEADERS, &LISTW_HEADERS);
	new TTestListWindow(this, IDC_TEST_FONT_LIST_NOHEADS, &LISTW_NOHEADS);
}

bool snd_check(const char *s)
{
	sn_cls::init();	// rare
	printsub_init();

	sn_dec *const snd = new sn_dec;
	TVoidPtr VP(snd);

	strcpy(snd->sn, s);
	snd->make_key(vk.key, vk.version);
	return snd->precode();
}

void initialize_config()
{
	*SERIAL_NO = '\0';

	{	// init local config name
		DWORD size = sizeof CONFIG_NAME_LOCAL;

		if (!GetComputerName(CONFIG_NAME_LOCAL, &size))
			fatal("Грешка %lu при определяне името на компютъра.", GETLASTERROR);

		if (NetValidateName)
		{
			NET_API_STATUS status = net_validate_name(NULL, CONFIG_NAME_LOCAL, NULL, NULL, NetSetupMachine);

			if (status != NERR_Success)
				fatal("Невалидно име на компютъра (%ld).", (long) status);
		}

		strupr(CONFIG_NAME_LOCAL);
	}

	TConfigure::Load(CONFIG_NAME_GLOBAL);
	TConfigure::Load(CONFIG_NAME_LOCAL);
	TConfigure::Apply();
	TConfigure::ShowErrors();

	{	// parse/init serial#
		int i;
		char *s = SERIAL_NO;

		for (i = strlen(s); i < 24 + 3; i++)
			s[i] = "X-"[i % 7 == 6];
		s[i] = '\0';

		_COURT_CODE = new int[1];
		_COURT_AREA = new int[1];
		_CONFIG_MAX = _COURT_UCLP = _CourtType = NULL;
		_COURT_FLAT = _COURT_WRIT = _COURT_N10K = NULL;
		_COURT_MINI = _COURT_LOCK = NULL;
		// 2015:315 40 -> SIZE_OF_COURT_NAME
		AppNameCourt = new char[SIZE_OF_COURT_NAME];
		memset(AppNameCourt, '\0', SIZE_OF_COURT_NAME);

		sn_dec *const snd = new sn_dec;
		TVoidPtr VP(snd);

		strcpy(snd->sn, SERIAL_NO);
		snd->make_key(vk.key, vk.version);
		SERIAL_ERROR = snd->decode();

		if (!SERIAL_ERROR)
		{
			CDate serialDate(snd->day, snd->month, snd->z_year + YEAR_MIN);
			Court->Seek(snd->court);

			_CourtType = new int[1];
			CourtType = Court->courtType - '0';

			if (!serialDate.Valid())
				SERIAL_ERROR = sn_dec::SNE_VDATE;
		#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
			else if (CourtType != COURT_TYPE)
				SERIAL_ERROR = sn_dec::SNE_CTYPE;
		#endif  // APPEAL || ADMIN || AREAL || REGIONAL
		#if COURT_TYPE == COURT_MILITARY
			else if (CourtType != COURT_MILITARY && CourtType != COURT_MILITARY_APPEAL)
				SERIAL_ERROR = sn_dec::SNE_CTYPE;
		#endif  // MILITARY
			else
			{
				_CONFIG_MAX = new int[1];
				CONFIG_MAX = snd->users;
				COURT_CODE = snd->court;
				_COURT_UCLP = new int[1];
				COURT_UCLP = Court->uclp;
				COURT_AREA = Court->courtArea;

				_COURT_FLAT = new int[1];
				_COURT_WRIT = new int[1];
				_COURT_N10K = new int[1];
				_COURT_MINI = new int[1];
				_COURT_LOCK = new int[1];
				_COURT_SIME = new int[1];
				_COURT_CSVE = new int[1];
				_COURT_EXAR = new int[1];

				COURT_FLAT = snd->flags & 0x01;
				COURT_WRIT = snd->flags & 0x02;
				COURT_N10K = snd->flags & 0x04;
				COURT_MINI = snd->flags & 0x08;
				COURT_LOCK = snd->flags & 0x10;
				COURT_SIME = snd->flags & 0x20;
				COURT_CSVE = snd->flags & 0x40;
				COURT_EXAR = snd->flags & 0x80;

				// 2018-02-08 LPR: separated
				serialDate += 366;

				if (Today > serialDate && !COURT_MINI)
					COURT_MINI = COURT_FLAT = COURT_WRIT = 0x0B;
			}
		}

		if (SERIAL_ERROR)
		{
			// 2013:003 LPR: revived to avoid fatals
			COURT_AREA = BUILDIN_AREAL_COURT;
		#if COURT_TYPE == COURT_APPEAL
			COURT_CODE = BUILDIN_APPEAL_COURT;
		#endif  // APPEAL
		#if COURT_TYPE == COURT_ADMIN
			COURT_CODE = BUILDIN_ADMIN_COURT;
		#endif  // ADMIN
		#if COURT_TYPE == COURT_AREAL
			COURT_CODE = BUILDIN_AREAL_COURT;
		#endif  // AREAL
		#if COURT_TYPE == COURT_MILITARY
		#if SPECIAL
			COURT_CODE = BUILDIN_SPECIAL_COURT;
		#else  // SPECIAL
			COURT_CODE = BUILDIN_MILITARY_COURT;
		#endif  // SPECIAL
		#endif  // AREAL
		#if COURT_TYPE == COURT_REGIONAL
			COURT_CODE = BUILDIN_REGIONAL_COURT;
		#endif  // REGIONAL
		}
	}

	if (!any(OUT_PATH))
	{
		char MyDocuments[MAX_PATH];
		LPITEMIDLIST ppMyDocs = NULL;
		if (!ppMalloc)
			CoGetMalloc(1, &ppMalloc);

		*MyDocuments = '\0';
		if (SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &ppMyDocs) == NOERROR)
			SHGetPathFromIDList(ppMyDocs, MyDocuments);
		strcpy(OUT_PATH, any(MyDocuments) && strlen(MyDocuments) < SIZE_OF_DIR ? MyDocuments : "C:\\MYDOCU~1");
		if (ppMalloc && ppMyDocs)
			IMalloc_Free(ppMalloc, ppMyDocs);
	}

	TConfigure::GetCurrent("chrome", "FONT_SCALE", 1, false);

	if (SMS_ACTIVE)
	{
		TConfigure::GetCurrent("sms", "PATH", false);

		if (!any(SMS_PATH))
		{
			TConfigure::AddError("Директорията за изпращане на SMS-и липсва или е с невалидна стойност. "
				"Изпращането на SMS-и е изключено");
			SMS_ACTIVE = 0;
		}
	}

	if (EISPP_ACTIVE)
	{
		bool failed = false;

		TConfigure::GetCurrent("eispp", "OUTPATH", false);
		TConfigure::GetCurrent("eispp", "XMLPATH", false);

		if (!any(EISPP_OUTPATH))
		{
			TConfigure::AddError("Директорията за изпращане на файлове към ЕИСПП липсва или е с невалидна стойност");
			failed = true;
		}

		if (!any(EISPP_XMLPATH))
		{
			TConfigure::AddError("Директорията под която се съхраняват файловете на ЕИСПП липсва или е с невалидна стойност");
			failed = true;
		}

		Court->Seek(COURT_CODE);
		COURT_EISPP = Court->eispp;

		if (COURT_EISPP)
		{
			EisDepart->Seek(COURT_EISPP);

			if (strlen(EisDepart->tlacr) == 3)
				strcpy(COURT_TLACR, EisDepart->tlacr);
			else
			{
				TConfigure::AddError("Текущия съд няма буквен ЕИСПП код");
				failed = true;
			}
		}
		else
		{
			TConfigure::AddError("Текущия съд няма цифров ЕИСПП код");
			failed = true;
		}

		if (failed)
		{
			TConfigure::AddError("Обмена на данни с ЕИСПП е изключен");
			EISPP_ACTIVE = 0;
		}
	}

	if (XFER_LEGAL_AID)
	{
		TConfigure::GetCurrent("transfer", "LEGAL_AID_PATH", false);

		if (!any(XFER_LEGAL_AID_PATH))
		{
			TConfigure::AddError("Директорията за обмен на данни с програма \"Служебни Защити\" липсва или "
				"е невалидна. Обмена на данни със \"Служебни Защити\" е изключен");
			XFER_LEGAL_AID = 0;
		}
	}

	if (UPDATE_ACTIVE)
	{
		TConfigure::GetCurrent("update", "PATH", false);

		if (!any(UPDATE_DIRECTORY))
		{
			TConfigure::AddError("Директорията за автоматична актуализация липсва или е невалидна. "
				"Автоматичната актуализация е изключена");
			UPDATE_ACTIVE = 0;
		}
	}

	if (PRINT_BARCODES && !any(BARCODE_FONT))
	{
		TConfigure::AddError("Не е зададен BARCODE_FONT. Печатът на баркодове ще бъде изключен");
		PRINT_BARCODES = 0;
	}

	if (!any(FILTER_PATH))
		FILTER_DECISIONS = FILTER_SESSIONS = FILTER_OTHERS = 0;

	if (XFER_ACTIVE & XFER_REQLAWS)
		XFER_LEN = strlen(XFER_PATH);

	if (SERIAL_VALID)
	{
		TConfigName configName(CONFIG_NAME_LOCAL);

		if (!(configName.Try() ? configName.Check() : configName.Insert()))
			failure();
	}

	if (LOGIN_LIMIT == 1 || LOGIN_LIMIT == 2)
	{
		TConfigure::AddError("serial", "LOGIN_LIMIT", "Невалидна стойност");
		LOGIN_LIMIT = 3;
	}

	// 2011:312 LPR: email only for uclp == buildin
	// 2011:325 LPR: must be after initialize court
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	if (_COURT_UCLP && COURT_UCLP == BUILDIN_UCLP)
		TConfigure::GetCurrent("transfer", "EMAIL", false);
#endif  // APPEAL || ADMIN || AREAL || REGIONAL

	if (FONT_SCALE > 1 && FONT_SCALE < 8)
	{
		TConfigure::AddError("chrome", "FONT_SCALE", "Невалидна стойност");
		FONT_SCALE = 1;
	}

	if (FONT_SCALE)
		initialize_fonts();

	if (FONT_SCALE <= 8 || FONT_MONO.fsOrig < 24 || FONT_NORMAL.fsOrig < 24)
		CHROME_PUSH_IMAGE = 0;

	TListTestDialog().Create();	// 2010:036

	if (SetThemeAppProperties)
		SetThemeAppProperties(CHROME_THEME ? STAP_ALLOW_NONCLIENT | STAP_ALLOW_CONTROLS : STAP_ALLOW_NONCLIENT);
	else
		CHROME_PUSH_IMAGE = 0;

	// POSTERM
	if (POSTERM_ECR)
	{
		TConfigData configData(CONFIG_NAME_LOCAL);
		msql m(configData.Name);

		m.AddString("NOT F_CONFIG", CONFIG_NAME_LOCAL);
		m.AddString("F_SECTION", "posterm");
		m.AddString("F_NAME", "ECR");
		m.Add("F_VALUE");
		m.printf("LIKE '%%%d%%'", POSTERM_ECR);

		TQuery q(str(m));
		while (q.Read())
		{
			configData << q;

			if (atoi(configData.value) == POSTERM_ECR)
			{
				mstr m;

				m.printf("[posterm] ECR се дублира с този на конфигурация %s. Коригирайте го", configData.config);
				TConfigure::AddError(str(m));
				POSTERM_ECR = 0;
				break;
			}
		}
	}

	if (POSTERM_ECR)
	{
		if (POSTERM_ACTIVE[POSTERM_BUDGET] && POSTERM_ACTIVE[POSTERM_DEPOSITS] &&
			!strcmp(POSTERM_PORT_NAME[POSTERM_BUDGET], POSTERM_PORT_NAME[POSTERM_DEPOSITS]))
		{
			TConfigure::AddError("[posterm] BUDGET_PORT_NAME и DEPOSITS_PORT_NAME съвпадат.\n\nПревода на суми по "
				"сметка депозити е изключен");
			POSTERM_ACTIVE[POSTERM_DEPOSITS] = 0;
		}
	}
	else if (POSTERM_ACTIVE[POSTERM_BUDGET] || POSTERM_ACTIVE[POSTERM_DEPOSITS])
	{
		TConfigure::AddError("posterm", "ECR", "липсва или е с невалидна стойност. Превода на суми през ПОС-терминал е изключен");
		POSTERM_ACTIVE[POSTERM_BUDGET] = POSTERM_ACTIVE[POSTERM_DEPOSITS] = 0;
	}

#if COURT_TYPE == COURT_MILITARY
	// 2012:303 LPR: handle military appeal; 2014:086 LPR: const -> config
	strcpy(KIND_APPEAL_IN_DOCS, _CourtType && CourtType == COURT_MILITARY_APPEAL ? KIND_APPEAL_2ND_INDOCS :
		KIND_APPEAL_1ST_INDOCS);
	strcpy(KIND_IN_REG_X_DOCS, _CourtType && CourtType == COURT_MILITARY_APPEAL ? KIND_IN_REG_X_2ND_DOCS :
		KIND_IN_REG_X_1ST_DOCS);
#endif  // MILITARY

	// 2015:219
	if (SAVE_SUBPOENAS && !any(Electrics[TYPE_TEXT].docketPath))
	{
		TConfigure::AddError("external", "TEXT_PATH", "липсва или е с невалидна стойност. Съхранението на призовки и "
			"списъци на призованите лица в електронната папка е изключено");
		SAVE_SUBPOENAS = 0;
	}

	// 2016:165
	strcpy(Electrics[TYPE_PORTREG].sourcePath, Electrics[TYPE_IMAGE].sourcePath);
	strcpy(Electrics[TYPE_PORTREG].docketPath, Electrics[TYPE_IMAGE].docketPath);

#if TESTVER
	// 2017:317
	constant default_text_editor = electric_matches("a.doc", TYPE_TEXT, true, true) ||
		electric_matches("a.docx", TYPE_TEXT, true, true) ? "*Word.Document" : "";

	TConfigure::GetCurrent("external", "TEXT_EDITOR", default_text_editor, false);
#endif  // TESTVER

	TConfigure::ShowErrors();
}

void shutdown_config()
{
	FONT_SCALE = 0;

	delete[] _COURT_EXAR;
	delete[] _COURT_CSVE;
	delete[] _COURT_SIME;
	delete[] _COURT_LOCK;
	delete[] _COURT_MINI;
	delete[] _COURT_N10K;
	delete[] _COURT_WRIT;
	delete[] _COURT_FLAT;
	delete[] _COURT_CODE;
	delete[] _COURT_AREA;
	delete[] _CourtType;
	delete[] _COURT_UCLP;
	delete[] _CONFIG_MAX;
	delete[] AppNameCourt;
}

// ----- TPrinterGroup -------------------------------------------------------
TPrinterGroup::TPrinterGroup()
{
	Add(
		new TLong("PORT", &port),
		new TCheck("CP1251", &cp1251, TRUE),
		new TCheck("CHECK", &check, TRUE),
		new TCheck("PSEUDO", &pseudo, TRUE),
		new TCheck("WINDOWS", &windows, TRUE),
		new TCheck("SETUPEX", &setupex, TRUE),
		new TLong("JUSTIFY", &justify),
		new TLong("PAGE_LEN", &pagelen),
		new TLong("LINE_WIDTH", &linewidth),
		new TCheck("WINDOWS_A3", &windowsA3, TRUE),
		new TLong("ALIGN_TO", &alignto),
		NULL
	);
}

void TPrinterGroup::Load()
{
	port = PRINT_PORT;
	cp1251 = PRINT_CP1251 != 0;
	check = PRINT_CHECK != 0;
	pseudo = PRINT_PSEUDO != 0;
	windows = WINDOWS_PRINT != 0;
	setupex = PRINT_SETUPEX != 0;
	justify = PRINT_JUSTIFY;
	pagelen = DEFAULT_PAGE_LEN;
	linewidth = PRINT_LINE_WIDTH;
	windowsA3 = WINDOWS_A3 != 0;
	alignto = NO_ALIGNMENT;
}

void TPrinterGroup::Save()
{
	TConfigData configData(CONFIG_NAME_LOCAL);

	strcpy(configData.section, "print");
#if _TREE_GROUPS
	for (TLong *field = (TLong *) GetFirst(); field; field = (TLong *) GetNext(field))
	{
#else  // _TREE_GROUPS
	for (int i = 0; i < Count(); i++)
	{
		TLong *field = (TLong *) (*this)[i];
#endif  // _TREE_GROUPS

		strcpy(configData.name, field->Name);
		bool edit = configData.Try();

		ltoa(field->L(), configData.value, 10);
		if (edit)
			configData.Update();
		else
			configData.Insert();
	}

	PRINT_PORT = port;
	PRINT_CP1251 = cp1251;
	PRINT_CHECK = check;
	PRINT_PSEUDO = pseudo;
	WINDOWS_PRINT = windows;
	PRINT_SETUPEX = setupex;
	PRINT_JUSTIFY = justify;
	DEFAULT_PAGE_LEN = pagelen;
	PRINT_LINE_WIDTH = linewidth;
	WINDOWS_A3 = windowsA3;
	DEFAULT_ALIGNMENT = alignto;
}

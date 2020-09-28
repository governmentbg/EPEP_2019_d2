#include "cac.h"

#if TESTVER

#define DUMP_INVOLVEMENTS 0
#define TEST_DATE_RECR 0
#define TEST_DATE_DUMP 0
#define TEST_DATE_BACK 0
#define TEST_DATE_TIME 0
#define TEST_CTYPE 0
#define TEST_CATEGORY 0
#define TEST_LINKS 0
#define TEST_APPEAL 0
#define TEST_OUTCOMES 0
#define TEST_FINDWORD 0
#define TEST_SUBJSBES 0
#define TEST_GTTIME 0
#define TEST_TABDESC 0
#define TEST_QUOT 0
#define TEST_BDERROR 0
#define TEST_POSTERM 0
#define TEST_WAIT 0
#define TEST_RANDOM_ACT 0
#define TEST_MPRINT 0
#define TEST_CRITERIA 0
#define TEST_TEXT 0
#define TEST_POWERSH 0
#define TEST_PERSONAL 0
#define TEST_SCANDATE 0
#define TEST_POSTERM_LRC 0
#define TEST_CLOSEDS 0
#define TEST_EIS_USED 0
#define TEST_OPEN_SAVE 0
#define TEST_FILETYPE 0
#define TEST_MAKE_DIR 0
#define TEST_DISCARD_UCNS 0
#define TEST_LOG_TEXT 0
#define TEST_DIALECT 0
#define TEST_DBSTRIM 0
#define TEST_WEEK_DAY 0
#define TEST_GOP_LOAD 0
#define TEST_FIRM_PRINT 0
#define TEST_LOAD_HALL 0
#define TEST_SYS_COLOR 0
#define TEST_VAR_FLAGS 0
#define TEST_SINDICK 0
#define TEST_MESSAGE 0
#define TEST_REGISTRY 0
#define TEST_ODBCURS 0
#define TEST_ODBCLOSE 0
#define TEST_DOUBLEXC 0
#define TEST_STR2MSTR 0
#define TEST_PROCID 0
#define TEST_SUMWORDS 0
#define TEST_UROUND 0
#define TEST_SYNCDATE 0
#define TEST_DOTTED 0
#define TEST_MENU_FONT 0
#define TEST_SHORT_NAME 0
#define TEST_FAILURE 0
#define TEST_PLAIN 0
#define TEST_NIA_PROT 0
#define TEST_BARCODE 0
#define TEST_TRANSLIT 0
#define TEST_IBCTABLE 0
#define TEST_ADD_ARRAY 0
#define TEST_BSTRING 0
#define TEST_SHOWHTML 0
#define TEST_PRINT_MISS 0
#define TEST_NEWDATABASE 0
#define TEST_VSS_INDEXES 0
#define TEST_CREATE_DAMN 0
#define TEST_EIS_SBES 0
#define TEST_DBI_TEMP 0
#define TEST_UP_LOW 0
#define TEST_HTM_NAMES 0
#define TEST_SUBPOENA_NO 0
#define TEST_EXPORT_CSV 0
#define TEST_FINALNONENT 0
#define TEST_AUTOPTR 0
#define TEST_WAITIME 0
#define TEST_FILESERV 0
#define TEST_ATTACH_ACTS 0
#define TEST_QUERY_UNATT 0
#define TEST_BYTEA_ENCOD 0
#define TEST_BYTEA_DECOD 0
#define TEST_CRLF_CONV 0
#define TEST_BLOB_SERVER 0
#define TEST_ASK_JOIN 0
#define TEST_GET_ADDRESS 0
#define TEST_PASS_XOR 0
#define TEST_XFER_UUID 0
#define TEST_CLASS_NAME 0
#define TEST_SUBJECTS 0
#define TEST_DUMP_SUBJS 0
#define TEST_LAST_MOVE 0
#define TEST_SURROUND 0
#define TEST_SURROUNDMENT 0
#define TEST_SPLITACTFLAGS 0
#define TEST_SPLITSESFLAGS 0
#define TEST_SNPRINTF 0
#define TEST_JUROR_LOAD 0
#define TEST_COUNTEX 0
#define TEST_WAIT_RESID 0
#define TEST_OTHER_INVOLV 0
#define TEST_SUBJECT_TEXT 0
#define TEST_QUERY_RESTART 0
#define TEST_PSHELL_CKSUM 0
#define TEST_REQUEST_ID 0
#define TEST_FATAL 0
#define TEST_POST_CHIEF 0
#define TEST_STREQUAL 0
#define TEST_EIS_NAME 0
#define TEST_REKIND_NAME 0
#define TEST_ACVAR_BOZA 0
#define DUMP_XFER_INDOC_KINDS 0
#define TEST_UNFINISHED 0
#define TEST_BANK_HTML 0
#define TEST_SEQ_OPEN 0
#define TEST_POSTERM_LOGGING 0
#define TEST_EXEC_WAIT 0
#define TEST_TRANS_ID_INVALID 0
#define TEST_CLIP_UNICODE 0
#define TEST_SENCOR_NAME 0
#define TEST_PRINT_CONNECT 0
#define TEST_DUMP_KEYS 0
#define TEST_COMMON_FIELDS 0
#define TEST_LOG_MD5SUM 0
#define TEST_OPEN_ENTER 0

void test_rtl()
{
	char cia, ciu;
	int count = sscanf("яю", "%1c%1c", &cia, &ciu);

	cac_assert(count == 2 && cia == 'я' && ciu == 'ю');

	for (int c = 0; c <= 0xFF; c++)
		cac_assert((isdigit(c) != 0) == (c >= '0' && c <= '9'));

	cac_assert(memicmp("абвгдежзийклмнопрстуфхцчшщъыьэюя", "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ", 32) == 0);
	cac_assert(strncmpi("бо", "БОЗА", 2) == 0);

	mstr m;
	CDate date(1, 1, 1990);

	m.printf("%0D", &date);
	cac_assert(m.cmp("01.01.1990") == 0);
}

#if DUMP_INVOLVEMENTS
static void list_invs(FILE *f, const char *which, const char *involvements)
{
	fprintf(f, "Страни: %s\n\n", which);
	for (const char *inv = involvements; *inv; inv++)
	{
		Involvement->Seek(*inv);
		fprintf(f, "%s\n", Involvement->name);
	}
	fprintf(f, "\n");
}

static void list_kinds(FILE *f, const char *what, const char *kinds)
{
	fprintf(f, ">> Група: %s\n\n", what);

	for (const char *kind = kinds; *kind; kind++)
	{
		Kind->Seek(*kind);
		fprintf(f, "> Елемент: %s\n\n", Kind->name);
		list_invs(f, "Леви", Kind->suitors);
		list_invs(f, "Десни", Kind->defendants);
		list_invs(f, "Трети", Kind->others);
	}
}
#endif  // DUMP_INVOLVEMENTS

#define TEST_DATE_FUNC (TEST_DATE_RECR || TEST_DATE_DUMP || TEST_DATE_BACK)

#if TEST_DATE_FUNC
static void test_date_func()
{
	TUserFile uFile("служебен");
	FILE *check;
	CDateTime stamp;

#if TEST_DATE_RECR
	for (int i = 0; i < BASE_COUNT; i++)
		data[i].Initialize();

	if ((check = fopen("c:\\tmp\\RECR.DAT", "wb")) == NULL)
		fatal("open RECR.DAT");
	for (int i = 0; i < BASE_COUNT; i++)
	{
		if (fwrite(&data[i].base, sizeof(long), 1, check) != 1)
			fatal("write base %d", i);
		for (int n = 0; n < CALC_COUNT; n++)
			if (fwrite(&data[i].calc[n], sizeof(long), 1, check) != 1)
				fatal("write data %d, %d", i, n);
	}
	if (fclose(check))
		fatal("close RECR.DAT");
#endif  // TEST_DATE_RECR

#if TEST_DATE_DUMP
	stamp = DATETIME_MIN;

	if ((check = fopen("c:\\tmp\\datetime.txt", "w")) == NULL)
		fatal("open datetime.txt");

	while (stamp <= DATETIME_MAX)
	{
		fprintf(check, "%2d %2d %4ld %2d %02d %ld\n", stamp.day, stamp.month, stamp.year, stamp.hour, stamp.minute,
			stamp.Value());
		//fprintf(check, "%ld\n", stamp.Value());

		if (++stamp.minute == 60)
		{
			if (++stamp.hour == 24)
			{
				((CDate &) stamp) += 1;
				stamp.hour = 0;
			}

			stamp.minute = 0;
		}
	}

	if (fclose(check))
		fatal("close datetime.txt");
#endif  // TEST_DATE_DUMP

#if TEST_DATE_BACK
	char buffer[0x100];
	long value;
	CDateTime proof;
	TWaitWindow wait(NULL, PLEASE_WAIT);
	int count = 0;
	time_t start = time(NULL);

	if ((check = fopen("c:\\tmp\\datetime.txt", "r")) == NULL)
		fatal("open datetime.txt");

	while (fgets(buffer, sizeof buffer, check) != NULL)
	{
		if (sscanf(buffer, "%d %d %ld %d %d %ld", &stamp.day, &stamp.month, &stamp.year, &stamp.hour, &stamp.minute,
			&value) != 6)
		{
			fatal("read datetime.txt");
		}

		proof.SetValue(value);

		if (proof != stamp)
			fatal("date time error, value = %ld", value);

		if (!(++count % 100000))
		{
			sprintf(buffer, "%d обработени...", count);
			wait.TickText(buffer);
		}
	}

	if (fclose(check))
		fatal("close datetime.txt");

	message("%d seconds", time(NULL) - start);
#endif  // TEST_DATE_BACK
}
#endif  // TEST_DATE_FUNC

#if TEST_TABDESC
class TTestTable : public TTable
{
public:
	TTestTable(int table, const char *name) : TTable(name) { descs = Descs + table; }

	void Dump(FILE *f);
};

void TTestTable::Dump(FILE *f)
{
	GetDescs();

	for (int i = 0; i < descs->Count(); i++)
	{
		pFLDDesc desc = (*descs)[i];
		fprintf(f, "table = %s, name = %s, ilen = %d, primary = %d\n", Name, desc->szName, (int) desc->iLen,
			(int) desc->iUnUsed[0]);
	}
}
#endif  // TEST_TABDESC

FILE *datim = NULL;

class TTest : public TGroup
{
public:
	TTest();

	char test;
};

TTest::TTest()
	: TGroup("T_TEST", NULL)
{
	Add(new TChar("F_TEST", &test));

	descs = Descs + TABLE_REQUEST_RESOLUTION;
}

#include "dumpsubj.cpp"

#if TEST_SNPRINTF
extern "C" int snprintf(char *s, size_t n, const char *format, ...);
#endif  // TEST_SNPRINTF

#if TEST_OTHER_INVOLV
static void common_involvements(const char *kinds, char *commonOtherInvl)
{
	Kind->Seek(*kinds);
	strcpy(commonOtherInvl, Kind->others);

	for (const char *s = kinds + 1; *s; s++)
	{
		Kind->Seek(*s);

		for (char *p = commonOtherInvl; *p; )
		{
			if (strchr(Kind->others, *p))
				p++;
			else
				strmove(p, p + 1);
		}

		if (!*commonOtherInvl)
			break;
	}
}

static void common_with_other_sess(const char *commonOtherInvl)
{
	constant otherSessInvl = "WX6RрN3ASTZCVLBDEGHMOBDПАМКчЉE†‡";
	mstr m("Общи участия фигуриращи съдо в друго заседание: ");

	for (const char *p = commonOtherInvl; *p; p++)
	{
		if (strchr(otherSessInvl, *p))
		{
			Involvement->Seek(*p);
			m.printf("%c (%d, %s)", *p, *p, Involvement->name);
		}
	}

	message("%s.", str(m));
}
#endif  // TEST_OTHER_INVOLV

#if TEST_EXEC_WAIT
class TTestExecWaitDialog : public TNTDialog
{
public:
	TTestExecWaitDialog(TWindow *parent) : TNTDialog(parent, IDD_WAIT_MORE) { }

protected:
	virtual bool EvInitDialog(HWND hWndFocus);
	virtual TResult DefWindowProc(uint msg, TParam1 p1, TParam2 p2);

	void EvShowWindow(bool show, uint status);

DECLARE_RESPONSE_TABLE(TTestExecWaitDialog);
};

DEFINE_RESPONSE_TABLE1(TTestExecWaitDialog, TNTDialog)
//{{TTestExecWaitDialogRSP_TBL_BEGIN}}
	EV_WM_SHOWWINDOW,
//{{TTestExecWaitDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

bool TTestExecWaitDialog::EvInitDialog(HWND hWndFocus)
{
	//ShowWindow(SW_HIDE);
	return TNTDialog::EvInitDialog(hWndFocus);
}

TResult TTestExecWaitDialog::DefWindowProc(uint msg, TParam1 p1, TParam2 p2)
{
	return msg == WM_SHOWWINDOW ? 0 : TNTDialog::DefWindowProc(msg, p1, p2);
}

void TTestExecWaitDialog::EvShowWindow(bool show, uint status)
{
	if (show)
		TNTDialog::EvShowWindow(show, status);
}

class TTestExecWaitThread : public TThread
{
public:
	TTestExecWaitThread(TWindow *tParent);
	virtual ~TTestExecWaitThread();

	bool IsExecuting();
	void CloseDialog();

protected:
	TWindow *parent;
	TDialog *modal;

	virtual int Run();
};

TTestExecWaitThread::TTestExecWaitThread(TWindow *tParent)
	: parent(tParent), modal(NULL)
{
}

TTestExecWaitThread::~TTestExecWaitThread()
{
	delete modal;
}

bool TTestExecWaitThread::IsExecuting()
{
	return modal && modal->IsWindowVisible();
}

void TTestExecWaitThread::CloseDialog()
{
	modal->CloseDialog(IDCANCEL);
}

int TTestExecWaitThread::Run()
{
	modal = new TTestExecWaitDialog(parent);
	return modal->Execute();
}

class TTestExecWaitWindow
{
public:
	TTestExecWaitWindow(TWindow *parent);
	~TTestExecWaitWindow();

	bool IsExecuting() { return thread->IsExecuting(); }

protected:
	TTestExecWaitThread *thread;
};

TTestExecWaitWindow::TTestExecWaitWindow(TWindow *parent)
{
	thread = new TTestExecWaitThread(parent);
	thread->Start();
}

TTestExecWaitWindow::~TTestExecWaitWindow()
{
	thread->CloseDialog();
	thread->WaitForExit();
	delete thread;
}

void test_wait_1(TWindow *parent)
{
	const SLEEP = 5000;
	const STEP = 70;
	//DWORD ticks = GetTickCount();

	{
		TDialog shadow(parent, IDD_WAIT);
		shadow.Create();

		//TTestExecWaitWindow wait(parent);

		for (int i = 0; i < SLEEP / STEP; i++)
		{
			//if (wait.IsExecuting())
			//	shadow.ShowWindow(SW_HIDE);

			Sleep(STEP);

			MSG msg;

			while (::PeekMessage(&msg, shadow.HWindow, 0, 0, PM_REMOVE))
				//if (!::IsDialogMessage(shadow.HWindow, &msg))
					GetApplicationObject()->ProcessMsg(msg);
		}
	}

	//message("%lu ticks", GetTickCount() - ticks);
}

void test_wait(TWindow *parent)
{
	const SLEEP = 5000;
	const STEP = 70;
	TWaitWindow wait(parent, WAIT);

	for (int i = 0; i < SLEEP / STEP; i++)
	{
		Sleep(STEP);

		if (i % 14 == 0)
		{
			mstr m;

			m.printf("Приблизително %d секунди...");
			wait.SetText(str(m));
		}
	}
}
#endif  // TEST_EXEC_WAIT

#if TEST_COMMON_FIELDS
class TTestGroup : public TLawsuit
{
public:
	void Test(const TGroup &that);
};

void TTestGroup::Test(const TGroup &that)
{
	for (int i = 0; i < Count(); i++)
	{
		TField *source = (*this)[i];
		TField *target = that.Find(source->Name);

		if (target)
			message("%s <=> %s: %s", Name, that.Name, target->Name);
	}
}

#endif  // TEST_COMMON_FIELDS

#if TEST_LOG_MD5SUM
static void CalcMD5(const TLog &log, long *tResBlock)
{
	char buffer[128];
	char *ptr = buffer + 8;
	char *s;

	for (s = buffer; s < buffer + sizeof buffer; s += sizeof(int))
		*(int *) s = rand();

	long pKey[2] = { log.key.no, (log.key.year << 11) + log.key.type };
	memcpy(ptr, pKey, sizeof pKey);
	ptr += sizeof pKey;

	char pStamp[6] = { (char) (log.stamp.year - YEAR_MIN), (char) log.stamp.month, (char) log.stamp.day,
		(char) log.stamp.hour, (char) log.stamp.minute, (char) log.stamp.second };
	memcpy(ptr, pStamp, sizeof pStamp);
	ptr += sizeof pStamp;

	*(int *) ptr = atoi(log.ucn);
	ptr += sizeof(int);

	memcpy(ptr, &log.gop, sizeof log.gop);
	ptr += sizeof log.gop;

	memcpy(ptr, &log.log, sizeof log.log);
	ptr += sizeof log.log;

	memcpy(ptr, &log.opc, sizeof log.opc);
	ptr += sizeof log.opc;

	char pDate[3] = { (char) (log.date.year - YEAR_MIN), (char) log.date.month, (char) log.date.year };
	memcpy(ptr, pDate, sizeof pDate);
	ptr += sizeof pDate;

	strcpy(ptr, log.text);
	ptr += strlen(log.text);

	unsigned seed = 0;

	for (s = buffer + 8; s < ptr; s++)
		seed ^= *s << (8 * ((s - buffer) & 0x03));

	srand(seed);
	*(int *) buffer = rand();
	*(int *) (buffer + 4) = rand();

	while (s < buffer + sizeof buffer)
		*s++ = (char) (rand() & 0xFF);

	calc_md5sum(buffer, sizeof buffer, (char *) tResBlock);

	write_all_bytes("c:\\tmp\\__cac_md5_hash", (const char *) tResBlock, 16);
}
#endif  // TEST_LOG_MD5SUM

void testfunc(TWindow *parent)
{
	(void) parent;	// suppress -wpar

#if DUMP_INVOLVEMENTS
	{
		mstr m;
		m.printf("__invl_%c.txt", COURT_LETTER);
		FILE *f = fopen(str(m), "w");

		list_kinds(f, "Входящи документи", KIND_INDOCS);
		list_kinds(f, "Виртуални документи", KIND_VIRTUALS);
		list_kinds(f, "Видове дела", KIND_LAWSUITS);
		list_kinds(f, "Заседания", KIND_VISIBLE_SESSIONS);

		fclose(f);
	}
#endif  // DUMP_INVOLVEMENTS

#if TEST_DATE_FUNC
	test_date_func();
#endif  // TEST_DATES

#if TEST_DATE_TIME
	{
		TSession session;

		while (get_date(parent, &session, NULL, true, true))
			message("%D = %ld", &session.date, session.date.Value());
	}
#endif  // TEST_DATE_TIME

#if TEST_CTYPE
	{
		FILE *f = fopen("__ctype", "w");

		fputs("\talnum\talpha\tascii\tcntrl\tdigit\tgraph\tlower\tprint"
			"\tpunct\tspace\tupper\txdigit\ttoupp\ttolow\tlocal\n\n", f);
		for (int c = EOF; c <= CHAR_MAX; c++)
		{
			fprintf(f, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
				c, isalnum(c) != 0, isalpha(c) != 0, isascii(c) != 0, iscntrl(c) != 0, isdigit(c) != 0, isgraph(c) != 0,
				islower(c) != 0, isprint(c) != 0, ispunct(c) != 0, isspace(c) != 0, isupper(c) != 0, isxdigit(c) != 0,
				c == EOF ? EOF : toupper(c), c == EOF ? EOF : tolower(c), islocal(c) != 0);
		}

		fputs("\n_ctype[]\n", f);
		for (int c = EOF; c <= CHAR_MAX; c++)
		{
			unsigned char u = 0;

			if (iscntrl(c))
				u |= _IS_CTL;
			if (isdigit(c))
				u |= _IS_DIG;
			if (islower(c))
				u |= _IS_LOW;
			if (ispunct(c))
				u |= _IS_PUN;
			if (isspace(c))
				u |= _IS_SP;
			if (isupper(c))
				u |= _IS_UPP;
			if (isxdigit(c))
				u |= _IS_HEX;
			if (isprint(c) && !(isalpha(c) || isdigit(c) || ispunct(c)))
				u |= _IS_BLK;

			fprintf(f, "0x%02X,%c", u, (c & 0x0F) == 0x0F ? '\n' : ' ');
		}

		fputs("\n_lower[]\n", f);
		for (int c = 0; c <= CHAR_MAX; c++)
			fprintf(f, "0x%02X,%c", tolower(c), (c & 0x0F) == 0x0F ? '\n' : ' ');

		fputs("\n_upper[]\n", f);
		for (int c = 0; c <= CHAR_MAX; c++)
			fprintf(f, "0x%02X,%c", toupper(c), (c & 0x0F) == 0x0F ? '\n' : ' ');

		char buffer[0x100];
		for (int c = 0; c < sizeof buffer; c++)
			buffer[c] = (char) (c + 1);

		fputs("\nstrlwr()\n", f);
		strlwr(buffer);
		for (int c = 0; c < sizeof buffer; c++)
			fprintf(f, "0x%02X,%c", buffer[c], (c & 0x0F) == 0x0F ? '\n' : ' ');

		fputs("\nstrupr()\n", f);
		strupr(buffer);
		for (int c = 0; c < sizeof buffer; c++)
			fprintf(f, "0x%02X,%c", buffer[c], (c & 0x0F) == 0x0F ? '\n' : ' ');

		fclose(f);
	}
#endif  // TEST_CTYPE

#if TEST_CATEGORY
	{
		TDummyContainer container;
		FILE *f = fopen("__categ", "w");

		TQuery q("SELECT F_INVOLVEMENT, F_ORDER, F_NAME FROM T_INVOLVEMENT ORDER BY F_ORDER;");
		while (q.Read())
		{
			*Involvement << q;
			// 2015:194 note: AppC requires kind for W
			bool other = container.Other('?', Involvement->involvement);

			if (other != (Involvement->Order() >= 300))
			{
				fprintf(f, "'%c' \"%s\" other=%s, order=%ld\n", Involvement->involvement, Involvement->name,
					other ? "true" : "false", Involvement->Order());
			}
		}

		fclose(f);
	}
#endif  // TEST_CATEGORY

#if TEST_LINKS
	{
		TLinkWind linkWind;

		linkWind.key.no = 1;
		linkWind.key.year = 2011;
		linkWind.key.type = 'p';
		linkWind.kind = 'Щ';
		linkWind.date = Today;

		strcpy(linkWind.ucn, "7106051022");
		linkWind.ucnType = UCN_CITIZEN_UCN;
		linkWind.involvement = 'B';

		strcpy(linkWind.proxyUcn, "7112101077");
		linkWind.proxyUcnType = UCN_CITIZEN_UCN;
		linkWind.proxyInvolvement = 'd';
		linkWind.proxyKind = SIDE_MARK_INDIRECT_AS_ANY_1;

		strcpy(linkWind.redirectUcn, "7109111104");
		linkWind.redirectUcnType = UCN_CITIZEN_UCN;
		linkWind.redirectInvolvement = 'L';
		linkWind.redirectKind = SIDE_MARK_LIKE_LAWYER;

		char buffer[0x400];
		side_link_to_string(buffer, sizeof buffer, linkWind, false, false, NULL);

		mstr m(buffer);

		m.cpy("text");
		print_side_links(m, &linkWind, true, false, NULL);
	}
#endif  // TEST LINKS

#if TEST_APPEAL
	dump_appeal_instances();
#endif  // TEST_APPEAL

#if TEST_OUTCOMES
	{
		TSession session;
		msql m(session.Name);

		session.key.no = 1129;
		session.key.year = 2016;
		session.key.type = 't';
		m.Add(&session, "F_NO, F_YEAR, F_TYPE");

		TQuery q(str(m));
		if (q.Read())
		{
			session << q;
			session.result = *RESULT_HOLDS;
			message("category = %d", session_outcomes(&session));
		}
		else
			error("read session for %ld/%ld%c", session.key.no, session.key.year, session.key.type);
	}
#endif  // TEST_OUTCOMES

#if TEST_FINDWORD
	{
		const char *str = "THE QUICK BROWN 123TIMES";

		message("find THE Q = %d", find_words("THE Q\0", str, 1, 0));
		message("find 123 = %d", find_words("123\0", str, 1, 0));
		message("find 123T = %d", find_words("123T\0", str, 1, 0));
	}
#endif  // TEST_FINDWORD

#if TEST_SUBJSBES
	{
		msql m(Subject->Name);
		TIntegerArray lset;

		m.AddChar("F_TYPE", 'p');
		TQuery q(str(m));
		while (q.Read())
		{
			*Subject << q;
			lset.Clear();

			if (any(Subject->sbes) && !strvalue_to_lset(Subject->sbes, &lset, NULL))
				error("wrong sbes in subject %ld", Subject->subject);
		}
	}
#endif  // TEST_SUBJSBES

#if TEST_GTTIME
	{
		CDateTime stamp;
		message("TodayNow = %d", (int) stamp.IBaseTodayNow());
	}
#endif  // TEST_GTTIME

#if TEST_TABDESC
	{

		FILE *f = fopen("__descs", "w");

		TTestTable(TABLE_ACCOMPLY, "TABLE_ACCOMPLY").Dump(f);
		fclose(f);
	}
#endif  // TEST_TABDESC

#if TEST_QUOT
	{
		msql m("boza");
		m.AddString("F_NAME", "test'test");
		m.AddLike("F_NAME", "test'test", true);
		message(str(m));
	}
#endif  // TEST_QUOT

#if TEST_BDERROR
	TCACDataBase::DefaultDB->Execute("SELECT X FROM Y", ET_ERROR);
#endif  // TEST_BDERROR

#if TEST_POSTERM
	{
		char transId[SIZE_OF_TRANS_ID];
		char authId[SIZE_OF_PAYMENT_AUTH_ID];
		char cardId[SIZE_OF_PAYMENT_CARD_ID];

		if (TPosTerm::Approval(parent, POSTERM_BUDGET))
			if (TPosTerm::Payment(parent, POSTERM_BUDGET, 0.10, transId, authId, cardId))
				message("PosTerm::OK");
	}
#endif  // TEST_POSTERM

#if TEST_WAIT
	{
		//TWaitWindow wait(parent, "Комуникация с ПОС терминала...", IDD_WAIT_MORE_HIDDEN);
		Sleep(3000);
		TWaitWindow wait(parent, "Комуникация с ПОС терминала...");

		for (int i = 0; i < 300 && !wait.Canceled(); i++)
		{
			if (i == 15)
			{
				//wait.ShowWindow(SW_SHOWNOACTIVATE);
				wait.AllowCancel(true);
			}

			Sleep(100);
		}

		if (wait.Canceled())
			message("прекъсване");
	}
#endif  // TEST_WAIT

#if TEST_RANDOM_ACT
	{
		extern void random_act(TWindow *parent);
		random_act(parent);
	}
#endif  // TEST_RANDOM_ACT

#if TEST_MPRINT
	{
		FILE *f = fopen("__test", "w");
		TStreetMap smap;
		msql m(smap.Name);

		DWORD ticks = GetTickCount();
		strcpy(smap.ucn, "8005121062");
		smap.uclp = 10135;
		smap.street = -16;

		if (smap.Get())
		{
			smap.Print(m);
			fprintf(f, "%s\n", str(m));
		}

		fprintf(f, "%lu seconds\n", (GetTickCount() - ticks) / 1000);
		fclose(f);
	}
#endif  // TEST_MPRINT

#if TEST_CRITERIA
	{
		TSession session;
		TSideWind sideWind;

		session.key.no = 1;
		session.key.year = 2012;
		session.key.type = 'a';
		session.kind = 'И';
		session.date = CDate(10, 6, 2012);
		sideWind << session;
		strcpy(sideWind.ucn, "7106051022");
		sideWind.ucnType = UCN_CITIZEN_UCN;
		sideWind.involvement = 'm';
		kill_forward_sides(&session, &sideWind);
	}
#endif  // TEST_CRITERIA

#if TEST_TEXT
	{
		TSession session;
		TSurround surround;

		strcpy(surround.text, "text");
		session << surround;
		message("%s", str(session.text));

		TDecision decision;
		msql m;

		decision.key.no = 1;
		decision.key.year = 2010;
		decision.key.type = 'a';
		decision.FloatCriteria(m);

		TQuery q(str(m));
		if (q.Read())
		{
			decision << q;
			message("%s", str(decision.text));
		}
	}
#endif  // TEST_TEXT

#if TEST_POWERSH
	{
		static constant UPDATE_DIRECTORY = "c:\\tmp\\update";
		static constant POWERSHELL = "powershell.exe";
		mstr m;
		constant tMyDir = "C:\\TMP";
		constant start = "/start:3";

		if (chdir(UPDATE_DIRECTORY))
			fatal("%s: грешка %d при смяна на текущата директория.", UPDATE_DIRECTORY, errno);

		m.printf("try { .\\lmupdate2.ps1 %s %s %s } "
			"catch { Write-Host $_.exception -foregroundcolor 'Red'; "
			"Write-Host -NoNewline 'Press ENTER to continue...'; "
			"$x = Read-Host }", tMyDir, start, "");
		execlp(POWERSHELL, "PowerShell", "-Command", str(m), NULL);
		//execlp(POWERSHELL, "PowerShell", ".\\lmupdate.ps1", tMyDir, start, NULL, NULL);
		fatal("%s: грешка %d при стартиране.", POWERSHELL, errno);
	}
#endif  // TEST_POWERSH

#if TEST_PERSONAL
	{
		DWORD ticks = GetTickCount();
		TDecision decision;
		// <= 4500 to avoid BDE overflow in PC (ODBC is OK)
		TQuery q("select * from t_decision where f_year in (2008, 2009) and f_no <= 4500 and not f_text is null");
		//TQuery q("select * from t_decision where f_year in (2010, 2011) and not f_text is null");
		//TQuery q("select * from t_decision where f_year in (2010) and f_no <= 500 and not f_text is null");
		//TQuery q("select * from t_decision where f_no = 475 and f_year = 2008 and f_type = 'a' and f_kind = 'Ш'");

	#define PERSONAL_UNICODE 1	// should match personal1.h

	#if PERSONAL_UNICODE
		FILE *p2 = fopen("__pers2.W", "w");
	#else  // PERSONAL_UNICODE
		FILE *p2 = fopen("__pers2.A", "w");
	#endif  // PERSONAL_UNICODE
		while (q.Read())
		{
			decision << q;
			//TPersonal personal(&decision);	// null
			TPersonal(&decision).Filter(decision.text, 1 + (decision.key.type == 'p'));
			fprintf(p2, "%ld %ld %c ", decision.key.no, decision.key.year, decision.key.type);
			fprintf(p2, "%ld %D %c\n", decision.date.Value(), &decision.date, decision.kind);
			fprintf(p2, "%s\n\n", str(decision.text));
		}

		fclose(p2);
		message("personal finished, %lu millisec.", GetTickCount() - ticks);
	}
#endif  // TEST_PERSONAL

#if TEST_SCANDATE
	{
		struct DMY
		{
			int day, month, year;
		} d;

		extern int scan_date(const char *text, DMY *d);
		message("scan result %d", scan_date("13.04.1949", &d));
	}
#endif  // TEST_SCANDATE

#if TEST_POSTERM_LRC
	{
		char s[] = "50149399992500003897677900000000658945800000000011146FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5100787";
		char *etx = strchr(s, '');
		unsigned char lrc = '\0';

		for (const char *data = s + 1; data <= etx; data++)
			lrc ^= *data;

		if (lrc != etx[1])
			error("невалидна контролна сума");
		else
			message("валидна контролна сума");
	}
#endif  // TEST_POSTERM_LRC

#if TEST_CLOSEDS
	{
		TPeriodGroup period("T_PERIOD");

		if (TPeriodDialog(NULL, &period).Execute() == IDOK)
		{
			TTextFile tf;
			FILE *f = tf.OpenTemp();
			fprintf(f, "Вписване на закрити заседания в журналния период от %0D до %0D\n\n", &period.minDate, &period.maxDate);
			fputs("Дело           ЗЗ от дата  Върнато на  Вписано в САС\n", f);
			fputs("-------------  ----------  ----------  ----------------\n", f);

			TWaitWindow wait(NULL, WAIT);
			TLog log;
			TSession session;
			msql m(log.Name);
			int count = 0;
			CDateTime start(period.minDate.day, period.minDate.month, period.minDate.year, 0, 0, 0);
			CDateTime final(period.maxDate.day, period.maxDate.month, period.maxDate.year, 23, 59, 59);

			m.AddRange("F_STAMP", start, final);
			m.AddChar("F_GOP", LOG_ENTER);
			m.AddIsNull("F_DATE", false);
			m.AddChars("F_KIND", KIND_CLOSED_SESSIONS);
			m.AddOrder("F_STAMP");
			TQuery q(str(m));

			while (q.Read())
			{
				log << q;
				session << log;

				if (session.Try("F_RETURNED"))
				{
					fprintf(f, "%cД %5ld/%ld  %0D  %0D  %0D %0T\n", *Type->Seek(log.key.type), log.key.no,
						log.key.year, &log.date, &session.returned, &((CDate &) log.stamp), &((CTime &) log.stamp));
				}

				count++;
			}

			fprintf(f, "\n%d заседания за периода", count);
			tf.Show(&wait);
		}
	}
#endif  // TEST_CLOSEDS

#if TEST_EIS_USED
	{
		static const long test[] = { 8805, 706, 691, 1630, 1113, 723, 730, 728, 729, 722, 134, 709, 763, 979, 978, 825,
			824, 11994, 1028, 1107, 1116, 14883, 14884, 1725, 1577, 642, 641, 647, 640, 673, 674, 675, 676, 681, 685,
			688, 671, 682, 14344, 148, 692, 693, 696, 635, 2081, 0 };

		TSbeDesc desc;
		msql m(desc.Name, "F_VALUE");
		FILE *f = fopen("__eimiss", "w");

		m.AddIsNull("F_VALUE", false);
		TQuery q(str(m));
		while (q.Read())
		{
			TIntegerArray lset;

			desc << q;
			strvalue_to_lset(desc.value, &lset, "test");

			for (int i = 0; i < lset.Count(); i++)
			{
				long l = lset[i];

				m.Begin("T_EISPP_VAL", "F_NOM");
				m.AddLong("F_CODE", l);

				TQuery r(str(m));
				if (!r.Read())
				{
					m.Begin("T_EISPP_DICT", "F_NOM");
					m.AddLong("F_CODE", l);
					TQuery p(str(m));
					fprintf(f, "sbe_desc %ld: %sfound in dict\n", l, p.Read() ? "" : "not ");
				}
			}
		}

		for (int i = 0; test[i]; i++)
		{
			long l = test[i];

			m.Begin("T_EISPP_VAL", "F_NOM");
			m.AddLong("F_CODE", l);

			TQuery r(str(m));
			if (!r.Read())
			{
				m.Begin("T_EISPP_DICT", "F_NOM");
				m.AddLong("F_CODE", l);
				TQuery p(str(m));
				fprintf(f, "eisconst %ld: %sfound in dict\n", l, p.Read() ? "" : "not ");
			}
		}

		fclose(f);
	}
#endif  // TEST_EIS_USED

#if TEST_OPEN_SAVE
	{
		char name[21];
		GetSaveFileName(NULL, name, "Документи", "c:\\tmp", "DOC", sizeof name);
	}
#endif  // TEST_OPEN_SAVE

#if TEST_FILETYPE
	message(".doc -> %s, file.doc -> %s, file -> %s", file_ext(".doc"), file_ext("file.doc"), file_ext("file"));
#endif  // TEST_FILETYPE

#if TEST_MAKE_DIR
	make_directory("boza");
#endif  // TEST_MAKE_DIR

#if TEST_DISCARD_UCNS
	discard_citizens(parent);
#endif  // TEST_DISCARD_UCNS

#if TEST_LOG_TEXT
	log(0, LOG_POSTERM, "много дълъг текст за проба много дълъг текст за проба много дълъг текст за проба много дълъг");
#endif  // TEST_LOG_TEXT

#if TEST_DIALECT
	{
		static constant flags = "F_FLAGS";
		msql m("T_USER", flags);

		m.AddString("F_UCN", INTER);
		m.Add("");
		m.printf("(CAST((%s / %d) AS INTEGER)) * %d > %s", flags, FLAG_ACTIVE << 1, FLAG_ACTIVE << 1, flags);

		//TCACDataBase::DefaultDB->Execute("SET SQL DIALECT 3", ET_ERROR);
		TQuery q(str(m));
		message("dialect %d", q.Read() ? 1 : 3);
	}
#endif  // TEST_DIALECT

#if TEST_DBSTRIM
	{
		TSender sender;

		sender.ucnType = UCN_INSTANCE_SELF;
		strcpy(sender.ucn, INSTANCE_SELF);
		sender.Get("F_NAME");
		message("name = '%s'", sender.name);
	}
#endif  // TEST_ODBCSTR

#if TEST_WEEK_DAY
	message("%d", Today.DayOfWeek());
#endif  // TEST_WEEK_DAY

#if TEST_GOP_LOAD
	{
		TWaitWindow wait(NULL, WAIT);
		TLawsuit lawsuit;
		DWORD ticks = GetTickCount();

		lawsuit.key.no = 2469;
		lawsuit.key.year = 2011;
		lawsuit.key.type = 't';

		if (lawsuit.Get())
		{
			query_group(parent, &lawsuit, true);
			message("ticks = %lu", (GetTickCount() - ticks) / 1000);
		}
	}
#endif  // TEST_GOP_LOAD

#if TEST_FIRM_PRINT
	{
		TSideWind sideWind;
		mstr m;

		sideWind.ucnType = UCN_FIRM_CODE;
		strcpy(sideWind.ucn, "0000036575");
		print_side_wind(m, &sideWind, true);
		m.cat(" | ");
		strcpy(sideWind.ucn, "0000036576");
		print_side_wind(m, &sideWind, true);
		m.cat(" | ");
		strcpy(sideWind.ucn, "0000036577");
		print_side_wind(m, &sideWind, true);
		message("filtered = %s", str(m));
	}
#endif  // TEST_FIRM_PRINT

#if TEST_LOAD_HALL
	load_sessions(parent);
#endif  // TEST_LOAD_HALL

#if TEST_SYS_COLOR
	{
		FILE *f = fopen("__scol", "w");

		for (int i = 0; i < 30; i++)
			fprintf(f, "%d = %06lx\n", i, GetSysColor(i));

		fclose(f);
	}
#endif  // TEST_SYS_COLOR

#if TEST_VAR_FLAGS
	{
		msql m("T_TEST");

		//m.AddFlags("F_FLAGS", SESSION_CLOSEDD | SESSION_PRIVATE, SESSION_PRIVATE, SESSION_ALL);
		m.AddFlags("F_FLAGS", SUBJECT_EDITED, SUBJECT_EDITED, 0x0F);
		message("condition = %s", str(m));
		//m.Add("F_FLAGS");
		//TCACDataBase::DefaultDB->CondActive(m, "F_FLAGS");
	}
#endif  // TEST_VAR_FLAGS

#if TEST_SINDICK
	{
		TLawsuit lawsuit;

		lawsuit.key.no = 1;
		lawsuit.key.year = 2013;
		lawsuit.key.type = 't';

		if (lawsuit.Get())
			message("э = %c, ь = %c", lawsuit.Category('э'), lawsuit.Category('ь'));
	}
#endif  // TEST_SINDICK

#if TEST_MESSAGE
	{
		message("carriage\rreturn");
		message("line\nfeed");
		message("carriage return\r\nline feed");
	}
#endif  // TEST_MESSAGE

#if TEST_REGISTRY
	{
		long result;
		HKEY hkey;
		DWORD disposition;

		result = RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\PowerShell\\2\\BoZZa", 0, NULL,
			REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hkey, &disposition);
		message("create = %ld", result);

		result = RegSetValueEx(hkey, "Whatever", 0, REG_SZ, (const BYTE *) "test-test", 10);
		message("setvalue = %ld", result);

		message("close = %d", CloseHandle(hkey));
	}
#endif  // TEST_REGISTRY

#if TEST_ODBCURS
	{
		constant STATEMENT_1 = "SELECT F_ORDER FROM T_TYPE WHERE F_TYPE = 'r'";
		constant STATEMENT_2 = "SELECT F_ORDER FROM T_RESOLUTION WHERE F_RESOLUTION = 'h'";
		TODBCDataBase *dataBase = (TODBCDataBase *) TCACDataBase::DefaultDB;
		SQLHSTMT hstmt = dataBase->AllocStmt();
		long order = 0;
		SQLINTEGER unused;

		if (SQLExecDirect(hstmt, (SQLCHAR *) STATEMENT_1, SQL_NTS) != SQL_SUCCESS) fatal("exec");
		//if (SQLBindCol(hstmt, 1, SQL_INTEGER, &order, sizeof(long), &unused) != SQL_SUCCESS) fatal("bind");
		if (SQLFetch(hstmt) != SQL_SUCCESS) fatal("fetch");
		if (SQLGetData(hstmt, 1, SQL_INTEGER, &order, sizeof(long), &unused) != SQL_SUCCESS) fatal("getdata");
		if (order != 10) fatal("order");

		if (SQLCloseCursor(hstmt) != SQL_SUCCESS) fatal("close");
		if (SQLExecDirect(hstmt, (SQLCHAR *) STATEMENT_2, SQL_NTS) != SQL_SUCCESS) fatal("exec");
		if (SQLFetch(hstmt) != SQL_SUCCESS) fatal("re-fetch");
		if (order == 1) fatal("re-order");

		dataBase->FreeStmt(hstmt);
	}
#endif  // TEST_ODBCURS
#if TEST_ODBCLOSE
	{
		constant STATEMENT_1 = "UPDATE T_TYPE SET F_NAME = 'В' || F_NAME WHERE F_TYPE = 'P'";
		constant STATEMENT_2 = "EXECUTE PROCEDURE DELETE_GROUP_DATA(99999, 2014, 'p')";
		TODBCDataBase *dataBase = (TODBCDataBase *) TCACDataBase::DefaultDB;
		SQLHSTMT hstmt;
		SQLRETURN result;

		hstmt = dataBase->AllocStmt();
		result = SQLCloseCursor(hstmt);
		message("return = %d", (int) result);
		AppName->FreeStmt(hstmt);

		hstmt = AppName->AllocStmt();
		if (SQLExecDirect(hstmt, (SQLCHAR *) STATEMENT_1, SQL_NTS) != SQL_SUCCESS) fatal("increment");
		result = SQLCloseCursor(hstmt);
		message("return = %d", (int) result);
		AppName->FreeStmt(hstmt);

		hstmt = AppName->AllocStmt();
		if (SQLExecDirect(hstmt, (SQLCHAR *) STATEMENT_2, SQL_NTS) != SQL_SUCCESS) fatal("execute");
		result = SQLCloseCursor(hstmt);
		message("return = %d", (int) result);
		dataBase->FreeStmt(hstmt);
	}
#endif  // TEST_ODBCLOSE

#if TEST_DOUBLEXC
	{
		class MyClass : public TQuery
		{
		public:
			MyClass(const char *expr, TDataBase *dataBase = NULL) :
				TQuery(expr, dataBase) { fatal("start"); }
			~MyClass() { fatal("final"); }
		};

		MyClass q("SELECT * FROM T_ACCUSATION");
		q.Read();
	}
#endif  // TEST_DOUBLEXC

#if TEST_STR2MSTR
	{
		DISTRIB_COUNT_COMPLEX = 1;
		DISTRIB_LOAD_BALANCE = 255;
		Default->CalcLoad(1);

		DISTRIB_COUNT_COMPLEX = 0;
		DISTRIB_LOAD_BALANCE = 0;
		Default->CalcLoad(0);
	}
#endif  // TEST_STR2MSTR

#if TEST_PROCID
	{
		for (int i = 0; i < 5; i++)
			message("%lu", GetWindowThreadProcessId((HWND) (10000 + random(10000)), NULL));
	}
#endif  // TEST_PROCID

#if TEST_SUMWORDS
	{
		FILE *f = fopen("__sumw", "w");
		mstr m;

		srand(GetTickCount());

		for (int i = 0; i < 900000; i++)
		{
			double d = labs(rand());

			for (int n = 0; n < (i & 3); n++)
				d /= 10;

			if (i < 2)
				d = i;

			m.clear();
			sum_in_words(d, m);
			fprintf(f, "%.6lf -> %.2lf -> %s\n", d, d, str(m));

		}

		fclose(f);
	}
#endif  // TEST_SUMWORDS

#if TEST_UROUND
	{
		message(" 23.5 -> %lf \n 24.5 -> %lf \n 28.5 -> %lf \n 29.5 -> %lf", uround(23.5), uround(24.5), uround(28.5),
			uround(29.5));
	}
#endif  // TEST_UROUND

#if TEST_SYNCDATE
	{
		CDateTime::SyncServer(SYNC_EXACT);
		datim = fopen("__datim", "w");
		srand(GetTickCount());

		for (int i = 0; i < 120; i++)
		{
			Sleep(rand() % 2200);
			CDateTime::SyncServer(SYNC_PRECISE);
		}

		fclose(datim);
		datim = NULL;
	}
#endif  // TEST_SYNCDATE

#if TEST_DOTTED
	{
		TCitizen citizen;
		msql m(citizen.Name);

		strcpy(citizen.ucn, "7106051022");
		citizen.ucnType = UCN_CITIZEN_UCN;
		citizen.Get();
		m.Add(&citizen, "C.F_NAME, C.F_FAMILY, C.F_RENAME, C.F_REFAMILY");

		m.Begin(citizen.Name);
		m.AddString("C.F_NAME", citizen.name);
		m.AddString("C.F_FAMILY", citizen.family);
		m.AddString("C.F_RENAME", citizen.reName);
		m.AddString("C.F_REFAMILY", citizen.reFamily);
	}
#endif  // TEST_DOTTED

#if TEST_MENU_FONT
	{
		HMENU hMenu = parent->GetMenu();
		message("%lx", ::SendMessage((HWND) hMenu, WM_GETFONT, 0, 0));
	}
#endif  // TEST_MENU_FONT

#if TEST_SHORT_NAME
	{
		char buffer[0x100] = "";
		int result = GetShortPathName("\\\\dc1.var.is-bg.net\\Shares\\Groups\\SAS\\FolderA1.boza\\FolderA2.boza\\"
			"FolderA3.boza\\FolderA4.boza\\FolderA5.boza\\FolderA6.boza\\FolderA7.boza\\FolderA8.boza\\"
			"FolderA9.boza\\FileA10.boza", buffer, sizeof buffer);

		message("result = %d, last error = %d, s = %s", result, (int) GetLastError(), buffer);
	}
#endif  // TEST_SHORT_NAME

#if TEST_FAILURE
	TCACDataBase::DefaultDB->Execute("BOZA", ET_FATAL);
#endif  // TEST_FAILURE

#if TEST_PLAIN
	electric_action("c:\\tmp\\screen.c", TYPE_PLAIN, ACTION_EDIT);
#endif  // TEST_PLAIN

#if TEST_NIA_PROT
	//nia_protocol(parent);
	nia_feedback(parent);
#endif  // TEST_NIA_PROT

#if TEST_BARCODE
	mstr m;
	const half = 36 * 36 * 36 * 18;
	const full = half * 2;
	long court, requestId;

	for (int asterisk = false; asterisk <= true; asterisk++)
	{
		m.clear();
		barcode_generate(m, half, asterisk, 310);
		barcode_scan(str(m), &court, &requestId, NULL);
		if (court != 310 || requestId != half)
			fatal("failed medium");

		m.clear();
		barcode_generate(m, full, asterisk, 310);
		barcode_scan(str(m), &court, &requestId, NULL);
		if (court != 310 || requestId != full)
			fatal("failed full");

		m.clear();
		barcode_generate(m, INT_MAX, asterisk, 728);
		barcode_scan(str(m), &court, &requestId, NULL);
		if (court != 728 || requestId != INT_MAX)
			fatal("failed maximal");
	}
#endif  // TEST_BARCODE

#if TEST_TRANSLIT
	{
		constant source = "Ябълка";
		char target[20];

		transliterate(source, target, sizeof target);
		message("%s -> %s", source, target);
	}
#endif  // TEST_TRANSLIT

#if TEST_IBCTABLE
	{
		TTest test;

		for (int i = 0; i < 0x100; i++)
		{
			test.test = (char) i;

			if (!test.Insert())
				message("failed for %d", i);
		}
	}
#endif  // TEST_IBCTABLE

#if TEST_ADD_ARRAY
	{
		static constant condition[5] =
		{
			"SELECT * FROM T_TYPE WHERE F_TYPE IS NULL",
			"SELECT * FROM T_TYPE WHERE F_TYPE = 'c'",
			"SELECT * FROM T_TYPE WHERE F_TYPE IN ('c', 'p')",
			"SELECT * FROM T_TYPE WHERE (F_TYPE IS NULL OR F_TYPE = 'c')",
			"SELECT * FROM T_TYPE WHERE (F_TYPE IS NULL OR F_TYPE IN ('c', 'p'))"
		};

		for (int i = 2; i < 5; i++)
		{
			TGroupArray array;
			msql m(Type->Name);
			TType *type;

			if (i != 1 && i != 2)
			{
				type = new TType;
				type->type = '\0';
				array.Add(type);
			}

			if (i)
			{
				type = new TType;
				type->type = 'c';
				array.Add(type);
			}

			if (i == 2 || i == 4)
			{
				type = new TType;
				type->type = 'p';
				array.Add(type);
			}

			m.AddArray("F_TYPE", array);

			if (m.cmp(condition[i]))
				message("failed on %d: cond %s vs. mstr %s", i, condition[i], str(m));
		}
	}
#endif  // TEST_ADD_ARRAY

#if TEST_BSTRING
	{
		TBSTR testA("text");

		message("bstrA lpref = %ld", *(long *) (testA.bstr() - 2));
		MessageBoxW(NULL, testA.bstr(), L"Title", MB_OK);

		TBSTR testW(L"text text", 4);
		message("bstrW lpref = %ld", *(long *) (testA.bstr() - 2));
		MessageBoxW(NULL, testW.bstr(), L"Title", MB_OK);
	}
#endif  // TEST_BSTRING

#if TEST_SHOWHTML
	{
		static constant text =
			"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\""
			"\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"
			"<html xmlns=\"http://www.w3.org/1999/xhtml\" >"
			"<head>"
			"<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />"
			"</head>"
			"<body>"
			"<p>test1</p>"
			"</body>"
			"</html>";

		//show_html(NULL, "C:\\TMP\\debright\\test1.htm", false);
		show_html(NULL, strnew(text));
	}
#endif  // SHOW_HTML

#if TEST_PRINT_MISS
	{
		mstr m;

		print_missing(m, '?', "1234567890");
		message("%s", str(m));
		m.clear();

		print_missing(m, 'c', "7106051022");
		message("%s", str(m));
	}
#endif  // TEST_PRINT_MISS

#if TEST_NEWDATABASE
	{
		TODBCDataBase db("CAC_REGCOURT");
		db.Connect("", ET_FATAL);

		//TBDEDataBase db("SAREAL");
		//db.Connect("????????", ET_FATAL);

		//TQuery q("SELECT F_TEXT FROM T_DECISION WHERE F_NO = 1 AND F_YEAR = 2014", &db);
		TQuery q("SELECT F_TEXT FROM T_DECISION D JOIN T_SESSION S ON S.F_NO = D.F_NO "
			"AND S.F_YEAR = D.F_YEAR AND S.F_TYPE = D.F_TYPE AND S.F_DATE = D.F_DATE "
			"AND S.F_KIND = D.F_SESSION_KIND WHERE F_NO = 1 AND F_YEAR = 2014", &db);
		mstr m;

		while (q.Read())
		{
			q.GetText(q.Find("F_TEXT"), &m);
			message("text = %s", str(m));
		}

		//db.Execute("INSERT INTO T_TYPE VALUES ('Я', 0010, 'Входящ документ', 'Вх.док.')", ET_WRITE, "T_TYPE", "добавяне");
		//db.Execute("INSERT INTO T_TYPE (F_TYPE) VALUES (NULL)", ET_ERROR, "T_TYPE", "добавяне");
		//db.Execute("DELETE FROM T_TYPE WHERE F_TYPE IS NULL", ET_DELETE, "T_TYPE", "изтриване");

		TType type;
		type.type = 'Я';
		strcpy(type.name, "Входящ документ");
		strcpy(type.alias, "Вх.док.");
		type.Update();
	}
#endif  // TEST_NEWDATABASE

#if TEST_VSS_INDEXES
	{
		TGSArray<TVSSIndexSet> indexSets;

		VSSIndexSet->LoadData(&indexSets, NULL, FLAG_NULL);

		if (indexSets.Count())
		{
			for (int index = 0; index < indexSets.Count(); index++)
			{
				TVSSIndexSet *indexSet = indexSets[index];
				TGArray<TVSSIndex> vssIndexes;
				mstr m;

				indexSet->AddIndexes(vssIndexes);

				for (int n = 0; n < vssIndexes.Count(); n++)
				{
					m.sep(" ");
					m.cat(vssIndexes[n]->index);
				}

				for (char *s = indexSet->indexes; *s; )
				{
					if (*s == ',')
						strmove(s, s + 1);
					else
						s++;
				}

				if (strcmp(str(m), indexSet->indexes))
					error("orig = %s\nconv = %s", indexSets[0]->indexes, str(m));
			}
		}
		else
			error("no vss index sets");

		TIntegerArray lset;

		strvalue_to_lset("1,   2   3 4 5", &lset, "test");
		message("lset count = %ld", lset.Count());
	}
#endif  // TEST_VSS_INDEXES

#if TEST_CREATE_DAMN
	TDamn damn;

	strcpy(damn.name, "ЕТ");
	strcpy(damn.ucn, "");
	damn.ucnType = 'D';
	damn.companyType = 'e';
	damn.key.no = 1;
	damn.key.year = 2015;
	damn.key.type = 'p';
	damn.Insert();
	damn.Delete(true);
#endif  // TEST_CREATE_DAMN

#if TEST_EIS_SBES
	msql m(SbeDesc->Name);
	TIntegerArray dlosigs;
	TIntegerArray sprvids;
	mstr sbehits;
	mstr sbemiss;

	m.AddString("F_NAME", "NPR.DLO.dlosig");
	TQuery q(str(m));

	while (q.Read())
	{
		*SbeDesc << q;
		dlosigs.Clear();

		if (any(SbeDesc->value))
			strvalue_to_lset(SbeDesc->value, &dlosigs, "dlosig");

		if (SbeDesc->vid == 865)
			MessageBeep(-1);

		if (!dlosigs.Count() || dlosigs.Find(1008) != INT_MAX)
		{
			strcpy(SbeDesc->name, "NPR.DLO.dlosprvid");
			sprvids.Clear();

			if (SbeDesc->Try() && any(SbeDesc->value))
				strvalue_to_lset(SbeDesc->value, &sprvids, "dlosprvid");

			mstr &sbes = !sprvids.Count() || sprvids.Find(1025) != INT_MAX ? sbehits : sbemiss;

			sbes.sep(", ");
			itom(SbeDesc->vid, sbes);
		}
	}

	message("hits = %s\n\n miss=%s", str(sbehits), str(sbemiss));
#endif  // TEST_EIS_SBES

#if TEST_DBI_TEMP
	SESInfo sesInfo;

	if (TBDEDataBase::CheckDI("ses info", "ses info", DbiGetSesInfo(&sesInfo), ET_ERROR))
		info("NetDir = %s\nPrivDir=%s", sesInfo.szNetDir, sesInfo.szPrivDir);
#endif  // TEST_DBI_TEMP

#if TEST_UP_LOW
	char test = 'љ';
	message("char %c, upper = %d, lower = %d", test, isupper(test), islower(test));
#endif  // TEST_UP_LOW

#if TEST_HTM_NAMES
	{
		TStringArray htm_names;
		WIN32_FIND_DATA find;
		HANDLE hFind = FindFirstFile("htm\\*.htm", &find);

		if (hFind == INVALID_HANDLE_VALUE)
			fatal("FindFirst error %lu", GETLASTERROR);

		do
		{
			mstr m(find.cFileName);
			m.lower();

			if (m.index("_header.") == INT_MAX)
			{
				m.cut(m.chr('.'));
				htm_names.Add(m.newstr());
			}
		} while (FindNextFile(hFind, &find));

		unsigned long dwError = GETLASTERROR;

		if (dwError != ERROR_NO_MORE_FILES)
			fatal("FindNextFile error %lu", dwError);

		if (!FindClose(hFind))
			fatal("FindClose error %lu", GETLASTERROR);

		for (int i = 0; i < htm_names.Count(); i++)
		{
			const char *s = htm_names[i];
			size_t slen = strlen(s);

			for (int n = 0; n < htm_names.Count(); n++)
			{
				const char *t = htm_names[n];

				if (!strncmp(s, t, slen) && t[slen] == '_' && t[slen + 1])
					message("%s vs. %s", s, t);
			}
		}
	}
#endif  // TEST_HTM_NAMES

#if TEST_SUBPOENA_NO
	{
		TSubpoena subpoena;
		msql m(subpoena.Name, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_UCN, F_UCN_TYPE, F_INVOLVEMENT, F_EVENT_STATUS, "
			"F_SUBPOENA_NO");
		TQuery q(str(m));
		TWaitWindow wait(parent, WAIT);
		int count = 0;

		while (q.Read())
		{
			subpoena << q;

			m.Begin(subpoena.Name, "F_SUBPOENA_NO");
			m.Add(&subpoena, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_UCN, F_UCN_TYPE, F_INVOLVEMENT, F_SUBPOENA_NO");
			m.AddChar("NOT F_EVENT_STATUS", subpoena.eventStatus);

			TQuery r(str(m));
			if (r.Read())
				fatal("Дублиране!");

			wait.TickCount(++count, "проверени призовки...");
		}
	}
#endif  // TEST_SUBPOENA_NO

#if TEST_EXPORT_CSV
#if COURT_TYPE == COURT_ADMIN
	if (!any(EXPORT_CSV_PATH) && !strcmp(Default->login, "DZHEKOV"))
		strcpy(EXPORT_CSV_PATH, "C:\\TMP\\incom\\IBS\\csv");
	export_csv(parent);
#endif  // ADMIN
#endif  // TEST_EXPORT_CSV

#if TEST_FINALNONENT
	for (const char *s = RESULT_FINALIZEDS; *s; s++)
		if (strchr(RESULT_FINALIZEDS, *s) && !strchr(RESULT_ENTERABLES, *s))
			message("FINALNONENT %d", *s);
#endif  // TEST_FINALNONENT

#if TEST_AUTOPTR
	{
		TRequestResolution *resolution = new TRequestResolution;
		TVirtPtr VP(resolution);
	}
#endif  // TEST_AUTOPTR

#if TEST_WAITIME
	{
		TWaitWindow wait(parent, WAIT);
		DWORD ticks = GetTickCount();

		for (int i = 0; i < 600; i++)
		{
			wait.TickCount(i, "елемента");
			Sleep(100);
		}

		message("%ld ticks\n", (long) (GetTickCount() - ticks));
	}
#endif  // TEST_WAITIME

#if TEST_FILESERV
	{
		TWaitWindow wait(parent, WAIT);
		DWORD ticks = GetTickCount();
		mstr m;

		srand(ticks);

		for (int i = 0; i < 1000; i++)
		{
			m.clear();
			m.printf("\\\\irina\\123\\pic\\test%ld.txt", rand());
			exist(str(m));
		}

		message("%.3lf seconds\n", ((double) (GetTickCount() - ticks)) / 1000);
	}
#endif  // TEST_FILESERV

#if TEST_ATTACH_ACTS
	attach_acts(parent);
#endif  // TEST_ATTACH_ACTS

#if TEST_QUERY_UNATT
	query_unattached(parent);
#endif  // TEST_QUERY_UNATT

#if TEST_BYTEA_ENCOD
	{
		mstr m("INSERT INTO T_REGISTER_ACT VALUES (1, 2015, 'p', 'x', 1, '1', 1, ");
		TMemBlock blob;
		TBLOB blobField("F_FILE_DATA", &blob);

		/*for (int n = 0; n < 4096; n++)
		{
			for (int i = 0; i <= 255; i++)
			{
				unsigned char c = (unsigned char) i;
				mbk_append(&blob, &c, 1);
			}
		}*/

		FILE *f;
		TUserFile uf("даннов");
		/*f = fopen("c:\\tmp\\blob\\senddata", "wb");
		fwrite(blob.data, 1, blob.size, f);
		fclose(f);

		blobField.Print(m);
		m.cat(")");
		f = fopen("c:\\tmp\\blob\\sendtext", "wb");
		fwrite(str(m), 1, m.len(), f);
		fputc('\n', f);
		fclose(f);*/

		if (!uf.ReadAll("c:\\tmp\\blob\\gtkinval.dat", &blob, false, 70000000))
			return;

		ExtraDataBase->Execute("DELETE FROM T_REGISTER_ACT", ET_FATAL);
		Sleep(1000);
		f = fopen("c:\\tmp\\blob\\sendstat.txt", "w");
		DWORD ticks = GetTickCount();

		m.cat("?");
		TIArray<TCACParameter> params;
		params.Add(blobField.CreateParam());

		m.cat(")");
		fprintf(f, "create statement: %.3lf seconds\n", ((double) (GetTickCount() - ticks)) / 1000);
		ticks = GetTickCount();
		ExtraDataBase->Execute(str(m), ET_ERROR, NULL, NULL, &params);
		//uf.WriteAll("\\\\313-pc3\\shared\\blobdata", blob.data, blob.size);
		fprintf(f, "execute statement: %.3lf seconds\n", ((double) (GetTickCount() - ticks)) / 1000);
		fclose(f);
	}
#endif  // TEST_BYTEA_ENCOD

#if TEST_BYTEA_DECOD
	{
		mstr m("SELECT F_FILE_DATA FROM T_REGISTER_ACT WHERE F_NO = 1 AND F_YEAR = 2015 AND F_TYPE = 'p' "
			"AND F_KIND = 'x' AND F_DATE = 1 AND F_JUDGE = '1' AND F_ACT_NO = 1");
		TQuery q(str(m), ExtraDataBase);

		if (q.Read())
		{
			TMemBlock blob;

			FILE *f = fopen("c:\\tmp\\blob\\recvstat.txt", "w");
			DWORD ticks = GetTickCount();
			q.GetBLOB(q.Find("F_FILE_DATA"), &blob);
			fprintf(f, "fetch data: %.3lf seconds\n", ((double) (GetTickCount() - ticks)) / 1000);
			fclose(f);
			f = fopen("c:\\tmp\\blob\\recvdata", "wb");
			fwrite(blob.data, 1, blob.size, f);
			fclose(f);
		}
		else
			error("no F_FILE_DATA selected");
	}
#endif  // TEST_BYTEA_DECOD

#if TEST_CRLF_CONV
	{
		ExtraDataBase->Execute("DELETE FROM T_TEST", ET_ERROR);
		ExtraDataBase->Execute("INSERT INTO T_TEST VALUES (1, 'ab\rcd\nef')", ET_ERROR);
	}
#endif  // TEST_CRLF_CONV

#if TEST_BLOB_SERVER
	{
		DWORD ticks = GetTickCount();
		long oid = ExtraDataBase->GetLong("SELECT lo_import('c:\\tmp\\blob\\places.dat') AS F_OID", "F_OID");

		message("oid = %ld, eta = %.3lf seconds\n", ((double) (GetTickCount() - ticks)) / 1000);

		if (oid)
		{
			mstr m;

			m.printf("SELECT lo_unlink(%lu) AS F_OID", oid);

			if (ExtraDataBase->GetLong(str(m), "F_OID") != 1)
				error("unlink error");
		}
	}
#endif  // TEST_BLOB_SERVER

#if TEST_ASK_JOIN
	{
		TCitizen oldCitizen, newCitizen;

		oldCitizen.ucnType = newCitizen.ucnType = 'c';
		strcpy(oldCitizen.ucn, "7106051022");
		strcpy(newCitizen.ucn, "7112101077");
		ask_join(&oldCitizen, &newCitizen);

		TSender oldSender, newSender;

		oldSender.ucnType = newSender.ucnType = 'J';
		strcpy(oldSender.ucn, "0000000310");
		strcpy(newSender.ucn, "0000000410");
		ask_join(&oldSender, &newSender);
	}
#endif  // TEST_ASK_JOIN

#if TEST_GET_ADDRESS
	get_address(parent, "7106051022", 'c', ADDRESS_BULGARIAN_ONLY);
#endif  // TEST_GET_ADDRESS

#if TEST_PASS_XOR
	{
		//unsigned char buffer[] = "????????";
		//unsigned char buffer[] = "????????";
		unsigned char buffer[] = "boza";
		unsigned char backup[sizeof buffer];
		mstr m;

		strcpy((char *) backup, (char *) buffer);
		calc_simple_xor(buffer, sizeof buffer);

		for (int i = 0; i < sizeof buffer; i++)
		{
			m.sep(" ");
			m.printf("%02x", buffer[i]);
		}

		calc_simple_xor(buffer, sizeof buffer);

		if (strcmp((char *) buffer, (char *) backup))
			fatal("calc_simple_xor failed");

		message("%s", str(m));
	}
#endif  // TEST_PASS_XOR

#if TEST_XFER_UUID
	{
		extern void xfer_uuid(char *id, TRCDKeyContainer *container);

		TLawsuit lawsuit;
		char id[SIZE_OF_UUID];
		//int courtCode = COURT_CODE;

		lawsuit.key.no = 1271;
		lawsuit.key.year = 2017;
		lawsuit.key.type = TYPE_CITIZEN_LAW;
		lawsuit.date = CDate(18, 8, 2016);
		//lawsuit.kind = KIND_1ST_TRADE_LAW;

		//COURT_CODE = 140;
		xfer_uuid(id, &lawsuit);
		//COURT_CODE = courtCode;
		message("uuid = %s", id);
	}
#endif  // TEST_XFER_UUID

#if TEST_CLASS_NAME
	{
		char className[10] = { 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', '\0' };
		int result;

		result = GetClassName(parent->HWindow, className, 6);
		message("GetClassName = %s, result = %d", className, result);
	}
#endif  // TEST_CLASS_NAME

#if TEST_SUBJECTS
	{
		TGArray<TSubject> subjects;
		select_subjects(parent, &subjects, KIND_LAWSUITS, true);
	}
#endif  // TEST_SUBJECTS

#if TEST_DUMP_SUBJS
	test_dump_subjs();
#endif  // TEST_DUMP_SUBJS

#if TEST_LAST_MOVE
	{
		TLawsuit lawsuit;

		lawsuit.key.no = 1;
		lawsuit.key.year = 2015;
		lawsuit.key.type = 'c';
		lawsuit.Get();

		TMovement *movement = lawsuit.LastMove(&Today, &LNow());
		//TMovement *movement = lawsuit.LastMove(NULL, NULL);
		message("%d", movement != NULL);
		delete movement;
	}
#endif  // TEST_LAST_MOVE

#if TEST_COMPOS
	{
		msql m(Composition.Name);
		TQuery q(str(m));

		while (q.Read())
		{
			*Composition << q;
			message("name = %s", Composition->name);
		}
	}
#endif  // TEST_COMPOS

#if TEST_SURROUND
	{
		TSession session;
		TGArray<TSurroundment> surroundments;

		session.key.no = 5;
		session.key.year = 2015;
		session.key.type = 'p';
		session.kind = 'И';
		session.date.day = 26;
		session.date.month = 1;
		session.date.year = 2015;
		try2surround(&session, &surroundments);
		message("%d", try2surround(&session, NULL));
		try2surround(&session, &surroundments, "F_BOZA");
	}
#endif  // TEST_SURROUND

#if TEST_SURROUNDMENT
	{
		TSurroundment surroundment;

		surroundment.key.no = 830;
		surroundment.key.year = 2015;
		surroundment.kind = 'ё';
		surroundment.lawsuit.no = 5;
		surroundment.lawsuit.year = 2015;
		surroundment.lawsuit.type = 'p';
		surroundment.sessionKind = 'И';
		surroundment.sessionDate.day = 26;
		surroundment.sessionDate.month = 1;
		surroundment.sessionDate.year = 2015;
		surroundment.Try("F_NO");
	}
#endif  // TEST_SURROUNDMENT

#if TEST_SPLITACTFLAGS
	{
		TDecision decision;
		mstr m;

		decision.key.no = 1;
		decision.key.year = 2016;
		decision.key.type = 'p';
		decision.kind = 'Ш';
		decision.date.day = 4;
		decision.date.month = 4;
		decision.date.year = 2016;

		if (!decision.Get())
			failure();

		for (int n = 0; n < 0x80; n++)
		{
			decision.flags = n;
			decision.flagsX = 0;
			decision.Update("F_FLAGS, F_FLAGS_X");

			static constant statements[] =
			{
				"UPDATE T_DECISION SET F_FLAGS_X = 4, F_FLAGS = F_FLAGS - 64 WHERE F_FLAGS >= 64",
				"UPDATE T_DECISION SET F_FLAGS_X = F_FLAGS_X + 2, F_FLAGS = F_FLAGS - 32 WHERE F_FLAGS >= 32 AND NOT F_FLAGS_X IS NULL",
				"UPDATE T_DECISION SET F_FLAGS_X = 2, F_FLAGS = F_FLAGS - 32 WHERE F_FLAGS >= 32 AND F_FLAGS_X IS NULL",
				"UPDATE T_DECISION SET F_FLAGS_X = F_FLAGS_X + 1, F_FLAGS = F_FLAGS - 1 WHERE F_FLAGS IN (1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31) AND NOT F_FLAGS_X IS NULL",
				"UPDATE T_DECISION SET F_FLAGS_X = 1, F_FLAGS = F_FLAGS - 1 WHERE F_FLAGS IN (1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31) AND F_FLAGS_X IS NULL",
				NULL
			};

			for (int i = 0; statements[i]; i++)
			{
				m.cpy(statements[i]);
				m.cat(" AND F_NO = 1 AND F_YEAR = 2016 AND F_TYPE = 'p' AND F_KIND = 'Ш' AND F_DATE = 90283873");
				AppName->Execute(str(m), ET_FATAL);
			}

			if (!decision.Get("F_FLAGS, F_FLAGS_X"))
				failure();

			if (decision.flags != (n & 0x1E) || decision.flagsX != ((n & 0x60) >> 4) + (n & 0x01))
				fatal("failed for n = %d", n);
		}

		decision.flags = DECISION_PUBLIC;
		decision.flagsX = 0;
		decision.Update("F_FLAGS, F_FLAGS_X");
	}
#endif  // TEST_SPLITACTFLAGS

#if TEST_SPLITSESFLAGS
	{
		msql m;

		m.AddFlags("F_FLAGS", SESSION_PRIVATE, SESSION_PRIVATE, SESSION_ALL);
		message("%s", str(m));
	}

	{
		TSession session;
		mstr m;

		session.key.no = 1;
		session.key.year = 2016;
		session.key.type = 'p';
		session.kind = 'Щ';
		session.date.day = 4;
		session.date.month = 4;
		session.date.year = 2016;

		if (!session.Get())
			failure();

		for (int n = 0; n < 0x100; n++)
		{
			session.flags = n;
			session.flagsX = 0;
			session.Update("F_FLAGS, F_FLAGS_X");

			static constant statements[] =
			{
				"UPDATE T_SESSION SET F_FLAGS_X = 8, F_FLAGS = F_FLAGS - 128 WHERE F_FLAGS >= 128",
				"UPDATE T_SESSION SET F_FLAGS_X = F_FLAGS_X + 4, F_FLAGS = F_FLAGS - 64 WHERE F_FLAGS >= 64 AND NOT F_FLAGS_X IS NULL",
				"UPDATE T_SESSION SET F_FLAGS_X = 4, F_FLAGS = F_FLAGS - 64 WHERE F_FLAGS >= 64 AND F_FLAGS_X IS NULL",
				"UPDATE T_SESSION SET F_FLAGS_X = F_FLAGS_X + 2, F_FLAGS = F_FLAGS - 8 WHERE F_FLAGS IN (8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 26, 27, 28, 29, 30, 31, 40, 41, 42, 43, 44, 45, 46, 47, 56, 57, 58, 59, 60, 61, 62, 63) AND NOT F_FLAGS_X IS NULL",
				"UPDATE T_SESSION SET F_FLAGS_X = 2, F_FLAGS = F_FLAGS - 8 WHERE F_FLAGS IN (8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 26, 27, 28, 29, 30, 31, 40, 41, 42, 43, 44, 45, 46, 47, 56, 57, 58, 59, 60, 61, 62, 63) AND F_FLAGS_X IS NULL",
				"UPDATE T_SESSION SET F_FLAGS_X = F_FLAGS_X + 1, F_FLAGS = F_FLAGS - 4 WHERE F_FLAGS IN (4, 5, 6, 7, 20, 21, 22, 23, 36, 37, 38, 39, 52, 53, 54, 55) AND NOT F_FLAGS_X IS NULL",
				"UPDATE T_SESSION SET F_FLAGS_X = 1, F_FLAGS = F_FLAGS - 4 WHERE F_FLAGS IN (4, 5, 6, 7, 20, 21, 22, 23, 36, 37, 38, 39, 52, 53, 54, 55) AND F_FLAGS_X IS NULL",
				NULL
			};

			for (int i = 0; statements[i]; i++)
			{
				m.cpy(statements[i]);
				m.cat(" AND F_NO = 1 AND F_YEAR = 2016 AND F_TYPE = 'p' AND F_KIND = 'Щ' AND F_DATE = 90283873");
				AppName->Execute(str(m), ET_FATAL);
			}

			if (!session.Get("F_FLAGS, F_FLAGS_X"))
				failure();

			if (session.flags != (n & 0x33) || session.flagsX != ((n & 0xC0) >> 4) + ((n & 0x0C) >> 2))
				fatal("failed for n = %d", n);
		}

		session.flags = 0;
		session.flagsX = 0;
		session.Update("F_FLAGS, F_FLAGS_X");
	}
#endif  // TEST_SPLITSESFLAGS

#if TEST_SNPRINTF
	{
		const char *text = "proba proba proba";
		const char *x_x = "xxxxxxxxxx";
		char s[10];
		char ss[20];
		int result;

		memset(s, 'x', sizeof s);
		memset(ss, 'x', sizeof ss);

		result = snprintf(NULL, 0, text);
		cac_assert(result == strlen(text));

		result = snprintf(s, 0, text);
		cac_assert(result == strlen(text));
		cac_assert(memcmp(s, x_x, sizeof s) == 0);

		result = snprintf(s, 1, text);
		cac_assert(result == strlen(text));
		cac_assert(*s == '\0');
		cac_assert(memcmp(s + 1, x_x, sizeof s - 1) == 0);

		result = snprintf(s, sizeof s, text);
		cac_assert(result == strlen(text));
		cac_assert(memcmp(s, text, sizeof s - 1) == 0);
		cac_assert(s[sizeof s - 1] == '\0');

		result = snprintf(ss, sizeof s, text);
		cac_assert(result == strlen(text));
		cac_assert(memcmp(ss, text, sizeof s - 1) == 0);
		cac_assert(ss[sizeof s - 1] == '\0');
		cac_assert(memcmp(ss + sizeof s, x_x, sizeof ss - sizeof s) == 0);
	}
#endif  // TEST_SNPRINTF

#if TEST_JUROR_LOAD
	{
		void query_juror_test(TWindow *parent);
		query_juror_test(parent);
	}
#endif  // TEST_JUROR_LOAD

#if TEST_COUNTEX
	{
		msql m(Court->Name, "F_COURT, F_NAME");
		FILE *f = fopen("c:\\tmp\\countex.sql", "w");

		m.AddChar("F_COURT_TYPE", COURT_REGIONAL + '0');
		m.AddOrder("F_COURT");
		TQuery q(str(m));

		while (q.Read())
		{
			*Court << q;

			const char *minus = strstr(Court->name, " - ");
			UCLP->uclp = 0;

			if (minus)
			{
				m.Begin(UCLP->Name, "F_UCLP");
				m.AddString("F_NAME", minus + 3);

				TQuery r(str(m));
				if (r.Read() && !r.Read())
					*UCLP << r;
			}

			fprintf(f, "INSERT INTO T_SENDER VALUES ('0000000%ld', 'N', 'ДСИ при %s', NULL, NULL, NULL);\n",
				Court->court, Court->name);

			fprintf(f, "INSERT INTO T_ADDRESS (F_UCN, F_UCN_TYPE, F_ADDRESS, F_UCLP) VALUES ('0000000%ld', 'N', 1, ",
				Court->court);

			if (UCLP->uclp)
				fprintf(f, "%ld", UCLP->uclp);
			else
				fputs("NULL", f);

			fputs(");\n\n", f);
		}

		fclose(f);
	}
#endif  // TEST_COUNTEX

#if TEST_WAIT_RESID
	{
		message("resId = %d", TWaitWindow::ComputeResId("Проверка на цифровия подпис..."));
	}
#endif  // TEST_WAIT_RESID

#if TEST_OTHER_INVOLV
	{
		char commonOtherLawsInvl[0x100];
		char commonOtherReqsInvl[0x100];

		common_involvements(KIND_LAWSUITS, commonOtherLawsInvl);
		message("Общи трети участия по делата: %s", commonOtherLawsInvl);
		common_with_other_sess(commonOtherLawsInvl);

		common_involvements(KIND_ALLDOCS, commonOtherReqsInvl);
		message("Общи трети участия по преписките: %s", commonOtherReqsInvl);
		common_with_other_sess(commonOtherReqsInvl);
	}
#endif  // TEST_OTHER_INVOLV

#if TEST_SUBJECT_TEXT
	{
		TSubject subject;
		message("empty subject name len = %u", strlen(subject.name));

		subject.subject = 10000;
		subject.type = 'c';
		if (subject.Get("F_NAME"))
			message("subject c/100 name len = %u", strlen(subject.name));
	}
#endif  // TEST_SUBJECT_TEXT

#if TEST_QUERY_RESTART
	{
		msql m;
		TDecision decision;

		decision.key.no = 581;
		decision.key.year = 2016;
		decision.key.type = 'c';
		decision.FloatCriteria(m);

		TQuery q(str(m), NULL);  //, true);
		if (q.Read())
		{
			decision << q;
			message("Read, %s, text = %s", TCACDataBase::DefaultDB->Protocol(), str(decision.text));
			decision.text.cpy("");

			q.Restart();
			if (q.Read())
			{
				decision << q;
				message("Re-Read, text = %s", str(decision.text));
			}
			else
				error("failed to re-read decision");
		}
		else
			error("failed to read decision");
	}
#endif  // TEST_QUERY_RESTART

#if TEST_PSHELL_CKSUM
	message("pshell=%d", check_pshell_md5sum(parent, "lmupdate\\lmupdate.psh"));
#endif  // TEST_PSHELL_CKSUM

#if TEST_REQUEST_ID
	{
		#define read32(s, len) read3x((s), (len), 32, true)
		long tRequestId = read32("bac002f4700000t" + 9, 6);
		message ("tRequestId=%ld", tRequestId);
	}
#endif  // TEST_REQUEST_ID

#if TEST_POST_CHIEF
	Default->post = POST_CHIEF;
#endif  // TEST_POST_CHIEF

#if TEST_STREQUAL
	{
		constant s1 = "IS3GBUYDPLHJKТQAАНКhNр‡";
		constant s2 = "IS3GLHJKPBUYDПТQAАНКhN‡";

		for (const char *s = s1; *s; s++)
			if (!strchr(s2, *s))
				message("only in s1: %d", *s);

		for (const char *s = s2; *s; s++)
			if (!strchr(s1, *s))
				message("only in s2: %d", *s);
	}
#endif  // TEST_STREQUAL

#if TEST_EIS_NAME
	{
		TRequest request;

		request.key.no = 3;
		request.key.year = 2017;

		if (request.Get())
		{
			char name[SIZE_OF_PATH];
			char tName[SIZE_OF_PATH];

			electric_name(&request, EISPP_XMLPATH, name, TYPE_SBE_DESC + 10);
			request.kind = 'З';
			electric_name(&request, EISPP_XMLPATH, tName, TYPE_SBE_DESC + 10);
			message("%s\n%s", name, tName);
		}
	}
#endif  // TEST_EIS_NAME

#if TEST_REKIND_NAME
	{
		TRequest request;

		request.key.no = 2;
		request.key.year = 2017;

		if (request.Get())
			electric_rekind(&request, 'w', request.date);
	}
#endif  // TEST_REKIND_NAME

#if TEST_ACVAR_BOZA
	{
		TRequest request;
		char testInv = 'C';

		request.key.no = 2;
		request.key.year = 2017;

		if (request.Get())
		{
			if (request.Suitor(testInv))
				message("%c, %c -> Suitor", request.kind, testInv);

			if (request.Defendant(testInv))
				message("%c, %c -> Defendant", request.kind, testInv);

			if (request.Other(testInv))
				message("%c, %c -> Other", request.kind, testInv);
		}
	}
#endif  // TEST_ACVAR_BOZA

#if DUMP_XFER_INDOC_KINDS
	void dump_xfer_indoc_kinds();
	dump_xfer_indoc_kinds();
#endif  // DUMP_XFER_INDOC_KINDS

#if TEST_UNFINISHED
	{
		TLawsuit lawsuit;

		lawsuit.key.no = 10003;
		lawsuit.key.year = 2016;
		lawsuit.key.type = 'p';

		if (lawsuit.Get())
		{
			TUnfinishedLawsuit unfinished(Today);

			unfinished << lawsuit;
			unfinished.FindPrevious(); 
		}
	}
#endif  // TEST_UNFINISHED

#if TEST_BANK_HTML
	query_bank_html(parent, true);
#endif  // TEST_BANK_HTML

#if TEST_SEQ_OPEN
	{
		SHELLEXECUTEINFO execInfo;
		//constant name1 = "c:\\tmp\\closed3.htm";
		constant name1 = "c:\\tmp\\MS-logo.TIF";
		constant name2 = "c:\\tmp\\closed1.htm";

		memset(&execInfo, '\0', sizeof execInfo);
		execInfo.cbSize = sizeof execInfo;
		#define SEE_MASK_WAITFORINPUTIDLE 0x2000000
		execInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI; //| SEE_MASK_CLASSNAME;
		// | SEE_MASK_WAITFORINPUTIDLE;
		// | SEE_MASK_CLASSNAME;
		execInfo.lpVerb = "open";
		execInfo.lpFile = name1;
		execInfo.nShow = SW_NORMAL;
		//execInfo.lpClass = "Word.Document";
		//execInfo.lpClass = "TIFImage.Document";

		DWORD ticks = GetTickCount();

		if (ShellExecuteEx(&execInfo))
		{
			sleep_sync();
		#if 1
			WaitForSingleObject(execInfo.hProcess, 120);
			message("I have returned, after %d seconds.", (GetTickCount() - ticks) / 1000);
		#else
			execInfo.lpVerb = "open";
			execInfo.lpFile = name2;

			if (ShellExecuteEx(&execInfo))
				message("I have returned, after %d seconds.", (GetTickCount() - ticks) / 1000);
			else
				error("%s: грешка %lu при отваряне.", name2, (unsigned long) execInfo.hInstApp);
		#endif
		}
		else
			error("%s: грешка %lu при отваряне.", name1, (unsigned long) execInfo.hInstApp);
	}
#endif  // TEST_SEQ_OPEN

#if TEST_POSTERM_LOGGING
	{
		extern const char *LOG_NAME;
		extern FILE *log_file;
		extern void log_data(const char *what, const char *data, size_t len);

		mstr send("send");
		mstr m("error");
		log_data("SEND", str(send), send.len());

		if (!log_file)
		{
			log_file = fopen(LOG_NAME, "a");
			m.printf(", журнал: %lu", (unsigned long) log_file);
		}

		DWORD written = 5;
		char buffer[] = "RECV";
		DWORD read = sizeof buffer;

		log_data("INIT", NULL, 0);
		log_data("SEND", str(send), send.len());
		log_data("CUTD", (const char *) &written, sizeof written);
		log_data("RECV", buffer, read);
		log_data("SEND ACK", NULL, 0);
		log_data("FAIL", NULL, 0);
	}
#endif  // TEST_POSTERM_LOGGING

#if TEST_EXEC_WAIT
	test_wait(parent);
#endif  // TEST_EXEC_WAIT

#if TEST_TRANS_ID_INVALID
	{
		TPayment payment;

		strcpy(payment.transId, TRANS_ID_INVALID);
		payment.Insert();
	}
#endif  // TEST_TRANS_ID_INVALID

#if TEST_CLIP_UNICODE
	{
		static const uint FORMATS[4] = { CF_TEXT, CF_OEMTEXT, CF_UNICODETEXT, CF_LOCALE };
		static constant FORMAT_NAMES[4] = { "ansi", "oem", "unicode", "loc_id" };

		TClipboard clipboard(parent->Handle);
		mstr m;

		for (int index = 0; index < 4; index++)
		{
			HANDLE handle;
			const void *address;

			m.printf("clip %s -> ", FORMAT_NAMES[index]);

			if (!clipboard.IsClipboardFormatAvailable(FORMATS[index]))
				m.cat("format not available");
			else if ((handle = clipboard.GetClipboardData(FORMATS[index])) == NULL)
				m.cat("failed to get handle");
			else if ((address = GlobalLock(handle)) == NULL)
				m.cat("failed to lock handle");
			else
			{
				switch (index) {
					case 0 :
					case 1 : m.cat((const char *) address); break;
					case 2 :
					{
						for (const wchar_t *ws = (const wchar_t *) address; *ws; ws++)
							m.printf("0x%04X ", *ws);

						const wchar_t *const ws = (const wchar_t *) address;

						if (*ws)
						{
							char *s = tocp1251(ws, '.');
							TCharPtr CP(s);

							if (s)
								m.cat(s);
						}

						break;
					}
					default : m.printf("0x%08X ", *(LCID *) address);
				}

				::GlobalUnlock(handle);
			}

			m.cat("\n");
		}

		message("%s", str(m));

		if (CHROME_UNICODE_CONVERT)
		{
			char *s = cliptext(parent, ".? "[CHROME_UNICODE_CONVERT - 1]);
			TCharPtr CP(s);

			if (s)
				message("conv = %s", s);
		}
	}
#endif  // TEST_CLIP_UNICODE

#if TEST_SENCOR_NAME
	{
		mstr m;
		get_sencor_name(UCN_INSTANCE_SELF, COURT_CODE, m);
		message("%s", str(m));
	}
#endif  // TEST_SENCOR_NAME

#if TEST_PRINT_CONNECT
	{
		TConnect connect;

		connect.key.no = 1150;
		connect.key.year = 2016;
		connect.key.type = 't';
		connect.connectType = CONNECT_TYPE_LOWER_INSTANCE;

		if (connect.Get())
		{
			mstr m;

			connect.Print(m);
			m.cat("\n\n");
			connect.Print(m, false, false, "състав");
			message("%s", str(m));
		}
	}
#endif  // TEST_PRINT_CONNECT

#if TEST_DUMP_KEYS
	{
		TConfigName configName(CONFIG_NAME_GLOBAL);
		TKnownPerson knownPerson;
		TAnnounce announce;
		TAnnounceVisit announceVisit;
		TAddress address;
		TCitizen citizen;
		TConfigData configData(CONFIG_NAME_GLOBAL);
		TDecision decision;
		TFirm firm;
		TForeignAddress foreignAddress;
		TInReg inReg;
		TInRegEvent inRegEvent;
		TLawsuit lawsuit;
		TLinkWind linkWind;
		TOutReg outReg;
		TRequest request;
		TSender sender;
		TSession session;
		TSideWind sideWind;
		TSubpoena subpoena;
		TSubpoenaVisit subpoenaVisit;

		FILE *f = fopen("c:\\tmp\\__cac_keys.txt", "w");
		TGroup *groups[] =
		{
			// Nom
			Area, College, Composition, &configName, ConnectKind, ConnectType, Country, Court, FineComposition, Hall,
			Involvement, Kind, &knownPerson, ReceivedStatus, Region, Resolution, Result, Street, Subject, SubpoenaKind,
			Type, UCLP, UCNType, User,
			// Doc
			&address, &announce, &announceVisit, &citizen, &configData, &decision, &firm, &foreignAddress, &inReg,
			&inRegEvent, &lawsuit, &linkWind, &outReg, &request, &sender, &session, &sideWind, &subpoena,
			&subpoenaVisit
		};

		for (int i = 0; i < sizeof groups / sizeof groups[0]; i++)
		{
			TGroup *group = groups[i];

			for (int exact = true; exact >= false; exact--)
			{
				constant keyName = exact ? "exact" : "float";
				msql m;

				clearGroupData(*group);

				if (exact)
					group->ExactCriteria(m);
				else if (!strcmp(group->Name, "T_INREG_EVENT"))
				{
					m.Begin(group->Name);
					m.Add(group, "F_NO, F_YEAR, F_TYPE");
				}
				else
					group->FloatCriteria(m);

				m.CutOrder();
				fprintf(f, "%s: %s: %s\n", group->Name, keyName, str(m));
			}
		}

		{
			msql m;
			
			subpoena.RegardCriteria(m, NULL, false);
			fprintf(f, "T_SUBPOENA: regard: %s\n", str(m));

			subpoena.MultiCriteria(m, NULL);
			fprintf(f, "T_SUBPOENA: multi: %s\n", str(m));
		}

		fputc('\n', f);
		fclose(f);
	}
#endif  // TEST_DUMP_KEYS

#if TEST_COMMON_FIELDS
	{
		TTestGroup group;
		TSession session;
		group.Test(session);
	}
#endif  // TEST_COMMON_FIELDS

#if TEST_LOG_MD5SUM
	{
		TLog log;

		log.log = 123456789;
		strcpy(log.ucn, "7106051022");
		log.gop = 'Y';
		log.opc = 21930;
		log.stamp.day = 12;
		log.stamp.month = 9;
		log.stamp.year = 2018;
		log.stamp.hour = 13;
		log.stamp.minute = 38;
		log.stamp.second = 38;
		log.key.no = 700;
		log.key.year = 2015;
		log.key.type = 'c';
		log.kind = 'Г';
		log.date.day = 10;
		log.date.month = 2;
		log.date.year = 2017;
		strcpy(log.text, "Бързата кафява лисица предкача ленивото");

		long resBlock[4];

		srand(0x55AA);
		CalcMD5(log, resBlock);

		mstr m;

		m.printf("%ld %ld %ld %ld", resBlock[0], resBlock[1], resBlock[2], resBlock[3]);
		write_all_bytes("c:\\tmp\\__cac_resblock", (const char *) m.data, m.len());
	}
#endif  // TEST_LOG_MD5SUM

#if TEST_OPEN_ENTER
	{
		char result;

		for (const char *s = RESULT_FINALIZEDS; (result = *s) != '\0'; s++)
			if (strchr(RESULT_SESSIONS, result) && !strchr(RESULT_ENTERABLES, result))
				message("fin-ses-non-act: %d %s", result, Result->Seek(result));
	}
#endif  // TEST_OPEN_ENTER

	message("testfunc finished");
}

void write_all_bytes(const char *filename, const char *data, size_t size)
{
	FILE *f = fopen(filename, "wb");

	if (!f)
		error("wab: %s: %s", filename, strerror(errno));
	else
	{
		fwrite(data, size, 1, f);
		fclose(f);
	}
}
#endif  // TESTVER

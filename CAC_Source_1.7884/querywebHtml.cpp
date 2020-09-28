#include "cac.h"

// ----- WEB_QUERY_ ----------------------------------------------------------
enum
{
	WEB_QUERY_FIXED,
	WEB_QUERY_PUBLIC,
	WEB_QUERY_FORCED,
	WEB_QUERY_POLICE,
	WEB_QUERY_PROSEC,
	WEB_QUERY_ACQUIT,		// 2014:041 +related
	WEB_QUERY_REINVEST,	// 2014:041 +related
	WEB_QUERY_PROTOCOL,	// 2018-11-09: +related
	WEB_QUERY_COUNT
};

enum { SIZE_OF_WEB_QUERY_NAME = SIZE_OF_DIR + SIZE_OF_BASE_NAME };

// ----- WEB_SCHEME_ ---------------------------------------------------------
enum
{
	WEB_SCHEME_MM00	= 1,
	WEB_SCHEME_MMDD	= 2,
	WEB_SCHEME_WEEK	= 3,
	WEB_SCHEME_FLAT	= 4,
	WEB_SCHEME_NULL	= 0	// default electric
};

// ----- TQueryWebGroup ------------------------------------------------------
class TQueryWebGroup : public TPeriodGroup
{
public:
	TQueryWebGroup();

	virtual void FormCriteria(msql &m);

	long variant;
	char kinds[SIZE_OF_KINDS];
	long judges;
	long important;
	long filtered;
	long order;
	char name[SIZE_OF_OFFICE_TARGET];
	long attached;
	char dir[SIZE_OF_OFFICE_TARGET];
	long text;
	long limited;		// for FIXED _only_
	long appeal;
	char compositions[SIZE_OF_COMPOSITIONS];

	bool fixed;  // variant == FIXED
	bool police;  // variant == POLICE
	bool inspect;  // variant == ACQUIT || REINVEST
	bool protocol;  // variant == PROTOCOL
};

TQueryWebGroup::TQueryWebGroup()
	: TPeriodGroup("T_QUERY_WEB")
{
	Add(
		new TRadio("F_FIXED", &variant, WEB_QUERY_FIXED),
		new TRadio("F_PUBLIC", &variant, WEB_QUERY_PUBLIC),
		new TRadio("F_FORCED", &variant, WEB_QUERY_FORCED),
		new TRadio("F_POLICE", &variant, WEB_QUERY_POLICE),
		new TRadio("F_PROSEC", &variant, WEB_QUERY_PROSEC),
		new TCharList("F_KINDS", kinds, sizeof kinds),
		new TCheck("F_JUDGES", &judges, TRUE),
		new TCheck("F_IMPORTANT", &important, TRUE),
		new TCheck("F_FILTERED", &filtered, TRUE),
		new TRadio("F_BY_TYN", &order, ORDER_BY_TYN),
		new TRadio("F_BY_DTYN", &order, ORDER_BY_DTYN),
		new TString("F_NAME", name, sizeof name),
		new TCheck("F_ATTACHED", &attached, TRUE),
		new TCheck("F_TEXT", &text, TRUE),
		new TCheck("F_LIMITED", &limited, TRUE),
		new TCheck("F_APPEAL", &appeal, TRUE),
		new TRadio("F_ACQUIT", &variant, WEB_QUERY_ACQUIT),
		new TRadio("F_REINVEST", &variant, WEB_QUERY_REINVEST),
		new TRadio("F_PROTOCOL", &variant, WEB_QUERY_PROTOCOL),
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		NULL
	);

	variant = WEB_QUERY_FIXED;
	judges = TRUE;
	order = ORDER_BY_DTYN;
	filtered = TRUE;
	text = TRUE;
}

void TQueryWebGroup::FormCriteria(msql &m)
{
	const char *dateName;

	switch (variant)
	{
		case WEB_QUERY_FIXED :
		case WEB_QUERY_PROTOCOL : dateName = "S.F_DATE"; break;
		case WEB_QUERY_FORCED :	dateName = "D.F_FORCE_DATE"; break;
		case WEB_QUERY_PROSEC : dateName = "D.F_RETURNED"; break;
		case WEB_QUERY_PUBLIC : dateName = "D.F_PUBLIC_DATE"; break;
		default : dateName = "D.F_FROM_DATE";
	}

	mstr name("T_SESSION S");
	mstr fields("S.F_NO, S.F_YEAR, S.F_TYPE, L.F_FLAGS AS F_LAW_FLAGS, L.F_KIND AS F_LAW_KIND, L.F_COMPOSITION, "
		"L.F_SUBJECT");

	if (judges)
		fields.cat(", S.F_PRESIDENT, S.F_JUDGE");

	if (fixed)
		fields.cat(", S.F_TIME, S.F_END, S.F_ENTIME, S.F_DATE, S.F_KIND, S.F_HALL, S.F_TEXT");
	else if (protocol)
		fields.cat(", S.F_RESULT, S.F_DATE, S.F_KIND, S.F_TEXT, S.F_FORCE_DATE");
	else
	{
		name.cat(" JOIN T_DECISION D ON D.F_NO = S.F_NO AND D.F_YEAR = S.F_YEAR AND D.F_TYPE = S.F_TYPE "
			"AND D.F_DATE = S.F_DATE AND D.F_SESSION_KIND = S.F_KIND");

		fields.cat(", D.F_DATE, D.F_KIND, D.F_TEXT, D.F_FROM_DATE, D.F_SESSION_KIND, D.F_MOTIVES, D.F_FORCE_DATE, "
			"D.F_OPINION, D.F_FLAGS, D.F_RESULT, D.F_RETURNED");

		if (variant == WEB_QUERY_REINVEST)
			fields.cat(", L.F_SOURCE_NO, L.F_SOURCE_YEAR, L.F_SOURCE_TYPE, L.F_SOURCE_KIND");
	}

	name.cat(" JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE "
		"JOIN T_KIND K ON K.F_KIND = L.F_KIND");

	m.Begin(str(name), str(fields));
	m.AddRange(dateName, minDate, maxDate);

	if (fixed)
		m.AddChars("S.F_RESULT", RESULT_FIXEDS);

	if (protocol)
		m.AddChars("S.F_KIND", KIND_OPEN_SESSIONS);

	if (any(kinds))
		m.AddChars("L.F_KIND", kinds, KIND_LAWSUITS);

	if (important)
		m.AddFlags("L.F_FLAGS", LAWSUIT_IMPORTANT, LAWSUIT_IMPORTANT, LAWSUIT_ALL);

	// 2014:043
	if (variant == WEB_QUERY_ACQUIT)
	{
		m.Add("");
		m.printf("(SELECT COUNT(J.F_NO) FROM T_JUDGEMENT J WHERE J.F_NO = D.F_NO AND J.F_YEAR = D.F_YEAR "
			"AND J.F_TYPE = D.F_TYPE AND J.F_KIND = D.F_KIND AND J.F_DATE = D.F_DATE AND J.F_JUDGED = '%c') > 0",
			JUDGED_ACQUIT);
	}
	else if (variant == WEB_QUERY_REINVEST)
		m.AddChars("S.F_RESULT", RESULT_REINVESTS);

	// 2014:044
	m.AddCompos("L.F_COMPOSITION", compositions);
	// 2014:149 LPR: for IRQ/URQ speedup
	switch (variant)
	{
		case WEB_QUERY_PROSEC : m.AddFlags("D.F_FLAGS", DECISION_MOTIVES, DECISION_MOTIVES, DECISION_ALL); break;
		case WEB_QUERY_PUBLIC : //m.AddIsNull("S.F_FINAL", false);	// falldown; 2015:147 LRQ: comout
		case WEB_QUERY_FORCED : m.AddFlags("D.F_FLAGS", DECISION_PUBLIC, DECISION_PUBLIC, DECISION_ALL); break;
		case WEB_QUERY_PROTOCOL : m.AddFlags("S.F_FLAGS_X", SESSION_X_CLOSEDD, 0, SESSION_X_ALL);
	}

	m.AddOrder("");

	if (order == ORDER_BY_DTYN)
	{
		m.printf("%s, ", variant == WEB_QUERY_PUBLIC ? "D.F_FROM_DATE" : dateName);

		if (fixed)
			m.cat("S.F_TIME, ");
	}

	m.cat("L.F_TYPE, L.F_YEAR, L.F_NO");

	if (!fixed && !protocol && m.index("D.F_FROM_DATE") == INT_MAX)
		m.cat(", D.F_FROM_DATE");
}

// ----- TWebSession ---------------------------------------------------------
class TWebSession : public TSession
{
public:
	TWebSession();
	virtual ~TWebSession() { delete personal; }

	CDate fromDate;
	char sessionKind;
	mstr motives;
	mstr opinion;
	char lawKind;
	long lawFlags;
	long subject;
	char ucnType;
	char ucn[SIZE_OF_UCN];
	mstr judText;
	long appeal;
	TRCDKey source;
	char sourceKind;

	char *categories;
	TQueryWebGroup *group;
	TGArray<TSideWind> sideWinds;
	TPersonal *personal;

	void LoadSides();
};

TWebSession::TWebSession()
	: personal(NULL)
{
	Add(
		new TDate("F_FROM_DATE", &fromDate, true),
		new TCharAlias("F_SESSION_KIND", &sessionKind),
		new TText("F_MOTIVES", &motives, SIZE_OF_MOTIVES),
		new TText("F_OPINION", &opinion, SIZE_OF_OPINION),
		new TCharAlias("F_LAW_KIND", &lawKind),
		new TLong("F_LAW_FLAGS", &lawFlags),
		new TLongAlias("F_SUBJECT", &subject),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		new TString("F_UCN", ucn, sizeof ucn),
		NEW_RCDKEY("F_SOURCE_", &source),
		new TCharAlias("F_SOURCE_KIND", &sourceKind),
		NULL
	);
}

void TWebSession::LoadSides()
{
	TSession tSession;
	tSession << this;

	if (!group->fixed && !group->protocol)
		tSession.kind = sessionKind;

	sideWinds.Flush();
	categories = load_sidebasic(&tSession, sideWinds);

	delete personal;
	personal = new TPersonal(&tSession);
}

static void query_web_side_list(mstr &m, TGArray<TSideWind> &sideWinds, const char *categories, char category, bool filtered)
{
	for (int i = 0; i < sideWinds.Count(); i++)
	{
		TSideWind *sideWind = sideWinds[i];

		if ((categories ? categories[i] : sideWind->involvement) == category)
		{
			m.sep(",\v");
			print_side_wind(m, sideWind, filtered);
		}
	}

	if (!any(m))
		m.cat("\x06");
}

static bool emit_link(mstr &m, const TQueryWebGroup *group, const TRCDKeyContainer *container, const char *prefix,
	const char *linkText, bool always, int type = -1)
{
	if (group->attached)
	{
		char source[SIZE_OF_PATH];

		if (type == -1)
		{
			if (group->inspect || group->variant == WEB_QUERY_PROSEC)
			{
				electric_name(container, source, TYPE_TEXT);
				type = exist(source) ? TYPE_TEXT : TYPE_IMAGE;
			}
			else
				type = TYPE_HTML;
		}

		electric_name(container, source, type);

		if (exist(source))
		{
			char target[SIZE_OF_PATH];

			strcpy(target, group->dir);
			char *relname = target + strlen(target);

			if (WEB_SCHEME % WEB_SCHEME_FLAT)
			{
				CDate date = group->minDate;	// rem Today

				scprintf(target, "%04ld", date.year);
				if (WEB_SCHEME == WEB_SCHEME_WEEK)
				{
					int dow = CDate(1, 1, date.year).DayOfWeek();
					int week = (date.DayOfYear() + dow + 5) / 7;
					scprintf(target, "00%02d\\", week);
				}
				else
					scprintf(target, "%02d%02d\\", date.month, WEB_SCHEME & WEB_SCHEME_MMDD ? date.day : 0);
			}

			electric_name(container, target, target, type);

			if (WEB_SCHEME)
				*strrchr(relname, '\\') = '_';

			if (copy_overead_file(source, target, false, true))
				if (type == TYPE_HTML)
					html_group_action(source, copy_overead_file, target);

			if (exist(target))
			{
				if (!linkText)
					linkText = type == TYPE_IMAGE ? "Графичен документ" : "Текстов документ";

				m.cat(prefix);
				for (char *url = relname; (url = strchr(url, '\\')) != NULL; *url = '/');
				m.printf("\aa href=\"%s\"\b%s\a/a\b", relname, linkText);
				return true;
			}
		}
	}

	if (always)
		m.printf("%s%s", prefix, linkText);

	return false;
}

static void emit_connected(mstr &m, const TWebSession *session, TOutReg &outReg, char instance, mstr &remark,
	const TQueryWebGroup *webGroup, bool interprete)
{
	TConnect connect;
	sign linked = -1;

	connect.key = outReg.key;
	connect.connectType = instance;

	if (connect.Try())
	{
		if (connect.decisionKind && !connect.decisionDate.Empty())
		{
			linked = interprete && emit_link(m, webGroup, &outReg, "\v", Kind->Seek(connect.decisionKind), true);
			m.printf(" от %D", &connect.decisionDate);
		}

		if (connect.connectKind && connect.connectNo && connect.connectYear)
			m.printf("\v%s № %s/%ld", ConnectKind->Seek(connect.connectKind), connect.connectNo, connect.connectYear);
	}

	if (linked == -1 && interprete)
		emit_link(m, webGroup, &outReg, "\v", NULL, false);

	if (webGroup->appeal == TRUE && any(remark))
	{
		// Copied from below
		TDecision decision;
		decision << session;
		bool personal = decision.PersonalData();

		m.cat("\v");
		session->personal->Filter(remark, webGroup->filtered << personal);
		m.cat(remark);
	}

	if (!any(m))
		m.cat("\x06");
}

static void emit_appeal_force(mstr &m, const TWebSession *session, const TQueryWebGroup *webGroup, bool linked)
{
	if (webGroup->appeal)	// 12.11.2012
	{
		msql w;
		TOutReg outReg;

		session->OutRegCriteria(w, "O.*");
		TQuery q(str(w));

		while (q.Read())
		{
			outReg << q;
			outReg.date = outReg.returned;	// for electrics

			if (strchr(RESULT_COMPLECTS, outReg.result) && !strchr(RESULT_SERVICES, outReg.result))
				continue;

			bool both = outReg.resultI != RESOLUTION_FAKE && outReg.resultKS != RESOLUTION_FAKE;

			if (outReg.resultI != RESOLUTION_FAKE)
			{
				m.printf("\v\vРезултат от въззивна инстанция: %s", Resolution->Seek(outReg.resultI));
				emit_connected(m, session, outReg, CONNECT_TYPE_VZ_LAWSUIT, outReg.remarkVZ, webGroup, !both);
			}

			if (outReg.resultKS != RESOLUTION_FAKE)
			{
				m.printf("\v\vРезултат от касационна инстанция: %s", Resolution->Seek(outReg.resultKS));
				emit_connected(m, session, outReg, CONNECT_TYPE_KS_LAWSUIT, outReg.remarkKS, webGroup, !both);
			}

			if (both)
			{
				if (webGroup->variant == WEB_QUERY_ACQUIT)
				{
					linked = emit_link(m, webGroup, &outReg, "\v\v", NULL, false, TYPE_TEXT);

					if (emit_link(m, webGroup, &outReg, "\v\v" + linked, NULL, false, TYPE_IMAGE))
						linked = true;
				}
				else
				{
					// 2014:164 FIX: FINISHED and FORCED are public -> filtered
					linked = emit_link(m, webGroup, &outReg, "\v\v", "Обезличен документ", false, TYPE_HTML);
				}

				if (linked && !session->forceDate.Empty())
					m.cat("\v");
			}
		}
	}

	session->forceDate.Print(m, "\vВ законна сила на %Dг.");
}

#define queryweb OFFICE_GROUP_CAST(TQueryWebGroup)

static void webTitle_lawsuit_kinds(mstr &m, TGroup *group)
{
	print_kinds_list(m, queryweb->kinds, false, false);
}

static void webTitle_lawsuit_types(mstr &m, TGroup *group)
{
	switch (queryweb->variant)
	{
		case WEB_QUERY_PUBLIC : m.cpy("съдебните актове, подлежащи на публикуване"); break;
		case WEB_QUERY_PROSEC : m.cpy("делата с изготвени мотиви"); break;
		case WEB_QUERY_PROTOCOL : m.cpy("протоколите от О.С.З, подлежащи на публикуване"); break;
		default : m.cpy("влезлите в законна сила дела");
	}
}

static void webTitle_important(mstr &m, TGroup *group)
{
	if (queryweb->important)
		m.cpy("от обществен интерес");
}

static void webTitle_min_date(mstr &m, TGroup *group) { queryweb->minDate.Print(m); }
static void webTitle_max_date(mstr &m, TGroup *group) { queryweb->maxDate.Print(m); }

static void webTitle_suitors_title(mstr &m, TGroup *)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	m.cpy("Обвинител, ищец, жалбо - подател");
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_ADMIN
	m.cpy("Жалбоподател, ищец");
#endif  // ADMIN
}

static void webTitle_defendants_title(mstr &m, TGroup *)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	m.cpy("Обвиняем, ответник или ответник на жалбата");
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_ADMIN
	m.cpy("Ответник");
#endif  // ADMIN
}

static void webTitle_judge_title(mstr &m, TGroup *) { m.cpy("Съдия - докладчик и председател на съдебния състав"); }

#undef queryweb

static const offield webTitle_fields[] =
{
	{ "F_LAWSUIT_TYPES", webTitle_lawsuit_types },
	{ "F_LAWSUIT_KINDS", webTitle_lawsuit_kinds },
	{ "F_IMPORTANT", webTitle_important },
	{ "F_MIN_DATE", webTitle_min_date },
	{ "F_MAX_DATE", webTitle_max_date },
	{ "F_SUITORS_TITLE", webTitle_suitors_title },
	{ "F_DEFENDANTS_TITLE", webTitle_defendants_title },
	{ "F_JUDGE_TITLE", webTitle_judge_title },
	{ NULL, NULL }
};

#define session OFFICE_GROUP_CAST(TWebSession)

static void web_lawsuit_key(mstr &m, TGroup *group)
{
	m.printf("%s No %d/%d", Kind->Seek(session->lawKind), flag_key_no(&session->key), session->key.year);

	if (PRINT_COMPOSITIONS)
		m.printf(", %s състав", Composition->Seek(session->composition));
}

static void web_subject(mstr &m, TGroup *group) { m.cpy(Subject->Seek(session->key.type, session->subject)); }

static void web_suitors(mstr &m, TGroup *group)
{
	query_web_side_list(m, session->sideWinds, session->categories, SIDEWIND_LEFT, session->group->filtered);
}

static void web_defendants(mstr &m, TGroup *group)
{
	query_web_side_list(m, session->sideWinds, session->categories, SIDEWIND_RIGHT, session->group->filtered);
}

static void web_judge(mstr &m, TGroup *group)
{
	char name[SIZE_OF_USER_NAME];

	// 2013:273 IRQ/TRQ: kill president for 1st instance
#if COURT_TYPE == COURT_APPEAL
	if (atob(session->president))
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (atob(session->president) && !strchr(KIND_1ST_LAWSUITS, session->lawKind))
#endif  // ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	{
		m.cpy("Председател");

		if (!strcmp(session->president, session->judge))
		{
			m.cat(" и докладчик");
			*session->judge = '\0';
		}

		print_user(name, session->president);
		m.printf(": \v%s", name);
		// 2013:273 LPR: separator
		if (atob(session->judge))
			m.puts("\v");
	}
#endif  // APPEAL || ADMIN || AREAL || MILITARY

	if (atob(session->judge))
	{
		print_user(name, session->judge);
		m.printf("Докладчик:\v%s", name);
	}

	if (!any(m))
		m.cat("\x06");
}

static void web_session_date(mstr &m, TGroup *group)
{
	session->date.Print(m);
	m.cat(" ");
	session->time.Print(m);

	bool endate = !session->end.Empty() && session->end != session->date;

	if (endate || !session->entime.Empty())
	{
		m.cat(" до ");

		if (endate)
			session->end.Print(m);
		m.cat(" ");
		session->entime.Print(m);
	}

	if (session->hall)
	{
		Hall->Seek(session->hall);
		m.cat(", ");

		if (!strstr(Hall->name, "ЗАЛА"))
			m.cat("Зала ");

		m.cat(Hall->name);
	}
}

static void web_ses_result(mstr &m, TGroup *group)
{
	const TQueryWebGroup *webGroup = session->group;
	bool linked = emit_link(m, webGroup, session, "", "Протокол", true);
	bool text = webGroup->text == TRUE || (webGroup->text == -1 && !linked);

	m.printf( " от %Dг.", &session->date);

	if (text && any(session->text))
	{
		session->personal->Filter(session->text, webGroup->filtered << (session->key.type == TYPE_PUNISHMENT_LAW));
		m.printf("\v%s", str(session->text));
	}

	emit_appeal_force(m, session, webGroup, linked);

	if (!any(m))
		m.cat("\x06");
}

static void web_result(mstr &m, TGroup *group)
{
	char kindName[SIZE_OF_NAME];
	strcpy(kindName, session->kind ? Kind->Seek(session->kind) : "Съдебен акт");

	bool closed = strchr(KIND_CLOSED_SESSIONS, session->sessionKind);
	bool linked = emit_link(m, session->group, session, "", kindName, !closed);
	const TQueryWebGroup *webGroup = session->group;

	if (!linked && closed)
	{
		char kind = session->kind;

		session->kind = session->sessionKind;
		linked = emit_link(m, webGroup, session, "", kindName, true);
		session->kind = kind;
	}

	if (!session->fromDate.Empty())
		session->fromDate.Print(m, " от %Dг.");

	// WTF wrote this?
	TDecision decision;
	decision << session;

	bool personal = decision.PersonalData();
	bool text = webGroup->text == TRUE || (webGroup->text == -1 && !linked);

	if (text && any(session->text))
	{
		session->personal->Filter(session->text, webGroup->filtered << personal);
		m.printf("\v%s", str(session->text));
	}

	if (personal)
	{
		TJudgement judgement;
		TSideWind *sideWind;

		for (int i = 0; i < session->sideWinds.Count(); i++)
		{
			if (session->categories[i] != SIDEWIND_RIGHT)
				continue;

			sideWind = session->sideWinds[i];

			judgement << sideWind;
			judgement.key = session->key;
			judgement.kind = session->kind;
			judgement.date = session->date;

			if (judgement.Try("F_UCN, F_UCN_TYPE, F_TEXT"))
			{
				m.cat("\v");
				print_side_wind(m, &judgement, webGroup->filtered);

				if (text && any(judgement.text))
				{
					session->personal->Filter(judgement.text, webGroup->filtered << 1);
					m.printf("\v%s", str(judgement.text));
				}
			}
		}
	}

	if (any(session->opinion))
	{
		char kind = session->kind;

		session->kind = KIND_OPINION;
		linked = emit_link(m, webGroup, session, "\v", "Особено мнение", true);
		session->kind = kind;

		if (webGroup->text == TRUE || (webGroup->text == -1 && !linked))
		{
			session->personal->Filter(session->opinion, webGroup->filtered << personal);
			m.printf("\v%s", str(session->opinion));
		}
	}

	if ((session->flags & DECISION_MOTIVES) && !session->returned.Empty())
	{
		char kind = session->kind;

		session->kind = KIND_MOTIVES;
		linked = emit_link(m, webGroup, session, "\v", "Мотиви", true);
		session->kind = kind;
		session->returned.Print(m, " от %Dг.");

		if (any(session->motives) && webGroup->text == TRUE || (webGroup->text == -1 && !linked))
		{
			session->personal->Filter(session->motives, webGroup->filtered << 1);
			m.printf("\v%s", str(session->motives));
		}
	}

	// 2019-11-12: LPR: separate function
	emit_appeal_force(m, session, webGroup, linked);

	if (webGroup->variant == WEB_QUERY_REINVEST)
	{
		TRequest request;
		bool linked = false;

		request.date.Clear();

		if (session->source.type == TYPE_REQUEST)
		{
			request.key = session->source;
			request.kind = session->sourceKind;

			if (request.Get("F_DATE"))
				linked = emit_link(m, webGroup, &request, "\v\v", Kind->Seek(session->sourceKind), false);
		}

		if (!linked)
			m.printf("\v\v%s", Kind->Seek(session->sourceKind));

		request.date.Print(m, " от %D");
	}

	if (!any(m))
		m.cat("\x06");
}

static void web_judgement_name(mstr &m, TGroup *group)
{
	if (atol(session->ucn))
		print_name(m, session->ucn, session->ucnType);
	else
		m.cat("\x06");
}

static void web_judgement_ucn(mstr &m, TGroup *group)
{
	if (is_real_ucn(session->ucnType, session->ucn))
		m.cpy(session->ucn);
	else
		m.cat("\x06");
}

static void web_pol_result(mstr &m, TGroup *group)
{
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	bool showResult = true;

	if (strchr(KIND_1ST_LAWSUITS, session->lawKind))
		showResult = false;

	if (showResult)
#endif  // ADMIN || AREAL || MILITARY
	{
		m.cpy(Result->Seek(session->result));
	}

	m.printf("\v%s", Kind->Seek(session->kind));

	if (any(session->text))
		m.printf("\v%s", str(session->text));

	if (any(session->judText))
		m.printf("\v%s", str(session->judText));
}

#undef session

static const offield web_fields[]=
{
	{ "F_LAWSUIT_KEY", web_lawsuit_key },
	{ "F_SUBJECT", web_subject },
	{ "F_SUITORS", web_suitors },
	{ "F_DEFENDANTS", web_defendants },
	{ "F_JUDGE", web_judge },
	{ "F_SESSION_DATE", web_session_date },
	{ "F_RESULT", web_result },
	{ "F_JUDGEMENT_NAME", web_judgement_name },
	{ "F_JUDGEMENT_UCN", web_judgement_ucn },
	{ "F_POL_RESULT", web_pol_result },
	{ "F_SES_RESULT", web_ses_result },	
	{ NULL, NULL }
};

class TFloatWeb : public TFloatQuery
{
public:
	TFloatWeb(TWindow *parent, TQueryWebGroup *tGroup, const char *tName, const char *tTitle);

	TWebSession session;
	TQueryWebGroup *group;

	msql m;
	void Execute();
	virtual void Count();
	void SetJudgement(TWebSession *session, TJudgement *judgement);
};

TFloatWeb::TFloatWeb(TWindow *parent, TQueryWebGroup *tGroup, const char *tName, const char *tTitle)
	: TFloatQuery(parent, NULL, tName, tTitle), group(tGroup)
{
	group->FormCriteria(m);
}

void TFloatWeb::Execute()
{
	if (office.Open(name, OFFICE_COPY_ALL, group->name))
	{
		tr1start = office.Find("tr1 start");
		tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, webTitle_fields, group);
	#if SPEEDTESTING
		#pragma message speed testing only
		DWORD ticks = GetTickCount();
	#endif  // SPEEDTESTING
		Count();
	#if SPEEDTESTING
		message("test version: %lu d-seconds\n", (GetTickCount() - ticks) / 100);
	#endif  // SPEEDTESTING
		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(wait);
		Debug();
	}
}

void TFloatWeb::SetJudgement(TWebSession *session, TJudgement *judgement)
{
	if (judgement)
	{
		strcpy(session->ucn, judgement->ucn);
		session->ucnType = judgement->ucnType;
		session->judText.cpy(judgement->text);
	}
	else
	{
		*session->ucn = '\0';
		session->ucnType = '\0';
		session->judText.clear();
	}
}

void TFloatWeb::Count()
{
	session.group = group;
	TQuery q(str(m));

	while (q.Read())
	{
		Increment();
		session << q;
		session.LoadSides();

		if (group->limited || !(session.lawFlags & LAWSUIT_LIMITED))
		{
			if (group->police)
			{
				msql m;
				bool printed = false;
				TJudgement judgement;

				judgement << session;
				judgement.FloatCriteria(m, "F_UCN, F_UCN_TYPE, F_TEXT");

				TQuery r(str(m));
				while (r.Read())
				{
					printed = true;
					judgement << r;
					SetJudgement(&session, &judgement);

					office.Emit(tr1start, tr1final, web_fields, &session);
					SetJudgement(&session, NULL);
				}

				if (!printed)
					office.Emit(tr1start, tr1final, web_fields, &session);
			}
			else
				office.Emit(tr1start, tr1final, web_fields, &session);
		}

		delete[] session.categories;
	}
}

static void update_date(const CDate &dialogDate, CDate &configDate, const char *name)
{
	if (!configDate.Empty() && dialogDate != configDate)
	{
		mstr m;

		configDate = dialogDate;
		configDate.Print(m);
		TConfigure::Update("application", name, str(m));
	}
}

// ----- entry point ---------------------------------------------------------
#include "QueryWebDialog.h"
#include "QueryWebDialog.cpp"

void query_web_html(TWindow *parent)
{
	TQueryWebGroup group;

	if (TQueryWebDialog(parent, &group).Execute() == IDOK)
	{
		// 2013:246
		if (group.variant == WEB_QUERY_FIXED)
		{
			update_date(group.minDate, WEB_FIXED_START_DATE, "WEB_FIXED_START_DATE");
			update_date(group.maxDate, WEB_FIXED_FINAL_DATE, "WEB_FIXED_FINAL_DATE");
		}
		else
		{
			update_date(group.minDate, WEB_QUERY_START_DATE, "WEB_QUERY_START_DATE");
			update_date(group.maxDate, WEB_QUERY_FINAL_DATE, "WEB_QUERY_FINAL_DATE");
		}

		group.fixed = (group.variant == WEB_QUERY_FIXED);
		group.police = (group.variant == WEB_QUERY_POLICE);
		group.inspect = (group.variant == WEB_QUERY_ACQUIT || group.variant == WEB_QUERY_REINVEST);
		group.protocol = (group.variant == WEB_QUERY_PROTOCOL);

		// 2014:043
		if (!any(group.kinds))
		{
	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
			strcpy(group.kinds, group.inspect ? KIND_PUNISHMENT_LAWS : KIND_LAWSUITS);
	#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	#if COURT_TYPE == COURT_ADMIN
			strcpy(group.kinds, KIND_LAWSUITS);
	#endif  // ADMIN
		}

		if (group.attached)
		{
			strcpy(group.dir, group.name);
			*basename(group.dir) = '\0';
		}

		if (group.police)
		{
			static constant PolDec[2] = { "PolDecided.htm", "PolDec.htm" };
			TFloatWeb(parent, &group, PolDec[WEB_QUERY_USE_STYLES], "Справка за РДВР").Execute();
		}
		else if (group.inspect)
		{
			TFloatWeb(parent, &group, group.variant == WEB_QUERY_ACQUIT ? "Acquit.htm" : "Reinvest.htm",
				"Справка за инспектората").Execute();
		}
		else
		{
			static constant WebFix[2] = { "WebFixed.htm", "WebFix.htm" };
			static constant WebDec[2] = { "WebDecided.htm", "WebDec.htm" };
			static constant WebSes[2] = { "WebSession.htm", "WebSes.htm" };
			constant *const names = group.fixed ? WebFix : group.protocol ? WebSes : WebDec;

			TFloatWeb(parent, &group, names[WEB_QUERY_USE_STYLES], "Справка за WEB страница").Execute();
		}
	}
}

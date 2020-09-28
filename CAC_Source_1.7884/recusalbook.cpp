#include "cac.h"

// ----- TRecusalBookGroup ---------------------------------------------------
class TRecusalBookGroup : public TBookGroup
{
public:
	TRecusalBookGroup();

	virtual void FormCriteria(msql &m);
	virtual void Title(mstr &m);

	char compositions[SIZE_OF_COMPOSITIONS];
	long filtered;
	long recujed;
};

TRecusalBookGroup::TRecusalBookGroup()
	: TBookGroup("T_SESSION S JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE",
		"S.F_TYPE", "S.F_DATE", false, false)
{
	Add(
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TCheck("F_FILTERED", &filtered, TRUE),
		new TCheck("F_RECUJED", &recujed, TRUE),
		NULL
	);

	fields = "S.*, L.F_KIND AS F_LAW_KIND";
	*types = '\0';
	by = BOOK_BY_PERIOD;
	//recujed = -1;
}

void TRecusalBookGroup::FormCriteria(msql &m)
{
	TBookGroup::FormCriteria(m);

	m.AddChars("S.F_RESULT", recujed ? RESULT_RECUJEDS : RESULT_RECUSALS);
	m.AddCompos("S.F_COMPOSITION", compositions);

	if (recujed)
		m.AddFlags("S.F_FLAGS_X", SESSION_X_RECUJED, SESSION_X_RECUJED, SESSION_X_ALL);
}

void TRecusalBookGroup::Title(mstr &m)
{
	book_types(m, this);
	m.cat("\v");
	book_dates(m, this);
}

// ----- TRecusalSession -----------------------------------------------------
class TRecusalSession : public TSession
{
public:
	TRecusalSession() { Add(new TCharAlias("F_LAW_KIND", &lawKind)); }

	bool operator<=(const TRecusalSession &that);

	char lawKind;
	bool filtered;
};

bool TRecusalSession::operator<=(const TRecusalSession &that)
{
	// 2015:056 LRQ/IRQ: order by date, lawsuit, time/kind
	return date != that.date ? date < that.date : key.year != that.key.year ? key.year < that.key.year :
		lawKind != that.lawKind ? lawKind < that.lawKind : key.no != that.key.no ? key.no < that.key.no :
		TRCDKeyContainer::operator<=(that);
}

// ----- TRecusalBook --------------------------------------------------------
class TRecusalBook : public TBook
{
public:
	TRecusalBook(TWindow *parent, TRecusalBookGroup *group);

	virtual TGroup *Process(TQuery &q);

	static const offield fields[];
};

TRecusalBook::TRecusalBook(TWindow *parent, TRecusalBookGroup *group)
	: TBook(parent, group, "RecusalReg.htm", TRecusalBook::fields, ::book_fields, BOOK_FIXED_DATES,
		new TGSArray<TRecusalSession>)
{
}

TGroup *TRecusalBook::Process(TQuery &q)
{
	TRecusalSession *session = new TRecusalSession;

	*session << q;
	session->filtered = ((TRecusalBookGroup *) group)->filtered;
	return session;
}

#define recses OFFICE_GROUP_CAST(TRecusalSession)

static void recses_kind(mstr &m, TGroup *group)
{
	if (strchr(KIND_OPEN_SESSIONS, recses->kind))
		m.cat("О.С.З");
	else
	{
		Kind->Seek(recses->kind);
		m.cat(Kind->alias);
	}
}

static void recses_law_kind(mstr &m, TGroup *group) { Kind->Seek(recses->lawKind); m.cat(Kind->alias); }

static void recses_judges(mstr &m, TGroup *group)
{
	m.cat("Докладчик: ");
	print_name(m, recses->judge, UCN_CITIZEN_UCN);

#if COURT_TYPE == COURT_APPEAL
	if (atob(recses->president))
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (atob(recses->president) && strchr(KIND_2ND_LAWSUITS, recses->lawKind))
#endif  // ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	if (atob(recses->president) && strcmp(recses->president, recses->judge))
#endif  // REGIONAL
	{
		m.cat("\vПредседател: ");
		print_name(m, recses->president, UCN_CITIZEN_UCN);
	}

	if (atob(recses->judgeX))
	{
		m.cat("\vСъдия: ");
		print_name(m, recses->judgeX, UCN_CITIZEN_UCN);
	}

	if (atob(recses->reJudgeX))
	{
		m.cat("\vСъдия: ");
		print_name(m, recses->reJudgeX, UCN_CITIZEN_UCN);
	}
}

static void recses_text(mstr &m, TGroup *group)
{
	if (recses->filtered)
		filter_personal(recses, recses->text, 1 << (recses->key.type == TYPE_PUNISHMENT_LAW));

	m.cat(recses->text);
}

static void recses_future(mstr &m, TGroup *group)
{
	msql w(recses->Name, "F_KIND, F_DATE, F_TIME, F_JUDGE");

	nearest_event_crit(w, recses, true, true);
	w.AddChar("NOT F_RESULT", RESULT_FIXED);
	w.AddFlags("F_FLAGS", SESSION_CHANJED, SESSION_CHANJED, SESSION_ALL);

	TSession *const session = TYPECHECK_PTR_CAST(TSession, nearest_event_find(w, recses, true));

	if (session)
	{
		TVirtPtr VP(session);
		print_name(m, session->judge, UCN_CITIZEN_UCN);
	}
}

#undef recses

const offield TRecusalBook::fields[] =
{
	{ "F_KIND",		recses_kind },
	{ "F_DATE",		NULL },
	{ "F_RETURNED",	NULL },
	{ "F_LAW_KIND",	recses_law_kind },
	{ "F_NO",		NULL },
	{ "F_YEAR",		NULL },
	{ "F_JUDGES",	recses_judges },
	{ "F_TEXT",		recses_text },
	{ "F_FUTURE",	recses_future },
	{ NULL, NULL }
};

#include "RecusalBookDialog.h"
#include "RecusalBookDialog.cpp"

void recusal_book(TWindow *parent)
{
	TRecusalBookGroup group;

	if (TRecusalBookDialog(parent, &group).Execute() == IDOK)
		TRecusalBook(parent, &group).Execute(BOOK_EXEC_HEADER);
}

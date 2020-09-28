#include "cac.h"

// ----- TPeriodUCNGroup -----------------------------------------------------
TPeriodUCNGroup::TPeriodUCNGroup(const char *name)
	: TFloatGroup(name)
{
	Add(
		new TDate("F_MIN_DATE", &minDate, true),
		new TDate("F_MAX_DATE", &maxDate, true),
		new TString("F_UCN", ucn, SIZE_OF_UCN),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		NULL
	);
}

void TPeriodUCNGroup::InnerCriteria(msql &m)
{
	m.AddString("W.F_UCN", ucn);
	m.AddChar("W.F_UCN_TYPE", ucnType);
	m.AddRange("W.F_DATE", minDate, maxDate);
}

void TPeriodUCNGroup::Title(mstr &m, const char *prefix, const char *infix)
{
	TSideWind sideWind;

	m.cpy(prefix);
	m.cach(' ');
	sideWind << this;
	sideWind.PrintNames(m);
	m.cach(' ');
	m.cat(infix);
	minDate.Print(m, " от %D");
	maxDate.Print(m, " до %D");
}

// ----- TPeriodSessionGroup -------------------------------------------------
class TPeriodSessionGroup : public TPeriodUCNGroup
{
public:
	TPeriodSessionGroup();

	virtual void FormCriteria(msql &m);
};

TPeriodSessionGroup::TPeriodSessionGroup()
	: TPeriodUCNGroup("T_SIDE_WIND W JOIN T_SESSION S ON S.F_NO = W.F_NO AND S.F_YEAR = W.F_YEAR "
		"AND S.F_TYPE = W.F_TYPE AND S.F_KIND = W.F_KIND AND S.F_DATE = W.F_DATE "
		"JOIN T_LAWSUIT L ON L.F_NO = W.F_NO AND L.F_YEAR = W.F_YEAR AND L.F_TYPE = W.F_TYPE")
{
}

void TPeriodSessionGroup::FormCriteria(msql &m)
{
	m.Begin(Name, "F_NO, F_YEAR, F_TYPE, L.F_KIND AS F_LAW_KIND, S.F_JUDGE AS F_SES_JUDGE, "
		"L.F_FINISHED AS F_LAW_FINISHED, W.F_KIND AS F_SES_KIND, W.F_DATE AS F_SES_DATE, "
		"S.F_RESULT AS F_SES_RESULT, S.F_RETURNED AS F_SES_RETURNED");
	InnerCriteria(m);
	m.AddOrder("W.F_DATE, F_TYPE, F_YEAR, F_NO");	// == EndCriteria()
}

// ----- float ---------------------------------------------------------------
void period_session(TWindow *parent)
{
	TFoundGroup ucnGroup;

	if (get_ucn(parent, &ucnGroup, UCN_DATED_SIDES))
	{
		TPeriodGroup period("T_SESSION");

		period.minDate = CDate(1, Today.month, Today.year);
		if (get_period(parent, &period))
		{
			msql m, title;
			TPeriodSessionGroup group;

			group << ucnGroup;
			group << period;
			group.FormCriteria(m);

			// 2009:079 LPR: title support
			group.Title(title, "«аседани€ с участието на", "в периода");
			query_session_tab(parent, str(m), str(title), false, false);
		}
	}
}

// ----- TUnwrittenSession -------------------------------------------------------------
class TUnwrittenSession : public TSession
{
public:
	TUnwrittenSession();

	char lawKind;
	long subject;
};

TUnwrittenSession::TUnwrittenSession()
{
	Add(
		new TCharAlias("F_LAW_KIND", &lawKind),
		new TLong("F_SUBJECT", &subject),
		NULL
	);
}

// ----- TUnwrittenSesGroup --------------------------------------------------
class TUnwrittenSesGroup : public TUnwrittenGroup
{
public:
	TUnwrittenSesGroup();

	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_TYPES];
	long byActDate;
};

TUnwrittenSesGroup::TUnwrittenSesGroup()
	// 2014:210 FB2: nested selects -> join, see below
	: TUnwrittenGroup("T_SESSION S JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE",
		"S.F_NO, S.F_YEAR, S.F_TYPE, S.F_KIND, S.F_DATE, S.F_JUDGE, S.F_RESULT, S.F_RETURNED, "
		"L.F_KIND AS F_LAW_KIND, L.F_SUBJECT")	// 2018-01-10: was "L.F_SUBJECT AS F_FLAGS"
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TRadio("F_BY_SES_DATE", &byActDate, false),
		new TRadio("F_BY_ACT_DATE", &byActDate, true),
		NULL
	);
}

void TUnwrittenSesGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddChars("S.F_TYPE", types, TYPE_LAWSUITS);
	m.AddArray("F_UCN", judges, "S.F_JUDGE");
	m.AddChars("S.F_KIND", adjust_xxx_kinds(kinds, KIND_VISIBLE_CLOSEDS, KIND_CLOSED_SESSIONS), KIND_SESSIONS);
	// 2014:210 FB2: nested select -> joined
	m.AddArray("L.F_SUBJECT", newSubjects);
	m.AddArray("F_SUBJECT", oldSubjects, "L.F_OLD_SUBJECT");
	m.Add("S.F_DATE");
	m.printf("< %ld", date.Value());
	m.AddOrder("S.F_JUDGE, S.F_TYPE, S.F_YEAR, S.F_NO, S.F_DATE");
}

// ----- TUnwrittenSesQuery ------------------------------------------------------------
class TUnwrittenSesQuery : public TFloatQuery
{
public:
	TUnwrittenSesQuery(TWindow *parent, const char *criteria, const char *title, const CDate &tMaxActDate);

protected:
	const CDate &maxActDate;

	virtual void Count();
};

TUnwrittenSesQuery::TUnwrittenSesQuery(TWindow *parent, const char *criteria, const char *title, const CDate &tMaxActDate)
	: TFloatQuery(parent, criteria, "QueryUws.htm", title), maxActDate(tMaxActDate)
{
}

#define session OFFICE_GROUP_CAST(TUnwrittenSession)

static void floatuws_ses_judge(mstr &m, TGroup *group) { print_user(m, session->judge); }
static void floatuws_college(mstr &m, TGroup *group) { m.printf("%c", *College->Seek(session->key.type)); }
static void floatuws_law_subject(mstr &m, TGroup *group) { m.cat(TSubject::Code(session->subject, session->lawKind)); }
static void floatuws_ses_returned(mstr &m, TGroup *group) { session->returned.Print(m); }

#undef session

static const offield floatuws_fields[]=
{
	{ "F_SES_JUDGE", floatuws_ses_judge },
	{ "F_COLLEGE", floatuws_college },
	{ "F_NO", NULL },
	{ "F_YEAR", NULL },
	{ "F_LAW_SUBJECT", floatuws_law_subject },
	{ "F_SES_DATE", container_date },
	{ "F_SES_RETURNED", floatuws_ses_returned },
	{ NULL, NULL }
};

void TUnwrittenSesQuery::Count()
{
	TUnwrittenSession session;
	TQuery q(criteria);

	while (q.Read())
	{
		session << q;
		// 2014:210 FB2: loop select -> joined
		if (unwritten_act(&session, maxActDate) && Increment())
			office.Emit(tr1start, tr1final, floatuws_fields, &session);
	}
}

#include "UnwrittenSesDialog.h"
#include "UnwrittenSesDialog.cpp"

// ----- query ---------------------------------------------------------------
void unwritten_ses(TWindow *parent)
{
	TUnwrittenSesGroup group;
	TUnwrittenSesDialog(parent, &group).Execute();
}

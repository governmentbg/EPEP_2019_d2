#include "cac.h"

// ----- TUnfinishedLawGroup -------------------------------------------------
enum { ORDER_BY_JUDGE = ORDER_BY_USER };

class TUnfinishedLawGroup : public TFloatGroup
{
public:
	TUnfinishedLawGroup();

	virtual void FormCriteria(msql &m);
	void Title(mstr &m);

	char kinds[SIZE_OF_KINDS];
	TGArray<TSubject> newSubjects;
	TGArray<TOldSubject> oldSubjects;
	char generalKinds[SIZE_OF_KINDS];
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	TGroupArray senders;
#endif  // APPEAL || ADMIN || AREAL || MILITARY
	CDate finished;
	// 2006:052 LRQ/FIX: moved compositions from region to common +related
	char compositions[SIZE_OF_COMPOSITIONS];
	TGroupArray judges;
	long reJudge;
	long ageFirst;
	long age;
};

// 2017:046 lawsuits -> intervals
TUnfinishedLawGroup::TUnfinishedLawGroup()
	// 2014:210 FB2: nested selects -> join, see below
	: TFloatGroup("T_LAW_INTERVAL")	// 2017:207
{
	Add(
		new TCharList("F_KINDS", kinds, sizeof kinds),
		new TCharList("F_GENERAL_KINDS", generalKinds, sizeof generalKinds),
		new TDate("F_FINISHED", &finished, true),
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TCheck("F_REJUDGE", &reJudge, TRUE),
		new TCheck("F_AGE_FIRST", &ageFirst, TRUE),
		new TRadio("F_AGE_ALLS", &age, AGE_NONE_YET),
		new TRadio("F_AGE_THREE_OR_LESS", &age, AGE_THREE_OR_LESS),
		new TRadio("F_AGE_THREE_TO_SIX", &age, AGE_THREE_TO_SIX),
		new TRadio("F_AGE_SIX_TO_TWELVE", &age, AGE_SIX_TO_TWELVE),
		new TRadio("F_AGE_MORE_THAN_TWELVE", &age, AGE_MORE_THAN_TWELVE),
		new TRadio("F_BY_JUDGE", &order, ORDER_BY_JUDGE),		
		NULL
	);

	age = AGE_NONE_YET;
	order = ORDER_BY_TKYN;
}

void TUnfinishedLawGroup::FormCriteria(msql &m)
{
	mstr tables("T_LAW_INTERVAL I JOIN T_LAWSUIT L ON L.F_NO = I.F_NO AND L.F_YEAR = I.F_YEAR AND L.F_TYPE = I.F_TYPE "
		"LEFT JOIN T_CONNECT C ON C.F_NO = I.F_NO AND C.F_YEAR = I.F_YEAR AND C.F_TYPE = I.F_TYPE");
	constant judgeField = reJudge ? "L.F_REJUDGE" : "L.F_JUDGE";

	if (order == ORDER_BY_JUDGE)
	{
		// 2017:234 LPR: judgeField -> L.F_JUDGE, as query always displays judge
		tables.printf(" JOIN T_CITIZEN Z ON Z.F_UCN = %s AND Z.F_UCN_TYPE = '%c'", "L.F_JUDGE", UCN_CITIZEN_UCN);
	}

	m.Begin(str(tables), "I.F_NO, I.F_YEAR, I.F_TYPE, L.F_KIND, L.F_SUBJECT, L.F_JUDGE, I.F_DATE, I.F_FINISHED, "
		"L.F_OLD_SUBJECT, L.F_DATE AS F_STARTED");

	m.AddChars("L.F_KIND", kinds, KIND_LAWSUITS);
	m.AddArray("L.F_SUBJECT", newSubjects);
	m.AddArray("F_SUBJECT", oldSubjects, "L.F_OLD_SUBJECT");
	m.AddChars("L.F_SOURCE_KIND", adjust_general_kinds(generalKinds), KIND_ALLDOCS);

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	// 2014:210 FB2: nested select -> joined
	// 2015:169 LPR: custom -> AddPairs()
	m.AddPairs("F_UCN", "F_UCN_TYPE", senders, "C.F_UCN", "C.F_UCN_TYPE");
#endif  // APPEAL || ADMIN || AREAL || MILITARY
	m.Add("I.F_DATE");
	// 2010:284 LRQ: "<" -> "<="
	m.printf("<= %ld", finished.Value());

	//finished += 1;	// 2011:138 LRQ
	//char s[0x400];
	//non_finished_firm_law_patch(s, sizeof s, any(kinds) ? kinds : KIND_LAWSUITS, &finished);
	//m.cat(s);
	m.AddIsNull("(I.F_FINISHED", true);
	m.NextOr();
	m.Add("I.F_FINISHED");
	m.printf("> %ld)", finished.Value());
#if COURT_TYPE == COURT_AREAL
	if (strchr(kinds, KIND_FIRM_LAW))
	{
		m.AddChar("(NOT I.F_TYPE", TYPE_FIRM_LAW);
		m.NextOr();
		m.AddChar("(I.F_TYPE", TYPE_FIRM_LAW);
		// 2014:219 CHANGES: for VOS only
		if (COURT_CODE == BUILDIN_AREAL_COURT)
		{
			m.Add("I.F_DATE");
			m.printf(">= %ld", VARNA_FIRM_LAWS_NOT_FIN_DATE.Value());
		}
		m.AddChars("(SELECT COUNT(F_NO) FROM T_DECISION D WHERE D.F_NO = I.F_NO AND D.F_YEAR = I.F_YEAR "
			"AND D.F_TYPE = I.F_TYPE AND D.F_RESULT", RESULT_FIRM_REGISTRATIONS);
		m.Add("D.F_ENTER_DATE");
		m.printf("<= %ld) = 0))", finished.Value());
	}
#endif  // AREAL

	m.AddCompos("L.F_COMPOSITION", compositions);
	m.AddArray("F_UCN", judges, judgeField);

	if (order == ORDER_BY_JUDGE)
		m.AddOrder("Z.F_NAME, Z.F_RENAME, Z.F_FAMILY");
	else
		EndCriteria(m, "L.");
}

void TUnfinishedLawGroup::Title(mstr &m)
{
	m.cpy("Несвършени ");

	if (any(kinds))
		print_kinds_list(m, kinds, false, false);
	else
		m.cat("дела");

	finished.Print(m, "\vкъм %D");
}

// ----- TUnfinishedLawsuit --------------------------------------------------
TUnfinishedLawsuit::TUnfinishedLawsuit(const CDate &tFinished)
	: finished(tFinished)
{
	Add(new TDate("F_STARTED", &started, false));
}

long TUnfinishedLawsuit::Age()
{
	const int interval = interval2Months(started, finished);

	switch (interval)
	{
		case 0 :
		case 1 :
		case 2 :
		case 3 : return AGE_THREE_OR_LESS;
		case 4 :
		case 5 :
		case 6 : return AGE_THREE_TO_SIX;
	}

	return (interval <= 12) ? AGE_SIX_TO_TWELVE : AGE_MORE_THAN_TWELVE;
}

void TUnfinishedLawsuit::FindPrevious(const TLawsuit *lawsuit)
{
	TConnect connect;
	msql m(connect.Name, "F_CONNECT_NO, F_CONNECT_YEAR, F_CONNECT_KIND");

	m.Add(lawsuit, "F_NO, F_YEAR, F_TYPE");
	m.AddChars("F_CONNECT_TYPE", CONNECT_TYPE_LAW_LINKS);
	TQuery q(str(m));

	while (q.Read())
	{
		connect << q;
		TLawsuit *tLawsuit = new TLawsuit;

		tLawsuit->key.no = atoi(connect.connectNo);
		tLawsuit->key.year = connect.connectYear;
		tLawsuit->key.type = connectKind2Type(connect.connectKind);

		if (!tLawsuit->Try("F_DATE"))
			tLawsuit->date.Clear();
		else if (tLawsuit->date < started)
			started = tLawsuit->date;

		for (int i = 0; i < previous.Count(); i++)
		{
			TLawsuit *tPrevious = previous[i];

			if (tPrevious->key == tLawsuit->key)
			{
				m.clear();
				m.printf("%s %ld/%ld: зациклени дела на други институции/инстанции.\n\n",
					Type->Seek(key.type), key.no, key.year);
				m.printf("%s %ld/%ldг. сочи обратно към ", Type->Seek(lawsuit->key.type), lawsuit->key.no,
					lawsuit->key.year);
				error("%s %s %ld/%ldг.", str(m), Type->Seek(tPrevious->key.type), tPrevious->key.no,
					tPrevious->key.year);
				return;
			}
		}

		previous.Add(tLawsuit);
		// 2013:198 LPR: check for missing type
		if (tLawsuit->key.type)
			FindPrevious(tLawsuit);
	}
}

// ----- TUnfinishedLawQuery -------------------------------------------------
class TUnfinishedLawQuery : public TFloatQuery
{
public:
	TUnfinishedLawQuery(TWindow *parent, const char *criteria, const char *title, TUnfinishedLawGroup *tGroup);

protected:
	TUnfinishedLawGroup *group;

	virtual void Count();
};

TUnfinishedLawQuery::TUnfinishedLawQuery(TWindow *parent, const char *criteria, const char *title,
	TUnfinishedLawGroup *tGroup)
	: TFloatQuery(parent, criteria, "FloatUnfin.htm", title), group(tGroup)
{
}

#define lawsuit OFFICE_GROUP_CAST(TUnfinishedLawsuit)

static void unfinlaw_kind(mstr &m, TGroup *group) { m.cat(Kind->Seek(lawsuit->kind)); }
static void unfinlaw_judge(mstr &m, TGroup *group) { print_user(m, lawsuit->judge); }

static void unfinlaw_subject(mstr &m, TGroup *group)
{
	m.cat(lawsuit->Code());
	print_old_subject(m, lawsuit->oldSubject);
}

static void unfinlaw_started(mstr &m, TGroup *group) { m.printf("%D", &lawsuit->date); }
static void unfinlaw_age(mstr &m, TGroup *group) { m.cat(Age->Seek((char) lawsuit->Age())); }

static void unfinlaw_previous_lawsuits(mstr &m, TGroup *group)
{
	for (int i = 0; i < lawsuit->previous.Count(); i++)
	{
		TLawsuit *tLawsuit = lawsuit->previous[i];

		m.sep("\v");
		// 2013:198 LPR: check for missing type
		m.printf("%s %ld/%ld", tLawsuit->key.type ? Type->Seek(tLawsuit->key.type) : "Дело", tLawsuit->key.no,
			tLawsuit->key.year);

		if (!tLawsuit->date.Empty())
			m.printf(", образувано на %0D", &tLawsuit->date);
	}
}

#undef lawsuit

static const offield unfinlaw_fields[] =
{
	{ "F_KIND",		unfinlaw_kind },
	{ "F_NO",		NULL },
	{ "F_YEAR",		NULL },
	{ "F_JUDGE",	unfinlaw_judge },
	{ "F_SUBJECT",	unfinlaw_subject },
	{ "F_STARTED",	unfinlaw_started },
	{ "F_FINISHED",	NULL },
	{ "F_AGE",		unfinlaw_age },
	{ "F_PREVIOUS_LAWSUITS", unfinlaw_previous_lawsuits },
	{ NULL, NULL }
};

void TUnfinishedLawQuery::Count()
{
	TQuery q(criteria);
	TUnfinishedLawsuit lawsuit(group->finished);
	TLawsuit previous;

	while (q.Read())
	{
		lawsuit << q;
		//lawsuit.started = lawsuit.date;	// 2017:207 should be auto now
		lawsuit.previous.Flush();

		if (group->ageFirst)
			lawsuit.FindPrevious();

		// 2014:223 FIX: avoid duplicate print caused by N connect join
		if ((group->age == AGE_NONE_YET || lawsuit.Age() == group->age) && lawsuit.key != previous.key)
		{
			office.Emit(tr1start, tr1final, unfinlaw_fields, &lawsuit);
			previous.key = lawsuit.key;
			// 2013:254 FIX: count all records -> only matching age
			if (!Increment())
				break;
		}
	}
}

// ----- query ---------------------------------------------------------------
#include "UnfinishedLawDialog.h"
#include "UnfinishedLawDialog.cpp"

void unfinished_law(TWindow *parent)
{
	TUnfinishedLawGroup group;
	TUnfinishedLawDialog(parent, &group).Execute();
}

// ----- TUnwrittenGroup -----------------------------------------------------
TUnwrittenGroup::TUnwrittenGroup(const char *name, const char *fields)
	: TFloatGroup(name, fields)
{
	Add(
		new TCharList("F_KINDS", kinds, sizeof kinds),
		new TDate("F_DATE", &date, true),
		NULL
	);
}

void TUnwrittenGroup::Title(mstr &m)
{
	date.Print(m, " с ненаписани съдебни актове към %D от ");

	if (judges.Count())
	{
		for (int i = 0; i < judges.Count(); i++)
		{
			if (i)
				m.cat(", ");

			judges[i]->Print(m);
		}
	}
	else
		m.cat("всички съдии");
}

// ----- TUnwrittenActGroup --------------------------------------------------
class TUnwrittenActGroup : public TUnwrittenGroup
{
public:
	TUnwrittenActGroup();

	virtual void FormCriteria(msql &m);
};

TUnwrittenActGroup::TUnwrittenActGroup()
	: TUnwrittenGroup("T_LAWSUIT")
{
	// 2001:082 PRQ; REM: 2+ TYN entry may not be present, subject ignored
	order = ORDER_BY_JTYN;
}

void TUnwrittenActGroup::FormCriteria(msql &m)
{
	// 2018-06-05: -F_DATE -F_FINISHED
	m.Begin(Name, "F_NO, F_YEAR, F_TYPE, F_SUBJECT, F_JUDGE");
	m.AddArray("F_UCN", judges, "F_JUDGE");
	m.AddChars("F_KIND", kinds, KIND_LAWSUITS);
	m.AddArray("F_SUBJECT", newSubjects);
	m.AddArray("F_SUBJECT", oldSubjects, "F_OLD_SUBJECT");
	EndCriteria(m);
}

// ----- query ---------------------------------------------------------------
bool unwritten_act(const TSession *session, const CDate &maxActDate)
{
	if (!strchr(RESULT_ENTERABLES, session->result))
		return false;

#if COURT_TYPE == COURT_AREAL
	// 2010:111
	if (session->result == RESULT_LENDERS_MEETING)
		return false;
#endif  // AREAL

	// First, we need to know is there a movement undo after the session
	msql m(session->Name, "F_NO");
	// 2004:218 LRQ: F_DATE greater _or equal to_
	// 2008:340 LRQ: RESULT_FINALIZEDS, not only UNDO; F_DATE > OR F_DATE ==
	m.Add(session, "F_NO, F_YEAR, F_TYPE");
	m.Add("(F_DATE");
	// 2017:220 LPR: plain text -> construct
	//m.printf("> %ld OR (F_DATE = %ld AND NOT F_KIND = '%c'))", session->date.Value(), session->date.Value(), session->kind);
	m.printf("> %ld", session->date.Value());
	m.NextOr();
	m.AddDate("(F_DATE", session->date);
	m.AddChar("NOT F_KIND", session->kind);
	m.cat("))");

	mstr results(RESULT_FINALIZEDS);	// 2017:220 was char []

	results.cat(RESULT_UNDOS);
	results.cat(RESULT_RECUSALS);		// 2017:220 PRQ: p.33
	m.AddChars("F_RESULT", str(results));

	TQuery q(str(m));
	if (q.Read())
		return false;	// undo found - session does not count

	// Second, see if no acts exist || at least one of them is incomplete
	TGArray<TDecision> decisions;
	bool writtenAct = false;

	try2decide(session, &decisions, NULL, "F_FLAGS, F_FROM_DATE, F_RETURNED");

	// 2018-01-10: maxActDate support and cleaner check
	for (int i = 0; i < decisions.Count(); i++)
	{
		const TDecision *decision = decisions[i];

		if (decision->fromDate < maxActDate)
		{
			if ((decision->flags & DECISION_MOTIVES) && decision->returned.Empty())
				return true;  // an unwritten act

			writtenAct = true;
		}
	}

	return !writtenAct;  // no suitable acts
}

#define lawsuit OFFICE_GROUP_CAST(TLawsuit)

static void floatuwa_law_judge(mstr &m, TGroup *group) { print_user(m, lawsuit->judge); }
static void floatuwa_college(mstr &m, TGroup *group) { m.cach(*College->Seek(lawsuit->key.type)); }
static void floatuwa_law_subject(mstr &m, TGroup *group) { m.cat(lawsuit->Code()); }
static void floatuwa_law_finished(mstr &m, TGroup *group) { lawsuit->finished.Print(m); }

#undef lawsuit

static const offield floatuwa_fields[]=
{
	{ "F_LAW_JUDGE", floatuwa_law_judge },
	{ "F_COLLEGE", floatuwa_college },
	{ "F_NO", NULL },
	{ "F_YEAR", NULL },
	{ "F_LAW_SUBJECT", floatuwa_law_subject },
	{ "F_LAW_DATE", container_date },
	{ "F_LAW_FINISHED", floatuwa_law_finished },
	{ NULL, NULL }
};

class TUnwrittenActQuery : public TFloatQuery
{
public:
	TUnwrittenActQuery(TWindow *parent, const char *criteria, const char *title, CDate *tDate);

protected:
	void Count();

	CDate *date;
};

TUnwrittenActQuery::TUnwrittenActQuery(TWindow *parent, const char *criteria, const char *title, CDate *tDate)
	: TFloatQuery(parent, criteria, "QueryUwa.htm", title), date(tDate)
{
}

void TUnwrittenActQuery::Count()
{
	TLawsuit lawsuit;
	TSession session;
	TQuery q(criteria);
	int found = 0;

	while (q.Read())
	{
		msql m(session.Name, "F_KIND, F_DATE, F_RESULT");

		lawsuit << q;
		m.Add(&lawsuit, "F_NO, F_YEAR, F_TYPE");
		m.Add("F_DATE");
		m.printf("< %ld", date->Value());
		m.AddChars("F_RESULT", RESULT_ENTERABLES);

		TQuery p(str(m));
		while (p.Read())
		{
			session << p;
			session.key = lawsuit.key;
			Increment();

			if (unwritten_act(&session))
			{
				TLawInterval interval;  // 2018-06-05

				interval.key = lawsuit.key;
				interval.date = session.date;

				interval.Read("F_DATE, F_FINISHED", false);
				lawsuit.date = interval.date;
				lawsuit.finished = interval.finished;

				found++;
				office.Emit(tr1start, tr1final, floatuwa_fields, &lawsuit);
				break;	// to the next lawsuit
			}
		}
	}

	count = found;
}

#include "UnwrittenActDialog.h"
#include "UnwrittenActDialog.cpp"

void unwritten_act(TWindow *parent)
{
	TUnwrittenActGroup group;
	TUnwrittenActDialog(parent, &group).Execute();
}

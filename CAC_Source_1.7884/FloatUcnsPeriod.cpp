#include "cac.h"

enum
{
	NON_FIN_LAW,
	CREATED_LAW,
	FIN_LAW_PERIOD,
	SES_PERIOD
};

// ----- TFloatUCNsGroup -----------------------------------------------------
class TFloatUCNsGroup : public TFloatGroup
{
public:
	TFloatUCNsGroup();

	virtual void FormCriteria(msql &m);
	void PrintTitle(mstr &title);

	TGArray<TUCNGroup> *ucnGroups;

	CDate date;
	CDate minDate;
	CDate maxDate;
	long floatType;
};

TFloatUCNsGroup::TFloatUCNsGroup()
	: TFloatGroup("T_UCNS"), ucnGroups(NULL)
{
	Add(
		new TDate("F_DATE", &date, false),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TRadio("F_NON_FIN_LAW", &floatType, NON_FIN_LAW),
		new TRadio("F_CREATED_LAW", &floatType, CREATED_LAW),
		new TRadio("F_FIN_LAW_PERIOD", &floatType, FIN_LAW_PERIOD),
		new TRadio("F_SES_PERIOD", &floatType, SES_PERIOD),
		NULL
	);

	date = Today;
	minDate = Today;
	maxDate = Today;
	floatType = NON_FIN_LAW;
}

void TFloatUCNsGroup::FormCriteria(msql &m)
{
	m.AddChars("S.F_TYPE", TYPE_LAWSUITS);

	if (floatType == NON_FIN_LAW)
	{
		m.Add("(L.F_FINISHED");
		m.printf(">= %ld", date.Value());
		m.NextOr();
		m.AddIsNull("L.F_FINISHED", true);
		m.cat(")");
		// 2018:028 copied from unwritten
		m.Add("L.F_DATE");
		m.printf("<= %ld", date.Value());
	}
	else
	{
		m.AddRange(floatType == FIN_LAW_PERIOD ? "L.F_FINISHED" :
			floatType == CREATED_LAW ? "L.F_DATE" : "SES.F_DATE", minDate, maxDate);
	}
}

void TFloatUCNsGroup::PrintTitle(mstr &title)
{
	if (floatType == NON_FIN_LAW)
		title.printf("Несвършени дела към дата %0D", &date);
	else
	{
		static constant titles[] = { "Образувани дела", "Свършили дела", "Заседания" };

		title.printf("%s в периода %0D - %0D", titles[floatType - CREATED_LAW], &minDate, &maxDate);
	}
}

// ----- TUCNsPeriodLaw ------------------------------------------------------
class TUCNsPeriodLaw : public TLawsuit
{
public:
	TUCNsPeriodLaw();

	bool operator<=(TUCNsPeriodLaw &that);

	char ucnType;
	char ucn[SIZE_OF_UCN];
};

TUCNsPeriodLaw::TUCNsPeriodLaw()
{
	Add(
		new TString("F_UCN", ucn, sizeof ucn),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		NULL
	);
}

bool TUCNsPeriodLaw::operator<=(TUCNsPeriodLaw &that)
{
	// 2013:140 LPR: added type
	return key.year != that.key.year ? key.year < that.key.year : key.no != that.key.no ? key.no < that.key.no :
		key.type <= that.key.type;
}

// ----- TUCNsPeriodSes ------------------------------------------------------
class TUCNsPeriodSes : public TSession
{
public:

	TUCNsPeriodSes();

	bool operator<=(TUCNsPeriodSes &that);	// 2013:140

	char lawKind;
	CDate lawDate;
	CDate lawFinished;
	char lawJudge[SIZE_OF_UCN];
	long subject;
};

TUCNsPeriodSes::TUCNsPeriodSes()
{
	Add(
		new TCharAlias("F_LAW_KIND", &lawKind),
		new TDate("F_LAW_DATE", &lawDate, false),
		new TDate("F_LAW_FINISHED", &lawFinished, false),
		new TPersonAlias("F_LAW_JUDGE", lawJudge, sizeof lawJudge),
		new TLongAlias("F_SUBJECT", &subject),
		NULL
	);
}

bool TUCNsPeriodSes::operator<=(TUCNsPeriodSes &that)
{
	return date != that.date ? date < that.date : key.year != that.key.year ? key.year < that.key.year :
		key.no != that.key.no ? key.no < that.key.no : key.type <= that.key.type;
}

static void print_names(mstr &m, const char *ucn, char ucnType)
{
	TCitizen citizen;

	strcpy(citizen.ucn, ucn);
	citizen.ucnType = ucnType;

	if (citizen.Try())
	{
		m.printf(" %s", citizen.name);
		if (any(citizen.reName)) m.printf(" %s", citizen.reName);
		m.printf(" %s", citizen.family);
		if (any(citizen.reFamily)) m.printf("-%s", citizen.reFamily);
	}
	else
		print_name(m, ucn, ucnType);
}

#define lawsuit OFFICE_GROUP_CAST(TUCNsPeriodLaw)

static void floatside_law_kind(mstr &m, TGroup *group) { m.cat(KindAlias->Seek(lawsuit->kind)); }
static void floatside_no(mstr &m, TGroup *group) { itom(lawsuit->key.no, m); }
static void floatside_year(mstr &m, TGroup *group) { itom(lawsuit->key.year, m); }
static void floatside_judge(mstr &m, TGroup *group) { print_names(m, lawsuit->judge, UCN_CITIZEN_UCN); }

static void floatside_subject(mstr &m, TGroup *group)
{
	if (lawsuit->subject)
	{
		Subject->Seek(lawsuit->key.type, lawsuit->subject);
		m.printf("%s %s", lawsuit->Code(), Subject->name);
	}
}

static void floatside_create(mstr &m, TGroup *group) { lawsuit->date.Print(m); }
static void floatside_finished(mstr &m, TGroup *group) { lawsuit->finished.Print(m); }

static void floatside_young_side(mstr &m, TGroup *group)
{
	TSideWind sideWind;

	sideWind << group;
	sideWind.Print(m);

	msql w(sideWind.Name, "DISTINCT F_INVOLVEMENT");
	bool first = true;

	w.Add(&sideWind, "F_NO, F_YEAR, F_TYPE, F_UCN, F_UCN_TYPE");
	TQuery q(str(w));

	while (q.Read())
	{
		sideWind << q;
		m.printf("%s %s", first ? " - " : ", ", Involvement->Seek(sideWind.involvement));
	}
}

#undef lawsuit

static const offield floatSide_fields[] =
{
	{ "F_LAW_KIND", floatside_law_kind },
	{ "F_NO", floatside_no },
	{ "F_YEAR", floatside_year },
	{ "F_LAW_JUDGE", floatside_judge },
	{ "F_SUBJECT", floatside_subject },
	{ "F_LAW_CREATE", floatside_create },
	{ "F_LAW_FINISHED", floatside_finished },
	{ "F_YOUNG_SIDE", floatside_young_side },
	{ NULL, NULL }
};

#define session OFFICE_GROUP_CAST(TUCNsPeriodSes)

static void floatses_law_kind(mstr &m, TGroup *group) { m.cat(KindAlias->Seek(session->lawKind)); }
static void floatses_no(mstr &m, TGroup *group) { itom(session->key.no, m); }
static void floatses_year(mstr &m, TGroup *group) { itom(session->key.year, m); }
static void floatses_create(mstr &m, TGroup *group) { session->lawDate.Print(m); }
static void floatses_judge(mstr &m, TGroup *group) { print_names(m, session->judge, UCN_CITIZEN_UCN); }
static void floatses_finished(mstr &m, TGroup *group) { session->lawFinished.Print(m); }
static void floatses_kind(mstr &m, TGroup *group) { m.cat(Kind->Seek(session->kind)); }
static void floatses_date(mstr &m, TGroup *group) { session->date.Print(m); }
static void floatses_result(mstr &m, TGroup *group) { m.printf("%s", Result->Seek(session->result)); }
static void floatses_returned(mstr &m, TGroup *group) { session->returned.Print(m); }
static void floatses_force_date(mstr &m, TGroup *group) { session->forceDate.Print(m); }
static void floatses_law_judge(mstr &m, TGroup *group) { print_names(m, session->lawJudge, UCN_CITIZEN_UCN); }

static void floatses_subject(mstr &m, TGroup *group)
{
	Subject->Seek(session->key.type, session->subject);
	m.printf("%d %s", session->subject, Subject->name);
}

static void floatses_young_sides(mstr &m, TGroup *group)
{
	TSideWind sideWind;
	msql w;

	sideWind << group;
	sideWind.FloatCriteria(w);
	w.CutOrder();
	w.AddFlags("F_FLAGS", SIDE_WIND_YOUNG, SIDE_WIND_YOUNG, SIDE_WIND_ALL);
	TQuery q(str(w));

	while (q.Read())
	{
		m.sep("\v");
		sideWind << q;
		sideWind.PrintGroup(m);
		m.printf(" - %s", Involvement->Seek(sideWind.involvement));
	}
}

#undef session

static const offield floatSes_fields[] =
{
	{ "F_LAW_KIND", floatses_law_kind },
	{ "F_NO", floatses_no },
	{ "F_YEAR", floatses_year },
	{ "F_LAW_CREATE", floatses_create },
	{ "F_SES_JUDGE", floatses_judge },
	{ "F_LAW_FINISHED", floatses_finished },
	{ "F_SES_KIND", floatses_kind },
	{ "F_SES_DATE", floatses_date },
	{ "F_SES_RESULT", floatses_result },
	{ "F_SES_RETURNED", floatses_returned },
	{ "F_SES_FORCE_DATE", floatses_force_date },
	{ "F_LAW_JUDGE", floatses_law_judge },
	{ "F_SUBJECT", floatses_subject },
	{ "F_YOUNG_SIDES", floatses_young_sides },	
	{ NULL, NULL }
};

// --------------- TFloatUCNsQuery -------------------------------------------
class TFloatUCNsQuery : public TFloatQuery
{
public:
	TFloatUCNsQuery(TWindow *parent, const char *title, TFloatUCNsGroup *tGroup, const char *name);

	virtual void Execute();
	virtual void Count();

protected:
	bool printLaws;
	TFloatUCNsGroup *group;
	TGroupArray *array;

	void BeginCriteria(msql &m);
	void ReadGroups(const char *criteria);
};

TFloatUCNsQuery::TFloatUCNsQuery(TWindow *parent, const char *title, TFloatUCNsGroup *tGroup, const char *name)
	: TFloatQuery(parent, NULL, name, title), printLaws(tGroup->floatType != SES_PERIOD), group(tGroup)
{
}

void TFloatUCNsQuery::BeginCriteria(msql &m)
{
	// 2017:010 FIX: +S.F_FLAGS for minor letter
	if (printLaws)
	{
		m.Begin("T_SIDE_WIND S JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR "
			"AND L.F_TYPE = S.F_TYPE", "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_DATE, L.F_JUDGE, "
			"L.F_SUBJECT, L.F_FINISHED, S.F_UCN, S.F_UCN_TYPE, S.F_FLAGS");
	}
	else
	{
		m.Begin("T_SIDE_WIND S JOIN T_SESSION SES ON SES.F_NO = S.F_NO AND SES.F_YEAR = S.F_YEAR "
			"AND SES.F_TYPE = S.F_TYPE JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR "
			"AND L.F_TYPE = S.F_TYPE", "DISTINCT SES.*, L.F_KIND AS F_LAW_KIND, L.F_DATE AS F_LAW_DATE, "
			// 2013:107 FIX: +F_LAW_JUDGE +F_SUBJECT
			"L.F_FINISHED AS F_LAW_FINISHED, L.F_JUDGE AS F_LAW_JUDGE, L.F_SUBJECT, S.F_FLAGS");
	}
}

void TFloatUCNsQuery::ReadGroups(const char *criteria)
{
	TQuery q(criteria);
	while (q.Read())
	{
		TGroup *group = printLaws ? (TGroup *) new TUCNsPeriodLaw : (TGroup *) new TUCNsPeriodSes;

		*group << q;
		array->Add(group);
		Increment();
	}
}

void TFloatUCNsQuery::Execute()
{
	TWindowHideParent hide(parent);

	if (office.Open(name))
	{
		TGSArray<TUCNsPeriodLaw> lawsuits;
		TGSArray<TUCNsPeriodSes> sessions;
		tr1start = office.Find("tr1 start");
		tr1final = office.Find("tr1 final");

		array = printLaws ? (TGroupArray *) &lawsuits : (TGroupArray *) &sessions;
		office.Emit(NULL, tr1start, float_fields, this);

		if (group->ucnGroups)
		{
			for (int i = 0; i < group->ucnGroups->Count(); i++)
			{
				msql m;
				TUCNGroup *ucnGroup = (*group->ucnGroups)[i];

				BeginCriteria(m);
				m.AddString("S.F_UCN", ucnGroup->ucn);
				m.AddChar("S.F_UCN_TYPE", ucnGroup->ucnType);
				group->FormCriteria(m);
				ReadGroups(str(m));
			}
		}
		else
		{
			msql m;

			BeginCriteria(m);
			m.AddFlags("S.F_FLAGS", SIDE_WIND_YOUNG, SIDE_WIND_YOUNG, SIDE_WIND_ALL);
			group->FormCriteria(m);
			ReadGroups(str(m));
		}

		Count();
		office.Emit(tr1final, NULL, float_fields, this);
		Show();
	}
}

void TFloatUCNsQuery::Count()
{
	for (int i = 0; i < array->Count(); i++)
		office.Emit(tr1start, tr1final, printLaws ? floatSide_fields : floatSes_fields, (*array)[i]);
}

// ----- functions -----------------------------------------------------------
#include "FloatUcnsPeriodDialog.h"
#include "FloatUcnsPeriodDialog.cpp"

void float_ucns_period(TWindow *parent, TGArray<TUCNGroup> *ucnGroups)
{
	TFloatUCNsGroup group;

	if (TFloatUCNsPeriodDialog(parent, &group).Execute() == IDOK)
	{
		static constant names[] = { "FloatSide.htm", "FloatSes.htm" };
		mstr title;

		group.PrintTitle(title);
		title.cat(ucnGroups->Count() > 1 ? " за лицата" : " за лицето");

		for (int i = 0; i < ucnGroups->Count(); i++)
		{
			TUCNGroup *ucnGroup = (*ucnGroups)[i];

			title.printf("\v%s %s ", UCNType->Seek(ucnGroup->ucnType), ucnGroup->ucn);
			print_names(title, ucnGroup->ucn, ucnGroup->ucnType);
		}

		group.ucnGroups = ucnGroups;
		TFloatUCNsQuery(parent, str(title), &group, names[group.floatType == SES_PERIOD]).Execute();
	}
}

void float_young_period(TWindow *parent)
{
	TFloatUCNsGroup group;

	if (TFloatUCNsPeriodDialog(parent, &group).Execute() == IDOK)
	{
		static constant names[] = { "YoungSide.htm", "YoungSes.htm" };
		mstr title;

		group.PrintTitle(title);
		title.cat(" с участието на малолетни/непълнолетни лица");
		TFloatUCNsQuery(parent, str(title), &group, names[group.floatType == SES_PERIOD]).Execute();
	}
}

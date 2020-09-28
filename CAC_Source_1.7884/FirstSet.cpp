#include "cac.h"

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
enum
{
	FIRST_SET_ALL		= 2,
	FIRST_SET_MONTH		= 3,
	FIRST_SET_MONTH_12	= 4,
	FIRST_SET_MONTH_23	= 5,
	FIRST_SET_MONTHS		= 6
};

// ----- TFirstSetGroup ------------------------------------------------------
class TFirstSetGroup : public TFloatGroup
{
public:
	TFirstSetGroup();

	virtual void FormCriteria(msql &m);

	char kinds[SIZE_OF_KINDS];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	TGArray<TSubject> newSubjects;
	TGArray<TOldSubject> oldSubjects;
	CDate minDate;
	CDate maxDate;
	long doublex;
	long change;
	long quick;
	long firstSet;
};

TFirstSetGroup::TFirstSetGroup()
#if 20180214
	: TFloatGroup("T_LAW_INTERVAL I JOIN T_LAWSUIT L ON L.F_NO = I.F_NO AND L.F_YEAR = I.F_YEAR AND L.F_TYPE = I.F_TYPE",
		"I.F_NO, I.F_YEAR, I.F_TYPE, L.F_KIND, L.F_SUBJECT, I.F_DATE, I.F_FIRST_SET")
#else  // 20180214
	: TFloatGroup("T_LAWSUIT L JOIN T_LAW_INTERVAL I ON I.F_NO = L.F_NO AND I.F_YEAR = L.F_YEAR AND I.F_TYPE = L.F_TYPE")
#endif  // 20180214
{
	Add(
		new TCharList("F_KINDS", kinds, sizeof kinds),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TCheck("F_DOUBLEX", &doublex, LAWSUIT_X_DOUBLEX),
		new TCheck("F_FIRST_SET", &firstSet, TRUE),
		new TRadio("F_SET_ALL", &change, FIRST_SET_ALL),
		new TRadio("F_SET_MONTH", &change, FIRST_SET_MONTH),
		new TRadio("F_SET_MONTH_2", &change, FIRST_SET_MONTH_12),
		new TRadio("F_SET_MONTH_3", &change, FIRST_SET_MONTH_23),
		new TRadio("F_SET_MONTHS", &change, FIRST_SET_MONTHS),
		new TRadio("F_SLOW", &quick, QUICK_NULL),
		new TRadio("F_QUICK", &quick, QUICK_QUICK),
		new TRadio("F_ANY_SPEED", &quick, QUICK_ANY_SPEED),
		NULL
	);

	quick = QUICK_ANY_SPEED;
	change = FIRST_SET_ALL;
	firstSet = TRUE;
}

void TFirstSetGroup::FormCriteria(msql &m)
{
#if 20180214
	// 2018-02-14: LPR/IRQ: leading intervals
	m.Begin(Name, fields);
	m.AddRange("I.F_NO", minNo, maxNo);
	m.AddRange("I.F_YEAR", minYear, maxYear);
	m.AddRange("I.F_FIRST_SET", minDate, maxDate);
	// 2018-02-14: LPR: was KIND_FIRST_SET_LAWS
	m.AddChars("L.F_KIND", kinds, KIND_EXCHANGE_LAWS);
#else  // 20180214
	m.Begin(Name, "L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_SUBJECT, I.F_DATE, I.F_FIRST_SET");
	m.AddChars("L.F_KIND", kinds, KIND_FIRST_SET_LAWS);
	m.AddRange("L.F_NO", minNo, maxNo);
	m.AddRange("L.F_YEAR", minYear, maxYear);
	m.AddRange("I.F_FIRST_SET", minDate, maxDate);
#endif  // 20180214
	m.AddArray("L.F_SUBJECT", newSubjects);
	m.AddArray("F_SUBJECT", oldSubjects, "L.F_OLD_SUBJECT");
	m.AddChars("L.F_SOURCE_KIND", KIND_EXCHANGE_INDOCS, KIND_INDOCS);
	m.AddQuick("L.F_QUICK", quick);
#if 20180214
	// 2018-02-14: LPR: -1 for doublex, firstSet
	if (doublex != -1)
		m.AddFlags("L.F_FLAGS_X", LAWSUIT_X_DOUBLEX, doublex, LAWSUIT_X_ALL);
	if (firstSet != -1)
		m.AddFlags("I.F_FLAGS", LAW_INTERVAL_SECONDARY, firstSet ? 0 : LAW_INTERVAL_SECONDARY, LAW_INTERVAL_ALL);
	m.AddOrder("I.F_NO, I.F_YEAR, I.F_TYPE, I.F_DATE");
#else  // 20180214
	m.AddFlags("L.F_FLAGS_X", LAWSUIT_X_DOUBLEX, doublex, LAWSUIT_X_ALL);
	m.AddFlags("I.F_FLAGS", LAW_INTERVAL_SECONDARY, firstSet ? 0 : LAW_INTERVAL_SECONDARY, LAW_INTERVAL_ALL);
	m.AddOrder("L.F_NO, L.F_YEAR, L.F_TYPE, I.F_DATE");
#endif  // 20180214
}

// ----- query ---------------------------------------------------------------
#define lawsuit OFFICE_GROUP_CAST(TLawsuit)

static void first_type(mstr &m, TGroup *group) { Type->Seek(lawsuit->key.type); m.cpy(Type->alias); }
static void first_subject(mstr &m, TGroup *group) { m.cat(lawsuit->Code()); }
static void first_date(mstr &m, TGroup *group) { lawsuit->date.Print(m); }
static void first_first_set(mstr &m, TGroup *group) { lawsuit->firstSet.Print(m); }
static void first_period(mstr &m, TGroup *group) { m.cpy(lawsuit->text); }

#undef lawsuit

static const offield first_fields[] =
{
	{ "F_TYPE", first_type },
	{ "F_NO", keygroup_key_no },
	{ "F_YEAR", keygroup_key_year },
	{ "F_SUBJECT", first_subject },
	{ "F_DATE", first_date },
	{ "F_FIRST_SET", first_first_set },
	{ "F_PERIOD", first_period },
	{ NULL, NULL }
};

class TFirstSetQuery : public TFloatQuery
{
public:
	TFirstSetQuery(TWindow *parent, const char *criteria, TFirstSetGroup *tGroup, long tFirstSet, const CDate &tMinDate,
		const CDate &tMaxDate);

	virtual void Count();

	CDate minDate;
	CDate maxDate;
	long firstSet;
	TFirstSetGroup *group;
};

TFirstSetQuery::TFirstSetQuery(TWindow *parent, const char *criteria, TFirstSetGroup *tGroup, long tFirstSet,
	const CDate &tMinDate, const CDate &tMaxDate)
	: TFloatQuery(parent, criteria, "FirstSet.htm", "времетраене на размяната на книжа"), group(tGroup),
		firstSet(tFirstSet), minDate(tMinDate), maxDate(tMaxDate)
{
}

void TFirstSetQuery::Count()
{
	static int changes[] = { 0, FIRST_SET_MONTH, FIRST_SET_MONTH_12, FIRST_SET_MONTH_23, FIRST_SET_MONTHS };
	static constant texts[] = { "", "До 1 месец", "1-2 месеца", "2-3 месеца", "Над 3 месеца" };
	long matching = 0;	// 2015:189 FIX: display matching count, not query

	TLawsuit lawsuit;
	TQuery q(criteria);

	while (q.Read() && Increment())
	{
		lawsuit << q;
		int delta = interval2Months(lawsuit.date, lawsuit.firstSet);

		if (delta < 1)
			delta = 1;
		else if (delta > 4)
			delta = 4;

		if (group->change == FIRST_SET_ALL || changes[delta] == group->change)
		{
			strcpy(lawsuit.text, texts[delta]);
			office.Emit(tr1start, tr1final, first_fields, &lawsuit);
			matching++;
		}
	}

	count = matching;
}

static void first_set(TWindow *parent, const char *criteria, TFirstSetGroup *group)
{
	TFirstSetQuery(parent, criteria, group, group->firstSet, group->minDate, group->maxDate).Execute();
}

#include "FirstSetDialog.h"
#include "FirstSetDialog.cpp"

void first_set(TWindow *parent)
{
	TFirstSetGroup group;
	TFirstSetDialog(parent, &group).Execute();
}
#endif  // AREAL || REGIONAL

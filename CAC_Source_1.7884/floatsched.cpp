#include "cac.h"

struct THTMField
{
	const char *tr1start;
	const char *tr2start;
	const char *tr2final;
	const char *sm1break;
	const char *tr1final;
};

#define title OFFICE_GROUP_CAST(TSched)

static void title_title_1(mstr &m, TGroup *group) { m.cpy(Type->Seek(title->key.type)); }

static void title_title_2(mstr &m, TGroup *group)
{
	m.printf("към %D, %s\n", &title->date, Type->Seek(title->key.type));
}

static void title_sched(mstr &m, TGroup *group) { m.cpy(CalInterval->Seek(title->sched)); }

static void title_count(mstr &m, TGroup *group)
{
	if (title->key.no > 0)
	{
		if (title->key.no == 1)
			m.cpy("1 срок");
		else
			m.printf("%d срока", title->key.no);
	}
}

static void title_total(mstr &m, TGroup *group)
{
	if (title->key.no > 0)
	{
		if (title->key.no == 1)
			m.cpy("Общо един срок.");
		else
			m.printf("Общо %d срока.", title->key.no);
	}
	else if (title->key.no == 0)
		m.cpy("Няма срокове.");
}

#undef title

static const offield title_fields[] =
{
	{ "F_TITLE_1", title_title_1 },
	{ "F_TITLE_2", title_title_2 },
	{ "F_SCHED", title_sched },
	{ "F_COUNT", title_count },
	{ "F_TOTAL", title_total },
	{ NULL, NULL }
};


#define tSched OFFICE_GROUP_CAST(TSched)

static void sched_lawsuit_key(mstr &m, TGroup *group) { m.printf("%d/%d", tSched->lawsuit.no, tSched->lawsuit.year); }
static void sched_judge(mstr &m, TGroup *group) { if (any(tSched->judge)) print_user(m, tSched->judge); }
static void sched_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(tSched->kind)); }

static void sched_key(mstr &m, TGroup *group)
{
	if (strchr(KIND_SESSIONS, tSched->kind) || strchr(KIND_ENDOCS, tSched->kind) || strchr(KIND_LAWSUITS, tSched->kind))
		m.cpy("");	// NOP
	else
		m.printf("%d/%d", tSched->key.no, tSched->key.year);
}

static void sched_judge_action(mstr &m, TGroup *group) { m.cpy(CalInterval->Seek(tSched->sched)); }
static void sched_start_date(mstr &m, TGroup *group) { tSched->start.Print(m); }
static void sched_end_date(mstr &m, TGroup *group) { tSched->final.Print(m); }
static void sched_count_name(mstr &m, TGroup *group) { m.cpy(Type->Seek(tSched->lawsuit.type)); }

#undef tSched

static const offield sched_fields[] =
{
	{ "F_LAWSUIT_KEY", sched_lawsuit_key },
	{ "F_KIND", sched_kind },
	{ "F_JUDGE", sched_judge },
	{ "F_KEY", sched_key },
	{ "F_DATE", NULL },
	{ "F_JUDGE_ACTION", sched_judge_action },
	{ "F_START_DATE", sched_start_date },
	{ "F_END_DATE", sched_end_date },
	{ "F_COUNT_NAME", sched_count_name },
	{ NULL, NULL }
};

// ----- TExpireSchedGroup ---------------------------------------------------
TExpireSchedGroup::TExpireSchedGroup(const char *colleges)
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TDate("F_DATE", &date, true),
		NULL
	);

	// 2016:299
	//strcpy(types, QUERY_COLLEGES);
	strcpy(types, colleges);
}

static void mksql(msql &m, char type, CDate &date, const char *ucn)
{
	m.AddChar("F_LAWSUIT_TYPE", type);
	m.Add("F_FINAL");
	m.printf("<= %ld", date.Value());
	m.AddString("F_JUDGE", ucn);
	m.AddOrder("F_SCHED");
}

static void query_sched(TWindow *, char type, CDate &date, TOffice &office, THTMField trs)
{
	TSched title;
	TSched sched;
	int count = 0;
	msql m(sched.Name);
	char tSched = SCHED_NULL;

	mksql(m, type, date, "");
	TQuery q(str(m));
	while (q.Read())
	{
		sched << q;

		if (sched.final < Today && strchr(SCHED_AUTOENDS, sched.sched))
		{
			sched.Delete(true);
			continue;
		}

		if (!count)
		{
			title.key.type = sched.key.type;
			office.Emit(trs.tr1start, trs.tr2start, title_fields, &title);
		}

		if (sched.sched != tSched)
		{
			if (tSched != SCHED_NULL || !count)
			{
				title.sched = sched.sched;
				office.Emit(trs.tr2start, trs.sm1break, title_fields, &title);
			}
		}

		tSched = sched.sched;

		office.Emit(trs.sm1break, trs.tr2final, sched_fields, &sched);
		count++;
	}

	if (count)
	{
		title.key.no = count;
		office.Emit(trs.tr2final, trs.tr1final, title_fields, &title);
	}
}

static void query_sched(TWindow *, char type, CDate &date, TGroupArray *judges, TOffice &office, THTMField trs)
{
	TSched title;
	TSched sched;
	int count = 0;
	int total = 0;

	for (int i = 0; i < judges->Count(); i++)
	{
		TUserAliasGroup *judge = (TUserAliasGroup *) (*judges)[i];
		msql m(sched.Name);

		mksql(m, type, date, judge->ucn);
		TQuery q(str(m));
		while (q.Read())
		{
			if (!count)
			{
				title.key.type = type;
				title.date = date;
				office.Emit(trs.tr1start, trs.tr2start, title_fields, &title);
			}

			sched << q;
			strcpy(sched.judge, judge->ucn);

			office.Emit(trs.tr2start, trs.tr2final, sched_fields, &sched);
			count++;
		}

		if (count)
		{
			title.key.no = count;
			office.Emit(trs.tr2final, trs.sm1break, title_fields, &title);
		}

		total += count;
		count = 0;
	}

	if (total)
	{
		title.key.no = total;
		office.Emit(trs.sm1break, trs.tr1final, title_fields, &title);
	}
}

static void query_sched(TWindow *parent, const char *types, CDate &date, TGroupArray *judges)
{
	THTMField trs;
	TOffice office;

	if (office.Open(judges->Count() ? "FltSchJ.htm" : "FltSchA.htm"))
	{
		trs.tr1start = office.Find("tr1 start");
		trs.tr2start = office.Find("tr2 start");

		if (judges->Count() == 0)
			trs.sm1break = office.Find("sm1 break");

		trs.tr2final = office.Find("tr2 final");

		if (judges->Count() != 0)
			trs.sm1break = office.Find("sm1 break");

		trs.tr1final = office.Find("tr1 final");

		office.Emit(NULL, trs.tr1start, null_fields, NULL);

		while (*types)
		{
			if (judges->Count() == 0)
				query_sched(parent, *types++, date, office, trs);
			else
				query_sched(parent, *types++, date, judges, office, trs);
		}

		office.Emit(trs.tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

void query_sched(TWindow *parent, TExpireSchedGroup *group)
{
	query_sched(parent, group->types, group->date, &group->judges);
}

#include "ExpireSchedDialog.h"
#include "ExpireSchedDialog.cpp"

void expire_sched(TWindow *parent)
{
	TExpireSchedGroup group(QUERY_COLLEGES);
	TExpireSchedDialog(parent, &group).Execute();
}

// ----- TFloatSchedGroup ----------------------------------------------------
class TFloatSchedGroup : public TFloatGroup
{
public:
	TFloatSchedGroup();

	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_TYPES];
	char scheds[SIZE_OF_SCHEDS];
	CDate minStart;
	CDate maxStart;
	CDate minFinal;
	CDate maxFinal;
	TGArray<TUserAliasGroup> judges;
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
};

// ----- TFloatSchedGroup ----------------------------------------------------
TFloatSchedGroup::TFloatSchedGroup()
	: TFloatGroup("T_SCHED")
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TCharList("F_SCHEDS", scheds, sizeof scheds),
		new TDate("F_MIN_START", &minStart, false),
		new TDate("F_MAX_START", &maxStart, false),
		new TDate("F_MIN_FINAL", &minFinal, false),
		new TDate("F_MAX_FINAL", &maxFinal, false),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		NULL
	);
}

void TFloatSchedGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddChars("F_LAWSUIT_TYPE", types, TYPE_LAWSUITS);
	m.AddChars("F_SCHED", scheds, SCHED_ALLS);
	m.AddRange("F_START", minStart, maxStart);
	m.AddRange("F_FINAL", minFinal, maxFinal);
	m.AddRange("F_LAWSUIT_NO", minNo, maxNo);
	m.AddRange("F_LAWSUIT_YEAR", minYear, maxYear);

	if (judges.Count() == 0)
		m.AddOrder("F_LAWSUIT_TYPE, F_SCHED");
	else
	{
		m.AddArray("F_UCN", judges, "F_JUDGE");
		m.AddOrder("F_JUDGE, F_TYPE, F_YEAR, F_NO");
	}
}

static void query_sched_sched(TWindow *, const char *criteria)
{
	TSched title;
	TSched sched;
	int count = 0;
	TOffice office;
	char tType = TYPE_NULL;
	char tSched = SCHED_NULL;

	if (office.Open("FltSchA.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr2start = office.Find("tr2 start");
		constant sm1break = office.Find("sm1 break");
		constant tr2final = office.Find("tr2 final");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, title_fields, &title);
		TQuery q(criteria);

		while (q.Read())
		{
			sched << q;

			if (sched.lawsuit.type != tType)
			{
				if (tType != SCHED_NULL || !count)
				{
					if (count)
					{
						title.key.no = count;
						office.Emit(tr2final, tr1final, title_fields, &title);
						count = 0;
					}

					title.key.type = sched.lawsuit.type;
					office.Emit(tr1start, tr2start, title_fields, &title);
				}
			}
			tType = sched.lawsuit.type;

			if (tSched != sched.sched)
			{
				if (tSched != SCHED_NULL || !count)
				{
					title.sched = sched.sched;
					office.Emit(tr2start, sm1break, title_fields, &title);
				}

				tSched = sched.sched;
			}

			office.Emit(sm1break, tr2final, sched_fields, &sched);
			count++;
		}

		title.key.no = count;
		office.Emit(tr2final, tr1final, title_fields, &title);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

static void query_sched_judge(TWindow *, const char *criteria)
{
	TSched title;
	TSched sched;
	int count = 0;
	TOffice office;
	char tJudge[SIZE_OF_UCN];

	*tJudge = '\0';	// 2010:145 LPR: FIX: was missing

	if (office.Open("FltSchJ.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr2start = office.Find("tr2 start");
		constant tr2final = office.Find("tr2 final");
		constant sm1break = office.Find("sm1 break");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, null_fields, NULL);
		office.Emit(tr1start, tr2start, null_fields, NULL);

		TQuery q(criteria);
		while (q.Read())
		{
			sched << q;

			if (atob(sched.judge) && strcmp(tJudge, sched.judge))
				office.Emit(tr2final, sm1break, sched_fields, &sched);

			office.Emit(tr2start, tr2final, sched_fields, &sched);
			strcpy(tJudge, sched.judge);
			count++;
		}

		title.key.no = count;
		office.Emit(sm1break, tr1final, title_fields, &title);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

#include "FloatSchedDialog.h"
#include "FloatSchedDialog.cpp"

void float_sched(TWindow *parent)
{
	TFloatSchedGroup group;
	TFloatSchedDialog(parent, &group).Execute();
}
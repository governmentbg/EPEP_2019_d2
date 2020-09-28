#include "cac.h"

// ----- TCorruptGroup -------------------------------------------------------
class TCorruptGroup : public TFloatGroup
{
public:
	TCorruptGroup();

	virtual void FormCriteria(msql &m);
	void Print(mstr &m);

	long minMonth;
	long minYear;
	long maxMonth;
	long maxYear;
	long forExport;
};

TCorruptGroup::TCorruptGroup()
	: TFloatGroup("T_LAW_INTERVAL I JOIN T_LAWSUIT L ON L.F_NO = I.F_NO AND L.F_YEAR = I.F_YEAR AND L.F_TYPE = I.F_TYPE",
		"I.*, L.F_KIND, L.F_SOURCE_KIND, L.F_SUBJECT, L.F_TEXT")
{
	Add(
		new TLongAlias("F_MIN_MONTH", &minMonth),
		new TLong("F_MIN_YEAR", &minYear),
		new TLongAlias("F_MAX_MONTH", &maxMonth),
		new TLong("F_MAX_YEAR", &maxYear),
		new TCheck("F_FOR_EXPORT", &forExport, TRUE),
		NULL
	);

	minMonth = maxMonth = Today.month;
	minYear = maxYear = Today.year;
}

void TCorruptGroup::FormCriteria(msql &m)
{
	CDate minDate = CDate(1, minMonth, minYear);
	CDate maxDate = CDate(CDate::DaysInMonth(maxYear, maxMonth), maxMonth, maxYear);

	m.Begin(Name, fields);
	m.AddRange("(I.F_DATE", minDate, maxDate);
	m.NextOr();
	m.AddRange("I.F_FINISHED", minDate, maxDate);
	m.cat(")");
	m.AddChar("I.F_TYPE", TYPE_PUNISHMENT_LAW);
	m.AddFlags("L.F_FLAGS", LAWSUIT_CORRUPT, LAWSUIT_CORRUPT, LAWSUIT_ALL);
	m.AddOrder("I.F_YEAR, I.F_NO, I.F_DATE");
}

void TCorruptGroup::Print(mstr &m)
{
	m.printf("%03ld_%02ld.%04ld-%02ld.%04ld", COURT_CODE, minMonth, minYear, maxMonth, maxYear);
}

#define corgroup OFFICE_GROUP_CAST(TCorruptGroup)

static void corgroup_ident(mstr &m, TGroup *group) { corgroup->Print(m); }
static void corgroup_min_month(mstr &m, TGroup *group) { m.cat(Month->Seek(corgroup->minMonth)); }
static void corgroup_max_month(mstr &m, TGroup *group) { m.cat(Month->Seek(corgroup->maxMonth)); }

#undef corgroup

// ----- TCorruptDefendant ---------------------------------------------------
class TCorruptDefendant : public TSideWind
{
public:
	TCorruptDefendant(const TLawsuit *tLawsuit) : lawsuit(tLawsuit) { }

	const TLawsuit *const lawsuit;
};

// ----- TCorruptLawsuit -----------------------------------------------------
class TCorruptLawsuit : public TLawsuit
{
public:
	bool Defendant(char involvement);
	void LoadDefendants();

	TGArray<TSideWind> defendants;
};

bool TCorruptLawsuit::Defendant(char involvement)
{
#if COURT_TYPE == COURT_APPEAL
	if (strchr(INVOLVEMENT_VZ_CORR_DEFENDS, involvement))
		return true;
#endif  // APPEAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (strchr(INVOLVEMENT_VZ_CORR_DEFENDS, involvement) && strchr(KIND_2ND_LAWSUITS, kind))
		return true;
#endif  // AREAL || MILITARY
	return TLawsuit::Defendant(involvement);
}

void TCorruptLawsuit::LoadDefendants()
{
	TSideWind sideWind;
	msql m(sideWind.Name, "DISTINCT F_UCN, F_UCN_TYPE, F_INVOLVEMENT");

	m.Add(this, "F_NO, F_YEAR, F_TYPE");
	m.AddChars("F_UCN_TYPE", UCN_CITIZENS);
	m.AddRange("F_DATE", date, finished);

	TQuery p(str(m));
	while (p.Read())
	{
		sideWind << p;
		sideWind.key = key;

		if (Defendant(sideWind.involvement))
		{
			TCorruptDefendant *defendant = new TCorruptDefendant(this);
			*defendant << sideWind;
			defendants.Add(defendant);
		}
	}
};

// ----- TCorruptQuery -------------------------------------------------------
class TCorruptQuery : public TFloatQuery
{
public:
	TCorruptQuery(TWindow *parent, const char *criteria, TCorruptGroup *tGroup);

	void Execute();

	TCorruptGroup *group;

protected:
	const char *tr1defendant;

	virtual void GetMarkers();
	virtual void Count();
};

TCorruptQuery::TCorruptQuery(TWindow *parent, const char *criteria, TCorruptGroup *tGroup)
	: TFloatQuery(parent, criteria, "Corrupt.htm"), group(tGroup)
{
}

#define lawsuit OFFICE_GROUP_CAST(TCorruptLawsuit)

static void querycor_subcount(mstr &m, TGroup *group) { subcount(m, lawsuit->defendants.Count()); }

static void querycor_lawsuit(mstr &m, TGroup *group)
{
	m.printf("%s %ld/%ld%s", Kind->Seek(lawsuit->kind), lawsuit->key.no, lawsuit->key.year, GOD_SUFFIX);
}

#undef lawsuit

#define defendant OFFICE_GROUP_CAST(TCorruptDefendant)

static void querycor_defendant(mstr &m, TGroup *group)
{
	if (strchr(UCN_CITIZENS, defendant->ucnType))
	{
		TCitizen citizen;
		citizen << defendant;

		if (citizen.Try())
		{
			m.printf("%s %s", citizen.name, citizen.family);
			if (any(citizen.reFamily))
				m.printf("-%s", citizen.reFamily);
			return;
		}
	}

	defendant->PrintNames(m);
}

static void querycor_involvement(mstr &m, TGroup *group) { m.cat(Involvement->Seek(defendant->involvement)); }

static void querycor_crime_names(mstr &m, TGroup *group)
{
	if (!eispp_print_pnes(m, defendant))
	{
		m.cat(Subject->Seek(defendant->lawsuit->key.type, defendant->lawsuit->subject));
		if (any(defendant->lawsuit->text))
			m.printf(", %s", defendant->lawsuit->text);
	}
}

static void querycor_judgements(mstr &m, TGroup *group)
{
	msql w("T_JUDGEMENT J JOIN T_DECISION D ON D.F_NO = J.F_NO AND D.F_YEAR = J.F_YEAR AND D.F_TYPE = J.F_TYPE "
		"AND D.F_KIND = J.F_KIND AND D.F_DATE = J.F_DATE", "J.*, D.F_EVENT_NO, D.F_FROM_DATE");
	TDecision decision;

	w.AddLong("J.F_NO", defendant->key.no);
	w.AddLong("J.F_YEAR", defendant->key.year);
	w.AddChar("J.F_TYPE", defendant->key.type);
	w.AddString("J.F_UCN", defendant->ucn);
	w.AddChar("J.F_UCN_TYPE", defendant->ucnType);
	w.AddChar("NOT J.F_JUDGED", JUDGED_NOT_KNOWN);
	w.AddRange("J.F_DATE", defendant->lawsuit->date, defendant->lawsuit->finished);

	{
		TQuery q(str(w));
		TJudgement judgement;

		if (q.Read())
		{
			do
			{
				decision << q;
				judgement << q;
				m.sep("\ahr\b");
				m.printf("%s %ld/%D%s", Kind->Seek(decision.kind), decision.eventNo, &decision.fromDate, GOD_SUFFIX);
				m.printf("\v%s %s", Judged->Seek(judgement.judged), Punishment->Seek(judgement.punishment));
				if (any(judgement.text))
					m.printf("\v%s", str(judgement.text));
			} while (q.Read());
			return;
		}
	}

	w.Begin("T_DECISION D JOIN T_SESSION S ON S.F_NO = D.F_NO AND S.F_YEAR = D.F_YEAR AND S.F_TYPE = D.F_TYPE "
		"AND S.F_KIND = D.F_SESSION_KIND AND S.F_DATE = D.F_DATE", "D.*");
	w.AddLong("D.F_NO", defendant->key.no);
	w.AddLong("D.F_YEAR", defendant->key.year);
	w.AddChar("D.F_TYPE", defendant->key.type);
	w.AddRange("D.F_DATE", defendant->lawsuit->date, defendant->lawsuit->finished);
	w.AddIsNull("S.F_FINAL", false);

	{
		TQuery q(str(w));
		TDecision decision;

		while (q.Read())
		{
			decision << q;
			m.sep("\ahr\b");
			m.printf("%s %ld/%D%s", Kind->Seek(decision.kind), decision.eventNo, &decision.fromDate, GOD_SUFFIX);
			if (any(decision.text))
				m.printf("\v%s", str(decision.text));
		}
	}
}

#undef defendant

static void combocor_court_list(mstr &m, TGroup *group);

static offield querycor_fields[] =
{
	{ "F_COURT_LIST",  combocor_court_list },
	{ "F_IDENT",       corgroup_ident },
	{ "F_MIN_MONTH",   corgroup_min_month },
	{ "F_MIN_YEAR",    NULL },
	{ "F_MAX_MONTH",   corgroup_max_month },
	{ "F_MAX_YEAR",    NULL },
	{ "F_SUBCOUNT",    querycor_subcount },
	{ "F_LAWSUIT",     querycor_lawsuit },
	{ "F_DATE",        NULL },
	{ "F_DEFENDANT",   querycor_defendant },
	{ "F_INVOLVEMENT", querycor_involvement },
	{ "F_CRIME_NAMES", querycor_crime_names },
	{ "F_JUDGEMENTS",  querycor_judgements },
	{ NULL, NULL }
};

void TCorruptQuery::GetMarkers()
{
	tr1start = office.Find("tr1 start");
	tr1defendant = office.Find("tr1 defendant");
	tr1final = office.Find("tr1 final");
}

void TCorruptQuery::Count()
{
	TQuery q(criteria);
	TLawsuit lawsuit;

	while (q.Read() && Increment())
	{
		TCorruptLawsuit lawsuit;

		lawsuit << q;
		lawsuit.LoadDefendants();
		office.Emit(tr1start, tr1defendant, querycor_fields, &lawsuit);

		if (lawsuit.defendants.Count())
		{
			for (int i = 0; i < lawsuit.defendants.Count(); i++)
				office.Emit(tr1defendant, tr1final, querycor_fields, lawsuit.defendants[i]);
		}
		else
			office.Emit(tr1defendant, tr1final, null_fields, NULL);
	}
}

void TCorruptQuery::Execute()
{
	const char *target = NULL;
	char path[SIZE_OF_PATH];

	if (group->forExport)
	{
		mstr base("Corrupt_");

		group->Print(base);
		base.cat(".htm");
		build_fn(OUT_PATH, str(base), path);
		target = path;
	}

	if (office.Open(name, OFFICE_SKIP_HEADER, target))
	{
		TWindowHideParent hide(parent);

		GetMarkers();
		office.Emit(NULL, tr1start, querycor_fields, group);
		Count();
		office.Emit(tr1final, NULL, querycor_fields, group);
		Show();
	}
}

#include "QueryCorruptDialog.h"
#include "QueryCorruptDialog.cpp"

// ----- TCombineCorruptItem -------------------------------------------------
class TCombineCorruptItem : public TGroup
{
public:
	TCombineCorruptItem();
	virtual ~TCombineCorruptItem();

	virtual void Print(mstr &m);

	mstr base;
	long courtCode;
	char *data;

	bool operator<=(const TCombineCorruptItem &that) { return courtCode <= that.courtCode; }
	bool Read(const char *name, TCorruptGroup &meta);
};

TCombineCorruptItem::TCombineCorruptItem()
	: TGroup("T_CORRUPT", NULL), data(NULL)
{
}

TCombineCorruptItem::~TCombineCorruptItem()
{
	delete[] data;
}

void TCombineCorruptItem::Print(mstr &m)
{
	m.printf("%s - %s", str(base), Court->Seek(courtCode));
}

bool TCombineCorruptItem::Read(const char *name, TCorruptGroup &meta)
{
	long size;

	if ((data = TUserFile("хипертекстов").ReadAll(name, size)) == NULL)
		return false;

	static constant START_MARK = "<!-- qry start -->";
	char *start = strstr(data, START_MARK);

	if (!start)
		return error("Липсващ начален маркер.");

	start += strlen(START_MARK);
	while (*start && strchr("\r\n", *start))
		start++;

	if (strlen(start) <= 50 || memcmp(start, "<!-- ", 5) || memcmp(start + 24, " -->", 4))
		return error("Грешка при четене на маркер.");

	int n;

	if (sscanf(start + 5, "%03ld_%02ld.%04ld-%02ld.%04ld%n", &courtCode, &meta.minMonth, &meta.minYear, &meta.maxMonth,
		&meta.maxYear, &n) != 5 || n != 19)
	{
		return error("Невалидни метаданни.");
	}

	if (!CDate(1, meta.minMonth, meta.minYear).Valid() || !CDate(1, meta.maxMonth, meta.maxYear).Valid())
		return error("Невалиден период.");

	Court->court = courtCode;
	if (!Court->Try())
		return error("Невалиден съд.");

	start += 28;
	while (*start && strchr("\r\n", *start))
		start++;

	static constant FINAL_MARK = "<!-- qry final -->";
	const char *final = strstr(start, FINAL_MARK);

	if (!final)
		return error("Липсващ краен маркер.");

	base.cpy(basename(name));
	memmove(data, start, final - start);
	data[final - start] = '\0';
	return true;
}

// ----- TCombineCorruptGroup ------------------------------------------------
class TCombineCorruptGroup : public TCorruptGroup
{
public:
	TGSArray<TCombineCorruptItem> array;
};

#define combocor OFFICE_GROUP_CAST(TCombineCorruptGroup)

static void combocor_court_list(mstr &m, TGroup *group)
{
	for (int i = 0; i < combocor->array.Count(); i++)
	{
		if (i)
			m.cat(i == combocor->array.Count() - 1 ? " и " : ", ");

		m.cat(Court->Seek(combocor->array[i]->courtCode));
	}
}

#undef combocor

// ----- functions -----------------------------------------------------------
void query_corrupt(TWindow *parent)
{
	TCorruptGroup group;

	if (TQueryCorruptDialog(parent, &group).Execute() == IDOK)
	{
		msql m;
		group.FormCriteria(m);
		TCorruptQuery(parent, str(m), &group).Execute();
	}
}

#include "CombineCorruptListWindow.h"
#include "CombineCorruptListWindow.cpp"

#include "CombineCorruptListDialog.h"
#include "CombineCorruptListDialog.cpp"

void combine_corrupts(TWindow *parent)
{
	TCombineCorruptGroup group;

	if (TCombineCorruptListDialog(parent, &group).Execute() == IDOK)
	{
		TOffice office;

		if (office.Open("CorruptCom.htm"))
		{
			constant tr1holder = office.Find("tr1 holder");

			office.Emit(NULL, tr1holder, querycor_fields, &group);

			for (int i = 0; i < group.array.Count(); i++)
				office.PutRaw(group.array[i]->data);

			office.Emit(tr1holder, NULL, querycor_fields, &group);
			office.Show(NULL);
		}
	}
}

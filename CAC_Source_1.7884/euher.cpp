#include "cac.h"

#if COURT_TYPE == COURT_REGIONAL
// ----- TBookEuherGroup -----------------------------------------------------
enum
{
	ORDER_BY_LEAVER	= ORDER_BY_USER,
	ORDER_BY_EUHER_NO	= ORDER_BY_USER + 1
};

class TBookEuherGroup : public TBookGroup
{
public:
	TBookEuherGroup();

	virtual void FormCriteria(msql &m);
};

TBookEuherGroup::TBookEuherGroup()
	: TBookGroup("T_DECISION D JOIN T_LAWSUIT L ON L.F_NO = D.F_NO AND L.F_YEAR = D.F_YEAR AND L.F_TYPE = D.F_TYPE",
		"D.F_TYPE", "D.F_DATE", false)
{
	Add(
		new TRadio("F_ORDER_BY_LEAVER", &order, ORDER_BY_LEAVER),
		new TRadio("F_ORDER_BY_EUHER_NO", &order, ORDER_BY_EUHER_NO),
		NULL
	);

	fields = "D.*, L.F_KIND AS F_LAWSUIT_KIND, L.F_DATE AS F_LAWSUIT_DATE, L.F_FINISHED, L.F_COMPOSITION";
	order = ORDER_BY_LEAVER;
}

void TBookEuherGroup::FormCriteria(msql &m)
{
	TBookGroup::FormCriteria(m);
	m.AddChar("D.F_KIND", KIND_EUHER_CERT);
}

// ----- TEuherDecision ------------------------------------------------------
class TEuherDecision : public TDecision
{
public:
	TEuherDecision();
	virtual ~TEuherDecision();

	void CreateDefendants();

	char lawsuitKind;
	CDate lawsuitDate;
	CDate finished;
	long composition;

	const char *categories;
	TGArray<TSideWind> sideWinds;
	mstr defendants;
};

TEuherDecision::TEuherDecision()
	: categories(NULL)
{
	Add(
		new TCharAlias("F_LAWSUIT_KIND", &lawsuitKind),
		new TDate("F_LAWSUIT_DATE", &lawsuitDate, false),
		new TDate("F_FINISHED", &finished, false),
		new TLongAlias("F_COMPOSITION", &composition),
		NULL
	);
}

TEuherDecision::~TEuherDecision()
{
	delete[] categories;
}

void TEuherDecision::CreateDefendants()
{
	for (int i = 0; i < sideWinds.Count(); i++)
		if (categories[i] == SIDEWIND_RIGHT)
			add_side_name(defendants, sideWinds[i], "\v", false);
}

// ----- TEuherDecisionArray -------------------------------------------------
class TEuherDecisionArray : public TGSArray<TEuherDecision>
{
public:
	TEuherDecisionArray(long order);

protected:
	static bool LessEqlLeaver(const long t1, const long t2);
	static bool LessEqlEuherNo(const long t1, const long t2);
};

TEuherDecisionArray::TEuherDecisionArray(long order)
	: TGSArray<TEuherDecision>(order == ORDER_BY_LEAVER ? LessEqlLeaver : LessEqlEuherNo)
{
}

bool TEuherDecisionArray::LessEqlLeaver(const long t1, const long t2)
{
	const TEuherDecision *e1 = (const TEuherDecision *) t1;
	const TEuherDecision *e2 = (const TEuherDecision *) t2;

	return strcmp(str(e1->defendants), str(e2->defendants));
}

bool TEuherDecisionArray::LessEqlEuherNo(const long t1, const long t2)
{
	const TDecision *d1 = (const TDecision *) t1;
	const TDecision *d2 = (const TDecision *) t2;

	return d1->key.year != d2->key.year ? d1->key.year < d2->key.year : d1->eventNo <= d2->eventNo;
}

#define euher OFFICE_GROUP_CAST(TEuherDecision)

static void euher_book_defendants(mstr &m, TGroup *group)
{
	m.cat(str(euher->defendants));
	//full_side_list(m, &euher->sideWinds, euher->categories, SIDEWIND_RIGHT, false);
}

static void euher_book_suitors(mstr &m, TGroup *group)
{
	full_side_list(m, &euher->sideWinds, euher->categories, SIDEWIND_LEFT, false);
}

static void euher_book_lawsuit(mstr &m, TGroup *group)
{
	m.printf("%s № %ld/%ld%s, %s състав", Kind->Seek(euher->lawsuitKind), euher->key.no, euher->key.year, GOD_SUFFIX,
		Composition->Seek(euher->composition));
}

static void euher_book_decision(mstr &m, TGroup *group)
{
	TDecision decision;
	msql w("T_DECISION D JOIN T_KIND K ON K.F_KIND = D.F_KIND", "D.*");

	w.AddLong("D.F_NO", euher->key.no);
	w.AddLong("D.F_YEAR", euher->key.year);
	w.AddChar("D.F_TYPE", euher->key.type);
	w.AddDate("D.F_DATE", euher->date);
	w.AddChar("NOT D.F_KIND", KIND_EUHER_CERT);
	w.AddOrder("K.F_ORDER");

	TQuery q(str(w));
	if (q.Read())
	{
		decision << q;
		m.printf("%s № %ld/%D%s", Kind->Seek(decision.kind), decision.eventNo, &decision.fromDate, GOD_SUFFIX);

		if (any(decision.text))
			m.printf("\v\v%s", str(decision.text));
	}
}

static void euher_book_ecs(mstr &m, TGroup *group)
{
	m.printf("%ld/%D%s", euher->eventNo, &euher->fromDate, GOD_SUFFIX);
}

static void euher_book_copies(mstr &m, TGroup *group)
{
	TBaseWind baseWind("T_SUBPOENA", NULL);
	msql w;

	baseWind << euher;
	baseWind.FloatCriteria(w, "F_INVOLVEMENT, F_UCN, F_UCN_TYPE");
	w.AddChar("F_SUBPOENA_KIND", SUBPOENA_KIND_EUHER_COPY);

	TQuery q(str(w));
	while (q.Read())
	{
		baseWind << q;
		m.sep(", ");
		m.printf("%s ", Involvement->Seek(baseWind.involvement));
		baseWind.PrintNames(m);
	}
}

#undef euher

static offield euher_book_fields[] =
{
	{ "F_DEFENDANTS",		euher_book_defendants },
	{ "F_SUITORS",		euher_book_suitors },
	{ "F_LAWSUIT",		euher_book_lawsuit },
	{ "F_LAWSUIT_DATE",	NULL },
	{ "F_FINISHED",		NULL },
	{ "F_DECISION",		euher_book_decision },
	{ "F_ECS",			euher_book_ecs },
	{ "F_COPIES",		euher_book_copies },	
	{ NULL, NULL }
};

// ----- TBookEuher ----------------------------------------------------------
#include "BookEuherDialog.h"
#include "BookEuherDialog.cpp"

class TBookEuher : public TBook
{
public:
	TBookEuher(TWindow *parent, TBookEuherGroup *group);

protected:
	virtual TGroup *Process(TQuery &q);
};

TBookEuher::TBookEuher(TWindow *parent, TBookEuherGroup *group)
	: TBook(parent, group, "Reg_ECS.htm", euher_book_fields, ::book_fields, BOOK_OLD_FORMAT,
		new TEuherDecisionArray(group->order))
{
}

TGroup *TBookEuher::Process(TQuery &q)
{
	TEuherDecision *euher = new TEuherDecision;

	*euher << q;
	euher->categories = load_sidebasic(euher, euher->sideWinds);
	euher->CreateDefendants();
	return euher;
}

void book_euher(TWindow *parent)
{
	TBookEuherGroup group;

	if (TBookEuherDialog(parent, &group).Execute() == IDOK)
		TBookEuher(parent, &group).Execute();
}
#endif  // REGIONAL

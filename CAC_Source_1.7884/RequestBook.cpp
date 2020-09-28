#include "cac.h"

class TRequestBookGroup : public TBookGroup
{
public:
	TRequestBookGroup();
	long flags;

protected:
	virtual void FormCriteria(msql &m);
	virtual void Title(mstr &m);
};

TRequestBookGroup::TRequestBookGroup()
	: TBookGroup("T_REQUEST R LEFT JOIN T_LAWSUIT L ON L.F_SOURCE_NO = R.F_NO AND L.F_SOURCE_YEAR = R.F_YEAR "
		"AND L.F_SOURCE_TYPE = R.F_TYPE", "R.F_COLLEGE", "R.F_DATE", true)
{
	Add(
		new TCheck("F_INSTANCE_1ST", &flags, BOOK_1ST_INST),
		new TCheck("F_INSTANCE_2ND", &flags, BOOK_2ND_INST),
		NULL
	);
}

void TRequestBookGroup::FormCriteria(msql &m)
{
	m.Begin(Name, "R.F_NO AS F_REQ_NO, R.F_YEAR AS F_REQ_YEAR, R.F_TYPE AS F_REQ_TYPE, R.F_KIND AS F_REQ_KIND, "
		"R.F_DATE AS F_REQ_DATE, L.F_JUDGE AS F_LAW_JUDGE, L.F_COMPOSITION AS F_LAW_COMPOSITION, L.F_NO, "
		"L.F_YEAR, L.F_TYPE, L.F_COLLEGE, L.F_DATE, L.F_KIND, R.F_LAW_KIND"
	);

	m.AddChars("R.F_COLLEGE", types, COLLEGE_ALLS);

	if (by == BOOK_BY_NO_YEAR)
	{
		m.AddRange("R.F_NO", minNo, maxNo);
		m.AddLong("R.F_YEAR", year);
	}
	else
		m.AddRange("R.F_DATE", minDate, maxDate);

	for (const char *kind = KIND_INDOCS; *kind; kind++)
	{
	#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		long instance = flags % BOOK_BOTH_INST;

		if (instance == BOOK_1ST_INST)
			m.AddChars("R.F_LAW_KIND", KIND_1ST_LAWSUITS);
		else if (instance == BOOK_2ND_INST)
			m.AddChars("R.F_LAW_KIND", KIND_2ND_LAWSUITS);
	#endif  // ADMIN || AREAL || MILITARY
	}

	// == EndCriteria()
	m.AddOrder(by == BOOK_BY_NO_YEAR ? "R.F_NO, R.F_YEAR, R.F_TYPE" : "R.F_DATE, R.F_NO, R.F_TYPE, R.F_YEAR");
}

void TRequestBookGroup::Title(mstr &m)
{
	book_types(m, this);
	m.cat("\v");

	if (by == BOOK_BY_NO_YEAR)
		book_range(m, this);
	else
		book_dates(m, this);
}

class TBookRequest : public TRequest
{
public:
	TBookRequest();
	virtual ~TBookRequest();

	TRCDKey reqKey;
	char reqKind;
	CDate reqDate;
	char lawJudge[SIZE_OF_UCN];
	long lawComposition;
	TGArray<TSideWind> sideWinds;
	char *categories;
};

TBookRequest::~TBookRequest()
{
	delete[] categories;
}

TBookRequest::TBookRequest()
{
	Add(
		new TLong("F_REQ_NO", &reqKey.no),
		new TLong("F_REQ_YEAR", &reqKey.year),
		new TChar("F_REQ_KIND", &reqKind),
		new TChar("F_REQ_TYPE", &reqKey.type),
		new TLong("F_LAW_COMPOSITION", &lawComposition),
		new TDate("F_REQ_DATE", &reqDate, false),
		new TString("F_LAW_JUDGE", lawJudge, sizeof lawJudge),
		NULL
	);
}

#define request OFFICE_GROUP_CAST(TBookRequest)

static void request_lower(mstr &m, TGroup *group)
{
	static const char lowerTypes[] = { CONNECT_TYPE_LOWER_INSTANCE, CONNECT_TYPE_RECALL_LAWSUIT, '\0' };
	bool first = true;

	m.printf("%d/%D", request->reqKey.no, &request->reqDate);

	for (const char *lt = lowerTypes; *lt; lt++)
	{
		TConnect connect;

		if (connect.TryBy(request->reqKey, *lt))
		{
			if (first)
			{
				m.cat(" ");
				first = false;
			}
			else
				m.cat("\v");

			TSender sender;

			strcpy(sender.ucn, connect.ucn);
			sender.ucnType = connect.ucnType;
			sender.Try();
			m.printf(" %s %s/%d %s", connect.connectKind ? ConnectKind->Seek(connect.connectKind) : ConnectType->Seek(connect.connectType), connect.connectNo, connect.connectYear, sender.name);
		}
	}
}

static void request_law_kind(mstr &m, TGroup *group)
{
	m.cpy(Kind->Seek(request->lawKind));
}

static void request_law_composition(mstr &m, TGroup *group)
{
	if (request->key.no && request->key.year)
		m.cpy(Composition->Seek(request->lawComposition));
}

static void request_side_left(mstr &m, TGroup *group) { full_side_list(m, &request->sideWinds, request->categories, SIDEWIND_LEFT); }
static void request_side_right(mstr &m, TGroup *group) { full_side_list(m, &request->sideWinds, request->categories, SIDEWIND_RIGHT); }

static void request_law_judge(mstr &m, TGroup *group)
{
	if (request->key.no && request->key.year)
		print_user(m, request->lawJudge);
}

static void request_remark(mstr &m, TGroup *group)
{
	if (request->key.no && request->key.year)
	{
		m.printf("Образувано в дело от %D %s %d/%d", &request->date, Kind->Seek(request->kind), request->key.no,
			request->key.year);
	}
}

#undef request

static const offield request_fields[] =
{
	{ "F_LOWER",	request_lower},
	{ "F_LAW_KIND",	request_law_kind},
	{ "F_LAW_COMPOSITION", request_law_composition},
	{ "F_SIDE_LEFT", request_side_left},
	{ "F_SIDE_RIGHT",	request_side_right},
	{ "F_LAW_JUDGE",	request_law_judge},
	{ "F_REMARK",	request_remark},
	{ NULL, NULL }
};

class TRequestBook : public TBook
{
public:
	TRequestBook(TWindow *parent, TBookGroup *tGroup);

protected:
	virtual TGroup *Process(TQuery &q);
};

TRequestBook::TRequestBook(TWindow *parent, TBookGroup *tGroup)
	: TBook(parent, tGroup, "ReqBook.htm", request_fields)
{
}

TGroup *TRequestBook::Process(TQuery &q)
{
	TBookRequest *request = new TBookRequest;

	*request << q;
	request->key = request->reqKey;
	request->kind = request->reqKind;
	request->date = request->reqDate;
	request->categories = load_sidebasic(request, request->sideWinds);
	MinMax(request->key.no, request->date);
	*request << q;	// re-get key, kind, date

	return request;
}

// ----- functions -----------------------------------------------------------
static void request_book(TWindow *parent, TRequestBookGroup *group)
{
	TRequestBook(parent, group).Execute();
}

#include "RequestBookDialog.h"
#include "RequestBookDialog.cpp"

void request_book(TWindow *parent)
{
	TRequestBookGroup group;

	if (TRequestBookDialog(parent, &group).Execute() == IDOK)
		request_book(parent, &group);
}

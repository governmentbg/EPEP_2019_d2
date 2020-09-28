#include "cac.h"

// ----- TFloatRequestGroup --------------------------------------------------
enum { ORDER_BY_CKYN = ORDER_BY_USER };

class TFloatRequestGroup : public TFloatGroup
{
public:
	TFloatRequestGroup();

	virtual void FormCriteria(msql &m);

	char generalKinds[SIZE_OF_KINDS];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	char colleges[SIZE_OF_COLLEGES];
	CDate minDate;
	CDate maxDate;
	TGArray<TSubject> newSubjects;
	TGArray<TOldSubject> oldSubjects;
	char text[SIZE_OF_ORIGIN_TEXT];
	long quick;
	char eisppNmr[SIZE_OF_EISPP_NMR];
};

TFloatRequestGroup::TFloatRequestGroup()
	: TFloatGroup("T_REQUEST")
{
	Add(
		new TCharList("F_GENERAL_KINDS", generalKinds, sizeof generalKinds),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TCharList("F_COLLEGES", colleges, sizeof colleges),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TString("F_TEXT", text, sizeof text),
		new TRadio("F_SLOW", &quick, QUICK_NULL),
		new TRadio("F_QUICK", &quick, QUICK_QUICK),
		new TRadio("F_IMMEDIATE", &quick, QUICK_QUICK | QUICK_IMMEDIATE),
		new TRadio("F_ANY_SPEED", &quick, QUICK_ANY_SPEED),
		new TRadio("F_BY_CKYN", &order, ORDER_BY_CKYN),
		new TString("F_EISPP_NMR", eisppNmr, sizeof eisppNmr),
		NULL
	);

	fields = "F_KIND, F_NO, F_YEAR, F_DATE, F_COLLEGE, F_TEXT";
	quick = QUICK_ANY_SPEED;
	order = ORDER_BY_TYN;
}

void TFloatRequestGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);
	m.AddChars("F_KIND", adjust_general_kinds(generalKinds), KIND_INDOCS);
	m.AddRange("F_NO", minNo, maxNo);
	m.AddRange("F_YEAR", minYear, maxYear);
	m.AddChars("F_COLLEGE", colleges, COLLEGE_ALLS);
	m.AddRange("F_DATE", minDate, maxDate);
	m.AddArray("F_SUBJECT", newSubjects);
	m.AddArray("F_SUBJECT", oldSubjects, "F_OLD_SUBJECT");
	m.AddQuick("F_QUICK", quick);
	m.AddLike("F_TEXT", text, true);
	m.AddLike("F_EISPP_NMR", eisppNmr, true);
	m.AddOrder(order == ORDER_BY_CKYN ? "F_COLLEGE, F_KIND, F_YEAR, F_NO" : "F_TYPE, F_YEAR, F_NO");
}

// ----- TFloatRequest -------------------------------------------------------
class TFloatRequest : public TRequest
{
public:
	TFloatRequest();

	TLawsuit lawsuit;
};

TFloatRequest::TFloatRequest()
{
	lawsuit.kind = '\0';
}

// ----- TFloatRequestQuery --------------------------------------------------
class TFloatRequestQuery : public TFloatQuery
{
public:
	TFloatRequestQuery(TWindow *parent, const char *criteria, const char *name);

protected:
	void Count();
};

TFloatRequestQuery::TFloatRequestQuery(TWindow *parent, const char *criteria, const char *name)
	: TFloatQuery(parent, criteria, name)
{
}

#define request OFFICE_GROUP_CAST(TFloatRequest)

static void floatreq_college(mstr &m, TGroup *group) { m.printf("%c", *College->Seek(request->college)); }

static void floatreq_law_kind(mstr &m, TGroup *group)
{
	if (request->lawsuit.kind)
		m.cat(KindAlias->Seek(request->lawsuit.kind));
}

static void floatreq_law_key(mstr &m, TGroup *group)
{
	if (request->lawsuit.key.no)
		m.printf("%d/%d", request->lawsuit.key.no, request->lawsuit.key.year);
}

#undef request

static const offield floatreq_fields[] =
{
	{ "F_NO", NULL },
	{ "F_YEAR", NULL },
	{ "F_REQ_KIND", container_kind },
	{ "F_COLLEGE", floatreq_college },
	{ "F_REQ_DATE", container_date },
	{ "F_LAW_KIND", floatreq_law_kind },
	{ "F_LAW_KEY", floatreq_law_key},
	{ "F_REQ_TEXT", container_text },
	{ NULL, NULL }
};

void TFloatRequestQuery::Count()
{
	TQuery q(criteria);
	while (q.Read() && Increment())
	{
		// 2011:117 LPR: moved here to clear the lawsuit
		TFloatRequest request;

		request << q;
		request.lawsuit.TryBy(&request, "F_NO, F_YEAR, F_TYPE, F_KIND");
		office.Emit(tr1start, tr1final, floatreq_fields, &request);
	}
}

// ----- query ---------------------------------------------------------------
static void query_request(TWindow *parent, const char *criteria)
{
	TFloatRequestQuery(parent, criteria, "FloatReq.htm").Execute();
}

#include "FloatRequestDialog.h"
#include "FloatRequestDialog.cpp"

void float_request(TWindow *parent)
{
	TFloatRequestGroup group;
	TFloatRequestDialog(parent, &group).Execute();
}

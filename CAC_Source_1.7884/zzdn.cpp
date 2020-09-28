#include "cac.h"

#if COURT_TYPE == COURT_REGIONAL
// ----- TZZDNBookGroup ------------------------------------------------------
class TZZDNBookGroup : public TBookGroup
{
public:
	TZZDNBookGroup();

	virtual void FormCriteria(msql &m);
};

TZZDNBookGroup::TZZDNBookGroup()
	: TBookGroup("T_REQUEST R LEFT JOIN T_LAWSUIT L ON L.F_SOURCE_NO = R.F_NO AND L.F_SOURCE_YEAR = R.F_YEAR AND "
		"L.F_SOURCE_TYPE = R.F_TYPE", "R.F_TYPE", "R.F_DATE", false, true, "R.F_NO", "R.F_YEAR")
{
	autoSerialNo = 1;
	fields = "R.F_NO, R.F_YEAR, R.F_TYPE, R.F_KIND, R.F_DATE, L.F_NO AS F_LAWSUIT_NO, L.F_YEAR AS F_LAWSUIT_YEAR, "
		"L.F_KIND AS F_LAWSUIT_KIND, L.F_DATE AS F_LAWSUIT_DATE, L.F_COMPOSITION";
}

void TZZDNBookGroup::FormCriteria(msql &m)
{
	strcpy(types, C2S[TYPE_REQUEST]);
	TBookGroup::FormCriteria(m);

	m.AddChar("R.F_KIND", KIND_PROTECTIVE_REQUEST);
	m.AddOrder("R.F_DATE, R.F_NO");
}

// ----- TZZDNRequest --------------------------------------------------------
class TZZDNRequest : public TRequest
{
public:
	TZZDNRequest();

	char lawsuitKind;
	long lawsuitNo;
	CDate lawsuitDate;
	long composition;
};

TZZDNRequest::TZZDNRequest()
{
	Add(
		new TCharAlias("F_LAWSUIT_KIND", &lawsuitKind),
		new TLong("F_LAWSUIT_NO", &lawsuitNo),
		new TDate("F_LAWSUIT_DATE", &lawsuitDate, false),
		new TLongAlias("F_COMPOSITION", &composition),
		NULL
	);
}

static void print_side_winds(mstr &m, const char *categories, TGArray<TSideWind> &sideWinds, char category)
{
	for (int i = 0; i < sideWinds.Count(); i++)
	{
		if (categories[i] == category)
		{
			TSideWind *sideWind = sideWinds[i];

			m.sep("\v");
			m.printf("%s ", Involvement->Seek(sideWind->involvement));
			sideWind->PrintNames(m);

			if (PRINT_UCN && sideWind->ucnType == UCN_CITIZEN_UCN)
				m.printf(" ЕГН %s", sideWind->ucn);
		}
	}
}

#define request OFFICE_GROUP_CAST(TZZDNRequest)

static void zzdn_lawsuit(mstr &m, TGroup *group)
{
	if (request->lawsuitKind)
	{
		m.printf("%s %ld/%D%s", KindAlias->Seek(request->lawsuitKind), request->lawsuitNo, &request->lawsuitDate,
			GOD_SUFFIX);
		if (PRINT_COMPOSITIONS)
			m.printf("\v%s състав", Composition->Seek(request->composition));
	}
}

static void zzdn_sides(mstr &m, TGroup *group)
{
	TGArray<TSideWind> sideWinds;
	char *const categories = load_sidebasic(request, sideWinds);
	TCharPtr CP(categories);

	print_side_winds(m, categories, sideWinds, SIDEWIND_LEFT);
	print_side_winds(m, categories, sideWinds, SIDEWIND_RIGHT);
}

#undef request

static offield zzdn_fields[] =
{
	{ "F_NO",		NULL },
	{ "F_DATE",		NULL },
	{ "F_LAWSUIT",	zzdn_lawsuit },
	{ "F_SIDES",	zzdn_sides },
	{ NULL, NULL }
};

// ----- TZZDNBook -----------------------------------------------------------
class TZZDNBook : public TBook
{
public:
	TZZDNBook(TWindow *parent, TZZDNBookGroup *group);

protected:
	virtual TGroup *Process(TQuery &q);
};

TZZDNBook::TZZDNBook(TWindow *parent, TZZDNBookGroup *group)
	: TBook(parent, group, "Reg_ZZDN.htm", zzdn_fields, ::book_fields, BOOK_OLD_FORMAT)
{
}

TGroup *TZZDNBook::Process(TQuery &q)
{
	TZZDNRequest *request = new TZZDNRequest;
	*request << q;
	return request;
}

// ----- functions -----------------------------------------------------------
#include "ZZDNBookDialog.h"
#include "ZZDNBookDialog.cpp"

void zzdn_book(TWindow *parent)
{
	TZZDNBookGroup group;
	group.autoSerialNo = 1;

	if (TZZDNBookDialog(parent, &group).Execute() == IDOK)
		TZZDNBook(parent, &group).Execute();
}
#endif  // REGIONAL

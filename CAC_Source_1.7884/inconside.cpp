#include "cac.h"

#if COURT_TYPE == COURT_AREAL
// ----- TInconBaseGroup -----------------------------------------------------
TInconBaseGroup::TInconBaseGroup(const char *name, const char *fields, long tCreditor)
	: TFloatGroup(name, fields), creditor(tCreditor)
{
	Add(
		new TRadio("F_CREDITOR", &creditor, TRUE),
		new TRadio("F_DEBTOR", &creditor, FALSE),
		NULL
	);

	creditor = tCreditor;
}

void TInconBaseGroup::AddCount(msql &m, const char *involvements, const char *condition)
{
	m.AddChars("(SELECT COUNT(F_UCN) FROM T_SIDE_WIND W WHERE W.F_NO = S.F_NO AND W.F_YEAR = S.F_YEAR "
		"AND W.F_TYPE = S.F_TYPE AND W.F_KIND = S.F_KIND AND W.F_DATE = S.F_DATE AND W.F_INVOLVEMENT",
		involvements);
	m.cat(condition);
}

void TInconBaseGroup::FormCriteria(msql &m)
{
	if (creditor == -1)
	{
		m.AddChars("(S.F_INVOLVEMENT", INVOLVEMENT_CREDITOR_RIGHTS);
		m.NextOr();
		m.AddChars("(S.F_INVOLVEMENT", INVOLVEMENT_DEBTOR_CHECKS);
		AddCount(m, INVOLVEMENT_INCONSISTENCES, " AND NOT W.F_INVOLVEMENT = S.F_INVOLVEMENT) = 0))");
	}
	else if (creditor)
	{
		m.AddChars("S.F_INVOLVEMENT", INVOLVEMENT_CREDITOR_LEFTS);
		AddCount(m, INVOLVEMENT_CREDITOR_RIGHTS, ") > 0");
	}
	else
	{
		m.AddChars("S.F_INVOLVEMENT", INVOLVEMENT_DEBTOR_LEFTS);
		AddCount(m, INVOLVEMENT_INCONSISTENCES, " AND NOT W.F_INVOLVEMENT = S.F_INVOLVEMENT) = 0");
	}
}

// ----- TInconSideGroup -----------------------------------------------------
TInconSideGroup::TInconSideGroup(TUCNGroup *ucnGroup, const char *name, const char *fields)
	: TInconBaseGroup(name, fields, TRUE)
{
	strcpy(ucn, ucnGroup->ucn);
	ucnType = ucnGroup->ucnType;

	Add(
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		NULL
	);

	order = ORDER_BY_DTYN;
}

void TInconSideGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddString("S.F_UCN", ucn);
	m.AddChar("S.F_UCN_TYPE", ucnType);
	m.AddRange("S.F_DATE", minDate, maxDate);
	m.AddChars("S.F_TYPE", TYPE_TRADEALIKE_LAWS);
	m.AddChars("S.F_KIND", KIND_TRADEALIKE_LAWS);
	m.AddChars("L.F_SOURCE_KIND", KIND_INCONSISTENCE_REQUESTS);

	TInconBaseGroup::FormCriteria(m);
	EndCriteria(m, "S.");
}

// ----- TInconSide ---------------------------------------------------------
class TInconSide : public TSideWind
{
public:
	TInconSide(long tCreditor);

	long creditor;

	void PrintSides(mstr &m, const char *involvements);
};

TInconSide::TInconSide(long tCreditor)
	: creditor(tCreditor)
{
}

void TInconSide::PrintSides(mstr &m, const char *involvements)
{
	msql w;
	TGArray<TSideWind> sideWinds;

	TBaseWind::FloatCriteria(w);
	w.AddChars("F_INVOLVEMENT", involvements);
	w.AddOrder("F_ORDER");
	load_sidebasicv(sideWinds, str(w));

	for (int i = 0; i < sideWinds.Count(); i++)
	{
		if (any(m))
			m.cat("\v");
		print_side(m, sideWinds[i], PRINT_SUB_FULL_NAME);
	}
}

#define inconside OFFICE_GROUP_CAST(TInconSide)

static void inconside_suitors(mstr &m, TGroup *group)
{
	inconside->PrintSides(m, inconside->creditor ? INVOLVEMENT_CREDITOR_LEFTS : INVOLVEMENT_DEBTOR_LEFTS);
}

static void inconside_defendants(mstr &m, TGroup *group)
{
	if (inconside->creditor)
		inconside->PrintSides(m, INVOLVEMENT_CREDITOR_RIGHTS);
}

#undef inconside

static const offield inconside_fields[] =
{
	{ "F_KIND",	basewind_kind },
	{ "F_NO",	NULL },
	{ "F_YEAR",	NULL },
	{ "F_DATE",	NULL },
	{ "F_SUITORS",	inconside_suitors },
	{ "F_DEFENDANTS",	inconside_defendants },
	{ NULL, NULL }
};

// ----- TInconSideQuery ----------------------------------------------------
class TInconSideQuery : public TFloatQuery
{
public:
	TInconSideQuery(TWindow *parent, const char *criteria, bool tCreditor);

protected:
	bool creditor;

	virtual void Count();
};

TInconSideQuery::TInconSideQuery(TWindow *parent, const char *criteria, bool tCreditor)
	: TFloatQuery(parent, criteria, "InconSide.htm"), creditor(tCreditor)
{
}

void TInconSideQuery::Count()
{
	TInconSide inconside(creditor);

	TQuery q(criteria);
	while (q.Read())
	{
		inconside << q;
		office.Emit(tr1start, tr1final, inconside_fields, &inconside);
		count++;
	}
}

// ----- query --------------------------------------------------------------
#include "QueryInconSideDialog.h"
#include "QueryInconSideDialog.cpp"

static void query_inconside(TWindow *parent, TFoundGroup *ucnGroup)
{
	TInconSideGroup incons(ucnGroup,
		"T_SIDE_WIND S JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE",
		"S.F_NO, S.F_YEAR, S.F_TYPE, S.F_KIND, S.F_DATE, L.F_FINISHED");

	if (TQueryInconSideDialog(parent, &incons).Execute() == IDOK)
	{
		msql m;
		incons.FormCriteria(m);
		TInconSideQuery(parent, str(m), incons.creditor).Execute();
	}
}

void query_inconside(TWindow *parent)
{
	TFoundGroup ucnGroup;
	repeat_ucn(parent, &ucnGroup, UCN_REAL_SIDES, query_inconside);
}
#endif  // AREAL

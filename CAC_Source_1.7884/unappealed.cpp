#include "cac.h"

// ----- TFloatUnappealedGroup -----------------------------------------------
class TFloatUnappealedGroup : public TFloatGroup
{
public:
	TFloatUnappealedGroup();

	virtual void FormCriteria(msql &m);

	char lawKinds[SIZE_OF_KINDS];
	TGArray<TSubject> newSubjects;
	TGArray<TOldSubject> oldSubjects;
	char decKinds[SIZE_OF_KINDS];
	TGroupArray judges;
	char compositions[SIZE_OF_COMPOSITIONS];
	CDate minForceDate;
	CDate maxForceDate;
	CDate minFromDate;
	CDate maxFromDate;
	long decFinal;
};

TFloatUnappealedGroup::TFloatUnappealedGroup()
	: TFloatGroup("T_DECISION D JOIN T_SESSION S ON S.F_NO = D.F_NO AND S.F_YEAR = D.F_YEAR AND S.F_TYPE = D.F_TYPE "
		"AND S.F_KIND = D.F_SESSION_KIND AND S.F_DATE = D.F_DATE JOIN T_LAWSUIT L ON L.F_NO = D.F_NO "
		"AND L.F_YEAR = D.F_YEAR AND L.F_TYPE = D.F_TYPE LEFT JOIN T_INREG IR ON IR.F_LAWSUIT_NO = L.F_NO "
		"AND IR.F_LAWSUIT_YEAR = L.F_YEAR AND IR.F_LAWSUIT_TYPE = L.F_TYPE AND IR.F_DECISION_KIND = D.F_KIND "
		"AND IR.F_SESSION_KIND = D.F_SESSION_KIND LEFT JOIN T_LAW_INTERVAL LI ON LI.F_NO = D.F_NO "
		"AND LI.F_YEAR = D.F_YEAR AND LI.F_TYPE = D.F_TYPE AND LI.F_FINISHED = S.F_FINAL",
		"D.F_NO, D.F_YEAR, D.F_TYPE, D.F_KIND, D.F_EVENT_NO, D.F_FROM_DATE, D.F_FORCE_DATE, S.F_JUDGE, "
		"S.F_COMPOSITION, L.F_KIND AS F_LAW_KIND, L.F_SUBJECT")
{
	Add(
		new TCharList("F_LAW_KINDS", lawKinds, sizeof lawKinds),
		new TCharList("F_DEC_KINDS", decKinds, sizeof decKinds),
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TDate("F_MIN_FORCE_DATE", &minForceDate, false),
		new TDate("F_MAX_FORCE_DATE", &maxForceDate, false),
		new TDate("F_MIN_FROM_DATE", &minFromDate, false),
		new TDate("F_MAX_FROM_DATE", &maxFromDate, false),
		new TCheck("F_DEC_FINAL", &decFinal, TRUE),
		NULL
	);

	decFinal = -1;
	order = ORDER_BY_DTYN;
}

void TFloatUnappealedGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddChars("L.F_KIND", lawKinds, KIND_LAWSUITS);
	m.AddArray("L.F_SUBJECT", newSubjects);
	m.AddArray("F_SUBJECT", oldSubjects, "L.F_OLD_SUBJECT");
	m.AddChars("D.F_KIND", adjust_xxx_kinds(decKinds, KIND_VISIBLE_DECISIONS, KIND_DECISIONS), KIND_ENDOCS);
	m.AddArray("F_UCN", judges, "S.F_JUDGE");
	m.AddCompos("S.F_COMPOSITION", compositions);
	m.AddRange("D.F_FORCE_DATE", minForceDate, maxForceDate);
#if 20180516
	m.AddRange("D.F_FROM_DATE", minFromDate, maxFromDate);
	m.AddIsNull("IR.F_DATE", true);
#else  // 20180516
	{
		m.AddIsNull("(IR.F_DATE", true);
		m.NextOr();
		m.AddRange("NOT IR.F_DATE", minFromDate, maxFromDate);
		m.cat(")");
	}
#endif  // 20180516
	if (decFinal != -1)
		m.AddIsNull("LI.F_FINISHED", !decFinal);

	EndCriteria(m);
}

// ----- TUnappealedAct ------------------------------------------------------
class TUnappealedAct : public TDecision
{
public:
	TUnappealedAct();

	char judge[SIZE_OF_UCN];
	long composition;
	char lawKind;
	long subject;
};

TUnappealedAct::TUnappealedAct()
{
	Add(
		new TPersonAlias("F_JUDGE", judge, sizeof judge),
		new TLongAlias("F_COMPOSITION", &composition),
		new TCharAlias("F_LAW_KIND", &lawKind),
		new TLongAlias("F_SUBJECT", &subject),
		NULL
	);
}

#define decision OFFICE_GROUP_CAST(TUnappealedAct)

static void floatuna_judge(mstr &m, TGroup *group) { print_user(m, decision->judge); }

static void floatuna_composition(mstr &m, TGroup *group)
{
	if (PRINT_COMPOSITIONS)
		m.printf("%s с-в", Composition->Seek(decision->composition));
}

static void floatuna_law_kind(mstr &m, TGroup *group) { m.cat(Kind->Seek(decision->lawKind)); }
static void floatuna_subject(mstr &m, TGroup *group) { m.cat(TSubject::Code(decision->subject, decision->lawKind)); }
static void floatuna_dec_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(decision->kind)); }

#undef decision

static offield floatuna_fields[] =
{
	{ "F_JUDGE",       floatuna_judge },
	{ "F_COMPOSITION", floatuna_composition },
	{ "F_LAW_KIND",    floatuna_law_kind },
	{ "F_SUBJECT",     floatuna_subject },
	{ "F_NO",          NULL },
	{ "F_YEAR",        NULL },
	{ "F_DEC_KIND",    floatuna_dec_kind },
	{ "F_EVENT_NO",    NULL },
	{ "F_FROM_DATE",   NULL },
	{ "F_FORCE_DATE",  NULL },
	{ NULL, NULL }
};

// ----- TFloatUnappealedQuery -----------------------------------------------
class TFloatUnappealedQuery : public TFloatQuery
{
public:
	TFloatUnappealedQuery(TWindow *parent, const char *criteria, const char *title);

protected:
	virtual void Count();
};

TFloatUnappealedQuery::TFloatUnappealedQuery(TWindow *parent, const char *criteria, const char *title)
	: TFloatQuery(parent, criteria, "FloatUnapp.htm", title)
{
}

void TFloatUnappealedQuery::Count()
{
	TUnappealedAct act;
	TQuery q(criteria);

	while (q.Read() && Increment())
	{
		act << q;
		office.Emit(tr1start, tr1final, floatuna_fields, &act);
	}
}

static void query_unappealed(TWindow *parent, const char *criteria, const CDate &minForceDate, const CDate &maxForceDate)
{
	mstr m;

	if (!minForceDate.Empty() || !maxForceDate.Empty())
	{
		m.cat("¬лезли в з.с.");
		minForceDate.Print(m, " от %D");
		maxForceDate.Print(m, " до %D");
	}

	TFloatUnappealedQuery(parent, criteria, str(m)).Execute();
}

#include "FloatUnappealedDialog.h"
#include "FloatUnappealedDialog.cpp"

void float_unappealed(TWindow *parent)
{
	TFloatUnappealedGroup group;
	TFloatUnappealedDialog(parent, &group).Execute();
}

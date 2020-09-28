#include "cac.h"

// ----- TFloatOutRegGroup ----------------------------------------------------
enum { ORDER_BY_RTYN = ORDER_BY_USER };

TFloatOutRegGroup::TFloatOutRegGroup()
	: TFloatGroup("T_OUTREG",
		// 2008:214 LRQ: +text; text -> remark
		"O.F_DATE, O.F_IN_NO, O.F_IN_YEAR, O.F_RETURNED, O.F_RECEIVER, O.F_LAWSUIT_NO, O.F_LAWSUIT_YEAR, "
		"O.F_LAWSUIT_TYPE, O.F_VSS_INDEX, O.F_REMARK, O.F_NO, I.F_DECISION_KIND AS F_DEC_KIND, I.F_KIND AS F_IN_KIND, "
		"I.F_EXTRA_NO, IE.F_JUDGE AS F_IN_EVENT_JUDGE, O.F_REMARK_VZ, O.F_REMARK_KS")
{
	Add(
		new TCharList("F_LAW_TYPES", lawTypes, sizeof lawTypes),
		new TCharList("F_IN_REG_KINDS", inRegKinds, sizeof inRegKinds),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TDate("F_MIN_RETURNED", &minReturned, false),
		new TDate("F_MAX_RETURNED", &maxReturned, false),
		new TCharList("F_RESULTS_III", resultsIII, sizeof resultsIII),
		new TCharList("F_RESULTS_KS", resultsKS, sizeof resultsKS),
		new TCharList("F_DEC_KINDS", decKinds, sizeof decKinds),
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TRadio("F_BY_RTYN", &order, ORDER_BY_RTYN),
		new TString("F_TEXT", text, sizeof text),
		new TCharList("F_LAW_KINDS_RT", lawKindsRT, sizeof lawKindsRT),
		new TCheck("F_FINAL_DECS", &finalDecs, TRUE),
		NULL
	);

	order = ORDER_BY_DTYN;
	strcpy(lawTypes, TYPE_LAWSUITS);	// 2007:221 LPR: deselect indocs
	finalDecs = -1;
}

void TFloatOutRegGroup::FormCriteria(msql &m)
{
	mstr tables;		// 2014:049 was ctor constant
	mstr tFields(fields);	// 2017:237 for law/req kinds

	tables.printf("T_OUTREG O JOIN T_INREG I ON I.F_NO = O.F_IN_NO AND I.F_YEAR = O.F_IN_YEAR "
		// 2011:020 LPR: FIX: outreg date or returned
		//"ON IE.F_NO = O.F_IN_NO AND IE.F_YEAR = O.F_IN_YEAR AND IE.F_DATE = O.F_DATE AND IE.F_RESULT = O.F_RESULT "
		"JOIN T_INREG_EVENT IE ON IE.F_NO = O.F_IN_NO AND IE.F_YEAR = O.F_IN_YEAR AND IE.F_RESULT = O.F_RESULT "
		// 2011:304 FIX: anything not send is returned
		//"AND ((O.F_RESULT = 'Ж' AND IE.F_DATE = O.F_DATE) OR (O.F_RESULT IN ('Н', 'з') AND IE.F_DATE = O.F_RETURNED))"
		"AND ((O.F_RESULT = '%c' AND IE.F_DATE = O.F_DATE) OR (NOT O.F_RESULT = '%c' AND IE.F_DATE = O.F_RETURNED))",
		RESULT_IOREG_SEND, RESULT_IOREG_SEND);

	if (finalDecs != -1)
	{
		tables.cat(" JOIN T_SESSION S ON S.F_NO = I.F_LAWSUIT_NO AND S.F_YEAR = I.F_LAWSUIT_YEAR"
			" AND S.F_TYPE = I.F_LAWSUIT_TYPE AND S.F_KIND = I.F_SESSION_KIND AND S.F_DATE = I.F_DECISION_DATE");
	}

	bool requests;
	bool lawsuits;

	if (any(lawKindsRT))
	{
		requests = !any(lawKindsRT) || *lawKindsRT == KIND_INVALID;
		lawsuits = strcmp(lawKindsRT, C2S[KIND_INVALID]);
	}
	else
	{
		// 2015:189 FIX: also select/criteria the required fields
		requests = !any(lawTypes) || strchr(lawTypes, TYPE_REQUEST);
		lawsuits = strcmp(lawTypes, TYPE_REQUESTS);
	}

	// 2014:210 FB2: nested selects -> join; 2015:180 LPR: only needed joins
	if (lawsuits)
	{
		tables.cat(" LEFT JOIN T_LAWSUIT L ON L.F_NO = O.F_LAWSUIT_NO AND L.F_YEAR = O.F_LAWSUIT_YEAR "
			"AND L.F_TYPE = O.F_LAWSUIT_TYPE");
		tFields.cat(", L.F_KIND AS F_LAW_KIND");
	}

	if (requests)
	{
		tables.cat(" LEFT JOIN T_REQUEST R ON R.F_NO = O.F_LAWSUIT_NO AND R.F_YEAR = O.F_LAWSUIT_YEAR "
			"AND R.F_TYPE = O.F_LAWSUIT_TYPE");
		tFields.cat(", R.F_KIND AS F_REQ_KIND");
	}

	m.Begin(str(tables), str(tFields));
	m.AddChars("O.F_LAWSUIT_TYPE", lawTypes, TYPE_ORIGINS);

	if (any(lawKindsRT) && lawsuits)
	{
		if (requests)
		{
			m.AddChar("(O.F_LAWSUIT_TYPE", TYPE_REQUEST);
			m.NextOr();
			m.AddChars("L.F_KIND", lawKindsRT + 1);
			m.cat(")");
		}
		else
			m.AddChars("L.F_KIND", lawKindsRT);
	}

	if (finalDecs != -1)
		m.AddIsNull("S.F_FINAL", !finalDecs);

	// 2014:210 FB2: nested select -> joined
	if (newSubjects.Count())
	{
		if (lawsuits && requests)
		{
			m.AddArray("(L.F_SUBJECT", newSubjects);
			m.NextOr();
			m.AddArray("R.F_SUBJECT", newSubjects);
			m.cat(")");
		}
		else
			m.AddArray(lawsuits ? "L.F_SUBJECT" : "R.F_SUBJECT", newSubjects);
	}

	if (oldSubjects.Count())
	{
		if (lawsuits && requests)
		{
			m.AddArray("F_SUBJECT", oldSubjects, "(L.F_OLD_SUBJECT");
			m.NextOr();
			m.AddArray("F_SUBJECT", oldSubjects, "R.F_OLD_SUBJECT");
			m.cat(")");
		}
		else
			m.AddArray("F_SUBJECT", oldSubjects, lawsuits ? "L.F_OLD_SUBJECT" : "R.F_OLD_SUBJECT");
	}

	// 2011:192 FIX: was missing, used by judge_checkup()
	// 2014:210 FB2: nested select -> joined
	if (lawsuits)
		m.AddCompos("L.F_COMPOSITION", compositions);

	m.AddChars("I.F_KIND", inRegKinds, KIND_IN_REG_DOCS);
	m.AddRange("O.F_DATE", minDate, maxDate);
	m.AddRange("O.F_RETURNED", minReturned, maxReturned);
	if (receivers.Count())
		m.AddArray("F_UCN", receivers, "O.F_RECEIVER");
	else
		m.AddIsNull("O.F_RECEIVER", false);

	m.AddArray("F_UCN", judges, "IE.F_JUDGE");

	{
		char tResultsIII[SIZE_OF_RESOLUTIONS];

		*tResultsIII = '\0';
		for (char *s = resultsIII; *s; s++)
		{
			AppealResolution->Seek(*s);
			strcat(tResultsIII, AppealResolution->resolutions);
		}

		if (*tResultsIII)
		{
			m.AddChars("(O.F_RESULT_I", tResultsIII);
			m.NextOr();
			m.AddChars("O.F_RESULT_II", tResultsIII);
			m.cach(')');
		}
	}
	{
		char tResultsKS[SIZE_OF_RESOLUTIONS];

		*tResultsKS = '\0';
		for (char *s = resultsKS; *s; s++)
		{
			AppealResolution->Seek(*s);
			strcat(tResultsKS, AppealResolution->resolutions);
		}
		m.AddChars("O.F_RESULT_KS", tResultsKS, RESOLUTION_APPEALEDS);
	}
	m.AddArray("F_INDEX", vssIndexes, "O.F_VSS_INDEX");
	m.AddChars("I.F_DECISION_KIND", adjust_xxx_kinds(adjust_xxx_kinds(decKinds, KIND_VISIBLE_DECISIONS, KIND_DECISIONS),
		KIND_ORDERS, KIND_ORDERS_AND_IN_REG_DOCS), KIND_ENDOCS);

	if (any(text))
	{
		m.AddILike1("(O.F_TEXT", text, true);
		m.NextOr();
		m.AddILike1("O.F_REMARK", text, true);
		m.NextOr();
		m.AddILike1("O.F_REMARK_VZ", text, true);
		m.NextOr();
		m.AddILike1("O.F_REMARK_KS", text, true);
		m.cach(')');
	}

	if (order == ORDER_BY_RTYN)
		m.AddOrder("F_RETURNED, F_TYPE, F_YEAR, F_NO", "O.");
	else if (order == ORDER_BY_DTYN)
		m.AddOrder("F_DATE, F_TYPE, F_YEAR, F_NO", "O.");
}

// ----- TFloatOutReg ---------------------------------------------------------
class TFloatOutReg : public TOutReg
{
public:
	TFloatOutReg();
	CDate outDate;
	char decKind;
	char inKind;
	char inEventJudge[SIZE_OF_UCN];
	long extraNo;
	char lawKind;
	char reqKind;
};

TFloatOutReg::TFloatOutReg()
{
	Add(
		new TChar("F_DEC_KIND", &decKind),
		new TChar("F_IN_KIND", &inKind),
		new TString("F_IN_EVENT_JUDGE", inEventJudge, sizeof inEventJudge),
		new TLong("F_EXTRA_NO", &extraNo),
		new TChar("F_LAW_KIND", &lawKind),
		new TChar("F_REQ_KIND", &reqKind),
		NULL
	);
}

class TFloatOutRegQuery : public TFloatQuery
{
public:
	TFloatOutRegQuery(TWindow *parent, const char *criteria, const char *name);

protected:
	void Count();
};

TFloatOutRegQuery::TFloatOutRegQuery(TWindow *parent, const char *criteria, const char *name)
	: TFloatQuery(parent, criteria, name)
{
}

#define outReg OFFICE_GROUP_CAST(TFloatOutReg)

static void floatout_judge(mstr &m, TGroup *group)
{
	if (outReg->inNo && atob(outReg->inEventJudge))
		print_user(m, outReg->inEventJudge);
}

static void floatout_receiver(mstr &m, TGroup *group)
{
	TSender sender;

	sender.ucnType = UCN_INSTANCE_HIGHER;
	strcpy(sender.ucn, outReg->receiver);
	m.cpy(sender.Try() ? sender.name : outReg->receiver);
}

static void floatout_in_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(outReg->inKind)); }

static void floatout_dec_kind(mstr &m, TGroup *group)
{
	if (outReg->decKind)
	{
		m.cpy(strchr(KIND_ENDOCS, outReg->decKind) ? Kind->Seek(outReg->decKind) : "Разпореждане");

		if (outReg->extraNo)
			m.cat(" по възражение");
	}
}

static void floatout_law_kind(mstr &m, TGroup *group)
{
	if (outReg->lawKind)
		m.cat(Kind->Seek(outReg->lawKind));
	else if (outReg->reqKind)
		m.cat(Kind->Seek(outReg->reqKind));
	else if (outReg->lawsuit.type)
		m.cpy(Type->Seek(outReg->lawsuit.type));
}

static void floatout_remark(mstr &m, TGroup *group)
{
	m.cat(outReg->remark);

	if (any(outReg->remarkVZ))
	{
		m.sep("\ahr\b");
		m.cat(outReg->remarkVZ);
	}

	if (any(outReg->remarkKS))
	{
		m.sep("\ahr\b");
		m.cat(outReg->remarkKS);
	}

	// 2016:096
	TConnect connect;

	if (connect.TryBy(outReg->lawsuit, CONNECT_TYPE_LOWER_INSTANCE))
	{
		m.sep("\ahr\b");
		connect.Print(m, true, false, "с-в");
	}
}

static void floatout_subject(mstr &m, TGroup *group)
{
	TOriginContainer *const container = (TOriginContainer *) type2RCDKeyContainer(outReg->lawsuit.type);
	TVirtPtr VP(container);

	container->key = outReg->lawsuit;

	if (container->Get("F_SUBJECT, F_KIND, F_OLD_SUBJECT"))
	{
		if (container->subject && container->LawKind())
			m.cat(container->Code());

		print_old_subject(m, container->oldSubject);
	}
}

#undef outReg

static const offield outReg_fields[] =
{
	{ "F_JUDGE", floatout_judge },
	{ "F_VSS_INDEX", NULL },
	{ "F_RETURNED", NULL },
	{ "F_NO", NULL },
	{ "F_DATE", NULL },
	{ "F_RECEIVER", floatout_receiver },
	{ "F_IN_KIND", floatout_in_kind },
	{ "F_DEC_KIND", floatout_dec_kind },
	{ "F_LAWSUIT_KIND", floatout_law_kind },
	{ "F_LAWSUIT_NO", NULL },
	{ "F_LAWSUIT_YEAR", NULL },
	{ "F_REMARK", floatout_remark },
	{ "F_SUBJECT", floatout_subject },
	{ NULL, NULL }
};

void TFloatOutRegQuery::Count()
{
	TFloatOutReg outReg;
	TQuery q(criteria);

	while (q.Read() && Increment())
	{
		outReg << q;

		//if (strchr(KIND_IN_NOEVENTS, outReg.inKind))
		if (!atob(outReg.inEventJudge))
		{
			TLawsuit lawsuit;
			lawsuit.key = outReg.lawsuit;

			if (lawsuit.Try("F_JUDGE"))
				strcpy(outReg.inEventJudge, lawsuit.judge);
		}

		office.Emit(tr1start, tr1final, outReg_fields, &outReg);
	}
}
// ----- query ---------------------------------------------------------------
static void query_outreg(TWindow *parent, const char *criteria)
{
	TFloatOutRegQuery(parent, criteria, "FloatOut.htm").Execute();
}

#include "VSSIndexListBox.h"
#include "VSSIndexListBox.cpp"

#include "LawKindsRTFace.h"
#include "LawKindsRTFace.cpp"

#include "FloatOutRegDialog.h"
#include "FloatOutRegDialog.cpp"

void float_outreg(TWindow *parent)
{
	TFloatOutRegGroup group;
	TFloatOutRegDialog(parent, &group).Execute();
}

// ----- TPeriodOutRegGroup --------------------------------------------------
class TPeriodOutRegGroup : public TPeriodUCNGroup
{
public:
	TPeriodOutRegGroup();

	virtual void FormCriteria(msql &m);
};

TPeriodOutRegGroup::TPeriodOutRegGroup()
	: TPeriodUCNGroup("T_SIDE_WIND W JOIN T_OUTREG O ON O.F_NO = W.F_NO AND O.F_YEAR = W.F_YEAR "
		"AND O.F_TYPE = W.F_TYPE AND O.F_KIND = W.F_KIND AND O.F_DATE = W.F_DATE")
{
	order = ORDER_BY_NULL;
}

void TPeriodOutRegGroup::FormCriteria(msql &m)
{
	m.Begin(Name, "O.F_NO, O.F_KIND, O.F_DATE, O.F_TEXT, O.F_LAWSUIT_NO, O.F_LAWSUIT_YEAR, O.F_LAWSUIT_TYPE");
	InnerCriteria(m);
}

// ----- query ---------------------------------------------------------------
#define outReg OFFICE_GROUP_CAST(TOutReg)

static void floatpuo_kind(mstr &m, TGroup *group)
{
	m.cpy(strchr(KIND_IOREG_OTHERS, outReg->kind) ? str(outReg->text) : Kind->Seek(outReg->kind));
}

static void floatpuo_lawsuit_type(mstr &m, TGroup *group)
{
	if (outReg->lawsuit.type)
		m.cpy(Type->Seek(outReg->lawsuit.type));
}

static void floatpuo_lawsuit_no_year(mstr &m, TGroup *group)
{
	if (outReg->lawsuit.no || outReg->lawsuit.year)
		m.printf("%ld/%ld", outReg->lawsuit.no, outReg->lawsuit.year);
}

#undef outReg

static const offield querypuo_fields[] =
{
	{ "F_KIND", floatpuo_kind },
	{ "F_NO", keygroup_key_no },
	{ "F_DATE", container_date },
	{ "F_LAWSUIT_TYPE", floatpuo_lawsuit_type },
	{ "F_LAWSUIT_NO_YEAR", floatpuo_lawsuit_no_year },
	{ NULL, NULL }
};

class TPeriodOutRegQuery : public TFloatQuery
{
public:
	TPeriodOutRegQuery(TWindow *parent, const char *criteria, const char *title);

	virtual void Count();
};

TPeriodOutRegQuery::TPeriodOutRegQuery(TWindow *parent, const char *criteria, const char *title)
	: TFloatQuery(parent, criteria, "QueryPuo.htm", title)
{
}

void TPeriodOutRegQuery::Count()
{
	TQuery q(criteria);
	TOutReg outReg;

	while (q.Read() && Increment())
	{
		outReg << q;
		office.Emit(tr1start, tr1final, querypuo_fields, &outReg);
	}
}

void period_outreg(TWindow *parent, TFoundGroup *ucnGroup, TPeriodGroup *period)
{
	msql m, title;
	TPeriodOutRegGroup group;

	group << ucnGroup;
	group << period;
	group.FormCriteria(m);

	group.Title(title, "Изпратени на", "дела/документи");
	TPeriodOutRegQuery(parent, str(m), str(title)).Execute();
}

#include "cac.h"

#if COURT_TYPE == COURT_REGIONAL
// ----- THeritageGroup ------------------------------------------------------
class THeritageGroup : public TBookGroup
{
public:
	THeritageGroup(const char *tExtraTables, const char *tExtraFields);

	virtual void FormCriteria(msql &m);
	const char *Heritages() { return heritages; }

protected:
	char heritages[SIZE_OF_RESULTS];
	char sourceKinds[SIZE_OF_KINDS];

	const char *extraTables;
	const char *extraFields;
};

THeritageGroup::THeritageGroup(const char *tExtraTables, const char *tExtraFields)
	: TBookGroup("T_DECISION", "F_TYPE", "F_DATE", false), extraTables(tExtraTables), extraFields(tExtraFields)
{
	Add(
		new TCharList("F_HERITAGES", heritages, sizeof heritages),
		new TCharList("F_SOURCE_KINDS", sourceKinds, sizeof sourceKinds),
		NULL
	);

	minNo = 0;
	order = ORDER_BY_TYN;	// T is const, so YN
}

void THeritageGroup::FormCriteria(msql &m)
{
	mstr tables("T_LAWSUIT L JOIN T_DECISION D ON D.F_NO = L.F_NO AND D.F_YEAR = L.F_YEAR AND D.F_TYPE = L.F_TYPE "
		"JOIN T_SESSION S ON S.F_NO = D.F_NO AND S.F_YEAR = D.F_YEAR AND S.F_TYPE = D.F_TYPE AND S.F_DATE = D.F_DATE "
		"AND S.F_KIND = D.F_SESSION_KIND");

	if (extraTables)
		tables.printf(" JOIN %s", extraTables);

	mstr fields("L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_DATE, L.F_COMPOSITION, L.F_SOURCE_NO, L.F_SOURCE_YEAR, "
		"L.F_SOURCE_TYPE, L.F_SOURCE_KIND, L.F_TEXT, L.F_FINISHED, L.F_GRANTED, L.F_FLAGS, D.F_FROM_DATE, D.F_RESULT, "
		"D.F_EVENT_NO, D.F_KIND AS F_DEC_KIND, D.F_DATE AS F_DEC_DATE");

	if (extraFields)
		fields.printf(", %s", extraFields);

	m.Begin(str(tables), str(fields));
	m.AddChar("L.F_TYPE", TYPE_CITIZEN_LAW);
	m.AddChars("L.F_SOURCE_KIND", sourceKinds);
	mstr results(heritages);	// 2016:351 local heritage support
	results.cat(RESULT_WITH_READY_ANYS);
	m.AddChars("D.F_RESULT", str(results));		// 2014:261 FIX: was missing
}

// ----- THeritageBookGroup---------------------------------------------------
class THeritageBookGroup : public THeritageGroup
{
public:
	THeritageBookGroup();

	long byFinal;

	virtual void FormCriteria(msql &m);
};

THeritageBookGroup::THeritageBookGroup()
	 : THeritageGroup(NULL, NULL)
{
	Add(
		new TRadio("F_BY_FINAL", &byFinal, TRUE),
		new TRadio("F_BY_FORCE", &byFinal, FALSE),
		NULL
	);

	strcpy(sourceKinds, KIND_HERITAGE_INDOCS);
	strcpy(heritages, RESULT_HERITAGE_STATES);
	byFinal = TRUE;
}

void THeritageBookGroup::FormCriteria(msql &m)
{
	if (!byFinal)
		extraFields = "D.F_FORCE_DATE AS F_DEC_FORCE";

	THeritageGroup::FormCriteria(m);

	if (byFinal)
	{
		// 2017:013 or F_FROM_DATE if no final
		m.AddRange("(S.F_FINAL", minDate, maxDate);
		m.NextOr();
		m.AddIsNull("(S.F_FINAL", true);
		m.AddRange("D.F_FROM_DATE", minDate, maxDate);
		m.cat("))");
	}
	else  // 2018-05-31
		m.AddRange("D.F_FORCE_DATE", minDate, maxDate);
}

// ----- THeritageLawsuit ----------------------------------------------------
class THeritageLawsuit : public TLawsuit
{
public:
	THeritageLawsuit();

	bool operator<=(const THeritageLawsuit &that) const;

	CDate fromDate;
	char result;
	long eventNo;
	char decKind;
	CDate decDate;
	CDate reqDate;
	CDate decForce;
	TSideWind *sideWind;

	TSideWindArray suitors;
	TSideWindArray defendants;
};

THeritageLawsuit::THeritageLawsuit()
{
	Add(
		new TDate("F_FROM_DATE", &fromDate, false),
		new TCharAlias("F_RESULT", &result),
		new TLong("F_EVENT_NO", &eventNo),
		new TCharAlias("F_DEC_KIND", &decKind),
		new TDate("F_DEC_DATE", &decDate, false),
		new TDate("F_REQ_DATE", &reqDate, false),
		new TDate("F_DEC_FORCE", &decForce, false),		
		NULL
	);
}

bool THeritageLawsuit::operator<=(const THeritageLawsuit &that) const
{
	if (!decForce.Empty())
		return decForce != that.decForce ? decForce < that.decForce :
			key.year != that.key.year ? key.year < that.key.year :
			key.no <= that.key.no;

	int compare = fromDate.Compare(that.fromDate);
	return compare ? compare < 0 : (!that.eventNo || eventNo <= that.eventNo);
}

#define lawsuit OFFICE_GROUP_CAST(THeritageLawsuit)

static void heritage_request(mstr &m, TGroup *group)
{
	m.printf("Вх № %d/%dг., %s %d/%dг.", lawsuit->source.no, lawsuit->source.year, KindAlias->Seek(lawsuit->kind),
		flag_key_no(&lawsuit->key), lawsuit->key.year);

	if (PRINT_COMPOSITIONS)
		m.printf(", %s състав", Composition->Seek(lawsuit->composition));
}

static void heritage_date(mstr &m, TGroup *group)
{
	TRequest request;
	request.key = lawsuit->source;

	if (request.Try("F_DATE"))
		request.date.Print(m);
}

static void heritage_suitors(mstr &m, TGroup *group)
{
	for (int i = 0; i < lawsuit->suitors.Count(); i++)
	{
		TSideWind *sideWind = lawsuit->suitors[i];

		m.sep("\v");
		m.cat(Involvement->Seek(sideWind->involvement));
		m.cat("\v");
		print_name(m, sideWind->ucn, sideWind->ucnType);

		if (sideWind->ucnType == UCN_CITIZEN_UCN && PRINT_UCN)
			m.printf(" ЕГН %s", sideWind->ucn);

		TAddress address;

		address.ucnType = sideWind->ucnType;
		strcpy(address.ucn, sideWind->ucn);

		if (address.Try("F_UCLP"))
		{
			TUCLP uclp;
			uclp.uclp = address.uclp;

			if (uclp.Try())
				m.printf(" %s", uclp.name);
		}
	}
}

static void heritage_reject(mstr &m, TGroup *group)
{
	if (lawsuit->sourceKind == KIND_HERITAGE_REJECT)
		m.cpy(Kind->Seek(lawsuit->sourceKind));
}

static void heritage_accept(mstr &m, TGroup *group)
{
	// 2014:206 LPR: accept shows REQ_51 too
	if (lawsuit->sourceKind != KIND_HERITAGE_REJECT)
		m.cpy(Kind->Seek(lawsuit->sourceKind));
}

static void heritage_defendants(mstr &m, TGroup *group)
{
	for (int i = 0; i < lawsuit->defendants.Count(); i++)
	{
		TSideWind *sideWind = lawsuit->defendants[i];

		m.sep("\v");
		m.cat(Involvement->Seek(sideWind->involvement));
		m.cat("\v");
		print_name(m, sideWind->ucn, sideWind->ucnType);

		if (sideWind->ucnType == UCN_CITIZEN_UCN && PRINT_UCN)
			m.printf(" ЕГН %s", sideWind->ucn);
	}
}

static void heritage_text(mstr &m, TGroup *group) { m.cpy(lawsuit->text); }

static void heritage_finished(mstr &m, TGroup *group)
{
	m.cpy(Kind->Seek(lawsuit->decKind));

	if (PRINT_JUDGEMENTS && lawsuit->eventNo)
		m.printf(" № %d/", lawsuit->eventNo);
	else
		m.cat(" от ");

	m.printf("%Dг.", &lawsuit->fromDate);
	m.printf("\v%s", Result->Seek(lawsuit->result));
	lawsuit->decForce.Print(m, "\vв з.с. %D");
}

static void heritage_float_request(mstr &m, TGroup *group)
{
	m.printf("%s %d/%Dг.", Kind->Seek(lawsuit->sourceKind), lawsuit->source.no, &lawsuit->reqDate);
}

static void heritage_lawsuit(mstr &m, TGroup *group)
{
	m.printf("%s %d/%dг.", KindAlias->Seek(lawsuit->kind), flag_key_no(&lawsuit->key), lawsuit->key.year);

	if (PRINT_COMPOSITIONS)
		m.printf(", %s състав", Composition->Seek(lawsuit->composition));
}

static void heritage_suitor(mstr &m, TGroup *group)
{
	TSideWind *sideWind = lawsuit->sideWind;

	print_name(m, sideWind->ucn, sideWind->ucnType);

	if (sideWind->ucnType == UCN_CITIZEN_UCN && PRINT_UCN)
		m.printf(" ЕГН %s", sideWind->ucn);

	m.printf(" - %s", Involvement->Seek(sideWind->involvement));
}

static void heritage_result(mstr &m, TGroup *group)
{
	m.cat(Result->Seek(heritageState2Result(lawsuit->sideWind->state)));
}

static void heritage_decision(mstr &m, TGroup *group)
{
	// Note: subset of heritage_finished()
	m.cpy(Kind->Seek(lawsuit->decKind));

	if (PRINT_JUDGEMENTS && lawsuit->eventNo)
		m.printf(" № %d/", lawsuit->eventNo);
	else
		m.cat(" от ");

	m.printf("%Dг.", &lawsuit->fromDate);
}

#undef lawsuit

static const offield heritage_book_fields[] =
{
	{ "F_REQUEST",	heritage_request },
	{ "F_DATE",		heritage_date },
	{ "F_SUITORS",	heritage_suitors },
	{ "F_REJECT",	heritage_reject },
	{ "F_ACCEPT",	heritage_accept },
	{ "F_DEFENDANTS",	heritage_defendants },
	{ "F_TEXT",		heritage_text },
	{ "F_FINISHED",	heritage_finished },
	{ NULL, NULL }
};

static const offield heritage_float_fields[] =
{
	{ "F_REQUEST",	heritage_float_request },
	{ "F_LAWSUIT",	heritage_lawsuit },
	{ "F_SUITOR",	heritage_suitor },
	{ "F_RESULT",	heritage_result },
	{ "F_DEFENDANTS",	heritage_defendants },
	{ "F_DECISION",	heritage_decision },
	{ NULL, NULL }
};

// ----- THeritageQuery ----------------------------------------------------------------
class THeritageQuery : public TBook
{
public:
	THeritageQuery(TWindow *parent, THeritageGroup *group, const char *name, const offield *bodyFields);

protected:
	virtual TGroup *Process(TQuery &q);

	mstr heritages;
	void DecisionCriteria(msql &m, const THeritageLawsuit *lawsuit);
};

THeritageQuery::THeritageQuery(TWindow *parent, THeritageGroup *group, const char *name, const offield *bodyFields)
	: TBook(parent, group, name, bodyFields, ::book_fields, BOOK_OLD_FORMAT, new TGSArray<THeritageLawsuit>)
{
	for (const char *res = group->Heritages(); *res; res++)
		heritages.cach(heritageResult2State(*res));
}

void THeritageQuery::DecisionCriteria(msql &m, const THeritageLawsuit *lawsuit)
{
	m.Begin("T_SIDE_WIND", "F_UCN, F_UCN_TYPE, F_INVOLVEMENT, F_STATE");
	m.Add(lawsuit, "F_NO, F_YEAR, F_TYPE");
	m.AddChar("F_KIND", lawsuit->decKind);
	m.AddDate("F_DATE", lawsuit->decDate);
	m.AddChars("F_INVOLVEMENT", INVOLVEMENT_HERITAGES);
	m.AddChars("F_STATE", str(heritages));
}

TGroup *THeritageQuery::Process(TQuery &q)
{
	THeritageLawsuit *lawsuit = new THeritageLawsuit;
	*lawsuit << q;

	// PASS 1: RESULT HERITAGES
	if (strchr(RESULT_HERITAGE_ENDOCS, lawsuit->result))
	{
		msql m;
		msql involvements(INVOLVEMENT_HERITAGES);

		load_sidebasicv_criteria(m, lawsuit);
		m.CutOrder();
		involvements.cach(INVOLVEMENT_LEGATOR);
		m.AddChars("F_INVOLVEMENT", str(involvements));

		TQuery r(str(m));
		while (r.Read())
		{
			TSideWind *sideWind = new TSideWind;
			*sideWind << r;

			if (strchr(INVOLVEMENT_HERITAGES, sideWind->involvement))
			{
				sideWind->state = heritageResult2State(lawsuit->result);
				lawsuit->suitors.Add(sideWind);
			}
			else
				lawsuit->defendants.Add(sideWind);
		}

		// check for conflicting local states
		TSideWind tSideWind;
		DecisionCriteria(m, lawsuit);
		TQuery p(str(m));

		while (p.Read())
		{
			m.clear();
			lawsuit->Print(m);
			m.printf(" %s от %D: ", Kind->Seek(lawsuit->decKind), &lawsuit->fromDate);
			tSideWind << p;
			tSideWind.Print(m);
			m.compress();
			dprintf("%s: общ и индивидуален резултат", str(m));
		}

		return lawsuit;
	}

	// PASS 2: RESULT READY ANYS
	{	// result with ready any-s
		TSideWindArray defendants;
		bool defendantsLoaded = false;
		msql m;

		DecisionCriteria(m, lawsuit);
		TQuery r(str(m));
		while (r.Read())
		{
			THeritageLawsuit *tLawsuit = new THeritageLawsuit;
			TSideWind *suitor = new TSideWind;

			*tLawsuit << lawsuit;
			*suitor << r;
			tLawsuit->suitors.Add(suitor);

			if (!defendantsLoaded)
			{
				load_sidebasicv_criteria(m, lawsuit);
				m.CutOrder();
				m.AddChar("F_INVOLVEMENT", INVOLVEMENT_LEGATOR);
				load_sidebasicv(defendants, str(m));
				defendantsLoaded = true;
			}

			for (int i = 0; i < defendants.Count(); i++)
				tLawsuit->defendants.AddCopy(defendants[i]);

			tLawsuit->result = heritageState2Result(suitor->state);
			array->Add(tLawsuit);
		}
	}

	delete lawsuit;
	return NULL;
}

// ----- THeritageBook -----------------------------------------------------------------
class THeritageBook : public THeritageQuery
{
public:
	THeritageBook(TWindow *parent, THeritageBookGroup *group);

protected:
	const char *tr1reject;
	const char *tr1shared;
	const char *tr1accept;

	virtual void Markers();
	virtual void EmitRow(TGroup *group);
};

THeritageBook::THeritageBook(TWindow *parent, THeritageBookGroup *group)
	: THeritageQuery(parent, group, "HeritBk.htm", heritage_book_fields)
{
}

void THeritageBook::Markers()
{
	tr1start = office.Find("tr1 start");
	tr1reject = office.Find("tr1 reject");
	tr1shared = office.Find("tr1 shared");
	tr1accept = office.Find("tr1 accept");
	tr1final = office.Find("tr1 final");
}

#define lawsuit OFFICE_GROUP_CAST(THeritageLawsuit)

void THeritageBook::EmitRow(TGroup *group)
{
	office.Emit(tr1start, tr1reject, body_fields, group);

	if (lawsuit->sourceKind == KIND_HERITAGE_REQ_51)
		office.Emit(tr1shared, tr1accept, null_fields, NULL);
	else
		office.Emit(tr1reject, tr1shared, body_fields, group);

	office.Emit(tr1accept, tr1final, body_fields, group);
}

#undef lawsuit

#include "HeritageBookDialog.h"
#include "HeritageBookDialog.cpp"

void heritage_book(TWindow *parent)
{
	THeritageBookGroup group;

	if (THeritageBookDialog(parent, &group).Execute() == IDOK)
		THeritageBook(parent, &group).Execute(BOOK_EXEC_HEADER);
}

// ----- TFloatHeritageGroup -----------------------------------------------------------
class TFloatHeritageGroup : public THeritageGroup
{
public:
	TFloatHeritageGroup();

	virtual void FormCriteria(msql &m);

protected:
	long minYear;
	long maxYear;
};

TFloatHeritageGroup::TFloatHeritageGroup()
	: THeritageGroup("T_REQUEST R ON R.F_NO = L.F_SOURCE_NO AND R.F_YEAR = L.F_SOURCE_YEAR", "R.F_DATE AS F_REQ_DATE")
{
	Add(
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		NULL
	);
}

void TFloatHeritageGroup::FormCriteria(msql &m)
{
	THeritageGroup::FormCriteria(m);
	m.AddRange("L.F_NO", minNo, maxNo);
	m.AddRange("L.F_YEAR", minYear, maxYear);
	m.AddRange("D.F_FROM_DATE", minDate, maxDate);
	EndCriteria(m);
}

// ----- TFloatHeritageQuery -----------------------------------------------------------
class TFloatHeritageQuery : public THeritageQuery
{
public:
	TFloatHeritageQuery(TWindow *parent, TFloatHeritageGroup *group);

protected:
	virtual void EmitBody();
};

TFloatHeritageQuery::TFloatHeritageQuery(TWindow *parent, TFloatHeritageGroup *group)
	: THeritageQuery(parent, group, "FloatHerit.htm", heritage_float_fields)
{
}

void TFloatHeritageQuery::EmitBody()
{
	office.Emit(NULL, tr1start, book_fields, group);
	group->count = 0;

	for (int i = 0; i < array->Count(); i++)
	{
		THeritageLawsuit *lawsuit = (THeritageLawsuit *) (*array)[i];

		for (int n = 0; n < lawsuit->suitors.Count(); n++)
		{
			lawsuit->sideWind = lawsuit->suitors[n];
			EmitRow(lawsuit);
			group->count++;
		}

		wait->TickCount(group->count, "отпечатани записа");
	}

	office.Emit(tr1final, NULL, book_fields, group);
}

#include "FloatHeritageDialog.h"
#include "FloatHeritageDialog.cpp"

void float_heritage(TWindow *parent)
{
	TFloatHeritageGroup group;

	if (TFloatHeritageDialog(parent, &group).Execute() == IDOK)
		TFloatHeritageQuery(parent, &group).Execute();
}
#endif  // REGIONAL

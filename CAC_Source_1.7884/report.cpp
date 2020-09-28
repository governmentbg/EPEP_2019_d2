#include "cac.h"

// ----- TReportGroup --------------------------------------------------------
TReportGroup::TReportGroup()
	: TGroup("T_REPORT", NULL)
{
	Add(
		new TLongAlias("F_MIN_MONTH", (long *) &start.month),
		new TLong("F_MIN_YEAR", &start.year),
		new TLongAlias("F_MAX_MONTH", (long *) &final.month),
		new TLong("F_MAX_YEAR", &final.year),
		NULL
	);

	Add(
		new TDate("F_START", &start, true),
		new TDate("F_FINAL", &final, true),
		new TRadio("F_INSTANCE_1ST", &instances, INSTANCE_1ST),
		new TRadio("F_INSTANCE_2ND", &instances, INSTANCE_2ND),
		new TRadio("F_INSTANCE_ALL", &instances, INSTANCE_ALL),
		new TCharList("F_TYPES", types, sizeof types),
		new TLongAlias("F_COMPOSITION", &composition),
		new TCharAlias("F_CONNECT_TYPE", &connectType),
		NULL
	);

	instances = INSTANCE_ALL;
}

void TReportGroup::Import(begMYEndMYTypeCIU *pFMJWACSW)
{
	start = pFMJWACSW->BegDate();
	final = pFMJWACSW->EndDate();
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	instances = (pFMJWACSW->firstInstLaws == pFMJWACSW->secondInstLaws) ? INSTANCE_ALL :
		pFMJWACSW->firstInstLaws ? INSTANCE_1ST : INSTANCE_2ND;
#endif  // ADMIN || TYPE == AREAL || TYPE == COURT_MILITARY
	composition = pFMJWACSW->composition;
	strcpy(types, pFMJWACSW->types);
#if COURT_TYPE == COURT_REGIONAL
	for (int i = 0; i < pFMJWACSW->lArrPtr()->Count(); i++)
		subjects.Add((*pFMJWACSW->lArrPtr())[i]);
#endif  // REGIONAL
}

// ----- TReportLawsuit -------------------------------------------------
TReportLawsuit::TReportLawsuit()
{
	Add(
		new TLong("F_INTERVAL_FLAGS", &intervalFlags),
		new TChar("F_SESSION_KIND", &sessionKind),
		new TChar("F_SESSION_RESULT", &sessionResult),
		new TDate("F_SESSION_FINAL", &sessionFinal, false),
		new TDate("F_STARTED", &started, false),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		new TString("F_UCN", ucn, sizeof ucn),
		NULL
	);
}

bool TReportLawsuit::SessionLeft()
{
	int outcomes = session_outcomes(sessionResult);
	return outcomes == SESSION_LEFT || outcomes == SESSION_STOPPED;
}

bool TReportLawsuit::SessionOtherAct()
{
	if (sessionFinal == finished || sessionFinal.Empty())
		return false;

	msql m("T_LAW_INTERVAL", "F_NO");

	m.Add(this, "F_NO, F_YEAR, F_TYPE");
	m.AddDate("F_FINISHED", sessionFinal);

	TQuery q(str(m));
	return !q.Read();
}

// ----- TReport --------------------------------------------------------
TReport::TReport(TWindow *parent, const TReportGroup &tReportGroup)
	: TOutput(parent, "отчета", WAIT_MORE), reportGroup(tReportGroup), types(tReportGroup.types),
		start(tReportGroup.start), final(tReportGroup.final), composition(tReportGroup.composition),
	#if COURT_TYPE == COURT_REGIONAL
		subjects(tReportGroup.subjects),
	#endif  // REGIONAL
		htmlog("журнален"), stoppedLaws(0)
{
}

void TReport::SetOutKey()
{
	sprintf(sOutKey, "Изходящ документ %ld/%ldг.", outReg.key.no, outReg.key.year);
}

void TReport::SetInKey()
{
	sprintf(sInKey, "Входящ документ %ld/%ldг.", outReg.inNo, outReg.inYear);
}

bool TReport::FindFinalizer()
{
//	session.key = lawsuit.key;	// for find_finalizer(session, decision...)
	return TOutput::FindFinalizer(lawsuit, session, start, final);
}

char TReport::PublicActAge()
{
	if (session.result == RESULT_PUBLIC_DECISION)
	{
		TGArray<TDecision> decisions;

		session.key = lawsuit.key;
		// 2008:213 LRQ: KIND_DECISIONS -> NULL for sync w/ timed ses
		if (try2decide(&session, &decisions, NULL, "F_FROM_DATE"))
			return interval2ActAge(session.date, decisions[0]->fromDate);
	}

	return AGE_NULL;
}

TSubject *TReport::SeekSubject()
{
	Subject->Seek(lawsuit.key.type, lawsuit.subject);
	return Subject;
}

void TReport::dlprintf(const char *format, ...)
{
	long dlaw = lawsuit.key.type * 10000000 + lawsuit.key.no * 100 + (lawsuit.key.year - YEAR_MIN);
	va_list ap;

	for (int i = 0; i < debugLaws.Count(); i++)
		if (debugLaws[i] == dlaw)
			return;

	va_start(ap, format);
	vdprintf(format, ap);
	va_end(ap);

	debugLaws.Add(dlaw);
}

void TReport::CritBasic(msql &m, const char *typeName)
{
	m.AddChars(typeName, types);

	if (composition)
		m.AddLong("L.F_COMPOSITION", composition);

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	int instance = reportGroup.instances % INSTANCE_ALL;

	if (instance)
	{
		constant KIND_INST_LAWSUITS = (instance == INSTANCE_1ST) ? KIND_1ST_LAWSUITS : KIND_2ND_LAWSUITS;
		mstr lawKinds;

		for (const char *lk = KIND_INST_LAWSUITS; *lk; lk++)
			if (strchr(types, kind2type(*lk)))
				lawKinds.cach(*lk);

		lawCrit.AddChars("L.F_KIND", str(lawKinds));
	}
#endif  // ADMIN || AREAL || MILITARY
}

void TReport::LawCritBasic()
{
	mstr tables("T_LAW_INTERVAL I JOIN T_LAWSUIT L ON L.F_NO = I.F_NO AND L.F_YEAR = I.F_YEAR AND L.F_TYPE = I.F_TYPE");
	mstr fields("I.F_NO, I.F_YEAR, I.F_TYPE, L.F_KIND, I.F_DATE, L.F_JUDGE, L.F_SUBJECT, I.F_AGE, I.F_FINISHED, "
		"I.F_GRANTED, L.F_DANGER, L.F_SOURCE_KIND, L.F_FLAGS, L.F_QUICK, I.F_FLAGS AS F_INTERVAL_FLAGS, "
		"L.F_FORCE_DATE, L.F_DATE AS F_STARTED, L.F_CUTDOWN");

	if (reportGroup.connectType)
	{
		tables.printf(" LEFT JOIN T_CONNECT C ON C.F_NO = I.F_NO AND C.F_YEAR = I.F_YEAR AND C.F_TYPE = I.F_TYPE "
			"AND C.F_CONNECT_TYPE = '%c'", reportGroup.connectType);
		fields.cat(", C.F_UCN_TYPE, C.F_UCN");
	}

	lawCrit.Begin(str(tables), str(fields));
	CritBasic(lawCrit, "I.F_TYPE");
	lawCrit.cat(" AND ");
}

void TReport::LawCritExtra()
{
}

void TReport::SesCritExtra()
{
}

void TReport::QueryLawsuits(const char *tWait, int what, const char *waitCount)
{
	wait->SetText(tWait);
	int tCount = 0;
	TQuery q(str(lawCrit));
	mstr m;

	while (q.Read())
	{
		lawsuit << q;
		SetLawKey();

		if (waitCount)
		{
			m.clear();
			m.printf(waitCount, ++tCount);
			wait->TickText(str(m));
		}

		CountLawsuits(what);
	}
	// 2016:188 debug / old criteria
	lawCrit.clear();
}

void TReport::CountStart()
{
	LawCritBasic();
	//lawCrit.printf("F_DATE < %ld AND (F_FINISHED IS NULL OR F_FINISHED >= %ld)", start.Value(), start.Value());
	lawCrit.printf("I.F_DATE < %ld AND (I.F_FINISHED IS NULL OR I.F_FINISHED >= %ld)", start.Value(), start.Value());
	LawCritExtra();
	QueryLawsuits("Несвършени дела в началото на периода...", COUNT_UNFIN_START);
}

void TReport::CountPeriod()
{
	LawCritBasic();
	//lawCrit.printf("F_DATE BETWEEN %ld AND %ld", start.Value(), final.Value());
	lawCrit.printf("I.F_DATE BETWEEN %ld AND %ld", start.Value(), final.Value());
	LawCritExtra();
	QueryLawsuits("Постъпили дела през отчетния период...", COUNT_NEW_PERIOD, "%d постъпили дела...");
}

void TReport::CountFinished()
{
	LawCritBasic();
	//lawCrit.printf("F_FINISHED BETWEEN %ld AND %ld", start.Value(), final.Value());
	lawCrit.printf("I.F_FINISHED BETWEEN %ld AND %ld", start.Value(), final.Value());
	LawCritExtra();
	QueryLawsuits("Свършени дела...", COUNT_FINISHED, "%d свършени дела...");
}

void TReport::CountFinal()
{
	LawCritBasic();
	//lawCrit.printf("F_DATE <= %ld AND (F_FINISHED IS NULL OR F_FINISHED > %ld)", final.Value(), final.Value());
	lawCrit.printf("I.F_DATE <= %ld AND (I.F_FINISHED IS NULL OR I.F_FINISHED > %ld)", final.Value(), final.Value());
	LawCritExtra();
	QueryLawsuits("Несвършени дела в края на периода...", COUNT_UNFIN_FINAL);
}

void TReport::CountSessions()
{
	lawCrit.Begin("T_SESSION S JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE",
		"S.F_TYPE, L.F_SUBJECT, L.F_KIND, S.F_KIND AS F_SESSION_KIND, S.F_RESULT AS F_SESSION_RESULT, "
		"S.F_FINAL AS F_SESSION_FINAL");
	CritBasic(lawCrit, "S.F_TYPE");
	lawCrit.AddRange("S.F_DATE", start, final);
	lawCrit.AddChars("NOT S.F_RESULT", RESULT_UNLINKEDS);
	LawCritExtra();
	SesCritExtra();
	QueryLawsuits("Проведени заседания...", COUNT_RL_SESSION, "%d проведени заседания...");
}

void TReport::CheckStopped()
{
	if (lawsuit.age > AGE_ONE_OR_LESS)
	{
		msql m("T_SESSION", "F_RESULT");

		m.Add(&lawsuit, "F_NO, F_YEAR, F_TYPE");
		m.AddRange("F_DATE", lawsuit.date, lawsuit.finished);
		m.AddChar("F_RESULT", RESULT_STOPPED);

		TQuery q(str(m));
		if (q.Read())
		{
			if (!stoppedLaws)
			{
				htmlog.OpenTemp();
				htmlog.puts("Свършили дела с продължителност над 1 месец, които са били спрени:\n\n");
			}

			htmlog.printf("%s е било спряно\n", sLawKey);
			stoppedLaws++;
		}
	}
}

void TReport::ShowStopped()
{
	if (stoppedLaws)
		htmlog.Show(wait, false, false);
}

#define report OFFICE_GROUP_CAST(TReport)

static void report_period(mstr &m, TGroup *group)
{
	m.cpy(Month->Seek(report->start.month));
	if (report->start.year != report->final.year)
		m.printf(" %ldг.", report->start.year);
	if (report->start.year != report->final.year || report->start.month != report->final.month)
		m.printf(" - %s", Month->Seek(report->final.month));
	m.printf(" %ldг.", report->final.year);
}

static void report_types(mstr &m, TGroup *group)
{
	for (const char *type = report->types; *type; type++)
	{
		if (type > report->types)
			m.cat(", ");

		m.cat(Type->Seek(*type));
	}
}

#if COURT_TYPE == COURT_REGIONAL
static void report_subjects(mstr &m, TGroup *group)
{
	for (int i = 0; i < report->subjects.Count(); i++)
	{
		m.sep(", ");
		m.printf("%ld", report->subjects[i] / SUBJECT_MULTIPLY);
	}
}
#endif  // REGIONAL

static void report_composition(mstr &m, TGroup *group)
{
	if (PRINT_COMPOSITIONS && report->composition)
		m.printf(" %s състав", Composition->Seek(report->composition));
}

#undef report

const offield report_fields[] =
{
	{ "F_PERIOD",      report_period },
	{ "F_TYPES",       report_types },
#if COURT_TYPE == COURT_REGIONAL
	{ "F_SUBJECTS",    report_subjects },
#endif  // REGIONAL
	{ "F_COMPOSITION", report_composition },
	{ NULL,            NULL }
};

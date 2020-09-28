#include "cac.h"

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
// ----- TPun1stRangeReport --------------------------------------------------
class TPun1stRangeReport : public TRangeReport
{
public:
	TPun1stRangeReport(TWindow *parent, const TRangeReportGroup &reportGroup);

protected:
	TJudgement judgement;
	TUnfinishedLawsuit unfinished;

	int CountCanceleds();
	void JudgementCriteria();
	virtual void SplitJudgements() = 0;
	void CountJudgements();
	void SplitReinvests();
	void CountSlowMotives();
	int CountRenewInregs();
	int CountStoppedLaws();
	bool CumulationFinish();

	enum
	{
		Q3_L1 = 0,
		Q3_L2 = 1,
		Q3_L3 = 2,
		Q3_L4 = 3,
		Q3_L5 = 4,
		Q3_L6 = 5,
		Q3_L7 = 6,
		Q3_L8 = 7,
		Q3_L9 = 8,
		Q3_LA = 9,
		Q3_M1  = 10,
		Q3_M2  = 11,
		Q3_COUNT = 12,

		Q4_L1 = 0,
		Q4_L2 = 1,
		Q4_L3 = 2,
		Q4_L4 = 3,
		Q4_L5 = 4,
		Q4_L6 = 5,
		Q4_COUNT = 6,

		Q6_L1 = 0,
		Q6_L2 = 1,
		Q6_L3 = 2,
		Q6_L4 = 3,
		Q6_COUNT = 4,

		QA_L1 = 0,
		QA_COUNT = 1
	};

	long query3[Q3_COUNT];
	long query4[Q4_COUNT];
	long query6[Q6_COUNT];
	long queryA[QA_COUNT];

	void CountRLSession();
	void SplitUnfinished();
	void SplitQuick368Cut();

	virtual void SesCritExtra();

	static const offield rangerep_fields[];
};

TPun1stRangeReport::TPun1stRangeReport(TWindow *parent, const TRangeReportGroup &reportGroup)
	: TRangeReport(parent, reportGroup), unfinished(Today)
{
	Add(
		new TLong("F_Q3_L1", query3 + Q3_L1),
		new TLong("F_Q3_L2", query3 + Q3_L2),
		new TLong("F_Q3_L3", query3 + Q3_L3),
		new TLong("F_Q3_L4", query3 + Q3_L4),
		new TLong("F_Q3_L5", query3 + Q3_L5),
		new TLong("F_Q3_L6", query3 + Q3_L6),
		new TLong("F_Q3_L7", query3 + Q3_L7),
		new TLong("F_Q3_L8", query3 + Q3_L8),
		new TLong("F_Q3_L9", query3 + Q3_L9),
		new TLong("F_Q3_LA", query3 + Q3_LA),
		new TLong("F_Q3_M1", query3 + Q3_M1),
		new TLong("F_Q3_M2", query3 + Q3_M2),
		new TLong("F_Q4_L1", query4 + Q4_L1),
		new TLong("F_Q4_L2", query4 + Q4_L2),
		new TLong("F_Q4_L3", query4 + Q4_L3),
		new TLong("F_Q4_L4", query4 + Q4_L4),
		new TLong("F_Q4_L5", query4 + Q4_L5),
		new TLong("F_Q4_L6", query4 + Q4_L6),
		new TLong("F_Q6_L1", query6 + Q6_L1),
		new TLong("F_Q6_L2", query6 + Q6_L2),
		new TLong("F_Q6_L3", query6 + Q6_L3),
		new TLong("F_Q6_L4", query6 + Q6_L4),
		new TLong("F_QA_L1", queryA + QA_L1),
		NULL
	);
}

int TPun1stRangeReport::CountCanceleds()
{
	// PART 1: find the relevant container
	TIndexContainer *container = &session;
	mstr decisionKinds;
	TGArray<TDecision> decisions;
	int canceleds = 0;

	session.DecisionKinds(decisionKinds, &lawsuit);

	if (any(decisionKinds))
	{
		TDecision *decision = find_finalizer(session, decisions);

		if (decision)
			container = decision;
		else
		{
			dprintf("Липсва съдебен акт за %s от %D по %s\n", Kind->Seek(session.kind), &session.date, sLawKey);
			return canceleds;
		}
	}

	// PART 2: count the container sides
	msql m;
	TSideWind sideWind;

	load_sidebasicv_criteria(m, container);

	TQuery q(str(m));
	while (q.Read())
	{
		sideWind << q;

		if (session.Defendant(sideWind.involvement))
		{
			m.Begin(judgement.Name, "F_NO");
			m.Add(&lawsuit, "F_NO, F_YEAR, F_TYPE");
			m.Add(&sideWind, "F_UCN, F_UCN_TYPE, F_INVOLVEMENT");
			m.AddChar("NOT F_JUDGED", JUDGED_NOT_KNOWN);

			TQuery p(str(m));
			if (!p.Read())
				canceleds++;
		}
	}

	return canceleds;
}

void TPun1stRangeReport::JudgementCriteria()
{
	lawCrit.Begin("T_JUDGEMENT J JOIN T_LAWSUIT L ON L.F_NO = J.F_NO AND L.F_YEAR = J.F_YEAR AND L.F_TYPE = J.F_TYPE "
		"JOIN T_SUBJECT S ON S.F_SUBJECT = L.F_SUBJECT AND S.F_TYPE = L.F_TYPE",
		"J.F_NO, J.F_YEAR, J.F_TYPE, J.F_JUDGED, J.F_PUNISHMENT, L.F_SUBJECT, L.F_KIND, J.F_FLAGS, J.F_FLAGS_X, "
			"J.F_FORCE_DATE, J.F_FLAGS_Y, J.F_RECIDIVE");
	lawCrit.AddRange("J.F_DATE", start, final);
	CritBasic(lawCrit, "J.F_TYPE");
	lawCrit.AddChar("NOT J.F_JUDGED", JUDGED_NOT_KNOWN);
}

void TPun1stRangeReport::CountJudgements()
{
	JudgementCriteria();
	LawCritExtra();
	wait->SetText("Осъдени през отчетния период...");

	TQuery q(str(lawCrit));
	int tCount = 0;

	while (q.Read())
	{
		judgement << q;
		lawsuit << q;

		if (!strchr(SubjectAttrib(), ATTRIB_DO_NOT_COUNT_DEFENDANTS))
		{
			wait->TickCount(++tCount, "осъдени лица");
			SplitJudgements();
		}
	}
}

void TPun1stRangeReport::SplitReinvests()
{
	if (strchr(RESULT_REINVESTS, session.result))
	{
		query3[Q3_L5]++;

		if (strchr(KIND_OPEN_SESSIONS, session.kind))
			query3[Q3_L6]++;

		if (!lawsuit.forceDate.Empty() && lawsuit.forceDate <= final)
			query3[Q3_L7]++;
	}
}

void TPun1stRangeReport::CountSlowMotives()
{
	TGArray<TDecision> decisions;

	if (try2decide(&session, &decisions, NULL, "F_FLAGS, F_FROM_DATE, F_RETURNED"))
	{
		for (int i = 0; i < decisions.Count(); i++)
		{
			TDecision *decision = decisions[i];

			if (decision->flags & DECISION_MOTIVES)
			{
				decision->fromDate += 30;

				if (decision->returned.Empty())	// option
					decision->returned = Today;

				if (decision->returned > decision->fromDate)
				{
					query4[Q4_L6]++;
					break;
				}
			}
		}
	}
	else
		dprintf("Липсва съдебен акт за %s от %D по %s\n", Kind->Seek(session.kind), &session.date, sLawKey);
}

int TPun1stRangeReport::CountRenewInregs()
{
	lawCrit.Begin("T_INREG I JOIN T_LAWSUIT L ON L.F_NO = I.F_LAWSUIT_NO AND L.F_YEAR = I.F_LAWSUIT_YEAR "
		"AND L.F_TYPE = I.F_LAWSUIT_TYPE",
		"COUNT(I.F_NO) AS F_COUNT");

	lawCrit.AddRange("I.F_DATE", start, final);
	lawCrit.AddChar("I.F_KIND", KIND_RENEW_REQUEST);
	CritBasic(lawCrit, "L.F_TYPE");
	LawCritExtra();
	wait->SetText("Искания за възобновяване...");
	return lawsuit.DataBase->GetLong(str(lawCrit), "F_COUNT");
}

int TPun1stRangeReport::CountStoppedLaws()
{
	lawCrit.Begin("T_SESSION S JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE",
		"DISTINCT S.F_NO, S.F_YEAR, S.F_TYPE");
	int stoppedLaws = 0;

	lawCrit.AddRange("S.F_DATE", start, final);
	lawCrit.AddChar("S.F_RESULT", RESULT_STOPPED);
	CritBasic(lawCrit, "S.F_TYPE");
	LawCritExtra();
	wait->SetText("Спрени дела...");
	TQuery q(str(lawCrit));
	while (q.Read())
		wait->TickCount(++stoppedLaws, "спрени дела");
	return stoppedLaws;
}

bool TPun1stRangeReport::CumulationFinish()
{
	if (strchr(SubjectAttrib(), POST_CUMULATION))
	{
		TGArray<TDecision> decisions;
		const TDecision *decision = find_finalizer(session, decisions, RESULT_CUMULATABLES);
		return decision && strchr(RESULT_CUMULATABLES, decision->result);
	}

	return false;
}

void TPun1stRangeReport::CountRLSession()
{
	query3[Q3_L1]++;

	if (lawsuit.kind == KIND_COMMON_PUNISHMENT_LAW)
		query3[Q3_L2]++;

	if (lawsuit.SessionLeft())
	{
		query3[Q3_L3]++;

		if (lawsuit.kind == KIND_COMMON_PUNISHMENT_LAW)
			query3[Q3_L4]++;
	}
}

void TPun1stRangeReport::SplitUnfinished()
{
	query4[Q4_L1]++;

	// 2017:207 IRQ: find previous
	//int months = interval2Months(lawsuit.started, final);
	unfinished << lawsuit;
	unfinished.previous.Flush();
	unfinished.FindPrevious();
	int months = interval2Months(unfinished.started, final);

	if (months > 12)
		query4[Q4_L5]++;
	else if (months > 6)
		query4[Q4_L4]++;
	else if (months > 3)
		query4[Q4_L3]++;
	else
		query4[Q4_L2]++;
}

void TPun1stRangeReport::SplitQuick368Cut()
{
	if (lawsuit.quick & QUICK_QUICK)
	{
		if (lawsuit.quick & QUICK_IMMEDIATE)
			query6[Q6_L2]++;
		else
			query6[Q6_L1]++;
	}

	if (lawsuit.sourceKind == KIND_REQUEST_368)
		query6[Q6_L3]++;

	if (lawsuit.cutDown)
		query6[Q6_L4]++;
}

void TPun1stRangeReport::SesCritExtra()
{
	lawCrit.AddChars("S.F_KIND", KIND_OPEN_SESSIONS);
	lawCrit.AddChars("L.F_KIND", KIND_RANGEREP_Q3_L1S);
}

const offield TPun1stRangeReport::rangerep_fields[] =
{
	{ "F_Q3_L1",	NULL },
	{ "F_Q3_L2",	NULL },
	{ "F_Q3_L3",	NULL },
	{ "F_Q3_L4",	NULL },
	{ "F_Q3_L5",	NULL },
	{ "F_Q3_L6",	NULL },
	{ "F_Q3_L7",	NULL },
	{ "F_Q3_L8",	NULL },
	{ "F_Q3_L9",	NULL },
	{ "F_Q3_LA",	NULL },
	{ "F_Q3_M1",	NULL },
	{ "F_Q3_M2",	NULL },
	{ "F_Q4_L1",	NULL },
	{ "F_Q4_L2",	NULL },
	{ "F_Q4_L3",	NULL },
	{ "F_Q4_L4",	NULL },
	{ "F_Q4_L5",	NULL },
	{ "F_Q4_L6",	NULL },
	{ "F_Q6_L1",	NULL },
	{ "F_Q6_L2",	NULL },
	{ "F_Q6_L3",	NULL },
	{ "F_Q6_L4",	NULL },
	{ "F_QA_L1",	NULL },
	{ NULL,		NULL }
};
#endif  // AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
// ----- COLUMN_ -------------------------------------------------------------
enum
{
	COLUMN_FIN_DECIDED	= 8,
	COLUMN_FIN_CANCELED	= 9,
	COLUMN_FIN_BULWARK	= 10,
	COLUMN_FINISHED_FAST	= 11,
	COLUMN_UNFIN_FINAL	= 12,
	COLUMN_APPEALED		= 0xD,
};

enum
{
	COLUMN_JUDGED_ALL		= 16,
	COLUMN_JUDGED_ACQUIT	= 17,
	COLUMN_JUDGED_PUNISHED	= 18,
	COLUMN_JUDGED_CHILD	= 19,
	COLUMN_JUDGED_TO_3Y	= 20,
	COLUMN_JUDGED_TO_3YCOND	= 21,
	COLUMN_JUDGED_3_TO_10Y	= 22,
	COLUMN_JUDGED_10_TO_30Y	= 23,
	COLUMN_JUDGED_FOR_LIFE	= 24,
	COLUMN_JUDGED_LIFE_HARD	= 25,
	COLUMN_JUDGED_OTHERS	= 26,
	COLUMN_JUDGED_SETTLED	= 27
};

class TPun1stJudRangeReport : public TPun1stRangeReport
{
public:
	TPun1stJudRangeReport(TWindow *parent, const TRangeReportGroup &reportGroup, const char *tName);

	void Execute();

protected:
	const char *name;

	void SplitPunishments();
	virtual void SplitJudgements();
	virtual void SplitFinished();
	void Show();
};

TPun1stJudRangeReport::TPun1stJudRangeReport(TWindow *parent, const TRangeReportGroup &reportGroup, const char *tName)
	: TPun1stRangeReport(parent, reportGroup), name(tName)
{
}

void TPun1stJudRangeReport::Execute()
{
	CountStart();
	CountPeriod();
	CountFinished();
	CountFinal();
	CountAppealed();
	CountJudgements();
	CountSessions();
	queryA[QA_L1] = CountRenewInregs();
	query3[Q3_L8] = CountStoppedLaws();
	ShowStopped();
	Show();
	Check();
	Debug();
}

void TPun1stJudRangeReport::Show()
{
	if (office.Open(name))
	{
		ShowUpper();
		office.Emit(tr1final, NULL, rangerep_fields, this);
		office.Show(wait);
	}
}

void TPun1stJudRangeReport::SplitPunishments()
{
	if (strchr(PUNISHMENT_UP_TO_3_YEARS_S, judgement.punishment))
	{
		Increment(COLUMN_JUDGED_TO_3Y);

		if (strchr(PUNISHMENT_UP_TO_3_YEARS_CONDS, judgement.punishment))
			Increment(COLUMN_JUDGED_TO_3YCOND);
	}
	else if (strchr(PUNISHMENT_3_TO_10_YEARS_S, judgement.punishment))
		Increment(COLUMN_JUDGED_3_TO_10Y);
	else if (strchr(PUNISHMENT_10_TO_30_YEARS_S, judgement.punishment))
		Increment(COLUMN_JUDGED_10_TO_30Y);
	else if (judgement.punishment == PUNISHMENT_FOR_LIFE)
		Increment(COLUMN_JUDGED_FOR_LIFE);
	else if (judgement.punishment == PUNISHMENT_LIFE_HARD)
		Increment(COLUMN_JUDGED_LIFE_HARD);
	else if (strchr(PUNISHMENT_REPORT_OTHERS, judgement.punishment))
		Increment(COLUMN_JUDGED_OTHERS);

	if (strchr(PUNISHMENT_REPORT_PROBAS, judgement.punishment))
		query3[Q3_L9]++;
}

void TPun1stJudRangeReport::SplitJudgements()
{
	Increment(COLUMN_JUDGED_ALL);

	if (strchr(JUDGED_RIGHT_JUDGEDS, judgement.judged))
	{
		if (judgement.punishment == PUNISHMENT_NOT_KNOWN)
			dprintf("%s: осъдено лице без наказание", sLawKey);
		else
		{
			Increment(COLUMN_JUDGED_PUNISHED);

			if (judgement.flags & SIDE_WIND_CHILD)
				Increment(COLUMN_JUDGED_CHILD);

			SplitPunishments();

			if (judgement.judged == JUDGED_SETTLED)
				Increment(COLUMN_JUDGED_SETTLED);

			if (judgement.flagsY & JUDGEMENT_Y_COMULATION)
				query3[Q3_LA]++;
		}

	}
	else if (judgement.judged == JUDGED_ACQUIT)
		Increment(COLUMN_JUDGED_ACQUIT);
}

void TPun1stJudRangeReport::SplitFinished()
{
	if (FindFinalizer())
	{
		if (strchr(RESULT_ALL_CANCELEDS, session.result))
		{
			Increment(COLUMN_FIN_CANCELED);

			if (!strchr(SubjectAttrib(), ATTRIB_DO_NOT_COUNT_DEFENDANTS))
				AddCount(COLUMN_JUDGED_ALL, CountCanceleds());

			SplitReinvests();

			if (session.result == RESULT_CANCEL_SETTLED || session.result == RESULT_WITH_BULWARK)
				Increment(COLUMN_FIN_BULWARK);
		}
		else
		{
			Increment(COLUMN_FIN_DECIDED);
			// 2017:263
			if (CumulationFinish())
				query3[Q3_LA]++;
		}
	}

	switch (lawsuit.age)
	{
		case AGE_ONE_OR_LESS :
		case AGE_THREE_OR_LESS :
		case AGE_ONE_TO_THREE : Increment(COLUMN_FINISHED_FAST); break;
		case AGE_NONE_YET : dprintf("%s е решено, но няма продължителност\n", sLawKey);
	}

	CheckStopped();
	CountSlowMotives();
}
#endif  // AREAL || MILITARY

#if COURT_TYPE == COURT_AREAL || SPECIAL
#include "aprangerep.cpp"
#endif  // AREA

#if COURT_TYPE == COURT_MILITARY && !SPECIAL
#include "mprangerep.cpp"
#endif  // MILITARY

#if COURT_TYPE == COURT_REGIONAL
#include "rprangerep.cpp"
#endif  // REGIONAL

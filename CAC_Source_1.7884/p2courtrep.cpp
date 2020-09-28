#include "cac.h"

#if COURT_TYPE == COURT_APPEAL
static const KIND_2ND_ADMIN_PUNISHMENT_LAW = '?';
#endif  // APPEAL

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
// ----- COLUMN_ -------------------------------------------------------------
enum
{
	COLUMN_UNFIN_START	= 0,
	COLUMN_ALL_NEW_SS		= 1,
	COLUMN_NEW_APPEALS	= 2,
	COLUMN_NEW_PROTESTS	= 3,
	COLUMN_ALL_PROGRESS	= 4,
	COLUMN_ALL_FINISHED	= 5,
	COLUMN_SPLIT_FINISHED	= 6,
	COLUMN_UNFIN_FINAL	= 17,
	COLUMN_SPLIT_COUNT	= COLUMN_UNFIN_FINAL - COLUMN_SPLIT_FINISHED,

	COLUMN_P_UNFIN_START	= 18,
	COLUMN_P_ALL_NEW_SS	= 19,
	COLUMN_P_ALL_FINISHED	= 20,
	COLUMN_P_UNFIN_FINAL	= 21
};

// ----- TPunCourtReport -----------------------------------------------------
class TPunCourtReport : public TCourtReport
{
public:
	TPunCourtReport(TWindow *parent, TReportGroup &group);

	void Execute();

protected:
	void SplitFinished(bool adminPun2ndLaw);

	enum
	{
		Q1_L1 = 0,
		Q1_L2 = 1,
		Q1_COUNT,

		Q2_L1 = 0,
		Q2_L2 = 1,
		Q2_L3 = 2,
		Q2_L4 = 3,
		Q2_L5 = 4,
		Q2_COUNT
	};

	long query1[Q1_COUNT];
	long query2[Q2_COUNT];

	virtual void SesCritExtra();
	void CountRLSession();

	virtual void CountLawsuits(int what);
	void Show();
};

TPunCourtReport::TPunCourtReport(TWindow *parent, TReportGroup &group)
	: TCourtReport(parent, group)
{
	Add(
		new TLong("F_Q1_L1", query1 + Q1_L1),
		new TLong("F_Q1_L2", query1 + Q1_L2),
		new TLong("F_Q2_L1", query2 + Q2_L1),
		new TLong("F_Q2_L2", query2 + Q2_L2),
		new TLong("F_Q2_L3", query2 + Q2_L3),
		new TLong("F_Q2_L4", query2 + Q2_L4),
		new TLong("F_Q2_L5", query2 + Q2_L5),
		NULL
	);
}

void TPunCourtReport::Execute()
{
	CountStart();
	CountPeriod();
	CountFinished();
	CountFinal();
	CountSessions();
	Show();
	Check();
	Debug();
}

// ----- RESULT_ -------------------------------------------------------------
static constant RESULT_SPLIT_FINISHEDS[COLUMN_SPLIT_COUNT] =
{
	//k7     //k8   //k9   //k10  //k11  //k12     //k13  //k14    //k15    //k16    //k17
	"ÃCv",   "Ä",   "È",   "É",   "Ë",   "ÏwEð",   "Ó",   "Ô¾Î",   "Ö¿¯",   "×Gz",   "ØÊ"
};

enum { COLUMN_Z_SPLIT_CANCELEDS = 10 };

void TPunCourtReport::SplitFinished(bool adminPun2ndLaw)
{
	int column = FindFinishedColumn(RESULT_SPLIT_FINISHEDS, COLUMN_SPLIT_COUNT);

	if (column != INT_MAX)
	{
		Increment(COLUMN_SPLIT_FINISHED + column);

		if (adminPun2ndLaw && column != COLUMN_Z_SPLIT_CANCELEDS)
		{
			query2[Q2_L4]++;

			if (decision->result == RESULT_CONFIRM_DECISION)
				query2[Q2_L5]++;
		}
	}
}

void TPunCourtReport::SesCritExtra()
{
	lawCrit.AddChars("S.F_KIND", KIND_OPEN_SESSIONS);
}

void TPunCourtReport::CountRLSession()
{
	query1[Q1_L1]++;

	if (lawsuit.SessionLeft())
		query1[Q1_L2]++;
}

void TPunCourtReport::CountLawsuits(int what)
{
	if (what == COUNT_RL_SESSION)
	{
		CountRLSession();
	}
	// 2017:096 TQ/FIX: +OBJECTIONS
	else if (strchr(KIND_PRIVATE_COMPLAINTS, lawsuit.sourceKind) || strchr(KIND_PRIVATE_OBJECTIONS, lawsuit.sourceKind))
	{
		switch (what)
		{
			case COUNT_UNFIN_START : Increment(COLUMN_P_UNFIN_START); break;
			case COUNT_NEW_PERIOD : Increment(COLUMN_P_ALL_NEW_SS); break;
			case COUNT_FINISHED : Increment(COLUMN_P_ALL_FINISHED); break;
			case COUNT_UNFIN_FINAL : Increment(COLUMN_P_UNFIN_FINAL); break;
			default : fatal("%d: unsupported p2courtrep-P counter", what);
		}
	}
	else
	{
		bool adminPun2ndLaw = (lawsuit.kind == KIND_2ND_ADMIN_PUNISHMENT_LAW);

		switch (what)
		{
			case COUNT_UNFIN_START :
			{
				Increment(COLUMN_UNFIN_START);
				Increment(COLUMN_ALL_PROGRESS);
				if (adminPun2ndLaw)
					query2[Q2_L1]++;
				break;
			}
			case COUNT_NEW_PERIOD :
			{
				Increment(COLUMN_ALL_NEW_SS);
				Increment(COLUMN_ALL_PROGRESS);
				if (strchr(KIND_EXACT_OBJECTIONS, lawsuit.sourceKind))
					Increment(COLUMN_NEW_PROTESTS);
				else
					Increment(COLUMN_NEW_APPEALS);

				if (adminPun2ndLaw)
				{
					query2[Q2_L1]++;
					query2[Q2_L2]++;
				}
				break;
			}
			case COUNT_FINISHED :
			{
				Increment(COLUMN_ALL_FINISHED);
				if (adminPun2ndLaw)
					query2[Q2_L3]++;

				SplitFinished(adminPun2ndLaw);
				break;
			}
			case COUNT_UNFIN_FINAL :
			{
				Increment(COLUMN_UNFIN_FINAL);
				break;
			}
			default : fatal("%d: unsupported p2courtrep counter", what);
		}
	}
}

static offield p2courtrep_fields[] =
{
	{ "F_Q1_L1",	NULL },
	{ "F_Q1_L2",	NULL },
	{ "F_Q2_L1",	NULL },
	{ "F_Q2_L2",	NULL },
	{ "F_Q2_L3",	NULL },
	{ "F_Q2_L4",	NULL },
	{ "F_Q2_L5",	NULL },
	{ NULL,		NULL }
};

void TPunCourtReport::Show()
{
	if (office.Open("SCCRep_AP2.htm"))
	{
		ShowUpper();
		office.Emit(tr1final, NULL, p2courtrep_fields, this);
		office.Show(wait);
	}
}
#endif  // APPEAL || AREAL || MILITARY

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
// ----- entry point ---------------------------------------------------------
void p2courtrep(TWindow *parent, TReportGroup &group)
{
	TPunCourtReport(parent, group).Execute();
}
#endif  // AREAL || MILITARY

#if COURT_TYPE == COURT_APPEAL
// ----- COLUMN_ -------------------------------------------------------------
enum
{
	COLUMN_RENEW_UNFIN_START	= 22,
	COLUMN_RENEW_ALL_NEW_SS		= 23,
	COLUMN_RENEW_ALL_FINISHED	= 24,
	COLUMN_RENEW_UNFIN_FINAL	= 25
};

class TAppPunCourtReport : public TPunCourtReport
{
public:
	TAppPunCourtReport(TWindow *parent, TReportGroup &group);

	void Execute();

protected:
	void SplitRenewFinished();
	void SplitPrivFinished();

	enum
	{
		Q3_L1 = 0,
		Q3_L2 = 1,
		Q3_L3 = 2,
		Q3_L4 = 3,
		Q3_COUNT,

		Q4_L1 = 0,
		Q4_L2 = 1,
		Q4_L3 = 2,
		Q4_L4 = 3,
		Q4_COUNT,

		Q5_L1 = 0,
		Q5_L2 = 1,
		Q5_L3 = 2,
		Q5_L4 = 3,
		Q5_L5 = 4,
		Q5_COUNT,
	};

	long query3[Q3_COUNT];
	long query4[Q4_COUNT];
	long query5[Q5_COUNT];

	TGSArray<TReportRange> queryRanges;

	virtual void CountLawsuits(int what);
	void Show();
};

TAppPunCourtReport::TAppPunCourtReport(TWindow *parent, TReportGroup &group)
	: TPunCourtReport(parent, group)
{
	Add(
		new TLong("F_Q3_L1", query3 + Q3_L1),
		new TLong("F_Q3_L2", query3 + Q3_L2),
		new TLong("F_Q3_L3", query3 + Q3_L3),
		new TLong("F_Q3_L4", query3 + Q3_L4),
		new TLong("F_Q4_L1", query4 + Q4_L1),
		new TLong("F_Q4_L2", query4 + Q4_L2),
		new TLong("F_Q4_L3", query4 + Q4_L3),
		new TLong("F_Q4_L4", query4 + Q4_L4),
		new TLong("F_Q5_L1", query5 + Q5_L1),
		new TLong("F_Q5_L2", query5 + Q5_L2),
		new TLong("F_Q5_L3", query5 + Q5_L3),
		new TLong("F_Q5_L4", query5 + Q5_L4),
		new TLong("F_Q5_L5", query5 + Q5_L5),
		NULL
	);

	TRangeReportGroup tReportGroup;

	tReportGroup.instances = INSTANCE_1ST;
	strcpy(tReportGroup.types, C2S[TYPE_PUNISHMENT_LAW]);
	tReportGroup.attrib = POST_REPORT_PQ34;
	TReportRange::Load(queryRanges, tReportGroup);
}

void TAppPunCourtReport::Execute()
{
	CountStart();
	CountPeriod();
	CountFinished();
	CountFinal();
	CountSessions();
	Show();
	Check();
	Debug();
}

// ----- RESULT_ -------------------------------------------------------------
enum { PRIV_SPLIT_COUNT = 4 };

static constant RESULT_PRIV_SPLIT_FINISHEDS[PRIV_SPLIT_COUNT] =
{
	//l1   //l2    //l3   //k4
	"v",   "zÎ",   "w",   "ØÊ"
};

static constant RESULT_Q5_L5_CONFIRMEDS = "vÃCq";

void TAppPunCourtReport::SplitRenewFinished()
{
	decisions.Flush();

	if (FindFinalizer())
	{
		decision = find_finalizer(session, decisions);

		if (!decision)
			dprintf("Ëèïñâà ñúäåáåí àêò çà %s îò %D ïî %s\n", Kind->Seek(session.kind), &session.date, sLawKey);
		else if (!strchr(RESULT_CANCELAWS, decision->result))
		{
			query5[Q5_L4]++;

			if (strchr(RESULT_Q5_L5_CONFIRMEDS, decision->result))
				query5[Q5_L5]++;
		}
	}
}

void TAppPunCourtReport::SplitPrivFinished()
{
	int i;
	const TReportRange *queryRange;

	for (i = 0; i < queryRanges.Count(); i++)
	{
		queryRange = queryRanges[i];

		if (queryRange->InsideRange(lawsuit.kind, lawsuit.subject))
			break;
	}

	if (i < queryRanges.Count())
	{
		if (!isdigit(*queryRange->alias))
		{
			int q3line = FindFinishedColumn(RESULT_PRIV_SPLIT_FINISHEDS, PRIV_SPLIT_COUNT);

			if (q3line != INT_MAX)
				query3[q3line]++;
		}
		else
			query4[atoi(queryRange->alias)]++;
	}
}

static offield appcourtrep_fields[] =
{
	{ "F_Q1_L1",	NULL },
	{ "F_Q1_L2",	NULL },
	{ "F_Q2_L1",	NULL },
	{ "F_Q2_L2",	NULL },
	{ "F_Q2_L3",	NULL },
	{ "F_Q2_L4",	NULL },
	{ "F_Q2_L5",	NULL },
	{ "F_Q3_L1",	NULL },
	{ "F_Q3_L2",	NULL },
	{ "F_Q3_L3",	NULL },
	{ "F_Q3_L4",	NULL },
	{ "F_Q4_L1",	NULL },
	{ "F_Q4_L2",	NULL },
	{ "F_Q4_L3",	NULL },
	{ "F_Q4_L4",	NULL },
	{ "F_Q5_L1",	NULL },
	{ "F_Q5_L2",	NULL },
	{ "F_Q5_L3",	NULL },
	{ "F_Q5_L4",	NULL },
	{ "F_Q5_L5",	NULL },
	{ NULL,		NULL }
};

void TAppPunCourtReport::CountLawsuits(int what)
{
	if (what == COUNT_RL_SESSION || !strchr(KIND_RENEW_INDOCS, lawsuit.sourceKind))
	{
		TPunCourtReport::CountLawsuits(what);

		if (what == COUNT_FINISHED && lawsuit.kind == KIND_2ND_PRIVATE_PUNISHMENT_LAW)
			SplitPrivFinished();
	}
	else
	{
		switch (what)
		{
			case COUNT_UNFIN_START :
			{
				Increment(COLUMN_RENEW_UNFIN_START);
				query5[Q5_L1]++;
				break;
			}
			case COUNT_NEW_PERIOD :
			{
				Increment(COLUMN_RENEW_ALL_NEW_SS);
				query5[Q5_L1]++;
				query5[Q5_L2]++;
				break;
			}
			case COUNT_FINISHED :
			{
				Increment(COLUMN_RENEW_ALL_FINISHED);
				query5[Q5_L3]++;
				SplitRenewFinished();
				break;
			}
			case COUNT_UNFIN_FINAL : Increment(COLUMN_RENEW_UNFIN_FINAL); break;
			case COUNT_RL_SESSION : CountRLSession(); break;
			default : fatal("%d: unsupported apcourtrep counter", what);
		}
	}
}

void TAppPunCourtReport::Show()
{
	if (office.Open("SCCRep_PP2.htm"))
	{
		ShowUpper();
		office.Emit(tr1final, NULL, appcourtrep_fields, this);
		office.Show(wait);
	}
}

void p2courtrep(TWindow *parent, TReportGroup &group)
{
	TAppPunCourtReport(parent, group).Execute();
}
#endif  // APPEAL

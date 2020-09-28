#include "cac.h"

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
// ----- COLUMN_ -------------------------------------------------------------
enum
{
	COLUMN_UNFIN_START	= 0,
	COLUMN_NEW_UNSPEC		= 1,	// mostly plain
	COLUMN_NEW_JURISD		= 2,
	COLUMN_NEW_RETURN		= 3,
	COLUMN_SS_RESTART		= 4,
	COLUMN_ALL_NEW_SS		= 5,
	COLUMN_ALL_PROGRESS	= 6,
	COLUMN_ALL_FINISHED	= 7,
	COLUMN_GRANT_ENTIRE	= 8,
	COLUMN_GRANT_PARTIAL	= 9,
	COLUMN_GRANT_REJECTED	= 10,
	COLUMN_CANCEL_SETTLED	= 11,
	COLUMN_CANCEL_OTHERS	= 12,
	COLUMN_FINISHED_LE_3	= 0x0D,
	COLUMN_UNFIN_FINAL	= 14,
	COLUMN_APPEALED		= 15,
	COLUMN_FINISHED_3_TO_6	= 16	// 2018-02-09
};

// ----- TCit1stRangeReport --------------------------------------------------
class TCit1stRangeReport : public TRangeReport
{
public:
	TCit1stRangeReport(TWindow *parent, const TRangeReportGroup &reportGroup);

	void Execute();

protected:
	void SplitFinished();
	void SplitUnfinished();

	enum
	{
		Q1_L1 = 0,
		Q1_L2 = 1,
		Q1_L3 = 2,
		Q1_COUNT,

		Q2_L1 = 0,
		Q2_L2 = 1,
		Q2_L3 = 2,
		Q2_L4 = 3,
		Q2_COUNT,

		Q3_TOTAL = 0,
		Q3_S1_C1 = 1,
		Q3_S1_C2 = 2,
		Q3_S1_C3 = 3,
		Q3_S1_C4 = 4,
		Q3_S2_C1 = 5,
		Q3_S2_C2 = 6,
		Q3_S2_C3 = 7,
		Q3_S2_C4 = 8,
		// OC TRADE
		Q3_S3_C1 = 9,
		Q3_S3_C2 = 10,
		Q3_S3_C3 = 11,
		Q3_S3_C4 = 12,
		Q3_COUNT
	};

	long query1[Q1_COUNT];
	long query2[Q2_COUNT];
	long query3[Q3_COUNT];

	void CountExchanges();
	virtual void SesCritExtra();
	void CountRLSession();

	virtual void CountLawsuits(int what);
	void Show();
};

TCit1stRangeReport::TCit1stRangeReport(TWindow *parent, const TRangeReportGroup &reportGroup)
	: TRangeReport(parent, reportGroup)
{
	Add(
		new TLong("F_Q1_L1", query1 + Q1_L1),
		new TLong("F_Q1_L2", query1 + Q1_L2),
		new TLong("F_Q1_L3", query1 + Q1_L3),
		new TLong("F_Q2_L1", query2 + Q2_L1),
		new TLong("F_Q2_L2", query2 + Q2_L2),
		new TLong("F_Q2_L3", query2 + Q2_L3),
		new TLong("F_Q2_L4", query2 + Q2_L4),
		new TLong("F_Q3_TOTAL", query3 + Q3_TOTAL),
		new TLong("F_Q3_S1_C1", query3 + Q3_S1_C1),
		new TLong("F_Q3_S1_C2", query3 + Q3_S1_C2),
		new TLong("F_Q3_S1_C3", query3 + Q3_S1_C3),
		new TLong("F_Q3_S1_C4", query3 + Q3_S1_C4),
		new TLong("F_Q3_S2_C1", query3 + Q3_S2_C1),
		new TLong("F_Q3_S2_C2", query3 + Q3_S2_C2),
		new TLong("F_Q3_S2_C3", query3 + Q3_S2_C3),
		new TLong("F_Q3_S2_C4", query3 + Q3_S2_C4),
		new TLong("F_Q3_S3_C1", query3 + Q3_S3_C1),
		new TLong("F_Q3_S3_C2", query3 + Q3_S3_C2),
		new TLong("F_Q3_S3_C3", query3 + Q3_S3_C3),
		new TLong("F_Q3_S3_C4", query3 + Q3_S3_C4),
		NULL
	);
}

void TCit1stRangeReport::Execute()
{
	CountStart();
	CountPeriod();
	CountFinished();
	CountFinal();
	CountAppealed();
	CountSessions();
	CountExchanges();
	ShowStopped();
	Show();
	Check();
	Debug();
}

void TCit1stRangeReport::SplitFinished()
{
	if (FindFinalizer())
	{
		if (strchr(RESULT_ALL_CANCELEDS, session.result))
		{
			if (session.result == RESULT_CANCEL_SETTLED)
				Increment(COLUMN_CANCEL_SETTLED);
			else
				Increment(COLUMN_CANCEL_OTHERS);
		}
		else
		{
			switch (lawsuit.grant)
			{
				case GRANT_ENTIRE : Increment(COLUMN_GRANT_ENTIRE); break;
				case GRANT_PARTIAL : Increment(COLUMN_GRANT_PARTIAL); break;
				case GRANT_NULLIFY :	// IRQ: == rejected
				case GRANT_REJECTED : Increment(COLUMN_GRANT_REJECTED); break;
				default : dprintf("%s е решено, но няма степен на уважаване на иска\n", sLawKey);
			}
		}

		if (PublicActAge() == AGE_MORE_THAN_THREE)
			query2[Q2_L4]++;
	}

	switch (lawsuit.age)
	{
		case AGE_ONE_OR_LESS :
		case AGE_THREE_OR_LESS :
		case AGE_ONE_TO_THREE : Increment(COLUMN_FINISHED_LE_3); break;
		case AGE_THREE_TO_SIX : Increment(COLUMN_FINISHED_3_TO_6); break;
		case AGE_NONE_YET : dprintf("%s е решено, но няма продължителност\n", sLawKey);
	}

	CheckStopped();
}

void TCit1stRangeReport::SplitUnfinished()
{
	int months = interval2Months(lawsuit.kind, lawsuit.date, lawsuit.firstSet, final);

	if (months > 60)
		query2[Q2_L3]++;
	else if (months > 36)
		query2[Q2_L2]++;
	else if (months > 12)
		query2[Q2_L1]++;
}

void TCit1stRangeReport::CountExchanges()
{
	lawCrit.Begin("T_LAW_INTERVAL I JOIN T_LAWSUIT L ON L.F_NO = I.F_NO AND L.F_YEAR = I.F_YEAR "
			"AND L.F_TYPE = I.F_TYPE",
			"I.F_NO, I.F_YEAR, I.F_TYPE, I.F_DATE, I.F_FIRST_SET, L.F_QUICK");

	CritBasic(lawCrit, "I.F_TYPE");
	lawCrit.AddRange("I.F_FIRST_SET", start, final);
	lawCrit.AddChars("L.F_KIND", KIND_EXCHANGE_LAWS);
	// 2018-02-14: LRQ: indocs, from firstSet
	lawCrit.AddChars("L.F_SOURCE_KIND", KIND_EXCHANGE_INDOCS, KIND_INDOCS);
	LawCritExtra();

	TQuery q(str(lawCrit));
	while (q.Read())
	{
		lawsuit << q;
		query3[Q3_TOTAL]++;

		int delta = interval2Months(lawsuit.date, lawsuit.firstSet);
		bool quick = lawsuit.quick & QUICK_QUICK;
		int basic;

		if (delta < 1)
			delta = 1;
		else if (delta > 4)
			delta = 4;

	#if COURT_TYPE == COURT_AREAL
		if (lawsuit.key.type == TYPE_TRADE_LAW)
			basic = Q3_S3_C1;
		else
	#endif  // AREAL
		if (quick)
			basic = Q3_S2_C1;
		else
			basic = Q3_S1_C1;

		query3[basic + delta - 1]++;
	}
}

void TCit1stRangeReport::SesCritExtra()
{
	lawCrit.AddChars("S.F_KIND", KIND_OPEN_SESSIONS);
}

void TCit1stRangeReport::CountRLSession()
{
	query1[Q1_L1]++;

	if (lawsuit.SessionLeft())
	{
		query1[Q1_L2]++;

		if (strchr(RESULT_RANGEREP_Q1_L3S, lawsuit.sessionResult))
			query1[Q1_L3]++;
	}
}

void TCit1stRangeReport::CountLawsuits(int what)
{
	switch (what)
	{
		case COUNT_UNFIN_START :
		{
			Increment(COLUMN_UNFIN_START);
			Increment(COLUMN_ALL_PROGRESS);
			break;
		}
		case COUNT_NEW_PERIOD :
		{
			if (lawsuit.intervalFlags & LAW_INTERVAL_SECONDARY)
				Increment(COLUMN_SS_RESTART);
			else
			{
				switch (lawsuit.MainConnectType())
				{
					case CONNECT_TYPE_JURISDICTION : Increment(COLUMN_NEW_JURISD); break;
					case CONNECT_TYPE_RETURNED_LAWSUIT : Increment(COLUMN_NEW_RETURN); break;
					default : Increment(COLUMN_NEW_UNSPEC);
				}
			}

			Increment(COLUMN_ALL_NEW_SS);
			Increment(COLUMN_ALL_PROGRESS);
			break;
		}
		case COUNT_FINISHED :
		{
			if (Increment(COLUMN_ALL_FINISHED))
				SplitFinished();
			break;
		}
		case COUNT_UNFIN_FINAL :
		{
			if (Increment(COLUMN_UNFIN_FINAL))
				SplitUnfinished();
			break;
		}
		case COUNT_RL_SESSION : CountRLSession(); break;
		case COUNT_APPEALED : Increment(COLUMN_APPEALED); break;
		default : fatal("%d: unsupported c1rangerep counter", what);
	}
}

static const offield rangerep_fields[] =
{
	{ "F_Q1_L1",	NULL },
	{ "F_Q1_L2",	NULL },
	{ "F_Q1_L3",	NULL },
	{ "F_Q2_L1",	NULL },
	{ "F_Q2_L2",	NULL },
	{ "F_Q2_L3",	NULL },
	{ "F_Q2_L4",	NULL },
	{ "F_Q3_TOTAL",	NULL },
	{ "F_Q3_S1_C1",	NULL },
	{ "F_Q3_S1_C2",	NULL },
	{ "F_Q3_S1_C3",	NULL },
	{ "F_Q3_S1_C4",	NULL },
	{ "F_Q3_S2_C1",	NULL },
	{ "F_Q3_S2_C2",	NULL },
	{ "F_Q3_S2_C3",	NULL },
	{ "F_Q3_S2_C4",	NULL },
	{ "F_Q3_S3_C1",	NULL },
	{ "F_Q3_S3_C2",	NULL },
	{ "F_Q3_S3_C3",	NULL },
	{ "F_Q3_S3_C4",	NULL },
	{ NULL,		NULL }
};

void TCit1stRangeReport::Show()
{
	char name[SIZE_OF_BASE_NAME];
	sprintf(name, "SCCRep_%cC1.htm", toupper(COURT_LETTER));

	if (office.Open(name))
	{
		ShowUpper();
		office.Emit(tr1final, NULL, rangerep_fields, this);
		office.Show(wait);
	}
}

// ----- entry point ---------------------------------------------------------
void c1rangerep(TWindow *parent, const TRangeReportGroup &reportGroup)
{
	TCit1stRangeReport(parent, reportGroup).Execute();
}
#endif  // AREAL || MILITARY || REGIONAL

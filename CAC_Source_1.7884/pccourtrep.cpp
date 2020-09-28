// ----- COLUMN_ -------------------------------------------------------------
enum
{
	COLUMN_UNFIN_START	= 0,
	COLUMN_ALL_NEW_SS		= 1,
	COLUMN_ALL_PROGRESS	= 2,
	COLUMN_ALL_FINISHED	= 3,
	COLUMN_SPLIT_FINISHED	= 4,
	COLUMN_UNFIN_FINAL	= 10,
	COLUMN_SPLIT_COUNT	= COLUMN_UNFIN_FINAL - COLUMN_SPLIT_FINISHED,

	COLUMN_P_UNFIN_START	= 11,
	COLUMN_P_ALL_NEW_SS	= 12,
	COLUMN_P_ALL_FINISHED	= 13,
	COLUMN_P_UNFIN_FINAL	= 14,

	COLUMN_SLOW_ALL_NEW_SS	= 15,
	COLUMN_SLOW_ACCEPTED	= 16,
	COLUMN_SLOW_REJECTED	= 17,
	COLUMN_SLOW_SPLIT_COUNT	= 2,

	COLUMN_274_UNFIN_START	= 18,
	COLUMN_274_ALL_NEW_SS	= 19,
	COLUMN_274_ALL_FINISHED	= 20,
	COLUMN_274_UNFIN_FINAL	= 21
};

// ----- TCitCourtReport -----------------------------------------------------
class TCitCourtReport : public TCourtReport
{
public:
	TCitCourtReport(TWindow *parent, TReportGroup &group);

	void Execute();

protected:
	void SplitFinished();
	void SplitSlowFinished();

	enum
	{
		Q1_L1 = 0,
		Q1_L2 = 1,
		Q1_L3 = 2,
		Q1_COUNT
	};

	long query1[Q1_COUNT];

	void CountRLSession();

	virtual void CountLawsuits(int what);
	void Show();
};

TCitCourtReport::TCitCourtReport(TWindow *parent, TReportGroup &group)
	: TCourtReport(parent, group)
{
	Add(
		new TLong("F_Q1_L1", query1 + Q1_L1),
		new TLong("F_Q1_L2", query1 + Q1_L2),
		new TLong("F_Q1_L3", query1 + Q1_L3),
		NULL
	);
}

void TCitCourtReport::Execute()
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
	//k5    //k6     //k7      //k8    //k9   //k10
	"vC",   "Eæw",   "GøzŒ",   "HÒ",   "?",   "ÿ "
};

static constant RESULT_SLOW_SPLIT_FINISHEDS[COLUMN_SLOW_SPLIT_COUNT] =
{
	//k17   //k18
	"12",   "u"
};

void TCitCourtReport::SplitFinished()
{
	int column = FindFinishedColumn(RESULT_SPLIT_FINISHEDS, COLUMN_SPLIT_COUNT);

	if (column != INT_MAX)
		Increment(COLUMN_SPLIT_FINISHED + column);
}

void TCitCourtReport::SplitSlowFinished()
{
	int column = FindFinishedColumn(RESULT_SLOW_SPLIT_FINISHEDS, COLUMN_SLOW_SPLIT_COUNT);

	if (column != INT_MAX)
		Increment(COLUMN_SLOW_ACCEPTED + column);
}

// copied from accourtrep 1:1
void TCitCourtReport::CountRLSession()
{
	if (strchr(KIND_OPEN_SESSIONS, lawsuit.sessionKind))
	{
		query1[Q1_L1]++;

		if (lawsuit.SessionLeft())
			query1[Q1_L2]++;
	}

	if (strchr(RESULT_UNDOS, lawsuit.sessionResult))
		query1[Q1_L3]++;
}

void TCitCourtReport::CountLawsuits(int what)
{
	if (what == COUNT_RL_SESSION)
	{
		CountRLSession();
	}
	else if (strchr(KIND_CT274_INDOCS, lawsuit.sourceKind))
	{
		switch (what)
		{
			case COUNT_UNFIN_START : Increment(COLUMN_274_UNFIN_START); break;
			case COUNT_NEW_PERIOD : Increment(COLUMN_274_ALL_NEW_SS); break;
			case COUNT_FINISHED : Increment(COLUMN_274_ALL_FINISHED); break;
			case COUNT_UNFIN_FINAL : Increment(COLUMN_274_UNFIN_FINAL); break;
			default : fatal("%d: unsupported pccourtrep-27 counter", what);
		}
	}
	else if (strchr(KIND_SLOW_REQUESTS, lawsuit.sourceKind))
	{
		switch (what)
		{
			case COUNT_UNFIN_START : break;
			case COUNT_NEW_PERIOD : Increment(COLUMN_SLOW_ALL_NEW_SS); break;
			case COUNT_FINISHED : SplitSlowFinished(); break;
			case COUNT_UNFIN_FINAL : break;
			default : fatal("%d: unsupported pccourtrep-SL counter", what);
		}
	}
	else if (strchr(KIND_PRIVATE_COMPLAINTS, lawsuit.sourceKind))
	{
		// copied from accourtrep 1:1
		switch (what)
		{
			case COUNT_UNFIN_START : Increment(COLUMN_P_UNFIN_START); break;
			case COUNT_NEW_PERIOD : Increment(COLUMN_P_ALL_NEW_SS); break;
			case COUNT_FINISHED : Increment(COLUMN_P_ALL_FINISHED); break;
			case COUNT_UNFIN_FINAL : Increment(COLUMN_P_UNFIN_FINAL); break;
			default : fatal("%d: unsupported pccourtrep-P counter", what);
		}
	}
	else
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
				Increment(COLUMN_ALL_NEW_SS);
				Increment(COLUMN_ALL_PROGRESS);
				break;
			}
			case COUNT_FINISHED :
			{
				Increment(COLUMN_ALL_FINISHED);
				SplitFinished();
				break;
			}
			case COUNT_UNFIN_FINAL :
			{
				Increment(COLUMN_UNFIN_FINAL);
				break;
			}
			default : fatal("%d: unsupported accourtrep counter", what);
		}
	}
}

static offield courtrep_fields[] =
{
	{ "F_Q1_L1",	NULL },
	{ "F_Q1_L2",	NULL },
	{ "F_Q1_L3",	NULL },
	{ NULL,		NULL }
};

void TCitCourtReport::Show()
{
	if (office.Open("SCCRep_PC2.htm"))
	{
		ShowUpper();
		office.Emit(tr1final, NULL, courtrep_fields, this);
		office.Show(wait);
	}
}

// ----- entry point ---------------------------------------------------------
void c2courtrep(TWindow *parent, TReportGroup &group)
{
	TCitCourtReport(parent, group).Execute();
}

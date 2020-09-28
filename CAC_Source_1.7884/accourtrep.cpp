// ----- COLUMN_ -------------------------------------------------------------
enum
{
	COLUMN_UNFIN_START	= 0,
	COLUMN_ALL_NEW_SS		= 1,
	COLUMN_ALL_PROGRESS	= 2,
	COLUMN_ALL_FINISHED	= 3,
	COLUMN_SPLIT_FINISHED	= 4,
	COLUMN_UNFIN_FINAL	= 9,
	COLUMN_SPLIT_COUNT	= COLUMN_UNFIN_FINAL - COLUMN_SPLIT_FINISHED,

	COLUMN_P_UNFIN_START	= 10,
	COLUMN_P_ALL_NEW_SS	= 11,
	COLUMN_P_ALL_FINISHED	= 12,
	COLUMN_P_UNFIN_FINAL	= 13
};

// ----- TCitCourtReport -----------------------------------------------------
class TCitCourtReport : public TCourtReport
{
public:
	TCitCourtReport(TWindow *parent, TReportGroup &group);

	void Execute();

protected:
	bool zspzzInact;

	void SplitFinished();

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
		Q2_COUNT
	};

	long query1[Q1_COUNT];
	long query2[Q2_COUNT];

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
		new TLong("F_Q2_L1", query2 + Q2_L1),
		new TLong("F_Q2_L2", query2 + Q2_L2),
		new TLong("F_Q2_L3", query2 + Q2_L3),
		new TLong("F_Q2_L4", query2 + Q2_L4),
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
	//k5     //k6    //k7      //k8   //9
	"Cv",   "Eѕw",   "GїzО",   "H",   "ШК"
};

static constant RESULT_COURTREP_Q2_L4S = "Gz";

enum { COLUMN_Z_DEC_CONFIRMEDS = 0 };

void TCitCourtReport::SplitFinished()
{
	int column = FindFinishedColumn(RESULT_SPLIT_FINISHEDS, COLUMN_SPLIT_COUNT);

	if (column != INT_MAX)
	{
		Increment(COLUMN_SPLIT_FINISHED + column);

		if (column == COLUMN_Z_DEC_CONFIRMEDS && zspzzInact)
			query2[Q2_L3]++;

		if (zspzzInact && strchr(RESULT_COURTREP_Q2_L4S, decision->result))
			query2[Q2_L4]++;
	}
}

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
	SeekSubject();
	zspzzInact = strchr(SubjectAttrib(), POST_ZSPZZ_INACT);

	if (strchr(KIND_SLOWDOWN_COMPLAINTS, lawsuit.sourceKind))
	{
		// 2017:046 IRQ
		dlprintf("%s е образувано по %s\n", sLawKey, Kind->Seek(lawsuit.sourceKind));
	}
	else if (what == COUNT_RL_SESSION)
	{
		CountRLSession();
	}
	else if (strchr(KIND_PRIVATE_COMPLAINTS, lawsuit.sourceKind))
	{
		switch (what)
		{
			case COUNT_UNFIN_START : Increment(COLUMN_P_UNFIN_START); break;
			case COUNT_NEW_PERIOD : Increment(COLUMN_P_ALL_NEW_SS); break;
			case COUNT_FINISHED : Increment(COLUMN_P_ALL_FINISHED); break;
			case COUNT_UNFIN_FINAL : Increment(COLUMN_P_UNFIN_FINAL); break;
			default : fatal("%d: unsupported accourtrep-P counter", what);
		}

		if (zspzzInact)
		{
			TKind tKind;
			dlprintf("%s с предмет %s е образувано по %s\n", sLawKey, lawsuit.Code(), tKind.Seek(lawsuit.sourceKind));
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
				if (zspzzInact)
					query2[Q2_L1]++;
				break;
			}
			case COUNT_NEW_PERIOD :
			{
				Increment(COLUMN_ALL_NEW_SS);
				Increment(COLUMN_ALL_PROGRESS);
				if (zspzzInact)
					query2[Q2_L2]++;
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
	{ "F_Q2_L1",	NULL },
	{ "F_Q2_L2",	NULL },
	{ "F_Q2_L3",	NULL },
	{ "F_Q2_L4",	NULL },
	{ NULL,		NULL }
};

void TCitCourtReport::Show()
{
	if (office.Open("SCCRep_AC2.htm"))
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

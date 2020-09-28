// ----- COLUMN_ -------------------------------------------------------------
enum
{
	COLUMN_UNFIN_START	= 0,
	COLUMN_ALL_NEW_SS		= 1,
	//COLUMN_COME_PERIOD	= 2,
	COLUMN_NEW_WHATEVER	= 2,  // 2018-02-05: URQ: +related
	COLUMN_SS_RESTART		= 3,
	COLUMN_ALL_PROGRESS	= 4,
	COLUMN_ALL_FINISHED	= 5,
};

// ----- TMilitaryPunRangeReport ---------------------------------------------
class TMilitaryPunRangeReport : public TPun1stJudRangeReport
{
public:
	TMilitaryPunRangeReport(TWindow *parent, const TRangeReportGroup &reportGroup);

protected:
	virtual void SplitJudgements();
	virtual void CountLawsuits(int what);
};

TMilitaryPunRangeReport::TMilitaryPunRangeReport(TWindow *parent, const TRangeReportGroup &reportGroup)
	: TPun1stJudRangeReport(parent, reportGroup, "SCCRep_MP1.htm")
{
}

void TMilitaryPunRangeReport::SplitJudgements()
{
	TPun1stJudRangeReport::SplitJudgements();

	if (judgement.recidive != RECIDIVE_NOT_KNOWN)
		query3[Q3_M1]++;

	if (judgement.punishment == PUNISHMENT_PROBATION)
		query3[Q3_M2]++;
}

void TMilitaryPunRangeReport::CountLawsuits(int what)
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
			//else
			//	Increment(COLUMN_COME_PERIOD);
			else
			{
				long mask = 0;

				switch (lawsuit.MainConnectType())
				{
					case CONNECT_TYPE_JURISDICTION       : mask = 1; break;
					case CONNECT_TYPE_RETURNED_LAWSUIT   : mask = 2; break;
					case CONNECT_TYPE_RENEWED_LAWSUIT    : mask = 4; break;
					case CONNECT_TYPE_RERETURNED_LAWSUIT : mask = 8; break;
					case '\0' : Increment(COLUMN_NEW_WHATEVER);
				}

				if (REPORT_NEW_LAWSUITS & mask)
					Increment(COLUMN_NEW_WHATEVER);
			}

			Increment(COLUMN_ALL_NEW_SS);
			Increment(COLUMN_ALL_PROGRESS);
			break;
		}
		case COUNT_FINISHED :
		{
			if (Increment(COLUMN_ALL_FINISHED))
				SplitFinished();

			SplitQuick368Cut();
			break;
		}
		case COUNT_UNFIN_FINAL :
		{
			Increment(COLUMN_UNFIN_FINAL);
			SplitUnfinished();
			break;
		}
		case COUNT_APPEALED : Increment(COLUMN_APPEALED); break;
		case COUNT_RL_SESSION : CountRLSession(); break;
		default : fatal("%d: unsupported mprangerep counter", what);
	}
}

// ----- entry point ---------------------------------------------------------
void p1rangerep(TWindow *parent, const TRangeReportGroup &reportGroup)
{
	TMilitaryPunRangeReport(parent, reportGroup).Execute();
}

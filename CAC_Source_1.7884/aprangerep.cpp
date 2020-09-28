// ----- COLUMN_ -------------------------------------------------------------
enum
{
	COLUMN_UNFIN_START	= 0,
	COLUMN_COME_PERIOD	= 1,
	COLUMN_NEW_RETURN		= 2,
	COLUMN_SS_RESTART		= 3,
	COLUMN_ALL_NEW_SS		= 4,
	COLUMN_ALL_PROGRESS	= 5,
	COLUMN_FIN_RENEWED	= 6,
	COLUMN_ALL_FINISHED	= 7,
	COLUMN_JUDGED_FORCED	= 14,
	COLUMN_ACQUIT_FORCED	= 15
};

// ----- TAreaPunRangeReport -------------------------------------------------
class TAreaPunRangeReport : public TPun1stJudRangeReport
{
public:
	TAreaPunRangeReport(TWindow *parent, const TRangeReportGroup &reportGroup);

protected:
	virtual void SplitJudgements();
	virtual void SplitFinished();
	virtual void CountLawsuits(int what);
};

TAreaPunRangeReport::TAreaPunRangeReport(TWindow *parent, const TRangeReportGroup &reportGroup)
	: TPun1stJudRangeReport(parent, reportGroup, "SCCRep_AP1.htm")
{
}

void TAreaPunRangeReport::SplitJudgements()
{
	TPun1stJudRangeReport::SplitJudgements();

	if (!judgement.forceDate.Empty())
	{
		Increment(COLUMN_JUDGED_FORCED);

		if (judgement.judged == JUDGED_ACQUIT)
			Increment(COLUMN_ACQUIT_FORCED);
	}
}

void TAreaPunRangeReport::SplitFinished()
{
	TPun1stJudRangeReport::SplitFinished();
}

void TAreaPunRangeReport::CountLawsuits(int what)
{
	switch (what)
	{
		case COUNT_UNFIN_START :
		{
			Increment(COLUMN_UNFIN_START);
			Increment(COLUMN_ALL_PROGRESS);

			if (lawsuit.MainConnectType() == CONNECT_TYPE_RENEWED_LAWSUIT)
				Increment(COLUMN_FIN_RENEWED);
			break;
		}
		case COUNT_NEW_PERIOD :
		{
			char mainConnectType = lawsuit.MainConnectType();

			if (lawsuit.intervalFlags & LAW_INTERVAL_SECONDARY)
				Increment(COLUMN_SS_RESTART);
			else
			{
				switch (mainConnectType)
				{
					case CONNECT_TYPE_RETURNED_LAWSUIT :
					case CONNECT_TYPE_RERETURNED_LAWSUIT :
					case CONNECT_TYPE_RENEWED_LAWSUIT : Increment(COLUMN_NEW_RETURN); break;
					// no special column for the rest/unspec
				}

				Increment(COLUMN_COME_PERIOD);
			}

			Increment(COLUMN_ALL_NEW_SS);
			Increment(COLUMN_ALL_PROGRESS);

			if (mainConnectType == CONNECT_TYPE_RENEWED_LAWSUIT)
				Increment(COLUMN_FIN_RENEWED);
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
		default : fatal("%d: unsupported aprangerep counter", what);
	}
}

// ----- entry point ---------------------------------------------------------
void p1rangerep(TWindow *parent, const TRangeReportGroup &reportGroup)
{
	TAreaPunRangeReport(parent, reportGroup).Execute();
}

#include "cac.h"

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
// ----- TBossRangeReport ----------------------------------------------------
enum
{
	COLUMN_UNFIN_START	= 0,
	COLUMN_ALL_NEW_SS		= 1,
	COLUMN_ALL_PROGRESS	= 2,
	COLUMN_FIXED_SESSIONS	= 3,
	COLUMN_ALL_FINISHED	= 4,
	COLUMN_LEFT_SESSIONS	= 5,
	COLUMN_UNFIN_FINAL	= 6,
	COLUMN_OTHER_OPEN		= 7,
	COLUMN_OTHER_CLOSED	= 8
};

class TBossRangeReport : public TRangeReport
{
public:
	TBossRangeReport(TWindow *parent, const TRangeReportGroup &reportGroup) : TRangeReport(parent, reportGroup) { }

	void Execute();

protected:
	void CountRLSession();

	virtual void CountLawsuits(int what);

	void Show();
};

void TBossRangeReport::Execute()
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

void TBossRangeReport::CountRLSession()
{
	if (strchr(KIND_OPEN_SESSIONS, lawsuit.sessionKind))
		Increment(COLUMN_FIXED_SESSIONS);

	if (lawsuit.SessionLeft())
		Increment(COLUMN_LEFT_SESSIONS);
	else if (lawsuit.SessionOtherAct())
		Increment(strchr(KIND_OPEN_SESSIONS, lawsuit.sessionKind) ? COLUMN_OTHER_OPEN : COLUMN_OTHER_CLOSED);
}

void TBossRangeReport::CountLawsuits(int what)
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
			break;
		}
		case COUNT_UNFIN_FINAL :
		{
			Increment(COLUMN_UNFIN_FINAL);
			break;
		}
		case COUNT_RL_SESSION :
		{
			CountRLSession();
			break;
		}
		default : fatal("%d: unsupported bsrangerep counter", what);
	}
}

void TBossRangeReport::Show()
{
	if (office.Open("BosRep1.htm"))
	{
		ShowUpper();
		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(wait);
	}
}

// ----- entry point ---------------------------------------------------------
void bsrangerep(TWindow *parent, const TRangeReportGroup &reportGroup)
{
	TBossRangeReport(parent, reportGroup).Execute();
}
#endif  // ADMIN || AREAL || MILITARY || REGIONAL

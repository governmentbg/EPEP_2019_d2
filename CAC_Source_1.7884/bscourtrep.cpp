#include "cac.h"

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
// ----- TBossCourtReport ----------------------------------------------------
enum
{
	COLUMN_UNFIN_START	= 0,
	COLUMN_ALL_NEW_SS		= 1,
	COLUMN_ALL_PROGRESS	= 2,
	COLUMN_ALL_FINISHED	= 3,
	COLUMN_UNFIN_FINAL	= 4,

	COLUMN_P_UNFIN_START	= 5,
	COLUMN_P_ALL_NEW_SS	= 6,
	COLUMN_P_ALL_PROGRESS	= 7,
	COLUMN_P_ALL_FINISHED	= 8,
	COLUMN_P_UNFIN_FINAL	= 9,
	
	COLUMN_OTHER_OPEN		= 10,
	COLUMN_OTHER_CLOSED	= 11
};

class TBossCourtReport : public TCourtReport
{
public:
	TBossCourtReport(TWindow *parent, const TReportGroup &reportGroup);

	void Execute();

protected:
	mstr KIND_PRIVATES;

	void CountRLSession();

	virtual void CountLawsuits(int what);

	void Show();
};

TBossCourtReport::TBossCourtReport(TWindow *parent, const TReportGroup &reportGroup)
	: TCourtReport(parent, reportGroup), KIND_PRIVATES(KIND_PRIVATE_COMPLAINTS)
{
	KIND_PRIVATES.cat(KIND_SLOWDOWN_COMPLAINTS);
	KIND_PRIVATES.cat(KIND_PRIVATE_OBJECTIONS);
}

void TBossCourtReport::Execute()
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

void TBossCourtReport::CountRLSession()
{
	if (lawsuit.SessionOtherAct())
		Increment(strchr(KIND_OPEN_SESSIONS, lawsuit.sessionKind) ? COLUMN_OTHER_OPEN : COLUMN_OTHER_CLOSED);
}

void TBossCourtReport::CountLawsuits(int what)
{
	if (what == COUNT_RL_SESSION)
		CountRLSession();
	else
	{
		int pb = KIND_PRIVATES.chr(lawsuit.sourceKind) ? 0 : (COLUMN_P_UNFIN_START - COLUMN_UNFIN_START);

		switch (what)
		{
			case COUNT_UNFIN_START :
			{
				Increment(COLUMN_UNFIN_START + pb);
				Increment(COLUMN_ALL_PROGRESS + pb);
				break;
			}
			case COUNT_NEW_PERIOD :
			{
				Increment(COLUMN_ALL_NEW_SS + pb);
				Increment(COLUMN_ALL_PROGRESS + pb);
				break;
			}
			case COUNT_FINISHED :
			{
				Increment(COLUMN_ALL_FINISHED + pb);
				break;
			}
			case COUNT_UNFIN_FINAL :
			{
				Increment(COLUMN_UNFIN_FINAL + pb);
				break;
			}
			default : fatal("%d: unsupported bscourtrep counter", what);
		}
	}
}

void TBossCourtReport::Show()
{
	if (office.Open("BosRep2.htm"))
	{
		ShowUpper();
		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(wait);
	}
}

// ----- entry point ---------------------------------------------------------
void bscourtrep(TWindow *parent, const TReportGroup &reportGroup)
{
	TBossCourtReport(parent, reportGroup).Execute();
}
#endif  // ADMIN || AREAL || MILITARY

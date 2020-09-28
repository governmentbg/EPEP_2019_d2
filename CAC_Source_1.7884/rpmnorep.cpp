#include "cac.h"

#if COURT_TYPE == COURT_REGIONAL
// ----- COLUMN_ -------------------------------------------------------------
enum
{
	COLUMN_COME_PERIOD	= 0,
	COLUMN_ALL_FINISHED	= 1,
	COLUMN_FIN_CONFIRMED	= 2,
	COLUMN_FIN_CHANGED	= 3,
	COLUMN_FIN_CANCELED	= 4,
	COLUMN_UNFIN_FINAL	= 5
};

// ----- TMNOReport ----------------------------------------------------------
class TMNOReport : public TReport
{
public:
	TMNOReport(TWindow *parent, const TReportGroup &reportGroup);

	void Execute();

protected:
	TReportJudgeArray judges;
	TReportJudge summary;

	virtual void LawCritExtra();

	void Increment(const char *ucn, int column);

	void SplitFinished();

	virtual void CountLawsuits(int what);
	void Show();
};

TMNOReport::TMNOReport(TWindow *parent, const TReportGroup &reportGroup)
	: TReport(parent, reportGroup), summary(EMPTY)
{
	summary.name.cpy("ОБЩО");
}

void TMNOReport::Execute()
{
	CountPeriod();
	CountFinished();
	CountFinal();
	Show();
	Debug();
}

void TMNOReport::LawCritExtra()
{
	lawCrit.AddChar("L.F_KIND", KIND_PRIVATE_PUNISHMENT_LAW);
	lawCrit.AddLongs("L.F_SUBJECT", reportGroup.subjects);
}

void TMNOReport::Increment(const char *ucn, int column)
{
	judges.Increment(ucn, 0, column, false);
	summary.values[0][column]++;
}

void TMNOReport::SplitFinished()
{
	if (FindFinalizer())
	{
		Increment(session.judge, COLUMN_ALL_FINISHED);

		if (strchr(RESULT_ALL_CANCELEDS, session.result))
			Increment(session.judge, COLUMN_FIN_CANCELED);
		else
		{
			enum { CONFIRMEDS = 1, CHANGEDS = 2 };
			long flags = 0;
			TGArray<TDecision> decisions;
			try2decide(&session, &decisions, NULL, "F_RESULT");

			for (int i = 0; i < decisions.Count(); i++)
			{
				const TDecision *decision = decisions[i];

				if (strchr(RESULT_KEEPING_METHOD_CONFIRMEDS, decision->result))
					flags |= CONFIRMEDS;
				else if (strchr(RESULT_KEEPING_METHOD_CHANGEDS, decision->result))
					flags |= CHANGEDS;
			}

			if (!flags)
				dprintf("%s е решено, но няма акт за \"без ув\" или \"уваж.\"\n", sLawKey);
			else
			{
				if (flags & CONFIRMEDS)
					Increment(session.judge, COLUMN_FIN_CONFIRMED);
				if (flags & CHANGEDS)
					Increment(session.judge, COLUMN_FIN_CHANGED);
				if (flags == (CONFIRMEDS | CHANGEDS))
					dprintf("%s има актове едновременно за \"без ув\" и \"уваж.\"\n", sLawKey);
			}
		}
	}
}

void TMNOReport::CountLawsuits(int what)
{
	switch (what)
	{
		case COUNT_NEW_PERIOD :
		{
			if (find_finalizer(&lawsuit, session, start, final))
				strcpy(lawsuit.judge, session.judge);

			Increment(lawsuit.judge, COLUMN_COME_PERIOD);
			break;
		}
		case COUNT_FINISHED :
		{
			SplitFinished();
			break;
		}
		case COUNT_UNFIN_FINAL :
		{
			Increment(lawsuit.judge, COLUMN_UNFIN_FINAL);
			break;
		}
		default : fatal("%d: unsupported c2courtrep counter", what);
	}
}

void TMNOReport::Show()
{
	TOffice office;

	if (office.Open("MNORep_R.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, report_fields, this);

		for (int i = 0; i < judges.Count(); i++)
			office.Emit(tr1start, tr1final, repjudge_fields, judges[i]);

		office.Emit(tr1start, tr1final, repjudge_fields, &summary);
		office.Emit(tr1final, NULL, report_fields, this);
		office.Show(wait);
	}
}

// ----- entry point ---------------------------------------------------------
void rpmnorep(TWindow *parent, const TReportGroup &reportGroup)
{
	TMNOReport(parent, reportGroup).Execute();
}
#endif  // REGIONAL
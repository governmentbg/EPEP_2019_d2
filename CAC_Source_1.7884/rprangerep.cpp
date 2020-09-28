// ----- COLUMN_ -------------------------------------------------------------
enum
{
	COLUMN_UNFIN_START	= 0,
	COLUMN_COME_PERIOD	= 1,
	COLUMN_NEW_RETURN		= 2,
	COLUMN_NEW_UNSPEC		= 3,
	COLUMN_NEW_356		= 4,
	COLUMN_SS_RESTART		= 5,
	COLUMN_ALL_NEW_SS		= 6,
	COLUMN_ALL_PROGRESS	= 7,
	COLUMN_ALL_FINISHED	= 8,
	COLUMN_FIN_DECIDED	= 9,
	COLUMN_FIN_CANCELED	= 10,
	COLUMN_FIN_BULWARK	= 11,
	COLUMN_FIN_DECIDED_356	= 12,
	COLUMN_FIN_CANCELED_356	= 13,
	COLUMN_FINISHED_FAST	= 14,
	COLUMN_APPEALED		= 15,
	COLUMN_UNFIN_FINAL	= 16
};

enum
{
	COLUMN_JUDGED_ALL		= 17,
	COLUMN_JUDGED_ACQUIT	= 18,
	COLUMN_JUDGED_PUNISHED	= 19,
	COLUMN_JUDGED_CHILD	= 20,
	COLUMN_JUDGED_TO_3Y	= 21,
	COLUMN_JUDGED_TO_3YCOND	= 22,
	COLUMN_JUDGED_3_TO_15Y	= 23,
	COLUMN_JUDGED_FINE	= 24,
	COLUMN_JUDGED_PROBES	= 25,
	COLUMN_JUDGED_OTHERS	= 26,
	COLUMN_JUDGED_SETTLED	= 27
};

enum
{
	COLUMN_A_UNFIN_START	= 0,
	COLUMN_A_ALL_NEW_SS	= 1,
	COLUMN_A_ALL_PROGRESS	= 2,
	COLUMN_A_ALL_FINISHED	= 3,
	COLUMN_A_FIN_CONFIRMED	= 4,
	COLUMN_A_FIN_CHANGED	= 5,
	COLUMN_A_FIN_REJECTED	= 6,
	COLUMN_A_FIN_CANCELED	= 7,
	COLUMN_A_UNFIN_FINAL	= 8
};

// ----- TRegPunRangeReport --------------------------------------------------
class TRegPunRangeReport : public TPun1stRangeReport
{
public:
	TRegPunRangeReport(TWindow *parent, const TRangeReportGroup &reportGroup);

	void Execute();

protected:
	bool Is356() { return (lawsuit.quick & (QUICK_QUICK | QUICK_IMMEDIATE)) == QUICK_QUICK; }
	const TRangeReportGroup &RangeReportGroup() { return (const TRangeReportGroup &) reportGroup; }
	bool CountAsAlternate();

	virtual void SplitJudgements();
	void SplitPunishments();
	void SplitFinished();
	void SplitAFinished();

	virtual void LawCritExtra();
	virtual void CountLawsuits(int what);

	void PrintRanges(const char *trNstart, const char *trNfinal, bool alternate);
	void Show();
};

TRegPunRangeReport::TRegPunRangeReport(TWindow *parent, const TRangeReportGroup &reportGroup)
	: TPun1stRangeReport(parent, reportGroup)
{
}

void TRegPunRangeReport::Execute()
{
	CountStart();
	CountPeriod();
	CountFinished();
	CountFinal();
	CountAppealed();
	CountJudgements();
	CountSessions();
	ShowStopped();
	Show();
	Check();
	Debug();
}

bool TRegPunRangeReport::CountAsAlternate()
{
#if TESTVER
	bool first = true;
#endif  // TESTVER
	bool countAsAlternate = false;

	for (int i = 0; i < reportRanges.Count(); i++)
	{
		TReportRange *reportRange = reportRanges[i];

		if (reportRange->InsideRange(lawsuit.kind, lawsuit.subject))
		{
		#if TESTVER
			bool tCountAsAlternate = strchr(reportRange->attrib, POST_ALTERNATE) != NULL;

			if (first)
			{
				countAsAlternate = tCountAsAlternate;
				first = false;
			}
			else if (tCountAsAlternate != countAsAlternate)
				dprintf("Предмет %s по %s попада и в двете таблици\n", lawsuit.Code(), sLawKey);
		#else  // TESTVER
			countAsAlternate = strchr(reportRange->attrib, POST_ALTERNATE) != NULL;
			break;
		#endif  // TESTVER
		}
	}
#if TESTVER
	if (first)
		dprintf("Предмет %s по %s не попада в нито една таблица\n", lawsuit.Code(), sLawKey);
#endif  // TESTVER
	return countAsAlternate;
}

void TRegPunRangeReport::SplitPunishments()
{
	if (strchr(PUNISHMENT_UP_TO_3_YEARS_S, judgement.punishment))
	{
		Increment(COLUMN_JUDGED_TO_3Y);

		if (strchr(PUNISHMENT_UP_TO_3_YEARS_CONDS, judgement.punishment))
			Increment(COLUMN_JUDGED_TO_3YCOND);
	}
	else if (strchr(PUNISHMENT_3_TO_15_YEARS, judgement.punishment))
		Increment(COLUMN_JUDGED_3_TO_15Y);
	else if (judgement.punishment == PUNISHMENT_FINE)
		Increment(COLUMN_JUDGED_FINE);
	else if (strchr(PUNISHMENT_REPORT_PROBAS, judgement.punishment))
		Increment(COLUMN_JUDGED_PROBES);
	else if (strchr(PUNISHMENT_REPORT_OTHERS, judgement.punishment))
		Increment(COLUMN_JUDGED_OTHERS);
}

void TRegPunRangeReport::SplitJudgements()
{
	if (!CountAsAlternate())
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
}

void TRegPunRangeReport::SplitFinished()
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

			if (Is356())
				Increment(COLUMN_FIN_CANCELED_356);
		}
		else
		{
			Increment(COLUMN_FIN_DECIDED);

			if (Is356())
				Increment(COLUMN_FIN_DECIDED_356);

			if (CumulationFinish())  // 2017:263
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

void TRegPunRangeReport::SplitAFinished()
{
	if (FindFinalizer())
	{
		if (strchr(RESULT_ALL_CANCELEDS, session.result))
		{
			Increment(COLUMN_A_FIN_CANCELED);
			SplitReinvests();
		}
		else
		{
			TGArray<TDecision> decisions;
			TDecision *decision = find_finalizer(session, decisions, RESULT_RANGEREP_ADMPLS);

			if (decision && strchr(RESULT_RANGEREP_ADMPLS, decision->result))
			{
				switch (decision->result)
				{
					case RESULT_A_FIN_CONFIRMED : Increment(COLUMN_A_FIN_CONFIRMED); break;
					case RESULT_A_FIN_CHANGED : Increment(COLUMN_A_FIN_CHANGED); break;
					default : Increment(COLUMN_A_FIN_REJECTED);
				}
			}
			else
			{
				dprintf("Липсва съдебен акт с резултат \"НП-...\" за %s от %D по %s\n", Kind->Seek(session.kind),
					&session.date, sLawKey);
			}
		}
	}
}

void TRegPunRangeReport::LawCritExtra()
{
	lawCrit.AddRange("NOT L.F_SUBJECT", RangeReportGroup().excludeLow, RangeReportGroup().excludeHigh);
}

void TRegPunRangeReport::CountLawsuits(int what)
{
	if (!CountAsAlternate())
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
					Increment(COLUMN_COME_PERIOD);

					switch (lawsuit.MainConnectType())
					{
						case CONNECT_TYPE_RETURNED_LAWSUIT :
						case CONNECT_TYPE_RERETURNED_LAWSUIT :
						case CONNECT_TYPE_RENEWED_LAWSUIT : Increment(COLUMN_NEW_RETURN); break;
						default : Increment(COLUMN_NEW_UNSPEC);
					}

					if (Is356())
						Increment(COLUMN_NEW_356);
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
			default : fatal("%d: unsupported rprangerep counter", what);
		}
	}
	else
	{
		switch (what)
		{
			case COUNT_UNFIN_START :
			{
				Increment(COLUMN_A_UNFIN_START);
				Increment(COLUMN_A_ALL_PROGRESS);
				break;
			}
			case COUNT_NEW_PERIOD :
			{
				Increment(COLUMN_A_ALL_NEW_SS);
				Increment(COLUMN_A_ALL_PROGRESS);
				break;
			}
			case COUNT_FINISHED :
			{
				if (Increment(COLUMN_A_ALL_FINISHED))
					SplitAFinished();

				SplitQuick368Cut();
				break;
			}
			case COUNT_UNFIN_FINAL :
			{
				Increment(COLUMN_A_UNFIN_FINAL);
				break;
			}
			case COUNT_APPEALED : break;
			default : fatal("%d: unsupported rprangerep-A counter", what);
		}
	}
}

void TRegPunRangeReport::PrintRanges(const char *trNstart, const char *trNfinal, bool alternate)
{
	for (int i = 0; i < reportRanges.Count(); i++)
	{
		TReportRange *reportRange = (TReportRange *) reportRanges[i];

		if (reportRange->Visible() && (strchr(reportRange->attrib, POST_ALTERNATE) != NULL) == alternate)
			office.Emit(trNstart, trNfinal, reprange_fields, reportRange);
	}
}

void TRegPunRangeReport::Show()
{
	if (office.Open("SCCRep_RP1.htm"))
	{
		tr1start = office.Find("tr1 start");
		tr1final = office.Find("tr1 final");
		constant tr2start = office.Find("tr2 start");
		constant tr2final = office.Find("tr2 final");

		office.Emit(NULL, tr1start, report_fields, this);
		PrintRanges(tr1start, tr1final, false);
		office.Emit(tr1final, tr2start, rangerep_fields, this);

		PrintRanges(tr2start, tr2final, true);
		office.Emit(tr2final, NULL, rangerep_fields, this);
		office.Show(wait);
	}
}

// ----- entry point ---------------------------------------------------------
void p1rangerep(TWindow *parent, const TRangeReportGroup &reportGroup)
{
	TRegPunRangeReport(parent, reportGroup).Execute();
}

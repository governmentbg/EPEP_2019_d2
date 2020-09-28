#include "cac.h"

// ----- SECTION_ ------------------------------------------------------------
enum
{
	SECTION_UNFIN_START 	= 1,
	SECTION_COME_PERIOD	= 2,
	SECTION_ALL_PROGRESS	= 3,
	SECTION_ALL_FINISHED	= 4,
	SECTION_FIN_DECIDED	= 5,
	SECTION_FIN_CANCELED	= 6,
	SECTION_THREE_OR_LESS	= 7,
	SECTION_UNFIN_FINAL	= 8,
	SECTION_NULL		= 0
};

#if COURT_TYPE == COURT_APPEAL
enum
{
	COLUMN_CT274_SOURCES	= 6
};
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN
enum
{
	COLUMN_CONNECT_ALLS	= 16,
	COLUMN_CONNECT_TAMDCH	= 17,
	COLUMN_CONNECT_OTHERS	= 18
};
#endif  // ADMIN

// ----- TLawsuitJudgeReport -------------------------------------------------
class TLawsuitJudgeReport : public TReport
{
public:
	TLawsuitJudgeReport(TWindow *parent, const TReportGroup &reportGroup);

	void Execute();

protected:
	TReportJudgeArray judges;
	TGArray<TSubject> subjects;
	long column;

	bool SeekSubject();
	void Increment(const char *ucn, int section);

	void SplitFinished();
	virtual void CountLawsuits(int what);
	void Show();
	void Check();
};

TLawsuitJudgeReport::TLawsuitJudgeReport(TWindow *parent, const TReportGroup &reportGroup)
	: TReport(parent, reportGroup)
{
	msql m(Subject->Name);

	wait->SetText("Зареждане на предмети...");
	m.AddChars("F_TYPE", types);
	Subject->LoadArray(subjects, str(m));
}

bool TLawsuitJudgeReport::SeekSubject()
{
	TSubject *subject = NULL;

	for (int i = 0; i < subjects.Count(); i++)
	{
		if (subjects[i]->type == lawsuit.key.type && subjects[i]->subject == lawsuit.subject)
		{
			subject = subjects[i];
			break;
		}
	}

	if (!subject)
		dlprintf("Липсва предмет %s за %s\n", lawsuit.Code(), sLawKey);
	else
	{
	#if COURT_TYPE == COURT_APPEAL
		// 2017:096 IRQ/LPR: lacking dedicated subject
		if (strchr(KIND_CT274_INDOCS, lawsuit.sourceKind))
			column = COLUMN_CT274_SOURCES;
		else
	#endif  // APPEAL
		{
			bool quick = CourtType == COURT_REGIONAL && (lawsuit.quick & QUICK_QUICK) &&
				subject->QuickReport(lawsuit.kind);

			column = quick ? subject->QuickReport(lawsuit.kind) : subject->JudgeReport(lawsuit.kind);
		}

		if (!column)
		{
			dlprintf("Предмет %s за %s няма отчетна колона\n", lawsuit.Code(), sLawKey);
			subject = NULL;
		}
	}

	return subject != NULL;
}

void TLawsuitJudgeReport::Increment(const char *ucn, int section)
{
	judges.Increment(ucn, section - 1, column - 1);
#if COURT_TYPE == COURT_ADMIN
	if (column == COLUMN_CONNECT_TAMDCH || column == COLUMN_CONNECT_OTHERS)
		judges.Increment(ucn, section - 1, COLUMN_CONNECT_ALLS - 1, false);
#endif  // ADMIN
}

void TLawsuitJudgeReport::SplitFinished()
{
	if (FindFinalizer())
	{
		// 2008:198 LRQ: moved here to use finalizer judge
		Increment(session.judge, SECTION_ALL_FINISHED);

		if (strchr(RESULT_ALL_CANCELEDS, session.result))
			Increment(session.judge, SECTION_FIN_CANCELED);
		else
			Increment(session.judge, SECTION_FIN_DECIDED);

		if (strchr(AGES_THREE_OR_LESS, lawsuit.age))
			Increment(session.judge, SECTION_THREE_OR_LESS);
	}
}

void TLawsuitJudgeReport::CountLawsuits(int what)
{
	if (SeekSubject())
	{
		switch (what)
		{
			case COUNT_UNFIN_START :
			{
				Increment(lawsuit.judge, SECTION_UNFIN_START);
				Increment(lawsuit.judge, SECTION_ALL_PROGRESS);
				break;
			}
			case COUNT_NEW_PERIOD :
			{
				// 2008:197 LRQ: rejudge -> judge
				//Increment(lawsuit.rejudge, SECTION_COME_PERIOD);
				//Increment(lawsuit.rejudge, SECTION_ALL_PROGRESS);

				// 2010:015 LRQ/VSS: COME_PERIOD judge = finalizer-judge-if-any
				if (find_finalizer(&lawsuit, session, start, final))
					strcpy(lawsuit.judge, session.judge);

				Increment(lawsuit.judge, SECTION_COME_PERIOD);
				Increment(lawsuit.judge, SECTION_ALL_PROGRESS);
				break;
			}
			case COUNT_FINISHED :
			{
				// 2008:198 LPR: moved to split, using session judge
				//Increment(lawsuit.judge, SECTION_ALL_FINISHED);
				SplitFinished();
				break;
			}
			case COUNT_UNFIN_FINAL :
			{
				Increment(lawsuit.judge, SECTION_UNFIN_FINAL);
				break;
			}
			default : fatal("%d: unsupported judgerep counter", what);
		}
	}
}

void TLawsuitJudgeReport::Show()
{
	TOffice office;
	char name[SIZE_OF_BASE_NAME];

	// 2009:317 IRQ: court type military appeal -> appeal
	sprintf(name, "JudRep%c%c.htm", *types == TYPE_PUNISHMENT_LAW ? TYPE_PUNISHMENT_LAW : TYPE_CITIZEN_LAW,
		CourtType == COURT_MILITARY_APPEAL ? COURT_LETTER_APPEAL : COURT_LETTER);

	if (office.Open(name))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");
		constant tr2start = office.Find("tr2 start");
		constant tr2final = office.Find("tr2 final");
	#if COURT_TYPE == COURT_ADMIN
		constant tr3start = office.Find("tr3 start");
		constant tr3final = office.Find("tr3 final");
		constant tr4start = office.Find("tr4 start");
		constant tr4final = office.Find("tr4 final");
	#endif  // ADMIN

		office.Emit(NULL, tr1start, report_fields, this);
		for (int i = 0; i < judges.Count(); i++)
		{
			judges[i]->no = i;
			office.Emit(tr1start, tr1final, repjudge_fields, judges[i]);
		}
		office.Emit(tr1final, tr2start, report_fields, this);
		for (int i = 0; i < judges.Count(); i++)
			office.Emit(tr2start, tr2final, repjudge_fields, judges[i]);
	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		office.Emit(tr2final, NULL, report_fields, NULL);
	#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	#if COURT_TYPE == COURT_ADMIN
		office.Emit(tr2final, tr3start, report_fields, this);
		for (int i = 0; i < judges.Count(); i++)
			office.Emit(tr3start, tr3final, repjudge_fields, judges[i]);
		office.Emit(tr3final, tr4start, report_fields, this);
		for (int i = 0; i < judges.Count(); i++)
			office.Emit(tr4start, tr4final, repjudge_fields, judges[i]);
		office.Emit(tr4final, NULL, report_fields, NULL);
	#endif  // ADMIN

		office.Show(wait);
	}
}

void TLawsuitJudgeReport::Check()
{
	for (int i = 0; i < judges.Count(); i++)
		for (int sect = 0; sect < SECTIONS_MAX; sect++)
			for (int col = 0; col < JUDGE_COLUMNS_MAX; col++)
				if (judges[i]->values[sect][col])
					dprintf("Непрехвърлени данни: %s, секция %d, колона %d, стойност %d\n",
						str(judges[i]->name), sect + 1, col + 1, judges[i]->values[sect][col]);
}

void TLawsuitJudgeReport::Execute()
{
	CountStart();
	CountPeriod();
	CountFinished();
	CountFinal();
	judges.Summary("За всичко дела");
	Show();
	Check();
	Debug();
}

// ----- entry point ---------------------------------------------------------
void lawsuit_judge_report(TWindow *parent, const TReportGroup &reportGroup)
{
	TLawsuitJudgeReport(parent, reportGroup).Execute();
}

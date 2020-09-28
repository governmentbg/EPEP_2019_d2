#include "cac.h"

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
// ----- COUNT_ --------------------------------------------------------------
enum { COUNT_RERETURNED = COUNT_REPORT + 1 };

// ----- COLUMN_ -------------------------------------------------------------
enum
{
	COLUMN_UNFIN_START	= 0,
	COLUMN_COME_PERIOD	= 1,
	COLUMN_ALL_PROGRESS	= 2,
	COLUMN_ALL_FINISHED	= 3,
	COLUMN_FIN_JUDGEMENT	= 4,
	COLUMN_FIN_ALL_381_4	= 5,
	COLUMN_FIN_SETT_381_4	= 8,
	COLUMN_THREE_OR_LESS	= 9,
	COLUMN_UNFIN_FINAL	= 10,
	COLUMN_RERETURNED		= 11,
	COLUMN_JUDGED_ALL		= 12,
	COLUMN_JUDGED_FOREIGN	= 0xD,
	COLUMN_DISCHAR_ALL	= 14,
	COLUMN_DISCHAR_FOREIGN	= 15,

	COLUMN_CONVICT_ALL	= 16,
	COLUMN_CONVICT_3_OR_LESS= 17,
	COLUMN_CONVICT_3_OR_COND= 18,	// subset of 3_OR_LESS
	COLUMN_CONVICT_3_TO_10	= 19,
	COLUMN_CONVICT_10_TO_30	= 20,
	COLUMN_CONVICT_FINE	= 21,
	COLUMN_CONVICT_PROBATION= 22,
	COLUMN_CONVICT_OTHERS	= 23,
	COLUMN_CONVICT_BULWARK	= 24,

	COLUMN_DAMAGES		= 25,
	COLUMNS_MAX			= 26
};

class TReportDanger : public TDanger
{
public:
	TReportDanger();
	static TGroup *Clone();

	long values[COLUMNS_MAX - 1];
	double damage;
	bool nonzero;
};

TReportDanger::TReportDanger()
	: TDanger(TReportDanger::Clone), nonzero(false)
{
	memset(values, '\0', sizeof values);
	damage = 0.0;
}

TGroup *TReportDanger::Clone()
{
	return new TReportDanger();
}

#define danger OFFICE_GROUP_CAST(TReportDanger)

static void dangerep_name(mstr &m, TGroup *group) { m.cpy(danger->name); }

static void dangerep_value(mstr &m, TGroup *group)
{
	unsigned col;

	if (sscanf(danger->Name, "F_C_%d", &col) == 1 && --col < COLUMNS_MAX)
	{
		if (col == COLUMN_DAMAGES)
		{
			if (danger->damage)
			{
				m.printf("%.2lf", danger->damage);
				danger->damage = 0.0;	// mark as used
			}
		}
		else
		{
			itom(danger->values[col], m);
			danger->values[col] = 0;	// mark as used
		}
	}
}

#undef danger

static const offield dangerep_fields[] =
{
	{ "F_NAME",	dangerep_name },
	{ "*F_C_",	dangerep_value },
	{ NULL,	NULL }
};

// ----- TDangerReport -------------------------------------------------------
class TDangerReport : public TReport
{
public:
	TDangerReport(TWindow *parent, const TReportGroup &reportGroup);

	void Execute();

protected:
	TGSArray<TReportDanger> dangers;
	TJudgement judgement;
	TDecision decision;

	virtual void LawCritExtra();

	bool Increment(int column, int muldiv, const char *name, bool optional);
	void Increment(int column);

	void TrackJudgeds();
	void SplitFinished();
	virtual void HandleRelated(const TConnect &);
	virtual void CountLawsuits(int what);
	void CountReReturned();
	void SplitConvicted();
	void CountConvicted();
	void Show();
	void Check();
};

TDangerReport::TDangerReport(TWindow *parent, const TReportGroup &reportGroup)
	: TReport(parent, reportGroup)
{
	msql m;

	Danger->FloatCriteria(m);
	wait->SetText("Зареждане на глави и текстове от НК...");
	TReportDanger().LoadArray(dangers, str(m));
}

void TDangerReport::LawCritExtra()
{
	lawCrit.AddIsNull("L.F_DANGER", false);
}

bool TDangerReport::Increment(int column, int muldiv, const char *name, bool optional)
{
	int danger = lawsuit.danger / muldiv * muldiv;

	for (int i = 0; i < dangers.Count(); i++)
	{
		TReportDanger *reportDanger = dangers[i];

		if (reportDanger->danger == danger)
		{
			if (column == COLUMN_DAMAGES)
				reportDanger->damage += decision.damages;
			else
				reportDanger->values[column]++;

			reportDanger->nonzero = true;
			return true;
		}
	}

	if (optional)
		return true;

	dprintf("Липсва %s за код %ld от %s (колона %d)\n", name, lawsuit.danger, sLawKey, column);
	return false;
}

void TDangerReport::Increment(int column)
{
	if (Increment(column, DANGER_MASK_SECTION, "глава", false))
		if (Increment(column, DANGER_MASK_ARTICLE, "раздел", true))
			Increment(column, 1, "член", false);
}

void TDangerReport::TrackJudgeds()
{
	TGArray<TSideWind> sideWinds;
	load_sidebasicv(&lawsuit, sideWinds);

	for (int i = 0; i < sideWinds.Count(); i++)
	{
		TSideWind *sideWind = sideWinds[i];

		if (lawsuit.Defendant(sideWind->involvement))
		{
			Increment(COLUMN_JUDGED_ALL);

			bool foreign = false;

			if (strchr(UCN_CITIZENS, sideWind->ucnType))
			{
				TCitizen citizen;
				citizen << sideWind;
				if (citizen.Try())
					foreign = strcmp(citizen.country, BUILDIN_COUNTRY);
				else
					dprintf("Липсват данни за лицето %s %s, страна по %s\n", UCNType->Seek(sideWind->ucnType), sideWind->ucn, sLawKey);
			}

			if (foreign)
				Increment(COLUMN_JUDGED_FOREIGN);

			msql m("T_JUDGEMENT", "F_UCN");

			m.Add(sideWind, "F_UCN, F_UCN_TYPE");
			m.Add(&lawsuit, "F_NO, F_YEAR, F_TYPE");
			m.AddChars("F_JUDGED", JUDGED_DISCHARGEDS);

			TQuery q(str(m));
			if (q.Read())
			{
				Increment(COLUMN_DISCHAR_ALL);
				if (foreign)
					Increment(COLUMN_DISCHAR_FOREIGN);
			}
		}
	}
}

void TDangerReport::SplitFinished()
{
	if (FindFinalizer())
	{
		if (strchr(RESULT_FIN_JUDGEMENTS, session.result))
			Increment(COLUMN_FIN_JUDGEMENT);
		// 2008:274 LPR: using else catches any unknown results
		else
		{
			Increment(COLUMN_FIN_ALL_381_4);
			if (strchr(RESULT_FIN_SETT_381_4S, session.result))
				Increment(COLUMN_FIN_SETT_381_4);
		}

		if (strchr(AGES_THREE_OR_LESS, lawsuit.age))
			Increment(COLUMN_THREE_OR_LESS);

		if (!strchr(RESULT_FIN_ABORTEDS, session.result))
		{
			Subject->Seek(lawsuit.key.type, lawsuit.subject);

			if (!strchr(Subject->Attrib(lawsuit.kind), ATTRIB_DO_NOT_COUNT_DEFENDANTS))
				TrackJudgeds();
		}

		msql m(decision.Name, "F_DAMAGES");

		m.Add(&lawsuit, "F_NO, F_YEAR, F_TYPE");
		m.AddIsNull("F_DAMAGES", false);

		TQuery q(str(m));
		while (q.Read())
		{
			decision << q;
			Increment(COLUMN_DAMAGES);
		}
	}
}

void TDangerReport::HandleRelated(const TConnect &)
{
	Increment(COLUMN_RERETURNED);
}

void TDangerReport::CountLawsuits(int what)
{
	if (lawsuit.danger)
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
				Increment(COLUMN_COME_PERIOD);
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
			case COUNT_RERETURNED :
			{
				TraceRelated(lawsuit.key, CONNECT_TYPE_RERETURNED_LAWSUIT);
				break;
			}
			default : fatal("%d: unsupported dangerep counter", what);
		}
	}
}

void TDangerReport::CountReReturned()
{
	LawCritBasic();
	lawCrit.printf("L.F_FORCE_DATE BETWEEN %ld AND %ld", start.Value(), final.Value());
	LawCritExtra();
	QueryLawsuits("Изпращания за доразследване...", COUNT_RERETURNED, "%d изпращания за доразследване");
}

void TDangerReport::SplitConvicted()
{
	if (strchr(PUNISHMENT_UP_TO_3_YEARS_S, judgement.punishment))
	{
		Increment(COLUMN_CONVICT_3_OR_LESS);
		if (strchr(PUNISHMENT_UP_TO_3_YEARS_CONDS, judgement.punishment))
			Increment(COLUMN_CONVICT_3_OR_COND);
	}
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	else if (strchr(PUNISHMENT_3_TO_10_YEARS_S, judgement.punishment))
		Increment(COLUMN_CONVICT_3_TO_10);
	else if (strchr(PUNISHMENT_10_TO_30_YEARS_S, judgement.punishment))
		Increment(COLUMN_CONVICT_10_TO_30);
#endif  // AREAL || REGIONAL
	else if (judgement.punishment == PUNISHMENT_FINE)
		Increment(COLUMN_CONVICT_FINE);
	else if (judgement.punishment == PUNISHMENT_PROBATION)
		Increment(COLUMN_CONVICT_PROBATION);
	else if (strchr(PUNISHMENT_OTHERS, judgement.punishment))
		Increment(COLUMN_CONVICT_OTHERS);

	if (strchr(JUDGED_CONVICTED_BULWARKS, judgement.judged))
		Increment(COLUMN_CONVICT_BULWARK);
}

void TDangerReport::CountConvicted()
{
	// 2008:301 FIX: +F_DANGER; 2014:210 LPR: use aliases
	msql m("T_JUDGEMENT J JOIN T_LAWSUIT L ON L.F_NO = J.F_NO AND L.F_YEAR = J.F_YEAR AND L.F_TYPE = J.F_TYPE",
		"J.F_NO, J.F_YEAR, J.F_TYPE, J.F_UCN_TYPE, J.F_UCN, J.F_JUDGED, J.F_PUNISHMENT, L.F_SUBJECT, J.F_FLAGS_X, "
		"L.F_DANGER, L.F_KIND");
	CritBasic(m, "J.F_TYPE");
	m.AddRange("J.F_FORCE_DATE", start.Value(), final.Value());
	m.AddChars("J.F_JUDGED", JUDGED_CONVICTEDS);
	m.AddChars("J.F_UCN_TYPE", UCN_CITIZENS);
	m.AddIsNull("L.F_DANGER", false);
	m.AddFlags("J.F_FLAGS_X", JUDGEMENT_X_PRIORITY, JUDGEMENT_X_PRIORITY, JUDGEMENT_X_ALL);
	TQuery q(str(m));

	int count = 0;
	wait->SetText("Осъдени лица с влязла в сила присъда...");
	while (q.Read())
	{
		judgement << q;
		wait->TickCount(++count, "осъдени лица");
		lawsuit << q;
		Subject->Seek(lawsuit.key.type, lawsuit.subject);

		if (strchr(Subject->Attrib(lawsuit.kind), ATTRIB_DO_NOT_COUNT_DEFENDANTS))
			continue;

		Increment(COLUMN_CONVICT_ALL);
		SplitConvicted();
	}
}

void TDangerReport::Show()
{
	TOffice office;

	if (office.Open("HiDRep.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, report_fields, this);

		for (int i = 0; i < dangers.Count(); i++)
			if (dangers[i]->nonzero)
				office.Emit(tr1start, tr1final, dangerep_fields, dangers[i]);

		office.Emit(tr1final, NULL, report_fields, this);
		office.Show(wait);
	}
}

void TDangerReport::Check()
{
	for (int i = 0; i < dangers.Count(); i++)
	{
		for (int col = 0; col < COLUMNS_MAX - 1; col++)
			if (dangers[i]->values[col])
				dprintf("Непрехвърлени данни: %s, колона %d, стойност %d\n", dangers[i]->name, col + 1, dangers[i]->values[col]);
		if (dangers[i]->damage)
			dprintf("Непрехвърлени данни: %s, колона %d, стойност %.2lfлв\n", dangers[i]->name, COLUMN_DAMAGES, dangers[i]->damage);
	}
}

void TDangerReport::Execute()
{
	CountStart();
	CountPeriod();
	CountFinished();
	CountFinal();
	CountReReturned();
	CountConvicted();
	Show();
	Check();
	Debug();
}

// ----- entry point ---------------------------------------------------------
void danger_report(TWindow *parent, const TReportGroup &reportGroup)
{
	TDangerReport(parent, reportGroup).Execute();
}
#endif	// AREAL || MILITARY || REGIONAL

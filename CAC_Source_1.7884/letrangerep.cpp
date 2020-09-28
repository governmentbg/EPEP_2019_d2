#include "cac.h"

// 2017:088 official
#if COURT_TYPE == COURT_ADMIN
// ----- COLUMN_ -------------------------------------------------------------
enum
{
	COLUMN_UNFIN_START		= 0,
	COLUMN_COME_PERIOD		= 1,
	COLUMN_ALL_PROGRESS		= 2,
	COLUMN_FIN_DECIDED		= 3,
	COLUMN_GRANT_NULLIFY		= 4,
	COLUMN_GRANT_PARTIAL		= 5,
	COLUMN_GRANT_REJECTED		= 6,
	COLUMN_GRANT_ENTIRE		= 7,	// N.B. GRANT are DECIDED
	COLUMN_FIN_CANCELED		= 8,
	COLUMN_CANCEL_SETTLED		= 9,
	COLUMN_ALL_FINISHED		= 10,
	COLUMN_FIN_ONE_OR_LESS		= 11,	// N.B. FIN are FINISHED
	COLUMN_FIN_ONE_TO_THREE		= 12,
	COLUMN_FIN_MORE_THAN_THREE	= 13,
	COLUMN_UNFIN_FINAL		= 14,
	COLUMN_ACT_ONE_OR_LESS		= 15,
	COLUMN_ACT_ONE_TO_THREE		= 16,
	COLUMN_ACT_MORE_THAN_THREE	= 17,
	COLUMN_OUT_SEND			= 18,
	COLUMN_OUT_RETURNED_FINISHED	= 19,	// 2017:010 was ACCEPTED
	COLUMN_OUT_RETURNED_DECIDED	= 22,
	COLUMN_OUT_RETURNED_CANCELED	= 25,
	COLUMN_OUT_RETURNED_INTERMED	= 28,	//  2017:010
	COLUMN_SS_RESTART			= 31
};

enum
{
	SECTION_FINISHED		= 0,	// 2017:010 was RETURNED
	SECTION_DECIDED		= 1,
	SECTION_CANCELED		= 2,
	SECTION_INTERMED		= 3,	//  2017:010 new section
	RETURN_SECTIONS_MAX	= 4,

	RETURN_COLUMN_ACCEPTS	= 0,
	RETURN_COLUMN_REJECTS	= 1,
	RETURN_COLUMN_OTHERS	= 2,
	RETURN_COLUMNS_MAX	= 3
};

// ----- TLetterRangeReport --------------------------------------------------
class TLetterRangeReport : public TRangeReport
{
public:
	TLetterRangeReport(TWindow *parent, TRangeReportGroup &tReportGroup);

	long values[RETURN_SECTIONS_MAX][RETURN_COLUMNS_MAX];

	void Execute();

protected:
	int returnColumn;

	void SplitAges();
	void SplitFinished();
	virtual void CountLawsuits(int what);
	void OutLoad(TQuery &q);
	void OutSend();
	void OutRetIndex();
	void OutReturned();
	void Show();
	void Check();
};

TLetterRangeReport::TLetterRangeReport(TWindow *parent, TRangeReportGroup &tReportGroup)
	: TRangeReport(parent, tReportGroup)
{
	memset(values, '\0', sizeof values);
}

void TLetterRangeReport::SplitAges()
{
	switch (lawsuit.age)
	{
		case AGE_ONE_OR_LESS : Increment(COLUMN_FIN_ONE_OR_LESS); break;
		case AGE_THREE_OR_LESS : // LRQ: falldown
		case AGE_ONE_TO_THREE : Increment(COLUMN_FIN_ONE_TO_THREE); break;
		case AGE_NONE_YET : dprintf("%s е решено, но няма продължителност\n", sLawKey); break;
		default :
		{
			if (strchr(AGES_MORE_THAN_THREE, lawsuit.age))
				Increment(COLUMN_FIN_MORE_THAN_THREE);
			else
				dprintf("%s е решено, но има невалидна продължителност\n", sLawKey);
		}
	}

	switch (PublicActAge())
	{
		case AGE_ONE_OR_LESS : Increment(COLUMN_ACT_ONE_OR_LESS); break;
		case AGE_ONE_TO_THREE : Increment(COLUMN_ACT_ONE_TO_THREE); break;
		case AGE_NULL : break;
		default : Increment(COLUMN_ACT_MORE_THAN_THREE);
	}
}

void TLetterRangeReport::SplitFinished()
{
	if (FindFinalizer())
	{
		if (strchr(RESULT_ALL_CANCELEDS, session.result))
		{
			Increment(COLUMN_FIN_CANCELED);
		#if COURT_TYPE == COURT_ADMIN
			if (session.result == RESULT_CANCEL_SETTLED)
				Increment(COLUMN_CANCEL_SETTLED);
		#endif  // ADMIN
		}
		else
		{
			Increment(COLUMN_FIN_DECIDED);

			switch (lawsuit.grant)
			{
				case GRANT_NULLIFY : Increment(COLUMN_GRANT_NULLIFY); break;
				case GRANT_ENTIRE : Increment(COLUMN_GRANT_ENTIRE); break;
				case GRANT_PARTIAL : Increment(COLUMN_GRANT_PARTIAL); break;
				case GRANT_REJECTED : Increment(COLUMN_GRANT_REJECTED); break;
				default : dprintf("%s е решено, но няма степен на уважаване на иска\n", sLawKey);
			}
		}

		SplitAges();
	}
}

void TLetterRangeReport::CountLawsuits(int what)
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
			// 2017:058 TRQ: FIX: was missing
			if (lawsuit.intervalFlags & LAW_INTERVAL_SECONDARY)
				Increment(COLUMN_SS_RESTART);
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
		default :
			fatal("%d: unsupported activrep counter", what);
	}
}

void TLetterRangeReport::OutLoad(TQuery &q)
{
	outReg << q;
	SetOutKey();
	lawsuit << q;
	lawsuit.key = outReg.lawsuit;
	SetLawKey();
}

void TLetterRangeReport::OutSend()
{
	TFloatOutRegGroup group;

	strcpy(group.lawTypes, types);
	group.minDate = start;
	group.maxDate = final;
	sprintf(group.compositions, "%c", composition);
	// 2014:210 LPR: loop select -> joined; 2018-01-18 FIX: +L.F_KIND for range instances
	group.fields = "O.F_NO, O.F_YEAR, O.F_LAWSUIT_NO, O.F_LAWSUIT_YEAR, O.F_LAWSUIT_TYPE, L.F_SUBJECT, L.F_KIND";

	msql m;
	group.FormCriteria(m);
	TQuery q(str(m));

	while (q.Read())
	{
		OutLoad(q);

		if (lawsuit.subject)
			Increment(COLUMN_OUT_SEND);
		else
			dprintf("Липсва %s за %s", sLawKey, sOutKey);
	}
}

void TLetterRangeReport::OutRetIndex()
{
	char lawsuitKind = origin2LawsuitKind(outReg.lawsuit, false);

	if (lawsuitKind)
	{
		TVSSIndexSet *indexSet = TVSSIndexSet::Seek(lawsuitKind, outReg.returned);

		if (indexSet)
		{
			int numIndex = atoi(outReg.vssIndex);

			if (indexSet->iAccepts.Find(numIndex) != INT_MAX)
				returnColumn = RETURN_COLUMN_ACCEPTS;
			else if (indexSet->iRejects.Find(numIndex) != INT_MAX)
				returnColumn = RETURN_COLUMN_REJECTS;
			else
			{
				if (numIndex <= 0)
					fatal("Индекс %s на %s не започва с число.", outReg.vssIndex, sOutKey);

				returnColumn = RETURN_COLUMN_OTHERS;
			}

			if (lawsuit.subject)
			{
				inReg.key.no = outReg.inNo;
				inReg.key.year = outReg.inYear;
				SetInKey();

				if (!inReg.Try("F_SESSION_KIND, F_DECISION_DATE"))
					dprintf("Липсва %s за %s\n", sInKey, sOutKey);
				else
				{
					session.key = lawsuit.key;
					session.kind = inReg.sessionKind;
					session.date = inReg.decisionDate;

					// 2011:038 LRQ/LPR: RETURNED does not equal DECIDED + CANCELED
					if (session.kind && !session.date.Empty() && session.Try() && !session.final.Empty())
					{
						if (strchr(RESULT_ALL_CANCELEDS, session.result))
						{
							Increment(COLUMN_OUT_RETURNED_CANCELED + returnColumn);
							values[SECTION_CANCELED][returnColumn]++;
						}
						else
						{
							Increment(COLUMN_OUT_RETURNED_DECIDED + returnColumn);
							values[SECTION_DECIDED][returnColumn]++;
						}

						Increment(COLUMN_OUT_RETURNED_FINISHED + returnColumn);
						values[SECTION_FINISHED][returnColumn]++;
					}
					else	// 2017:010
					{
						Increment(COLUMN_OUT_RETURNED_INTERMED + returnColumn);
						values[SECTION_INTERMED][returnColumn]++;
					}
				}
			}
			else
				dprintf("Липсва %s за %s", sLawKey, sOutKey);
		}
		else
			dprintf("Индекс %s на %s е извън дефинициите\n", outReg.vssIndex, sOutKey);
	}
	else
		dprintf("Грешка при четене на делото/входящия документ за %s\n", sOutKey);
}

void TLetterRangeReport::OutReturned()
{
	TFloatOutRegGroup group;

	strcpy(group.lawTypes, types);
	group.minReturned = start;
	group.maxReturned = final;
	sprintf(group.compositions, "%c", composition);
	group.fields = "O.F_NO, O.F_YEAR, O.F_LAWSUIT_NO, O.F_LAWSUIT_YEAR, O.F_LAWSUIT_TYPE, O.F_VSS_INDEX, O.F_IN_NO, "
		"O.F_IN_YEAR, O.F_RETURNED, L.F_SUBJECT, L.F_KIND";	// 2018-01-18 FIX: +L.F_KIND for range instances

	msql m;
	group.FormCriteria(m);
	TQuery q(str(m));

	while (q.Read())
	{
		OutLoad(q);

		if (any(outReg.vssIndex))
			OutRetIndex();
	}
}

#define report OFFICE_GROUP_CAST(TLetterRangeReport)

static void activrep_value(mstr &m, TGroup *group)
{
	unsigned sect, col;

	if (sscanf(report->Name, "F_S_%d_C_%d", &sect, &col) == 2 && --sect < RETURN_SECTIONS_MAX && --col < RETURN_COLUMNS_MAX)
	{
		itom(report->values[sect][col], m);
		report->values[sect][col] = 0;	// mark as used
	}
}

#undef report

static const offield letrangerep_fields[] =
{
	{ "*F_S_",		activrep_value },
	{ NULL,		NULL }
};

void TLetterRangeReport::Show()
{
	if (office.Open("SCCRep_LD.htm"))
	{
		ShowUpper();
		office.Emit(tr1final, NULL, letrangerep_fields, this);
		office.Show(wait);
	}
}

void TLetterRangeReport::Check()
{
	TRangeReport::Check();

	for (int sect = 0; sect < RETURN_SECTIONS_MAX; sect++)
		for (int col = 0; col < RETURN_COLUMNS_MAX; col++)
			if (values[sect][col])
				dprintf("Непрехвърлени данни: %s, секция %d, колона %d, стойност %d\n",
					"Резултати от върнати дела", sect + 1, col + 1, values[sect][col]);
}

void TLetterRangeReport::Execute()
{
	CountStart();
	CountPeriod();
	CountFinished();
	CountFinal();
	OutSend();
	OutReturned();
	Show();
	Check();
	Debug();
}

// ----- entry point ---------------------------------------------------------
void letrangerep(TWindow *parent, TRangeReportGroup &reportGroup)
{
	TLetterRangeReport(parent, reportGroup).Execute();
}
#endif  // ADMIN

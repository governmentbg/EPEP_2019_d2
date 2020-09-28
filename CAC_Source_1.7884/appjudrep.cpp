#include "cac.h"

// ----- TAppealVSSIndexSet --------------------------------------------------
class TAppealVSSIndexSet : public TVSSIndexSet
{
public:
	TAppealVSSIndexSet(TVSSIndexSet *indexSet);

	TReportJudgeArray judges;
	TGArray<TVSSIndex> headers;
};

TAppealVSSIndexSet::TAppealVSSIndexSet(TVSSIndexSet *indexSet)
{
	*this << indexSet;
	AddIndexes(headers);
	TVSSIndex *total = new TVSSIndex;
	strcpy(total->index, "ОБЩО");
	headers.Insert(total, 0);
}

// ----- TAppealJudgeReport --------------------------------------------------
class TAppealJudgeReport : public TReport
{
public:
	TAppealJudgeReport(TWindow *parent, const TReportGroup &reportGroup);

	void Execute();

	int table;
	int column;
	int current_width;

	enum { TABLES = 2 };
	TGArray<TAppealVSSIndexSet> indexSets;

	TAppealVSSIndexSet *indexSet;
	TReportJudge *reportJudge;

protected:
	void Collect();
	void EmitColumns(TOffice &office, const char *start, const char *final);
	virtual void CountLawsuits(int what);
	void Show();

	//enum { HDVL_ENTIRE_WIDTH = 1085, HDVL_MIN_TOTAL_WIDTH = 72 };
	enum { HDVL_ENTIRE_WIDTH = 8146, HDVL_MIN_TOTAL_WIDTH = 550 };
	static const double HDVL_TOTAL_WIDTH_FACTOR;
	static offield fields[];
};

TAppealJudgeReport::TAppealJudgeReport(TWindow *parent, const TReportGroup &reportGroup)
	: TReport(parent, reportGroup)
{
}

void TAppealJudgeReport::Execute()
{
	TGSArray<TVSSIndexSet> tIndexSets;
	CDate indexStart;

	TVSSIndexSet::AddMatching(tIndexSets, COURT_TYPE + '0', types, '\0', start, final, true);

	if (!tIndexSets.Count())
		error("Няма дефинирани индекси за отчетния период.");

	for (int index = 0; index < tIndexSets.Count(); index++)
	{
		TVSSIndexSet *tIndexSet = tIndexSets[index];

		if (index)
		{
			if (tIndexSet->start != indexStart)
			{
				error("Отчетния период включва несъвместими индекси.");
				return;
			}
		}
		else
			indexStart = tIndexSet->start;

		indexSets.Add(new TAppealVSSIndexSet(tIndexSet));
	}

	Collect();
	Show();
	Debug();
}

void TAppealJudgeReport::Collect()
{
	TFloatOutRegGroup group;
	TInRegEvent inRegEvent;
	msql m;

	strcpy(group.lawTypes, types);
	strcpy(group.compositions, C2S[composition]);	// 2016:007 IRQ: FIX: was missing
	group.minReturned = start;
	group.maxReturned = final;
	group.fields = "O.F_NO, O.F_YEAR, O.F_VSS_INDEX, O.F_LAWSUIT_NO, O.F_LAWSUIT_YEAR, O.F_LAWSUIT_TYPE, O.F_RETURNED, "
		"I.F_DECISION_KIND, IE.F_JUDGE";
	group.order = ORDER_BY_NULL;
	group.FormCriteria(m);
	m.AddIsNull("O.F_VSS_INDEX", false);
	TQuery q(str(m));
	while (q.Read())
	{
		outReg << q;
		SetOutKey();
		wait->TickText(sOutKey);

		int index;
		char lawsuitKind = origin2LawsuitKind(outReg.lawsuit, false);

		if (!lawsuitKind)
			dprintf("Грешка при четене на делото/входящия документ за %s\n", sOutKey);

		for (index = 0; index < indexSets.Count(); index++)
		{
			indexSet = indexSets[index];

			if (indexSet->Matches(COURT_TYPE + '0', C2S[lawsuitKind ? kind2type(lawsuitKind) : outReg.college],
				lawsuitKind, outReg.returned, outReg.returned))
			{
				for (column = 1; column < indexSet->headers.Count(); column++)
				{
					if (!strcmp(indexSet->headers[column]->index, outReg.vssIndex))
					{
						inReg << q;
						inRegEvent << q;

						indexSet->judges.Increment(inRegEvent.judge,
							// 2015:216 KIND_IN_UNDECIDABLES -> KIND_APPJUDREP_SETTS
							(strchr(KIND_APPJUDREP_SETTS, inReg.decisionKind) != NULL ||
							strchr(KIND_IN_REG_DOCS, inReg.decisionKind) != NULL), column);
						break;
					}
				}

				if (column == indexSet->headers.Count())
					dprintf("Индекс %s на %s е извън колоните\n", outReg.vssIndex, sOutKey);

				break;
			}
		}

		if (index == indexSets.Count())
			dprintf("Индекс %s на %s е извън дефинициите\n", outReg.vssIndex, sOutKey);
	}
}

#define report OFFICE_GROUP_CAST(TAppealJudgeReport)

static void report_table(mstr &m, TGroup *group)
{
	constant table_names[TAppealJudgeReport::TABLES] = { "РЕШЕНИЯ", "ОПРЕДЕЛЕНИЯ" };

	m.cat(table_names[report->table]);

	if (any(report->indexSet->kinds))
	{
		m.cat(" по ");
		print_kinds_list(m, report->indexSet->kinds, false);
	}
	else if (not_inset(report->types, report->indexSet->types))
	{
		mstr w;

		for (const char *s = report->types; *s; s++)
			if (strchr(report->indexSet->types, *s))
				w.cach_nx(*s);

		m.cat(" по ");
		print_types_list(m, str(w), false);
	}
}

static void report_subcount(mstr &m, TGroup *group)
{
	itom(report->indexSet->headers.Count(), m);
}

static void report_width(mstr &m, TGroup *group) { m.printf("%.2lf", report->current_width / 100.0); }
static void report_index(mstr &m, TGroup *group) { m.cat(report->indexSet->headers[report->column]->index); }
static void report_judge(mstr &m, TGroup *group) { m.cat(report->reportJudge->name); }
static void report_value(mstr &m, TGroup *group) { itom(report->reportJudge->values[report->table][report->column], m); }

#undef report

offield TAppealJudgeReport::fields[] =
{
	{ "F_TABLE",	report_table },
	{ "F_SUBCOUNT",	report_subcount },
	{ "F_WIDTH",	report_width },
	{ "F_INDEX",	report_index },
	{ "F_JUDGE",	report_judge },
	{ "F_VALUE",	report_value },
	{ NULL, NULL }
};

const double TAppealJudgeReport::HDVL_TOTAL_WIDTH_FACTOR = 1.2;

void TAppealJudgeReport::EmitColumns(TOffice &office, const char *start, const char *final)
{
	double remaining_width = HDVL_ENTIRE_WIDTH;
	int remaining = indexSet->headers.Count();

	for (column = 0; column < indexSet->headers.Count(); column++, remaining--)
	{
		current_width = uround(remaining_width / remaining);

		if (!column)
		{
			current_width *= HDVL_TOTAL_WIDTH_FACTOR;

			if (current_width < HDVL_MIN_TOTAL_WIDTH)
				current_width = HDVL_MIN_TOTAL_WIDTH;
		}

		office.Emit(start, final, fields, this);
		remaining_width -= current_width;
	}
}

void TAppealJudgeReport::CountLawsuits(int what)
{
	fatal("%d: unsupported appjudrep counter", what);
}

void TAppealJudgeReport::Show()
{
	TOffice office;

	if (office.Open("JudRep__1.htm"))
	{
		constant tb1start = office.Find("tb1 start");
		constant hd1start = office.Find("hd1 start");
		constant hd1final = office.Find("hd1 final");
		constant tr1start = office.Find("tr1 start");
		constant vl1start = office.Find("vl1 start");
		constant vl1final = office.Find("vl1 final");
		constant tr1final = office.Find("tr1 final");
		constant tb1final = office.Find("tb1 final");

		office.Emit(NULL, tb1start, report_fields, this);

		for (int index = 0; index < indexSets.Count(); index++)
		{
			indexSet = indexSets[index];
			indexSet->judges.Summary("Общо");

			for (table = 0; table < TABLES; table++)
			{
				if (index || table)
					office.EmitBreak();

				office.Emit(tb1start, hd1start, fields, this);
				EmitColumns(office, hd1start, hd1final);
				office.Emit(hd1final, tr1start, null_fields, NULL);
				office.InitAutoGen(0);

				for (int judge = 0; judge < indexSet->judges.Count(); judge++)
				{
					reportJudge = indexSet->judges[judge];

					if (!judge || reportJudge->values[table][0])
					{
						reportJudge->no = judge;
						office.Emit(tr1start, vl1start, fields, this);
						EmitColumns(office, vl1start, vl1final);
						office.Emit(vl1final, tr1final, null_fields, NULL);
					}
				}

				office.Emit(tr1final, tb1final, null_fields, NULL);
			}
		}

		office.Emit(tb1final, NULL, report_fields, this);
		office.Show(wait);
	}
}

// ----- entry point ---------------------------------------------------------
void appeal_judge_report(TWindow *parent, const TReportGroup &reportGroup)
{
	TAppealJudgeReport(parent, reportGroup).Execute();
}
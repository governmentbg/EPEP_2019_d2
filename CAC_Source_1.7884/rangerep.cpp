#include "cac.h"

// ----- TRangeReportGroup ---------------------------------------------------
TRangeReportGroup::TRangeReportGroup()
{
	Add(
		new TChar("F_ATTRIB", &attrib),
		new TCheck("F_WITH_SUBJECTS", &withSubjects, TRUE),
	#if COURT_TYPE == COURT_REGIONAL
		new TLong("F_EXCLUDE_LOW", &excludeLow),
		new TLong("F_EXCLUDE_HIGH", &excludeHigh),
	#endif  // REGIONAL
		NULL
	);

	attrib = POST_BUILTIN;
}

#include "RangeReportDialog.h"
#include "RangeReportDialog.cpp"

// ----- TRangeReport --------------------------------------------------------
TRangeReport::TRangeReport(TWindow *parent, const TRangeReportGroup &tReportGroup)
	: TReport(parent, tReportGroup)
{
	TReportRange::Load(reportRanges, tReportGroup);
}

bool TRangeReport::AddCount(int column, int count)
{
	bool counted = false;

	for (int i = 0; i < reportRanges.Count(); i++)
	{
		TReportRange *reportRange = reportRanges[i];

		if (reportRange->InsideRange(lawsuit.kind, lawsuit.subject))
		{
		#if TESTVER
			if (lawsuit.kind == '?' && lawsuit.subject == 90050)
				message("%c %ld matches %s", lawsuit.kind, lawsuit.subject, reportRange->alias);
		#endif  // TESTVER
			reportRange->values[column] += count;
			counted = true;
		}
	}

	if (!counted)
		dprintf("Предмет %s от %s не попада в нито един обхват\n", lawsuit.Code(), sLawKey);

	return counted;
}

void TRangeReport::CountAppealed()
{
	lawCrit.Begin("T_OUTREG O JOIN T_LAWSUIT L ON L.F_NO = O.F_LAWSUIT_NO AND L.F_YEAR = O.F_LAWSUIT_YEAR "
		"AND L.F_TYPE = O.F_LAWSUIT_TYPE",
		"DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_SUBJECT, L.F_KIND");
	lawCrit.AddRange("O.F_DATE", start, final);
	CritBasic(lawCrit, "O.F_LAWSUIT_TYPE");
	LawCritExtra();
	// 2016:058 TRQ: FIX: was missing
	lawCrit.AddChar("O.F_RECEIVER_TYPE", UCN_INSTANCE_HIGHER);
	QueryLawsuits("Обжалвани през отчетния период...", COUNT_APPEALED, "%d обжалвани дела");
}

void TRangeReport::Check()
{
	for (int i = 0; i < reportRanges.Count(); i++)
	{
		const TReportRange *reportRange = reportRanges[i];
		size_t positive = reportRange->Positive();

		if (positive)
		{
			mstr m("Непрехвърлени данни: обхват ");

			print_without_directives(m, any(reportRange->alias) ? reportRange->alias : reportRange->name);
			dprintf("%s, колона %u, стойност %ld\n", str(m), positive, reportRange->values[positive - 1]);
		}
	}
}

#define reprange OFFICE_GROUP_CAST(TReportRange)

static void reprange_name(mstr &m, TGroup *group)
{
	if (reprange->singleSubject)
	{
	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		m.printf("%s%s", HTML_COLUMN_INDENT, reprange->name);
	#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	#if COURT_TYPE == COURT_ADMIN
		m.cat(reprange->name);	// these can't be sorted
	#endif  // ADMIN
	}
	else
		print_with_directives(m, reprange->name);
}

static void reprange_alias(mstr &m, TGroup *group)
{
	if (reprange->singleSubject)
		m.cat(reprange->alias);
	else
		print_with_directives(m, reprange->alias);
}

static void reprange_value(mstr &m, TGroup *group)
{
	unsigned col;

	if (sscanf(reprange->Name, "F_C_%d", &col) == 1 && --col < RANGE_COLUMNS_MAX)
	{
		itom(reprange->values[col], m);
		reprange->values[col] = 0;	// mark as used
	}
}

#undef reprange

const offield TRangeReport::reprange_fields[] =
{
	{ "F_NAME",		reprange_name },
	{ "F_ALIAS",	reprange_alias },
	{ "*F_C_",		reprange_value },
	{ NULL,		NULL }
};

void TRangeReport::ShowUpper()
{
	tr1start = office.Find("tr1 start");
	tr1final = office.Find("tr1 final");

	office.Emit(NULL, tr1start, report_fields, this);

	for (int i = 0; i < reportRanges.Count(); i++)
	{
		TReportRange *reportRange = (TReportRange *) reportRanges[i];

		if (reportRange->Visible())
			office.Emit(tr1start, tr1final, reprange_fields, reportRange);
	}
}
